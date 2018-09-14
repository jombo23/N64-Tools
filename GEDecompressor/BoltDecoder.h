// By Zoinkity

#pragma once

class CBoltDecoder
{
public:
	CBoltDecoder(void);
	~CBoltDecoder(void);
	int dec(unsigned char* input, unsigned char* output, int& compressedSize, int decompressedSize);
};
