// Compression by Zoinkity
#pragma once

#include "MidiExports.h"

class MIDI_API CSnowDecoder
{
public:
	CSnowDecoder(void);
	~CSnowDecoder(void);
	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
