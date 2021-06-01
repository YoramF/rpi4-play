/*
	list.c

	Handles link lists functionalities.
	The list is sorted
*/

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/**
 * Insert new element to list.
 * list - start of list. If NULL than list was not created yet
 * entry - address of list element
 * cf - compare element callback function
 *  cf should return 1, 0, -1 according to this comparison
 * rf - delete element callback function 
 * returns - pointer to head of list or NULL if error
 */
l_node *insert(l_node *list, void *entry, int (*cf)(void *e1, void *e2), void (*rf)(void *en)) {
	l_node *p, *cp, *bp;
	int c;

	if ((p = malloc(sizeof(l_node))) == NULL) {
		perror("[insert] failed: ");

		// if list is not NULL release list before returning to caller
		if (list != NULL)
			rem_list(list, rf);
		return NULL;
	}

	p->ep = entry;

	// find where to enter the new entry. bp will back track cp
	cp = list;
	bp = NULL;
	while ((cp != NULL) && ((c = cf((void *)(cp->ep), entry)) < 0)) {
		bp = cp;
		cp = cp->next;
	}

	// insert new entry to list
	p->next = cp;

	// check if new element should become head of the list
	if (bp == NULL) {
		list = p;
	}
	else {
		bp->next = p;
	}

	return list;
}

/**
 * list - start of list or NULL if list is empty
 * entry - address of an item to look for
 * sf - address of a comparison function that check if item in the list is what we
 * are looking for
 * returns a pointer to the entry we found. If nothing was found return NULL
 */
void *find(l_node *list, void *entry, int (*cf)(void *e1, void *e2)) {
	int c;

	if (list != NULL) {
		while ((list != NULL) && ((c = cf((void *)(list->ep), entry)) < 0))
			list = list->next;

		if (c == 0)
			return list->ep;
	}

	return NULL;
}

/**
 * c_node - current node in the list. In first call *c_node should 
 * be the head of the list.
 * returns - next element in list or NULL if reaced end of list
 */
void *g_next(l_node **c_node) {
	l_node *p = *c_node;

	if (p != NULL) {
		*c_node = p->next;
		return p->ep;
	}

	return NULL;
}

/**
 * list - head of list.
 * rme - delete element callback function
 * delete list
 */
void rem_list(l_node *list, void (*rf)(void *en)) {
	l_node *p = list;

	while (p != NULL) {
		list = p->next;
		rf(p->ep);
		free(p);
		p = list;
	}
}