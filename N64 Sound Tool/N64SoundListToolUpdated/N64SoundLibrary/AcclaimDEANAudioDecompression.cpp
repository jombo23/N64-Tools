#include "StdAfx.h"
#include "AcclaimDEANAudioDecompression.h"
#include "SharedFunctions.h"
#include <math.h>

CAcclaimDEANAudioDecompression::CAcclaimDEANAudioDecompression(void)
{
}

CAcclaimDEANAudioDecompression::~CAcclaimDEANAudioDecompression(void)
{
}

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00105fcc(unsigned char* RAM, int param_1)
{
  int iVar1;
  short sVar2;
  
  iVar1 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x2a8);
  sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, iVar1 + 0xd0) + 1;
  CSharedFunctions::WriteShortToBuffer(RAM, (iVar1 + 0xd0), sVar2);
  CSharedFunctions::WriteShortToBuffer(RAM, (iVar1 + 0xd6), (short)CSharedFunctions::CharArrayToShort(RAM, iVar1 + 0xd6) + -1);
  if ((int)(unsigned int)CSharedFunctions::CharArrayToShort(RAM, iVar1 + 0xcc) <= (int)sVar2) {
	  CSharedFunctions::WriteShortToBuffer(RAM, (iVar1 + 0xd0), 0);
  }
  iVar1 = iVar1 + (short)CSharedFunctions::CharArrayToShort(RAM, iVar1 + 0xd0) * 0x10;
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x4d8), (unsigned int)(CSharedFunctions::CharArrayToShort(RAM, iVar1 + 6) >> 1));
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 4), CSharedFunctions::CharArrayToLong(RAM, iVar1 + 8));
  return;
}

unsigned __int64 FUN_00104db0(unsigned char* RAM, int param_1, unsigned short param_2,unsigned short param_3,short param_4, int in_stack_00000010)
{
  unsigned short uVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  unsigned short uVar5;
  unsigned long puVar6; //unsigned short *
  unsigned int uVar7;
  unsigned long puVar8; //unsigned short *
  unsigned long puVar9; //unsigned short *
  
  if ((short)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ac) == 0) {
	  uVar1 = CSharedFunctions::CharArrayToShort(RAM, CSharedFunctions::CharArrayToLong(RAM, in_stack_00000010 + 4));
	  CSharedFunctions::WriteLongToBuffer(RAM, (in_stack_00000010 + 4), CSharedFunctions::CharArrayToLong(RAM, in_stack_00000010 + 4) + 1 * 2);
    iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, in_stack_00000010 + 0x4d8) + -1;
	CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x2ac), 0x10);
	CSharedFunctions::WriteLongToBuffer(RAM, (in_stack_00000010 + 0x4d8), iVar3);
	CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x4b0), uVar1);
    if (iVar3 < 1) {
	  // If ran out of compressed data
      FUN_00105fcc(RAM, in_stack_00000010);

	  // added to end when done reading
	  return 0;
    }
  }
  iVar3 = 0;
  if (param_3 != 0) {
    iVar4 = (unsigned int)param_2 * 2;
    puVar9 = (iVar4 + -0x7fec652c);
    puVar8 = (iVar4 + -0x7fec6550);
    do {
      uVar5 = CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ac);
      if (uVar5 < param_2) {
        uVar7 = (unsigned int)param_2 - (unsigned int)uVar5;
        puVar6 = (((iVar3 << 0x10) >> 0xf) + param_1);
		CSharedFunctions::WriteShortToBuffer(RAM, puVar6, (CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x4b0) & CSharedFunctions::CharArrayToShort(RAM, (unsigned int)uVar5 * 2 + -0x7fec6550)) << (uVar7 & 0x1f));
		uVar1 = CSharedFunctions::CharArrayToShort(RAM, CSharedFunctions::CharArrayToLong(RAM, in_stack_00000010 + 4));
		iVar4 = (int)CSharedFunctions::CharArrayToLong(RAM, in_stack_00000010 + 0x4d8) + -1;
		CSharedFunctions::WriteLongToBuffer(RAM, (in_stack_00000010 + 4), CSharedFunctions::CharArrayToLong(RAM, in_stack_00000010 + 4) + 1 * 2);
		CSharedFunctions::WriteLongToBuffer(RAM, (in_stack_00000010 + 0x4d8), iVar4);
		CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x4b0), uVar1);
        if (iVar4 < 1) {
		  // If ran out of compressed data
          FUN_00105fcc(RAM, in_stack_00000010);

		  // added to end when done reading
		  return 0;
        }
		uVar5 = CSharedFunctions::CharArrayToShort(RAM, puVar6) | (unsigned short)((int)(unsigned int)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x4b0) >> (0x10 - (uVar7 & 0xffff) & 0x1f)) & CSharedFunctions::CharArrayToShort(RAM, (uVar7 & 0xffff) * 2 + -0x7fec6550);
		CSharedFunctions::WriteShortToBuffer(RAM, puVar6, uVar5);
        if (param_4 != 0) {
          if ((uVar5 & CSharedFunctions::CharArrayToShort(RAM, puVar9)) == 0) {
            sVar2 = 0x10 - (short)uVar7;
            goto LAB_00104fa8;
          }
		  CSharedFunctions::WriteShortToBuffer(RAM, puVar6, uVar5 | ~CSharedFunctions::CharArrayToShort(RAM, puVar8));
        }
        sVar2 = 0x10 - (short)uVar7;
      }
      else {
        uVar5 = (unsigned short)((int)(unsigned int)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x4b0) >>
                        ((unsigned int)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ac) - (unsigned int)param_2 & 0x1f)) &
						CSharedFunctions::CharArrayToShort(RAM, puVar8);
        puVar6 = (((iVar3 << 0x10) >> 0xf) + param_1);
		CSharedFunctions::WriteShortToBuffer(RAM, puVar6, uVar5);
        if ((param_4 != 0) && ((uVar5 & CSharedFunctions::CharArrayToShort(RAM, puVar9)) != 0)) {
          CSharedFunctions::WriteShortToBuffer(RAM, puVar6, uVar5 | ~CSharedFunctions::CharArrayToShort(RAM, puVar8));
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ac) - param_2;
      }
LAB_00104fa8:
	  CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x2ac), sVar2);
      iVar3 = iVar3 + 1;
    } while (iVar3 * 0x10000 >> 0x10 < (int)(unsigned int)param_3);
  }
  return 1;
}


unsigned short FUN_00105000(unsigned char* RAM, int param_1,int param_2)
{
  int iVar1;
  int iVar2;
  unsigned short uVar3;
  
  iVar2 = 0;
  uVar3 = 0;
  if (0 < (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x1a0)) {
    iVar1 = 0;
    while( true ) {
      iVar2 = iVar2 + 1;
      uVar3 = (unsigned short)iVar2;
	  CSharedFunctions::WriteShortToBuffer(RAM, ((iVar1 >> 0x10) * 2 + param_2), CSharedFunctions::CharArrayToShort(RAM, param_1 + (iVar1 >> 0x10) * 4 + 0xa2));
      if ((int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x1a0) <= iVar2 * 0x10000 >> 0x10) break;
      iVar1 = iVar2 * 0x10000;
    }
  }
  return uVar3;
}

int FUN_0010505c(unsigned char* RAM, int param_1,int param_2,short param_3,unsigned long param_4, int in_stack_00000010)
{
	//param_4 short *
  unsigned short uVar1;
  int iVar2;
  int iVar3;
  short sVar5;
  int iVar4;
  int iVar6;
  int iVar7;
  unsigned int uVar8;
  unsigned short uVar9;
  unsigned long psVar10; //short *
  int iVar11;
  int iVar12;
  
  uVar8 = 0;
  iVar7 = 0;
  CSharedFunctions::WriteShortToBuffer(RAM, param_4, 0);
  iVar12 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x10);
  iVar6 = (int)param_3;
  iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, iVar12 + 0x20);
  iVar11 = 0;
  sVar5 = 0;
  if (0 < iVar6) {
    do {
		if ((short)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ae) == 0) {
        sVar5 = FUN_00104db0(RAM, in_stack_00000010 + 0x4b2,0x10,1,0, in_stack_00000010);
        if (sVar5 == 0) {
          return -1;
        }
		CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x2ae), (short)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ae) + 0x10);
