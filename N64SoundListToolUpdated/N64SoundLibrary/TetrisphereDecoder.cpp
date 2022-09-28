#include "StdAfx.h"
#include "TetrisphereDecoder.h"

CTetrisphereDecoder::CTetrisphereDecoder(void)
{
}

CTetrisphereDecoder::~CTetrisphereDecoder(void)
{
}

int CTetrisphereDecoder::decompressLZ(unsigned char* inputBuffer, int compressedSize, unsigned char* output, bool initialize = true)
{
	int inputPosition = 0;
	int outputPosition = 0;

    unsigned long r = 0;
	unsigned long p = 0;
	unsigned long sz = compressedSize;

    unsigned char ring[0x1000];
	for (int x = 0; x < 0x1000; x++)
		ring[x] = 0x00; // probably python initializes to 0

    if (initialize)
	{
        //# There is undoubtedly a better way to do this.
        for (int i = 0; i < 0x100; i++)
		{
            ring[i]=i;
            ring[511-i]=i;
            ring[0x200+(i<<2)]=i;
            ring[0x201+(i<<2)]=i;
            ring[0x202+(i<<2)]=i;
            ring[0x203+(i<<2)]=i;
		}
		for (int i = 0x600, j = 0x0; i < 0x700; i++, j++)
		{
			ring[i] = ring[j];
		}
		for (int i = 0x700, j = 0x600; i < 0x800; i++, j++)
		{
			ring[i] = ring[j];
		}
		for (int i = 0x800, j = 0x600; i < 0xA00; i++, j++)
		{
			ring[i] = ring[j];
		}
		for (int i = 0xA00, j = 0x600; i < 0xC00; i++, j++)
		{
			ring[i] = ring[j];
		}
		for (int i = 0xC00, j = 0x800; i < 0x1000; i++, j++)
		{
			ring[i] = ring[j];
		}
	}

    while(p<sz)
	{
        int c = inputBuffer[p];
        int l = c>>4;
        p+=1;
        if (l)
		{
            unsigned long b = ((c&0xF)<<8);
            b|= inputBuffer[p];
            p+=1;
            for (int i = 0; i <(l+2); i++)
			{
				output[outputPosition++] = ring[(b+i)&0xFFF];
                ring[r] = output[outputPosition-1];
                r = outputPosition&0xFFF;
			}
		}
        else
		{
            c+=1;

			for (int x = p; x < (p+c); x++)
			{
				output[outputPosition++] = inputBuffer[x];
			}

            //# Ring management.
            l = 0x1000 - r;
            if (l < c)
			{
				for (int i = r, j = p; i < 0x1000; i++, j++)
				{
					ring[i] = inputBuffer[j];
				}
                for (int i = 0, j = p+l; i < (c-l); i++, j++)
				{
					ring[i] = inputBuffer[j];
				}
			}
            else
			{
				for (int i = r, j = p; i < (r+c); i++, j++)
				{
					ring[i] = inputBuffer[j];
				}
			}
            p+=c;
			r = outputPosition&0xFFF;
		}
	}
    return outputPosition;
}

int CTetrisphereDecoder::decompressRLE(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

    int p = 0;
	int l = compressedSize;

    while(p<l)
	{
		unsigned short c = ((inputBuffer[p] << 8) | inputBuffer[p+1]);
        p+=2;
        if (c & 0x8000)
		{
            c&= 0x7FFF;
			for (int i = 0; i < c; i++)
			{
				output[outputPosition++] = inputBuffer[p];
			}
            p+=1;
		}
        else
		{
			for (int i = p; i < (p+c); i++)
			{
				output[outputPosition++] = inputBuffer[i];
			}
            p+=c;
		}
	}
    return outputPosition;
}

//def decompressHuffman(data):
    //# Bit of an inside joke here ;*)
    //print("Idiot!\nDon't use HuffMan!")
    //return bytes(data)

int CTetrisphereDecoder::decode(unsigned char* inputBuffer, int compressedSize, unsigned char* output, int back=1)
{
	int inputPosition = 0;
	int outputPosition = 0;

    //"""Runs the decode scheme on data.
    //Setting back to 1 is equivalent to 'D'; set back=2 for 'F'
    //"""
    int l = compressedSize;

	for (int i = 0; i < back; i++)
	{
		output[outputPosition++] = inputBuffer[i];
	}

    for (int i = back; i < l; i++)
	{
        int v = output[outputPosition-back] - inputBuffer[i];
        output[outputPosition++] = v&0xFF;
	}
    return outputPosition;
}

