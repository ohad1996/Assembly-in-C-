#define _CRT_SECURE_NO_WARNING
#include "exitsitu.h"

#include <stdlib.h>
#include "word.h"
#include "base.h"


const char simbles[] =
{'!', '@','#','$','%','^',
'&','*','<','>', 'a', 'b',
'c', 'd', 'e', 'f', 'g', 'h',
'i', 'j' ,'k' ,'l' , 'm', 'n',
'o' , 'p','q','r','s','t','u','v'};

//Return value from 0 to 31
int getInDeca5LowerBits(const Word* w) {
	int base=1;
	int i, res=0;
	Word mask = { 0b0,0b0 };
	mask.data |= w->data;
	for (i=0;i< WORD_BIT_SIZE/2;i++) {
		res += ((mask.data)&0b1)*base;
		base *= 2;
		mask.data >>= 1;
	}
	return res;
}

//this method returns the symble that belongs to the base 32
char getSymble(const int num) {
	if (num<0 || num>=BASE_LEN) {
		printf("%s", BASE_EXEPTION);
		exit(1);
	}
	else {
		return simbles[num];
	}
}

//Print word in special base i.e given word 'a' (97) => $ #
void printWordInBase32(const Word* w,FILE* file)
{
	char higher, lower;
	Word mask;
	initWord(&mask);
	mask.data |= ((w->data) >> 5); //higher bits
	higher = getSymble(getInDeca5LowerBits(&mask));
	fprintf(file, "%c", higher);
	initWord(&mask);
	mask.data |= w->data; //higher bits
	mask.data <<= 5;
	mask.data >>= 5;
	lower = getSymble(getInDeca5LowerBits(&mask));
	fprintf(file, "%c", lower);

}
