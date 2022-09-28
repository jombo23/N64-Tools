// Decompression by Zoinkity
#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API SydneyDecoder
{
public:
	SydneyDecoder(void);
	~SydneyDecoder(void);

	int dec(unsigned char* inputBuffer, int& compressedSize, int expectedSize, unsigned char* output);
};
