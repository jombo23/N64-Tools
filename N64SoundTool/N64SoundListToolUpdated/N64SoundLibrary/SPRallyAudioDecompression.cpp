#include "StdAfx.h"
#include "SPRallyAudioDecompression.h"
#include "SharedFunctions.h"

CSPRallyAudioDecompression::CSPRallyAudioDecompression(void)
{
}

CSPRallyAudioDecompression::~CSPRallyAudioDecompression(void)
{
}

int CSPRallyAudioDecompression::FUN_00003424(unsigned char* inputBuffer, unsigned char* outputBuffer, unsigned long inputBufferOffset,__int64 outputBufferOffset,int numberEntries, int currentValue)
{
	//byte *inputBufferOffset;
  byte bVar1;
  byte bVar2;
  bool bVar3;
  int iVar4;
  unsigned long puVar5Offset; // unsigned char
  unsigned int uVar6;
  unsigned int uVar7;
  
  iVar4 = (int)outputBufferOffset;
  do {
    //cacheOp(0xd,outputBufferOffset);
    puVar5Offset = outputBufferOffset;
    outputBufferOffset = (__int64)(int)(puVar5Offset + 0x10);
	bVar2 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset);
    bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 1);
    uVar7 = 0x1c - bVar2;
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
	CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (bVar2 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
	bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 2);
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 1, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    uVar6 = (unsigned int)bVar2;
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
	CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 2, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (uVar6 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
	bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 3);
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 3, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 4, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (uVar6 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 4);
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 5, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 6, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (uVar6 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 5);
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 7, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 8, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (uVar6 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 6);
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 9, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    uVar6 = (unsigned int)bVar2;
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 10, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (uVar6 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 7);
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 0xb, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 0xc, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (uVar6 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    bVar1 = CSharedFunctions::CharArrayToChar(inputBuffer, inputBufferOffset + 8);
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 0xd, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 << 0x1c) >> (uVar7 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 0xe, (char)((unsigned int)currentValue >> 8));
    currentValue = currentValue + (((int)(char)bVar1 >> 4) << (uVar6 & 0x1f));
    bVar3 = currentValue < (short)-0x8000;
    if (0x7ffe < currentValue) {
      currentValue = 0x7fff;
    }
    inputBufferOffset = inputBufferOffset + 9;
    if (bVar3) {
      currentValue = (short)-0x8000;
    }
    CSharedFunctions::WriteCharToBuffer(outputBuffer, puVar5Offset + 0xf, (char)((unsigned int)currentValue >> 8));
  } while (outputBufferOffset != numberEntries * 0x10 + iVar4);
  return currentValue;
}


bool CSPRallyAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
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

	int flags = waveType; //0 = 8bit, 1=16bit

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

//80076230
unsigned int CSPRallyAudioDecompression::CalculateHash(unsigned char* nameArray, unsigned long offset, unsigned long filetableMultiplier) // Calculate Hash
{
	//byte *offset

  byte bVar1;
  unsigned long uVar2;
  
  bVar1 = CSharedFunctions::CharArrayToChar(nameArray, offset);
  while (bVar1 != 0) {
    uVar2 = (unsigned long)CSharedFunctions::CharArrayToChar(nameArray, offset);
    if (CSharedFunctions::CharArrayToChar(nameArray, offset) == 0x5c) {
      uVar2 = 0x2f;
    }
    offset = offset + 1;
    filetableMultiplier = (filetableMultiplier >> 1) + (uVar2 & 0x5f) * filetableMultiplier;
    bVar1 = CSharedFunctions::CharArrayToChar(nameArray, offset);
  }
  return filetableMultiplier;
}

std::vector<SPRallySoundTableEntry> CSPRallyAudioDecompression::GetSoundTableEntries(unsigned char* ROM, int startSoundTable, int endSoundTable)
{
	std::vector<SPRallySoundTableEntry> soundTableEntries;

	// 0x5B Entries
	for (int x = startSoundTable; x < endSoundTable; x += 0xC)
	{
		unsigned long namePtr = CSharedFunctions::CharArrayToLong(&ROM[x]);
		unsigned short flag = CSharedFunctions::CharArrayToShort(&ROM[x + 4]);
		unsigned short samplingRate = CSharedFunctions::CharArrayToShort(&ROM[x + 6]);
		unsigned long unknown = CSharedFunctions::CharArrayToLong(&ROM[x + 8]);
		soundTableEntries.push_back(SPRallySoundTableEntry(flag, samplingRate, unknown, namePtr));
	}
	return soundTableEntries;
}

