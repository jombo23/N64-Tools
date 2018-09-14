#include "stdafx.h"
#include "erzdec.h"
/*
 mipsdis.c
 redistribute and modify at will, my name is spinout, keep it here.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "r3400i.h"

#define BSWAP32(x)	(unsigned long)((x & 0xFF000000) >> 24 | (x & 0xFF0000) >> 8 | (x & 0xFF00) << 8 | (x & 0xFF) << 24)

#define U32(x)  ((unsigned long)( (((unsigned char*)(x))[0]<<24)|(((unsigned char*)(x))[1]<<16)|(((unsigned char*)(x))[2]<<8)|((unsigned char*)(x))[3] ))
#define U16(x)  ((unsigned short)( ((*((unsigned char*)(x)))<<8)|((unsigned char*)(x))[1] ))
#define U8(x)	((unsigned char)((unsigned char*)(x))[0])
#define S32(x)  ((signed long)(U32(x)))
#define S16(x)  ((signed short)(U16(x)))
#define setU8(x,val)                   \
{                                       \
        (x)[0] = (val) & 0xFF;          \
}
#define setU32(x,val)                   \
{                                       \
        (x)[0] = ((val)>>24) & 0xFF;    \
        (x)[1] = ((val)>>16) & 0xFF;    \
        (x)[2] = ((val)>>8) & 0xFF;     \
        (x)[3] = (val) & 0xFF;          \
}
#define setU16(x,val)                   \
{                                       \
        (x)[0] = ((val)>>8) & 0xFF;     \
        (x)[1] = (val) & 0xFF;          \
}
typedef void (*Handler)( unsigned long word );
void run_op(unsigned long word);
unsigned char * buff;

signed long regs[32];
unsigned long pc;

void
do_branch(unsigned long word)
{
    unsigned char *dslot = buff + (pc & 0xFFFFFF) + 4;
    run_op( U32(dslot) );
    pc += getOFFSET(word);
}

void NONE(unsigned long word)
{
    printf("\a(Invalid opcode: %08X @ %08X)\n", word, pc);
    pc+=4;
}

void BLTZ(unsigned long word)
{
    if(regs[getRS(word)] < 0)do_branch(word);
    else pc+=4;
}

void BGEZ(unsigned long word)
{
    if(regs[getRS(word)] >= 0)do_branch(word);
    else pc+=4;
}

Handler REGIMM_T[32] = {
BLTZ,	BGEZ,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE
};

//SPECIAL op types
void SLL(unsigned long word)
{		//00 (00)
    if (word)
        regs[getRD(word)] = (((unsigned long)regs[getRT(word)] << (unsigned long)getSA(word)) & 0xFFFFFFFF);
    pc+=4;
}
void SRL(unsigned long word)
{		//02 (02)
    regs[getRD(word)] = (((unsigned long)regs[getRT(word)] >> (unsigned long)getSA(word)) & 0xFFFFFFFF);
    pc+=4;
}

void SLLV(unsigned long word)
{		//04 (04)
    regs[getRD(word)] = (((unsigned long)regs[getRT(word)] << (unsigned long)regs[getRS(word)]) & 0xFFFFFFFF);
    pc+=4;
}

void SRLV(unsigned long word)
{		//06 (06)
    regs[getRD(word)] = (((unsigned long)regs[getRT(word)] >> (unsigned long)regs[getRS(word)]) & 0xFFFFFFFF);
    pc+=4;
}

void JR(unsigned long word)
{		//08 (08)
    unsigned char *dslot = buff + (pc & 0xFFFFFF) + 4;
    run_op( U32(dslot) );
    pc = regs[getRS(word)];
}

void JALR(unsigned long word)
{		//08 (08)
    unsigned char *dslot = buff + (pc & 0xFFFFFF) + 4;
    run_op( U32(dslot) );
    regs[getRD(word)] = pc + 8;
    pc = regs[getRS(word)];
}

void ADD(unsigned long word)
{		//32 (20)
    regs[getRD(word)] = (signed long)regs[getRS(word)] + (signed long)regs[getRT(word)];
    pc+=4;
}

void ADDU(unsigned long word)
{		//33 (21)
    regs[getRD(word)] = (signed long)regs[getRS(word)] + (signed long)regs[getRT(word)];
    pc+=4;
}

void SUB(unsigned long word)
{		//34 (22)
    regs[getRD(word)] = (signed long)regs[getRS(word)] - (signed long)regs[getRT(word)];
    pc+=4;
}

void SUBU(unsigned long word)
{		//35 (23)
    regs[getRD(word)] = (unsigned long)regs[getRS(word)] - (signed long)regs[getRT(word)];
    pc+=4;
}

void AND(unsigned long word)
{		//36 (24)
    regs[getRD(word)] = regs[getRS(word)] & regs[getRT(word)];
    pc+=4;
}

void OR(unsigned long word)
{		//37 (25)
    regs[getRD(word)] = regs[getRS(word)] | regs[getRT(word)];
    pc+=4;
}

void XOR(unsigned long word)
{		//37 (25)
    regs[getRD(word)] = regs[getRS(word)] ^ regs[getRT(word)];
    pc+=4;
}

void NOR(unsigned long word)
{		//39 (27)
    regs[getRD(word)] = ~(regs[getRS(word)] | regs[getRT(word)]);
    pc+=4;
}

Handler SPECIAL_T[64] = {
SLL,	NONE,	SRL,	NONE,	SLLV,	NONE,	SRLV,	NONE,
JR,	JALR,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
ADD,	ADDU,	SUB,	SUBU,	AND,	OR,	XOR,	NOR,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE
};

//Main op types
void SPECIAL(unsigned long word)
{	//00(00)
    SPECIAL_T[word & 63](word);
}

void REGIMM(unsigned long word)
{	//01(01)
    REGIMM_T[getRT(word)](word);
}

void J(unsigned long word)
{		//02(02)
    unsigned char *dslot = buff + (pc & 0xFFFFFF) +4;
    run_op( U32(dslot) );
    pc = getTARGET(word)|0x80000000;
}

void JAL(unsigned long word)
{		//03(03)
    unsigned char *dslot = buff + (pc & 0xFFFFFF) + 4;
    regs[31] = pc + 8;
    run_op( U32(dslot) );
    pc = getTARGET(word)|0x80000000;
}

void BEQ(unsigned long word)
{		//04(04)
    if(regs[getRS(word)] == regs[getRT(word)])
        do_branch(word);
    else pc+=4;
}

void BNE(unsigned long word)
{		//05(05)
    if(regs[getRS(word)] != regs[getRT(word)])
        do_branch(word);
    else pc+=4;
}

void ADDI(unsigned long word)
{		//08(08)
    regs[getRT(word)] = regs[getRS(word)] + (signed short)getIMM(word);
    pc+=4;
}

void ADDIU(unsigned long word)
{	//09(09)
    regs[getRT(word)] = regs[getRS(word)] + (signed short)getIMM(word);
    pc+=4;
}

void ANDI(unsigned long word)
{		//12(0C)
    regs[getRT(word)] = regs[getRS(word)] & getIMM(word);
    pc+=4;
}

void ORI(unsigned long word)
{
    regs[getRT(word)] = regs[getRS(word)] | getIMM(word);
    pc+=4;
}

void LUI(unsigned long word)
{
    regs[getRT(word)] = ((signed short)getIMM(word)) << 16;
    pc+=4;
}

void LH(unsigned long word)
{
    unsigned addr = (signed short)getIMM(word) + regs[getBASE(word)];
    regs[getRT(word)] = S16(buff + (addr & 0xFFFFFF));
    pc+=4;
}

void LW(unsigned long word)
{		//35(23)
    unsigned addr = (signed short)getIMM(word) + regs[getBASE(word)];
    regs[getRT(word)] = S32(buff + (addr & 0xFFFFFF));
    pc+=4;
}

void LBU(unsigned long word)
{		//36(24)
    unsigned addr = (signed short)getIMM(word) + regs[getBASE(word)];
    regs[getRT(word)] = U8(buff + (addr & 0xFFFFFF));
    pc+=4;
}

void LHU(unsigned long word)
{
    unsigned addr = (signed short)getIMM(word) + regs[getBASE(word)];
    regs[getRT(word)] = U16(buff + (addr & 0xFFFFFF));
    pc+=4;
}

void SB(unsigned long word)
{		//40(28)
    unsigned addr = (signed short)getIMM(word) + regs[getBASE(word)];
    setU8(buff + (addr&0xFFFFFF), regs[getRT(word)]);
    pc+=4;
}

void SH(unsigned long word)
{
    unsigned addr = (signed short)getIMM(word) + regs[getBASE(word)];
    setU16(buff + (addr&0xFFFFFF), regs[getRT(word)]);
    pc+=4;
}

void SW(unsigned long word)
{		//43(2B)
    unsigned char *addr = buff + (((signed short)getIMM(word) + regs[getBASE(word)]) & 0xFFFFFF);
    setU32(addr, regs[getRT(word)]);
    pc+=4;
}

Handler MAIN_T[64] =
{
SPECIAL,REGIMM, J,	JAL,	BEQ,	BNE,	NONE,	NONE,
ADDI,	ADDIU,	NONE,	NONE,	ANDI,	ORI,	NONE,	LUI,
NONE,   NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,   NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,	LH,	NONE,	LW,	LBU,	LHU,	NONE,	NONE,
SB,	SH,	NONE,	SW,	NONE,	NONE,	NONE,	NONE,
NONE,   NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
NONE,   NONE,	NONE,	NONE,	NONE,	NONE,	NONE,	NONE,
};

void
run_op(unsigned long word)
{
    MAIN_T[ word >> 26 ](word);
}


int ERZ::decode(unsigned char * src, int filesize, unsigned char* result, int& fileSizeCompressed)
{
	int length = -1;
    int wordc=0;

    unsigned return_addr;
    
    int address=0;

    buff=(unsigned char*)malloc(0x300000);
	memset(buff, 0, 0x300000);
    memcpy(&buff[0x2000], src, filesize);

	// Get the ERZ header from the buffer.
	ERZHeader *pHeader = (ERZHeader*)src;
   
	// Verify the header magic is correct.
	if (pHeader->Magic[0] != 'E' || pHeader->Magic[1] != 'R' || pHeader->Magic[2] != 'Z')
	{
		// Header signature is invalid.
		free(buff);
		return -1;
	}
 
    memset((void*)regs, 0, sizeof(regs));
    return_addr = regs[31] = 0xdeadbeef;

	// Setup parameters for decompression.
    regs[4] = 0x80002000;							// $a0 = pCompressedData
    regs[5] = 0x80100000;							// $a1 = pDecompressedData
	regs[6] = BSWAP32(pHeader->DecompressedSize);	// $a2 = dwDecompressedSize
	regs[7] = BSWAP32(pHeader->CompressedSize);		// $a3 = dwCompressedSize
    regs[29] =0x80280000;	//stack

    #include "func.h"
	memcpy(buff + (ERZ1_EP&0xFFFFFF), erz1, sizeof(erz1));
    memcpy(buff + (ERZ2_EP&0xFFFFFF), erz2, sizeof(erz2));

	// Check the compression type and handle accordingle.
	if (pHeader->CompressionType == 1)
	{
		pc = ERZ1_EP;
	}
	else if (pHeader->CompressionType == 2)
	{
		pc = ERZ2_EP;
	}
	else
	{
		free(buff);
		return -1;
	}

	for (; pc != return_addr;)
	{
		run_op(U32(buff + (pc & 0xFFFFFF)));
		regs[0] = 0;
	}

	memcpy(result, (buff + 0x100000), BSWAP32(pHeader->DecompressedSize));

	// Free temp buffer.
    free(buff);
    
	// Return the length of the decompressed data.
	return BSWAP32(pHeader->DecompressedSize);
}
