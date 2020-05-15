#include "stdafx.h"
#include "SSBMain21990Setup.h"
#include <algorithm>
#include "VPK0Decoder.h"
#include <iostream>

using namespace std;

CSSBMain21990Setup::CSSBMain21990Setup()
{
	SSBROM = NULL;
	romSize = 0;
}


CSSBMain21990Setup::~CSSBMain21990Setup()
{
	if (SSBROM)
	{
		delete[] SSBROM;
		SSBROM = NULL;
	}

	ClearFileTable();
}

void CSSBMain21990Setup::Initialize(int chosenGame)
{
	game = chosenGame;
}

unsigned long CSSBMain21990Setup::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");

	if (inFile == NULL)
		return 0;

	fseek(inFile, 0, SEEK_END);
	unsigned long fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

CString CSSBMain21990Setup::CharArrayToString(unsigned char* currentSpot)
{
	unsigned char* tempSpot = currentSpot;
	CString returnString;
	while (*tempSpot != 0x0)
	{
		CString tempChar;
		tempChar.Format("%c", *tempSpot);
		returnString += tempChar;
		tempSpot++;
	}
	return returnString;
}

CString CSSBMain21990Setup::CharArrayToString20ByRef(unsigned char* currentSpot)
{
	CString returnString;
	while (*currentSpot != 0x20)
	{
		CString tempChar;
		tempChar.Format("%c", *currentSpot);
		returnString += tempChar;
		currentSpot++;
	}
	return returnString;
}

unsigned short CSSBMain21990Setup::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CSSBMain21990Setup::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CSSBMain21990Setup::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

float CSSBMain21990Setup::CharArrayToFloat(unsigned char* currentSpot)
{
	unsigned long tempLong = CharArrayToLong(currentSpot);
	return (*reinterpret_cast<float*> (&tempLong));
}

unsigned long CSSBMain21990Setup::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

int CSSBMain21990Setup::_httoi(const TCHAR *value)
{
	struct CHexMap
	{
		TCHAR chr;
		int value;
	};
	const int HexMapL = 16;
	CHexMap HexMap[HexMapL] =
	{
		{ '0', 0 }, { '1', 1 },
		{ '2', 2 }, { '3', 3 },
		{ '4', 4 }, { '5', 5 },
		{ '6', 6 }, { '7', 7 },
		{ '8', 8 }, { '9', 9 },
		{ 'A', 10 }, { 'B', 11 },
		{ 'C', 12 }, { 'D', 13 },
		{ 'E', 14 }, { 'F', 15 }
	};
	TCHAR *mstr = _tcsupr(_tcsdup(value));
	TCHAR *s = mstr;
	int result = 0;
	if (*s == '0' && *(s + 1) == 'X') s += 2;
	bool firsttime = true;
	while (*s != '\0')
	{
		bool found = false;
		for (int i = 0; i < HexMapL; i++)
		{
			if (*s == HexMap[i].chr)
			{
				if (!firsttime) result <<= 4;
				result |= HexMap[i].value;
				found = true;
				break;
			}
		}
		if (!found) break;
		s++;
		firsttime = false;
	}
	free(mstr);
	return result;
}

int CSSBMain21990Setup::HexToInt(char inChar)
{
	switch (inChar)
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

unsigned long CSSBMain21990Setup::StringHexToLong(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 8)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (8 - tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0; x < 8; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt << ((7 - x) * 4);
	}
	return tempLong;
}

unsigned short CSSBMain21990Setup::StringToUnsignedShort(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 4)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (4 - tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0; x < 4; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt << ((3 - x) * 4);
	}
	return tempLong;
}

unsigned char CSSBMain21990Setup::StringToUnsignedChar(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 2)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (2 - tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0; x < 2; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt << ((1 - x) * 4);
	}
	return (unsigned char)tempLong;
}

bool CSSBMain21990Setup::ReturnInvalidHexChar(CString inChar)
{
	char tempChar[100];
	strcpy(tempChar, inChar);


	switch (tempChar[0])
	{
	case '0':
		return true;
	case '1':
		return true;
	case '2':
		return true;
	case '3':
		return true;
	case '4':
		return true;
	case '5':
		return true;
	case '6':
		return true;
	case '7':
		return true;
	case '8':
		return true;
	case '9':
		return true;
	case 'A':
		return true;
	case 'a':
		return true;
	case 'B':
		return true;
	case 'b':
		return true;
	case 'C':
		return true;
	case 'c':
		return true;
	case 'D':
		return true;
	case 'd':
		return true;
	case 'E':
		return true;
	case 'e':
		return true;
	case 'F':
		return true;
	case 'f':
		return true;
	default:
		return false;
	}
}

bool CSSBMain21990Setup::Initialize(unsigned char* ROM, unsigned long romSize)
{
	if (SSBROM != NULL)
	{
		delete[] SSBROM;
		SSBROM = NULL;
	}

	this->romSize = romSize;

	SSBROM = new unsigned char[MAX_ROM_SIZE_BYTES];
	memcpy(SSBROM, ROM, romSize);

	bool byteFlippedROM;
	if (SSBROM[0] == 0x37)
		byteFlippedROM = true;
	else if (SSBROM[0] == 0x80)
		byteFlippedROM = false;
	else
	{
		delete[] SSBROM;
		SSBROM = NULL;
		cout << "Not an SSB ROM" << endl;
		return false;
	}

	if ((!byteFlippedROM && SSBROM[0x3F] != 0x00) || (byteFlippedROM && SSBROM[0x3E] != 0x00))
	{
		delete[] SSBROM;
		SSBROM = NULL;
		cout << "Wrong version, NTSC version 1.0 only" << endl;
		return false;
	}

	if (byteFlippedROM == true)
	{
		for (int x = 0; x < romSize; x = x + 2)
		{
			unsigned char tempSpot = SSBROM[x];
			SSBROM[x] = SSBROM[x + 1];
			SSBROM[x + 1] = tempSpot;
		}
		byteFlippedROM = false;
	}

	ReadAllDataFrom21990();

	changedFileListing = false;

	return true;
}

