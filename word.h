#ifndef __WORD__
#define __WORD__
#define WORD_BIT_SIZE 10
#define WORD_SIZE 5

/*this file holds all the functions that relevant only to words*/
typedef struct {
	signed data : WORD_BIT_SIZE;
	unsigned used : 1;
}Word;

void initWord(Word*);
void changeLock(Word*);
void wordCpy(Word*, const Word*);
void addWord(Word*, const Word*);
int wordToDeca(const Word* w);
int wordCmp(const Word* src, const Word* dst);
void decaToWord(unsigned int deca, Word* ptr);

#endif
