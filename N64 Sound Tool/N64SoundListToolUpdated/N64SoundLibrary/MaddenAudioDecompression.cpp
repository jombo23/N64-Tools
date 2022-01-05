#include "StdAfx.h"
#include "MaddenAudioDecompression.h"
#include "SharedFunctions.h"
#include <vector>

CMaddenAudioDecompression::CMaddenAudioDecompression(void)
{
}

CMaddenAudioDecompression::~CMaddenAudioDecompression(void)
{
}

unsigned long FUN_00027900(unsigned char* RAM, unsigned long param_1,unsigned long param_2)

{
	//byte **param_1
	//unsigned int param_2
  byte bVar1;
  unsigned long pbVar2Offset; //byte *
  unsigned long uVar3;
  unsigned long pbVar4Offset; //byte *
  
  pbVar2Offset = CSharedFunctions::CharArrayToLong(RAM, param_1 + 1 * 4);
  uVar3 = CSharedFunctions::CharArrayToLong(RAM, param_2 * 4 + 0x8004A370);
  pbVar4Offset = CSharedFunctions::CharArrayToLong(RAM, param_1 + 2 * 4) + -param_2;
  CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 1 * 4, ((unsigned long)pbVar2Offset >> (param_2 & 0x1f)));
  CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 2 * 4, pbVar4Offset);
  if ((int)pbVar4Offset < 8) {
    bVar1 = CSharedFunctions::CharArrayToChar(RAM, CSharedFunctions::CharArrayToLong(RAM, param_1));
    CSharedFunctions::WriteLongToBuffer(RAM, param_1,  CSharedFunctions::CharArrayToLong(RAM, param_1) + 1);
    CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 2 * 4, pbVar4Offset + 8);
    CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 1 * 4, ((unsigned long)CSharedFunctions::CharArrayToLong(RAM, param_1 + 1 * 4) | (unsigned long)bVar1 << ((unsigned long)pbVar4Offset & 0x1f)));
  }
  return (unsigned long)pbVar2Offset & uVar3;
}



void FUN_00028800(unsigned char* RAM, unsigned long param_1, unsigned long param_2, int param_3)

{
  //byte *param_1;
  //byte **param_2;
  byte bVar1;
  int iVar2;
  unsigned long pbVar4Offset; // byte*
  int iVar5;
  __int64 lVar3;
  unsigned long ppbVar6Offset; // byte**
  int iVar7;
  double dVar8;
  
  bVar1 = CSharedFunctions::CharArrayToChar(RAM, param_1);
  CSharedFunctions::WriteLongToBuffer(RAM, param_2 + 2 * 4, 0x8);
  CSharedFunctions::WriteLongToBuffer(RAM, param_2, param_1 + 1);
  CSharedFunctions::WriteLongToBuffer(RAM, param_2 + 1 * 4, (unsigned long)bVar1);
  pbVar4Offset = FUN_00027900(RAM, param_2, 1);
  CSharedFunctions::WriteLongToBuffer(RAM, param_2 + 3 * 4, pbVar4Offset);
  iVar5 = FUN_00027900(RAM, param_2, 4);
  CSharedFunctions::WriteLongToBuffer(RAM, param_2 + 4 * 4, (0x20 - iVar5));
  iVar5 = FUN_00027900(RAM, param_2, 4);
  dVar8 = (double)(iVar5 + 1);
  if (iVar5 + 1 < 0) {
	  dVar8 = dVar8 + CSharedFunctions::CharArrayToDouble(RAM, 0x80000a88);
  }
  CSharedFunctions::WriteFloatToBuffer(RAM, param_2 + 5 * 4, (float)(dVar8 * CSharedFunctions::CharArrayToDouble(RAM, 0x80000a90)));
  lVar3 = FUN_00027900(RAM, param_2, 6);
  dVar8 = (double)(int)lVar3;
  if (lVar3 < 0) {
    dVar8 = dVar8 + CSharedFunctions::CharArrayToDouble(RAM, 0x80000a98);
  }
  iVar7 = 1;
  dVar8 = dVar8 * CSharedFunctions::CharArrayToDouble(RAM, 0x80000aa0) + CSharedFunctions::CharArrayToDouble(RAM, 0x80000aa8);
  iVar5 = 0;
  ppbVar6Offset = param_2;
  do {
    iVar2 = iVar5 + 0x14;
    iVar5 = iVar5 + 4;
    iVar7 = iVar7 + 1;
    CSharedFunctions::WriteFloatToBuffer(RAM, ppbVar6Offset + 6 * 4, (CSharedFunctions::CharArrayToFloat(RAM, (int)param_2 + iVar2) * (float)dVar8));
    ppbVar6Offset = ppbVar6Offset + 1 * 4;
  } while (iVar7 < 0x40);
  iVar5 = 0;
  ppbVar6Offset = param_2;
  do {
    CSharedFunctions::WriteLongToBuffer(RAM, ppbVar6Offset + 0x45 * 4, 0x0);
    CSharedFunctions::WriteLongToBuffer(RAM, ppbVar6Offset + 0x51 * 4, 0x0);
    iVar5 = iVar5 + 1;
    ppbVar6Offset = ppbVar6Offset + 4;
  } while (iVar5 < 0xc);
  iVar5 = 0x143;
  param_2 = param_2 + 0x143 * 4;
  do {
	  CSharedFunctions::WriteLongToBuffer(RAM, param_2 + 0x5d * 4, 0x0);
    iVar5 = iVar5 + -1;
    param_2 = param_2 + -1 * 4;
  } while (-1 < iVar5);

	/*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
	fwrite(RAM, 1, 0x400000, a);
	fflush(a);
	fclose(a);*/

  return;
}


void FUN_00027970(unsigned char* RAM, unsigned long param_1, unsigned int param_2)

