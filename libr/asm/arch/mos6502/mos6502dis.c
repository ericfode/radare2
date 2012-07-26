#include "mos6502.h"
#include "stdlib.h"
#include "string.h"

//hash table to look up opcodes, small init cost much simple and faster search
op opHash[128];

inline void copyOp(op* to, op* from)
{
	memcpy(to,from,sizeof(op));
}

void prepOpHash(op opcodes[]){
	//copy op codes to new table that can be addressed by code
	for(int i =0; i < MOS6502_TOTAL_INSTRUCTIONS -1; i++)
	{
		for(int j=0; j < opcodes[i].modecnt;j++)
		{
			copyOp(&opHash[opcodes[i].opcodes[j].code],&opcodes[i]);
		}
	}
}

