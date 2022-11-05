#ifndef __INSTRUCTION__
#define __INSTRUCTION__


#define INSTRUCTIONS_SIZE 16
#define INSTRUCTION_MAX_LEN 3
#define INSTRUCTION_MAX_PARTS_SIZE 3
#define MAX_LEN_INSTRUCTION 100
#define MOVE_DST_INTO_WORD_SPOT 2
#define MOVE_SRC_INTO_WORD_SPOT 6
#include "word.h"
#include "linkedList.h"

/*this file designed to keep the functions that decode
 the words from the lines of the given file from FT & ST.
(FT-First transition)
(ST-Second transition) */

typedef enum {
	NUM_REG_TYPE, STRUCT_LABEL_TYPE, EXTERN_ARE_TYPE
}TypeOfARE;

typedef enum {
	NUM_TYPE, STRUCT_TYPE ,LABEL_TYPE,REGISTER_TYPE, INVALID_TYPE
}TypeOfOperand;

typedef enum {
	MOV,CMP,ADD,SUB ,LEA,NOT,CLR,INC,DEC,JMP,BNE,GET,PRN,JSR,RTS,HLT,INVALID_OP
}Instruction_Op;

typedef enum {
	TWO_OPERANDS,ONE_OPERAND,NONE, INVALID_INS_GROUP
}InsGroup;


int isEmptyLine(char* str);
void updateInstructionFT(Word* words, int* size, Instruction_Op op, InsGroup IG, TypeOfOperand left, TypeOfOperand right);
void updateDstFT(Word* words, TypeOfOperand right);
void updateSrcFT(Word* words, TypeOfOperand left);
int updateInstructionST(List* lst, FILE* file, Word* words, int* size, int curLine, int printLine, Instruction_Op op, InsGroup IG, char* src, TypeOfOperand left, char* dst, TypeOfOperand right);
int updateDstST(FILE* file, List* lst, Word* words, char* dst, TypeOfOperand right, int isReset, int curLine, int printLine);
int updateSrcST(FILE* file, List* lst, Word* words, char* dst, TypeOfOperand right, int curLine, int printLine);
int findSymbleLocation(FILE* file, List* lst, Word* word, char* symbName, int curLine, int printLine);
Instruction_Op isValidOP(char* inst);

void wordByOp(Word* w, const Instruction_Op op);
TypeOfOperand isValidOperand(char* op);
int isValidReg(char* Op);

InsGroup getInsGroupByInsOp(Instruction_Op insOp);



#endif
