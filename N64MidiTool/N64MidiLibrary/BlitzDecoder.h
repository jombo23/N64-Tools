#pragma once

#include "MidiExports.h"

class MIDI_API CBlitzDecoder
{
public:
	CBlitzDecoder(void);
	~CBlitzDecoder(void);

    unsigned long counter(unsigned char*& input, unsigned char cmd, unsigned char mask, int& compressedSize);
    int dec(unsigned char* input, unsigned char* output, int& compressedSize);

};
