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

void FUN_00008c38(unsigned char* RAM, unsigned long param_1, int param_2)
{
  //unsigned int **param_1
  short sVar1;
  unsigned int uVar2;
  int iVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  unsigned long psVar7; //short *
  short sVar8;
  unsigned long psVar9; //short *
  unsigned long puVar10; // unsigned int *
  unsigned long puVar11; // unsigned int *
  unsigned long puVar12; // unsigned int *
  int iVar13;
  
  sVar5 = (short)param_2;
  if (sVar5 == 0) {
    return;
  }
  puVar12 = CSharedFunctions::CharArrayToLong(RAM, param_1);
  puVar11 = CSharedFunctions::CharArrayToLong(RAM,param_1 + 1 * 4);
  sVar4 =  (short)CSharedFunctions::CharArrayToShort(RAM, param_1 + 2 * 4);
  sVar8 = (short)CSharedFunctions::CharArrayToShort(RAM, (int)param_1 + 10);
  sVar1 = (short)CSharedFunctions::CharArrayToShort(RAM, param_1 + 0xe);
  iVar13 = CSharedFunctions::CharArrayToShort(RAM, param_1 + 3 * 4) - 1;
  if (sVar1 == 2) {
    if (0 < sVar5) {
      psVar9 = ((int)puVar11 + 0x16);
      do {
        uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
        iVar6 = (uVar2 & 0xff) * 8 + 0x800472D0;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 8 & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
		CSharedFunctions::WriteShortToBuffer(RAM, puVar11, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 10 & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -10 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xc & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -9 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xe & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -8 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x10 & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -7 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x12 & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -6 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x14 & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -5 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x16 & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -4 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x18 & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -3 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1a & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -2 * 2, sVar4);
        puVar12 = puVar12 + 1 * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1c & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -1 * 2, sVar4);
        puVar10 = puVar11 + 6 * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1e & 3U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9, sVar4);
        psVar9 = psVar9 + 0xc * 2;
        if (0 < iVar13 * 0x10000) {
          psVar7 = ((int)puVar11 + 0x36);
          iVar3 = iVar13;
          do {
			  uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, (uVar2 & 3) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
			CSharedFunctions::WriteShortToBuffer(RAM, puVar10, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 2 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xe * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 4 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xd * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 6 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xc * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 8 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xb * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 10 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -10 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xc & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -9 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xe & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -8 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x10 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -7 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x12 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -6 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x14 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -5 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x16 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -4 * 2, sVar4);
            puVar12 = puVar12 + 1 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x18 & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -3 * 2, sVar4);
            psVar9 = psVar9 + 0x10 * 2;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1a & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -2 * 2, sVar4);
            puVar10 = puVar10 + 8 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1c & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -1 * 2, sVar4);
            iVar3 = iVar3 + -1;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1e & 3U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7, sVar4);
            psVar7 = psVar7 + 0x10 * 2;
          } while (0 < iVar3 * 0x10000);
        }
        puVar11 = puVar10;
        param_2 = param_2 + -1;
      } while (0 < param_2 * 0x10000);
	  CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 2 * 4), sVar4);
      goto LAB_00009d34;
    }
  }
  else if (sVar1 < 3) {
    if (sVar1 != 1) {
      CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 2 * 4), sVar4);
      goto LAB_00009d34;
    }
    if (0 < sVar5) {
      psVar9 = ((int)puVar11 + 0x2e);
      do {
		  uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
        iVar6 = (uVar2 & 0xff) * 4 + 0x80046ED0;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 8 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
		CSharedFunctions::WriteShortToBuffer(RAM, puVar11, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 9 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0x16 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 10 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0x15 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xb & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0x14 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xc & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0x13 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xd & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0x12 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xe & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0x11 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xf & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0x10 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x10 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0xf * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x11 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0xe * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x12 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0xd * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x13 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0xc * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x14 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -0xb * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x15 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -10 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x16 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -9 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x17 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -8 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x18 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -7 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x19 & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -6 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1a & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -5 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1b & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -4 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1c & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -3 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1d & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -2 * 2, sVar4);
        puVar12 = puVar12 + 1 * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1e & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -1 * 2, sVar4);
        puVar10 = puVar11 + 0xc * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1f & 1U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9, sVar4);
        psVar9 = psVar9 + 0x18 * 2;
        if (0 < iVar13 * 0x10000) {
          psVar7 = ((int)puVar11 + 0x6e);
          iVar3 = iVar13;
          do {
			  uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, (uVar2 & 1) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
			CSharedFunctions::WriteShortToBuffer(RAM, puVar10, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 1 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
			CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x1e * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 2 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x1d * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 3 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x1c * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 4 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x1b * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 5 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x1a * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 6 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x19 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 7 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x18 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 8 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x17 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 9 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x16 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 10 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x15 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xb & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x14 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xc & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x13 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xd & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x12 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xe & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x11 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xf & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0x10 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x10 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xf * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x11 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xe * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x12 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xd * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x13 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xc * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x14 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -0xb * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x15 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -10 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x16 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -9 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x17 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -8 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x18 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -7 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x19 & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -6 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1a & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -5 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1b & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -4 * 2, sVar4);
            puVar12 = puVar12 + 1 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1c & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -3 * 2, sVar4);
            psVar9 = psVar9 + 0x20 * 2;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1d & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -2 * 2, sVar4);
            puVar10 = puVar10 + 0x10 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1e & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -1 * 2, sVar4);
            iVar3 = iVar3 + -1;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1f & 1U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7, sVar4);
            psVar7 = psVar7 + 0x20 * 2;
          } while (0 < iVar3 * 0x10000);
        }
        puVar11 = puVar10;
        param_2 = param_2 + -1;
      } while (0 < param_2 * 0x10000);
	  CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 2 * 4), sVar4);
      goto LAB_00009d34;
    }
  }
  else if (sVar1 == 3) {
    if (0 < sVar5) {
      psVar9 = ((int)puVar11 + 0xe);
      do {
		  uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
        iVar6 = (uVar2 & 0xff) * 0x10 + 0x80047AD0;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 8 & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
		CSharedFunctions::WriteShortToBuffer(RAM, puVar11, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xb & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -6 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xe & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -5 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x11 & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -4 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x14 & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -3 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x17 & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -2 * 2, sVar4);
        puVar12 = puVar12 + 1 * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1a & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -1 * 2, sVar4);
        puVar10 = puVar11 + 4 * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1d & 7U) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9, sVar4);
        psVar9 = psVar9 + 8 * 2;
        if (0 < iVar13 * 0x10000) {
          psVar7 = ((int)puVar11 + 0x22);
          iVar3 = iVar13;
          do {
			  uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, (uVar2 & 7) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
			CSharedFunctions::WriteShortToBuffer(RAM, puVar10, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 3 & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
			CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -8 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 6 & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -7 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 9 & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -6 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xc & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -5 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xf & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -4 * 2, sVar4);
            puVar12 = puVar12 + 1 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x12 & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -3 * 2, sVar4);
            psVar9 = psVar9 + 10 * 2;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x15 & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -2 * 2, sVar4);
            puVar10 = puVar10 + 5 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x18 & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -1 * 2, sVar4);
            iVar3 = iVar3 + -1;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1b & 7U) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7, sVar4);
            psVar7 = psVar7 + 10 * 2;
          } while (0 < iVar3 * 0x10000);
        }
        puVar11 = puVar10;
        param_2 = param_2 + -1;
      } while (0 < param_2 * 0x10000);
	  CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 2 * 4), sVar4);
      goto LAB_00009d34;
    }
  }
  else {
    if (sVar1 != 4) {
      CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 2 * 4), sVar4);
      goto LAB_00009d34;
    }
    if (0 < sVar5) {
      psVar9 = ((int)puVar11 + 10);
      puVar10 = puVar11;
      do {
		  uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
        iVar6 = (uVar2 & 0xff) * 0x20 + 0x80048AD0;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 8 & 0xfU) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
		CSharedFunctions::WriteShortToBuffer(RAM, puVar10, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xc & 0xfU) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -4 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x10 & 0xfU) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -3 * 2, sVar4);
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x14 & 0xfU) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -2 * 2, sVar4);
        puVar12 = puVar12 + 1 * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x18 & 0xfU) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9 + -1 * 2, sVar4);
        puVar11 = puVar10 + 3 * 4;
        sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1c & 0xfU) * 2 + iVar6);
        sVar4 = sVar4 + sVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, psVar9, sVar4);
        psVar9 = psVar9 + 6 * 2;
        if (0 < iVar13 * 0x10000) {
          psVar7 = ((int)puVar10 + 0x1a);
          iVar3 = iVar13;
          do {
			  uVar2 = CSharedFunctions::CharArrayToLong(RAM, puVar12);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, (uVar2 & 0xf) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
			CSharedFunctions::WriteShortToBuffer(RAM, puVar11, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 4 & 0xfU) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
			CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -6 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 8 & 0xfU) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -5 * 2, sVar4);
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0xc & 0xfU) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -4 * 2, sVar4);
            puVar12 = puVar12 + 1 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x10 & 0xfU) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -3 * 2, sVar4);
            psVar9 = psVar9 + 8 * 2;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x14 & 0xfU) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -2 * 2, sVar4);
            puVar11 = puVar11 + 4 * 4;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x18 & 0xfU) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7 + -1 * 2, sVar4);
            iVar3 = iVar3 + -1;
            sVar8 = sVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, ((int)uVar2 >> 0x1c & 0xfU) * 2 + iVar6);
            sVar4 = sVar4 + sVar8;
            CSharedFunctions::WriteShortToBuffer(RAM, psVar7, sVar4);
            psVar7 = psVar7 + 8 * 2;
          } while (0 < iVar3 * 0x10000);
        }
        param_2 = param_2 + -1;
        puVar10 = puVar11;
      } while (0 < param_2 * 0x10000);
    }
  }
  CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 2 * 4), sVar4);
LAB_00009d34:
  CSharedFunctions::WriteShortToBuffer(RAM, ((int)param_1 + 10), sVar8);
  CSharedFunctions::WriteLongToBuffer(RAM, param_1, puVar12);
  CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 1 * 4, puVar11);

  /*FILE* outRAM = fopen("C:\\temp\\RAM.bin", "wb");
	fwrite(RAM, 1, 0x400000, outRAM);
	fclose(outRAM);*/
  return;
}


unsigned long FUN_00009e84(unsigned char* RAM, int param_1)
{
	//returns int *
  unsigned short uVar1;
  unsigned short uVar2;
  int iVar3;
  unsigned int uVar4;
  int iVar5;
  short sVar8;
  int iVar6;
  unsigned long puVar7; //int *
  unsigned int uVar9;
  unsigned long puVar10; //unsigned short *
  int iVar11;
  unsigned long puVar12; //int *
  
  uVar1 = CSharedFunctions::CharArrayToShort(RAM, param_1 + 0xc0);
  iVar11 = param_1 + 0x30;
  iVar3 = param_1 + (short)CSharedFunctions::CharArrayToShort(RAM, param_1 + 0xce) * 0x10;
  puVar12 = CSharedFunctions::CharArrayToLong(RAM, iVar3 + 8); // Output spot
  uVar9 = (unsigned int)(CSharedFunctions::CharArrayToShort(RAM, iVar3 + 6) >> 1);
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x30), CSharedFunctions::CharArrayToLong(RAM, iVar3 + 0xc));
  if ((short)uVar1 != 0) {
    uVar4 = (unsigned int)uVar1;
    if ((int)uVar9 < (int)(short)uVar1) {
      uVar4 = uVar9;
    }
	puVar10 = (iVar11 + CSharedFunctions::CharArrayToShort(RAM, param_1 + 0xc2) * 2 + 0x10);
    uVar9 = uVar9 - uVar4;
    iVar3 = uVar4 << 0x10;
    while (0 < iVar3) {
      uVar2 = CSharedFunctions::CharArrayToShort(RAM, puVar10);
      puVar10 = puVar10 + 1 * 2;
      uVar4 = uVar4 - 1;
	  CSharedFunctions::WriteShortToBuffer(RAM, puVar12, uVar2);
      puVar12 = ((int)puVar12 + 2);
      iVar3 = uVar4 * 0x10000;
    }
	CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 0xc0), 0);
    if ((uVar9 & 0xffff) == 0) goto LAB_0000a0c0;
  }
  sVar8 = (short)CSharedFunctions::CharArrayToShort(RAM, param_1 + 0xc4);
  iVar5 = (int)(short)uVar9;
  iVar3 = iVar5 / (int)sVar8;
  iVar6 = iVar5 % (int)sVar8;
  if (sVar8 == 0) {
    //trap(0x1c00);
	throw;
  }
  if ((sVar8 == -1) && (iVar5 == -0x80000000)) {
    //trap(0x1800);
	throw;
  }
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x34), puVar12);
  iVar5 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 200);
  if (iVar5 < (short)iVar3) {
    iVar6 = 0;
    iVar3 = iVar5;
  }
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 200), iVar5 - (short)iVar3);
  FUN_00008c38(RAM, iVar11, iVar3); // iVar3 added...
  puVar12 = CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x34);
  sVar8 = (short)iVar6;
  if (sVar8 == 0) {
	  uVar9 = uVar9 - iVar3 * (unsigned int)CSharedFunctions::CharArrayToShort(RAM, param_1 + 0xc4);
    if (0 < (int)(uVar9 * 0x10000)) {
      if ((uVar9 & 1) != 0) {
		  CSharedFunctions::WriteShortToBuffer(RAM, puVar12, 0);
        puVar12 = ((int)puVar12 + 2);
        uVar9 = uVar9 - 1;
      }
      iVar3 = (int)(uVar9 << 0x10) >> 0x14;
      if (iVar3 != 0) {
        puVar7 = puVar12 + 7 * 4;
        do {
			CSharedFunctions::WriteLongToBuffer(RAM, puVar12, 0);
		  CSharedFunctions::WriteLongToBuffer(RAM, puVar7 + -6 * 4, 0);
          CSharedFunctions::WriteLongToBuffer(RAM, puVar7 + -5 * 4, 0);
          CSharedFunctions::WriteLongToBuffer(RAM, puVar7 + -4 * 4, 0);
          CSharedFunctions::WriteLongToBuffer(RAM, puVar7 + -3 * 4, 0);
          CSharedFunctions::WriteLongToBuffer(RAM, puVar7 + -2 * 4, 0);
          CSharedFunctions::WriteLongToBuffer(RAM, puVar7 + -1 * 4, 0);
          CSharedFunctions::WriteLongToBuffer(RAM, puVar7, 0);
          puVar7 = puVar7 + 8 * 4;
          iVar3 = iVar3 + -1;
          puVar12 = puVar12 + 8 * 4;
        } while (iVar3 != 0);
      }
      uVar4 = (int)(uVar9 << 0x10) >> 0x11 & 7;
      uVar9 = uVar4;
      while (uVar9 != 0) {
		  CSharedFunctions::WriteLongToBuffer(RAM, puVar12, 0);
        uVar4 = uVar4 - 1;
        puVar12 = puVar12 + 1 * 4;
        uVar9 = uVar4 & 0xffff;
      }
    }
  }
  else {
    puVar10 = (param_1 + 0x40);
	CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x34), puVar10);
	CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 200), (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 200) + -1);
    FUN_00008c38(RAM, iVar11,1);

	/*FILE* outRAM = fopen("C:\\temp\\RAM.bin", "wb");
	fwrite(RAM, 1, 0x400000, outRAM);
	fclose(outRAM);*/

	CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 0xc2), sVar8);
	CSharedFunctions::WriteShortToBuffer(RAM, (param_1 + 0xc0), (short)CSharedFunctions::CharArrayToShort(RAM, param_1 + 0xc4) - sVar8);
    if (0 < sVar8) {
      do {
		  uVar2 = CSharedFunctions::CharArrayToShort(RAM, puVar10);
        puVar10 = puVar10 + 1 * 2;
        iVar6 = iVar6 + -1;
		CSharedFunctions::WriteShortToBuffer(RAM, puVar12, uVar2);
        puVar12 = ((int)puVar12 + 2);
      } while (0 < iVar6 * 0x10000);
    }
  }
