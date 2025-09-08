#include "StdAfx.h"
#include "NaganoDecoder.h"
#include <algorithm>
#include <vector>

// By Zoinkity!!!

CNaganoDecoder::CNaganoDecoder(void)
{
}

CNaganoDecoder::~CNaganoDecoder(void)
{
}

void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

void CNaganoDecoder::KonamiLZW(unsigned char* inputBuffer, int compressedSizeSection, int& inputPosition, unsigned char* output, int& outputPosition)
{
	unsigned long endPosition = (inputPosition + compressedSizeSection - 4);
    while (inputPosition < endPosition)
	{
		unsigned char i = inputBuffer[inputPosition++];

        if (i < 0x80)
		{
            unsigned long b = (i<<8) + inputBuffer[inputPosition++];
            b &= 0x3FF;
            i >>= 2;
            for (int j = 0; j <(i+2); j++)
                output[outputPosition++] = output[outputPosition-b];
		}
        else if (i < 0xA0)
		{
            i &= 0x1F;
            for (int j = 0; j < i; j++)
			{
                output[outputPosition++] = inputBuffer[inputPosition++];
			}
		}
        else if (i < 0xE0)
		{
            i &= 0x1F;
            unsigned char v = inputBuffer[inputPosition++];
			int end = i+2;
            for (int j = 0; j < end; j++)
			{
                output[outputPosition++] = v;
			}
		}
        else if (i < 0xFF)
		{
            i &= 0x1F;
			for (int j = 0; j < (i + 2); j++)
				output[outputPosition++] = 0x00;
		}
        else // if (i == 0xFF)
		{
			int end = inputBuffer[inputPosition++] + 2;
			for (int j = 0; j < end; j++)
				output[outputPosition++] = 0x00;
		}
	}
}

int CNaganoDecoder::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* output)
{
	int inputPosition = 0;
	int outputPosition = 0;

	while (outputPosition < compressedSize)
	{
		if ((compressedSize - outputPosition) <= 0x18)
			break;

        unsigned long size = ((((((inputBuffer[inputPosition] << 8) | inputBuffer[inputPosition+1]) << 8) | inputBuffer[inputPosition+2]) << 8) | inputBuffer[inputPosition+3]);
        unsigned long width  = size >> 28;
        if (width != 0)
		{
            // #Interleaved data.
            inputPosition += 4;
            
            for (int w = 0; w < (width+1); w++)
			{
                size = ((((((inputBuffer[inputPosition] << 8) | inputBuffer[inputPosition+1]) << 8) | inputBuffer[inputPosition+2]) << 8) | inputBuffer[inputPosition+3]);
                size &= 0xFFFFFFF;

				inputPosition += 4;
                KonamiLZW(inputBuffer, size, inputPosition, output, outputPosition);
			}
		}
        else
		{
            size &= 0xFFFFFFF;

			inputPosition += 4;
            KonamiLZW(inputBuffer, size, inputPosition, output, outputPosition);
		}
	}

	return outputPosition;
}

//# Subfunction to return copylengths for comparison during sliding window.
 void samelength(int p, int& length, int& value, unsigned char* data, int sz)
 {
     //"""Determines number of duplicates of char starting at [p]osition.
     //Returns (length, char)"""
     unsigned char c = data[p];
     if (((p+1) >= sz) || (data[p+1] != c))
	 {
		 length = 0;
		 value = c;
         return;
	 }

     int l;
	 if (c != 0)
		l = min(p + 33, sz+1);
	 else
		l = min(p + 257, sz+1);

	 int i = 0;

     for (i = (p + 2); i < l; i++)
	 {
         //# This line bugfixes first zero not recognized as part of set.
         if (i == sz)
             break;
         if (data[i] != c)
             break;
	 }
	 length = (i - p);
	 value = c;
	 return;
 }


bool Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex)
{
    // compare
    for (int i = 0, p = startIndex; i < needleLen; i++, p++)
    {
        if (array[p] != needle[i])
        {
            return false;
        }
    }
    return true;
}

int Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
{
    int index;

    while (startIndex < (sourceLength - needleLen + 1))
    {
        // find needle's starting element
		int index = -1;
		for (int r = startIndex; r < (sourceLength - needleLen + 1); r++)
		{
			if (array[r] == needle[0])
			{
				index = r;
				break;
			}
		}

        // if we did not find even the first element of the needle, then the search is failed
        if (index == -1)
            return -1;

        int i, p;
        // check for needle
        for (i = 0, p = index; i < needleLen; i++, p++)
        {
            if (array[p] != needle[i])
            {
                break;
            }
        }

        if (i == needleLen)
        {
            // needle was found
            return index;
        }

        // continue to search for needle
        //sourceLength -= (index - startIndex + 1);
        startIndex = index + 1;
    }
    return -1;
}

 void copylength(int p, int& length, int& position, unsigned char* data, int sz)
 {
     //"""Determines longest duplicate of sequence starting at [p]osition.
     //Returns (length, position)"""
     if ((p+1) >= sz)
	 {
		 length = 0;
		 position = 0;
         return;
	 }
     int l = 2;
	 std::vector<int> hl;
     int i = 0;
     int c = min(p+0x401, sz);
     while (true)
	 {
		 i = Find(&data[p], &data[p], l, i+1, (c-p));

         if (i < 0)
             break;
         hl.push_back(i + p);
	 }

     if (hl.size() == 0)
	 {
         length = 0;
		 position = 0;
         return;
	 }

     //# Uh, this needs to check if hit EOF...
     while (l < 34)
	 {
         l += 1;

		 std::vector<int> hh;
		 for (i = 0; i < hl.size(); i++)
		 {
			if (Compare(&data[hl[i]], &data[p], l, 0))
			{
				hh.push_back(hl[i]);
			}
		 }

         
         if (hh.size() == 0)
             break;
         hl = hh;
	 }
     l -= 1;

	for (int r = 0; r < hl.size(); r++)
	{
	     c = *std::min_element(hl.begin(), hl.end()) - p;
	}

	length = l;
	position = c;
	return;
 }

 std::vector<int> modella(int start, int length, int enclen, unsigned char* data, int sz)
{
	std::vector<int> ret;
	ret.resize(3);
     //"""Given a start position, length, and encoded command length,
     //returns optimal length for command."""
     //# First figure what your actual hit gets you.
     int oe = enclen;
     int ol = length;
	 int x = 0;
	 int y = 0;
     samelength(start+length, x, y, data, sz);
	 int throwawayPos = 0;
	 int v = 0;
     copylength(start+length, v, throwawayPos, data, sz);
     int e = (y != 0) | (x > 33);
     if (x >= (v+e))
	 {
         oe = enclen + 1 + e;
         ol += x;
	 }
     else if (v != 0)
	 {
         oe += 2;
         ol += v;
	 }
     //# This checks if a shorter initial copy leads to longer copies later.
	 ret[0] = length;
	 ret[1] = ol;
	 ret[2] = oe;

     int f = 1;
     for (int i = 1; i < length; i++)
	 {
		 int te = 0;
		 if (i == 1)
			te = 2;
		 else
			te = enclen;

         int tl = i;

         while ((tl < ol) && (te < oe))
		 {
             samelength(start+tl, x, y, data, sz);

			 int throwadayPos;
             copylength(start+tl, v, throwadayPos, data, sz);

             e = (y == 0) && (x < 33);

             if (f > 30)
                 f = 0;
             if ((x == 0) && (v == 0))
			 {
                 if (f == 0)
                     te += 1;
                 te += 1;
                 tl += 1;
                 f += 1;
                 continue;
			 }
             else if (x >= (v - e))
			 {
				te += 1 + (int)(bool)(y != 0);
                tl += x;
			 }
             else if (v != 0)
			 {
                 te += 2;
                 tl += v;
                 x = v;
                 e = false;
			 }
             if ((0 < f) && (f < 0x1C) && (x == 2) && (e == 0))
			 {
				 int e = 0;
				 int tempValue = 0;
				 samelength(start+tl, e, tempValue, data, sz);

				 int eTemp = 0;
                 copylength(start+tl, e, tempValue, data, sz);

				 e |= eTemp;

                 if (e == 0)
                     f += 2;
                 else
                     f = 0;
			 }
             else
                 f = 0;
		 }

         //# If encoded length shorter for same distance, prefer it.
         if ((te <= oe) && (tl >= ol))
		 {
			 ret[0] = i;
			 ret[1] = tl;
			 ret[2] = te;
		 }
//##        # DEBUGGERY!
//##        if ret[0] != length:
//##            print("Case! 0x{:X}, 0x{:X} encodes to 0x{:X} (0x{:X}, 0x{:X})".format(start, length, ret[0], ret[1], ret[2]))
	 }
     return ret;
}

