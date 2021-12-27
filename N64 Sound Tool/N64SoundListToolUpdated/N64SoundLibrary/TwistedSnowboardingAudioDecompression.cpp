#include "StdAfx.h"
#include "TwistedSnowboardingAudioDecompression.h"
#include "SharedFunctions.h"

CTwistedSnowboardingAudioDecompression::CTwistedSnowboardingAudioDecompression(void)
{
}

CTwistedSnowboardingAudioDecompression::~CTwistedSnowboardingAudioDecompression(void)
{
}

void CTwistedSnowboardingAudioDecompression::FUN_00001220(unsigned char* input, int& inputSpot, unsigned char* output, int& outputLength, unsigned char* predictorData, int countEntries, signed short& previousValue1, signed short& previousValue2)
{
  //byte *param_1
  //short *param_2
  byte bVar1;
  short sVar2;
  short sVar3;
  __int64 lVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  unsigned long psVar8Offset; //short *psVar8;
  int iVar9;
  int iVar10;
  
  lVar4 = countEntries; //(__int64)CSharedFunctions::CharArrayToShort(RAM, param_3 + 0x1a);
  if (lVar4 == 0)
	  throw;
  iVar5 = (int)(short)previousValue1; //(int)(short)CSharedFunctions::CharArrayToShort(RAM, param_3 + 0x1c);
  iVar6 = (int)(short)previousValue2; //(int)(short)CSharedFunctions::CharArrayToShort(RAM, param_3 + 0x1e);
  while( true ) {
    lVar4 = (__int64)((int)lVar4 + -1);
    iVar7 = (CSharedFunctions::CharArrayToChar(input, inputSpot) >> 4) + 0xb;
	psVar8Offset = (/*param_4 + */(CSharedFunctions::CharArrayToChar(input, inputSpot) & 0xf) * 4);
    //sVar2 = (short)CSharedFunctions::CharArrayToShort(RAM, psVar8Offset + 1 * 2);
	sVar2 = (short)CSharedFunctions::CharArrayToShort(predictorData, psVar8Offset + 1 * 2);
	//sVar3 = (short)CSharedFunctions::CharArrayToShort(RAM, psVar8Offset);
	sVar3 = (short)CSharedFunctions::CharArrayToShort(predictorData, psVar8Offset);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 1);
    iVar9 = sVar3 * iVar5 + (((int)(char)bVar1 >> 4) << iVar7) + sVar2 * iVar6 >> 0xb;
	CSharedFunctions::WriteShortToBuffer(output, outputLength, (short)iVar9);
    iVar10 = sVar3 * iVar6 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + sVar2 * iVar9 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 1 * 2, (short)iVar10);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 2);
    iVar5 = (int)sVar3;
    iVar6 = (int)sVar2;
    iVar9 = iVar5 * iVar9 + (((int)(char)bVar1 >> 4) << iVar7) + iVar6 * iVar10 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 2 * 2, (short)iVar9);
    iVar10 = iVar5 * iVar10 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + iVar6 * iVar9 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 3 * 2, (short)iVar10);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 3);
    iVar9 = iVar5 * iVar9 + (((int)(char)bVar1 >> 4) << iVar7) + iVar6 * iVar10 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 4 * 2, (short)iVar9);
    iVar10 = iVar5 * iVar10 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + iVar6 * iVar9 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 5 * 2, (short)iVar10);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 4);
    iVar5 = (int)sVar3;
    iVar6 = (int)sVar2;
    iVar9 = iVar5 * iVar9 + (((int)(char)bVar1 >> 4) << iVar7) + iVar6 * iVar10 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 6 * 2, (short)iVar9);
    iVar10 = iVar5 * iVar10 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + iVar6 * iVar9 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 7 * 2, (short)iVar10);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 5);
    iVar9 = iVar5 * iVar9 + (((int)(char)bVar1 >> 4) << iVar7) + iVar6 * iVar10 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 8 * 2, (short)iVar9);
    iVar10 = iVar5 * iVar10 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + iVar6 * iVar9 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 9 * 2, (short)iVar10);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 6);
    iVar9 = iVar5 * iVar9 + (((int)(char)bVar1 >> 4) << iVar7) + iVar6 * iVar10 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 10 * 2, (short)iVar9);
    iVar10 = iVar5 * iVar10 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + iVar6 * iVar9 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 0xb * 2, (short)iVar10);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 7);
    iVar9 = iVar5 * iVar9 + (((int)(char)bVar1 >> 4) << iVar7) + iVar6 * iVar10 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 0xc * 2, (short)iVar9);
    iVar6 = iVar5 * iVar10 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + iVar6 * iVar9 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 0xd * 2, (short)iVar6);
    bVar1 = CSharedFunctions::CharArrayToChar(input, inputSpot + 8);
    iVar5 = sVar3 * iVar9 + (((int)(char)bVar1 >> 4) << iVar7) + sVar2 * iVar6 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 0xe * 2, (short)iVar5);
    iVar6 = sVar3 * iVar6 + ((((int)(char)bVar1 << 0x1c) >> 0x1c) << iVar7) + sVar2 * iVar5 >> 0xb;
    CSharedFunctions::WriteShortToBuffer(output, outputLength + 0xf * 2, (short)iVar6);
    inputSpot = inputSpot + 9;
    if (lVar4 == 0) break;
    outputLength = outputLength + 0x10 * 2;
  }
  previousValue1 = (short)iVar5;
  //CSharedFunctions::WriteShortToBuffer(RAM, param_3 + 0x1c, (short)iVar5);
  previousValue2 = (short)iVar6;
  //CSharedFunctions::WriteShortToBuffer(RAM, param_3 + 0x1e, (short)iVar6);
  return;
}

bool CTwistedSnowboardingAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
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