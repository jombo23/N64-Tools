#include "StdAfx.h"
#include "CommandAndConquerDecoder.h"
#include <vector>

CommandAndConquerDecoder::CommandAndConquerDecoder(void)
{
}

CommandAndConquerDecoder::~CommandAndConquerDecoder(void)
{
}

#define NONE -1

struct tuple
{
	unsigned char* data;
	int length;
	int value;

	tuple()
	{
		data = NULL;
	}

	tuple(unsigned char* dataIn, int lengthIn, int valueIn)
	{
		length = lengthIn;
		data = dataIn;
		value = valueIn;
	}
};


int hasho(unsigned char* b, int& w)
{
    unsigned long v = (b[0] << 8) ^ (b[1] << 4) ^ b[2];
    v *= 0x9E5F;
    if (w == NONE)
	{
        v >>= 4;
        v &= 0xFFF;
	}
    else
	{
        v >>= 1;
        v &= 0xFF8;
        v += w;
        w += 1;
        w &= 7;
	}
    return v;
}

int CommandAndConquerDecoder::dec(unsigned char* input, unsigned char* output, int& compressedSize, int kind)
{
	int inputPosition = 0;
	int outputPosition = 0;

	//"""Decompresses bytes object [input] returning a bytes object.
    //The [kind] argument selects the dictionary fill method.
    //Three kinds are supported:
    //    0   store (no decompression occurs)
    //    1   type 11 files
    //    2   type 22 files
    
    if (kind == 0)
	{
		memcpy(output, input, compressedSize);
		return compressedSize;
	}

    if (input[0] == 1)
	{
		memcpy(output, input, compressedSize - 4);
		return compressedSize - 4;
	}

	unsigned char defaultChar[] = { 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38 };
    //default = b"123456789012345678"

	tuple buf[0x1000];
	for (int x = 0; x < 0x1000; x++)
	{
		buf[x] = tuple(defaultChar, 18, 0);
	}

	int count = 0;
	unsigned long bits = 1;
	inputPosition = 4;
    
    int wrap = 0;
	if (kind == 2)
		wrap = 0;
	else 
		wrap = NONE;

    while (inputPosition < compressedSize)
	{
        if (bits == 1)
		{
            bits = 0x10000 | input[inputPosition];
            bits |= input[inputPosition + 1] << 8;
            inputPosition += 2;
		}
        if (bits & 1)
		{
            unsigned char l = input[inputPosition];
            unsigned long idx = (l << 4) & 0xF00;
            idx |= input[inputPosition + 1];
            inputPosition += 2;
            l &= 0xF;
            l += 3;
			unsigned char* src = buf[idx].data;
            int p = buf[idx].value;

            int org = outputPosition;
            for (int i = 0; i < l; i++)
			{
				output[outputPosition++] = src[p + i];
			}

            if (count)
			{
                unsigned long o = org - count;
                p = hasho(&output[o], wrap);
                buf[p] = tuple(&output[0], outputPosition, o);
                if (count == 2)
				{
                    p = hasho(&output[o+1], wrap);
                    buf[p] = tuple(&output[0], outputPosition, o+1);
				}
                count = 0;
			}

            if (wrap == NONE)
			{
                idx &= 0xFFF;
			}
            else
			{
                idx &= 0xFF8;
                idx += wrap;
                wrap += 1;
                wrap &= 7;
			}
            buf[idx] = tuple(&output[0], outputPosition, org);
		}
        else
		{
            output[outputPosition++] = input[inputPosition];
            inputPosition += 1;
            count += 1;
            if (count == 3)
			{
                count = 2;
                int org = outputPosition - 3;
                int p = hasho(&output[org], wrap);
                buf[p] = tuple(&output[0], outputPosition, org);
			}
		}
        bits >>= 1;
	}

	return outputPosition;
}

unsigned long Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned long CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}


