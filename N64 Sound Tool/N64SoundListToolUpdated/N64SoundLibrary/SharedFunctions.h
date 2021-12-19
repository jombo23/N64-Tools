#pragma once

#include "SoundToolExports.h"

class SOUNDTOOL_API CSharedFunctions
{
public:
	CSharedFunctions(void);
	~CSharedFunctions(void);

	static int GetSizeFile(CString filename);
	static int HexToInt(char inChar);
	static unsigned long StringHexToLong(CString inString);
	static unsigned char StringToUnsignedChar(CString inString);
	static unsigned char StringToSignedChar(CString inString);
	static unsigned short StringToUnsignedShort(CString inString);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	static void WriteCharToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static unsigned long CharArrayToChar(unsigned char* currentSpot);
	static unsigned short CharArrayToShort(unsigned char* Buffer, int address);
	static unsigned long CharArrayToLong(unsigned char* Buffer, int address);
	static unsigned long CharArrayToChar(unsigned char* Buffer, int address);
	static unsigned long Flip32Bit(unsigned long inLong);
};
