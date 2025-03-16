#include "StdAfx.h"
#include "EDLCompress.h"
#include <vector>
#include <algorithm>

unsigned char tbl1[] = {0,1,2,3,4,5,6,7,8,0xA,0xC,0xE,0x10,0x14,0x18,0x1C,0x20,0x28,0x30,0x38,0x40,0x50,0x60,0x70,0x80,0xA0,0xC0,0xE0,0xFF,0,0,0};
unsigned char tbl2[] = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0,0};
unsigned short tbl3[] = {0,1,2,3,4,6,8,0xC,0x10,0x18,0x20,0x30,0x40,0x60,0x80,0xC0,0x100,0x180,0x200,0x300,0x400,0x600,0x800,0xC00,0x1000,0x1800,0x2000,0x3000,0x4000,0x6000};
unsigned char tbl4[] = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,0xA,0xA,0xB,0xB,0xC,0xC,0xD,0xD,0,0};


unsigned long CEDLCompress::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned long CEDLCompress::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

void CEDLCompress::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

std::vector<unsigned long> CEDLCompress::_grab(unsigned char* data, int dataSize, CString endian, bool padZero)
{
    //"""Iterates stream data using either 'big' or 'little' endian order.
    //The mode can be either 'read' or 'peek'.
    //By default, n bits are read off of the stream and the stream is advanced.
    //In 'peek' mode, the bits are read but the stream is not advanced."""
    unsigned long c = 0;
	unsigned long b = 0;
	std::vector<unsigned long> src;
	for (int x = 0; x < dataSize; x += 4)
	{
		if (endian == "big")
		{
			src.push_back(CharArrayToLong(&data[x]));
		}
		else
		{
			src.push_back(Flip32Bit(CharArrayToLong(&data[x])));
		}
	}
	if (padZero)
		src.push_back(0);
	return src;
}

unsigned long CEDLCompress::next(std::vector<unsigned long>& src, int bits, CString mode, unsigned __int64& v, unsigned __int64& b, unsigned long& c)
{
	while (c>=bits)
	{
        unsigned __int64 m = (1<<bits)-1; //# mask
        v = b&m;         //# value = bitfield & mask
        if (mode != "peek")
		{
            c-= bits;        //# count-= #bits
            b>>=bits;        //# bitfield without said bits
		}
		return (unsigned long)v;
	}

	v = src.front();
	src.erase(src.begin());

    v<<=c;
    b|=v;     //# bitfield | new bits<<count
    c+=32;    //# count+=32

    while (c>=bits)
	{
        unsigned __int64 m = (1<<bits)-1; //# mask
        v = b&m;         //# value = bitfield & mask
        if (mode != "peek")
		{
            c-= bits;        //# count-= #bits
            b>>=bits;        //# bitfield without said bits
		}
		return (unsigned long)v;
	}
}

