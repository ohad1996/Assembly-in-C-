#define _CRT_SECURE_NO_WARNINGS
#include "instruction.h"

#include "reg.h"
#include "mem.h"
#include "exitsitu.h"
#include "linkedList.h"
#include "label.h"
#include "general.h"
#include "base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char INSTRUCTIONS[][INSTRUCTION_MAX_LEN + 1] =
{ "mov","cmp","add","sub","lea","not","clr","inc",
"dec","jmp","bne","get","prn","jsr","rts","hlt" };

const Word operWord[] =
{ {0b0000000000,0b0},{0b0001000000,0b0},{0b0010000000,0b0},{0b0011000000,0b0}
, {0b0100000000,0b0},{0b0101000000,0b0},{0b0110000000,0b0},{0b0111000000,0b0}
, {0b1000000000,0b0},{0b1001000000,0b0},{0b1010000000,0b0},{0b1011000000,0b0}
, {0b1100000000,0b0},{0b1101000000,0b0},{0b1110000000,0b0},{0b1111000000,0b0} };

//0 - number 1 - struct, 2 -Label 3 - Regiter
const Word opDstChange[] = {
	{0b0000000000,0b0},
	{0b0000001000,0b0},
	{0b0000000100,0b0},
	{0b0000001100,0b0}
};

const Word opSrcChange[] = {
	{0b0000000000,0b0}, //number
	{0b0000100000,0b0}, //struct
	{0b0000010000,0b0}, //label
	{0b0000110000,0b0} //register
};

const Word changeARE[] = {
	{0b0000000000,0b0}, //NUM and Reg
	{0b0000000010,0b0}, //Struct and Label
	{0b0000000011,0b0}, //Extern
};

int isEmptyLine(char* str)
{
	char temp[MAX_LEN_INSTRUCTION];
	char* ptr = NULL;
	strcpy(temp,str);
	ptr = temp;
	removeSpaceFromEnd(ptr);
	removeSpaceFromStart(&ptr);
	if (*ptr == '\0' || *ptr == ';')//checks if the line isnt empty
		return 1;
	return 0;
}

//Word is the pointer to the correct data to be updated i.e if IC = 3 so we call function &w[3]
void updateInstructionFT(Word* words, int* size, Instruction_Op op, InsGroup IG, TypeOfOperand left, TypeOfOperand right)
{
	int counter = 0;
	wordCpy(&words[counter++], &operWord[(int)op]);
	//incase of none operands there is no need for counting beacuse there is nothing to count!
	if (IG == ONE_OPERAND) //incase of one operand
	{
		if (right == STRUCT_TYPE) {
			counter += 2;
		}
		else
			counter++;
		updateDstFT(words, right);
	}
	else if (IG == TWO_OPERANDS) //incase of two operands
	{
		if (right == REGISTER_TYPE && left == REGISTER_TYPE) {//spesific situation that counts only +1
			counter++;
		}
		else {
			if (right == STRUCT_TYPE) {
				counter += 2;
			}
			else
				counter++;
			if (left == STRUCT_TYPE) {
				counter += 2;
			}
			else
				counter++;
		}
		//updating the source and the destination
		updateSrcFT(words, left);
		updateDstFT(words, right);
	}
	*size = counter;
	//updating & returninng the size that we found
	// thanks to the operands.
}

/*
this method update the word by its bitwise of the type of operand from the parameter that
was on the right side of the file line
*/
void updateDstFT(Word* words, TypeOfOperand right)
{
	Word temp;
	initWord(&temp);
	if (right == NUM_TYPE)
	{
		wordCpy(&temp, &opDstChange[(int)NUM_TYPE]);
		words[0].data |= temp.data;
	}
	else if (right == STRUCT_TYPE)
	{
		wordCpy(&temp, &opDstChange[(int)STRUCT_TYPE]);
		words[0].data |= temp.data;

	}
	else if (right == LABEL_TYPE)
	{
		wordCpy(&temp, &opDstChange[(int)LABEL_TYPE]);
		words[0].data |= temp.data;

	}
	else if (right == REGISTER_TYPE)
	{
		wordCpy(&temp, &opDstChange[(int)REGISTER_TYPE]);
		words[0].data |= temp.data;
	}
}

/*
this method update the word by its bitwise from the type of the parameter that
was on the left side of the file line
*/
void updateSrcFT(Word* words, TypeOfOperand right)
{
	Word temp;
	initWord(&temp);

	if (right == NUM_TYPE)
	{
		wordCpy(&temp, &opSrcChange[(int)NUM_TYPE]);
		words[0].data |= temp.data;
	}
	else if (right == STRUCT_TYPE)
	{
		wordCpy(&temp, &opSrcChange[(int)STRUCT_TYPE]);
		words[0].data |= temp.data;

	}
	else if (right == LABEL_TYPE)
	{
		wordCpy(&temp, &opSrcChange[(int)LABEL_TYPE]);
		words[0].data |= temp.data;

	}
	else if (right == REGISTER_TYPE)
	{
		wordCpy(&temp, &opSrcChange[(int)REGISTER_TYPE]);
		words[0].data |= temp.data;
	}
}

