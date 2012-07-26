#ifndef MOS6502_DISASM_H
#define MOS6502_DISASM_H
#endif
/* Total number of assembly instructions, Maximum number of operands */
#define MOS6502_TOTAL_INSTRUCTIONS	56
#define MOS6502_MAX_NUM_OPERANDS	2
#define MOS6502_TOTAL_MODES           16
#include <stdint.h>
int mos6502dis (char *out, unsigned long long addr, const unsigned char *buf, int len);
#define byte char
//all opcodes are the same width reletive to addressing mode
typedef struct {
	char name[4];
	byte opcode[MOS6502_TOTAL_MODES]; //opcode
	short addr_mode; //addresingmodeP
} typedef op;



#define AM_NONE		 0x1
#define AM_ACCUM     0x2
#define AM_ABS 		 0x4
#define AM_ABS_XIDX  0x8
#define AM_ABS_YIDX  0x10
#define AM_IMM       0x20
#define AM_IMP       0x40
#define AM_IND	     0x80
#define AM_IND_X     0x100
#define AM_IND_Y     0x200
#define AM_REL       0x400
#define AM_ZPG       0x800
#define AM_ZPG_X     0x1000
#define AM_ZPG_Y     0x2000

/* Enumeration for all types of AVR Operands */
enum MOS6502_Addr_Mode {
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
};



