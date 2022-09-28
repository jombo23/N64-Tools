// BY ZOINKITY
#include "StdAfx.h"
#include "FLA2Decoder.h"

CFLA2Decoder::CFLA2Decoder(void)
{
}

CFLA2Decoder::~CFLA2Decoder(void)
{
}


int CFLA2Decoder::decFLA2(unsigned char* data, int& compressedSize, int dec_s, unsigned char* output)
{
        if (dec_s == 0)
            dec_s = 0x1000000;

        //# set up a ring of 0x1000, init to 0; buffer pos = 0
        unsigned char ring[0x1000];
        for (int x = 0; x < 0x1000; x++)
            ring[x] = 0;

        unsigned long cmd = 0x10000;
		unsigned long cur = 0;

		int inputPosition = 0;
		int outputPosition = 0;

        while (outputPosition < dec_s)
		{
            if (cmd>0xFFFF)
                cmd = data[inputPosition++] | 0x100;
            cmd<<=1;
            if (cmd&0x100)
			{
                //# if set, copy from ring to output
                unsigned char c = data[inputPosition++];
                //# if zero, this is EOF
                if (!c)
                    break;
                unsigned long b = (c<<4)&0xF00;
                b|= data[inputPosition++];
                c&=0xF;
                for (int i = 0; i < (c+2); i++)
				{
                    unsigned char v = ring[(cur-b)&0xFFF];
                    output[outputPosition++] = v;
                    ring[cur]=v;
                    cur+=1;
                    cur&=0xFFF;
				}
			}
            else
			{
                //# if not set, flush next byte to output
                unsigned char v = data[inputPosition++];
                output[outputPosition++] = v;
                ring[cur]=v;
                cur+=1;
                cur&=0xFFF;
			}
		}
		compressedSize = inputPosition;

        return outputPosition;
}