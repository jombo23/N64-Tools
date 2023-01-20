#include "StdAfx.h"
#include "StarFox64AudioDecompression.h"
#include "SharedFunctions.h"

CStarFox64AudioDecompression::CStarFox64AudioDecompression(void)
{
}

CStarFox64AudioDecompression::~CStarFox64AudioDecompression(void)
{
}

void FUN_00009384(unsigned char* RAM, unsigned long& S0, unsigned long& A1, unsigned long& A2, unsigned long& A3, unsigned long& V1, unsigned long& T0, unsigned long& T1)
{
	unsigned long T8 = V1 << 2;
	unsigned long T5 = A3 + T8;
	unsigned long A0 = S0 & 0xF;
	unsigned long S1 = 0;
	
	//80009394:	.XREF:800093E0[D],
label80009394:
	S0 = CSharedFunctions::CharArrayToShort(RAM, A1);
	A1 += 2;
	unsigned long T9 = (int)S0 >> 0xC;
label800093A0:
	//800093A0:	.XREF:800093D8[D],
	V1 = T9 & 0xF;
	T8 = V1 & 0x7;
	T9 = T8 - 4;
	unsigned long T6 = T9 << A0;
	float F6 = (float)(int)T6;
	S0 = S0 << 4;	
	unsigned long T7 = S0 & 0xFFFF;
	S1++;
	S0 = T7;
	T5 += 4;
	
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 4, F6);
	if (V1 >= 8)
		return;
	
	if (S1 != A2)
	{
		T9 = (int)S0 >> 0xC;
		goto label800093A0;
	}

	S1 = 0;
	goto label80009394;
}

void FUN_000093E8(unsigned char* RAM, unsigned long& S0, unsigned long& A1, unsigned long& A2, unsigned long& A3, unsigned long& V1, unsigned long& T0, unsigned long& T1)
{
	unsigned long T7 = V1 << 2;
	unsigned long T5 = A3 + T7;

	unsigned long S1 = 0;
	unsigned long A0 = S0 & 0xF;

	float F18, F16, F14, F12;

	unsigned long T6, T8, T9, V0;
	
	S1++;

	S0 = CSharedFunctions::CharArrayToShort(RAM, A1);
	if (S1 == T1)
		goto label80009498;

label80009404:
//80009404:	.XREF:80009490[D],
	T8 = (int)S0 >> 0xC;
	T9 = T8 & 0xF;
	T7 = T9 - (signed short)8;
	V0 = S0 << 4;
	T8 = T7 << A0;

	F18 = (float)(int)T8;
	T9 = (int)V0 >> 0xC;
	T6 = T9 & 0xF;
	
	T8 = T6 - 8;
	S0 = V0 << 4;

	T9 = T8 << A0;
	F16 = (float)(int)T9;

	T6 = (int)S0 >> 0xC;
	T7 = T6 & 0xF;

	T9 = T7 - 8;
	T8 = S0 << 4;
	T6 = T9 << A0;
	F14 = (float)(int)T6;
	T7 = (int)T8 >> 0xC;
	T8 = T7 & 0xF;
	T9 = T8 - 8;
	T6 = T9 << A0;
	F12 = (float)(int)T6;
	
	CSharedFunctions::WriteFloatToBuffer(RAM, T5, F18);
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 + 4, F16);
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 + 8, F14);
	S1++;
	A1 = A1 + 0x2;
	V1 = V1 + 0x4;
	T5 = T5 + 0x10;
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 4, F12);

	S0 = CSharedFunctions::CharArrayToShort(RAM, A1);
    if (S1 != T1)
		goto label80009404;

//80009498:	.XREF:800093FC[U],
label80009498:
	T8 = (int)S0 >> 0xC;
	T9 = T8 & 0xF;
	T7 = T9 - 8;
	V0 = S0 << 4;
	T9 = (int)V0 >> 0xC;
	T8 = T7 << A0;
	F18 = (float)(int)T8;
	T6 = T9 & 0xF;
	T8 = T6 - 8;
	S0 = V0 << 4;
	T6 = (int)S0 >> 0xC;
	T9 = T8 << A0;
	F16 = (float)(int)T9;
	T7 = T6 & 0xF;
	T9 = T7 - 8;
	T8 = S0 << 4;
	T7 = (int)T8 >> 0xC;
	T6 = T9 << A0;
	T8 = T7 & 0xF;
	F14 = (float)(int)T6;
	T9 = T8 - 8;
	T6 = T9 << A0;
	F12 = (float)(int)T6;
	A1 += 2;
	V1 += 4;
	T5 += 0x10;
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 0x10, F18);
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 0xC, F16);
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 8, F14);
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 4, F12);

	return;
}


void FUN_00009528(unsigned char* RAM, unsigned long& S0, unsigned long& A1, unsigned long& A2, unsigned long& A3, unsigned long& V1, unsigned long& T0, unsigned long& T1)
{
	unsigned long A0 = S0 & 0xF;
label8000952C:
//8000952C:	.XREF:80009598[D],
	S0 = CSharedFunctions::CharArrayToShort(RAM, A1);
	unsigned long T8 = V1 << 2;
	A1 += 2;
	unsigned long T7 = (int)S0 >> 8;
	unsigned long S1 = T7 & 0xFF;
	unsigned long T9 = S1 & 0x3F;
	unsigned long T6 = T9 - 0x20;
	T7 = T6 << A0;
	float F18 = (float)(int)T7;
	T6 = S0 & 0xFF;
	T9 = A3 + T8;
	unsigned long T5 = (int)S1 >> 0x6;
	T7 = T6 & 0x3F;
	T8 = T7 - 0x20;

	CSharedFunctions::WriteFloatToBuffer(RAM, T9, F18);
	if (T5 == 0)
		return;

	T9 = T8 << A0;
	float F6 = (float)(int)T9;
	V1 = V1 + T5;
	S0 = T6;
	T6 = V1 << 2;
	T7 = A3 + T6;
	S1 = (int)S0 >> 0x6;

	CSharedFunctions::WriteFloatToBuffer(RAM, T7, F6);
	if (S1 == 0)
		return;
	
	V1 = V1 + S1;
	goto label8000952C;
}

void FUN_000095A0(unsigned char* RAM, unsigned long& S0, unsigned long& A1, unsigned long& A2, unsigned long& A3, unsigned long& V1, unsigned long& T0, unsigned long& T1)
{
	unsigned long T8 = V1 << 2;
	unsigned long T5 = A3 + T8;
	unsigned long A0 = S0 & 0xF;
label800095AC:
	//800095AC:	.XREF:80009600[D],

	S0 = CSharedFunctions::CharArrayToShort(RAM, A1);
	A1 += 2;
	T5 += 4;
	unsigned long T9 = (int)S0 >> 8;
	unsigned long T6 = T9 & 0xFF;
	unsigned long T7 = T6 & 0x7F;
	T8 = T7 - 0x40;
	T9 = T8 << A0;
	float F10 = (float)(int)T9;
	unsigned long AT = (T6 >= 0x80);
	T6 = S0 & 0xFF;
	T7 = T6 & 0x7F;
	T8 = T7 - 0x40;
	T9 = T8 << A0;
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 4, F10);

	if (AT)
		return;
	
	float F18 = (float)(int)T9;
	T5 += 4;
	
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 - 4, F18);
	if (T6 < 0x80)
	{
		goto label800095AC;
	}

	return;
}