LAB_001053a8:
        iVar4 = iVar11 << 0x10;
      }
      else {
        uVar1 = CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x4b2);
		CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x4b2), uVar1 << 1);
        CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x2ae), (short)CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ae) + -1);
        if ((uVar1 & 0x8000) == 0) {
          uVar1 = CSharedFunctions::CharArrayToShort(RAM, ((int)(uVar8 << 0x10) >> 0xe) + iVar3);
        }
        else {
          uVar1 = CSharedFunctions::CharArrayToShort(RAM, ((int)(uVar8 << 0x10) >> 0xe) + iVar3 + 2);
        }
        uVar8 = (unsigned int)uVar1;
        psVar10 = (((int)((unsigned int)uVar1 << 0x10) >> 0xe) + iVar3);
		if (CSharedFunctions::CharArrayToShort(RAM, psVar10) != 0x800) goto LAB_001053a8;
        uVar1 = CSharedFunctions::CharArrayToShort(RAM, psVar10 + 1 * 2);
        uVar9 = uVar1 & 0xf00;
        uVar8 = 0;
        if (((uVar1 & 0xf00) == 0) || (uVar9 == 0xf00)) {
          iVar2 = iVar11 << 0x10;
          iVar11 = iVar11 + 1;
		  CSharedFunctions::WriteShortToBuffer(RAM, ((iVar2 >> 0xf) + param_2), uVar1);
        }
        else if (uVar1 == 0x900) {
          uVar1 = CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x4b2);
		  sVar5 = FUN_00104db0(RAM, in_stack_00000010 + 0x4b2, CSharedFunctions::CharArrayToShort(RAM, iVar12 + 0xe),1,0, in_stack_00000010);
          iVar2 = iVar11 << 0x10;
          if (sVar5 == 0) {
            return -1;
          }
          iVar11 = iVar11 + 1;
          uVar9 = CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x4b2);
          CSharedFunctions::WriteShortToBuffer(RAM, (in_stack_00000010 + 0x4b2), uVar9 << (0x10 - CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ae) & 0x1f));
		  CSharedFunctions::WriteShortToBuffer(RAM, ((iVar2 >> 0xf) + param_2), uVar1 | (unsigned short)((int)(unsigned int)uVar9 >> (CSharedFunctions::CharArrayToShort(RAM, in_stack_00000010 + 0x2ae) & 0x1f )));
        }
        else if (uVar9 == 0xa00) {
		  // manual params
          iVar2 = FUN_00105000(RAM, (CSharedFunctions::CharArrayToLong(RAM, iVar12 + 0x1C) + ((uVar1 & 0xFF) * 0x1B8)), (param_2 + ((iVar11 << 0x10) >> 0xf)));
          iVar11 = iVar11 + iVar2;
        }
        else if (uVar9 == 0xb00) {
          iVar3 = (int)(short)iVar11;
          while (iVar3 < iVar6) {
            CSharedFunctions::WriteShortToBuffer(RAM, (((iVar11 << 0x10) >> 0xf) + param_2), 0);
            iVar3 = (iVar11 + 1) * 0x10000 >> 0x10;
            iVar11 = iVar11 + 1;
          }
          iVar3 = (short)iVar7 * 0x14 + param_1;
		  for (iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, iVar3 + 4) + (int)CSharedFunctions::CharArrayToLong(RAM, iVar3 + 8); iVar7 = iVar7 + 1, iVar3 <= iVar6;
              iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, iVar3 + 4) + (int)CSharedFunctions::CharArrayToLong(RAM, iVar3 + 8)) {
				  CSharedFunctions::WriteShortToBuffer(RAM, param_4, CSharedFunctions::CharArrayToShort(RAM, param_4) + 1);
            iVar3 = (iVar7 * 0x10000 >> 0x10) * 0x14 + param_1;
          }
          return iVar6;
        }
        sVar5 = (short)iVar11;
        iVar2 = (short)iVar7 * 0x14 + param_1;
        iVar4 = iVar11 << 0x10;
        if ((int)CSharedFunctions::CharArrayToLong(RAM, iVar2 + 4) + (int)CSharedFunctions::CharArrayToLong(RAM, iVar2 + 8) <= (int)sVar5) {
          iVar7 = iVar7 + 1;
          CSharedFunctions::WriteShortToBuffer(RAM, param_4, CSharedFunctions::CharArrayToShort(RAM, param_4) + 1);
          if (iVar6 <= sVar5) break;
          iVar12 = (int)CSharedFunctions::CharArrayToLong(RAM, (iVar7 * 0x10000 >> 0x10) * 0x14 + param_1 + 0x10);
          iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, iVar12 + 0x20);
          iVar4 = iVar11 << 0x10;
        }
      }
      sVar5 = (short)iVar11;
    } while (iVar4 >> 0x10 < iVar6);
  }
  return (int)sVar5;
}

unsigned __int64 FUN_001053f4(unsigned char* RAM, int param_1)
{
  unsigned long SP = 0x801998F0;
  int iVar1;
  short sVar2;
  int iVar3;
  unsigned long piVar4; //int *
  unsigned long asStack24 = SP + 24; // short[12];
  
  unsigned long S1 = param_1;

  piVar4 = 0x80139cb4;
  if ((short)CSharedFunctions::CharArrayToShort(RAM, param_1 + 0x4b4) == 0) {
	  CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 0x4b4), 1);
    iVar1 = FUN_00104db0(RAM, param_1 + 8,6,0x10,0, S1);
    iVar3 = 0;
    if (iVar1 << 0x10 < 1) {
      return 0xffffffffffffffff;
    }
	CSharedFunctions::WriteLongToBuffer(RAM, 0x80139cc0, CSharedFunctions::CharArrayToLong(RAM, 0x80139cc0) + 0x60);
    do {
      iVar1 = param_1 + ((iVar3 << 0x10) >> 0xf);
      iVar3 = iVar3 + 1;
	  CSharedFunctions::WriteShortToBuffer(RAM, (iVar1 + 0x4b6), CSharedFunctions::CharArrayToShort(RAM, iVar1 + 8));
    } while (iVar3 * 0x10000 >> 0x10 < 0x10);
	if (CSharedFunctions::CharArrayToLong(RAM, 0x80139cb4) != 0) {
		for (piVar4 = 0x80139cc8; CSharedFunctions::CharArrayToLong(RAM, piVar4) != 0; piVar4 = piVar4 + 5 * 4) {
      }
    }
  }
  else {
    sVar2 = FUN_0010505c(RAM, 0xffffffff80139cb4,param_1 + 8,0x10, asStack24, S1);
    if (sVar2 != 0x10) {
      return 0xffffffffffffffff;
    }
    iVar1 = 0;
	piVar4 = (0x80139cb4 + (short)CSharedFunctions::CharArrayToShort(RAM, asStack24 + 0 * 4) * 0x14);
    do {
      iVar3 = param_1 + ((iVar1 << 0x10) >> 0xf);
      iVar1 = iVar1 + 1;
	  sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, iVar3 + 8) + (short)CSharedFunctions::CharArrayToShort(RAM, iVar3 + 0x4b6);
	  CSharedFunctions::WriteShortToBuffer(RAM, (iVar3 + 8), sVar2);
      CSharedFunctions::WriteShortToBuffer(RAM, (iVar3 + 0x4b6), sVar2);
    } while (iVar1 * 0x10000 >> 0x10 < 0x10);
  }
  sVar2 = FUN_0010505c(RAM, piVar4, param_1 + 0xa8, 0x100, asStack24, S1);
  if (sVar2 != 0x100) {
    return 0xffffffffffffffff;
  }
  return 1;
}


unsigned int FUN_00105560(unsigned char* RAM, unsigned long param_1)

