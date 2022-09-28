#include "StdAfx.h"
#include "BlastCorpsLZBDecoder.h"

BlastCorpsLZBDecoder::BlastCorpsLZBDecoder(void)
{
}

BlastCorpsLZBDecoder::~BlastCorpsLZBDecoder(void)
{
}

unsigned long BlastCorpsLZBDecoder::bit(unsigned char* input, int& inputPosition, int& compressedSize, int count)
{
    //"""Reads [count] bits off the top of the source.

    //Normally I'd use a generator, but
    //parsing the file isn't as clear-cut as you'd think."""
    while (nbits < count)
	{
        value<<=8;
        value|= input[inputPosition];
        nbits+= 8;
        inputPosition  += 1;
	}
    nbits -= count;
    unsigned long v = value >> nbits;
    //# Mask away unused data to prevent a really big number accruing.
    unsigned long m = (1<<nbits) - 1;
    value &= m;
    return v;
}

void BlastCorpsLZBDecoder::reset(int& inputPosition)
{
    //"""Pushes position to an even address and clears loaded source data."""
    if (inputPosition&1)
        inputPosition +=1;
    nbits = 0;
    value = 0;
    base  = 0;
	for (int x = 0; x < 2000; x++)
		ring[x] = 0;
}

void BlastCorpsLZBDecoder::read(unsigned char* input, int& inputPosition, unsigned char* output, int& outputPosition, int& compressedSize)
{
    //"""Decompresses next block of data until endcode is encountered.
    //"""
    while (true)
	{
        if (bit(input, inputPosition, compressedSize, 1))
		{
            output[outputPosition++] = bit(input, inputPosition, compressedSize, 8);
            base += 1;
            base&=0x1FFF;
            ring[base] = output[outputPosition-1];
		}
        else
		{
            unsigned long o = bit(input, inputPosition, compressedSize, 13);
            if (o == 0)
                break;
            unsigned long v = bit(input, inputPosition, compressedSize, 3);
            for (int i = 0; i < (v+3); i++)
			{
                unsigned long p = (o+i) & 0x1FFF;
                output[outputPosition++] = ring[p];
                base += 1;
                base&=0x1FFF;
                ring[base] = output[outputPosition-1];
			}
		}
	}
    reset(inputPosition);
}


int BlastCorpsLZBDecoder::dec(unsigned char* input, unsigned char* output, int& compressedSize)
{
	int inputPosition = 0;
	int outputPosition = 0;

	reset(inputPosition);

	//"""Decompresses data until source is exhausted,
    //returning decompressed data as a bytes object."""
    while (inputPosition < compressedSize)
	{
		read(input, inputPosition, output, outputPosition, compressedSize);    
        if ((outputPosition > 0) && (outputPosition>=inputPosition))
            break;
	}

	return outputPosition;
}