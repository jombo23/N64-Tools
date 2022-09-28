// Compression by Zoinkity
#pragma once

#include "MidiExports.h"

class MIDI_API CSnoDecoder
{
public:
	CSnoDecoder(void);
	~CSnoDecoder(void);
	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
