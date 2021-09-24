#include "StdAfx.h"
#include "SK_ASCCompression.h"

CSK_ASCCompression::CSK_ASCCompression(void)
{
}

CSK_ASCCompression::~CSK_ASCCompression(void)
{
}

unsigned long GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == NULL)
		return 0;

	fseek(inFile, 0, SEEK_END);
	unsigned long fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	address = address & 0xFFFFFFF;

	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	address = address & 0xFFFFFFF;

	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

void WriteCharToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	address = address & 0xFFFFFFF;

	Buffer[address] = ((data) & 0xFF);	
}


unsigned short CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

unsigned long CharArrayToByte(unsigned char* currentSpot)
{
	return currentSpot[0];
}

unsigned short CharArrayToShort(unsigned char* Buffer, int address)
{
	address = address & 0xFFFFFFF;

	return ((Buffer[address + 0] << 8) | Buffer[address + 1]);
}

unsigned long CharArrayToLong(unsigned char* Buffer, int address)
{
	address = address & 0xFFFFFFF;

	return ((((((Buffer[address + 0] << 8) | Buffer[address + 1]) << 8) | Buffer[address + 2]) << 8) | Buffer[address + 3]);
}

unsigned long CharArrayToChar(unsigned char* Buffer, int address)
{
	address = address & 0xFFFFFFF;

	return Buffer[address];
}

// Grab next set of bytes (0x1000)
void FUN_80145edc(unsigned char* RAM, unsigned char* compressedFile, int param_1)

{
  // Pull 0x1000
  WriteLongToBuffer(RAM, param_1 + 0x2000, 0x1000);
  // If bytes left, less than 0x1000, grab what's left
  if ((int)CharArrayToLong(RAM, param_1 + 0x2010) < 0x1000) {
    WriteLongToBuffer(RAM, param_1 + 0x2000, (int)CharArrayToLong(RAM, param_1 + 0x2010));
  }
  if ((int)CharArrayToLong(RAM, param_1 + 0x2000) != 0) {
	unsigned long functionPointer = CharArrayToLong(RAM, (param_1 + 0x2ad0));


    if (functionPointer == 0x8015CE48)
	{
		int amount = CharArrayToLong(RAM, param_1 + 0x2000);
		int compressedOffset = 8 + CharArrayToLong(RAM, param_1 + 0x200C) - CharArrayToLong(RAM, param_1 + 0x2010);
		memcpy(&RAM[param_1 & 0xFFFFFFF], &compressedFile[compressedOffset], amount);

		//FUN_8015CE48(param_1, (int)CharArrayToLong(RAM, param_1 + 0x2000), CharArrayToLong(RAM, param_1 + 0x2acc));
	}
	else
	{
		throw "UNKNOWN Function Pointer 80145edc";
	}
    WriteLongToBuffer(RAM, param_1 + 0x2010, (int)CharArrayToLong(RAM, param_1 + 0x2010) - (int)CharArrayToLong(RAM, param_1 + 0x2000));
  }
  WriteLongToBuffer(RAM, param_1 + 0x2004, 0);

  //FILE* out = fopen("C:\\temp\\a.bin", "wb");
  //fwrite(RAM, 1, 0x1800000, out);
  //fflush(out); 
  //fclose(out);
  return;
}

void FUN_80145774(unsigned char* RAM, int param_1,int param_2,int param_3,short param_4,int param_5)
{
  int psVar1Offset;
  int psVar2Offset;
  short sVar3;
  unsigned long uVar4;
  int puVar5Offset;
  int psVar6Offset;
  int psVar7Offset;
  int iVar8;
  int iVar9;
  unsigned long uVar10;
  
  for (param_5 = param_5 + param_2; param_5 != 0; param_5 = param_5 >> 1) {
    WriteShortToBuffer(RAM, (param_1 + param_5 * 2), (short)CharArrayToShort(RAM, param_1 + param_5 * 2) + param_4);
  }
  if ((int)(unsigned long)(short)CharArrayToShort(RAM, param_1 + 2) < param_3) {
    return;
  }
  iVar8 = param_2 * 2 + -1;
  puVar5Offset = (param_1 + iVar8 * 2);
  iVar9 = param_2;
  if (param_2 <= iVar8) {
    do {
      if (1 < CharArrayToShort(RAM, puVar5Offset)) {
        WriteShortToBuffer(RAM, puVar5Offset, (unsigned short)((int)(unsigned long)CharArrayToShort(RAM, puVar5Offset) >> 1));
      }
      puVar5Offset = puVar5Offset + (-1 * 2);
      iVar9 = iVar9 + -1;
    } while (iVar9 != 0);
  }
  uVar4 = param_2 - 1;
  psVar6Offset = (param_1 + (param_2 * 2 + -2) * 2);
  psVar7Offset = (param_1 + uVar4 * 2);

  if (uVar4 != 0) {
    uVar10 = uVar4 >> 3;
    if (uVar10 != 0) {
      do {
		  WriteShortToBuffer(RAM, psVar7Offset, (short)((short)CharArrayToShort(RAM, psVar6Offset) + (short)CharArrayToShort(RAM, psVar6Offset + (1 * 2))));
		  WriteShortToBuffer(RAM, psVar7Offset - (1 * 2), (short)((short)CharArrayToShort(RAM, psVar6Offset - (2 * 2)) + (short)CharArrayToShort(RAM, psVar6Offset - (1 * 2))));
		  WriteShortToBuffer(RAM, psVar7Offset - (2 * 2), (short)((short)CharArrayToShort(RAM, psVar6Offset - (4 * 2)) + (short)CharArrayToShort(RAM, psVar6Offset - (3 * 2))));
		  WriteShortToBuffer(RAM, psVar7Offset - (3 * 2), (short)((short)CharArrayToShort(RAM, psVar6Offset - (6 * 2)) + (short)CharArrayToShort(RAM, psVar6Offset - (5 * 2))));
		  WriteShortToBuffer(RAM, psVar7Offset - (4 * 2), (short)((short)CharArrayToShort(RAM, psVar6Offset - (8 * 2)) + (short)CharArrayToShort(RAM, psVar6Offset - (7 * 2))));
		  WriteShortToBuffer(RAM, psVar7Offset - (5 * 2), (short)((short)CharArrayToShort(RAM, psVar6Offset - (10 * 2)) + (short)CharArrayToShort(RAM, psVar6Offset - (9 * 2))));
		  WriteShortToBuffer(RAM, psVar7Offset - (6 * 2), (short)((short)CharArrayToShort(RAM, psVar6Offset - (0xC * 2)) + (short)CharArrayToShort(RAM, psVar6Offset - (0xB * 2))));

        psVar1Offset = psVar6Offset + -0xe * 2;
        psVar2Offset = psVar6Offset + -0xd * 2;
        psVar6Offset = psVar6Offset + -0x10 * 2;

		WriteShortToBuffer(RAM, psVar7Offset - (7 * 2), (short)((short)CharArrayToShort(RAM, psVar1Offset) + (short)CharArrayToShort(RAM, psVar2Offset)));
        psVar7Offset = psVar7Offset + -8 * 2;
        uVar10 = uVar10 - 1;
      } while (uVar10 != 0);
      uVar4 = uVar4 & 7;
      if (uVar4 == 0) {
        return;
      }
    }
    do {
      sVar3 = (short)(CharArrayToShort(RAM, psVar6Offset));
      psVar1Offset = psVar6Offset + 1 * 2;
      psVar6Offset = psVar6Offset + -2 * 2;
      WriteShortToBuffer(RAM, psVar7Offset, (short)(sVar3 + CharArrayToShort(RAM, psVar1Offset)));
      psVar7Offset = psVar7Offset + -1 * 2;
      uVar4 = uVar4 - 1;
    } while (uVar4 != 0);
    return;
  }
  return;
}

/*unsigned _int64 CONCAT44(unsigned long a, unsigned long b)
{
	return ((_int64)a << 64) | (_int64)b;
}

unsigned _int64 FUN_8020c970(void)

{
  unsigned long in_MSR;
  
  return CONCAT44(in_MSR >> 0xf,in_MSR) & 0x1ffff7fff;
}


unsigned long FUN_8020d318(unsigned char* RAM, int param_1, int param_2Offset, unsigned long param_3)

{
  unsigned long uVar1;
  int iVar2;
  
  uVar1 = FUN_8020c970();
  while( true ) {
    if ((int)CharArrayToLong(RAM, param_1 + 0x1c) != 0) {
      if (param_2Offset != 0x0) {
        WriteLongToBuffer(RAM, param_2Offset, CharArrayToLong(RAM, (int)CharArrayToLong(RAM, param_1 + 0x10) + (int)CharArrayToLong(RAM, param_1 + 0x18) * 4));
      }
      iVar2 = (int)CharArrayToLong(RAM, param_1 + 0x18) + 1;
      WriteLongToBuffer(RAM, (param_1 + 0x18), (int)(iVar2 - (iVar2 / (int)CharArrayToLong(RAM, param_1 + 0x14)) * (int)CharArrayToLong(RAM, param_1 + 0x14)));
      WriteLongToBuffer(RAM, (param_1 + 0x1c), (int)((int)CharArrayToLong(RAM, param_1 + 0x1c) + -1));
      FUN_802100f0(param_1);
      FUN_8020c998(uVar1);
      return 1;
    }
    if ((param_3 & 1) == 0) break;
    FUN_80210004(param_1 + 8);
  }
  FUN_8020c998(uVar1);
  return 0;
}*/