{
  short sVar1;
  sVar1 = FUN_001053f4(RAM, param_1);
  return (unsigned int)~(int)sVar1 >> 0x1f;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

short FUN_00105c84(unsigned char* RAM, float param_1)

{
  bool bVar1;
  float fVar2;
  
  if (0.0 <= param_1) {
	  param_1 = param_1 + CSharedFunctions::CharArrayToFloat(RAM, 0x80144da4);
    bVar1 = CSharedFunctions::CharArrayToFloat(RAM, 0x80144da8) < param_1;
    fVar2 = CSharedFunctions::CharArrayToFloat(RAM, 0x80144da8);
  }
  else {
    param_1 = param_1 - CSharedFunctions::CharArrayToFloat(RAM, 0x80144dac);
    bVar1 = param_1 < CSharedFunctions::CharArrayToFloat(RAM, 0x80144db0);
    fVar2 = CSharedFunctions::CharArrayToFloat(RAM, 0x80144db0);
  }
  if (bVar1) {
    param_1 = fVar2;
  }
  return (short)(int)param_1;
}


void FUN_00105a4c(unsigned char* RAM, int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  unsigned long pfVar3; //float *
  float fVar4;
  float fVar5;
  float fVar6;
  
  iVar2 = 0;
  do {
    pfVar3 = ((short)iVar2 * 8 + param_1);
    iVar1 = (short)iVar2 * 4;
	fVar5 = CSharedFunctions::CharArrayToFloat(RAM, pfVar3);
    fVar6 = CSharedFunctions::CharArrayToFloat(RAM, iVar1 + param_2);
    fVar4 = CSharedFunctions::CharArrayToFloat(RAM, iVar1 + param_3);
    iVar2 = iVar2 + 1;
	CSharedFunctions::WriteFloatToBuffer(RAM, pfVar3, fVar5 * fVar6 - CSharedFunctions::CharArrayToFloat(RAM, pfVar3 + 1 * 4) * fVar4);
	CSharedFunctions::WriteFloatToBuffer(RAM, pfVar3 + 1 * 4, fVar5 * fVar4 + CSharedFunctions::CharArrayToFloat(RAM, pfVar3 + 1 * 4) * fVar6);
  } while (iVar2 * 0x10000 >> 0x10 < 0x80);
  return;
}



void FUN_00105ac8(unsigned char* RAM, unsigned long param_1,int param_2, int param_3,unsigned long param_4, unsigned long in_stack_00000010, unsigned long in_stack_00000014)

{
	//unsigned long* param_1
	//float *param_4
  unsigned long pfVar1; //float *
  unsigned long pfVar2; //float *
  unsigned long puVar3; //unsigned long*
  int iVar4;
  int iVar5;
  int iVar6;
  unsigned long pfVar7; //float *
  unsigned long pfVar8; //float *
  int iVar9;
  int iVar10;
  float fVar11;
  float fVar12;
  unsigned long uVar13;
  float fVar14;
  float fVar15;
  //float *in_stack_00000010;
  //int *in_stack_00000014;
  
  iVar6 = 0;
  puVar3 = param_1;
  if (0 < param_2) {
    do {
		if (iVar6 < CSharedFunctions::CharArrayToLong(RAM, in_stack_00000014)) {
			uVar13 = CSharedFunctions::CharArrayToLong(RAM, puVar3);
			CSharedFunctions::WriteLongToBuffer(RAM, puVar3, CSharedFunctions::CharArrayToLong(RAM, param_1 + ((CSharedFunctions::CharArrayToLong(RAM, in_stack_00000014) * 2) * 4)));
        CSharedFunctions::WriteLongToBuffer(RAM, param_1 + ((CSharedFunctions::CharArrayToLong(RAM, in_stack_00000014) * 2) * 4), uVar13);
        uVar13 = CSharedFunctions::CharArrayToLong(RAM, puVar3 + 1 * 4);
        CSharedFunctions::WriteLongToBuffer(RAM, puVar3 + 1 * 4, CSharedFunctions::CharArrayToLong(RAM, param_1 + ((CSharedFunctions::CharArrayToLong(RAM, in_stack_00000014) * 2 + 1) * 4)));
        CSharedFunctions::WriteLongToBuffer(RAM, param_1 + ((CSharedFunctions::CharArrayToLong(RAM, in_stack_00000014) * 2 + 1) * 4), uVar13);
      }
      in_stack_00000014 = in_stack_00000014 + 1 * 4;
      iVar6 = iVar6 + 1;
      puVar3 = puVar3 + 2 * 4;
    } while (iVar6 < param_2);
  }
  iVar9 = 2;
  iVar10 = 1;
  for (iVar6 = param_2 >> 1; 0 < iVar6; iVar6 = iVar6 >> 1) {
    iVar5 = 0;
    if (0 < iVar10) {
      iVar4 = 0;
      pfVar8 = in_stack_00000010;
      pfVar7 = param_4;
      while( true ) {
		  fVar11 = CSharedFunctions::CharArrayToFloat(RAM, pfVar8);
        fVar15 = CSharedFunctions::CharArrayToFloat(RAM, pfVar7);
        if (iVar4 < param_2 << 1) {
          pfVar2 = (param_1 + (iVar5 * 2 * 4));
          do {
            pfVar1 = (param_1 + ((iVar4 + iVar9) * 4));
            fVar14 = fVar15 * CSharedFunctions::CharArrayToFloat(RAM, pfVar1) - (float)(int)param_3 * fVar11 * CSharedFunctions::CharArrayToFloat(RAM, pfVar1 + 1 * 4);
            fVar12 = fVar15 * CSharedFunctions::CharArrayToFloat(RAM, pfVar1 + 1 * 4) + (float)(int)param_3 * fVar11 * CSharedFunctions::CharArrayToFloat(RAM, pfVar1);
			CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1, CSharedFunctions::CharArrayToFloat(RAM, pfVar2) - fVar14);
			CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1 + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar2 + 1 * 4) - fVar12);
            iVar4 = iVar4 + iVar9 * 2;
			
			// Goes wrong here, or before leading to this
			CSharedFunctions::WriteFloatToBuffer(RAM, pfVar2, CSharedFunctions::CharArrayToFloat(RAM, pfVar2) + fVar14);
			CSharedFunctions::WriteFloatToBuffer(RAM, pfVar2 + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, pfVar2 + 1 * 4) + fVar12);
            pfVar2 = pfVar2 + (iVar9 * 2) * 4;
          } while (iVar4 < param_2 << 1);
        }
        pfVar8 = pfVar8 + iVar6 * 4;
        pfVar7 = pfVar7 + iVar6 * 4;
        iVar5 = iVar5 + 1;
        if (iVar10 <= iVar5) break;
        iVar4 = iVar5 * 2;
      }
    }
    iVar10 = iVar10 << 1;
    iVar9 = iVar9 << 1;
  }
  return;
}

void FUN_00105c4c(unsigned char* RAM, unsigned __int64 param_1,unsigned long param_2,unsigned __int64 param_3)

{
	//param2 unsigned long*
	FUN_00105ac8(RAM, param_1, CSharedFunctions::CharArrayToLong(RAM, param_2 + 3 * 4) ,param_3, CSharedFunctions::CharArrayToLong(RAM, param_2), CSharedFunctions::CharArrayToLong(RAM, param_2 + 4), CSharedFunctions::CharArrayToLong(RAM, param_2 + 8));
  return;
}


void FUN_001059ec(unsigned char* RAM, int param_1,int param_2)

{
  int iVar1;
  short sVar2;
  int iVar3;
  unsigned long puVar4; //unsigned long*
  
  iVar1 = 0;
  do {
    sVar2 = (short)iVar1;
    iVar3 = sVar2 * 8;
    puVar4 = (iVar3 + param_1);
    iVar1 = iVar1 + 1;
	CSharedFunctions::WriteFloatToBuffer(RAM, puVar4, CSharedFunctions::CharArrayToFloat(RAM, iVar3 + param_2));
	CSharedFunctions::WriteFloatToBuffer(RAM, puVar4 + 1 * 4, CSharedFunctions::CharArrayToFloat(RAM, (sVar2 * -2 + 0xff) * 4 + param_2));
  } while (iVar1 * 0x10000 >> 0x10 < 0x80);
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

unsigned __int64 FUN_001058b4(unsigned char* RAM, int param_1,int param_2)

{
  float fVar1;
  unsigned int uVar3;
  int iVar4;
  __int64 lVar2;
  int iVar5;
  short sVar6;
  
  fVar1 = CSharedFunctions::CharArrayToFloat(RAM, 0x80144da0);
  iVar4 = 0;
  uVar3 = 0;
  do {
    iVar5 = (iVar4 << 0x10) >> 0xf;
	if ((int)(short)uVar3 < (int)(unsigned int)CSharedFunctions::CharArrayToShort(RAM, 0x80139af8 + iVar5)) {
      do {
        sVar6 = (short)uVar3;
        uVar3 = uVar3 + 1;
		CSharedFunctions::WriteFloatToBuffer(RAM, (sVar6 * 4 + param_1), 
			((float)(int)(short)CSharedFunctions::CharArrayToShort(RAM, sVar6 * 2 + param_2 + 0xa0) /
			CSharedFunctions::CharArrayToFloat(RAM, (short)CSharedFunctions::CharArrayToShort(RAM, param_2 + iVar5) * 4 + -0x7fec64e8)) * fVar1);
      } while ((int)(uVar3 * 0x10000) >> 0x10 < (int)(unsigned int)CSharedFunctions::CharArrayToShort(RAM, 0x80139af8 + iVar5));
    }
	uVar3 = (unsigned int)CSharedFunctions::CharArrayToShort(RAM, 0x80139af8 + ((iVar4 << 0x10) >> 0xf));
    iVar4 = iVar4 + 1;
  } while (iVar4 * 0x10000 >> 0x10 < 0x10);
  lVar2 = (__int64)(short)CSharedFunctions::CharArrayToShort(RAM, 0x80139b16);
  iVar4 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, 0x80139b16);
  if (lVar2 < 0x100) {
    do {
		CSharedFunctions::WriteLongToBuffer(RAM, (((iVar4 << 0x10) >> 0xe) + param_1), 0);
      iVar4 = (int)lVar2 + 1;
      lVar2 = (__int64)iVar4;
    } while (iVar4 * 0x10000 >> 0x10 < 0x100);
  }
  return 1;
}