std::vector<int> _hasho(unsigned char b[4], int w)
{
    unsigned long v = (b[0] << 8) ^ (b[1] << 4) ^ b[2];
    v *= 0x9E5F;
    if (w == NONE)
	{
        v >>= 4;
        v &= 0xFFF;
	}
    else
	{
        v >>= 1;
        v &= 0xFF8;
        v += w;
        w += 1;
        w &= 7;
	}
	std::vector<int> returnHash;
	returnHash.resize(2);
	returnHash[0] = v;
	returnHash[1] = w;
	return returnHash;
}

bool Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex)
{
    // compare
    for (int i = 0, p = startIndex; i < needleLen; i++, p++)
    {
        if (array[p] != needle[i])
        {
            return false;
        }
    }
    return true;
}


std::vector<int> _search(unsigned char* data, std::vector<int> hashes, int pos, int sz, int wrap)
{
	std::vector<int> returnData;
	returnData.resize(2);

    //# TODO: combine these?  (or not...)
    int ml = min(18, sz - pos);
    if (ml < 4)
	{
		returnData[0] = 0;
		returnData[1] = 0;
        return returnData;
	}
    //# Annoyingly only can look back to specific entries.  Stupidly inefficient.
	int hitl;
	int h;
	int hitp;

    if (wrap == NONE)
	{
        //# Type 1 is a simple table lookup.
        hitl = 3;
        h = _hasho(&data[pos], wrap)[0];
        hitp = hashes[h];
        //# Test if accidental collision.
        if ((hitp < 0) || (Compare(&data[pos], &data[hitp], hitl, 0) == false))
		{
            hitl = 0;
		}
        //# Determine length.
        else
		{
            while ((hitl < ml) && (data[pos + hitl] == data[hitp + hitl]))
                hitl += 1;
		}
	}
    else
	{
        //# Type 2 is really a 0x200 table with up to 8 entries for each index.
        //#   In this case, iterate _hasho w/ wrap=range(0,8) to get all possible hitp
        //#   then find the best possible match from list.
        int hh = _hasho(&data[pos], 0)[0]; //   # base hash
        hitl = 2;
		h = -1; //     # first real hit sets actual return, h
        for (int i = 0; i < 8; i++)
		{
            hitp = hashes[hh+i];
            if (hitp < 0)
			{
                continue;
			}

            //# Test if accidental collision.
            if ((Compare(&data[pos], &data[hitp], hitl, 0)) == false)
			{
                continue;
			}

            //# Determine length.
            while ((hitl < ml) && (data[pos + hitl] == data[hitp + hitl]))
			{
                hitl += 1;
                h = hh + i; //  # this updates the hash for the longest hit
			}
		}
        if (h < 0)
		{
            hitl = 0;
		}
	}

	// Put back later
	//_default = b"123456789012345678"

    //# See if default string would make a better hit.
    /*if ((data[pos:pos+3] == _default[:3]) and -1 in hashes)
	{
        int tstl = 3;
        while ((tstl < ml) and (data[pos + tstl] == _default[tstl]))
		{
            tstl += 1;
		}
        if (tstl > hitl)
            return hashes.index(-1), tstl
	}*/

	returnData[0] = h;
	returnData[1] = hitl;
	return returnData;
}

