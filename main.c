/*
** COMP20007
** Design of Algorithms
** Assignment 2
** Mike Weng 695023
**
*/
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "heap.h"
#include "set.h"
	
int 
main(int argc, char *argv[]) {
    Graph *g;
    
    //input the data from the file to the graph structure
    g = input_graph();
    
    // check if the input is valid and graph is fully connected
    
    if (!check_graph(g, 0)) {
        fprintf(stderr, "ERROR! The input is invalid\n");
        exit(EXIT_FAILURE);
    }

    int i, nset=0;
    set_t **all_set = (set_t **)malloc(sizeof(set_t*) * g->S);
    
    // using dijkstra's SSSP to create sets for each school vertices
    for (i=g->H;i<g->number_of_vertices;i++) {
    	    all_set[nset] = dijkstra(g, i);
    	    nset++;
    }
    // create a set with all house vertices
    set_t *U = make_empty_set();
    for (i=0;i<g->H;i++) {
    	    insert_at_foot(U, i);
    }

    // using set cover algorithm to calculate the school vertices that
    // cover the largest number of houses
    
    int num = 0;
    int *A = set_cover(all_set, nset, U, &num);
    
    // print out the result
    for (i=0;i<num;i++) {
    	    fprintf(stdout, "%d\n", A[i]-g->H);
    }
    
    // free the graph
    free_graph(g);
    return EXIT_SUCCESS;
    
}