/* WARNING: Removing unreachable block (ram,0x0010b108) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_0010af60(unsigned char* RAM, float param_1)
{
	return sin(param_1);
  /*unsigned int uVar1;
  double dVar2;
  double dVar3;
  double dVar4;
  double dVar5;
  double dVar6;
  
  uVar1 = (int)ReinterpretAsU32(param_1) >> 0x16 & 0x1ff;
  if (uVar1 < 0xff) {
    dVar2 = (double)param_1;
    if (uVar1 < 0xe6) {
      return (float)param_1;
    }
    dVar4 = dVar2 * dVar2;
    return (float)(dVar2 * dVar4 *
                         (CSharedFunctions::CharArrayToDouble(RAM, 0x80144f58) +
						 ((CSharedFunctions::CharArrayToDouble(RAM, 0x80144f70) * dVar4 + CSharedFunctions::CharArrayToDouble(RAM, 0x80144f68)) * dVar4 + CSharedFunctions::CharArrayToDouble(RAM, 0x80144f60)) * dVar4 )
                        + dVar2);
  }
  if (uVar1 < 0x136) {
    dVar2 = (double)param_1 * CSharedFunctions::CharArrayToDouble(RAM, 0x80144f78);
    if ((double)0 <= dVar2) {
      dVar2 = dVar2 + 0.5; // not 100% sure
    }
    else {
      dVar2 = dVar2 - 0.5; // not 100% sure
    }
    dVar4 = (double)(int)dVar2;
    dVar3 = ((double)param_1 - dVar4 * CSharedFunctions::CharArrayToDouble(RAM, 0x80144f80)) - dVar4 * CSharedFunctions::CharArrayToDouble(RAM, 0x80144f88);
    dVar5 = dVar3 * dVar3;
    dVar6 = CSharedFunctions::CharArrayToDouble(RAM, 0x80144f58) +
            ((CSharedFunctions::CharArrayToDouble(RAM, 0x80144f70) * dVar5 + CSharedFunctions::CharArrayToDouble(RAM, 0x80144f68)) * dVar5 + CSharedFunctions::CharArrayToDouble(RAM, 0x80144f60)) * dVar5;
    if (((int)dVar2 & 1U) != 0) {
      return -(float)(dVar3 * dVar5 * dVar6 + dVar3);
    }
    return (float)(dVar3 * dVar5 * dVar6 + dVar3);
  }
  return CSharedFunctions::CharArrayToFloat(RAM, 0x80144f90);*/
}


/* WARNING: Removing unreachable block (ram,0x0010a790) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_0010a640(unsigned char* RAM, float param_1)
{
	return cos(param_1);
  /*float in_f0;
  double dVar1;
  double dVar2;
  float in_f4;
  double dVar3;
  double dVar4;
  double dVar5;
  
  if (0x135 < ((int)ReinterpretAsU32(param_1) >> 0x16 & 0x1ffU)) {
	  return CSharedFunctions::CharArrayToFloat(RAM, 0x80144f20);
  }

  if (param_1 <= 0.0) {
    param_1 = -param_1;
  }
  dVar3 = (double)param_1 * CSharedFunctions::CharArrayToDouble(RAM, 0x80144f08) + 0.5;
  if (0 <= dVar3) {
    dVar3 = dVar3 + 0.5; // not sure
  }
  else {
    dVar3 = dVar3 - 0.5; // not sure
  }
  dVar1 = (double)(int)dVar3 - 0.5; // not sure
  dVar2 = ((double)param_1 - dVar1 * CSharedFunctions::CharArrayToDouble(RAM, 0x80144f10)) - dVar1 * CSharedFunctions::CharArrayToDouble(RAM, 0x80144f18);
  dVar4 = dVar2 * dVar2;
  dVar5 = CSharedFunctions::CharArrayToDouble(RAM, 0x80144ee8) + ((CSharedFunctions::CharArrayToDouble(RAM, 0x80144f00) * dVar4 + CSharedFunctions::CharArrayToDouble(RAM, 0x80144ef8)) * dVar4 + CSharedFunctions::CharArrayToDouble(RAM, 0x80144ef0)) * dVar4;
  if (((int)dVar3 & 1U) == 0) {
    return (float)(dVar2 * dVar4 * dVar5 + dVar2);
  }
  return -(float)(dVar2 * dVar4 * dVar5 + dVar2);*/
}

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0010558c(unsigned char* RAM)

{
  int iVar1;
  int iVar2;
  short sVar3;
  float uVar4;
  float fVar5;
  float fVar6;
  float uVar7;
  float uVar8;
  float uVar9;
  float uVar10;
  
  uVar10 = (float)CSharedFunctions::CharArrayToFloat(RAM, 0x80144d80);
  uVar9 = (float)CSharedFunctions::CharArrayToFloat(RAM, 0x80144d84);
  iVar1 = 0;
  uVar8 = (float)CSharedFunctions::CharArrayToFloat(RAM, 0x80144d88);
  uVar7 = (float)CSharedFunctions::CharArrayToFloat(RAM, 0x80144d8c);
  do {
    fVar5 = ((float)(int)(short)iVar1 * (float)uVar10 + (float)uVar9) * (float)uVar8 * (float)uVar7;
    iVar2 = (short)iVar1 * 4;
    uVar4 = FUN_0010a640(RAM, fVar5);
	CSharedFunctions::WriteFloatToBuffer(RAM, (0x801952f0 + iVar2), uVar4);
    uVar4 = FUN_0010af60(RAM, fVar5);
    iVar1 = iVar1 + 1;
    CSharedFunctions::WriteFloatToBuffer(RAM, (0x801954f0 + iVar2), uVar4);
  } while (iVar1 * 0x10000 >> 0x10 < 0x80);
  iVar1 = 0;
  uVar8 = (float)CSharedFunctions::CharArrayToFloat(RAM, 0x80144d90);
  uVar7 = (float)CSharedFunctions::CharArrayToFloat(RAM, 0x80144d94);
  do {
    sVar3 = (short)iVar1;
    fVar6 = (float)(int)sVar3 * (float)uVar8 * (float)uVar7;
    fVar5 = (float)FUN_0010a640(RAM, fVar6);
    fVar6 = (float)FUN_0010af60(RAM, fVar6);
    iVar1 = iVar1 + 1;
    iVar2 = sVar3 * 4;
	CSharedFunctions::WriteFloatToBuffer(RAM, (0x801956f0 + iVar2), fVar5 + fVar6);
    CSharedFunctions::WriteFloatToBuffer(RAM, (0x801958f0 + iVar2), fVar6 - fVar5);
  } while (iVar1 * 0x10000 >> 0x10 < 0x80);
  return;
}


void FUN_00105704(unsigned char* RAM, unsigned long param_1,int param_2)
{
	//int * param_1
  bool bVar1;
  unsigned long piVar2; //int *
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar4 = 0;
  piVar2 = param_1;
  if (0 < param_2) {
    do {
		CSharedFunctions::WriteLongToBuffer(RAM, piVar2, iVar4);
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 1 * 4;
    } while (iVar4 < param_2);
  }
  iVar4 = 1;
  iVar5 = 1;
  if (0 < param_2 << 1) {
    do {
      if (iVar5 < iVar4) {
		  iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + (((iVar4 + -1) / 2) * 4));
		  CSharedFunctions::WriteLongToBuffer(RAM, param_1 + (((iVar4 + -1) / 2) * 4), (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + (((iVar5 + -1) / 2) * 4)));
        CSharedFunctions::WriteLongToBuffer(RAM, param_1 + (((iVar5 + -1) / 2) * 4), iVar3);
      }
      iVar3 = param_2;
      if (1 < param_2) {
        bVar1 = param_2 < iVar4;
        do {
          if (!bVar1) break;
          iVar4 = iVar4 - iVar3;
          iVar3 = iVar3 >> 1;
          bVar1 = iVar3 < iVar4;
        } while (1 < iVar3);
      }
      iVar5 = iVar5 + 2;
      iVar4 = iVar4 + iVar3;
    } while (iVar5 <= param_2 << 1);
  }
  return;
}

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

