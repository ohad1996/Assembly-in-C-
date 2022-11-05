#define _CRT_SECURE_NO_WARNINGS
#include "macro.h"
#include "node.h"
#include "exitsitu.h"
#include <stdlib.h>
#include <string.h>


//this method initialize the given macro
int initMacro(Macro* md,const char* name) {
	if (md == NULL)
		return 0;
	md->commandsSize = 0;
	md->name = (char*)malloc(sizeof(char) * (strlen(name)+1));
	if (md->name == NULL) {
		printf("%s", ALLOCATION_ERROR_EXEPTION);
		exit(ALLOCATION_ERROR_EXIT);

	}
	strcpy(md->name,name);
	md->commands = NULL;
	return 1;
}

//this method adds a line to our given macro
void addLineToMacro(Macro* md, const char* line) {
	char** lines = NULL;
	int size = strlen(line) + 1;
	lines = (char**)realloc(md->commands, sizeof(char*) * ((md->commandsSize) + 1));

	if (!lines) {
		printf("%s", ALLOCATION_ERROR_EXEPTION);
		exit(ALLOCATION_ERROR_EXIT);
	}
	lines[md->commandsSize] = (char*)malloc(sizeof(char*) * size);
	if (!lines[md->commandsSize]) {
		printf("%s", ALLOCATION_ERROR_EXEPTION);
		exit(ALLOCATION_ERROR_EXIT);
	}
	strcpy(lines[md->commandsSize], line);
	md->commands = lines;
	md->commandsSize++;
}

//this method compares between 2 macros by their names
int compareMacroData(const void* a, const void* b){
	Macro* Ma = (Macro*)a;
	char* Mb = (char*)b;
	return strcmp(Ma->name,Mb);
}

//this method freeing macro's memory incase there is a need.
void freeMacro(void* m) {
	int i;
	Macro* macro = (Macro*)m;
	free(macro->name);
	for (i = 0; i < macro->commandsSize; i++)
		free(macro->commands[i]);
	free(macro->commands);
	free(macro);
}
