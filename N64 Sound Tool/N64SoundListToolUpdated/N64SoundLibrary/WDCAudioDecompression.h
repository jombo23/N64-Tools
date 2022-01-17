// Thanks to Gilles Siberlin and Glide64!
#pragma once
#include "SharedFunctions.h"

struct ZSortBOSSState {
	signed short table[8][8];
};

class CWDCAudioDecompression
{
public:
	CWDCAudioDecompression(void);
	~CWDCAudioDecompression(void);

	void ZSortBOSS_Audio1(unsigned char* RAM, unsigned char* DMEM, unsigned long _w0, unsigned long _w1 );
	void ZSortBOSS_Audio2(unsigned char* RAM, unsigned char* DMEM, unsigned long _w0, unsigned long _w1, int lengthOverride = -1, int DMEMDST = 0x30, int DMEMSPECIALVALUE = 0x904);
	void ZSortBOSS_Audio3(unsigned char* RAM, unsigned char* DMEM, ZSortBOSSState& gstate, unsigned long _w0, unsigned long _w1 );
	void ZSortBOSS_Audio4(unsigned char* RAM, unsigned char* DMEM, ZSortBOSSState& gstate, unsigned long _w0, unsigned long _w1, int inputSizeOverride = -1, int DMEMDST = 0x30 );
	bool DecompressSound(unsigned char* romSampleData, unsigned char* predictors, int inputSize, int outputSize, CString outputFilename, float samplingRateFloat);
	bool SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename);
};
