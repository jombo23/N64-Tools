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

	HRESULT SetupMidiSoundBank(std::vector<ALBank*> alBanks, float timeMultiplier, bool halfSamplingRate, bool overrideSamplingRate, int samplingRate, std::vector<int> skipInstruments, bool combineBanks, CString gameMidiType, bool useT1Bank, int soundbankNumber, std::vector<t1Result> t1Results);
	HRESULT GetNextEvent(DMUS_NOTIFICATION_PMSG& pPmsg);

	CString mainFolder;
	bool StartedSegment;

	float EADGameReleaseTimeSeconds[0x100];
};