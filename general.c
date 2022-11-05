#define _CRT_SECURE_NO_WARNINGS
#include "general.h"
#include <string.h>


void removeSpaceFromStart(char** str) {
	//removes the string's pointer until to the first char that isn't any kind of spaces
	while (*(*str) == ' ' || *(*str) == '\t') {
		*str = *str + 1;
	}
}

void removeSpaceFromEnd(char* str) {
	int i;
	int strLength = strlen(str) - 1;
	if (str[strLength] == ' ' || str[strLength] == '\t') {//checkes if there is a need to move from the end the pointer
		for (i = strLength; i >= 0; --i) {
			if (!(str[i] == ' ' || str[i] == '\t')) {
				str[i + 1] = '\0';
				break;
			}
		}
	}
}
