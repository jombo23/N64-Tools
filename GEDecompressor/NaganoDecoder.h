#pragma once
#include <vector>

// By Zoinkity!!!
class CNaganoDecoder
{
public:
	CNaganoDecoder(void);
	~CNaganoDecoder(void);

	void samelength(int p, int& length, int& value, unsigned char* data, int sz);
	bool Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex);
	int Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength);
	void copylength(int p, int& length, int& position, unsigned char* data, int sz);
	std::vector<int> modella(int start, int length, int enclen, unsigned char* data, int sz);
	void flushpush(int l, int p, unsigned char* data, unsigned char* out, int& outputPosition);
	void flushcopy(int l, int p, unsigned char* out, int& outputPosition);
	void flushsame(int l, int c, unsigned char* out, int& outputPosition);

	int dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output);
	void KonamiLZW(unsigned char* inputBuffer, int compressedSizeSection, int& inputPosition, unsigned char* output, int& outputPosition);

	int EncodeKonamiLZW(unsigned char* inputBuffer, int uncompressedSize, unsigned char* outputBuffer, int outputBufferSize, int& compressedSize, int& compressedRealSize, int level);
};
