// By Zoinkity
#pragma once
#include <vector>

class CHexenDecoder
{
public:
	CHexenDecoder(void);
	~CHexenDecoder(void);

	int decode(unsigned char* input, unsigned char* output, int length = -1);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned char* data;
    int bitc;
	std::vector<int> freq;
	std::vector<int> prnt;
	std::vector<int> son;
	void init();
	unsigned char src();
	void src(unsigned char* data);
	int next(int length);
	void _h_refill();
	void _h_update(int idx);
	int _h_chr();
	int _h_pos();
	static unsigned char d_code[256];
	static unsigned char d_len[256];
};
