#pragma once

#include "SoundToolExports.h"

#include "StdAfx.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "yay0.h"
#include "rnc_deco.h"
#include "gecompression.h"
#include "BlitzDecoder.h"
#include "SupermanDecoder.h"
#include "n643docompression.h"
#include "MKMythologiesDecode.h"
#include "MidwayLZ.h"
#include "BlastCorpsLZBDecoder.h"
#include "H20Decoder.h"
#include "TetrisphereDecoder.h"
#include "SnoDecoder.h"
#include "flzh_rn.h"
#include "SydneyDecoder.h"
#include "NinDec.h"
#include "SharedFunctions.h"
#include "MidwayDecoder.h"
#include "NintendoEncoder.h"
#include "flzh_rn.h"
#include "ViewpointDecoder.h"

#define STANDARDFORMAT 0
#define STARFOX64FORMAT 1
#define N64PTRWAVETABLETABLEV1 2
#define N64PTRWAVETABLETABLEV2 3
#define N64PTRWAVETABLETABLEV2YAY0 4
#define ZELDAFORMAT 5
#define CONKERFORMAT 6
#define TUROKFORMAT 7
#define STANDARDRNCCOMPRESSED 8
#define SUPERMARIO64FORMAT 9
#define STANDARDFORMATRAWALLOWED 10
#define ZLIBSN64 11
#define SN64 12
#define N64PTRWAVETABLETABLEV2ZLIB 13
#define N64PTRWAVETABLETABLEV2BLITZ 14
#define BNKB 15
#define TITUS 16
#define ARMYMENFORMAT 18
#define MKMYTHOLOGIES 19
#define N64DD 20
#define STANDARDRNXCOMPRESSED 21
#define RNCCOMPRESSEDN64PTR 22
#define BANJOTOOIEAUDIO 23
#define VOX 24
#define MUSYX 25
#define MUSYXLZ 26
#define MUSYXZLIB 27
#define ZLIBSTANDARD 28
#define BLASTCORPSZLBSTANDARD 29
#define MUSYXSMALLZLIB 30
#define MEGAMAN64PTRV2 31
#define H20RAW816 32
#define TETRISPHERERAW816 33
#define EAD1080 34
#define SNOW 35
#define TAZHUFFMAN 36
#define SYDNEY 37
#define NINDEC 38
#define MARIOPARTY2E 39
#define PAPERMARIO 40
#define DUCKDODGERS 41
#define B0 42
#define AVL_0PTR 43
#define KONAMICTL 44
#define KONAMI8CTL 45
#define MORT 46
#define WAV 47
#define KOBE2 48
#define N64PTRWAVETABLETABLEQUAKE2 49
#define N64PTRWAVETABLETABLESNOWBOARDKIDS 50
#define N64PTRWAVETABLEVIEWPOINT 51

#pragma once

enum    {AL_ADPCM_WAVE = 0, AL_RAW16_WAVE = 1, AL_VOX_WAVE=2, AL_MUSYX_WAVE=3, AL_SIGNED_RAW8=4, AL_SIGNED_RAW16=5, AL_MORT_WAVE=6, AL_WAV=7, AL_KOBE2_WAVE};

enum    {PRIMARY = 0, PREVIOUS = 1, SECONDARY = 2};

struct ALADPCMBook {
        unsigned long             order;
        unsigned long             npredictors;
        signed short*			predictors;
};

struct ALADPCMloop {
        unsigned long             start;
        unsigned long             end;
        unsigned long             count;
        short     state[16];

		unsigned long unknown1;
};

struct ALRawLoop{
        unsigned long             start;
        unsigned long             end;
        unsigned long             count;
};


struct ALADPCMWaveInfo
{
    ALADPCMloop     *loop;
    ALADPCMBook     *book;

	ALADPCMWaveInfo()
	{
		loop = NULL;
		book = NULL;
	}
};

struct ALRAWWaveInfo
{
        ALRawLoop       *loop;

		ALRAWWaveInfo()
		{
			loop = NULL;
		}
};

struct ALWave
{
	unsigned long base;
	unsigned long len;
	byte type;
	byte flags;
    ALADPCMWaveInfo* adpcmWave;
    ALRAWWaveInfo*  rawWave;
	unsigned char* wavData;
	int sampleRateNotInDefaultNintendoSpec;

