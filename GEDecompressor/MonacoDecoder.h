// BY ZOINKITY
#pragma once

class MonacoDecoder
{
public:
	MonacoDecoder(void);
	~MonacoDecoder(void);
	int header(unsigned char* data, int& compressedSize);
	unsigned long GetBits(int numBits, unsigned char* data, int& inputPosition, int compressedSize);
	int dec(unsigned char* data, int compressedSize, int dec_s, unsigned char* output);
	unsigned long bitRegister;
	int numBitsInRegister;
};