std::vector<unsigned short> _fillBuffer(std::vector<unsigned char>& buffer, size_t size)
{
    std::vector<unsigned short> table;
	for (int x = 0; x < 0x600; x++)
		table.push_back(0);

	std::vector<int> when;
	std::vector<int> n;
	n.push_back(0);

    //# build occurance table, sorted by bits
    //# num = len(buffer)
    for (int y = 1; y < 16; y++)
	{
		int bufferCount = 0;
		for (int c = 0; c < buffer.size(); c++)
		{
			if (buffer[c] == y)
				bufferCount++;
		}
        n.push_back(bufferCount);
        int x = -1;
        while (true)
		{
			bool found = false;
			for (int c = (x + 1); c < buffer.size(); c++)
			{
				if (buffer[c] == y)
				{
					found = true;
					x = c;
					break;
				}
			}
			if (!found)
				x = -1;
			
            if (x < 0)
                break;
            when.push_back(x);
		}
	}
    //# Now you have the count of entries.
	int count = 0;
	for (int c = 0; c < n.size(); c++)
	{
		count += n[c];
	}

	std::vector<unsigned short> samp;
	for (int c = 0; c < count; c++)
	{
		samp.push_back(0);
	}

    //# set counts in the buffer
    int i=0;
    for (int y = 0; y < 16; y++)
	{
        for (int x = n[y]; x > 0; x--)
		{
            buffer[i]=y;
            i+=1;
		}
	}

    //# generate bitsample table
    unsigned long z = buffer[0];
	int back = 0;
    for (int x = 0; x < count; x++)
	{
        unsigned long y = buffer[x];
        if (y!=z)
		{
            z = y-z;
            back*=(1<<z);
            z=y;
		}
        y = (1<<y) | back;
        back+=1;
        while (true)
		{
            samp[x]<<=1;
            samp[x]+=(y&1);
            //# Enforce short value.
            samp[x]&=0xFFFF;
            y>>=1;
            if (y==1)
                break;
		}
	}

    //# fills temp buffer with formatted bitsamples
    //#b    = list(bytes(1<<size))
	std::vector<unsigned char> b;
	for (int c = 0; c < (1 << size); c++)
		b.push_back(0);

    for (int x = 0; x < count; x++)
	{
        back = buffer[x];
        if (back<size)
		{
            unsigned long y = 1<<back;
            z = samp[x];
            while (true)
			{
                table[z] = (when[x]<<7) + buffer[x];
                z+=y;
                if (z>>size)
                    break;
			}
		}
        else
		{
            unsigned long y = (1<<size)-1;
            z = samp[x] & y;
            b[z] = buffer[x];
		}
	}

    //# Read coded types > size
    z = 0;
	int x = 0;
    while (!(x>>size))
	{
        int y = b[x];
        if (y)
		{
            y-=size;
            if (y>8)
			{
				MessageBox(NULL, "Got error y < 8", "Error", NULL);
				return table;
			}
            back = (z<<7) + (y<<4);
            table[x] = back;
            z+=(1<<y);
		}
        x+=1;
	}

    if (z>0x1FF)
	{
		MessageBox(NULL, "Got error z > 0x1FF", "Error", NULL);
		return table;
	}

    //# Aliased entries
    back = 1<<size;
    for (int x = 0; x < count; x++)
	{
        if (buffer[x]<size)
            continue;
        z = samp[x] & (back-1);
        z = table[z];
        int y = samp[x]>>size;
        while (true)
		{
            int i = y + (z>>7) + (1<<size);
            table[i] = (when[x]<<7) + buffer[x];
            i = buffer[x]-size;
            y+= (1<<i);
            i = (z>>4)&7;
            if (y>>i)
                break;
		}
	}
    return table;
}

HeaderData CEDLCompress::header(unsigned char* data)
{
    //"""Retrieves format and swapping from header.
    //Returns type(0-2), endianness (str), compressed size, decompressed size"""
	HeaderData headerData;
    char field0[4];
	memcpy(field0, data, 3);
	field0[3] = 0;
	unsigned char field1 = data[3];

	CString endian;
	unsigned long cmp_s;
	unsigned long dec_s;
	if (strcmp(field0, "EDL") == 0)
	{
        //# Use embedded size.
		if ((field1 & 0x80) != 0)
		{
			cmp_s = CharArrayToLong(&data[4]);
			dec_s = CharArrayToLong(&data[8]);
			endian = "big";
		}
		else
		{
			cmp_s = Flip32Bit(CharArrayToLong(&data[4]));
			dec_s = Flip32Bit(CharArrayToLong(&data[8]));
			endian = "little";
		}
	}
	else
	{
		// BAD, needs to be EDL
		field1 = 0xFF;
	}
    //## grab compressed and decompressed sizes
    return HeaderData((field1 & 0xF), endian, cmp_s, dec_s);
}


