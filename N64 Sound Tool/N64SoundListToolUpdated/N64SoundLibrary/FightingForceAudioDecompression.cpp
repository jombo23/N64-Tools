#include "StdAfx.h"
#include "FightingForceAudioDecompression.h"

CFightingForceAudioDecompression::CFightingForceAudioDecompression(void)
{
}

CFightingForceAudioDecompression::~CFightingForceAudioDecompression(void)
{
}

char CFightingForceAudioDecompression::table8008251C[8] = { 0xFE, 0xFE, 0xFE, 0xFE, 0x04, 0x08, 0x0C, 0x10 };

short CFightingForceAudioDecompression::table80082524[0x100] = { 
	0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000D, 0x000E,
	0x0010, 0x0011, 0x0013, 0x0015, 0x0017, 0x0019, 0x001C, 0x001F,
	0x0022, 0x0025, 0x0029, 0x002D, 0x0032, 0x0037, 0x003C, 0x0042,
	0x0049, 0x0050, 0x0058, 0x0061, 0x006B, 0x0076, 0x0082, 0x008F,
	0x009D, 0x00AD, 0x00BE, 0x00D1, 0x00E6, 0x00FD, 0x0117, 0x0133,
	0x0151, 0x0173, 0x0198, 0x01C1, 0x01EE, 0x0220, 0x0256, 0x0292,
	0x02D4, 0x031C, 0x036C, 0x03C3, 0x0424, 0x048E, 0x0502, 0x0583,
	0x0610, 0x06AB, 0x0756, 0x0812, 0x08E0, 0x09C3, 0x0ABD, 0x0BD0,
	0x0CFF, 0x0E4C, 0x0FBA, 0x114C, 0x1307, 0x14EE, 0x1706, 0x1954,
	0x1BDC, 0x1EA5, 0x21B6, 0x2515, 0x28CA, 0x2CDF, 0x315B, 0x364B,
	0x3BB9, 0x41B2, 0x4844, 0x4F7E, 0x5771, 0x602F, 0x69CE, 0x7462,
	0x7FFF, 0x0000, 0x800F, 0x4228, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x27BD, 0xFFD0,
	0xAFB2, 0x0018, 0x0080, 0x9021, 0xAFB3, 0x001C, 0x00A0, 0x9821,
	0xAFB5, 0x0024, 0x2415, 0xFFFF, 0xAFBF, 0x0028, 0xAFB4, 0x0020,
	0xAFB1, 0x0014, 0xAFB0, 0x0010, 0x1275, 0x006B, 0x0000, 0x1021,
	0x0012, 0x1140, 0x0052, 0x1021, 0x0002, 0x10C0, 0x3C03, 0x800D,
	0x2463, 0x8750, 0x0043, 0x8821, 0x0013, 0x1140, 0x0053, 0x1021,
	0x0002, 0x10C0, 0x0043, 0x8021, 0x3C03, 0x800C, 0x2463, 0xFC68,
	0x8624, 0x0034, 0x8625, 0x003C, 0x8606, 0x0034, 0x8622, 0x0040,
	0x8607, 0x003C, 0x0002, 0x1080, 0x0043, 0x1021, 0x8C54, 0x0000,
	0x0C01, 0x7020, 0x0000, 0x0000, 0x9623, 0x0044, 0x0003, 0x1940,
	0x0043, 0x1023, 0x2442, 0x1000, 0x3042, 0xFFFF, 0x2C42, 0x2001,
	0x1040, 0x004D, 0x0000, 0x1021, 0x8604, 0x0034, 0x8605, 0x003C,
	0x8626, 0x0034, 0x8627, 0x003C, 0x0C01, 0x7020, 0x0000, 0x0000,
	0x9603, 0x0044, 0x0003, 0x1940, 0x0043, 0x1023, 0x2442, 0x1000,
	0x3042, 0xFFFF, 0x2C42, 0x2001, 0x1440, 0x0003, 0x3C02, 0x800D,
	0x0802, 0x09F3, 0x0000, 0x1021, 0x8C42, 0x3580, 0x2842, 0x0101,
	0x1040, 0x0039, 0x0000, 0x1021, 0x8602, 0x004E, 0x1040, 0x0036,
	0x0000, 0x1021, 0x8202, 0x0085, 0x1455, 0x0033, 0x0000, 0x1021,
	0x0C02, 0x1B9A, 0x0200, 0x2021, 0x0040, 0x1821, 0x2402, 0x008D,
	0x1062, 0x0009, 0x2402, 0x009A, 0x1062, 0x0007, 0x2402, 0x00AF,
};

