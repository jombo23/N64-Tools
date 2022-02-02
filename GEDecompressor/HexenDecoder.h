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
	void init(int fill=0x20, int lookahead=128, int threshold=2);
	unsigned char src();
	void src(unsigned char* data);
	int next(int length);
	void _h_refill();
	void _h_update(int idx);
	int _h_chr();
	int _h_pos();
	static unsigned char d_code[256];
	static unsigned char d_len[256];
	static unsigned char p_len[0x40];
	static unsigned char p_code[0x40];
	int lookahead;
	int fill;
	int threshold;
	int _T;
	int _N_CHAR;
	int _R;
	std::vector<unsigned char> tbl;
	int tbl_p;
	int outa;
	int outb;
	int outc;
	void _write(unsigned char* output, int& outputPosition, int value, int count);
	void _flush(unsigned char* output, int& outputPosition);
	void _enc_chr(unsigned char* output, int& outputPosition, int idx);
	void _enc_pos(unsigned char* output, int& outputPosition, int idx);
	int m_len;
	int m_pos;
	void _find();
	int encode(unsigned char* input, int inputLength, unsigned char* output, int& outputPosition);
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
};
