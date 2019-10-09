Instructions to run:
	make
	./p3

Instructions to clean:
	make clean

Note:
My code does not prevent cycles in the distance tables. However, often these cycles are indeed the shortest path. This being the case, my algorithm does
indeed find the shortest path for each node to all other nodes. I attempted to implement some logic to prevent cycles, and it seemed to work correctly for
nodes 0 and 1, but tables of nodes 2 and 3 were never fully populated. I gave up trying to troubleshoot it.
 