short CFightingForceAudioDecompression::table800824DC[0x20] = {
	0xFFF7, 0x0020, 0x002C, 0x0021, 0x0068, 0x0010, 0x0010, 0x0058, 
	0x0084, 0x00E8, 0x00E8, 0x0102, 0x00EA, 0x00BE, 0x005A, 0x003D,
	0x0023, 0x0023, 0x000D, 0xFFF9, 0xFFF9, 0x003B, 0x004F, 0x004F,
	0x002E, 0x0079, 0x004D, 0x0075, 0x0051, 0x0020, 0x0020, 0xFFE9
};

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool CFightingForceAudioDecompression::FUN_00081a70(unsigned char* RAM, int param_1, unsigned long& param_2, unsigned long& param_3, int param_4, int in_stack_00000010, int dataLength)
{
  bool bVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  unsigned long psVar5Offset; // short
  unsigned long uVar6;
  int in_v0_lo;
  int in_v1_lo;
  unsigned long piVar7Offset; // int
  int iVar8;
  unsigned long uVar9;
  unsigned long uVar10;
  unsigned long uVar11;
  unsigned long psVar12Offset; // short
  int iVar13;
  int iVar14;
  unsigned long uVar15;
  unsigned long puVar16Offset; // short
  unsigned long puVar17Offset; // short
  
  iVar4 = CSharedFunctions::CharArrayToLong(RAM, 0x800f59e8);
  iVar3 = CSharedFunctions::CharArrayToLong(RAM, 0x800f59e4);
  uVar10 = param_3 >> 0xc & 0xfffffff0;
  if (uVar10 == 0xffffffff) goto LAB_00082204;
LAB_00081af4:
  do {
    psVar12Offset = (((int)uVar10 >> 2) + uVar10 + param_1);
	if (psVar12Offset >= (param_1 + dataLength))
		return false;

	iVar14 = (int)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset);
    puVar16Offset = 0x800824dc;
	uVar15 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 1 * 2) << 1 & 0xff;
    if ((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 1 * 2) << 1 < 0) {
      uVar9 = (unsigned long)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 2 * 2);
      iVar13 = 4;
      do {
        psVar5Offset = (0x80082524 + uVar15);
		uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
		iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar17Offset = puVar16Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned long)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 3 * 2);
      iVar13 = 4;
      do {
        psVar5Offset = (0x80082524 + uVar15);
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
        iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar16Offset = puVar17Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar16Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned long)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 4 * 2);
      iVar13 = 4;
      do {
        psVar5Offset = (0x80082524 + uVar15);
		uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
        iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar17Offset = puVar16Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
	  uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 5 * 2);
      iVar13 = 4;
      do {
        psVar5Offset = (0x80082524 + uVar15);
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
		iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar16Offset = puVar17Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar16Offset;
      } while (0 < iVar13);
	  uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 6 * 2);
      iVar13 = 4;
      do {
        CSharedFunctions::WriteShortToBuffer(RAM, psVar5Offset, CSharedFunctions::CharArrayToLong(RAM, 0x80082524) + uVar15);
		uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
        iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar17Offset = puVar16Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned long)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 7 * 2);
      iVar13 = 4;
      do {
        psVar5Offset = (0x80082524 + uVar15);
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
        iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar16Offset = puVar17Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar16Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 8 * 2);
      iVar13 = 4;
      do {
        CSharedFunctions::WriteShortToBuffer(RAM, psVar5Offset, (CSharedFunctions::CharArrayToLong(RAM, 0x80082524) + uVar15));
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
        iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar17Offset = puVar16Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned long)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 9 * 2);
      iVar13 = 4;
      do {
        psVar5Offset = (0x80082524 + uVar15);
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, (uVar9 & 7) + 0x8008251c);
        iVar8 = uVar15 - 0xb0;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        iVar13 = iVar13 + -1;
        if (0 < iVar8) {
          uVar15 = 0xb0;
        }
        iVar8 = (int)((int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar5Offset) * (uVar9 & 7)) >> 2;
        if ((uVar9 & 8) != 0) {
          iVar8 = -iVar8;
        }
        uVar9 = uVar9 >> 4;
        iVar14 = iVar14 + iVar8;
        iVar8 = iVar14 + 0x7fff;
        if (-1 < iVar14 + -0x8000) {
          iVar14 = 0x7fff;
        }
        puVar16Offset = puVar17Offset + 1 * 2;
        if (iVar8 < 1) {
          iVar14 = -0x8000;
        }
        CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar16Offset;
      } while (0 < iVar13);
      if (iVar13 != 0) goto LAB_00081f04;
    }
    else {
LAB_00081f04:
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 2 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        puVar17Offset = puVar16Offset + 1 * 2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 3 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        puVar16Offset = puVar17Offset + 1 * 2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar16Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 4 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        puVar17Offset = puVar16Offset + 1 * 2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 5 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        puVar16Offset = puVar17Offset + 1 * 2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar16Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 6 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        puVar17Offset = puVar16Offset + 1 * 2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 7 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        puVar16Offset = puVar17Offset + 1 * 2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar16Offset;
      } while (0 < iVar13);
	  uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 8 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        puVar17Offset = puVar16Offset + 1 * 2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
        CSharedFunctions::WriteShortToBuffer(RAM, puVar16Offset, (short)iVar14);
        puVar16Offset = puVar17Offset;
      } while (0 < iVar13);
      uVar9 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar12Offset + 9 * 2);
      iVar13 = 4;
      sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
      do {
        uVar6 = uVar9 & 7;
        iVar8 = (int)sVar2;
        uVar15 = uVar15 + (int)(char)CSharedFunctions::CharArrayToChar(RAM, uVar6 + 0x8008251c);
        uVar11 = uVar9 & 8;
        uVar9 = uVar9 >> 4;
        iVar13 = iVar13 + -1;
        if ((int)uVar15 < 0) {
          uVar15 = 0;
        }
        sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x80082524 + uVar15);
        iVar8 = (int)(iVar8 * uVar6) >> 2;
        if (uVar11 != 0) {
          iVar8 = -iVar8;
        }
        iVar14 = iVar14 + iVar8;
		CSharedFunctions::WriteShortToBuffer(RAM, puVar17Offset, (short)iVar14);
        puVar17Offset = puVar17Offset + 1 * 2;
      } while (0 < iVar13);
    }
