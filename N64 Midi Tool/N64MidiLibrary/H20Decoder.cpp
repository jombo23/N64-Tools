#include "StdAfx.h"
#include "H20Decoder.h"

CH20Decoder::CH20Decoder(void)
{
}

CH20Decoder::~CH20Decoder(void)
{
}

int CH20Decoder::decPolaris(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	//"""Wrapper containing either uncompressed or LZO1x files."""
    if ((inputBuffer[0] != 0x48) || (inputBuffer[1] != 0x32) || (inputBuffer[2] != 0x4F)) // H2O
        return 0;

    //# Contains the decompressed size, which we won't putz with.
    int dec = ((((((inputBuffer[4] << 8) | inputBuffer[5]) << 8) | inputBuffer[6]) << 8) | inputBuffer[7]);
	unsigned char* adjustData;

	int inputPosition = 8;

	if (inputBuffer[3] == 'N')
	{
		memcpy(output, &inputBuffer[inputPosition], dec);
		return dec;
	}
	
	
	int outputPosition = 0;

	

    //"""Decompresses files from SnoCross, which looks an awful lot like LZO1x.
    //Returns the decompressed data as a bytes object.

    //Slight modification to lzo1x, particularly when it does long backtracking."""

    //# First byte is handled kinda special.
    unsigned long c = inputBuffer[inputPosition++];
    unsigned long l = c - 17;
	unsigned long f;
	unsigned long o;
	unsigned long p;
	unsigned long s;

    if (c < 18)
	{
        f = 0;
		s = false;
	}
    else
	{
        for (int i = 0; i < l; i++)
		{
			output[outputPosition++] = inputBuffer[inputPosition++];
		}
        
		f = 1;
        s = c>21;
        c = inputBuffer[inputPosition++];
	}

    //# Run until the iterator is exhausted or you hit an explicit EOF.
    while (true)
	{
        //# Write portion
        if (!f && (c < 16))
		{
            //# 0001llll ...
            l = 3;
            if (!c)
			{
                l+= 15;
                c = inputBuffer[inputPosition++];
			}
            while (!c)
			{
                c = inputBuffer[inputPosition++];
                l += 255;
			}
            l+= c;
            for (int i = 0; i < l; i++)
			{
                output[outputPosition++] = inputBuffer[inputPosition++];
			}
            //# Copy portion special case, backtracking -0x800 in this case only
            //# Weird rule if ever there was one.
            c = inputBuffer[inputPosition++];
            if (c < 16)
			{
                s = true;
			}
		}

        //# Copy portion
        if (c < 16)
		{
            //# 0000bbww BBBBBBBB
            o = c>>2;
            if (s)
			{
                o+= 0x800;
			}
            o += (inputBuffer[inputPosition++] << 2);
			p = outputPosition - o;
            l = 2;
            s = false;
		}
		else if (c < 32)
		{
            //# 001.Llll ... ooooooww OOOOOOOO
            l = 1;
            o = (c&8) << 11;
            c &= 7;
            if (!c)
			{
                l+=7;
                c = inputBuffer[inputPosition++];
			}
            while (!c)
			{
                l+=255;
                c = inputBuffer[inputPosition++];
			}
            l+=c;
            c = inputBuffer[inputPosition++];
            o|= (inputBuffer[inputPosition++]<<6);
            o|= (c>>2);
            //# If no offset, you're at end of file.
            if (!o)
			{
                break;
			}
            o+= 0x3FFF;
		}
        else if (c < 64)
		{
            //# 01.lllll ... bbbbbbww BBBBBBBB
            l = 1;
            c &= 0x1F;
            if (!c)
			{
                l+=0x1F;
                c = inputBuffer[inputPosition++];
			}
            while (!c)
			{
                l+=255;
                c = inputBuffer[inputPosition++];
			}
            l+=c;
            c = inputBuffer[inputPosition++];
            o = (inputBuffer[inputPosition++]<<6);
            o|= (c>>2);
		}
        else
		{
            //# lllbbbww BBBBBBBB
            o = (c>>2);
            o &= 7;
            o|= (inputBuffer[inputPosition++]<<3);
            l = c>>5;
		}
        //# Copying is the same regardless
        l+= 1;
		p = outputPosition - o - 1;
        for (int i = 0; i < l; i++)
		{
            output[outputPosition++] = output[p+i];
		}
        //# Optional write; when taken, don't do a write as usual.
        f = c & 3;
        for (int i = 0; i < f; i++)
		{
            output[outputPosition++] = inputBuffer[inputPosition++];
		}
        c = inputBuffer[inputPosition++];
	}

    //# Shuffle output into a bytes object to prevent alteration.
    return dec;
}
