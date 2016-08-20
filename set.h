/*
** Set Module & Dijkstra's SSSP - header file
** Attributed from Alistair Moffat
*/
typedef struct node node_t;

struct node {
    unsigned int data;		// data of the node (label of the vertices)
    node_t *next;		// the next node connected
};

typedef struct {
    int n;			// number of items in the set
    node_t *head;		// head node of the set
    node_t *foot;		// foot node of the set
} set_t;

set_t *make_empty_set(void);	
int is_empty_set(set_t *set);
void free_set(set_t *set);
set_t *insert_at_head(set_t *set, int value);
set_t *insert_at_foot(set_t *set, int value);
int get_head(set_t *set);
set_t *get_tail(set_t *set);
set_t *delete_element(set_t *set, int value); // added Turpin March 2015
set_t *dijkstra(Graph *g, Label src);
int is_in_set(set_t *s, int data);
set_t *setIntersect(set_t *s1, set_t *s2);
set_t *setComplement(set_t *s1, set_t *s2);
int *set_cover(set_t **all_set, int n, set_t *U, int *num_sets);
