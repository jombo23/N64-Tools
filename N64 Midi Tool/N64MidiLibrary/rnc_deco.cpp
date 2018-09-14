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

int RncDecoder::unpackM1(const void *input, void *output, unsigned short key, int& packLen) {
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
		(READ_BE_UINT32(inputptr) != RNX_SIGNATURE) && (READ_BE_UINT32(inputptr) != RNX_SIGNATUREV2))
		return NOT_PACKED;

	int version = inputptr[3];

	inputptr += 4;

	// read unpacked/packed file length
	unpackLen = READ_BE_UINT32(inputptr); inputptr += 4;
	packLen = READ_BE_UINT32(inputptr); inputptr += 4;

	unsigned char blocks = *(inputptr + 5);

	//read CRC's
	crcUnpacked = READ_BE_UINT16(inputptr); inputptr += 2;
	crcPacked = READ_BE_UINT16(inputptr); inputptr += 2;
	inputptr = (inputptr + HEADER_LEN - 16);

	if (crcBlock(inputptr, packLen) != crcPacked)
		return PACKED_CRC;

	inputptr = (((const unsigned char *)input) + HEADER_LEN);

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
	}
/*
while (!end) 
{
   if (!get_bit()) 
   {
     *dst++ = get_byte();
   } else 
   {
     if (!get_bit()) 
	 {
       length = 4 + get_bit();
       if (get_bit()) 
	   {
         length = (length - 1) * 2 + get_bit();
         if (length == 9) 
		 {
           length = (get_bits(4) + 3) * 4;
           for (i = 0; i < length; i++)
               *dst++ = get_byte();
           continue;
         }
       }
       offset = get_offset();
     } else {
       if (get_bit()) 
	   {
         if (get_bit()) 
		 {
           length = get_byte() + 8;
           if (length == 8) 
		   {
              get_bit();
              continue; //dunno why
           }
         } else 
		 {
           length = 3;
         }
         offset = get_offset();
       } else 
	   {
         offset = get_byte() + 1;
       }
     }
     memmove(dst, dst - offset, length);
     dst += length;
   }
 }
*/


	if (crcBlock((unsigned char *)output, unpackLen) != crcUnpacked)
		return UNPACKED_CRC;

	// all is done..return the amount of unpacked bytes
	return unpackLen;
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
