#pragma once

#include "MidiExports.h"

class MIDI_API CRugratsDecoder
{
public:
	CRugratsDecoder(void);
	~CRugratsDecoder(void);

	int dec(unsigned char* data, int compressedSize, unsigned char* output);
};
