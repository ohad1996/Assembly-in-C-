#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "secondtransition.h"
#include "firsttransition.h"
#include "general.h"
#include "macrohandler.h"


int main(int argc, char* argv[])
{

	if(argc != 2)
	{
		printf("Didn't give a file name!\n");
		exit(1);
	}

	MacIO io;
	FT ft;
	ST st;
	char afterMacro[30];
	char assembli[30];
	char entry[30];
	char externFile[30];
	char* sub = NULL;

	strcpy(afterMacro,argv[1]);
	strcpy(assembli,argv[1]);
	strcpy(entry,argv[1]);
	strcpy(externFile,argv[1]);
	
	//initiate the files names for the macro & first & second algorithems!
	sub = strstr(afterMacro, ".");
	strcpy(sub,AFTER_MACRO_FILE_NAME);
	sub = strstr(assembli, ".");
	strcpy(sub,ASSEMBLI_FILE_NAME);
	sub = strstr(entry, ".");
	strcpy(sub,ENTRY_FILE_NAME);
	sub = strstr(externFile, ".");
	strcpy(sub,EXTERN_FILE_NAME);

	//begins the macro algorithem on the given file!
	initMacIO(&io, argv[1], afterMacro);
	initMacAlgo(&io);
	
	//begins the first transition of the algorithem!
	initFT1stAlgo(&ft, afterMacro);
	initFT2ndAlgo(&ft);

	//begins the second transition of the algorithem!
	initST1stAlgo(&st, &ft, afterMacro, assembli, entry, externFile);
	initST2ndAlgo(&st);

	return 0;
}
