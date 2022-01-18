//By Zoinkity!
#pragma once
#include "MidiExports.h"
#include "AidynDecoder.h"
#include <vector>
#include <map>

struct inf
{
	unsigned long ul[3];
	unsigned long i[1];
};

struct fileData
{
	int fileSize;
	unsigned char* data;

	fileData()
	{
		fileSize = 0;
		data = NULL;
	}
};

struct DCMEntry
{
	unsigned long ul[3];
	unsigned short us[2];
};

class MIDI_API CAidynToDCMConvertor
{
public:
	CAidynToDCMConvertor(void);
	~CAidynToDCMConvertor(void);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static int GetSizeFile(CString filename);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	static unsigned short Flip16Bit(unsigned short ShortValue);
	static unsigned long Flip32Bit(unsigned long inLong);
	static bool convert(unsigned char* data, unsigned char* ROM, int fileTable, int fileTableDataStart, std::map<int, fileData>& smp, CString kind, unsigned char* output, int& outputPosition, int smpOffset);
};