int updateInstructionST(List* lst,FILE* file ,Word* words, int* size,int curLine, int printLine, Instruction_Op op, InsGroup IG, char* src, TypeOfOperand left, char* dst, TypeOfOperand right)
{
	int counter = 0;
	if (IG == ONE_OPERAND)
	{
		if (right == STRUCT_TYPE) {
			if (!updateDstST(file, lst, words + 1, dst, right,1,curLine,  printLine))
			{
				*size = 0;
				return 0;
			}
			counter += 2;
		}
		else
		{
			if (updateDstST(file, lst, words + 1, dst, right,1, curLine,  printLine) == 0)
			{
				*size = 0;
				return 0;
			}
			counter++;
		}

	}
	else if (IG == TWO_OPERANDS)
	{
		if (right == REGISTER_TYPE && left == REGISTER_TYPE) {
			initWord(&words[1]);
			if (!updateSrcST(file, lst, words + 1, src, left, curLine,  printLine) || !updateDstST(file, lst, words + 1, dst, right,0, curLine,  printLine))
			{
				*size = 0;
				return 0;
			}
			counter++;
		}
		else {
			if (left == STRUCT_TYPE) {
				if (!updateSrcST(file, lst, words + 1, src, left, curLine,  printLine))
				{
					*size = 0;
					return 0;
				}
				counter += 2;
			}
			else
			{
				if (!updateSrcST(file, lst, words + 1, src, left, curLine,  printLine))
				{
					*size = 0;
					return 0;
				}
				counter++;
			}

			if (right == STRUCT_TYPE)
			{
				if (!updateDstST(file, lst, words + 1 + counter, dst, right,1, curLine,  printLine))
				{
					*size = 0;
					return 0;
				}
				counter += 2;
			}
			else
			{
				if (!updateDstST(file, lst, words + 1 + counter, dst, right,1, curLine,  printLine))
				{
					*size = 0;
					return 0;
				}
				counter++;
			}
		}
	}
	*size = counter +1 ;
	return 1;
}

int updateDstST(FILE* file,List*lst, Word* words, char* dst, TypeOfOperand right, int isReset,int curLine, int printLine)
{
	Word temp;
	Word val ;
	int valnum;
	char* subStruct = NULL;
	initWord(&temp);
	initWord(&val);
	if (right == NUM_TYPE)
	{
		subStruct = strstr(dst, "#");
		subStruct++;
		valnum = atoi(subStruct);
		decaToWord(valnum, &val);
		val.data <<= MOVE_DST_INTO_WORD_SPOT;
		initWord(&words[0]);

		words[0].data |= val.data;
	}
	else if (right == STRUCT_TYPE)
	{
		subStruct = strtok(dst, ".");
		if (findSymbleLocation( file,lst, words, subStruct,curLine,  printLine))
		{
			wordCpy(&temp, &changeARE[(int)STRUCT_LABEL_TYPE]);
			words[0].data |= temp.data;
		}
		else
		{
			printf("in line %d : %s", curLine, INVALID_OPERAND_EXEPTION);
			return 0;
		}
		subStruct = strtok(NULL, ".");
		valnum = atoi(subStruct);
		decaToWord(valnum, &val);
		val.data <<= MOVE_DST_INTO_WORD_SPOT;
		initWord(&words[1]);

		words[1].data |= val.data;
	}
	else if (right == LABEL_TYPE)
	{
		if (findSymbleLocation(file,lst, words, dst, curLine,  printLine))
		{
			wordCpy(&temp, &changeARE[(int)STRUCT_LABEL_TYPE]);
			words[0].data |= temp.data;
		}
		else
		{
			printf("in line %d : %s", curLine,  INVALID_OPERAND_EXEPTION);
			return 0;
		}
	}
	else if (right == REGISTER_TYPE)
	{
		subStruct = strstr(dst, "r");
		subStruct++;
		valnum = atoi(subStruct);
		decaToWord(valnum, &val);
		val.data <<= MOVE_DST_INTO_WORD_SPOT;
		if(isReset)
			initWord(&words[0]);

		words[0].data |= val.data;
	}
	return 1;
}