//# TODO: smallList files okay; some conditional wrong here!
int CEDLCompress::_decEDL1(CString endian, unsigned char* data, int cmp_s, int dec_s, unsigned char* outputDecompressed)
{
    //"""LZW variant: generate bittables and decompress."""
    int outputDecompressedSize = 0;
    //# initialize the data stream
    //# This is not a dirty hack (of my own)!
    //# They initialize the block beforehand and do not store hanging zeroes.
	std::vector<unsigned long> d = _grab(data, cmp_s, endian, true);
	unsigned __int64 v = 0;
	unsigned __int64 b = 0;
	unsigned long c = 0;
    //# initializing these isn't absolutely necessary
    //# smallList, largeList = list(bytes(0x600)), list(bytes(0x600))
	std::vector<unsigned short> largeList;
	std::vector<unsigned short> smallList;
    unsigned long n = 0;   //# This is a global!
    //# alternates between raw and compressed output
    while (outputDecompressedSize<dec_s)
	{
        if (next(d, 1, "read", v, b, c))
		{
            //# yeah, should probably use a subroutine...
            //# largeList table
            unsigned long x = next(d, 9, "read", v, b, c);
            if (x != 0)
			{
                //# build an empty buffer
				std::vector<unsigned char> buf;
                //# iterate to grab entries
                for (int i = 0; i < x; i++)
				{
                    if (next(d, 1, "read", v, b, c))
                        n = next(d, 4, "read", v, b, c);
                    buf.push_back(n);
				}
                largeList = _fillBuffer(buf, 10);
			}
            //# smallList table
            x = next(d, 9, "read", v, b, c);
            if (x)
			{
                //# build an empty buffer
                std::vector<unsigned char> buf;
                //# iterate to grab entries
                for (int i = 0; i < x; i++)
				{
                    if (next(d, 1, "read", v, b, c))
                        n = next(d, 4, "read", v, b, c);
                    buf.push_back(n);
				}
                smallList = _fillBuffer(buf, 8);
			}
            //# write data using tables
            x = 0;
            while (x!=0x100)
			{
                x = largeList[next(d, 10, "peek", v, b, c)];
                if (!(x&0xF))
				{
                    //# grab reference
                    unsigned long m = (x>>4)&7;
                    x >>= 7;
                    v = next(d, 10+m, "peek", v, b, c);
                    v >>=10;
                    x = largeList[x + v + 0x400];
				}
                //# Advance the stream the appropriate bitcount
                next(d, (x & 0xF), "read", v, b, c);
                x>>=7;
                if (x<0x100)     //# Write byte to output
					outputDecompressed[outputDecompressedSize++] = (unsigned char)x;
                else if (x>0x100)   //# Copy previous
				{
                    //# Determine length
                    unsigned long l = 0;
                    int z = tbl2[x-0x101];
                    if (z)
					{
                        l = next(d, z, "read", v, b, c);
					}
                    l+= tbl1[x-0x101] + 3;
                    //# Determine backtrack
                    x = smallList[next(d, 8, "peek", v, b, c)];
                    if (!(x&0xF))
					{
                        //# grab reference
                        int m = (x>>4)&7;
                        x>>=7;
                        v = next(d, 8+m, "peek", v, b, c);
                        v>>=8;
                        x = smallList[x + v + 0x100];
					};
                    //# Advance the stream the appropriate bits.
                    next(d, x&0xF, "read", v, b, c);
                    //# Position from next sample
                    unsigned long p = 0;
                    x>>=7;
                    z = tbl4[x];
                    if (z)
					{
                        p = next(d, z, "read", v, b, c);
					}
                    p+= tbl3[x] + 1;
                    //# Copy and append l bytes from cur-p
					p = outputDecompressedSize - p;
                    for (int i = 0; i < l; i++)
					{
						if (((p+i) >= 0) && ((p + i) < outputDecompressedSize))
							v = outputDecompressed[p + i];
						else
							v = 0;
						outputDecompressed[outputDecompressedSize++] = v;
					}
				}
			}
		}
        else
		{
            //# direct write
            int num = next(d, 15, "read", v, b, c);
            for (int i = 0; i < num; i++)
			{
				outputDecompressed[outputDecompressedSize++] = next(d, 8, "read", v, b, c);
			}
		}
        //# EOF mark is a set bit
        if (next(d, 1, "read", v, b, c))
            break;
	}
    return outputDecompressedSize;
}

