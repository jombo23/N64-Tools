#include "StdAfx.h"
#include "NamcoAudioDecompression.h"
#include "SharedFunctions.h"

CNamcoAudioDecompression::CNamcoAudioDecompression(void)
{
}

CNamcoAudioDecompression::~CNamcoAudioDecompression(void)
{
}
int CNamcoAudioDecompression::FUN_00097cec(unsigned char* RAM, int param_1, unsigned long param_2Offset, int param_3, int compressedDataOffset)
{
  //int *param_2
  byte bVar1;
  int iVar2;
  int iVar3;
  int uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  unsigned long pbVar8Offset; //byte *pbVar8;
  int iVar9;
  unsigned int uVar10;
  int iVar11;
  unsigned int uVar12;
  int iVar13;
  unsigned int uVar14;
  int iVar15;
  int iVar16;
  
  iVar9 = 0;
  uVar10 = 0;
  iVar13 = 0;
  iVar11 = 0; //(int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x84);
  iVar15 = 0; //(int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x88);
  uVar12 = 0; //CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x8c);
  uVar14 = 0; //CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x90);
  iVar7 = param_1; //param_1 + (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x44) * 4;
  iVar16 = 7; //(int)CSharedFunctions::CharArrayToLong(RAM, 0x800a2880 + iVar15 * 4);
  iVar2 = 0; //(int)CSharedFunctions::CharArrayToLong(RAM, iVar7 + 0x1c);
  iVar3 = compressedDataOffset; //(int)CSharedFunctions::CharArrayToLong(RAM, iVar7 + 0x10);
  iVar7 = param_3; //(int)CSharedFunctions::CharArrayToLong(RAM, iVar7 + 0x28);
  pbVar8Offset = iVar3; //(iVar3 + ((CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x58) >> 1) - iVar2));
  do {
    uVar10 = 0x291840ac; //uVar10 + CSharedFunctions::CharArrayToLong(RAM, param_1 + 100);
    iVar9 = 1; //iVar9 + (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x60) + (unsigned int)(uVar10 < CSharedFunctions::CharArrayToLong(RAM, param_1 + 100));
    iVar13 = iVar13 + 1;
  } while (iVar9 < 1);
  if (0 < param_3) {
    do {
      uVar10 = uVar14 & 0xf;
      if (uVar12 == 0) {
        if ((iVar3 + (iVar7 - iVar2)) <= pbVar8Offset)
			break;
		bVar1 = CSharedFunctions::CharArrayToChar(RAM, pbVar8Offset);
        pbVar8Offset = pbVar8Offset + 1;
        iVar9 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x58);
        uVar4 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x5c);
        uVar14 = (int)((unsigned int)bVar1 << 0x18) >> 0x18;
        uVar10 = ((unsigned int)bVar1 << 0x18) >> 0x1c;
      }
      else {
        iVar9 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x58);
		uVar4 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x5c);
      }
	  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x58), (int)(iVar9 + 1));
	  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x5c), uVar4);
      iVar15 = iVar15 + (int)CSharedFunctions::CharArrayToLong(RAM, uVar10 * 4 + 0x800A2840);
      uVar12 = (unsigned int)(uVar12 == 0);
      if (iVar15 < 0) {
        iVar15 = 0;
      }
      if (0x58 < iVar15) {
        iVar15 = 0x58;
      }
      iVar9 = iVar16 >> 3;
      if ((uVar10 & 4) != 0) {
        iVar9 = iVar9 + iVar16;
      }
      if ((uVar10 & 2) != 0) {
        iVar9 = iVar9 + (iVar16 >> 1);
      }
      if ((uVar10 & 1) != 0) {
        iVar9 = iVar9 + (iVar16 >> 2);
      }
      if ((uVar10 & 8) != 0) {
        iVar9 = -iVar9;
      }
      iVar11 = iVar11 + iVar9;
      if (iVar11 < 0x8000) {
        if (iVar11 < -0x8000) {
          iVar11 = -0x8000;
        }
      }
      else {
        iVar11 = 0x7fff;
      }
      iVar9 = 0xC800; //(int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x4c);
      iVar5 = 0xC800; //(int)CSharedFunctions::CharArrayToLong(RAM, param_1 + 0x50);
	  iVar16 = (int)CSharedFunctions::CharArrayToLong(RAM, (0x800a2880 + iVar15 * 4));
      for (iVar6 = iVar13; iVar6 != 0; iVar6 = iVar6 + -1) {
        param_3 = param_3 + -1;

		CSharedFunctions::WriteLongToBuffer(RAM, param_2Offset, (int)CSharedFunctions::CharArrayToLong(RAM, param_2Offset) + (iVar11 * iVar9 >> 0x10));
        CSharedFunctions::WriteLongToBuffer(RAM, param_2Offset + 1 * 4, (int)CSharedFunctions::CharArrayToLong(RAM, param_2Offset + 1 * 4) + (iVar11 * iVar5 >> 0x10));

        param_2Offset = param_2Offset + 2 * 4;
      }
    } while (true); //(0 < param_3);
  }
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x90), uVar14);
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x84), iVar11);
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x88), iVar15);
  CSharedFunctions::WriteLongToBuffer(RAM, (param_1 + 0x8c), uVar12);
  return param_3;
}


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void CNamcoAudioDecompression::FUN_00098778(unsigned char* RAM, unsigned long param_1Offset, unsigned long param_2Offset, int param_3, int param_4)
{
	//param_1 int*
	//param_2 short*
  int iVar1;
  unsigned int uVar2;
  
  param_3 = param_3 * 2;
  do {
	  iVar1 = (int)CSharedFunctions::CharArrayToLong(RAM, param_1Offset);
    param_1Offset = param_1Offset + 1 * 4;
    uVar2 = (unsigned int)((__int64)iVar1 * (__int64)param_4) >> 0x10 |
            (int)((unsigned __int64)((__int64)iVar1 * (__int64)param_4) >> 0x20) << 0x10;
    if ((int)uVar2 < -0x8000) {
      uVar2 = 0xffff8000;
	  CSharedFunctions::WriteLongToBuffer(RAM, 0x8011427c, CSharedFunctions::CharArrayToLong(RAM, 0x8011427c) + 1);
    }
    if (0x7fff < (int)uVar2) {
      uVar2 = 0x7fff;
      CSharedFunctions::WriteLongToBuffer(RAM, 0x8011427c, CSharedFunctions::CharArrayToLong(RAM, 0x8011427c) + 1);
    }
	CSharedFunctions::WriteShortToBuffer(RAM, param_2Offset, (short)uVar2);
    param_2Offset = param_2Offset + 1 * 2;
    param_3 = param_3 + -1;
  } while (param_3 != 0);
  return;
}

