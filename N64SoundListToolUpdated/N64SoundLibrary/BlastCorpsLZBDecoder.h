#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API BlastCorpsLZBDecoder
{
public:
	BlastCorpsLZBDecoder(void);
	~BlastCorpsLZBDecoder(void);
	int dec(unsigned char* input, unsigned char* output, int& compressedSize);
	int nbits;
    unsigned long value;
    int base;
    unsigned char ring[0x2000];
	unsigned long bit(unsigned char* input, int& inputPosition, int& compressedSize, int count);
	void reset(int& inputPosition);
	void read(unsigned char* input, int& inputPosition, unsigned char* output, int& outputPosition, int& compressedSize);
};