{
	//param_1  byte **
  byte bVar1;
  unsigned long pbVar2Offset; //byte *pbVar2;
  
  pbVar2Offset = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 2 * 4) + -(int)param_2;
  CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 1 * 4, ((unsigned int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 1 * 4) >> (param_2 & 0x1f)));
  CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 2 * 4, pbVar2Offset);
  if ((int)pbVar2Offset < 8) {
	  bVar1 = CSharedFunctions::CharArrayToChar(RAM, CSharedFunctions::CharArrayToLong(RAM, param_1));
	  CSharedFunctions::WriteLongToBuffer(RAM, param_1, CSharedFunctions::CharArrayToLong(RAM, param_1) + 1);
    CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 2 * 4, pbVar2Offset + 8);
    CSharedFunctions::WriteLongToBuffer(RAM, param_1 + 1 * 4, CSharedFunctions::CharArrayToLong(RAM, param_1 + 1 * 4) | ((unsigned int)bVar1 << ((unsigned int)pbVar2Offset & 0x1f)));
  }
  return;
}



/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_000279c8(unsigned char* RAM, int param_1, __int64 param_2, unsigned long param_3,int param_4)

{
	//param_3 float*
  byte bVar1;
  int iVar3;
  __int64 lVar2;
  unsigned int uVar4;
  int iVar5;
  unsigned long pfVar6Offset; //float *pfVar6;
  int iVar7;
  unsigned long uVar8;
  int iVar9;
  unsigned long pfVar10Offset; //float *pfVar10;
  float uVar11; // Not sure why Ghidra says unsigned __int64
  float uVar12; // Not sure why Ghidra says unsigned __int64
  
/*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  if (param_2 == 0) {
    pfVar10Offset = param_3 + 0x6c * 4;
	uVar12 = CSharedFunctions::CharArrayToFloat(RAM, 0x80000a60);
    uVar11 = CSharedFunctions::CharArrayToFloat(RAM, 0x80000a64);
    do {
      uVar4 = CSharedFunctions::CharArrayToLong(RAM, param_1 + 4) & 3;
      if (uVar4 == 1) {
        CSharedFunctions::WriteFloatToBuffer(RAM, param_3, (float)uVar12);
LAB_00027bc4:
        uVar8 = 2;
LAB_00027bcc:
        FUN_00027970(RAM, param_1,uVar8);
      }
      else {
        if (uVar4 == 0) {
          CSharedFunctions::WriteFloatToBuffer(RAM, param_3, 0.0);
LAB_00027bb4:
          uVar8 = 1;
          goto LAB_00027bcc;
        }
        if (uVar4 == 2) {
          CSharedFunctions::WriteFloatToBuffer(RAM, param_3, 0.0);
          goto LAB_00027bb4;
        }
        if (uVar4 == 3) {
          CSharedFunctions::WriteFloatToBuffer(RAM, param_3, (float)uVar11);
          goto LAB_00027bc4;
        }
      }
      param_3 = param_3 + param_4 * 4;
    } while ((int)param_3 < (int)pfVar10Offset);
  }
  else {
    iVar9 = 0;
    iVar5 = 0;
    pfVar10Offset = param_3;
    do {
      //80027a38
	  bVar1 = CSharedFunctions::CharArrayToChar(RAM, (unsigned int)CSharedFunctions::CharArrayToChar(RAM, param_1 + 7) + iVar5 + 0x8004A494);
      iVar3 = (unsigned long)bVar1 * 0xc;
      iVar5 = (int)CSharedFunctions::CharArrayToLong(RAM, iVar3 + -0x7ffb596c);
      FUN_00027970(RAM, param_1,(int)CSharedFunctions::CharArrayToLong(RAM, iVar3 + -0x7ffb5968));
      if (bVar1 < 4) {
        iVar3 = 7;
        if (bVar1 < 2) {
          while (lVar2 = FUN_00027900(RAM, param_1,1), lVar2 == 1) {
            iVar3 = iVar3 + 1;
          }
          lVar2 = FUN_00027900(RAM, param_1,1);
          if (lVar2 == 1) {
            CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, (float)iVar3);
          }
          else {
            CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, (float)-iVar3);
          }
          pfVar10Offset = pfVar10Offset + param_4 * 4;
          iVar9 = iVar9 + param_4;
        }
        else {
          iVar3 = FUN_00027900(RAM, param_1, 6);
          iVar3 = iVar3 + 7;
          if (0x6c < iVar9 + param_4 * iVar3) {
            iVar3 = (0x6c - iVar9) / param_4;
          }
          iVar7 = 0;
          if (0 < iVar3) {
            pfVar6Offset = param_3 + iVar9 * 4;
            do {
              CSharedFunctions::WriteFloatToBuffer(RAM, pfVar6Offset, 0.0);
              pfVar6Offset = pfVar6Offset + param_4 * 4;
              pfVar10Offset = pfVar10Offset + param_4 * 4;
              iVar7 = iVar7 + 1;
              iVar9 = iVar9 + param_4;
            } while (iVar7 < iVar3);
          }
        }
      }
      else {
        iVar9 = iVar9 + param_4;
        CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, CSharedFunctions::CharArrayToFloat(RAM, iVar3 + -0x7ffb5964));
        pfVar10Offset = pfVar10Offset + param_4 * 4;
      }
      iVar5 = iVar5 << 8;
    } while (iVar9 < 0x6c);
  }
  return;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00027c20(unsigned char* RAM, unsigned long param_1)

{
	//param_1 float *
  double dVar1;
  double dVar2;
  double dVar3;
  int iVar4;
  
  dVar3 = CSharedFunctions::CharArrayToDouble(RAM, 0x80000a78);
  dVar2 = CSharedFunctions::CharArrayToDouble(RAM, 0x80000a70);
  dVar1 = CSharedFunctions::CharArrayToDouble(RAM, 0x80000a68);
  iVar4 = 0;
  do {
    iVar4 = iVar4 + 2;
	CSharedFunctions::WriteFloatToBuffer(RAM, param_1, (float)((double)(CSharedFunctions::CharArrayToFloat(RAM, param_1 + -1 * 4) + CSharedFunctions::CharArrayToFloat(RAM, param_1 + 1 * 4)) * dVar1 +
                       (double)(CSharedFunctions::CharArrayToFloat(RAM, param_1 + -3 * 4) + CSharedFunctions::CharArrayToFloat(RAM, param_1 + 3 * 4)) * dVar2 +
                      (double)(CSharedFunctions::CharArrayToFloat(RAM, param_1 + -5 * 4) + CSharedFunctions::CharArrayToFloat(RAM, param_1 + 5 * 4)) * dVar3));
    param_1 = param_1 + 2 * 4;
  } while (iVar4 < 0x6c);
  return;
}




/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00027ca0(unsigned char* RAM, int param_1, unsigned long param_2, unsigned long SP)

{
	SP = SP - 0x60;
	//float *param_2;
  unsigned long puVar1Offset; // float
  unsigned long pfVar2Offset; // float
  int iVar3;
  int iVar4;
  unsigned long pfVar5Offset; // float
  unsigned long puVar6Offset; // float
  unsigned long pfVar7Offset; // float
  int iVar8;
  unsigned long pfVar9Offset; // float
  unsigned long pfVar10Offset; // float
  float uVar11; // float
  float fVar12;
  float fVar13;
  float fVar14;
  unsigned long afStack96Offset = SP;//float [10]
  unsigned long afStack56Offset = SP + 0x28; //float [14];
  
  iVar3 = 10;
  puVar6Offset = (param_1 + 0x28);
  iVar4 = 0x2c;
  do {
	  uVar11 = CSharedFunctions::CharArrayToFloat(RAM, puVar6Offset);
    puVar6Offset = puVar6Offset + -1 * 4;
    puVar1Offset = ((int)afStack96Offset + iVar4);
    iVar4 = iVar4 + -4;
    iVar3 = iVar3 + -1;
    CSharedFunctions::WriteFloatToBuffer(RAM, puVar1Offset, uVar11);
  } while (-1 < iVar3);
  iVar3 = 0;
  pfVar9Offset = afStack56Offset + 2 * 4;

  // Why is Ghidra missing this?
  CSharedFunctions::WriteFloatToBuffer(RAM, SP, CSharedFunctions::CharArrayToFloat(RAM, 0x80000A80));
  // End what did Ghidra do

  pfVar10Offset = param_2;
  do {
    iVar4 = 10;
    iVar8 = 0x2c;
    pfVar7Offset = afStack56Offset;
    pfVar5Offset = (param_1 + 0x28);
    fVar14 = -CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x2c) * CSharedFunctions::CharArrayToFloat(RAM, afStack56Offset + 1 * 4);

	 /*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

    do {
      fVar12 = CSharedFunctions::CharArrayToFloat(RAM, pfVar5Offset);
      fVar13 = CSharedFunctions::CharArrayToFloat(RAM, pfVar7Offset);
      fVar14 = fVar14 - fVar12 * fVar13;
      pfVar2Offset = ((int)afStack96Offset + iVar8);
      iVar8 = iVar8 + -4;
      pfVar7Offset = pfVar7Offset + -1 * 4;
      pfVar5Offset = pfVar5Offset + -1 * 4;
      iVar4 = iVar4 + -1;
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar2Offset, fVar12 * fVar14 + fVar13);
    } while (-1 < iVar4);
	// Why is Ghidra missing this spot again somehow?
	CSharedFunctions::WriteFloatToBuffer(RAM, SP, fVar14);
	// End what did Ghidra do again

    iVar4 = 0;
    CSharedFunctions::WriteFloatToBuffer(RAM, pfVar9Offset, fVar14);
    pfVar5Offset = param_2;
    if (0 < iVar3) {
      do {
        iVar8 = iVar3 - iVar4;
        iVar4 = iVar4 + 1;
		float multiply = (CSharedFunctions::CharArrayToFloat(RAM, pfVar5Offset) * CSharedFunctions::CharArrayToFloat(RAM, afStack56Offset + 2 * 4 + iVar8 * 4 + -1 * 4));
        fVar14 = fVar14 - multiply;
        pfVar5Offset = pfVar5Offset + 1 * 4;
      } while (iVar4 < iVar3);
    }

	// Floats are being done differently in Nemu, TODO figure out what to do
    CSharedFunctions::WriteFloatToBuffer(RAM, pfVar10Offset, fVar14);
    pfVar10Offset = pfVar10Offset + 1 * 4;
    iVar3 = iVar3 + 1;
    pfVar9Offset = pfVar9Offset + 1 * 4;
  } while (iVar3 < 0xc);
  SP = SP + 0x60;
  return;
}

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00027da0(unsigned char* RAM, int param_1,int param_2,int param_3, __int64 param_4, unsigned long SP)

{
	SP = SP - 0x70;
  unsigned long pfVar1Offset; //float *pfVar1;
  unsigned long pfVar2Offset; //float *pfVar2;
  int iVar3;
  float fVar4;
  float fVar5;
  
  fVar5 = 0.0;
  if (param_4 != 0) {
    fVar5 = CSharedFunctions::CharArrayToFloat(RAM, 0x80000a84);
  }
  FUN_00027ca0(RAM, param_1 + 0x114, SP + 0x10, SP);

  float fStack96 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x10);
  float fStack92 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x14);
  float fStack88 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x18);
  float fStack84 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x1C);
  float fStack80 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x20);
  float fStack76 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x24);
  float fStack72 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x28);
  float fStack68 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x2C);
  float fStack64 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x30);
  float fStack60 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x34);
  float fStack56 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x38);
  float fStack52 = CSharedFunctions::CharArrayToFloat(RAM, SP + 0x3C);

  
  /*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  // AT HERE THOUGH FLOATS NOT IDENTICAL

  pfVar2Offset = (param_1 + param_2 * 4 + 0x684);
  iVar3 = 0;
  if (0 < param_3) {
    pfVar1Offset = pfVar2Offset + 0xb * 4;
    do {
		fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar2Offset) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x170), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar2Offset, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -10 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x16c), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -10 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -9 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x168), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -9 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -8 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x164), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -8 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -7 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x160), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -7 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -6 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x15c), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -6 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -5 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x158), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -5 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -4 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x154), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -4 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -3 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x150), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -3 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -2 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x14c), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -2 * 4, fVar4 + fVar5);
      fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset + -1 * 4) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148);
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x148), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset + -1 * 4, fVar4 + fVar5);
	  fVar4 = CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset) + fStack96 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x148) +
              fStack92 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x14c) + fStack88 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x150) +
              fStack84 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x154) + fStack80 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x158) +
              fStack76 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x15c) + fStack72 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x160) +
              fStack68 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x164) + fStack64 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x168) +
              fStack60 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x16c) + fStack56 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x170) +
              fStack52 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x144);
      iVar3 = iVar3 + 1;
      pfVar2Offset = pfVar2Offset + 0xc * 4;
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + 0x144), fVar4);
      CSharedFunctions::WriteFloatToBuffer(RAM, pfVar1Offset, fVar4 + fVar5);
      pfVar1Offset = pfVar1Offset + 0xc * 4;
    } while (iVar3 < param_3);
  }
  SP = SP + 0x70;
  return;
}


void FUN_00028950(unsigned char* RAM, int param_1, int param_2)

{
	unsigned long SP = 0x800529F8;
  unsigned long pfVar1Offset; // float *
  bool bVar2;
  unsigned int uVar4;
  int iVar5;
  int iVar6;
  __int64 lVar3;
  int iVar7;
  int iVar8;
  int iVar9;
  unsigned long puVar10Offset; // int *
  int iVar11;
  int iVar12;
  int iVar13;
  unsigned long pfVar14Offset; // float *
  double dVar15;
  float fVar16;
  float fVar17; 
  // These are all wrong need to fix...by breakpointing
  unsigned long afStack576Offset = SP + 0x10; //float afStack576 [4];
  unsigned long afStack560Offset = SP + 0x20; //float afStack560 [2];
  unsigned long auStack124Offset = SP + 0x1D4; //int auStack124 [5];
  unsigned long afStack104Offset = SP + 0x1E8; //float afStack104 [26];
  
  uVar4 = FUN_00027900(RAM, param_1,6);
  iVar11 = 1;
  pfVar14Offset = afStack104Offset;
  bVar2 = uVar4 < CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x10);
  CSharedFunctions::WriteFloatToBuffer(RAM, afStack104Offset + 0 * 4, (float)((double)(CSharedFunctions::CharArrayToFloat(RAM, uVar4 * 4 + 0x8004A394) - CSharedFunctions::CharArrayToFloat(RAM, param_1 + 0x114)) * CSharedFunctions::CharArrayToDouble(RAM, 0x80000ab0)));
  
  //800289B4
  dVar15 = CSharedFunctions::CharArrayToDouble(RAM, 0x80000ab0);
  iVar12 = param_1;
  do {
    pfVar14Offset = pfVar14Offset + 1 * 4;
    iVar5 = FUN_00027900(RAM, param_1,6);
    iVar11 = iVar11 + 1;
    CSharedFunctions::WriteFloatToBuffer(RAM, pfVar14Offset, (float)((double)(CSharedFunctions::CharArrayToFloat(RAM, iVar5 * 4 + 0x8004A394) - CSharedFunctions::CharArrayToFloat(RAM, iVar12 + 0x118))  * dVar15));
    iVar12 = iVar12 + 4;
  } while (iVar11 < 4);
  iVar12 = 4;
  pfVar14Offset = afStack104Offset + 4 * 4;
  iVar11 = param_1 + 0x10;
  dVar15 = CSharedFunctions::CharArrayToDouble(RAM, 0x80000ab8);

  //80028a38
  /*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  do {
    iVar5 = FUN_00027900(RAM, param_1,5);
    pfVar1Offset = (iVar11 + 0x114);
    iVar11 = iVar11 + 4;
    iVar12 = iVar12 + 1;
	CSharedFunctions::WriteFloatToBuffer(RAM, pfVar14Offset, (float)((double)(CSharedFunctions::CharArrayToFloat(RAM, (iVar5 + 0x10) * 4 + 0x8004A394) - CSharedFunctions::CharArrayToFloat(RAM, pfVar1Offset)) * dVar15));
    pfVar14Offset = pfVar14Offset + 1 * 4;
  } while (iVar12 < 0xc);
  iVar5 = 0;
  iVar11 = 0;
  iVar12 = 0xd8;

  /*a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  //80028a98
  do {
    iVar6 = FUN_00027900(RAM, param_1,8);
    lVar3 = FUN_00027900(RAM, param_1,4);
    dVar15 = (double)(int)lVar3;
    if (lVar3 < 0) {
      dVar15 = dVar15 + CSharedFunctions::CharArrayToDouble(RAM, 0x80000ac0);
    }
    fVar17 = (float)(dVar15 * CSharedFunctions::CharArrayToDouble(RAM, 0x80000ac8));
    iVar7 = FUN_00027900(RAM, param_1,6);
    fVar16 = CSharedFunctions::CharArrayToFloat(RAM, param_1 + iVar7 * 4 + 0x14);
    if ((int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xc) == 0) {
      FUN_000279c8(RAM, param_1,bVar2,afStack560Offset + 1 * 4,1);
    }
    else {
      iVar7 = FUN_00027900(RAM, param_1,1);
      lVar3 = FUN_00027900(RAM, param_1,1);


  /*a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  // 80028B50
      FUN_000279c8(RAM, param_1,bVar2,afStack560Offset + iVar7 * 4 + 1 * 4,2);
	  
  /*a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

      iVar13 = 0;
      if (lVar3 == 0) {
        iVar13 = 4;
        pfVar14Offset = afStack560Offset;
        do {
          CSharedFunctions::WriteFloatToBuffer(RAM, pfVar14Offset, 0.0);
          iVar13 = iVar13 + -1;
          pfVar14Offset = pfVar14Offset + -1 * 4;
        } while (-1 < iVar13);
        iVar13 = 0x71;
        puVar10Offset = auStack124Offset;
        do {
		  CSharedFunctions::WriteLongToBuffer(RAM, puVar10Offset, 0);
          iVar13 = iVar13 + 1;
          puVar10Offset = puVar10Offset + 1 * 4;
        } while (iVar13 < 0x76);
        FUN_00027c20(RAM, (int)afStack576Offset - (iVar7 * 4 + -0x18));
		fVar16 = (float)((double)fVar16 * CSharedFunctions::CharArrayToDouble(RAM, 0x80000ad0));
      }
      else {
        iVar8 = -(iVar7 + -6);
        do {
			CSharedFunctions::WriteFloatToBuffer(RAM, afStack576Offset + iVar8 * 4, 0.0);
          iVar13 = iVar13 + 2;
          iVar8 = iVar13 - (iVar7 + -6);
        } while (iVar13 < 0x6c);
      }
    }
	
	//80028BE8
    /*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
    fwrite(RAM, 1, 0x400000, a);
    fflush(a);
    fclose(a);*/

    iVar13 = 0;
    iVar7 = 0x14;
    do {
      iVar8 = (iVar12 - iVar6) + iVar13;
      iVar9 = iVar11 + iVar13;
      iVar13 = iVar13 + 1;
	  // 80028C24
      CSharedFunctions::WriteFloatToBuffer(RAM, (param_1 + iVar9 * 4 + 0x684), 
           fVar17 * CSharedFunctions::CharArrayToFloat(RAM, param_1 + iVar8 * 4 + 0x174) +
           fVar16 * CSharedFunctions::CharArrayToFloat(RAM, (int)afStack576Offset + iVar7));
      iVar7 = iVar7 + 4;
    } while (iVar13 < 0x6c);
    iVar11 = iVar11 + 0x6c;
    iVar5 = iVar5 + 1;
    iVar12 = iVar12 + 0x6c;
  } while (iVar5 < 4);

  //80028C48
  /*a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  iVar5 = 0;
  iVar11 = 0x1b0;
  iVar12 = param_1;
  do {
    iVar6 = param_1 + iVar11;
    iVar11 = iVar11 + 4;
    iVar5 = iVar5 + 1;
	CSharedFunctions::WriteLongToBuffer(RAM, (iVar12 + 0x174), (int)CSharedFunctions::CharArrayToLong(RAM, iVar6 + 0x684));
    iVar12 = iVar12 + 4;
  } while (iVar5 < 0x144);

  iVar11 = 0;
  pfVar14Offset = afStack104Offset;
  iVar12 = param_1;
  do {
	  fVar16 = CSharedFunctions::CharArrayToFloat(RAM, pfVar14Offset);
    pfVar14Offset = pfVar14Offset + 1 * 4;
    iVar11 = iVar11 + 1;
    CSharedFunctions::WriteFloatToBuffer(RAM, (iVar12 + 0x114), CSharedFunctions::CharArrayToFloat(RAM, iVar12 + 0x114) + fVar16);
    iVar12 = iVar12 + 4;
  } while (iVar11 < 0xc);

  /*a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  //80028cb0
  FUN_00027da0(RAM, param_1,0,1,param_2, SP);
  
  /*a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  iVar11 = 0;
  pfVar14Offset = afStack104Offset;
  iVar12 = param_1;
  do {
	  fVar16 = CSharedFunctions::CharArrayToFloat(RAM, pfVar14Offset);
    pfVar14Offset = pfVar14Offset + 1 * 4;
    iVar11 = iVar11 + 1;
    CSharedFunctions::WriteFloatToBuffer(RAM, (iVar12 + 0x114), CSharedFunctions::CharArrayToFloat(RAM, iVar12 + 0x114) + fVar16);
    iVar12 = iVar12 + 4;
  } while (iVar11 < 0xc);
  FUN_00027da0(RAM, param_1,0xc,1,param_2, SP);
  iVar11 = 0;
  pfVar14Offset = afStack104Offset;
  iVar12 = param_1;
  do {
	  fVar16 = CSharedFunctions::CharArrayToFloat(RAM, pfVar14Offset);
    pfVar14Offset = pfVar14Offset + 1 * 4;
    iVar11 = iVar11 + 1;
    CSharedFunctions::WriteFloatToBuffer(RAM, (iVar12 + 0x114), CSharedFunctions::CharArrayToFloat(RAM, iVar12 + 0x114) + fVar16);
    iVar12 = iVar12 + 4;
  } while (iVar11 < 0xc);
  FUN_00027da0(RAM, param_1,0x18,1,param_2, SP);
  iVar11 = 0;
  pfVar14Offset = afStack104Offset;
  iVar12 = param_1;
  do {
	  fVar16 = CSharedFunctions::CharArrayToFloat(RAM, pfVar14Offset);
    pfVar14Offset = pfVar14Offset + 1 * 4;
    iVar11 = iVar11 + 1;
    CSharedFunctions::WriteFloatToBuffer(RAM, (iVar12 + 0x114), CSharedFunctions::CharArrayToFloat(RAM, iVar12 + 0x114) + fVar16);
    iVar12 = iVar12 + 4;
  } while (iVar11 < 0xc);
  FUN_00027da0(RAM, param_1,0x24,0x21,param_2, SP);

  
  /*a = fopen("C:\\temp\\ram.bin", "wb");
  fwrite(RAM, 1, 0x400000, a);
  fflush(a);
  fclose(a);*/

  return;
}