bool CNamcoAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
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

bool CNamcoAudioDecompression::DecompressSound(unsigned char* ROM, int offset, CString outputFilename, float samplingRateFloat)
{
	int sizeRAM = 0x800000;
	unsigned char* RAM = new unsigned char[sizeRAM];
	for (int x = 0; x < 0x800000; x++)
		RAM[x] = 0x00;

	/*FILE* inRAM = fopen("C:\\GoldeneyeStuff\\N64Hack\\Save\\prenamco.bin", "rb");
	fread(RAM, 1, sizeRAM, inRAM);
	fclose(inRAM);*/

	CSharedFunctions::StringToByteArray(&RAM[0x000A2840], "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000002000000040000000600000008FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000002000000040000000600000008");
	CSharedFunctions::StringToByteArray(&RAM[0x000A2880], "0000000700000008000000090000000A0000000B0000000C0000000D0000000E0000001000000011000000130000001500000017000000190000001C0000001F0000002200000025000000290000002D00000032000000370000003C00000042000000490000005000000058000000610000006B00000076000000820000008F0000009D000000AD000000BE000000D1000000E6000000FD0000011700000133000001510000017300000198000001C1000001EE000002200000025600000292000002D40000031C0000036C000003C3000004240000048E000005020000058300000610000006AB0000075600000812000008E0000009C300000ABD00000BD000000CFF00000E4C00000FBA0000114C00001307000014EE000017060000195400001BDC00001EA5000021B600002515000028CA00002CDF0000315B0000364B00003BB9000041B20000484400004F7E000057710000602F000069CE0000746200007FFF000014EE0000170600001954");

	int ramLocation = 0x80000000; //0x80118FE0;
	unsigned short samplingRate = samplingRateFloat; //CSharedFunctions::CharArrayToShort(ROM, offset + 2);
	int decompressedSize = CSharedFunctions::CharArrayToLong(ROM, offset + 4);
	unsigned long unknown = CSharedFunctions::CharArrayToLong(ROM, offset + 8);

	/*int compressedSize = (compressedSubDirOffset + compressedSubDirSize) - (offsetSubEntry + 0xC);
	if (subEntry < (numberSubEntries - 1))
		compressedSize = (boltLocation + CSharedFunctions::CharArrayToLong(ROM, compressedSubDirOffset + ((subEntry + 1) * 0x10) + 8)) - (offsetSubEntry + 0xC);*/

	memcpy(&RAM[ramLocation & 0xFFFFFF], &ROM[offset + 0xC], decompressedSize);

	FUN_00097cec(RAM, 0x80116260, 0x80100000/*0x80116720*/, decompressedSize, ramLocation);

	FUN_00098778(RAM, 0x80100000/*0x80116720*/, 0x80500000/*0x80117720*/, decompressedSize, 0x00007FFD);

	unsigned char* output = new unsigned char[decompressedSize * 4];
	for (int x = 0; x < decompressedSize * 4; x+= 2)
	{
		output[x] = RAM[(0x80500000 & 0xFFFFFF) + (x + 1)];
		output[x + 1] = RAM[(0x80500000 & 0xFFFFFF) + (x)];
	}

	SaveProcessedWav(output, decompressedSize * 4, 1, 22050, outputFilename);

	delete[] output;

	/*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
	fwrite(RAM, 1, sizeRAM, a);
	fflush(a);
	fclose(a);*/

	delete [] RAM;

	return true;
}