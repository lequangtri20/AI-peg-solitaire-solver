#ifndef __LIST__
#define __LIST__

#include "utils.h"

/**
 * Linked list for storing nodes popped from stack. This is used for
 * memory management.
 * Structure of the code below was originally written by Alistair Moffat.
 * Adapted by Quang Tri Le
 */

typedef struct node node_list_t;

struct node {
	node_t *popped;
	node_list_t *next;
};

typedef struct {
	node_list_t *head;
	node_list_t *foot;
} list_t;


list_t *make_empty_list(void);
void free_list(list_t *list);
list_t *insert_at_foot(list_t *list, void *popped);





#endif