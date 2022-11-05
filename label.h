#ifndef __LABEL__
#define __LABEL__
#define LABEL_SIZE 31


typedef enum {
	EXTERNAL,ENTERY,DATA,CODE
}labelType;

typedef struct {
	char name[LABEL_SIZE];
	int spot;
	labelType type;
}Label;


void initLabel(Label* lb, const char* name, int spot, labelType type);

int compareLable(const void* a, const void* b);
void printLable(const void* m);

#endif
