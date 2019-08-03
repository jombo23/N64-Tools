#include "StdAfx.h"
#include "AidynDecoder.h"
#include "H20Decoder.h"

CAidynDecoder::CAidynDecoder(void)
{
}

CAidynDecoder::~CAidynDecoder(void)
{
}

unsigned short CAidynDecoder::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CAidynDecoder::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

//# Attempts to refill quietly in the background.
    //# Pretty sure that falls under "voodoo magic"
unsigned long CAidynDecoder::bit(unsigned char* data, int& dataPosition, int dataLength, unsigned long cmdlen, unsigned long& cmd, CString mode)
{
    //# If not an iterator, make it one.
    unsigned long mask = 1<<(4<<cmdlen);
    mask -= 1;
    unsigned long hit  = 1<<((4<<cmdlen)-1);

    //# Turn cmdlen into #bytes to extract
    if (cmdlen == 3)
        cmdlen = 4;

	unsigned long v = 0;

    //# Pretty sure this is a crummy way to write this, but screw it.
    if (mode == "cmd")
	{
        if (cmd == hit)
		{
            cmd = 0;
            //# freakin' little endian!
            for (int i = 0; i < cmdlen; i++)
			{
				v = data[dataPosition++];
                v<<= (i<<3);
                cmd|= v;
			}
            cmd<<=1;
            cmd+= 1;
		}
        else
		{
            cmd<<=1;
		}
        v = cmd>>8;
        cmd&=mask;
	}
    else if (mode == "byte")
	{
        v = data[dataPosition++];
	}
    else if (mode == "init")
	{
        cmd = hit;
        v = 0;
	}
        
	return v;
}

int CAidynDecoder::decAidyn(unsigned char* data, int dataLength, unsigned char* output, int& outputPosition, int cmdlen)
{
    //"""LZB, using binary command registers of variable length.
    //As far as I can tell they only actually used the 8bit one,
    //    but they included decompressors for 16 and 32bit as well.

    //If cmdlen = 1, each cmd is 8bit
    //if cmdlen = 2, each cmd is 16bit (LE)
    //if cmdlen = 3, each cmd is 32bit (LE)
    //TODO: The generator could use some work."""

    //# Create the generator and initialize it.
    int base = 1;
	outputPosition = 0;
	unsigned long cmd = 0;

	int inputPosition = 0;

	bit(data, inputPosition, dataLength, cmdlen, cmd, "init");

    while (true)
	{
        if (bit(data, inputPosition, dataLength, cmdlen, cmd, "cmd") != 0)
		{
            output[outputPosition++] = bit(data, inputPosition, dataLength, cmdlen, cmd, "byte");
		}
        else
		{
            unsigned long back = 1;
            bool b = false;
            while (!b)
			{
                back<<=1;
                back += bit(data, inputPosition, dataLength, cmdlen, cmd, "cmd");
                b = (bool)bit(data, inputPosition, dataLength, cmdlen, cmd, "cmd");
			}
            if (back == 2)
                back = base;
            else
			{
                back -= 3;
                back<<= 8;
                back |= bit(data, inputPosition, dataLength, cmdlen, cmd, "byte");
                //# if 0xFFFF, stop decompression.
                if (back == 0xFFFFFFFF)
                    break;
                back+=1;
                base = back;
			}
            unsigned long l = bit(data, inputPosition, dataLength, cmdlen, cmd, "cmd")<<1;
            l+= bit(data, inputPosition, dataLength, cmdlen, cmd, "cmd");
            if (!l)
			{
                l = 1;
                b = false;
                while (!b)
				{
                    l<<= 1;
                    l += bit(data, inputPosition, dataLength, cmdlen, cmd, "cmd");
                    b = bit(data, inputPosition, dataLength, cmdlen, cmd, "cmd");
				}
                l+=2;
			}
            //# Add one if backtrack not less than 0xD01
            l+= (back>0xD00);
            l+=1;
            for (int i = 0; i < l; i++)
                output[outputPosition++] = output[outputPosition-back];
		}
	}
    //# Return an immutable object.
	return outputPosition;
}

unsigned char* CAidynDecoder::DecompressAidynFile(unsigned char* ROM, int fileTableAddress, int startDataAddress, int fileID, int& decompressedSize)
{
	int fileTableEntry = fileTableAddress + 8 + (fileID * 0x10);

	unsigned char type =  ROM[fileTableEntry + 1];
	unsigned char compressionType =  ROM[fileTableEntry + 3];
	unsigned long compressedLength = CharArrayToLong(&ROM[fileTableEntry + 4]);
	unsigned long decompressedLength = CharArrayToLong(&ROM[fileTableEntry + 8]);
	unsigned long offset = CharArrayToLong(&ROM[fileTableEntry + 0xC]);

	if (compressionType == 0)
	{
		// Assume uncompressed
		unsigned char* newOutput = new unsigned char[decompressedLength];
		memcpy(newOutput, &ROM[startDataAddress + offset], decompressedLength);
		decompressedSize = decompressedLength;
		return newOutput;
	}
	else if (compressionType == 1)
	{
		unsigned char* output = new unsigned char[0x100000];
		int outputPosition = 0;
		CH20DecoderMidiTool h20Dec;
		outputPosition = h20Dec.decPolaris(&ROM[startDataAddress + offset], compressedLength, output, true);

		unsigned char* newOutput = new unsigned char[outputPosition];
		memcpy(newOutput, output, outputPosition);
		delete [] output;

		decompressedSize = outputPosition;
		return newOutput;
	}
	else if (compressionType == 2)
	{
		unsigned char* output = new unsigned char[0x100000];
		int outputPosition = 0;
		if (decAidyn(&ROM[startDataAddress + offset], compressedLength, output, outputPosition) == -1)
		{
			decompressedSize = -1;
			return NULL;
		}

		unsigned char* newOutput = new unsigned char[outputPosition];
		memcpy(newOutput, output, outputPosition);
		delete [] output;

		decompressedSize = outputPosition;
		return newOutput;
	}
	else
	{
		return NULL;
	}
}