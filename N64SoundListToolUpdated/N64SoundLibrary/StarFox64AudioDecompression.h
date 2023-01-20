#pragma once
#include <vector>

class CStarFox64AudioDecompression
{
public:
	CStarFox64AudioDecompression(void);
	~CStarFox64AudioDecompression(void);

	bool DecompressSound(unsigned char* ROM, int romOffset, int inputLength, CString outputFilename, float samplingRateFloat);
	bool SaveWav(std::vector<unsigned short> samples, int samplingRate, CString outputFilename);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int samplingRate, CString outputFilename);
};
