#pragma once

#define MADDENSTANDARDAUDIO 0
#define MADDENFLOATAUDIO 1

class CMaddenAudioDecompression
{
public:
	CMaddenAudioDecompression(void);
	~CMaddenAudioDecompression(void);

	bool SaveWav(unsigned char* wavData, int len, int waveType, int samplingRate, CString outputFilename);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
	bool DecompressSound(int soundType, unsigned char* ROM, int offset, int compressedSize, int decompressedSize, CString outputFilename, float samplingRateFloat);
};