std::vector<SPRallyFileTableEntry> CSPRallyAudioDecompression::GetFileTableEntries(unsigned char* ROM, int fileTableStart, unsigned long& fileTableHashMultiplier)
{
	unsigned long numberFileTableEntries = CSharedFunctions::CharArrayToLong(&ROM[fileTableStart]);
	fileTableHashMultiplier = CSharedFunctions::CharArrayToLong(&ROM[fileTableStart + 4]);

	std::vector<SPRallyFileTableEntry> fileTable;

	for (int x = 0; x < numberFileTableEntries; x++)
	{
		unsigned long offsetValue = CSharedFunctions::CharArrayToLong(&ROM[fileTableStart + 8 + (x * 8)]);
		unsigned long hash = CSharedFunctions::CharArrayToLong(&ROM[fileTableStart + 8 + (x * 8) + 4]);

		fileTable.push_back(SPRallyFileTableEntry(fileTableStart + offsetValue, hash));
	}
	return fileTable;
}

CString CSPRallyAudioDecompression::GetSoundFileName(unsigned char* ROM, SPRallySoundTableEntry soundTableEntry)
{
	unsigned long nameOffsetROM = (soundTableEntry.namePtr + 0xC00) & 0xFFFFFF;
	CString nameROMStr;
	nameROMStr.Format("sound/%s", &ROM[nameOffsetROM]);
	return nameROMStr;
}

SPRallyFileTableEntry CSPRallyAudioDecompression::GetSoundFile(unsigned char* ROM, SPRallySoundTableEntry soundTableEntry, std::vector<SPRallyFileTableEntry> fileTable, unsigned long fileTableHashMultiplier, CString& nameROMStr)
{
	SPRallyFileTableEntry spRallyFileTableEntry;
	nameROMStr = GetSoundFileName(ROM, soundTableEntry);
	int strLen = nameROMStr.GetLength();
	const char* nameROMStrBytes = nameROMStr.GetString();

	unsigned long hash = CalculateHash((unsigned char*)nameROMStrBytes, 0, fileTableHashMultiplier);
	for (int x = 0; x < fileTable.size(); x++)
	{
		if (fileTable[x].hash == hash)
		{
			return fileTable[x];
		}
	}

	throw;
}

bool CSPRallyAudioDecompression::DecodeSoundFile(unsigned char* ROM, SPRallySoundTableEntry soundTableEntry, std::vector<SPRallyFileTableEntry> fileTable, unsigned long fileTableHashMultiplier, int fileTableStart, float samplingRateFloat, CString outputFile)
{
	CString nameROMStr;
	SPRallyFileTableEntry currentEntry = GetSoundFile(ROM, soundTableEntry, fileTable, fileTableHashMultiplier, nameROMStr);

	unsigned long dataPointer = fileTableStart + CSharedFunctions::CharArrayToLong(&ROM[currentEntry.offset]);
	unsigned long sizeData = CSharedFunctions::CharArrayToLong(&ROM[currentEntry.offset + 4]);

	unsigned long numberSamples = (((unsigned __int64)sizeData * (unsigned __int64)0x38E38E39) >> 32) & 0xFFFFFFFF;
	numberSamples = numberSamples >> 1;

	unsigned long currentFileOffset = dataPointer;

	unsigned char* outputBuffer = new unsigned char[numberSamples * 0x10];
	FUN_00003424(&ROM[currentFileOffset], outputBuffer, 0, 0, numberSamples, 0);

	/*unsigned long A0 = 0x80255EE0;
	unsigned long A1 = 0x80155ED0;
	unsigned long A2 = numberSamples; //0x000000A4;
	unsigned long A3 = 0x00000000;

	int compressedSize = (currentEntry.offset - currentFileOffset);
	memcpy(&RAM[A0 & 0xFFFFFF], &ROM[currentFileOffset], compressedSize);

	FUN_00003424(RAM, A0, A1, A2, A3);

	FILE* a = fopen("C:\\temp\\ram.bin", "wb");
	fwrite(RAM, 1, 0x400000, a);
	fflush(a);
	fclose(a);*/

	bool result = SaveProcessedWav(outputBuffer, numberSamples * 0x10, 1, samplingRateFloat, outputFile);

	delete [] outputBuffer;
	return result;
}