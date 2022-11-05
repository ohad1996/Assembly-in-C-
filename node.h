#ifndef __NODE__
#define __NODE__
#include <stdio.h>

/*this file keeps all the functons that needed for controlling nodes*/
typedef struct Node {
	void* data;
	struct Node* next;
}Node;


void insert(Node** head, void* data);//, size_t size);
Node* findNode(Node* head, void* data, int(*compare)(const void*, const void*));
void freeAllNodes(Node* head, void(*fFree)(void*));





#endif
