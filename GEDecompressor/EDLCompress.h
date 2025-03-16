// BY ZOINKITY
#pragma once
#include <vector>

struct HeaderData
{
	unsigned char data_t;
	CString endian;
	unsigned long cmp_s;
	unsigned long dec_s;

	HeaderData()
	{

	}

	HeaderData(unsigned char data_t, CString endian, unsigned long cmp_s, unsigned long dec_s)
	{
		this->data_t = data_t;
		this->endian = endian;
		this->cmp_s = cmp_s;
		this->dec_s = dec_s;
	}
};

struct EDLPair
{
	unsigned short cnt;
	unsigned short value;
	EDLPair()
	{
		cnt = 0;
		value = 0;
	}

	EDLPair(unsigned short cnt, unsigned short value)
	{
		this->cnt = cnt;
		this->value = value;
	}
};

class CEDLCompress
{
public:
	int decode(unsigned char* data, int& compressedSize, unsigned char* outputDecompressed);
	void encode(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, int mode, CString endian);
	void nibbleswap(unsigned char* data, int dataLength);
	static unsigned long Flip32Bit(unsigned long inLong);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	HeaderData header(unsigned char* data);
private:
	std::vector<unsigned long> _grab(unsigned char* data, int dataSize, CString endian="big", bool padZero = false);
	unsigned long next(std::vector<unsigned long>& src, int bits, CString mode, unsigned __int64& v, unsigned __int64& b, unsigned long& c);
	int _decEDL1(CString endian, unsigned char* data, int cmp_s, int dec_s, unsigned char* outputDecompressed);
	int _decEDL2(CString endian, unsigned char* data, int cmp_s, int dec_s, unsigned char* outputDecompressed);
	bool Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex);
	int Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength);
	int RFind(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength);
	std::vector<int> _search(unsigned char* data, int pos, int sz, int cap = 0x108);
	void _encEDL0(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, CString endian);
	void _encEDL1(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, CString endian);
	void _encEDL2(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, CString endian);
	std::vector<unsigned long> _push(CString endian, std::vector<EDLPair> edlPairs);
};
