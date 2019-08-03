//By Zoinkity!
#pragma once

#include "MidiExports.h"

class MIDI_API CAidynDecoder
{
public:
	CAidynDecoder(void);
	~CAidynDecoder(void);
	static unsigned long bit(unsigned char* data, int& dataPosition, int dataLength, unsigned long cmdlen, unsigned long& cmd, CString mode);
	static int decAidyn(unsigned char* data, int dataLength, unsigned char* output, int& outputPosition, int cmdlen = 1);
	static unsigned char* DecompressAidynFile(unsigned char* ROM, int fileTableAddress, int startDataAddress, int fileID, int& decompressedSize);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
};