int CommandAndConquerDecoder::encode(unsigned char* data, int dataSize, unsigned char* output, int kind = 1)
{
    //"""Do things here.  TODO: fix type 2 to be efficient.  Type 1 okay.
    //Valid kinds are:
        //0   store (which just returns data)
        //1   type 11
        //2   type 22
    //Should only require a minor modification to support type22, but meh."""
	int wrap;

    if (kind == 0)
	{
        memcpy(output, data, dataSize);
		return dataSize;
	}
    else if (kind == 1)
	{
        wrap = NONE;
	}
    else if (kind == 2)
	{
        wrap = 0;
	}
    else
	{
        //raise ValueError('"kind" must be 0, 1, or 2.')
		return 0;
	}

    int l = dataSize;
    int ll = l - 3;
    if (ll < 0)
	{
        //# raise UserError("Why are you trying to compress a file with so little data?")
		for (int r = 0; r < 6; r++)
		{
			output[r] = 0;
		}
		return 6;
	}

	int outputPosition = 0;
	output[outputPosition++] = 0x00;
	output[outputPosition++] = 0x00;
	output[outputPosition++] = 0x00;
	output[outputPosition++] = 0x00;

	std::vector<unsigned char> buf;

	std::vector<int> returnData;

    //# Generate a table for hashes.  Yes, it can only use hashes it initializes.  This is dumb.
    //# TODO: Really should init. to default string, but not terribly likely will need to.
	std::vector<int> hashes;
	hashes.resize(0x1000);
	for (int x = 0; x < 0x1000; x++)
	{
		hashes[x] = -1;
	}

    int pos = 0;
	int count = 0;
	unsigned long f = 1;
	unsigned long fl = 0;

    while (pos < l)
	{
        if (pos == 0x1E)
		{
            pos = pos;
		}

        if (f & 0x10000)
		{
            //# Flush.
			output[outputPosition++] = (fl & 0xFF);
            output[outputPosition++] = (fl >> 8);

			for (int x = 0; x < buf.size(); x++)
			{
				output[outputPosition++] = buf[x];
			}
            buf.clear();
            f = 1;
			fl = 0;
		}

		int hitp;
		int hitl;
		int h;

		returnData = _search(data, hashes, pos, l, wrap);
		hitp = returnData[0];
		hitl = returnData[1];

        if (hitl < 3)
		{
            buf.push_back(data[pos]);

            pos += 1;
            count += 1;
            if (count == 3)
			{
                count = 2;
				
                returnData = _hasho(&data[pos-3], wrap);
				h = returnData[0];
				wrap = returnData[1];

                hashes[h] = pos - 3;
			}
		}
        else
		{
            //# Prognostication!
            returnData = _search(data, hashes, pos+1, l, wrap);
			int tstp = returnData[0];
			int tstl = returnData[1];

            if ((hitl + 1) < tstl)
			{
                buf.push_back(data[pos]);
                pos += 1;
                count += 1;
                if (count == 3)
				{
                    count = 2;
                    returnData = _hasho(&data[pos-3], wrap);
					h = returnData[0];
					wrap = returnData[1];

                    hashes[h] = pos - 3;
				}
                f <<= 1;
                if (f & 0x10000)
				{
                    //# Flush.
                    output[outputPosition++] = (fl & 0xFF);
                    output[outputPosition++] = (fl >> 8);
                    
					for (int x = 0; x < buf.size(); x++)
					{
						output[outputPosition++] = buf[x];
					}
					buf.clear();

                    f = 1;
					fl = 0;
				}
                hitl = tstl;
                hitp = tstp;
			}

            //# Update hash table.
            if (count)
			{
                int v = pos - count;
                returnData = _hasho(&data[v], wrap);
				h = returnData[0];
				wrap = returnData[1];

                hashes[h] = v;
                if (count == 2)
				{
                    returnData = _hasho(&data[v+1], wrap);
					h = returnData[0];
					wrap = returnData[1];
                    hashes[h] = v+1;
				}
                count = 0;
			}
            returnData = _hasho(&data[pos], wrap);
			h = returnData[0];
			wrap = returnData[1];
            hashes[h] = pos;
			count = 0;

            //# Encode.
            fl |= f;
            pos += hitl;
            hitl -= 3;
            hitl |= (hitp >> 4) & 0xF0;
            buf.push_back(hitl);
            buf.push_back(hitp & 0xFF);
		}
		f <<= 1;
	}

	//# Flush any remaining data
    if (f != 1)
	{
        output[outputPosition++] = (fl & 0xFF);
        output[outputPosition++] = (fl >> 8);
        
		for (int x = 0; x < buf.size(); x++)
		{
			output[outputPosition++] = buf[x];
		}
		buf.clear();
	}

	return outputPosition;
}