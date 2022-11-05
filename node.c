#define _CRT_SECURE_NO_WARNINGS
#include "node.h"
#include "exitsitu.h"
#include <stdlib.h>
#include <string.h>

//this method initialize a new node 
void insert(Node** head, void* data ) {
	Node* newNode = NULL;
	newNode = (Node*)malloc(sizeof(Node));
	if (newNode==NULL) {
		printf("%s", ALLOCATION_ERROR_EXEPTION);
		exit(ALLOCATION_ERROR_EXIT);
	}

	newNode->next = *head;
	newNode->data = data;
	*head = newNode;
}

//this method ment for to find a node by the given data of his details.
Node* findNode(Node* head, void* data, int(*compare)(const void*, const void*)) {
	Node *temp = head;
	while (temp != NULL) {
		if (compare(temp->data,data) != 0)//useing a comparing method to check if this is the data that we looked for
			temp = temp->next;
		else
			return temp;
	}
	return temp;
}

//this method is freeing all the nodes 
void freeAllNodes(Node* head, void(*fFree)(void*)){
	Node* curNode = head;
	Node* nextNode = NULL;
	while (curNode)
	{
		nextNode = curNode->next;
		if (fFree != NULL)
			fFree(curNode->data);//useing the given function to freeing the current data.
		free(curNode);
		curNode = nextNode;
	}
}