	unsigned long unknown1;
	unsigned long unknown2;
	unsigned long unknown3;
	unsigned long unknown4;

	unsigned char wavFlags;

	ALWave()
	{
		type = AL_ADPCM_WAVE;
		adpcmWave = NULL;
		rawWave = NULL;
		wavData = NULL;
		flags = 0;
		wavFlags = 0;
	}
};


struct ALEnv
{
	unsigned long	attackTime;
	unsigned long	decayTime;
	unsigned long	releaseTime;
	unsigned char attackVolume;
	unsigned char decayVolume;
	unsigned short zeroPad;

	ALEnv()
	{
		zeroPad = 0x0000;
	}
};

struct ALKey
{
	char velocitymin;
	char velocitymax;
	char keymin;
	char keymax;
	char keybase;
	char detune;

	ALKey()
	{
		velocitymin = 0x00;
		velocitymax = 0x7F;
		keymin = 0x00;
		keymax = 0x7F;
		keybase = 0x3C;
		detune = 0x00;
	}
};

struct ALSound
{
	ALEnv	env;
	ALKey	key;
	ALWave	wav;
	char	samplePan;
	char	sampleVolume;
	short	flags;

	bool hasWaveSecondary;
	ALWave	wavSecondary;

	bool hasWavePrevious;
	ALWave	wavPrevious;

	unsigned long unknown1;
	unsigned long floatKeyBasePrev;
	unsigned long floatKeyBase;
	unsigned long floatKeyBaseSec;

	unsigned short adsrEAD[0x8];

	std::vector<unsigned short> conkerShorts;

	ALSound()
	{
		flags = 0x00;
		hasWaveSecondary = false;
		hasWavePrevious = false;
		samplePan = 0x40;
		sampleVolume = 0x7F;
		unknown1 = 0;
		floatKeyBasePrev = 0;
		floatKeyBase = 0;
		floatKeyBaseSec = 0;

		conkerShorts.clear();

		for (int x = 0; x < 8; x++)
			adsrEAD[x] = 0x7FBC;
	}
};


struct EADPercussion
{
	unsigned char unknown1A;
	unsigned char pan;
	unsigned short unknown1C;
	ALWave	wav;
	unsigned long keyBase;

	unsigned short adsrEAD[0x8];

	EADPercussion()
	{
		unknown1A = 0;
		pan = 0;
		unknown1C = 0;
		keyBase = 0x3C;

		wav.base = 0;
		wav.len = 0;
		wav.type = 0;
		wav.flags = 0;

		keyBase = 0x3C;
		pan = 0;
	}
};

struct ALInst
{
	char	volume;
	char	pan;
	char	priority;
	char	flags;	
	char	tremType;
	char	tremRate;
	char	tremDepth;
	char	tremDelay;
	char	vibType;
	char	vibRate;
	char	vibDepth;
	char	vibDelay;
	short	bendRange;	
	short	soundCount;
	
	ALSound	**sounds;

	// extras
	unsigned short samplerate;
};

//AABBCCDDEE AA=course tune correction. BB=fine tune correction. CC=Attack (01=really slow to reach max attack. 7F=max). DD=decay (01=hold decay for awhile. 7F=none). EE=release time (01=really long. 7F=none.).
struct KonamiADSR
{
	signed char coarseTune;
	signed char fineTune;
	unsigned char attackTime;
	unsigned char decayTime;
	unsigned char releaseTime;
	unsigned char unknownOne;
	unsigned char unknownTwo;
};

struct KonamiADSRDrum : KonamiADSR
{
	unsigned char instrumentLookup;
};

struct T1BankEntry
{
	unsigned char bankNumber;
	unsigned char instrumentNumber;
	unsigned char soundNumber;
	unsigned char flags;
	unsigned long frequency;
};

struct T1Bank
{
	std::vector<T1BankEntry> t1Entries;
};

struct ALBank
{
	unsigned short	count;
	unsigned short	flags;
	unsigned short	pad;
	unsigned short	samplerate;
	ALInst*	percussion;
	ALInst**			inst;

	std::vector<KonamiADSR> konamiADSR;
	std::vector<KonamiADSRDrum> konamiDrumsADSR;

	// extras
	int soundBankFormat;
	CString bankName;

	unsigned long countEADPercussion;
	unsigned long unknown2;
	unsigned long unknown3;
	EADPercussion* eadPercussion;

	// Ocarina third bank
	unsigned long countSfx;
	ALWave** alSfx;

