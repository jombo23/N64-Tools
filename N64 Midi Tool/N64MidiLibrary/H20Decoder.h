#pragma once

#include "MidiExports.h"

class MIDI_API CH20Decoder
{
public:
	CH20Decoder(void);
	~CH20Decoder(void);
	int decPolaris(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
