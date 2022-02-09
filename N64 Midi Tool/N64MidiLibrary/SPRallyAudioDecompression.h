#pragma once
#include <vector>

struct SPRallyFileTableEntry
{
	unsigned long offset;
	unsigned long hash;

	SPRallyFileTableEntry() { }
	SPRallyFileTableEntry(unsigned long offset, unsigned long hash)
	{
		this->offset = offset;
		this->hash = hash;
	}
};

struct SPSample
{
	unsigned char* data;
	int size;
	int offset;

	SPSample()
	{
		data = NULL;
		size = 0;
		offset = 0;
	}
};

struct SPRallySoundTableEntry
{
	unsigned short flag;
	unsigned short samplingRate;
	unsigned long unknown;
	unsigned long namePtr;

	SPRallySoundTableEntry() { }
	SPRallySoundTableEntry(unsigned short flag, unsigned short samplingRate, unsigned long unknown, unsigned long namePtr)
	{
		this->flag = flag;
		this->samplingRate = samplingRate;
		this->unknown = unknown;
		this->namePtr = namePtr;
	}
};

class CSPRallyAudioDecompression
{
public:
	CSPRallyAudioDecompression(void);
	~CSPRallyAudioDecompression(void);
	int FUN_00003424(unsigned char* inputBuffer, unsigned char* outputBuffer, unsigned long inputBufferOffset,__int64 outputBufferOffset,int numberEntries, int currentValue);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
	unsigned int CalculateHash(unsigned char* nameArray, unsigned long offset, unsigned long filetableMultiplier);
	std::vector<SPRallySoundTableEntry> GetSoundTableEntries(unsigned char* ROM, int startSoundTable, int endSoundTable);
	std::vector<SPRallyFileTableEntry> GetFileTableEntries(unsigned char* ROM, int fileTableStart, unsigned long& fileTableHashMultiplier);
	CString GetSoundFileName(unsigned char* ROM, SPRallySoundTableEntry soundTableEntry);
	SPRallyFileTableEntry GetSoundFile(unsigned char* ROM, SPRallySoundTableEntry soundTableEntry, std::vector<SPRallyFileTableEntry> fileTable, unsigned long fileTableHashMultiplier, CString& nameROMStr, int& index);
	SPRallyFileTableEntry GetFileEntry(unsigned char* ROM, CString nameROMStr, std::vector<SPRallyFileTableEntry> fileTable, unsigned long fileTableHashMultiplier, int& index);
	bool DecodeSoundFile(unsigned char* ROM, SPRallySoundTableEntry soundTableEntry, std::vector<SPRallyFileTableEntry> fileTable, unsigned long fileTableHashMultiplier, int fileTableStart, float samplingRateFloat, CString outputFile);
	bool DecodeImpulseTracker(unsigned char* ROM, unsigned long fileTableHashMultiplier, int fileTableStart, unsigned long musicOffset, CString outputFile, int sampleTableOffset);
};