	int subBank;

	ALBank()
	{
		count = 0;
		flags = 0;
		pad = 0;
		samplerate = 22050;
		percussion = NULL;
		inst = NULL;
		countEADPercussion = 0;
		unknown2 = 0;
		unknown3 = 0;
		eadPercussion = NULL;
		countSfx = 0;
		alSfx = NULL;
		subBank = 0;
	}
};

struct t1Result
{
	int bankStart;
	int bankEnd;
	T1Bank* bank;

	int t1Start;
	int t1End;
};

struct ctlTblResult
{
	unsigned long ctlOffset;
	unsigned long ctlSize;
	unsigned long tblOffset;
	unsigned long tblSize;
	ALBank* bank;
	bool halfSamplingRate;
	int samplingRate;
	bool overrideSamplingRate;


	ctlTblResult()
	{
		halfSamplingRate = false;

		overrideSamplingRate = false;
		samplingRate = 22050;
	}
};

struct WaveInformation
{
	unsigned long	version;
	unsigned long	adpcmsize;
	unsigned long	samplesize;
	unsigned long	loop_startsamplepoint;
	unsigned long	loop_endsamplepoint;
	unsigned short	loop_count;
	unsigned char	book_bytes;
	unsigned char	samplingbaseMIDIkey;
	float	samplingrate;
	float	tuning_C4_32K;
};

class SOUNDTOOL_API CN64AIFCAudio
{
public:
	CN64AIFCAudio(void);
	~CN64AIFCAudio(void);

