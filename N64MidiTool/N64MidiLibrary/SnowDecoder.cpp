#include "StdAfx.h"
#include "SnowDecoder.h"

CSnowDecoder::CSnowDecoder(void)
{
}

CSnowDecoder::~CSnowDecoder(void)
{
}

int CSnowDecoder::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

	unsigned short* shortVersions = new unsigned short[compressedSize/2];
	for (int i = 0; i < compressedSize/2; i++)
	{
		shortVersions[i] = ((inputBuffer[i*2] << 8) | inputBuffer[i*2+1]);
	}

	for (int i = 0; i < (compressedSize/2); i++)
	{
		if (shortVersions[i] < 0x100)
		{
			output[outputPosition++] = shortVersions[i] & 0xFF;
		}
		else
		{
			unsigned long l = shortVersions[i] >> 12;
			shortVersions[i] &= 0xFFF;

			for (int j = 0; j < l; j++)
			{
				output[outputPosition++] = output[outputPosition-shortVersions[i]];
			}
		}
	}

	delete [] shortVersions;

	return outputPosition;
}