bool CSSBMain21990Setup::Initialize(CString inFilepath1, CString inFilename1)
{
	inFilepath = inFilepath1;
	inFilename = inFilename1;

	romSize = GetSizeFile((inFilepath + inFilename));

	FILE* inROM = fopen((inFilepath + inFilename), "rb");
	if (inROM == NULL)
	{
		cout << "SSBROM not found";
		return false;
	}

	if (SSBROM != NULL)
	{
		delete [] SSBROM;
		SSBROM = NULL;
	}

	SSBROM = new unsigned char[MAX_ROM_SIZE_BYTES];
	fread(SSBROM, 1, romSize, inROM);
	fclose(inROM);

	bool byteFlippedROM;
	if (SSBROM[0] == 0x37)
		byteFlippedROM = true;
	else if (SSBROM[0] == 0x80)
		byteFlippedROM = false;
	else
	{
		delete[] SSBROM;
		SSBROM = NULL;
		cout << "Not a SSB ROM" << endl;
		return false;
	}

	if ((!byteFlippedROM && SSBROM[0x3F] != 0x00) || (byteFlippedROM && SSBROM[0x3E] != 0x00))
	{
		delete[] SSBROM;
		SSBROM = NULL;
		cout << "Wrong version, NTSC version 1.0 only" << endl;
		return false;
	}

	if (byteFlippedROM == true)
	{
		for (int x = 0; x < romSize; x = x + 2)
		{
			unsigned char tempSpot = SSBROM[x];
			SSBROM[x] = SSBROM[x + 1];
			SSBROM[x + 1] = tempSpot;
		}
		byteFlippedROM = false;
	}

	ReadAllDataFrom21990();

	changedFileListing = false;

	return true;
}

