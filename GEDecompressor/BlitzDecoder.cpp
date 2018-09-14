#include "StdAfx.h"
#include "BlitzDecoder.h"

CBlitzDecoder::CBlitzDecoder(void)
{
}

CBlitzDecoder::~CBlitzDecoder(void)
{
}

unsigned long CBlitzDecoder::counter(unsigned char*& input, unsigned char cmd, unsigned char mask, int& compressedSize)
{
	unsigned long count = cmd & mask;
	if (count==0)
	{
		cmd = *input++;
		compressedSize++;
		while (cmd==0)
		{
			count+=0xFF;
			cmd = *input++;
			compressedSize++;
		}
		count += (cmd + mask);
	}
	return count;
}
int CBlitzDecoder::dec(unsigned char* input, unsigned char* output, int& compressedSize)
{
	unsigned char* head = output;
	int uncompressedSize = 0;
	compressedSize = 0;
    int skip = 0;
	unsigned char cmd = *input++;
	compressedSize++;

	if (cmd>17)
	{
		for (int x = 0; x < (cmd-17); x++)
		{
			*output++ = *input++;
			uncompressedSize++;
			compressedSize++;
		}
	}
	else
		skip = 1;

	while (true)
	{
		cmd = *input++;
		compressedSize++;

		if (skip==0)
		{
			// if didn't copy yet, test if you should
			if (cmd<16)
			{
				int count = counter(input, cmd, 15, compressedSize);
				for (int x = 0; x < (count+3); x++)
				{
					*output++ = *input++;
					compressedSize++;
					uncompressedSize++;
				}
				cmd = *input++;
				compressedSize++;
			}
			else
			{
				skip = 1;
			}
		}
		// index from end of file back
		int back = (uncompressedSize - 1);
		int count = 0;
        // handle each bit identifier
		if (cmd & 0xC0)
		{
			back -= ((cmd&0x1C)>>2) + ((*input++)<<3);
			compressedSize++;
			count = ((cmd&0xE0)>>5) - 1;
		}
		else if (cmd & 0x20)
		{
			count = counter(input, cmd, 31, compressedSize);
			cmd = *input++;
			compressedSize++;
			back -= ((*input++)<<6) + (cmd>>2);
			compressedSize++;
		}
		else if (cmd & 0x10)
		{
			back -= ((cmd&8)<<11);
			count = counter(input, cmd, 7, compressedSize);
			cmd = *input++;
			compressedSize++;
			back -= ((*input++)<<6) + (cmd>>2);
			compressedSize++;
			// if no backtracking, you've hit the end!
			if ((back + 1)==uncompressedSize)
				 break;
			back-=0x3FFF;
		}
		else
		{
			back -= (((*input++)<<2) + (cmd>>2));
			compressedSize++;
            // slightly different if you copied
			if (skip==0)
			{
				back -= 0x800;
				count = 1;
			}
		}
		// copy count+2 bytes from referenced point
		for (int x = 0; x < (count+2); x++)
		{
			if ((back < 0) || (back >= uncompressedSize))
				return 0;
			unsigned char val = head[back+x];
			*output++ = val;
			uncompressedSize++;
		}

		// directly copy any hanging entry
		skip = (cmd&3);
		for (int x = 0; x < skip; x++)
		{
			*output++ = *input++;
			compressedSize++;
			uncompressedSize++;
		}
	}

	return uncompressedSize;
}
