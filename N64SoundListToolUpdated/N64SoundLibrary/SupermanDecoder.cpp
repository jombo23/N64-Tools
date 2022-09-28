// BY ZOINKITY
#include "StdAfx.h"
#include "SupermanDecoder.h"

SupermanDecoder::SupermanDecoder(void)
{
}

SupermanDecoder::~SupermanDecoder(void)
{
}


int SupermanDecoder::header(unsigned char* data, int& compressedSize)
{
    //"""Retrieves offset from header.
    //Honestly note certain what the other data is; probably uses 1-2 byte codes to identify them.
    //Returns offset to data, which is 11 when compressed and 0 otherwise."""
    unsigned long val = ((((((data[4] << 8) | data[5]) << 8) | data[6]) << 8) | data[7]);
    if (val==0x9D89646C)
	{
        compressedSize = ((data[0x9] << 16) | (data[0xB] << 8) | data[0xA]);
        int dec_s = ((data[0xE] << 16) | (data[0x10] << 8) | data[0xF]);
		return dec_s;
	}
	else
		return 0;
}

unsigned long SupermanDecoder::_flags(unsigned char* data, int& inputPosition, int compressedSize)
{
	if (inputPosition < compressedSize)
	{
		//"""Accepts an iterable, which should be the same as the data stream.
		//Yields the next bit in the register, refilling as needed."""
		//# initialize.  Annoyingly redundant, but refill requires this.
		unsigned long f;
		f = data[inputPosition++];
		f |= (data[inputPosition++] << 8);
		f|=0x10000;
		return f;
	}
	else
		return 0;
}

bool SupermanDecoder::GetFlag(unsigned long& flag, unsigned char* data, int& inputPosition, int compressedSize)
{
	bool v = (bool)(flag&1);
    flag = flag >> 1;

	if (flag<2)
	{
		flag = _flags(data, inputPosition, compressedSize);
	}

	return v;
}

int SupermanDecoder::dec(unsigned char* data, int compressedSize, int dec_s, unsigned char* output)
{
    //"""RLE: uses bitcodes to handle RLE output."""

    //# initialize the data stream

	int inputPosition = 0;
    unsigned long flag = _flags(data, inputPosition, compressedSize);
	int flagSpot = 0;
	int outputPosition = 0;

    while (true)
	{
        if (outputPosition > dec_s)
		{
            return 0;
		}
		else if (outputPosition == dec_s)
		{
			return dec_s;
		}
        //# test if copy or write
        if (GetFlag(flag, data, inputPosition, compressedSize))
		{
            //# push single byte
			output[outputPosition++] = data[inputPosition++];
		}
        else
		{
            bool v = GetFlag(flag, data, inputPosition, compressedSize); // # fix: read flag before backtrack
            int back = data[inputPosition++];
            int bank = -1;
	
			int num = 0;

            if (v == true)
			{
                // # 0.1.x
                bank = (bank<<1) | GetFlag(flag, data, inputPosition, compressedSize);
                //# determine bank
                if (GetFlag(flag, data, inputPosition, compressedSize) == false)
				{
                    //#  0.1.f.0...
                    int tempV = 2;
                    for (int i = 0; i < 3; i++)
					{
                        if (GetFlag(flag, data, inputPosition, compressedSize))
							break;
                        bank = (bank<<1) | GetFlag(flag, data, inputPosition, compressedSize);
                        tempV<<=1;
					}
                    bank-=tempV;
				}

                //# figure # bytes    0.1.f.x...
                if (GetFlag(flag, data, inputPosition, compressedSize))
				{
					num=3;
				}
                else if (GetFlag(flag, data, inputPosition, compressedSize))
				{
					num=4;
				}
                else if (GetFlag(flag, data, inputPosition, compressedSize))
				{
					num=5;
				}
                else if (GetFlag(flag, data, inputPosition, compressedSize))
				{
					num=6;
				}
                else if (GetFlag(flag, data, inputPosition, compressedSize))
				{
                    num = 7 + GetFlag(flag, data, inputPosition, compressedSize);
				}
                else
				{
                    if (GetFlag(flag, data, inputPosition, compressedSize))
					{
                        //# ...0.0.0.0.0.1    copy next+0x11 bytes
                        num = data[inputPosition++] + 0x11;
					}
                    else
					{
                        //# ...0.0.0.0.0.0.n,n,n  copy n+9 bytes
                        num = GetFlag(flag, data, inputPosition, compressedSize);
                        num = (num<<1) | GetFlag(flag, data, inputPosition, compressedSize);
                        num = (num<<1) | GetFlag(flag, data, inputPosition, compressedSize);
                        num+=9;
					}
				}
			}
            else
			{
                //# 0.0.x copy 2 bytes
                num = 2;
                if (GetFlag(flag, data, inputPosition, compressedSize))
				{
					//# 0.0.1.n,n,n
                    bank = (bank<<1) | GetFlag(flag, data, inputPosition, compressedSize);
                    bank = (bank<<1) | GetFlag(flag, data, inputPosition, compressedSize);
                    bank = (bank<<1) | GetFlag(flag, data, inputPosition, compressedSize);
                    bank-=1;
				}
                else
				{
                    //# test for EOF
                    if (back == -1)
                        return outputPosition;
				}
			}

            //# assemble offset
            bank<<=8;
            back|=bank;

            //# copy num bytes
            for (int i = 0; i < num; i++)
			{
                unsigned char outputChar = output[outputPosition+back];
                output[outputPosition++] = outputChar;
			}
		}
	}

	return outputPosition;
}