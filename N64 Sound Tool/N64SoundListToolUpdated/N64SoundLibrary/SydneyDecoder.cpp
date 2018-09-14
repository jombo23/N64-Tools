#include "StdAfx.h"
#include "SydneyDecoder.h"

SydneyDecoder::SydneyDecoder(void)
{
}

SydneyDecoder::~SydneyDecoder(void)
{
}


bool _cmd(unsigned char* inputBuffer, int& inputPosition, unsigned long long& cmd, int totalLength)
{
    while (inputPosition < totalLength)
	{
        if (cmd < 2)
		{
            cmd = (unsigned long long)(((unsigned long long)inputBuffer[inputPosition+3] << 24) | ((unsigned long long)inputBuffer[inputPosition+2] << 16) | ((unsigned long long)inputBuffer[inputPosition+1] << 8) | ((unsigned long long)inputBuffer[inputPosition]));
            cmd |= 0x100000000;
            inputPosition += 4;
		}
        bool returnValue = (cmd&1);
        cmd>>=1;
		return returnValue;
	}

	// should never hit
	return false;
}

int _value(unsigned char* inputBuffer, int& inputPosition, int length=1)
{
	int v = -1;

	if (length == 1)
		v = inputBuffer[inputPosition];
	else if (length == 2)
		v = (inputBuffer[inputPosition+1] << 8) | inputBuffer[inputPosition];

    inputPosition += length;
    return v;
}

int SydneyDecoder::dec(unsigned char* inputBuffer, int& compressedSize, int expectedSize, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

	unsigned long long c = 0;
	
    //# Not necessary, but could prevent an overrun.
    while (outputPosition < expectedSize)
	{
        if (_cmd(inputBuffer, inputPosition, c, compressedSize))
		{
            if (_cmd(inputBuffer, inputPosition, c, compressedSize))
			{
                int l = _value(inputBuffer, inputPosition, 2);
                int p = (l>>3) - 0x2000;
                l &= 7;
                if (!l)
				{
                    l = _value(inputBuffer, inputPosition, 1);
                    if (l & 0x80)
					{
                        p-=0x2000;
					}
                    l &= 0x7F;
                    if (l==1)
                        break;
                    if (l==0)
                        l = _value(inputBuffer, inputPosition, 2) - 2;
				}
                for (int i = 0; i < (l+2); i++)
				{
                    output[outputPosition++] = output[outputPosition+p];
				}
			}
            else
			{
                int p = _value(inputBuffer, inputPosition, 1);
                p-=256;
                output[outputPosition++] = output[outputPosition+p];
                output[outputPosition++] = output[outputPosition+p];
                if (_cmd(inputBuffer, inputPosition, c, compressedSize))
				{
                    output[outputPosition++] = output[outputPosition+p];
					output[outputPosition++] = output[outputPosition+p];
				}
                if (_cmd(inputBuffer, inputPosition, c, compressedSize))
				{
                    output[outputPosition++] = output[outputPosition+p];
				}
			}
		}
        else
		{
            output[outputPosition++] = _value(inputBuffer, inputPosition, 1);
		}
	}
	return outputPosition;
}