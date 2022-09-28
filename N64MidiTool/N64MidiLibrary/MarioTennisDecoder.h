#pragma once

#include "MidiExports.h"

class MIDI_API CMarioTennisDecoder
{
public:
	CMarioTennisDecoder(void);
	~CMarioTennisDecoder(void);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
