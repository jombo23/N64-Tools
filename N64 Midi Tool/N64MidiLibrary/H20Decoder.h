#pragma once

#include "MidiExports.h"

class MIDI_API CH20DecoderMidiTool
{
public:
	CH20DecoderMidiTool(void);
	~CH20DecoderMidiTool(void);
	int decPolaris(unsigned char* inputBuffer, int compressedSize, unsigned char* output, bool skipheader = false);
};
