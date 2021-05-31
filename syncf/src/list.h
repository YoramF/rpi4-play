/*
	list.c

	Handles link lists functionalities.
	The list is sorted
*/

#ifndef LIST_H_
#define LIST_H_

typedef struct _l_node {
	struct _l_node *next;
	void *ep;
} l_node;

l_node *insert(l_node *list, void *entry, int (*cf)(void *e1, void *e2), void (*rf)(void *en));
void *find(l_node *list, void *entry, int (*cf)(void *e1, void *e2));
void *g_next(l_node **c_node);
void rem_list(l_node *list, void (*rf)(void *en));

#endif /* LIST_H_ */