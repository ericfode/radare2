#ifndef MOS6502_DISASM_H
#define MOS6502_DISASM_H

#include <stdint.h>
int mos6502dis (char *out, unsigned long long addr, const unsigned char *buf, int len);


struct op

/* Total number of assembly instructions, Maximum number of operands */
#define MOS6502_TOTAL_INSTRUCTIONS	56
#define MOS6502_MAX_NUM_OPERANDS	2
#define MOS6502_MAX_MODES           16

#define OT_NONE		 B00000000000001
#define OT_ACCUM     B00000000000010
#define OT_ABS 		 B00000000000100
#define OT_ABS_XIDX  B00000000001000
#define OT_ABS_YIDX  B00000000010000
#define OT_IMM       B00000000100000
#define OT_IMP       B00000001000000
#define OT_IND	     B00000010000000
#define OT_X_IND     B00000100000000
#define OT_IND_Y     B00001000000000
#define OT_REL       B00010000000000
#define OT_ZPG       B00100000000000
#define OT_ZPG_X     B01000000000000
#define OT_ZPG_Y     B10000000000000

/* Enumeration for all types of AVR Operands */
enum MOS6502_Operand_Types {
	NONE,
	ACCUM,
	ABS, //absolute address
	ABS_XIDX, ABS_YIDX, //abs incremented by X or Y  
	IMM, // immidate
	IMP, // operand is implied
	IND,X_IND,IND_Y, // indirect
	REL, // relitave address to the current PC
	ZPG, ZPG_X, ZPG_Y//zeropage
};


//format strings for printing asm
static char* asm_fmt[] = {
	"%s", // NONE 
	"%s A", // Accum 
	"%s $%4x", //ABS 
	"%s $%4x,X", //ABS Incremented
	"%s $%4x,Y", //ABS Incremented
	"%s #$%2x", //Immediate
	"%s", //implied op
	"%s ($%4x)", //indirect
	"%s ($%2x,X)",//x_IND
	"%s ($%2x),Y", //IND_Y
	"%s $%2x", //Zeropage
	"%s $%2x,X", //zpg,X
	"%s $%2x,Y",   //zpg,y
}

