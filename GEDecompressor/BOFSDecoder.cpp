// BY ZOINKITY

#include "StdAfx.h"
#include "BOFSDecoder.h"

CBOFSDecoder::CBOFSDecoder(void)
{
}

CBOFSDecoder::~CBOFSDecoder(void)
{
}


int  CBOFSDecoder::_flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft)
{
	if (flagLeft == 0)
	{
		flagLeft = 8;
		flags = inputBuffer[inputPosition++];
	}

    int value = ((flags&0x80)>0);
    flags<<=1;
	flagLeft--;
	return value;
}

unsigned long CBOFSDecoder::helper(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, int num)
{
    //"""Returns a value equal to the next num bits in stream.
    //itr should point to the self._flags() method above."""
    unsigned long v=0;
    for (int i = 0; i < num; i++)
	{
        v <<=1;
		bool flagRet = _flags(inputBuffer, inputPosition, flags, flagLeft);;
        v|=flagRet;
	}
    return v;
}


int CBOFSDecoder::dec(unsigned char* inputBuffer, int compressedSize, int dec_s, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

	unsigned char tbl[0xA] = {0x20, 0x65, 0x74, 0x6F, 0x61, 0x72, 0x6E, 0x69, 0x73, 0x68};
	

	unsigned char flags = 0;
	int flagsLeft = 0;

    //# init variables
    unsigned long cnt = 0;
	unsigned long reg = 0;

   while (true)
   {
		if (outputPosition>=dec_s)
			break;
        
		unsigned long v = helper(inputBuffer, inputPosition, flags, flagsLeft, 1);
        cnt+=1;
        reg<<=1;
        reg|=v;
        if (v != 0)
		{
			//# bit set
            if (cnt<7)
                continue;
            else if (cnt>7)
                break;
            output[outputPosition++] = tbl[9];
            cnt = 0;
			reg = 0;
		}
        else
		{
			unsigned long base = 0;
            //# bit not set
            if ((cnt == 2) || (cnt == 3))
                base = 0;
			else if ((cnt == 4) || (cnt == 5))
                base = 3;
            if ((cnt == 6) || (cnt == 7))
                base= 6;
            else if (cnt > 7)
                break;

            if (cnt==1)
			{
                //# if cnt==1 write a byte to file
                v = helper(inputBuffer, inputPosition, flags, flagsLeft, 8);
                output[outputPosition++] = (v & 0xFF);
			}
            else
			{
                //# otherwise, grab a value out of table
                if (!(cnt&1))
				{
                    v = helper(inputBuffer, inputPosition, flags, flagsLeft, 1);
                    reg<<=1;
                    reg|=v;
				}
                reg&=3;
                reg+=base;
                v = tbl[reg];
                output[outputPosition++] = (v & 0xFF);
			}
            cnt = 0;
			reg = 0;
		}
	}

	return outputPosition;
}