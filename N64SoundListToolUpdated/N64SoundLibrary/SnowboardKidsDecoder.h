#pragma once
#include <vector>
class CSnowboardKidsDecoder
{
public:
	CSnowboardKidsDecoder(void);
	~CSnowboardKidsDecoder(void);
	static void _linko(int idx, std::vector<int>& lnk, std::vector<signed short>& t1, std::vector<signed short>& p1, std::vector<signed short>& c1);
	static void _freq(int idx, std::vector<int>& lnk, std::vector<signed short>& t1, std::vector<signed short>& p1, std::vector<signed short>& c1);
	static int decode(unsigned char* data, unsigned char* output, int& outputPosition);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
};
