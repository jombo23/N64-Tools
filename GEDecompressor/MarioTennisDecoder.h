#pragma once

class CMarioTennisDecoder
{
public:
	CMarioTennisDecoder(void);
	~CMarioTennisDecoder(void);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
