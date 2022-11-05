#define _CRT_SECURE_NO_WARNINGS
#include "mem.h"
#include "base.h"
#include "exitsitu.h"
#include <stdio.h>
#include <stdlib.h>


//this method ment for initialze the memory
void initMem(Memory* mem)
{
	int i;
	for ( i = 0; i < MEMORY_SIZE; ++i)
	{
		initWord(&mem->dataMem[i]);
		initWord(&mem->instMem[i]);
	}
}

//this method ment for extracting the data from the memory in the given of his adress.
void extractMem(Memory* mem, const int beginAdress,Word* word)
{
	if (beginAdress < 0 || beginAdress >= MEMORY_SIZE){

		printf("%s", LABEL_MEM_EXEPTION);
		exit(LABEL_MEM_EXIT);
		return;
	}

	wordCpy(word, &mem->dataMem[beginAdress]);
}