LAB_00082204:
    iVar14 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, 0x800824dc + ((int)param_3 >> 10 & 0x3e)) << 0x10;
    uVar15 = uVar10;
    piVar7Offset = param_2;
    if (iVar3 == iVar4) {
      if (CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0) < 1) {
        do {
          iVar13 = (int)((unsigned __int64)((__int64)iVar14 * (__int64)iVar3) >> 0x20);
          param_3 = param_3 + param_4;
          in_stack_00000010 = in_stack_00000010 + -1;
          uVar10 = (int)param_3 >> 0xc & 0xfffffff0;
          param_2 = piVar7Offset + 2 * 4;
          iVar14 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, 0x800824dc + ((int)param_3 >> 10 & 0x3e)) << 0x10;
          CSharedFunctions::WriteLongToBuffer(RAM, piVar7Offset, (int)CSharedFunctions::CharArrayToLong(RAM, piVar7Offset) + iVar13);
           CSharedFunctions::WriteLongToBuffer(RAM, piVar7Offset + 1 * 4, (int)CSharedFunctions::CharArrayToLong(RAM, piVar7Offset + 1 * 4) + iVar13);
          iVar8 = iVar13;
          if (in_stack_00000010 < 1) {
LAB_00082414:
			  /*FILE* outFileRAM = fopen("C:\\temp\\ram.bin", "wb");
			fwrite(RAM, 1, 0x400000, outFileRAM);
			fflush(outFileRAM);
			fclose(outFileRAM);*/

            CSharedFunctions::WriteLongToBuffer(RAM, 0x800f59ec, iVar8);
            CSharedFunctions::WriteLongToBuffer(RAM, 0x800f53dc, iVar13);
            return true; //return (((__int64)in_v0_lo << 8) | (__int64)in_v1_lo); // Just returns the V0/V1 passed in that was thrown on stack
          }
          bVar1 = uVar15 == uVar10;
          uVar15 = uVar10;
          piVar7Offset = param_2;
        } while (bVar1);
        goto LAB_00081af4;
      }
    }
	else if (CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0) < 1) goto LAB_00082224;
    do {
      param_3 = param_3 + param_4;
      in_stack_00000010 = in_stack_00000010 + -1;
      param_2 = piVar7Offset + 2 * 4;
      uVar10 = (int)param_3 >> 0xc & 0xfffffff0;
      CSharedFunctions::WriteLongToBuffer(RAM, 0x800f53dc, (int)(CSharedFunctions::CharArrayToLong(RAM, 0x800f53dc) * CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0) +
           (int)((unsigned __int64)((__int64)iVar14 * (__int64)iVar3) >> 0x20) * (0x40 - CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0)))
           >> 6);
      CSharedFunctions::WriteLongToBuffer(RAM, piVar7Offset, (int)CSharedFunctions::CharArrayToLong(RAM, piVar7Offset) + CSharedFunctions::CharArrayToLong(RAM, 0x800f53dc));
      CSharedFunctions::WriteLongToBuffer(RAM, 0x800f59ec, (int)(CSharedFunctions::CharArrayToLong(RAM, 0x800f59ec) * CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0) +
           (int)((unsigned __int64)((__int64)iVar14 * (__int64)iVar4) >> 0x20) * (0x40 - CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0)))
           >> 6);
      iVar14 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, 0x800824dc + ((int)param_3 >> 10 & 0x3e)) << 0x10;
      CSharedFunctions::WriteLongToBuffer(RAM, 0x800f59f0, CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0) + -1);
      CSharedFunctions::WriteLongToBuffer(RAM, piVar7Offset + 1 * 4, CSharedFunctions::CharArrayToLong(RAM, piVar7Offset + 1 * 4) + CSharedFunctions::CharArrayToLong(RAM, 0x800f59ec));
      iVar13 = CSharedFunctions::CharArrayToLong(RAM, 0x800f53dc);
      iVar8 = CSharedFunctions::CharArrayToLong(RAM, 0x800f59ec);
      if (in_stack_00000010 < 1) goto LAB_00082414;
      if (CSharedFunctions::CharArrayToLong(RAM, 0x800f59f0) == 0) goto LAB_00082278;
      bVar1 = uVar15 == uVar10;
      uVar15 = uVar10;
      piVar7Offset = param_2;
    } while (bVar1);
  } while( true );