void FUN_000276cc(unsigned char* RAM, unsigned long param_1, unsigned long param_2,int param_3, unsigned char* outputBuffer, int& outputBufferSize)

{
	//uint *param_1
	//short *param_2
  unsigned int uVar1;
  short uVar2;
  int iVar3;
  
  iVar3 = 0;
  if (0 < param_3) {
    do {
		uVar1 = CSharedFunctions::CharArrayToLong(RAM, param_1) & 0x1ffff;
      if (uVar1 - 0x8000 < 0x10000) {
        uVar2 = 0x8000;
        if (uVar1 < 0x10000) {
          uVar2 = 0x7fff;
        }

		// Added
		unsigned short temp = CSharedFunctions::Flip16Bit(uVar2);
		CSharedFunctions::WriteShortToBuffer(outputBuffer, outputBufferSize, temp);
		outputBufferSize += 2;
		// End Added

		CSharedFunctions::WriteShortToBuffer(RAM, param_2, uVar2);
      }
      else {
		  
		// Added
		unsigned short temp = CSharedFunctions::Flip16Bit((short)uVar1);
		CSharedFunctions::WriteShortToBuffer(outputBuffer, outputBufferSize, temp);
		outputBufferSize += 2;
		// End Added

        CSharedFunctions::WriteShortToBuffer(RAM, param_2, (short)uVar1);
      }
      param_2 = param_2 + 1 * 2;
      iVar3 = iVar3 + 1;
      param_1 = param_1 + 1 * 4;
    } while (iVar3 < param_3);
  }
  return;
}