int CEDLCompress::_decEDL2(CString endian, unsigned char* data, int cmp_s, int dec_s, unsigned char* outputDecompressed)
{
    //"""RLE: uses bitcodes to handle RLE output."""
    //# append 0x1000 NULLs to simulate the buffer
	for (int x = 0; x < 0x1000; x++)
		outputDecompressed[x] = 0x00;
    int outputDecompressedSize = 0x1000;
    dec_s += 0x1000;

    //# initialize the data stream
    std::vector<unsigned long> d = _grab(data, cmp_s, endian, true);
	unsigned __int64 v = 0;
	unsigned __int64 b = 0;
	unsigned long c = 0;

    while (outputDecompressedSize < dec_s)
	{
        if (next(d, 1, "read", v, b, c))
		{
            int num = next(d, 2, "read", v, b, c);
            int back = 0;
            if (num==0)
			{
                num = 4;
                if (next(d, 1, "read", v, b, c))
				{
                    //# 6 or 7
					if (next(d, 1, "read", v, b, c))
						num = 7;
					else
						num = 6;
				}
			}
            else if (num==1)
			{
                //# no offseting
                num = 2;
                back = 1;
			}
            else if (num==2)
			{
                num = 5;
                if (next(d, 1, "read", v, b, c))
                    if (next(d, 1, "read", v, b, c))
					{
                        //# writes x+3 words
                        num = next(d, 4, "read", v, b, c) + 3;
						for (int i = 0; i < (num << 2); i++)
							outputDecompressed[outputDecompressedSize++] = next(d, 8, "read", v, b, c);
                        continue;
					}
                    else
					{
                        num = 8;
					}
			}
            else if (num==3)
			{
                if (next(d, 1, "read", v, b, c))
				{
                    num = next(d, 8, "read", v, b, c);
                    //# 1111.00000000:    terminate code
                    if (!num)
                        break;
                    num+=8;
				}
			}
            //# fetch offset
            if (!back)
			{
                if (next(d, 1, "read", v, b, c))
				{
                    back = next(d, 2, "read", v, b, c);
                    if (back==0)
					{
						if (next(d, 1, "read", v, b, c))
							back = 0x300;
						else
							back = 0x200;
					}
                    else if (back==1)
					{
                        back = 0x100;
					}
                    else if (back==2)
					{
                        back = next(d, 2, "read", v, b, c);
                        if (back==0)
						{
							if (next(d, 1, "read", v, b, c))
								back = 0x900;
							else
								back = 0x800;
						}
                        else if (back==1)
						{
							if (next(d, 1, "read", v, b, c))
								back = 0xB00;
							else
								back = 0xA00;
						}
                        else if (back==2)
						{
                            back = 0x400;
						}
                        else if (back==3)
						{
                            back = 0x500;
						}
					}
                    else if (back==3)
					{
                        back = next(d, 2, "read", v, b, c);
                        if (back==0)
						{
							if (next(d, 1, "read", v, b, c))
								back = 0xD00;
							else
								back = 0xC00;
						}
                        else if (back==1)
						{
							if (next(d, 1, "read", v, b, c))
								back = 0xF00;
							else
								back = 0xE00;
						}
                        else if (back==2)
						{
                            back=0x600;
						}
                        else if (back==3)
						{
                            back=0x700;
						}
					}
				}
                back+=1;
			}
            //# use next 8 bits for backtrack, then copy
            back+= next(d, 8, "read", v, b, c);
            for (int i = 0; i < num; i++)
			{
				v = outputDecompressed[outputDecompressedSize-back];
				outputDecompressed[outputDecompressedSize++] = v;
			}
		}
        else
		{
            //# push single byte
			outputDecompressed[outputDecompressedSize++] = next(d, 8, "read", v, b, c);
		}
	}

    //# limit output to expected size
	for (int x = 0; x < (outputDecompressedSize - 0x1000); x++)
	{
		outputDecompressed[x] = outputDecompressed[x + 0x1000];
	}
	return outputDecompressedSize - 0x1000;
}

