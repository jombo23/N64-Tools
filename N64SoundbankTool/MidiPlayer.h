#pragma once

#include <dmusicc.h>
#include <dmusici.h>
#include <dmksctrl.h>
#include <vector>
#include "N64AIFCAudio.h"

/*struct INSTRUMENT_DOWNLOAD
{
	DMUS_DOWNLOADINFO dlInfo;
	ULONG			  ulOffsetTable[4];
	DMUS_INSTRUMENT   dmInstrument;
	
	DMUS_REGION		  dmRegion;
	DMUS_ARTICULATION dmArticulation;
	DMUS_ARTICPARAMS  dmArticParams;
};

struct INSTRUMENT_DOWNLOAD4
{
	DMUS_DOWNLOADINFO dlInfo;
	ULONG			  ulOffsetTable[13];
	DMUS_INSTRUMENT   dmInstrument;

	DMUS_REGION		  dmRegion;
	DMUS_ARTICULATION dmArticulation;
	DMUS_ARTICPARAMS  dmArticParams;

	DMUS_REGION		  dmRegion2;
	DMUS_ARTICULATION dmArticulation2;
	DMUS_ARTICPARAMS  dmArticParams2;

	DMUS_REGION		  dmRegion3;
	DMUS_ARTICULATION dmArticulation3;
	DMUS_ARTICPARAMS  dmArticParams3;

	DMUS_REGION		  dmRegion4;
	DMUS_ARTICULATION dmArticulation4;
	DMUS_ARTICPARAMS  dmArticParams4;
};*/

struct WAVE_DOWNLOAD
{
	DMUS_DOWNLOADINFO dlInfo;
	ULONG			  ulOffsetTable[2];
	DMUS_WAVE		  dmWave;
	DMUS_WAVEDATA	  dmWaveData;
};

struct MIDIINFO 
{
   CString description;
   unsigned long dwFlags;
   unsigned long dwClass;
   GUID guidPort;
   int dwMaxChannelGroups;
};


struct Factor5Page
{
	unsigned short objId;
	unsigned char priority;
	unsigned char maxVoices;
	unsigned char unk;
	unsigned char programNo;
	unsigned char pad;
};

struct Factor5SongInfo
{
	unsigned char programNo;
	unsigned char volume;
	unsigned char panning;
	unsigned char reverb;
	unsigned char chorus;
};

struct Factor5SongArray
{
	unsigned short songId;
	Factor5SongInfo songInfo[0x10];
};

struct Factor5ADSR
{
	unsigned short objectId;
	unsigned long attackTime;
	unsigned long decayTime;
	float sustainPercentage;
	unsigned long releaseTime;
};

struct Factor5Keymap
{
	unsigned short objectId;
	unsigned char transpose;
	unsigned char pan;
	unsigned char priorityOffset;
};

struct Factor5KeymapGroup
{
	unsigned short objectId;
	Factor5Keymap keys[128];
};

struct Factor5Layer
{
	unsigned short objectId;
	unsigned char keyLo;
	unsigned char keyHi;
	unsigned char transpose;
	unsigned char volume;
	unsigned char priorityOffset;
	unsigned char span;
	unsigned char pan;
};

struct Factor5LayerGroup
{
	int objectId;
	std::vector<Factor5Layer> factor5Layers;
};

struct Factor5SoundMacro
{
	unsigned char commandData[0x8];
};

struct Factor5SoundMacroList
{
	std::vector<Factor5SoundMacro> soundMacros;	
};

class CMidiPlayer
{
private:
	IDirectMusic8* mMusic8;
	IDirectMusicLoader8* mLoader;
	IDirectMusicPort8* mMusicPort;
	IDirectMusicPortDownload8* mPortDownload;
	IDirectMusicPerformance8* mPerformance;
	IDirectMusicSegment8* mSegment;
	IDirectMusicSegmentState8* mSegmentState8;
	IDirectMusicCollection8* mCollection;

	std::vector<IDirectMusicDownloadedInstrument*> mInstrumentsLoaded;
	std::vector<IDirectMusicDownload8*> mInstrumentGeneratedLoaded;
public:
    CMidiPlayer();
    ~CMidiPlayer();
    
	HRESULT Startup(CString mainFolder, HWND hwnd);

	HRESULT GetNextPort(unsigned long portIndex, MIDIINFO& midiPort);
	HRESULT ChoosePort(MIDIINFO& midiPort);
	HRESULT LoadMidi(LPCSTR midiName);
	HRESULT UnloadMidi();
	HRESULT UnloadAllLoadedInstruments();
	
	HRESULT Play(); 
	HRESULT SetRepeat(bool repeat);
	HRESULT SetLoopSpot(int startLoop, int endLoop);
	HRESULT Stop();

	HRESULT Pause();
	HRESULT Resume();

    HRESULT IsPlaying();

	HRESULT LoadDLS(LPTSTR dlsFilename);

	static TCENT TimeSecondsToCents(double timeSeconds);
	static PCENT PitchHzToCents(double frequencyHz);
	static PCENT PercentToUnits(double percent);

	void ParseSoundMacroList(std::vector<Factor5SoundMacro> soundMacroList, std::map<int, Factor5ADSR> poolTables, int& sampleId, unsigned long& attackTime, unsigned long& decayTime, float& sustainPercentage, unsigned long& releaseTime, unsigned char& macroPan);
	HRESULT SetupMidiSoundBankFactor5(int matchSongId, unsigned char* buffer, int romSize, unsigned long projOffset, unsigned long poolOffset, int projSize, int poolSize, ALBank* alBankInstr, float timeMultiplier, bool halfSamplingRate, bool overrideSamplingRate, int samplingRate, CString gameSoundType, bool absoluteOffsets);

	HRESULT SetupMidiSoundBank(std::vector<ALBank*> alBanks, float timeMultiplier, bool halfSamplingRate, bool overrideSamplingRate, int samplingRate, std::vector<int> skipInstruments, bool combineBanks, CString gameMidiType, bool useT1Bank, int soundbankNumber, std::vector<t1Result> t1Results);
	HRESULT GetNextEvent(DMUS_NOTIFICATION_PMSG& pPmsg);

	CString mainFolder;
	bool StartedSegment;

	float EADGameReleaseTimeSeconds[0x100];
	static unsigned short Flip16Bit(unsigned short ShortValue);
	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long Flip32Bit(unsigned long inLong);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
};