void FUN_00009610(unsigned char* RAM, unsigned long& S0, unsigned long& A1, unsigned long& A2, unsigned long& A3, unsigned long& V1, unsigned long& T0, unsigned long& T1)
{
	unsigned long A0 = S0 & 0xF;

	//80009614
label80009614:
	S0 = CSharedFunctions::CharArrayToShort(RAM, A1);
	unsigned long T9 = V1 << 2;
	A1 = A1 + 2;
	unsigned long T6 = S0 & 0xFFF;

	unsigned long T7 = T6 - (signed short)0x800;
	unsigned long T8 = T7 << A0;
	float F6 = (float)(int)T8;
	T6 = A3 + T9;

	unsigned long T5 = (int)S0 >> 0xC;

	CSharedFunctions::WriteFloatToBuffer(RAM, T6, F6);

	if (T5 == 0)
		return; //0x8000968C

	V1 = V1 + T5;
	goto label80009614;
}

void FUN_0000964C(unsigned char* RAM, unsigned long& S0, unsigned long& A1, unsigned long& A2, unsigned long& A3, unsigned long& V1, unsigned long& T0, unsigned long& T1)
{
	unsigned long T7 = V1 << 2;
	unsigned long T5 = A3 + T7;
	unsigned long A0 = S0 & 0xF;

label80009658:
//80009658:	.XREF:80009684[D],

	S0 = CSharedFunctions::CharArrayToShort(RAM, A1);
	A1 += 2;
	V1++;
	unsigned long T8 = S0 & 0x7FFF;

	unsigned long T9 = T8 - 0x4000;
	unsigned long T6 = T9 << A0;
	float F10 = (float)(int)T6;
	T7 = (int)S0 >> 0xF;

	CSharedFunctions::WriteFloatToBuffer(RAM, T5, F10);
	if (T0 == T7)
		return;

	T5 += 4;
	goto label80009658;
}

void FunctionCall800C564C(unsigned char* RAM, int tableChoice, unsigned long& S0, unsigned long& A1, unsigned long& A2, unsigned long& A3, unsigned long& V1, unsigned long& T0, unsigned long& T1)
{
	//param_2 unsigned short*
	//param_4 float*
	if (tableChoice == 0)
	{
		//80009384
		FUN_00009384(RAM, S0, A1, A2, A3, V1, T0, T1);
	}
	else if (tableChoice == 1)
	{
		//800093E8
		FUN_000093E8(RAM, S0, A1, A2, A3, V1, T0, T1);
	}
	else if (tableChoice == 2)
	{
		//800095A0
		FUN_000095A0(RAM, S0, A1, A2, A3, V1, T0, T1);
	}
	else if (tableChoice == 3)
	{
		//80009610
		FUN_00009610(RAM, S0, A1, A2, A3, V1, T0, T1);
	}
	else if (tableChoice == 4)
	{
		//8000964C
		FUN_0000964C(RAM, S0, A1, A2, A3, V1, T0, T1);
	}
	else if (tableChoice == 5)
	{
		//80009528
		FUN_00009528(RAM, S0, A1, A2, A3, V1, T0, T1);
	}
	else
	{
		throw;
	}
}


void FUN_000092d4(unsigned char* RAM, unsigned long A0)
{
	unsigned long param_1 = A0;

	//800092D8:	AFB2000C	SW      $s2, 0x000C($sp)	#
	//800092DC:	AFB10008	SW      $s1, 0x0008($sp)	#
	//800092E0:	AFB00004	SW      $s0, 0x0004($sp)	#
	//800092E4:	AFA40010	SW      $a0, 0x0010($sp)	# param_1

	unsigned long SP = 0x800E9690;
	unsigned long T5 = 0x80151084;
	unsigned long V0 = 0x80150C88;

	SP = SP - 0x10;
	unsigned long A1 = CSharedFunctions::CharArrayToLong(RAM, A0);

	//80009300:	.XREF:80009308[D],
label80009300:
	T5 = T5 - 4;
	CSharedFunctions::WriteFloatToBuffer(RAM, T5 + 4, 0);
	if (T5 >= V0)
		goto label80009300;

	//80009310
	V0 = (int)(signed short)CSharedFunctions::CharArrayToShort(RAM, A1);
	A1 = A1 + 4;
	unsigned long T3 = V0 << 0x10;
	V0 = (int)(signed short)CSharedFunctions::CharArrayToShort(RAM, A1 - 2);
	unsigned long A3 = 0x80150C88;
	unsigned long T2 = 0;
	unsigned long T4 = 0x100;
	unsigned long T1 = 0x10;
	unsigned long T0 = 0x1;
	unsigned long A2 = 0x4;

	T3 = T3 | V0;
	unsigned long T7 = T3 >> 0x18;
	
label80009344:
	//80009344:	.XREF:80009690[D],
	unsigned long S0 = T7 & 0xFF;
	A0 = (int)S0 >> 4;
	unsigned long T9 = T3 << 8;
	unsigned long T6 = A0 & 0xF;
	unsigned long V1 = T2;

	T3 = T9;

	//80009358:	11C000CC	BEQZ    $t6, 0x8000968C	#
	if (T6 != 0)
	{	
		T7 = T6 - 1;

		if (T7 < 6)
		{
			/*FILE* a = fopen("C:\\temp\\address.txt", "a");
			fprintf(a, "T7 %08X S0 %08X A1 %08X A2 %08X A3 %08X V1 %08X T0 %08X T1 %08X\n", T7, S0, A1, A2, A3, V1, T0, T1);
			fflush(a);
			fclose(a);*/
			//8000937C:	01E00008	JR      $t7	#
			//80009380:	00000000	NOP	#
			FunctionCall800C564C(RAM, T7, S0, A1, A2, A3, V1, T0, T1);
		}
		else
			throw;
	}

	//8000968C:	.XREF:80009358[U], .XREF:80009368[U], .XREF:800093D0[U], .XREF:80009568[U], 
	//8000968C:	.XREF:80009590[U], .XREF:800095E8[U], .XREF:8000963C[U], .XREF:8000967C[U],
	//8000968C:	254A0040	ADDIU   $t2, $t2, 0x0040	#
	//80009690:	.XREF:80009520[U], .XREF:80009608[U],
	//80009690:	554CFF2C	BNEL    $t2, $t4, 0x80009344	#
	//80009694:	000B7E02	SRL     $t7, $t3, 0x18	#
	
	//8000968C
	//Some point to next spot but add T2 themselves + 0x40
	T2 = T2 + 0x40;

	//80009690
	if (T2 != T4)
	{
		T7 = T3 >> 0x18;
		goto label80009344;
	}

	// param_1
	CSharedFunctions::WriteLongToBuffer(RAM, param_1, A1);
}


/* WARNING: Instruction at (ram,0x000091a8) overlaps instruction at (ram,0x000091a4)
    */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00008930(unsigned char* RAM, unsigned long param_1Offset, unsigned long param_2, unsigned long param_3Offset)