LAB_00082278:
	/*FILE* outFileRAM = fopen("C:\\temp\\ram.bin", "wb");
			fwrite(RAM, 1, 0x400000, outFileRAM);
			fflush(outFileRAM);
			fclose(outFileRAM);*/
  if (uVar15 != uVar10) goto LAB_00081af4;
LAB_00082224:
  iVar13 = (int)((unsigned __int64)((__int64)iVar14 * (__int64)iVar3) >> 0x20);
  param_3 = param_3 + param_4;
  in_stack_00000010 = in_stack_00000010 + -1;
  CSharedFunctions::WriteLongToBuffer(RAM, param_2, (int)CSharedFunctions::CharArrayToLong(RAM, param_2) + iVar13);
  iVar8 = (int)((unsigned __int64)((__int64)iVar14 * (__int64)iVar4) >> 0x20);
  iVar14 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, 0x800824dc + ((int)param_3 >> 10 & 0x3e)) << 0x10;
  CSharedFunctions::WriteLongToBuffer(RAM, param_2 + 1 * 4, CSharedFunctions::CharArrayToLong(RAM, param_2 + 1 * 4) + iVar8);
  uVar15 = uVar10;
  uVar10 = (int)param_3 >> 0xc & 0xfffffff0;
  param_2 = param_2 + 2 * 4;
  if (in_stack_00000010 < 1) goto LAB_00082414;
  goto LAB_00082278;
}

