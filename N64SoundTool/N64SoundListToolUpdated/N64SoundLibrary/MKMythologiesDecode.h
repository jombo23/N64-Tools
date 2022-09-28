// BY ZOINKITY
#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API CMKMythologiesDecode
{
public:
	CMKMythologiesDecode(void);
	~CMKMythologiesDecode(void);
	int  _flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft);
	unsigned long helper(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, int num);
	int dec(unsigned char* inputBuffer, int compressedSize, int dec_s, unsigned char* output);
};