LAB_0000a0c0:
  //FUN_0000a0f8();
  return puVar12;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00037a80(unsigned char* RAM)

{
  int iVar1;
  short sVar2;
  
  do {
    //FUN_000082a8(RAM, 0xffffffff800d1f78,0,1);
	  iVar1 = (int)CSharedFunctions::CharArrayToLong(RAM, CSharedFunctions::CharArrayToLong(RAM, 0x80056a28) * 4 + 0x800CF7B0);
	  if ((short)CSharedFunctions::CharArrayToShort(RAM, iVar1 + 0xd2) == 1) {
		FUN_00009e84(RAM, iVar1);
	  }
	  CSharedFunctions::WriteLongToBuffer(RAM, 0x80056a28, CSharedFunctions::CharArrayToLong(RAM, 0x80056a28) + 1);
    if (0x1f < CSharedFunctions::CharArrayToLong(RAM, 0x80056a28)) {
      CSharedFunctions::WriteLongToBuffer(RAM, 0x80056a28, 0);
    }
    sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, iVar1 + 0xce) + 1;
	CSharedFunctions::WriteShortToBuffer(RAM, (iVar1 + 0xce), sVar2);
	if ((int)(unsigned int)CSharedFunctions::CharArrayToShort(RAM, iVar1 + 0xcc) <= (int)sVar2) {
		CSharedFunctions::WriteShortToBuffer(RAM, (iVar1 + 0xce), 0);
    }
    CSharedFunctions::WriteShortToBuffer(RAM, (iVar1 + 0xd4), 0);
  } while( true );
}

