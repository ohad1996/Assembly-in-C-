#define _CRT_SECURE_NO_WARNINGS
#include "filehandler.h"
#include "exitsitu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*this method initialize the file field*/
void initFileText(FileHandler* fh, const char* fileName, const char* format)
{
	fh->file = fopen(fileName, format);
	if (fh->file == NULL) {//checks if the file isn't empty
		printf("%s", NULL_PTR_EXEPTION);
		exit(NULL_PTR_EXIT);
	}
}

/*this method ment for save the field of the last file line that
 we reed from the method*/
int readFileLine(FileHandler* fh)
{
	if (fh->file == NULL) {//checks if the given file exsist!
		printf("%s", NULL_PTR_EXEPTION);
		exit(NULL_PTR_EXIT);
	}
	//checks if the line is empty
	if (fgets(fh->fileLine, MAX_LINE_LEN, fh->file) == 0) {
		return 0;
	}
	//incase we found a not empty line' we will save it in our last line field
	fh->fileLine[strcspn(fh->fileLine, "\n")] =  '\0';
	return 1;
}