	static bool InjectCtlTblIntoROMArrayInPlace(unsigned char* ROM, unsigned char* ctl, int ctlSize, unsigned char* tbl, int tblSize, unsigned long ctlOffset, unsigned long tblOffset, unsigned long maxCtl, unsigned long maxTbl, unsigned char* ctl2, int ctlSize2, int ctlOffset2, int maxCtl2);
	static void DisposeT1Bank(T1Bank*& t1Bank);
	static void DisposeALBank(ALBank*& alBank);
	static void DisposeALInst(ALInst*& alInst);
	static bool WriteT1Bank(T1Bank* t1Bank, unsigned char* ROM, unsigned long t1Start, unsigned long t1End);
	static T1Bank* ReadT1Bank(unsigned char* ROM, unsigned long t1Start, unsigned long t1End);
	static ALBank* ReadAudio(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber);
	static ALBank* ReadAudioKonami(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset);
	static ALBank* ReadAudioKonami8(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset);
	static ALBank* ReadAudioMarioParty2E(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask);
	static ALBank* ReadAudioMusyx(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd);
	static ALBank* ReadAudioMusyxSmallerTable(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd);
	static ALBank* ReadAudioZLibMusyx78DA(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd);
	static ALBank* ReadAudioZLibMusyxSmall(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd);
	static ALBank* ReadAudioBlastCorps(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioLzMusyx(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd);
	static ALBank* ReadAudioZLibMusyx(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, int lastSoundEnd, int game);
	static ALBank* ReadAudioBanjoTooie(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber);
	static ALBank* ReadAudioConker(unsigned char* ctl, int ctlSize, int ctlOffset, int ctlOffsetPartTwo, unsigned char* tbl);
	static ALBank* ReadAudio64dd(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioVox(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int samplingRate);
	static ALBank* ReadAudioWav(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioMORT(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioKobe2(unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableV2ZLIB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableV2MultiPartERZ(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioRawAllowed(unsigned char* ROM, unsigned char* ctl, int ctlSize, int ctlOffset, unsigned char* tbl, int ctlFlaggedOffset, unsigned long mask, int bankNumber);
	static ALBank* ReadAudioMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long& tblSize, int bankNumber);
	static ALBank* ReadAudioStarFox(unsigned char* ctl, int ctlSize, int ctlOffset, int tblOffset, int instrumentCount, unsigned long endSpot, unsigned char* rom, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, int ctlIndex, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd);
	static ALBank* ReadAudioZelda(std::vector<ctlTblResult>& results, unsigned char* ctl, int ctlSize, int ctlOffset, int tblOffset, int instrumentCount, unsigned long endSpot, unsigned char* rom, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, int ctlIndex, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd, bool isZLib);
	static ALBank* ReadAudioRawTest(unsigned char* rawData, int size);
	static ALBank* ReadAudioPaperMario(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioB0(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioDuckDodgers(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioSydney(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int fileSizeCompressed);
	static ALBank* ReadAudioNinDec(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int fileSizeCompressed);
	static ALBank* ReadAudioTazN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioMegamanN64PtrWavetableV2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioArmyMenCompressed(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, bool littleEndianCompressedHeader, bool sarge2style);
	static ALBank* ReadAudioBnkB(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableV2YAY0(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableQuake2(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableSnowboardKids(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64PtrWavetableV2Viewpoints(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int fileSizeDecompressed);
	static ALBank* ReadAudioN64Titus(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64MKMythologies(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long uncompressedSize);
	static ALBank* ReadAudioN64PtrWavetableBlitz(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioN64SN64(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask);
	static ALBank* ReadAudioN64SN64Zlib(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments, unsigned long mask);
	static ALBank* ReadRNCAudio(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber);
	static ALBank* ReadAudioSno(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long expectedSize);
	static ALBank* ReadAudioRNCN64Ptr(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioAVL_0Ptr(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioRNCN64PtrOffset(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, unsigned long offset);
	static ALBank* ReadAudioH20Raw816(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments);
	static ALBank* ReadAudioTetrisphereRaw816(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int numberInstruments);
	static ALBank* ReadRNXAudio(unsigned char* ROM, unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankNumber);
	static ALBank* ReadAudioN64PtrWavetableV1(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl);
	static ALBank* ReadAudioTurok(unsigned char* ctl, unsigned long& ctlSize, int ctlOffset, unsigned char* tbl, int bankTrueOffset, unsigned long mask, unsigned char* ROM, int bankNumber);
	static void UpdateAudioOffsets(ALBank* alBank);
	static void UpdateAudioOffsets(std::vector<ALBank*> alBanks);
	static void WriteAudio(std::vector<ALBank*>, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioConker(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize, int ctlOffsetPartTwo, unsigned char*& ctl2, int& ctlSize2);
	static void WriteKonamiADSR(unsigned char* ROM, ALBank*& alBank, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset);
	static void WriteKonami8ADSR(unsigned char* ROM, ALBank*& alBank, unsigned long instrumentOffset, unsigned long endInstrumentOffset, unsigned long startDrumOffset, unsigned long endDrumOffset);
	static bool CompareBooks(ALWave* wavIn, ALWave* wavOut);
	static bool CompareLoops(ALWave* wavIn, ALWave* wavOut);
	static bool CompareWavs(ALWave* wavIn, ALWave* wavOut);
	static bool CompareTbls(ALWave* wavIn, ALWave* wavOut);
	static bool CompareBytes(unsigned char* in, int inLength, unsigned char* out, int outLength);
	static void CheckAddWriteZeldaADSRData(unsigned short* adsrEAD, int instrumentIndex, std::vector<unsigned long>& offsetADSR, std::vector<unsigned short*>& adsrData, std::vector<int>& adsrIndex, unsigned char* temporaryCtlBuffer, int& dataOffset);
	static void CheckAddWriteZeldaWaveData(ALWave* waveData, int instrumentIndex, std::vector<ALADPCMBook*>& predictorData, std::vector<unsigned long>& offsetPredictorData, std::vector<ALADPCMloop*>& loopData, std::vector<unsigned long>& offsetLoopData, std::vector<int>& waveDataIndex, std::vector<int>& predictorIndex, std::vector<int>& loopIndex, unsigned char* temporaryCtlBuffer, unsigned char* temporaryTblBuffer, unsigned long& outputTblCounter, std::vector<unsigned char>& waveFlag, std::vector<unsigned long>& waveDataLength, std::vector<unsigned long>& offsetWaveData, int& dataOffset, std::vector<unsigned long>& offsetSoundData, std::vector<int>& soundDataIndex, std::vector<int>& soundDataOverallWaveIndex, std::vector<int>& soundDataOverallPredictorIndex, std::vector<int>& soundDataOverallLoopIndex, std::vector<unsigned char>& soundDataOverallWaveFlag, int tblIndex);
	static void WriteAudioStarFox(unsigned char* ROM, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, std::vector<ctlTblResult>& results, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd);
	static void WriteAudioZelda(unsigned char* ROM, int romSize, unsigned long offsetZeldaCtlTable, unsigned long offsetZeldaTblTable, unsigned long startZeldaCtlData, unsigned long startZeldaTblData, std::vector<ctlTblResult>& results, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize, bool isCompressedZeldaCtlTblTables, unsigned long compressedZeldaCtlTblTableStart, unsigned long compressedZeldaCtlTblTableEnd);
	static void WriteAudioSuperMario(std::vector<ctlTblResult>& results, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioToFile(ALBank* alBank, CString outFileNameCtl, CString outFileNameTbl);
	static void WriteAudioToFile(std::vector<ALBank*> alBanks, CString outFileNameCtl, CString outFileNameTbl);
	static void WriteAudioN64PtrWavetableV2(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioN64PtrWavetableV2Blitz(CString mainFolder, ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioN64PtrWavetableV2Yay0(CString mainFolder, ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static void WriteAudioN64PtrWavetableV1(ALBank*& alBank, unsigned char*& ctl, int& ctlSize, unsigned char*& tbl, int& tblSize);
	static bool ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate, bool& hasLoopData, unsigned char& keyBase, unsigned long& loopStart, unsigned long& loopEnd, unsigned long& loopCount);
	static bool ReplaceSoundWithWavData(ALBank*& alBank, int instrument, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, byte primSel, bool decode8Only, bool samePred);
	static bool ReplacePercussionWithWavData(ALBank*& alBank, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, bool samePred);
	static bool ReplaceEADPercussionWithWavData(ALBank*& alBank, int percussion, CString rawWavFileName, unsigned long& samplingRate, bool newType, bool decode8Only, bool samePred);
	static bool ReplaceSfxWithWavData(ALBank*& alBank, int sound, CString rawWavFileName, unsigned long& samplingRate, bool newType, bool decode8Only, bool samePred);
	static CString CompareALBanks(ALBank* alBank1, ALBank* alBank2);
	static CString CompareALInstrument(ALInst* alInst1, ALInst* alInst2);
	static CString CompareALSound(ALSound* alSound1, ALSound* alSound2);
	static CString CompareALEnv(ALEnv* alEnv1, ALEnv* alEnv2);
	static CString CompareALKey(ALKey* alKey1, ALKey* alKey2);
	static CString CompareALTbl(ALWave* alWave1, ALWave* alWave2, bool checkBase = false);
	static CString CompareALWav(ALWave* alWave1, ALWave* alWave2);
	static CString CompareALLoop(ALWave* alWave1, ALWave* alWave2);
	static CString CompareALBook(ALWave* alWave1, ALWave* alWave2);
	static bool ExtractRawSound(CString mainFolder, ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, byte primSel, bool halfSamplingRate);
	static bool ExtractLoopSound(CString mainFolder, ALBank* alBank, int instrument, int sound, CString outputFile, unsigned long samplingRate, byte primSel, bool halfSamplingRate);
	static bool ExtractPercussion(CString mainFolder, ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate);
	static bool ExtractLoopPercussion(CString mainFolder, ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate);
	static bool ExtractEADPercussion(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate);
	static bool ExtractLoopEADPercussion(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate);
	static bool ExtractSfx(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate);
	static bool ExtractLoopSfx(ALBank* alBank, int sound, CString outputFile, unsigned long samplingRate, bool halfSamplingRate);
	static unsigned char ConvertEADGameValueToKeyBase(float eadKeyvalue);
	static bool ExtractRawPCMData(CString mainFolder, ALBank* alBank, int instrument, int sound, CString outputFile, byte primSel);
	static bool ExtractPercussionRawPCMData(CString mainFolder, ALBank* alBank, int sound, CString outputFile);
	static bool ExtractEADPercussionRawPCMData(CString mainFolder, ALBank* alBank, int sound, CString outputFile);
	static bool ExtractSfxRawPCMData(CString mainFolder, ALBank* alBank, int sound, CString outputFile);
	static bool AddSound(ALBank*& alBank, int instrument, CString rawWavFileName, unsigned long& samplingRate, bool type);
	static bool AddSound(ALBank*& alBank, int instrument);
	static bool AddSfx(ALBank*& alBank, CString rawWavFileName, unsigned long& samplingRate, bool type);
	static bool AddSfx(ALBank*& alBank);
	static bool AddPercussion(ALBank*& alBank, CString rawWavFileName, unsigned long& samplingRate, bool type);
	static bool AddPercussion(ALBank*& alBank);
	static bool AddEADPercussion(ALBank*& alBank, CString rawWavFileName, unsigned long& samplingRate, bool type);
	static bool AddEADPercussion(ALBank*& alBank);
	static bool AddInstrument(ALBank*& alBank);
	static bool DeleteInstrument(ALBank*& alBank, int instrSel);
	static void DeleteSound(ALBank*& alBank, int instrument, int sound);
	static void DeleteSfx(ALBank*& alBank, int sound);
	static void DeletePercussion(ALBank*& alBank, int sound);
	static void DeleteEADPercussion(ALBank*& alBank, int sound);
	static void MoveUpSound(ALBank*& alBank, int instrument, int sound);
	static void MoveDownSound(ALBank*& alBank, int instrument, int sound);
	static void MoveUpPercussion(ALBank*& alBank, int sound);
	static void MoveDownPercussion(ALBank*& alBank, int sound);
	static void ExportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static void ExportPercussionPredictors(ALBank*& alBank, int sound, CString fileName);
	static void ExportEADPercussionPredictors(ALBank*& alBank, int percussion, CString fileName);
	static void ExportSfxPredictors(ALBank*& alBank, int sfx, CString fileName);
	static void ImportPredictors(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static void ImportPercussionPredictors(ALBank*& alBank, int sound, CString fileName);
	static void ImportEADPercussionPredictors(ALBank*& alBank, int percussion, CString fileName);
	static void ImportSfxPredictors(ALBank*& alBank, int sfx, CString fileName);
	static void ExportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static void ImportRawData(ALBank*& alBank, int instrument, int sound, CString fileName, byte primSel);
	static void ExportPercussionRawData(ALBank*& alBank, int sound, CString fileName);
	static void ImportPercussionRawData(ALBank*& alBank, int sound, CString fileName);
	static void ExportEADRawPercussionData(ALBank*& alBank, int sound, CString fileName);
	static void ImportEADRawPercussionData(ALBank*& alBank, int sound, CString fileName);
	static void ExportRawSfxData(ALBank*& alBank, int sound, CString fileName);
	static void ImportRawSfxData(ALBank*& alBank, int sound, CString fileName);
	static int ReadCtlTblLocations(unsigned char* ROM, int romSize, std::vector<ctlTblResult>& results);

	static BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);

	static unsigned long decode( unsigned char *in, signed short *out, unsigned long len, ALADPCMBook *book, bool decode8Only );
	static unsigned long decodemusyxadpcm( unsigned char *in, signed short *out, unsigned long len, ALADPCMBook *book );
	static float encode(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book);
	static float encode_half(signed short* inPCMSamples, int numberSamplesIn, unsigned char* outVADPCM, unsigned long& lenOut, ALADPCMBook *book);
	static int GetSizeFile(CString filename);
	static float keyTable[0x100];
	static unsigned long ReadAddiuAddress(unsigned char* GEROM, unsigned long upperLocation, unsigned long lowerLocation);
private:
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static unsigned long Flip32Bit(unsigned long inLong);
	static unsigned short Flip16Bit(unsigned short ShortValue);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static float CharArrayToFloat(unsigned char* currentSpot);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);

	static void decode_8( unsigned char *in, signed short *out , int index, signed short * pred1, signed short lastsmp[8]);
	static void decode_8_half( unsigned char *in, signed short *out , int index, signed short * pred1, signed short lastsmp[8]);
	static int determineBestEncodeIndexAndPredictor_half(signed short* predictors, int numPredictors, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex, int& predictorIndex);
	static int determineBestEncodeIndexAndPredictor(signed short* predictors, int numPredictors, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex, int& predictorIndex);
	static int determineBestEncodeIndex(signed short* pred1, signed short* pred2, signed short* lastSampleSet, signed short* inPCMSamples, float& bestFitIndex);
	static signed short* determineBestPredictors(ALBank* alBank, unsigned long& npredictors, unsigned long& norder, signed short* inPCMSamples, int numberSamplesIn);

	static void selectionSortAscending(float *array, signed short** arrayCopy, int length);
	static void InitializeKeyTable();

	static bool WriteWav(CString wavFilename, float samplingRate, std::vector<unsigned short> pcmSamples);
	static bool WriteWavStereo(CString wavFilename, float samplingRate, std::vector<unsigned short> pcmSamples);
};

