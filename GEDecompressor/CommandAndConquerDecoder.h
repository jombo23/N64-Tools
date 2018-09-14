// Compression/Decompression by Zoinkity!
#pragma once

class CommandAndConquerDecoder
{
public:
	CommandAndConquerDecoder(void);
	~CommandAndConquerDecoder(void);
	int dec(unsigned char* input, unsigned char* output, int& compressedSize, int kind);
	int encode(unsigned char* data, int dataSize, unsigned char* output, int kind);
};
