#pragma once

#include "MidiExports.h"

class MIDI_API CMidwayDecoder
{
public:
	CMidwayDecoder(void);
	~CMidwayDecoder(void);
	int dec(unsigned char* data, int& compressedSize, unsigned char* output, CString mode="MIDWAY", int pos = 0);
};
