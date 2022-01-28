#pragma once

#include "MidiExports.h"
#include <vector>
#include <map>

class GECompression;

#define MAXCHANNELS 1024

#define EADMUSICSTYLEZELDA 0
#define EADMUSICSTYLEMARIO 1
#define EADMUSICSTYLESTARFOX 2

struct ExtraGameMidiInfo
{
	bool zlbCompressed;
	bool naganoCompressed;

	unsigned long trackOffset;
	unsigned long compressedFileOffset;
	unsigned long compressedFileEndOffset;

	unsigned long trackIncrement;

	unsigned long trackAddressOffset;
	unsigned long trackAddressEndsOffset;
	unsigned long checksum1;
	unsigned long checksum2;
	unsigned long tracksEnd;

	ExtraGameMidiInfo()
	{
		zlbCompressed = false;
		naganoCompressed = false;

		trackOffset = 0x00000000;
		compressedFileOffset = 0x00000000;
		compressedFileEndOffset = 0x00000000;
		trackIncrement = 0x00000000;

		trackAddressOffset = 0x00000000;
		trackAddressEndsOffset = 0x00000000;

		checksum1 = 0x00000000;
		checksum2 = 0x00000000;

		tracksEnd = 0x00000000;
	}
};

struct TimeAndValue
{
	unsigned long absoluteTime;
	unsigned long value;

	TimeAndValue(unsigned long absoluteTime, unsigned long value)
	{
		this->absoluteTime = absoluteTime;
		this->value = value;
	}
};

struct timeAndValueSortByTime
{
    inline bool operator() (const TimeAndValue& struct1, const TimeAndValue& struct2)
    {
        return (struct1.absoluteTime < struct2.absoluteTime);
    }
};

#define INSTRUMENTTYPE 3
#define EFFECTTYPE 2
#define VOLUMETYPE 0
#define PANTYPE 1
#define PITCHBENDTYPE 4
#define VIBRATOTYPE 5

struct TimeAndValueAndType : public TimeAndValue
{
	int type;
	TimeAndValueAndType(unsigned long absoluteTime, unsigned long value, int type) : TimeAndValue(absoluteTime, value)
	{
		this->type = type;
	}
};

struct timeAndValueAndTypeSortByTime
{
    inline bool operator() (const TimeAndValueAndType& struct1, const TimeAndValueAndType& struct2)
    {
		if (struct1.absoluteTime == struct2.absoluteTime)
			return (struct1.type < struct2.type);
		else
			return (struct1.absoluteTime < struct2.absoluteTime);
    }
};

struct SngTimeValue
{
	unsigned long startAbsoluteTime;
	unsigned long endAbsoluteTime;

	unsigned char value;
};

struct SngNoteInfo
{
	unsigned long startAbsoluteTime;
	unsigned long endAbsoluteTime;
	int noteNumber;
	unsigned char velocity;

	unsigned long instrument;

	int pan;
	int volume;
	unsigned char pitchBend;
	unsigned char vibrato;

	unsigned long tempo;

	unsigned char effect;

	int originalTrack;
	int originalNoteUniqueId;

	bool ignoreNoteOn;

	int segmentNumber;

	SngNoteInfo()
	{
		volume = 0x7F;
		pitchBend = 0x40;
		pan = 0x40;

		originalTrack = 0;
		originalNoteUniqueId = 0;

		effect = 0;

		ignoreNoteOn = false;

		segmentNumber = 0x00;

		vibrato = 0x00;
	}
};

struct SngSegmentTrackInfoPaperMario
{
	std::vector<SngNoteInfo> sngNoteList;
};

struct SngSubSegmentInfoPaperMario
{
	int startSegmentTime;
	int endSegmentTime;
	unsigned short trackFlags;
};

struct SngSegmentInfoPaperMario
{
	std::vector<TimeAndValue> tempoPositions;
	SngSegmentTrackInfoPaperMario sngSegmentTracks[0x10];
	std::map<int, SngSubSegmentInfoPaperMario> sngSubSegmentInfo;
};