bool CSSBMain21990Setup::hiddenExec(PTSTR pCmdLine, CString currentDirectory)
{
	STARTUPINFO si;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&processInfo, sizeof(processInfo));
	/*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
	WaitForSingleObject(processInfo.hProcess, 20000);
	DWORD exitCode;
	if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
	{
		if (exitCode == STILL_ACTIVE)
		{
			cout << "For some reason GZip Failed" << endl;
			TerminateProcess(processInfo.hProcess, exitCode);

			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
			return false;
		}
	}
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return true;
};

bool CSSBMain21990Setup::CompressGZipFile(CString inputFile, CString outputFile, bool byteFlipCompressed, int compressedPadSize)
{
	CString gzipFileName = (mainFolder + "gzip.exe");
	char tempFileExistName[1000];
	strcpy(tempFileExistName, (mainFolder + "gzip.exe"));
	if (IsFileExist(tempFileExistName) == false)
	{
		cout << "gzip.exe not found!" << endl;
		return false;
	}

	strcpy(tempFileExistName, inputFile);
	if (IsFileExist(tempFileExistName))
	{
		FILE* tempInputFile = fopen(inputFile, "rb");
		int size = 0;
		fseek(tempInputFile, 0, SEEK_END);
		size = ftell(tempInputFile);
		fseek(tempInputFile, 0, SEEK_SET);

		unsigned char* tempBuffer;
		tempBuffer = new unsigned char[size + 0x10];
		for (int x = 0; x < (size + 0x10); x++)
			tempBuffer[x] = 0;
		fread(tempBuffer, 1, size, tempInputFile);
		if ((game == GOLDENEYE) || (game == PD))
		{
			if ((size % 0x10) != 0)
			{
				size = ((size - (size % 0x10)) + 0x10);
			}
		}
		else if ((game == SNAP) || (game == SSB))
		{
			if ((size % 8) != 0)
			{
				size = ((size - (size % 8)) + 8);
			}
		}

		FILE* tempOutputFile = fopen((mainFolder + "tempgh9.bin"), "wb");
		if (tempOutputFile == 0)
		{
			delete [] tempBuffer;
			fclose(tempInputFile);
			cout << "Cannot Write Temporary File" << endl;
			return false;
		}

		fwrite(tempBuffer, 1, size, tempOutputFile);

		fclose(tempInputFile);
		fflush(tempOutputFile);
		fclose(tempOutputFile);

		delete[] tempBuffer;

		::SetCurrentDirectory(mainFolder);
		CString tempStr = ("gzip.exe -f -q -9 tempgh9.bin");
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));
		CString outputGZippedName = (mainFolder + "TEMPGH9.BIZ");

		strcpy(tempFileExistName, outputGZippedName);
		if (!IsFileExist(tempFileExistName))
		{
			outputGZippedName = (mainFolder + "tempgh9.bin.gz");;
		}

		strcpy(tempFileExistName, outputGZippedName);
		if (IsFileExist(tempFileExistName))
		{
			FILE* inputFileName = fopen(outputGZippedName, "rb");
			if (inputFileName == NULL)
			{
				cout << "Error opening output file" << endl;
				return false;
			}
			int sizeNew = 0;
			fseek(inputFileName, 0, SEEK_END);
			sizeNew = ftell(inputFileName);
			fseek(inputFileName, 0, SEEK_SET);

			unsigned char* tempBufferNew;
			tempBufferNew = new unsigned char[sizeNew + 1];
			fread(tempBufferNew, 1, sizeNew, inputFileName);

			fclose(inputFileName);
			DeleteFile(outputGZippedName);
			FILE* outputFileName = fopen(outputFile, "wb");

			if (outputFileName == NULL)
			{
				delete[] tempBufferNew;
				cout << "Error cannot open output file" << endl;
				return false;
			}

			if (game == GOLDENEYE)
			{
				tempBufferNew[0x14] = 0x11;
				tempBufferNew[0x15] = 0x72;
			}
			else if (game == PD)
			{
				tempBufferNew[0x11] = 0x11;
				tempBufferNew[0x12] = 0x73;
				tempBufferNew[0x13] = ((size >> 16) & 0xFF);
				tempBufferNew[0x14] = ((size >> 8) & 0xFF);
				tempBufferNew[0x15] = ((size)& 0xFF);
			}
			else if ((game == SNAP) || (game == SSB))
			{
				tempBufferNew[0x11] = (size & 0xFF);
				tempBufferNew[0x12] = ((size >> 8) & 0xFF);
				tempBufferNew[0x13] = ((size >> 16) & 0xFF);
				tempBufferNew[0x14] = ((size >> 24) & 0xFF);
				tempBufferNew[0x15] = 0x09;
			}

			if (byteFlipCompressed == true)
			{
				if (game == GOLDENEYE)
				{
					if ((sizeNew % 2) == 1)
					{
						tempBufferNew[sizeNew - 0x8] = 0;
						sizeNew++;
					}

					for (int x = 0; x < sizeNew; x = x + 2)
					{
						unsigned char tempSpot = tempBufferNew[x];
						tempBufferNew[x] = tempBufferNew[x + 1];
						tempBufferNew[x + 1] = tempSpot;
					}
				}
				else if (game == PD)
				{
					if (((sizeNew + 1) % 2) == 1)
					{
						tempBufferNew[sizeNew - 0x8] = 0;
						sizeNew++;
					}

					for (int x = 1; x < sizeNew; x = x + 2)
					{
						unsigned char tempSpot = tempBufferNew[x];
						tempBufferNew[x] = tempBufferNew[x + 1];
						tempBufferNew[x + 1] = tempSpot;
					}
				}
				else if ((game == SNAP) || (game == SSB))
				{
					if (((sizeNew + 1) % 2) == 1)
					{
						tempBufferNew[sizeNew - 0x8] = 0;
						sizeNew++;
					}

					for (int x = 1; x < sizeNew; x = x + 2)
					{
						unsigned char tempSpot = tempBufferNew[x];
						tempBufferNew[x] = tempBufferNew[x + 1];
						tempBufferNew[x + 1] = tempSpot;
					}
				}
			}
			unsigned long fileSizeNew;
			if (game == GOLDENEYE)
			{
				fwrite((tempBufferNew + 0x14), 1, (sizeNew - 0x1C), outputFileName);
				fileSizeNew = (sizeNew - 0x1C);
			}
			else if (game == PD)
			{
				fwrite((tempBufferNew + 0x11), 1, (sizeNew - 0x19), outputFileName);
				fileSizeNew = (sizeNew - 0x19);
			}
			else if ((game == SNAP) || (game == SSB))
			{
				fwrite((tempBufferNew + 0x11), 1, (sizeNew - 0x19), outputFileName);
				fileSizeNew = (sizeNew - 0x19);
			}


			if ((fileSizeNew % compressedPadSize) != 0)
			{
				unsigned char tempZero = 0;
				for (int x = 0; x < (compressedPadSize - (fileSizeNew % compressedPadSize)); x++)
					fwrite(&tempZero, 1, 1, outputFileName);
			}
			delete[] tempBufferNew;
			fflush(outputFileName);
			fclose(outputFileName);
			return true;


		}
		else
		{
			cout << "Error Compressing - GZip didn't spit out a file" << endl;
			return false;
		}
	}

	return true;
}

bool CSSBMain21990Setup::IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return false;
	else 
		return true;
}

void CSSBMain21990Setup::ClearFileTable()
{
	for (int x = 0; x < ssbFileTable.size(); x++)
	{
		if (ssbFileTable[x].dataCompressed != NULL)
		{
			delete [] ssbFileTable[x].dataCompressed;
			ssbFileTable[x].dataCompressed = NULL;
		}
	}

	ssbFileTable.clear();
}

void CSSBMain21990Setup::ReadFileList()
{
	fileTableStart = CharArrayToLong(&SSBROM[0x41F08]); //0x1AC870
	int numberEntries = ReadAddiuAddress(SSBROM, 0x527E8, 0x527F8); //0x00000854
	fileTableEnd = fileTableStart + (numberEntries * 0xC) + 0xC; //0x1B2C6C

	ClearFileTable();

	int fileTableIndex = 0;
	for (int address = fileTableStart; address < (fileTableEnd - 0xC); address += 0xC)
	{
		SSBFileTableEntry fileTableEntry;
		fileTableEntry.index = fileTableIndex++;
		fileTableEntry.dataOffset = CharArrayToLong(&SSBROM[address]);
		fileTableEntry.isCompressed = (fileTableEntry.dataOffset < 0);
		fileTableEntry.dataOffset = fileTableEntry.dataOffset & 0x7FFFFFFF;
		
		fileTableEntry.internalResourceTableOffsetWords = CharArrayToShort(&SSBROM[address + 4]);
		fileTableEntry.compressedSizeWords = CharArrayToShort(&SSBROM[address + 6]);
		fileTableEntry.internalResourceDataWords = CharArrayToShort(&SSBROM[address + 8]);
		fileTableEntry.decompressedSizeWords = CharArrayToShort(&SSBROM[address + 0xA]);

		int trueCompressedSize = 0;
		if (address < (fileTableEnd - 0xC))
		{
			trueCompressedSize = ((CharArrayToLong(&SSBROM[address + 0xC]) & 0x7FFFFFFF) - (CharArrayToLong(&SSBROM[address]) & 0x7FFFFFFF));
		}
		else
		{
			trueCompressedSize = fileTableEntry.compressedSizeWords * 4;
		}

		fileTableEntry.dataCompressed = new unsigned char[fileTableEntry.compressedSizeWords * 4];
		memcpy(fileTableEntry.dataCompressed, &SSBROM[fileTableEnd + fileTableEntry.dataOffset], (fileTableEntry.compressedSizeWords * 4));

		if (trueCompressedSize != (fileTableEntry.compressedSizeWords * 4))
		{
			int delta = (trueCompressedSize - (fileTableEntry.compressedSizeWords * 4));

			for (int x = 0; x < delta; x+=2)
			{
				fileTableEntry.reqFile.push_back(CharArrayToShort(&SSBROM[(fileTableEnd + fileTableEntry.dataOffset) + (fileTableEntry.compressedSizeWords * 4) + x]));
			}
		}

		ssbFileTable.push_back(fileTableEntry);
	}
}

void CSSBMain21990Setup::ReadAllDataFrom21990()
{
	ReadFileList();
	ReadMidiFileList();
	
}

void CSSBMain21990Setup::GetDecompressedFile(unsigned char*& data, int& dataSize, int fileNumber)
{
	dataSize = ssbFileTable[fileNumber].decompressedSizeWords * 4;
	data = new unsigned char[dataSize];

	if (ssbFileTable[fileNumber].isCompressed)
	{
		vpkDecoder.dec(ssbFileTable[fileNumber].dataCompressed + 8, (ssbFileTable[fileNumber].compressedSizeWords * 4), dataSize, data);
	}
	else
	{
		memcpy(data, ssbFileTable[fileNumber].dataCompressed, dataSize);
	}
}

bool CSSBMain21990Setup::InjectFileIntoROM(unsigned char* buffer, int bufferSize, int index, int internalResourceTableOffset, int compressionLevel, int internalResourceDataOffset)
{
	CString workingFolder;
	workingFolder.Format("%s\\Temp\\", mainFolder);

	FILE* outTemp = fopen(workingFolder + "TempSSBImgA1.bin", "wb");
	if (outTemp == NULL)
	{
		cout << "Can't open file " + workingFolder + "TempSSBImgA1.bin" << endl;
		return false;
	}
	fwrite(buffer, 1, bufferSize, outTemp);
	fclose(outTemp);

	return InjectFileIntoROM(workingFolder + "TempSSBImgA1.bin", index, internalResourceTableOffset, compressionLevel, internalResourceDataOffset);
	return true;
}

bool CSSBMain21990Setup::InjectFileIntoROM(CString inputfilename, int index, int internalResourceTableOffset, int compressionLevel, int internalResourceDataOffset)
{
	int sizeUncompressed = GetSizeFile(inputfilename);

	CString workingFolder;
	workingFolder.Format("%s\\Temp\\", mainFolder);

	int fileTableIndex = index;

	if (ssbFileTable[fileTableIndex].isCompressed)
	{
		CString compressionLevelStr;
		compressionLevelStr.Format("%d", compressionLevel);

		if (vpkDecoder.CompressVPK0File(mainFolder, inputfilename, workingFolder + "ResourceSUBTemp.cmp", compressionLevelStr))
		{
			int sizeCompressed = GetSizeFile(workingFolder + "ResourceSUBTemp.cmp");

			if (sizeCompressed == 0)
			{
				if (compressionLevel == 2)
				{
					if (vpkDecoder.CompressVPK0File(mainFolder, inputfilename, workingFolder + "ResourceSUBTemp.cmp", "1"))
					{
						sizeCompressed = GetSizeFile(workingFolder + "ResourceSUBTemp.cmp");
					}
					else
					{
						cout << "Cannot compress SUB, 0 sized" << endl;
						return false;
					}
				}
			}

			if (sizeCompressed > 0)
			{
				changedFileListing = true;

				int paddedCompressedSize = sizeCompressed;
				if ((paddedCompressedSize % 0x10) != 0)
					paddedCompressedSize += (0x10 - (paddedCompressedSize % 0x10));

				ssbFileTable[fileTableIndex].decompressedSizeWords = sizeUncompressed / 4;

				int trueCompressedSize = paddedCompressedSize;

				ssbFileTable[fileTableIndex].compressedSizeWords = trueCompressedSize >> 2;

				delete [] ssbFileTable[fileTableIndex].dataCompressed;
				ssbFileTable[fileTableIndex].dataCompressed = new unsigned char[trueCompressedSize];

				for (int x = 0; x < trueCompressedSize; x++)
					ssbFileTable[fileTableIndex].dataCompressed[x] = 0x00;

				if (internalResourceTableOffset != -1)
					ssbFileTable[fileTableIndex].internalResourceTableOffsetWords = internalResourceTableOffset >> 2;

				if (internalResourceDataOffset != -1)
					ssbFileTable[fileTableIndex].internalResourceDataWords = internalResourceDataOffset >> 2;

				FILE* inVpk = fopen(workingFolder + "ResourceSUBTemp.cmp", "rb");
				if (inVpk == NULL)
				{
					cout << "Error can't open compressed file" << endl;
					return false;
				}
				fread(ssbFileTable[fileTableIndex].dataCompressed, 1, sizeCompressed, inVpk);
				fclose(inVpk);
			}
			else
			{
				cout << "Cannot compress SUB, 0 sized" << endl;
				return false;
			}
		}
		else
		{
			cout << "Cannot compress SUB" << endl;
			return false;
		}
	}
	else
	{
		if (sizeUncompressed > 0)
		{
			changedFileListing = true;

			int paddedCompressedSize = sizeUncompressed;
			if ((paddedCompressedSize % 0x10) != 0)
				paddedCompressedSize += (0x10 - (paddedCompressedSize % 0x10));

			ssbFileTable[fileTableIndex].decompressedSizeWords = paddedCompressedSize / 4; // uncompressed files *must* align to upper 0x10

			int trueUncompressedSize = paddedCompressedSize;

			ssbFileTable[fileTableIndex].compressedSizeWords = trueUncompressedSize >> 2;

			delete [] ssbFileTable[fileTableIndex].dataCompressed;
			ssbFileTable[fileTableIndex].dataCompressed = new unsigned char[trueUncompressedSize];

			for (int x = 0; x < trueUncompressedSize; x++)
				ssbFileTable[fileTableIndex].dataCompressed[x] = 0x00;

			if (internalResourceTableOffset != -1)
				ssbFileTable[fileTableIndex].internalResourceTableOffsetWords = internalResourceTableOffset >> 2;

			if (internalResourceDataOffset != -1)
				ssbFileTable[fileTableIndex].internalResourceDataWords = internalResourceDataOffset >> 2;

			FILE* inUncompressedFile = fopen(inputfilename, "rb");
			if (inUncompressedFile == NULL)
			{
				cout << "Error can't open input file" << endl;
				return false;
			}
			fread(ssbFileTable[fileTableIndex].dataCompressed, 1, sizeUncompressed, inUncompressedFile);
			fclose(inUncompressedFile);
		}
		else
		{
			cout << "0 Sized file" << endl;
			return false;
		}
	}

	return true;
}

unsigned long CSSBMain21990Setup::ReadAddiuAddress(unsigned char* SSBROM, unsigned long upperLocation, unsigned long lowerLocation)
{
	unsigned short upper = CharArrayToShort(&SSBROM[upperLocation+2]);
	unsigned short lower = CharArrayToShort(&SSBROM[lowerLocation+2]);
	if (lower > 0x7FFF)
	{
		return ((upper - 1) << 16) | lower;
	}
	else
	{
		return ((upper) << 16) | lower;
	}
}

void CSSBMain21990Setup::WriteAddiuAddress(unsigned char* SSBROM, unsigned long address, unsigned long upperLocation, unsigned long lowerLocation)
{
	unsigned short upper = ((address >> 16) & 0xFFFF);
	unsigned short lower = (address & 0xFFFF);

	if (lower > 0x7FFF)
	{
		upper += 1;
	}

	WriteShortToBuffer(SSBROM, (upperLocation+2), upper);
	WriteShortToBuffer(SSBROM, (lowerLocation+2), lower);
}

void CSSBMain21990Setup::ChangeAddress(unsigned char* SSBROM, int change, unsigned long location)
{
	unsigned long address = CharArrayToLong(&SSBROM[location]);
	address += change;
	WriteLongToBuffer(SSBROM, location, address);
}

void CSSBMain21990Setup::ChangeAddiuAddress(unsigned char* SSBROM, int change, unsigned long upperLocation, unsigned long lowerLocation)
{
	unsigned long address = ReadAddiuAddress(SSBROM, upperLocation, lowerLocation);
	address += change;

	unsigned short upper = ((address >> 16) & 0xFFFF);
	unsigned short lower = (address & 0xFFFF);

	if (lower > 0x7FFF)
	{
		upper += 1;
	}

	WriteShortToBuffer(SSBROM, (upperLocation+2), upper);
	WriteShortToBuffer(SSBROM, (lowerLocation+2), lower);
}

void CSSBMain21990Setup::WriteFileList()
{
	// Pre-get address
	fileTableStart = CharArrayToLong(&SSBROM[0x41F08]); //0x1AC870
	int numberEntries = ssbFileTable.size();
	fileTableEnd = fileTableStart + (numberEntries * 0xC) + 0xC; //0x1B2C6C

	int currentDataAddress = fileTableEnd;
	for (int x = 0; x < ssbFileTable.size(); x++)
	{
		currentDataAddress += (ssbFileTable[x].compressedSizeWords * 4);

		for (int y = 0; y < ssbFileTable[x].reqFile.size(); y++)
		{
			currentDataAddress += 2;
		}
	}

	if ((currentDataAddress % 0x10) != 0)
		currentDataAddress = currentDataAddress + (0x10 - (currentDataAddress % 0x10));

	int endSpotFileTableData = currentDataAddress;
	int oldEndFileTable = ReadAddiuAddress(SSBROM, 0x78AB8, 0x78AE0);
	unsigned long oldEndOfROM = CharArrayToLong(&SSBROM[0x3D7A0]);

	int deltaFileChange = (endSpotFileTableData - oldEndFileTable);

	unsigned char testSize = 16; // 16MB
	while ((CharArrayToLong(&SSBROM[0x3D7A0]) + deltaFileChange) > romSize) // expand ROM until new data fits
	{
		const unsigned long testBytes = testSize * 0x100000;
		testSize += 4; // increment 4MB up
		if (testSize > MAX_ROM_SIZE_MB) // if bigger than max ROM limit, abort
		{
			cout << "Too Big change, goes past ROM end" << endl;
			return;
		}
		if (romSize <= testBytes) // if ROM is equal or less than tested size, expand ROM by 4MB and set new ROM region to FF
		{
			const unsigned long newSize = testSize * 0x100000;
			for (unsigned long o = romSize; o < newSize; o++)
				SSBROM[o] = 0xFF;
			romSize = newSize;
		}
	}

	// Move old data into place
	memmove(&SSBROM[endSpotFileTableData], &SSBROM[oldEndFileTable], (oldEndOfROM - oldEndFileTable));


	// Update Pointers
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x78AB8, 0x78AE0);
	//("spr_bank-1", ((0x78AB8, 0x78AE0),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x78ABC, 0x78AD8);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x78AC0, 0x78AD4);
    //("img_bank-1", ((0x78ABC, 0x78AD8),(0x78AC0, 0x78AD4),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x78AC4, 0x78AD0);
	ChangeAddress(SSBROM, deltaFileChange, 0x9EED0);
    //("spr_bank-2", ((0x78AC4, 0x78AD0),0x9EED0,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x9EED4);
	ChangeAddress(SSBROM, deltaFileChange, 0x9EED8);
    //("img_bank-2", (0x9EED4, 0x9EED8)),
	ChangeAddress(SSBROM, deltaFileChange, 0x9EEDC);
	ChangeAddress(SSBROM, deltaFileChange, 0xA2EF0);
    //("spr_bank-3", (0x9EEDC, 0xA2EF0)),
	ChangeAddress(SSBROM, deltaFileChange, 0xA2EF4);
	ChangeAddress(SSBROM, deltaFileChange, 0xA2EF8);
    //("img_bank-3", (0xA2EF4, 0xA2EF8)),
	ChangeAddress(SSBROM, deltaFileChange, 0x9C220);
	ChangeAddress(SSBROM, deltaFileChange, 0xA2EFC);
    //("spr_bank-4", (0x9C220, 0xA2EFC)),
	ChangeAddress(SSBROM, deltaFileChange, 0x9C224);
	ChangeAddress(SSBROM, deltaFileChange, 0x9C228);
    //("img_bank-4", (0x9C224, 0x9C228)),
	ChangeAddress(SSBROM, deltaFileChange, 0x9C22C);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0xE8994, 0xE89B8);
    //("spr_bank-5", (0x9C22C, (0xE8994, 0xE89B8),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0xE8998, 0xE89B0);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0xE899C, 0xE89AC);
    //("img_bank-5", ((0xE8998, 0xE89B0),(0xE899C, 0xE89AC),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x81E88, 0x81EBC);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0xE89A0, 0xE89A8);
    //("spr_bank-6", ((0x81E88, 0x81EBC),(0xE89A0, 0xE89A8),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x81E8C, 0x81EB4);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x81E90, 0x81EB0);
    //("img_bank-6", ((0x81E8C, 0x81EB4),(0x81E90, 0x81EB0),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x81E94, 0x81EAC);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x862DC, 0x86300);
    //("spr_bank-7", ((0x81E94, 0x81EAC),(0x862DC, 0x86300),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x862E0, 0x862F8);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x862E4, 0x862F4);
    //("img_bank-7", ((0x862E0, 0x862F8),(0x862E4, 0x862F4),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x8441C, 0x8443C);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x862E8, 0x862F0);
    //("spr_bank-8", ((0x8441C, 0x8443C),(0x862E8, 0x862F0),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x84420, 0x84434);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x84424, 0x84430);
    //("img_bank-8", ((0x84420, 0x84434),(0x84424, 0x84430),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x84428, 0x8442C);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x1194A8, 0x1194C4);
    //("spr_bank-9", ((0x84428, 0x8442C),(0x1194A8, 0x1194C4),)),
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x1194AC, 0x1194C0);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x1194B0, 0x1194BC);
    //("img_bank-9", ((0x1194AC, 0x1194C0),(0x1194B0, 0x1194BC),)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D768);
	ChangeAddiuAddress(SSBROM, deltaFileChange, 0x1194B4, 0x1194B8);
    //("S1_music.sbk", (0x3D768, (0x1194B4, 0x1194B8),)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D75C);
    //("B1_sounds1.ctl", (0x3D75C,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D760);
	ChangeAddress(SSBROM, deltaFileChange, 0x3D764);
    //("B1_sounds1.tbl", (0x3D760, 0x3D764,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D750);
    //("B1_sounds2.ctl", (0x3D750,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D754);
	ChangeAddress(SSBROM, deltaFileChange, 0x3D758);
    //("B1_sounds2.tbl", (0x3D754, 0x3D758,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D78C);
    //("sounds3.rates", (0x3D78C,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D790);
	ChangeAddress(SSBROM, deltaFileChange, 0x3D794);
    //("sounds3.seq", (0x3D790, 0x3D794,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D798);
	ChangeAddress(SSBROM, deltaFileChange, 0x3D79C);
    //("sounds4.seq", (0x3D798, 0x3D79C,)),
	ChangeAddress(SSBROM, deltaFileChange, 0x3D7A0);
    //("EOF", (0x3D7A0,)),
	

	for (int x = 0; x < ssbS1Bank.entries.size(); x++)
	{
		ssbS1Bank.entries[x].ROMAddress = ssbS1Bank.entries[x].ROMAddress + deltaFileChange;
	}

	// Write File Table Length
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x527E8, 0x527F8);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x10B228, 0x10B23C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x10B378, 0x10B38C);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x10B928, 0x10B93C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x1134B8, 0x1134CC);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x116EF8, 0x116F0C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x1197C8, 0x1197D8);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x119C98, 0x119CAC);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x11A824, 0x11A834);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x11C214, 0x11C228);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x11D9A4, 0x11D9B4);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x11EFB4, 0x11EFC4);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x120C24, 0x120C34);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x1221BC, 0x1221CC);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x124F7C, 0x124F8C);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x127CEC, 0x127CFC);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x129638, 0x129648);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x12A14C, 0x12A15C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x12AB88, 0x12AB9C);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x12DE80, 0x12DE94);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x12FB34, 0x12FB48);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x139350, 0x139364);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x14053C, 0x140550);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x147294, 0x1472A8);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x14CD10, 0x14CD24);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x14FE7C, 0x14FE90);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x157D18, 0x157D2C);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x15D3D0, 0x15D3E0);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x15FE80, 0x15FE94);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x1652E0, 0x1652F0);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x166430, 0x166444);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x1666F8, 0x16670C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x167838, 0x16784C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x168938, 0x16894C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x169AE8, 0x169AFC);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x16AC18, 0x16AC2C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x16BD18, 0x16BD2C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x16CE78, 0x16CE8C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x16DFD8, 0x16DFEC);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x16F9B4, 0x16F9C8);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x170350, 0x170364);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x171048, 0x17105C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x171ED0, 0x171EE4);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x172890, 0x1728A4);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x1736E4, 0x1736F8);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x174670, 0x174684);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x174948, 0x17495C);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x175C58, 0x175C6C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x17713C, 0x17714C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x178278, 0x17828C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x17A9C0, 0x17A9D4);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x17DF1C, 0x17DF30);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x17E6D8, 0x17E6EC);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x17EE8C, 0x17EEA0);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x1820E4, 0x1820F4);

	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x188690, 0x1886A0);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x18BC58, 0x18BC6C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x18CE08, 0x18CE1C);
	WriteAddiuAddress(SSBROM, ssbFileTable.size(), 0x18D16C, 0x18D17C);

	currentDataAddress = fileTableEnd;
	for (int x = 0; x < ssbFileTable.size(); x++)
	{
		signed long offsetToFile = (currentDataAddress - fileTableEnd);
		if (ssbFileTable[x].isCompressed)
			offsetToFile = offsetToFile | 0x80000000;
		WriteLongToBuffer(SSBROM, fileTableStart + (x * 0xC), offsetToFile);
		WriteShortToBuffer(SSBROM, fileTableStart + (x * 0xC) + 4, ssbFileTable[x].internalResourceTableOffsetWords);
		WriteShortToBuffer(SSBROM, fileTableStart + (x * 0xC) + 6, ssbFileTable[x].compressedSizeWords);
		WriteShortToBuffer(SSBROM, fileTableStart + (x * 0xC) + 8, ssbFileTable[x].internalResourceDataWords);
		WriteShortToBuffer(SSBROM, fileTableStart + (x * 0xC) + 0xA, ssbFileTable[x].decompressedSizeWords);

		memcpy(&SSBROM[currentDataAddress], ssbFileTable[x].dataCompressed, (ssbFileTable[x].compressedSizeWords * 4));
		currentDataAddress += (ssbFileTable[x].compressedSizeWords * 4);

		for (int y = 0; y < ssbFileTable[x].reqFile.size(); y++)
		{
			WriteShortToBuffer(SSBROM, currentDataAddress, ssbFileTable[x].reqFile[y]);
			currentDataAddress += 2;
		}
	}

	// Terminator
	WriteLongToBuffer(SSBROM, fileTableStart + (ssbFileTable.size() * 0xC), (currentDataAddress - fileTableEnd));
	WriteShortToBuffer(SSBROM, fileTableStart + (ssbFileTable.size() * 0xC) + 4, 0);
	WriteShortToBuffer(SSBROM, fileTableStart + (ssbFileTable.size() * 0xC) + 6, 0);
	WriteShortToBuffer(SSBROM, fileTableStart + (ssbFileTable.size() * 0xC) + 8, 0);
	WriteShortToBuffer(SSBROM, fileTableStart + (ssbFileTable.size() * 0xC) + 0xA, 0);

	if ((currentDataAddress % 0x10) != 0)
		currentDataAddress = currentDataAddress + (0x10 - (currentDataAddress % 0x10));


	/*if (currentDataAddress > (0xAC7340))
	{
		CString tempStr;
		tempStr.Format("%04X > %04X", currentDataAddress, 0xAC7340);
		cout << "Too big file table data " + tempStr << endl;
	}*/
}

