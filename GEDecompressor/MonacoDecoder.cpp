#include "StdAfx.h"
#include "MonacoDecoder.h"

MonacoDecoder::MonacoDecoder(void)
{
}

MonacoDecoder::~MonacoDecoder(void)
{
}

unsigned long MonacoDecoder::GetBits(int numBits, unsigned char* data, int& inputPosition, int compressedSize)
{
    while (numBits>numBitsInRegister)
	{
        //# if need more data, read in from iterator
        int v = data[inputPosition++];
        bitRegister = bitRegister << 8; //   # offset for #bits
        bitRegister |= v;
        numBitsInRegister += 8; //    # #bits+8
	}

    //# read upper #bits from data
    numBitsInRegister -= numBits;
    unsigned long returnValue = bitRegister>>numBitsInRegister;

    //# generate the mask and correct the data
    unsigned long mask = (1<<numBitsInRegister)-1;
    bitRegister = bitRegister & mask;

	return returnValue;
}


int MonacoDecoder::header(unsigned char* data, int& compressedSize)
{
    compressedSize = ((((((data[4] << 8) | data[5]) << 8) | data[6]) << 8) | data[7]);
	return ((((((data[0] << 8) | data[1]) << 8) | data[2]) << 8) | data[3]);
}

int MonacoDecoder::dec(unsigned char* data, int compressedSize, int dec_s, unsigned char* output)
{
	bitRegister = 0;
	numBitsInRegister = 0;

	//"""Decompresses data, so feed it the actual compressed
    //data without a header."""

    //# initialize the data stream
	int inputPosition = 0;
	int outputPosition = 0;

    //# first two reads set size of copy commands
    int back_l = data[inputPosition++];
    int len_l  = data[inputPosition++];
    //# MFHI, or upper 32bits of a 64bit register
    //# subtract 1 if sum was negative, which it effectively can't be
    unsigned __int64 def_l = ((((unsigned __int64)0x38E38E39) * ((unsigned __int64)back_l + (unsigned __int64)len_l + 1))>>((unsigned __int64)33));
    def_l+= 1;
    def_l&= 0xFF;

    while (true)
	{
        //# bit determines mode.  Note default value is 0, so will quit on a bad iterator.
        if (GetBits(1, data, inputPosition, compressedSize))
		{
            //# if bit set, write byte
            output[outputPosition++] = GetBits(8, data, inputPosition, compressedSize);
		}
        else
		{
            //# copy l bytes from end of output-b
            int b = GetBits(back_l, data, inputPosition, compressedSize);
            
			int l = GetBits(len_l, data, inputPosition, compressedSize);
            for (int i = 0; i < (l + def_l); i++)
			{
                unsigned char v = output[outputPosition-b];
                output[outputPosition++] = v;
			}
		}
	}
}