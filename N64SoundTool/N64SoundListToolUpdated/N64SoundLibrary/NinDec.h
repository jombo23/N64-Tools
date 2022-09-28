#pragma once
#include <vector>
// Decompression/Compression by Zoinkity

class CNinDec
{
public:
	CNinDec(void);
	~CNinDec(void);
	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* outputBuffer);
	bool Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex);
	int Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength);
	int RFind(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength);
	std::vector<int> _search(unsigned char* data, int pos, int sz, int cap, int window, int minl);
	int encode(unsigned char* data, int dataSize, unsigned char* outputBuffer, int peek = 0, bool header = true);
	static unsigned long Flip32Bit(unsigned long inLong);
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
};
