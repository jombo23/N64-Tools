#include "StdAfx.h"
#include "HexenDecoder.h"
#include <vector>

CHexenDecoder::CHexenDecoder(void)
{
}

CHexenDecoder::~CHexenDecoder(void)
{
}

unsigned short CHexenDecoder::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CHexenDecoder::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}
unsigned char CHexenDecoder::d_code[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
    0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
    0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
    0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D,
    0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
    0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11,
    0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
    0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15,
    0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
    0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B,
    0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
    0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23,
    0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
    0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B,
    0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F};

	unsigned char CHexenDecoder::d_len[256] = {
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};

	//# tables for encoding
	unsigned char CHexenDecoder::p_len[0x40] = {
    	0x03, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05,
    	0x05, 0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06,
    	0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
    	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    	0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
	};

	unsigned char CHexenDecoder::p_code[0x40] = {
    	0x00, 0x20, 0x30, 0x40, 0x50, 0x58, 0x60, 0x68,
    	0x70, 0x78, 0x80, 0x88, 0x90, 0x94, 0x98, 0x9C,
    	0xA0, 0xA4, 0xA8, 0xAC, 0xB0, 0xB4, 0xB8, 0xBC,
    	0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
    	0xD0, 0xD2, 0xD4, 0xD6, 0xD8, 0xDA, 0xDC, 0xDE,
    	0xE0, 0xE2, 0xE4, 0xE6, 0xE8, 0xEA, 0xEC, 0xEE,
    	0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7,
    	0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
	};

    void CHexenDecoder::init(int fill, int lookahead, int threshold)
	{
		this->outa = 0;
		this->outb = 0;
		this->outc = 0;

		this->lookahead = lookahead;
        this->threshold = threshold;
		this->fill = fill;
		this->_N_CHAR = 256 + lookahead - threshold;   //# 0x17E
        this->_T = (_N_CHAR << 1) - 1;            //# 0x2FB
        this->_R = _T - 1;                        //# 0x2FA
        //// Initialize each list w/ zeroes before filling.
		freq.resize(_T + 1);
		for (int x = 0; x < _T + 1; x++)
			freq[x] = 0;

        prnt.resize(_T + _N_CHAR);
		for (int x = 0; x < _T + _N_CHAR; x++)
			prnt[x] = 0;

        son.resize(_T);
		for (int x = 0; x < _T; x++)
			son[x] = 0;

        for (int i = 0; i < _N_CHAR; i++)
		{
            freq[i] = 1;
            prnt[i+_T] = i;
            son[i] = _T + i;
		}
        for (int i = _N_CHAR; i < _T; i++)
		{
            int j = (i - _N_CHAR) << 1;
            int k = _N_CHAR + j;
            freq[i] = freq[j] + freq[j+1];
            prnt[j] = i;
            prnt[j+1] = i;
            son[i] = j;
		}
        prnt[_R] = 0;
        freq[_T] = -1;
        //# Set up ring.  Only first 0xF80 need to be set to fill value.
        tbl.resize(0x1000);
		for (int x = 0; x < 0x1000; x++)
			tbl[x] = fill;
        tbl_p = 0x1000 - lookahead;
        //// Data stream things for decoding.
        data = NULL;
        bitc = 0;
	}

    //// Little bit of magic.  self.src returns current byte, self.src = data sets datastream
    //@property
    unsigned char CHexenDecoder::src()
	{
        return data[0];
	}

    void CHexenDecoder::src(unsigned char* data)
	{
        this->data = data;
        this->bitc = 0;
	}

    int CHexenDecoder::next(int length)
	{
        // A...noniterable iterable?
        int i = (1 << length) - 1;
        int j = src();
        if ((bitc + length) >= 8)
		{
			data++;
            j |= src() << 8;
		}
        j >>= bitc;
        bitc += length;
        bitc &= 7;
        return j & i;
	}

    void CHexenDecoder::_h_refill()
	{
        int j = 0;
        for (int i = 0; i < 0x2FB; i++)  // collect leaves in first half
		{
            int v = son[i];
            if (v < 0x2FB)
				continue;
            int k = freq[i] + 1;
            if (k < 0)
				k += 1;
            freq[j] = k >> 1;
            son[j] = v;
            j += 1;
		}
        int i = 0;
        for (int j = 0x17E; j < 0x2FB; j++) // connect sons
		{
            int k = (freq[i] + freq[i + 1]) & 0xFFFF;
            freq[j] = k;
            int v = j - 1;
            while (k < freq[v])
				v -= 1;
            v += 1;
			//freq[v+1:j+1] = freq[v:j];
			std::vector<int> tempBuffer;
			for (int x = v; x < j; x++)
				tempBuffer.push_back(freq[x]);
			for (int x = v+1; x < j+1; x++)
			{
				freq[x] = tempBuffer[0];
				tempBuffer.erase(tempBuffer.begin());
			}
            freq[v] = k;
			//son[v+1:j+1] = son[v:j];
			tempBuffer.clear();
			for (int x = v; x < j; x++)
				tempBuffer.push_back(son[x]);
			for (int x = v+1; x < j+1; x++)
			{
				son[x] = tempBuffer[0];
				tempBuffer.erase(tempBuffer.begin());
			}
            son[v] = i;
            i += 2;
		}
        for (int i = 0; i < 0x2FB; i++)  // connect parents
		{
            int v = son[i];
            prnt[v] = i;
            if (v < 0x2FB)
                prnt[v+1] = i;
		}
	}

    void CHexenDecoder::_h_update(int idx)
	{
        if (freq[0x2FA] == 0x8000)  // When ceiling met, reconstitute at max frequency.
            _h_refill();
        int i = prnt[0x2FB + idx];
        while (true)
		{
            freq[i] += 1;
            int j = i + 1;
            int q = freq[i];
            if ((0 <= freq[j]) && (freq[j] < q))    // Not a mistake.
			{
                while ((0 <= freq[j + 1]) && (freq[j + 1] < q))
					j += 1;
                int v = son[i];
                freq[i] = freq[j];
                freq[j] = q;
                prnt[v] = j;
                if (v < 0x2FB)
					prnt[v + 1] = j;
                int k = son[j];
                son[j] = v;
                prnt[k] = i;
                if (k < 0x2FB)
					prnt[k + 1] = i;
                son[i] = k;
			}
            else
			{
                j = i;
			}
            i = prnt[j];
            if (i == 0)
				break;
		}
	}

    int CHexenDecoder::_h_chr()
	{
        int i = son[0x2FA];
        while (i < 0x2FB)
		{
            i += next(1);
            i = son[i & 0xFFFF];
		}
        i -= 0x2FB;
        _h_update(i);
        return i;
	}

    int CHexenDecoder::_h_pos()
	{
        int j = next(8);
        int q = d_len[j] - 2;
        int k = (d_code[j] << 6) & 0xFFFF;
        int v = next(q);
        v += j << q;
        k += v & 0x3F;
        return k;
	}

    // Defined in a weird way so you can just type lzhuff.decode(yada yada)
	int CHexenDecoder::decode(unsigned char* input, unsigned char* output, int length)
	{
		int outputPosition = 0;
        int p = 0;
        // If length not given, read from start of file.
        if (length < 0)
		{
			length = CharArrayToLong(&input[0]);
            p = 4;
		}

		init();
        this->data = &input[p];

        int l_adj = 0xFF - threshold;   //# 0xFD
        while (outputPosition < length)
		{
            int i = _h_chr();
            if (i < 0x100)
			{
                output[outputPosition++] = i;
                tbl[tbl_p] = i;
                tbl_p += 1;
                tbl_p &= 0xFFF;
			}
			else
			{
                int j = _h_pos();
                int p = (tbl_p - j - 1) & 0xFFF;
                i -= l_adj; //# len + 1 + threshold
                for (int x = 0; x < i; x++)
				{
                    output[outputPosition++] = tbl[p];
                    tbl[tbl_p] = tbl[p];
                    p += 1;
                    p &= 0xFFF;
                    tbl_p += 1;
                    tbl_p &= 0xFFF;
				}
			}
		}

		tbl.clear();

		return outputPosition;
	}

	void CHexenDecoder::_write(unsigned char* output, int& outputPosition, int value, int count)
	{
        value &= (1 << count) - 1; //   # sanitize
        outb |= (value << outc);
        outc += count;
        while (outc > 7)
		{
            output[outputPosition++] = outb & 0xFF;
            outb >>= 8;
            outc -= 8;
		}
	}

    void CHexenDecoder::_flush(unsigned char* output, int& outputPosition)
	{
        //"""Flushes remaining output to self.out, resetting buffer value & count.
        //Returns a bytes copy of output."""
        if (outc) 
			output[outputPosition++] = outb;
        outb = 0;
        outc = 0;
	}

	void CHexenDecoder::_enc_chr(unsigned char* output, int& outputPosition, int idx)
	{
        int i = prnt[_T + idx];
        int j = 0;
		int k = 0;
        while (true)
		{
            j <<= 1;
            j |= (i & 1);
            k += 1;
            i = prnt[i];
            if (i == _R)
				break;
		}
        _write(output, outputPosition, j, k);
        _h_update(idx);
	}

    void CHexenDecoder::_enc_pos(unsigned char* output, int& outputPosition, int idx)
	{
        int v = idx >> 6;
        int i = 8 - p_len[v];
        int j = (idx & 0x3F) << (i + 2);
        j += p_code[v] << 8;
        _write(output, outputPosition, j >> 8, 8);
        _write(output, outputPosition, idx & 0x3F, 6 - i);
	}

	int RFind(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
	{
		int resultIndex = -1;
		int index;

		while (startIndex < (sourceLength - needleLen + 1))
		{
			// find needle's starting element
			int index = -1;
			for (int r = startIndex; r < (sourceLength - needleLen + 1); r++)
			{
				if (array[r] == needle[0])
				{
					index = r;
					break;
				}
			}

			// if we did not find even the first element of the needle, then the search is failed
			if (index == -1)
				return resultIndex;

			int i, p;
			// check for needle
			for (i = 0, p = index; i < needleLen; i++, p++)
			{
				if (array[p] != needle[i])
				{
					break;
				}
			}

			if (i == needleLen)
			{
				resultIndex = index;
				// needle was found
			}

			// continue to search for needle
			//sourceLength -= (index - startIndex + 1);
			startIndex = index + 1;
		}
		return resultIndex;
	}

	int RFind(std::vector<unsigned char> array, std::vector<unsigned char> needle, int needleLen, int startIndex, int sourceLength)
	{
		unsigned char* arrayBytes = new unsigned char[sourceLength];
		unsigned char* needleBytes = new unsigned char[needleLen];

		for (int x = 0; x < sourceLength; x++)
			arrayBytes[x] = array[x];

		for (int x = 0; x < needleLen; x++)
			needleBytes[x] = needle[x];

		return RFind(arrayBytes, needleBytes, needleLen, startIndex, sourceLength);

		delete [] arrayBytes;
		delete [] needleBytes;
	}

	int Count(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
	{
		int totalCount = 0;
		int index;

		while (startIndex < (sourceLength - needleLen + 1))
		{
			// find needle's starting element
			int index = -1;
			for (int r = startIndex; r < (sourceLength - needleLen + 1); r++)
			{
				if (array[r] == needle[0])
				{
					index = r;
					break;
				}
			}

			// if we did not find even the first element of the needle, then the search is failed
			if (index == -1)
				return totalCount;

			int i, p;
			// check for needle
			for (i = 0, p = index; i < needleLen; i++, p++)
			{
				if (array[p] != needle[i])
				{
					break;
				}
			}

			if (i == needleLen)
			{
				totalCount++;
				// needle was found
			}

			// continue to search for needle
			//sourceLength -= (index - startIndex + 1);
			startIndex = index + 1;
		}
		return totalCount;
	}

	int Count(std::vector<unsigned char> array, std::vector<unsigned char> needle, int needleLen, int startIndex, int sourceLength)
	{
		unsigned char* arrayBytes = new unsigned char[sourceLength];
		unsigned char* needleBytes = new unsigned char[needleLen];

		for (int x = 0; x < sourceLength; x++)
			arrayBytes[x] = array[x];

		for (int x = 0; x < needleLen; x++)
			needleBytes[x] = needle[x];

		return Count(arrayBytes, needleBytes, needleLen, startIndex, sourceLength);

		delete [] arrayBytes;
		delete [] needleBytes;
	}

	void CHexenDecoder::_find()
	{
        m_len = 1;
        int p = tbl_p;
        int floorvalue = max(0, tbl_p - 0x1000);
        int ceilvalue = min(p + lookahead, tbl.size());
        int q = ceilvalue - p;
        if (q <= threshold)
			return;

		std::vector<int> samp;
		samp.resize(ceilvalue - p);
		for (int x = p; x < ceilvalue; x++)
			samp[x - p] = tbl[x];
        //samp = tbl[p:ceil]; //# Just slice this subsample.
        for (int i = threshold + 1; i < q; i++)
		{
			std::vector<unsigned char> subSamp;
			subSamp.resize(i);
			for (int x = 0; x < i; x++)
				subSamp[x] = samp[x];
			int v = Count(tbl, subSamp, (int)subSamp.size(), floorvalue, p+i);
            if (v == 1)
				break;
            int j = RFind(tbl, subSamp, subSamp.size(), floorvalue, p);
			if (j != -1)
			{
				m_len = i;
				m_pos = p - j - 1;
			}
		}
	}

	void CHexenDecoder::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
	{
		Buffer[address] = ((data >> 24) & 0xFF);
		Buffer[address+1] = ((data >> 16) & 0xFF);
		Buffer[address+2] = ((data >> 8) & 0xFF);
		Buffer[address+3] = ((data) & 0xFF);
	}

	int CHexenDecoder::encode(unsigned char* input, int inputLength, unsigned char* output, int& outputPosition)
	{
		init();
		data = input;

		outputPosition = 0;
		WriteLongToBuffer(output, outputPosition, inputLength);
		outputPosition += 4;
        
        //if not data:  return huff._flush()
        int l_adj = 0xFF - threshold;   //# 0xFD
        int limit = lookahead - 1;      //# 0x7F
        //# Set up ring and extend it slightly.
		tbl.resize(tbl_p + inputLength);
		for (int x = 0; x < inputLength; x++) //tbl[tbl_p:] = data
			tbl[x + tbl_p] = data[x];
        
        int k = tbl.size();
        //# Compress data.
        while (tbl_p < k)
		{
            _find();
            if (m_len <= threshold)
			{
                m_len = 1;
                _enc_chr(output, outputPosition, tbl[tbl_p]);
			}
            else
			{
                _enc_chr(output, outputPosition, m_len + l_adj);
                _enc_pos(output, outputPosition, m_pos);
			}
            tbl_p += m_len;
		}
        output[outputPosition++] = outb;

		tbl.clear();
        return outputPosition;
	}