int CEDLCompress::decode(unsigned char* data, int& compressedSize, unsigned char* outputDecompressed)
{
    //"""Decompresses EDL bytes-like object, returning bytes."""
    HeaderData headerData = header(data);
	compressedSize = headerData.cmp_s;
    if (headerData.data_t == 0)
	{
        //# EDL0: Store
        memcpy(outputDecompressed, &data[12], headerData.cmp_s);
		return headerData.cmp_s;
	}
    else if (headerData.data_t == 1)
	{
        return _decEDL1(headerData.endian, &data[12], headerData.cmp_s, headerData.dec_s, outputDecompressed);
	}
    else if (headerData.data_t == 2)
	{
         return _decEDL2(headerData.endian, &data[12], headerData.cmp_s, headerData.dec_s, outputDecompressed);
	}
	else
	{
		return 0;
	}
}

void CEDLCompress::encode(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, int mode, CString endian)
{
    //# Funnel to appropriate function, or run all and keep best.
    if (mode == -1)
	{
		mode = 0;
        if (dataLength == 0)
		{
            _encEDL0(data, dataLength, outputCompressed, outputCompressedLength, endian);
			return;
		}
        //# Keep best
		int length0 = 0;
        _encEDL0(data, dataLength, outputCompressed, length0, endian);
        //# TODO: EDL1 not yet implemented!
		int length2 = 0;
        _encEDL2(data, dataLength, outputCompressed, length2, endian);
        if (length2 < length0)
		{
			mode = 2;
		}
	}
    
	if (mode == 0)
        _encEDL0(data, dataLength, outputCompressed, outputCompressedLength, endian);
    else if (mode == 1)
        _encEDL1(data, dataLength, outputCompressed, outputCompressedLength, endian);
    else if (mode == 2)
        _encEDL2(data, dataLength, outputCompressed, outputCompressedLength, endian);
    else
        throw "Unknown EDL";
}

void CEDLCompress::_encEDL0(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, CString endian)
{
	if (endian == "big")
	{
        outputCompressed[0] = 'E'; outputCompressed[1] = 'D'; outputCompressed[2] = 'L';
        outputCompressed[3] = 0x80;
    }
	else
	{
        outputCompressed[0] = 'E'; outputCompressed[1] = 'D'; outputCompressed[2] = 'L';
        outputCompressed[3] = 0x00;
    }
	outputCompressed[4] = (dataLength) >> 24;
    outputCompressed[5] = (dataLength) >> 16;
    outputCompressed[6] = (dataLength) >> 8;
    outputCompressed[7] = (dataLength) & 0xFF;
    outputCompressed[8] = dataLength >> 24;
    outputCompressed[9] = dataLength >> 16;
    outputCompressed[10] = dataLength >> 8;
    outputCompressed[11] = dataLength & 0xFF;
	int dataLengthPadded = dataLength;
	if ((dataLengthPadded % 4) != 0)
		dataLengthPadded += (4 - (dataLengthPadded % 4));
	for (int x = 0; x < dataLength; x++)
		outputCompressed[12 + x] = data[x];
	for (int x = dataLength; x < dataLengthPadded; x++)
		outputCompressed[12 + x] = 0x00;
    if (endian == "little")
	{
		for (int x = 12; x < 12 + dataLengthPadded; x += 4)
		{
			unsigned long value = CharArrayToLong(&outputCompressed[x]);
			value = Flip32Bit(value);
			WriteLongToBuffer(outputCompressed, x, value);
		}
	}
	outputCompressedLength = 12 + dataLengthPadded;
}

