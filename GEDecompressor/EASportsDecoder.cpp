#include "StdAfx.h"
#include "EASportsDecoder.h"

EASportsDecoder::EASportsDecoder(void)
{
}

EASportsDecoder::~EASportsDecoder(void)
{
}

int EASportsDecoder::dec(unsigned char* input, unsigned char* output, int& compressedSize)
{
	int outputSize = 0;

	int pos=0;
    unsigned long ctl = ((((((input[pos+2] << 8) | input[pos+1]) << 8) | input[pos])));
    pos+=2;
    // if not 10FB, then you've got straight data
    if (ctl!=0xFB10) 
	{
		return 0;
	}
    //[80000D10] decompressed size; nevermind that next thing can't happen
    if (ctl&1)
		pos+=3;
    int s_out = (((((input[pos] << 8) | input[pos+1]) << 8) | input[pos+2]));
    pos+=3;

    //[80000D9C] decompression loop
    while (true)
	{
        if (outputSize>s_out)
			return 0;
        
		// grab all possible commands here
        int cmd = input[pos];
        int cmd2= input[pos+1];
        int cmd3= input[pos+2];
        int cmd4= input[pos+3];
        
		if (cmd >= 0xE0)
		{
            //[80000FF8]
            pos+=1;
            cmd &= 0x1F;
            if (cmd<0x1C)
			{
                //[80001018] ---LLLLL
                int lnt = (cmd+1)<<2;
				for (int j = pos; j < (pos + lnt); j++)
				{
					output[outputSize] = input[j];
					outputSize++;
				}
                pos+=lnt;
			}
            else
			{
                //[80001064] end of file
                int lnt = (cmd & 3);
				for (int j = pos; j < (pos + lnt); j++)
				{
					output[outputSize] = input[j];
					outputSize++;
				}
                pos+=lnt;
                return outputSize;
			}
		}
        else if (cmd >= 0xC0)
		{
            //[80000F2C] --0BLLcc.BBBBBBBB.bbbbbbbb.00000000
            pos+=4;
            int lnt = (cmd & 3);
			for (int j = pos; j < (pos + lnt); j++)
			{
				output[outputSize] = input[j];
				outputSize++;
			}
            pos+=lnt;
            //[80000F80]
            int back = (cmd<<12) & 0x10000;
            back+= cmd2<<8;
            back+= cmd3 + 1;
            lnt = (cmd<<6) & 0x300;
            lnt+= cmd4;
            for (int i = 0; i < (lnt+5); i++)
			{
				if (back > outputSize)
					return 0;
                unsigned char val = output[outputSize-back];
                output[outputSize] = val;
				outputSize++;
			}
		}
        else if (cmd & 0x80)
		{
            //[80000E70] 00LLLLLL.ccBBBBBB.bbbbbbbb
            pos+=3;
            int lnt = (cmd2>>6)&3;
            for (int j = pos; j < (pos + lnt); j++)
			{
				output[outputSize] = input[j];
				outputSize++;
			}
            pos+=lnt;
            //[80000ECC]
            int back = (cmd2 & 0x3F)<<8;
            back+= cmd3 + 1;
            lnt = cmd & 0x3F;
            for (int i = 0; i < (lnt+4); i++)
			{
				if (back > outputSize)
					return 0;
                unsigned char val = output[outputSize-back];
                output[outputSize] = val;
				outputSize++;
			}
		}
        else
		{
            //[80000DBC] 0BBLLLcc.bbbbbbbb
            pos+=2;
            int lnt = cmd & 3;
            for (int j = pos; j < (pos + lnt); j++)
			{
				output[outputSize] = input[j];
				outputSize++;
			}
            pos+=lnt;
            //[80000E10]
            int back = (cmd<<3)&0x300;
            back+= cmd2 + 1;
            lnt = (cmd>>2) & 7;
            for (int i = 0; i < (lnt+3); i++)
			{
				if (back > outputSize)
					return 0;
                unsigned char val = output[outputSize-back];
                output[outputSize] = val;
				outputSize++;
			}
		}
	}
	return outputSize;
}