#include "mos6502.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
//hash table to look up opcodes, small init cost much simple and faster search
op opHash[128];
int addroffset;
inline void copyOp(op *to, op *from)
{
    memcpy(to, from, sizeof(op));
}

void disopCtor(byte* in,disop* newDisop)
{
	newDisop->opData = opHash[(byte)*in];
	newDisop->mode = findMode(newDisop, in);
	newDisop->operandCnt = oplen[newDisop->opData.opcodes[newDisop->mode].addr_mode] -1;
	newDisop->addr = (int)in - addroffset;
	getOpStr(newDisop);
	getOperands(newDisop, in);
}

void disopDtor(disop* in)
{
	free(in);
}

void getOpStr(disop* newDisop)
{
	switch (newDisop->operandCnt)
	{
		case 0:
			sprintf(newDisop->str,opfmt[newDisop->mode],newDisop->opData.name);	
			break;
		case 1:
			sprintf(newDisop->str,opfmt[newDisop->mode],newDisop->opData.name,newDisop->operands[0]);
			break;
		case 2:
			sprintf(newDisop->str,opfmt[newDisop->mode],newDisop->opData.name,newDisop->operands[0],newDisop->operands[1]);
			break;
	}
		
}

void getOperands(disop* find, byte* code)
{
	switch (find->operandCnt)
	{
		case 0:
			break;
		case 1:
			find->operands[0] = *(code +1);
			break;
		case 2:
			find->operands[0] = *(code +1);
			find->operands[1] = *(code +2);
			break;
	}
	return;
}

short findMode(disop* find, byte* opcode)
{
	for(short i = 0; i < find->opData.modecnt; i++)
	{
		if(find->opData.opcodes[i].code == (byte) *opcode)
		{
			return i;
		}
	}
	return -1;
}

void prepOpHash(op opcodes[])
{
    //copy op codes to new table that can be addressed by code
    for (int i = 0; i < MOS6502_TOTAL_INSTRUCTIONS - 1; i++) {
        for (int j = 0; j < opcodes[i].modecnt; j++) {
            copyOp(&opHash[opcodes[i].opcodes[j].code], &opcodes[i]);
        }
    }
}

byte* nextop(byte* code,disop* lastop)
{
	code = code + lastop->operandCnt + 1;
	return code;
}

disop* mos6502dis(long long addr,byte* in,int len)
{
	byte* code = in;
	disop* disops = malloc(sizeof(disop)*len);
	int cnt =0 ;
	int idx = 0;
	while(code && (cnt < len))
	{
		disopCtor(in,&disops[idx]);
		code = nextop(code, &disops[idx]);
		idx++;
		cnt++;
	}
	return disops;
}