bool CTetrisphereDecoder::sqshHeader(unsigned char* inputBuffer, int& compressedSize, int& uncompressedSize)
{
	if ((inputBuffer[0] != 0x53) || (inputBuffer[1] != 0x51) || (inputBuffer[2] != 0x53) || (inputBuffer[3] != 0x48))
        return false;

    uncompressedSize = ((((((inputBuffer[4] << 8) | inputBuffer[5]) << 8) | inputBuffer[6]) << 8) | inputBuffer[7]);
    compressedSize = ((((((inputBuffer[8] << 8) | inputBuffer[9]) << 8) | inputBuffer[0xA]) << 8) | inputBuffer[0xB]);
    return true;
}

int CTetrisphereDecoder::sqshDecompress(unsigned char* inputBuffer, int compressedSize, int expectedSize, unsigned char* output)
{
	int outputPosition = 0;

    //"""Decompress data, returning a bytes object.
    //If update is True, replaces self.data with decompressed
        //data if decompression successful."""
    if (compressedSize == expectedSize)
	{
        memcpy(output, inputBuffer, expectedSize);
		return expectedSize;
	}

    //# This is handled as an error, since you muddled up the state.
    
	std::vector<unsigned short> shortArray;

    int p = 0;
    while (p < compressedSize)
	{
        int c = inputBuffer[p];
        p+= 1;
        if (c&0x80)
		{
            c &= 0x7F;
            unsigned short b = inputBuffer[p] << 8;
            b|= inputBuffer[p+1];
            p+= 2;
            for (int i = 0; i < (c+1); i++)
			{
                shortArray.push_back(shortArray[b+i]);
			}
		}
        else
		{
            c+= 1;
            int b = c<<1;
			for (int i = p; i < (p+b); i+=2)
			{
				shortArray.push_back((inputBuffer[i] << 8) | inputBuffer[i+1]);
			}
            p+= b;
		}
	}

    return expectedSize;
}

int CTetrisphereDecoder::sphereRead(std::vector<unsigned char>& state, unsigned char* inputBuffer, int compressedSize, unsigned char* output, bool writeback=false)
{
    //"""Performs next command on data, returning the result as a bytes object.
    //If [writeback] is True, saves the result to self."""
	int returnSize = compressedSize;

	if (state.size() == 0)
		return compressedSize;
    
	char c = (char)state[0];

    if (c == 'D')
        returnSize = decode(inputBuffer, compressedSize, output);
    else if (c == 'F')
        returnSize = decode(inputBuffer, compressedSize, output, 2);
    else if (c == 'S')
        returnSize = decompressLZ(inputBuffer, compressedSize, output, true);
    else if (c == 'R')
        returnSize = decompressRLE(inputBuffer, compressedSize, output);
    //else if (c == 'H')
        //decompressHuffman(inputBuffer);
    else if (c == 'J')
	{
		int expectedSize;
		sqshHeader(inputBuffer, compressedSize, expectedSize);
        returnSize = sqshDecompress(&inputBuffer[0xC], compressedSize, expectedSize, output);
	}

    if (writeback)
	{
		state.erase(state.begin());
	}

	return returnSize;
}

int CTetrisphereDecoder::sphereDecompress(unsigned char* inputBuffer, unsigned char* output)
{
	int c = (inputBuffer[0]+1);
	std::vector<unsigned char> state;
	for (int x = 1; x < (c+1); x++)
	{
		if (inputBuffer[x] == 0)
			break;
		state.push_back(inputBuffer[x]);
	}
    int l = ((((((inputBuffer[c] << 8) | inputBuffer[c+1]) << 8) | inputBuffer[c+2]) << 8) | inputBuffer[c+3]);
    c+=4;

    //return cls(cmds, data[c:c+l])

	inputBuffer = &inputBuffer[c];

	unsigned char* inputBufferTemp = new unsigned char[0x2000000];
	memcpy(inputBufferTemp, inputBuffer, l);
	memcpy(output, inputBufferTemp, l);

	int readSize = l;

    //"""Fully decompressed [data], calling read() repeatedly
    //until file is completely uncompressed."""
    while (state.size() > 0)
	{	
        readSize = sphereRead(state, inputBufferTemp, readSize, output, true);
		
		memcpy(inputBufferTemp, output, readSize);
	}        

	delete [] inputBufferTemp;

	return readSize;
}
