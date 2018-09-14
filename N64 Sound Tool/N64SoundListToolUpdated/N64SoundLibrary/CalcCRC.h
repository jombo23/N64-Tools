// By Zoinkity
#pragma once
#include <vector>
#include <map>

class CalcCRC
{
public:
	CalcCRC(void);
	~CalcCRC(void);
	std::map<CString, unsigned char> cic_names;
	unsigned long PC;
	CString cic2seed(CString cic, unsigned char& cicValue);
	std::vector<unsigned long> calccrc(unsigned char* rom, unsigned long romSize, CString cic="mario", bool fix = false, unsigned long long seed = 0x00000000, unsigned long base=0x1000, unsigned long seel=0);
};