{
  //param_1 float *
  //param_3 float *
  unsigned long uVar1;
  unsigned long pfVar2Offset; //float *
  unsigned long pfVar3Offset; //float *
  unsigned long pfVar4Offset; //float *
  int iVar5;
  unsigned long pfVar6Offset; //float *
  unsigned long pfVar7Offset; //float *
  unsigned long pfVar8Offset; //float *
  unsigned long uVar9;
  unsigned long pfVar10Offset; //float *
  unsigned long pfVar11Offset; //float *
  unsigned long pfVar12Offset; //float *
  unsigned long pfVar13Offset; //float *
  unsigned long pfVar14Offset; //float *
  unsigned long pfVar15Offset; //float *
  unsigned long uVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  unsigned long pfVar20Offset; //float *
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  int iStack48;
  int iStack28;
  int iStack24;
  int iStack16;
  int iStack8;
  
  uVar16 = 1 << (param_2 & 0x1f);
  iStack28 = uVar16 << 1;
  uVar1 = uVar16;
  if ((int)uVar16 < 0) {
    uVar1 = uVar16 + 7;
  }
  iVar19 = ((int)uVar1 >> 3) + -1;
  if (uVar16 == 1) {
    return;
  }
  if (uVar16 == 2) {
	fVar21 = (CSharedFunctions::CharArrayToFloat(RAM, param_1Offset) - CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 1 * 4)) * CSharedFunctions::CharArrayToFloat(RAM, 0x800c563c);
	CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset, (CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 1 * 4) + CSharedFunctions::CharArrayToFloat(RAM, param_1Offset)) * CSharedFunctions::CharArrayToFloat(RAM, 0x800c563c));
    CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 1 * 4, fVar21);
    return;
  }
  if (uVar16 == 4) {
    fVar22 = CSharedFunctions::CharArrayToFloat(RAM, param_1Offset);
    fVar23 = CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 1 * 4);
	CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset, (CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 2 * 4) + fVar22) * 0.5);
    fVar21 = (CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 3 * 4) + fVar23) * 0.5;
	CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 2 * 4, (fVar22 - CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 2 * 4)) * 0.5);
    fVar22 = CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 2 * 4);
	CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 1 * 4, fVar21);
    CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 3 * 4, (fVar23 - CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 3 * 4)) * 0.5);
    CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 2 * 4, CSharedFunctions::CharArrayToFloat(RAM, param_1Offset) - fVar21);
    CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset, CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 1 * 4) + CSharedFunctions::CharArrayToFloat(RAM, param_1Offset));
    CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 3 * 4) + fVar22);
    CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 3 * 4, fVar22 - CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + 3 * 4));
    return;
  }
  pfVar10Offset = param_3Offset;
  if (uVar16 != (int)CSharedFunctions::CharArrayToFloat(RAM, param_3Offset)) {
    iVar5 = 0;
	CSharedFunctions::WriteFloatToBuffer(RAM, param_3Offset, (float)uVar16);
	fVar22 = CSharedFunctions::CharArrayToFloat(RAM, 0x800c5640) / (float)uVar16;
    pfVar11Offset = param_3Offset + iVar19 * 4;
    pfVar12Offset = pfVar11Offset + iVar19 * 4;
    pfVar14Offset = pfVar12Offset + iVar19 * 4;
    fVar21 = fVar22;
    if (0 < iVar19) {
	  throw; // TODO if used
      /*do {
        pfVar14Offset = pfVar14Offset + 1 * 4;
        pfVar12Offset = pfVar12Offset + 1 * 4;
        pfVar11Offset = pfVar11Offset + 1 * 4;
        param_3Offset = param_3Offset + 1 * 4;
        fVar23 = (float)FUN_00022970(fVar21);
		CSharedFunctions::WriteFloatToBuffer(RAM, param_3Offset, fVar23);
        fVar23 = (float)FUN_000227b0(fVar21);
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset, fVar23);
        fVar24 = fVar21 * 3.0;
        fVar23 = (float)FUN_00022970();
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar12Offset, fVar23);
        fVar23 = (float)FUN_000227b0(fVar24);
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar14Offset, fVar23);
        iVar5 = iVar5 + 1;
        fVar21 = fVar21 + fVar22;
      } while (iVar5 != iVar19);*/
    }
  }
  fVar21 = CSharedFunctions::CharArrayToFloat(RAM, 0x800c5644);
  iStack24 = 1;
  iStack8 = 0;
  if (0 < (int)(param_2 - 1)) {
    do {
      iVar5 = 1;
      iVar18 = iStack28 >> 4;
      iStack48 = iStack28;
      do {
        iVar17 = iStack28 >> 3;
        for (iVar5 = iVar5 + -1; iVar5 < (int)uVar16; iVar5 = iVar5 + iStack48) {
          pfVar14Offset = param_1Offset + iVar5 * 4;
		  CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c94, CSharedFunctions::CharArrayToFloat(RAM, pfVar14Offset));
          pfVar20Offset = pfVar14Offset + iVar17 * 4;
          pfVar11Offset = pfVar20Offset + iVar17 * 4;
          pfVar12Offset = pfVar11Offset + iVar17 * 4;
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar14Offset,  CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94));
          CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c98, CSharedFunctions::CharArrayToFloat(RAM, pfVar20Offset));
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar20Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset) + CSharedFunctions::CharArrayToFloat(RAM, pfVar20Offset));
          CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c9c, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset));
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset, ((CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c9c)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98)) - CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset));
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar12Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset) + ((CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c9c)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98)));
          if (1 < iVar17) {
            pfVar15Offset = pfVar14Offset + iVar18 * 4;
			CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c94, CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset));
            pfVar12Offset = pfVar15Offset + iVar17 * 4;
            pfVar11Offset = pfVar12Offset + iVar17 * 4;
            pfVar13Offset = pfVar11Offset + iVar17 * 4;
            CSharedFunctions::WriteFloatToBuffer(RAM, pfVar15Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94));
            CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c98, CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset));
            CSharedFunctions::WriteFloatToBuffer(RAM, pfVar12Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98));
            CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset, (CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset)) * fVar21);
            CSharedFunctions::WriteFloatToBuffer(RAM, pfVar13Offset, (CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98) - CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset)) * fVar21);
            pfVar11Offset = pfVar10Offset + iStack24 * 4;
            pfVar12Offset = pfVar11Offset + iVar19 * 4;
            pfVar13Offset = pfVar12Offset + iVar19 * 4;
            pfVar15Offset = pfVar13Offset + iVar19 * 4;
            iStack16 = 1;
            if (1 < iVar18) {
              do {
                pfVar2Offset = pfVar14Offset + iStack16 * 4;
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c94, CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset));
                pfVar3Offset = pfVar2Offset + iVar17 * 4;
                pfVar4Offset = pfVar3Offset + iVar17 * 4;
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c98, CSharedFunctions::CharArrayToFloat(RAM, pfVar3Offset));
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c9c, CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset));
                pfVar6Offset = pfVar20Offset + -iStack16 * 4;
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151ca0, CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset + iVar17 * 4));
                pfVar7Offset = pfVar6Offset + iVar17 * 4;
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151ca4, CSharedFunctions::CharArrayToFloat(RAM, pfVar6Offset));
                pfVar8Offset = pfVar7Offset + iVar17 * 4;
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151ca8, CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset));
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151cac, CSharedFunctions::CharArrayToFloat(RAM, pfVar8Offset));
                CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151cb0, CSharedFunctions::CharArrayToFloat(RAM, pfVar8Offset + iVar17 * 4));
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar2Offset, CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151c9c));
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar3Offset, CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca0));
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar4Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset) *
                          (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca0) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca8)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151cb0)) +
                          (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c9c)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca4)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151cac)) *
                          CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset));
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar4Offset + iVar17 * 4, 
                     (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c9c)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca4)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151cac)) * CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset)
                     - CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset) *
                       (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca0) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca8)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151cb0)));
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar6Offset, CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca4) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151cac));
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar7Offset, CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca8) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151cb0));
                fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset);
                pfVar12Offset = pfVar12Offset + iStack24 * 4;
                fVar23 = CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset);
                pfVar11Offset = pfVar11Offset + iStack24 * 4;
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar8Offset, (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c9c)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca4)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151cac)) *
                          fVar22 - fVar23 * (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca0) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca8)) -
                                            CSharedFunctions::CharArrayToFloat(RAM, 0x80151cb0)));
                fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset);
                pfVar13Offset = pfVar13Offset + iStack24 * 4;
                fVar23 = CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset);
                pfVar15Offset = pfVar15Offset + iStack24 * 4;
                CSharedFunctions::WriteFloatToBuffer(RAM, pfVar8Offset + iVar17 * 4, 
                     fVar22 * (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca0) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c98)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca8)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151cb0))  +
                     (((CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151c9c)) - CSharedFunctions::CharArrayToFloat(RAM, 0x80151ca4)) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151cac)) * fVar23);
                iStack16 = iStack16 + 1;
              } while (iStack16 != iVar18);
            }
          }
        }
        iVar5 = (iStack48 * 2 - (iStack28 >> 1)) + 1;
        iStack48 = iStack48 << 2;
      } while (iVar5 < (int)uVar16);
      iStack24 = iStack24 << 1;
      uVar1 = 0;
      if (0 < (int)uVar16) {
        uVar9 = uVar16 & 3;
        pfVar11Offset = param_1Offset;
        if (uVar9 != 0) {
          uVar1 = 1;
          fVar22 = CSharedFunctions::CharArrayToFloat(RAM, param_1Offset);
          pfVar12Offset = param_1Offset;
          if (uVar9 != 1) {
            do {
              fVar23 = fVar22 / fVar21;
              pfVar12Offset = pfVar11Offset + 1 * 4;
              fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset);
              uVar1 = uVar1 + 1;
			  CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset, fVar23);
              pfVar11Offset = pfVar12Offset;
            } while (uVar9 != uVar1);
          }
          pfVar11Offset = pfVar12Offset + 1 * 4;
		  CSharedFunctions::WriteFloatToBuffer(RAM, pfVar12Offset, fVar22 / fVar21);
          if (uVar1 == uVar16) goto LAB_000090c4;
        }
        fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset);
        while (uVar1 = uVar1 + 4, uVar1 != uVar16) {
          fVar23 = fVar22 / fVar21;
          fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset + 4 * 4);
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset, fVar23);
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset + 1 * 4) / fVar21);
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset + 2 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset + 2 * 4) / fVar21);
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset + 3 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset + 3 * 4) / fVar21);
          pfVar11Offset = pfVar11Offset + 4 * 4;
        }
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset, fVar22 / fVar21);
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset + 1 * 4) / fVar21);
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset + 2 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset + 2 * 4) / fVar21);
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset + 3 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset + 3 * 4) / fVar21);
      }
