#pragma once
#include "SharedFunctions.h"

struct WavEntry
{
	unsigned long offset;
	unsigned long size;
	CString name;

	WavEntry()
	{

	}

	WavEntry(unsigned long offset, unsigned long size, CString name)
	{
		this->offset = offset;
		this->size = size;
		this->name = name;
	}

};


class CFightingForceAudioDecompression
{
public:
	static char table8008251C[8];
	static short table80082524[0x100];
	static short table800824DC[0x20];
	CFightingForceAudioDecompression(void);
	~CFightingForceAudioDecompression(void);
	void DecodeWav(unsigned char* wavData, int wavDataLength, CString outputFilename, float samplingRateFloat);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
	bool FUN_00081a70(unsigned char* RAM, int param_1, unsigned long& param_2, unsigned long& param_3, int param_4, int in_stack_00000010, int dataLength);
};
