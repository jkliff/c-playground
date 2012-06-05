/* 
 * linkedlist.c
 *
 *  Created on: Oct 14, 2011
 *      Author: john
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define DUMP_NODE(N) \
	printf ("Node at %p\n\tprev = %p\n\tnext = %p\n\tdata = %p\n", N, N->prev, N->next, N->data);
#else
#define DUMP_NODE(N) ;
#endif

typedef struct node {
    void *data;
    struct node *prev;
    struct node *next;
} node_t;

typedef struct list {
    node_t *first;
    node_t *last;
    node_t *curr;
} list_t;

void list_add(list_t *, const void *restrict);
list_t *list_init();

list_t *list_init() {
    list_t *l;
    l = malloc(sizeof(list_t));

    l->first = NULL;
    l->last = NULL;
    l->curr = NULL;

    return l;
}

void list_add(list_t * l, const void *restrict node_data) {
    node_t *n;
    n = malloc(sizeof(node_t));

    n->data = node_data;
    n->prev = NULL;
    n->next = NULL;

    // list is empty
    if (l->last == NULL)
    {

        l->first = n;
        l->last = n;
        l->curr = n;
    }
    else
    {
        n->prev = l->last;
        l->last->next = n;
        l->last = n;
    }
    DUMP_NODE(n);
}

int main(int argc, char *argv[]) {

    int s = 5;
    // default
    if (argc > 1)
    {
        s = atoi(argv[1]);
    }
    int i;
    list_t *list = list_init();

    for (i = 0; i < s; i++)
    {
        char *data = malloc(1024);
        sprintf(data, "I'm a text inside a list: %d", i);
        list_add(list, data);
    }
    printf("inited the list\nwill now walk:\n");
    node_t *curr;
    for (curr = list->first; curr != NULL; curr = curr->next)
    {
        printf("data inside the node: \n\t%s\n", (char *)curr->data);
    }

    return 0;
}
