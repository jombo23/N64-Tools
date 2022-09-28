#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API TigDecoder
{
public:
	TigDecoder(void);
	~TigDecoder(void);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
