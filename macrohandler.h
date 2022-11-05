#ifndef __MACRO_HANDLER__
#define __MACRO_HANDLER__


#include "macro.h"
#include "filehandler.h"
#include "linkedList.h"
#include "psw.h"


#define  END_OF_MAC "endmacro"
#define  START_MAC "macro "

typedef struct {
	FileHandler input;
	FileHandler output;
	List macList;
	PSW psw;

}MacIO;


void macroCpy(FileHandler* fh, Macro* mac);
int isMac(char** fixStr);
int isEndMac(char* str);
void macInsertAlgo(MacIO* macIO, Macro* md);
void macLinesCreatingAlgo(MacIO* macIO, Macro* mac);
void macCreatingAlgo(MacIO* macIO, Macro* mac, char* macName);
void initMacAlgo(MacIO* macIO);
void initMacIO(MacIO* macIO, const char* input, const char* output);
#endif