__int64 FUN_0002772c(unsigned char* RAM, int param_1,int param_2, unsigned long param_3, unsigned char* outputBuffer, int& outputBufferSize)

{
  //int *param_3
  __int64 uVar1; // 0 or -1
  int iVar2;
  int iVar3;
  unsigned long puVar4Offset; // short
  int iVar5;
  
  if (CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd4c) < CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd44)) {
    iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, param_3);
    iVar2 = CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd4c) + param_2;
    iVar5 = iVar2 - (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd44);
    CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0xd4c), (int)iVar2);
    if (iVar5 < 1) {
      iVar5 = 0;
    }
    else {
      param_2 = param_2 - iVar5;
    }
    iVar2 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd48);
    if (param_2 < (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd48)) {
      iVar2 = param_2;
    }
    FUN_000276cc(RAM, param_1 + (0x1b0 - (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd48)) * 4 + 0x684,iVar3,iVar2, outputBuffer, outputBufferSize);
    puVar4Offset = (iVar3 + iVar2 * 2);
	CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0xd48), (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd48) - iVar2);
    for (param_2 = param_2 - iVar2; 0 < param_2; param_2 = param_2 - iVar3) {
      FUN_00028950(RAM, param_1,1);
      CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0xd48), 0x1b0);
      iVar3 = 0x1b0;
      if (param_2 < 0x1b0) {
        iVar3 = param_2;
      }
      FUN_000276cc(RAM, param_1 + 0x684,puVar4Offset,iVar3, outputBuffer, outputBufferSize);
      puVar4Offset = puVar4Offset + iVar3;
      CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0xd48), (int)((int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0xd48) - iVar3));

	  /*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
	  fwrite(RAM, 1, 0x400000, a);
	  fflush(a);
	  fclose(a);*/
    }
    if (iVar5 < 1) {
      uVar1 = 0;
    }
    else {
      do {
		  CSharedFunctions::WriteShortToBuffer(RAM, puVar4Offset, 0);
        iVar5 = iVar5 + -1;
        puVar4Offset = puVar4Offset + 1 * 2;
      } while (0 < iVar5);
      uVar1 = 0;
    }
  }
  else {
    uVar1 = -1;
  }
  //80027868
  return uVar1;
}

