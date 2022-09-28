// By Zoinkity

#pragma once
#include <vector>

class CViewpointDecoder
{
public:
	CViewpointDecoder(void);
	~CViewpointDecoder(void);
	int dec(unsigned char* input, unsigned char* output, int& compressedSize, int decompressedSize);
	int Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength);
	std::vector<int> _search(unsigned char* data, int pos, int sz, int cap);
	int encode(unsigned char* data, int dataSize, unsigned char* output, bool lookahead = true);
};
