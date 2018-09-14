#pragma once

#include "MidiExports.h"

class MIDI_API CVigilanteDecoder
{
public:
	CVigilanteDecoder(void);
	~CVigilanteDecoder(void);
	int dec(unsigned char* data, int& compressedSize, unsigned char* output);
};
