#ifndef __FIRST_TRANSITION__
#define __FIRST_TRANSITION__


#include "filehandler.h"
#include "cpu.h"
#include "psw.h"
#include "label.h"
#include "instruction.h"

#define COMMA  ","
#define _STRUCT 0
#define _STRING 1
#define _DATA 2
#define _EXTERN 0
#define _ENTERY 1

/*this file do the first transition algorithem that presented in page 45 in the maman 14.

**some of the algorithems are not realy exsist here but they are mentioned 
so we wont lose the tracking after the algorithem of the first transition.*/
typedef struct {
	FileHandler input;
	CPU cpu;
	PSW psw;
	char tmpLabelName[LABEL_SIZE];
	char fileName[LABEL_SIZE];
	int IC, DC,L,curLine,linePrinter;
}FT;

int countStructType(const char* line);
int countStringType(const char* line);
int countDataType(const char* line);
int extractLabel(const char* line, char* dst);

int isLabel(const char* line, char* labelName);
int isValidNum(char* word);
int isInstruct(unsigned int labelFlag, const char* inst, const char* cmp);


void insertStruct(FT* ft, char* fixedStr);
void insertData(FT* ft, char* fixedStr);
void insertString(FT* ft, char* fixedStr);
void insertToSymbleTable(FT* ft, char* labelName);

void initFT1stAlgo(FT* ft, const char* input);
void initFT2ndAlgo(FT*);
/*FT4th & FT5th algo -

The methods will turn on the "symbol is defined" flag.
Then, if this is an instruction to store data, or string or struct - 
we must go to 8th Algo*/
void initFT6thAlgo(FT* ft, char* tmpStr);
void initFT7thAlgo(FT* ft, int isData, int isString, int isStruct);
/*FT8th algo -

The method will check if this is an extern instruction or an entry instruction -
 if not - the method will run the 11th Algo */
void initFT9thAlgo(FT* ft);
/*FT10th algo- 

this algorithem ment only to return us to the FT2ndAlgo*/
void initFT11thAlgo(FT* ft);
Instruction_Op initFT12thAlgo(FT* ft, InsGroup* insGroup, char* src, char* dst);
/*FT13th & FT14th & FT15th algo -

Designed to analyze the structure of the operands
and calculate its binary code and also calculate L and of course update the IC with
it */
void initFT16thAlgo(FT* ft);
void initFT17thAlgo(FT* ft);
void initFT18thAlgo(FT* ft);


#endif
