/*
** Heap/Priority Queue Module - header file
** Attributed from Andrew Turpin
*/
typedef unsigned int uint;

typedef struct item {
    float key;         // the key for deciding position in heap (distance of edge)
    uint  dataIndex; // the payload index provided by the calling program
} HeapItem;

typedef struct heap {
    HeapItem *H;     // the underlying array
    uint   *map;   // map[i] is index into H of location of payload with dataIndex == i
    uint    n;     // the number of items currently in the heap
    uint    size;  // the maximum number of items allowed in the heap
} Heap;

#define HEAP_SUCCESS 1
#define HEAP_FAIL    0

Heap *createHeap(void);                               // returns a pointer to a new, empty heap
int insert(Heap *h, uint dataIndex, float key);     // inserts dataIndex into h
uint peek(Heap *h);                                 // returns the data index of the root.
float peekKey(Heap *h);                              // returns the key of the root.
uint removeMin(Heap *h);                            // removes the root, returns the data index to it, and re-heapifies 
void changeKey(Heap *h, uint dataIndex, float delta); // adds delta to the key of dataIndex
void destroyHeap(Heap *h);                            // free any memory you might of alloced in heap creation
void printHeap(Heap *h);			    
