#pragma once

class CAcclaimDEANAudioDecompression
{
public:
	CAcclaimDEANAudioDecompression(void);
	~CAcclaimDEANAudioDecompression(void);

	bool DecompressSound(unsigned char* ROM, int romOffset, int compressedSize, CString outputFilename, float samplingRateFloat);
	bool DecompressMARKSound(unsigned char* ROM, int romOffset, int compressedSize, CString outputFilename, float samplingRateFloat, int scale, int type, int chunk, int chunkAmount);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
	int FindBestPredictorIndex(signed short previousPredictor, signed short previousValue, signed short sample, unsigned char* predictor);
	bool CompressSound(unsigned char* data, int dataLength, unsigned char*& outputBuffer, int& outputBufferLength, int scale, int type, int chunk);
	bool CompressSound(CString rawWavFileName, unsigned char*& outputBuffer, int& outputBufferLength, int scale, int type, int chunk);
	bool ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate);
};