bool CAcclaimDEANAudioDecompression::DecompressMARKSound(unsigned char* ROM, int romOffset, int compressedSize, CString outputFilename, float samplingRateFloat, int scale, int type, int chunk, int chunkAmount)
{
	int sizeRAM = 0x800000;
	unsigned char* RAM = new unsigned char[sizeRAM];
	for (int x = 0; x < sizeRAM; x++)
		RAM[x] = 0x00;

	unsigned long A0 = 0x80083840;

	CSharedFunctions::WriteShortToBuffer(RAM, 0x83900, 0x0000);
	CSharedFunctions::WriteShortToBuffer(RAM, 0x8390E, 0x0000);
	CSharedFunctions::WriteShortToBuffer(RAM, 0x83904, chunk); //0x0036
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00083878, 0x00000000);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x0008387C, (scale << 16) | type); // 0x00070004

	CSharedFunctions::StringToByteArray(&RAM[0x46ED0], "FFE00020FFC00040FFA00060FF800080FF5F00A1FF3F00C1FF1F00E1FEFF0101FEDE0122FEBE0142FE9E0162FE7E0182FE5E01A2FE3D01C3FE1D01E3FDFD0203FDDD0223FDBC0244FD9C0264FD7C0284FD5C02A4FD3C02C4FD1B02E5FCFB0305FCDB0325FCBB0345FC9A0366FC7A0386FC5A03A6FC3A03C6FC1903E7FBF90407FBD90427FBB90447FB990467FB780488FB5804A8FB3804C8FB1804E8FAF70509FAD70529FAB70549FA970569FA770589FA5605AAFA3605CAFA1605EAF9F6060AF9D5062BF9B5064BF995066BF975068BF95406ACF93406CCF91406ECF8F4070CF8D4072CF8B3074DF893076DF873078DF85307ADF83207CEF81207EEF7F2080EF7D2082EF7B2084EF791086FF771088FF75108AFF73108CFF71008F0F6F00910F6D00930F6B00950F6900970F66F0991F64F09B1F62F09D1F60F09F1F5EE0A12F5CE0A32F5AE0A52F58E0A72F56D0A93F54D0AB3F52D0AD3F50D0AF3F4ED0B13F4CC0B34F4AC0B54F48C0B74F46C0B94F44B0BB5F42B0BD5F40B0BF5F3EB0C15F3CB0C35F3AA0C56F38A0C76F36A0C96F34A0CB6F3290CD7F3090CF7F2E90D17F2C90D37F2A80D58F2880D78F2680D98F2480DB8F2280DD8F2070DF9F1E70E19F1C70E39F1A70E59F1860E7AF1660E9AF1460EBAF1260EDAF1060EFAF0E50F1BF0C50F3BF0A50F5BF0850F7BF0640F9CF0440FBCF0240FDCF0040FFCEFE3101DEFC3103DEFA3105DEF83107DEF63109DEF4210BEEF2210DEEF0210FEEEE2111EEEC1113FEEA1115FEE81117FEE61119FEE4111BFEE2011E0EE001200EDE01220EDC01240ED9F1261ED7F1281ED5F12A1ED3F12C1ED1F12E1ECFE1302ECDE1322ECBE1342EC9E1362EC7D1383EC5D13A3EC3D13C3EC1D13E3EBFC1404EBDC1424EBBC1444EB9C1464EB7C1484EB5B14A5EB3B14C5EB1B14E5EAFB1505EADA1526EABA1546EA9A1566EA7A1586EA5A15A6EA3915C7EA1915E7E9F91607E9D91627E9B81648E9981668E9781688E95816A8E93716C9E91716E9E8F71709E8D71729E8B71749E896176AE876178AE85617AAE83617CAE81517EBE7F5180BE7D5182BE7B5184BE795186BE774188CE75418ACE73418CCE71418ECE6F3190DE6D3192DE6B3194DE693196DE673198DE65219AEE63219CEE61219EEE5F21A0EE5D11A2FE5B11A4FE5911A6FE5711A8FE5501AB0E5301AD0E5101AF0E4F01B10E4D01B30E4AF1B51E48F1B71E46F1B91E44F1BB1E42E1BD2E40E1BF2E3EE1C12E3CE1C32E3AE1C52E38D1C73E36D1C93E34D1CB3E32D1CD3E30C1CF4E2EC1D14E2CC1D34E2AC1D54E28B1D75E26B1D95E24B1DB5E22B1DD5E20B1DF5E1EA1E16E1CA1E36E1AA1E56E18A1E76E1691E97E1491EB7E1291ED7E1091EF7E0E91F17E0C81F38E0A81F58E0881F78E0681F98E0471FB9E0271FD9E0071FF9DFE72019DFC6203AFFD3FFF70009002DFFA5FFED0013005BFF77FFE3001D0089FF49FFD9002700B7FF1BFFCF003100E5FEEDFFC5003B0113FEBFFFBB00450141FE91FFB1004F016FFE63FFA70059019DFE36FF9D006301CAFE08FF93006D01F8FDDAFF8A00760226FDACFF8000800254FD7EFF76008A0282FD50FF6C009402B0FD22FF62009E02DEFCF4FF5800A8030CFCC6FF4E00B2033AFC98FF4400BC0368FC6BFF3A00C60395FC3DFF3000D003C3FC0FFF2600DA03F1FBE1FF1C00E4041FFBB3FF1300ED044DFB85FF0900F7047BFB57FEFF010104A9FB29FEF5010B04D7FAFBFEEB01150505FACDFEE1011F0533FAA0FED701290560FA72FECD0133058EFA44FEC3013D05BCFA16FEB9014705EAF9E8FEAF01510618F9BAFEA6015A0646F98CFE9C01640674F95EFE92016E06A2F930FE88017806D0F902FE7E018206FEF8D5FE74018C072BF8A7FE6A01960759F879FE6001A00787F84BFE5601AA07B5F81DFE4C01B407E3F7EFFE4201BE0811F7C1FE3801C8083FF793FE2F01D1086DF765FE2501DB089BF737FE1B01E508C9F70AFE1101EF08F6F6DCFE0701F90924F6AEFDFD02030952F680FDF3020D0980F652FDE9021709AEF624FDDF022109DCF5F6FDD5022B0A0AF5C8FDCB02350A38F59AFDC1023F0A66F56CFDB802480A94F53FFDAE02520AC1F511FDA4025C0AEFF4E3FD9A02660B1DF4B5FD9002700B4BF487FD86027A0B79F459FD7C02840BA7F42BFD72028E0BD5F3FDFD6802980C03F3CFFD5E02A20C31F3A2FD5402AC0C5EF374FD4B02B50C8CF346FD4102BF0CBAF318FD3702C90CE8F2EAFD2D02D30D16F2BCFD2302DD0D44F28EFD1902E70D72F260FD0F02F10DA0F232FD0502FB0DCEF204FCFB03050DFCF1D7FCF1030F0E29F1A9FCE703190E57F17BFCDD03230E85F14DFCD4032C0EB3F11FFCCA03360EE1F0F1FCC003400F0FF0C3FCB6034A0F3DF095FCAC03540F6BF067FCA2035E0F99F039FC9803680FC7F00CFC8E03720FF4EFDEFC84037C1022EFB0FC7A03861050EF82FC700390107EEF54FC66039A10ACEF26FC5D03A310DAEEF8FC5303AD1108EECAFC4903B71136EE9CFC3F03C11164EE6EFC3503CB1192EE41FC2B03D511BFEE13FC2103DF11EDEDE5FC1703E9121BEDB7FC0D03F31249ED89FC0303FD1277ED5BFBF9040712A5ED2DFBF0041012D3ECFFFBE6041A1301ECD1FBDC0424132FECA3FBD2042E135DEC76FBC80438138AEC48FBBE044213B8EC1AFBB4044C13E6EBECFBAA04561414EBBEFBA004601442EB90FB96046A1470EB62FB8C0474149EEB34FB82047E14CCEB06FB79048714FAEAD8FB6F04911528EAABFB65049B1555EA7DFB5B04A51583EA4FFB5104AF15B1EA21FB4704B915DFE9F3FB3D04C3160DE9C5FB3304CD163BE997FB2904D71669E969FB1F04E11697E93BFB1504EB16C5E90DFB0B04F516F3E8E0FB0204FE1720E8B2FAF80508174EE884FAEE0512177CE856FAE4051C17AAE828FADA052617D8E7FAFAD005301806E7CCFAC6053A1834E79EFABC05441862E770FAB2054E1890E743FAA8055818BDE715FA9E056218EBE6E7FA95056B1919E6B9FA8B05751947E68BFA81057F1975E65DFA77058919A3E62FFA6D059319D1E601FA63059D19FFE5D3FA5905A71A2DE5A5FA4F05B11A5BE578FA4505BB1A88E54AFA3B05C51AB6E51CFA3105CF1AE4E4EEFA2705D91B12E4C0FA1E05E21B40E492FA1405EC1B6EE464FA0A05F61B9CE436FA0006001BCAE408F9F6060A1BF8E3DAF9EC06141C26E3ADF9E2061E1C53E37FF9D806281C81E351F9CE06321CAFE323F9C4063C1CDDE2F5F9BA06461D0BE2C7F9B1064F1D39E299F9A706591D67E26BF99D06631D95E23DF993066D1DC3E20FF98906771DF1E1E2F97F06811E1EE1B4F975068B1E4CE186F96B06951E7AE158F961069F1EA8E12AF95706A91ED6E0FCF94D06B31F04E0CEF94306BD1F32E0A0F93A06C61F60E072F93006D01F8EE044F92606DA1FBCE017F91C06E41FE9DFE9F91206EE2017DFBBF90806F82045DF8DF8FE07022073DF5FF8F4070C20A1DF31F8EA071620CFDF03F8E0072020FDDED5F8D6072A212BDEA7F8CC07342159DE79F8C3073D2187DE4CF8B9074721B4DE1EF8AF075121E2DDF0F8A5075B2210DDC2F89B0765223EDD94F891076F226CDD66F8870779229ADD38F87D078322C8DD0AF873078D22F6DCDCF86907972324DCAFF85F07A12351DC81F85607AA237FDC53F84C07B423ADDC25F84207BE23DBDBF7F83807C82409DBC9F82E07D22437DB9BF82407DC2465DB6DF81A07E62493DB3FF81007F024C1DB11F80607FA24EFDAE4F7FC0804251CDAB6F7F2080E254ADA88F7E808182578DA5AF7DF082125A6DA2CF7D5082B25D4D9FEF7CB08352602D9D0F7C1083F2630D9A2F7B70849265ED974F7AD0853268CD946F7A3085D26BAD919F799086726E7D8EBF78F08712715D8BDF785087B2743D88FF77B08852771D861F771088F279FD833F768089827CDD805F75E08A227FBD7D7F75408AC2829D7A9F74A08B62857D77BF74008C02885D74EF73608CA28B2D720F72C08D428E0D6F2F72208DE290ED6C4F71808E8293CD696F70E08F2296AD668F70408FC2998D63AF6FB090529C6D60CF6F1090F29F4D5DEF6E709192A22D5B0F6DD09232A50D583F6D3092D2A7DD555F6C909372AABD527F6BF09412AD9D4F9F6B5094B2B07D4CBF6AB09552B35D49DF6A1095F2B63D46FF69709692B91D441F68D09732BBFD413F684097C2BEDD3E5F67A09862C1BD3B8F67009902C48D38AF666099A2C76D35CF65C09A42CA4D32EF65209AE2CD2D300F64809B82D00D2D2F63E09C22D2ED2A4F63409CC2D5CD276F62A09D62D8AD248F62009E02DB8D21AF61609EA2DE6FFC1FFE0FFF3FFFC0004000D0020003FFF82FFBFFFE5FFF80008001B0041007EFF42FF9FFFD7FFF4000C0029006100BEFF03FF7EFFCAFFF000100036008200FDFEC3FF5EFFBCFFEC0014004400A2013DFE84FF3DFFAEFFE80018005200C3017CFE44FF1DFFA1FFE4001C005F00E301BCFE05FEFCFF93FFE00020006D010401FBFDC5FEDCFF85FFDC0024007B0124023BFD86FEBBFF77FFD8002800890145027AFD46FE9BFF6AFFD4002C0096016502BAFD07FE7AFF5CFFD0003000A4018602F9FCC7FE5AFF4EFFCC003400B201A60339FC88FE39FF41FFC8003800BF01C70378FC48FE19FF33FFC4003C00CD01E703B8FC09FDF8FF25FFBF004100DB020803F7FBC9FDD8FF17FFBB004500E902280437FB8AFDB7FF0AFFB7004900F602490476FB4AFD97FEFCFFB3004D0104026904B6FB0BFD76FEEEFFAF00510112028A04F5FACBFD56FEE1FFAB0055011F02AA0535FA8CFD35FED3FFA70059012D02CB0574FA4DFD15FEC5FFA3005D013B02EB05B3FA0DFCF4FEB7FF9F00610149030C05F3F9CEFCD4FEAAFF9B00650156032C0632F98EFCB3FE9CFF9700690164034D0672F94FFC93FE8EFF93006D0172036D06B1F90FFC72FE81FF8F0071017F038E06F1F8D0FC52FE73FF8B0075018D03AE0730F890FC31FE65FF870079019B03CF0770F851FC11FE57FF83007D01A903EF07AFF811FBF0FE4AFF7E008201B6041007EFF7D2FBD0FE3CFF7A008601C40430082EF792FBAFFE2EFF76008A01D20451086EF753FB8FFE21FF72008E01DF047108ADF713FB6EFE13FF6E009201ED049208EDF6D4FB4EFE05FF6A009601FB04B2092CF694FB2DFDF7FF66009A020904D3096CF655FB0DFDEAFF62009E021604F309ABF615FAECFDDCFF5E00A20224051409EBF5D6FACCFDCEFF5A00A6023205340A2AF596FAABFDC1FF5600AA023F05550A6AF557FA8BFDB3FF5200AE024D05750AA9F518FA6AFDA5FF4E00B2025B05960AE8F4D8FA4AFD98FF4A00B6026805B60B28F499FA29FD8AFF4600BA027605D70B67F459FA09FD7CFF4200BE028405F70BA7F41AF9E8FD6EFF3D00C3029206180BE6F3DAF9C8FD61FF3900C7029F06380C26F39BF9A7FD53FF3500CB02AD06590C65F35BF987FD45FF3100CF02BB06790CA5F31CF966FD38FF2D00D302C8069A0CE4F2DCF946FD2AFF2900D702D606BA0D24F29DF925FD1CFF2500DB02E406DB0D63F25DF905FD0EFF2100DF02F206FB0DA3F21EF8E4FD01FF1D00E302FF071C0DE2F1DEF8C4FCF3FF1900E7030D073C0E22F19FF8A3FCE5FF1500EB031B075D0E61F15FF883FCD8FF1100EF0328077D0EA1F120F862FCCAFF0D00F30336079E0EE0F0E0F842FCBCFF0900F7034407BE0F20F0A1F821FCAEFF0500FB035207DF0F5FF061F801FCA1FF0100FF035F07FF0F9FF022F7E0FC93FEFC0104036D08200FDE");
	CSharedFunctions::StringToByteArray(&RAM[0x47ED0], "EFE3F7C0FC85FEF80108037B0840101DEFA3F79FFC78FEF4010C03880861105DEF64F77FFC6AFEF0011003960881109CEF24F75EFC5CFEEC011403A408A210DCEEE5F73EFC4EFEE8011803B208C2111BEEA5F71DFC41FEE4011C03BF08E3115BEE66F6FDFC33FEE0012003CD0903119AEE26F6DCFC25FEDC012403DB092411DAEDE7F6BCFC18FED8012803E809441219EDA7F69BFC0AFED4012C03F609651259ED68F67BFBFCFED00130040409851298ED28F65AFBEEFECC0134041209A612D8ECE9F63AFBE1FEC80138041F09C61317ECA9F619FBD3FEC4013C042D09E71357EC6AF5F9FBC5FEC00140043B0A071396EC2AF5D8FBB8FEBB014504480A2813D6EBEBF5B8FBAAFEB7014904560A481415EBABF597FB9CFEB3014D04640A691455EB6CF577FB8EFEAF015104720A891494EB2CF556FB81FEAB0155047F0AAA14D4EAEDF536FB73FEA70159048D0ACA1513EAAEF515FB65FEA3015D049B0AEB1552EA6EF4F5FB58FE9F016104A80B0B1592EA2FF4D4FB4AFE9B016504B60B2C15D1E9EFF4B4FB3CFE97016904C40B4C1611E9B0F493FB2FFE93016D04D10B6D1650E970F473FB21FE8F017104DF0B8D1690E931F452FB13FE8B017504ED0BAE16CFE8F1F432FB05FE87017904FB0BCE170FE8B2F411FAF8FE83017D05080BEF174EE872F3F1FAEAFE7F018105160C0F178EE833F3D0FADCFE7A018605240C3017CDE7F3F3B0FACFFE76018A05310C50180DE7B4F38FFAC1FE72018E053F0C71184CE774F36FFAB3FE6E0192054D0C91188CE735F34EFAA5FE6A0196055B0CB218CBE6F5F32EFA98FE66019A05680CD2190BE6B6F30DFA8AFE62019E05760CF3194AE676F2EDFA7CFE5E01A205840D13198AE637F2CCFA6FFE5A01A605910D3419C9E5F7F2ACFA61FE5601AA059F0D541A09E5B8F28BFA53FE5201AE05AD0D751A48E579F26BFA45FE4E01B205BB0D951A87E539F24AFA38FE4A01B605C80DB61AC7E4FAF22AFA2AFE4601BA05D60DD61B06E4BAF209FA1CFE4201BE05E40DF71B46E47BF1E9FA0FFE3E01C205F10E171B85E43BF1C8FA01FE3901C705FF0E381BC5E3FCF1A8F9F3FE3501CB060D0E581C04E3BCF187F9E5FE3101CF061B0E791C44E37DF167F9D8FE2D01D306280E991C83E33DF146F9CAFE2901D706360EBA1CC3E2FEF126F9BCFE2501DB06440EDA1D02E2BEF105F9AFFE2101DF06510EFB1D42E27FF0E5F9A1FE1D01E3065F0F1B1D81E23FF0C4F993FE1901E7066D0F3C1DC1E200F0A4F985FE1501EB067B0F5C1E00E1C0F083F978FE1101EF06880F7D1E40E181F063F96AFE0D01F306960F9D1E7FE141F042F95CFE0901F706A40FBE1EBFE102F022F94FFE0501FB06B10FDE1EFEE0C2F001F941FE0101FF06BF0FFF1F3EE083EFE1F933FDFD020306CD101F1F7DE043EFC0F925FDF8020806DB10401FBDE004EFA0F918FDF4020C06E810601FFCDFC5EF7FF90AFDF0021006F61081203BDF85EF5FF8FCFDEC0214070410A1207BDF46EF3EF8EFFDE80218071110C220BADF06EF1EF8E1FDE4021C071F10E220FADEC7EEFDF8D3FDE00220072D11032139DE87EEDDF8C6FDDC0224073A11232179DE48EEBCF8B8FDD802280748114421B8DE08EE9CF8AAFDD4022C0756116421F8DDC9EE7BF89CFDD00230076411852237DD89EE5BF88FFDCC0234077111A52277DD4AEE3AF881FDC80238077F11C622B6DD0AEE1AF873FDC4023C078D11E622F6DCCBEDF9F866FDC00240079A12072335DC8BEDD9F858FDBC024407A812272375DC4CEDB8F84AFDB7024907B6124823B4DC0CED98F83CFDB3024D07C4126823F4DBCDED77F82FFDAF025107D112892433DB8DED57F821FDAB025507DF12A92473DB4EED36F813FDA7025907ED12CA24B2DB0EED16F806FDA3025D07FA12EA24F2DACFECF5F7F8FD9F02610808130B2531DA90ECD5F7EAFD9B02650816132B2570DA50ECB4F7DCFD9702690824134C25B0DA11EC94F7CFFD93026D0831136C25EFD9D1EC73F7C1FD8F0271083F138D262FD992EC53F7B3FD8B0275084D13AD266ED952EC32F7A6FD870279085A13CE26AED913EC12F798FD83027D086813EE26EDD8D3EBF1F78AFD7F02810876140F272DD894EBD1F77CFD7B02850884142F276CD854EBB0F76FFD76028A0891145027ACD815EB90F761FD72028E089F147027EBD7D5EB6FF753FD6E029208AD1491282BD796EB4FF746FD6A029608BA14B1286AD756EB2EF738FD66029A08C814D228AAD717EB0EF72AFD62029E08D614F228E9D6D7EAEDF71CFD5E02A208E415132929D698EACDF70FFD5A02A608F115332968D658EAACF701FD5602AA08FF155429A8D619EA8CF6F3FD5202AE090D157429E7D5D9EA6BF6E6FD4E02B2091A15952A27D59AEA4BF6D8FD4A02B6092815B52A66D55BEA2AF6CAFD4602BA093615D62AA5D51BEA0AF6BDFD4202BE094315F62AE5D4DCE9E9F6AFFD3E02C2095116172B24D49CE9C9F6A1FD3A02C6095F16372B64D45DE9A8F693FD3502CB096D16582BA3D41DE988F686FD3102CF097A16782BE3D3DEE967F678FD2D02D3098816992C22D39EE947F66AFD2902D7099616B92C62D35FE926F65DFD2502DB09A316DA2CA1D31FE906F64FFD2102DF09B116FA2CE1D2E0E8E5F641FD1D02E309BF171B2D20D2A0E8C5F633FD1902E709CD173B2D60D261E8A4F626FD1502EB09DA175C2D9FD221E884F618FD1102EF09E8177C2DDFD1E2E863F60AFD0D02F309F6179D2E1ED1A2E843F5FDFD0902F70A0317BD2E5ED163E822F5EFFD0502FB0A1117DE2E9DD123E802F5E1FD0102FF0A1F17FE2EDDD0E4E7E1F5D3FCFD03030A2D181F2F1CD0A4E7C1F5C6FCF903070A3A183F2F5CD065E7A0F5B8FCF4030C0A4818602F9BD026E780F5AAFCF003100A5618802FDACFE6E75FF59DFCEC03140A6318A1301ACFA7E73FF58FFCE803180A7118C13059CF67E71EF581FCE4031C0A7F18E23099CF28E6FEF573FCE003200A8D190230D8CEE8E6DDF566FCDC03240A9A19233118CEA9E6BDF558FCD803280AA819433157CE69E69CF54AFCD4032C0AB619643197CE2AE67CF53DFCD003300AC3198431D6CDEAE65BF52FFCCC03340AD119A53216CDABE63BF521FCC803380ADF19C53255CD6BE61AF513FCC4033C0AED19E63295CD2CE5FAF506FCC003400AFA1A0632D4CCECE5D9F4F8FCBC03440B081A273314CCADE5B9F4EAFCB803480B161A473353CC6DE598F4DDFCB3034D0B231A683393CC2EE578F4CFFCAF03510B311A8833D2CBEEE557F4C1FCAB03550B3F1AA93412CBAFE537F4B3FCA703590B4D1AC93451CB6FE516F4A6FCA3035D0B5A1AEA3491CB30E4F6F498FC9F03610B681B0A34D0CAF1E4D5F48AFC9B03650B761B2B350FCAB1E4B5F47DFC9703690B831B4B354FCA72E494F46FFC93036D0B911B6C358ECA32E474F461FC8F03710B9F1B8C35CEC9F3E453F454FC8B03750BAC1BAD360DC9B3E433F446FC8703790BBA1BCD364DC974E412F438FC83037D0BC81BEE368CC934E3F2F42AFC7F03810BD61C0E36CCC8F5E3D1F41DFC7B03850BE31C2F370BC8B5E3B1F40FFC7703890BF11C4F374BC876E390F401FC72038E0BFF1C70378AC836E370F3F4FC6E03920C0C1C9037CAC7F7E34FF3E6FC6A03960C1A1CB13809C7B7E32FF3D8FC66039A0C281CD13849C778E30EF3CAFC62039E0C361CF23888C738E2EEF3BDFC5E03A20C431D1238C8C6F9E2CDF3AFFC5A03A60C511D333907C6B9E2ADF3A1FC5603AA0C5F1D533947C67AE28CF394FC5203AE0C6C1D743986C63AE26CF386FC4E03B20C7A1D9439C6C5FBE24BF378FC4A03B60C881DB53A05C5BCE22BF36AFC4603BA0C961DD53A44C57CE20AF35DFC4203BE0CA31DF63A84C53DE1EAF34FFC3E03C20CB11E163AC3C4FDE1C9F341FC3A03C60CBF1E373B03C4BEE1A9F334FC3603CA0CCC1E573B42C47EE188F326FC3103CF0CDA1E783B82C43FE168F318FC2D03D30CE81E983BC1C3FFE147F30AFC2903D70CF61EB93C01C3C0E127F2FDFC2503DB0D031ED93C40C380E106F2EFFC2103DF0D111EFA3C80C341E0E6F2E1FC1D03E30D1F1F1A3CBFC301E0C5F2D4FC1903E70D2C1F3B3CFFC2C2E0A5F2C6FC1503EB0D3A1F5B3D3EC282E084F2B8FC1103EF0D481F7C3D7EC243E064F2AAFC0D03F30D561F9C3DBDC203E043F29DFC0903F70D631FBD3DFDC1C4E023F28FFC0503FB0D711FDD3E3CC184E002F281FC0103FF0D7F1FFE3E7CC145DFE2F274FBFD04030D8C201E3EBBC105DFC1F266FBF904070D9A203F3EFBC0C6DFA1F258FBF5040B0DA8205F3F3AC086DF80F24AFBF004100DB620803F7AFFA2FFBBFFD0FFE0FFECFFF5FFFA000000020006000B0014002000300045005EFF43FF76FF9FFFC0FFD8FFE9FFF400010005000C0017002800400061008A00BDFEE4FF31FF6FFF9FFFC3FFDDFFEE0002000700120023003D0061009100CF011CFE86FEECFF3EFF7FFFAFFFD1FFE80003000A0018002F0051008100C20114017AFE27FEA7FF0EFF5EFF9AFFC5FFE20003000C001E003B006600A200F2015901D9FDC8FE62FEDDFF3EFF86FFB9FFDC0004000F00240047007A00C20123019E0238FD69FE1DFEADFF1DFF71FFAEFFD600050011002A0052008F00E3015301E30297FD0BFDD8FE7CFEFDFF5DFFA2FFD0000600140030005E00A301030184022802F5FCACFD93FE4CFEDCFF48FF96FFCA000600170036006A00B8012401B4026D0354FC4DFD4EFE1BFEBCFF34FF8AFFC400070019003C007600CC014401E502B203B3FBEFFD09FDEBFE9BFF1FFF7EFFBE0008001C0042008200E10165021502F70411FB90FCC4FDBAFE7BFF0BFF72FFB80009001E0048008E00F501850246033C0470FB31FC7FFD8AFE5AFEF7FF67FFB200090021004E0099010901A60276038104CFFAD2FC3AFD59FE3AFEE2FF5BFFAC000A0023005400A5011E01C602A703C6052EFA74FBF5FD29FE19FECEFF4FFFA6000B0026005A00B1013201E702D7040B058CFA15FBB0FCF8FDF9FEB9FF43FF9F000C0028006100BD014702070308045005EBF9B6FB6BFCC8FDD8FEA5FF37FF99000C002B006700C9015B022803380495064AF957FB25FC97FDB8FE90FF2BFF93000D002E006D00D501700248036904DB06A9F8F9FAE0FC67FD97FE7CFF20FF8D000E0030007300E001840269039905200707F89AFA9BFC36FD77FE67FF14FF87000F0033007900EC0199028903CA05650766F83BFA56FC06FD56FE53FF08FF81000F0035007F00F801AD02AA03FA05AA07C5F7DDFA11FBD5FD36FE3EFEFCFF7B001000380085010401C202CA042B05EF0823F77EF9CCFBA5FD15FE2AFEF0FF750011003A008B011001D602EB045B06340882F71FF987FB74FCF5FE15FEE4FF6F0012003D0091011C01EB030B048C067908E1F6C0F942FB44FCD4FE01FED8FF690012003F0097012801FF032C04BC06BE0940F662F8FDFB13FCB4FDEDFECDFF6300130042009D01330213034C04ED0703099EF603F8B8FAE3FC93FDD8FEC1FF5D0014004500A3013F0228036D051D074809FDF5A4F873FAB2FC73FDC4FEB5FF570015004700A9014B023C038D054E078D0A5CF545F82EFA82FC52FDAFFEA9FF510015004A00AF0157025103AE057E07D20ABBF4E7F7E9FA51FC32FD9BFE9DFF4B0016004C00B50163026503CE05AF08170B19F488F7A4FA21FC11FD86FE91FF450017004F00BB016F027A03EF05DF085C0B78F429F75FF9F0FBF1FD72FE86FF3E0018005100C2017A028E040F061008A10BD7");
	CSharedFunctions::StringToByteArray(&RAM[0x48ED0], "F3CBF71AF9C0FBD0FD5DFE7AFF380019005400C8018602A30430064008E60C35F36CF6D5F98FFBB0FD49FE6EFF320019005600CE019202B704500671092B0C94F30DF690F95FFB8FFD34FE62FF2C001A005900D4019E02CC047106A109700CF3F2AEF64AF92EFB6FFD20FE56FF26001B005C00DA01AA02E0049106D209B60D52F250F605F8FEFB4EFD0BFE4AFF20001C005E00E001B602F504B2070209FB0DB0F1F1F5C0F8CDFB2EFCF7FE3FFF1A001C006100E601C1030904D207330A400E0FF192F57BF89DFB0DFCE3FE33FF14001D006300EC01CD031D04F307630A850E6EF133F536F86CFAEDFCCEFE27FF0E001E006600F201D90332051307940ACA0ECDF0D5F4F1F83CFACCFCBAFE1BFF08001F006800F801E50346053407C40B0F0F2BF076F4ACF80BFAACFCA5FE0FFF02001F006B00FE01F1035B055407F50B540F8AF017F467F7DBFA8BFC91FE03FEFC0020006D010401FD036F057508250B990FE9EFB9F422F7AAFA6BFC7CFDF8FEF600210070010A02080384059508560BDE1047EF5AF3DDF77AFA4AFC68FDECFEF00022007301100214039805B608860C2310A6EEFBF398F749FA2AFC53FDE0FEEA002200750116022003AD05D608B70C681105EE9CF353F719FA09FC3FFDD4FEE400230078011C022C03C105F708E70CAD1164EE3EF30EF6E8F9E9FC2AFDC8FEDD0024007A0123023803D6061709180CF211C2EDDFF2C9F6B8F9C8FC16FDBCFED70025007D0129024403EA063809480D371221ED80F284F687F9A8FC01FDB0FED10025007F012F025003FF065809790D7C1280ED21F23FF657F987FBEDFDA5FECB002600820135025B0413067909A90DC112DFECC3F1FAF626F967FBD9FD99FEC500270084013B02670427069909DA0E06133DEC64F1B5F5F6F946FBC4FD8DFEBF0028008701410273043C06BA0A0A0E4B139CEC05F16FF5C5F926FBB0FD81FEB90028008A0147027F045006DA0A3B0E9113FBEBA7F12AF595F905FB9BFD75FEB30029008C014D028B046506FB0A6B0ED61459EB48F0E5F564F8E5FB87FD69FEAD002A008F015302970479071B0A9C0F1B14B8EAE9F0A0F534F8C5FB72FD5EFEA7002B0091015902A2048E073B0ACC0F601517EA8AF05BF503F8A4FB5EFD52FEA1002B0094015F02AE04A2075C0AFD0FA51576EA2CF016F4D3F884FB49FD46FE9B002C0096016502BA04B7077C0B2D0FEA15D4E9CDEFD1F4A2F863FB35FD3AFE95002D0099016B02C604CB079D0B5E102F1633E96EEF8CF472F843FB20FD2EFE8F002E009C017102D204E007BD0B8E10741692E90FEF47F441F822FB0CFD22FE89002E009E017702DE04F407DE0BBF10B916F1E8B1EF02F411F802FAF7FD17FE83002F00A1017D02E9050907FE0BEF10FE174FE852EEBDF3E0F7E1FAE3FD0BFE7C003000A3018402F5051D081F0C20114317AEE7F3EE78F3B0F7C1FACFFCFFFE76003100A6018A03010531083F0C501188180DE795EE33F37FF7A0FABAFCF3FE70003200A80190030D054608600C8111CD186BE736EDEEF34FF780FAA6FCE7FE6A003200AB01960319055A08800CB1121218CAE6D7EDA9F31EF75FFA91FCDBFE64003300AD019C0325056F08A10CE212571929E678ED64F2EEF73FFA7DFCCFFE5E003400B001A20331058308C10D12129C1988E61AED1FF2BDF71EFA68FCC4FE58003500B301A8033C059808E20D4312E119E6E5BBECDAF28DF6FEFA54FCB8FE52003500B501AE034805AC09020D7313261A45E55CEC94F25CF6DDFA3FFCACFE4C003600B801B4035405C109230DA4136C1AA4E4FDEC4FF22CF6BDFA2BFCA0FE46003700BA01BA036005D509430DD413B11B03E49FEC0AF1FBF69CFA16FC94FE40003800BD01C0036C05EA09640E0513F61B61E440EBC5F1CBF67CFA02FC88FE3A003800BF01C6037805FE09840E35143B1BC0E3E1EB80F19AF65BF9EDFC7DFE34003900C201CC0383061309A50E6614801C1FE383EB3BF16AF63BF9D9FC71FE2E003A00C401D2038F062709C50E9614C51C7DE324EAF6F139F61AF9C5FC65FE28003B00C701D8039B063B09E60EC7150A1CDCE2C5EAB1F109F5FAF9B0FC59FE22003B00CA01DE03A706500A060EF7154F1D3BE266EA6CF0D8F5D9F99CFC4DFE1B003C00CC01E503B306640A270F2815941D9AE208EA27F0A8F5B9F987FC41FE15003D00CF01EB03BF06790A470F5815D91DF8E1A9E9E2F077F598F973FC36FE0F003E00D101F103CA068D0A680F89161E1E57E14AE99DF047F578F95EFC2AFE09003E00D401F703D606A20A880FB916631EB6E0EBE958F016F557F94AFC1EFE03003F00D601FD03E206B60AA90FEA16A81F15E08DE913EFE6F537F935FC12FDFD004000D9020303EE06CB0AC9101A16ED1F73E02EE8CEEFB5F516F921FC06FDF7004100DB020903FA06DF0AEA104B17321FD2DFCFE889EF85F4F6F90CFBFAFDF1004100DE020F040606F40B0A107B17772031DF71E844EF54F4D5F8F8FBEFFDEB004200E10215041107080B2B10AC17BC208FDF12E7FFEF24F4B5F8E3FBE3FDE5004300E3021B041D071D0B4B10DC180120EEDEB3E7B9EEF3F494F8CFFBD7FDDF004400E60221042907310B6C110D1847214DDE54E774EEC3F474F8BBFBCBFDD9004400E80227043507450B8C113D188C21ACDDF6E72FEE92F453F8A6FBBFFDD3004500EB022D0441075A0BAD116E18D1220ADD97E6EAEE62F433F892FBB3FDCD004600ED0233044D076E0BCD119E19162269DD38E6A5EE31F412F87DFBA7FDC7004700F00239045907830BEE11CF195B22C8DCD9E660EE01F3F2F869FB9CFDC1004700F2023F046407970C0E11FF19A02327DC7BE61BEDD0F3D1F854FB90FDBA004800F50246047007AC0C2F123019E52385DC1CE5D6EDA0F3B1F840FB84FDB4004900F8024C047C07C00C4F12601A2A23E4DBBDE591ED6FF390F82BFB78FDAE004A00FA0252048807D50C7012911A6F2443DB5FE54CED3FF370F817FB6CFDA8004B00FD0258049407E90C9012C11AB424A1DB00E507ED0EF34FF802FB60FDA2004B00FF025E04A007FE0CB112F21AF92500DAA1E4C2ECDEF32FF7EEFB55FD9C004C0102026404AB08120CD113221B3E255FDA42E47DECADF30EF7D9FB49FD96004D0104026A04B708270CF213531B8325BED9E4E438EC7DF2EEF7C5FB3DFD90004E0107027004C3083B0D1213831BC8261CD985E3F3EC4CF2CDF7B1FB31FD8A004E0109027604CF084F0D3313B41C0D267BD926E3AEEC1CF2ADF79CFB25FD84004F010C027C04DB08640D5313E41C5226DAD8C7E369EBEBF28CF788FB19FD7E0050010F028204E708780D7414151C972739D869E324EBBBF26CF773FB0EFD7800510111028804F2088D0D9414451CDC2797D80AE2DEEB8AF24BF75FFB02FD7200510114028E04FE08A10DB514761D2227F6D7ABE299EB5AF22BF74AFAF6FD6C005201160294050A08B60DD514A61D672855D74DE254EB29F20AF736FAEAFD6600530119029A051608CA0DF614D71DAC28B3D6EEE20FEAF9F1EAF721FADEFD600054011B02A0052208DF0E1615071DF12912D68FE1CAEAC8F1C9F70DFAD2FD590054011E02A7052E08F30E3715381E362971D630E185EA98F1A9F6F8FAC6FD530055012102AD053A09080E5715681E7B29D0D5D2E140EA67F189F6E4FABBFD4D0056012302B30545091C0E7715991EC02A2ED573E0FBEA37F168F6CFFAAFFD470057012602B9055109310E9815C91F052A8DD514E0B6EA06F148F6BBFAA3FD410057012802BF055D09450EB815FA1F4A2AECD4B5E071E9D6F127F6A7FA97FD3B0058012B02C5056909590ED9162A1F8F2B4BD457E02CE9A5F107F692FA8BFD350059012D02CB0575096E0EF9165B1FD42BA9D3F8DFE7E975F0E6F67EFA7FFD2F005A013002D1058109820F1A168B20192C08D399DFA2E944F0C6F669FA74FD29005A013202D7058C09970F3A16BC205E2C67D33BDF5DE914F0A5F655FA68FD23005B013502DD059809AB0F5B16EC20A32CC5D2DCDF18E8E3F085F640FA5CFD1D005C013802E305A409C00F7B171D20E82D24D27DDED3E8B3F064F62CFA50FD17005D013A02E905B009D40F9C174D212D2D83D21EDE8EE882F044F617FA44FD11005D013D02EF05BC09E90FBC177E21722DE2D1C0DE49E852F023F603FA38FD0B005E013F02F505C809FD0FDD17AE21B72E40D161DE03E821F003F5EEFA2DFD05005F014202FB05D30A120FFD17DF21FD2E9FD102DDBEE7F1EFE2F5DAFA21FCFF00600144030105DF0A26101E180F22422EFED0A3DD79E7C0EFC2F5C5FA15FCF800610147030805EB0A3B103E184022872F5DD045DD34E790EFA1F5B1FA09FCF200610149030E05F70A4F105F187022CC2FBBCFE6DCEFE75FEF81F59DF9FDFCEC0062014C031406030A63107F18A12311301ACF87DCAAE72FEF60F588F9F1FCE60063014F031A060F0A7810A018D123563079CF29DC65E6FEEF40F574F9E6FCE0006401510320061A0A8C10C01902239B30D7CECADC20E6CEEF1FF55FF9DAFCDA00640154032606260AA110E1193223E03136CE6BDBDBE69DEEFFF54BF9CEFCD400650156032C06320AB51101196324253195CE0CDB96E66DEEDEF536F9C2FCCE006601590332063E0ACA11221993246A31F4CDAEDB51E63CEEBEF522F9B6FCC80067015B0338064A0ADE114219C424AF3252CD4FDB0CE60CEE9DF50DF9AAFCC20067015E033E06560AF3116319F424F432B1CCF0DAC7E5DBEE7DF4F9F99EFCBC00680160034406620B0711831A2525393310CC92DA82E5ABEE5CF4E4F993FCB600690163034A066D0B1C11A41A55257E336ECC33DA3DE57AEE3CF4D0F987FCB0006A0166035006790B3011C41A8625C333CDCBD4D9F8E54AEE1BF4BCF97BFCAA006A0168035606850B4411E51AB62608342CCB75D9B3E519EDFBF4A7F96FFCA4006B016B035C06910B5912051AE7264D348BCB17D96EE4E9EDDAF493F963FC9E006C016D0362069D0B6D12261B17269234E9CAB8D928E4B8EDBAF47EF957FC97006D0170036906A90B8212461B4826D83548CA59D8E3E488ED99F46AF94CFC91006D0172036F06B40B9612671B78271D35A7C9FAD89EE457ED79F455F940FC8B006E0175037506C00BAB12871BA927623606C99CD859E427ED58F441F934FC85006F0177037B06CC0BBF12A81BD927A73664C93DD814E3F6ED38F42CF928FC7F0070017A038106D80BD412C81C0A27EC36C3C8DED7CFE3C6ED17F418F91CFC790070017D038706E40BE812E91C3A28313722C880D78AE395ECF7F403F910FC730071017F038D06F00BFD13091C6B28763780C821D745E365ECD6F3EFF905FC6D00720182039306FB0C11132A1C9B28BB37DFC7C2D700E334ECB6F3DAF8F9FC6700730184039907070C26134A1CCC2900383EC763D6BBE304EC95F3C6F8EDFC6100730187039F07130C3A136B1CFC2945389DC705D676E2D3EC75F3B2F8E1FC5B0074018903A5071F0C4E138B1D2D298A38FBC6A6D631E2A3EC54F39DF8D5FC550075018C03AB072B0C6313AC1D5D29CF395AC647D5ECE272EC34F389F8C9FC4F0076018E03B107370C7713CC1D8E2A1439B9C5E8D5A7E242EC13F374F8BDFC490076019103B707430C8C13ED1DBE2A593A18C58AD562E211EBF3F360F8B2FC430077019403BD074E0CA0140D1DEF2A9E3A76C52BD51DE1E1EBD2F34BF8A6FC3D0078019603C3075A0CB5142E1E1F2AE33AD5C4CCD4D8E1B0EBB2F337F89AFC360079019903CA07660CC9144E1E502B283B34");
	CSharedFunctions::StringToByteArray(&RAM[0x49ED0], "C46ED493E180EB91F322F88EFC30007A019B03D007720CDE146F1E802B6D3B92C40FD44DE14FEB71F30EF882FC2A007A019E03D6077E0CF2148F1EB12BB33BF1C3B0D408E11FEB50F2F9F876FC24007B01A003DC078A0D0714B01EE12BF83C50C351D3C3E0EEEB30F2E5F86BFC1E007C01A303E207950D1B14D01F122C3D3CAFC2F3D37EE0BEEB0FF2D0F85FFC18007D01A603E807A10D3014F11F422C823D0DC294D339E08DEAEFF2BCF853FC12007D01A803EE07AD0D4415111F732CC73D6CC235D2F4E05DEACEF2A8F847FC0C007E01AB03F407B90D5815321FA32D0C3DCBC1D6D2AFE02CEAAEF293F83BFC06007F01AD03FA07C50D6D15521FD42D513E2AC178D26ADFFCEA8DF27FF82FFC00008001B0040007D10D81157320042D963E88C119D225DFCBEA6DF26AF824FBFA008001B2040607DC0D96159320352DDB3EE7C0BAD1E0DF9BEA4DF256F818FBF4008101B5040C07E80DAA15B320652E203F46C05CD19BDF6AEA2CF241F80CFBEE008201B7041207F40DBF15D420962E653FA4BFFDD156DF3AEA0CF22DF800FBE8008301BA041808000DD315F420C62EAA4003BF9ED111DF09E9EBF218F7F4FBE2008301BD041E080C0DE8161520F72EEF4062BF3FD0CCDED9E9CBF204F7E8FBDC008401BF042408180DFC163521272F3440C1BEE1D087DEA8E9AAF1EFF7DDFBD5008501C2042B08230E11165621582F79411FBE82D042DE78E98AF1DBF7D1FBCF008601C40431082F0E25167621882FBE417EBE23CFFDDE47E969F1C6F7C5FBC9008601C70437083B0E3A169721B9300341DDBDC4CFB8DE17E949F1B2F7B9FBC3008701C9043D08470E4E16B721E93048423CBD66CF72DDE6E928F19EF7ADFBBD008801CC044308530E6216D8221A308E429ABD07CF2DDDB6E908F189F7A1FBB7008901CE0449085F0E7716F8224A30D342F9BCA8CEE8DD85E8E7F175F795FBB1008901D1044F086B0E8B1719227B31184358BC4ACEA3DD55E8C7F160F78AFBAB008A01D4045508760EA0173922AB315D43B6BBEBCE5EDD24E8A6F14CF77EFBA5008B01D6045B08820EB4175A22DC31A24415BB8CCE19DCF4E886F137F772FB9F008C01D90461088E0EC9177A230C31E74474BB2DCDD4DCC3E865F123F766FB99008C01DB0467089A0EDD179B233D322C44D3BACFCD8FDC93E845F10EF75AFB93008D01DE046D08A60EF217BB236D32714531BA70CD4ADC62E824F0FAF74EFB8D008E01E0047308B20F0617DC239E32B64590BA11CD05DC32E804F0E5F743FB87008F01E3047908BD0F1B17FC23CE32FB45EFB9B2CCC0DC01E7E3F0D1F737FB81008F01E5047F08C90F2F181D23FF3340464EB954CC7BDBD1E7C3F0BCF72BFB7B009001E8048508D50F44183D242F338546ACB8F5CC36DBA0E7A2F0A8F71FFB74009101EB048C08E10F58185E246033CA470BB896CBF1DB70E782F094F713FB6E009201ED049208ED0F6C187E2490340F476AB838CBACDB3FE761F07FF707FB68009301F0049808F90F81189F24C1345447C8B7D9CB67DB0FE741F06BF6FCFB62009301F2049E09040F9518BF24F134994827B77ACB22DADEE720F056F6F0FB5C009401F504A409100FAA18E0252234DE4886B71BCADDDAAEE700F042F6E4FB56009501F704AA091C0FBE19002552352348E5B6BDCA97DA7DE6DFF02DF6D8FB50009601FA04B009280FD31921258335694943B65ECA52DA4DE6BFF019F6CCFB4A009601FC04B609340FE7194125B335AE49A2B5FFCA0DDA1CE69EF004F6C0FB44009701FF04BC09400FFC196225E435F34A01B5A0C9C8D9ECE67EEFF0F6B4FB3E0098020204C2094C10101982261436384A60B542C983D9BBE65DEFDBF6A9FB380099020404C80957102519A32645367D4ABEB4E3C93ED98BE63DEFC7F69DFB320099020704CE0963103919C3267536C24B1DB484C8F9D95AE61CEFB2F691FB2C009A020904D4096F104E19E426A637074B7CB426C8B4D92AE5FCEF9EF685FB26009B020C04DA097B10621A0426D6374C4BDAB3C7C86FD8F9E5DBEF8AF679FB20009C020E04E0098710761A25270737914C39B368C82AD8C9E5BBEF75F66DFB1A009C021104E60993108B1A45273737D64C98B309C7E5D898E59AEF61F662FB13009D021304ED099E109F1A662768381B4CF7B2ABC7A0D868E57AEF4CF656FB0D009E021604F309AA10B41A86279838604D55B24CC75BD837E559EF38F64AFB07009F021904F909B610C81AA727C938A54DB4B1EDC716D807E539EF23F63EFB01009F021B04FF09C210DD1AC727F938EA4E13B18EC6D1D7D6E518EF0FF632FAFB00A0021E050509CE10F11AE8282A392F4E72B130C68CD7A6E4F8EEFAF626FAF500A10220050B09DA11061B08285A39744ED0B0D1C647D775E4D7EEE6F61BFAEF00A20223051109E5111A1B29288B39B94F2FB072C602D745E4B7EED1F60FFAE900A20225051709F1112F1B4928BB39FE4F8EB014C5BCD714E496EEBDF603FAE300A30228051D09FD11431B6A28EC3A444FECAFB5C577D6E4E476EEA8F5F7FADD00A4022B05230A0911581B8A291C3A89504BAF56C532D6B3E455EE94F5EBFAD700A5022D05290A15116C1BAB294D3ACE50AAAEF7C4EDD683E435EE80F5DFFAD100A50230052F0A2111801BCB297D3B135109AE99C4A8D652E414EE6BF5D4FACB00A6023205350A2C11951BEC29AE3B585167AE3AC463D622E3F4EE57F5C8FAC500A70235053B0A3811A91C0C29DE3B9D51C6ADDBC41ED5F1E3D3EE42F5BCFABF00A8023705410A4411BE1C2D2A0F3BE25225AD7CC3D9D5C1E3B3EE2EF5B0FAB900A8023A05470A5011D21C4D2A3F3C275284AD1EC394D590E392EE19F5A4FAB200A9023C054E0A5C11E71C6E2A703C6C52E2ACBFC34FD560E372EE05F598FAAC00AA023F05540A6811FB1C8E2AA03CB15341AC60C30AD52FE351EDF0F58CFAA600AB0242055A0A7412101CAF2AD13CF653A0AC02C2C5D4FFE331EDDCF581FAA000AC024405600A7F12241CCF2B013D3B53FEABA3C280D4CEE311EDC7F575FA9A00AC024705660A8B12391CEF2B323D80545DAB44C23BD49EE2F0EDB3F569FA9400AD0249056C0A97124D1D102B623DC554BCAAE5C1F6D46DE2D0ED9EF55DFA8E00AE024C05720AA312621D302B933E0A551BAA87C1B1D43DE2AFED8AF551FA8800AF024E05780AAF12761D512BC33E4F5579AA28C16CD40CE28FED76F545FA8200AF0251057E0ABB128A1D712BF43E9455D8A9C9C127D3DCE26EED61F53AFA7C00B0025305840AC6129F1D922C243ED95637A96AC0E1D3ABE24EED4DF52EFA7600B10256058A0AD212B31DB22C553F1F5696A90CC09CD37BE22DED38F522FA7000B2025905900ADE12C81DD32C853F6456F4A8ADC057D34AE20DED24F516FA6A00B2025B05960AEA12DC1DF32CB63FA95753A84EC012D31AE1ECED0FF50AFA6400B3025E059C0AF612F11E142CE63FEE57B2A7F0BFCDD2E9E1CCECFBF4FEFA5E00B4026005A20B0213051E342D1740335810A791BF88D2B9E1ABECE6F4F3FA5800B5026305A80B0D131A1E552D474078586FA732BF43D288E18BECD2F4E7FA5100B5026505AF0B19132E1E752D7840BD58CEA6D3BEFED258E16AECBDF4DBFA4B00B6026805B50B2513431E962DA84102592DA675BEB9D227E14AECA9F4CFFA4500B7026A05BB0B3113571EB62DD94147598BA616BE74D1F7E129EC94F4C3FA3F00B8026D05C10B3D136C1ED72E09418C59EAA5B7BE2FD1C6E109EC80F4B7FA3900B8027005C70B4913801EF72E3A41D15A49A558BDEAD196E0E8EC6CF4ABFA3300B9027205CD0B5513941F182E6A42165AA8A4FABDA5D165E0C8EC57F4A0FA2D00BA027505D30B6013A91F382E9B425B5B06A49BBD60D135E0A7EC43F494FA2700BB027705D90B6C13BD1F592ECB42A05B65A43CBD1BD104E087EC2EF488FA2100BB027A05DF0B7813D21F792EFC42E55BC4A3DEBCD6D0D4E066EC1AF47CFA1B00BC027C05E50B8413E61F9A2F2C432A5C22A37FBC91D0A3E046EC05F470FA1500BD027F05EB0B9013FB1FBA2F5D436F5C81A320BC4CD073E025EBF1F464FA0F00BE028105F10B9C140F1FDB2F8D43B45CE0A2C1BC06D042E005EBDCF459FA0900BE028405F70BA714241FFB2FBE43FA5D3FA263BBC1D012DFE4EBC8F44DFA0300BF028705FD0BB31438201C2FEE443F5D9DA204BB7CCFE1DFC4EBB3F441F9FD00C0028906030BBF144D203C301F44845DFCA1A5BB37CFB1DFA3EB9FF435F9F700C1028C06090BCB1461205D304F44C95E5BA146BAF2CF80DF83EB8AF429F9F000C2028E06100BD71476207D3080450E5EBA8005B2C88005B3C88005B4C8000000800000000100030007000F001F003F007F00FF01FF03FF07FF0FFF1FFF3FFF7FFFFFFF000000000001000200040008001000200040008001000200040008001000200040008000000000040008000C001200190021002C0035003F004B00590069007E009500B90100379837F137B504F337D744FD38000000381837F0383504F3385744FD38800000389837F038B504F338D744FD39000000393504F3391837F0395744FD39800000399837F039B504F339D744FD3A0000003A1837F03A3504F33A5744FD3A8000003A9837F13AB504F33AD744FD3B0000003B1837F03B3504F33B5744FD3B8000003B9837F03BB504F33BD744FD3C0000003C1837F03C3504F33C5744FC3C8000003C9837F03CB504F33CD744FD3D0000003D1837F03D3504F33D5744FD3D8000003D9837F03DB504F33DD744FD3E0000003E1837F03E3504F43E5744FD3E8000003E9837F03EB504F33ED744FD3F0000003F1837F03F3504F33F5744FD3F8000000001000000000000FFFFFF810000007F000000FF000000100000010100000100000000018004AD348004AEECFFFFFF810000007F000000FF000000100000010500000100000000058004B6F88004BF90FFFFFF810000007F000000FF000000100000010900000100000000098004C7BC8004D734FFFFFFC000000040FFFFFFFF00000010FFFFFFFF0000008100000000000000008004DF8000000004000000010000000000000010000030C08004ACBC000000000000000000000023000000008004AC50000000000000002300000059000000008004AC74000000000000007C00000084000000008004AC9828202031312020203027732029200000000000000000000000000000");

	int lengthDecompressed = compressedSize / 0x1C * 0x36 * 2;

	int newInputSpot = 0x400000;
	int newOutputSpot = 0x500000;
	// Length
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00083908, compressedSize / 0x1C); //chunkAmount
	CSharedFunctions::WriteShortToBuffer(RAM, 0x0008390C, compressedSize % 0x1C);
	CSharedFunctions::WriteShortToBuffer(RAM, 0x80083846, lengthDecompressed);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x00083848, 0x80000000 | newOutputSpot); // Output Spot
	CSharedFunctions::WriteLongToBuffer(RAM, 0x0008384C, 0x80000000 | newInputSpot); // Input spot
	memcpy(&RAM[newInputSpot], &ROM[romOffset], compressedSize);

	FUN_00009e84(RAM, A0);

	for (int x = 0; x < lengthDecompressed; x+=2)
	{
		unsigned short tempValue = CSharedFunctions::CharArrayToShort(RAM, newOutputSpot + x);
		tempValue = CSharedFunctions::Flip16Bit(tempValue);
		CSharedFunctions::WriteShortToBuffer(RAM, newOutputSpot + x, tempValue);
	}
	
	bool result = SaveProcessedWav(&RAM[newOutputSpot], lengthDecompressed, 1, samplingRateFloat, outputFilename);

	delete [] RAM;

	return result;
}

