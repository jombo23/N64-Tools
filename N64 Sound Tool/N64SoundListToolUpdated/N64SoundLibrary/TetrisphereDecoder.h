// By Zoinkity
#pragma once

#include "SoundToolExports.h"
#include <vector>

class SOUNDTOOL_API CTetrisphereDecoder
{
public:
	CTetrisphereDecoder(void);
	~CTetrisphereDecoder(void);
	int decompressLZ(unsigned char* inputBuffer, int compressedSize, unsigned char* output, bool initialize);
	int decompressRLE(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
	int decode(unsigned char* inputBuffer, int compressedSize, unsigned char* output, int back);
	bool sqshHeader(unsigned char* inputBuffer, int& compressedSize, int& uncompressedSize);
	int sqshDecompress(unsigned char* inputBuffer, int compressedSize, int expectedSize, unsigned char* output);
	int sphereRead(std::vector<unsigned char>& state, unsigned char* inputBuffer, int compressedSize, unsigned char* output, bool writeback);
	int sphereDecompress(unsigned char* inputBuffer, unsigned char* output);
};
