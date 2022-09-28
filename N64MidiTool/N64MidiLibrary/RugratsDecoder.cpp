#include "StdAfx.h"
#include "RugratsDecoder.h"

CRugratsDecoder::CRugratsDecoder(void)
{
}

CRugratsDecoder::~CRugratsDecoder(void)
{
}

int CRugratsDecoder::dec(unsigned char* data, int compressedSize, unsigned char* output)
{
    int inputPosition = 0;
	int outputPosition = 0;

      while (true)
	  {
         unsigned long c = data[inputPosition++];
         if (c == 0)
         	return outputPosition;

         if (c & 0x80)
		 {
         	unsigned long l = data[inputPosition++];
         	c &= 0x7F;
         	c <<= 3;
         	c |= l >> 5;
         	l &= 0x1F;
         	for (int i = 0; i < l; i++)
         		output[outputPosition++] = output[outputPosition-c];
		 }
         else if (c & 0x40)
		 {
         	c &= 0x3F;
         	unsigned long l = data[inputPosition++];

			for (int x = 0; x < c; x++)
			{
         		output[outputPosition++] = l;
			}
		}
         else
		 {
         	for (int i = 0; i < c; i++)
			{
         		output[outputPosition++] = data[inputPosition++];
			}
		 }
	  }

}