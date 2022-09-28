#pragma once

#include "SoundToolExports.h"


class SOUNDTOOL_API CSharedFunctions
{
public:
	CSharedFunctions(void);
	~CSharedFunctions(void);

	static double round(double number);
	static int GetSizeFile(CString filename);
	static int HexToInt(char inChar);
	static unsigned long StringHexToLong(CString inString);
	static unsigned char StringToUnsignedChar(CString inString);
	static unsigned char StringToSignedChar(CString inString);
	static unsigned short StringToUnsignedShort(CString inString);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	static void WriteCharToBuffer(unsigned char* Buffer, unsigned long address, unsigned char data);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static unsigned long CharArrayToChar(unsigned char* currentSpot);
	static unsigned short CharArrayToShort(unsigned char* Buffer, int address);
	static unsigned long CharArrayToLong(unsigned char* Buffer, int address);
	static unsigned long CharArrayToChar(unsigned char* Buffer, int address);
	static unsigned long Flip32Bit(unsigned long inLong);
	static void WriteFloatToBuffer(unsigned char* Buffer, unsigned long address, float data);
	static void WriteDoubleToBuffer(unsigned char* Buffer, unsigned long address, double data);
	static float CharArrayToFloat(unsigned char* currentSpot);
	static float CharArrayToFloat(unsigned char* Buffer, int address);
	static double CharArrayToDouble(unsigned char* Buffer, int address);
	static void StringToByteArray(unsigned char* dataArray, CString hexStr);
	static unsigned short Flip16Bit(unsigned short ShortValue);
};
