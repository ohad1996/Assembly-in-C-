#ifndef __MACRO__
#define __MACRO__
#include <stdio.h>


typedef struct {
	char* name;
	char** commands;
	int commandsSize;
}Macro;

//void setMacroName(Macro*,const char*);
int initMacro(Macro* md, const char* name);//md == MacroData
void addLineToMacro(Macro* md, const char* line);
int compareMacroData(const void* a, const void* b);
void freeMacro(void* m);
#endif
