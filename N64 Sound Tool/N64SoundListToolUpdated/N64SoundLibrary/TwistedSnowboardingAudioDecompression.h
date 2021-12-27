#pragma once

class CTwistedSnowboardingAudioDecompression
{
public:
	CTwistedSnowboardingAudioDecompression(void);
	~CTwistedSnowboardingAudioDecompression(void);
	void FUN_00001220(unsigned char* input, int& inputSpot, unsigned char* output, int& outputLength, unsigned char* predictorData, int countEntries, signed short& previousValue1, signed short& previousValue2);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
};
