//By Zoinkity!
#pragma once
#include <vector>

class CQuakeDecoder
{
public:
	CQuakeDecoder(void);
	~CQuakeDecoder(void);
	bool getbit(unsigned char* data, int& bitPosition);
	void update(std::vector<int>& tbl1, std::vector<int>& tbl2, std::vector<int>& tbl3, std::vector<int>& tbl4, int x, int y);
	int aridecode(unsigned char* data, int dataLength, unsigned char* output, int& outputPosition);
};