void CEDLCompress::_encEDL1(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, CString endian)
{
    throw "EDL1 not implemented";
}

void CEDLCompress::_encEDL2(unsigned char* data, int dataLength, unsigned char* outputCompressed, int& outputCompressedLength, CString endian)
{
	std::vector<EDLPair> edlPairs;
    //# Write patterns in backwards.  Other values go into _push.
    //# Ring buffer should be preloaded with 0x1000 zeroes.
	unsigned char* dataTemp = new unsigned char[0x1000 + dataLength];
	for (int x = 0; x < 0x1000; x++)
		dataTemp[x] = 0x00;
	for (int x = 0; x < dataLength; x++)
		dataTemp[x + 0x1000] = data[x];

    int pos = 0x1000;
    int sz = 0x1000 + dataLength;
	std::vector<unsigned char> buf;   //# We'll put runs of raws in here.
    //# Let's do this thing.
    while (pos < sz)
	{
		std::vector<int> searchReturn;
        searchReturn = _search(dataTemp, pos, sz);
		int hitp = searchReturn[0];
		int hitl = searchReturn[1];

        if (hitl < 2)
		{
            //# Push a raw if copying isn't possible.
            buf.push_back(dataTemp[pos]);
            pos += 1;
		}
        else
		{
            searchReturn = _search(dataTemp, pos+1, sz);
			int tstp = searchReturn[0];
			int tstl = searchReturn[1];
            if ((hitl + 1) < tstl)
			{
                //# Push a raw if slightly more convenient to do so.
                buf.push_back(dataTemp[pos]);
                pos += 1;
                hitl = tstl;
                hitp = tstp;
			}
            //# Push any raws waiting in the buffer.
            if (buf.size() > 0)
			{
                while (buf.size() > 11)
				{
                    //# Encode n + 3 words in a single command and truncate buf.
                    int n = min(18, (buf.size()) >> 2);
                    edlPairs.push_back(EDLPair(5, 0x1D));
                    edlPairs.push_back(EDLPair(4, n - 3));
                    n <<= 2;
                    for (int i = 0; i < n; i++)
					{
                        edlPairs.push_back(EDLPair(8, buf.front()));
						buf.erase(buf.begin());
					}
				}

                while (buf.size() > 0)
				{
                    //# 0, then byte.
                    edlPairs.push_back(EDLPair(9, (buf.front() << 1)));
					buf.erase(buf.begin());
				}
			}
            //# Encode for length.
            if (hitl == 2)
                edlPairs.push_back(EDLPair(3, 3));
            else if (hitl == 3)
                edlPairs.push_back(EDLPair(4, 7));
            else if (hitl == 4)
                edlPairs.push_back(EDLPair(4, 1));
            else if (hitl == 5)
                edlPairs.push_back(EDLPair(4, 5));
            else if (hitl == 6)
                edlPairs.push_back(EDLPair(5, 9));
            else if (hitl == 7)
                edlPairs.push_back(EDLPair(5, 0x19));
            else if (hitl == 8)
                edlPairs.push_back(EDLPair(5, 0xD));
            else
			{
                edlPairs.push_back(EDLPair(4, 0xF));
                edlPairs.push_back(EDLPair(8, hitl - 8));
			}
            //# Figure out encoding for offset if len > 2.
            int e = pos - hitp - 1;
            if (hitl > 2)
			{
                int v = e >> 8;
                if (v == 0)
                    edlPairs.push_back(EDLPair(1, 0));
                else if (v == 1)  
                    edlPairs.push_back(EDLPair(3, 3));
                else if (v == 2)
                    edlPairs.push_back(EDLPair(4, 1));
                else if (v == 3)
                    edlPairs.push_back(EDLPair(4, 9));
                else if (v == 4)
                    edlPairs.push_back(EDLPair(5, 0x15));
                else if (v == 5)
                    edlPairs.push_back(EDLPair(5, 0x1D));
                else if (v == 6)
                    edlPairs.push_back(EDLPair(5, 0x17));
                else if (v == 7)
                    edlPairs.push_back(EDLPair(5, 0x1F));
                else if (v == 8)
                    edlPairs.push_back(EDLPair(6, 5));
                else if (v == 9)
                    edlPairs.push_back(EDLPair(6, 0x25));
                else if (v == 10)
                    edlPairs.push_back(EDLPair(6, 0xD));
                else if (v == 11)
                    edlPairs.push_back(EDLPair(6, 0x2D));
                else if (v == 12)
                    edlPairs.push_back(EDLPair(6, 7));
                else if (v == 13)
                    edlPairs.push_back(EDLPair(6, 0x27));
                else if (v == 14)
                    edlPairs.push_back(EDLPair(6, 0xF));
                else
                    edlPairs.push_back(EDLPair(6, 0x2F));
			}
            e &= 0xFF;
            edlPairs.push_back(EDLPair(8, e));
            pos += hitl;
		}
	}
    //# Push any remaining raws.
    if (buf.size() > 0)
	{
        while (buf.size() > 11)
		{
            //# Encode n + 3 words in a single command and truncate buf.
            int n = min(18, ((buf.size()) >> 2));
            edlPairs.push_back(EDLPair(5, 0x1D));
            edlPairs.push_back(EDLPair(4, n - 3));
            n <<= 2;
            for (int i = 0; i < n; i++)
			{
				edlPairs.push_back(EDLPair(8, buf.front()));
				buf.erase(buf.begin());
			}
		}
        while (buf.size() > 0)
		{
            //# 0, then byte.
			edlPairs.push_back(EDLPair(9, buf.front() << 1));
			buf.erase(buf.begin());
		}
	}
    //# Write the terminator.
    edlPairs.push_back(EDLPair(4, 0xF));
    edlPairs.push_back(EDLPair(8, 0));

	std::vector<unsigned long> tbl = _push(endian, edlPairs);
    
	if (endian == "big")
	{
        outputCompressed[0] = 'E'; outputCompressed[1] = 'D'; outputCompressed[2] = 'L';
        outputCompressed[3] = 0x82;
    }
	else
	{
        outputCompressed[0] = 'E'; outputCompressed[1] = 'D'; outputCompressed[2] = 'L';
        outputCompressed[3] = 0x02;
    }
	outputCompressed[4] = (dataLength + 12) >> 24;
    outputCompressed[5] = (dataLength + 12) >> 16;
    outputCompressed[6] = (dataLength + 12) >> 8;
    outputCompressed[7] = (dataLength + 12) & 0xFF;
    outputCompressed[8] = dataLength >> 24;
    outputCompressed[9] = dataLength >> 16;
    outputCompressed[10] = dataLength >> 8;
    outputCompressed[11] = dataLength & 0xFF;
	
	for (int x = 0; x < tbl.size(); x++)
	{
		if (endian == "big")
			WriteLongToBuffer(outputCompressed, 12 + x * 4, tbl[x]);
		else
			WriteLongToBuffer(outputCompressed, 12 + x * 4, Flip32Bit(tbl[x]));
	}
	outputCompressedLength = 12 + (tbl.size() * 4);

	delete [] dataTemp;
}

