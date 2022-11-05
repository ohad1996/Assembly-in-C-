#define _CRT_SECURE_NO_WARNINGS
#include "macrohandler.h"
#include "exitsitu.h"
#include "general.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//this method initialze the macro - input - output for holding its fields!
void initMacIO(MacIO* macIO, const char* input, const char* output) {
	if (!macIO) {
		printf("%s", NULL_PTR_EXEPTION);
		exit(NULL_PTR_EXIT);
	}
	initFileText(&macIO->input, input, "r");
	initFileText(&macIO->output, output, "w");
	initializeList(&macIO->macList);
	initPSW(&macIO->psw);
}

//level 1 - Read the following line from the source file. If the file is finished- do the last part of the algo.
void initMacAlgo(MacIO* macIO) {
	Node* ptr = NULL;
	Macro* mac = NULL;
	Macro* tmp=NULL;
	char* tmpStr;
	if (!readFileLine(&macIO->input)) {
		fclose(macIO->output.file);
		fclose(macIO->input.file);

		return;
	}
	tmpStr = macIO->input.fileLine;
	removeSpaceFromStart(&tmpStr);
	removeSpaceFromEnd(tmpStr);

	ptr = findNode(macIO->macList.head, tmpStr, compareMacroData);
	if (ptr != NULL) {
		mac = (Macro*)ptr->data;
		macroCpy(&macIO->output, mac);
		initMacAlgo(macIO);
	}
	else if (isMac(&tmpStr)) {
		tmp = (Macro*)malloc(sizeof(Macro));
		initMacro(tmp, tmpStr);
		changeFlagMac(&macIO->psw);
		macLinesCreatingAlgo(macIO, tmp);
	}
	else {
		fprintf(macIO->output.file, "%s\n", tmpStr);
		initMacAlgo(macIO);
	}
}

//level 2- this method changes the macro flag incase of its apperence.
//& continue to the next level of the algorithem.
void macCreatingAlgo(MacIO* macIO, Macro* mac, char* macName) {
	changeFlagMac(&macIO->psw);
	macLinesCreatingAlgo(macIO, mac);
}

/*level 3 - this method incase of apperence of the first field a
macro name in the macro table is replacing the macro name and copy
all the appropriate lines from the table to the file and if its goes
completly, the method will continue
to the next level of the algorithem. in any other cases it will return to the beginning*/
void macLinesCreatingAlgo(MacIO* macIO, Macro* mac) {
	char* temp = NULL;

	while (macIO->psw.macroFlag) {
		if (!readFileLine(&macIO->input)) {
			printf("%s", INVALID_MACRO_EXEPTION);
			exit(INVALID_MACRO_EXIT);
		}

		temp = macIO->input.fileLine;
		removeSpaceFromStart(&temp);
		removeSpaceFromEnd(temp);
		 if (isEndMac(temp) ){
			macInsertAlgo(macIO, mac);
		}
		else {
			addLineToMacro(mac, temp);
		}
	}
	initMacAlgo(macIO);
}

//level 4 - this method turns on the macro flag
void macInsertAlgo(MacIO* macIO, Macro* md) {
	changeFlagMac(&macIO->psw);
	insert(&macIO->macList.head, md);
}
//this method checks when it is the end of the macro!  
int isEndMac(char* str) {
	return !strcmp(str, END_OF_MAC);
}

//this method checks if the given str represents a macro.
int isMac(char** fixStr) {
	char* ptr = strstr(*fixStr, START_MAC);
	if (ptr != NULL) {
		ptr += strlen(START_MAC);
		removeSpaceFromStart(&ptr);
		*fixStr = ptr;
		return 1;
	}
	return 0;
}

//this method prints in the given file the given macro commands!
void macroCpy(FileHandler* fh, Macro* mac) {
	int i;
	for (i = 0; i < mac->commandsSize; ++i) {
		fprintf(fh->file, "%s\n", mac->commands[i]);
	}
}
