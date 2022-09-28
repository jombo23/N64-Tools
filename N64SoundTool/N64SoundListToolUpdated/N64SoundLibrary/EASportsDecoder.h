#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API EASportsDecoder
{
public:
	EASportsDecoder(void);
	~EASportsDecoder(void);

	int dec(unsigned char* input, unsigned char* output, int& compressedSize);
};
