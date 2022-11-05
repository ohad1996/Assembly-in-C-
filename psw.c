#define _CRT_SECURE_NO_WARNINGS
#include "psw.h"
#include <stdio.h>

//this method initialize all the flags that needed from the psw.
void initPSW(PSW* psw) {
	psw->macroFlag &= 0b0;
	psw->labelFlag &= 0b0;
	psw->errorFlag &= 0b0;
	psw->entryFlag &= 0b0;
	psw->externFlag &= 0b0;
}

//this method changes the flag of the macro.
void changeFlagMac(PSW* psw) {
	psw->macroFlag = !psw->macroFlag;
}

//this method changes the flag of the label.
void changeFlagLabel(PSW* psw) {
	psw->labelFlag = !psw->labelFlag;
}