struct SngDrumPaperMario
{
	unsigned char flags;
	unsigned char instrument;
	unsigned char unknown2;
	unsigned char unknown3;
	unsigned char volume;
	unsigned char pan;
	unsigned char effect;
	unsigned char unknown7;
	unsigned char unknown8;
	unsigned char unknown9;
	unsigned char unknownA;
	unsigned char unknownB;
};

struct SngInstrumentPaperMario
{
	unsigned char flags;
	unsigned char instrument;
	unsigned char volume;
	unsigned char pan;
	unsigned char effect;
	unsigned char unknown5;
	unsigned char unknown6;
	unsigned char unknown7;
};

struct SngNoteInfoMidiImport : SngNoteInfo
{
	unsigned char originalController;
	SngNoteInfoMidiImport()
	{
		originalController = 0;	
		endAbsoluteTime = 0xFFFFFFFF;
	}
};

struct sngSortByStartTime
{
    inline bool operator() (const SngNoteInfo& struct1, const SngNoteInfo& struct2)
    {
        return (struct1.startAbsoluteTime < struct2.startAbsoluteTime);
    }
};

struct sngSortByEndTime
{
    inline bool operator() (const SngNoteInfo& struct1, const SngNoteInfo& struct2)
    {
        return (struct1.endAbsoluteTime < struct2.endAbsoluteTime);
    }
};

struct TrackEvent // referenced in order, but first = 2710, doors refer to 27XX + preset to it implicitly, 0x44 per
{
	bool obsoleteEvent;
	unsigned long deltaTime;
	unsigned long durationTime;
	unsigned long absoluteTime;
	byte type;
	byte* contents;  // remember delete all track events mem later
	int contentSize;

	TrackEvent()
	{
		type = 0x00;

		contents = NULL;
		contentSize = 0;

		deltaTime = 0;
		absoluteTime = 0;
		obsoleteEvent = false;
		durationTime = 0;
	}
};


class MIDI_API CMidiParse
{
public:
	enum SngStyle { Normal, Old, OldBfx, Bfx, PtrBfx, OldDD, HexenSng };

	CMidiParse(void);
	~CMidiParse(void);

