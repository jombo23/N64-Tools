#pragma once
// By Zoinkity!
#include <vector>

class CNintendoEncoder
{
public:
	CNintendoEncoder(void);
	~CNintendoEncoder(void);

	std::vector<CString> Ya;
	std::vector<CString> MI;
	std::vector<CString> stream;
	std::vector<CString> block;
	std::vector<CString> valid;
	int decode(unsigned char* data, int dataSize, unsigned char* output);
	int encode(unsigned char* data, int dataSize, unsigned char* output, CString mode);
private:
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	std::vector<bool> _next_bit(std::vector<unsigned char> ctl);
	std::vector<unsigned short> _next_cpy(std::vector<unsigned char> back);
	int _dec(int dec_s, std::vector<unsigned char> ctl, std::vector<unsigned char> back, std::vector<unsigned char> vals, unsigned char* output, CString mode);
	int _z_from_tables(std::vector<unsigned char> ctl, std::vector<unsigned char> back, std::vector<unsigned char> values, unsigned char* output, int& outputPosition, int dec_s, CString mode);
	int _build_file(CString name, int dec_s, std::vector<unsigned char> ctl, std::vector<unsigned char> back, std::vector<unsigned char> values, unsigned char* output);
	void _z_to_tables(unsigned char* data, int dataSize, std::vector<unsigned char>& ctl, std::vector<unsigned char>& back, std::vector<unsigned char>& vals, CString mode);
	unsigned long Flip32Bit(unsigned long inLong);
	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	bool _from_header(unsigned char* data, int dataSize, CString& name, int& dec_s, std::vector<unsigned char>& ctl, std::vector<unsigned char>& back, std::vector<unsigned char>& values, int& cmp_s);
	bool Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex);
	int Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength);
	std::vector<int> _search(unsigned char* data, int pos, int sz, int cap);
	int Yax0r(unsigned char* data, int dataSize, unsigned char* output);
private:
};