bool CMaddenAudioDecompression::SaveWav(unsigned char* wavData, int len, int waveType, int samplingRate, CString outputFilename)
{
	int processedWavDataLength = len / 2;
	unsigned char* processedWavData = new unsigned char[processedWavDataLength];
	for (int x = 0; x < len; x += 4)
	{
		int value = (int)CSharedFunctions::CharArrayToLong(&wavData[x]);
		short adjust = (short)(value);

		// Little endian
		processedWavData[(x / 2) + 1] = ((adjust >> 8) & 0xFF);
		processedWavData[(x / 2) + 0] = adjust & 0xFF;
	}

	bool result = SaveProcessedWav(processedWavData, processedWavDataLength, waveType, samplingRate, outputFilename);
	delete [] processedWavData;
	return result;
}

bool CMaddenAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
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

bool CMaddenAudioDecompression::DecompressSoundMadden64(int soundType, unsigned char* ROM, int offset, int compressedSize, int decompressedSize, CString outputFilename, float samplingRateFloat)
{
	std::vector<int> valueLookup;
	valueLookup.push_back(0x00000000);
	valueLookup.push_back(0x00000001);
	valueLookup.push_back(0x00000002);
	valueLookup.push_back(0x00000003);
	valueLookup.push_back(0x00000004);
	valueLookup.push_back(0x00000005);
	valueLookup.push_back(0x00000006);
	valueLookup.push_back(0x00000007);
	valueLookup.push_back(0xFFFFFFF8);
	valueLookup.push_back(0xFFFFFFF9);
	valueLookup.push_back(0xFFFFFFFA);
	valueLookup.push_back(0xFFFFFFFB);
	valueLookup.push_back(0xFFFFFFFC);
	valueLookup.push_back(0xFFFFFFFD);
	valueLookup.push_back(0xFFFFFFFE);
	valueLookup.push_back(0xFFFFFFFF);

	if (soundType == MADDENSTANDARDAUDIO)
	{
		unsigned char* outputData = new unsigned char[0x100000];
		int outputSize = 0;

		unsigned long previousValue = 0;
		unsigned long previousValue2 = 0;

		unsigned long soundDataOffset = offset;

		while (outputSize < (decompressedSize * 4))
		{
			unsigned char scale = ((ROM[soundDataOffset] >> 4) & 0xF);
			unsigned char type = ((ROM[soundDataOffset]) & 0xF) >> 2;
			if (type > 3)
				break;
			soundDataOffset++;

			int count = 0;
			while (count < 0x10)
			{
				unsigned long value = (((ROM[soundDataOffset] >> 4)) & 0xF);
				value = valueLookup[value];
				value = (int)value << scale;
				if (type == 0)
				{
				}
				else if (type == 1)
				{			
					unsigned long valueAdjust = (int)previousValue2 * 0xF0;
					value = (int)value + ((int)valueAdjust >> 8);
				}
				else if (type == 2)
				{
					unsigned long valueAdjust2 = (int)previousValue2 * 0x1e8;
					unsigned long valueAdjust = (int)previousValue * 0xf0;
					value = (int)value + (((int)valueAdjust2 - (int)valueAdjust) >> 8);
				}
				else if (type == 3)
				{
					unsigned long valueAdjust2 = (int)previousValue2 * 0x1cc;
					unsigned long valueAdjust = (int)previousValue * 0xd0;
					value = (int)value + (((int)valueAdjust2 - (int)valueAdjust) >> 8);
				}
				else
					throw;

				previousValue = value;
				count++;

				

				int valueresult = (((int)value * 0xF) >> 5);
				if ((short)valueresult < (short)-0x8000) {
					valueresult = (short)-0x8000;
				  }
				  else if ((short)0x7fff < (short)valueresult) {
					valueresult = (short)0x7fff;
				  }
				CSharedFunctions::WriteLongToBuffer(outputData, outputSize, valueresult);
				outputSize += 4;

				unsigned long value2 = ((ROM[soundDataOffset]) & 0xF);
				value2 = valueLookup[value2];
				value2 = (int)value2 << scale;
				if (type == 0)
				{

				}
				else if (type == 1)
				{
					unsigned long valueAdjust = (int)previousValue * 0xF0;
					value2 = (int)value2 + ((int)valueAdjust >> 8);
				}
				else if (type == 2)
				{
					unsigned long valueAdjust2 = (int)previousValue * 0x1e8;
					unsigned long valueAdjust = (int)previousValue2 * 0xf0;
					value2 = (int)value2 + (((int)valueAdjust2 - (int)valueAdjust) >> 8);
				}
				else if (type == 3)
				{
					unsigned long valueAdjust2 = (int)previousValue * 0x1cc;
					unsigned long valueAdjust = (int)previousValue2 * 0xd0;
					value2 = (int)value2 + (((int)valueAdjust2 - (int)valueAdjust) >> 8);
				}
				else
					throw;

				int value2result = (((int)value2 * 0xF) >> 5);
				if ((short)value2result < (short)-0x8000) {
					value2result = (short)-0x8000;
				  }
				  else if ((short)0x7fff < (short)value2result) {
					value2result = (short)0x7fff;
				  }

				CSharedFunctions::WriteLongToBuffer(outputData, outputSize, value2result);
				outputSize += 4;
				previousValue2 = value2;
				count++;

				soundDataOffset++;
			}

			/*FILE* out = fopen("C:\\temp\\out.bin", "wb");
			fwrite(outputData, 1, outputSize, out);
			fflush(out);
			fclose(out);*/
		}

		/*FILE* outWav = fopen("C:\\temp\\outwav.bin", "wb");
		for (int x = 0; x < outputSize; x+=4)
		{
			int value = (int)CSharedFunctions::CharArrayToLong(&outputData[x + 4]);
			short adjust = (short)(value);
			fwrite(&adjust, 1, 2, outWav);
		}
		
		fflush(outWav);
		fclose(outWav);*/

		SaveWav(outputData, outputSize, 1, samplingRateFloat, outputFilename);

		delete [] outputData;
	}
}

