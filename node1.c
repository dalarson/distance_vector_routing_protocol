#include <stdio.h>
#include <string.h>
#include "project3.h"

#define NODE_NUM 1

extern int TraceLevel;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};
struct distance_table dt1;
struct NeighborCosts   *neighbor1;

extern float clocktime;


void printdt1(int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);

/* students to write the following two routines, and maybe some others */

void rtinit1() {
    printf("rinit1() called at time %f\n", clocktime);
    neighbor1 = getNeighborCosts(NODE_NUM);

    // Initialize distance table
    int i, j;
    for (i = 0; i < MAX_NODES; i++){
        for (j = 0; j < MAX_NODES; j++){
            dt1.costs[i][j] = INFINITY;
        }
    }
    for (i = 0; i < MAX_NODES; i++){
        dt1.costs[i][i] = neighbor1->NodeCosts[i];
    }

    printdt1(NODE_NUM, neighbor1, &dt1);

    for (i = 0; i < MAX_NODES; i++){
        if (i != NODE_NUM && neighbor1->NodeCosts[i] != INFINITY) { // make sure we aren't sending to self
            // create struct
            struct RoutePacket pkt = {NODE_NUM, i};
            memcpy(pkt.mincost, neighbor1 -> NodeCosts, sizeof(int) * MAX_NODES);
            // send each pkt
            printf("At time t=%f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, pkt.sourceid, pkt.destid, pkt.mincost[0], pkt.mincost[1], pkt.mincost[2], pkt.mincost[3]);
            toLayer2(pkt);
        }
    }
}


void rtupdate1( struct RoutePacket *rcvdpkt ) {
    print_rcvdpkt(rcvdpkt);

    // update distance table here
    int i, j;
    int has_changed = 0;
    for (i = 0; i < MAX_NODES; i++){
        if (rcvdpkt->mincost[i] + neighbor1->NodeCosts[rcvdpkt->sourceid] < dt1.costs[i][rcvdpkt->sourceid]){ // if given cost is less than current cost
            dt1.costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i] + neighbor1->NodeCosts[rcvdpkt->sourceid]; // set current cost to new cost
            if (dt1.costs[i][rcvdpkt->sourceid] == min_array(dt1.costs[i])){
                has_changed = 1;
            }
        }
    }

    if (has_changed == 1){
        struct RoutePacket pkt;
        pkt.sourceid = NODE_NUM;
        for (i = 0; i < MAX_NODES; i++){
            pkt.mincost[i] = min_array(dt1.costs[i]);
        }

        for (i = 0; i < MAX_NODES; i++){
            if (i != NODE_NUM && neighbor1->NodeCosts[i] != INFINITY){
                pkt.destid = i;
                printf("Distance table updated. At time t=%f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, pkt.sourceid, pkt.destid, pkt.mincost[0], pkt.mincost[1], pkt.mincost[2], pkt.mincost[3]);
                toLayer2(pkt);
            }
        }
    }

    printdt1(NODE_NUM, neighbor1, &dt1);
    

}



/////////////////////////////////////////////////////////////////////
//  printdt
//  This routine is being supplied to you.  It is the same code in
//  each node and is tailored based on the input arguments.
//  Required arguments:
//  MyNodeNumber:  This routine assumes that you know your node
//                 number and supply it when making this call.
//  struct NeighborCosts *neighbor:  A pointer to the structure 
//                 that's supplied via a call to getNeighborCosts().
//                 It tells this print routine the configuration
//                 of nodes surrounding the node we're working on.
//  struct distance_table *dtptr: This is the running record of the
//                 current costs as seen by this node.  It is 
//                 constantly updated as the node gets new
//                 messages from other nodes.
/////////////////////////////////////////////////////////////////////
void printdt1( int MyNodeNumber, struct NeighborCosts *neighbor, 
		struct distance_table *dtptr ) {
    int       i, j;
    int       TotalNodes = neighbor->NodesInNetwork;     // Total nodes in network
    int       NumberOfNeighbors = 0;                     // How many neighbors
    int       Neighbors[MAX_NODES];                      // Who are the neighbors

    // Determine our neighbors 
    for ( i = 0; i < TotalNodes; i++ )  {
        if (( neighbor->NodeCosts[i] != INFINITY ) && i != MyNodeNumber )  {
            Neighbors[NumberOfNeighbors] = i;
            NumberOfNeighbors++;
        }
    }
    // Print the header
    printf("                via     \n");
    printf("   D%d |", MyNodeNumber );
    for ( i = 0; i < NumberOfNeighbors; i++ )
        printf("     %d", Neighbors[i]);
    printf("\n");
    printf("  ----|-------------------------------\n");

    // For each node, print the cost by travelling thru each of our neighbors
    for ( i = 0; i < TotalNodes; i++ )   {
        if ( i != MyNodeNumber )  {
            printf("dest %d|", i );
            for ( j = 0; j < NumberOfNeighbors; j++ )  {
                    printf( "  %4d", dtptr->costs[i][Neighbors[j]] );
            }
            printf("\n");
        }
    }
    printf("\n");
}    // End of printdt1

