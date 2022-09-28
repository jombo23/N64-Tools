#pragma once

#include "MidiExports.h"

class MIDI_API CASMICDecoder
{
public:
	CASMICDecoder(void);
	~CASMICDecoder(void);
	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
