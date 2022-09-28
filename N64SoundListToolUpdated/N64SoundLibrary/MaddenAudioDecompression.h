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
	bool DecompressSoundMadden64(int soundType, unsigned char* ROM, int offset, int compressedSize, int decompressedSize, CString outputFilename, float samplingRateFloat);
	bool CompressSound(int soundType, unsigned char* data, int dataLength, unsigned char*& outputBuffer, int& outputBufferLength);
	bool CompressSound(int soundType, CString rawWavFileName, unsigned char*& outputBuffer, int& outputBufferLength);
	bool ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate);
};
