/**
*Linked List operations
*Structure of the code below was originally written by Alistair Moffat.
*Adapted by Quang Tri Le
*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"

list_t *make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}

void free_list(list_t *list) {
	node_list_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev->popped);
		free(prev);
	}
	free(list);
}


list_t *insert_at_foot(list_t *list, void* popped) {
	node_list_t *new;
	new = (node_list_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->popped = popped;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}