bool CAcclaimDEANAudioDecompression::ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate)
{
	FILE* inWavFile = fopen(rawWavFileName, "rb");
	if (inWavFile == NULL)
	{
		MessageBox(NULL, "Error cannot read wav file", "Error", NULL);
		return false;
	}

	fseek(inWavFile, 0, SEEK_END);
	int fileSize = ftell(inWavFile);
	rewind(inWavFile);

	unsigned char* wavData = new unsigned char[fileSize];
	fread(wavData, 1, fileSize, inWavFile);
	fclose(inWavFile);

	if (((((((wavData[0] << 8) | wavData[1]) << 8) | wavData[2]) << 8) | wavData[3]) != 0x52494646)
	{
		delete [] wavData;
		MessageBox(NULL, "Error not RIFF wav", "Error", NULL);
		return false;
	}

	if (((((((wavData[0x8] << 8) | wavData[0x9]) << 8) | wavData[0xA]) << 8) | wavData[0xB]) != 0x57415645)
	{
		delete [] wavData;
		MessageBox(NULL, "Error not WAVE wav", "Error", NULL);
		return false;
	}

	bool endFlag = false;

	unsigned long currentOffset = 0xC;

	unsigned short channels = 0;
	samplingRate = 0;
	unsigned short bitRate = 0;

	bool returnFlag = false;

	while (!endFlag)
	{
		if (currentOffset >= (fileSize - 8))
			break;

		unsigned long sectionType = ((((((wavData[currentOffset] << 8) | wavData[currentOffset + 1]) << 8) | wavData[currentOffset + 2]) << 8) | wavData[currentOffset + 3]);

		if (sectionType == 0x666D7420) // fmt
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			channels = ((wavData[currentOffset + 0xB] << 8) | wavData[currentOffset + 0xA]);

			if (channels != 0x0001)
			{
				MessageBox(NULL, "Warning: Only mono wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
			}

			samplingRate = ((((((wavData[currentOffset + 0xF] << 8) | wavData[currentOffset + 0xE]) << 8) | wavData[currentOffset + 0xD]) << 8) | wavData[currentOffset + 0xC]);
			
			bitRate = ((wavData[currentOffset + 0x17] << 8) | wavData[currentOffset + 0x16]);

			currentOffset += chunkSize + 8;
		}
		else if (sectionType == 0x64617461) // data
		{
			rawLength = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			if ((channels == 0) || (samplingRate == 0) || (bitRate == 0))
			{
				MessageBox(NULL, "Incorrect section type (missing fmt)", "Error unknown wav format", NULL);
				endFlag = true;
				returnFlag = false;
			}

			if (bitRate == 0x0010)
			{
				rawData = new unsigned char[rawLength];
				for (int x = 0; x < rawLength; x++)
				{
					rawData[x] = wavData[currentOffset + 0x8 + x];
				}
			
				returnFlag = true;
			}
			else
			{
				MessageBox(NULL, "Error only 16-bit PCM wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
			}

			currentOffset += rawLength + 8;
		}
		else if (sectionType == 0x736D706C) // smpl
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			/*unsigned long numSampleBlocks = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x24]));
			if (numSampleBlocks > 0)
			{
				hasLoopData = true;

				keyBase = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x14])) & 0xFF;
				loopStart = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x34]));
				loopEnd = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x38]));
				loopCount = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x40]));
				if (loopCount == 0)
					loopCount = 0xFFFFFFFF;
			}*/

			currentOffset += 8 + chunkSize;
		}
		else
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			currentOffset += 8 + chunkSize;
		}
	}

	delete [] wavData;
	return returnFlag;
}

