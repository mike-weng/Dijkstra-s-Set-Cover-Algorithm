/*
** Set Module & Dijskra's SSSP
** Attributed from Alistair Moffat
**
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "graph.h"
#include "heap.h"
#include "set.h"

/*
** Create an empty set
*/
set_t
*make_empty_set(void) {
    set_t *set;
    set = (set_t*)malloc(sizeof(*set));
    assert(set!=NULL);
    set->head = set->foot = NULL;
    set->n = 0;
    return set;
}

/*
** Checks if the set is empty
*/
int
is_empty_set(set_t *set) {
    assert(set!=NULL);
    return set->head==NULL;
}

/*
** Free the memory allocated to the set
*/
void
free_set(set_t *set) {
    node_t *curr, *prev;
    assert(set!=NULL);
    curr = set->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(set);
}

/*
** Insert a new node from the head of the set
*/

set_t
*insert_at_head(set_t *set, int value) {
    node_t *new_node;
    new_node = (node_t*)malloc(sizeof(*new_node));
    assert(set!=NULL && new_node!=NULL);
    new_node->data = value;
    new_node->next = set->head;
    set->head = new_node;
    if (set->foot==NULL) {
        /* this is the first insertion into the set */
        set->foot = new_node;
    }
    set->n++;
    return set;
}

/*
** Insert a new node from the foot of the set
*/
set_t
*insert_at_foot(set_t *set, int value) {
    node_t *new_node;
    new_node = (node_t*)malloc(sizeof(*new_node));
    assert(set!=NULL && new_node!=NULL);
    new_node->data = value;
    new_node->next = NULL;
    if (set->foot==NULL) {
        /* this is the first insertion into the set */
        set->head = set->foot = new_node;
    } else {
        set->foot->next = new_node;
        set->foot = new_node;
    }
    set->n++;
    return set;
}

/*
** Returns the head data of the set
*/
int
get_head(set_t *set) {
    assert(set!=NULL && set->head!=NULL);
    return set->head->data;
}

/*
** Returns a new set with its original head deleted
*/
set_t
*get_tail(set_t *set) {
    node_t *oldhead;
    assert(set!=NULL && set->head!=NULL);
    oldhead = set->head;
    set->head = set->head->next;
    if (set->head==NULL) {
        /* the only set node just got deleted */
        set->foot = NULL;
    }
    set->n--;
    free(oldhead);
    return set;
}

/*
** Deletes a node from the set
** If element not present, do nothing.
** Return altered set
*/
set_t
*delete_element(set_t *set, int element) {
    assert(set);

    node_t *curr = set->head, *prev = set->head;
    while (curr != NULL && curr->data != element) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL)
        return set;

    if (curr == set->head) {
        if (set->foot == set->head)
            set->foot = curr->next;
        set->head = curr->next;
    } else {
        prev->next = curr->next;
        if (set->foot == curr)
            set->foot = curr->next;
    }
    set->n--;
    free(curr);

    return(set);
}

/*
** Checks if the data is in the set
*/
int
is_in_set(set_t *s, int data) {
	node_t *temp = s->head;
	while (temp != NULL) {
		if (temp->data == data) {
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}


/*
** Create a new set with the intersection of data in s1 and s2
*/
set_t
*setIntersect(set_t *s1, set_t *s2) {
	set_t *new_s = make_empty_set();
	node_t *temp = s1->head;
	while (temp != NULL) {
		if (is_in_set(s2, temp->data)) {
			insert_at_foot(new_s, temp->data);
		}
		temp = temp->next;
	}
	return new_s;
}

/*
** Delete the nodes in s1 that are both in s1 and s2
*/
set_t
*setComplement(set_t *s1, set_t *s2) {
	node_t *temp = s2->head;
	while (temp != NULL) {
		if (is_in_set(s1, temp->data)) {
			delete_element(s1, temp->data);
		}
		temp = temp->next;
	}
	return s1;
}

/*
** Set cover algorithm
** return an array with the vertices that covers the largest set
*/
int
*set_cover(set_t **all_set, int nset, set_t *U, int *num) {
	/* all_set - array of all the sets
	** nset - number of sets in all_set
	** U - a set of all houses
	** num - the number of school vertices in the final array
	*/
	int *A = (int *)malloc(sizeof(*A));
	int A_n=0, A_size = 1, count = 0;
	assert(U);
	while (!is_empty_set(U)) {
		// if there are houses that cannot be covered by any school
		// when the loop has run as many times as the number of 
		// school vertices and U is not empty means there are duplicates
		if (count == nset){
			*num = A_n;
			free(U);
			return A;
		}

		//select S with max S intersect U
		int max = 0;
		int index = 0;
		int i;
		for (i=0;i<nset;i++){
			set_t *new_s = setIntersect(all_set[i], U);
			if (new_s->n > max){
				max = new_s->n;
				index = i;
			}
			free_set(new_s);
		}
		// remove elements of the largest uncovered set from U
		U = setComplement(U, all_set[index]);
		
		// Adding index to array of the school vertex
		if (A_n >= A_size){
			A_size *= 2;
			A = (int *)realloc(A, sizeof(*A) * A_size);
		}
		
		// Checks if there are duplicates
		int is_in_A = 0;
		for (i=0;i<A_n;i++) {
			if (all_set[index]->head->data == A[i]) {
				is_in_A = 1;
			}
		}
		// append to the final array only if it's not in A
		if (is_in_A == 0) {
			A[A_n] = all_set[index]->head->data;
			A_n++;
		}
		count++;
	}
	
	*num = A_n; //assign the size of A to num_sets
	free_set(U);
	return A;
}

