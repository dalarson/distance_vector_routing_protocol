

all: node0.c node1.c node2.c node3.c project3.c project3.h 
	gcc -o p3 node0.c node1.c node2.c node3.c project3.c

clean:
	rm p3
