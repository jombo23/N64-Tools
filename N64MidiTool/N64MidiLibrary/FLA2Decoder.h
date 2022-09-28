// BY ZOINKITY
#pragma once

#include "MidiExports.h"

class MIDI_API CFLA2Decoder
{
public:
	CFLA2Decoder(void);
	~CFLA2Decoder(void);
	int decFLA2(unsigned char* data, int& compressedSize, int dec_s, unsigned char* output);
};