bool CAcclaimDEANAudioDecompression::CompressSound(CString rawWavFileName, unsigned char*& outputBuffer, int& outputBufferLength, int scale, int type, int chunk)
{
	unsigned char* data = NULL;
	int dataLength = 0;
	unsigned long samplingRate = 0;
	bool result = ReadWavData(rawWavFileName, data, dataLength, samplingRate);
	if (!result)
		return result;

	for (int x = 0; x < dataLength; x += 2)
	{
		unsigned char valueTemp = data[x];
		data[x] = data[x+1];
		data[x+1] = valueTemp;
	}

	result = CompressSound(data, dataLength, outputBuffer, outputBufferLength, scale, type, chunk);

	delete [] data;
	return result;
}

int CAcclaimDEANAudioDecompression::FindBestPredictorIndex(signed short previousPredictor, signed short previousValue, signed short sample, unsigned char* predictor)
{
	double bestDelta = 99999999999999;
	double bestDeltaIndex = -1;
	for (int index = 0; index < 0x10; index++)
	{
		signed short valueIndex = previousPredictor + (short)CSharedFunctions::CharArrayToShort(&predictor[index * 2]);
		signed short newValue = previousValue + valueIndex;
		double delta = fabs((double)newValue - (double)sample);
		if (delta < bestDelta)
		{
			bestDelta = delta;
			bestDeltaIndex = index;
		}
	}

	return bestDeltaIndex;
}

