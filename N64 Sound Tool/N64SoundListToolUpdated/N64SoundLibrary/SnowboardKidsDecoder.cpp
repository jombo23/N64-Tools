#include "StdAfx.h"
#include "SnowboardKidsDecoder.h"
#include <vector>

CSnowboardKidsDecoder::CSnowboardKidsDecoder(void)
{
}

CSnowboardKidsDecoder::~CSnowboardKidsDecoder(void)
{
}

unsigned short CSnowboardKidsDecoder::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CSnowboardKidsDecoder::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

//## Links children and parents.
void CSnowboardKidsDecoder::_linko(int idx, std::vector<int>& lnk, std::vector<signed short>& t1, std::vector<signed short>& p1, std::vector<signed short>& c1)
{
    lnk[2] += 1;
    int v = lnk[0];
    if (v < 0)
	{
        c1[idx] = lnk[1];
        p1[idx] = -1;
        lnk[0] = idx;
        lnk[1] = idx;
	}
    else
	{
        int i = t1[idx];
        while (v >= 0)
		{
            int j = t1[v];
            if (j < i)
                break;
            v = c1[v];
		}
        if (v < 0)
		{
            //# Linked to root.
            v = lnk[1];
            c1[v] = idx;
            c1[idx] = -1;
            p1[idx] = v;
            lnk[1] = idx;
		}
        else
		{
            c1[idx] = v;
            p1[idx] = p1[v];
            p1[v] = idx;
            v = p1[idx];
            if (v < 0)
                lnk[0] = idx;
            else
                c1[v] = idx;
		}
	}
}

void CSnowboardKidsDecoder::_freq(int idx, std::vector<int>& lnk, std::vector<signed short>& t1, std::vector<signed short>& p1, std::vector<signed short>& c1)
{
    if (lnk[0] < 0)
        return;
    lnk[2] -= 1;
    int i = p1[idx];
    int j = c1[idx];
    if (i < 0)
	{
        lnk[0] = j;
	}
    else
	{
        c1[i] = j;
        //# i = p1[idx]
	}
    if (j < 0)
        lnk[1] = i;
    else
        c1[j] = i;
}

int CSnowboardKidsDecoder::decode(unsigned char* data, unsigned char* output, int& outputPosition)
{
	outputPosition = 0;
    unsigned long dec_sz = CharArrayToLong(&data[0]);
	unsigned char* src = data;
	int sourcePosition = 4;

    int mode = src[sourcePosition++];
    //out = bytearray()

    //# Split the linked lists into tables.
	std::vector<signed short> c1;
	c1.resize(512);
    std::vector<signed short> p1;
	p1.resize(512);
	std::vector<signed short> t1;
	t1.resize(512);
	std::vector<signed short> t2;
	t2.resize(512);
	std::vector<signed short> t3;
	t3.resize(512);
	std::vector<signed short> t4;
	t4.resize(512);
	std::vector<int> lnk;
	lnk.push_back(-1);
	lnk.push_back(-1);
	lnk.push_back(0);
	
    int cnt = 0;

    //## Actual decompressor.
    int c = src[sourcePosition++];
    while (true)
	{
        //# Fill table at 80110928 (t1)
        int n = src[sourcePosition++];
        for (int i = c; i < (n+1); i++)
		{
            //## These next four lines may not be correct.
            t1[cnt] = src[sourcePosition++];
            t2[cnt] = -1;
            t3[cnt] = -1;
            t4[cnt] = i;
            _linko(cnt, lnk, t1, p1, c1);
            cnt += 1;
		}
        c = src[sourcePosition++];
        if (!c)
            break;
	}

    while (lnk[2] > 1)
	{
        int i = lnk[1];
        _freq(i, lnk, t1, p1, c1);
        int j = lnk[1];
        _freq(j, lnk, t1, p1, c1);
        t1[cnt] = t1[i] + t1[j];
        t2[cnt] = j;
        t3[cnt] = i;
        t4[cnt] = -1;
        _linko(cnt, lnk, t1, p1, c1);
        cnt += 1;
	}

    //# Assemble the output.
    //# Somewhat faster duplicating the bitflag stuff than putting tests on the else.
    c = cnt - 1;
    unsigned char m = 0;
	unsigned char j = 0;
   // # RLE
    if (!mode)
	{
        while (outputPosition < dec_sz)
		{
            int v = t4[c];
            if (v < 0)
			{
                if (m < 1)
				{
                    m = 0x80;
                    j = src[sourcePosition++];
				}
				if (m & j)
					c = t3[c];
				else
					c = t2[c];
                m >>= 1;
			}
            else
			{
                //# Pretty sure this will only ever be a byte, but safer!
				output[outputPosition++] = v & 0xFF;
                c = cnt - 1;
			}
		}
        return dec_sz;
	}

    //# LZ
    while (outputPosition < dec_sz)
	{
        int v = t4[c];
        while (v < 0)
		{
            if (m < 1)
			{
                m = 0x80;
                j = src[sourcePosition++];
			}
			if (m & j)
				c = t3[c];
			else
				c = t2[c];
            m >>= 1;
            v = t4[c];
		}
        //# Gets the next value (part of a pair).
        c = cnt - 1;
        int i = t4[c];
        while (i < 0)
		{
            if (m < 1)
			{
                m = 0x80;
                j = src[sourcePosition++];
			}
            if (m & j)
				c = t3[c];
			else
				c = t2[c];
            m >>= 1;
            i = t4[c];
		}
        //# Push a literal or copy a run.
        i &= 0xFF;
        if (!v)
			output[outputPosition++] = i;
        else
		{
            int k = v << 8;
            k |= i;
            k &= 0xFFF;
            v >>= 4;
            for (int i = 0; i < v; i++)
                output[outputPosition++] = output[outputPosition - k];
		}
        c = cnt - 1;
	}
    return dec_sz;
}