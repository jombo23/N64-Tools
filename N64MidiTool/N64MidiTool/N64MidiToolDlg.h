// N64MidiToolDlg.h : header file
//

#pragma once
//#include "vld.h"

#include "..\N64MidiLibrary\MidiParse.h"
#include "..\N64MidiToolReader\N64MidiToolReader.h"
#include "..\N64MidiLibrary\GECompression.h"
#include "..\N64MidiLibrary\SupermanDecoder.h"
#include "..\N64MidiLibrary\rnc_deco.h"
#include "..\N64MidiLibrary\H20Decoder.h"
#include "..\N64MidiLibrary\TetrisphereDecoder.h"
#include "..\N64MidiLibrary\MidwayDecoder.h"
#include "..\N64MidiLibrary\ASMICDecoder.h"

#include "afxwin.h"

// CN64MidiToolDlg dialog
class CN64MidiToolDlg : public CDialog
{
// Construction
public:
	CN64MidiToolDlg(CWnd* pParent = NULL);	// standard constructor
	~CN64MidiToolDlg();
// Dialog Data
	enum { IDD = IDD_N64MIDITOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_game;
	CComboBox m_spot;
	CButton m_loop;
	afx_msg void OnBnClickedButtonloadrom();
private:
	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned long Flip32Bit(unsigned long inLong);
	int GetSizeFile(CString filename);
	unsigned char* buffer;
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	unsigned short Flip16Bit(unsigned short ShortValue);
	bool compressed;
	unsigned long startSpot;
	unsigned long endSpot;
	CString gameName;
	bool CompressGZipFile(CString inputFile, CString outputFile, bool byteFlipCompressed, CString gameType);
	BOOL IsFileExist(LPSTR lpszFilename);
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
	BOOL normalExec (PTSTR pCmdLine, CString currentDirectory);
	CString mainFolder;
	void InitializeSpecificGames();
	int countGames;
public:
	afx_msg void OnCbnSelchangeCombomidi();
	afx_msg void OnBnClickedButtonexportbin();
	afx_msg void OnBnClickedButtonexportmidi();
	afx_msg void OnBnClickedButtonimportbin();
	afx_msg void OnBnClickedButtonimportmidi();
	void ConvertIntoSpot(CString inputFile);
	afx_msg void OnBnClickedButtonexportalltomidi();
	void KillWMPlayer();
	afx_msg void OnBnClickedButtonexportalltorawbin();
	afx_msg void OnBnClickedButtonwriterom();
	CString lastRomName;
	unsigned long romSize;
	afx_msg void OnBnClickedButtonplaymidi();
	afx_msg void OnBnClickedButtonscandirformidis();
	int WorkOnFilesIntoADirectory(char* pszFiles, CString fileNames[0x1000]);
	MidiGameConfig* gameConfig;
	afx_msg void OnBnClickedButtonriproms();
	CEdit m_numberInstruments;
	CEdit mLoopPoint;
	CButton mNoRepeaters;
	afx_msg void OnBnClickedLoopDebug();
	afx_msg void OnBnClickedButtongenmidis();
	CButton mSeparateByInstrument;
	CButton mDebugTextFile;
	CButton mShowEffectsGames;
	afx_msg void OnBnClickedCheckshowbfx();
	CStatic mMasterTrackEffectLabel;
	CComboBox mMasterTrackEffect;
	CButton mOutputLoop;
	CEdit mOutputLoopCount;
	int CEditControlToDecimalValue(CEdit* editControl);
	CButton mExtendSmallerTracksToEnd;
	afx_msg void OnBnClickedCheckextendloopstohighesttrack();
	afx_msg void OnBnClickedCheckoutputloop();
	CStatic mGameName;
	CStatic mMidiType;
	CStatic mMasterTrackEffectLabel2;
	CComboBox mMasterTrackEffect2;
	CStatic mEffectGroupBox;
	CStatic mPitchBendSensitivityLabel;
	CComboBox mPitchBendSensitivity;
};