void CFightingForceAudioDecompression::DecodeWav(unsigned char* wavData, int wavDataLength, CString outputFilename, float samplingRateFloat)
{
	int sizeRAM = 0x800000;
	unsigned char* RAM = new unsigned char[sizeRAM];
	for (int x = 0; x < (sizeRAM & 0xFFFFFF); x++)
		RAM[x] = 0x00;

	for (int x = 0; x < 8; x++)
	{
		CSharedFunctions::WriteCharToBuffer(RAM, (0x8008251C & 0xFFFFFF) + x, table8008251C[x]);
	}

	for (int x = 0; x < 0x20; x++)
	{
		CSharedFunctions::WriteShortToBuffer(RAM, (0x800824DC & 0xFFFFFF) + (x * 2), table800824DC[x]);
	}

	for (int x = 0; x < 0x100; x++)
	{
		CSharedFunctions::WriteShortToBuffer(RAM, (0x80082524 & 0xFFFFFF) + (x * 2), table80082524[x]);
	}

	unsigned long A0 = 0x80313CB0; // Compressed data, looks like 0x14 sized compress chunks
	unsigned long A1 = 0x800F59F8; // Temporary output buffer
	unsigned long A2 = 0x00000000; // Current spot (with weird shifting)
	unsigned long A3 = 0x000004F8; // Sampling rate
	unsigned long T0 = 0x100; // Chunk to do

	memcpy(&RAM[A0 & 0xFFFFFF], wavData, wavDataLength);

	int sizeOutput = 0;

	unsigned char* output = new unsigned char[0x1000000];

	CFightingForceAudioDecompression fightingForceAudioDecompression;

	for (int y = 0; y < 1000; y++)
	{
		for (int x = 0xF59F8; x < 0xF59F8 + 0x800; x++)
			RAM[x] = 0x00;

		CSharedFunctions::WriteLongToBuffer(RAM, 0x000F59E4, 0x00000000);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x000F59E8, 0x00085555);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x000F59EC, 0x00000000);
		CSharedFunctions::WriteLongToBuffer(RAM, 0x000F53DC, 0x00000000);

		if (y > 0)
		{
			CSharedFunctions::WriteLongToBuffer(RAM, 0x000F59F0, 0x00000000);
		}
		else
		{
			CSharedFunctions::WriteLongToBuffer(RAM, 0x000F59F0, 0x0000003F);
		}

		unsigned long param_2 = A1;
		bool result = fightingForceAudioDecompression.FUN_00081a70(RAM, A0, param_2, A2, A3, T0, wavDataLength);

		for (int x = 0; x< (param_2 - A1); x+=8)
		{
			int value = (int)CSharedFunctions::CharArrayToLong(&RAM[(A1 + x + 4) & 0xFFFFFF]) >> 3;
			short adjust = (short)value;
			output[sizeOutput] = ((adjust) & 0xFF);
			output[sizeOutput + 1] = ((adjust >> 8) & 0xFF);
			sizeOutput += 2;
		}

		if (!result)
			break;
	}

	fightingForceAudioDecompression.SaveProcessedWav(output, sizeOutput, 0, samplingRateFloat, outputFilename);

	delete [] RAM;
	delete [] output;
}

bool CFightingForceAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
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