unsigned long FUN_001057cc(unsigned char* RAM, unsigned __int64 param_1)
{
  //returns unsigned char*
  int iVar1;
  unsigned long pfVar2; //float *
  int iVar3;
  int iVar4;
  float uVar5;
  float fVar6;
  double dVar7;
  double dVar8;
  
  iVar3 = 0;
  iVar4 = (int)param_1;
  if (0 < iVar4 >> 1) {
    dVar7 = (double)iVar4;
	dVar8 = CSharedFunctions::CharArrayToDouble(RAM, 0x80144d98);
    do {
      fVar6 = (float)(((double)iVar3 * dVar8) / dVar7);
      uVar5 = FUN_0010a640(RAM, fVar6);
      iVar1 = iVar3 * 4;
	  CSharedFunctions::WriteFloatToBuffer(RAM, (iVar1 + CSharedFunctions::CharArrayToLong(RAM, 0x80139aa0)), uVar5);
      iVar3 = iVar3 + 1;
      pfVar2 = (iVar1 + CSharedFunctions::CharArrayToLong(RAM, 0x80139aa4));
      fVar6 = (float)FUN_0010af60(RAM, fVar6);
	  CSharedFunctions::WriteFloatToBuffer(RAM, pfVar2, -fVar6);
    } while (iVar3 < iVar4 >> 1);
  }
  FUN_00105704(RAM, CSharedFunctions::CharArrayToLong(RAM, 0x80139aa8),param_1);
  CSharedFunctions::WriteLongToBuffer(RAM, 0x80139aac, iVar4);
  return 0x80139aa0;
}

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00105cf0(unsigned char* RAM, int param_1,int param_2, unsigned char* outputSamples, int& outputSampleLength)
{
  unsigned long SP = 0x80199908;
  SP = SP - 0x848;
  unsigned short uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float uVar10;
  //float afStack264248 [65536];
  unsigned long afStack2104 = SP + 0x10; //float [256];
  unsigned long auStack1080 = SP + 0x410; //unsigned char [1080];
  
  if (CSharedFunctions::CharArrayToShort(RAM, 0x80139c18) == 0) {
    FUN_0010558c(RAM);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x80195af0, FUN_001057cc(RAM, 0x80));
	CSharedFunctions::WriteShortToBuffer(RAM, 0x80139c18, 1);
  }
  FUN_001058b4(RAM, auStack1080, param_2 + 8);
  FUN_001059ec(RAM, afStack2104, auStack1080);
  FUN_00105a4c(RAM, afStack2104, 0xffffffff801952f0,0xffffffff801954f0);
  iVar3 = 0;
  uVar10 = CSharedFunctions::CharArrayToFloat(RAM, 0x80144db4);
  FUN_00105c4c(RAM, afStack2104, CSharedFunctions::CharArrayToLong(RAM, 0x80195af0), 0xffffffffffffffff);
  FUN_00105a4c(RAM, afStack2104,0xffffffff801956f0,0xffffffff801958f0);
  do {
    iVar2 = (int)(short)iVar3;
    iVar4 = iVar2 * 4;
    fVar8 = CSharedFunctions::CharArrayToFloat(RAM, param_2 + iVar4 + 0x2b0);
    fVar7 = CSharedFunctions::CharArrayToFloat(RAM, 0x801952f0 + iVar4);
	fVar5 = CSharedFunctions::CharArrayToFloat(RAM, afStack2104 + ((iVar2 * 2 + 1) * 4));
    fVar6 = CSharedFunctions::CharArrayToFloat(RAM, 0x801954f0 + iVar4);
    fVar9 = fVar8 * fVar6 - fVar5 * fVar7;
	CSharedFunctions::WriteFloatToBuffer(RAM, (param_2 + iVar4 + 0x2b0), CSharedFunctions::CharArrayToFloat(RAM, afStack2104 + iVar2 * 2 * 4));
    uVar1 = FUN_00105c84(RAM, (float)uVar10 * (fVar8 * fVar7 + fVar5 * fVar6));
	// Write output
	CSharedFunctions::WriteShortToBuffer(RAM, (iVar4 + param_1), uVar1);

	/*FILE* aa = fopen("C:\\temp\\temp.bin", "ab");
	fwrite(&uVar1, 1, 2, aa);
	fflush(aa);
	fclose(aa);*/
	CSharedFunctions::WriteShortToBuffer(outputSamples, outputSampleLength, CSharedFunctions::Flip16Bit(uVar1));
	outputSampleLength += 2;

    uVar1 = FUN_00105c84(RAM, (float)uVar10 * fVar9);
    iVar3 = iVar3 + 1;
    CSharedFunctions::WriteShortToBuffer(RAM, ((iVar2 * -2 + 0xff) * 2 + param_1), uVar1);
  } while (iVar3 * 0x10000 >> 0x10 < 0x80);
  return;
}


bool FUN_00105eb4(unsigned char* RAM, int param_1, unsigned char* outputSamples, int& outputSampleLength)
{
  unsigned int uVar1;
  
  if (0 < (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xe0)) {
    uVar1 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x5bc) + 1;
	CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x5bc), uVar1);
    if (2 < uVar1) {
      CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x5bc), 0);
    }
    unsigned long returnValue = FUN_00105560(RAM, param_1 + 0xe0);
	if (!returnValue) // Done
		return true;

	/*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
	fwrite(RAM, 1, 0x400000, a);
	fflush(a);
	fclose(a);*/

    FUN_00105cf0(RAM, param_1 + (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x5bc) * 0x200 + 0x5c4,param_1 + 0xe0, outputSamples, outputSampleLength);
	CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 0xbc8), 0x100);
    CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0xbc4), param_1 + (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x5bc) * 0x200 + 0x5c4);
    CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0xe0), (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xe0) + -1);
    //FUN_0010b350(RAM);
  }
  return false;
}

bool CAcclaimDEANAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
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
	unsigned long chunkSize = 0x2C + processedWavDataLength;
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

