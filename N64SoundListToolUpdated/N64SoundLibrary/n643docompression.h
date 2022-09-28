// By ZOINKITY
#pragma once

#include "SoundToolExports.h"
#include	<vector>
#include <math.h>

class SOUNDTOOL_API n643docompression
{
public:
	n643docompression(void);
	~n643docompression(void);

	int  _flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft);
	unsigned long helper(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, int num);
	int dec(unsigned char* inputBuffer, int dec_s, unsigned char* outputBuffer, int& compressedSize, bool littleEndianHeaderSize, bool sarge2style);
	int uncompressedSize(unsigned char* inputBuffer, bool littleEndianHeaderSize);
};