LAB_000090c4:
      iStack8 = iStack8 + 1;
      iStack28 = iStack28 >> 1;
    } while (iStack8 != param_2 - 1);
  }
  fVar21 = CSharedFunctions::CharArrayToFloat(RAM, 0x800c5648);
  iVar19 = 1;
  iVar18 = 4;
  iVar5 = 1;
  do {
    for (iVar19 = iVar19 + -1; iVar19 < (int)uVar16; iVar19 = iVar19 + iVar18) {
      pfVar10Offset = param_1Offset + iVar19 * 4;
	  CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c94, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 1 * 4) + CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94) - CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 1 * 4));
    }
    iVar19 = iVar18 * 2 + -1;
    iVar18 = iVar18 << 2;
  } while (iVar19 < (int)uVar16);
  if (0 < (int)uVar16) {
    uVar1 = uVar16 & 3;
    if (uVar1 == 0) {
      uVar1 = 4;
      pfVar10Offset = param_1Offset;
    }
    else {
      uVar9 = 1;
      fVar22 = CSharedFunctions::CharArrayToFloat(RAM, param_1Offset);
      pfVar10Offset = param_1Offset;
      pfVar11Offset = param_1Offset;
      if (uVar1 != 1) {
        do {
          fVar23 = fVar22 / fVar21;
          fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 1 * 4);
          uVar9 = uVar9 + 1;
          pfVar11Offset = pfVar10Offset + 1 * 4;
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, fVar23);
          pfVar10Offset = pfVar11Offset;
        } while (uVar1 != uVar9);
      }
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar11Offset, fVar22 / fVar21);
      uVar1 = uVar9 + 4;
      pfVar10Offset = pfVar11Offset + 1 * 4;
      if (uVar9 == uVar16) goto LAB_00009220;
    }
    fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset);
    for (; uVar1 != uVar16; uVar1 = uVar1 + 4) {
      fVar23 = fVar22 / fVar21;
      fVar22 = CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 4 * 4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, fVar23);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 1 * 4) / fVar21);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + 2 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 2 * 4) / fVar21);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + 3 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 3 * 4) / fVar21);
      pfVar10Offset = pfVar10Offset + 4 * 4;
    }
    CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, fVar22 / fVar21);
    CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 1 * 4) / fVar21);
    CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + 2 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 2 * 4) / fVar21);
    CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + 3 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset + 3 * 4) / fVar21);
  }
LAB_00009220:
  uVar1 = 1;
  if (1 < (int)uVar16) {
    iVar19 = (int)uVar16 >> 1;
    pfVar10Offset = param_1Offset;
    do {
      if ((int)uVar1 < iVar5) {
        CSharedFunctions::WriteFloatToBuffer(RAM, 0x80151c94, CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + (iVar5 + -1) * 4));
        CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + (iVar5 + -1) * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar10Offset));
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, CSharedFunctions::CharArrayToFloat(RAM, 0x80151c94));
      }
      pfVar10Offset = pfVar10Offset + 1 * 4;
      iVar18 = iVar19;
      if (iVar19 < iVar5) {
        do {
          iVar5 = iVar5 - iVar18;
          iVar18 = iVar18 >> 1;
        } while (iVar18 < iVar5);
      }
      uVar1 = uVar1 + 1;
      iVar5 = iVar5 + iVar18;
    } while (uVar1 != uVar16);
  }
  return;
}

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00008514(unsigned char* RAM, unsigned long param_1Offset, unsigned long param_2Offset,unsigned long param_3, unsigned long param_4Offset)

