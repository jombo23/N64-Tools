#include "StdAfx.h"
#include "TigDecoder.h"

TigDecoder::TigDecoder(void)
{
}

TigDecoder::~TigDecoder(void)
{
}

int TigDecoder::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

    unsigned long cmd = 0;

    //# Ends at end of input, which will throw StopIteration.
    while (inputPosition < compressedSize)
	{
        if (cmd < 2)
		{
            cmd = inputBuffer[inputPosition++];
            cmd |= (inputBuffer[inputPosition++] << 8);
            cmd |= 0x10000;
		}
        if (cmd&1)
		{
            unsigned long l = inputBuffer[inputPosition++];
            unsigned long p = l & 0xF0;
            p <<= 4;
            p |= inputBuffer[inputPosition++];
            l &= 0xF;
            for (int i = 0; i < (l+1); i++)
			{
                output[outputPosition++] = output[outputPosition-p];
			}
		}
        else
		{
            output[outputPosition++] = inputBuffer[inputPosition++];
		}
        cmd >>= 1;
	}
    return outputPosition;

}