unsigned long FUN_8015cffc(unsigned char* RAM, int param_1, unsigned long param_2, int param_3)
{
  unsigned long uVar1;
  unsigned long uVar2;
  int iVar3;
  int local_28 [5]; // TODO sort out
  
  iVar3 = 0;
  if ((int)CharArrayToLong(RAM, (param_3 + 0x2050)) == 1) {
    //iVar3 = FUN_8020d318(RAM, CharArrayToLong(RAM, param_3 + 0x2060), local_28, 0);
    if ((int)CharArrayToLong(RAM, (param_3 + 0x2078)) != 0) {
      //FUN_80158850((int)CharArrayToLong(RAM, param_3 + 0x2078));
      WriteLongToBuffer(RAM, (param_3 + 0x2078), 0);
    }
    if (iVar3 != 0) {
      if ((int)CharArrayToLong(RAM, local_28[0] + 4) == 2) {
        //FUN_80158850(local_28[0]);
        return 1;
      }
      if ((int)CharArrayToLong(RAM, (param_3 + 0x205c)) != 0) {
        //FUN_8020d250((int)CharArrayToLong(RAM, param_3 + 0x205c), local_28[0],1);
      }
    }
    uVar1 = 1;
  }
  else {
    do {
      if ((int)CharArrayToLong(RAM, (param_3 + 0x2058)) == 0) {
        //FUN_8020d318(RAM, CharArrayToLong(RAM, param_3 + 0x2060), local_28, 1);
        WriteLongToBuffer(RAM, (param_3 + 0x2078), (int)local_28[0]);
        if ((CharArrayToLong(RAM, (int)CharArrayToLong(RAM, param_3 + 0x2078) + 4) == 2) || ((int)CharArrayToLong(RAM, param_3 + 0x2050) == 1)) {
          WriteLongToBuffer(RAM, param_3 + 0x2050, 1);
          return 1;
        }
        WriteLongToBuffer(RAM, (param_3 + 0x2058), CharArrayToLong(RAM, (int)CharArrayToLong(RAM, param_3 + 0x2078) + 0xc));
        WriteLongToBuffer(RAM, (param_3 + 0x2054), CharArrayToLong(RAM, (int)CharArrayToLong(RAM, param_3 + 0x2078) + 0x10));
        WriteLongToBuffer(RAM, (param_3 + 0x205c), CharArrayToLong(RAM, (int)CharArrayToLong(RAM, param_3 + 0x2078) + 0x14));
      }
      uVar2 = param_2;
      if (CharArrayToLong(RAM, param_3 + 0x2058) < param_2) {
        uVar2 = CharArrayToLong(RAM, param_3 + 0x2058);
      }
      //FUN_80005278(CharArrayToLong(RAM, param_3 + 0x2054), param_1 + iVar3, uVar2);
      //FUN_8020b740(CharArrayToLong(RAM, param_3 + 0x2054), uVar2);
      if ((int)CharArrayToLong(RAM, (param_3 + 0x207c)) == 0) {
        WriteLongToBuffer(RAM, (param_3 + 0x207c), (int)((int)(CharArrayToLong(RAM, CharArrayToLong(RAM, param_3 + 0x2054))) + 4));
      }
      param_2 = param_2 - uVar2;
      iVar3 = iVar3 + uVar2;
      WriteLongToBuffer(RAM, (param_3 + 0x2054), (unsigned long)((int)CharArrayToLong(RAM, param_3 + 0x2054) + uVar2));
      WriteLongToBuffer(RAM, (param_3 + 0x2058), (unsigned long)((int)CharArrayToLong(RAM, param_3 + 0x2058) - uVar2));
      WriteLongToBuffer(RAM, (param_3 + 0x207c), (unsigned long)((int)CharArrayToLong(RAM, param_3 + 0x207c) - uVar2));
      if ((int)CharArrayToLong(RAM, (param_3 + 0x2058)) == 0) {
        //FUN_8020d250(CharArrayToLong(RAM, param_3 + 0x205c), CharArrayToLong(RAM, param_3 + 0x2078), 1);
        WriteLongToBuffer(RAM, (param_3 + 0x2078), 0);
      }
    } while (param_2 != 0);
    uVar1 = 0;
  }
  return uVar1;
}

void FUN_800051bc(unsigned char* RAM, int param_1, unsigned long param_2, unsigned long param_3)

{
  // Untested
  unsigned long uVar1;
  //unsigned long *puVar2;
  int puVar2Offset;
  //unsigned char *puVar3;
  int puVar3Offset;
  
  param_2 = param_2 & 0xff;
  puVar3Offset = (param_1 - 1);
  if (0x1f < param_3) {
    uVar1 = ~(unsigned long)puVar3Offset & 3;
    if (uVar1 != 0) {
      param_3 = param_3 - uVar1;
      do {
        uVar1 = uVar1 - 1;
        puVar3Offset = puVar3Offset + 1;
        WriteCharToBuffer(RAM, puVar3Offset, (char)param_2);
      } while (uVar1 != 0);
    }
    if (param_2 != 0) {
      param_2 = param_2 | param_2 << 8 | param_2 << 0x18 | param_2 << 0x10;
    }
    puVar2Offset = (puVar3Offset + -3);
    for (uVar1 = param_3 >> 5; uVar1 != 0; uVar1 = uVar1 - 1) {
      WriteLongToBuffer(RAM, puVar2Offset + 1 * 4, param_2);
      WriteLongToBuffer(RAM, puVar2Offset + 2 * 4, param_2);
      WriteLongToBuffer(RAM, puVar2Offset + 3 * 4, param_2);
      WriteLongToBuffer(RAM, puVar2Offset + 4 * 4, param_2);
      WriteLongToBuffer(RAM, puVar2Offset + 5 * 4, param_2);
      WriteLongToBuffer(RAM, puVar2Offset + 6 * 4, param_2);
      WriteLongToBuffer(RAM, puVar2Offset + 7 * 4, param_2);
      puVar2Offset = puVar2Offset + 8 * 4;
      WriteLongToBuffer(RAM, puVar2Offset, param_2);
    }
    for (uVar1 = param_3 >> 2 & 7; uVar1 != 0; uVar1 = uVar1 - 1) {
      puVar2Offset = puVar2Offset + 1 * 4;
      WriteLongToBuffer(RAM, puVar2Offset, param_2);
    }
    puVar3Offset = ((int)puVar2Offset + 3);
    param_3 = param_3 & 3;
  }
  if (param_3 != 0) {
    do {
      param_3 = param_3 - 1;
      puVar3Offset = puVar3Offset + 1;
      WriteCharToBuffer(RAM, puVar3Offset, (char)param_2);
    } while (param_3 != 0);
    return;
  }
  return;
}

unsigned long FUN_8000518c(unsigned char* RAM, int param_1, unsigned long param_2, unsigned long param_3)

{
  FUN_800051bc(RAM, param_1, param_2, param_3);
  return param_1;
}

// This functions copies temporary buffer to real output buffer
void FUN_80145f54(unsigned char* RAM, int param_1)

{
  int iVar1 = 0;

  unsigned long functionPointer = CharArrayToLong(RAM, (param_1 + 0x2ad4));


  if (functionPointer == 0x8015CFFC)
  {
     //iVar1 = FUN_8015cffc(RAM, (param_1 + 0x1000), CharArrayToLong(RAM, param_1 + 0x2008), CharArrayToLong(RAM, param_1 + 0x2acc));
  }
  else
  {
 	throw;
  }
 
  if (iVar1 == 1) {
    WriteLongToBuffer(RAM, param_1 + 0x2010, 0);
    FUN_8000518c(RAM, param_1 + 0x2028,0,0xaa4);
  }

  WriteLongToBuffer(RAM, (param_1 + 0x2014), (int)((int)CharArrayToLong(RAM, param_1 + 0x2014) + (int)CharArrayToLong(RAM, param_1 + 0x2008)));
  WriteLongToBuffer(RAM, param_1 + 0x2008, 0);
  return;
}

void FUN_80145e60(unsigned char* RAM, int param_1, unsigned long param_2, unsigned long param_3)

{
  WriteLongToBuffer(RAM, param_1 + 0x2ad0, param_2);
  WriteLongToBuffer(RAM, param_1 + 0x2ad4, param_3);
  return;
}

void FUN_80145e6c(unsigned char* RAM, int param_1, unsigned long param_2, unsigned long param_3, unsigned long param_4,
                 unsigned long param_5)

{
  FUN_8000518c(RAM, param_1,0,0x2ad8);
  WriteLongToBuffer(RAM, param_1 + 0x2010, param_5);
  WriteLongToBuffer(RAM, param_1 + 0x200c, param_5);
  WriteLongToBuffer(RAM, param_1 + 0x2000, 0);
  WriteLongToBuffer(RAM, param_1 + 0x2004, 0);
  WriteLongToBuffer(RAM, param_1 + 0x2008, 0);
  FUN_80145e60(RAM, param_1, param_2, param_3);
  WriteLongToBuffer(RAM, param_1 + 0x2acc, param_4);
  return;
}


