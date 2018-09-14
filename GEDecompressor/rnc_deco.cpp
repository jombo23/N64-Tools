#include "stdafx.h"
/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */


#include "rnc_deco.h"
#include <vector>

//return codes
#define NOT_PACKED	0
#define PACKED_CRC	0
#define UNPACKED_CRC	0

//other defines
#define TABLE_SIZE	(16 * 8)
#define MIN_LENGTH	2
#define HEADER_LEN	18

RncDecoder::RncDecoder() {
	initCrc();
}

RncDecoder::~RncDecoder() { }

void RncDecoder::initCrc() {
	unsigned short cnt = 0;
	unsigned short tmp1 = 0;
	unsigned short tmp2 = 0;

	for (tmp2 = 0; tmp2 < 0x100; tmp2++) {
		tmp1 = tmp2;
		for (cnt = 8; cnt > 0; cnt--) {
			if (tmp1 % 2) {
				tmp1 >>= 1;
				tmp1 ^= 0x0a001;
			} else
				tmp1 >>= 1;
		}
		_crcTable[tmp2] = tmp1;
	}
}

//calculate 16 bit crc of a block of memory
unsigned short RncDecoder::crcBlock(const unsigned char *block, unsigned long size) {
	unsigned short crc = 0;
	unsigned char *crcTable8 = (unsigned char *)_crcTable; //make a unsigned char* to crc_table
	unsigned char tmp;
	unsigned long i;

	for (i = 0; i < size; i++) {
		tmp = *block++;
		crc ^= tmp;
		tmp = (unsigned char)((crc >> 8) & 0x00FF);
		crc &= 0x00FF;
		crc = *(unsigned short *)&crcTable8[crc << 1];
		crc ^= tmp;
	}

	return crc;
}

unsigned short RncDecoder::inputBits(unsigned char amount) {
	unsigned short newBitBuffh = _bitBuffh;
	unsigned short newBitBuffl = _bitBuffl;
	signed short newBitCount = _bitCount;
	unsigned short remBits, returnVal;

	returnVal = ((1 << amount) - 1) & newBitBuffl;
	newBitCount -= amount;

	if (newBitCount < 0) {
		newBitCount += amount;
		remBits = (newBitBuffh << (16 - newBitCount));
		newBitBuffh >>= newBitCount;
		newBitBuffl >>= newBitCount;
		newBitBuffl |= remBits;
		_srcPtr += 2;
		newBitBuffh = READ_LE_UINT16(_srcPtr);
		amount -= newBitCount;
		newBitCount = 16 - amount;
	}
	remBits = (newBitBuffh << (16 - amount));
	_bitBuffh = newBitBuffh >> amount;
	_bitBuffl = (newBitBuffl >> amount) | remBits;
	_bitCount = (unsigned char)newBitCount;

	return returnVal;
}

void RncDecoder::makeHufftable(unsigned short *table) {
	unsigned short bitLength, i, j;
	unsigned short numCodes = inputBits(5);

	if (!numCodes)
		return;

	unsigned char huffLength[16];
	for (i = 0; i < numCodes; i++)
		huffLength[i] = (unsigned char)(inputBits(4) & 0x00FF);

	unsigned short huffCode = 0;

	for (bitLength = 1; bitLength < 17; bitLength++) {
		for (i = 0; i < numCodes; i++) {
			if (huffLength[i] == bitLength) {
				*table++ = (1 << bitLength) - 1;

				unsigned short b = huffCode >> (16 - bitLength);
				unsigned short a = 0;

				for (j = 0; j < bitLength; j++)
					a |= ((b >> j) & 1) << (bitLength - j - 1);
				*table++ = a;

				*(table + 0x1e) = (huffLength[i] << 8) | (i & 0x00FF);
				huffCode += 1 << (16 - bitLength);
			}
		}
	}
}

