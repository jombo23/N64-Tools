#pragma once

#include "SoundToolExports.h"

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


#define RNC_SIGNATURE   0x524E4301 // "RNC\001"
#define RNC_SIGNATUREV2   0x524E4302 // "RNC\002"
#define RNC_SIGNATURE81   0x524E4381 // "RNC\081"
#define RNX_SIGNATURE   0x524E5801 // "RNX\001"
#define RNX_SIGNATUREV2   0x524E5802 // "RNX\002"

class SOUNDTOOL_API RncDecoder {

protected:
	unsigned short _rawTable[64];
	unsigned short _posTable[64];
	unsigned short _lenTable[64];
	unsigned short _crcTable[256];

	unsigned short _bitBuffl;
	unsigned short _bitBuffh;
	unsigned char _bitCount;

	const unsigned char *_srcPtr;
	unsigned char *_dstPtr;

public:
	RncDecoder();
	~RncDecoder();
	int unpackM1(const void *input, void *output, unsigned short key, int& packLen);
	bool CompressRNCFile(CString mainFolder, CString inputFile, CString outputFile, bool useRNX);
protected:
	void initCrc();
	unsigned short crcBlock(const unsigned char *block, unsigned long size);
	unsigned short inputBits(unsigned char amount);
	void makeHufftable(unsigned short *table);
	unsigned short inputValue(unsigned short *table);
	unsigned long READ_BE_UINT32(const unsigned char* buffer);
	unsigned short READ_BE_UINT16(const unsigned char* buffer);
	unsigned short READ_LE_UINT16(const unsigned char* buffer);

	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
	BOOL IsFileExist(LPSTR lpszFilename);
	bool get_bit(const unsigned char*& inputptr, unsigned long& bitBuffer, int& bitBufferCount, int& inpOffset);
	int get_offset(const unsigned char*& inputptr, unsigned long& bitBuffer, int& bitBufferCount, int& inpOffset);
	unsigned char get_byte(const unsigned char*& inputptr, int& inpOffset);

};