bool CSSBMain21990Setup::Write21990File()
{
	if (changedFileListing)
	{
		WriteFileList();
		WriteMidiFileList();
	}

	return true;
}

void CSSBMain21990Setup::UnlockEverything()
{
	// Unlock all Characters 1P
	WriteLongToBuffer(SSBROM, 0x001404B0, 0x340EFFFF);

	// Unlock All Characters 1P Training
	WriteLongToBuffer(SSBROM, 0x00147028, 0x3408FFFF);

	// Unlock All Characters Bonus 1P/2P
	WriteLongToBuffer(SSBROM, 0x0014CBF8, 0x340AFFFF);

	// Unlock all Characters VS
	WriteLongToBuffer(SSBROM, 0x00139218, 0x340CFFFF);

	// Enable Sound Test/Item Switch
	WriteLongToBuffer(SSBROM, 0x00125308, 0x240E00FF);
	WriteLongToBuffer(SSBROM, 0x00051E34, 0x240E00FF);
	WriteLongToBuffer(SSBROM, 0x00120F78, 0x240E00FF);

	// Unlock Mushroom Kingdom Stage
	WriteLongToBuffer(SSBROM, 0x0014F894, 0x240F00FF);
	WriteLongToBuffer(SSBROM, 0x00052704, 0x240F00FF);
	WriteLongToBuffer(SSBROM, 0x000EDC70, 0x240F00FF);
	WriteLongToBuffer(SSBROM, 0x0011336C, 0x240F00FF);

	// Enable all Characters in Menu
	WriteLongToBuffer(SSBROM, 0x0015F62C, 0x340EFFFF);
	WriteLongToBuffer(SSBROM, 0x0015F658, 0x240400FF);

	// Enable all Characters for Records
	WriteLongToBuffer(SSBROM, 0x0015CC68, 0x340EFFFF);
	WriteLongToBuffer(SSBROM, 0x001404B0, 0x340EFFFF);
	WriteLongToBuffer(SSBROM, 0x00175B50, 0x340EFFFF);
}

