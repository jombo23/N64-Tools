#pragma once

class EASportsDecoder
{
public:
	EASportsDecoder(void);
	~EASportsDecoder(void);

	int dec(unsigned char* input, unsigned char* output, int& compressedSize);
};
