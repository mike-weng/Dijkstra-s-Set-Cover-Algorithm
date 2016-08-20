/*
** Graph Module - header file
** Uses an adjacency list representation (arrays for lists).
**
** Attributed from Andrew Turpin
*/
#define infinity 2147483647
typedef int Label;   // a vertex label (should be numeric to index edge lists)
typedef float Distance; // Distance
typedef int Status; // status of the vertices visited = 1 unvisited = 0

typedef struct {
    Distance  dist;   // any data you want to store about an edge
    Label u;        // end vertex of edge
} Edge;

typedef struct {
    Status visited;        // any data you want to store about a vertex
    Label  label;        // this vertex's label
    Edge  *edges;        // array of edges [0..num_edges-1]
    int   num_edges;     // number of edges
    int   max_num_edges; // current malloced size of edges array, num_edges <= max_num_edges
} Vertex;

typedef struct {
    int    S;              // any data you want to store about a graph
    int    H;
    int    number_of_vertices; // |V|
    Vertex *vertices;          // array of vertices [0..number_of_vertices-1]
} Graph;

// prototypes

Graph *graph_new(int number_of_vertices);
void  graph_add_edge(Graph *g, Label v, Label u, Distance dist);
void  graph_del_edge(Graph *g, Label v, Label u);   
int   graph_has_edge(Graph *g, Label v, Label u);
Edge *graph_get_edge_array(Graph *g, Label v, int *num_edges);
void  graph_set_vertex_data(Graph *g, Label v, Status visited);
void graph_explore(Graph *g, Label v, int *nvertex);
void graph_dfs(Graph *g, Label v);
Graph *input_graph(void);
int  check_graph(Graph *g, Label v);
void graph_print(Graph *g);
void free_graph(Graph *g);