void CSSBMain21990Setup::RelockEverything()
{
	// Lock all Characters 1P
	WriteLongToBuffer(SSBROM, 0x001404B0, 0x95CE4938);

	// Lock All Characters 1P Training
	WriteLongToBuffer(SSBROM, 0x00147028, 0x95084938);

	// Lock All Characters Bonus 1P/2P
	WriteLongToBuffer(SSBROM, 0x0014CBF8, 0x954A4938);

	// Lock all Characters VS
	WriteLongToBuffer(SSBROM, 0x00139218, 0x958C4938);

	// Disable Sound Test/Item Switch
	WriteLongToBuffer(SSBROM, 0x00125308, 0x91CE4937);
	WriteLongToBuffer(SSBROM, 0x00051E34, 0x91CE4937);
	WriteLongToBuffer(SSBROM, 0x00120F78, 0x91CE4937);

	// Lock Mushroom Kingdom Stage
	WriteLongToBuffer(SSBROM, 0x0014F894, 0x91EF4937);
	WriteLongToBuffer(SSBROM, 0x00052704, 0x91EF4937);
	WriteLongToBuffer(SSBROM, 0x000EDC70, 0x91EF4937);
	WriteLongToBuffer(SSBROM, 0x0011336C, 0x91EF4937);

	// Disable all Characters in Menu
	WriteLongToBuffer(SSBROM, 0x0015F62C, 0x94AE0458);
	WriteLongToBuffer(SSBROM, 0x0015F658, 0x90A40456);

	// Disable all Characters for Records
	WriteLongToBuffer(SSBROM, 0x0015CC68, 0x95CE4938);
	WriteLongToBuffer(SSBROM, 0x001404B0, 0x95CE4938);
	WriteLongToBuffer(SSBROM, 0x00175B50, 0x95CE4938);
}