void flushpush(int l, int p, unsigned char* data, unsigned char* out, int& outputPosition)
{
     if (l == 0)
         return;

	 for (int r = p-l; r < p; r++)
		out[outputPosition++] = data[r];

     out[outputPosition++] = (0x80 | l);
//##        print("\tpush\t{:02X}  \t{:02X}".format(out[-1], l))
}

void flushcopy(int l, int p, unsigned char* out, int& outputPosition)
{
     l -= 2;
     out[outputPosition++] = (p & 0xFF);
     out[outputPosition++] = ((l << 2) | (p >> 8));
//##        print("\tcopy\t{:02X}{:02X}\t{:02X} {:02X}".format(out[-1], out[-2], l+2, p))
}

void flushsame(int l, int c, unsigned char* out, int& outputPosition)
{
     l -= 2;
     if (c != 0)
	 {
		 out[outputPosition++] = c;
         out[outputPosition++] = (0xC0 | l);
	 }
     else if (l > 30)
	 {
         out[outputPosition++] = l;
         out[outputPosition++] = 0xFF;
//##            print("\tcopy\t{:02X}{:02X}\t{:02X} {:02X}".format(out[-1], out[-2], c, l+2))
         return;
	 }
     else
	 {
         out[outputPosition++] = (0xE0 | l);
//##        print("\tsame\t{:02X}  \t{:02X} {:02X}".format(out[-1], c, l+2))
	 }
}


