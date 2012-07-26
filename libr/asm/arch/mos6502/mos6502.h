/* Total number of assembly instructions, Maximum number of operands */
#define MOS6502_TOTAL_INSTRUCTIONS	56
#define MOS6502_MAX_NUM_OPERANDS	2
#define MOS6502_TOTAL_MODES           14


//int mos6502dis (char *out, unsigned long long addr, const unsigned char *buf, int len);
#define byte char


/* Enumeration for all types of MOS6502 Operands */
enum {
	NONE =0,
	ACCUM = 1,
	ABS= 2, //absolute address
	ABS_XIDX=3, ABS_YIDX=4, //abs incremented by X or Y  
	IMM=5, // immidate
	IMP=6, // operand is implied
	IND=7,IND_X=8,IND_Y=9, // indirect
	REL=10, // relitave address to the current PC
	ZPG=11, ZPG_X=12, ZPG_Y=13//zeropage
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


typedef struct {
	byte code;	
	int addr_mode;
} opcode;

typedef struct {
	char name[4];
	opcode opcodes[MOS6502_TOTAL_MODES]; //opcode
	short addr_mode; //addresingmodeP
	short modecnt;
} typedef op;


static op mos6502ops[]= {
	{
		"ADC", 
		{{0x69,IMM}, {0x65,ZPG},{0x75,ZPG_X},{0x6d,ABS},{0x7D,ABS_XIDX},
		{0x79,ABS_YIDX},{0x61,IND_X},{0x71,IND_Y}},
		8
	},
	{
		"AND",
		{{0x29,IMM},{0x25,ZPG},{0x35,ZPG_X},{0x2D,ABS},
		{0x3d,ABS_XIDX},{0x39,ABS_YIDX},{0x21,IND_X},{0x31,IND_Y}},
		8
	},
	{
		"ASL",
		{{0x0a,ACCUM},{0x06,ZPG},{0x16,ZPG_X},{0x0e,ABS},{0x1e,ABS_XIDX}},
		5
	},
	{
		"BCC",
		{{0x90,REL}},
		1
	},
	{
		"BCS",
		{{0xB0,REL}},
		1
	},
	{
		"BEQ",
		{{0xF0,REL}},
		1
	},
	{
		"BIT",
		{{0x24,ZPG},{0x2C,ABS}},
		2
	},
	{
		"BMI",
		{{0x30,REL}},
		1
	},
	{
		"BNE",
		{{0xD0,REL}},
		1
	},
	{
		"BPL",
		{{0x10,REL}},
		1
	},
	{
		"BRK",
		{{0x00,IMP}},
		1
	},
	{
		"BVC",
		{{0x50,REL}},
		1
	},
	{
		"BVS",
		{{0x70,REL}},
		1
	},
	{
		"CLC",
		{{0x18,IMP}},
		1
	},
	{
		"CLD",
		{{0xD8,IMP}},
		1
	},
	{
		"CLI",
		{{0x58,IMP}},
		1
	},
	{
		"CLV",
		{{0xB8,IMP}},
		1
	},
	{
		"CMP",
		{{0xC9,IMM},{0xC5,ZPG},{0xD5,ZPG_X},{0xCD,ABS},{0xDD,ABS_XIDX},
		{0xD9,ABS_YIDX},{0xC1,IND_X},{0xD1,IND_Y}},
		8
	},
	{
		"CPX",
		{{0xE0,IMM},{0xE4,ZPG},{0xEC,ABS}},
		3
	},
	{
		"CPY",
		{{0xc0,IMM},{0xc4,ZPG},{0xcc,ABS}},
		3
	},
	{
		"DEC",
		{{0xc6,ZPG},{0xd6,ZPG_Y},{0xce,ABS},{0xde,ABS_XIDX}},
		4
	},
	{
		"DEX",
		{{0xCA,IMP}},
		1
	},
	{
		"DEY",
		{{0x88,IMP}},
		1
	},
	{
		"EOR",
		{{0x49,IMM},{0x45,ZPG},{0x55,ZPG_X},{0x4d,ABS},
		{0x5d,ABS_XIDX},{0x59,ABS_YIDX},{0x41,IND_X},{0x51,IND_Y}},
		8
	},
	{
		"INC",
		{{0xE6,ZPG},{0xF6,ZPG_X},{0xEE,ABS},{0xFE,ABS_XIDX}},
		4
	},
	{
		"INX",
		{{0xE8,IMP}},
		1
	},
	{
		"INY",
		{{0xC8,IMP}},
		1
	},
	{
		"JMP",
		{{0x4C,ABS},{0x6c,IND}},
		2
	},
	{
		"JSR",
		{{0x20,ABS}},
		1
	},
	{
		"LDA",
		{{0xA9,IMM},{0xA5,ZPG},{0xB5,ZPG_X},{0xAD,ABS},
		{0xBD,ABS_XIDX},{0xB9,ABS_YIDX},{0xA1,IND_X},{0xB1,IND_Y}},
		8
	},
	{
		"LDX",
		{{0xA2,IMM},{0xA6,ZPG},{0xB6,ZPG_Y},{0xAE,ABS},{0xBE,ABS_YIDX}},
		5
	},
	{
		"LDY",
		{{0xA0,IMM},{0xa4,ZPG},{0xb4,ZPG_Y},{0xac,ABS},{0xbc,ABS_YIDX}},
		5
	},
	{
		"LSR",
		{{0x4A,ACCUM},{0x46,ZPG},{0x56,ZPG_X},{0x4e,ABS},{0x5e,ABS_XIDX}},
		5
	},
	{
		"NOP",
		{{0xEA,IMP}},
		1
	},
	{
		"ORA",
		{{0x09,IMM},{0x05,ZPG},{0x15,ZPG_X},{0x0d,ABS},
		{0x1d,ABS_XIDX},{0x19,ABS_YIDX},{0x01,IND_X},{0x11,IND_Y}},
		8
	},
	{
		"PHA",
		{{0x48,IMP}},
		1
	},
	{
		"PHP",
		{{0x08,IMP}},
		1
	},
	{
		"PLA",
		{{0x68,IMP}},
		1
	},
	{
		"PLP",
		{{0x28,IMP}},
		1
	},
	{
		"ROL",
		{{0x2A,ACCUM},{0x26,ZPG},{0x36,ZPG_X},{0x2e,ABS},{0x3e,ABS_XIDX}},
		5
	},
	{
		"ROR",
		{{0x6a,ACCUM},{0x66,ZPG},{0x76,ZPG_X},{0x6e,ABS},{0x7e,ABS_XIDX}},
		5
	},
	{
		"RTI",
		{{0x40,IMP}},
		1
	},
	{
		"RTS",
		{{0x60,IMP}},
		1
	},
	{
		"SEB",
		{{0xE9,IMM},{0xE5,ZPG},{0xF5,ZPG_X},{0xED,ABS},
		{0xFD,ABS_XIDX},{0xF9,ABS_YIDX},{0xE1,IND_X},{0xF1,IND_Y}},
		8
	},
	{
		"SEC",
		{{0x38,IMP}},
		1
	},
	{
		"SED",
		{{0xF8,IMP}},
		1
	},
	{
		"SEI",
		{{0x78,IMP}},
		1
	},
	{
		"STA",
		{{0x85,ZPG},{0x95,ZPG_X},{0x8d,ABS},{0x9d,ABS_XIDX},
		{0x99,ABS_YIDX},{0x81,IND_X},{0x91,IND_Y}},
		8
	},
	{
		"STX",
		{{0x86,ZPG},{0x96,ZPG_Y},{0x8e,ABS}},
		3
	},
	{
		"STY",
		{{0x84,ZPG},{0x94,ZPG_X},{0x8c,ABS}},
		3
	},
	{
		"TAX",
		{{0xAA,IMP}},
		1
	},
	{
		"TAY",
		{{0xA8,IMP}},
		1
	},
	{
		"TSX",
		{{0xBA,IMP}},
		1
	},
	{
		"TXA",
		{{0x8A,IMP}},
		1
	},
	{
		"TXS",
		{{0x9a,IMP}},
		1
	},
	{
		"TYA",
		{{0x98,IMP}},
		1
	}
};	