bool CMaddenAudioDecompression::DecompressSound(int soundType, unsigned char* ROM, int offset, int compressedSize, int decompressedSize, CString outputFilename, float samplingRateFloat)
{
	if (soundType == MADDENSTANDARDAUDIO)
	{
		unsigned char* outputData = new unsigned char[0x100000];
		int outputSize = 0;

		unsigned long previousValue = 0;
		unsigned long previousValue2 = 0;

		unsigned long soundDataOffset = offset;

		while (outputSize < (decompressedSize * 4))
		{
			unsigned char scale = (ROM[soundDataOffset] & 0xF) + 8;
			unsigned char type = ((ROM[soundDataOffset] >> 4) & 0xF);
			if (type > 3)
				break;
			soundDataOffset++;

			int count = 0;
			while (count < 0x1C)
			{
				unsigned long value = ((ROM[soundDataOffset] >> 4));
				value = (value << 0x1C);

				value = (int)value >> scale;
				if (type == 0)
				{
				}
				else if (type == 1)
				{			
					unsigned long valueAdjust = (int)previousValue2 * 0xF0;
					value = (int)value + (int)valueAdjust;
				}
				else if (type == 2)
				{
					unsigned long valueAdjust2 = (int)previousValue2 * 0x1cc;
					unsigned long valueAdjust = (int)previousValue * 0xd0;
					value = (int)value + ((int)valueAdjust2 - (int)valueAdjust);
				}
				else if (type == 3)
				{
					unsigned long valueAdjust2 = (int)previousValue2 * 0x188;
					unsigned long valueAdjust = (int)previousValue * 0xdc;
					value = (int)value + ((int)valueAdjust2 - (int)valueAdjust);
				}
				else
					throw;

				value = (int)value >> 8;
				if ((short)value < (short)-0x8000) {
					value = (short)-0x8000;
				  }
				  else if ((short)0x7fff < (short)value) {
					value = (short)0x7fff;
				  }
				previousValue = value;
				count++;

				CSharedFunctions::WriteLongToBuffer(outputData, outputSize, value);
				outputSize += 4;

				unsigned long value2 = ((ROM[soundDataOffset]));
				value2 = (value2 << 0x1C);

				value2 = (int)value2 >> scale;
				if (type == 0)
				{

				}
				else if (type == 1)
				{
					unsigned long valueAdjust = (int)previousValue * 0xF0;
					value2 = (int)value2 + (int)valueAdjust;
				}
				else if (type == 2)
				{
					unsigned long valueAdjust2 = (int)previousValue * 0x1cc;
					unsigned long valueAdjust = (int)previousValue2 * 0xd0;
					value2 = (int)value2 + ((int)valueAdjust2 - (int)valueAdjust);
				}
				else if (type == 3)
				{
					unsigned long valueAdjust2 = (int)previousValue * 0x188;
					unsigned long valueAdjust = (int)previousValue2 * 0xdc;
					value2 = (int)value2 + ((int)valueAdjust2 - (int)valueAdjust);
				}
				else
					throw;

				value2 = (int)value2 >> 8;

				if ((short)value2 < (short)-0x8000) {
					value2 = (short)-0x8000;
				  }
				  else if ((short)0x7fff < (short)value2) {
					value2 = (short)0x7fff;
				}
				CSharedFunctions::WriteLongToBuffer(outputData, outputSize, value2);
				outputSize += 4;
				previousValue2 = value2;
				count++;

				soundDataOffset++;
			}
		}

		/*FILE* out = fopen("C:\\temp\\out.bin", "wb");
		fwrite(outputData, 1, outputSize, out);
		fflush(out);
		fclose(out);

		FILE* outWav = fopen("C:\\temp\\outwav.bin", "wb");
		for (int x = 0; x < outputSize; x+=4)
		{
			int value = (int)CSharedFunctions::CharArrayToLong(&outputData[x + 4]);
			short adjust = (short)(value);
			fwrite(&adjust, 1, 2, outWav);
		}
		
		fflush(outWav);
		fclose(outWav);*/

		SaveWav(outputData, outputSize, 1, samplingRateFloat, outputFilename);

		delete [] outputData;
	}
	else if (soundType == MADDENFLOATAUDIO)
	{
		int sizeRAM = 0x400000;
		unsigned char* RAM = new unsigned char[sizeRAM];
		for (int x = 0; x < sizeRAM; x++)
			RAM[x] = 0x00;

		CSharedFunctions::WriteFloatToBuffer(RAM, 0x00000A60, -2.0f);
		CSharedFunctions::WriteFloatToBuffer(RAM, 0x00000A64, 2.0f);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000A68, 0.59738597000000004);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000A70, -0.11459156);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000A78, 0.018032679999999999);
		CSharedFunctions::WriteFloatToBuffer(RAM, 0x00000A80, 1.0f);
		CSharedFunctions::WriteFloatToBuffer(RAM, 0x00000A84, 1.25829e+07f);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000A88, 4294967296.0);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000A90, 8);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000A98, 4294967296.0); // may not be used
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000AA0, 0.001);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000AA8, 1.04);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000AB0, 0.25);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000AB8, 0.25);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000AC0, 4294967296.0); // may not be used
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000AC8, 0.066666666666666666);
		CSharedFunctions::WriteDoubleToBuffer(RAM, 0x00000AD0, 0.5);

		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A370, 0x00000000);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A374, 0x00000001);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A378, 0x00000003);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A37C, 0x00000007);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A380, 0x0000000F);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A384, 0x0000001F);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A388, 0x0000003F);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A38C, 0x0000007F);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x0004A390, 0x000000FF);

		CSharedFunctions::StringToByteArray(&RAM[0x0004A394], "00000000BF7F2CB6BF7D8612BF7BDF7FBF7A38EBBF789247BF76EBB3BF75450FBF739E7CBF6E45E5BF67AB75BF611117BF5A76A7BF53DC38BF4D41D9BF46A769BF400CFABF39729BBF32D82CBF2BAEBCBF1E79EEBF11450FBF041041BEEDB6E5BED34D27BEB8E38ABE9E79EEBE841052BE534D27BE1E79EEBDD34D6ABD534CE4000000003D534CE43DD34D6A3E1E79EE3E534D273E8410523E9E79EE3EB8E38A3ED34D273EEDB6E53F0410413F11450F3F1E79EE3F2BAEBC3F32D82C3F39729B3F400CFA3F46A7693F4D41D93F53DC383F5A76A73F6111173F67AB753F6E45E53F739E7C3F75450F3F76EBB33F7892473F7A38EB3F7BDF7F3F7D86123F7F2CB6040605090406050D0406050A04060511040605090406050E0406050A04060515040605090406050D0406050A04060512040605090406050E0406050A04060519040605090406050D0406050A04060511040605090406050E0406050A04060516040605090406050D0406050A04060512040605090406050E0406050A04060500040605090406050D0406050A04060511040605090406050E0406050A04060515040605090406050D0406050A04060512040605090406050E0406050A0406051A040605090406050D0406050A04060511040605090406050E0406050A04060516040605090406050D0406050A04060512040605090406050E0406050A04060502040B070F040C0813040B0710040C0817040B070F040C0814040B0710040C081B040B070F040C0813040B0710040C0818040B070F040C0814040B0710040C0801040B070F040C0813040B0710040C0817040B070F040C0814040B0710040C081C040B070F040C0813040B0710040C0818040B070F040C0814040B0710040C0803040B070F040C0813040B0710040C0817040B070F040C0814040B0710040C081B040B070F040C0813040B0710040C0818040B070F040C0814040B0710040C0801040B070F040C0813040B0710040C0817040B070F040C0814040B0710040C081C040B070F040C0813040B0710040C0818040B070F040C0814040B0710040C08030000000100000008000000000000000100000007000000000000000000000008000000000000000000000007000000000000000000000002000000000000000000000002BF80000000000000000000023F8000000000000000000003BF80000000000000000000033F8000000000000100000004C00000000000000100000004400000000000000100000003C00000000000000100000003400000000000000100000005C04000000000000100000005404000000000000100000004C04000000000000100000004404000000000000100000006C08000000000000100000006408000000000000100000005C08000000000000100000005408000000000000100000007C0A00000000000010000000740A000000000000100000006C0A00000000000010000000640A000000000000100000008C0C00000000000010000000840C000000000000100000007C0C00000000000010000000740C00000");

		//F1382CA53AD8D893 first data
		// BnkB copied (from data 5936F0 to 680AF8/data at 680B38)
		//unsigned long inputSpot = 0x80343670;
		unsigned long inputSpot = 0x803D0000; // Moved to give more space
		unsigned long outputFloatsSpot = 0x80344270;

		memcpy(&RAM[inputSpot & 0xFFFFFF], &ROM[offset], compressedSize);

		/*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
		fwrite(RAM, 1, 0x400000, a);
		fflush(a);
		fclose(a);*/

		//800278A4
		FUN_00028800(RAM, inputSpot, outputFloatsSpot, decompressedSize);
		//800278AC
		CSharedFunctions::WriteLongToBuffer(RAM, outputFloatsSpot + 0xD44, decompressedSize);
		CSharedFunctions::WriteLongToBuffer(RAM, outputFloatsSpot + 0xD48, 0);
		CSharedFunctions::WriteLongToBuffer(RAM, outputFloatsSpot + 0xD4C, 0);


		//8002772c
		//FUN_00028950(RAM, outputFloatsSpot, 1);

		// Not sure where this came from, output spot
		CSharedFunctions::WriteLongToBuffer(RAM, 0x80052C88, 0x80344070);

		unsigned char* outputBuffer = new unsigned char[0x100000];
		int outputBufferSize = 0;
		while (FUN_0002772c(RAM, outputFloatsSpot, 0x00000100, 0x80052C88, outputBuffer, outputBufferSize) != -1);

		/*FILE* out = fopen("C:\\temp\\raw.bin", "wb");
		fwrite(outputBuffer, 1, outputBufferSize, out);
		fclose(out);*/

		SaveProcessedWav(outputBuffer, outputBufferSize, 1, samplingRateFloat, outputFilename);

		delete [] outputBuffer;
		delete [] RAM;
	}
	else
	{
		throw;
	}
}