bool CAcclaimDEANAudioDecompression::DecompressSound(unsigned char* ROM, int romOffset, int compressedSize, CString outputFilename, float samplingRateFloat)
{
	int sizeRAM = 0x800000;
	unsigned char* RAM = new unsigned char[sizeRAM];
	for (int x = 0; x < sizeRAM; x++)
		RAM[x] = 0x00;

	int ramOffset = 0x80400000;

	CSharedFunctions::WriteLongToBuffer(RAM, 0x00139AA0, 0x80195AF8);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00139AA4, 0x80195BF8);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00139AA8, 0x80195CF8);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00139AAC, 0x00000000);

	CSharedFunctions::WriteLongToBuffer(RAM, 0x00210790, 0x00000401);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00210C6C, 0x00000000);

	//139AB0-13D434 Shorts/Pointers
	CSharedFunctions::StringToByteArray(&RAM[0x139AB0], "0000000100030007000F001F003F007F00FF01FF03FF07FF0FFF1FFF3FFF7FFFFFFF000000000001000200040008001000200040008001000200040008001000200040008000000000040008000C001200190021002C0035003F004B00590069007E009500B90100379837F137B504F337D744FD38000000381837F0383504F3385744FD38800000389837F038B504F338D744FD39000000393504F3391837F0395744FD39800000399837F039B504F339D744FD3A0000003A1837F03A3504F33A5744FD3A8000003A9837F13AB504F33AD744FD3B0000003B1837F03B3504F33B5744FD3B8000003B9837F03BB504F33BD744FD3C0000003C1837F03C3504F33C5744FC3C8000003C9837F03CB504F33CD744FD3D0000003D1837F03D3504F33D5744FD3D8000003D9837F03DB504F33DD744FD3E0000003E1837F03E3504F43E5744FD3E8000003E9837F03EB504F33ED744FD3F0000003F1837F03F3504F33F5744FD3F80000000006942E240FB72FFFFFF810000007F000000FF0000001000000101000001000000000180139D0480139EBCFFFFFF810000007F000000FF000000100000010500000100000000058013A6C88013AF60FFFFFF810000007F000000FF000000100000010900000100000000098013B78C8013C704FFFFFFC000000040FFFFFFFF00000010FFFFFFFF0000008100000000000000008013CF50000000040000000100000000000000100000000080139C8C0000000000000000000000230000000080139C200000000000000023000000590000000080139C44000000000000007C000000840000000080139C6828202031312020203027732029200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000B000000000000000000000000FFFFFFFFFFFFFFFF0001000600020005000300040800FFFE080000020800000000070200000800A70009000A0800FFFD000B000C08000004000D00A6000E00A1000F00580010003B001100200012001F0013001E0014001D0015001A00160019001700180800006F0800FF830800FFA4001B001C08000061080000580800FFD80800FFE308000016002100220800FFEA0023002A0024002900250028002600270800004E080000520800FFC20800FFD6002B0036002C002F002D002E0800FFB00800005600300033003100320800FF8908000073003400350800FF850800FF87003700380800FFCE0039003A0800FFA70800FFA9003C0057003D0042003E0041003F004008000029080000280800001E0043005200440047004500460800FFC50800003D0048004908000038004A004D004B004C080000780800FF8C004E004F0800FF9D0050005108000B000800007900530056005400550800003B080000350800FFD90800FFF000590084005A006F005B006E005C006D005D0064005E005F0800003900600063006100620800007A08000066080000530065006A0066006708000062006800690800FF980800FF92006B006C0800FFAA0800004D0800001D0800001500700075007100740072007308000027080000250800001C007600770800FFE50078008300790080007A007D007B007C0800FF880800FF99007E007F080000770800007F00810082080000540800FFAE0800FFDB0085008608000010008700880800FFEB00890092008A008B0800FFDA008C0091008D0090008E008F0800FF8A0800FF8E0800FFAB080000360093009600940095080000370800FFC80097009C0098009B0099009A08000069080000740800FFB5009D009E0800004F009F00A00800006B0800FF9300A200A30800000900A400A50800000C080009000800FFFB00A801C700A9014600AA00AB0800000500AC00C700AD00AE0800FFF700AF00B00800FFF400B100BA00B200B900B300B40800FFE400B500B60800002600B700B80800FFCC0800FFC90800FFEC00BB00BE00BC00BD0800001B0800FFE600BF00C008000A0000C100C600C200C30800FFC700C400C5080000460800FFBB0800FFDC00C800FF00C900E200CA00E100CB00E000CC00D700CD00D600CE00D300CF00D200D000D10800007E0800FF970800FFB200D400D50800FFB3080000470800002200D800DF00D900DA0800003200DB00DE00DC00DD0800FF8F0800005A0800FFB608000023080000140800FFF100E300FE00E400F500E500F400E600EF00E700E80800FFCA00E900EC00EA00EB0800FF960800006300ED00EE0800FF9B0800007200F000F300F100F20800FFB808000051080000340800001A00F600FD00F700F80800FFDD00F900FC00FA00FB0800FFB4080000480800FFCD080000190800000F010001010800FFF5010201310103010E010401050800FFE70106010D0107010C0108010B0109010A0800FF8B0800006E0800004B0800003008000024010F011A011001150111011401120113080000440800FFBF0800FFD001160119011701180800FFB90800FFBC0800FFCF011B0124011C0123011D0122011E0121011F01200800FF8108000076080000600800FFBE080000310125012A012601270800004C012801290800006C0800FF90012B0130012C012F012D012E080000750800FF820800FF950800FFB70132013308000013013401390135013801360137080000330800FFD108000021013A013B0800FFDE013C0141013D013E0800FFB1013F01400800FF9A0800006D0142014501430144080000700800FF9C080000450147014A014801490800FFFA08000006014B0184014C014D0800FFF8014E014F0800000B0150015D015101520800FFED01530154080000180155015C015601570800002E0158015B0159015A0800FF940800FF910800FFC60800FFDF015E0173015F017201600165016101620800002D016301640800004A0800003F0166016D0167016C0168016908000067016A016B0800FF840800007C08000042016E0171016F01700800FFAC0800FFA60800FFC30800FFE80174017B0175017A017601770800FFD3017801790800FFC00800004308000020017C0183017D0182017E017F0800004101800181080000680800005F0800FFD50800FFE001850186080000080187018A018801890800FFF20800000E018B01A0018C019F018D019E018E019B018F019601900193019101920800FF9E0800005E019401950800FFA20800FFA00197019A019801990800FF9F0800005C0800FFBD019C019D0800002F0800FFD40800FFE90800FFEE01A101C601A201B301A301B201A401AB01A501A801A601A7080000590800FFA501A901AA080000550800006A01AC01AD0800FFBA01AE01AF0800005D01B001B10800007B0800007D0800001F01B401B701B501B60800002B0800FFD201B801BD01B901BC01BA01BB0800FFA3080000500800004901BE01C101BF01C00800FFAD0800006401C201C501C301C4080000710800FF860800FFAF0800001201C801FF01C901FE01CA01FB01CB01CE01CC01CD0800000A0800FFF601CF01F601D001F501D101DE01D201D30800001701D401D50800FFE101D601DB01D701D80800FFC101D901DA080000650800FFA101DC01DD0800003A0800FFC401DF01E801E001E10800FFE201E201E701E301E601E401E50800005B0800FF8D080000400800002A01E901F001EA01EB0800002C01EC01ED0800003C01EE01EF080000570800FFA801F101F20800FFD701F301F40800FFCB0800003E0800FFF301F701F80800000D01F901FA0800FFEF0800001101FC01FD0800FFF9080000070800FFFC0800000302010202080000010800FFFF28202031312020203027732029200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000B000000000000000000000000FFFFFFFFFFFFFFFF282020313620202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000FFFFFFFFFFFFFFFF282020323320202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	CSharedFunctions::StringToByteArray(&RAM[0x13AAB0], "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000017000000000000000000000000FFFFFFFFFFFFFFFF28202032382020203027732029200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001C000000000000000000000000FFFFFFFFFFFFFFFF282020333220202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020000000000000000000000000FFFFFFFFFFFFFFFF0001020A0002000308000001000402090005020600060017000700160008000908000004000A000D000B000C080000060800FFFA000E0011000F001008000A0208000007001200130800FFF9001400150800FFF80800000808000003001800190800FFFD001A0201001B0200001C001D08000A01001E0175001F017200200023002100220800FFF50800000B00240073002500720026002F0027002C0028002B0029002A08000023080000250800001F002D002E080000190800FFEA0030004700310034003200330800FFE10800FFDE0035004600360037080000260038003F0039003C003A003B0800FFA80800FFCA003D003E0800FFA90800FFC80040004300410042080000570800005800440045080000690800006C0800FFE0004800490800FFE8004A0067004B0058004C0053004D0050004E004F0800FF9B0800003A005100520800FF9C0800003900540057005500560800006A0800006B0800002A00590060005A005D005B005C0800007408000075005E005F080000710800007600610064006200630800007B0800007C006500660800FFAE0800FFAF006800690800FFDD006A0071006B006E006C006D0800FFA70800FFCB006F00700800FFAB0800FFC40800002B0800FFF300740105007500760800000F007700A0007800790800FFE6007A008D007B0084007C0083007D0080007E007F0800006D0800006E008100820800FF900800004308000029008500860800FFCD0087008A008800890800FF8C08000049008B008C0800FF8D08000046008E0097008F00900800FFD500910094009200930800004708000048009500960800FF970800FF980098009908000028009A009D009B009C0800FF8B0800004A009E009F0800005E0800006100A100DA00A200C100A300B200A400AB00A500A800A600A70800FFC20800FFC300A900AA080000630800006400AC00AF00AD00AE0800004B0800004C00B000B10800FF940800003F00B300BA00B400B700B500B60800FF9E0800003700B800B9080000660800006700BB00BE00BC00BD0800005A0800005D00BF00C0080000510800005400C200D100C300CA00C400C700C500C6080000620800006500C800C90800FFAD0800FFC000CB00CE00CC00CD0800007E0800007F00CF00D00800FFAA0800FFC500D200D30800FFCE00D400D700D500D60800FFAC0800FFC100D800D90800FFB00800FFBF00DB00F400DC00E500DD00E400DE00E100DF00E00800FF850800FF8600E200E30800FF93080000400800FFD000E600ED00E700EA00E800E90800007D0800007800EB00EC0800FFA20800FFA300EE00F100EF00F0080000790800007A00F200F3080000550800005600F500FC00F600FB00F700FA00F800F90800FF960800003D0800FFB20800002F00FD010400FE010100FF01000800FFA008000035010201030800006F080000700800FFC901060171010701700108014101090122010A0113010B0112010C010F010D010E0800FF990800003C011001110800FFB80800FFBB0800FFC70114011B01150118011601170800FF8F080000440119011A0800FFB90800FFBA011C011F011D011E0800005B0800005C012001210800FF950800003E012301320124012B01250128012601270800FF91080000420129012A0800FF9208000041012C012F012D012E0800FF9A0800003B01300131080009000800FF830133013A01340137013501360800FF8E08000045013801390800FF880800FF89013B013E013C013D0800FFA10800FFD1013F01400800FFA40800FFCC0142015F0143015001440149014501460800FFC6014701480800005208000053014A014D014B014C0800FFB30800FFBD014E014F0800FFB40800FFBC0151015801520155015301540800FF9D08000038015601570800004E080000590159015C015A015B0800FF840800FF87015D015E0800FFB10800FFBE0160016F0161016801620165016301640800005F08000060016601670800FFA50800FFA60169016C016A016B0800FFB60800FFB7016D016E0800FF8A0800004D0800FFDA080000130800000E01730174080000090800FFF7017601D9017701A6017801A30179019C017A017B08000B00017C019B017D0198017E0197017F018E0180018701810184018201830800FF9F080000360185018608000072080000730188018B0189018A0800006808000077018C018D0800004F08000050018F019401900193019101920800FF810800FF820800FFD201950196080000320800FFB50800FFE50199019A08000022080000240800FFEE019D019E0800FFF1019F01A201A001A10800FFE7080000180800FFED01A401A50800000C0800FFF401A701A808000A0401A901BC01AA01B701AB01AC0800001201AD01B001AE01AF0800FFE20800001B01B101B601B201B30800002701B401B50800FFD40800FFD70800002001B801B90800FFEF01BA01BB080000150800001701BD01CA01BE01C901BF01C00800FFE901C101C20800FFE301C301C601C401C50800FFD60800FFD801C701C80800FFCF0800002D0800001101CB01D401CC01CD0800001601CE01CF0800001A01D001D301D101D20800002C0800002E0800FFDB01D501D801D601D70800001C0800001D0800001401DA01DD01DB01DC08000A030800000A01DE01DF0800FFF601E001F101E101E20800FFF201E301E40800FFF001E501F001E601E901E701E8080000210800001E01EA01EF01EB01EE01EC01ED08000034080000300800FFDC0800FFD90800FFEC01F201FF01F301FE01F401F50800FFEB01F601FD01F701FC01F801FB01F901FA08000033080000310800FFD30800FFDF0800FFE4080000100800000D08000A000202020502030204080000050800FFFB0800FFFC02070208080000020800FFFE0800FFFF08000000282020203820202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000008000000000000000000000000FFFFFFFFFFFFFFFF28202031342020203027732029200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	CSharedFunctions::StringToByteArray(&RAM[0x13BAB0], "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000E000000000000000000000000FFFFFFFFFFFFFFFF282020313820202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012000000000000000000000000FFFFFFFFFFFFFFFF282020323220202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000016000000000000000000000000FFFFFFFFFFFFFFFF282020323420202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000018000000000000000000000000FFFFFFFFFFFFFFFF28202032362020203027732029200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001A000000000000000000000000FFFFFFFFFFFFFFFF28202032382020203027732029200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001C000000000000000000000000FFFFFFFFFFFFFFFF28202033302020203027732029200000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001E000000000000000000000000FFFFFFFFFFFFFFFF282020333220202030277320292000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000020000000000000000000000000FFFFFFFFFFFFFFFF000102120002000308000001000402110005001A000600070800FFFE0008001900090018000A0015000B000C08000A08000D000E08000A05000F001008000A070011001400120013080000070800FFF908000006001600170800FFFC0800000408000A0108000B00001B001C08000A00001D0210001E020D001F002008000A020021020A0022020700230206002400250800FFFA002600AF0027002A002800290800FFF708000009002B003C002C003B002D002E0800FFF5002F0036003000310800001000320035003300340800001B080000440800FFEC003700380800FFF10039003A08000016080000190800FFF6003D004E003E003F0800000B0040004B004100420800FFEF004300440800FFEB00450048004600470800FFAE0800FFB30049004A0800FFD50800FFD6004C004D0800000E0800FFF3004F0070005000510800000D005200610053005A00540057005500560800FFD00800FFD1005800590800FF940800FF95005B005E005C005D0800002708000028005F00600800002308000024006200690063006600640065080000580800005B006700680800FFDE0800FFDF006A006D006B006C0800FFBE08000050006E006F080000320800003500710090007200810073007A00740077007500760800007308000074007800790800004C0800004F007B007E007C007D0800001D08000043007F00800800002108000041008200890083008600840085080000260800003F008700880800002D0800003E008A008D008B008C0800FFBF08000049008E008F0800FFCC0800FFCD009100A00092009900930096009400950800002E0800003B009700980800002F0800003A009A009D009B009C0800FFC40800FFC5009E009F0800001F0800002000A100A800A200A500A300A40800FFCE0800FFCF00A600A7080000760800007700A900AC00AA00AB0800FFEA0800001300AD00AE080000790800007A00B0020300B1017C00B2010D00B300EE00B400D100B500C200B600BB00B700BA00B800B9080009000800FFC30800001A00BC00BF00BD00BE080000530800005400C000C1080000660800006700C300CA00C400C700C500C60800FFA60800FFA700C800C90800FFA40800FFA900CB00CE00CC00CD0800FFBA0800FFBD00CF00D00800003C0800003D00D200DF00D300DA00D400D700D500D60800FFBB0800FFBC00D800D90800FFAA0800FFAB00DB00DE00DC00DD0800FFB10800FFB20800FFC000E000E700E100E400E200E3080000600800006500E500E60800FFB40800FFB500E800EB00E900EA0800001E08000042");
	CSharedFunctions::StringToByteArray(&RAM[0x13CAB0], "00EC00ED080000550800006A00EF010C00F000FF00F100F800F200F500F300F40800FFA00800FFA300F600F7080000360800003700F900FC00FA00FB0800005C0800005D00FD00FE0800FF8C0800FF8D01000105010101020800FFED010301040800FF910800FF9201060109010701080800001708000018010A010B08000033080000340800000C010E0143010F012601100119011101120800FFE701130116011401150800FFE20800FF89011701180800FFE40800FF88011A011F011B011C08000012011D011E080000750800FFE101200123012101220800FFA50800FFA8012401250800FFDC0800FFE0012701340128012F0129012C012A012B0800FFE50800FF85012D012E080000560800005701300133013101320800FFAF0800FFB00800FFE30135013C01360139013701380800007C0800007F013A013B0800006308000064013D0140013E013F0800FFE80800FF84014101420800FFE90800FF8301440163014501540146014D0147014A014801490800004708000048014B014C0800FFC90800FFCA014E0151014F01500800FF8B0800FF8E015201530800FF9C0800FF9F0155015C01560159015701580800FF900800FF9B015A015B0800FF970800FF98015D0160015E015F0800005108000052016101620800007108000072016401730165016C01660169016701680800003808000039016A016B0800002B0800002C016D0170016E016F0800FF810800FF8201710172080000780800007B017401750800FFF201760179017701780800FFAC0800FFAD017A017B0800006E0800006F017D01D8017E019B017F01800800FFF40181018E0182018901830186018401850800006C0800006D018701880800007D0800007E018A018B08000022018C018D0800FF8A0800FF8F018F019601900193019101920800FFB60800FFB7019401950800FF930800FF9A019701980800FFE60199019A080000290800002A019C01BB019D01AC019E01A5019F01A201A001A10800FF960800FF9901A301A4080000610800006201A601A901A701A8080000140800004601AA01AB080000150800004501AD01B401AE01B101AF01B0080000300800003101B201B30800FFC60800FFC701B501B801B601B70800004A0800004B01B901BA0800005E0800005F01BC01CB01BD01C401BE01C101BF01C00800FFEE0800001101C201C30800004D0800004E01C501C801C601C7080000250800004001C901CA0800FFB80800FFB901CC01D101CD01CE0800001C01CF01D00800FF9D0800FF9E01D201D501D301D4080000680800006901D601D70800FFA10800FFA201D9020201DA01E301DB01E201DC01E101DD01DE0800FFDD01DF01E00800FFD20800FFD30800000F0800FFF001E401F301E501EC01E601E901E701E8080000590800005A01EA01EB0800FFC10800FFC201ED01F001EE01EF080000700800006B01F101F20800FFD70800FFD801F401FB01F501F801F601F70800FF860800FF8701F901FA0800FFD40800FFDB01FC01FF01FD01FE0800FFD90800FFDA020002010800FFC80800FFCB0800000A02040205080000080800FFF808000A06020802090800FFFB08000005020B020C08000A0408000A03020E020F080000030800FFFD080000020800FFFF080000000001001E0002001D0003000408000002000500060800FFFD0007001C000800090800FFFA000A000B08000008000C001B000D000E0800000C000F001A001000190011001800120017001300140800001A001500160800FFE30800001E0800FFE60800FFE9080000110800000E0800000A0800FFFC08000000001F003A00200039002100220800000300230038002400250800000600260037002700280800FFF600290036002A0035002B002C0800FFEC002D0034002E0033002F00300800FFE4003100320800FFE20800001D0800FFE7080000150800FFF10800FFF40800FFF9080000040800FFFF003B003C08000001003D0100003E00A3003F00A2004000A1004100A00042009F0043004600440045080000100800FFED0047009E004800490800FFEA004A009D004B009C004C0065004D004E0800FFE1004F005C005000550051005208000026005300540800003F0800004000560059005700580800FFD00800FFD1005A005B0800FFC40800FFC5005D005E0800FFDE005F0062006000610800FFC00800FFC1006300640800FFC80800FFC90066007D006700760068006F0069006C006A006B0800003108000034006D006E080000350800003600700073007100720800003D0800FFCB007400750800FFCF0800FFD20077007C007800790800FFDC007A007B080000250800002908000024007E008D007F008600800083008100820800FFD80800002A008400850800FFDB0800FFDD0087008A008800890800003E0800FFCA008B008C0800FFC60800FFC7008E0095008F0092009000910800002B0800002E00930094080000390800FFCD00960099009700980800003A0800FFCC009A009B080000270800002808000019080000170800FFEE0800000B0800FFF7080000070800FFFB00A400A50800000500A600AD00A700A80800000900A900AC00AA00AB0800FFF20800000D0800FFF500AE00FF00AF00EA00B000E500B100E400B200B30800001400B400B50800FFE800B600B70800FFE500B800E300B900D800BA00C900BB00C200BC00BF00BD00BE0800003B0800003C00C000C10800002C0800002D00C300C600C400C50800FFC20800FFC300C700C80800FFCE0800FFD500CA00D100CB00CE00CC00CD0800FFD30800FFD400CF00D00800002F0800003000D200D500D300D40800FFD60800FFD700D600D7080000370800003800D900E200DA00E100DB00DE00DC00DD0800FFD90800FFDA00DF00E0080000320800003308000021080000200800001C0800000F00E600E900E700E808000013080000120800FFEF00EB00FE00EC00FD00ED00FC00EE00EF0800001600F000FB00F100FA00F200F30800001F00F400F50800FFE000F600F900F700F808000022080000230800FFDF0800001B080000180800FFEB0800FFF00800FFF30800FFF80800FFFE94B859D400CE59F05AE608380000000000000000000000000000000000013000801D93008325DE188D853A93000000010002FFFF002008007FFF40001118147011DC1B381214187C125412D012EC1328140C12941E24138C170C144CF0000F0000F0000F00010010010010000002000400060008000A000C000E00100001000100010001000100010001000100000001000200040008001001000200000100000000000000010000000000000000000100000000000000010000000000000000000100000000000000010000000000000000000100000000000000012000400060008000A000C000E000FFFF0C3966AD0D46FFDF0B3966960E5FFFD80A4466690F83FFD0095A662610B4FFC8087D65CD11F0FFBF07AB655E1338FFB606E464D9148CFFAC0628643F15EBFFA10577638F1756FF9604D162CB18CBFF8A043561F31A4CFF7E03A461061BD7FF71031C60071D6CFF64029F5EF51F0BFF56022A5DD020B3FF4801BE5C9A2264FF3A015B5B53241EFF2C010159FC25E0FF1E00AE589627A9FF1000635720297AFF02001F559D2B50FEF4FFE2540D2D2CFEE8FFAC52702F0DFEDBFF7C50C730F3FED0FF534F1432DCFEC6FF2E4D5734C8FEBDFF0F4B9136B6FEB6FEF549C238A5FEB0FEDF47ED3A95FEACFECE46113C85FEABFEC044303E74FEACFEB6424A4060FEAFFEAF4060424AFEB6FEAC3E744430FEC0FEAB3C854611FECEFEAC3A9547EDFEDFFEB038A549C2FEF5FEB636B64B91FF0FFEBD34C84D57FF2EFEC632DC4F14FF53FED030F350C7FF7CFEDB2F0D5270FFACFEE82D2C540DFFE2FEF42B50559D001FFF02297A57200063FF1027A9589600AEFF1E25E059FC0101FF2C241E5B53015BFF3A22645C9A01BEFF4820B35DD0022AFF561F0B5EF5029FFF641D6C6007031CFF711BD7610603A4FF7E1A4C61F30435FF8A18CB62CB04D1FF961756638F0577FFA115EB643F0628FFAC148C64D906E4FFB61338655E07ABFFBF11F065CD087DFFC810B46626095AFFD00F8366690A44FFD80E5F66960B39FFDF0D4666AD0C39000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000ED000000005003C0EF080CFF00000000000000000000000000000000FFFF004800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000525350204766782075636F6465204633444558202020202020206669666F20322E30382020596F73686974616B6120596173756D6F746F2031393939204E696E74656E646F2E0A0000010000000000020000000100000002000100000000FFFE000000010000FFFE000000000001FFFF0000000000010001FFFF000400087F00FFFC400004207FFF7FFC140010000100FFF0FFF800100020C00044D36CB300020001000001000BA80000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000F800097100000001018016F100009D0000009D0004000E001F00420008001DA018000F801E0022001D800DA11901078119011941194119411C01028104C11D810F0103811201FAC100011EC119410301140113C102C102C1214121412141214121012101210124C121012401210120012101210121012101210121012101210121012101210122012201220121414F81C801C401C6817BC17B417B41194042004480470049804C004E8051005380560058805B005D8060006280650067806A006C806F0071807400768079007B807E0080808300858088008A808D008F80920FFFF8000000000008000303040400000000000000000000000000000000000000000000000000000000000000000000000000000000000100000002000001000000020000000000040000000004000001188020712D800000258021F12D80000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

	CSharedFunctions::WriteShortToBuffer(RAM, 0x00210A3C, 0x0000);
	CSharedFunctions::WriteShortToBuffer(RAM, 0x00210A3E, 0x0000);
	CSharedFunctions::WriteShortToBuffer(RAM, 0x00210C44, 0x0000);

	//144CA0-00144DC0 Floats/Doubles
	CSharedFunctions::StringToByteArray(&RAM[0x144CA0], "000000003D0000002D0000005E6E00003F3000000000000040B00000000000003AC90FDB458000003F0000003C8EFA353F8000003F8000003F8000003C8EFA353F8000003F8000003F800000478000003F80000000000000000000000000000041F0000000000000427000000000000041F000000000000000000000000000004120000046FFFE0043C80000412000003F800000467FFC0000000000000000003F8000003F800000408F400000000000408F4000000000004092C000000000004496000045BB80004270000042B4000046FFFE00000000000000000000000000408000003F80000040C90FDB3A00000040C90FDB3B000000401921FB54442EEA380000003F00000046FFFE003F000000C700000046B504F30000000000000000");


	CSharedFunctions::WriteLongToBuffer(RAM, 0x80210794 & 0xFFFFFF, ramOffset);
	memcpy(&RAM[ramOffset & 0xFFFFFF], &ROM[romOffset], compressedSize);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x80210790 & 0xFFFFFF, compressedSize);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x80210C68 & 0xFFFFFF, compressedSize / 2);

	unsigned long A0 = 0x802106B0;

	unsigned char* outputSamples = new unsigned char[0x500000];
	int outputSampleLength = 0;


	for (int x = 0; x< 0x10000; x++)
	{
		if (FUN_00105eb4(RAM, A0, outputSamples, outputSampleLength))
			break;
	}
	
	bool result = SaveProcessedWav(outputSamples, outputSampleLength, 1, samplingRateFloat, outputFilename);

	delete [] outputSamples;
	delete [] RAM;

	return result;
}