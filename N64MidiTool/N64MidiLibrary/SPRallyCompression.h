// By Zoinkity

#pragma once

class CSPRallyCompression
{
public:
	CSPRallyCompression(void);
	~CSPRallyCompression(void);

	int decode(unsigned char* data, int dec_sz, unsigned char* out, int& compressedSize);
	int encode(unsigned char* data, int inputSize, unsigned char* out);
};
