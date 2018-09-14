#pragma once

class CH20Decoder
{
public:
	CH20Decoder(void);
	~CH20Decoder(void);
	int decPolaris(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
};
