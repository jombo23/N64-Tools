// Compression by Zoinkity
#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API CSnoDecoder
{
public:
	CSnoDecoder(void);
	~CSnoDecoder(void);
	int dec(unsigned char* inputBuffer, int compressedSize, int expectedSize, unsigned char* output);
};