void CSSBMain21990Setup::WriteLongToFile(FILE* outFile, unsigned long data)
{
	unsigned long tempLong = Flip32Bit(data);
	fwrite(&tempLong, 1, 4, outFile);
}

void CSSBMain21990Setup::WriteShortToFile(FILE* outFile, unsigned short data)
{
	unsigned short tempLong = Flip16Bit(data);
	fwrite(&tempLong, 1, 2, outFile);
}

void CSSBMain21990Setup::WriteFloatToBuffer(unsigned char* Buffer, unsigned long address, float data)
{
	WriteLongToBuffer(Buffer, address, (*reinterpret_cast<unsigned long*> (&data)));
}

void CSSBMain21990Setup::WriteHigherToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	unsigned short upper = (data >> 16);
	unsigned short lower = (data & 0xFFFF);
	WriteShortToBuffer(Buffer, address, upper);
}
void CSSBMain21990Setup::WriteLowerToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	unsigned short upper = (data >> 16);
	unsigned short lower = (data & 0xFFFF);
	WriteShortToBuffer(Buffer, address, lower);
}

void CSSBMain21990Setup::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address + 1] = ((data)& 0xFF);
}

void CSSBMain21990Setup::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address + 1] = ((data >> 16) & 0xFF);
	Buffer[address + 2] = ((data >> 8) & 0xFF);
	Buffer[address + 3] = ((data)& 0xFF);
}


