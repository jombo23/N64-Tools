#include "StdAfx.h"
#include "VigilanteDecoder.h"

CVigilanteDecoder::CVigilanteDecoder(void)
{
}

CVigilanteDecoder::~CVigilanteDecoder(void)
{
}

int CVigilanteDecoder::dec(unsigned char* data, int& compressedSize, unsigned char* output)
{
	int inputPosition = 8;
	int outputPosition = 0;

	int expectedSize = ((((((data[7] << 8) | data[6]) << 8) | data[5]) << 8) | data[4]);

     unsigned char ring[0x800];

     int rp = 0;
     unsigned long c = data[inputPosition++] | 0x100;
	 while (outputPosition < expectedSize)
	 {
         if (c > 0xFFFF)
             c = data[inputPosition++] | 0x100;

         if (c & 0x80)
		 {
             output[outputPosition++] = data[inputPosition++];
             ring[rp] = output[outputPosition-1];
             rp += 1;
             rp &= 0x7FF;
		 }
         else
		 {
             unsigned long p = data[inputPosition++];
             p |= (data[inputPosition++] << 8);
             unsigned long l = (p & 0x1F) + 2;
             p >>= 5;
             for (int i = p; i < p+l; i++)
			 {
                 output[outputPosition++] = (ring[i & 0x7FF]);
                 ring[rp] = output[outputPosition-1];
                 rp += 1;
				 rp &= 0x7FF;
			 }
		 }
         c <<= 1;
	 }
     return outputPosition;
}