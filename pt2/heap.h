#include <stdio.h>
#include <stdlib.h>

//adapted from https://www.geeksforgeeks.org/dijkstras-algorithm-for-adjacency-list-representation-greedy-algo-8/

// Structure to represent a min heap node
typedef struct MinHeapNode
{
    int  v;
    int dist;
}MinHeapNode;

// Structure to represent a min heap
typedef struct MinHeap
{

    // Number of heap nodes present currently
    int size;

    // Capacity of min heap
    int capacity;

    // This is needed for decreaseKey()
    int *pos;
    MinHeapNode **array;
}MinHeap;

// A utility function to create a
// new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist);

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity);

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b);

// A standard function to
// heapify at given idx
// This function also updates
// position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx);

// A utility function to check if
// the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap);

// Standard function to extract
// minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap);

// Function to decreasy dist value
// of a given vertex v. This function
// uses pos[] of min heap to get the
// current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist);

// A utility function to check if a given vertex
// 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v);


void freeMinHeap(MinHeap* heap);