bool CSSBMain21990Setup::Compare(std::vector<unsigned char> array, std::vector<unsigned char> needle, int needleLen, int startIndex)
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

void CSSBMain21990Setup::ReadMidiFileList()
{
	int s1BankStart = CharArrayToLong(&SSBROM[0x3D768]); //0xB277B0;
	int numberMidis = CharArrayToShort(&SSBROM[s1BankStart + 2]);

	ssbS1Bank.entries.clear();
	int fileCounter = 0;
	for (int x = 0; x < numberMidis; x++)
	{
		SSBS1Entry ssbS1Entry;
		ssbS1Entry.ROMAddress = s1BankStart + CharArrayToLong(&SSBROM[s1BankStart + 4 + (x * 8)]);
		ssbS1Entry.length = CharArrayToLong(&SSBROM[s1BankStart + 4 + (x * 8) + 4]);

		ssbS1Bank.entries.push_back(ssbS1Entry);
	}
}

void CSSBMain21990Setup::WriteMidiFileList()
{
	int s1BankStart = CharArrayToLong(&SSBROM[0x3D768]); //0xB277B0;

	for (int x = 0; x < ssbS1Bank.entries.size(); x++)
	{
		WriteLongToBuffer(SSBROM, s1BankStart + 4 + (x * 8), ssbS1Bank.entries[x].ROMAddress - s1BankStart);
		WriteLongToBuffer(SSBROM, s1BankStart + 4 + (x * 8) + 4, ssbS1Bank.entries[x].length);
	}
}

