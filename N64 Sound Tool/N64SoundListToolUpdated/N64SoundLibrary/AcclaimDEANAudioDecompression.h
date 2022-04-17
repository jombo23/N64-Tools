#pragma once

class CAcclaimDEANAudioDecompression
{
public:
	CAcclaimDEANAudioDecompression(void);
	~CAcclaimDEANAudioDecompression(void);

	bool DecompressSound(unsigned char* ROM, int romOffset, int compressedSize, CString outputFilename, float samplingRateFloat);
	bool DecompressMARKSound(unsigned char* ROM, int romOffset, int compressedSize, CString outputFilename, float samplingRateFloat);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
};
