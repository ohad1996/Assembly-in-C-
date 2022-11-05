#define _CRT_SECURE_NO_WARNINGS
#include "word.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "exitsitu.h"

//this method recived a word with a binary number 
//and returns this number in a base of 10 (decemal number)
int wordToDeca(const Word* w) {
	int base = 1;
	int i, res = 0;
	Word mask = { 0b0,0b0 };
	mask.data |= w->data;

	for (i = 0; i < WORD_BIT_SIZE - 1; i++) {
		res += ((mask.data) & 0b1) * base;
		base *= 2;
		mask.data >>= 1;
	}
	if (mask.data)
	{

		res = -(int)pow(2, 9) + res;
	}
	return res;
}

//this method return the decimal number as a binary number.
void decaToWord(unsigned int deca, Word* ptr) {
	initWord(ptr);
	ptr->data = deca;
}

//this method initialize a word
void initWord(Word* w) {
	if (w == NULL) {
		printf("%s\n", NULL_PTR_EXEPTION);
		exit(NULL_PTR_EXIT);
	}
	w->data &= 0b0;
	w->used &= 0b0;
}

void changeLock(Word* w) {
	w->used = !(w->used);
}

void wordCpy(Word* dst, const Word* src)
{
	initWord(dst);
	dst->data |= src->data;
	dst->used |= src->used;
}

//this method adds a word (src) to the other given word as a parameter(dst) 
void addWord(Word* dst, const Word* src) {
	int dstDec, srcDec;
	Word temp;
	initWord(&temp);
	dstDec = wordToDeca(dst);
	srcDec = wordToDeca(src);
	dstDec += srcDec;

	decaToWord(dstDec,&temp);
	wordCpy(dst, &temp);
}

//this method comapares between two words!
int wordCmp(const Word* src, const Word* dst)
{
	int i;
	Word srcMask;
	Word dstMask;
	initWord(&srcMask);
	initWord(&dstMask);
	wordCpy(&srcMask, src);
	wordCpy(&dstMask, dst);
	for (i = 0; i < WORD_BIT_SIZE; ++i)
	{
		if ((((srcMask.data) << 9) >> 9) > (((dstMask.data) << 9) >> 9))
			return -1;
		else if  ((((srcMask.data) << 9) >> 9) < (((dstMask.data) << 9) >> 9))
			return 1;
		srcMask.data >>= 1;
		dstMask.data >>= 1;
	}
	return 0;
}
