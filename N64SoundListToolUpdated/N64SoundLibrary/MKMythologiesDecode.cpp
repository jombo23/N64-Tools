// BY ZOINKITY

#include "StdAfx.h"
#include "MKMythologiesDecode.h"

CMKMythologiesDecode::CMKMythologiesDecode(void)
{
}

CMKMythologiesDecode::~CMKMythologiesDecode(void)
{
}

int CMKMythologiesDecode::_flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft)
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

unsigned long CMKMythologiesDecode::helper(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, int num)
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

int CMKMythologiesDecode::dec(unsigned char* inputBuffer, int compressedSize, int dec_s, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

	unsigned char flags = 0;
	int flagsLeft = 0;

      
    unsigned long prev = 0;
    unsigned long tbl[0x1000];
	unsigned long length[0x1000];
	for (int x = 0; x < 0x1000; x++)
	{
		tbl[x] = 0;
		length[x] = 0;
	}

    unsigned long c_tbl = 0;
    int bits = 9;

	unsigned long lnt = 1;
	unsigned long idx = 0;

    while (true)
	{
        //# test if over file output size
        if (outputPosition>dec_s)
           break;
        //# decompress
        unsigned long cmd = helper(inputBuffer, inputPosition, flags, flagsLeft, bits);
        lnt = 1;

        if (cmd<0x100)
		{
            //# normal write
            output[outputPosition++]= cmd;
		}
        else if (cmd==0x100)
		{
            //# (re)initialize the lookup table
            idx = outputPosition;
            c_tbl = 0;
            bits = 9;
            prev = 0x102;
            continue;
		}
        else if (cmd==0x101)
		{
            //# normal return
            break;
		}
        else
		{
            //# add new entry based on previous
            if ((cmd-0x102) < c_tbl)
			{
                //# unless recalling previous entry
                prev = cmd;
			}
            if (prev<0x100)
                output[outputPosition++] = (prev & 0xFF);
            else
			{
                prev-=0x102;
                unsigned long pos = tbl[prev];
				lnt = length[prev];
                for (int x = 0; x < lnt; x++)
				{
                   unsigned char val = output[pos+x];
                   output[outputPosition++] = val;
				}
			}
            //# set cur. as prev. and loop
            prev = cmd;
		}
        //# add new entry to list
        tbl[c_tbl] = idx;
		length[c_tbl] = lnt+1;
        c_tbl += 1;
        idx += lnt;
        if ((c_tbl+0x103)>(1<<bits))
		{
            bits+=1;
            if (bits>12)
				bits=12;
		}
	}

	return outputPosition;
}