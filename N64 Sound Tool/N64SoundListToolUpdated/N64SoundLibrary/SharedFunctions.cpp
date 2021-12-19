#include "StdAfx.h"
#include "SharedFunctions.h"

CSharedFunctions::CSharedFunctions(void)
{
}

CSharedFunctions::~CSharedFunctions(void)
{
}

int CSharedFunctions::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

int CSharedFunctions::HexToInt(char inChar)
{
	switch(inChar)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
		return 10;
	case 'a':
		return 10;
	case 'B':
		return 11;
	case 'b':
		return 11;
	case 'C':
		return 12;
	case 'c':
		return 12;
	case 'D':
		return 13;
	case 'd':
		return 13;
	case 'E':
		return 14;
	case 'e':
		return 14;
	case 'F':
		return 15;
	case 'f':
		return 15;
	default:
		return 0;
	}
}

unsigned short CSharedFunctions::StringToUnsignedShort(CString inString)
{
	int tempA = inString.GetLength();
	if (inString.GetLength() < 4)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (4-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 4; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((3-x)*4);
	}
	return tempLong;
}

unsigned char CSharedFunctions::StringToSignedChar(CString inString)
{
	int tempA = inString.GetLength();
	if (inString.GetLength() < 2)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (2-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = (inString.GetLength() - 2);x < (inString.GetLength()); x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((1-x)*4);
	}
	return (unsigned char) tempLong;
}

unsigned char CSharedFunctions::StringToUnsignedChar(CString inString)
{
	int tempA = inString.GetLength();
	if (inString.GetLength() < 2)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (2-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 2; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((1-x)*4);
	}
	return (unsigned char) tempLong;
}

unsigned long CSharedFunctions::StringHexToLong(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 8)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (8-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 8; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((7-x)*4);
	}
	return tempLong;
}

void CSharedFunctions::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{

	if (address > 0x80800000)
		return;

	address = address & 0xFFFFFFF;

	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CSharedFunctions::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	if (address > 0x80800000)
		return;

	address = address & 0xFFFFFFF;

	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

void CSharedFunctions::WriteCharToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{

	if (address > 0x80800000)
		return;

	address = address & 0xFFFFFFF;

	Buffer[address] = ((data) & 0xFF);	
}


unsigned short CSharedFunctions::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CSharedFunctions::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

unsigned long CSharedFunctions::CharArrayToChar(unsigned char* currentSpot)
{
	return currentSpot[0];
}

unsigned short CSharedFunctions::CharArrayToShort(unsigned char* Buffer, int address)
{
	if (address > 0x80800000)
		return 0;

	address = address & 0xFFFFFFF;

	return ((Buffer[address + 0] << 8) | Buffer[address + 1]);
}

unsigned long CSharedFunctions::CharArrayToLong(unsigned char* Buffer, int address)
{
	if (address > 0x80800000)
		return 0;

	address = address & 0xFFFFFFF;

	/*if (std::find(addressesWrite.begin(), addressesWrite.end(), address) == addressesWrite.end())
	{
		if (std::find(addresses.begin(), addresses.end(), address) == addresses.end())
		{
			addresses.push_back(address);
			addressesValues.push_back(((((((Buffer[address + 0] << 8) | Buffer[address + 1]) << 8) | Buffer[address + 2]) << 8) | Buffer[address + 3]));
		}
	}*/

	return ((((((Buffer[address + 0] << 8) | Buffer[address + 1]) << 8) | Buffer[address + 2]) << 8) | Buffer[address + 3]);
}

unsigned long CSharedFunctions::CharArrayToChar(unsigned char* Buffer, int address)
{
	if (address > 0x80800000)
		return 0;

	address = address & 0xFFFFFFF;

	return Buffer[address];
}

unsigned long CSharedFunctions::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}