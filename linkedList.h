#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include "node.h"

/*this file holds all the functions that ment for holding all the types of lists that needed! */
typedef struct {
	Node* head;
}List;

int initializeList(List* lst);
void freeList(List* lst, void(*freeList)(void*));

#endif