int updateSrcST(FILE* file,List * lst, Word * words, char* dst, TypeOfOperand right,int curLine,int printLine)	{
	Word temp;
	Word val;
	int valnum;
	char* subStruct = NULL;
	initWord(&temp);
	initWord(&val);
	if (right == NUM_TYPE)//incase the right type of operand is number
	{
		subStruct = strstr(dst, "#");
		subStruct++;
		valnum = atoi(subStruct);
		decaToWord(valnum, &val);
		val.data <<= MOVE_DST_INTO_WORD_SPOT;
		initWord(&words[0]);
		words[0].data |= val.data;
	}
	else if (right == STRUCT_TYPE)//incase the right type of operand is struct
	{
		subStruct = strtok(dst, ".");
		if (findSymbleLocation(file,lst, words, subStruct,curLine,printLine))
		{
			wordCpy(&temp, &changeARE[(int)STRUCT_LABEL_TYPE]);
			words[0].data |= temp.data;
		}
		else
		{
			printf("in line %d : %s", curLine, INVALID_OPERAND_EXEPTION);
			return 0;
		}
		subStruct = strtok(NULL, ".");
		valnum = atoi(subStruct);
		decaToWord(valnum, &val);
		val.data <<= MOVE_DST_INTO_WORD_SPOT;
		initWord(&words[1]);
		words[1].data |= val.data;
	}
	else if (right == LABEL_TYPE)//incase the right type of operand is label
	{
		if (findSymbleLocation(file,lst, words, dst,curLine,printLine))
		{
			wordCpy(&temp, &changeARE[(int)STRUCT_LABEL_TYPE]);
			words[0].data |= temp.data;
		}
		else
		{
			printf("in line %d : %s", curLine,  INVALID_OPERAND_EXEPTION);
			return 0;
		}
	}
	else if (right == REGISTER_TYPE)//incase the right type of operand is register
	{
		subStruct = strstr(dst, "r");
		subStruct++;
		valnum = atoi(subStruct);
		decaToWord(valnum, &val);
		val.data <<= MOVE_SRC_INTO_WORD_SPOT;
		initWord(&words[0]);
		words[0].data |= val.data;
	}
	return 1;
}

/*
this method finds the location of the given name of the symble and update its word.
*/
int findSymbleLocation(FILE* file,List*lst,Word* word,char* symbName,int curLine,int printLine) {
	Node* node = NULL;
	Word wordExternal;
	Label* label = NULL;
	int adress;
	node = findNode(lst->head, symbName, compareLable);
	if (node == NULL)
		return 0;
	label = (Label*)node->data;
	adress = label->spot + IN_ADRESS; //Adress begin in line 100
	decaToWord(adress, word);
	word->data <<= MOVE_DST_INTO_WORD_SPOT;
	if (label->type == EXTERNAL) {
		fprintf(file, "%s\t", label->name);
		adress = printLine+1;
		decaToWord(adress, &wordExternal);
		printWordInBase32(&wordExternal,file);
		fprintf(file, "\n");
	}
	return 1;
}

/*this method returning ENUM of the instruction operand that been giving as a 
string parameter */
Instruction_Op isValidOP(char* inst) {
	int i;
	for (i = 0; i < INSTRUCTIONS_SIZE; ++i)
		if (!strcmp(inst, INSTRUCTIONS[i]))
			return (Instruction_Op)i;
	return INVALID_OP;
}

/*this method update the a binary word of the given instruction operand
incase its a real operand!*/
void wordByOp(Word* w, const Instruction_Op op)
{
	if (op != INVALID_OP)
	{
		wordCpy(w, &operWord[op]);
	}
}

/*this method ment to check if the string is pointing out to any type of
operand. if yes, the method will return the ENUM of the type of the operand 
if its not a real operand,the method returns ENUM of the invalid operand!*/
TypeOfOperand isValidOperand(char* op)
{
	int i, len = (int)strlen(op);
	char* DELIMITER = ".";
	char* word = NULL;
	char temp[MAX_LEN_INSTRUCTION];
	strcpy(temp, op);
	if (op[0] == '#') //case immidate number (value)
	{
		i = 1;
		if (op[i] == '+' || op[i] == '-')
			i += 1;

		while (i < len - 1)
		{
			if (!isdigit(op[i]))
				return INVALID_TYPE;
			++i;
		}
		return NUM_TYPE;
	}
	else if (isValidReg(op)) //case register
	{
		return  REGISTER_TYPE;
	}

	//case label
	if (!isupper(op[0]))
		return INVALID_TYPE;
	word = strtok(temp, DELIMITER);
	word = strtok(NULL, DELIMITER);
	if (word == NULL)
		return LABEL_TYPE;

	if ( (*word != '1'&& *word != '2') || strlen(word)!=1)
		return INVALID_TYPE;

	word = strtok(NULL, DELIMITER);
	if (word != NULL)
		return INVALID_TYPE;

	return STRUCT_TYPE;
}

//this method checks if the given string is points to register.
//i.e if the string is "r1" => the method will return 1;
//if the string is "r10" or "b__" => the method will return 0;
int isValidReg(char* Op)
{
	if (strlen(Op) != REGISTER_NAME_LEN-1)
		return 0;
	if (Op[0] != 'r')
		return 0;
	if (Op[1] < '0' || Op[1] > '7')
		return 0;
	return 1;
}

//this method returns the instruction group by the useing the instruction operand that 
//is given as a parameter!
InsGroup getInsGroupByInsOp(Instruction_Op insOp)
{
	switch (insOp)
	{
	case MOV:
	case CMP:
	case ADD:
	case SUB:
	case LEA:
		return TWO_OPERANDS;
		break;
	case NOT:
	case CLR:
	case INC:
	case DEC:
	case JMP:
	case BNE:
	case GET:
	case PRN:
	case JSR:
		return ONE_OPERAND;
	case RTS:
	case HLT:
		return NONE;
		break;
	default:
		printf("%s", INVALID_INST_EXEPTION);
		exit(INVALID_INST_EXIT);
		break;
	}
}
