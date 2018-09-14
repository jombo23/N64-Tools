#pragma once
// Decompression by Zoinkity

class CNinDec
{
public:
	CNinDec(void);
	~CNinDec(void);
	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* outputBuffer);
};