int CNaganoDecoder::EncodeKonamiLZW(unsigned char* inputBuffer, int uncompressedSize, unsigned char* outputBuffer, int outputBufferSize, int& compressedSize, int& compressedRealSize, int level = 9)
{
	unsigned char* out = new unsigned char[outputBufferSize];
	int outputPosition = 0;

     //"""Compresses [data], returning a bytes object.
     //Current compression levels:
         //9   slowest and hopefully most aggressive compresion
         //3   applies one-step lookahead model
         //2   naive compression, optimizing short copies when raw shorter
         //1   naive compression with no attempt at optimization
         //0   spits out raw data in wrappers; probably larger than input
     //"""
     //#    [0]11111.11 11111111	copy u+2 bytes from -l   (33)
     //#    [100]11111	push next n bytes  (31)
     //#    [110]11111. 11111111	push l u+2 times (33)
     //#    [111]11111	push n+2 zeroes    (32)
     //#    [11111111] 11111111	push n+2 zeroes   (257)

	unsigned char* data = new unsigned char[uncompressedSize];
	for (int x = 0; x < uncompressedSize; x++)
	{
		data[x] = inputBuffer[uncompressedSize - x - 1];
	}

	/*FILE* reverse = fopen("C:\\temp\\reverse.bin", "wb");
	fwrite(data, 1, uncompressedSize, reverse);
	fclose(reverse);*/

     int sz = uncompressedSize;
     int pos = 0;
     //# Normalize level.
     if (level > 9)
         level = 9;
     if (level < 0)
         level = 0;

	 
     int pl = 0;
     if (level == 0)
	 {
         //# Stuffs data in wrappers.  Output likely larger than input.
         while (pos < sz)
		 {
			 int sl, ss;
             samelength(pos, sl, ss, data, sz);
             if (sl == 0)
			 {
                 pl += 1;
                 pos += 1;
                 if (pl == 31)
				 {
                     flushpush(pl, pos, data, out, outputPosition);
                     pl = 0;
				 }
                 continue;
			 }
             if (pl != 0)
			 {
                 flushpush(pl, pos, data, out, outputPosition);
                 pl = 0;
			 }
             flushsame(sl, ss, out, outputPosition);
             pos += sl;
		 }
	 }
     else
	 {
         while (pos < sz)
		 {
			 /*FILE* outA = fopen("C:\\temp\\pos.txt", "a");
			 fprintf(outA, "%d\n", pos);
			 fclose(outA);*/
			 int cl = 0;
			 int cs = 0;
			 int sl = 0;
			 int ss = 0;

             //# same-length, copy-length, push-length
             samelength(pos, sl, ss, data, sz);
             copylength(pos, cl, cs, data, sz);
             if ((sl == 0) && (cl == 0))
			 {
                 pl += 1;
                 pos += 1;
                 if (pl == 31)
				 {
                     flushpush(pl, pos, data, out, outputPosition);
                     pl = 0;
				 }
                 continue;
			 }
             //# Check the end of the samelength to see if a copylength can be encoded.
             //# Prefer samelengths since algo decodes them (marginally) faster.
             else if (sl >= cl)
			 {
                 if (level > 2)
				 {
                     if (ss != 0)
					 {
                         sl = modella(pos, sl, 2, data, sz)[0];
					 }
                     //# Test if a shorter sequence results in a 1-byte encode.
                     else if ((32 < sl) && (sl < 65))
					 {
//##                        sl = modella(pos+31, sl-31, 1)
						 std::vector<int> i = modella(pos, 33, 1, data, sz);
                         //# Would be faster with a shorter sequence, but anyone who actually knows how to write a compressor would have thrown all this script out by now.
						 std::vector<int> j = modella(pos, sl, 2, data, sz);
                         if ((i[2] < j[2]) && (i[1] >= j[1]))
                             sl = i[0];
					 }
				 }
                 if (level > 1)
				 {
                     //# If length 2, not pushing zeroes, and not at end of pl, check encoding as raws.
                     if ((sl == 2) && (ss != 0) && (0 < pl) && (pl < 0x1C) && ((pos+sl) < sz))
					 {
						 int tempValue = 0;
						 int i = 0;
                         samelength(pos+2, i, tempValue, data, sz);

                         int tempI = 0;
						 copylength(pos+2, tempI, tempValue, data, sz);
						 i |= tempI;

                         if (i == 0)
						 {
                             pl += 2;
                             pos += 2;
                             continue;
						 }
					 }
				 }
                 if (sl == 1)
				 {
                     pl += 1;
                     pos += 1;
                     continue;
				 }
                 //# Encode this command; allow next pass to handle the next one.
                 if (pl != 0)
				 {
                     flushpush(pl, pos, data, out, outputPosition);
                     pl = 0;
				 }
                 flushsame(sl, ss, out, outputPosition);
                 pos += sl;
             //# Check copylength if samelengths can be encoded to produce a shorter total sequence.
			 }
             else
			 {
                 if (level > 2)
				 {
                     //# Check copylength if samelengths can be encoded to produce a shorter total sequence.
                     cl = modella(pos, cl, 2, data, sz)[0];
				 }
                 if (level > 1)
				 {
                     //# If length 2 and not at end of pl, check encoding as raws.
                     if ((cl == 2) && (0 < pl) && (pl < 0x1C) && ((pos+pl) < sz))
					 {
						 int i = 0;
						 int tempValue;
                         samelength(pos+2, i, tempValue, data, sz);

						 int tempI;
                         copylength(pos+2, tempI, tempValue, data, sz);

						 i |= tempI;
                         if (i == 0)
						 {
                             pl += 2;
                             pos += 2;
                             continue;
						 }
					 }
				 }
                 if (cl == 1)
				 {
                     pl += 1;
                     pos += 1;
                     continue;
				 }
                 //# Encode this command; allow next pass to handle the next one.
                 if (pl != 0)
				 {
                     flushpush(pl, pos, data, out, outputPosition);
                     pl = 0;
				 }
                 flushcopy(cl, cs, out, outputPosition);
                 pos += cl;
			 }
		 }
	 }
     //# Flush any remaining bytes.
     if (pl != 0)
         flushpush(pl, pos, data, out, outputPosition);

	 for (int x = 0; x < outputPosition; x++)
		 outputBuffer[x + 4] = out[outputPosition - x - 1];

	 outputPosition += 4;

	 WriteLongToBuffer(outputBuffer, 0, outputPosition);

	 compressedRealSize = outputPosition;

	 int pad = ((outputPosition) % 0x10);
	 if (pad != 0)
	 {
		for (int x = 0; x < (0x10 - pad); x++)
			outputBuffer[outputPosition++] = 0x00;
	 }

	 delete [] data;

	 compressedSize = outputPosition;

	 delete [] out;
     return compressedSize;
}