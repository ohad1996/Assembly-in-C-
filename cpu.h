#ifndef __CPU__
#define __CPU__

#include "mem.h"
#include "reg.h"
#include "linkedList.h"

/*this file ment for to keep our memory of all the demanded kinds : simbles, memories, registers*/

typedef struct {
	Memory mem;
	RegisterArr regs;
	List simbleTable;
}CPU;
#endif
