#define _CRT_SECURE_NO_WARNINGS
#include "firsttransition.h"
#include "exitsitu.h"
#include "general.h"
#include "psw.h"
#include "cpu.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


const char* instTypesFT[] = {".struct",".string",".data" };

const char* extrenOrEnteryFT[] = {".extern",".entry"};

/*
this method checks if the line is an instruction.
0 - if its false,1- if its true*/
int isInstruct(unsigned int labelFlag, const char* inst, const char* cmp)
{
	char temp[MAX_LINE_LEN];
	char* DELIMETERS = " \t";
	char* word = NULL;
	strcpy(temp, inst);
	word = strtok(temp, DELIMETERS);
	if (!word)
		return 0;
	if (labelFlag)//only if its label we shall check the next part of the instruction!
	{
		word = strtok(NULL, DELIMETERS);
		if (!word)
			return 0;
	}
	if (!strcmp(word, cmp))
		return 1;
	return 0;
}

/*this program extract a label
0 - if it didnt happend,1- if it happends*/
int extractLabel(const char* line,char* dst)
{
	char temp[MAX_LINE_LEN];
	strcpy(temp, line);
	char* ptr = strstr(temp, ":");
	if (ptr){
		*ptr = '\0';
		ptr = temp;
		strcpy(dst, ptr);
		return 1;
	}
	return 0;
}
/*this program checks if there is a use of label in the current line*/
int isLabel(const char* line,char* labelName) {
	char temp[MAX_LINE_LEN];
	char* ptr = NULL;
	strcpy(temp, line);
	if (!isalpha(temp[0])) {
		return 0;
	}
	ptr = strstr(temp, ":");
	if (!ptr)
		return 0;
	*ptr = '\0';
	--ptr;
	if ((ptr - temp) > 30)
		return 0;
	for (; temp < ptr; ptr--)
	{
		if (!isalpha(*ptr) && !isdigit(*ptr))
			return 0;
	}
	return extractLabel(line, labelName);
}

/*this method counts how many numbers there are in the given line
if there is no numbers in this line the method will return -1
*/
int countDataType(const char* line)
{
	char* DELIMETERS = ", \t";
	char* word = NULL;
	int counter = 0;
	char temp[MAX_LINE_LEN];
	strcpy(temp, line);
	word = strtok(temp, DELIMETERS);
	while (word)
	{
		if (!isValidNum(word))
		{
			return -1;
		}
		word = strtok(NULL, DELIMETERS);
		counter++;
	}
	return counter;
}

//this method counts the length of the string that inside -> " "
int countStringType(const char* line)
{
	int size = strlen(line);
	if (line[0] != '\"' || line[size-1] != '\"')
		return -1;
	return size - 2 + 1; //2 of " and 1 for '\0'
}


int countStructType(const char* line)
{
	int countString = 0;
	char temp[MAX_LINE_LEN];
	char* word = NULL;
	strcpy(temp, line);
	word = strtok(temp, COMMA);
	if (word == NULL)
		return -1;

	removeSpaceFromEnd(word);
	removeSpaceFromStart(&word);

	if (countDataType(word) != 1)
		return -1;
	strcpy(word, line);
	word = strstr(word, COMMA);
	word++;
	if (!word)
		return -1;
	word++;

	removeSpaceFromEnd(word);
	removeSpaceFromStart(&word);

	countString = countStringType(word);
	if (countString != -1)
		return countString + 1;
	return -1;
}

/*this method checks if the string is a valid number
0 - if its false,1- if its true*/
int isValidNum(char* word)
{
	if (*word == '+' || *word == '-')
		word++;
	while (*word != '\0')
	{
		if (!isdigit(*word))
			return 0;
		word++;
	}
	return 1;
}

//first level in the transition algorithem is to initialize the variabel FT and to reset
//DC & IC to 0.
void initFT1stAlgo(FT* ft, const char* input) {
	char temp[MAX_LINE_LEN];
	char* ptr = NULL;
	strcpy(temp,input);
	if (!ft)
	{
		printf("%s", NULL_PTR_EXEPTION);
		exit(NULL_PTR_EXIT);
	}
	ft->curLine = 0;
	initFileText(&ft->input, input, "r");
	initializeList(&ft->cpu.simbleTable);
	initPSW(&ft->psw);
	ft->DC = 0;
	ft->IC = 0;
	ft->linePrinter = IN_ADRESS;
	ptr = strstr(temp,".");
	ptr++;
	*ptr = '\0';
	ptr = temp;
	strcpy(ft->fileName,ptr);
}

