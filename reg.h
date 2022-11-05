#ifndef __REG__
#define __REG__

#define REGISTER_NAME_LEN 3
#define REGISTER_DATA_LEN 10
#define REGISTER_SIZE 8

#include "word.h"

typedef struct {
	char name[REGISTER_NAME_LEN];
	Word r;

}Register;

typedef struct {
	Register regArr[REGISTER_SIZE];
}RegisterArr;

void initRegs(RegisterArr*);
void setWord(RegisterArr*,const int, const Word*);
#endif
