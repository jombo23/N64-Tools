#include "StdAfx.h"
#include "BoltDecoder.h"

CBoltDecoder::CBoltDecoder(void)
{
}

CBoltDecoder::~CBoltDecoder(void)
{
}

int CBoltDecoder::dec(unsigned char* input, unsigned char* output, int& compressedSize, int decompressedSize)
{
	int inputPosition = 0;
	int outputPosition = 0;

	unsigned long l = 0;
	signed long b = 0;
	unsigned long cnt = 0;

    //# This catches the stream when decompressedSize isn't provided 
    try
	{
        while (true)
		{
            if ((decompressedSize > 0) && (outputPosition >= decompressedSize))
                break;
            unsigned long cmd = input[inputPosition++];
            if (cmd<0x80)
			{
                //# (cmd&0xF) NOR (b<<4) 
                b<<=4;
                b|= (cmd&0xF);
                b = outputPosition + (~b);
                l<<=3;
                l|= (cmd>>4);
                l+=cnt;
                for (int i = 0; i < (l+2); i++)
				{
                    unsigned char v = output[b+i];
                    output[outputPosition++] = v;
				}
                l = 0;
				b = 0;
				cnt = 0;
                continue;
			}
            else if (cmd<0x90)
			{
                l<<=4;
                l+= (cmd&0xF);
                for (int i = 0; i < (l+1); i++)
				{
                   output[outputPosition++] = input[inputPosition++]; 
				}
                l = 0;
				cnt = 0;
                continue;
			}
            else if (cmd<0xA0)
			{
                l = cmd&3;
                cmd>>=2;
                b = cmd&3;
			}
            else if (cmd<0xC0)
			{
                l<<=5;
                l|= (cmd&0x1F);
			}
            else
			{
                b<<=6;
                b|= (cmd&0x3F);
			}
            cnt+=1;
		}
	}
	catch (...)
	{

	}
    return outputPosition; 
}
