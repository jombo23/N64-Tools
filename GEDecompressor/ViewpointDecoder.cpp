#include "StdAfx.h"
#include "ViewpointDecoder.h"

CViewpointDecoder::CViewpointDecoder(void)
{
}

CViewpointDecoder::~CViewpointDecoder(void)
{
}

int CViewpointDecoder::dec(unsigned char* input, unsigned char* output, int& compressedSize, int decompressedSize)
{
	int inputPosition = 0;
	int outputPosition = 0;

    while (outputPosition < decompressedSize)
	{
        unsigned long v = input[inputPosition++];
        if (v == 0)
            output[outputPosition++] = input[inputPosition++];
        else
		{
            unsigned long i = (v & 0xF) << 8;
            i |= input[inputPosition++];
            v >>= 4;
            for (int x = 0; x < v; x++)
                output[outputPosition++] = output[outputPosition - i];
		}
	}

    return outputPosition; 
}

int CViewpointDecoder::Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
{
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
            return -1;

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
            // needle was found
            return index;
        }

        // continue to search for needle
        //sourceLength -= (index - startIndex + 1);
        startIndex = index + 1;
    }
    return -1;
}

// Compress
std::vector<int> CViewpointDecoder::_search(unsigned char* data, int pos, int sz, int cap)
{
	std::vector<int> returnData;
	returnData.resize(2);
    
    int ml = min(cap, sz - pos);

    if (ml < 2)
	{
		returnData[0] = 0;
		returnData[1] = 0;
		return returnData;
        //return 0, 0
	}

    int mp = max(0, pos - 0xFFF);
    int hitp = 0;
	int hitl = 1;

    if (mp < pos)
	{
		int hl = Find(&data[mp], &data[pos], hitl, 0, pos + hitl - mp);

        while (hl < (pos - mp))
		{
            while ((hitl < ml) && (data[pos + hitl] == data[mp + hl + hitl]))
			{
                hitl += 1;
			}
            mp += hl;
            hitp = mp;
            if (hitl == ml)
			{
				returnData[0] = hitp;
				returnData[1] = hitl;
                //return hitp, hitl
				return returnData;
			}
            mp += 1;
            hitl += 1;
            if (mp >= pos)
                break;
//##            hl = _seqfind(data[mp:pos+hitl], hitl)
            //hl = data[mp:pos+hitl].find(data[pos:pos+hitl])
			hl = Find(&data[mp], &data[pos], hitl, 0, pos + hitl - mp);
		}
	}
    //# If length less than 2, return miss.
    if (hitl < 2)
        hitl = 1;

    returnData[0] = hitp;
	returnData[1] = hitl-1;
    //return hitp, hitl-1
	return returnData;
}

int CViewpointDecoder::encode(unsigned char* data, int dataSize, unsigned char* output, bool lookahead)
{
    //"""Encodes a bytes-like object in given mode as a bytes object.
    //"""
    int cap = 0xF;
    int sz = dataSize;

    unsigned long pos = 0;

	unsigned long outputPosition = 0;
    while (pos < sz)
	{
        std::vector<int> searchReturn = _search(data, pos, sz, cap);

        int hitp = searchReturn[0];
		int hitl  = searchReturn[1];

        if (hitl < 2)
		{
            //# Push a raw if copying isn't possible.
            output[outputPosition++] = 0;
			output[outputPosition++] = data[pos];
            pos += 1;
		}
        else
		{
            if (lookahead)
			{
				searchReturn = _search(data, pos+1, sz, cap);

				int tstp = searchReturn[0];
				int tstl = searchReturn[1];

                if ((hitl + 1) < tstl)
				{
                    output[outputPosition++] = 0;
					output[outputPosition++] = data[pos];
                    pos += 1;
                    hitl = tstl;
                    hitp = tstp;
				}
			}
            int e = pos - hitp;
            pos += hitl;
            unsigned char v = (e >> 8) | (hitl << 4);
            output[outputPosition++] = v;
            output[outputPosition++] = e & 0xFF;
		}
	}
	return outputPosition;
}