#pragma once

class CNamcoAudioDecompression
{
public:
	CNamcoAudioDecompression(void);
	~CNamcoAudioDecompression(void);
	void FUN_00098778(unsigned char* RAM, unsigned long param_1Offset, unsigned long param_2Offset, int param_3, int param_4);
	int FUN_00097cec(unsigned char* RAM, int param_1, unsigned long param_2Offset, int param_3, int compressedDataOffset);
	bool DecompressSound(unsigned char* ROM, int offset, CString outputFilename, float samplingRateFloat);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
};