std::vector<unsigned long> CEDLCompress::_push(CString endian, std::vector<EDLPair> edlPairs)
{
	std::vector<unsigned long> tbl;
    //# Limit for pushing ~32 bytes or so.
    unsigned __int64 c = 0;
	unsigned __int64 v = 0;
	
	// yield?
    for (int x = 0; x < edlPairs.size(); x++)
	{
		int cnt = edlPairs[x].cnt;
		unsigned long value = edlPairs[x].value;

        v |= (unsigned __int64)value << c;
        c += cnt;
        if (c > 31)
		{
			tbl.push_back(v & 0xFFFFFFFF);
            c -= 32;
            v >>= 32;    //# Should retain the value...probably.
		}
	}
    //# Flush remaining stuff.
    if (c)
	{
        tbl.push_back(v);
	}
    //# Remove trailing zeroes.
    while (!tbl.empty() && tbl.back() == 0)
	{
        tbl.pop_back();
    }
    //# Pack into bytes.
	if (endian == "little")
	{
		for (int x = 0; x < tbl.size(); x++)
		{
			tbl[x] = Flip32Bit(tbl[x]);
		}
	}
	return tbl;
}

bool CEDLCompress::Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex)
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

int CEDLCompress::Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
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

int CEDLCompress::RFind(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
{
	int resultIndex = -1;
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
			return resultIndex;

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
			resultIndex = index;
			// needle was found
		}

		// continue to search for needle
		//sourceLength -= (index - startIndex + 1);
		startIndex = index + 1;
	}
	return resultIndex;
}

