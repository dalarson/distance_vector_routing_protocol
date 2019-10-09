#include <stdio.h>
#include <string.h>
#include "project3.h"

#define NODE_NUM 0

extern int TraceLevel;

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
} dt0;
struct NeighborCosts *neighbor0;
extern float clocktime;

void printdt0(int MyNodeNumber, struct NeighborCosts *neighbor, struct distance_table *dtptr);

/* students to write the following two routines, and maybe some others */

void rtinit0() {
    printf("rinit0() called at time %f\n", clocktime);
    neighbor0 = getNeighborCosts(NODE_NUM);

    // Initialize distance table
    int i, j;
    for (i = 0; i < MAX_NODES; i++){
        for (j = 0; j < MAX_NODES; j++){
            dt0.costs[i][j] = INFINITY; // populate all cells with infinity
        }
    }
    for (i = 0; i < MAX_NODES; i++){
        dt0.costs[i][i] = neighbor0->NodeCosts[i]; // fill in cells of immediate neighbors
    }

    printdt0(NODE_NUM, neighbor0, &dt0);

    for (i = 0; i < MAX_NODES; i++){
        if (i != NODE_NUM && neighbor0->NodeCosts[i] != INFINITY) { // make sure we aren't sending to self

            // create struct
            struct RoutePacket pkt = {NODE_NUM, i};
            memcpy(pkt.mincost, neighbor0 -> NodeCosts, sizeof(int) * MAX_NODES);
            // send each pkt
            printf("At time t=%f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, pkt.sourceid, pkt.destid, pkt.mincost[0], pkt.mincost[1], pkt.mincost[2], pkt.mincost[3]);
            toLayer2(pkt);
        }
    }
}


void rtupdate0( struct RoutePacket *rcvdpkt ) {
    print_rcvdpkt(rcvdpkt);

    // update distance table here
    int i, j;
    int has_changed = 0;
    for (i = 0; i < MAX_NODES; i++){
        if (rcvdpkt->mincost[i] + neighbor0->NodeCosts[rcvdpkt->sourceid] < dt0.costs[i][rcvdpkt->sourceid]){ // if given cost is less than current cost
            dt0.costs[i][rcvdpkt->sourceid] = rcvdpkt->mincost[i] + neighbor0->NodeCosts[rcvdpkt->sourceid]; // set current cost to new cost
            if (dt0.costs[i][rcvdpkt->sourceid] == min_array(dt0.costs[i])){
                has_changed = 1;
            }
        }
    }

    if (has_changed == 1){
        struct RoutePacket pkt;
        pkt.sourceid = NODE_NUM;
        for (i = 0; i < MAX_NODES; i++){
            pkt.mincost[i] = min_array(dt0.costs[i]);
        }

        for (i = 0; i < MAX_NODES; i++){
            if (i != NODE_NUM && neighbor0->NodeCosts[i] != INFINITY){
                pkt.destid = i;
                printf("Distance table updated. At time t=%f, node %d sends packet to node %d with: %d %d %d %d\n", clocktime, pkt.sourceid, pkt.destid, pkt.mincost[0], pkt.mincost[1], pkt.mincost[2], pkt.mincost[3]);
                toLayer2(pkt);
            }
        }
    }

    printdt0(NODE_NUM, neighbor0, &dt0);
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
void printdt0( int MyNodeNumber, struct NeighborCosts *neighbor, 
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
}    // End of printdt0

