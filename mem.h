#ifndef __MEM__
#define __MEM__
#define MEMORY_SIZE 156
#include "word.h"

/*this file ment for holding the memories for any kind of data or instructions that 
been holds in a binary word.
*/
typedef struct {
	Word dataMem[MEMORY_SIZE];
	Word instMem[MEMORY_SIZE];
}Memory;


void initMem(Memory* mem);
void extractMem(Memory* mem, const int beginAdress, Word* word);

#endif
