#pragma once

class CTopGearHyperbikeAudioDecompression
{
public:
	CTopGearHyperbikeAudioDecompression(void);
	~CTopGearHyperbikeAudioDecompression(void);
	void FUN_000481fc(unsigned char* RAM, unsigned long param_1, int param_2);
	bool DecompressSound(unsigned char* ROM, int offset, int numberUncompressedSamples, CString outputFilename, float samplingRateFloat);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
	bool SaveWav(unsigned char* wavData, int len, int waveType, int samplingRate, CString outputFilename);
};
