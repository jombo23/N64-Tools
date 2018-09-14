#include "StdAfx.h"
#include "NinDec.h"

CNinDec::CNinDec(void)
{
}

CNinDec::~CNinDec(void)
{
}

int CNinDec::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* outputBuffer)
{
	int inputPosition = 4;
	int outputPosition = 0;

     unsigned long cmd = 0x10000;

     int out_sz = ((((((inputBuffer[3] << 8) | inputBuffer[2]) << 8) | inputBuffer[1]) << 8) | inputBuffer[0]);

     while (outputPosition<out_sz)
	 {
         if (cmd>0xFFFF)
		 {
             cmd = 0x100 | inputBuffer[inputPosition++];
		 }
         if (cmd & 0x80)
		 {
			 unsigned long l = 0;

             unsigned long p = inputBuffer[inputPosition++];
             if (p&0x80)
			 {
                 l = (p>>4)&7;
                 l += 1;
                 p <<= 8;
                 p |= inputBuffer[inputPosition++];
                 p &= 0xFFF;
			 }
             else
			 {
                 l = 2;
			 }
             p+=1;
             for (int i = 0; i < l; i++)
			 {
                 outputBuffer[outputPosition++] = outputBuffer[outputPosition-p];
			 }
		 }
         else
		 {
             outputBuffer[outputPosition++] = inputBuffer[inputPosition++];
		 }
         cmd <<= 1;
	 }

     //# Amusingly, they overshoot sizes and cut at the final size.
     //# Simply allowing the decompression and resizing should be okay though.
     return out_sz;

}