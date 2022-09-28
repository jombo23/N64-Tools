// Spinout
#pragma once

#include "MidiExports.h"

class MIDI_API YAY0
{
public:
	int decodeAll(unsigned char * src, unsigned char* result, int& fileSizeCompressed);
};