/*for activating the first transition by the algorithem,
we will mark every level in the algorithem by:
init+FT+number of the level+th+Algo .
i.e: the first level of the algorithem: init1stAlgo*/
void initFT2ndAlgo(FT* ft) {
	Instruction_Op f12;
	char* tmpStr;
	char src[MAX_LINE_LEN];
	char dst[MAX_LINE_LEN];
	int L = 0;
	TypeOfOperand left= INVALID_TYPE, right = INVALID_TYPE;
	InsGroup insGroup = INVALID_INS_GROUP;
	int isData,isString,isStruct,isExtern,isEntery;
	if (!readFileLine(&ft->input)) {
		initFT16thAlgo(ft);
		initFT17thAlgo(ft);
		initFT18thAlgo(ft);//second Transition
		return;
	}
	tmpStr = ft->input.fileLine;
	removeSpaceFromStart(&tmpStr);
	removeSpaceFromEnd(tmpStr);

	if (isEmptyLine(tmpStr))
	{
		initFT2ndAlgo(ft);
		return;
	}
	ft->curLine++;
	//initFT3rdAlgo{
		if (isLabel(tmpStr,ft->tmpLabelName)) {
			//initFT4thAlgo{
				changeFlagLabel(&ft->psw);
			//}
		}
	//}
	isData = isInstruct(ft->psw.labelFlag, tmpStr, instTypesFT[_DATA]);
	isString = isInstruct(ft->psw.labelFlag, tmpStr, instTypesFT[_STRING]);
	isStruct = isInstruct(ft->psw.labelFlag, tmpStr, instTypesFT[_STRUCT]);
	//initFT5thAlgo{
		if (!isData && !isString && !isStruct)
		{
			//initFT8thAlgo{
				isExtern = isInstruct(ft->psw.labelFlag, tmpStr, extrenOrEnteryFT[_EXTERN]);
				isEntery = isInstruct(ft->psw.labelFlag, tmpStr, extrenOrEnteryFT[_ENTERY]);
				if (!isExtern && !isEntery) {
					if (ft->psw.labelFlag) {
						changeFlagLabel(&ft->psw);
						initFT11thAlgo(ft);
					}
					//initFT12thAlgo{
					f12 = initFT12thAlgo(ft,&insGroup,src,dst);
					//}
					if (f12 == INVALID_OP)
					{
						printf("in line %d : %s", ft->curLine, INVALID_OPERAND_EXEPTION);
						ft->psw.errorFlag |= 0b1;
					}
					else {
						if (insGroup == TWO_OPERANDS)
						{
							left =  isValidOperand(src);
							right =  isValidOperand(dst);
							if (left == INVALID_TYPE || right == INVALID_TYPE)
							{
								printf("in line %d : %s", ft->curLine, INVALID_OPERAND_EXEPTION);
								ft->psw.errorFlag |= 0b1;
							}
							else
							{
								//initFT13thAlgo{
									updateInstructionFT(&ft->cpu.mem.instMem[ft->IC], &L,
									f12, insGroup, left, right);
								//}
							}
						}
						else if (insGroup == ONE_OPERAND)
						{
							right = isValidOperand(dst);
							if ( right == INVALID_TYPE)
							{
								printf("in line %d : %s", ft->curLine, INVALID_OPERAND_EXEPTION);
								ft->psw.errorFlag |= 0b1;

							}
							else
							{
								//initFT13thAlgo{
									updateInstructionFT(&ft->cpu.mem.instMem[ft->IC], &L,
									f12, insGroup, left, right);
								//}
							}
						}
						else if (insGroup == NONE)
						{
						//initFT13thAlgo{
							updateInstructionFT(&ft->cpu.mem.instMem[ft->IC], &L,
								f12, insGroup, left, right);
						//}
						}
						//initFT14thAlgo{
							ft->IC += L;
						//}
					}
				}
				else if (isExtern) {
					tmpStr = strstr(tmpStr, extrenOrEnteryFT[_EXTERN]);
					tmpStr += (int)strlen(extrenOrEnteryFT[_EXTERN]);
					removeSpaceFromStart(&tmpStr);
					removeSpaceFromEnd(tmpStr);
					insertToSymbleTable(ft, tmpStr);
					if (ft->psw.labelFlag) {
						changeFlagLabel(&ft->psw);
						printf("in line %d : WARNNING: LABEL BEFORE EXTERN IS UNSAFE\n", ft->curLine);
						initFT9thAlgo(ft);
					}
				}
				initFT2ndAlgo(ft);
				return;
			//}
		}
		else
		{
		//Case data struct or string
			if (ft->psw.labelFlag) {
				changeFlagLabel(&ft->psw);
				initFT6thAlgo(ft, tmpStr);
			}
			initFT7thAlgo(ft, isData,isString,isStruct);
		}
	//}
}

