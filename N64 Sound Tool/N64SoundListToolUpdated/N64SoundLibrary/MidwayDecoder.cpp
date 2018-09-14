#include "StdAfx.h"
#include "MidwayDecoder.h"

CMidwayDecoder::CMidwayDecoder(void)
{
}

CMidwayDecoder::~CMidwayDecoder(void)
{
}

int CMidwayDecoder::dec(unsigned char* data, int& compressedSize, unsigned char* output, CString mode, int pos)
{
/*        """
    Three most-prevalent LZ-derived compression schemes.
    Can except modes:
        MIDWAY does not use a ring buffer
        LZSS for traditional LZSS with a ring buffer
        LZSS_0 for variation of LZSS with 00 NULLs o.0
        WILLIAMS for alternate decoding method
    Difference lies primarily in initialization buffer and values.
    LZSS is traditional lzss with 20 as fill character.
    LZSS_0 is not traditional; 0 is used as the NULL character.
    """*/
	int inputPosition = pos;
	int outputPosition = 0;

	unsigned char buffer[0x1000];

	int b_idx = 0;

    //# set up ring buffers for variations requiring it
    if (mode == "WILLIAMS")
	{
        for (int x = 0; x < 0x1000; x++)
			buffer[x] = 0;
        b_idx = 1;
	}
    else if ((mode == "LZSS") || (mode == "LZSS_0"))
	{
        b_idx = 0xFEE;

		int bCounter = 0;

        for (int x = 0; x < b_idx; x++)
		{
			if (mode.Find("0") != -1)
				buffer[bCounter++] = 0;
			else
				buffer[bCounter++] = 32;
		}

        for (int x = 0; x < (0x1000-b_idx); x++)
			buffer[bCounter++] = 0;
	}
    unsigned long bits = 0;

    try
	{
     while (inputPosition < compressedSize)
	 {
        if (bits<0x100)
		{
            bits = data[inputPosition] | 0xFF00;
            inputPosition+=1;
		}
        if(bits&1)
		{
            if (mode!="MIDWAY")
			{
                buffer[b_idx]=data[inputPosition];
                b_idx=(b_idx+1)&0xFFF;
			}
            output[outputPosition++] = data[inputPosition];
            inputPosition += 1;
		}
        else
		{
			unsigned long val;
			unsigned long size;

            //# size = data[inputPosition + (mode=="LZSS")]
            //# val = data[inputPosition + (mode!="LZSS")]
            if ((mode == "LZSS") || (mode == "LZSS_0"))
			{
                val = data[inputPosition];
                size = data[inputPosition+1];
			}
            else
			{
                size = data[inputPosition];
                val = data[inputPosition+1];
                if ((size+val)==0)
					break;
			}
            inputPosition+=2;

            unsigned long back = size&0xF0;
            back*=0x10;
            back|= val;
            back&=0xFFF;
            size = (size&0xF) + 2 + (int)((mode == "LZSS") || (mode == "LZSS_0"));
            for (int x = 0; x < size; x++)
			{
                if (mode != "MIDWAY")
				{
                    buffer[b_idx] = buffer[back];
                    output[outputPosition++] = (buffer[back]);
                    b_idx=(b_idx+1)&0xFFF;
                    back = (back+1)&0xFFF;
				}
                else
				{
                    output[outputPosition++] = (output[outputPosition - back]);
				}
			}
		}
        bits>>=1;
	   }
	}
    catch(...)
	{
        return 0;
	}
    return outputPosition;
}