{
  //param_1 float *
  //param_2 float *
  //param_4 float *
  unsigned long pfVar1Offset; //float *
  unsigned long pfVar2Offset; //float *
  unsigned long pfVar3Offset; //float *
  unsigned long pfVar4Offset; //float *
  unsigned long pfVar5Offset; //float *
  unsigned long pfVar6Offset; //float *
  unsigned long pfVar7Offset; //float *
  int iVar8;
  unsigned int uVar9;
  unsigned long pfVar10Offset; //float *
  unsigned long pfVar11Offset; //float *
  unsigned long pfVar12Offset; //float *
  unsigned long pfVar13Offset; //float *
  unsigned long pfVar14Offset; //float *
  unsigned long pfVar15Offset; //float *
  unsigned int uVar16;
  unsigned int uVar17;
  float fVar18;
  float fVar19;
  float fVar20;
  unsigned int uVar21;
  float uVar22;
  float uVar23;
  
  iVar8 = 1 << (param_3 & 0x1f);
  uVar17 = iVar8 >> 1;
  if (iVar8 != (int)CSharedFunctions::CharArrayToFloat(RAM, param_4Offset)) {
	throw; // TODO
    /*fVar20 = 0.0;
    uVar16 = 0;
    if (0 < (int)uVar17) {
		uVar22 = CSharedFunctions::CharArrayToFloat(RAM, 0x800c5638);
		uVar23 = (float)(CSharedFunctions::CharArrayToDouble(RAM, 0x800c5630) / (double)(iVar8 << 1));
      pfVar10Offset = param_4Offset + uVar17 * 4;
      pfVar13Offset = param_4Offset + uVar17 * 4 + uVar17 * 4;
      do {
	    // Prob a special function? I think F12->F0
        fVar18 = (float)FUN_00022970(RAM, fVar20);
        fVar19 = (float)FUN_000227b0(RAM, fVar20);
		CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, (fVar18 - fVar19) * (float)uVar22);
        fVar18 = (float)FUN_00022970(RAM, fVar20);
        fVar19 = (float)FUN_000227b0(RAM, fVar20);
        uVar16 = uVar16 + 1;
        fVar20 = fVar20 + (float)uVar23;
		CSharedFunctions::WriteFloatToBuffer(RAM, pfVar13Offset, (fVar19 + fVar18) * (float)uVar22);
        pfVar10Offset = pfVar10Offset + 1 * 4;
        pfVar13Offset = pfVar13Offset + 1 * 4;
      } while (uVar16 != uVar17);
    }*/
  }
  CSharedFunctions::WriteFloatToBuffer(RAM, param_2Offset, CSharedFunctions::CharArrayToFloat(RAM, param_1Offset));
  CSharedFunctions::WriteFloatToBuffer(RAM, param_2Offset + uVar17 * 4, CSharedFunctions::CharArrayToFloat(RAM, param_1Offset + uVar17 * 4));
  pfVar10Offset = (param_1Offset + iVar8 * 4 + -1 * 4);
  uVar16 = 1;
  if (1 < (int)uVar17) {
    uVar9 = uVar17 - 1 & 3;
    pfVar13Offset = (param_2Offset + 1 * 4);
    pfVar3Offset = (param_2Offset + 1 * 4);
    pfVar2Offset = (param_1Offset + 1 * 4);
    pfVar1Offset = (param_1Offset + 1 * 4);
    pfVar15Offset = param_4Offset + uVar17 * 2 * 4 + 1 * 4;
    pfVar14Offset = param_4Offset + uVar17 * 2 * 4 + 1 * 4;
    pfVar12Offset = param_4Offset + uVar17 * 4 + 1 * 4;
    pfVar11Offset = param_4Offset + uVar17 * 4 + 1 * 4;
    pfVar5Offset = (param_2Offset + iVar8 * 4 + -1 * 4);
    pfVar4Offset = (param_2Offset + iVar8 * 4 + -1 * 4);
    pfVar6Offset = pfVar10Offset;
    pfVar7Offset = pfVar10Offset;
    if (uVar9 != 0) {
      do {
        uVar16 = uVar16 + 1;
        pfVar13Offset = pfVar3Offset + 1 * 4;
        pfVar5Offset = pfVar4Offset + -1 * 4;
        pfVar2Offset = pfVar1Offset + 1 * 4;
        pfVar15Offset = pfVar14Offset + 1 * 4;
        pfVar12Offset = pfVar11Offset + 1 * 4;
        pfVar7Offset = pfVar6Offset + -1 * 4;

		CSharedFunctions::WriteFloatToBuffer(RAM, pfVar3Offset, (CSharedFunctions::CharArrayToFloat(RAM, pfVar6Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar14Offset)) + (CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset)));
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar4Offset,  (CSharedFunctions::CharArrayToFloat(RAM, pfVar14Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset)) - (CSharedFunctions::CharArrayToFloat(RAM, pfVar6Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar11Offset)));
        pfVar3Offset = pfVar13Offset;
        pfVar1Offset = pfVar2Offset;
        pfVar14Offset = pfVar15Offset;
        pfVar11Offset = pfVar12Offset;
        pfVar4Offset = pfVar5Offset;
        pfVar6Offset = pfVar7Offset;
      } while (uVar9 + 1 != uVar16);
      if (uVar16 == uVar17) goto LAB_00008838;
    }
    do {
      uVar16 = uVar16 + 4;
	  CSharedFunctions::WriteFloatToBuffer(RAM, pfVar13Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset) + CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar5Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset) - CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset) * CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar13Offset + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset + -1 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset + 1 * 4) + CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset + 1 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset + 1 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar5Offset + -1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset + 1 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset + 1 * 4) - CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset + -1 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset + 1 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar13Offset + 2 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset + -2 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset + 2 * 4) + CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset + 2 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset + 2 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar5Offset + -2 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset + 2 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset + 2 * 4) - CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset + -2 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset + 2 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar13Offset + 3 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset + -3 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset + 3 * 4) + CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset + 3 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset + 3 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar5Offset + -3 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar15Offset + 3 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset + 3 * 4) - CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset + -3 * 4) * CSharedFunctions::CharArrayToFloat(RAM, pfVar12Offset + 3 * 4));
      pfVar13Offset = pfVar13Offset + 4 * 4;
      pfVar2Offset = pfVar2Offset + 4 * 4;
      pfVar15Offset = pfVar15Offset + 4 * 4;
      pfVar12Offset = pfVar12Offset + 4 * 4;
      pfVar5Offset = pfVar5Offset + -4 * 4;
      pfVar7Offset = pfVar7Offset + -4 * 4;
    } while (uVar16 != uVar17);
  }
LAB_00008838:
  /*FILE* a = fopen("C:\\temp\\RAM.bin", "wb");
  if (a)
  {
	  fwrite(RAM, 1, 0x400000, a);
	  fflush(a);
	  fclose(a);
  }*/
  FUN_00008930(RAM, param_2Offset, param_3, param_4Offset);
  /*a = fopen("C:\\temp\\RAM.bin", "wb");
  if (a)
  {
	  fwrite(RAM, 1, 0x400000, a);
	  fflush(a);
	  fclose(a);
  }*/
  uVar16 = 0;
  pfVar13Offset = (param_2Offset + uVar17 * 4);
  if (0 < (int)uVar17) {
    if ((uVar17 & 3) != 0) {
      do {
		  uVar21 = CSharedFunctions::CharArrayToFloat(RAM, param_2Offset);
        uVar16 = uVar16 + 1;
        param_2Offset = param_2Offset + 1 * 4;
		CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset, uVar21);
		fVar20 = CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset);
        pfVar13Offset = pfVar13Offset + 1 * 4;
        param_1Offset = param_1Offset + 2 * 4;
        pfVar3Offset = pfVar10Offset + -2 * 4;
		CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, fVar20);
        pfVar10Offset = pfVar3Offset;
      } while ((uVar17 & 3) != uVar16);
      if (uVar16 == uVar17) {
        return;
      }
    }

	/*a = fopen("C:\\temp\\RAM.bin", "wb");
    if (a)
    {
	  fwrite(RAM, 1, 0x400000, a);
	  fflush(a);
	  fclose(a);
    }*/
    do {
      uVar16 = uVar16 + 4;
	  CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset, CSharedFunctions::CharArrayToFloat(RAM, param_2Offset));
	  CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset));
	  CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 2 * 4, CSharedFunctions::CharArrayToFloat(RAM, param_2Offset + 1 * 4));
	  CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + -2 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset + 1 * 4));
	  CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 4 * 4, CSharedFunctions::CharArrayToFloat(RAM, param_2Offset + 2 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + -4 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset + 2 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, param_1Offset + 6 * 4, CSharedFunctions::CharArrayToFloat(RAM, param_2Offset + 3 * 4));
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset + -6 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar13Offset + 3 * 4));
      param_2Offset = param_2Offset + 4 * 4;
      param_1Offset = param_1Offset + 8 * 4;
      pfVar13Offset = pfVar13Offset + 4 * 4;
      pfVar10Offset = pfVar10Offset + -8 * 4;
    } while (uVar16 != uVar17);
  }
  
	/*a = fopen("C:\\temp\\RAM.bin", "wb");
    if (a)
    {
	  fwrite(RAM, 1, 0x400000, a);
	  fflush(a);
	  fclose(a);
    }*/
  return;
}

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_000096b4(unsigned char* RAM, unsigned long param_1Offset,unsigned long param_2Offset, std::vector<unsigned short>& samples)

