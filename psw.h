#ifndef __PSW__
#define __PSW__

typedef struct {
	unsigned macroFlag : 1;
	unsigned labelFlag : 1;
	unsigned errorFlag : 1;
	unsigned externFlag : 1;
	unsigned entryFlag : 1;
	
}PSW;

void initPSW(PSW* psw);
void changeFlagMac(PSW* psw);
void changeFlagLabel(PSW* psw);

#endif
