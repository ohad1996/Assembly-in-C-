#ifndef __BASE__
#define __BASE__
#define BASE_LEN 32
#include "word.h"
#include <stdio.h>

/*this file ment for to keep all the functions that translate numbers to our special 32 base*/

int getInDeca5LowerBits(const Word* w);
char getSymble(const int num);
void printWordInBase32(const Word*, FILE*);

#endif