unsigned short RncDecoder::inputValue(unsigned short *table) {
	unsigned short valOne, valTwo, value = _bitBuffl;

	do {
		valTwo = (*table++) & value;
		valOne = *table++;

	} while (valOne != valTwo);

	value = *(table + 0x1e);
	inputBits((unsigned char)((value>>8) & 0x00FF));
	value &= 0x00FF;

	if (value >= 2) {
		value--;
		valOne = inputBits((unsigned char)value & 0x00FF);
		valOne |= (1 << value);
		value = valOne;
	}

	return value;
}

unsigned short RncDecoder::READ_BE_UINT16(const unsigned char* buffer)
{
	unsigned short tempValue = (((buffer[0] << 8) | buffer[1]));
	return tempValue;
}

unsigned short RncDecoder::READ_LE_UINT16(const unsigned char* buffer)
{
	unsigned short tempValue = (((buffer[1] << 8) | buffer[0]));
	return tempValue;
}

unsigned long RncDecoder::READ_BE_UINT32(const unsigned char* buffer)
{
	unsigned long tempValue = ((((((buffer[0] << 8) | buffer[1]) << 8) | buffer[2]) << 8) | buffer[3]);
	return tempValue;
}

int RncDecoder::unpackM1(const void *input, void *output, unsigned short key, int& packLen) 
{
	unsigned char *outputLow, *outputHigh;
	const unsigned char *inputHigh, *inputptr = (const unsigned char *)input;

	unsigned long unpackLen = 0;
	packLen = 0;
	unsigned short counts = 0;
	unsigned short crcUnpacked = 0;
	unsigned short crcPacked = 0;


	_bitBuffl = 0;
	_bitBuffh = 0;
	_bitCount = 0;

	//Check for "RNC "
	if ((READ_BE_UINT32(inputptr) != RNC_SIGNATURE) && (READ_BE_UINT32(inputptr) != RNC_SIGNATUREV2) && 
		(READ_BE_UINT32(inputptr) != RNX_SIGNATURE) && (READ_BE_UINT32(inputptr) != RNX_SIGNATUREV2)
		 && (READ_BE_UINT32(inputptr) != RNC_SIGNATURE81))
		return NOT_PACKED;

	int version = inputptr[3];

	inputptr += 4;

	// read unpacked/packed file length
	unpackLen = READ_BE_UINT32(inputptr); inputptr += 4;
	packLen = READ_BE_UINT32(inputptr); inputptr += 4;

	unsigned char blocks = *(inputptr + 5);

	int indexedBlockCount = 1;
	int indexedBlockSize = 0;
	std::vector<int> compressedSizes;

	int totalUnpackLen = unpackLen;

	if (version != 0x81)
	{
		//read CRC's
		crcUnpacked = READ_BE_UINT16(inputptr); inputptr += 2;
		crcPacked = READ_BE_UINT16(inputptr); inputptr += 2;
		inputptr = (inputptr + HEADER_LEN - 16);

		inputptr = (((const unsigned char *)input) + HEADER_LEN);
	}
	else
	{
		indexedBlockCount = READ_BE_UINT16(inputptr);
		inputptr += 2;

		indexedBlockSize = inputptr[0] * 0x1000;
		inputptr++;

		// reserved
		inputptr +=3;

		for (int x = 0; x < indexedBlockCount; x++)
		{
			compressedSizes.push_back(READ_BE_UINT16(inputptr));
			inputptr += 2;
		}
	}

	//if (crcBlock(inputptr, packLen) != crcPacked)
		//return PACKED_CRC;

	for (int x = 0; x < indexedBlockCount; x++)
	{
		if (version == 0x81)
		{
			unpackLen = indexedBlockSize;
			packLen = compressedSizes[x];
			blocks = 1;
		}

		if ((version == 0x01) || (version == 0x81))
		{
			_srcPtr = inputptr;

			inputHigh = ((const unsigned char *)input) + packLen + HEADER_LEN;
			outputLow = (unsigned char *)output;
			outputHigh = *(((const unsigned char *)input) + 16) + unpackLen + outputLow;

			if (! ((inputHigh <= outputLow) || (outputHigh <= inputHigh)) ) {
				_srcPtr = inputHigh;
				_dstPtr = outputHigh;
				memcpy((_dstPtr-packLen), (_srcPtr-packLen), packLen);
				_srcPtr = (_dstPtr-packLen);
			}

			_dstPtr = (unsigned char *)output;
			_bitCount = 0;

			_bitBuffl = READ_LE_UINT16(_srcPtr);
			inputBits(2);

			do {
				makeHufftable(_rawTable);
				makeHufftable(_posTable);
				makeHufftable(_lenTable);

				counts = inputBits(16);

				do {
					unsigned long inputLength = inputValue(_rawTable);
					unsigned long inputOffset;

					if (inputLength) {
						memcpy(_dstPtr, _srcPtr, inputLength); //memcpy is allowed here
						_dstPtr += inputLength;
						_srcPtr += inputLength;
						unsigned short a = READ_LE_UINT16(_srcPtr);
						unsigned short b = READ_LE_UINT16(_srcPtr + 2);

						_bitBuffl &= ((1 << _bitCount) - 1);
						_bitBuffl |= (a << _bitCount);
						_bitBuffh = (a >> (16 - _bitCount)) | (b << _bitCount);
					}

					if (counts > 1) {
						inputOffset = inputValue(_posTable) + 1;
						inputLength = inputValue(_lenTable) + MIN_LENGTH;

						// Don't use memcpy here! because input and output overlap.
						unsigned char *tmpPtr = (_dstPtr-inputOffset);
						while (inputLength--)
							*_dstPtr++ = *tmpPtr++;
					}
				} while (--counts);
			} while (--blocks);

			output = _dstPtr;
		}
		else
		{
			int fileSizeUncompressed = 0;
			unsigned long bitBuffer = 0;
			int bitBufferCount = 0;
			unsigned char* dst = (unsigned char*)output;
			int dstOffset = 0;
			int inpOffset = 0;
			 get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset);
			 get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset); // unused 2 bits

			 while ((fileSizeUncompressed < unpackLen) && (inpOffset < packLen))
			 {
			   if (!get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) 
			   {
				   unsigned char byte = get_byte(inputptr, inpOffset);
				 *dst++ = byte;
				 
				 dstOffset++;
				 fileSizeUncompressed ++;
			   }
			   else 
			   {
				   // ?
				 int length = 2;
				 int offset = 0;
				 if (!get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) 
				 {
				   length = 4 + get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset);
				   if (get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) 
				   {
					 length = (length - 1) * 2 + get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset);
					 if (length == 9) 
					 {
					   int valueToAdd = ((((((get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset) << 1) | get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) << 1) | get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) << 1) | get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset));
					   length = (valueToAdd + 3) * 4;
					   for (int i = 0; i < length; i++)
					   {
						   unsigned char byte = get_byte(inputptr, inpOffset);
						   *dst++ = byte;
						   dstOffset++;
					   }
					   fileSizeUncompressed += length;
					   continue;
					 }
				   }
				   offset = get_offset(inputptr, bitBuffer, bitBufferCount, inpOffset);
				 } 
				 else 
				 {
					if (get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) 
					{
						if (get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) 
						{
							length = get_byte(inputptr, inpOffset) + 8;
							if (length == 8) 
							{
								get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset);
								continue; //dunno why
							}
						} 
						else 
						{
							length = 3;
						}
						offset = get_offset(inputptr, bitBuffer, bitBufferCount, inpOffset);
					} 
					else 
					{
						offset = get_byte(inputptr, inpOffset) + 1;
					}
				 }
				 unsigned char *tmpPtr = (dst-offset);
				 int iterLength = length;
				 while (iterLength--)
				 {
					 unsigned char byte = *tmpPtr++;
					 *dst++ = byte;
					 dstOffset++;
				 }
				 fileSizeUncompressed = (fileSizeUncompressed + length);
			   }
			 }

			 output = dst;
		}
	}

	if (version != 0x81)
	{
		//if (crcBlock((unsigned char *)output, unpackLen) != crcUnpacked)
			//return UNPACKED_CRC;
	}

	// all is done..return the amount of unpacked bytes
	return totalUnpackLen;
}