bool CAcclaimDEANAudioDecompression::CompressSound(unsigned char* data, int dataLength, unsigned char*& outputBuffer, int& outputBufferLength, int scale, int type, int chunk)
{
	unsigned char predictorTable[0x2000];
	CSharedFunctions::StringToByteArray(&predictorTable[0], "FFA2FFBBFFD0FFE0FFECFFF5FFFA000000020006000B0014002000300045005EFF43FF76FF9FFFC0FFD8FFE9FFF400010005000C0017002800400061008A00BDFEE4FF31FF6FFF9FFFC3FFDDFFEE0002000700120023003D0061009100CF011CFE86FEECFF3EFF7FFFAFFFD1FFE80003000A0018002F0051008100C20114017AFE27FEA7FF0EFF5EFF9AFFC5FFE20003000C001E003B006600A200F2015901D9FDC8FE62FEDDFF3EFF86FFB9FFDC0004000F00240047007A00C20123019E0238FD69FE1DFEADFF1DFF71FFAEFFD600050011002A0052008F00E3015301E30297FD0BFDD8FE7CFEFDFF5DFFA2FFD0000600140030005E00A301030184022802F5FCACFD93FE4CFEDCFF48FF96FFCA000600170036006A00B8012401B4026D0354FC4DFD4EFE1BFEBCFF34FF8AFFC400070019003C007600CC014401E502B203B3FBEFFD09FDEBFE9BFF1FFF7EFFBE0008001C0042008200E10165021502F70411FB90FCC4FDBAFE7BFF0BFF72FFB80009001E0048008E00F501850246033C0470FB31FC7FFD8AFE5AFEF7FF67FFB200090021004E0099010901A60276038104CFFAD2FC3AFD59FE3AFEE2FF5BFFAC000A0023005400A5011E01C602A703C6052EFA74FBF5FD29FE19FECEFF4FFFA6000B0026005A00B1013201E702D7040B058CFA15FBB0FCF8FDF9FEB9FF43FF9F000C0028006100BD014702070308045005EBF9B6FB6BFCC8FDD8FEA5FF37FF99000C002B006700C9015B022803380495064AF957FB25FC97FDB8FE90FF2BFF93000D002E006D00D501700248036904DB06A9F8F9FAE0FC67FD97FE7CFF20FF8D000E0030007300E001840269039905200707F89AFA9BFC36FD77FE67FF14FF87000F0033007900EC0199028903CA05650766F83BFA56FC06FD56FE53FF08FF81000F0035007F00F801AD02AA03FA05AA07C5F7DDFA11FBD5FD36FE3EFEFCFF7B001000380085010401C202CA042B05EF0823F77EF9CCFBA5FD15FE2AFEF0FF750011003A008B011001D602EB045B06340882F71FF987FB74FCF5FE15FEE4FF6F0012003D0091011C01EB030B048C067908E1F6C0F942FB44FCD4FE01FED8FF690012003F0097012801FF032C04BC06BE0940F662F8FDFB13FCB4FDEDFECDFF6300130042009D01330213034C04ED0703099EF603F8B8FAE3FC93FDD8FEC1FF5D0014004500A3013F0228036D051D074809FDF5A4F873FAB2FC73FDC4FEB5FF570015004700A9014B023C038D054E078D0A5CF545F82EFA82FC52FDAFFEA9FF510015004A00AF0157025103AE057E07D20ABBF4E7F7E9FA51FC32FD9BFE9DFF4B0016004C00B50163026503CE05AF08170B19F488F7A4FA21FC11FD86FE91FF450017004F00BB016F027A03EF05DF085C0B78F429F75FF9F0FBF1FD72FE86FF3E0018005100C2017A028E040F061008A10BD7F3CBF71AF9C0FBD0FD5DFE7AFF380019005400C8018602A30430064008E60C35F36CF6D5F98FFBB0FD49FE6EFF320019005600CE019202B704500671092B0C94F30DF690F95FFB8FFD34FE62FF2C001A005900D4019E02CC047106A109700CF3F2AEF64AF92EFB6FFD20FE56FF26001B005C00DA01AA02E0049106D209B60D52F250F605F8FEFB4EFD0BFE4AFF20001C005E00E001B602F504B2070209FB0DB0F1F1F5C0F8CDFB2EFCF7FE3FFF1A001C006100E601C1030904D207330A400E0FF192F57BF89DFB0DFCE3FE33FF14001D006300EC01CD031D04F307630A850E6EF133F536F86CFAEDFCCEFE27FF0E001E006600F201D90332051307940ACA0ECDF0D5F4F1F83CFACCFCBAFE1BFF08001F006800F801E50346053407C40B0F0F2BF076F4ACF80BFAACFCA5FE0FFF02001F006B00FE01F1035B055407F50B540F8AF017F467F7DBFA8BFC91FE03FEFC0020006D010401FD036F057508250B990FE9EFB9F422F7AAFA6BFC7CFDF8FEF600210070010A02080384059508560BDE1047EF5AF3DDF77AFA4AFC68FDECFEF00022007301100214039805B608860C2310A6EEFBF398F749FA2AFC53FDE0FEEA002200750116022003AD05D608B70C681105EE9CF353F719FA09FC3FFDD4FEE400230078011C022C03C105F708E70CAD1164EE3EF30EF6E8F9E9FC2AFDC8FEDD0024007A0123023803D6061709180CF211C2EDDFF2C9F6B8F9C8FC16FDBCFED70025007D0129024403EA063809480D371221ED80F284F687F9A8FC01FDB0FED10025007F012F025003FF065809790D7C1280ED21F23FF657F987FBEDFDA5FECB002600820135025B0413067909A90DC112DFECC3F1FAF626F967FBD9FD99FEC500270084013B02670427069909DA0E06133DEC64F1B5F5F6F946FBC4FD8DFEBF0028008701410273043C06BA0A0A0E4B139CEC05F16FF5C5F926FBB0FD81FEB90028008A0147027F045006DA0A3B0E9113FBEBA7F12AF595F905FB9BFD75FEB30029008C014D028B046506FB0A6B0ED61459EB48F0E5F564F8E5FB87FD69FEAD002A008F015302970479071B0A9C0F1B14B8EAE9F0A0F534F8C5FB72FD5EFEA7002B0091015902A2048E073B0ACC0F601517EA8AF05BF503F8A4FB5EFD52FEA1002B0094015F02AE04A2075C0AFD0FA51576EA2CF016F4D3F884FB49FD46FE9B002C0096016502BA04B7077C0B2D0FEA15D4E9CDEFD1F4A2F863FB35FD3AFE95002D0099016B02C604CB079D0B5E102F1633E96EEF8CF472F843FB20FD2EFE8F002E009C017102D204E007BD0B8E10741692E90FEF47F441F822FB0CFD22FE89002E009E017702DE04F407DE0BBF10B916F1E8B1EF02F411F802FAF7FD17FE83002F00A1017D02E9050907FE0BEF10FE174FE852EEBDF3E0F7E1FAE3FD0BFE7C003000A3018402F5051D081F0C20114317AEE7F3EE78F3B0F7C1FACFFCFFFE76003100A6018A03010531083F0C501188180DE795EE33F37FF7A0FABAFCF3FE70003200A80190030D054608600C8111CD186BE736EDEEF34FF780FAA6FCE7FE6A003200AB01960319055A08800CB1121218CAE6D7EDA9F31EF75FFA91FCDBFE64003300AD019C0325056F08A10CE212571929E678ED64F2EEF73FFA7DFCCFFE5E003400B001A20331058308C10D12129C1988E61AED1FF2BDF71EFA68FCC4FE58003500B301A8033C059808E20D4312E119E6E5BBECDAF28DF6FEFA54FCB8FE52003500B501AE034805AC09020D7313261A45E55CEC94F25CF6DDFA3FFCACFE4C003600B801B4035405C109230DA4136C1AA4E4FDEC4FF22CF6BDFA2BFCA0FE46003700BA01BA036005D509430DD413B11B03E49FEC0AF1FBF69CFA16FC94FE40003800BD01C0036C05EA09640E0513F61B61E440EBC5F1CBF67CFA02FC88FE3A003800BF01C6037805FE09840E35143B1BC0E3E1EB80F19AF65BF9EDFC7DFE34003900C201CC0383061309A50E6614801C1FE383EB3BF16AF63BF9D9FC71FE2E003A00C401D2038F062709C50E9614C51C7DE324EAF6F139F61AF9C5FC65FE28003B00C701D8039B063B09E60EC7150A1CDCE2C5EAB1F109F5FAF9B0FC59FE22003B00CA01DE03A706500A060EF7154F1D3BE266EA6CF0D8F5D9F99CFC4DFE1B003C00CC01E503B306640A270F2815941D9AE208EA27F0A8F5B9F987FC41FE15003D00CF01EB03BF06790A470F5815D91DF8E1A9E9E2F077F598F973FC36FE0F003E00D101F103CA068D0A680F89161E1E57E14AE99DF047F578F95EFC2AFE09003E00D401F703D606A20A880FB916631EB6E0EBE958F016F557F94AFC1EFE03003F00D601FD03E206B60AA90FEA16A81F15E08DE913EFE6F537F935FC12FDFD004000D9020303EE06CB0AC9101A16ED1F73E02EE8CEEFB5F516F921FC06FDF7004100DB020903FA06DF0AEA104B17321FD2DFCFE889EF85F4F6F90CFBFAFDF1004100DE020F040606F40B0A107B17772031DF71E844EF54F4D5F8F8FBEFFDEB004200E10215041107080B2B10AC17BC208FDF12E7FFEF24F4B5F8E3FBE3FDE5004300E3021B041D071D0B4B10DC180120EEDEB3E7B9EEF3F494F8CFFBD7FDDF004400E60221042907310B6C110D1847214DDE54E774EEC3F474F8BBFBCBFDD9004400E80227043507450B8C113D188C21ACDDF6E72FEE92F453F8A6FBBFFDD3004500EB022D0441075A0BAD116E18D1220ADD97E6EAEE62F433F892FBB3FDCD004600ED0233044D076E0BCD119E19162269DD38E6A5EE31F412F87DFBA7FDC7004700F00239045907830BEE11CF195B22C8DCD9E660EE01F3F2F869FB9CFDC1004700F2023F046407970C0E11FF19A02327DC7BE61BEDD0F3D1F854FB90FDBA004800F50246047007AC0C2F123019E52385DC1CE5D6EDA0F3B1F840FB84FDB4004900F8024C047C07C00C4F12601A2A23E4DBBDE591ED6FF390F82BFB78FDAE004A00FA0252048807D50C7012911A6F2443DB5FE54CED3FF370F817FB6CFDA8004B00FD0258049407E90C9012C11AB424A1DB00E507ED0EF34FF802FB60FDA2004B00FF025E04A007FE0CB112F21AF92500DAA1E4C2ECDEF32FF7EEFB55FD9C004C0102026404AB08120CD113221B3E255FDA42E47DECADF30EF7D9FB49FD96004D0104026A04B708270CF213531B8325BED9E4E438EC7DF2EEF7C5FB3DFD90004E0107027004C3083B0D1213831BC8261CD985E3F3EC4CF2CDF7B1FB31FD8A004E0109027604CF084F0D3313B41C0D267BD926E3AEEC1CF2ADF79CFB25FD84004F010C027C04DB08640D5313E41C5226DAD8C7E369EBEBF28CF788FB19FD7E0050010F028204E708780D7414151C972739D869E324EBBBF26CF773FB0EFD7800510111028804F2088D0D9414451CDC2797D80AE2DEEB8AF24BF75FFB02FD7200510114028E04FE08A10DB514761D2227F6D7ABE299EB5AF22BF74AFAF6FD6C005201160294050A08B60DD514A61D672855D74DE254EB29F20AF736FAEAFD6600530119029A051608CA0DF614D71DAC28B3D6EEE20FEAF9F1EAF721FADEFD600054011B02A0052208DF0E1615071DF12912D68FE1CAEAC8F1C9F70DFAD2FD590054011E02A7052E08F30E3715381E362971D630E185EA98F1A9F6F8FAC6FD530055012102AD053A09080E5715681E7B29D0D5D2E140EA67F189F6E4FABBFD4D0056012302B30545091C0E7715991EC02A2ED573E0FBEA37F168F6CFFAAFFD470057012602B9055109310E9815C91F052A8DD514E0B6EA06F148F6BBFAA3FD410057012802BF055D09450EB815FA1F4A2AECD4B5E071E9D6F127F6A7FA97FD3B0058012B02C5056909590ED9162A1F8F2B4BD457E02CE9A5F107F692FA8BFD350059012D02CB0575096E0EF9165B1FD42BA9D3F8DFE7E975F0E6F67EFA7FFD2F005A013002D1058109820F1A168B20192C08D399DFA2E944F0C6F669FA74FD29005A013202D7058C09970F3A16BC205E2C67D33BDF5DE914F0A5F655FA68FD23005B013502DD059809AB0F5B16EC20A32CC5D2DCDF18E8E3F085F640FA5CFD1D005C013802E305A409C00F7B171D20E82D24D27DDED3E8B3F064F62CFA50FD17005D013A02E905B009D40F9C174D212D2D83D21EDE8EE882F044F617FA44FD11005D013D02EF05BC09E90FBC177E21722DE2D1C0DE49E852F023F603FA38FD0B005E013F02F505C809FD0FDD17AE21B72E40D161DE03E821F003F5EEFA2DFD05005F014202FB05D30A120FFD17DF21FD2E9FD102DDBEE7F1EFE2F5DAFA21FCFF00600144030105DF0A26101E180F22422EFED0A3DD79E7C0EFC2F5C5FA15FCF800610147030805EB0A3B103E184022872F5D");
	CSharedFunctions::StringToByteArray(&predictorTable[0x1000], "D045DD34E790EFA1F5B1FA09FCF200610149030E05F70A4F105F187022CC2FBBCFE6DCEFE75FEF81F59DF9FDFCEC0062014C031406030A63107F18A12311301ACF87DCAAE72FEF60F588F9F1FCE60063014F031A060F0A7810A018D123563079CF29DC65E6FEEF40F574F9E6FCE0006401510320061A0A8C10C01902239B30D7CECADC20E6CEEF1FF55FF9DAFCDA00640154032606260AA110E1193223E03136CE6BDBDBE69DEEFFF54BF9CEFCD400650156032C06320AB51101196324253195CE0CDB96E66DEEDEF536F9C2FCCE006601590332063E0ACA11221993246A31F4CDAEDB51E63CEEBEF522F9B6FCC80067015B0338064A0ADE114219C424AF3252CD4FDB0CE60CEE9DF50DF9AAFCC20067015E033E06560AF3116319F424F432B1CCF0DAC7E5DBEE7DF4F9F99EFCBC00680160034406620B0711831A2525393310CC92DA82E5ABEE5CF4E4F993FCB600690163034A066D0B1C11A41A55257E336ECC33DA3DE57AEE3CF4D0F987FCB0006A0166035006790B3011C41A8625C333CDCBD4D9F8E54AEE1BF4BCF97BFCAA006A0168035606850B4411E51AB62608342CCB75D9B3E519EDFBF4A7F96FFCA4006B016B035C06910B5912051AE7264D348BCB17D96EE4E9EDDAF493F963FC9E006C016D0362069D0B6D12261B17269234E9CAB8D928E4B8EDBAF47EF957FC97006D0170036906A90B8212461B4826D83548CA59D8E3E488ED99F46AF94CFC91006D0172036F06B40B9612671B78271D35A7C9FAD89EE457ED79F455F940FC8B006E0175037506C00BAB12871BA927623606C99CD859E427ED58F441F934FC85006F0177037B06CC0BBF12A81BD927A73664C93DD814E3F6ED38F42CF928FC7F0070017A038106D80BD412C81C0A27EC36C3C8DED7CFE3C6ED17F418F91CFC790070017D038706E40BE812E91C3A28313722C880D78AE395ECF7F403F910FC730071017F038D06F00BFD13091C6B28763780C821D745E365ECD6F3EFF905FC6D00720182039306FB0C11132A1C9B28BB37DFC7C2D700E334ECB6F3DAF8F9FC6700730184039907070C26134A1CCC2900383EC763D6BBE304EC95F3C6F8EDFC6100730187039F07130C3A136B1CFC2945389DC705D676E2D3EC75F3B2F8E1FC5B0074018903A5071F0C4E138B1D2D298A38FBC6A6D631E2A3EC54F39DF8D5FC550075018C03AB072B0C6313AC1D5D29CF395AC647D5ECE272EC34F389F8C9FC4F0076018E03B107370C7713CC1D8E2A1439B9C5E8D5A7E242EC13F374F8BDFC490076019103B707430C8C13ED1DBE2A593A18C58AD562E211EBF3F360F8B2FC430077019403BD074E0CA0140D1DEF2A9E3A76C52BD51DE1E1EBD2F34BF8A6FC3D0078019603C3075A0CB5142E1E1F2AE33AD5C4CCD4D8E1B0EBB2F337F89AFC360079019903CA07660CC9144E1E502B283B34C46ED493E180EB91F322F88EFC30007A019B03D007720CDE146F1E802B6D3B92C40FD44DE14FEB71F30EF882FC2A007A019E03D6077E0CF2148F1EB12BB33BF1C3B0D408E11FEB50F2F9F876FC24007B01A003DC078A0D0714B01EE12BF83C50C351D3C3E0EEEB30F2E5F86BFC1E007C01A303E207950D1B14D01F122C3D3CAFC2F3D37EE0BEEB0FF2D0F85FFC18007D01A603E807A10D3014F11F422C823D0DC294D339E08DEAEFF2BCF853FC12007D01A803EE07AD0D4415111F732CC73D6CC235D2F4E05DEACEF2A8F847FC0C007E01AB03F407B90D5815321FA32D0C3DCBC1D6D2AFE02CEAAEF293F83BFC06007F01AD03FA07C50D6D15521FD42D513E2AC178D26ADFFCEA8DF27FF82FFC00008001B0040007D10D81157320042D963E88C119D225DFCBEA6DF26AF824FBFA008001B2040607DC0D96159320352DDB3EE7C0BAD1E0DF9BEA4DF256F818FBF4008101B5040C07E80DAA15B320652E203F46C05CD19BDF6AEA2CF241F80CFBEE008201B7041207F40DBF15D420962E653FA4BFFDD156DF3AEA0CF22DF800FBE8008301BA041808000DD315F420C62EAA4003BF9ED111DF09E9EBF218F7F4FBE2008301BD041E080C0DE8161520F72EEF4062BF3FD0CCDED9E9CBF204F7E8FBDC008401BF042408180DFC163521272F3440C1BEE1D087DEA8E9AAF1EFF7DDFBD5008501C2042B08230E11165621582F79411FBE82D042DE78E98AF1DBF7D1FBCF008601C40431082F0E25167621882FBE417EBE23CFFDDE47E969F1C6F7C5FBC9008601C70437083B0E3A169721B9300341DDBDC4CFB8DE17E949F1B2F7B9FBC3008701C9043D08470E4E16B721E93048423CBD66CF72DDE6E928F19EF7ADFBBD008801CC044308530E6216D8221A308E429ABD07CF2DDDB6E908F189F7A1FBB7008901CE0449085F0E7716F8224A30D342F9BCA8CEE8DD85E8E7F175F795FBB1008901D1044F086B0E8B1719227B31184358BC4ACEA3DD55E8C7F160F78AFBAB008A01D4045508760EA0173922AB315D43B6BBEBCE5EDD24E8A6F14CF77EFBA5008B01D6045B08820EB4175A22DC31A24415BB8CCE19DCF4E886F137F772FB9F008C01D90461088E0EC9177A230C31E74474BB2DCDD4DCC3E865F123F766FB99008C01DB0467089A0EDD179B233D322C44D3BACFCD8FDC93E845F10EF75AFB93008D01DE046D08A60EF217BB236D32714531BA70CD4ADC62E824F0FAF74EFB8D008E01E0047308B20F0617DC239E32B64590BA11CD05DC32E804F0E5F743FB87008F01E3047908BD0F1B17FC23CE32FB45EFB9B2CCC0DC01E7E3F0D1F737FB81008F01E5047F08C90F2F181D23FF3340464EB954CC7BDBD1E7C3F0BCF72BFB7B009001E8048508D50F44183D242F338546ACB8F5CC36DBA0E7A2F0A8F71FFB74009101EB048C08E10F58185E246033CA470BB896CBF1DB70E782F094F713FB6E009201ED049208ED0F6C187E2490340F476AB838CBACDB3FE761F07FF707FB68009301F0049808F90F81189F24C1345447C8B7D9CB67DB0FE741F06BF6FCFB62009301F2049E09040F9518BF24F134994827B77ACB22DADEE720F056F6F0FB5C009401F504A409100FAA18E0252234DE4886B71BCADDDAAEE700F042F6E4FB56009501F704AA091C0FBE19002552352348E5B6BDCA97DA7DE6DFF02DF6D8FB50009601FA04B009280FD31921258335694943B65ECA52DA4DE6BFF019F6CCFB4A009601FC04B609340FE7194125B335AE49A2B5FFCA0DDA1CE69EF004F6C0FB44009701FF04BC09400FFC196225E435F34A01B5A0C9C8D9ECE67EEFF0F6B4FB3E0098020204C2094C10101982261436384A60B542C983D9BBE65DEFDBF6A9FB380099020404C80957102519A32645367D4ABEB4E3C93ED98BE63DEFC7F69DFB320099020704CE0963103919C3267536C24B1DB484C8F9D95AE61CEFB2F691FB2C009A020904D4096F104E19E426A637074B7CB426C8B4D92AE5FCEF9EF685FB26009B020C04DA097B10621A0426D6374C4BDAB3C7C86FD8F9E5DBEF8AF679FB20009C020E04E0098710761A25270737914C39B368C82AD8C9E5BBEF75F66DFB1A009C021104E60993108B1A45273737D64C98B309C7E5D898E59AEF61F662FB13009D021304ED099E109F1A662768381B4CF7B2ABC7A0D868E57AEF4CF656FB0D009E021604F309AA10B41A86279838604D55B24CC75BD837E559EF38F64AFB07009F021904F909B610C81AA727C938A54DB4B1EDC716D807E539EF23F63EFB01009F021B04FF09C210DD1AC727F938EA4E13B18EC6D1D7D6E518EF0FF632FAFB00A0021E050509CE10F11AE8282A392F4E72B130C68CD7A6E4F8EEFAF626FAF500A10220050B09DA11061B08285A39744ED0B0D1C647D775E4D7EEE6F61BFAEF00A20223051109E5111A1B29288B39B94F2FB072C602D745E4B7EED1F60FFAE900A20225051709F1112F1B4928BB39FE4F8EB014C5BCD714E496EEBDF603FAE300A30228051D09FD11431B6A28EC3A444FECAFB5C577D6E4E476EEA8F5F7FADD00A4022B05230A0911581B8A291C3A89504BAF56C532D6B3E455EE94F5EBFAD700A5022D05290A15116C1BAB294D3ACE50AAAEF7C4EDD683E435EE80F5DFFAD100A50230052F0A2111801BCB297D3B135109AE99C4A8D652E414EE6BF5D4FACB00A6023205350A2C11951BEC29AE3B585167AE3AC463D622E3F4EE57F5C8FAC500A70235053B0A3811A91C0C29DE3B9D51C6ADDBC41ED5F1E3D3EE42F5BCFABF00A8023705410A4411BE1C2D2A0F3BE25225AD7CC3D9D5C1E3B3EE2EF5B0FAB900A8023A05470A5011D21C4D2A3F3C275284AD1EC394D590E392EE19F5A4FAB200A9023C054E0A5C11E71C6E2A703C6C52E2ACBFC34FD560E372EE05F598FAAC00AA023F05540A6811FB1C8E2AA03CB15341AC60C30AD52FE351EDF0F58CFAA600AB0242055A0A7412101CAF2AD13CF653A0AC02C2C5D4FFE331EDDCF581FAA000AC024405600A7F12241CCF2B013D3B53FEABA3C280D4CEE311EDC7F575FA9A00AC024705660A8B12391CEF2B323D80545DAB44C23BD49EE2F0EDB3F569FA9400AD0249056C0A97124D1D102B623DC554BCAAE5C1F6D46DE2D0ED9EF55DFA8E00AE024C05720AA312621D302B933E0A551BAA87C1B1D43DE2AFED8AF551FA8800AF024E05780AAF12761D512BC33E4F5579AA28C16CD40CE28FED76F545FA8200AF0251057E0ABB128A1D712BF43E9455D8A9C9C127D3DCE26EED61F53AFA7C00B0025305840AC6129F1D922C243ED95637A96AC0E1D3ABE24EED4DF52EFA7600B10256058A0AD212B31DB22C553F1F5696A90CC09CD37BE22DED38F522FA7000B2025905900ADE12C81DD32C853F6456F4A8ADC057D34AE20DED24F516FA6A00B2025B05960AEA12DC1DF32CB63FA95753A84EC012D31AE1ECED0FF50AFA6400B3025E059C0AF612F11E142CE63FEE57B2A7F0BFCDD2E9E1CCECFBF4FEFA5E00B4026005A20B0213051E342D1740335810A791BF88D2B9E1ABECE6F4F3FA5800B5026305A80B0D131A1E552D474078586FA732BF43D288E18BECD2F4E7FA5100B5026505AF0B19132E1E752D7840BD58CEA6D3BEFED258E16AECBDF4DBFA4B00B6026805B50B2513431E962DA84102592DA675BEB9D227E14AECA9F4CFFA4500B7026A05BB0B3113571EB62DD94147598BA616BE74D1F7E129EC94F4C3FA3F00B8026D05C10B3D136C1ED72E09418C59EAA5B7BE2FD1C6E109EC80F4B7FA3900B8027005C70B4913801EF72E3A41D15A49A558BDEAD196E0E8EC6CF4ABFA3300B9027205CD0B5513941F182E6A42165AA8A4FABDA5D165E0C8EC57F4A0FA2D00BA027505D30B6013A91F382E9B425B5B06A49BBD60D135E0A7EC43F494FA2700BB027705D90B6C13BD1F592ECB42A05B65A43CBD1BD104E087EC2EF488FA2100BB027A05DF0B7813D21F792EFC42E55BC4A3DEBCD6D0D4E066EC1AF47CFA1B00BC027C05E50B8413E61F9A2F2C432A5C22A37FBC91D0A3E046EC05F470FA1500BD027F05EB0B9013FB1FBA2F5D436F5C81A320BC4CD073E025EBF1F464FA0F00BE028105F10B9C140F1FDB2F8D43B45CE0A2C1BC06D042E005EBDCF459FA0900BE028405F70BA714241FFB2FBE43FA5D3FA263BBC1D012DFE4EBC8F44DFA0300BF028705FD0BB31438201C2FEE443F5D9DA204BB7CCFE1DFC4EBB3F441F9FD00C0028906030BBF144D203C301F44845DFCA1A5BB37CFB1DFA3EB9FF435F9F700C1028C06090BCB1461205D304F44C95E5BA146BAF2CF80DF83EB8AF429F9F000C2028E06100BD71476207D3080450E5EBA");

	// First 6 section
	outputBuffer = new unsigned char[0x100000];
	for (int x = 0; x < 0x100000; x++)
		outputBuffer[x] = 0x00;

	outputBufferLength = 0;

	int inputSpot = 0;

	signed short previousPredictor = 0;
	signed short previousValue = 0;

	int countPerIndex = scale - 1;

	if ((dataLength % ((6 * 2) + (8 * 2 * countPerIndex))) != 0)
		dataLength = dataLength - (dataLength % ((6 * 2) + (8 * 2 * countPerIndex)));
	// Later truncate dataLength
	while (inputSpot < dataLength)
	{
		double bestPredictorDistance = 9999999999;
		int bestPredictor = -1;
		unsigned char* bestOutput = new unsigned char[4 + countPerIndex * 4];
		signed short bestPreviousPredictor = -1;
		signed short bestPreviousValue = -1;

		for (int predictorTest = 0; predictorTest < 0x100; predictorTest++)
		//int predictorTest = 0x05;
		{
			signed short tempPreviousPredictor = previousPredictor;
			signed short tempPreviousValue = previousValue;

			double totalDelta = 0;

			int tempOutputSpot = 0;
			unsigned char* tempOutputBuffer = new unsigned char[4 + countPerIndex * 4];
			for (int x = 0; x < 4 + countPerIndex * 4; x++)
				tempOutputBuffer[x] = 0;
			unsigned char* predictor = &predictorTable[predictorTest * 0x20];

			int tempInputSpot = inputSpot;

			signed short headerSamples[6];
			for (int x = 0; x < 6; x++)
				headerSamples[x] = CSharedFunctions::CharArrayToShort(&data[tempInputSpot + x * 2]);
			
			// Predictor Index is first 8-bits
			tempOutputBuffer[3] = predictorTest;

			int predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, headerSamples[0], predictor);
			tempOutputBuffer[tempOutputSpot + 2] = predictorIndex;
			tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
			tempPreviousValue += tempPreviousPredictor;
			totalDelta += fabs((double)tempPreviousValue - (double)headerSamples[0]);

			predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, headerSamples[1], predictor);
			tempOutputBuffer[tempOutputSpot + 2] = tempOutputBuffer[tempOutputSpot + 2] | (predictorIndex << 4);
			tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
			tempPreviousValue += tempPreviousPredictor;
			totalDelta += fabs((double)tempPreviousValue - (double)headerSamples[1]);

			predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, headerSamples[2], predictor);
			tempOutputBuffer[tempOutputSpot + 1] = predictorIndex;
			tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
			tempPreviousValue += tempPreviousPredictor;
			totalDelta += fabs((double)tempPreviousValue - (double)headerSamples[2]);

			predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, headerSamples[3], predictor);
			tempOutputBuffer[tempOutputSpot + 1] = tempOutputBuffer[tempOutputSpot + 1] | (predictorIndex << 4);
			tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
			tempPreviousValue += tempPreviousPredictor;
			totalDelta += fabs((double)tempPreviousValue - (double)headerSamples[3]);

			predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, headerSamples[4], predictor);
			tempOutputBuffer[tempOutputSpot + 0] = predictorIndex;
			tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
			tempPreviousValue += tempPreviousPredictor;
			totalDelta += fabs((double)tempPreviousValue - (double)headerSamples[4]);

			predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, headerSamples[5], predictor);
			tempOutputBuffer[tempOutputSpot + 0] = tempOutputBuffer[tempOutputSpot + 0] | (predictorIndex << 4);
			tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
			tempPreviousValue += tempPreviousPredictor;
			totalDelta += fabs((double)tempPreviousValue - (double)headerSamples[5]);

			tempOutputSpot += 4;
			tempInputSpot += 6 * 2;

			for (int samePredictor = 0; samePredictor < countPerIndex; samePredictor++)
			{
				signed short dataSamples[8];
				for (int x = 0; x < 8; x++)
					dataSamples[x] = CSharedFunctions::CharArrayToShort(&data[tempInputSpot + x * 2]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[0], predictor);
				tempOutputBuffer[tempOutputSpot + 3] = predictorIndex;
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[0]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[1], predictor);
				tempOutputBuffer[tempOutputSpot + 3] = tempOutputBuffer[tempOutputSpot + 3] | (predictorIndex << 4);
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[1]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[2], predictor);
				tempOutputBuffer[tempOutputSpot + 2] = predictorIndex;
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[2]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[3], predictor);
				tempOutputBuffer[tempOutputSpot + 2] = tempOutputBuffer[tempOutputSpot + 2] | (predictorIndex << 4);
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[3]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[4], predictor);
				tempOutputBuffer[tempOutputSpot + 1] = predictorIndex;
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[4]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[5], predictor);
				tempOutputBuffer[tempOutputSpot + 1] = tempOutputBuffer[tempOutputSpot + 1] | (predictorIndex << 4);
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[5]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[6], predictor);
				tempOutputBuffer[tempOutputSpot + 0] = predictorIndex;
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[6]);

				predictorIndex = FindBestPredictorIndex(tempPreviousPredictor, tempPreviousValue, dataSamples[7], predictor);
				tempOutputBuffer[tempOutputSpot + 0] = tempOutputBuffer[tempOutputSpot + 0] | (predictorIndex << 4);
				tempPreviousPredictor += (short)CSharedFunctions::CharArrayToShort(&predictor[predictorIndex * 2]);
				tempPreviousValue += tempPreviousPredictor;
				totalDelta += fabs((double)tempPreviousValue - (double)dataSamples[7]);

				tempOutputSpot += 4;
				tempInputSpot += 8 * 2;
			}

			if (totalDelta < bestPredictorDistance)
			{
				bestPredictorDistance = totalDelta;
				memcpy(bestOutput, tempOutputBuffer, 4 + countPerIndex * 4);

				bestPreviousPredictor = tempPreviousPredictor;
				bestPreviousValue = tempPreviousValue;
				bestPredictor = predictorTest;
			}

			delete [] tempOutputBuffer;
		}

		previousValue = bestPreviousValue;
		previousPredictor = bestPreviousPredictor;
		memcpy(&outputBuffer[outputBufferLength], bestOutput, 4 + countPerIndex * 4);
		outputBufferLength += (4 + countPerIndex * 4);
		inputSpot += ((6 + countPerIndex * 8) * 2);
		delete [] bestOutput;
	}
	return true;
}