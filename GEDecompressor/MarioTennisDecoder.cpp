#include "StdAfx.h"
#include "MarioTennisDecoder.h"

CMarioTennisDecoder::CMarioTennisDecoder(void)
{
}

CMarioTennisDecoder::~CMarioTennisDecoder(void)
{
}

int CMarioTennisDecoder::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

	if (inputBuffer[inputPosition++] != 1)
	{
		//raise ValueError("Unknown compression type.")
		return 0;
	}

     unsigned long size = (inputBuffer[inputPosition++] << 16);
     size |= (inputBuffer[inputPosition++] << 8);
     size |= inputBuffer[inputPosition++];

     //# Need to find the case it uses for that flag.
     while (outputPosition < size)
	 {
         unsigned long v = inputBuffer[inputPosition++];
         v |= 0x100;

         while (v < 0x10000)
		 {
             if (v & 0x80)
			 {
                 unsigned long l = inputBuffer[inputPosition++];
                 unsigned long p = (l << 4) & 0xF00;
                 p |= inputBuffer[inputPosition++];
                 l &= 0xF;
                 if (p == 0)
				 {
                     //print("Encoded end: 0x{:X} versus 0x{:X} expected".format(len(out), size))
                     return outputPosition;
				 }
                 if (l == 0)
                     l = inputBuffer[inputPosition++] + 0x10;
                 for (int i = 0; i < (l+1); i++)
				 {
                     output[outputPosition++] = output[outputPosition-p];
				 }
			 }
             else
			 {
                output[outputPosition++] = inputBuffer[inputPosition++];
			 }
             v <<= 1;
		 }
	 }
}