bool RncDecoder::get_bit(const unsigned char*& inputptr, unsigned long& bitBuffer, int& bitBufferCount, int& inpOffset)
{
	if (bitBufferCount == 0)
	{
		bitBuffer = *inputptr++;
		inpOffset++;
		bitBufferCount = 8;
	}
	bool returnBool = ((bitBuffer >> (bitBufferCount-1)) & 0x1);
	bitBufferCount--;
	return returnBool;
}

int RncDecoder::get_offset(const unsigned char*& inputptr, unsigned long& bitBuffer, int& bitBufferCount, int& inpOffset)
{
	int value = 0;
	int offset = 0;
	if (get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) 
	{
		value = get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset);
		if (get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset)) 
		{
			value = value * 2 + 4 + get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset);
			if (!get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset))
			{
				value = value * 2 + get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset);
			}
		} 
		else if (value == 0)
		{
			value = get_bit(inputptr, bitBuffer, bitBufferCount, inpOffset) + 2;
		}
	}
	offset = (value << 8) + get_byte(inputptr, inpOffset) + 1;
	return offset;

}
unsigned char RncDecoder::get_byte(const unsigned char*& inputptr, int& inpOffset)
{
	unsigned char returnChar = *inputptr;
	inputptr++;
	inpOffset++;
	return returnChar;
}

