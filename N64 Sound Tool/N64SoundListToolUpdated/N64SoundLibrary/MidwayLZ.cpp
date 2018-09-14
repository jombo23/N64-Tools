#include "StdAfx.h"
#include "MidwayLZ.h"

MidwayLZ::MidwayLZ(void)
{
}

MidwayLZ::~MidwayLZ(void)
{
}

int MidwayLZ::decLZSS0B(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	int expectedDecompressedSize = ((((((inputBuffer[0] << 8) | inputBuffer[1]) << 8) | inputBuffer[2]) << 8) | inputBuffer[3]);

	//# set up ring buffers for variations requiring it
	unsigned char buffer[0x1000];

	unsigned long b_idx = 0xFEE;
    for (int x = 0; x < b_idx ; x++)
	{
		buffer[x] = 0x00;
	}
    for (int x = 0; x < (0x1000-b_idx); x++)
	{
		buffer[x] = 0x00;
	}

    unsigned long bits = 0;

	int inputPosition = 4;
	int outputPosition = 0;

	while (outputPosition < expectedDecompressedSize)
	{
        if (bits<0x100)
		{
            bits = inputBuffer[inputPosition] | 0xFF00;
            inputPosition+=1;
		}
        if(bits&1)
		{
			buffer[b_idx]=inputBuffer[inputPosition];
            b_idx=(b_idx+1)&0xFFF;
            output[outputPosition++] = (inputBuffer[inputPosition]);
            inputPosition+=1;
		}
        else
		{
            //# size = data[pos + (mode=="LZSS")]
            //# val = data[pos + (mode!="LZSS")]
            unsigned long val = inputBuffer[inputPosition];
            unsigned long size = inputBuffer[inputPosition+1];

            inputPosition+=2;
            unsigned long back = size&0xF0;
            back*=0x10;
            back|= val;
            back&=0xFFF;
            size = (size&0xF) + 2 + 1;
            for (int x = 0; x < size; x++)
			{
				buffer[b_idx] = buffer[back];
                output[outputPosition++] = (buffer[back]);
                b_idx=(b_idx+1)&0xFFF;
                back = (back+1)&0xFFF;
			}
		}
        bits>>=1;
	 }

	return expectedDecompressedSize;
}

int MidwayLZ::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
    //# set up ring buffers for variations requiring it
    unsigned long bits = 0;

	int inputPosition = 0;
	int outputPosition = 0;

	while (inputPosition<compressedSize)
	{
        if (bits<0x100)
		{
            bits = inputBuffer[inputPosition] | 0xFF00;
            inputPosition+=1;
		}
        if(bits&1)
		{
            output[outputPosition++] = (inputBuffer[inputPosition]);
            inputPosition+=1;
		}
        else
		{
            //# size = data[inputPosition]
            //# val = data[inputPosition + 1]
            unsigned long size = inputBuffer[inputPosition];
            unsigned long val = inputBuffer[inputPosition+1];
            if ((size+val)==0)
			{
				return outputPosition;
			}
            inputPosition+=2;
            unsigned long back = size&0xF0;
            back*=0x10;
            back|= val;
            back&=0xFFF;
            size = (size&0xF) + 2;
            for (int x = 0; x < size; x++)
			{
				output[outputPosition++] = (output[outputPosition-back]);
			}
		}
        bits>>=1;
	 }
	return outputPosition;
}