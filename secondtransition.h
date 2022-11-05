#ifndef __SECOND_TRANSITION__
#define __SECOND_TRANSITION__
#include "firsttransition.h"

/*this file do the second transition algorithem that presented in page 46 in the maman 14*/
typedef struct {
	FT* ft;
	FileHandler externCode;
	FileHandler enteryCode;
	FileHandler asembliCode;
}ST;

void initST1stAlgo(ST* st, FT* ft, const char* input, const char* asembliCode, const char* enteryCode, const char* externCode);
void initST2ndAlgo(ST* st);
/*ST3th algo - this method checks if the first field is a symble, if yes- the method will skip it.*/
 
/*ST4th algo - this method checks if is this an instruction of the type: .extern, .struct, .string .data.
If yes - the method will return back to ST2nd algorithem.*/

/*ST5th algo - this method checks if this an entry prompt.
If it's not, the method will activate the ST7th algorithem. */
//void initST5thAlgo(ST* st, int isEntery,char* tmpStr);
void initST6thAlgo(ST* st,char* str);
void initST7thAlgo(ST* st, char* str);

void initST8thAlgo(ST* st, int L);
void initST9thAlgo(ST* st);
void initST10thAlgo(ST* st);
/*ST11th algo - Creating and saving the output files: a code file called 
an external symbol file, and an entry point symbol file.*/
#endif
