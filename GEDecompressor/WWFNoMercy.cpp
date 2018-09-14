// BY ZOINKITY, PORT UNTESTED
#include "StdAfx.h"
#include "WWFNoMercy.h"

CWWFNoMercy::CWWFNoMercy(void)
{
}

CWWFNoMercy::~CWWFNoMercy(void)
{
}

int CWWFNoMercy::decWWFNoMercy(unsigned char* data, int& compressedSize, int dec_s, unsigned char* output)
{

	int inputPosition = 0;
	int outputPosition = 0;

	unsigned long cmd = 0;

    while (outputPosition < dec_s)
	{
		cmd>>=1;
		if (cmd<2)
			cmd = data[inputPosition++]|0x100;

		if (cmd&1)
		{
			output[outputPosition++] = data[inputPosition++];
		}
		else
		{
			signed long p = data[inputPosition++];
			signed long l = data[inputPosition++];
			p|= ((l<<4)&0xF00);
			l&=0xF;
			p+=0x12;
			p&=0xFFF;
			//# funky correction here
			signed long c= outputPosition&0xFFF;
			p-=c;
			if (p>=0)
				p-=0x1000;
			p+=c;
			//# loopus
			for (int i = 0; i < (l+2); i++)
			{
				unsigned long v = outputPosition-p;

				if (v<0)
					v = 0;
				else
					v = output[v];
			
				output[outputPosition++] = v;
				p+=1;
			}
		}
	}
	return outputPosition;
}