/*
** Heap/Priority Queue Module
** Attributed from Andrew Turpin
** 
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "heap.h"

/*
** returns a pointer to a new, empty heap
*/
Heap 
*createHeap() { 
    Heap *h = (Heap*)malloc(sizeof(Heap));
    h->H   = NULL;
    h->map = NULL;
    h->n   = 0;
    h->size= 0;
    
    return h;
}                               

/* 
** double size of heap.
** return 1 on success, 0 on fail
** assumes h != NULL
*/
int
enlargeHeap(Heap *h) {
    if (h->size == 0) {
        h->size = 1;
    }

    if ((h->H = (HeapItem*)realloc(h->H, sizeof(HeapItem) * h->size * 2)) == NULL) {
        return 0;
    }

    if ((h->map = (uint*)realloc(h->map, sizeof(uint) * h->size * 2)) == NULL) {
        return 0;
    }

    h->size *= 2;

    return 1;
}

/* assumes h != NULL
** swap vertex a and vertex b and change their corresponding map index
*/
inline void 
swap(Heap *h, uint a, uint b) {
   uint i = h->map[h->H[a].dataIndex];                       
   HeapItem t = h->H[a];    
   h->map[h->H[a].dataIndex] = h->map[h->H[b].dataIndex];       
   h->H[a] = h->H[b];       
   h->map[h->H[b].dataIndex] = i;                               
   h->H[b] = t;
}

/* 
** sift up from Wikipedia (sort of)
** assumes h != NULL
*/
void
siftUp(Heap *h, int n) {
    int child  = n;
    int parent = (int)floor((child - 1) / 2);
    while ((child > 0) && (h->H[parent].key > h->H[child].key)) {
        swap(h, parent, child);
        child = parent;
        parent = (int)floor((child - 1) / 2);
    }
}

/*
** sift down from Wikipedia (sort of)
** assumes h != NULL
*/
void
siftDown(Heap *h, int n) {
    uint root = n;
    
    while (root*2+1 < h->n) {
    	uint child = 2*root+1;
    	if ((child + 1 < h->n) && (h->H[child].key > h->H[child+1].key)) {
            child += 1;
        }
        if (h->H[root].key > h->H[child].key) {
            swap(h, child, root);
            root = child;
        } else {
            return;
    	}
    }
}

/* 
** inserts dataIndex into h
*/
int
insert(Heap *h, uint dataIndex, float key) {
    
    if (h == NULL) {
        return HEAP_FAIL;
    }
    if (h->n == h->size) {
        if (!enlargeHeap(h))
            return HEAP_FAIL;
    }
    h->H[h->n].key = key;
    h->H[h->n].dataIndex = dataIndex;
    h->map[dataIndex] = h->n;
    h->n += 1;
    // reheapify the heap with the new data
    siftUp(h, h->n - 1); 

    return HEAP_SUCCESS;
}

/* 
**returns the data index of the root.
** or -1 if no data in heap
*/
uint
peek(Heap *h) { 
    if (h == NULL) {
    	return - 1;
    }
    if (h->n > 0) {
        return h->H[0].dataIndex;
    } else {
        return -1;
    }
}

/* 
** returns the key of the root.
** or -1 if no data in heap
*/
float 
peekKey(Heap *h) { 
    if (h == NULL) return - 1;
    if (h->n > 0) {
        return h->H[0].key;
    } else {
        return -1;
    }
}

/*
** removes the root, returns the data index to it, and re-heapifies
*/
uint 
removeMin(Heap *h) { 
	uint min = peek(h);
	if (h->H == NULL) {
		return -1;
	} else {
		swap(h, h->n-1, 0);
		h->n--;
		siftDown(h, 0);
	}
	return min;
}
                         

/*
** adds delta to the key of dataIndex
*/
void 
changeKey(Heap *h, uint dataIndex, float delta) { 
    uint i = h->map[dataIndex];   // the index of dataIndex in h->H 
    float curr_key;
    curr_key = h->H[i].key;
    h->H[i].key = delta;
    if (delta < curr_key) {
        siftUp(h, i);
    } else {
        siftDown(h, i);
    }
} 

/* 
** free any memory you might of alloced in heap creation
*/
void 
destroyHeap(Heap *h) {
    if (h == NULL) {
    	return;
    }
    if (h->size > 0) {
        free(h->map);
        free(h->H);
    }
    free(h);
}


/* 
** prints out the entire heap
*/
void
printHeap(Heap *h) {
    if (h == NULL) return;

    int i;
    printf("---------------------------\n");
    printf("keys: ");
    for(i = 0 ; i < h->n ; i++) {
        printf("%5.0f ", h->H[i].key);
    }
    printf("\n");
    printf("di  : ");
    for(i = 0 ; i < h->n ; i++) {
        printf("%5d ", h->H[i].dataIndex);
    }
    printf("\n");
    printf("map : ");
    for(i = 0 ; i < h->n ; i++) {
        printf("%5d ", h->map[i]);
    }
    printf("\n");
}
