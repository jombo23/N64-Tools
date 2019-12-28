// N64SoundListToolDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "..\N64SoundLibrary\N64AIFCAudio.h"
#include "..\N64SoundToolReader\N64SoundToolReader.h"
#include "shlobj.h"
#include <string>
#include "afxcmn.h"

#define NORMAL 0
#define EADPERCUSSION 1
#define SFX 2
#define PERCUSSION 3

// CN64SoundListToolDlg dialog
class CN64SoundListToolDlg : public CDialog
{
// Construction
public:
	CN64SoundListToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_N64SOUNDLISTTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void InitializeSpecificGames();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox mSoundBankIndex;
	CComboBox mInstrumentChoice;
	CEdit mSamplingRate;
	CEdit mVolume;
	CEdit mPan;
	CEdit mPriority;
	CEdit mTremType;
	CEdit mTremRate;
	CEdit mTremDepth;
	CEdit mTremDelay;
	CEdit mVibType;
	CEdit mVibRate;
	CEdit mVibDepth;
	CEdit mVibDelay;
	CComboBox mSoundChoice;
	CEdit mBase;
	CEdit mLength;
	CButton mCheckRaw;
	CEdit mSampleVol;
	CEdit mSamplePan;
	CEdit mAttackTime;
	CEdit mDecayTime;
	CEdit mReleaseTime;
	CEdit mAttackVolume;
	CEdit mDecayVolume;
	CEdit mVelMin;
	CEdit mVelMax;
	CEdit mKeyMin;
	CEdit mKeyMax;
	CEdit mKeyBase;
	CEdit mDetune;
	CEdit mLoopStart;
	CEdit mLoopEnd;
	CEdit mLoopCount;
	CButton mLoopingEnabled;
	void OnFileOpenrom();
	bool OpenROMPlusDlg();
	bool OpenROMPlusDlg(CString filename);
	CN64AIFCAudio n64AudioLibrary;
	unsigned char* ROM;
	int romSize;
	std::vector<t1Result> t1results;
	std::vector<ctlTblResult> results;
	int numberResults;
	afx_msg void OnClose();
	afx_msg void OnCbnSelchangeCombosoundbank();
	void ShowSoundBankControls();
	void ShowOfficialSDKImport();
	void ShowPercussionControls();
	void ShowEADPercussionControls();
	void ShowSfxControls();
	bool dontupdateitall;
	ALBank* alBankCurrent;
	T1Bank* t1BankCurrent;
	int subBankCurrent;
	afx_msg void OnCbnSelchangeCombosound();
	afx_msg void OnCbnSelchangeCombosound2();
	CStatic mL1Text;
	CStatic mL2Text;
	CStatic mL3Text;
	void UpdateTextBoxes();
	afx_msg void OnEnChangeEditsamplingrate();
	afx_msg void OnEnChangeEditvol();
	afx_msg void OnEnChangeEditpan();
	afx_msg void OnEnChangeEditpriority();
	afx_msg void OnEnChangeEdittremtype();
	afx_msg void OnEnChangeEdittremrate();
	afx_msg void OnEnChangeEdittremdepth();
	afx_msg void OnEnChangeEdittremdelay();
	afx_msg void OnEnChangeEdittvibtype();
	afx_msg void OnEnChangeEditvibrate();
	afx_msg void OnEnChangeEditvibdepth();
	afx_msg void OnEnChangeEditvibdelay();
	afx_msg void OnEnChangeEditsamplepan();
	afx_msg void OnEnChangeEditsamplevol();
	afx_msg void OnEnChangeEditattacktime();
	afx_msg void OnEnChangeEditdecaytime();
	afx_msg void OnEnChangeEditreleasetime();
	afx_msg void OnEnChangeEditattackvolume();
	afx_msg void OnEnChangeEditdecayvolume();
	afx_msg void OnEnChangeEditvelmin();
	afx_msg void OnEnChangeEditvelmax();
	afx_msg void OnEnChangeEditkeymin();
	afx_msg void OnEnChangeEditkeymax();
	afx_msg void OnEnChangeEditkeybase();
	afx_msg void OnEnChangeEditdetune();
	afx_msg void OnEnChangeEditloop();
	afx_msg void OnEnChangeEditend();
	afx_msg void OnEnChangeEditcount();
	afx_msg void OnBnClickedCheckloopenabled();
	afx_msg void OnBnClickedButtonup();
	afx_msg void OnBnClickedButtondown();
	afx_msg void OnBnClickedButtonplayoriginalsound();
	CString mainFolder;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonvadpcmimport();
	afx_msg void OnBnClickedButtonaddsound();
	afx_msg void OnBnClickedButtonaddsound2();
	afx_msg void OnBnClickedButtondeletesound();
	afx_msg void OnBnClickedButtondeletesound2();
	afx_msg void OnFileSave();
	afx_msg void OnBnClickedExportctltbl();
	afx_msg void OnBnClickedInjectplace();
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory, HANDLE out);
	BOOL IsFileExist(LPSTR lpszFilename);
	void PlayWMPlayerSound(CString wavFileSound);
	void KillWMPlayer();
	afx_msg void OnBnClickedButtonrip();
	bool GetFolder(CString& folderpath, const char* szCaption = NULL, HWND hOwner = NULL);
	CString romName;
	afx_msg void OnBnClickedButtontest();
	unsigned long Flip32Bit(unsigned long inLong);
	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	afx_msg void OnFileQuit();
	virtual BOOL DestroyWindow();
	/*afx_msg*/ void OnFileOpenromexplicit();
	afx_msg void OnBnClickedButtonimportpredictors();
	afx_msg void OnBnClickedButtonexportpreditors();
	afx_msg void OnBnClickedButtonimport16bitraw();
	afx_msg void OnBnClickedButtonexportraw();
	CButton m_importPredictors;
	CButton m_exportPredictors;
	afx_msg void OnOpenknownrom64dehakken();
	void OpenROMSpecific(SoundGameConfig gameConfig);
	SoundGameConfig* gameConfig;
	SoundGameConfig currentGameConfig;
	int countGames;
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnFileExtractallknowngames();
	bool OpenROMFilename(CString filename, bool quiet);
	/*afx_msg*/ void OnFileOpenromn64ptrwavetable2();
	afx_msg void OnBnClickedButtonrip3();
	CEdit m_bankName;
	CButton m_injectInPlaceButton;
	CButton m_playButton;
	CButton m_saveButton;
	CButton m_import16BitRaw;
	CButton m_import16BitADPCM;
	CButton m_import16BitRawValues;
	CButton m_add16BitRaw;
	CButton m_add16BitADPCM;
	CButton m_export16BitRawValues;
	CStatic m_importGroupBox;
	CStatic m_addToEndGroupBox;
	CStatic m_exportGroupBox;
	CButton m_deleteButton;
	CStatic m_miscGroupBox;
	CStatic m_tremGroupBox;
	CStatic m_vibrGroupBox;
	CStatic m_instrVolStatic;
	CStatic m_instrPanStatic;
	CStatic m_InstrPriorityStatic;
	CStatic m_instrTremTypeStatic;
	CStatic m_instrTremRateStatic;
	CStatic m_instrTremDepthStatic;
	CStatic m_instrTremDelayStatic;
	CStatic m_instrVibrTypeStatic;
	CStatic m_instrVibrRateStatic;
	CStatic m_instrVibrDepthStatic;
	CStatic m_instrVibrDelayStatic;
	CStatic m_samplePanStatic;
	CStatic m_sampleVolStatic;
	CStatic m_sampleVelMinStatic;
	CStatic m_sampleVelMaxStatic;
	CStatic m_sampleKeyMinStatic;
	CStatic m_sampleKeyMaxStatic;
	CStatic m_sampleKeyBaseStatic;
	CStatic m_sampleDetuneStatic;
	CStatic m_sampleAttackTimeStatic;
	CStatic m_sampleDecayTimeStatic;
	CStatic m_sampleReleaseTimeStatic;
	CStatic m_sampleAttackVolumeStatic;
	CStatic m_sampleDecayVolumeStatic;
	CButton mExportShortFilename;
	CComboBox mSubSound;
	afx_msg void OnCbnSelchangeCombosoundsubsound();
	afx_msg void OnBnClickedButtonimportlooppredictors();
	afx_msg void OnBnClickedButtonexportlooppredictors();
	CButton mImportLoopPredictors;
	CButton mExportLoopPredictors;
	CButton mAddPrevButton;
	CButton mRemovePrevButton;
	CButton mAddSecButton;
	CButton mRemoveSecButton;
	afx_msg void OnBnClickedButtonaddprev();
	afx_msg void OnBnClickedButtonremoveprev();
	afx_msg void OnBnClickedButtonaddprev2();
	afx_msg void OnBnClickedButtonremoveprev2();
	CButton mHalfSamplingRate;
	afx_msg void OnBnClickedButtonstopsound();
	CButton m_stopButton;
	CString LoadResourceText(int resourceTextId);
	void ReadSoundbanks(SoundGameConfig gameConfig, int& numberResults, std::vector<ctlTblResult>& results);
	HINSTANCE initiallyLoadedResource;
	HINSTANCE mainExeResource;
	CStatic m_InstrBendRangeStatic;
	CEdit mBendRange;
	afx_msg void OnEnChangeEditbendrange();
	CComboBox mPercussionChoice;
	int percussionMode;
	afx_msg void OnCbnSelchangeCombopercussion();
	CStatic mPercussionLabel;
	CButton mUpArrowButton;
	CButton mDownArrowButton;
	CStatic mSfxLabel;
	CComboBox mSfxChoice;
	afx_msg void OnCbnSelchangeCombosfx();
	afx_msg void OnBnClickedCheckhalfsamplingrate();
	CButton mHalfVADPCMPrecision;
	CButton mOverrideRate;
	afx_msg void OnBnClickedCheckoverriderate();
	CListCtrl mKeyBaseListCtrl;
	void UpdateSamplingRateKeyBaseList();
	void UpdateSamplingRateSelection();
	CButton mAddInstrumentButton;
	afx_msg void OnBnClickedButtonaddinstrument();
	CButton mDeleteInstrumentButton;
	afx_msg void OnBnClickedButtondeleteinstrument();
	void ImportVadpcmSGIWav(int order);
	afx_msg void OnBnClickedButtonimportsdktools();
	CButton mImportFromSDKTools;
	afx_msg void OnBnClickedButtonkonamiadsrbutton();
	CButton mKonamiADSREditorButton;
	CStatic mLabelTableIndex;
	CEdit mTableIndex;
	CButton mCheckUnknownEADFlag;
	CButton m_import16BitADPCMSamePred;
	afx_msg void OnBnClickedButtonvadpcmimportsamepred();
	CButton mDeletePercussionBank;
	CButton mAddPercussionBank;
	CButton mDeleteSfxBank;
	CButton mAddSfxBank;
	afx_msg void OnBnClickedButtondeletepercussionbank();
	afx_msg void OnBnClickedButtonaddpercussionbank();
	afx_msg void OnBnClickedButtondeletesfxbank();
	afx_msg void OnBnClickedButtonaddsfxbank();
	CButton mUseT1;
	CStatic mT1GroupBox;
	CStatic mT1IndexLabel;
	CComboBox mT1Index;
	CStatic mT1FrequencyLabel;
	CEdit mT1Frequency;
	CStatic mT1SubBankLabel;
	CComboBox mT1SubBank;
	CStatic mT1InstrumentLabel;
	CComboBox mT1Instrument;
	CStatic mT1SoundLabel;
	CComboBox mT1Sound;
	afx_msg void OnCbnSelchangeCombot1indexcombo();
	afx_msg void OnEnChangeEditt1frequency();
	afx_msg void OnCbnSelchangeCombot1subbank();
	afx_msg void OnCbnSelchangeCombot1subbank2();
	afx_msg void OnCbnSelchangeCombot1sound();
	CStatic mT1FlagsLabel;
	CEdit mT1Flags;
	afx_msg void OnEnChangeEditt1flags();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBnClickedButtonplaysnd2();
	CButton m_playLoopedButton;
	std::vector<ALBank*> GetCombinedActiveBanks();
};