{
	//param_1 short*
	// param_2 int*
  short sVar1;
  float fVar2;
  float fVar3;
  unsigned long pfVar4Offset; //float *
  unsigned long pfVar5Offset; //float *
  float fVar6;
  
  if ((int)CSharedFunctions::CharArrayToLong(RAM, param_2Offset) != 0) {
	CSharedFunctions::WriteLongToBuffer(RAM, param_2Offset + 3 * 4, (int)CSharedFunctions::CharArrayToLong(RAM, param_2Offset));
    CSharedFunctions::WriteLongToBuffer(RAM, param_2Offset, 0);
  }
  CSharedFunctions::WriteShortToBuffer(RAM, (param_2Offset + 6 * 4), (short)CSharedFunctions::CharArrayToShort(RAM, param_2Offset + 6 * 4) + (short)CSharedFunctions::CharArrayToShort(RAM, 0x801570f4));
  sVar1 = (short)CSharedFunctions::CharArrayToShort(RAM, param_2Offset + 6 * 4);
  while (0x1000 < sVar1) {
    FUN_000092d4(RAM, param_2Offset + 3 * 4);
    CSharedFunctions::WriteShortToBuffer(RAM, (param_2Offset + 6 * 4), (short)CSharedFunctions::CharArrayToShort(RAM, param_2Offset + 6 * 4) + -0x1000);
    sVar1 = (short)CSharedFunctions::CharArrayToShort(RAM, param_2Offset + 6 * 4);
  }
  /*FILE* a = fopen("C:\\temp\\RAM.bin", "wb");
  if (a)
  {
	  fwrite(RAM, 1, 0x400000, a);
	  fflush(a);
	  fclose(a);
  }*/
  FUN_00008514(RAM, 0x80150c88,0x80151088, 8, 0x80151488);
  /*a = fopen("C:\\temp\\RAM.bin", "wb");
  if (a)
  {
	  fwrite(RAM, 1, 0x400000, a);
	  fflush(a);
	  fclose(a);
  }*/
  fVar3 = CSharedFunctions::CharArrayToFloat(RAM, 0x800c5668);
  fVar2 = CSharedFunctions::CharArrayToFloat(RAM, 0x800c5664);
  pfVar4Offset = 0x80150c88;
  fVar6 = CSharedFunctions::CharArrayToFloat(RAM, 0x80150c88);
  while( true ) {
    if (fVar3 < fVar6) {
	  CSharedFunctions::WriteFloatToBuffer(RAM, pfVar4Offset, fVar3);
      fVar6 = CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset);
    }
    if (fVar6 < fVar2) {
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar4Offset, fVar2);
    }
    pfVar4Offset = pfVar4Offset + 1 * 4;
    if (0x80151087 < pfVar4Offset) break;
    fVar6 = CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset);
  }
  pfVar4Offset = 0x80150c88;
  do {
    pfVar5Offset = pfVar4Offset + 4 * 4;
	CSharedFunctions::WriteShortToBuffer(RAM, param_1Offset, (short)(int)CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset)); // this is the sound, can mux 4 channels
	CSharedFunctions::WriteShortToBuffer(RAM, param_1Offset + 1 * 2, (short)(int)CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset + 1 * 4));
    CSharedFunctions::WriteShortToBuffer(RAM, param_1Offset + 2 * 2, (short)(int)CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset + 2 * 4));
    CSharedFunctions::WriteShortToBuffer(RAM, param_1Offset + 3 * 2, (short)(int)CSharedFunctions::CharArrayToFloat(RAM, pfVar4Offset + 3 * 4));
	/*FILE* aa = fopen("C:\\temp\\raw.bin", "ab");
	fwrite(&RAM[param_1Offset & 0xFFFFFF], 1, 2, aa);
	fflush(aa);
	fclose(aa);*/
	samples.push_back(CSharedFunctions::CharArrayToShort(RAM, param_1Offset));

    pfVar4Offset = pfVar5Offset;
    param_1Offset = param_1Offset + 4 * 2;
  } while (pfVar5Offset != 0x80151088);
  return;
}

bool CStarFox64AudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int samplingRate, CString outputFilename)
{
	/*FILE* a = fopen(outputFilename + ".bin", "wb");
	fwrite(wavData, 1, len, a);
	fflush(a);
	fclose(a);*/

	/*FILE* aa = fopen("C:\\temp\\smp2.bin", "wb");
	fwrite(processedWavData, 1, processedWavDataLength, aa);
	fflush(aa);
	fclose(aa);*/

	float samplingRateFloat = samplingRate;

	int flags = 1; //(waveType == 5); //0 = 8bit, 1=16bit

	FILE* outFileTempRaw = fopen(outputFilename, "wb");
	if (outFileTempRaw == NULL)
	{
		MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
		return false;
	}

	unsigned char wavHeader[0x28];

	wavHeader[0x0] = 0x52;
	wavHeader[0x1] = 0x49;
	wavHeader[0x2] = 0x46;
	wavHeader[0x3] = 0x46;
	unsigned long chunkSize = 0x28 + processedWavDataLength;
	wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
	wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
	wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
	wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
	wavHeader[0x8] = 0x57;
	wavHeader[0x9] = 0x41;
	wavHeader[0xA] = 0x56;
	wavHeader[0xB] = 0x45;
	wavHeader[0xC] = 0x66;
	wavHeader[0xD] = 0x6D;
	wavHeader[0xE] = 0x74;
	wavHeader[0xF] = 0x20;
	wavHeader[0x10] = 0x10;
	wavHeader[0x11] = 0x00;
	wavHeader[0x12] = 0x00;
	wavHeader[0x13] = 0x00;
	wavHeader[0x14] = 0x01;
	wavHeader[0x15] = 0x00;
	wavHeader[0x16] = 0x01;
	wavHeader[0x17] = 0x00;
	wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
	wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
	wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
	wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
	wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
	wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
	wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
	wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
	wavHeader[0x20] = (1 + (flags & 1));
	wavHeader[0x21] = 0x00;
	wavHeader[0x22] = (((flags & 1) + 1) * 8);
	wavHeader[0x23] = 0x00;
	wavHeader[0x24] = 0x64;
	wavHeader[0x25] = 0x61;
	wavHeader[0x26] = 0x74;
	wavHeader[0x27] = 0x61;

	fwrite(wavHeader, 1, 0x28, outFileTempRaw );

	unsigned long length = processedWavDataLength;
	fwrite(&length, 1, 4, outFileTempRaw);

	if ((flags & 1) == 1)
	{
		fwrite(processedWavData, 1, processedWavDataLength, outFileTempRaw);
	}
	else
	{
		for (int r = 0; r < processedWavDataLength; r++)
		{
			int value = (signed char)processedWavData[r];
			value += 128;
			fwrite(&value, 1, 1, outFileTempRaw);

		}
	}

	fflush(outFileTempRaw);
	fclose(outFileTempRaw);

	return true;
}