std::vector<int> CEDLCompress::_search(unsigned char* data, int pos, int sz, int cap)
{
	std::vector<int> returnData;
	returnData.resize(2);

    int ml = min(cap, sz - pos);
    if (ml < 2)
	{
		returnData[0] = 0;
		returnData[1] = 0;
		return returnData;
        //return 0, 0
	}
    int mp = max(0, pos - 0x1000);
    int hitp = 0;
	int hitl = 2;
	int hl = -1;

    if (mp < pos)
	{	
		hl = Find(&data[mp], &data[pos], hitl, 0, pos + hitl - mp);
        while (hl < (pos - mp))
		{
            while ((hitl < ml) && (data[pos + hitl] == data[mp + hl + hitl]))
			{
                hitl += 1;
			}
            mp += hl;
            hitp = mp;
            if ((hitl == ml) && (!(hitl == 2 && ((pos - hitp) > 0xFF))))
			{
				returnData[0] = hitp;
				returnData[1] = hitl;
                //return hitp, hitl
				return returnData;
			}
            mp += 1;
            hitl += 1;
            if (mp >= pos)
                break;
//##            hl = _seqfind(data[mp:pos+hitl], hitl)
            //hl = data[mp:pos+hitl].find(data[pos:pos+hitl])
			hl = Find(&data[mp], &data[pos], hitl, 0, pos + hitl - mp);
		}
	}

    //# If length less than 3, return miss.
    if (hitl < 3)
	{
        hitl = 0;
	}
    //# Something of hack--you want the nearest, not farthest.
	else
	{
		hitl -= 1;
        hitp = RFind(data, &data[pos], hitl, 0, pos+hitl-1);
		// Done a different way...just automatically scan initially for latest of the best match
	}

	if (hitl == 2 && ((pos - hitp) > 0xFF))
        hitl = 0;

	returnData[0] = hitp;
	returnData[1] = hitl;
	return returnData;
}

void CEDLCompress::nibbleswap(unsigned char* data, int dataLength)
{
    //"""Some Eurocom files, especially images, swap nibbles for some reason.
    //This accepts a bytes or bytearray object and swaps each byte around for you."""
    //# By iterating instead of changing in place bytes objects are supported.
    unsigned char* tempData = new unsigned char[dataLength];
	for (int i = 0; i < dataLength; i++)
	{
        unsigned char b = (i<<4) & 0xF0;
        b|= (i>>4) & 0xF;
        tempData[i] = b;
	}
	for (int x = 0; x < dataLength; x++)
	{
		data[x] = tempData[x];
	}
	delete [] tempData;
}