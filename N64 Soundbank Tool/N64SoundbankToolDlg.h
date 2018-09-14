#pragma once

//#include "vld.h"
#include "N64SoundToolReader.h"
#include "N64MidiToolReader.h"
#include "N64AIFCAudio.h"
#include "afxwin.h"
#include "MidiPlayer.h"

struct SoundfontBank
{
	int soundBankNumber;
	std::vector<int> midiNumbers;
};

struct GameConfig
{
	CString gameType;
	CString gameName;
	std::vector<SoundfontBank> soundfontBank;
	bool halfSamplingRate;
	float timeMultiplier;
	int samplingRate;
	bool overrideSamplingRate;
	std::vector<int> skipInstruments;
	unsigned long percussionOffset;
	unsigned long projOffset;
	unsigned long poolOffset;

	GameConfig()
	{
		halfSamplingRate = false;
		timeMultiplier = 0.000001f;

		overrideSamplingRate = false;
		samplingRate = 22050;
		percussionOffset = 0;
		projOffset = 0;
		poolOffset = 0;
	}
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

// CN64SoundbankToolDlg dialog
class CN64SoundbankToolDlg : public CDialog
{
// Construction
public:
	CN64SoundbankToolDlg(CWnd* pParent = NULL);	// standard constructor
	~CN64SoundbankToolDlg();

// Dialog Data
	enum { IDD = IDD_N64SOUNDBANKTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString FilterGameName(CString gameName);
	void InitializeSpecificGames();
	CString mainFolder;
	int countGames;
	GameConfig* gameConfig;
	afx_msg void OnClose();

	int countGamesSound;
	SoundGameConfig* gameConfigSound;

	int countGamesMidi;
	MidiGameConfig* gameConfigMidi;
	CComboBox m_game;
	afx_msg void OnCbnSelchangeCombogame();

	std::vector<t1Result> t1BankList;
	std::vector<ALBank*> alBankList;
	CN64AIFCAudio n64audio;
	afx_msg void OnBnClickedButtonloadrom();

	int GetSizeFile(CString filename);
	CMidiPlayer* midiPlayer;
	afx_msg void OnBnClickedButtonwritedlssoundfont();
	void ParseSoundMacroList(std::vector<Factor5SoundMacro> soundMacroList, std::map<int, Factor5ADSR> poolTables, int& sampleId, unsigned long& attackTime, unsigned long& decayTime, float& sustainPercentage, unsigned long& releaseTime, unsigned char& macroPan);
	bool WriteDLS(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, bool combineBanks, FILE* outFileDebug = NULL);
	bool WriteDLSCombinedFactor5(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, unsigned char* buffer, int romSize, unsigned long projOffset, unsigned long poolOffset, FILE* outFileDebug = NULL);
	bool WriteDLSGMBankPaperMario(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, std::vector<int> instrumentIndex, std::vector<int> instrumentOffset, FILE* outFileDebug = NULL);
	bool WriteDLSCombinedPaperMario(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, unsigned char* buffer, int romSize, unsigned long percussionOffset, std::vector<int> instrumentIndex, std::vector<int> instrumentOffset, FILE* outFileDebug = NULL);
	float Round(float value);
	CComboBox mMidi;
	bool compressed;
	afx_msg void OnBnClickedButtonpreviewmidi();
	unsigned char* buffer;
	int romSize;
	CMidiParse midiParse;
	afx_msg void OnBnClickedButtonpreviewmidi2();
	std::string CStringToString(CString cstring);

	std::vector<MIDIINFO> midiInfos;
	CEdit mTimeMultiplier;
	CComboBox mSoundBankNumber;
	CButton mHalfSamplingRate;
	afx_msg void OnCbnSelchangeCombosound();
	CButton mLoopPlayback;
	CButton mPreviewButton;
	CButton mStopButton;
	CEdit mSamplingRate;
	afx_msg void OnBnClickedButtonwritedlssoundfont2();
	afx_msg void OnBnClickedButtonwritemidistofolder();
	afx_msg void OnBnClickedButtonexportmidi();
	CStatic mGameDescription;
	CButton mSeparateByInstrument;
	CEdit mOutputLoopCount;
	CButton mOutputLoop;
	int CEditControlToDecimalValue(CEdit* editControl);
	CButton mWriteDLSCombinedBanksButton;
	afx_msg void OnBnClickedButtonwritedlssoundfont3();
	CButton mMergePreviewBanksCheck;
	CButton mExtendSmallerTracksToEnd;
	afx_msg void OnBnClickedCheckextendloopstohighesttrack();
	afx_msg void OnBnClickedCheckoutputloop();

	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned long Flip32Bit(unsigned long inLong);
	CButton mUseT1SamplingRate;
	void fprintfIfDebug(FILE* outFileDebug, char* format,...);
	CButton mDebug;
};