static op mos6502ops[]= {
	{
		"ADC", 
		{0x69,0x65,0x75,0x6d,0x7D,0x79,0x61,0x71},
		(AM_IMM | AM_ZPG | AM_ZPG| AM_ZPG_X|AM_ABS|AM_ABS_XIDX|AM_ABS_YIDX|AM_IND_X|AM_IND_Y)
	},
	{
		"AND",
		{0x29,0x25,0x35,0x2D,0x3d,0x39,0x21,0x31},
		(AM_IMM | AM_ZPG | AM_ZPG| AM_ZPG_X|AM_ABS|AM_ABS_XIDX|AM_ABS_YIDX|AM_IND_X|AM_IND_Y)	
	},
	{
		"ASL",
		{0x0a,0x06,0x16,0x0e,0x1e},
		(AM_ACCUM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX)
	},
	{
		"BCC",
		{0x90},
		(AM_REL)
	},
	{
		"BCS",
		{0xB0},
		(AM_REL)
	},
	{
		"BEQ",
		{0xF0},
		(AM_REL)
	},
	{
		"BIT",
		{0x24,0x2C},
		(AM_ZPG|AM_ABS)
	},
	{
		"BMI",
		{0x30},
		(AM_REL)
	},
	{
		"BNE",
		{0xD0},
		(AM_REL)
	},
	{
		"BPL",
		{0x10},
		(AM_REL)
	},
	{
		"BRK",
		{0x00},
		(AM_IMP)
	},
	{
		"BVC",
		{0x50},
		(AM_REL)
	},
	{
		"BVS",
		{0x70},
		(AM_REL)
	},
	{
		"CLC",
		{0x18},
		(AM_IMP)
	},
	{
		"CLD",
		{0xD8},
		(AM_IMP)
	},
	{
		"CLI",
		{0x58},
		(AM_IMP)
	},
	{
		"CLV",
		{0xB8},
		(AM_IMP)
	},
	{
		"CMP",
		{0xC9,0xC5,0xD5,0xCD,0xDD,0xD9,0xC1,0xD1},
		(AM_IMM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX|AM_ABS_YIDX|AM_IND_X|AM_IND_Y)
	},
	{
		"CPX",
		{0xE0,0xE4,0xEC},
		(AM_IMM|AM_ZPG|AM_ABS)
	},
	{
		"CPY",
		{0xc0,0xc4,0xcc},
		(AM_IMM|AM_ZPG|AM_ABS)
	},
	{
		"DEC",
		{0xc6,0xd6,0xce,0xde},
		(AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX)
	},
	{
		"DEX",
		{0xCA},
		(AM_IMP)
	},
	{
		"DEY",
		{0x88},
		(AM_IMP)
	},
	{
		"EOR",
		{0x49,0x45,0x55,0x4d,0x5d,0x59,0x41,0x51},
		(AM_IMM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX|AM_ABS_YIDX|AM_IND_X|AM_IND_Y)
	},
	{
		"INC",
		{0xE6,0xF6,0xEE,0xFE},
		(AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX)
	},
	{
		"INX",
		{0xE8},
		(AM_IMP)
	},
	{
		"INY",
		{0xC8},
		(AM_IMP)
	},
	{
		"JMP",
		{0x4C,0x6c},
		(AM_ABS|AM_IND)
	},
	{
		"JSR",
		{0x20},
		(AM_ABS)
	},
	{
		"LDA",
		{0xA9,0xA5,0xB5,0xAD,0xBD,0xB9,0xA1,0xB1},
		(AM_IMM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX|AM_ABS_YIDX|AM_IND_Y|AM_IND_Y)
	},
	{
		"LDX",
		{0xA2,0xA6,0xB6,0xAE,0xBE},
		(AM_IMM|AM_ZPG|AM_ZPG_Y|AM_ABS|AM_ABS_YIDX)
	},
	{
		"LDY",
		{0xA0,0xa4,0xb4,0xac,0xbc},
		(AM_IMM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_YIDX)
	},
	{
		"LSR",
		{0x4A,0x46,0x56,0x4e,0x5e},
		(AM_ACCUM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX)
	},
	{
		"NOP",
		{0xEA},
		(AM_IMP)
	},
	{
		"ORA",
		{0x09,0x05,0x15,0x0d,0x1d,0x19,0x01,0x11},
		(AM_IMM|AM_ZPG|AM_ZPG|AM_ABS|AM_ABS_XIDX|AM_IND_X|AM_IND_Y)
	},
	{
		"PHA",
		{0x48},
		(AM_IMP)
	},
	{
		"PHP",
		{0x08},
		(AM_IMP)
	},
	{
		"PLA",
		{0x68},
		(AM_IMP)
	},
	{
		"PLP",
		{0x28},
		(AM_IMP)
	},
	{
		"ROL",
		{0x2A,0x26,0x36,0x2e,0x3e},
		(AM_ACCUM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX)
	},
	{
		"ROR",
		{0x6a,0x66,0x76,0x6e,0x7e},
		(AM_ACCUM|AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX)
	},
	{
		"RTI",
		{0x40},
		(AM_IMP)
	},
	{
		"RTS",
		{0x60},
		(AM_IMP)
	},
	{
		"SEC",
		{0x38},
		(AM_IMP)
	},
	{
		"SED",
		{0xF8},
		(AM_IMP)
	},
	{
		"SEI",
		{0x78},
		(AM_IMP)
	},
	{
		"STA",
		{0x85,0x95,0x8d,0x9d,0x99,0x81,0x91},
		(AM_ZPG|AM_ZPG_X|AM_ABS|AM_ABS_XIDX|AM_ABS_YIDX|AM_IND_X|AM_IND_Y)
	},
	{
		"STX",
		{0x86,0x96,0x8e},
		(AM_ZPG|AM_ZPG_Y|AM_ABS)
	},
	{
		"STY",
		{0x84,0x94,0x8c},
		(AM_ZPG|AM_ZPG_X|AM_ABS)
	},
	{
		"TAX",
		{0xAA},
		(AM_IMP)
	},
	{
		"TAY",
		{0xA8},
		(AM_IMP)
	},
	{
		"TSX",
		{0xBA},
		(AM_IMP)
	},
	{
		"TXA",
		{0x8A},
		(AM_IMP)
	},
	{
		"TXS",
		{0x9a},
		(AM_IMP)
	},
	{
		"TYA",
		{0x98},
		(AM_IMP)
	}
};	
