/*
** Graph Module
** Uses an adjacency list representation (arrays for lists).
** 
**
** Attributed from Andrew Turpin
** 
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#include "set.h"
#include "heap.h"


/*
** Create graph with number_of_vertices vertices
*/
Graph *
graph_new(int number_of_vertices) {
    assert(number_of_vertices > 0);
    Graph *g = (Graph *)malloc(sizeof(Graph));
    assert(g);
    g->S = 0;
    g->H = 0;
    g->number_of_vertices = number_of_vertices;
    g->vertices = (Vertex *)malloc(sizeof(Vertex) * number_of_vertices);
    assert(g->vertices);
    
    /*initialise all the data*/
    for(int i = 0 ; i < number_of_vertices ; i++) {
        g->vertices[i].visited  = 0;
        g->vertices[i].label = i;
        g->vertices[i].edges = NULL;
        g->vertices[i].num_edges = 0;
        g->vertices[i].max_num_edges = 0;
    }
        
    return(g);
}

/*
** Add edge to g. 
*/
void  
graph_add_edge(Graph *g, Label v, Label u, Distance dist) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);
    assert(u >= 0 && u < g->number_of_vertices);

        // Make room if no room.
    if (g->vertices[v].num_edges == g->vertices[v].max_num_edges) {
        if (g->vertices[v].max_num_edges == 0)
            g->vertices[v].max_num_edges = 1;
        else
            g->vertices[v].max_num_edges *= 2;
        g->vertices[v].edges = (Edge *)realloc(g->vertices[v].edges, sizeof(Edge) * g->vertices[v].max_num_edges);
        assert(g->vertices[v].edges);
    }

    g->vertices[v].edges[g->vertices[v].num_edges].u    = u;
    g->vertices[v].edges[g->vertices[v].num_edges].dist = dist;

    g->vertices[v].num_edges += 1;
}

/*
** Delete edge from g. Only deletes one copy if there are multiple.
*/
void  
graph_del_edge(Graph *g, Label v, Label u) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);
    assert(u >= 0 && u < g->number_of_vertices);

    int i;

    for(i = 0 ; i < g->vertices[v].num_edges
    	    && g->vertices[v].edges[i].u != u ; i++) {
    	if (i == g->vertices[v].num_edges) // not there
    		return;
    }

    for( ; i < g->vertices[v].num_edges - 1 ; i++) {
    	// move remainder to the left to fill hole at i
        g->vertices[v].edges[i] = g->vertices[v].edges[i+1];
        g->vertices[v].num_edges -= 1;
    }
}
/*
** Return pointer to start of edge array for vertex v
** Set n to the number of items inthe edge array.
*/
Edge *
graph_get_edge_array(Graph *g, Label v, int *num_edges) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);

    *num_edges = g->vertices[v].num_edges;

    return g->vertices[v].edges;
}

/*
** return 1 if (v,u) is in g
*/
int
graph_has_edge(Graph *g, Label v, Label u) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);
    assert(u >= 0 && u < g->number_of_vertices);
    
    for(int i = 0 ; i < g->vertices[v].num_edges ; i++)
        if (g->vertices[v].edges[i].u == u)
            return 1;

    return 0;
}


/*
** Set vertex's data to data
*/
void  
graph_set_vertex_data(Graph *g, Label v, Status visited) {
    assert(g);
    assert(v >= 0 && v < g->number_of_vertices);
    
    g->vertices[v].visited = visited;
}


/*
** Simple explore for calling from dfs in check_graph function
** calculates the number of vertices in the graph and checks that
** the graph is undirected
*/
void 
graph_explore(Graph *g, Label v, int *nvertex) {
    if (g->vertices[v].visited) {
        return;
    }

    g->vertices[v].visited = 1; // mark as visited
    (*nvertex)++; // count the number of times a vertex has be marked visited
    for(int i = 0 ; i < g->vertices[v].num_edges ; i++) {
        graph_explore(g, g->vertices[v].edges[i].u, nvertex);
    }
}

/*
** Input the the verteces and distances into the graph structure
*/
Graph
*input_graph(void) {
	int num_vertex, H, S, dist;
	Graph *g;
	Label v, u;
	// first two row is the number of house and school vertices
	scanf("%d", &H);
	scanf("%d", &S);
	num_vertex = H + S;
	g = graph_new(num_vertex);
	g->H = H;
	g->S = S;
	while (scanf("%d %d %d", &v, &u, &dist)==3) {
		graph_add_edge(g, v, u, dist);
		graph_add_edge(g, u, v, dist);
	}
	return g;
}

/*
** Checks if the graph is fully connected
** Perform DFS beginning at v.
** ASSUMES vertex->data is 1 or 0 for visited and unvisted
*/
int
check_graph(Graph *g, Label v) {
	assert(g);
	assert(v >= 0 && v < g->number_of_vertices);
	int nvertex=0;
        // first explore from v
        // if the graph is fully connected only one explore should be called
        graph_explore(g, v, &nvertex);
        // nvertex from explore should be the same as input if fully connected
        if (nvertex==g->number_of_vertices) {
        	return 1;
        } else {
        	return 0;
        }
}

void 
graph_print(Graph *g) {
	int i, j;
	for (i = 0; i < g->number_of_vertices; i++) {
		printf("Label is: %d\n", g->vertices[i].label);
		printf("Number of Edges: %d\n", g->vertices[i].num_edges);
		for (j = 0; j < g->vertices[i].num_edges; j++) {
			printf("Edge label connected: %d, Distance: %f meters\n", 
			        g->vertices[i].edges[j].u, g->vertices[i].edges[j].dist);
		}
	}
}

/*
** Free the graph
*/
void
free_graph(Graph *g) {
	assert(g);
	free(g->vertices);
	free(g);
}

/*
** Using dijkstra algorithm to obtain vertices shortest distance
** to the source and put into heap priority queue
** then is pushed to a set structure
*/
set_t
*dijkstra(Graph *g, Label src) {
	int i;
	float dist[g->number_of_vertices];
	uint u, v;
	
	// initialise all distances to infinity
	Heap *h = createHeap();
	set_t *s = make_empty_set();
	for (i=0;i<g->number_of_vertices;i++) {
		dist[i] = infinity;
		
		if (!insert(h, g->vertices[i].label, infinity)) {
			fprintf(stderr, "Heap not inserted successfully\n");
		}
	}
	// set the source distance to 0
	changeKey(h, src, 0);
	dist[src] = 0;
	
	while (h->n != 0) {
		// pop the min edge from the queue to explore
		u = removeMin(h);
		// early termination if the distance is bigger than 1km
		if (h->H[h->map[u]].key > 1000) {
			return s;
		}
		for (i=0;i<g->vertices[u].num_edges;i++) {
			// the next vertex connected to u
			v = g->vertices[u].edges[i].u;
			if (dist[v] > dist[u] + g->vertices[u].edges[i].dist) {
				dist[v] = dist[u] + g->vertices[u].edges[i].dist;
			}
			// update priority queue
			changeKey(h, v, dist[v]);
		}
		// insert into the set
		s = insert_at_foot(s, u);
	}
	destroyHeap(h);
	return s;
}
