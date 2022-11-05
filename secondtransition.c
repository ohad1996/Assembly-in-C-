#define _CRT_SECURE_NO_WARNINGS
#include "secondtransition.h"
#include "exitsitu.h"
#include "base.h"
#include "general.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char* instTypesST[] = { ".struct",".string",".data" };

const char* extrenOrEnteryST[] = { ".extern",".entry" };

/*for activating the first transition by the algorithem,
we will mark every level in the algorithem by:
init+ST+number of the level+th+Algo .
i.e: the first level of the algorithem: init1stAlgo*/

/*this method initialize the second transition with all the needed files!*/
void initST1stAlgo(ST* st, FT* ft, const char* input, const char* asembliCode, const char* enteryCode, const char* externCode)
{
	st->ft = ft;
	st->ft->curLine = 0;
	initFileText(&ft->input, input, "r");
	initFileText(&st->asembliCode, asembliCode, "w");
	initFileText(&st->enteryCode, enteryCode, "w");
	initFileText(&st->externCode, externCode, "w");
}

/*this method reads a line from the file and let the other algorithems to deal with it untill the 
the file ends.*/
void initST2ndAlgo(ST* st) {
	char* tmpStr;
	int preDC = st->ft->DC;
	int i;	
	Word line;
	int isData, isString, isStruct, isExtern, isEntery;
	if (!readFileLine(&st->ft->input)) {//incase we done to read any more lines....
		initST10thAlgo(st);
		return;
	}

	tmpStr = st->ft->input.fileLine;
	removeSpaceFromStart(&tmpStr);
	removeSpaceFromEnd(tmpStr);

	if (isEmptyLine(tmpStr))
	{
		initST2ndAlgo(st);
		return;
	}
	st->ft->curLine++;

	//initST3rdAlgo{
	if (isLabel(tmpStr, st->ft->tmpLabelName)) {
		tmpStr = strstr(tmpStr, ":");
		tmpStr++;
		removeSpaceFromStart(&tmpStr);
		removeSpaceFromEnd(tmpStr);
	}
	//}

	isData = isInstruct(st->ft->psw.labelFlag, tmpStr, instTypesST[_DATA]);
	isString = isInstruct(st->ft->psw.labelFlag, tmpStr, instTypesST[_STRING]);
	isStruct = isInstruct(st->ft->psw.labelFlag, tmpStr, instTypesST[_STRUCT]);
	isExtern = isInstruct(st->ft->psw.labelFlag, tmpStr, extrenOrEnteryST[_EXTERN]);
	isEntery = isInstruct(st->ft->psw.labelFlag, tmpStr, extrenOrEnteryST[_ENTERY]);

	//initST4thAlgo{
		if (isData || isString || isStruct || isExtern) {

			if (isData) {
				tmpStr = strstr(tmpStr, instTypesST[_DATA]);
				tmpStr += (int)strlen(instTypesST[_DATA]);
				removeSpaceFromStart(&tmpStr);
				removeSpaceFromEnd(tmpStr);
				st->ft->DC += countDataType(tmpStr);

			}
			else if (isString)
			{
				tmpStr = strstr(tmpStr, instTypesST[_STRING]);
				tmpStr += (int)strlen(instTypesST[_STRING]);
				removeSpaceFromStart(&tmpStr);
				removeSpaceFromEnd(tmpStr);
				st->ft->DC += countStringType(tmpStr);

			}
			else if (isStruct)
			{
				tmpStr = strstr(tmpStr, instTypesST[_STRUCT]);
				tmpStr += (int)strlen(instTypesST[_STRUCT]);
				removeSpaceFromStart(&tmpStr);
				removeSpaceFromEnd(tmpStr);
				st->ft->DC += countStructType(tmpStr);

			}
			else
			{
				st->ft->psw.externFlag |= 0b1;
			}

			if (!isExtern)
			{
				for (i = preDC; i < st->ft->DC; ++i)
				{
					decaToWord(st->ft->linePrinter++, &line);
					printWordInBase32(&line, st->asembliCode.file);
					fprintf(st->asembliCode.file, "\t");
					printWordInBase32(&st->ft->cpu.mem.dataMem[i], st->asembliCode.file);
					fprintf(st->asembliCode.file, "\n");

				}

			}


			initST2ndAlgo(st);
			return;
		}
	//}

	//initST5thAlgo{
		if (!isEntery) {
			initST7thAlgo(st, tmpStr);
			initST2ndAlgo(st);
			return;
		}

		if(isEntery)
		{
			st->ft->psw.entryFlag |= 0b1;

		}
	//}
	initST6thAlgo(st, tmpStr);
}

/*this method marks the appropriate symbols as entry in the symbol table. 
and then, it does the ST2nd algorithem again*/
void initST6thAlgo(ST* st, char* str) {
	Node* node = NULL;
	Label* label = NULL;
	Word word;
	initWord(&word);
	str = strstr(str, extrenOrEnteryST[_ENTERY]);
	str += (int)strlen(extrenOrEnteryST[_ENTERY]);
	removeSpaceFromStart(&str);
	removeSpaceFromEnd(str);
	node = findNode(st->ft->cpu.simbleTable.head, str, compareLable);
	if (!node)//incase we didnt found the symble by his name
	{
		printf("in line %d : Error - label not found\n", st->ft->curLine);
		st->ft->psw.errorFlag |= 0b1;
		return;
	}
	label = (Label*)node->data;
	fprintf(st->enteryCode.file,"%s\t",label->name);
	decaToWord(label->spot + IN_ADRESS, &word);
	printWordInBase32(&word, st->enteryCode.file);
	fprintf(st->enteryCode.file, "\n");
	initST2ndAlgo(st);
}

