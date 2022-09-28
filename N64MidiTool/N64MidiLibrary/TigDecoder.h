#pragma once

#include "MidiExports.h"

class MIDI_API TigDecoder
{
public:
	TigDecoder(void);
	~TigDecoder(void);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
