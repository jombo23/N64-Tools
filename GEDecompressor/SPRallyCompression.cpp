#include "StdAfx.h"
#include "SPRallyCompression.h"
#include <vector>
#include <algorithm>

CSPRallyCompression::CSPRallyCompression(void)
{
}

CSPRallyCompression::~CSPRallyCompression(void)
{
}

//# Variable length LZ.
//# This looks *awfully* familiar...  What type was it?

std::vector<int> _c(unsigned char* input, int& inputPosition)
{
    //"""Grabs next control code from iterator.
    //Returns (value, flag)"""
    int v = input[inputPosition++];
    int k = ((v & 0x40) != 0);
    int j = 6;
    int i = v & 0x3F;
    while (v & 0x80)
	{
        v = input[inputPosition++];
        i |= (v & 0x7F) << j;
        j += 7;
	}
	std::vector<int> returnData;
	returnData.push_back(i);
	returnData.push_back(k);
    return returnData;
}

int _v(unsigned char* input, int& inputPosition)
{
    //"""Grabs next variable-length value from iterator.
    //Returns value"""
    int i = 0;
	int j = 0;
    while (true)
	{
        int v = input[inputPosition++];
        i |= (v & 0x7F) << j;
        j += 7;
        if (!(v & 0x80))
			return i;
	}
}

int CSPRallyCompression::decode(unsigned char* data, int dec_sz, unsigned char* out)
{
	int outputPosition = 0;
	unsigned char* src;
	int cmp_sz = 0;

	int inputPosition = 0;
	src = data;

    //# Read sizes from header.
    if (dec_sz == -1)
	{
		dec_sz = (data[0] << 8) | data[1];
        cmp_sz = (data[2] << 8) | data[3];
		inputPosition = 4;
	}
    
    //# LZ algo
    while (dec_sz > 0)
	{
        std::vector<int> cdata = _c(src, inputPosition);
		int i = cdata[0];
		int j = cdata[1];
        if (j)
		{
            int k = _v(src, inputPosition);
            for (int x = 0; x < i; x++)
               out[outputPosition++] = out[outputPosition-k];
		}
        else
		{
            for (int x = 0; x < i; x++)
                out[outputPosition++] = src[inputPosition++];
		}
        dec_sz -= i;
	}
    return outputPosition;
}

int SPRallyRFind(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
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

std::vector<unsigned char> _ec(unsigned long v, bool copy)
{
    std::vector<unsigned char> out;
    int i = copy << 6; //   # 0x40 if copy
    i |= (v & 0x3F);
    v >>= 6;
    while (v)
	{
        out.push_back(i | 0x80);
        i = v & 0x7F;
        v >>= 7;
	}
    out.push_back(i);
    return out;
}

std::vector<unsigned char> _ev(unsigned long v)
{
	std::vector<unsigned char> out;
    
    int i = v & 0x7F;
    while (v = v >> 7)
	{
        out.push_back(i | 0x80);
        i = v & 0x7F;
	}
     out.push_back(i);
    return out;
}

std::vector<int> _f(unsigned char* data, int pos, int inputSize)
{
    int q = 3;
    int p = -1;
	int k = -1;
    while ((pos + q) < inputSize)
	{
        //# Nearest is smallest.
		//int RFind(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)

        int i = SPRallyRFind(data, &data[pos], q, 0, pos + q - 1);
        if (i < 0)
			break;
        //# Don't keep if encoded length grows filesize.
        int j = _ec(q, true).size();
        j += _ev(pos - i).size();
        if (j < q)
		{
            p = pos - i;
            k = q;
		}
        //# Keep trying either way.  Longer compresses better.
        q += 1;
	}
	std::vector<int> returnData;
	returnData.push_back(p);
	returnData.push_back(k);
    return returnData;
}

int CSPRallyCompression::encode(unsigned char* data, int inputSize, unsigned char* out)
{
	int outputPosition = 0;
    int l = inputSize;
    //# There will always be at least one literal pushed.
    int i = 0;
	int j = 1;
	
    int pos = 1;
    while (pos < l)
	{
		std::vector<int> fdata = _f(data, pos, inputSize);
        int p = fdata[0];
		int q = fdata[1];
			
        if (p > 0)
		{
            //# Push any outstanding literals first.
            if (j)
			{
				std::vector<unsigned char> returnData;
				returnData = _ec(j, false);
				for (int x = 0; x < returnData.size(); x++)
				{
					out[outputPosition++] = returnData[x];
				}
				for (int x = i; x < i+j; x++)
				{
					out[outputPosition++] = data[x];
				}
                j = 0;
			}
            //# Push copy command.
			std::vector<unsigned char> returnData;
            returnData = _ec(q, true);
			for (int x = 0; x < returnData.size(); x++)
			{
				out[outputPosition++] = returnData[x];
			}

            returnData = _ev(p);
			for (int x = 0; x < returnData.size(); x++)
			{
				out[outputPosition++] = returnData[x];
			}
            pos += q;
		}
        else
		{
            if (j == 0)
				i = pos;
            j += 1;
            pos += 1;
		}
	}
    //# Push out any remaining literals.
    if (j)
	{
		std::vector<unsigned char> returnData;
        returnData = _ec(j, false);
		for (int x = 0; x < returnData.size(); x++)
		{
			out[outputPosition++] = returnData[x];
		}

		for (int x = i; x < i+j; x++)
		{
			out[outputPosition++] = data[x];
		}
	}
    return outputPosition;
}