/*this method Completes the encoding of the operands starting from the second word
in the binary code of the instruction, according to the addressing method.
If operand is a symbol,the method will find the addressee in the symbol table*/
void initST7thAlgo(ST* st, char* str) {
	Instruction_Op cmd;
	char src[MAX_LINE_LEN];
	char dst[MAX_LINE_LEN];
	int L = 0,preIC = st->ft->IC,i;
	TypeOfOperand left = INVALID_TYPE, right = INVALID_TYPE;
	InsGroup insGroup = INVALID_INS_GROUP;
	Word line;
	cmd = initFT12thAlgo(st->ft, &insGroup, src, dst);
	if (cmd == INVALID_OP)
	{
		printf("%s", INVALID_OPERAND_EXEPTION);
		st->ft->psw.errorFlag |= 0b1;
	}
	else {
		if (insGroup == TWO_OPERANDS)//incase we got two operands
		{
			left = isValidOperand(src);
			right = isValidOperand(dst);
			if (left == INVALID_TYPE || right == INVALID_TYPE)
			{
				printf("in line %d : %s", st->ft->curLine, INVALID_OPERAND_EXEPTION);
				st->ft->psw.errorFlag |= 0b1;
			}
			else
			{
				if (!updateInstructionST(&st->ft->cpu.simbleTable,st->externCode.file,
					&st->ft->cpu.mem.instMem[st->ft->IC], &L, st->ft->curLine, st->ft->linePrinter,
					cmd, insGroup, src, left, dst, right)) {
					st->ft->psw.errorFlag |= 0b1;
				}
			}
		}
		else if (insGroup == ONE_OPERAND)//incase we got one operand
		{
			right = isValidOperand(dst);
			if (right == INVALID_TYPE)//checks if we had an invalid operand!
			{
				printf("in line %d : %s", st->ft->curLine, INVALID_OPERAND_EXEPTION);
				st->ft->psw.errorFlag |= 0b1;

			}
			else
			{
				if (!updateInstructionST(&st->ft->cpu.simbleTable, st->externCode.file,
					&st->ft->cpu.mem.instMem[st->ft->IC], &L, st->ft->curLine, st->ft->linePrinter,
					cmd, insGroup, src, left, dst, right)) {
					st->ft->psw.errorFlag |= 0b1;
				}
			}
		}
		else if (insGroup == NONE)
		{
			L = 1;
		}
		//updates the IC to be IC + L
		initST8thAlgo(st,L);
		for (i = preIC; i < st->ft->IC; ++i)
		{
			decaToWord(st->ft->linePrinter++, &line);
			printWordInBase32(&line, st->asembliCode.file);
			fprintf(st->asembliCode.file, "\t");
			printWordInBase32(&st->ft->cpu.mem.instMem[i], st->asembliCode.file);
			fprintf(st->asembliCode.file, "\n");
		}
	}
}

/*this method updates the IC to be IC + L */
void initST8thAlgo(ST* st, int L){
	st->ft->IC += L;
}

//this method just returns us to the ST2nd algorithem
void initST9thAlgo(ST * st) {
	initST2ndAlgo(st);
}

/*this method checks if there was any errors durring the second transition.
if yes, the method will close all the file that the second transition created!
on the other hand, if there are no error this method does the method of the 
ST11th algorithem - Creating and saving the output files: a code file called 
an external symbol file, and an entry point symbol file.
*/
void initST10thAlgo(ST * st) {
	char* ptr = st->ft->fileName;
	if (st->ft->psw.errorFlag) {
		printf("%s", SECOND_TRANSITION_EXEPTION);
		ptr = strstr(st->ft->fileName, ".");
		strcpy(ptr,ASSEMBLI_FILE_NAME);
		ptr = st->ft->fileName;
		remove(ptr);
		ptr = strstr(st->ft->fileName, ".");
		strcpy(ptr,ENTRY_FILE_NAME);
		ptr = st->ft->fileName;
		remove(ptr);
		ptr = strstr(st->ft->fileName, ".");
		strcpy(ptr,EXTERN_FILE_NAME);
		ptr = st->ft->fileName;
		remove(ptr);
		exit(SECOND_TRANSITION_EXIT);
	}

	fclose(st->asembliCode.file);
	fclose(st->externCode.file);
	fclose(st->enteryCode.file);
	if (!st->ft->psw.externFlag)
	{
		ptr = strstr(st->ft->fileName, ".");
		strcpy(ptr,EXTERN_FILE_NAME);
		ptr = st->ft->fileName;
		remove(ptr);
	}
	if (!st->ft->psw.entryFlag)
	{
		ptr = strstr(st->ft->fileName, ".");
		strcpy(ptr,ENTRY_FILE_NAME);
		ptr = st->ft->fileName;
		remove(ptr);
	}
}