	void GEMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool extendTracksToHighest, bool usePitchBendSensitity, int pitchBendSensitity);
	void BTMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool extendTracksToHighest, bool usePitchBendSensitity, int pitchBendSensitity);
	void MIDxMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments);
	int GetSngVariableLength(unsigned char* buffer, unsigned long offset);
	int WriteSngVariableLength(unsigned char* buffer, int& offset, int value);
	void WriteSngToMidiTrack(FILE* outFile, FILE* outDebug, std::vector<TrackEvent> trackEvents);
	void ParseSngTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, unsigned short* instrumentLookup, unsigned long adsrPointer, unsigned long drumPointer, std::vector<SngTimeValue> volumeByAbsoluteTime, std::vector<SngTimeValue> pitchBendByAbsoluteTime, SngStyle sngStyle, int& noteUniqueId, int totalInstruments);
	void SngToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, unsigned long extra);
	void SngToDebugTextFile(CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, unsigned long extra);
	void SngToDebugTextFile(CString gameName, unsigned long address, CString midiFile, CString textFileOut, unsigned long extra);
	void SngTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, unsigned long instrumentPointer, unsigned long adsrPointer, unsigned long drumPointer, SngStyle sngStyle, int totalInstruments);
	bool MidiToGEFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool AddLoopGEFormat(byte* inputMID, CString output, int inputSize, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool MidiToBTFormatStageOne(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters, unsigned short& numTracks);
	bool MidiToBTFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters);
	bool MidiToPaperMarioSngList(CString input, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfoMidiImport> channels[MAXCHANNELS], int& numChannels, std::vector<int>& instruments, int& lowestAbsoluteTime, int& highestAbsoluteTime, bool loop, unsigned long loopPoint, unsigned short & division);
	bool MidiToSngList(CString input, std::vector<SngNoteInfoMidiImport>& sngNoteList, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfoMidiImport> channels[MAXCHANNELS], int& numChannels, std::vector<int>& instruments, int& lowestAbsoluteTime, int& highestAbsoluteTime, bool loop, unsigned long& loopPoint);
	bool MidiToSng(CString input, CString output, bool loop, unsigned long loopPoint, SngStyle sngStyle, unsigned char masterTrackEffect);
	bool MidiToKonami(CString input, CString output, bool loop, unsigned long loopPoint, int& numberTracks, unsigned char separationAmount, unsigned char echoLength);
	void GEMidiToDebugTextFile(CString midiFile, CString textFileOut, bool extendTracksToHighest);
	void GEMidiToDebugTextFile(byte* inputMID, int inputSize, CString textFileOut, bool extendTracksToHighest);
	void MidiToDebugTextFile(CString midiFile, CString textFileOut);
	void ExportToBin(CString gameName, unsigned char* buffer, unsigned long address, unsigned long size, CString fileName, bool& compressed, unsigned long extra);
	void ExportToMidi(CString gameName, unsigned char* gamebuffer, int gamebufferSize, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division, bool& compressed, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool calculateInstrumentCountOnly, bool separateByInstrument, bool generateDebugTextFile, unsigned long extra, unsigned long extra2, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, bool isPreview);
	void ExportToMidi(CString gameName, unsigned char* gamebuffer, int gamebufferSize, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division, bool& compressed, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool calculateInstrumentCountOnly, bool separateByInstrument, bool generateDebugTextFile, unsigned long extra, unsigned long extra2, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, bool usePitchBendSensitity, int pitchBendSensitity, bool isPreview);
	byte* Decompress(unsigned char* Buffer, unsigned long size, int& fileSize, int& compressedSize);
	bool DecompressToFile(unsigned char* Buffer, unsigned long size, CString outputFile);
	void GenerateTestPattern(int type, CString outputFile);

	int FindHighestKonamiLengthTrack(int trackNumber, unsigned char* buffer, unsigned long offset, unsigned long end);
	void ParseKonamiTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, int& noteUniqueId, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength);
	void KonamiToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void KonamiToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void KonamiToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void KonamiTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, ExtraGameMidiInfo extraGameMidiInfo, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength);

	static unsigned char sseqCommandSizes[0x24];
	int FindHighestSSEQLengthTrack(unsigned char* inputMID, int endSize, int numberTracks);
	void ParseSSEQTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* inputMID, unsigned long offset, unsigned long end, int& noteUniqueId, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength);
	void SSEQToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void SSEQToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void SSEQToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void SSEQTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, ExtraGameMidiInfo extraGameMidiInfo, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength);

	void WritePaperMarioDelay(unsigned long delay, unsigned char* outputBuffer, int& outputPosition);
	bool MidiToPaperMario(CString input[4], CString output, bool loop, unsigned long& loopPoint, unsigned long name);
	bool MidiToPaperMario(CString output, std::vector<SngDrumPaperMario> drums, std::vector<SngInstrumentPaperMario> instruments, SngSegmentInfoPaperMario sngSegments[4], unsigned long name, bool loop);
	bool MidiSngListToPaperMario(CString input, CString output, bool loop, unsigned long loopPoint, int& numberTracks);
	void TestPaperMarioToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, std::vector<SngInstrumentPaperMario>& usedInstruments, std::vector<SngDrumPaperMario>& usedPercussionSet, std::vector<int>& usedExtraInstruments, bool combineSegments);
	void PaperMarioToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, std::vector<SngInstrumentPaperMario>& usedInstruments, std::vector<SngDrumPaperMario>& usedPercussionSet, std::vector<int>& usedExtraInstruments, bool combineSegments);
	void ParsePaperMarioTrack(unsigned char* ROM, int romSize, int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, int& noteUniqueId, unsigned long drumDataPointer, int numberDrum, unsigned long instrumentDataPointer, int numberInst, unsigned short trackFlags, std::vector<int>& usedExtraInstruments, unsigned char& currentPan, unsigned long& currentInstrument, unsigned char& currentVolume, int& currentEffect, unsigned long& currentTempo, signed long& currentCoarseTune, bool& setReverb, bool& setVolume, bool& setPan, int& absoluteTimeStart, int subSegmentCounter);
	void PaperMarioToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool combineSegments, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void PaperMarioToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool combineSegments, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra);
	void PaperMarioTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned short trackFlags);

	// Major portions of assistance in figuring out format by Saureen and Frabuer
	int IncrementSpot(unsigned long& spot, int count, unsigned char* coverage);
	int ReadEADMusicTimeValue(unsigned char* inputMID, unsigned long& offset, unsigned char* coverage);
	int WriteEADMusicTimeValue(unsigned char* outputMID, int& offset, int value);
	void ParseEADMusicTrackLayer(int gameStyle, FILE* outFileDebug, unsigned char* inputMID, int end, int offset, int channel, int layer, unsigned long absoluteTime, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& trackOutputNotes, int& noteUniqueId, std::vector<unsigned long>& jumpStarts, std::vector<unsigned long>& jumpsTaken, bool attemptJumps, unsigned char* coverage, bool channelStarted, bool isPreview);
	void ParseEADMusicChannel(int gameStyle, FILE* outFileDebug, unsigned char* inputMID, int end, int offset, int channel, unsigned long absoluteTime, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, int& noteUniqueId, int currentInstrument[0x10], int currentPan[0x10], int currentVolume[0x10], int currentPitchBend[0x10], int currentEffect[0x10], std::vector<unsigned long>& jumpStarts, std::vector<unsigned long>& jumpsTaken, bool attemptJumps, unsigned char* coverage, bool channelStarted[0x10], int currentVibrato[0x10], bool isPreview);
	void ParseEADMusicSequence(int gameStyle, FILE* outFileDebug, unsigned char* inputMID, int offset ,int end, unsigned long& absoluteTime, std::vector<unsigned long>& jumpStarts, std::vector<unsigned long>& jumpsTaken, bool attemptJumps, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, int& noteUniqueId, int currentInstrument[0x10], int currentPan[0x10], int currentVolume[0x10], int currentPitchBend[0x10], int currentReverb[0x10], bool& hitConditional, unsigned char* coverage, bool channelStarted[0x10], int currentVibrato[0x10], bool isPreview);
	void EADMusicToMidi(CString gameName, int gameStyle, byte* inputMID, int address, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, unsigned long extra, bool writeDebug, CString debugFilename, int pitchBendRange, bool isPreview);
	bool MidiToEADMusicSngList(CString input, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfoMidiImport> channels[0x10], int& numChannels, int& lowestAbsoluteTime, int& highestAbsoluteTime, bool loop, unsigned long loopPoint, unsigned short & division);
	bool MidiToEADMusic(int gameStyle, CString input, CString output, bool loop, unsigned long loopPoint);

	int DecodeFactor5DeltaTimeRLE(unsigned char* input, unsigned long& offset);
	void Factor5ToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool isRogueStyle);
	void Factor5ToDebugTextFile(CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool isRogueStyle);
	void Factor5ToDebugTextFile(CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool isRogueStyle);

	void WriteSngList(std::vector<SngNoteInfo> sngNoteList, std::vector<TimeAndValue> tempoPositions, CString outFileName, bool separateByInstrument, unsigned short division, bool expandBendRange, int bendRange);
	
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
private:
	int HexToInt(char inChar);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned long Flip32Bit(unsigned long inLong);

	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);

	unsigned long GetVLBytes(byte* vlByteArray, int& offset, unsigned long& original, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats);
	void WriteVLBytes(FILE* outFile, unsigned long value, unsigned long length, bool includeFERepeats);
	byte ReadMidiByte(byte* vlByteArray, int& offset, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats);

	unsigned long StringHexToLong(CString inString);
	unsigned char StringToUnsignedChar(CString inString);
	unsigned short Flip16bit(unsigned short tempShort);

	int numberTracks;
	TrackEvent** trackEvents;
	int trackEventCount[0x20];

	GECompression* compress;
	CString mainFolder;

	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
	BOOL CMidiParse::hiddenExec (PTSTR pCmdLine, CString currentDirectory, HANDLE out);

	bool IsOverlap(float x1, float x2, float y1, float y2);
	int GetSizeFile(CString filename);
	void fprintfIfDebug(FILE* outFileDebug, char* format,...);
};
