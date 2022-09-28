#pragma once

#ifdef DLL_CONFIG
	#ifdef MIDI_READER_EXPORTS
		#define MIDI_READER_API __declspec(dllexport)
	#else
		#define MIDI_READER_API __declspec(dllimport)
	#endif
#else
	#define MIDI_READER_API
#endif

#include "..\N64MidiLibrary\MidiParse.h"
#include <vector>

struct MidiConfig
{
	unsigned long start;
	unsigned long end;
	unsigned long extra;
	unsigned long extra2;

	MidiConfig()
	{
		extra = 0;
		extra2 = 0;
	}
};

struct MidiGameConfig
{
	int numberMidis;
	CString gameType;
	CString gameName;
	MidiConfig* midiBanks;
	int numberMidiNames;
	CString* midiNames;
	ExtraGameMidiInfo extraGameMidiInfo;

	MidiGameConfig()
	{
		midiBanks = NULL;
		midiNames = NULL;
	}
};


class MIDI_READER_API CN64MidiToolReader
{
public:
	CN64MidiToolReader(void);
	~CN64MidiToolReader(void);

	static int HexToInt(char inChar);
	static unsigned long StringHexToLong(CString inString);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static unsigned long Flip32Bit(unsigned long inLong);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned short Flip16Bit(unsigned short ShortValue);
	static void InitializeSpecificGames(CString iniPath, int& countGames, MidiGameConfig*& gameConfig);
	static void ProcessMidis(MidiGameConfig* gameConfig, int gameNumber, std::vector<CString>& addMidiStrings, int& numberMidiStrings, int& numberInstruments, bool& compressed, unsigned char* buffer, int bufferSize, unsigned long& startSpot, unsigned long& endSpot, bool calculateInstrumentCount, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo);
	static void ParseUncompressedType(unsigned char* gamebuffer, int gamebufferSize, unsigned long start, unsigned long endSpot, std::vector<CString>& addMidiStrings, int& numberMidiStrings, int& numberInstruments, bool& compressed, unsigned char* buffer, bool calculateInstrumentCount, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo);
	static unsigned long ReadAddiuAddress(unsigned char* GEROM, unsigned long upperLocation, unsigned long lowerLocation);
public:
	static CMidiParse midiParse;
	static GECompression compress;
	static int GetSizeFile(CString filename);
};
