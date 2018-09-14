#include "StdAfx.h"
#include "ASMICDecoder.h"

CASMICDecoder::CASMICDecoder(void)
{
}

CASMICDecoder::~CASMICDecoder(void)
{
}

int CASMICDecoder::dec(unsigned char* data, int compressedSize, unsigned char* output)
{
    int inputPosition = 0;
	int outputPosition = 0;

	//# default filesize is 3-bytes, skipping first which is compression indicator
	//# 0x0	1	00 if compressed
	//# 0x1	3	filesize, big-endian
	unsigned char compressed = data[inputPosition];
	inputPosition++;

	if (compressed != 0)
	{
		memcpy(output, data, compressedSize);
		return compressedSize;
	}

    int dec_s = ((((data[inputPosition] << 8) | data[inputPosition+1]) << 8) | data[inputPosition+2]);
	inputPosition += 3;

    unsigned long cmd = 0;
    while (outputPosition < dec_s)
	{
        cmd >>= 1;
        if (cmd < 2)
            cmd = data[inputPosition++] | 0x100;
        if (cmd&1)
            output[outputPosition++] = data[inputPosition++];
        else
		{
            signed long p = data[inputPosition++];
            unsigned long l = data[inputPosition++];
            p |= ((l<<4)&0xF00);
            l&=0xF;
            p+=0x12;
            p&=0xFFF;
            //# funky correction here
            unsigned long c = outputPosition&0xFFF;
            p -= c;
            if (p>=0)
                p -= 0x1000;
            p += outputPosition;
            //# loopus
            for (int i = 0; i < l+3; i++)
			{
                unsigned long v;
				if (p < 0)
					v = 0;
				else 
					v = output[p];
                output[outputPosition++] = v;
                p+=1;
			}
		}
	}
    return outputPosition;
}
