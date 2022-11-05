#define _CRT_SECURE_NO_WARNINGS

#include "label.h"
#include "exitsitu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void initLabel(Label* lb, const char* name,int spot, labelType type)
{
	if (lb == NULL) {
		printf("%s", NULL_PTR_EXEPTION);
		exit(NULL_PTR_EXIT);
	}


	lb->spot = spot;
	strcpy(lb->name, name);
	lb->type = type;

}

int compareLable(const void* a, const void* b) {
	Label* La = (Label*)a;
	char* Lb = (char*)b;
	return strcmp(La->name, Lb);
}

void printLable(const void* l) {
	Label* lV = (Label*)l;
	printf("%s\n%d\n", lV->name,lV->spot);
}
