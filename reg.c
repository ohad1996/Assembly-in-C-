#define _CRT_SECURE_NO_WARNINGS
#include "reg.h"
#include "exitsitu.h"
#include <stdio.h>
#include <stdlib.h>

//this method initiate all the registers and enter them into an array.
void initRegs(RegisterArr* regArr)
{
	if (regArr == NULL) {
		printf("%s", NULL_PTR_EXEPTION);
		exit(NULL_PTR_EXIT);
	}
	int i;
	for (i = 0; i < REGISTER_SIZE; ++i)
	{
		initWord(&regArr->regArr[i].r);
		regArr->regArr[i].name[0] = 'r';
		regArr->regArr[i].name[1] = i + '0';
	}
}

//this method sets the word that kept in the register that is in the regArr in the given index.
void setWord(RegisterArr* regArr, const int regIndex, const Word* word)
{
	if (regIndex < 0 || regIndex >= REGISTER_SIZE)//checks validation of the given index..
	{
		printf("%s", INVALID_LABEL_EXEPTION);
		exit(INVALID_LABEL_EXIT);
	}
	else
		wordCpy(&regArr->regArr[regIndex].r, word);
}
