#pragma once

#include "SoundToolExports.h"
// Decompression by Zoinkity

class SOUNDTOOL_API CNinDec
{
public:
	CNinDec(void);
	~CNinDec(void);
	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* outputBuffer);
};