bool CSSBMain21990Setup::InjectMidiFileIntoROM(CString inputfilename, int fileID)
{
	CString workingFolder;
	workingFolder.Format("%s\\Temp\\", mainFolder);

	CString tempFileNameUncompressed = inputfilename;

	unsigned long romLocation = ssbS1Bank.entries[fileID].ROMAddress;

	FILE* inTempROM = fopen(inputfilename, "rb");
	if (inTempROM == NULL)
	{
		cout << "Can't open ROM file " + inputfilename << endl;
		return false;
	}

	unsigned long outNewFileSize = GetSizeFile(inputfilename);
	unsigned long thisFileSize = outNewFileSize;
	unsigned long outOldFileSize = (ssbS1Bank.entries[fileID].length);
	if ((outOldFileSize % 4) != 0)
	{
		outOldFileSize = (outOldFileSize + (4 - (outOldFileSize % 4)));
	}

	if ((outNewFileSize % 4) != 0)
	{
		outNewFileSize = (outNewFileSize + (4 - (outNewFileSize % 4)));
	}

	signed long change = (outNewFileSize - outOldFileSize);


	unsigned char* BufferTempFile;
	BufferTempFile = new unsigned char[thisFileSize];
	rewind(inTempROM);
	fread(BufferTempFile, 1, thisFileSize, inTempROM);

	unsigned long endAddress = CharArrayToLong(&SSBROM[0x3D75C]); //0xB4E5C0;

	if (outNewFileSize == outOldFileSize)
	{
		// don't do anything special
	}
	else if (outNewFileSize < outOldFileSize)
	{
		for (int x = (fileID + 1); x < ssbS1Bank.entries.size(); x++)
		{
			ssbS1Bank.entries[x].ROMAddress = (unsigned long)((signed long)ssbS1Bank.entries[x].ROMAddress + (signed long)change);
		}
		
		memcpy(&SSBROM[ssbS1Bank.entries[fileID].ROMAddress + outNewFileSize], &SSBROM[ssbS1Bank.entries[fileID].ROMAddress + outOldFileSize], (endAddress - (ssbS1Bank.entries[fileID].ROMAddress + outOldFileSize)));
	}
	else // if (outNewFileSize > outOldFileSize)
	{

		unsigned long endSpot = ssbS1Bank.entries[ssbS1Bank.entries.size()-1].ROMAddress + ssbS1Bank.entries[ssbS1Bank.entries.size()-1].length;
		if ((endSpot % 4) != 0)
			endSpot = endSpot + (4 - (endSpot % 4));

		if ((endSpot + change) > endAddress)
		{
			fclose(inTempROM);
			delete [] BufferTempFile;
			cout << "Too big change in midi" << endl;
			return false;
		}

		for (int x = (fileID + 1); x < ssbS1Bank.entries.size(); x++)
		{
			ssbS1Bank.entries[x].ROMAddress = (unsigned long)((signed long)ssbS1Bank.entries[x].ROMAddress + (signed long)change);
		}


		for (int x = (endSpot + change); x >= (ssbS1Bank.entries[fileID].ROMAddress + outNewFileSize); x--)
		{
			SSBROM[x] = SSBROM[x - change];
		}
	}

	for (int x = romLocation; x < (romLocation + thisFileSize); x++)
	{
		SSBROM[x] = BufferTempFile[x - romLocation];
	}
	delete [] BufferTempFile;
	fclose(inTempROM);

	ssbS1Bank.entries[fileID].length = thisFileSize;

	DeleteFile((workingFolder + "tempROMOutw12.bin"));


	changedFileListing = true;

	return true;
}