BOOL RncDecoder::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESHOWWINDOW;
   si.wShowWindow  = SW_HIDE;
   ZeroMemory(&processInfo, sizeof(processInfo));
   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox(NULL, "For some reason GZip Failed", "Error", NULL);
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
};

BOOL RncDecoder::IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}

bool RncDecoder::CompressRNCFile(CString mainFolder, CString inputFile, CString outputFile, bool useRNX)
{
	CString gzipFileName = (mainFolder + "rncencode.exe");
	char tempFileExistName[1000];
	strcpy(tempFileExistName, (mainFolder + "rncencode.exe"));
	if (IsFileExist(tempFileExistName) == false)
	{
		MessageBox(NULL, "rncencode.exe not found!", "Error", NULL);
		return false;
	}

	strcpy(tempFileExistName, inputFile);
	if (IsFileExist(tempFileExistName))
	{
		FILE* tempInputFile = fopen(inputFile, "rb");
		fseek(tempInputFile, 0, SEEK_END);
		unsigned long size = ftell(tempInputFile);

		unsigned char* tempBuffer;
		tempBuffer = new unsigned char[size];
		
		fseek(tempInputFile, 0, SEEK_SET);
		fread(tempBuffer, 1, size, tempInputFile);

		FILE* tempOutputFile = fopen((mainFolder+"tempgh9.bin"), "wb");
		if (tempOutputFile == 0)
		{
			fclose(tempInputFile);
			delete [] tempBuffer;
			MessageBox(NULL, "Cannot Write Temporary File", "Error", NULL);
			return false;
		}

		fwrite(tempBuffer, 1, size, tempOutputFile);	

		fclose(tempInputFile);
		fclose(tempOutputFile);

		delete [] tempBuffer;

		::SetCurrentDirectory(mainFolder);
		CString tempStr = ("rncencode.exe tempgh9.bin -o TEMPGH9.BIZ");
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));
		CString outputGZippedName = (mainFolder+"TEMPGH9.BIZ");

		DeleteFile((mainFolder+"tempgh9.bin"));

		strcpy(tempFileExistName, outputGZippedName);
		if (IsFileExist(tempFileExistName))
		{
			FILE* inputFileName = fopen(outputGZippedName, "rb");
			int sizeNew = 0;
			fseek(inputFileName, 0, SEEK_END);
			sizeNew = ftell(inputFileName);
			fseek(inputFileName, 0, SEEK_SET);

			unsigned char* tempBufferNew;
			tempBufferNew = new unsigned char[sizeNew];
			fread(tempBufferNew, 1, sizeNew, inputFileName);

			fclose(inputFileName);
			DeleteFile((mainFolder+"TEMPGH9.BIZ"));
			FILE* outputFileName = fopen(outputFile, "wb");
			if (outputFileName == NULL)
			{
				delete [] tempBufferNew;
				MessageBox(NULL, "Error opening temp output file", "Error", NULL);
				return false;
			}

			if (useRNX)
				tempBufferNew[2] = 0x58;

			fwrite(&tempBufferNew[0], 1, (sizeNew), outputFileName);	

			delete [] tempBufferNew;
			fflush(outputFileName);
			fclose(outputFileName);
			return true;
		}
		else
		{
			MessageBox(NULL, "Error Compressing - rncencode didn't spit out a file", "Error", NULL);
			return false;
		}
	}
}

