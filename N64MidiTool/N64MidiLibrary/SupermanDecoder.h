// BY ZOINKITY
#pragma once

#include "MidiExports.h"

class MIDI_API SupermanDecoder
{
public:
	SupermanDecoder(void);
	~SupermanDecoder(void);
	int header(unsigned char* data, int& compressedSize);
	unsigned long _flags(unsigned char* data, int& inputPosition, int compressedSize);
	bool GetFlag(unsigned long& flag, unsigned char* data, int& inputPosition, int compressedSize);
	int dec(unsigned char* data, int compressedSize, int dec_s, unsigned char* output);
};