bool CStarFox64AudioDecompression::SaveWav(std::vector<unsigned short> samples, int samplingRate, CString outputFilename)
{
	int processedWavDataLength = samples.size() * 2;
	unsigned char* processedWavData = new unsigned char[processedWavDataLength];
	for (int x = 0; x < samples.size(); x++)
	{
		unsigned short adjust = (unsigned short)(samples[x]);

		// Little endian
		processedWavData[(x * 2) + 1] = ((adjust >> 8) & 0xFF);
		processedWavData[(x * 2) + 0] = adjust & 0xFF;
	}

	bool result = SaveProcessedWav(processedWavData, processedWavDataLength, samplingRate, outputFilename);
	delete [] processedWavData;
	return result;
}

bool CStarFox64AudioDecompression::DecompressSound(unsigned char* ROM, int romOffset, int inputLength, CString outputFilename, float samplingRateFloat)
{
	int sizeRAM = 0x400000;
	unsigned char* RAM = new unsigned char[sizeRAM];
	for (int x = 0; x < sizeRAM; x++)
		RAM[x] = 0x00;

	unsigned long offsetToDataPointer = 0x802FF88C;
	unsigned long inputOffset = 0x80375AB0;

	memcpy(&RAM[inputOffset & 0xFFFFFF], &ROM[romOffset], inputLength);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x802FF880, inputOffset);
	CSharedFunctions::WriteLongToBuffer(RAM, offsetToDataPointer, inputOffset);
	CSharedFunctions::WriteShortToBuffer(RAM, 0x802FF898, 0x0000);

	CSharedFunctions::WriteLongToBuffer(RAM, 0x00157DC4, inputOffset);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00157DC8, romOffset);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00157DCC, inputLength);

	CSharedFunctions::WriteLongToBuffer(RAM, 0x001570F4, 0x10000000);

	CSharedFunctions::WriteLongToBuffer(RAM, 0x000C5644, 0x3FB504F7);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x000C5648, 0x3FB504F7);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x000C5664, 0xC6FFFE00);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x000C5668, 0x46FFFE00);

	CSharedFunctions::StringToByteArray(&RAM[0x00150C88], "4421939944166E544413C046441F1B7F442946A844269CC1441BD7644413A037440CE20D43F6B26A43C4507C43B1F38E43C9F9B743D8A08E43C02F26439AF614437D56DC434B85A1431602E542C69EAF42A3E89642F4BF9E43194F4642AD4FA1C194E9B5C258BFC7C2765B38C2EE6940C3311EAEC331D13EC319E4D0C306AA12C3003D3AC30E74A9C3030679C2953396C292A1C2C31C92E0C33C6DDCC303C067C2D8C1D0C302C066C2DBE3ACC280B1A1C2391CFAC22127BCC257FAC8C29C7405C2128AC441F6944FC06254BBC29A8063C26ED416C21B9353C2E71245C33CBCB1C348E956C3643BBFC3883A50C399EEBBC3BBB30BC3D6B434C3CD7C27C3DE6E64C41469BEC42C11D7C42C30C2C43D7BD9C45C5833C467E84FC46DF8B2C480074EC48703DCC48C84E1C4930BCCC48D96CBC48022BFC481CA34C48C2198C48AE0ACC48B1304C498B172C49E15F9C4927241C4869DCEC477EF86C45D3088C452A8E4C44D420AC42F295FC4161961C417E70AC4055464C3AE43C5C3877AA0C3870BD7C324B42FC20E939A418E3C5242A94419431284864314667643702B2B43E080FB44114B3B442A3B184461D35A4484BE1C44781C30446216F34464A35C4462246F4467564C4484025144904AF744960BF444A3947544A7DF054496D13A448F9036449AB01144991816448E404A449388AC449A3825449252F4448B9B504483F8BA445D0735443CF5B9443645A144184BEF43ED6FFF4409083F441F32AE44044CA343D46E5043CBE82E439339564312D76B42D9075B424C7522C21FA72AC15679F1C182030BC3415A5DC393D83CC379C750C3A84ADEC400E719C4042FEAC3F50528C4067792C4002D14C3DA3E84C3EFDF9AC401B5AFC3F5468BC4175BE9C4401C82C42E67D6C40FC263C4204759C42C23B8C41E07ADC42B1E79C43D86E7C4221E19C3F755D0C3B73716C30F78F6C1476CEBC316A4ECC3809B91C32F81ECC323111EC33A0466C12BF7374323565342FEFD3A42A9936542CBF1204265832E42D16EDA4394184F43A4175C4354142C4370EA1C438EE98643221EDE4307A1DC4389888543789F044257928AC2A5543EC35A893BC399C412C2D64B02434C4AC443999C6E43A8041143B0FF6F42D95456C363E79BC357398B423679BC438AE38044071C22443A22C6442622E443FD15A843F9BFFD43EE9E4B43E8E1754429B03844537940442897AA43ED6BB743EB3B4343DEED9C43D087794402B2714401339C438DEA4B42BBA7D5C21B4BA1C35F1445C34AF50D42139126415E7EAEC3AFD3F7C42F4311C47E990EC4A73BC8C4A748CFC47A47AFC42EE630C3FB24ECC3BCF590C4046DF0C4472F00C436603EC3DDCFC0C38E9F03C30782BC421EE536C25C49FBC3933995C38E16D4C29AC2DF433D82964414C3F64463AE6F4456D2CF4437AAE044459A97442193994413C046442946A8441BD764440CE20D43C4507C43C9F9B743C02F26437D56DC431602E542A3E89643194F46C194E9B5C2765B38C3311EAEC319E4D0C3003D3AC3030679C292A1C2C33C6DDCC2D8C1D0C2DBE3ACC2391CFAC257FAC8C2128AC4C06254BBC26ED416C2E71245C348E956C3883A50C3BBB30BC3CD7C27C41469BEC42C30C2C45C5833C46DF8B2C48703DCC4930BCCC48022BFC48C2198C48B1304C49E15F9C4869DCEC45D3088C44D420AC4161961C4055464C3877AA0C324B42F418E3C524312848643702B2B44114B3B4461D35A44781C304464A35C4467564C44904AF744A394754496D13A449AB011448E404A449A3825448B9B50445D0735443645A143ED6FFF441F32AE43D46E504393395642D9075BC21FA72AC182030BC393D83CC3A84ADEC4042FEAC4067792C3DA3E84C401B5AFC4175BE9C42E67D6C4204759C41E07ADC43D86E7C3F755D0C30F78F6C316A4ECC32F81ECC33A04664323565342A993654265832E4394184F4354142C438EE9864307A1DC43789F04C2A5543EC399C412434C4AC443A8041142D95456C357398B438AE380443A22C643FD15A843EE9E4B4429B038442897AA43EB3B4343D087794401339C42BBA7D5C35F144542139126C3AFD3F7C47E990EC4A748CFC42EE630C3BCF590C4472F00C3DDCFC0C30782BCC25C49FBC38E16D4433D82964463AE6F4437AAE044459A974456D2CF4414C3F6C29AC2DFC3933995421EE536C38E9F03C436603EC4046DF0C3FB24ECC47A47AFC4A73BC8C42F4311415E7EAEC34AF50DC21B4BA1438DEA4B4402B27143DEED9C43ED6BB74453794043E8E17543F9BFFD442622E444071C22423679BCC363E79B43B0FF6F43999C6EC2D64B02C35A893B4257928A4389888543221EDE4370EA1C43A4175C42D16EDA42CBF12042FEFD3AC12BF737C323111EC3809B91C1476CEBC3B73716C4221E19C42B1E79C42C23B8C40FC263C4401C82C3F5468BC3EFDF9AC4002D14C3F50528C400E719C379C750C3415A5DC15679F1424C75224312D76B43CBE82E44044CA34409083F44184BEF443CF5B94483F8BA449252F4449388AC44991816448F903644A7DF0544960BF4448402514462246F446216F34484BE1C442A3B1843E080FB4314667642A94419C20E939AC3870BD7C3AE43C5C417E70AC42F295FC452A8E4C477EF86C4927241C498B172C48AE0ACC481CA34C48D96CBC48C84E1C480074EC467E84FC43D7BD9C42C11D7C3DE6E64C3D6B434C399EEBBC3643BBFC33CBCB1C21B9353C29A806341F6944FC29C7405C22127BCC280B1A1C302C066C303C067C31C92E0C2953396C30E74A9C306AA12C331D13EC2EE6940C258BFC742AD4FA142F4BF9E42C69EAF434B85A1439AF61443D8A08E43B1F38E43F6B26A4413A03744269CC1441F1B7F44166E54438000003F7FEC433F7FB10F3F7F4E6D3F7EC46D3F7E13233F7D3AAC3F7C3B283F7B14BE3F79C79D3F7853F83F76BA073F74FA0B3F7314473F7109083F6ED89D3F6C835E3F6A09A63F676BD73F64AA583F61C5963F5EBE043F5B94193F5848513F54DB303F514D3B3F4D9F003F49D1103F45E4013F41D86E3F3DAEF63F39683F3CC90AB13D48FB313D96A9063DC8BD373DFAB2743E1640843E2F10A33E47C5C33E605C153E78CFCF3E888E953E94A0333EA09AE73EAC7CD63EB8442C3EC3EF183ECF7BCE3EDAE8843EE633793EF15AEE3EFC5D2B3F039C3F3F08F59D3F0E39DC3F13682D3F187FC33F1D7FD43F22679C3F2736593F2BEB4D3F3085BE3F7F4E6D3F7D3AAC3F79C79D3F74FA0B3F6ED89E3F676BD73F5EBE053F54DB313F49D1113F3DAEF83F3085B93F2267973F1368283F039C393EE6336D3EC3EF0E3EA09ADA3E78CFB13E2F10883DC8BD063CC909BEBD48FBC0BDFAB2B7BE47C5E2BE888EA6BEAC7CE9BECF7BDEBEF15AFCBF08F5A6BF187FCABF27365F3D96A9063E1640843E605C153E94A0323EB8442B3EDAE8823EFC5D283F0E39DB3F1D7FD33F2BEB4B3F3968433F45E4053F514D3F3F5B941D3F64AA5B3F6C83603F7314493F7853FA3F7C3B293F7EC46E3F7FEC433F7FB10F3F7E13223F7B14BD3F76BA053F7109053F6A09A23F61C5933F58484B3F4D9EFA3F41D8680000000000000000000000003F3504F73F33E7C03F32C8CD3F31A8213F3085BF3F2F61A83F2E3BE13F2D146D3F2BEB4D3F2AC0853F2994183F2866083F2736593F26050E3F24D2283F239DAC3F22679C3F212FFC3F1FF6CE3F1EBC153F1D7FD43F1C420F3F1B02C83F19C2033F187FC33F173C0A3F15F6DC3F14B03C3F13682D3F121EB23F10D3CF3F0F87873F0E39DC3F0CEAD23F0B9A6E3F0A48AF3F08F59C3F07A1383F064B853F04F4853F039C3F3F0242B33F00E7E63EFF17B73EFC5D2B3EF9A0333EF6E0CF3EF41F0C3EF15AEF3EEE947F3EEBCBC23EE900BE3EE6337C3EE364033EE092573EDDBE823EDAE8893ED810743ED5364B3ED25A143ECF7BD43ECC9B953EC9B95D3EC6D5353EC3EF213EC1072A3EBE1D563EBB31AB3EB844353EB554F93EB263FD3EAF71473EAC7CE23EA986D33EA68F213EA395D43EA09AF43E9D9E883E9AA0963E97A1273E94A0423E919DED3E8E9A333E8B95163E888EA33E8586DD3E827DCE3E7EE6FC3E78CFE73E72B6693E6C9A963E667C7D3E605C293E5A39AD3E5415143E4DEE713E47C5D33E419B463E3B6EDB3E3540A33E2F10AD3E28DF053E22ABBE3E1C76E53E16408B3E1008BC3E09CF893E0395053DFAB2723DEE38773DE1BC2A3DD53DB33DC8BD2E3DBC3AB73DAFB66F3DA3307B3D96A8EE3D8A1FF23D7B2B463D62142E3D48FAEB3D2FDFCD3D16C2EB3CFB49263CC90A143C96C90F3C490D423BC90C8D3F3504F73F3620703F373A273F38521A3F3968453F3A7CA83F3B8F3F3F3CA0073F3DAEFD3F3EBC1F3F3FC76B3F40D0DD3F41D8743F42DE2C3F43E2053F44E3F93F45E4083F46E22E3F47DE6A3F48D8B83F49D1173F4AC7843F4BBBFB3F4CAE7E3F4D9F073F4E8D953F4F7A253F5064B43F514D433F5233CB3F53184E3F53FAC73F54DB363F55B9983F5695EB3F57702A3F5848573F591E6F3F59F26F3F5AC4553F5B94203F5C61CC3F5D2D583F5DF6C43F5EBE0A3F5F832C3F6046263F6106F73F61C59D3F6282143F633C5E3F63F4773F64AA5D3F655E103F660F8B3F66BED03F676BDB3F6816AC3F68BF3F3F6965953F6A09AA3F6AAB7F3F6B4B0F3F6BE85C3F6C83633F6D1C223F6DB2963F6E46C23F6ED8A03F6F68343F6FF5763F70806A3F71090B3F718F5A3F7213553F7294FB3F73144A3F7391423F740BE03F7484243F74FA0E3F756D9A3F75DECA3F764D993F76BA0C3F77241A3F778BC83F77F1133F7853FB3F78B47D3F79129B3F796E533F79C7A23F7A1E893F7A73073F7AC51B3F7B14C33F7B62013F7BACD13F7BF5353F7C3B2E3F7C7EB53F7CBFCE3F7CFE773F7D3AB13F7D747A3F7DABD13F7DE0B73F7E13293F7E43283F7E70B53F7E9BCF3F7EC4723F7EEAA13F7F0E5E3F7F2FA43F7F4E733F7F6ACC3F7F84B13F7F9C1E3F7FB1153F7FC3943F7FD39C3F7FE12E3F7FEC483F7FF4EC3F7FFB163F7FFECA");
	
	std::vector<unsigned short> samples;
	while (true)
	{
		FUN_000096b4(RAM, 0x80310190, 0x802FF880, samples);

		if (CSharedFunctions::CharArrayToLong(RAM, offsetToDataPointer) >= (inputOffset + inputLength))
			break;
	}

	SaveWav(samples, samplingRateFloat, outputFilename);

	delete [] RAM;

	return true;
}