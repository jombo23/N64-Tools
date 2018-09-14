// Spinout
#pragma once

struct ERZHeader
{
	/* 0x00 */ BYTE Magic[3];			// 'ERZ'
	/* 0x03 */ BYTE CompressionType;	// 1 - basic, 2 - advanced
	/* 0x04 */ DWORD DecompressedSize;	// Size of the decompressed data
	/* 0x08 */ DWORD CompressedSize;	// Size of compressed data
};

class ERZ
{
public:
	int decode(unsigned char * src, int filesize, unsigned char* result, int& fileSizeCompressed);
};
