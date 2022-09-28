#pragma once
#include <vector>

struct EBSoundIdx
{
	unsigned long index;
	unsigned long offset;
	unsigned long size;
	unsigned long flags;
	unsigned long hash;
	unsigned char* data;
	int dataLength;

	EBSoundIdx()
	{
	}

	EBSoundIdx(unsigned long index, unsigned long offset, unsigned long size, unsigned long flags, unsigned long hash)
	{
		this->index = index;
		this->offset = offset;
		this->size = size;
		this->flags = flags;
		this->hash = hash;
		this->data = NULL;
		this->dataLength = 0;
	}
};

class CExciteBikeSAMAudioDecompression
{
public:
	CExciteBikeSAMAudioDecompression(void);
	~CExciteBikeSAMAudioDecompression(void);
	bool SaveWav(unsigned char* wavData, int len, int waveType, int samplingRate, CString outputFilename);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
	bool DecompressSound(unsigned char* outputSfx, int currentSAMOffset, CString outputWavStr, float samplingRateFloat);
	static unsigned long GetSizeFile(CString filename);

	void FUN_0004f5f8(unsigned char* RAM);
	int GetInstrumentSlot(int id, std::vector<EBSoundIdx> ebSoundIndexes);
	bool DecodeSNG(unsigned char* sngFile, int sngFileOffset, CString outputFile, std::vector<EBSoundIdx> ebSoundIndexes, float samplingRateFloat);
	bool DecodeBIGSound(unsigned char* ROM, unsigned long offset, unsigned long size, CString outputWavStr, unsigned char*& data, int& sfxLength, float samplingRateFloat);
	std::vector<EBSoundIdx> GetSoundIndexes(unsigned char* outputSfx, int sfxOffset, unsigned long bigRawStart);
};
