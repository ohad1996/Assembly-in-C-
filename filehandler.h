#ifndef __FILE_HANDLER__
#define __FILE_HANDLER__
#define ARG_FILE 1
#define NUM_OF_ARGS 2//1.the name of project, 2.file to work with;
#define MAX_LINE_LEN 80//HERE AND IN LABEL.H THERE IS THIS VARIABLE

#include <stdio.h>

/*this file keeps all the needed functions that we need for to use FILE */
typedef struct {
	FILE* file;
	char fileLine[MAX_LINE_LEN];//the last line in our given file
}FileHandler;

void initFileText(FileHandler* fh, const char* fileName, const char* format);
int readFileLine(FileHandler*);

#endif
