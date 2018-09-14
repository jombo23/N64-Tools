#pragma once

class TigDecoder
{
public:
	TigDecoder(void);
	~TigDecoder(void);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