/*this method incase if there is a symbol (label) definition,the method will
insert it into the symbol table with a checkmark 
(symbol of type data)-Its value will be DC. If the symbol is already in the table,
an error must be reported by the method.*/
void initFT6thAlgo(FT* ft, char* tmpStr) {
	Node* temp = findNode(ft->cpu.simbleTable.head, ft->tmpLabelName, compareLable);
	Label* lb;
	if (temp != NULL)
	{
		printf("in line %d : %s", ft->curLine,DUPLICATION_OF_ARGUMENTS_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}
	lb = (Label*)malloc(sizeof(Label));
	if (!lb)
	{
		printf("%s", ALLOCATION_ERROR_EXEPTION);
		exit (ALLOCATION_ERROR_EXIT);
	}
	initLabel(lb, ft->tmpLabelName, ft->DC, DATA);
	insert(&ft->cpu.simbleTable.head, lb);
}

/*this method detecting the type of characters, encode them in memory, updates the data counter DC in the inner functions*/
void initFT7thAlgo(FT* ft,int isData,int isString,int isStruct)
{
	char* sub = NULL;
	if (isData == 1)
	{
		sub = strstr(ft->input.fileLine, ".data");
		sub += strlen(".data");
		insertData(ft, sub);
	}
	else if (isString == 1)
	{
		sub = strstr(ft->input.fileLine, ".string");
		sub += strlen(".string");
		insertString(ft,sub);
	}
	else if(isStruct == 1)
	{
		sub = strstr(ft->input.fileLine, ".struct");
		sub += strlen(".struct");
		insertStruct(ft,sub);
	}
	initFT2ndAlgo(ft);//and in the end-> continues to the second algorithem  in first transition!
}

void insertData(FT* ft, char* fixedStr){
	char tempStr[MAX_LINE_LEN];
	int counter = ft->DC;
	char* word = NULL;
	Word temp;
	int val;
	initWord(&temp);
	removeSpaceFromStart(&fixedStr);
	removeSpaceFromEnd(fixedStr);
	strcpy(tempStr, fixedStr);
	word = strtok(tempStr, COMMA);

	while (word != NULL)
	{
		removeSpaceFromStart(&word);
		removeSpaceFromEnd(word);
		if (!isValidNum(word))
		{
			printf("in line %d : %s", ft->curLine, INVALID_NUM_OF_ARGUMENT_EXEPTION);
			ft->psw.errorFlag |= 0b1;
			return;
		}
		val = atoi(word);
		decaToWord(val, &temp);
		wordCpy(&ft->cpu.mem.dataMem[counter++], &temp);
		word = strtok(NULL, COMMA);
	}
	ft->DC = counter;
}


void insertString(FT* ft,char* fixedStr) {
	char tempStr[MAX_LINE_LEN];
	const char DELIMINITERS = '\"';
	int counter = ft->DC, numOfArguments=1;
	const int MAX_ARGUMENTS = 2;
	Word temp;
	int val;
	char* ptr = tempStr;
	strcpy(tempStr, fixedStr);
	initWord(&temp);
	removeSpaceFromStart(&ptr);
	removeSpaceFromEnd(ptr);

	if (*ptr != DELIMINITERS)
	{
		printf("in line %d : %s", ft->curLine, INVALID_NUM_OF_ARGUMENT_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}
	ptr++;
	while (*ptr != '\0')
	{
		if (*ptr == DELIMINITERS)
			numOfArguments++;
		else
		{
			val = (int)*ptr;
			decaToWord(val, &temp);
			wordCpy(&ft->cpu.mem.dataMem[counter++], &temp);//entering to the data memory the word by its binary number!
		}

		//needs to be less than the maximum number of the arguments!
		if (numOfArguments > MAX_ARGUMENTS) {
			printf("in line %d : %s", ft->curLine, NUM_OF_ARGUMENTS_EXEPTION);
			ft->psw.errorFlag |= 0b1;
			return;
		}
		ptr++;
	}

	//needs to be equals to the maximum number of the arguments!
	if (numOfArguments != MAX_ARGUMENTS)
	{
		printf("in line %d : %s", ft->curLine, NUM_OF_ARGUMENTS_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}

	val = 0;
	decaToWord(val, &temp);
	wordCpy(&ft->cpu.mem.dataMem[counter++], &temp);//entering to the data memory the word by its binary number!
	ft->DC = counter;
}

void insertStruct(FT* ft, char* fixedStr){
	char tempStr[MAX_LINE_LEN];
	char* dataStr = NULL;
	removeSpaceFromStart(&fixedStr);
	removeSpaceFromEnd(fixedStr);
	strcpy(tempStr, fixedStr);
	dataStr = strstr(tempStr, COMMA);
	if (dataStr)
		*dataStr = '\0';
	else
	{
		printf("in line %d : %s", ft->curLine, INVALID_OPERAND_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}

	dataStr = tempStr;
	if (dataStr)
	{
		insertData(ft, dataStr);
		strcpy(tempStr, fixedStr);
		dataStr = strtok(tempStr, COMMA);
		dataStr = strtok(NULL, COMMA);
		if (dataStr)
		{
			removeSpaceFromStart(&dataStr);
			removeSpaceFromEnd(dataStr);
			insertString(ft, dataStr);
			dataStr = strtok(NULL, COMMA);
			if (dataStr)
			{
				printf("in line %d : %s", ft->curLine, INVALID_OPERAND_EXEPTION);
				ft->psw.errorFlag |= 0b1;
				return;
			}
		}
		else
		{
			printf("in line %d : %s", ft->curLine, INVALID_OPERAND_EXEPTION);
			ft->psw.errorFlag |= 0b1;
		}
	}
}
/*this method incase of an extern, inserts any symbol (one or more) that appears as an
operand of the directive into the symbol table without a value, with a check
 (symbol of type external ).*/
void initFT9thAlgo(FT* ft) {
	char* ptr = strstr(ft->input.fileLine,".extern");
	char* first = NULL;
	ptr += strlen(".extern");
	removeSpaceFromEnd(ptr);
	removeSpaceFromStart(&ptr);

	if (!isalpha(*ptr)) {
		printf("in line %d : %s", ft->curLine, EXTERN_ERROR_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return ;
	}
	if ((strlen(ptr)) > 30) {
		printf("in line %d : %s", ft->curLine, EXTERN_ERROR_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}
	first = ptr;
	ptr++;
	while(*ptr!='\0')
	{
		if (!isalpha(*ptr) && !isdigit(*ptr)) {
			printf("in line %d : %s", ft->curLine, EXTERN_ERROR_EXEPTION);
			ft->psw.errorFlag |= 0b1;
			return;
		}
		ptr++;
	}
	strcpy(ft->tmpLabelName, first);

	Node* temp = findNode(ft->cpu.simbleTable.head, ft->tmpLabelName, compareLable);
	Label* lb;
	if (temp != NULL)
	{
		printf("in line %d : %s", ft->curLine,DUPLICATION_OF_ARGUMENTS_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}
	lb = (Label*)malloc(sizeof(Label));
	if (!lb)
	{
		printf("in line %d : %s", ft->curLine, ALLOCATION_ERROR_EXEPTION);
		exit(ALLOCATION_ERROR_EXIT);
	}
	initLabel(lb, ft->tmpLabelName, 0, EXTERNAL);
	insert(&ft->cpu.simbleTable.head, lb);
}

/*those two methods checks if there is a symbol definition, if yes, insert it into the symbol table with a checkmark.
the first method ment for */
void initFT11thAlgo(FT* ft) {
	Node* temp = findNode(ft->cpu.simbleTable.head, ft->tmpLabelName, compareLable);
	Label* lb;
	if (temp != NULL)//incase of duplication of our symble definition
	{
		printf("in line %d: %s", ft->curLine, DUPLICATION_OF_ARGUMENTS_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}
	lb = (Label*)malloc(sizeof(Label));
	if (!lb)//incase there is no duplication-> the method will creat a spot to our new label!
	{
		printf("in line %d : %s", ft->curLine, ALLOCATION_ERROR_EXEPTION);
		exit(ALLOCATION_ERROR_EXIT);
	}
	initLabel(lb, ft->tmpLabelName, ft->IC, CODE);
	insert(&ft->cpu.simbleTable.head, lb);
}

//this method updates to the first transition's symble table a new label! 
void insertToSymbleTable(FT* ft,char* labelName) {
	Node* temp = findNode(ft->cpu.simbleTable.head, labelName, compareLable);
	Label* lb;
	if (temp != NULL)//incase of duplication of our symble definition
	{
		printf("in line %d : %s", ft->curLine, DUPLICATION_OF_ARGUMENTS_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return;
	}
	lb = (Label*)malloc(sizeof(Label));
	if (!lb)//incase there is no duplication-> the method will creat a spot to our new label!
	{
		printf("in line %d : %s", ft->curLine, ALLOCATION_ERROR_EXEPTION);
		exit(ALLOCATION_ERROR_EXIT);
	}
	initLabel(lb, labelName,1, EXTERNAL);
	insert(&ft->cpu.simbleTable.head, lb);
}

//This function supposed to extract fields i.e Instruion_Op = MOV, src = r3 , dst = "s1.1" 
Instruction_Op initFT12thAlgo(FT* ft, InsGroup* insGroup, char* src, char* dst) {
	char* DELIMITER = " ,\t";
	char* word = NULL;
	char temp[MAX_LINE_LEN];
	char* op = NULL;
	Instruction_Op command;
	InsGroup ig;
	strcpy(temp, ft->input.fileLine);
	op = strstr(temp, ":");
	if (op != NULL)
		op++;
	else
		op = temp;
	removeSpaceFromStart(&op);
	word = strtok(op, DELIMITER);
	command = isValidOP(word);//extracting the type of command from our word!
	if (command == INVALID_OP)//incase its not a real command!
	{
		printf("in line %d : %s", ft->curLine, INVALID_OPERAND_TYPE_EXEPTION);
		ft->psw.errorFlag |= 0b1;
		return INVALID_OP;
	}
	ig = getInsGroupByInsOp(command);//if its real command, this method will check what kind of instruction group this command from
	if (ig == TWO_OPERANDS)//incase we expecting for 2 operands
	{
		word = strtok(NULL, DELIMITER);
		if (!word)
		{
			printf("in line %d : %s", ft->curLine, INVALID_OPERAND_TYPE_EXEPTION);
			ft->psw.errorFlag |= 0b1;
			return INVALID_OP;
		}
		removeSpaceFromStart(&word);
		removeSpaceFromEnd(word);
		strcpy(src, word);
		word = strtok(NULL, DELIMITER);
		if (!word)
		{
			printf("in line %d : %s", ft->curLine, INVALID_OPERAND_TYPE_EXEPTION);
			ft->psw.errorFlag |= 0b1;
			return INVALID_OP;
		}
		removeSpaceFromEnd(word);
		strcpy(dst, word);
	}
	else if (ig == ONE_OPERAND)//incase we do expect for one operand
	{
		word = strtok(NULL, DELIMITER);
		if (!word)
		{
			printf("in line %d : %s", ft->curLine, INVALID_OPERAND_TYPE_EXEPTION);
			ft->psw.errorFlag |= 0b1;
			return INVALID_OP;
		}
		removeSpaceFromStart(&word);
		removeSpaceFromEnd(word);
		strcpy(dst, word);
	}
	//incase of none operand there notheing after the command!
	*insGroup = ig;
	return command;
}

/*this method checks i the first transition went without any mistakes.
incase of mistake, this method will stop the program and will block
the second transition to start!
*/
void initFT16thAlgo(FT* ft) {
	if (ft->psw.errorFlag) {
		printf("%s", FIRST_TRANSITION_EXEPTION);
		exit(FIRST_TRANSITION_EXIT);
	}
}

/*this method updates the symbol table with the value of the data type symbols,
by adding the final value of IC*/
void initFT17thAlgo(FT* ft) {
	Node* node = ft->cpu.simbleTable.head;
	Label* lb = NULL;
	while (node!=NULL) {
		lb = (Label*)node->data;
		if (lb->type==DATA) {
			lb->spot += ft->IC;//adding the final value of IC
		}
		node = node->next;
	}
}

/*this method prepares the begining of the second transition + restarting the IC & DC for the ST !*/
void initFT18thAlgo(FT* ft) {
	ft->DC = 0;
	ft->IC = 0;
	fclose(ft->input.file);
}
