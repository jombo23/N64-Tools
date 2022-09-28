// By Zoinkity
#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API MidwayLZ
{
public:
	MidwayLZ(void);
	~MidwayLZ(void);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
	int decLZSS0B(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