void FUN_801458c4(unsigned char* RAM, int param_1)
{
  //short *psVar1;
  int psVar1Offset;
  //short *psVar2;
  int psVar2Offset;
  //short *psVar3;
  int psVar3Offset;
  //short *psVar4;
  int psVar4Offset;
  //unsigned short *puVar5;
  int puVar5Offset;
  int iVar6;
  int iVar7;
  int iVar8;
  
  puVar5Offset = (param_1 + 0x80);
  WriteLongToBuffer(RAM, param_1 + 0xa5c, 1);
  WriteLongToBuffer(RAM, param_1 + 0xa60, 8);
  WriteLongToBuffer(RAM, param_1 + 0xa50, 0);
  WriteLongToBuffer(RAM, param_1 + 0xa64, 0);
  WriteLongToBuffer(RAM, param_1 + 0xa54, 1);
  WriteLongToBuffer(RAM, param_1 + 0xa58, 1);
  WriteShortToBuffer(RAM, param_1 + 0xa42, 0x28);
  WriteShortToBuffer(RAM, param_1 + 0xa40, 0x28);
  WriteShortToBuffer(RAM, param_1 + 0xa46, 0x28);
  WriteShortToBuffer(RAM, param_1 + 0xa44, 0x28);
  WriteShortToBuffer(RAM, param_1 + 0xa4a, 0x28);
  WriteShortToBuffer(RAM, param_1 + 0xa48, 0x28);
  WriteShortToBuffer(RAM, param_1 + 0xa4e, 0x28);
  WriteShortToBuffer(RAM, param_1 + 0xa4c, 0x28);
  iVar6 = 2;
  do {
    WriteShortToBuffer(RAM, puVar5Offset, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 1 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 2 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 3 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 4 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 5 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 6 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 7 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 8 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 9 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0xA * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0xB * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0xC * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0xD * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0xE * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0xF * 2, 0);

	WriteShortToBuffer(RAM, puVar5Offset + 0x10 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x11 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x12 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x13 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x14 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x15 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x16 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x17 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x18 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x19 * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x1A * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x1B * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x1C * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x1D * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x1E * 2, 0);
	WriteShortToBuffer(RAM, puVar5Offset + 0x1F * 2, 0);

    puVar5Offset = puVar5Offset + 0x20;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  psVar3Offset = (param_1 + 0xfc);
  psVar4Offset = (param_1 + 0x7e);
  iVar6 = 9;

  do {
    WriteShortToBuffer(RAM, psVar4Offset, (short)CharArrayToShort(RAM, psVar3Offset) + (short)CharArrayToShort(RAM, psVar3Offset + 1 * 2));
	WriteShortToBuffer(RAM, psVar4Offset + (-1 * 2), (short)CharArrayToShort(RAM, psVar3Offset + (-2 * 2)) + (short)CharArrayToShort(RAM, psVar3Offset + (-1 * 2)));
	WriteShortToBuffer(RAM, psVar4Offset + (-2 * 2), (short)CharArrayToShort(RAM, psVar3Offset + (-4 * 2)) + (short)CharArrayToShort(RAM, psVar3Offset + (-3 * 2)));
	WriteShortToBuffer(RAM, psVar4Offset + (-3 * 2), (short)CharArrayToShort(RAM, psVar3Offset + (-6 * 2)) + (short)CharArrayToShort(RAM, psVar3Offset + (-5 * 2)));
	WriteShortToBuffer(RAM, psVar4Offset + (-4 * 2), (short)CharArrayToShort(RAM, psVar3Offset + (-8 * 2)) + (short)CharArrayToShort(RAM, psVar3Offset + (-7 * 2)));
	WriteShortToBuffer(RAM, psVar4Offset + (-5 * 2), (short)CharArrayToShort(RAM, psVar3Offset + (-10 * 2)) + (short)CharArrayToShort(RAM, psVar3Offset + (-9 * 2)));

    psVar1Offset = psVar3Offset + -0xc * 2;
    psVar2Offset = psVar3Offset + -0xb * 2;
    psVar3Offset = psVar3Offset + -0xe * 2;

    WriteLongToBuffer(RAM, psVar4Offset + -6 * 2, (short)CharArrayToShort(RAM, psVar1Offset) + (short)CharArrayToShort(RAM, psVar2Offset));
    psVar4Offset = psVar4Offset + -7 * 2;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  iVar6 = param_1 + 0x80;
  iVar7 = 2;
  do {
    WriteShortToBuffer(RAM, iVar6 + 0x100, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x102, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x104, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x106, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x108, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x10a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x10c, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x10e, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x110, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x112, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x114, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x116, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x118, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x11a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x11c, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x11e, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x120, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x122, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x124, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x126, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x128, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x12a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x12C, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x12e, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x130, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x132, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x134, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x136, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x138, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x13a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x13c, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x13e, 1);
    iVar6 = iVar6 + 0x40;
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  iVar6 = param_1 + 0xfc;
  iVar7 = param_1 + 0x7e;
  iVar8 = 9;
  do {
    WriteShortToBuffer(RAM, (iVar7 + 0x100), (short)CharArrayToShort(RAM, iVar6 + 0x100) + (short)CharArrayToShort(RAM, iVar6 + 0x102));
    WriteShortToBuffer(RAM, (iVar7 + 0xfe), (short)CharArrayToShort(RAM, iVar6 + 0xfc) + (short)CharArrayToShort(RAM, iVar6 + 0xfe));
    WriteShortToBuffer(RAM, (iVar7 + 0xfc), (short)CharArrayToShort(RAM, iVar6 + 0xf8) + (short)CharArrayToShort(RAM, iVar6 + 0xfa));
    WriteShortToBuffer(RAM, (iVar7 + 0xfa), (short)CharArrayToShort(RAM, iVar6 + 0xf4) + (short)CharArrayToShort(RAM, iVar6 + 0xf6));
    WriteShortToBuffer(RAM, (iVar7 + 0xf8), (short)CharArrayToShort(RAM, iVar6 + 0xf0) + (short)CharArrayToShort(RAM, iVar6 + 0xf2));
    WriteShortToBuffer(RAM, (iVar7 + 0xf6), (short)CharArrayToShort(RAM, iVar6 + 0xec) + (short)CharArrayToShort(RAM, iVar6 + 0xee));
    psVar3Offset = (iVar6 + 0xe8);
    psVar4Offset = (iVar6 + 0xea);
    iVar6 = iVar6 + -0x1c;
    WriteShortToBuffer(RAM, (iVar7 + 0xf4), (short)CharArrayToShort(RAM, psVar3Offset) + (short)CharArrayToShort(RAM, psVar4Offset));
    iVar7 = iVar7 + -0xe;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  iVar6 = param_1 + 0x200;
  iVar7 = 8;
  do {
    WriteShortToBuffer(RAM, iVar6 + 0x240, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x242, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x244, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x246, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x248, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x24a, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x24c, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x24e, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x250, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x252, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x254, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x256, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x258, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x25a, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x25c, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x25e, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x260, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x262, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x264, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x266, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x268, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x26a, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x26c, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x26e, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x270, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x272, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x274, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x276, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x278, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x27a, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x27c, 0);
    WriteShortToBuffer(RAM, iVar6 + 0x27e, 0);
    iVar6 = iVar6 + 0x40;
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  iVar6 = param_1 + 0x3fc;
  iVar7 = param_1 + 0x1fe;
  iVar8 = 0x33;
  do {
    WriteShortToBuffer(RAM, (iVar7 + 0x240), (short)CharArrayToShort(RAM, iVar6 + 0x240) + (short)CharArrayToShort(RAM, iVar6 + 0x242));
    WriteShortToBuffer(RAM, (iVar7 + 0x23e), (short)CharArrayToShort(RAM, iVar6 + 0x23c) + (short)CharArrayToShort(RAM, iVar6 + 0x23e));
    WriteShortToBuffer(RAM, (iVar7 + 0x23c), (short)CharArrayToShort(RAM, iVar6 + 0x238) + (short)CharArrayToShort(RAM, iVar6 + 0x23a));
    WriteShortToBuffer(RAM, (iVar7 + 0x23a), (short)CharArrayToShort(RAM, iVar6 + 0x234) + (short)CharArrayToShort(RAM, iVar6 + 0x236));
    psVar3Offset = (iVar6 + 0x230);
    psVar4Offset = (iVar6 + 0x232);
    iVar6 = iVar6 + -0x14;
    WriteShortToBuffer(RAM, (iVar7 + 0x238), (short)CharArrayToShort(RAM, psVar3Offset) + (short)CharArrayToShort(RAM, psVar4Offset));
    iVar7 = iVar7 + -10;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  iVar6 = param_1 + 0x200;
  iVar7 = 8;
  do {
    WriteShortToBuffer(RAM, iVar6 + 0x640, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x642, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x644, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x646, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x648, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x64a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x64c, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x64e, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x650, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x652, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x654, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x656, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x658, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x65a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x65c, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x65e, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x660, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x662, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x664, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x666, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x668, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x66a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x66c, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x66e, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x670, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x672, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x674, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x676, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x678, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x67a, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x67c, 1);
    WriteShortToBuffer(RAM, iVar6 + 0x67e, 1);
    iVar6 = iVar6 + 0x40;
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  iVar6 = param_1 + 0x3fc;
  iVar7 = param_1 + 0x1fe;
  iVar8 = 0x33;
  do {
    WriteShortToBuffer(RAM, (iVar7 + 0x640), (short)CharArrayToShort(RAM, iVar6 + 0x640) + (short)CharArrayToShort(RAM, iVar6 + 0x642));
    WriteShortToBuffer(RAM, (iVar7 + 0x63e), (short)CharArrayToShort(RAM, iVar6 + 0x63c) + (short)CharArrayToShort(RAM, iVar6 + 0x63e));
    WriteShortToBuffer(RAM, (iVar7 + 0x63c), (short)CharArrayToShort(RAM, iVar6 + 0x638) + (short)CharArrayToShort(RAM, iVar6 + 0x63a));
    WriteShortToBuffer(RAM, (iVar7 + 0x63a), (short)CharArrayToShort(RAM, iVar6 + 0x634) + (short)CharArrayToShort(RAM, iVar6 + 0x636));
    psVar3Offset = (iVar6 + 0x630);
    psVar4Offset = (iVar6 + 0x632);
    iVar6 = iVar6 + -0x14;
    WriteShortToBuffer(RAM, (iVar7 + 0x638), (short)CharArrayToShort(RAM, psVar3Offset) + (short)CharArrayToShort(RAM, psVar4Offset));
    iVar7 = iVar7 + -10;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  WriteShortToBuffer(RAM, param_1 + 0x220, 0);
  iVar6 = param_1 + 0x3c;
  iVar7 = param_1 + 0x1e;
  WriteShortToBuffer(RAM, param_1 + 0x222, 0);
  WriteShortToBuffer(RAM, param_1 + 0x224, 0);
  WriteShortToBuffer(RAM, param_1 + 0x226, 0);
  WriteShortToBuffer(RAM, param_1 + 0x228, 0);
  WriteShortToBuffer(RAM, param_1 + 0x22a, 0);
  WriteShortToBuffer(RAM, param_1 + 0x22c, 0);
  WriteShortToBuffer(RAM, param_1 + 0x22e, 0);
  WriteShortToBuffer(RAM, param_1 + 0x230, 0);
  WriteShortToBuffer(RAM, param_1 + 0x232, 0);
  WriteShortToBuffer(RAM, param_1 + 0x234, 0);
  WriteShortToBuffer(RAM, param_1 + 0x236, 0);
  WriteShortToBuffer(RAM, param_1 + 0x238, 0);
  WriteShortToBuffer(RAM, param_1 + 0x23a, 0);
  WriteShortToBuffer(RAM, param_1 + 0x23c, 0);
  WriteShortToBuffer(RAM, param_1 + 0x23e, 0);
  iVar8 = 3;
  do {
    WriteShortToBuffer(RAM, (iVar7 + 0x200), (short)CharArrayToShort(RAM, iVar6 + 0x200) + (short)CharArrayToShort(RAM, iVar6 + 0x202));
    WriteShortToBuffer(RAM, (iVar7 + 0x1fe), (short)CharArrayToShort(RAM, iVar6 + 0x1fc) + (short)CharArrayToShort(RAM, iVar6 + 0x1fe));
    WriteShortToBuffer(RAM, (iVar7 + 0x1fc), (short)CharArrayToShort(RAM, iVar6 + 0x1f8) + (short)CharArrayToShort(RAM, iVar6 + 0x1fa));
    WriteShortToBuffer(RAM, (iVar7 + 0x1fa), (short)CharArrayToShort(RAM, iVar6 + 500) + (short)CharArrayToShort(RAM, iVar6 + 0x1f6));
    psVar3Offset = (iVar6 + 0x1f0);
    psVar4Offset = (iVar6 + 0x1f2);
    iVar6 = iVar6 + -0x14;
    WriteShortToBuffer(RAM, (iVar7 + 0x1f8), (short)CharArrayToShort(RAM, psVar3Offset) + (short)CharArrayToShort(RAM, psVar4Offset));
    iVar7 = iVar7 + -10;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  FUN_80145774(RAM, param_1 + 0x200,0x10,6000,0x18,0);
  return;
}

void FUN_80146004(unsigned char* RAM, unsigned char* compressedFile, int param_1)
{
  unsigned int uVar1;
  unsigned short uVar2;
  int iVar3;
  
  WriteShortToBuffer(RAM, param_1 + 0x201a, 0xffff);
  WriteShortToBuffer(RAM, param_1 + 0x201c, 0);
  WriteLongToBuffer(RAM, param_1 + 0x2020, 0);
  if ((int)CharArrayToLong(RAM, param_1 + 0x2000) < 1) {
    FUN_80145edc(RAM, compressedFile, param_1);
    if ((int)CharArrayToLong(RAM, param_1 + 0x2000) < 1) {
      uVar1 = 0xffffffff;
    }
    else {
      WriteLongToBuffer(RAM, param_1 + 0x2000, (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
      iVar3 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
      WriteLongToBuffer(RAM, param_1 + 0x2004, (int)(iVar3 + 1));
      uVar1 = (unsigned int)CharArrayToChar(RAM, param_1 + iVar3);
    }
  }
  else {
    WriteLongToBuffer(RAM, param_1 + 0x2000, (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
    iVar3 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
    WriteLongToBuffer(RAM, param_1 + 0x2004, (int)(iVar3 + 1));
    uVar1 = (unsigned int)CharArrayToChar(RAM, param_1 + iVar3);
  }
  WriteShortToBuffer(RAM, param_1 + 0x2018, (short)(uVar1 << 8));
  if ((int)CharArrayToLong(RAM, param_1 + 0x2000) < 1) {
    FUN_80145edc(RAM, compressedFile, param_1);
    if ((int)CharArrayToLong(RAM, param_1 + 0x2000) < 1) {
      uVar2 = 0xffff;
    }
    else {
      WriteLongToBuffer(RAM, param_1 + 0x2000, (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
      iVar3 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
      WriteLongToBuffer(RAM, param_1 + 0x2004, (int)(iVar3 + 1));
      uVar2 = (unsigned short)CharArrayToChar(RAM, param_1 + iVar3);
    }
  }
  else {
    WriteLongToBuffer(RAM, param_1 + 0x2000, (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
    iVar3 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
    WriteLongToBuffer(RAM, param_1 + 0x2004, (int)(iVar3 + 1));
    uVar2 = (unsigned short)CharArrayToChar(RAM, param_1 + iVar3);
  }
  WriteShortToBuffer(RAM, param_1 + 0x2018, (unsigned short)(CharArrayToShort(RAM, param_1 + 0x2018) | uVar2 & 0xff));
  return;
}

void FUN_80145fcc(unsigned char* RAM, unsigned char* compressedFile, int param_1)

{
  FUN_801458c4(RAM, param_1 + 0x2028);
  FUN_80146004(RAM, compressedFile, param_1);
  return;
}

unsigned long FUN_80146110(unsigned char* RAM, unsigned char* compressedFile, unsigned long compressedFileSize, unsigned long param_1, unsigned long outputAddress, CString outputFilename)
{
	FILE* outb = fopen(outputFilename, "wb");
    fclose(outb);

  bool bVar1;
  unsigned long uVar2;
  short sVar3;
  int iVar4;
  unsigned long uVar5;
  unsigned long uVar6;
  int iVar7;
  unsigned long uVar8;
  int iVar9;
  int iVar10;
  unsigned long uVar11;
  int iVar12;
  unsigned long uVar13;
  unsigned long uVar14;
  int iVar15;
  unsigned long uVar16;
  unsigned long uVar17;
  short uVar18;
  int iVar19;
  int iVar20;
  
  iVar20 = param_1 + 0x2028;
  FUN_80145e6c(RAM, param_1, 0x8015CE48, 0x8015CFFC, 0x805B9140, compressedFileSize);
  WriteLongToBuffer(RAM, param_1 + 0x2a9c, 0x79e0);
  WriteLongToBuffer(RAM, param_1 + 0x2ac8, outputAddress); //R8
  WriteLongToBuffer(RAM, param_1 + 0x2aa4, 0);

  //80146148
  FUN_80145fcc(RAM, compressedFile, param_1);

  iVar19 = (int)CharArrayToLong(RAM, param_1 + 0x2a78);
  uVar16 = CharArrayToLong(RAM, param_1 + 0x2a8c);
  do {
    while( true ) {
      uVar6 = CharArrayToShort(RAM, param_1 + 0x201c);
      iVar15 = iVar20 + uVar16 * 4;
      uVar17 = CharArrayToShort(RAM, param_1 + 0x2018);
      uVar11 = CharArrayToShort(RAM, iVar15 + 0xa40);
      uVar13 = uVar11 + CharArrayToShort(RAM, iVar15 + 0xa42);
      iVar4 = uVar13 + 1;
      iVar10 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar6) + 1;
      iVar7 = (int)(iVar4 * ((uVar17 - uVar6) + 1) + -1) / iVar10;
      if ((int)uVar11 <= iVar7) break;
      uVar8 = CharArrayToLong(RAM, param_1 + 0x2020);
      uVar11 = ((int)(iVar10 * uVar11) / iVar4 + uVar6) - 1;
      uVar5 = uVar11 ^ uVar6;
      while ((uVar5 & 0x8000) == 0) {
        uVar8 = uVar8 << 1;
        uVar6 = uVar6 << 1;
        uVar11 = uVar11 << 1 | 1;
        if ((uVar8 & 0xfe) == 0) {
          if ((int)CharArrayToLong(RAM, param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar8 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar8 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar8 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
          if ((uVar8 & 0x100) == 0) {
            uVar8 = uVar8 << 1 | 1;
          }
          else {
            uVar8 = 0x100;
          }
        }
        uVar17 = uVar17 << 1 | uVar8 >> 8 & 1;
        uVar5 = uVar11 ^ uVar6;
      }
      uVar18 = (short)uVar17;
      uVar6 = uVar11 << 0x10 | uVar6;
      uVar11 = uVar6;
      while ((uVar11 & 0x40004000) == 0x4000) {
        uVar8 = uVar8 << 1;
        uVar11 = uVar6 << 1;
        uVar6 = uVar11 & 0xffff7ffe | 0x80010000;
        if ((uVar8 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar8 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar8 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar8 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
          if ((uVar8 & 0x100) == 0) {
            uVar8 = uVar8 << 1 | 1;
          }
          else {
            uVar8 = 0x100;
          }
        }
        uVar17 = uVar17 << 1 ^ 0x8000 | uVar8 >> 8 & 1;
        uVar18 = (short)uVar17;
      }
      WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar6);
      WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar6 >> 0x10));
      WriteShortToBuffer(RAM, (param_1 + 0x2018), (short)uVar18);
      WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar8);
      WriteShortToBuffer(RAM, (iVar15 + 0xa40), (short)((short)CharArrayToShort(RAM, iVar15 + 0xa40) + 0x28));
      if (5999 < uVar13) {
        WriteShortToBuffer(RAM, (iVar15 + 0xa40), (short)((int)(unsigned long)CharArrayToShort(RAM, iVar15 + 0xa40) >> 1));
        if (CharArrayToShort(RAM, iVar15 + 0xa40) == 0) {
          WriteShortToBuffer(RAM, (iVar15 + 0xa40), 1);
        }
        WriteShortToBuffer(RAM, (iVar15 + 0xa42), (short)((int)(unsigned long)CharArrayToShort(RAM, iVar15 + 0xa42) >> 1));
        if (CharArrayToShort(RAM, iVar15 + 0xa42) == 0) {
          WriteShortToBuffer(RAM, (iVar15 + 0xa42), 1);
        }
      }
      uVar6  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
      uVar16 = (uVar16 & 1) << 1;
      uVar17  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
      uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x226a);
      iVar4 = uVar11 + (int)CharArrayToLong(RAM, param_1 + 0x2a84);
      iVar7 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar6) + 1;
      iVar10 = (int)(iVar4 * ((uVar17 - uVar6) + 1) + -1) / iVar7;
      if (iVar10 < (int)uVar11) {
        iVar7 = 2;
        iVar15 = 0;
        while( true ) {
          uVar6  = (unsigned long)CharArrayToShort(RAM, iVar20 + iVar7 * 2 + 0x240);
          if ((int)(iVar15 + uVar6) <= iVar10) {
            iVar15 = iVar15 + uVar6;
            iVar7 = iVar7 + 1;
          }
          if (0xff < iVar7) break;
          iVar7 = iVar7 << 1;
        }
        uVar5 = iVar7 - 0x100;
        uVar17  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
        uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
        iVar7 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar17) + 1;
        uVar6 = CharArrayToLong(RAM, param_1 + 0x2020);
        uVar13 = ((int)(iVar7 * (iVar15 + (unsigned long)CharArrayToShort(RAM, iVar20 + uVar5 * 2 + 0x440))) / iVar4 +
                 uVar17) - 1;
        uVar17 = uVar17 + (iVar7 * iVar15) / iVar4;
        uVar8 = uVar13 ^ uVar17;
        while ((uVar8 & 0x8000) == 0) {
          uVar6 = uVar6 << 1;
          uVar17 = uVar17 << 1;
          uVar13 = uVar13 << 1 | 1;
          if ((uVar6 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar6 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
                uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
            if ((uVar6 & 0x100) == 0) {
              uVar6 = uVar6 << 1 | 1;
            }
            else {
              uVar6 = 0x100;
            }
          }
          uVar11 = uVar11 << 1 | uVar6 >> 8 & 1;
          uVar8 = uVar13 ^ uVar17;
        }
        uVar18 = (short)uVar11;
        uVar17 = uVar13 << 0x10 | uVar17;
        uVar13 = uVar17;
        while ((uVar13 & 0x40004000) == 0x4000) {
          uVar6 = uVar6 << 1;
          uVar13 = uVar17 << 1;
          uVar17 = uVar13 & 0xffff7ffe | 0x80010000;
          if ((uVar6 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
			  // 80146a24
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar6 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
                uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
            if ((uVar6 & 0x100) == 0) {
              uVar6 = uVar6 << 1 | 1;
            }
            else {
              uVar6 = 0x100;
            }
          }
          uVar11 = uVar11 << 1 ^ 0x8000 | uVar6 >> 8 & 1;
          uVar18 = (short)uVar11;
        }
        WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar17);
        WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar17 >> 0x10));
        WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
        WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar6);
      }
      else {
        uVar13 = CharArrayToLong(RAM, param_1 + 0x2020);
        uVar8 = ((iVar7 * iVar4) / iVar4 + uVar6) - 1;
        uVar6 = uVar6 + (int)(iVar7 * uVar11) / iVar4;
        uVar11 = uVar8 ^ uVar6;
        while ((uVar11 & 0x8000) == 0) {
          uVar13 = uVar13 << 1;
          uVar6 = uVar6 << 1;
          uVar8 = uVar8 << 1 | 1;
          if ((uVar13 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar11 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
                uVar11 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar11 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
            if ((uVar11 & 0x100) == 0) {
              uVar13 = uVar11 << 1 | 1;
            }
            else {
              uVar13 = 0x100;
            }
          }
          uVar17 = uVar17 << 1 | uVar13 >> 8 & 1;
          uVar11 = uVar8 ^ uVar6;
        }
        uVar18 = (short)uVar17;
        uVar6 = uVar8 << 0x10 | uVar6;
        uVar11 = uVar6;
        while ((uVar11 & 0x40004000) == 0x4000) {
          uVar13 = uVar13 << 1;
          uVar11 = uVar6 << 1;
          uVar6 = uVar11 & 0xffff7ffe | 0x80010000;
          if ((uVar13 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar13 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
                uVar13 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar13 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
            if ((uVar13 & 0x100) == 0) {
              uVar13 = uVar13 << 1 | 1;
            }
            else {
              uVar13 = 0x100;
            }
          }
          uVar17 = uVar17 << 1 ^ 0x8000 | uVar13 >> 8 & 1;
          uVar18 = (short)uVar17;
        }
        WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar6);
		//80146574
        iVar4 = 2;
        iVar7 = 0;
        WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar6 >> 0x10));
        WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
        WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar13);
		// off here
        uVar6  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x266a);
        while( true ) {
          uVar17  = (unsigned long)CharArrayToShort(RAM, iVar20 + iVar4 * 2 + 0x640);
          if ((int)(iVar7 + uVar17) <=
              (int)(uVar6 * (((unsigned long)CharArrayToShort(RAM, param_1 + 0x2018) -
                             (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c)) + 1) + -1) /
              (int)(((unsigned long)CharArrayToShort(RAM, param_1 + 0x201a) - (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c)) + 1)
             ) {
            iVar7 = iVar7 + uVar17;
            iVar4 = iVar4 + 1;
          }
          if (0xff < iVar4) break;
          iVar4 = iVar4 << 1;
        }
		// 801465DC
        uVar5 = iVar4 - 0x100; // error this is 0x147 not 0x146 on 0x54b when issue occurs
        uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
        uVar13  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
        iVar10 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar11) + 1;
        uVar17 = CharArrayToLong(RAM, param_1 + 0x2020);
        uVar8 = ((int)(iVar10 * (iVar7 + (unsigned long)CharArrayToShort(RAM, iVar20 + uVar5 * 2 + 0x840))) /
                 (int)uVar6 + uVar11) - 1;
        uVar11 = uVar11 + (iVar10 * iVar7) / (int)uVar6;
        uVar6 = uVar8 ^ uVar11;
        while ((uVar6 & 0x8000) == 0) {
          uVar17 = uVar17 << 1;
          uVar11 = uVar11 << 1;
          uVar8 = uVar8 << 1 | 1;
          if ((uVar17 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar6 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
                uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
            }
            if ((uVar6 & 0x100) == 0) {
              uVar17 = uVar6 << 1 | 1;
            }
            else {
              uVar17 = 0x100;
            }
          }
          uVar13 = uVar13 << 1 | uVar17 >> 8 & 1;
          uVar6 = uVar8 ^ uVar11;
        }
        uVar18 = (short)uVar13;
        uVar11 = uVar8 << 0x10 | uVar11;
        uVar6 = uVar11;
        while ((uVar6 & 0x40004000) == 0x4000) {
          uVar17 = uVar17 << 1;
          uVar6 = uVar11 << 1;
          uVar11 = uVar6 & 0xffff7ffe | 0x80010000;
          if ((uVar17 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar17 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
                uVar17 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
              uVar17 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
            }
            if ((uVar17 & 0x100) == 0) {
              uVar17 = uVar17 << 1 | 1;
            }
            else {
              uVar17 = 0x100;
            }
          }
          uVar13 = uVar13 << 1 ^ 0x8000 | uVar17 >> 8 & 1;
          uVar18 = (short)uVar13;
        }
        WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar11);
        WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar11 >> 0x10));
        WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
        WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar17);
		//801467c4
        sVar3 = (short)(CharArrayToShort(RAM, iVar20 + iVar4 * 2 + 0x640));

        for (iVar7 = iVar4; iVar7 != 0; iVar7 = iVar7 >> 1) {
          iVar10 = iVar7 * 2 + 0x640;
          WriteShortToBuffer(RAM, (iVar20 + iVar10), (short)((short)CharArrayToShort(RAM, iVar20 + iVar10) - sVar3));
        }
        if (CharArrayToShort(RAM, param_1 + 0x266a) == 0) {
          WriteLongToBuffer(RAM, param_1 + 0x2a84, 0);
        }
        else {
          WriteLongToBuffer(RAM, (param_1 + 0x2a84), (int)((int)CharArrayToLong(RAM, param_1 + 0x2a84) + 1));
        }
        uVar6 = iVar4 - 0x108U & ~-((unsigned long)(uVar5 < 8) - ((int)uVar5 >> 0x1f));
        iVar7 = iVar20 + uVar6 * 2;
        while( true ) {
          iVar10 = 0xff;
          if (iVar4 + -0xf8 < 0xff) {
            iVar10 = iVar4 + -0xf8;
          }
          if (iVar10 <= (int)uVar6) break;
          if (CharArrayToShort(RAM, iVar7 + 0x840) != 0) {
            FUN_80145774(RAM, param_1 + 0x2668,0x100,1000,1,uVar6);
          }
          iVar7 = iVar7 + 2;
          uVar6 = uVar6 + 1;
        }
      }
      FUN_80145774(RAM, param_1 + 0x2268,0x100,1000,1,uVar5);
      if (CharArrayToShort(RAM, iVar20 + uVar5 * 2 + 0x440) == 3) {
        iVar4 = 1;
        if (CharArrayToLong(RAM, (int)param_1 + 0x2a84) < 2) {
          iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2a84) + -1;
        }
        WriteLongToBuffer(RAM, (param_1 + 0x2a84), (int)((int)CharArrayToLong(RAM, param_1 + 0x2a84) - iVar4));
      }
	  // Write Byte here to output 0x00, 0x26, etc
	  // 54B # is wrong...47 instead of 46, and all is off after this
	  // 80146AE8
      WriteCharToBuffer(RAM, ((int)CharArrayToLong(RAM, param_1 + 0x2ac8) + (int)CharArrayToLong(RAM, param_1 + 0x2aa4)), (char)uVar5);

      iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2008);
      WriteLongToBuffer(RAM, (param_1 + 0x2008), (int)(iVar4 + 1));
      WriteCharToBuffer(RAM, (param_1 + iVar4 + 0x1000), (char)uVar5);
      if (CharArrayToLong(RAM, (int)param_1 + 0x2008) == 0x1000) {
        FUN_80145f54(RAM, param_1);
      }
      iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2aa4) + 1;
      WriteLongToBuffer(RAM, (param_1 + 0x2aa4), (int)(iVar4));
      if (iVar4 == (int)CharArrayToLong(RAM, param_1 + 0x2a9c)) {
        WriteLongToBuffer(RAM, (param_1 + 0x2aa4), 0);

		int sizeOutputFile = GetSizeFile(outputFilename);
		int extraOffset = 0;
		if (sizeOutputFile == 0)
			extraOffset = 4;
		FILE* out2 = fopen(outputFilename, "ab");
		fwrite(&RAM[((int)CharArrayToLong(RAM, param_1 + 0x2ac8) & 0xFFFFFFF) + extraOffset], 1, iVar4 - extraOffset, out2);
        fflush(out2);
		fclose(out2);
      }
      if (iVar19 < 0x79e0) {
        iVar19 = iVar19 + 1;
      }
    }

	//80146b44
	// After writing normal bytes
    if ((int)uVar13 <= iVar7) {
      uVar16 = CharArrayToLong(RAM, param_1 + 0x2020);
      uVar11 = ((iVar10 * iVar4) / iVar4 + uVar6) - 1;
      uVar6 = uVar6 + (int)(iVar10 * uVar13) / iVar4;
      uVar13 = uVar11 ^ uVar6;
      while ((uVar13 & 0x8000) == 0) {
        uVar16 = uVar16 << 1;
        uVar6 = uVar6 << 1;
        uVar11 = uVar11 << 1 | 1;
        if ((uVar16 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar16 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar19 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar19 + 1));
              uVar16 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar19);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar19 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar19 + 1));
            uVar16 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar19);
          }
          if ((uVar16 & 0x100) == 0) {
            uVar16 = uVar16 << 1 | 1;
          }
          else {
            uVar16 = 0x100;
          }
        }
        uVar17 = uVar17 << 1 | uVar16 >> 8 & 1;
        uVar13 = uVar11 ^ uVar6;
      }
      uVar18 = (short)uVar17;
      uVar6 = uVar11 << 0x10 | uVar6;
      uVar11 = uVar6;
      while ((uVar11 & 0x40004000) == 0x4000) {
        uVar16 = uVar16 << 1;
        uVar11 = uVar6 << 1;
        uVar6 = uVar11 & 0xffff7ffe | 0x80010000;
        if ((uVar16 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar16 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar19 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar19 + 1));
              uVar16 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar19);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar19 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar19 + 1));
            uVar16 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar19);
          }
          if ((uVar16 & 0x100) == 0) {
            uVar16 = uVar16 << 1 | 1;
          }
          else {
            uVar16 = 0x100;
          }
        }
        uVar17 = uVar17 << 1 ^ 0x8000 | uVar16 >> 8 & 1;
        uVar18 = (short)uVar17;
      }
      WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar6);
      WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar6 >> 0x10));
      WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
      WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar16);
      FUN_80145f54(RAM, param_1);

      //FILE* out = fopen("C:\\temp\\a.bin", "wb");
	  //fwrite(RAM, 1, 0x1800000, out);
	  //fflush(out);
	  //fclose(out);

	    int sizeOutputFile = GetSizeFile(outputFilename);
		int extraOffset = 0;
		if (sizeOutputFile == 0)
			extraOffset = 4;
		int last = (int)CharArrayToLong(RAM, param_1 + 0x2aa4);
		FILE* out2 = fopen(outputFilename, "ab");
		fwrite(&RAM[((int)CharArrayToLong(RAM, param_1 + 0x2ac8) & 0xFFFFFFF) + extraOffset], 1, last - extraOffset, out2);
        fflush(out2);
		fclose(out2);

	  //80147dd0
      return CharArrayToLong(RAM, (param_1 + 0x2014));
    }
    uVar8 = CharArrayToLong(RAM, param_1 + 0x2020);
    uVar5 = ((int)(iVar10 * uVar13) / iVar4 + uVar6) - 1;
    uVar6 = uVar6 + (int)(iVar10 * uVar11) / iVar4;
    uVar11 = uVar5 ^ uVar6;
    while ((uVar11 & 0x8000) == 0) {
      uVar8 = uVar8 << 1;
      uVar6 = uVar6 << 1;
      uVar5 = uVar5 << 1 | 1;
      if ((uVar8 & 0xfe) == 0) {
        if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
          FUN_80145edc(RAM, compressedFile, param_1);
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            uVar11 = 0xffffffff;
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar11 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
        }
        else {
          WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
          iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
          WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
          uVar11 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
        }
        if ((uVar11 & 0x100) == 0) {
          uVar8 = uVar11 << 1 | 1;
        }
        else {
          uVar8 = 0x100;
        }
      }
      uVar17 = uVar17 << 1 | uVar8 >> 8 & 1;
      uVar11 = uVar5 ^ uVar6;
    }
    uVar18 = (short)uVar17;
    uVar6 = uVar5 << 0x10 | uVar6;
    uVar11 = uVar6;
    while ((uVar11 & 0x40004000) == 0x4000) {
      uVar8 = uVar8 << 1;
      uVar11 = uVar6 << 1;
      uVar6 = uVar11 & 0xffff7ffe | 0x80010000;
      if ((uVar8 & 0xfe) == 0) {
        if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
          FUN_80145edc(RAM, compressedFile, param_1);
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            uVar8 = 0xffffffff;
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar8 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
        }
        else {
          WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
          iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
          WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
          uVar8 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
        }
        if ((uVar8 & 0x100) == 0) {
          uVar8 = uVar8 << 1 | 1;
        }
        else {
          uVar8 = 0x100;
        }
      }
      uVar17 = uVar17 << 1 ^ 0x8000 | uVar8 >> 8 & 1;
      uVar18 = (short)uVar17;
    }
    WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar6);
    WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar6 >> 0x10));
    WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
    WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar8);
    WriteShortToBuffer(RAM, (iVar15 + 0xa42), (short)(CharArrayToShort(RAM, iVar15 + 0xa42) + 0x28));
    if (5999 < uVar13) {
      WriteShortToBuffer(RAM, (iVar15 + 0xa40), (short)((short)((int)(unsigned long)CharArrayToShort(RAM, iVar15 + 0xa40) >> 1)));
      if (CharArrayToShort(RAM, iVar15 + 0xa40) == 0) {
        WriteShortToBuffer(RAM, (iVar15 + 0xa40), 1);
      }
      WriteShortToBuffer(RAM, (iVar15 + 0xa42), (short)((short)((int)(unsigned long)CharArrayToShort(RAM, iVar15 + 0xa42) >> 1)));
      if (CharArrayToShort(RAM, iVar15 + 0xa42) == 0) {
        WriteShortToBuffer(RAM, (iVar15 + 0xa42), 1);
      }
    }
    uVar16 = (uVar16 & 1) << 1 | 1; // Unused?? Used later?
    while ((int)(CharArrayToLong(RAM, param_1 + 0x2a7c)) < iVar19) {
	  unsigned long R7 = (int)CharArrayToLong(RAM, param_1 + 0x2a80);
      WriteLongToBuffer(RAM, (param_1 + 0x2a80), (int)((int)R7 + 1));
	  //80146d74
      FUN_80145774(RAM, param_1 + 0x2228,0x10,6000,0x18, R7); //Missing param in decompile...
      WriteLongToBuffer(RAM, (param_1 + 0x2a7c), (int)((int)CharArrayToLong(RAM, param_1 + 0x2a7c) << 1));
    }
    iVar4 = 2;
    iVar7 = 0;
    uVar6  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x222a);
    while( true ) {
      uVar17  = (unsigned long)CharArrayToShort(RAM, iVar20 + iVar4 * 2 + 0x200);
      if ((int)(iVar7 + uVar17) <=
          (int)(uVar6 * (((unsigned long)CharArrayToShort(RAM, param_1 + 0x2018) - (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c))
                        + 1) + -1) /
          (int)(((unsigned long)CharArrayToShort(RAM, param_1 + 0x201a) - (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c)) + 1)) {
        iVar7 = iVar7 + uVar17;
        iVar4 = iVar4 + 1;
      }
      if (0xf < iVar4) break;
      iVar4 = iVar4 << 1;
    }
    uVar5 = iVar4 - 0x10;
    uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
    uVar13  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
    iVar4 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar11) + 1;
    uVar17 = CharArrayToLong(RAM, param_1 + 0x2020);
    uVar8 = ((int)(iVar4 * (iVar7 + (unsigned long)CharArrayToShort(RAM, iVar20 + uVar5 * 2 + 0x220))) / (int)uVar6 +
            uVar11) - 1;
    uVar11 = uVar11 + (iVar4 * iVar7) / (int)uVar6;
    uVar6 = uVar8 ^ uVar11;
    while ((uVar6 & 0x8000) == 0) {
      uVar17 = uVar17 << 1;
      uVar11 = uVar11 << 1;
      uVar8 = uVar8 << 1 | 1;
      if ((uVar17 & 0xfe) == 0) {
        if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
          FUN_80145edc(RAM, compressedFile, param_1);
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            uVar6 = 0xffffffff;
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
        }
        else {
          WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
          iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
          WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
          uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
        }
        if ((uVar6 & 0x100) == 0) {
          uVar17 = uVar6 << 1 | 1;
        }
        else {
          uVar17 = 0x100;
        }
      }
      uVar13 = uVar13 << 1 | uVar17 >> 8 & 1;
      uVar6 = uVar8 ^ uVar11;
    }
    uVar18 = (short)uVar13;
    uVar11 = uVar8 << 0x10 | uVar11;
    uVar6 = uVar11;
    while ((uVar6 & 0x40004000) == 0x4000) {
      uVar17 = uVar17 << 1;
      uVar6 = uVar11 << 1;
      uVar11 = uVar6 & 0xffff7ffe | 0x80010000;
      if ((uVar17 & 0xfe) == 0) {
        if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
          FUN_80145edc(RAM, compressedFile, param_1);
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            uVar17 = 0xffffffff;
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar17 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
        }
        else {
          WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
          iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
          WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
          uVar17 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
        }
        if ((uVar17 & 0x100) == 0) {
          uVar17 = uVar17 << 1 | 1;
        }
        else {
          uVar17 = 0x100;
        }
      }
      uVar13 = uVar13 << 1 ^ 0x8000 | uVar17 >> 8 & 1;
      uVar18 = (short)uVar13;
    }
    WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar11);
    WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar11 >> 0x10));
    WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
    WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar17);
    FUN_80145774(RAM, param_1 + 0x2228,0x10,6000,0x18,uVar5);
    if (1 < (int)uVar5) {
      iVar4 = 1;
      if (uVar5 != 0) {
        uVar6 = uVar5 >> 3;
        if (uVar6 != 0) {
          do {
            iVar4 = iVar4 << 8;
            uVar6 = uVar6 - 1;
          } while (uVar6 != 0);
          uVar5 = uVar5 & 7;
          if (uVar5 == 0) goto LAB_80147020;
        }
        do {
          iVar4 = iVar4 << 1;
          uVar5 = uVar5 - 1;
        } while (uVar5 != 0);
      }
LAB_80147020:
      iVar4 = iVar4 >> 1;
      iVar10 = (int)CharArrayToLong(RAM, param_1 + 0x2a7c) >> 1;
      iVar7 = iVar4;
      if (iVar4 == iVar10) {
        iVar7 = iVar19 - iVar10;
      }
      uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
      uVar17  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
      uVar6 = CharArrayToLong(RAM, param_1 + 0x2020);
      iVar10 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar17) + 1;
      iVar15 = (int)(iVar7 * ((uVar11 - uVar17) + 1) + -1) / iVar10;
      uVar13 = ((iVar10 * (iVar15 + 1)) / iVar7 + uVar17) - 1;
      uVar17 = uVar17 + (iVar10 * iVar15) / iVar7;
      uVar8 = uVar13 ^ uVar17;

	  //8014708c
      while ((uVar8 & 0x8000) == 0) {
        uVar6 = uVar6 << 1;
        uVar17 = uVar17 << 1;
        uVar13 = uVar13 << 1 | 1;
        if ((uVar6 & 0xfe) == 0) {
		  // 801470B0 C98 ingame vs 0000 here
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar6 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
            uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
          }
          if ((uVar6 & 0x100) == 0) {
            uVar6 = uVar6 << 1 | 1;
          }
          else {
            uVar6 = 0x100;
          }
        }
        uVar11 = uVar11 << 1 | uVar6 >> 8 & 1;
        uVar8 = uVar13 ^ uVar17;
      }
      uVar18 = (short)uVar11;
      uVar17 = uVar13 << 0x10 | uVar17;
      uVar13 = uVar17;
      while ((uVar13 & 0x40004000) == 0x4000) {
        uVar6 = uVar6 << 1;
        uVar13 = uVar17 << 1;
        uVar17 = uVar13 & 0xffff7ffe | 0x80010000;
        if ((uVar6 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar6 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
            uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
          }
          if ((uVar6 & 0x100) == 0) {
            uVar6 = uVar6 << 1 | 1;
          }
          else {
            uVar6 = 0x100;
          }
        }
        uVar11 = uVar11 << 1 ^ 0x8000 | uVar6 >> 8 & 1;
        uVar18 = (short)uVar11;
      }
      WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar17);
      uVar5 = iVar15 + iVar4;
      WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar17 >> 0x10));
	  // 8014720C
	  // Wrong spot written here 8871 game vs 8873 here
      WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
      WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar6);
    }
    uVar6  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
    uVar17  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
    uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x202a);
    iVar4 = uVar11 + (int)CharArrayToLong(RAM, param_1 + 0x2a88);
    iVar7 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar6) + 1;
    iVar10 = (int)(iVar4 * ((uVar17 - uVar6) + 1) + -1) / iVar7;
    if (iVar10 < (int)uVar11) {
      iVar7 = 2;
      iVar15 = 0;
      while( true ) {
        uVar6  = (unsigned long)CharArrayToShort(RAM, iVar20 + iVar7 * 2);
        if ((int)(iVar15 + uVar6) <= iVar10) {
          iVar15 = iVar15 + uVar6;
          iVar7 = iVar7 + 1;
        }
        if (0x3f < iVar7) break;
        iVar7 = iVar7 << 1;
      }
      uVar14 = iVar7 - 0x40;
      uVar17  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
      uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
      iVar7 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar17) + 1;
      uVar6 = CharArrayToLong(RAM, param_1 + 0x2020);
      uVar13 = ((int)(iVar7 * (iVar15 + (unsigned long)CharArrayToShort(RAM, iVar20 + uVar14 * 2 + 0x80))) / iVar4 +
               uVar17) - 1;
      uVar17 = uVar17 + (iVar7 * iVar15) / iVar4;
      uVar8 = uVar13 ^ uVar17;
      while ((uVar8 & 0x8000) == 0) {
        uVar6 = uVar6 << 1;
        uVar17 = uVar17 << 1;
        uVar13 = uVar13 << 1 | 1;
        if ((uVar6 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar6 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
          if ((uVar6 & 0x100) == 0) {
            uVar6 = uVar6 << 1 | 1;
          }
          else {
            uVar6 = 0x100;
          }
        }
        uVar11 = uVar11 << 1 | uVar6 >> 8 & 1;
        uVar8 = uVar13 ^ uVar17;
      }
      uVar18 = (short)uVar11;
      uVar17 = uVar13 << 0x10 | uVar17;
      uVar13 = uVar17;
      while ((uVar13 & 0x40004000) == 0x4000) {
        uVar6 = uVar6 << 1;
        uVar13 = uVar17 << 1;
        uVar17 = uVar13 & 0xffff7ffe | 0x80010000;
        if ((uVar6 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar6 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
          if ((uVar6 & 0x100) == 0) {
            uVar6 = uVar6 << 1 | 1;
          }
          else {
            uVar6 = 0x100;
          }
        }
        uVar11 = uVar11 << 1 ^ 0x8000 | uVar6 >> 8 & 1;
        uVar18 = (short)uVar11;
      }
      WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar17);
      WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar17 >> 0x10));
      WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
      WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar6);
    }
    else {
      uVar13 = CharArrayToLong(RAM, param_1 + 0x2020);
      uVar8 = ((iVar7 * iVar4) / iVar4 + uVar6) - 1;
      uVar6 = uVar6 + (int)(iVar7 * uVar11) / iVar4;
      uVar11 = uVar8 ^ uVar6;
      while ((uVar11 & 0x8000) == 0) {
        uVar13 = uVar13 << 1;
        uVar6 = uVar6 << 1;
        uVar8 = uVar8 << 1 | 1;
        if ((uVar13 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar11 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar11 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar11 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
          if ((uVar11 & 0x100) == 0) {
            uVar13 = uVar11 << 1 | 1;
          }
          else {
            uVar13 = 0x100;
          }
        }
        uVar17 = uVar17 << 1 | uVar13 >> 8 & 1;
        uVar11 = uVar8 ^ uVar6;
      }
      uVar18 = (short)uVar17;
      uVar6 = uVar8 << 0x10 | uVar6;
      uVar11 = uVar6;
      while ((uVar11 & 0x40004000) == 0x4000) {
        uVar13 = uVar13 << 1;
        uVar11 = uVar6 << 1;
        uVar6 = uVar11 & 0xffff7ffe | 0x80010000;
        if ((uVar13 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar13 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar13 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
            uVar13 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
          }
          if ((uVar13 & 0x100) == 0) {
            uVar13 = uVar13 << 1 | 1;
          }
          else {
            uVar13 = 0x100;
          }
        }
        uVar17 = uVar17 << 1 ^ 0x8000 | uVar13 >> 8 & 1;
        uVar18 = (short)uVar17;
      }
      WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar6);
      iVar4 = 2;
      iVar7 = 0;
      WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar6 >> 0x10));
      WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
      WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar13);
      uVar6  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x212a);
      while( true ) {
        uVar17  = (unsigned long)CharArrayToShort(RAM, iVar20 + iVar4 * 2 + 0x100);
        if ((int)(iVar7 + uVar17) <=
            (int)(uVar6 * (((unsigned long)CharArrayToShort(RAM, param_1 + 0x2018) -
                           (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c)) + 1) + -1) /
            (int)(((unsigned long)CharArrayToShort(RAM, param_1 + 0x201a) - (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c)) + 1))
        {
          iVar7 = iVar7 + uVar17;
          iVar4 = iVar4 + 1;
        }
        if (0x3f < iVar4) break;
        iVar4 = iVar4 << 1;
      }
      uVar14 = iVar4 - 0x40;
      uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
      uVar13  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
      iVar10 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar11) + 1;
      uVar17 = CharArrayToLong(RAM, param_1 + 0x2020);
      uVar8 = ((int)(iVar10 * (iVar7 + (unsigned long)CharArrayToShort(RAM, iVar20 + uVar14 * 2 + 0x180))) / (int)uVar6
              + uVar11) - 1;
      uVar11 = uVar11 + (iVar10 * iVar7) / (int)uVar6;
      uVar6 = uVar8 ^ uVar11;
      while ((uVar6 & 0x8000) == 0) {
        uVar17 = uVar17 << 1;
        uVar11 = uVar11 << 1;
        uVar8 = uVar8 << 1 | 1;
        if ((uVar17 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar6 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
            uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
          }
          if ((uVar6 & 0x100) == 0) {
            uVar17 = uVar6 << 1 | 1;
          }
          else {
            uVar17 = 0x100;
          }
        }
        uVar13 = uVar13 << 1 | uVar17 >> 8 & 1;
        uVar6 = uVar8 ^ uVar11;
      }
      uVar18 = (short)uVar13;
      uVar11 = uVar8 << 0x10 | uVar11;
      uVar6 = uVar11;
      while ((uVar6 & 0x40004000) == 0x4000) {
        uVar17 = uVar17 << 1;
        uVar6 = uVar11 << 1;
        uVar11 = uVar6 & 0xffff7ffe | 0x80010000;
        if ((uVar17 & 0xfe) == 0) {
          if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
            FUN_80145edc(RAM, compressedFile, param_1);
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              uVar17 = 0xffffffff;
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
              uVar17 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
            }
          }
          else {
            WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
            iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
            WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar7 + 1));
            uVar17 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar7);
          }
          if ((uVar17 & 0x100) == 0) {
            uVar17 = uVar17 << 1 | 1;
          }
          else {
            uVar17 = 0x100;
          }
        }
        uVar13 = uVar13 << 1 ^ 0x8000 | uVar17 >> 8 & 1;
        uVar18 = (short)uVar13;
      }
      WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar11);
      WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar11 >> 0x10));
      WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
      WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar17);
      sVar3 = (short)(CharArrayToShort(RAM, iVar20 + iVar4 * 2 + 0x100));
      for (iVar7 = iVar4; iVar7 != 0; iVar7 = iVar7 >> 1) {
        iVar10 = iVar7 * 2 + 0x100;
        WriteShortToBuffer(RAM, (iVar20 + iVar10), (short)((short)CharArrayToShort(RAM, iVar20 + iVar10) - sVar3));
      }
      if (CharArrayToShort(RAM, param_1 + 0x212a) == 0) {
        WriteLongToBuffer(RAM, (param_1 + 0x2a88), 0);
      }
      else {
        WriteLongToBuffer(RAM, (param_1 + 0x2a88), (int)((int)CharArrayToLong(RAM, param_1 + 0x2a88) + 8));
      }
      uVar6 = iVar4 - 0x44U & ~-((unsigned long)(uVar14 < 4) - ((int)uVar14 >> 0x1f));
      iVar7 = iVar20 + uVar6 * 2;
      while( true ) {
        iVar10 = 0x3f;
        if (iVar4 + -0x3c < 0x3f) {
          iVar10 = iVar4 + -0x3c;
        }
        if (iVar10 <= (int)uVar6) break;
        if (CharArrayToShort(RAM, iVar7 + 0x180) != 0) {
          FUN_80145774(RAM, param_1 + 0x2128,0x40,6000,1,uVar6);
        }
        iVar7 = iVar7 + 2;
        uVar6 = uVar6 + 1;
      }
    }
    FUN_80145774(RAM, iVar20,0x40,6000,8,uVar14);
    if (CharArrayToShort(RAM, iVar20 + uVar14 * 2 + 0x80) == 0x18) {
      iVar4 = 8;
      if (CharArrayToLong(RAM, (int)param_1 + 0x2a88) < 9) {
        iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2a88) + -1;
      }
      WriteLongToBuffer(RAM, (param_1 + 0x2a88), (int)((int)CharArrayToLong(RAM, param_1 + 0x2a88) - iVar4));
    }
    if (uVar14 == 0xf) {
      uVar14 = 0x30f;
    }
    else {
      if (0xf < (int)uVar14) {
        uVar11  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x2018);
        uVar17  = (unsigned long)CharArrayToShort(RAM, param_1 + 0x201c);
        uVar6 = CharArrayToLong(RAM, param_1 + 0x2020);
        iVar4 = (CharArrayToShort(RAM, param_1 + 0x201a) - uVar17) + 1;
        iVar7 = (int)(((uVar11 - uVar17) + 1) * 0x10 + -1) / iVar4;
        uVar13 = iVar4 * (iVar7 + 1);
        uVar8 = iVar4 * iVar7;
        uVar13 = (((int)uVar13 >> 4) + (unsigned long)((int)uVar13 < 0 && (uVar13 & 0xf) != 0) + uVar17) - 1;
        uVar17 = uVar17 + ((int)uVar8 >> 4) + (unsigned long)((int)uVar8 < 0 && (uVar8 & 0xf) != 0);
        uVar8 = uVar13 ^ uVar17;
        while ((uVar8 & 0x8000) == 0) {
          uVar6 = uVar6 << 1;
          uVar17 = uVar17 << 1;
          uVar13 = uVar13 << 1 | 1;
          if ((uVar6 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar6 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
                uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
            if ((uVar6 & 0x100) == 0) {
              uVar6 = uVar6 << 1 | 1;
            }
            else {
              uVar6 = 0x100;
            }
          }
          uVar11 = uVar11 << 1 | uVar6 >> 8 & 1;
          uVar8 = uVar13 ^ uVar17;
        }
        uVar18 = (short)uVar11;
        uVar17 = uVar13 << 0x10 | uVar17;
        uVar13 = uVar17;
        while ((uVar13 & 0x40004000) == 0x4000) {
          uVar6 = uVar6 << 1;
          uVar13 = uVar17 << 1;
          uVar17 = uVar13 & 0xffff7ffe | 0x80010000;
          if ((uVar6 & 0xfe) == 0) {
            if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
              FUN_80145edc(RAM, compressedFile, param_1);
              if (CharArrayToLong(RAM, (int)param_1 + 0x2000) < 1) {
                uVar6 = 0xffffffff;
              }
              else {
                WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
                iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
                WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
                uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
              }
            }
            else {
              WriteLongToBuffer(RAM, (param_1 + 0x2000), (int)((int)CharArrayToLong(RAM, param_1 + 0x2000) + -1));
              iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2004);
              WriteLongToBuffer(RAM, (param_1 + 0x2004), (int)(iVar4 + 1));
              uVar6 = (unsigned long)CharArrayToChar(RAM, param_1 + iVar4);
            }
            if ((uVar6 & 0x100) == 0) {
              uVar6 = uVar6 << 1 | 1;
            }
            else {
              uVar6 = 0x100;
            }
          }
          uVar11 = uVar11 << 1 ^ 0x8000 | uVar6 >> 8 & 1;
          uVar18 = (short)uVar11;
        }
        WriteShortToBuffer(RAM, (param_1 + 0x201c), (short)uVar17);
        WriteShortToBuffer(RAM, (param_1 + 0x201a), (short)(uVar17 >> 0x10));
        uVar14 = (uVar14 - 0x10) * 0x10 + iVar7 + 0xf;
        WriteShortToBuffer(RAM, (param_1 + 0x2018), uVar18);
        WriteLongToBuffer(RAM, (param_1 + 0x2020), uVar6);
      }
    }
    if ((iVar19 < 0x79e0) && (iVar19 = uVar14 + iVar19 + 3, 0x79e0 < iVar19)) {
      iVar19 = 0x79e0;
    }
    iVar4 = (int)CharArrayToLong(RAM, param_1 + 0x2aa4);

	//80147b74
    iVar15 = uVar14 + 3;
    iVar10 = (int)CharArrayToLong(RAM, param_1 + 0x2a9c);
    iVar7 = (int)CharArrayToLong(RAM, param_1 + 0x2ac8);
    if ((int)uVar5 < iVar4) {
      iVar12 = (iVar4 + -1) - uVar5;
    }
    else {
      iVar12 = iVar4 + ((iVar10 + -1) - uVar5);
    }
    while (bVar1 = iVar15 != 0, iVar15 = iVar15 + -1, bVar1) {
	  // 80147BA8
	  // Write Loop Bytes From Previous Buffer
      WriteCharToBuffer(RAM, (iVar7 + iVar4), CharArrayToChar(RAM, iVar7 + iVar12));
      iVar9 = (int)CharArrayToLong(RAM, param_1 + 0x2008);
      uVar2 = CharArrayToChar(RAM, (iVar7 + iVar12));
      WriteLongToBuffer(RAM, (param_1 + 0x2008), (int)(iVar9 + 1));
      WriteCharToBuffer(RAM, param_1 + iVar9 + 0x1000, uVar2); // is this wrong address?
      if (CharArrayToLong(RAM, (int)param_1 + 0x2008) == 0x1000) {
	    // 80147bd4
		// EED -> 11FF is where this happens, big loop?
		// Need implement this
	    // I think this copies to real output spot, since this buffer is only 0x79E0, every 0x1000 data?
		// To 80CA6980, though it's missing 4 byte beginning, not sure why...maybe that's the length?
        FUN_80145f54(RAM, param_1);
      }
      iVar4 = iVar4 + 1;
	  
	  //80147bdc
      if (iVar4 == iVar10) {
        // if 79E0

	    //FILE* out = fopen("C:\\temp\\a.bin", "wb");
        //fwrite(RAM, 1, 0x1800000, out);
        //fflush(out);
        //fclose(out);

		int sizeOutputFile = GetSizeFile(outputFilename);
		int extraOffset = 0;
		if (sizeOutputFile == 0)
			extraOffset = 4;
		FILE* out2 = fopen(outputFilename, "ab");
		fwrite(&RAM[((int)CharArrayToLong(RAM, param_1 + 0x2ac8) & 0xFFFFFFF) + extraOffset], 1, iVar4 - extraOffset, out2);
        fflush(out2);
		fclose(out2);

        iVar4 = 0;
      }
      iVar12 = iVar12 + 1;
      if (iVar12 == iVar10) {
        iVar12 = 0;
      }
    }

	// 80147c08
	// EED -> 11FF messes up the input counter to 0, 805EFEA0
	// 41B7, calls function to reload next 0x1000 compressed
	
	// Messes up at 0x6800 now
    WriteLongToBuffer(RAM, (param_1 + 0x2aa4), (int)iVar4); // Last one at 0x5F6...problem before this spot, bytes differ starting at byte 0x54B, 0549 last one here before that
  } while( true );
}

unsigned long DecompressSK_ASC(unsigned char* compressedFile, unsigned long compressedFileSize, CString outputFilename)
{
	unsigned char* RAM = new unsigned char[0x1800000];
	for (int x = 0; x < 0x1800000; x++)
		RAM[x] = 0x00;

	// length is first 4 bytes of decompressed buffer
	int length = FUN_80146110(RAM, compressedFile, compressedFileSize - 8, 0x805EDEA0, 0x805F0980, outputFilename);

	delete [] RAM;

	return length;
}

int CSK_ASCCompression::DecompressFile(CString compressedFilename, CString outputFilename)
{
	FILE* inCompressed = fopen(compressedFilename, "rb");
	int compressedFileSize = GetSizeFile(compressedFilename);
	unsigned char* compressedFile = new unsigned char[compressedFileSize];
	fread(compressedFile, 1, compressedFileSize, inCompressed);
	fclose(inCompressed);
	
	int length = DecompressSK_ASC(compressedFile, compressedFileSize, outputFilename);

	delete [] compressedFile;

	return length;
}