/*
void header(data)
{
    //"""Returns dictionary:
	//indexed     True if an indexed file
	//archive     True if an archive
	//unsupported True if some other flag raised
	//h_sz        size of header/start of data
	//cmp_type:   compression type, or -1 if invalid (not RNC)
	//cmp_sz:     compressed file size
	//dec_sz:     decompressed file size
	//Indexed types:
		//blk_n:      number of blocks
		//blk_s:      byte size of blocks
	//Normal types:
		//crc_in:     CRC for input
		//crc_out:    CRC for output
		//chk_d:      cmp. chunk overlap over dec. chunks
		//chk_n:      number of chunks to overlap

//Original Format:
	//0x0     3   signature, usually b'RNC'
	//0x3     1   mode
		//80    indexed mode
		//40    archive mode
		//03    compression types 0:'store', 1:'huff', or 2:'lz'
	//File types:
		//0x4     4   decompressed size
		//Compressed types:
			//0x8     4   compressed size
			//Normal mode:
				//0xC     2   CRC for compressed data
				//0xE     2   CRC for decompressed data
				//0x10    1   difference in cmp and dec sizes in largest chunk
				//0x11    1   overlap size for in-place decompression
			//Indexed mode:
				//0xC     2   block count
				//0xE     1   block size (in kb)
				//0xF     3   RESERVED
				//0x12    2ea compressed filesizes for each block
	//Archives:
		//0x4     2   size of header
		//0x6     2   CRC for filetable
		//0x8     var filetable
		//var     var files
		//files end with NULL ID+type (NULL word)
		//Filetable header:
			//0x0 2   offset to next directory or start of files
			//0x2 var directory name; root is NULL
			//var var file entries
		//Filetable entries:
			//0x0 var NULL-terminated filename string
			//var 4   offset to data
			//entries end with first NULL string (1 NULL byte)
	name = data[0:3].decode(errors='replace')
	t = True if data[3] & 0x80 else False
	a = True if data[3] & 0x40 else False
	u = True if data[3] & 0x3C else False
	mode = data[3] & 3
	if name not in RNC.IDs:
		mode = -1
	if mode == 3:
		mode = -1
	h = dict({'indexed':t, 'archive':a, 'unsupported':u, 'cmp_type':mode})
	//# Three things are lethal: bad mode, unsupported features, and archive+indexed mode.
	if (mode < 0) or u or (t&a):
		return h

	if a:
		h.update(RNC.arcs(data))
	elif t:
		h.update(RNC.indexed(data))
	else:
		h.update(RNC.files(data))
	return h
}

void files(data)
{
    z = 8
    dec_s= int.from_bytes(data[ 4: 8], byteorder='big')
    //# Test if mode 0 file.
    mode = data[3]&3
    if mode==0:
        return {'h_sz':z, 'cmp_sz':dec_s, 'dec_sz':dec_s}
    z+=10
    cmp_s= int.from_bytes(data[ 8:12], byteorder='big')
    crc_o= int.from_bytes(data[12:14], byteorder='big')
    crc_i= int.from_bytes(data[14:16], byteorder='big')
    //# Get encoded/locked flags
    if mode==2:
        lock = True if data[z]&0x80 else False
        enc  = True if data[z]&0x40 else False
    else:
        lock = True if data[z]&1 else False
        enc  = True if data[z]&2 else False
    return {'cmp_sz':cmp_s, 'dec_sz':dec_s, 'crc_in':crc_i, 'crc_out':crc_o, 'chk_d':data[16], 'chk_n':data[17], 'h_sz':z, 'encoded':enc, 'locked':lock}
}

void indexed(data)
{
    dec_s= int.from_bytes(data[ 4: 8], byteorder='big')
    cmp_s= int.from_bytes(data[ 8:12], byteorder='big')
    bn   = int.from_bytes(data[12:14], byteorder='big')
    bs   = data[14] << 10   # num kb * 0x400
    z    = (bn<<1) + 18

    //# Copy out the filesize table.
    t = []
    c = dec_s
    s = bs
    for i in range(18, z, 2):
        //# Actual offsets are short-aligned.
        p = int.from_bytes(data[i:i+2], byteorder='big') + 1
        p&= (~1)
        s = c if c<bs else bs
        c-=bs
        t.append({'cmp_sz':p, 'dec_sz':s})

    //# Get encoded/locked flags
    lock = True if data[z]&1 else False
    enc  = True if data[z]&2 else False
    return {'cmp_sz':cmp_s, 'dec_sz':dec_s, 'blk_n':bn, 'blk_s':bs, 'h_sz':z, 'table':t, 'encoded':enc, 'locked':lock}
}
void arcs(data)
{
    z   = int.from_bytes(data[ 4: 6], byteorder='big')
    crc = int.from_bytes(data[ 6: 8], byteorder='big')
    //# Calculate the filetable's checksum.
    calc= RNC.crc(data[8:z])
    //# Strip out the filetable.
    t = []
    p = 8
    while p < z:
        //# Get the directory name.
        d = int.from_bytes(data[p:p+2], byteorder='big', signed=False)
        p+=2
        l=0
        while data[p+l]:
            l+=1
        pre = data[p:p+l].decode(encoding='ascii', errors='strict')
        p+= l+1
        //# Fetch filenames, offsets for each of that directory's files.
        while p<d:
            l=0
            while data[p+l]:
                l+=1
            n = data[p:p+l].decode(encoding='ascii', errors='replace')
            p+= l+1
            //# Run until either a NULL filename or end of directory.
            if not n:
                break
            l = int.from_bytes(data[p:p+4], byteorder='big')
            t.append({'name':pre+n, 'pos':l})
            p+=4
    return {'h_sz':z, 'crc_ft':crc, 'crc_calc':calc, 'filetable':t}
}

def dec0(data, size=None, key=None)
{
    """Uncompressed data within wrapper."""
    if not out:
        out = bytearray()
    if not size:
        size = len(data)
    out.extend(data[0:size])
    return out
}

void _grab(itr, bits=0, mode='read')
{
	//"""Returns a value equal to the next num bits in data stream,
	//accounting for the fool-stupid way they pull values from a bitstream.
	//
	//Iterates stream data using either 'big' or 'little' endian order.
	//Two possible mode:
		//'read', which returns a value and advances bitstream
		//'peek' which just masks against current value
	
	i, c, b = 0, 0, 0
	while True:
		v = next(itr,0)
		v|= next(itr,0)<<8
		v<<=c
		b|=v     # bitfield | new bits<<count
		c+=16    # bitcount+=16
		i+=2
		while c>=bits:
			m = (1<<bits)-1 # mask
			v = b&m         # value = bitfield & mask
			if mode!='peek':
				c-= bits        # count-= #bits
				b>>=bits        # bitfield without said bits
			bits, mode = yield v  # yield the value, retrieving next #bits
}

void dec1(data, size, key=0, index=None)
{
	//"""Huffman + LZ77 scheme
	//Accepts an iterable for the bitstream,
		//a dictionary for the header,
		//and output to this point if applicable."""

	

	//# Initialize the stream and throw away the first two bits.
	d = RNC._stream(data)
	k = RNC._updatekey(key)
	itr = _grab(d, bits=2)
	next(itr)
	out = bytearray()
	if index != None:
		idx_n = 0

	while size:
		b = RNC.huf_in(itr, 5, 4)
		if b:   raws = b
		b = RNC.huf_in(itr, 5, 4)
		if b:   dist = b
		b = RNC.huf_in(itr, 5, 4)
		if b:   lens = b
		cnt = itr.send((16,'read'))

		while True:
			l = RNC.huf_rd(raws, itr)
			if l<0:
				break
			if l:
				size -= l
				key = next(k)
				if index!= None:
					index.extend([idx_n for i in range(l)])
					idx_n +=1
					idx_n%= 16
				while l:
					l-=1
					# should really just be a byte out; presumes magically no masking required
					out.append(next(d)^key)
			cnt-=1
			if not cnt:
				break
			p = RNC.huf_rd(dist, itr) + 1
			l = RNC.huf_rd(lens, itr) + 2
			for i in range(l):
				v = out[len(out)-p]
				out.append(v)
			# If keeping a copy index, do the same here.
			if index!= None:
				for i in range(l):
					v = index[len(index)-p]
					index.append(v)
			size-=l
	return out
}

void dec2(data, size, key=0, index=None)
{
    //"""LZ77 scheme alone.
    //Accepts an iterable for the bitstream,
        //and output to this point if applicable."""
    def _fetch(itr, bits=0, mode='read'):
        c, b = 0, 0
        while True:
            m = (1<<c)-1
            b&=m
            b<<=8
            b|= next(itr)
            c+=8
            while c>=bits:
                #m = 1<<(bits-1)
                v = b>>(c-bits)
                #v&=m
                v = RNC._mirror(v,bits)
                if mode!='peek':
                    c-= bits        # count-= #bits
                bits, mode = yield v

    //# initialize the stream; first two bits are discarded
    d = RNC._stream(data)
    b = _fetch(d, 2)
    k = RNC._updatekey(key)
    next(b)
    out = bytearray()
    if index != None:
        idx_n = 0

    while size:
        if b.send((1, 'read')):
            back, num = 0, 0
            if b.send((1, 'read')):
                if b.send((1, 'read')):
                    if b.send((1, 'read')):
                        //# 1.111.*L.-B-.*D   if L: copy L+8 bytes from D+1 in bank B
                        num = next(d) + 8
                        if num==8:
                            if b.send((1, 'read')): continue
                            else:                   break
                    else:
                        //# 1.110.-B-.*D  copy 3 bytes from D+1 in bank B
                        num = 3
                else:
                    //# 1.10.*D   copy 2 bytes from D+1
                    back, num = 1, 2
            else:
                //# fetch length value
                num = b.send((2, 'read'))
                if num==2:
                    num+=b.send((1, 'read'))
                elif num==3:
                    if b.send((1, 'read')):
                        //# 1.0.111.nnnn  write next (n+3) words
                        num+= RNC._mirror(b.send((4, 'read')),4)
                        num<<=2
                        key = next(k)
                        if index!= None:
                            index.extend([idx_n for i in range(num)])
                            idx_n +=1
                            idx_n%= 16
                        for i in range(num):
                            out.append(next(d)^key)
                        size-=num
                        continue
                    //# 1.0.110   length of 8
                    num+=1
                //# 1.0.-L-.-B-.*D    copy length L from D+1 in bank B
                num+=4
            //# fetch bank
            if not back:
                if b.send((1, 'read')):
                    back = b.send((2, 'read'))
                    if back==0:
                        back = 2 + b.send((1, 'read'))
                    elif back>1:
                        if b.send((2, 'peek')) & 2:
                            back<<=1
                            back|= (b.send((2, 'read')) & 1)
                        else:
                            back<<=2
                            b.send((3, 'peek'))
                            v=b.send((1, 'read'))<<1
                            b.send((1, 'read'))
                            v|=b.send((1, 'read'))
                            back+=v
                    back<<=8
                back+=1
            //# Copy num bytes from byte+offset
            back+= next(d)
            for i in range(num):
                v = out[len(out)-back]
                out.append(v)
                size-=1
            //# If keeping an index, update it as well.
            if index!= None:
                for i in range(num):
                    v = index[len(index)-back]
                    index.append(v)
        else:
            //# push single byte
            if index!= None:
                index.append(idx_n)
                idx_n +=1
                idx_n%= 16
            out.append(next(d) ^ next(k))
            size-=1
    return out
}

void dec_file(void *input, void *output, unsigned short key=0, int num=1)
{
    //"""Convenience, reading a header and calling the decompressor
    //until the entire RNC thread is exhausted.
	//
    //First two bits of compressed files indicate special features:
    //    bit 1: Locked (can't decompress with official tools)
    //    bit 2: Encoded
    //"""

	int inputPosition = 0;
	int outputPosition = 0;

    cmd = [RNC.dec0, RNC.dec1, RNC.dec2]

    h = RNC.header(data)
    if h['unsupported'] or h['cmp_type']<0:
        //# Add an error message
        app.writeToLog("\tError: {:02X} is an unrecognized format.\n".format(data[3]),('warning'))
        return None
    elif h.get('archive'):
        //# Should run as an archive to pull properly.
        RNCA.dec_all(data, None, h)
        return None

    //# Indexed types use a filesize table.
    if h.get('indexed'):
        t = h['table']
    else:
        //# Nonindexed use only one file.
        t = [{'cmp_sz':h['cmp_sz'], 'dec_sz':h['dec_sz']}]

    //# Spit out a crc check for the heck of it when able.
    if h.get('crc_in'):
        c = RNC.crc(data[h['h_sz']:h['h_sz'] + h['cmp_sz']])
        //# Temporary!
        //app.writeToLog("Compressed CRC: {:04X} vs. {:04X} (calulated)\n".format(h['crc_in'], c),('highlight') if h['crc_in']==c else ('warning'))

    //# Start from after the header and work onward through list.
    p = h['h_sz']
    e = h['h_sz'] + h['cmp_sz']
    c = h['cmp_type']
    //# Ignore the lock on the file.
    if h.get('locked'):
        app.writeToLog("\tIgnoring lock on file ;*)\n")

    //# This works because type 0 can't be encoded and archives are handled seperately.
    if h.get('encoded'):
        if key:
            output = cmd[c](data[p:p+t[0]['cmp_sz']], t[0]['dec_sz'], key)
            if t[0]['crc_out'] != RNC.crc(output):
                app.writeToLog("\tProvided key {:04X} did not work.\n".format(key))
                key = 0
        if not key:
            app.writeToLog("\tCracking key for encoded file.\n")
            idx = []
            output = cmd[c](data[p:p+t[0]['cmp_sz']], t[0]['dec_sz'], 0, idx)
            k = RNC.keyfinder(h['crc_out'], output, idx, num=num)
            //# If not found returns None.
            if not k:
                app.writeToLog("\tSome error occured in testing.\n",('warning'))
                return
            //# In the event multiples are found, select one.
            if len(k)>1:
                k = ListBoxChoice(app, "Possible Key Values", "Select one key value", k).returnValue()
                if not k:
                    return
            key = k[0]
            app.writeToLog("\tThe key for this file is {:d} (0x{:04X}).\n".format(key, key))
            output = RNC.applykey(output, idx, key)
    //# Extract and append all segments of the file.
    else:
        for i in t:
            output.extend(cmd[c](data[p:p+i['cmp_sz']], i['dec_sz'], key))
            p+=i['cmp_sz']
            if (p>=e) or (len(output)>=h['dec_sz']):
                break

        if h.get('crc_out'):
            c = RNC.crc(output)
            //# Temporary!
            //app.writeToLog("Decompressed CRC: {:04X} vs. {:04X} (calulated)\n".format(h['crc_out'], c),('highlight') if h['crc_out']==c else ('warning'))

    return output
}*/