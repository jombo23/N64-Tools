#pragma once
#include "structs.h"
#include <map>
#include <vector>
#include "VPK0Decoder.h"

struct SSBFileTableEntry
{
	int index;
	bool isCompressed;

	signed long dataOffset;
	// Internal to file offsets
	unsigned short internalResourceTableOffsetWords;
	unsigned short compressedSizeWords;
	// Req files
	unsigned short internalResourceDataWords;
	unsigned short decompressedSizeWords;
	
	unsigned char* dataCompressed;

	std::vector<unsigned short> reqFile;

	SSBFileTableEntry()
	{
		dataCompressed = NULL;
	}
};

struct SSBS1Entry
{
	unsigned long ROMAddress;
	unsigned long length;
	CString name;
};

struct SSBS1Bank
{
	std::vector<SSBS1Entry> entries;
};

class CSSBMain21990Setup
{
public:
	CSSBMain21990Setup(void);
	~CSSBMain21990Setup(void);

	void Initialize(int chosenGame);

	unsigned long CharArrayToLong(unsigned char* currentSpot);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	float CharArrayToFloat(unsigned char* currentSpot);
	CString CharArrayToString(unsigned char* currentSpot);
	CString CharArrayToString20ByRef(unsigned char* currentSpot);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned long Flip32Bit(unsigned long inLong);

	int game;

	bool Initialize(CString inFilepath1, CString inFilename1);
	bool Initialize(unsigned char* ROM, unsigned long romSize);

	unsigned char* SSBROM;
	unsigned long romSize;

	CString inFilename;
	CString inFilepath;

	unsigned long GetSizeFile(CString filename);

	int HexToInt(char inChar);
	int _httoi(const TCHAR *value);
	unsigned long StringHexToLong(CString inString);
	unsigned short StringToUnsignedShort(CString inString);
	unsigned char StringToUnsignedChar(CString inString);
	bool ReturnInvalidHexChar(CString inChar);

	CString mainFolder;

	bool CompressGZipFile(CString inputFile, CString outputFile, bool byteFlipCompressed, int compressedPadSize);
	bool IsFileExist(LPSTR lpszFilename);

	bool hiddenExec(PTSTR pCmdLine, CString currentDirectory);

	bool changedFileListing;

	unsigned long fileTableStart;
	unsigned long fileTableEnd;
	std::vector<SSBFileTableEntry> ssbFileTable;
	void ClearFileTable();
	void GetDecompressedFile(unsigned char*& data, int& dataSize, int fileNumber);
	bool InjectFileIntoROM(unsigned char* buffer, int bufferSize, int index, int internalResourceTableOffset = -1, int compressionLevel = 2, int internalResourceDataOffset = -1);
	bool InjectFileIntoROM(CString inputfilename, int index, int internalResourceTableOffset = -1, int compressionLevel = 2, int internalResourceDataOffset = -1);
	bool InjectMidiFileIntoROM(CString inputfilename, int fileID);

	bool Compare(std::vector<unsigned char> array, std::vector<unsigned char> needle, int needleLen, int startIndex);
	void ReadFileList();
	void ReadAllDataFrom21990();
	void ReadMidiFileList();

	void WriteFileList();
	bool Write21990File();
	void WriteMidiFileList();

	void UnlockEverything();
	void RelockEverything();

	void WriteLongToFile(FILE* Buffer, unsigned long data);
	void WriteShortToFile(FILE* Buffer, unsigned short data);
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	void WriteFloatToBuffer(unsigned char* Buffer, unsigned long address, float data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	void WriteHigherToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	void WriteLowerToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);

	static unsigned long ReadAddiuAddress(unsigned char* SSBROM, unsigned long upperLocation, unsigned long lowerLocation);
	static void WriteAddiuAddress(unsigned char* SSBROM, unsigned long address, unsigned long upperLocation, unsigned long lowerLocation);
	void ChangeAddiuAddress(unsigned char* SSBROM, int change, unsigned long upperLocation, unsigned long lowerLocation);
	void ChangeAddress(unsigned char* SSBROM, int change, unsigned long location);

	CVPK0Decoder vpkDecoder;
	SSBS1Bank ssbS1Bank;
};
