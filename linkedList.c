#include "linkedList.h"
#include <stdio.h>

//this method creates the list.
int initializeList(List* lst) {
	if (lst == NULL) {
		return 0;
	}
	lst->head = NULL;
	return 1;
}

//this method delete the list by freeing the memory of the nodes and the list himself!  
void freeList(List* lst, void(*freeList)(void*)) {
	freeAllNodes(lst->head, freeList);
}

