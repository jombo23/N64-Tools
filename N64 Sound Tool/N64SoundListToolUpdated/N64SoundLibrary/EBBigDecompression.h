// Compression by Zoinkity!
#pragma once
#include <vector>


struct EBBigEntry
{
	unsigned long offset;
	int dec_s;
	int cmp_s;
	CString data_t;
	CString name;

	EBBigEntry(unsigned long offset, int dec_s, int cmp_s, CString data_t, CString name)
	{
		this->offset = offset;
		this->dec_s = dec_s;
		this->cmp_s = cmp_s;
		this->data_t = data_t;
		this->name = name;
	}
};

class CEBBigDecompression
{
public:
	CEBBigDecompression(void);
	~CEBBigDecompression(void);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* outputBuffer, int& outputBufferPosition);
	std::vector<EBBigEntry> buffering_pointers(unsigned char* data);
	int  _flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, int compressedSize);
	unsigned long _nbits(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, int num, int compressedSize, unsigned long startValue = 0);
	void DecodeFile(unsigned char* ROM, unsigned long start, unsigned char*& data, int& dataLength);
};
