// N64SoundListToolDlg.cpp : implementation file
//
#pragma once
#include "stdafx.h"
#include "N64SoundListTool.h"
#include "N64SoundListToolDlg.h"
#include "mmsystem.h"
#include "OpenSpecificCtlTbl.h"
#include "FolderDialog.h"
#include "..\N64SoundLibrary\flzh_rn.h"
#include "..\N64SoundLibrary\SharedFunctions.h"
#include "..\N64SoundLibrary\yay0.h"
#include "..\N64SoundLibrary\SupermanDecoder.h"
#include "..\N64SoundLibrary\n643docompression.h"
#include "..\N64SoundLibrary\MKMythologiesDecode.h"
#include "..\N64SoundLibrary\TigDecoder.h"
#include "..\N64SoundLibrary\SydneyDecoder.h"
#include "..\N64SoundLibrary\NinDec.h"
#include "..\N64SoundLibrary\NintendoEncoder.h"
#include "..\N64SoundLibrary\CalcCRC.h"
#include "KonamiADSRDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAXRESULTS 4000

#define MENUEVENT 2999
#define STARTMENUOPTIONS 3000

// CN64SoundListToolDlg dialog


CN64SoundListToolDlg::CN64SoundListToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CN64SoundListToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	gameConfig = NULL;
}

void CN64SoundListToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOSOUNDBANK, mSoundBankIndex);
	DDX_Control(pDX, IDC_COMBOSOUND, mInstrumentChoice);
	DDX_Control(pDX, IDC_EDITSAMPLINGRATE, mSamplingRate);
	DDX_Control(pDX, IDC_EDITVOL, mVolume);
	DDX_Control(pDX, IDC_EDITPAN, mPan);
	DDX_Control(pDX, IDC_EDITPRIORITY, mPriority);
	DDX_Control(pDX, IDC_EDITTREMTYPE, mTremType);
	DDX_Control(pDX, IDC_EDITTREMRATE, mTremRate);
	DDX_Control(pDX, IDC_EDITTREMDEPTH, mTremDepth);
	DDX_Control(pDX, IDC_EDITTREMDELAY, mTremDelay);
	DDX_Control(pDX, IDC_EDITTVIBTYPE, mVibType);
	DDX_Control(pDX, IDC_EDITVIBRATE, mVibRate);
	DDX_Control(pDX, IDC_EDITVIBDEPTH, mVibDepth);
	DDX_Control(pDX, IDC_EDITVIBDELAY, mVibDelay);
	DDX_Control(pDX, IDC_COMBOSOUND2, mSoundChoice);
	DDX_Control(pDX, IDC_EDITBASE, mBase);
	DDX_Control(pDX, IDC_EDITLENGTH, mLength);
	DDX_Control(pDX, IDC_CHECKRAW, mCheckRaw);
	DDX_Control(pDX, IDC_EDITSAMPLEVOL, mSampleVol);
	DDX_Control(pDX, IDC_EDITSAMPLEPAN, mSamplePan);
	DDX_Control(pDX, IDC_EDITATTACKTIME, mAttackTime);
	DDX_Control(pDX, IDC_EDITDECAYTIME, mDecayTime);
	DDX_Control(pDX, IDC_EDITRELEASETIME, mReleaseTime);
	DDX_Control(pDX, IDC_EDITATTACKVOLUME, mAttackVolume);
	DDX_Control(pDX, IDC_EDITDECAYVOLUME, mDecayVolume);
	DDX_Control(pDX, IDC_EDITVELMIN, mVelMin);
	DDX_Control(pDX, IDC_EDITVELMAX, mVelMax);
	DDX_Control(pDX, IDC_EDITKEYMIN, mKeyMin);
	DDX_Control(pDX, IDC_EDITKEYMAX, mKeyMax);
	DDX_Control(pDX, IDC_EDITKEYBASE, mKeyBase);
	DDX_Control(pDX, IDC_EDITDETUNE, mDetune);
	DDX_Control(pDX, IDC_EDITLOOP, mLoopStart);
	DDX_Control(pDX, IDC_EDITEND, mLoopEnd);
	DDX_Control(pDX, IDC_EDITCOUNT, mLoopCount);
	DDX_Control(pDX, IDC_CHECKLOOPENABLED, mLoopingEnabled);
	DDX_Control(pDX, IDC_STATICL1, mL1Text);
	DDX_Control(pDX, IDC_STATICL2, mL2Text);
	DDX_Control(pDX, IDC_STATICL3, mL3Text);
	DDX_Control(pDX, IDC_BUTTONIMPORTPREDICTORS, m_importPredictors);
	DDX_Control(pDX, IDC_BUTTONEXPORTPREDITORS, m_exportPredictors);
	DDX_Control(pDX, IDC_EDITBANKNAME, m_bankName);
	DDX_Control(pDX, IDC_INJECTPLACE, m_injectInPlaceButton);
	DDX_Control(pDX, IDC_BUTTONPLAYSND, m_playButton);
	DDX_Control(pDX, IDC_BUTTONDELETESOUND2, m_saveButton);
	DDX_Control(pDX, IDC_BUTTON1, m_import16BitRaw);
	DDX_Control(pDX, IDC_BUTTONVADPCMIMPORT, m_import16BitADPCM);
	DDX_Control(pDX, IDC_BUTTONIMPORT16BITRAW, m_import16BitRawValues);
	DDX_Control(pDX, IDC_BUTTONADDSOUND, m_add16BitRaw);
	DDX_Control(pDX, IDC_BUTTONADDSOUND2, m_add16BitADPCM);
	DDX_Control(pDX, IDC_BUTTONEXPORTRAW, m_export16BitRawValues);
	DDX_Control(pDX, IDC_STATICIMPORTGROUPBOX, m_importGroupBox);
	DDX_Control(pDX, IDC_STATICADDTOENDGROUPBOX, m_addToEndGroupBox);
	DDX_Control(pDX, IDC_STATICEXPORTGROUPBOX, m_exportGroupBox);
	DDX_Control(pDX, IDC_BUTTONDELETESOUND, m_deleteButton);
	DDX_Control(pDX, IDC_STATICMISCGROUPBOX, m_miscGroupBox);
	DDX_Control(pDX, IDC_STATICTREMGROUPBOX, m_tremGroupBox);
	DDX_Control(pDX, IDC_STATICVIBRGROUPBOX, m_vibrGroupBox);
	DDX_Control(pDX, IDC_STATICINSTRVOL, m_instrVolStatic);
	DDX_Control(pDX, IDC_STATICINSTRPAN, m_instrPanStatic);
	DDX_Control(pDX, IDC_STATICINSTRPRIORITY, m_InstrPriorityStatic);
	DDX_Control(pDX, IDC_STATICINSTRTREM, m_instrTremTypeStatic);
	DDX_Control(pDX, IDC_STATICINSTRRATE, m_instrTremRateStatic);
	DDX_Control(pDX, IDC_STATICINSTRDEPTH, m_instrTremDepthStatic);
	DDX_Control(pDX, IDC_STATICINSTRDELAY, m_instrTremDelayStatic);
	DDX_Control(pDX, IDC_STATICINSTRVIBTYPE, m_instrVibrTypeStatic);
	DDX_Control(pDX, IDC_STATICINSTRVIBRATE, m_instrVibrRateStatic);
	DDX_Control(pDX, IDC_STATICINSTRVIBDEPTH, m_instrVibrDepthStatic);
	DDX_Control(pDX, IDC_STATICINSTRVIBDELAY, m_instrVibrDelayStatic);
	DDX_Control(pDX, IDC_STATICSAMPLEPANSTATIC, m_samplePanStatic);
	DDX_Control(pDX, IDC_STATICSAMPLEVOLTEXT, m_sampleVolStatic);
	DDX_Control(pDX, IDC_STATICVELMINTEXT, m_sampleVelMinStatic);
	DDX_Control(pDX, IDC_STATICVELMAXTEXT, m_sampleVelMaxStatic);
	DDX_Control(pDX, IDC_STATICKEYMINTEXT, m_sampleKeyMinStatic);
	DDX_Control(pDX, IDC_STATICKEYMAXTEXT, m_sampleKeyMaxStatic);
	DDX_Control(pDX, IDC_STATICKEYBASETEXT, m_sampleKeyBaseStatic);
	DDX_Control(pDX, IDC_STATICDETUNETEXT, m_sampleDetuneStatic);
	DDX_Control(pDX, IDC_STATICATTACKTIMETEXT, m_sampleAttackTimeStatic);
	DDX_Control(pDX, IDC_STATICDECAYTIMETEXT, m_sampleDecayTimeStatic);
	DDX_Control(pDX, IDC_STATICRELEASETIMETEXT, m_sampleReleaseTimeStatic);
	DDX_Control(pDX, IDC_STATICATTACKVOLUMETEXT, m_sampleAttackVolumeStatic);
	DDX_Control(pDX, IDC_STATICDECAYVOLUMETEXT, m_sampleDecayVolumeStatic);
	DDX_Control(pDX, IDC_CHECKSHORTWAVFILENAME, mExportShortFilename);
	DDX_Control(pDX, IDC_COMBOSOUNDSUBSOUND, mSubSound);
	DDX_Control(pDX, IDC_BUTTONIMPORTLOOPPREDICTORS, mImportLoopPredictors);
	DDX_Control(pDX, IDC_BUTTONEXPORTLOOPPREDICTORS, mExportLoopPredictors);
	DDX_Control(pDX, IDC_BUTTONADDPREV, mAddPrevButton);
	DDX_Control(pDX, IDC_BUTTONREMOVEPREV, mRemovePrevButton);
	DDX_Control(pDX, IDC_BUTTONADDPREV2, mAddSecButton);
	DDX_Control(pDX, IDC_BUTTONREMOVEPREV2, mRemoveSecButton);
	DDX_Control(pDX, IDC_CHECKHALFSAMPLINGRATE, mHalfSamplingRate);
	DDX_Control(pDX, IDC_BUTTONSTOPSOUND, m_stopButton);
	DDX_Control(pDX, IDC_STATICBENDRANGE, m_InstrBendRangeStatic);
	DDX_Control(pDX, IDC_EDITBENDRANGE, mBendRange);
	DDX_Control(pDX, IDC_COMBOPERCUSSION, mPercussionChoice);
	DDX_Control(pDX, IDC_PERCUSSIONLABEL, mPercussionLabel);
	DDX_Control(pDX, IDC_BUTTONUP, mUpArrowButton);
	DDX_Control(pDX, IDC_BUTTONDOWN, mDownArrowButton);
	DDX_Control(pDX, IDC_SFXLABEL, mSfxLabel);
	DDX_Control(pDX, IDC_COMBOSFX, mSfxChoice);
	DDX_Control(pDX, IDC_CHECKHALF_VADPCM, mHalfVADPCMPrecision);
	DDX_Control(pDX, IDC_CHECKOVERRIDERATE, mOverrideRate);
	DDX_Control(pDX, IDC_LISTKEYBASE, mKeyBaseListCtrl);
	DDX_Control(pDX, IDC_BUTTONADDINSTRUMENT, mAddInstrumentButton);
	DDX_Control(pDX, IDC_BUTTONDELETEINSTRUMENT, mDeleteInstrumentButton);
	DDX_Control(pDX, IDC_BUTTONIMPORTSDKTOOLS, mImportFromSDKTools);
	DDX_Control(pDX, IDC_BUTTONKONAMIADSRBUTTON, mKonamiADSREditorButton);
	DDX_Control(pDX, IDC_STATICTBLINDEX, mLabelTableIndex);
	DDX_Control(pDX, IDC_EDITTBLINDEX, mTableIndex);
	DDX_Control(pDX, IDC_CHECKUNKNOWNEADFLAG, mCheckUnknownEADFlag);
	DDX_Control(pDX, IDC_BUTTONVADPCMIMPORTSAMEPRED, m_import16BitADPCMSamePred);
	DDX_Control(pDX, IDC_BUTTONDELETEPERCUSSIONBANK, mDeletePercussionBank);
	DDX_Control(pDX, IDC_BUTTONADDPERCUSSIONBANK, mAddPercussionBank);
	DDX_Control(pDX, IDC_BUTTONDELETESFXBANK, mDeleteSfxBank);
	DDX_Control(pDX, IDC_BUTTONADDSFXBANK, mAddSfxBank);
	DDX_Control(pDX, IDC_CHECKOVERRIDET1, mUseT1);
	DDX_Control(pDX, IDC_T1GROUPLABEL, mT1GroupBox);
	DDX_Control(pDX, IDC_T1INDEXLABEL, mT1IndexLabel);
	DDX_Control(pDX, IDC_COMBOT1INDEXCOMBO, mT1Index);
	DDX_Control(pDX, IDC_T1FREQUENCYLABEL, mT1FrequencyLabel);
	DDX_Control(pDX, IDC_EDITT1FREQUENCY, mT1Frequency);
	DDX_Control(pDX, IDC_T1SUBBANK, mT1SubBankLabel);
	DDX_Control(pDX, IDC_COMBOT1SUBBANK, mT1SubBank);
	DDX_Control(pDX, IDC_T1INSTRUMENTLABEL, mT1InstrumentLabel);
	DDX_Control(pDX, IDC_COMBOT1SUBBANK2, mT1Instrument);
	DDX_Control(pDX, IDC_T1SOUNDLABEL, mT1SoundLabel);
	DDX_Control(pDX, IDC_COMBOT1SOUND, mT1Sound);
	DDX_Control(pDX, IDC_T1FLAGSLABEL, mT1FlagsLabel);
	DDX_Control(pDX, IDC_EDITT1FLAGS, mT1Flags);
	DDX_Control(pDX, IDC_BUTTONPLAYSND2, m_playLoopedButton);
}

BEGIN_MESSAGE_MAP(CN64SoundListToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBOSOUNDBANK, &CN64SoundListToolDlg::OnCbnSelchangeCombosoundbank)
	ON_CBN_SELCHANGE(IDC_COMBOSOUND, &CN64SoundListToolDlg::OnCbnSelchangeCombosound)
	ON_CBN_SELCHANGE(IDC_COMBOSOUND2, &CN64SoundListToolDlg::OnCbnSelchangeCombosound2)
	ON_EN_CHANGE(IDC_EDITSAMPLINGRATE, &CN64SoundListToolDlg::OnEnChangeEditsamplingrate)
	ON_EN_CHANGE(IDC_EDITVOL, &CN64SoundListToolDlg::OnEnChangeEditvol)
	ON_EN_CHANGE(IDC_EDITPAN, &CN64SoundListToolDlg::OnEnChangeEditpan)
	ON_EN_CHANGE(IDC_EDITPRIORITY, &CN64SoundListToolDlg::OnEnChangeEditpriority)
	ON_EN_CHANGE(IDC_EDITTREMTYPE, &CN64SoundListToolDlg::OnEnChangeEdittremtype)
	ON_EN_CHANGE(IDC_EDITTREMRATE, &CN64SoundListToolDlg::OnEnChangeEdittremrate)
	ON_EN_CHANGE(IDC_EDITTREMDEPTH, &CN64SoundListToolDlg::OnEnChangeEdittremdepth)
	ON_EN_CHANGE(IDC_EDITTREMDELAY, &CN64SoundListToolDlg::OnEnChangeEdittremdelay)
	ON_EN_CHANGE(IDC_EDITTVIBTYPE, &CN64SoundListToolDlg::OnEnChangeEdittvibtype)
	ON_EN_CHANGE(IDC_EDITVIBRATE, &CN64SoundListToolDlg::OnEnChangeEditvibrate)
	ON_EN_CHANGE(IDC_EDITVIBDEPTH, &CN64SoundListToolDlg::OnEnChangeEditvibdepth)
	ON_EN_CHANGE(IDC_EDITVIBDELAY, &CN64SoundListToolDlg::OnEnChangeEditvibdelay)
	ON_EN_CHANGE(IDC_EDITSAMPLEPAN, &CN64SoundListToolDlg::OnEnChangeEditsamplepan)
	ON_EN_CHANGE(IDC_EDITSAMPLEVOL, &CN64SoundListToolDlg::OnEnChangeEditsamplevol)
	ON_EN_CHANGE(IDC_EDITATTACKTIME, &CN64SoundListToolDlg::OnEnChangeEditattacktime)
	ON_EN_CHANGE(IDC_EDITDECAYTIME, &CN64SoundListToolDlg::OnEnChangeEditdecaytime)
	ON_EN_CHANGE(IDC_EDITRELEASETIME, &CN64SoundListToolDlg::OnEnChangeEditreleasetime)
	ON_EN_CHANGE(IDC_EDITATTACKVOLUME, &CN64SoundListToolDlg::OnEnChangeEditattackvolume)
	ON_EN_CHANGE(IDC_EDITDECAYVOLUME, &CN64SoundListToolDlg::OnEnChangeEditdecayvolume)
	ON_EN_CHANGE(IDC_EDITVELMIN, &CN64SoundListToolDlg::OnEnChangeEditvelmin)
	ON_EN_CHANGE(IDC_EDITVELMAX, &CN64SoundListToolDlg::OnEnChangeEditvelmax)
	ON_EN_CHANGE(IDC_EDITKEYMIN, &CN64SoundListToolDlg::OnEnChangeEditkeymin)
	ON_EN_CHANGE(IDC_EDITKEYMAX, &CN64SoundListToolDlg::OnEnChangeEditkeymax)
	ON_EN_CHANGE(IDC_EDITKEYBASE, &CN64SoundListToolDlg::OnEnChangeEditkeybase)
	ON_EN_CHANGE(IDC_EDITDETUNE, &CN64SoundListToolDlg::OnEnChangeEditdetune)
	ON_EN_CHANGE(IDC_EDITLOOP, &CN64SoundListToolDlg::OnEnChangeEditloop)
	ON_EN_CHANGE(IDC_EDITEND, &CN64SoundListToolDlg::OnEnChangeEditend)
	ON_EN_CHANGE(IDC_EDITCOUNT, &CN64SoundListToolDlg::OnEnChangeEditcount)
	ON_BN_CLICKED(IDC_CHECKLOOPENABLED, &CN64SoundListToolDlg::OnBnClickedCheckloopenabled)
	ON_BN_CLICKED(IDC_BUTTONUP, &CN64SoundListToolDlg::OnBnClickedButtonup)
	ON_BN_CLICKED(IDC_BUTTONDOWN, &CN64SoundListToolDlg::OnBnClickedButtondown)
	ON_BN_CLICKED(IDC_BUTTON1, &CN64SoundListToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTONVADPCMIMPORT, &CN64SoundListToolDlg::OnBnClickedButtonvadpcmimport)
	ON_BN_CLICKED(IDC_BUTTONADDSOUND, &CN64SoundListToolDlg::OnBnClickedButtonaddsound)
	ON_BN_CLICKED(IDC_BUTTONADDSOUND2, &CN64SoundListToolDlg::OnBnClickedButtonaddsound2)
	ON_BN_CLICKED(IDC_BUTTONDELETESOUND, &CN64SoundListToolDlg::OnBnClickedButtondeletesound)
	ON_BN_CLICKED(IDC_BUTTONDELETESOUND2, &CN64SoundListToolDlg::OnBnClickedButtondeletesound2)
	ON_COMMAND(ID_FILE_SAVE, &CN64SoundListToolDlg::OnFileSave)
	ON_BN_CLICKED(IDC_EXPORTCTLTBL, &CN64SoundListToolDlg::OnBnClickedExportctltbl)
	ON_BN_CLICKED(IDC_INJECTPLACE, &CN64SoundListToolDlg::OnBnClickedInjectplace)
	ON_BN_CLICKED(IDC_BUTTONRIP, &CN64SoundListToolDlg::OnBnClickedButtonrip)
	ON_BN_CLICKED(IDC_BUTTONPLAYSND, &CN64SoundListToolDlg::OnBnClickedButtonplayoriginalsound)
	ON_BN_CLICKED(IDC_BUTTONTEST, &CN64SoundListToolDlg::OnBnClickedButtontest)
	ON_COMMAND(ID_FILE_QUIT, &CN64SoundListToolDlg::OnFileQuit)
	ON_COMMAND(ID_FILE_OPENROMEXPLICIT, &CN64SoundListToolDlg::OnFileOpenromexplicit)
	ON_BN_CLICKED(IDC_BUTTONIMPORTPREDICTORS, &CN64SoundListToolDlg::OnBnClickedButtonimportpredictors)
	ON_BN_CLICKED(IDC_BUTTONEXPORTPREDITORS, &CN64SoundListToolDlg::OnBnClickedButtonexportpreditors)
	ON_BN_CLICKED(IDC_BUTTONIMPORT16BITRAW, &CN64SoundListToolDlg::OnBnClickedButtonimport16bitraw)
	ON_BN_CLICKED(IDC_BUTTONEXPORTRAW, &CN64SoundListToolDlg::OnBnClickedButtonexportraw)
	ON_COMMAND(ID_FILE_EXTRACTALLKNOWNGAMES, &CN64SoundListToolDlg::OnFileExtractallknowngames)
	ON_COMMAND(ID_FILE_OPENROMN64PTRWAVETABLE2, &CN64SoundListToolDlg::OnFileOpenromn64ptrwavetable2)
	ON_BN_CLICKED(IDC_BUTTONRIP3, &CN64SoundListToolDlg::OnBnClickedButtonrip3)
	ON_CBN_SELCHANGE(IDC_COMBOSOUNDSUBSOUND, &CN64SoundListToolDlg::OnCbnSelchangeCombosoundsubsound)
	ON_BN_CLICKED(IDC_BUTTONIMPORTLOOPPREDICTORS, &CN64SoundListToolDlg::OnBnClickedButtonimportlooppredictors)
	ON_BN_CLICKED(IDC_BUTTONEXPORTLOOPPREDICTORS, &CN64SoundListToolDlg::OnBnClickedButtonexportlooppredictors)
	ON_BN_CLICKED(IDC_BUTTONADDPREV, &CN64SoundListToolDlg::OnBnClickedButtonaddprev)
	ON_BN_CLICKED(IDC_BUTTONREMOVEPREV, &CN64SoundListToolDlg::OnBnClickedButtonremoveprev)
	ON_BN_CLICKED(IDC_BUTTONADDPREV2, &CN64SoundListToolDlg::OnBnClickedButtonaddprev2)
	ON_BN_CLICKED(IDC_BUTTONREMOVEPREV2, &CN64SoundListToolDlg::OnBnClickedButtonremoveprev2)
	ON_BN_CLICKED(IDC_BUTTONSTOPSOUND, &CN64SoundListToolDlg::OnBnClickedButtonstopsound)

	ON_EN_CHANGE(IDC_EDITBENDRANGE, &CN64SoundListToolDlg::OnEnChangeEditbendrange)
	ON_CBN_SELCHANGE(IDC_COMBOPERCUSSION, &CN64SoundListToolDlg::OnCbnSelchangeCombopercussion)
	ON_CBN_SELCHANGE(IDC_COMBOSFX, &CN64SoundListToolDlg::OnCbnSelchangeCombosfx)
	ON_BN_CLICKED(IDC_CHECKHALFSAMPLINGRATE, &CN64SoundListToolDlg::OnBnClickedCheckhalfsamplingrate)
	ON_BN_CLICKED(IDC_CHECKOVERRIDERATE, &CN64SoundListToolDlg::OnBnClickedCheckoverriderate)
	ON_BN_CLICKED(IDC_BUTTONADDINSTRUMENT, &CN64SoundListToolDlg::OnBnClickedButtonaddinstrument)
	ON_BN_CLICKED(IDC_BUTTONDELETEINSTRUMENT, &CN64SoundListToolDlg::OnBnClickedButtondeleteinstrument)
	ON_BN_CLICKED(IDC_BUTTONIMPORTSDKTOOLS, &CN64SoundListToolDlg::OnBnClickedButtonimportsdktools)
	ON_BN_CLICKED(IDC_BUTTONKONAMIADSRBUTTON, &CN64SoundListToolDlg::OnBnClickedButtonkonamiadsrbutton)
	ON_BN_CLICKED(IDC_BUTTONVADPCMIMPORTSAMEPRED, &CN64SoundListToolDlg::OnBnClickedButtonvadpcmimportsamepred)
	ON_BN_CLICKED(IDC_BUTTONDELETEPERCUSSIONBANK, &CN64SoundListToolDlg::OnBnClickedButtondeletepercussionbank)
	ON_BN_CLICKED(IDC_BUTTONADDPERCUSSIONBANK, &CN64SoundListToolDlg::OnBnClickedButtonaddpercussionbank)
	ON_BN_CLICKED(IDC_BUTTONDELETESFXBANK, &CN64SoundListToolDlg::OnBnClickedButtondeletesfxbank)
	ON_BN_CLICKED(IDC_BUTTONADDSFXBANK, &CN64SoundListToolDlg::OnBnClickedButtonaddsfxbank)
	ON_CBN_SELCHANGE(IDC_COMBOT1INDEXCOMBO, &CN64SoundListToolDlg::OnCbnSelchangeCombot1indexcombo)
	ON_EN_CHANGE(IDC_EDITT1FREQUENCY, &CN64SoundListToolDlg::OnEnChangeEditt1frequency)
	ON_CBN_SELCHANGE(IDC_COMBOT1SUBBANK, &CN64SoundListToolDlg::OnCbnSelchangeCombot1subbank)
	ON_CBN_SELCHANGE(IDC_COMBOT1SUBBANK2, &CN64SoundListToolDlg::OnCbnSelchangeCombot1subbank2)
	ON_CBN_SELCHANGE(IDC_COMBOT1SOUND, &CN64SoundListToolDlg::OnCbnSelchangeCombot1sound)
	ON_EN_CHANGE(IDC_EDITT1FLAGS, &CN64SoundListToolDlg::OnEnChangeEditt1flags)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BUTTONPLAYSND2, &CN64SoundListToolDlg::OnBnClickedButtonplaysnd2)
END_MESSAGE_MAP()

// CN64SoundListToolDlg message handlers

BOOL CN64SoundListToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText("N64 Sound Tool V1.4 (By SubDrag and Ice Mario and Zoinkity) " __DATE__);

	mUseT1.SetCheck(true);

	percussionMode = NORMAL;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	/*CString romName = "C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\Star Fox 64 (U) (V1.0) [!].z64";
	int romSize = n64AudioLibrary.GetSizeFile(romName);

	FILE* inTemp = fopen(romName, "rb");
	unsigned char* rom = new unsigned char[romSize];
	fread(rom, 1, romSize, inTemp);
	fclose(inTemp);

	unsigned long crc1a = CharArrayToLong(&rom[16]);
	unsigned long crc2a = CharArrayToLong(&rom[20]);

	std::map<unsigned char, CString> cicLookup;
	cicLookup[0] = "starf";
    cicLookup[0xD5] = "lylat";
    cicLookup[0xDE] = "mario";
    cicLookup[0xDB] = "diddy";
    cicLookup[0xE4] = "aleck";
    cicLookup[0x14] = "zelda";
    cicLookup[0xEC] = "yoshi";

	if (cicLookup.find(rom[0x173]) != cicLookup.end())
	{
		CString cic = cicLookup[rom[0x173]];
		if (cic == "starf")
		{
            if (rom[0x16F]==0xDB)
                cic = "ddipl";
            else if (rom[0x16F]==0xD9)
                cic = "dddev";
		}

		CalcCRC calcCRC;
		calcCRC.calccrc(rom, romSize, cic, true);

		unsigned long crc1b = CharArrayToLong(&rom[16]);
		unsigned long crc2b = CharArrayToLong(&rom[20]);

		if (crc1a != crc1b)
		{
			MessageBox("CRC Error 1");
		}

		if (crc2a != crc2b)
		{
			MessageBox("CRC Error 2");
		}
	}
	
	delete [] rom;*/

	// TEST
	/*FILE* inTemp = fopen("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\NBA Jam 99 (U) [!].z64", "rb");
	unsigned char* tempChar = new unsigned char[0xC00000];
	fread(tempChar, 1, 0xC00000, inTemp);
	fclose(inTemp);

	unsigned char* output = new unsigned char[0x100000];
	RncDecoder rncDec;
	int packLen = 0;
	int decSize = rncDec.unpackM1(&tempChar[0xC4DCC], output, 0, packLen);

	FILE* outTest = fopen("C:\\temp\\rnc81.bin", "wb");
	fwrite(output, 1, decSize, outTest);
	fclose(outTest);

	delete [] output;
	delete [] tempChar;*/

	
	/*FILE* inTemp = fopen("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\Paper Mario (E) (M4) (VC).z64", "rb");
	unsigned char* tempChar = new unsigned char[0x2800000];
	fread(tempChar, 1, 0x2800000, inTemp);
	fclose(inTemp);

	unsigned long sbnLocation = 0x013A0000;
	unsigned char* sbn = &tempChar[sbnLocation];

	unsigned long sbnSize = CharArrayToLong(&sbn[4]);
	unsigned long tblOffset = CharArrayToLong(&sbn[0x10]);
	unsigned long numTableEntries = CharArrayToLong(&sbn[0x14]);
	unsigned long dataFollowingTable = CharArrayToLong(&sbn[0x18]);
	unsigned long offsetInitTable = CharArrayToLong(&sbn[0x24]);

	FILE* outPaper = fopen("C:\\temp\\paper.txt", "w");

	for (int x = 0; x < numTableEntries; x++)
	{
		unsigned long tblDataOffset = tblOffset + (x * 8);
		unsigned long offsetEntryData = CharArrayToLong(&sbn[tblDataOffset + 0]);
		unsigned long typeSizeCombo = CharArrayToLong(&sbn[tblDataOffset + 4]);

		unsigned char type = ((typeSizeCombo >> 28) & 0xF);
		unsigned long size = typeSizeCombo & 0xFFFFFFF;

		if (type == 1) //1	BGM
		{

		}
		else if (type == 2) //2	SEF
		{

		}
		else if (type == 3) //3	BK
		{
			unsigned long bkAddress = offsetEntryData;
			fprintf(outPaper, "%08X,%08X\n", sbnLocation + bkAddress, sbnLocation + bkAddress);
			
			if (bkAddress == 0x2CFB00)
			{
				unsigned long bankSize = CharArrayToLong(&sbn[bkAddress+4]);
				CString bankName = (char*)sbn[bkAddress + 8];

				unsigned long instrumentData = CharArrayToLong(&sbn[bkAddress+0x10]);
	

			}
		}
		else if (type == 4) //4	MSEQ
		{

		}

		offsetEntryData += size;
	}

	fclose(outPaper);

	delete [] tempChar;*/

	mT1SubBank.ResetContent();
	mT1Instrument.ResetContent();
	mT1Sound.ResetContent();

	for (int x = 0; x < 0x100; x++)
	{
		CString tempStr;
		tempStr.Format("%02X", x);
		mT1SubBank.AddString(tempStr);
		mT1Instrument.AddString(tempStr);
		mT1Sound.AddString(tempStr);
	}

	dontupdateitall = true;
	romName = "";

	ROM = NULL;
	romSize = 0;
	t1results.clear();
	results.clear();
	numberResults = 0;
	alBankCurrent = NULL;
	t1BankCurrent = NULL;
	subBankCurrent = -1;

	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	mainFolder.Format("%s\\", tempFolder);
	InitializeSpecificGames();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CN64SoundListToolDlg::InitializeSpecificGames()
{
	
	char folder[8000];
	strcpy(folder, mainFolder);
	strcat(folder, "gameconfigsound.ini");

	CN64SoundToolReader::InitializeSpecificGames(folder, countGames, gameConfig);

	CMenu* mainMenu = this->GetMenu();
	CMenu *subMenu = mainMenu->GetSubMenu(0);
	
	CMenu tempGameMenu;
	tempGameMenu.CreatePopupMenu();
	int iteratorCount = 25;
	for (int x = 0; x < countGames; x+=iteratorCount)
	{   
		CMenu tempSubMenu1;
		tempSubMenu1.CreatePopupMenu();
		int endCount = iteratorCount;
		if ((x + iteratorCount) >= countGames)
			endCount = (countGames - x);

			CString groupText;
			groupText.Format("%s to %s", gameConfig[x].gameName, gameConfig[(x+(endCount-1))].gameName);
			for (int y = 0; y < endCount; y++)
			{
				tempSubMenu1.AppendMenu( MF_STRING, STARTMENUOPTIONS + x + y, gameConfig[x + y].gameName);
			}
			tempGameMenu.InsertMenu((x / iteratorCount), MF_BYPOSITION | MF_POPUP, (UINT)tempSubMenu1.m_hMenu, groupText);
	}
	subMenu->InsertMenu(0, MF_BYPOSITION | MF_POPUP, (UINT)tempGameMenu.m_hMenu, "Open Known Game");
	
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CN64SoundListToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CN64SoundListToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool CN64SoundListToolDlg::OpenROMFilename(CString filename, bool quiet)
{
	if (ROM != NULL)
	{
		delete [] ROM;
		ROM = NULL;
	}

	romSize = CSharedFunctions::GetSizeFile(filename);
	romName = filename;

	if (romSize == 0)
	{
		if (!quiet)
			MessageBox(LoadResourceText(IDS_STRINGERRORREADINGROM));
		return false;
	}

	FILE* inFile = fopen(filename, "rb");
	if (inFile == NULL)
	{
		romSize = 0;
		if (!quiet)
			MessageBox(LoadResourceText(IDS_STRINGERRORREADINGROM));
		return false;
	}

	ROM = new unsigned char[romSize];
	int result = fread(ROM, 1, romSize, inFile);

	if ((CharArrayToLong(&ROM[0]) == 0x37804012) && (filename.Find(".u2") != -1) && (filename.Find(".u3") != -1) && (filename.Find(".u4") != -1))
	{
		for (int x = 0; x < romSize; x=x+2)
		{
			unsigned char tempSpot = ROM[x];
			ROM[x] = ROM[x+1];
			ROM[x+1] = tempSpot;
		}
	}

	fclose(inFile);

	return true;
}

bool CN64SoundListToolDlg::OpenROMPlusDlg(CString filename)
{
	CFileDialog m_ldFile(TRUE, NULL, filename, OFN_HIDEREADONLY, "N64 ROM(*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin:*.ndd)|*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin;*.ndd|", this);

	int statusFileOpen = (int) m_ldFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return false;

	if (statusFileOpen == FALSE)
		return false;

	if (statusFileOpen != IDOK)
	{
		return false;
	}

	return OpenROMFilename(m_ldFile.GetPathName(), false);
}

bool CN64SoundListToolDlg::OpenROMPlusDlg()
{
	return OpenROMPlusDlg("Game.rom");
}

void CN64SoundListToolDlg::OnFileOpenrom()
{


	if (!OpenROMPlusDlg())
		return;


	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < t1results.size(); x++)
		{
			n64AudioLibrary.DisposeT1Bank(t1results[x].bank);
		}

		t1results.clear();

		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		results.clear();

		alBankCurrent = NULL;
		t1BankCurrent = NULL;
		subBankCurrent = -1;

		numberResults = n64AudioLibrary.ReadCtlTblLocations(ROM, romSize, results);

		for (int x = 0; x < numberResults; x++)
		{
			CString tempStr;
			tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);
			mSoundBankIndex.AddString(tempStr);
		}








		mSoundBankIndex.SetCurSel(0);

		OnCbnSelchangeCombosoundbank();
	}
	catch (char * str)
	{
		CString tempErrStr;
		tempErrStr.Format("%s", str);
		MessageBox(tempErrStr, LoadResourceText(IDS_STRINGERRORPROCESSING));

		numberResults = 0;
	}
}
void CN64SoundListToolDlg::OnClose()
{
	if (gameConfig != NULL)
	{
		for (int x = 0; x < countGames; x++)
		{
			delete[] gameConfig[x].soundBanks;
		}

		delete[] gameConfig;
	}

	CDialog::OnClose();
}

void CN64SoundListToolDlg::UpdateTextBoxes()
{
	if (alBankCurrent != NULL)
	{
		if (dontupdateitall)
		{
			dontupdateitall = false;

			CString tempStr;

			if (percussionMode == NORMAL)
			{
				int instrumentSel = mInstrumentChoice.GetCurSel();

				if (instrumentSel == -1)
					return;
			
				mSamplingRate.GetWindowText(tempStr);
				if (mOverrideRate.GetCheck())
				{
					for (int x = 0; x < results.size(); x++)
					{
						results[x].samplingRate = atoi(tempStr);
					}
				}
				else
				{
					alBankCurrent->samplerate = atoi(tempStr);
				}

				if (alBankCurrent->samplerate != 0)
				{
					if (alBankCurrent->inst[instrumentSel]->samplerate != 0)
					{
						alBankCurrent->inst[instrumentSel]->samplerate = atoi(tempStr);
					}
				}

				mVolume.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->volume = CSharedFunctions::StringToSignedChar(tempStr);
				
				mPan.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->pan = CSharedFunctions::StringToSignedChar(tempStr);

				mPriority.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->priority = CSharedFunctions::StringToSignedChar(tempStr);

				mTremType.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->tremType = CSharedFunctions::StringToSignedChar(tempStr);

				mTremRate.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->tremRate = CSharedFunctions::StringToSignedChar(tempStr);

				mTremDepth.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->tremDepth = CSharedFunctions::StringToSignedChar(tempStr);

				mTremDelay.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->tremDelay = CSharedFunctions::StringToSignedChar(tempStr);

				mVibType.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->vibType = CSharedFunctions::StringToSignedChar(tempStr);

				mVibRate.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->vibRate = CSharedFunctions::StringToSignedChar(tempStr);

				mVibDepth.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->vibDepth = CSharedFunctions::StringToSignedChar(tempStr);

				mVibDelay.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->vibDelay = CSharedFunctions::StringToSignedChar(tempStr);

				mBendRange.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->bendRange = CSharedFunctions::StringToUnsignedShort(tempStr);

				int soundSel = mSoundChoice.GetCurSel();

				if (soundSel == -1)
				{
					dontupdateitall = true;
					return;
				}

				mSamplePan.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->sounds[soundSel]->samplePan = CSharedFunctions::StringToSignedChar(tempStr);

				mSampleVol.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->sounds[soundSel]->sampleVolume = CSharedFunctions::StringToSignedChar(tempStr);
				
				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
				)
				{
					mAttackVolume.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->unknown1 = CSharedFunctions::StringHexToLong(tempStr);

					mAttackTime.GetWindowText(tempStr);
					float unknown3 = atof(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->floatKeyBasePrev = *reinterpret_cast<unsigned long*> (&unknown3);

					mDecayTime.GetWindowText(tempStr);
					unknown3 = atof(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->floatKeyBase = *reinterpret_cast<unsigned long*> (&unknown3);

					mReleaseTime.GetWindowText(tempStr);
					unknown3 = atof(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->floatKeyBaseSec = *reinterpret_cast<unsigned long*> (&unknown3);

				}
				else
				{
					mAttackTime.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.attackTime = CSharedFunctions::StringHexToLong(tempStr);
					mDecayTime.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.decayTime = CSharedFunctions::StringHexToLong(tempStr);
					mReleaseTime.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.releaseTime = CSharedFunctions::StringHexToLong(tempStr);				
				}
				
				
				

				mAttackVolume.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.attackVolume = CSharedFunctions::StringToUnsignedChar(tempStr);

				mDecayVolume.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.decayVolume = CSharedFunctions::StringToUnsignedChar(tempStr);
				
				mVelMin.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.velocitymin = CSharedFunctions::StringToSignedChar(tempStr);

				mVelMax.GetWindowText(tempStr);
				alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.velocitymax = CSharedFunctions::StringToSignedChar(tempStr);

				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
				)
				{
					mSamplePan.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[0] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mSampleVol.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[1] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mVelMin.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[2] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mVelMax.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[3] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyMin.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[4] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyMax.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[5] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyBase.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[6] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mDetune.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->adsrEAD[7] = CSharedFunctions::StringToUnsignedShort(tempStr);
				}
				else if (alBankCurrent->soundBankFormat == PAPERMARIO)
				{
					mSamplePan.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 = (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 & 0x00FFFFFF) | (CSharedFunctions::StringToUnsignedChar(tempStr) << 24);

					mSampleVol.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 = (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 & 0xFF00FFFF) | (CSharedFunctions::StringToUnsignedChar(tempStr) << 16);

					mVelMin.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 = (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 & 0xFFFF00FF) | (CSharedFunctions::StringToUnsignedChar(tempStr) << 8);

					mVelMax.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 = (alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.unknown1 & 0xFFFFFF00) | (CSharedFunctions::StringToUnsignedChar(tempStr));
				}
				else
				{
					mKeyMin.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.keymin = CSharedFunctions::StringToSignedChar(tempStr);

					mKeyMax.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.keymax = CSharedFunctions::StringToSignedChar(tempStr);

					mKeyBase.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.keybase = CSharedFunctions::StringToSignedChar(tempStr);

					mDetune.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.detune = CSharedFunctions::StringToSignedChar(tempStr);
				}

				
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				ALWave* alWave = NULL;
				if (primSel == PRIMARY)
				{
					alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav;
				}
				else if (primSel == PREVIOUS)
				{
					alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavPrevious;
				}
				else if (primSel == SECONDARY)
				{
					alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavSecondary;
				}

				if (alWave->type == AL_ADPCM_WAVE)
				{
					bool loopEnabled = mLoopingEnabled.GetCheck();

					if (loopEnabled)
					{
						if (alWave->adpcmWave->loop == NULL)
						{
							alWave->adpcmWave->loop = new ALADPCMloop();

							// TODO make real, better predictors, from looping point
							for (int x = 0; x < 16; x++)
							{
								alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];
							}
						}

						
					}
					else
					{
						if (alWave->adpcmWave->loop != NULL)
						{
							delete alWave->adpcmWave->loop;
							alWave->adpcmWave->loop = NULL;
						}
					}

					if (alWave->adpcmWave->loop != NULL)
					{
						mLoopStart.GetWindowText(tempStr);
						alWave->adpcmWave->loop->start = CSharedFunctions::StringHexToLong(tempStr);

						mLoopEnd.GetWindowText(tempStr);
						alWave->adpcmWave->loop->end = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.GetWindowText(tempStr);
						alWave->adpcmWave->loop->count = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);

						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						if (m_importGroupBox.IsWindowVisible())
						{
							mImportLoopPredictors.ShowWindow(SW_SHOW);
						}
						mExportLoopPredictors.ShowWindow(SW_SHOW);
					}
					else
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
				}
				else if (alWave->type == AL_RAW16_WAVE)
				{
					bool loopEnabled = mLoopingEnabled.GetCheck();

					if (loopEnabled)
					{
						if (alWave->rawWave->loop == NULL)
							alWave->rawWave->loop = new ALRawLoop();
					}
					else
					{
						if (alWave->rawWave->loop != NULL)
						{
							delete alWave->rawWave->loop;
							alWave->rawWave->loop = NULL;
						}
					}

					if (alWave->rawWave->loop != NULL)
					{
						mLoopStart.GetWindowText(tempStr);
						alWave->rawWave->loop->start = CSharedFunctions::StringHexToLong(tempStr);

						mLoopEnd.GetWindowText(tempStr);
						alWave->rawWave->loop->end = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.GetWindowText(tempStr);
						alWave->rawWave->loop->count = CSharedFunctions::StringHexToLong(tempStr);
						
						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);

						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
				}
			}
			else if (percussionMode == PERCUSSION)
			{
				mSamplingRate.GetWindowText(tempStr);

				if (mOverrideRate.GetCheck())
				{
					for (int x = 0; x < results.size(); x++)
					{
						results[x].samplingRate = atoi(tempStr);
					}
				}
				else
				{
					alBankCurrent->samplerate = atoi(tempStr);

					if (alBankCurrent->samplerate != 0)
					{
						if (alBankCurrent->percussion->samplerate != 0)
						{
							alBankCurrent->percussion->samplerate = atoi(tempStr);
						}
					}
				}

				mVolume.GetWindowText(tempStr);
				alBankCurrent->percussion->volume = CSharedFunctions::StringToSignedChar(tempStr);
				
				mPan.GetWindowText(tempStr);
				alBankCurrent->percussion->pan = CSharedFunctions::StringToSignedChar(tempStr);

				mPriority.GetWindowText(tempStr);
				alBankCurrent->percussion->priority = CSharedFunctions::StringToSignedChar(tempStr);

				mTremType.GetWindowText(tempStr);
				alBankCurrent->percussion->tremType = CSharedFunctions::StringToSignedChar(tempStr);

				mTremRate.GetWindowText(tempStr);
				alBankCurrent->percussion->tremRate = CSharedFunctions::StringToSignedChar(tempStr);

				mTremDepth.GetWindowText(tempStr);
				alBankCurrent->percussion->tremDepth = CSharedFunctions::StringToSignedChar(tempStr);

				mTremDelay.GetWindowText(tempStr);
				alBankCurrent->percussion->tremDelay = CSharedFunctions::StringToSignedChar(tempStr);

				mVibType.GetWindowText(tempStr);
				alBankCurrent->percussion->vibType = CSharedFunctions::StringToSignedChar(tempStr);

				mVibRate.GetWindowText(tempStr);
				alBankCurrent->percussion->vibRate = CSharedFunctions::StringToSignedChar(tempStr);

				mVibDepth.GetWindowText(tempStr);
				alBankCurrent->percussion->vibDepth = CSharedFunctions::StringToSignedChar(tempStr);

				mVibDelay.GetWindowText(tempStr);
				alBankCurrent->percussion->vibDelay = CSharedFunctions::StringToSignedChar(tempStr);

				mBendRange.GetWindowText(tempStr);
				alBankCurrent->percussion->bendRange = CSharedFunctions::StringToUnsignedShort(tempStr);

				int soundSel = mSoundChoice.GetCurSel();

				if (soundSel == -1)
				{
					dontupdateitall = true;
					return;
				}

				mSamplePan.GetWindowText(tempStr);
				alBankCurrent->percussion->sounds[soundSel]->samplePan = CSharedFunctions::StringToSignedChar(tempStr);

				mSampleVol.GetWindowText(tempStr);
				alBankCurrent->percussion->sounds[soundSel]->sampleVolume = CSharedFunctions::StringToSignedChar(tempStr);
				
				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
				)
				{
					mAttackVolume.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->unknown1 = CSharedFunctions::StringHexToLong(tempStr);

					mAttackTime.GetWindowText(tempStr);
					float unknown3 = atof(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->floatKeyBasePrev = *reinterpret_cast<unsigned long*> (&unknown3);

					mDecayTime.GetWindowText(tempStr);
					unknown3 = atof(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->floatKeyBase = *reinterpret_cast<unsigned long*> (&unknown3);

					mReleaseTime.GetWindowText(tempStr);
					unknown3 = atof(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->floatKeyBaseSec = *reinterpret_cast<unsigned long*> (&unknown3);

				}
				else
				{
					mAttackTime.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->env.attackTime = CSharedFunctions::StringHexToLong(tempStr);
					mDecayTime.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->env.decayTime = CSharedFunctions::StringHexToLong(tempStr);
					mReleaseTime.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->env.releaseTime = CSharedFunctions::StringHexToLong(tempStr);				
				}
				
				
				

				mAttackVolume.GetWindowText(tempStr);
				alBankCurrent->percussion->sounds[soundSel]->env.attackVolume = CSharedFunctions::StringToUnsignedChar(tempStr);

				mDecayVolume.GetWindowText(tempStr);
				alBankCurrent->percussion->sounds[soundSel]->env.decayVolume = CSharedFunctions::StringToUnsignedChar(tempStr);
				
				mVelMin.GetWindowText(tempStr);
				alBankCurrent->percussion->sounds[soundSel]->key.velocitymin = CSharedFunctions::StringToSignedChar(tempStr);

				mVelMax.GetWindowText(tempStr);
				alBankCurrent->percussion->sounds[soundSel]->key.velocitymax = CSharedFunctions::StringToSignedChar(tempStr);

				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
				)
				{
					mSamplePan.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[0] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mSampleVol.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[1] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mVelMin.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[2] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mVelMax.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[3] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyMin.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[4] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyMax.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[5] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyBase.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[6] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mDetune.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->adsrEAD[7] = CSharedFunctions::StringToUnsignedShort(tempStr);
				}
				else
				{
					mKeyMin.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->key.keymin = CSharedFunctions::StringToSignedChar(tempStr);

					mKeyMax.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->key.keymax = CSharedFunctions::StringToSignedChar(tempStr);

					mKeyBase.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->key.keybase = CSharedFunctions::StringToSignedChar(tempStr);

					mDetune.GetWindowText(tempStr);
					alBankCurrent->percussion->sounds[soundSel]->key.detune = CSharedFunctions::StringToSignedChar(tempStr);
				}

				
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				ALWave* alWave = NULL;
				if (primSel == PRIMARY)
				{
					alWave = &alBankCurrent->percussion->sounds[soundSel]->wav;
				}
				else if (primSel == PREVIOUS)
				{
					alWave = &alBankCurrent->percussion->sounds[soundSel]->wavPrevious;
				}
				else if (primSel == SECONDARY)
				{
					alWave = &alBankCurrent->percussion->sounds[soundSel]->wavSecondary;
				}

				if (alWave->type == AL_ADPCM_WAVE)
				{
					bool loopEnabled = mLoopingEnabled.GetCheck();

					if (loopEnabled)
					{
						if (alWave->adpcmWave->loop == NULL)
						{
							alWave->adpcmWave->loop = new ALADPCMloop();

							// TODO make real, better predictors, from looping point
							for (int x = 0; x < 16; x++)
							{
								alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];
							}
						}

						
					}
					else
					{
						if (alWave->adpcmWave->loop != NULL)
						{
							delete alWave->adpcmWave->loop;
							alWave->adpcmWave->loop = NULL;
						}
					}

					if (alWave->adpcmWave->loop != NULL)
					{
						mLoopStart.GetWindowText(tempStr);
						alWave->adpcmWave->loop->start = CSharedFunctions::StringHexToLong(tempStr);

						mLoopEnd.GetWindowText(tempStr);
						alWave->adpcmWave->loop->end = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.GetWindowText(tempStr);
						alWave->adpcmWave->loop->count = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);

						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						if (m_importGroupBox.IsWindowVisible())
						{
							mImportLoopPredictors.ShowWindow(SW_SHOW);
						}
						mExportLoopPredictors.ShowWindow(SW_SHOW);
					}
					else
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
				}
				else if (alWave->type == AL_RAW16_WAVE)
				{
					bool loopEnabled = mLoopingEnabled.GetCheck();

					if (loopEnabled)
					{
						if (alWave->rawWave->loop == NULL)
							alWave->rawWave->loop = new ALRawLoop();
					}
					else
					{
						if (alWave->rawWave->loop != NULL)
						{
							delete alWave->rawWave->loop;
							alWave->rawWave->loop = NULL;
						}
					}

					if (alWave->rawWave->loop != NULL)
					{
						mLoopStart.GetWindowText(tempStr);
						alWave->rawWave->loop->start = CSharedFunctions::StringHexToLong(tempStr);

						mLoopEnd.GetWindowText(tempStr);
						alWave->rawWave->loop->end = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.GetWindowText(tempStr);
						alWave->rawWave->loop->count = CSharedFunctions::StringHexToLong(tempStr);
						
						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);

						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
				}
			}
			else if (percussionMode == EADPERCUSSION)
			{
				int percussionSel = mPercussionChoice.GetCurSel();

				if (percussionSel == -1)
					return;

				ALWave* alWave = &alBankCurrent->eadPercussion[percussionSel].wav;

				mReleaseTime.GetWindowText(tempStr);
				float keyBase = atof(tempStr);
				alBankCurrent->eadPercussion[percussionSel].keyBase = *reinterpret_cast<unsigned long*> (&keyBase);

				mAttackVolume.GetWindowText(tempStr);
				alBankCurrent->eadPercussion[percussionSel].pan = CSharedFunctions::StringToUnsignedChar(tempStr);

				if (alWave->type == AL_ADPCM_WAVE)
				{
					bool loopEnabled = mLoopingEnabled.GetCheck();

					if (loopEnabled)
					{
						if (alWave->adpcmWave->loop == NULL)
						{
							alWave->adpcmWave->loop = new ALADPCMloop();

							// TODO make real, better predictors, from looping point
							for (int x = 0; x < 16; x++)
							{
								alWave->adpcmWave->loop->state[x] = alWave->adpcmWave->book->predictors[x];
							}
						}

						
					}
					else
					{
						if (alWave->adpcmWave->loop != NULL)
						{
							delete alWave->adpcmWave->loop;
							alWave->adpcmWave->loop = NULL;
						}
					}

					if (alWave->adpcmWave->loop != NULL)
					{
						mLoopStart.GetWindowText(tempStr);
						alWave->adpcmWave->loop->start = CSharedFunctions::StringHexToLong(tempStr);

						mLoopEnd.GetWindowText(tempStr);
						alWave->adpcmWave->loop->end = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.GetWindowText(tempStr);
						alWave->adpcmWave->loop->count = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);

						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						if (m_importGroupBox.IsWindowVisible())
						{
							mImportLoopPredictors.ShowWindow(SW_SHOW);
						}

						mExportLoopPredictors.ShowWindow(SW_SHOW);
					}
					else
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
				}
				else if (alWave->type == AL_RAW16_WAVE)
				{
					bool loopEnabled = mLoopingEnabled.GetCheck();

					if (loopEnabled)
					{
						if (alWave->rawWave->loop == NULL)
							alWave->rawWave->loop = new ALRawLoop();
					}
					else
					{
						if (alWave->rawWave->loop != NULL)
						{
							delete alWave->rawWave->loop;
							alWave->rawWave->loop = NULL;
						}
					}

					if (alWave->rawWave->loop != NULL)
					{
						mLoopStart.GetWindowText(tempStr);
						alWave->rawWave->loop->start = CSharedFunctions::StringHexToLong(tempStr);

						mLoopEnd.GetWindowText(tempStr);
						alWave->rawWave->loop->end = CSharedFunctions::StringHexToLong(tempStr);

						mLoopCount.GetWindowText(tempStr);
						alWave->rawWave->loop->count = CSharedFunctions::StringHexToLong(tempStr);
						
						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);

						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
				}

				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
				)
				{
					mSamplePan.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[0] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mSampleVol.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[1] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mVelMin.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[2] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mVelMax.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[3] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyMin.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[4] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyMax.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[5] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mKeyBase.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[6] = CSharedFunctions::StringToUnsignedShort(tempStr);

					mDetune.GetWindowText(tempStr);
					alBankCurrent->eadPercussion[percussionSel].adsrEAD[7] = CSharedFunctions::StringToUnsignedShort(tempStr);
				}
			}
			else if (percussionMode == SFX)
			{
				int sfxSel = mSfxChoice.GetCurSel();

				if (sfxSel == -1)
					return;

				mReleaseTime.GetWindowText(tempStr);
				float keyBase = atof(tempStr);
				alBankCurrent->alSfx[sfxSel]->unknown1 = *reinterpret_cast<unsigned long*> (&keyBase);
			}

			UpdateSamplingRateKeyBaseList();

			dontupdateitall = true;
		}
	}
}

void CN64SoundListToolDlg::UpdateSamplingRateSelection()
{
	mKeyBaseListCtrl.EnsureVisible(mKeyBaseListCtrl.GetItemCount()-1, false); // Scroll down to the bottom

	int topIndex;

	CString tempStr;
	if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
					|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
		)
	{
		mDecayTime.GetWindowText(tempStr);
		topIndex = n64AudioLibrary.ConvertEADGameValueToKeyBase(atof(tempStr));
	}
	else
	{
		mKeyBase.GetWindowText(tempStr);
		topIndex = CSharedFunctions::StringToSignedChar(tempStr);
	}

	if ((topIndex >= 0) && (topIndex < 0x80))
		mKeyBaseListCtrl.EnsureVisible(topIndex, false); // scroll back up just enough to show said item on top
}

void CN64SoundListToolDlg::UpdateSamplingRateKeyBaseList()
{

	CString tempStr;
	mSamplingRate.GetWindowText(tempStr);
	int samplingRate = atoi(tempStr);

	CRect rect;
	mKeyBaseListCtrl.GetClientRect(&rect);
	int nColInterval = rect.Width()/5;

	mKeyBaseListCtrl.DeleteAllItems();
	mKeyBaseListCtrl.DeleteColumn(1);
	mKeyBaseListCtrl.DeleteColumn(0);

	mKeyBaseListCtrl.InsertColumn(0, _T("Key"), LVCFMT_LEFT, nColInterval);
	mKeyBaseListCtrl.InsertColumn(1, _T("Sampling Rate"), LVCFMT_LEFT, (int)(float)nColInterval*3);

	LVITEM lvi;
	CString strItem;
	for (int i = 0; i < 0x7F; i++)
	{
	// Insert the first item
		lvi.mask =  LVIF_TEXT;
		strItem.Format(_T("%02X"), i);
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		mKeyBaseListCtrl.InsertItem(&lvi);
	// Set subitem 1
		strItem.Format(_T("%d"), (int)((float)samplingRate / CN64AIFCAudio::keyTable[i]));
		lvi.iSubItem =1;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		mKeyBaseListCtrl.SetItem(&lvi);
	}

	UpdateSamplingRateSelection();	
}

void CN64SoundListToolDlg::ShowOfficialSDKImport()
{
	CString aifcToolsDir = mainFolder;
	CString soxDir = mainFolder + "SOX.EXE";
	CString tabledesignDir = mainFolder + "tabledesign.exe";
	CString adpcmencDir = mainFolder + "ADPCMENC.EXE";

	if (!IsFileExist(soxDir.GetBuffer(0)))
	{
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		return;
	}

	if (!IsFileExist(tabledesignDir.GetBuffer(0)))
	{
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		return;
	}

	if (!IsFileExist(adpcmencDir.GetBuffer(0)))
	{
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		return;
	}

	mImportFromSDKTools.ShowWindow(SW_SHOW);
}

void CN64SoundListToolDlg::ShowSoundBankControls()
{
	mAddPrevButton.ShowWindow(SW_HIDE);
	mRemovePrevButton.ShowWindow(SW_HIDE);
	mAddSecButton.ShowWindow(SW_HIDE);
	mRemoveSecButton.ShowWindow(SW_HIDE);

	mSoundChoice.ShowWindow(SW_SHOW);
	mUpArrowButton.ShowWindow(SW_SHOW);
	mDownArrowButton.ShowWindow(SW_SHOW);

	m_sampleAttackTimeStatic.SetWindowText("Attack Time");
	m_sampleDecayTimeStatic.SetWindowText("Decay Time");
	m_sampleReleaseTimeStatic.SetWindowText("Release Time");
	m_sampleAttackVolumeStatic.SetWindowText("Attack Volume");

	m_samplePanStatic.SetWindowText("Sample Pan");
	m_sampleVolStatic.SetWindowText("Sample Vol");
	m_sampleVelMinStatic.SetWindowText("Vel Min");
	m_sampleVelMaxStatic.SetWindowText("Vel Max");
	m_sampleKeyMinStatic.SetWindowText("Key Min");
	m_sampleKeyMaxStatic.SetWindowText("Key Max");
	m_sampleKeyBaseStatic.SetWindowText("Key Base");
	m_sampleDetuneStatic.SetWindowText("Detune");

	m_instrVolStatic.SetWindowText("Vol");
	m_instrPanStatic.SetWindowText("Pan");

	if (alBankCurrent->soundBankFormat == STANDARDFORMAT)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_SHOW);
		mAddPercussionBank.ShowWindow(SW_SHOW);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == KONAMICTL)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == KONAMI8CTL)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == MARIOPARTY2E)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_SHOW);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_SHOW);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_SHOW);
		mAddPercussionBank.ShowWindow(SW_SHOW);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_SHOW);
		mCheckUnknownEADFlag.ShowWindow(SW_SHOW);
		mLabelTableIndex.ShowWindow(SW_SHOW);
		mTableIndex.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.SetWindowText("Key Prev");
		m_sampleDecayTimeStatic.SetWindowText("Key");
		m_sampleReleaseTimeStatic.SetWindowText("Key Sec");
		m_sampleAttackVolumeStatic.SetWindowText("Splits/RelTime");

		m_samplePanStatic.SetWindowText("ADSR 1");
		m_sampleVolStatic.SetWindowText("ADSR 2");
		m_sampleVelMinStatic.SetWindowText("ADSR 3");
		m_sampleVelMaxStatic.SetWindowText("ADSR 4");
		m_sampleKeyMinStatic.SetWindowText("ADSR 5");
		m_sampleKeyMaxStatic.SetWindowText("ADSR 6");
		m_sampleKeyBaseStatic.SetWindowText("ADSR 7");
		m_sampleDetuneStatic.SetWindowText("ADSR 8");

		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_SHOW);
		mAddPercussionBank.ShowWindow(SW_SHOW);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == N64DD)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == BANJOTOOIEAUDIO)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == MUSYX)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if ((alBankCurrent->soundBankFormat == MUSYXLZ) || (alBankCurrent->soundBankFormat == MUSYXZLIB) || (alBankCurrent->soundBankFormat == MUSYXSMALLZLIB))
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == BLASTCORPSZLBSTANDARD)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == CONKERFORMAT)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV1)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if ((alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2) || (alBankCurrent->soundBankFormat == RNCCOMPRESSEDN64PTR)
		|| (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0))
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == TAZHUFFMAN)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == SNOW)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == MEGAMAN64PTRV2)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == ARMYMENFORMAT)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == TITUS)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == SYDNEY)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == NINDEC)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_SHOW);
		mAddPercussionBank.ShowWindow(SW_SHOW);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == MKMYTHOLOGIES)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEQUAKE2)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLESNOWBOARDKIDS)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLEVIEWPOINT)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if ((alBankCurrent->soundBankFormat == H20RAW816) || (alBankCurrent->soundBankFormat == TETRISPHERERAW816) || (alBankCurrent->soundBankFormat == RAW16BITPCM) || (alBankCurrent->soundBankFormat == RAW8BITPCM))
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == BNKB)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == SEPARATEDBNKB)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == MADDENBNKB)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_SHOW);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == MADDEN64)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_SHOW);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == PAPERMARIO)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);

		m_samplePanStatic.SetWindowText("?");
		m_sampleVolStatic.SetWindowText("?");
		m_sampleVelMinStatic.SetWindowText("Coarse Tune");
		m_sampleVelMaxStatic.SetWindowText("Fine Tune");
	}
	else if (alBankCurrent->soundBankFormat == B0)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == AVL_0PTR)
	{
		m_instrVolStatic.SetWindowText("C Tune");
		m_instrPanStatic.SetWindowText("F Tune");

		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == DUCKDODGERS)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == VOX)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == WAV)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == MORT)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_SHOW);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == KOBE2)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == NAMCOMUSEUM)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == TOPGEARHYPERBIKE)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == TOPGEAROVERDRIVE)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == MP3)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_HIDE);
		m_playLoopedButton.ShowWindow(SW_HIDE);
		m_stopButton.ShowWindow(SW_HIDE);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if ((alBankCurrent->soundBankFormat == EXCITEBIKESAM) || (alBankCurrent->soundBankFormat == EXCITEBIKESFX) || (alBankCurrent->soundBankFormat == EXCITEBIKESNG))
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if ((alBankCurrent->soundBankFormat == WDCSFX) || (alBankCurrent->soundBankFormat == WDCINSTRUMENTS))
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == RNCDEAN)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == DEANTABLE)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == DEANINDIVIDUAL)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == SOUTHPARKRALLY)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == TWISTEDEDGESNOWBOARDING)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == TWISTEDEDGEMUSIC)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == FIGHTINGFORCE)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == ZLIBSN64)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == SN64)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_HIDE);
		mDecayTime.ShowWindow(SW_HIDE);
		mReleaseTime.ShowWindow(SW_HIDE);
		mAttackVolume.ShowWindow(SW_HIDE);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_HIDE);
		m_sampleVolStatic.ShowWindow(SW_HIDE);
		m_sampleVelMinStatic.ShowWindow(SW_HIDE);
		m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
		m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
		m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
		m_sampleDetuneStatic.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
		m_sampleReleaseTimeStatic.ShowWindow(SW_HIDE);
		m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_SHOW);
		mCheckUnknownEADFlag.ShowWindow(SW_SHOW);
		mLabelTableIndex.ShowWindow(SW_SHOW);
		mTableIndex.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.SetWindowText("Key Prev");
		m_sampleDecayTimeStatic.SetWindowText("Key");
		m_sampleReleaseTimeStatic.SetWindowText("Key Sec");
		m_sampleAttackVolumeStatic.SetWindowText("Splits/RelTime");

		m_samplePanStatic.SetWindowText("ADSR 1");
		m_sampleVolStatic.SetWindowText("ADSR 2");
		m_sampleVelMinStatic.SetWindowText("ADSR 3");
		m_sampleVelMaxStatic.SetWindowText("ADSR 4");
		m_sampleKeyMinStatic.SetWindowText("ADSR 5");
		m_sampleKeyMaxStatic.SetWindowText("ADSR 6");
		m_sampleKeyBaseStatic.SetWindowText("ADSR 7");
		m_sampleDetuneStatic.SetWindowText("ADSR 8");

		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_SHOW);
		mAddPercussionBank.ShowWindow(SW_SHOW);
		mDeleteSfxBank.ShowWindow(SW_SHOW);
		mAddSfxBank.ShowWindow(SW_SHOW);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == TUROKFORMAT)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == CLAYFIGHTER)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		mAddInstrumentButton.ShowWindow(SW_HIDE);
		mDeleteInstrumentButton.ShowWindow(SW_HIDE);
		mDeletePercussionBank.ShowWindow(SW_HIDE);
		mAddPercussionBank.ShowWindow(SW_HIDE);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_SHOW);
		m_vibrGroupBox.ShowWindow(SW_SHOW);
		mVolume.ShowWindow(SW_SHOW);
		mPan.ShowWindow(SW_SHOW);
		mPriority.ShowWindow(SW_SHOW);
		mTremType.ShowWindow(SW_SHOW);
		mTremRate.ShowWindow(SW_SHOW);
		mTremDepth.ShowWindow(SW_SHOW);
		mTremDelay.ShowWindow(SW_SHOW);
		mVibType.ShowWindow(SW_SHOW);
		mVibRate.ShowWindow(SW_SHOW);
		mVibDepth.ShowWindow(SW_SHOW);
		mVibDelay.ShowWindow(SW_SHOW);
		mBendRange.ShowWindow(SW_SHOW);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_SHOW);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_SHOW);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_SHOW);
		m_instrPanStatic.ShowWindow(SW_SHOW);
		m_InstrPriorityStatic.ShowWindow(SW_SHOW);
		m_instrTremTypeStatic.ShowWindow(SW_SHOW);
		m_instrTremRateStatic.ShowWindow(SW_SHOW);
		m_instrTremDepthStatic.ShowWindow(SW_SHOW);
		m_instrTremDelayStatic.ShowWindow(SW_SHOW);
		m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
		m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
		m_instrVibrRateStatic.ShowWindow(SW_SHOW);
		m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
		m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
	{
		mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
		mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
		mLabelTableIndex.ShowWindow(SW_HIDE);
		mTableIndex.ShowWindow(SW_HIDE);
		m_sampleAttackTimeStatic.SetWindowText("Key Prev");
		m_sampleDecayTimeStatic.SetWindowText("Key");
		m_sampleReleaseTimeStatic.SetWindowText("Key Sec");
		m_sampleAttackVolumeStatic.SetWindowText("Splits/RelTime");

		m_samplePanStatic.SetWindowText("ADSR 1");
		m_sampleVolStatic.SetWindowText("ADSR 2");
		m_sampleVelMinStatic.SetWindowText("ADSR 3");
		m_sampleVelMaxStatic.SetWindowText("ADSR 4");
		m_sampleKeyMinStatic.SetWindowText("ADSR 5");
		m_sampleKeyMaxStatic.SetWindowText("ADSR 6");
		m_sampleKeyBaseStatic.SetWindowText("ADSR 7");
		m_sampleDetuneStatic.SetWindowText("ADSR 8");

		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_playLoopedButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		mAddInstrumentButton.ShowWindow(SW_SHOW);
		mDeleteInstrumentButton.ShowWindow(SW_SHOW);
		mDeletePercussionBank.ShowWindow(SW_SHOW);
		mAddPercussionBank.ShowWindow(SW_SHOW);
		mDeleteSfxBank.ShowWindow(SW_HIDE);
		mAddSfxBank.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_HIDE);
		mPan.ShowWindow(SW_HIDE);
		mPriority.ShowWindow(SW_HIDE);
		mTremType.ShowWindow(SW_HIDE);
		mTremRate.ShowWindow(SW_HIDE);
		mTremDepth.ShowWindow(SW_HIDE);
		mTremDelay.ShowWindow(SW_HIDE);
		mVibType.ShowWindow(SW_HIDE);
		mVibRate.ShowWindow(SW_HIDE);
		mVibDepth.ShowWindow(SW_HIDE);
		mVibDelay.ShowWindow(SW_HIDE);
		mBendRange.ShowWindow(SW_HIDE);
		mSamplePan.ShowWindow(SW_SHOW);
		mSampleVol.ShowWindow(SW_SHOW);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_SHOW);
		mVelMax.ShowWindow(SW_SHOW);
		mKeyMin.ShowWindow(SW_SHOW);
		mKeyMax.ShowWindow(SW_SHOW);
		mKeyBase.ShowWindow(SW_SHOW);
		mKeyBaseListCtrl.ShowWindow(SW_HIDE);
		mDetune.ShowWindow(SW_SHOW);
		m_instrVolStatic.ShowWindow(SW_HIDE);
		m_instrPanStatic.ShowWindow(SW_HIDE);
		m_InstrPriorityStatic.ShowWindow(SW_HIDE);
		m_instrTremTypeStatic.ShowWindow(SW_HIDE);
		m_instrTremRateStatic.ShowWindow(SW_HIDE);
		m_instrTremDepthStatic.ShowWindow(SW_HIDE);
		m_instrTremDelayStatic.ShowWindow(SW_HIDE);
		m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
		m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
		m_instrVibrRateStatic.ShowWindow(SW_HIDE);
		m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
		m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
		m_samplePanStatic.ShowWindow(SW_SHOW);
		m_sampleVolStatic.ShowWindow(SW_SHOW);
		m_sampleVelMinStatic.ShowWindow(SW_SHOW);
		m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
		m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
		m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
		m_sampleDetuneStatic.ShowWindow(SW_SHOW);
		m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
		m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
		m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
}

void CN64SoundListToolDlg::ShowPercussionControls()
{
	mAddPrevButton.ShowWindow(SW_HIDE);
	mRemovePrevButton.ShowWindow(SW_HIDE);
	mAddSecButton.ShowWindow(SW_HIDE);
	mRemoveSecButton.ShowWindow(SW_HIDE);

	mSoundChoice.ShowWindow(SW_SHOW);
	mUpArrowButton.ShowWindow(SW_SHOW);
	mDownArrowButton.ShowWindow(SW_SHOW);

	mHalfVADPCMPrecision.ShowWindow(SW_HIDE);
	mCheckUnknownEADFlag.ShowWindow(SW_HIDE);
	mLabelTableIndex.ShowWindow(SW_HIDE);
	mTableIndex.ShowWindow(SW_HIDE);

	m_sampleAttackTimeStatic.SetWindowText("Attack Time");
	m_sampleDecayTimeStatic.SetWindowText("Decay Time");
	m_sampleReleaseTimeStatic.SetWindowText("Release Time");
	m_sampleAttackVolumeStatic.SetWindowText("Attack Volume");

	m_samplePanStatic.SetWindowText("Sample Pan");
	m_sampleVolStatic.SetWindowText("Sample Vol");
	m_sampleVelMinStatic.SetWindowText("Vel Min");
	m_sampleVelMaxStatic.SetWindowText("Vel Max");
	m_sampleKeyMinStatic.SetWindowText("Key Min");
	m_sampleKeyMaxStatic.SetWindowText("Key Max");
	m_sampleKeyBaseStatic.SetWindowText("Key Base");
	m_sampleDetuneStatic.SetWindowText("Detune");

	m_injectInPlaceButton.ShowWindow(SW_SHOW);
	m_playButton.ShowWindow(SW_SHOW);
	m_playLoopedButton.ShowWindow(SW_SHOW);
	m_stopButton.ShowWindow(SW_SHOW);
	m_saveButton.ShowWindow(SW_SHOW);
	m_import16BitRaw.ShowWindow(SW_HIDE);
	m_import16BitADPCM.ShowWindow(SW_SHOW);
	m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
	m_import16BitRawValues.ShowWindow(SW_SHOW);
	m_importPredictors.ShowWindow(SW_SHOW);
	ShowOfficialSDKImport();
	m_add16BitRaw.ShowWindow(SW_HIDE);
	m_add16BitADPCM.ShowWindow(SW_SHOW);
	m_importGroupBox.ShowWindow(SW_SHOW);
	m_addToEndGroupBox.ShowWindow(SW_SHOW);
	m_deleteButton.ShowWindow(SW_SHOW);
	m_miscGroupBox.ShowWindow(SW_SHOW);
	m_tremGroupBox.ShowWindow(SW_SHOW);
	m_vibrGroupBox.ShowWindow(SW_SHOW);
	mVolume.ShowWindow(SW_SHOW);
	mPan.ShowWindow(SW_SHOW);
	mPriority.ShowWindow(SW_SHOW);
	mTremType.ShowWindow(SW_SHOW);
	mTremRate.ShowWindow(SW_SHOW);
	mTremDepth.ShowWindow(SW_SHOW);
	mTremDelay.ShowWindow(SW_SHOW);
	mVibType.ShowWindow(SW_SHOW);
	mVibRate.ShowWindow(SW_SHOW);
	mVibDepth.ShowWindow(SW_SHOW);
	mVibDelay.ShowWindow(SW_SHOW);
	mBendRange.ShowWindow(SW_SHOW);
	mSamplePan.ShowWindow(SW_SHOW);
	mSampleVol.ShowWindow(SW_SHOW);
	mAttackTime.ShowWindow(SW_SHOW);
	mDecayTime.ShowWindow(SW_SHOW);
	mReleaseTime.ShowWindow(SW_SHOW);
	mAttackVolume.ShowWindow(SW_SHOW);
	mDecayVolume.ShowWindow(SW_SHOW);
	mVelMin.ShowWindow(SW_SHOW);
	mVelMax.ShowWindow(SW_SHOW);
	mKeyMin.ShowWindow(SW_SHOW);
	mKeyMax.ShowWindow(SW_SHOW);
	mKeyBase.ShowWindow(SW_SHOW);
	mKeyBaseListCtrl.ShowWindow(SW_SHOW);
	mDetune.ShowWindow(SW_SHOW);
	m_instrVolStatic.ShowWindow(SW_SHOW);
	m_instrPanStatic.ShowWindow(SW_SHOW);
	m_InstrPriorityStatic.ShowWindow(SW_SHOW);
	m_instrTremTypeStatic.ShowWindow(SW_SHOW);
	m_instrTremRateStatic.ShowWindow(SW_SHOW);
	m_instrTremDepthStatic.ShowWindow(SW_SHOW);
	m_instrTremDelayStatic.ShowWindow(SW_SHOW);
	m_instrVibrTypeStatic.ShowWindow(SW_SHOW);
	m_InstrBendRangeStatic.ShowWindow(SW_SHOW);
	m_instrVibrRateStatic.ShowWindow(SW_SHOW);
	m_instrVibrDepthStatic.ShowWindow(SW_SHOW);
	m_instrVibrDelayStatic.ShowWindow(SW_SHOW);
	m_samplePanStatic.ShowWindow(SW_SHOW);
	m_sampleVolStatic.ShowWindow(SW_SHOW);
	m_sampleVelMinStatic.ShowWindow(SW_SHOW);
	m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
	m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
	m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
	m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
	m_sampleDetuneStatic.ShowWindow(SW_SHOW);
	m_sampleAttackTimeStatic.ShowWindow(SW_SHOW);
	m_sampleDecayTimeStatic.ShowWindow(SW_SHOW);
	m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
	m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
	m_sampleDecayVolumeStatic.ShowWindow(SW_SHOW);
}

void CN64SoundListToolDlg::ShowEADPercussionControls()
{
	mSubSound.ShowWindow(SW_HIDE);
	mAddPrevButton.ShowWindow(SW_HIDE);
	mRemovePrevButton.ShowWindow(SW_HIDE);
	mAddSecButton.ShowWindow(SW_HIDE);
	mRemoveSecButton.ShowWindow(SW_HIDE);

	mSoundChoice.ShowWindow(SW_HIDE);
	mUpArrowButton.ShowWindow(SW_HIDE);
	mDownArrowButton.ShowWindow(SW_HIDE);

	mHalfVADPCMPrecision.ShowWindow(SW_SHOW);
	mCheckUnknownEADFlag.ShowWindow(SW_SHOW);
	mLabelTableIndex.ShowWindow(SW_SHOW);
	mTableIndex.ShowWindow(SW_SHOW);

	if (alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_importGroupBox.ShowWindow(SW_SHOW);
		mImportLoopPredictors.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_importGroupBox.ShowWindow(SW_SHOW);
		mImportLoopPredictors.ShowWindow(SW_SHOW);
	}
	else if (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		ShowOfficialSDKImport();
		m_importGroupBox.ShowWindow(SW_SHOW);
		mImportLoopPredictors.ShowWindow(SW_SHOW);
	}
	else
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitADPCMSamePred.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		mImportFromSDKTools.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		mImportLoopPredictors.ShowWindow(SW_HIDE);
	}

	m_playButton.ShowWindow(SW_SHOW);
	m_playLoopedButton.ShowWindow(SW_SHOW);
	m_stopButton.ShowWindow(SW_SHOW);
	m_saveButton.ShowWindow(SW_SHOW);
	m_import16BitRaw.ShowWindow(SW_HIDE);
	m_add16BitRaw.ShowWindow(SW_HIDE);
	m_add16BitADPCM.ShowWindow(SW_HIDE);
	m_addToEndGroupBox.ShowWindow(SW_HIDE);
	m_deleteButton.ShowWindow(SW_HIDE);
	m_miscGroupBox.ShowWindow(SW_HIDE);
	m_tremGroupBox.ShowWindow(SW_HIDE);
	m_vibrGroupBox.ShowWindow(SW_HIDE);
	mVolume.ShowWindow(SW_HIDE);
	mPan.ShowWindow(SW_HIDE);
	mPriority.ShowWindow(SW_HIDE);
	mTremType.ShowWindow(SW_HIDE);
	mTremRate.ShowWindow(SW_HIDE);
	mTremDepth.ShowWindow(SW_HIDE);
	mTremDelay.ShowWindow(SW_HIDE);
	mVibType.ShowWindow(SW_HIDE);
	mVibRate.ShowWindow(SW_HIDE);
	mVibDepth.ShowWindow(SW_HIDE);
	mVibDelay.ShowWindow(SW_HIDE);
	mBendRange.ShowWindow(SW_HIDE);
	mSamplePan.ShowWindow(SW_SHOW);
	mSampleVol.ShowWindow(SW_SHOW);
	mAttackTime.ShowWindow(SW_HIDE);
	mDecayTime.ShowWindow(SW_HIDE);
	mReleaseTime.ShowWindow(SW_SHOW);
	mAttackVolume.ShowWindow(SW_SHOW);
	mDecayVolume.ShowWindow(SW_HIDE);
	mVelMin.ShowWindow(SW_SHOW);
	mVelMax.ShowWindow(SW_SHOW);
	mKeyMin.ShowWindow(SW_SHOW);
	mKeyMax.ShowWindow(SW_SHOW);
	mKeyBase.ShowWindow(SW_SHOW);
	mKeyBaseListCtrl.ShowWindow(SW_HIDE);
	mDetune.ShowWindow(SW_SHOW);
	m_instrVolStatic.ShowWindow(SW_HIDE);
	m_instrPanStatic.ShowWindow(SW_HIDE);
	m_InstrPriorityStatic.ShowWindow(SW_HIDE);
	m_instrTremTypeStatic.ShowWindow(SW_HIDE);
	m_instrTremRateStatic.ShowWindow(SW_HIDE);
	m_instrTremDepthStatic.ShowWindow(SW_HIDE);
	m_instrTremDelayStatic.ShowWindow(SW_HIDE);
	m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
	m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
	m_instrVibrRateStatic.ShowWindow(SW_HIDE);
	m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
	m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
	m_samplePanStatic.ShowWindow(SW_SHOW);
	m_sampleVolStatic.ShowWindow(SW_SHOW);
	m_sampleVelMinStatic.ShowWindow(SW_SHOW);
	m_sampleVelMaxStatic.ShowWindow(SW_SHOW);
	m_sampleKeyMinStatic.ShowWindow(SW_SHOW);
	m_sampleKeyMaxStatic.ShowWindow(SW_SHOW);
	m_sampleKeyBaseStatic.ShowWindow(SW_SHOW);
	m_sampleDetuneStatic.ShowWindow(SW_SHOW);
	m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
	m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
	m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
	m_sampleAttackVolumeStatic.ShowWindow(SW_SHOW);
	m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);

	m_samplePanStatic.SetWindowText("ADSR 1");
	m_sampleVolStatic.SetWindowText("ADSR 2");
	m_sampleVelMinStatic.SetWindowText("ADSR 3");
	m_sampleVelMaxStatic.SetWindowText("ADSR 4");
	m_sampleKeyMinStatic.SetWindowText("ADSR 5");
	m_sampleKeyMaxStatic.SetWindowText("ADSR 6");
	m_sampleKeyBaseStatic.SetWindowText("ADSR 7");
	m_sampleDetuneStatic.SetWindowText("ADSR 8");

	m_sampleReleaseTimeStatic.SetWindowText("Float Key");
	m_sampleAttackVolumeStatic.SetWindowText("Pan");
}

void CN64SoundListToolDlg::ShowSfxControls()
{
	mSubSound.ShowWindow(SW_HIDE);
	mAddPrevButton.ShowWindow(SW_HIDE);
	mRemovePrevButton.ShowWindow(SW_HIDE);
	mAddSecButton.ShowWindow(SW_HIDE);
	mRemoveSecButton.ShowWindow(SW_HIDE);

	mSoundChoice.ShowWindow(SW_HIDE);
	mUpArrowButton.ShowWindow(SW_HIDE);
	mDownArrowButton.ShowWindow(SW_HIDE);

	mHalfVADPCMPrecision.ShowWindow(SW_SHOW);
	mCheckUnknownEADFlag.ShowWindow(SW_SHOW);
	mLabelTableIndex.ShowWindow(SW_SHOW);
	mTableIndex.ShowWindow(SW_SHOW);

	m_injectInPlaceButton.ShowWindow(SW_SHOW);
	m_import16BitADPCM.ShowWindow(SW_SHOW);
	m_import16BitADPCMSamePred.ShowWindow(SW_SHOW);
	m_import16BitRawValues.ShowWindow(SW_SHOW);
	m_importPredictors.ShowWindow(SW_SHOW);
	ShowOfficialSDKImport();
	m_importGroupBox.ShowWindow(SW_SHOW);
	mImportLoopPredictors.ShowWindow(SW_SHOW);
	
	m_playButton.ShowWindow(SW_SHOW);
	m_playLoopedButton.ShowWindow(SW_SHOW);
	m_stopButton.ShowWindow(SW_SHOW);
	m_saveButton.ShowWindow(SW_SHOW);
	m_import16BitRaw.ShowWindow(SW_HIDE);
	m_add16BitRaw.ShowWindow(SW_HIDE);
	m_add16BitADPCM.ShowWindow(SW_HIDE);
	m_addToEndGroupBox.ShowWindow(SW_HIDE);
	m_deleteButton.ShowWindow(SW_HIDE);
	m_miscGroupBox.ShowWindow(SW_HIDE);
	m_tremGroupBox.ShowWindow(SW_HIDE);
	m_vibrGroupBox.ShowWindow(SW_HIDE);
	mVolume.ShowWindow(SW_HIDE);
	mPan.ShowWindow(SW_HIDE);
	mPriority.ShowWindow(SW_HIDE);
	mTremType.ShowWindow(SW_HIDE);
	mTremRate.ShowWindow(SW_HIDE);
	mTremDepth.ShowWindow(SW_HIDE);
	mTremDelay.ShowWindow(SW_HIDE);
	mVibType.ShowWindow(SW_HIDE);
	mVibRate.ShowWindow(SW_HIDE);
	mVibDepth.ShowWindow(SW_HIDE);
	mVibDelay.ShowWindow(SW_HIDE);
	mBendRange.ShowWindow(SW_HIDE);
	mSamplePan.ShowWindow(SW_HIDE);
	mSampleVol.ShowWindow(SW_HIDE);
	mAttackTime.ShowWindow(SW_HIDE);
	mDecayTime.ShowWindow(SW_HIDE);
	mReleaseTime.ShowWindow(SW_SHOW);
	mAttackVolume.ShowWindow(SW_HIDE);
	mDecayVolume.ShowWindow(SW_HIDE);
	mVelMin.ShowWindow(SW_HIDE);
	mVelMax.ShowWindow(SW_HIDE);
	mKeyMin.ShowWindow(SW_HIDE);
	mKeyMax.ShowWindow(SW_HIDE);
	mKeyBase.ShowWindow(SW_HIDE);
	mKeyBaseListCtrl.ShowWindow(SW_HIDE);
	mDetune.ShowWindow(SW_HIDE);
	m_instrVolStatic.ShowWindow(SW_HIDE);
	m_instrPanStatic.ShowWindow(SW_HIDE);
	m_InstrPriorityStatic.ShowWindow(SW_HIDE);
	m_instrTremTypeStatic.ShowWindow(SW_HIDE);
	m_instrTremRateStatic.ShowWindow(SW_HIDE);
	m_instrTremDepthStatic.ShowWindow(SW_HIDE);
	m_instrTremDelayStatic.ShowWindow(SW_HIDE);
	m_instrVibrTypeStatic.ShowWindow(SW_HIDE);
	m_InstrBendRangeStatic.ShowWindow(SW_HIDE);
	m_instrVibrRateStatic.ShowWindow(SW_HIDE);
	m_instrVibrDepthStatic.ShowWindow(SW_HIDE);
	m_instrVibrDelayStatic.ShowWindow(SW_HIDE);
	m_samplePanStatic.ShowWindow(SW_HIDE);
	m_sampleVolStatic.ShowWindow(SW_HIDE);
	m_sampleVelMinStatic.ShowWindow(SW_HIDE);
	m_sampleVelMaxStatic.ShowWindow(SW_HIDE);
	m_sampleKeyMinStatic.ShowWindow(SW_HIDE);
	m_sampleKeyMaxStatic.ShowWindow(SW_HIDE);
	m_sampleKeyBaseStatic.ShowWindow(SW_HIDE);
	m_sampleDetuneStatic.ShowWindow(SW_HIDE);
	m_sampleAttackTimeStatic.ShowWindow(SW_HIDE);
	m_sampleDecayTimeStatic.ShowWindow(SW_HIDE);
	m_sampleReleaseTimeStatic.ShowWindow(SW_SHOW);
	m_sampleAttackVolumeStatic.ShowWindow(SW_HIDE);
	m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);

	m_sampleReleaseTimeStatic.SetWindowText("Float Key");
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosoundbank()
{
	int bankIndex = mSoundBankIndex.GetCurSel();
	if (bankIndex == -1)
		return;

	if (dontupdateitall)
	{
		dontupdateitall = false;

		
		m_bankName.SetWindowText("");
		alBankCurrent = results[bankIndex].bank;
		t1BankCurrent = NULL;
		subBankCurrent = -1;
		if (alBankCurrent == NULL)
		{
			dontupdateitall = true;
			return;
		}

		bool foundT1 = false;

		for (int x = 0; x < t1results.size(); x++)
		{
			if ((bankIndex >= t1results[x].bankStart) && (bankIndex < t1results[x].bankEnd))
			{
				mUseT1.ShowWindow(SW_SHOW);
				mT1GroupBox.ShowWindow(SW_SHOW);
				mT1IndexLabel.ShowWindow(SW_SHOW);
				mT1Index.ShowWindow(SW_SHOW);
				mT1FrequencyLabel.ShowWindow(SW_SHOW);
				mT1Frequency.ShowWindow(SW_SHOW);
				mT1SubBankLabel.ShowWindow(SW_SHOW);
				mT1SubBank.ShowWindow(SW_SHOW);
				mT1InstrumentLabel.ShowWindow(SW_SHOW);
				mT1Instrument.ShowWindow(SW_SHOW);
				mT1SoundLabel.ShowWindow(SW_SHOW);
				mT1Sound.ShowWindow(SW_SHOW);
				mT1FlagsLabel.ShowWindow(SW_SHOW);
				mT1Flags.ShowWindow(SW_SHOW);

				t1BankCurrent = t1results[x].bank;
				subBankCurrent = bankIndex - t1results[x].bankStart;

				mT1Index.ResetContent();
				for (int x = 0; x < t1BankCurrent->t1Entries.size(); x++)
				{
					CString tempStr;
					tempStr.Format("%04X", x);
					mT1Index.AddString(tempStr);
				}

				mT1Index.SetCurSel(0);

				foundT1 = true;
				break;
			}
		}

		if (!foundT1)
		{
			mUseT1.ShowWindow(SW_HIDE);
			mT1GroupBox.ShowWindow(SW_HIDE);
			mT1IndexLabel.ShowWindow(SW_HIDE);
			mT1Index.ShowWindow(SW_HIDE);
			mT1FrequencyLabel.ShowWindow(SW_HIDE);
			mT1Frequency.ShowWindow(SW_HIDE);
			mT1SubBankLabel.ShowWindow(SW_HIDE);
			mT1SubBank.ShowWindow(SW_HIDE);
			mT1InstrumentLabel.ShowWindow(SW_HIDE);
			mT1Instrument.ShowWindow(SW_HIDE);
			mT1SoundLabel.ShowWindow(SW_HIDE);
			mT1Sound.ShowWindow(SW_HIDE);
			mT1FlagsLabel.ShowWindow(SW_HIDE);
			mT1Flags.ShowWindow(SW_HIDE);
		}

		if (
			((alBankCurrent->soundBankFormat == KONAMICTL) && (alBankCurrent->konamiADSR.size() > 0))
			||
			((alBankCurrent->soundBankFormat == KONAMI8CTL) && (alBankCurrent->konamiADSR.size() > 0))
			)
		{
			mKonamiADSREditorButton.ShowWindow(SW_SHOW);
		}
		else
		{
			mKonamiADSREditorButton.ShowWindow(SW_HIDE);
		}

		mHalfSamplingRate.SetCheck(results[bankIndex].halfSamplingRate);

		if (alBankCurrent->bankName != "")
			m_bankName.SetWindowText(alBankCurrent->bankName);

		mInstrumentChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->count; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			CString name = "";
			if (alBankCurrent->inst[x] != NULL)
				name = " " + alBankCurrent->inst[x]->name;
			mInstrumentChoice.AddString(tempStr + name);
		}

		mInstrumentChoice.SetCurSel(0);

		mPercussionChoice.ResetContent();

		if (
			(alBankCurrent->soundBankFormat == STANDARDFORMAT)
			|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == CLAYFIGHTER)
			|| (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED)
			|| (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == ZLIBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BLASTCORPSZLBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BANJOTOOIEAUDIO)
			|| (alBankCurrent->soundBankFormat == TUROKFORMAT)
			|| (alBankCurrent->soundBankFormat == MKMYTHOLOGIES)
			|| (alBankCurrent->soundBankFormat == TITUS)
			|| (alBankCurrent->soundBankFormat == NINDEC)
			|| (alBankCurrent->soundBankFormat == KONAMICTL)
			|| (alBankCurrent->soundBankFormat == KONAMI8CTL)
			)
		{
			if (alBankCurrent->percussion != NULL)
			{
				CString tempStr;
				tempStr.Format("%04X", 0);
				mPercussionChoice.AddString(tempStr);

				mPercussionChoice.SetCurSel(0);
				mPercussionChoice.ShowWindow(SW_SHOW);
				mPercussionLabel.ShowWindow(SW_SHOW);
			}
			else
			{
				mPercussionChoice.ShowWindow(SW_HIDE);
				mPercussionLabel.ShowWindow(SW_HIDE);
			}
		}
		else if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT) ||
			(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT))
		{
			for (int x = 0; x < alBankCurrent->countEADPercussion; x++)
			{
				CString tempStr;
				tempStr.Format("%04X", x);
				mPercussionChoice.AddString(tempStr);
			}

			mPercussionLabel.ShowWindow(SW_SHOW);

			if (alBankCurrent->countEADPercussion > 0)
			{
				mPercussionChoice.SetCurSel(0);
				mPercussionChoice.ShowWindow(SW_SHOW);
			}
			else
			{
				mPercussionChoice.ShowWindow(SW_HIDE);
			}
		}

		mSfxChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->countSfx; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			mSfxChoice.AddString(tempStr);
		}

		if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
		{
			mSfxLabel.ShowWindow(SW_SHOW);
		}

		if (alBankCurrent->countSfx > 0)
		{
			mSfxChoice.SetCurSel(0);
			mSfxChoice.ShowWindow(SW_SHOW);
		}
		else
		{
			mSfxChoice.ShowWindow(SW_HIDE);
		}

		CString tempSampleStr;
		if (results[bankIndex].overrideSamplingRate)
		{
			tempSampleStr.Format("%d", results[bankIndex].samplingRate);
			mSamplingRate.SetWindowText(tempSampleStr);
			mOverrideRate.SetCheck(true);
		}
		else
		{
			tempSampleStr.Format("%d", alBankCurrent->samplerate);
			mSamplingRate.SetWindowText(tempSampleStr);
			mOverrideRate.SetCheck(false);
		}

		UpdateSamplingRateKeyBaseList();

		dontupdateitall = true;
	}

	percussionMode = NORMAL;
	OnCbnSelchangeCombosound();

	OnCbnSelchangeCombot1indexcombo();
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosound()
{
	if (dontupdateitall)
	{
		dontupdateitall = false;

		int instrSel = mInstrumentChoice.GetCurSel();

		if (instrSel == -1)
		{
			dontupdateitall = true;
			return;
		}

		CString tempStr;

		if (alBankCurrent->inst[instrSel]->samplerate != 0)
		{
			tempStr.Format("%d", alBankCurrent->inst[instrSel]->samplerate);
			mSamplingRate.SetWindowText(tempStr);

			UpdateSamplingRateKeyBaseList();
		}

	
		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->volume & 0xFF);
		mVolume.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->pan & 0xFF);
		mPan.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->priority);
		mPriority.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremType);
		mTremType.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremRate);
		mTremRate.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremDepth);
		mTremDepth.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->tremDelay);
		mTremDelay.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibType);
		mVibType.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibRate);
		mVibRate.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibDepth);
		mVibDepth.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->vibDelay);
		mVibDelay.SetWindowText(tempStr);

		tempStr.Format("%04X", alBankCurrent->inst[instrSel]->bendRange);
		mBendRange.SetWindowText(tempStr);

		mSoundChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->inst[instrSel]->soundCount; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			mSoundChoice.AddString(tempStr);
		}

		mSoundChoice.SetCurSel(0);

		percussionMode = NORMAL;
		ShowSoundBankControls();

		dontupdateitall = true;
	}

	OnCbnSelchangeCombosound2();
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosound2()
{
	if (dontupdateitall)
	{
		dontupdateitall = false;

		if (percussionMode == NORMAL)
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
			{
				dontupdateitall = true;
				return;
			}

			int bankIndex = mSoundBankIndex.GetCurSel();
			if (bankIndex == -1)
			{
				dontupdateitall = true;
				return;
			}
		

			if (alBankCurrent->inst[instrSel]->sounds[soundChoice] != NULL)
			{
				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT) || (alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
				{
					if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWavePrevious)
					{
						mRemovePrevButton.ShowWindow(SW_SHOW);
						mAddPrevButton.ShowWindow(SW_HIDE);
					}
					else
					{
						mAddPrevButton.ShowWindow(SW_SHOW);
						mRemovePrevButton.ShowWindow(SW_HIDE);
					}

					if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWaveSecondary)
					{
						mRemoveSecButton.ShowWindow(SW_SHOW);
						mAddSecButton.ShowWindow(SW_HIDE);
					}
					else
					{
						mAddSecButton.ShowWindow(SW_SHOW);
						mRemoveSecButton.ShowWindow(SW_HIDE);
					}
				}
				else
				{
					mAddPrevButton.ShowWindow(SW_HIDE);
					mRemovePrevButton.ShowWindow(SW_HIDE);
					mAddSecButton.ShowWindow(SW_HIDE);
					mRemoveSecButton.ShowWindow(SW_HIDE);

				}

				mCheckRaw.SetCheck(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type);

				CString tempStr;
				tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->samplePan);
				mSamplePan.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->sampleVolume);
				mSampleVol.SetWindowText(tempStr);


				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
					|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
					)
				{
					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->unknown1);
					mAttackVolume.SetWindowText(tempStr);

					tempStr.Format("%f", *reinterpret_cast<float*> (&alBankCurrent->inst[instrSel]->sounds[soundChoice]->floatKeyBasePrev));
					mAttackTime.SetWindowText(tempStr);

					tempStr.Format("%f", *reinterpret_cast<float*> (&alBankCurrent->inst[instrSel]->sounds[soundChoice]->floatKeyBase));
					mDecayTime.SetWindowText(tempStr);

					tempStr.Format("%f", *reinterpret_cast<float*> (&alBankCurrent->inst[instrSel]->sounds[soundChoice]->floatKeyBaseSec));
					mReleaseTime.SetWindowText(tempStr);

				}
				else
				{
					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.attackTime);
					mAttackTime.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.decayTime);
					mDecayTime.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.releaseTime);
					mReleaseTime.SetWindowText(tempStr);

					tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.attackVolume);
					mAttackVolume.SetWindowText(tempStr);

					tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.decayVolume);
					mDecayVolume.SetWindowText(tempStr);
				}

				tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.velocitymin);
				mVelMin.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.velocitymax);
				mVelMax.SetWindowText(tempStr);

				if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
					|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
					|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
					)
				{
					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[0]);
					mSamplePan.SetWindowText(tempStr);

					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[1]);
					mSampleVol.SetWindowText(tempStr);

					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[2]);
					mVelMin.SetWindowText(tempStr);

					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[3]);
					mVelMax.SetWindowText(tempStr);

					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[4]);
					mKeyMin.SetWindowText(tempStr);

					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[5]);
					mKeyMax.SetWindowText(tempStr);

					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[6]);
					mKeyBase.SetWindowText(tempStr);

					tempStr.Format("%04X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->adsrEAD[7]);
					mDetune.SetWindowText(tempStr);
				}
				else if (alBankCurrent->soundBankFormat == PAPERMARIO)
				{
					tempStr.Format("%02X", ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown1 >> 24) & 0xFF));
					mSamplePan.SetWindowText(tempStr);

					tempStr.Format("%02X", ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown1 >> 16) & 0xFF));
					mSampleVol.SetWindowText(tempStr);

					tempStr.Format("%02X", ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown1 >> 8) & 0xFF));
					mVelMin.SetWindowText(tempStr);

					tempStr.Format("%02X", ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown1) & 0xFF));
					mVelMax.SetWindowText(tempStr);
				}
				else
				{
					tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.keymin);
					mKeyMin.SetWindowText(tempStr);

					tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.keymax);
					mKeyMax.SetWindowText(tempStr);

					tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.keybase);
					mKeyBase.SetWindowText(tempStr);

					tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.detune);
					mDetune.SetWindowText(tempStr);
				}

				mSubSound.ResetContent();
				mSubSound.AddString("Primary");
				if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWavePrevious)
					mSubSound.AddString("Previous");
				if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWaveSecondary)
					mSubSound.AddString("Secondary");
				if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWavePrevious || alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWaveSecondary)
				{
					mSubSound.ShowWindow(SW_SHOW);
				}
				else
				{
					mSubSound.ShowWindow(SW_HIDE);
				}

			}

			if (mT1Index.GetCount() > 0)
			{
				if (t1BankCurrent != NULL)
				{
					for (int t = 0; t < t1BankCurrent->t1Entries.size(); t++)
					{
						if (
							(t1BankCurrent->t1Entries[t].bankNumber == bankIndex)
							&& (t1BankCurrent->t1Entries[t].instrumentNumber == instrSel)
							&& (t1BankCurrent->t1Entries[t].soundNumber == soundChoice)
							)
						{
							CString tempStr;
							tempStr.Format("%04X", t);
							mT1Index.SelectString(0, tempStr);
							break;
						}
					}
				}
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
			{
				dontupdateitall = true;
				return;
			}
		

			if (alBankCurrent->percussion->sounds[soundChoice] != NULL)
			{
				mAddPrevButton.ShowWindow(SW_HIDE);
				mRemovePrevButton.ShowWindow(SW_HIDE);
				mAddSecButton.ShowWindow(SW_HIDE);
				mRemoveSecButton.ShowWindow(SW_HIDE);

				mCheckRaw.SetCheck(alBankCurrent->percussion->sounds[soundChoice]->wav.type);

				CString tempStr;
				tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.base);
				mBase.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.len);
				mLength.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->samplePan);
				mSamplePan.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->sampleVolume);
				mSampleVol.SetWindowText(tempStr);


				tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->env.attackTime);
				mAttackTime.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->env.decayTime);
				mDecayTime.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->env.releaseTime);
				mReleaseTime.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->env.attackVolume);
				mAttackVolume.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->env.decayVolume);
				mDecayVolume.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->key.velocitymin);
				mVelMin.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->key.velocitymax);
				mVelMax.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->key.keymin);
				mKeyMin.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->key.keymax);
				mKeyMax.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->key.keybase);
				mKeyBase.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->percussion->sounds[soundChoice]->key.detune);
				mDetune.SetWindowText(tempStr);

				mSubSound.ResetContent();
				mSubSound.AddString("Primary");
				if (alBankCurrent->percussion->sounds[soundChoice]->hasWavePrevious)
					mSubSound.AddString("Previous");
				if (alBankCurrent->percussion->sounds[soundChoice]->hasWaveSecondary)
					mSubSound.AddString("Secondary");
				if (alBankCurrent->percussion->sounds[soundChoice]->hasWavePrevious || alBankCurrent->percussion->sounds[soundChoice]->hasWaveSecondary)
				{
					mSubSound.ShowWindow(SW_SHOW);
				}
				else
				{
					mSubSound.ShowWindow(SW_HIDE);
				}

			}
		}

		UpdateSamplingRateSelection();

		dontupdateitall = true;
	}

	mSubSound.SelectString(0, "Primary");

	OnCbnSelchangeCombosoundsubsound();
	OnCbnSelchangeCombot1indexcombo();
}

void CN64SoundListToolDlg::OnEnChangeEditsamplingrate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvol()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditpan()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditpriority()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremtype()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremrate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremdepth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittremdelay()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEdittvibtype()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvibrate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvibdepth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvibdelay()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditsamplepan()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditsamplevol()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditattacktime()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditdecaytime()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditreleasetime()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditattackvolume()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditdecayvolume()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvelmin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditvelmax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditkeymin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditkeymax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditkeybase()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditdetune()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditloop()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditend()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnEnChangeEditcount()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnBnClickedCheckloopenabled()
{
	UpdateTextBoxes();
}

void CN64SoundListToolDlg::OnBnClickedButtonup()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
				return;

			n64AudioLibrary.MoveUpSound(alBankCurrent, instrSel, soundChoice);

			if (dontupdateitall)
			{
				dontupdateitall = false;
				if (soundChoice != 0)
					mSoundChoice.SetCurSel(soundChoice-1);
				dontupdateitall = true;
			}
			OnCbnSelchangeCombosound2();
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
				return;

			n64AudioLibrary.MoveUpPercussion(alBankCurrent, soundChoice);

			if (dontupdateitall)
			{
				dontupdateitall = false;
				if (soundChoice != 0)
					mSoundChoice.SetCurSel(soundChoice-1);
				dontupdateitall = true;
			}
			OnCbnSelchangeCombosound2();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondown()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
				return;

			n64AudioLibrary.MoveDownSound(alBankCurrent, instrSel, soundChoice);

			if (dontupdateitall)
			{
				dontupdateitall = false;
				if (soundChoice != (alBankCurrent->inst[instrSel]->soundCount-1))
					mSoundChoice.SetCurSel(soundChoice+1);
				dontupdateitall = true;
			}
			OnCbnSelchangeCombosound2();
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
				return;

			n64AudioLibrary.MoveDownPercussion(alBankCurrent, soundChoice);

			if (dontupdateitall)
			{
				dontupdateitall = false;
				if (soundChoice != (alBankCurrent->percussion->soundCount-1))
					mSoundChoice.SetCurSel(soundChoice+1);
				dontupdateitall = true;
			}
			OnCbnSelchangeCombosound2();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonplayoriginalsound()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrSel == -1)
				return;

			if (soundSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (alBankCurrent->inst[instrSel]->samplerate)
				sampleRate = alBankCurrent->inst[instrSel]->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}
			else if (mUseT1.GetCheck())
			{
				if (mT1Index.GetCount() > 0)
				{
					if (t1BankCurrent != NULL)
					{
						int bankIndex = mSoundBankIndex.GetCurSel();

						for (int t = 0; t < t1BankCurrent->t1Entries.size(); t++)
						{
							if (
								(t1BankCurrent->t1Entries[t].bankNumber == bankIndex)
								&& (t1BankCurrent->t1Entries[t].instrumentNumber == instrSel)
								&& (t1BankCurrent->t1Entries[t].soundNumber == soundSel)
								)
							{
								sampleRate = t1BankCurrent->t1Entries[t].frequency;
								break;
							}
						}
					}
				}
			}

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			if (n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, instrSel, soundSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, primSel, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundSel = mSoundChoice.GetCurSel();

			if (soundSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (alBankCurrent->percussion->samplerate)
				sampleRate = alBankCurrent->percussion->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			if (n64AudioLibrary.ExtractPercussion(mainFolder, alBankCurrent, soundSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}

			if (n64AudioLibrary.ExtractEADPercussion(alBankCurrent, percussionSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
		else if (percussionMode == SFX)
		{
			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}

			if (n64AudioLibrary.ExtractSfx(alBankCurrent, sfxSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButton1()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int instrSel = mInstrumentChoice.GetCurSel();
				int soundChoice = mSoundChoice.GetCurSel();
				if (alBankCurrent->inst[instrSel]->sounds[soundChoice] == NULL)
					return;

				if (soundChoice == -1)
					return;

				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_RAW16_WAVE, primSel, false, false))
					return;

				float sampleRateRef = alBankCurrent->samplerate;
				if (alBankCurrent->inst[instrSel]->samplerate != 0)
					sampleRateRef = alBankCurrent->inst[instrSel]->samplerate;
				
				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int soundChoice = mSoundChoice.GetCurSel();
				if (alBankCurrent->percussion->sounds[soundChoice] == NULL)
					return;

				if (soundChoice == -1)
					return;

				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplacePercussionWithWavData(alBankCurrent, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_RAW16_WAVE, false))
					return;

				float sampleRateRef = alBankCurrent->samplerate;
				if (alBankCurrent->percussion->samplerate != 0)
					sampleRateRef = alBankCurrent->percussion->samplerate;
				
				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound2();
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonvadpcmimport()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int instrSel = mInstrumentChoice.GetCurSel();
				int soundChoice = mSoundChoice.GetCurSel();

				if (instrSel == -1)
					return;

				if (soundChoice == -1)
					return;

				if (alBankCurrent->inst[instrSel]->sounds[soundChoice] == NULL)
					return;

				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				bool decode8 = false;
				if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
				{
					decode8 = mHalfVADPCMPrecision.GetCheck();
				}

				if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
				{
					MessageBox("StarFox Special Sound Format unsupported");
					return;
				}

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, primSel, decode8, false))
					return;

				float sampleRateRef = alBankCurrent->samplerate;
				if (alBankCurrent->inst[instrSel]->samplerate != 0)
					sampleRateRef = alBankCurrent->inst[instrSel]->samplerate;
				
				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int instrSel = mInstrumentChoice.GetCurSel();
				int soundChoice = mSoundChoice.GetCurSel();
				if (alBankCurrent->percussion->sounds[soundChoice] == NULL)
					return;


				if (soundChoice == -1)
					return;

				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplacePercussionWithWavData(alBankCurrent, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, false))
					return;

				float sampleRateRef = alBankCurrent->samplerate;
				if (alBankCurrent->percussion->samplerate != 0)
					sampleRateRef = alBankCurrent->percussion->samplerate;
				
				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int percussionSel = mPercussionChoice.GetCurSel();

				if (percussionSel == -1)
					return;

				if (alBankCurrent->eadPercussion == NULL)
					return;

				if (percussionSel == -1)
					return;

				bool decode8 = false;
				if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
				{
					decode8 = mHalfVADPCMPrecision.GetCheck();
				}

				if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
				{
					MessageBox("StarFox Special Sound Format unsupported");
					return;
				}

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplaceEADPercussionWithWavData(alBankCurrent, percussionSel, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, decode8, false))
					return;

				float sampleRateRef = alBankCurrent->samplerate;

				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombopercussion();
			}
		}
		else if (percussionMode == SFX)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int sfxSel = mSfxChoice.GetCurSel();

				if (sfxSel == -1)
					return;

				if (alBankCurrent->alSfx[sfxSel] == NULL)
					return;

				bool decode8 = false;
				if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
				{
					decode8 = mHalfVADPCMPrecision.GetCheck();
				}

				if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
				{
					MessageBox("StarFox Special Sound Format unsupported");
					return;
				}

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplaceSfxWithWavData(alBankCurrent, sfxSel, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, decode8, false))
					return;

				float sampleRateRef = alBankCurrent->samplerate;

				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosfx();
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddsound()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int instrSel = mInstrumentChoice.GetCurSel();
				int soundChoice = mSoundChoice.GetCurSel();

				if (soundChoice == -1)
					return;

				unsigned long samplingRate;
				
				n64AudioLibrary.AddSound(alBankCurrent, instrSel,  m_ldFile.GetPathName(), samplingRate, AL_RAW16_WAVE);
				
				if (alBankCurrent->samplerate != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound();

				if (dontupdateitall)
				{
					dontupdateitall = false;
					mSoundChoice.SetCurSel(alBankCurrent->inst[instrSel]->soundCount-1);
					dontupdateitall = true;
				}
				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int soundChoice = mSoundChoice.GetCurSel();

				if (soundChoice == -1)
					return;

				unsigned long samplingRate;
				
				n64AudioLibrary.AddPercussion(alBankCurrent, m_ldFile.GetPathName(), samplingRate, AL_RAW16_WAVE);
				
				if (alBankCurrent->samplerate != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound();

				if (dontupdateitall)
				{
					dontupdateitall = false;
					mSoundChoice.SetCurSel(alBankCurrent->percussion->soundCount-1);
					dontupdateitall = true;
				}
				OnCbnSelchangeCombosound2();
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddsound2()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int instrSel = mInstrumentChoice.GetCurSel();

				unsigned long samplingRate;
				
				n64AudioLibrary.AddSound(alBankCurrent, instrSel,  m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE);
				
				if (alBankCurrent->samplerate != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound();

				if (dontupdateitall)
				{
					dontupdateitall = false;
					mSoundChoice.SetCurSel(alBankCurrent->inst[instrSel]->soundCount-1);
					dontupdateitall = true;
				}
				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				unsigned long samplingRate;
				
				n64AudioLibrary.AddPercussion(alBankCurrent, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE);
				
				if (alBankCurrent->samplerate != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombopercussion();

				if (dontupdateitall)
				{
					dontupdateitall = false;
					mSoundChoice.SetCurSel(alBankCurrent->percussion->soundCount-1);
					dontupdateitall = true;
				}
				OnCbnSelchangeCombosound2();
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeletesound()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
				return;

			n64AudioLibrary.DeleteSound(alBankCurrent, instrSel, soundChoice);

			OnCbnSelchangeCombosound();

			if (dontupdateitall)
			{
				dontupdateitall = false;
				if (soundChoice != 0)
					mSoundChoice.SetCurSel(soundChoice-1);
				dontupdateitall = true;
			}
			OnCbnSelchangeCombosound2();
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
				return;

			n64AudioLibrary.DeletePercussion(alBankCurrent, soundChoice);

			OnCbnSelchangeCombopercussion();

			if (dontupdateitall)
			{
				dontupdateitall = false;
				if (soundChoice != 0)
					mSoundChoice.SetCurSel(soundChoice-1);
				dontupdateitall = true;
			}

			OnCbnSelchangeCombosound2();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeletesound2()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			if ((alBankCurrent != NULL) && (alBankCurrent->soundBankFormat == MP3))
			{
				CFileDialog m_svFile(FALSE, "mp3", "ExtractedSound.mp3", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "MP3 (*.mp3)|*.mp3|");

				int statusFileOpen = (int) m_svFile.DoModal();
				CString fileName = m_svFile.GetFileName();
				if ((statusFileOpen == IDOK) && (fileName != ""))
				{
					int instrSel = mInstrumentChoice.GetCurSel();
					int soundChoice = mSoundChoice.GetCurSel();

					if (soundChoice == -1)
						return;

					CString subSoundStr;
					mSubSound.GetWindowText(subSoundStr);

					byte primSel = PRIMARY;
					if (subSoundStr == "Primary")
						primSel = PRIMARY;
					else if (subSoundStr == "Previous")
						primSel = PREVIOUS;
					else if (subSoundStr == "Secondary")
						primSel = SECONDARY;

					float sampleRate = alBankCurrent->samplerate;

					if (mOverrideRate.GetCheck())
					{
						CString tempStr;
						mSamplingRate.GetWindowText(tempStr);
						sampleRate = atoi(tempStr);
					}
					else if (mUseT1.GetCheck())
					{
						if (mT1Index.GetCount() > 0)
						{
							if (t1BankCurrent != NULL)
							{
								int bankIndex = mSoundBankIndex.GetCurSel();

								for (int t = 0; t < t1BankCurrent->t1Entries.size(); t++)
								{
									if (
										(t1BankCurrent->t1Entries[t].bankNumber == bankIndex)
										&& (t1BankCurrent->t1Entries[t].instrumentNumber == instrSel)
										&& (t1BankCurrent->t1Entries[t].soundNumber == soundChoice)
										)
									{
										sampleRate = t1BankCurrent->t1Entries[t].frequency;
										break;
									}
								}
							}
						}
					}

					if (n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, instrSel, soundChoice, m_svFile.GetPathName(), sampleRate, primSel, mHalfSamplingRate.GetCheck()))
					{
						
					}
				}
			}
			else
			{
				CFileDialog m_svFile(FALSE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Sound (*.wav)|*.wav|");

				int statusFileOpen = (int) m_svFile.DoModal();
				CString fileName = m_svFile.GetFileName();
				if ((statusFileOpen == IDOK) && (fileName != ""))
				{
					int instrSel = mInstrumentChoice.GetCurSel();
					int soundChoice = mSoundChoice.GetCurSel();

					if (soundChoice == -1)
						return;

					CString subSoundStr;
					mSubSound.GetWindowText(subSoundStr);

					byte primSel = PRIMARY;
					if (subSoundStr == "Primary")
						primSel = PRIMARY;
					else if (subSoundStr == "Previous")
						primSel = PREVIOUS;
					else if (subSoundStr == "Secondary")
						primSel = SECONDARY;

					float sampleRate = alBankCurrent->samplerate;

					if (mOverrideRate.GetCheck())
					{
						CString tempStr;
						mSamplingRate.GetWindowText(tempStr);
						sampleRate = atoi(tempStr);
					}
					else if (mUseT1.GetCheck())
					{
						if (mT1Index.GetCount() > 0)
						{
							if (t1BankCurrent != NULL)
							{
								int bankIndex = mSoundBankIndex.GetCurSel();

								for (int t = 0; t < t1BankCurrent->t1Entries.size(); t++)
								{
									if (
										(t1BankCurrent->t1Entries[t].bankNumber == bankIndex)
										&& (t1BankCurrent->t1Entries[t].instrumentNumber == instrSel)
										&& (t1BankCurrent->t1Entries[t].soundNumber == soundChoice)
										)
									{
										sampleRate = t1BankCurrent->t1Entries[t].frequency;
										break;
									}
								}
							}
						}
					}

					if (n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, instrSel, soundChoice, m_svFile.GetPathName(), sampleRate, primSel, mHalfSamplingRate.GetCheck()))
					{
						
					}
				}
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			CFileDialog m_svFile(FALSE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Sound (*.wav)|*.wav|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();
			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				int soundChoice = mSoundChoice.GetCurSel();

				if (soundChoice == -1)
					return;

				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				float sampleRate = alBankCurrent->samplerate;

				if (mOverrideRate.GetCheck())
				{
					CString tempStr;
					mSamplingRate.GetWindowText(tempStr);
					sampleRate = atoi(tempStr);
				}

				if (n64AudioLibrary.ExtractPercussion(mainFolder, alBankCurrent, soundChoice, m_svFile.GetPathName(), sampleRate, mHalfSamplingRate.GetCheck()))
				{

				}
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			CFileDialog m_svFile(FALSE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Sound (*.wav)|*.wav|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();
			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				int percussionSel = mPercussionChoice.GetCurSel();

				if (percussionSel == -1)
					return;

				//KillWMPlayer();

				float sampleRate = alBankCurrent->samplerate;

				if (mOverrideRate.GetCheck())
				{
					CString tempStr;
					mSamplingRate.GetWindowText(tempStr);
					sampleRate = atoi(tempStr);
				}

				if (n64AudioLibrary.ExtractEADPercussion(alBankCurrent, percussionSel, m_svFile.GetPathName(), sampleRate, mHalfSamplingRate.GetCheck()))
				{
				}
				else
				{
					MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
				}
			}
		}
		else if (percussionMode == SFX)
		{
			CFileDialog m_svFile(FALSE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Sound (*.wav)|*.wav|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();
			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				int sfxSel = mSfxChoice.GetCurSel();

				if (sfxSel == -1)
					return;

				//KillWMPlayer();

				float sampleRate = alBankCurrent->samplerate;

				if (mOverrideRate.GetCheck())
				{
					CString tempStr;
					mSamplingRate.GetWindowText(tempStr);
					sampleRate = atoi(tempStr);
				}

				if (n64AudioLibrary.ExtractSfx(alBankCurrent, sfxSel, m_svFile.GetPathName(), sampleRate, mHalfSamplingRate.GetCheck()))
				{
				}
				else
				{
					MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
				}
			}
		}
	}
}

void CN64SoundListToolDlg::OnFileSave()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_svFileOutROM(FALSE, "rom", (romName + ".rom"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "N64 ROM(*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin;*.ndd)|*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin;*.ndd|", this);

		int isFileOpened2 = m_svFileOutROM.DoModal();

		if ((isFileOpened2 != IDCANCEL) && (m_svFileOutROM.GetFileName() != ""))
		{
			CString romOutName = m_svFileOutROM.GetPathName();

			for (int x = 0; x < t1results.size(); x++)
			{
				bool resultT1 = n64AudioLibrary.WriteT1Bank(t1results[x].bank, ROM, t1results[x].t1Start, t1results[x].t1End);
				if (!resultT1)
				{
					MessageBox("Can't write T1 Bank");
					break;
				}
			}

			std::map<unsigned char, CString> cicLookup;
			cicLookup[0] = "starf";
			cicLookup[0xD5] = "lylat";
			cicLookup[0xDE] = "mario";
			cicLookup[0xDB] = "diddy";
			cicLookup[0xE4] = "aleck";
			cicLookup[0x14] = "zelda";
			cicLookup[0xEC] = "yoshi";

			if (cicLookup.find(ROM[0x173]) != cicLookup.end())
			{
				CString cic = cicLookup[ROM[0x173]];
				if (cic == "starf")
				{
					if (ROM[0x16F]==0xDB)
						cic = "ddipl";
					else if (ROM[0x16F]==0xD9)
						cic = "dddev";
				}

				CalcCRC calcCRC;
				calcCRC.calccrc(ROM, romSize, cic, true);

				FILE* outFile = fopen(m_svFileOutROM.GetPathName(), "wb");
				if (outFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENINGROMOUTPUT));
					return;
				}
				fwrite(ROM, 1, romSize, outFile);
				fclose(outFile);
			}
			else
			{
				FILE* outFile = fopen(m_svFileOutROM.GetPathName(), "wb");
				if (outFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENINGROMOUTPUT));
					return;
				}
				fwrite(ROM, 1, romSize, outFile);
				fclose(outFile);

				if (n64AudioLibrary.GetSizeFile((mainFolder + "rn64crc.exe")) == 0)
				{
					MessageBox("Warning, rn64crc.exe not found in main folder, could not re-crc ROM, if needed");
					return;
				}

				CString RN64dir = romOutName.Mid(0, (romOutName.ReverseFind('\\')+1));
				CString RN64name = romOutName.Mid((romOutName.ReverseFind('\\')+1), (romOutName.GetLength() - (romOutName.ReverseFind('\\')+1)));

				if (RN64dir != mainFolder)
					::CopyFile((mainFolder + "rn64crc.exe"), (RN64dir + "rn64crc.exe"), false);

				::SetCurrentDirectory(RN64dir);

				CString tempStr;
				tempStr.Format("rn64crc.exe -u \"%s\"", RN64name);
				hiddenExec(_T(tempStr.GetBuffer()), RN64dir);
			}
		}
	}
}

void CN64SoundListToolDlg::KillWMPlayer()
{
	STARTUPINFO si;
	::ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;    
   
	::ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if(CreateProcess(NULL, "taskkill /im wmplayer.exe", NULL,
	NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi))
	{
		WaitForSingleObject(pi.hProcess, 1000);
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}

	Sleep(1000);
}

void CN64SoundListToolDlg::PlayWMPlayerSound(CString wavFileSound)
{
	if (CSharedFunctions::GetSizeFile(wavFileSound) > 0)
	{
		/*STARTUPINFO si;
		::ZeroMemory(&si,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);

		PROCESS_INFORMATION pi;  
		::ZeroMemory(&si,sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		CString tempwmpStr;
		tempwmpStr.Format("C:\\Program Files\\Windows Media Player\\wmplayer.exe \"%s\"", wavFileSound);
		LPSTR pwsz = tempwmpStr.GetBuffer(0);
		CString strFilePath = tempwmpStr ;
			
		CFileFind finder;
		//Find the Default Drives 
		if(TRUE == finder.FindFile("C:\\Program Files\\Windows Media Player\\wmplayer.exe",0))
		{
			finder.FindNextFile() ;
				if(CreateProcess(NULL, pwsz, NULL,
			NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL,NULL,&si,&pi))
			{
						::CloseHandle(pi.hThread);
						::CloseHandle(pi.hProcess);
			}
			
		}*/

		::PlaySound(wavFileSound, 0, SND_ASYNC);

		return;
	}
}

BOOL CN64SoundListToolDlg::IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}

BOOL CN64SoundListToolDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory, HANDLE out)
{
	::SetCurrentDirectory(currentDirectory);
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESTDHANDLES;
   //si.wShowWindow  = SW_HIDE;
   si.hStdOutput = out;
   //si.hStdInput = out;
   //si.hStdError = out;

   ZeroMemory(&processInfo, sizeof(processInfo));


   if (currentDirectory.ReverseFind('\\') == (currentDirectory.GetLength()-1))
   {
		currentDirectory = currentDirectory.Mid(0, (currentDirectory.GetLength()-1));
   }

   /*return */CreateProcess(0, pCmdLine, 0, 0, TRUE, CREATE_DEFAULT_ERROR_MODE, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason Process Failed", LoadResourceText(IDS_STRINGERROR));
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
}

BOOL CN64SoundListToolDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
   ::SetCurrentDirectory(currentDirectory);
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESHOWWINDOW;
   si.wShowWindow  = SW_HIDE;
   ZeroMemory(&processInfo, sizeof(processInfo));


   if (currentDirectory.ReverseFind('\\') == (currentDirectory.GetLength()-1))
   {
		currentDirectory = currentDirectory.Mid(0, (currentDirectory.GetLength()-1));
   }

   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason Process Failed", LoadResourceText(IDS_STRINGERROR));
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
};

void CN64SoundListToolDlg::OnBnClickedExportctltbl()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_svFileCtl(FALSE, "ctl", "SoundBank.ctl", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ctl File (*.ctl)|*.ctl|", this);

		int statusFileOpen2 = (int) m_svFileCtl.DoModal();

		if ((statusFileOpen2 == IDCANCEL) || (m_svFileCtl.GetPathName() == ""))
			return;

		if (statusFileOpen2 == FALSE)
			return;

		CFileDialog m_svFileTbl(FALSE, "tbl", "SoundBank.tbl", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Tbl File (*.tbl)|*.tbl|", this);

		statusFileOpen2 = (int) m_svFileTbl.DoModal();

		if ((statusFileOpen2 == IDCANCEL) || (m_svFileTbl.GetPathName() == ""))
			return;

		if (statusFileOpen2 == FALSE)
			return;

		std::vector<ALBank*> alBanks = GetCombinedActiveBanks();
		n64AudioLibrary.WriteAudioToFile(alBanks, m_svFileCtl.GetPathName(), m_svFileTbl.GetPathName());

		/*int sizeCtl = n64AudioLibrary.GetSizeFile(m_svFileCtl.GetPathName());
		unsigned char* testCtl = new unsigned char[sizeCtl];
		FILE* inCtlTest = fopen(m_svFileCtl.GetPathName(), "rb");
		if (inCtlTest == NULL)
		{
			MessageBox("Can't open output ctl");
			delete [] testCtl;
			return;
		}
		fread(testCtl, 1, sizeCtl, inCtlTest);
		fclose(inCtlTest);

		int sizeTbl = n64AudioLibrary.GetSizeFile(m_svFileTbl.GetPathName());
		unsigned char* testTbl = new unsigned char[sizeTbl];
		FILE* inTblTest = fopen(m_svFileTbl.GetPathName(), "rb");
		if (inTblTest == NULL)
		{
			MessageBox("Can't open output tbl");
			delete [] testCtl;
			delete [] testTbl;
			return;
		}
		fread(testTbl, 1, sizeTbl, inTblTest);
		fclose(inTblTest);

		ALBank* bank2 = n64AudioLibrary.ReadAudio(testCtl, testCtl, sizeCtl, 0, testTbl, 0, 0xFFFFFFFF, 0);

		CString errors = n64AudioLibrary.CompareALBanks(alBankCurrent, bank2);

		delete [] testCtl;
		delete [] testTbl;*/
	}
}

std::vector<ALBank*> CN64SoundListToolDlg::GetCombinedActiveBanks()
{
	std::vector<ALBank*> alBanks;
	if ((alBankCurrent->soundBankFormat == STANDARDFORMAT) || (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED) || (alBankCurrent->soundBankFormat == KONAMICTL) || (alBankCurrent->soundBankFormat == KONAMI8CTL))
	{
		for (int x = 0; x < results.size(); x++)
		{
			if (results[x].bank == alBankCurrent)
			{
				bool isRun = false;
				if (((x + 1) < results.size()) && (results[x + 1].bank != NULL))
				{
					if (results[x + 1].bank->subBank == (results[x].bank->subBank + 1))
					{
						isRun = true;
					}
				}

				if ((x > 0) && (results[x - 1].bank != NULL))
				{
					if (results[x - 1].bank->subBank == (results[x].bank->subBank - 1))
					{
						isRun = true;
					}
				}

				if (isRun)
				{
					int startRun = x;
					int endRun = x;

					int currentRunValue = results[x].bank->subBank;
					while (startRun > 0)
					{
						int testSpot = startRun - 1;
						if (results[testSpot].bank->subBank == NULL)
							break;
						else if (results[testSpot].bank->subBank == (currentRunValue - 1))
						{
							startRun = testSpot;
							currentRunValue = results[testSpot].bank->subBank;
						}
						else
							break;
					}

					currentRunValue = results[x].bank->subBank;
					while ((endRun + 1) < results.size())
					{
						int testSpot = endRun + 1;
						if (results[testSpot].bank->subBank == NULL)
							break;
						else if (results[testSpot].bank->subBank == (currentRunValue + 1))
						{
							endRun = testSpot;
							currentRunValue = results[testSpot].bank->subBank;
						}
						else
							break;
					}

					for (int y = startRun; y <= endRun; y++)
					{
						alBanks.push_back(results[y].bank);
					}
				}
				else
				{
					alBanks.push_back(alBankCurrent);
				}

				break;
			}
		}
	}
	else
	{
		alBanks.push_back(alBankCurrent);
	}

	return alBanks;
}

void CN64SoundListToolDlg::OnBnClickedInjectplace()
{
	if ((alBankCurrent != NULL) && (romSize > 0))
	{
		int bank = mSoundBankIndex.GetCurSel();
		if (bank == -1)
			return;

		std::vector<ALBank*> alBanks = GetCombinedActiveBanks();

		unsigned char* ctl = NULL;
		unsigned char* tbl = NULL;
		unsigned char* ctl2 = NULL;
		int ctlSize = 0;
		int tblSize = 0;
		int ctlSize2 = 0;
		if ((alBankCurrent->soundBankFormat == STANDARDFORMAT) || (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED) || (alBankCurrent->soundBankFormat == KONAMICTL) || (alBankCurrent->soundBankFormat == KONAMI8CTL))
		{
			n64AudioLibrary.WriteAudio(alBanks, ctl, ctlSize, tbl, tblSize);

			/*ALBank* bank2 = n64AudioLibrary.ReadAudio(ctl, ctl, ctlSize, 0, tbl, 0, 0xFFFFFFFF, 0);
			CString errors = n64AudioLibrary.CompareALBanks(alBanks[0], bank2);

			ALBank* bank3 = n64AudioLibrary.ReadAudio(ctl, ctl, ctlSize, 0, tbl, 0, 0xFFFFFFFF, 1);
			errors = n64AudioLibrary.CompareALBanks(alBanks[1], bank3);

			ALBank* bank4 = n64AudioLibrary.ReadAudio(ctl, ctl, ctlSize, 0, tbl, 0, 0xFFFFFFFF, 2);
			errors = n64AudioLibrary.CompareALBanks(alBanks[2], bank4);

			ALBank* bank5 = n64AudioLibrary.ReadAudio(ctl, ctl, ctlSize, 0, tbl, 0, 0xFFFFFFFF, 3);
			errors = n64AudioLibrary.CompareALBanks(alBanks[3], bank5);
			errors = errors;*/
		}
		else if (alBankCurrent->soundBankFormat == NINDEC)
		{
			n64AudioLibrary.WriteAudioNinDec(alBanks, ctl, ctlSize, tbl, tblSize);
		}
		else if ((alBankCurrent->soundBankFormat == TUROKFORMAT)  || (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED) || (alBankCurrent->soundBankFormat == CLAYFIGHTER) || (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)|| (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED))
		{
			MessageBox("Sorry, no encoding yet for MIO0 format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
		{
			// Does all at once
			n64AudioLibrary.WriteAudioSuperMario(results, ctl, ctlSize, tbl, tblSize);
		}
		else if (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
		{
			n64AudioLibrary.WriteAudioStarFox(ROM, romSize, currentGameConfig.offsetZeldaCtlTable, currentGameConfig.offsetZeldaTblTable, currentGameConfig.startZeldaCtlData, currentGameConfig.startZeldaTblData, results, ctl, ctlSize, tbl, tblSize, currentGameConfig.isCompressedZeldaCtlTblTables, currentGameConfig.compressedZeldaCtlTblTableStart, currentGameConfig.compressedZeldaCtlTblTableEnd);
			if (ctl == NULL)
				return;
		}
		else if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
		{
			n64AudioLibrary.WriteAudioZelda(ROM, romSize, currentGameConfig.offsetZeldaCtlTable, currentGameConfig.offsetZeldaTblTable, currentGameConfig.startZeldaCtlData, currentGameConfig.startZeldaTblData, results, ctl, ctlSize, tbl, tblSize, currentGameConfig.isCompressedZeldaCtlTblTables, currentGameConfig.compressedZeldaCtlTblTableStart, currentGameConfig.compressedZeldaCtlTblTableEnd);
			if (ctl == NULL)
				return;
		}
		else if (alBankCurrent->soundBankFormat == CONKERFORMAT)
		{
			n64AudioLibrary.WriteAudioConker(alBankCurrent, ctl, ctlSize, tbl, tblSize, currentGameConfig.soundBanks[bank].numberInstruments, ctl2, ctlSize2);
			
			GECompression gecompression;
			gecompression.SetGame(CONKER);
			gecompression.SetPath(mainFolder);
			
			FILE* outFile = fopen(mainFolder + "tempConkEnc.bin", "wb");
			if(outFile == NULL)
			{
				if (ctl2 != NULL)
					delete [] ctl2;
				delete [] ctl;
				delete [] tbl;
				MessageBox("Can't open temp file");
				return;
			}
			fwrite(ctl, 1, ctlSize, outFile);
			fclose(outFile);

			FILE* outFileTestCtl2 = fopen(mainFolder + "tempConkEnc2.bin", "wb");
			if(outFileTestCtl2 == NULL)
			{
				if (ctl2 != NULL)
					delete [] ctl2;
				delete [] ctl;
				delete [] tbl;
				MessageBox("Can't open temp file 2");
				return;
			}
			fwrite(ctl2, 1, ctlSize2, outFileTestCtl2);
			fclose(outFileTestCtl2);

			if (!gecompression.CompressGZipFileNoPad(mainFolder + "tempConkEnc.bin", mainFolder + "tempConkEnc.gz", false))
			{
				if (ctl2 != NULL)
					delete [] ctl2;
				delete [] ctl;
				delete [] tbl;
				MessageBox("Can't open gzipped file");
				return;
			}

			int sizeCompressed = n64AudioLibrary.GetSizeFile(mainFolder + "tempConkEnc.gz");

			if (sizeCompressed == 0)
			{
				if (ctl2 != NULL)
					delete [] ctl2;
				delete [] ctl;
				delete [] tbl;
				MessageBox("Empty gzipped file");
				return;
			}

			FILE* inFile = fopen((mainFolder + "tempConkEnc.gz"), "rb");
			if (inFile == NULL)
			{
				if (ctl2 != NULL)
					delete [] ctl2;
				delete [] ctl;
				delete [] tbl;
				MessageBox("Error reading file");
				return;
			}
			delete [] ctl;
			ctlSize = sizeCompressed;

			ctl = new unsigned char[ctlSize];
			fread(ctl, 1, sizeCompressed, inFile);
			fclose(inFile);
			::DeleteFile(mainFolder + "tempConkEnc.bin");
			::DeleteFile(mainFolder + "tempConkEnc2.bin");
			::DeleteFile(mainFolder + "tempConkEnc.gz");
		}
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2)
			n64AudioLibrary.WriteAudioN64PtrWavetableV2(alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
			n64AudioLibrary.WriteAudioN64PtrWavetableV2Blitz(mainFolder, alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV1)
			n64AudioLibrary.WriteAudioN64PtrWavetableV1(alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
		{
			n64AudioLibrary.WriteAudioN64PtrWavetableV2Yay0(mainFolder, alBankCurrent, ctl, ctlSize, tbl, tblSize);
		}
		else if (alBankCurrent->soundBankFormat == TAZHUFFMAN)
		{
			MessageBox("Sorry, no encoding yet for TAZHUFFMAN format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == SNOW)
		{
			MessageBox("Sorry, no encoding yet for SNOW format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == ARMYMENFORMAT)
		{
			MessageBox("Sorry, no encoding yet for Army Men format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == MEGAMAN64PTRV2)
		{
			MessageBox("Sorry, no encoding yet for Mega Man format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
		{
			MessageBox("Sorry, no encoding yet for ZLIB N64Wave format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEQUAKE2)
		{
			MessageBox("Sorry, no encoding yet for Quake N64Wave format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLESNOWBOARDKIDS)
		{
			MessageBox("Sorry, no encoding yet for Snowboard Kids N64Wave format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLEVIEWPOINT)
		{
			MessageBox("Sorry, no encoding yet for Viewpoint 2064 N64Wave format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == TITUS)
		{
			MessageBox("Sorry, no encoding yet for Titus format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == SYDNEY)
		{
			MessageBox("Sorry, no encoding yet for Sydney format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == MKMYTHOLOGIES)
		{
			MessageBox("Sorry, no encoding yet for BOFS format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == ZLIBSN64)
		{
			MessageBox("Sorry, no encoding yet for ZLIBSN64 format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == SN64)
		{
			MessageBox("Sorry, no encoding yet for SN64 format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == MORT)
		{
			
		}
		else
		{
			MessageBox("Sorry, no encoding");
			return;
		}

		if (alBankCurrent->soundBankFormat == KONAMICTL)
		{
			n64AudioLibrary.WriteKonamiADSR(ROM, results[bank].bank, currentGameConfig.soundBanks[bank].numberInstruments, currentGameConfig.soundBanks[bank].mask, currentGameConfig.soundBanks[bank].extra, currentGameConfig.soundBanks[bank].extra2);
		}
		else if (alBankCurrent->soundBankFormat == KONAMI8CTL)
		{
			n64AudioLibrary.WriteKonami8ADSR(ROM, results[bank].bank, currentGameConfig.soundBanks[bank].numberInstruments, currentGameConfig.soundBanks[bank].mask, currentGameConfig.soundBanks[bank].extra, currentGameConfig.soundBanks[bank].extra2);
		}

		if (alBankCurrent->soundBankFormat == CONKERFORMAT)
		{
			if (n64AudioLibrary.InjectCtlTblIntoROMArrayInPlace(ROM, ctl, ctlSize, tbl, tblSize, results[bank].ctlOffset, results[bank].tblOffset, currentGameConfig.soundBanks[bank].numberInstruments - currentGameConfig.soundBanks[bank].ctl, results[bank].tblSize, ctl2, ctlSize2, currentGameConfig.soundBanks[bank].numberInstruments, results[bank].tblOffset - currentGameConfig.soundBanks[bank].numberInstruments))
			{
				/*for (int x = (results[bank].ctlOffset + ctlSize); x < (results[bank].ctlOffset + results[bank].ctlSize); x++)
				{
					ROM[x] = 0xFF;
				}*/
			}
		}
		else if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
		{
			if (n64AudioLibrary.InjectCtlTblIntoROMArrayInPlace(ROM, ctl, ctlSize, tbl, tblSize, currentGameConfig.startZeldaCtlData, currentGameConfig.startZeldaTblData, currentGameConfig.endZeldaCtlData - currentGameConfig.startZeldaCtlData + 1, currentGameConfig.endZeldaTblData - currentGameConfig.startZeldaTblData + 1, NULL, 0, 0, 0))
			{

			}
		}
		else if (alBankCurrent->soundBankFormat == MORT)
		{
			ctlTblResult result;
			for (int x = 0; x < results.size(); x++)
			{
				if (alBanks[0] == results[x].bank)
				{
					result = results[x];
					break;
				}
			}

			if (alBankCurrent->inst[0]->sounds[0]->wav.len > result.ctlSize)
			{
				CString ctlError;
				ctlError.Format("Error too big, %04X is too big for %04X", alBankCurrent->inst[0]->sounds[0]->wav.len, ctlSize);
				MessageBox(ctlError, "Error");
				return;
			}
			memcpy(&ROM[alBankCurrent->inst[0]->sounds[0]->wav.base], alBankCurrent->inst[0]->sounds[0]->wav.wavData, alBankCurrent->inst[0]->sounds[0]->wav.len);
		}
		else
		{
			ctlTblResult result;
			for (int x = 0; x < results.size(); x++)
			{
				if (alBanks[0] == results[x].bank)
				{
					result = results[x];
					break;
				}
			}
			if (n64AudioLibrary.InjectCtlTblIntoROMArrayInPlace(ROM, ctl, ctlSize, tbl, tblSize, result.ctlOffset, result.tblOffset, result.ctlSize, result.tblSize, NULL, 0, 0, 0))
			{
				/*for (int x = (results[bank].ctlOffset + ctlSize); x < (results[bank].ctlOffset + results[bank].ctlSize); x++)
				{
					ROM[x] = 0xFF;
				}*/
			}
		}

		if (ctl2 != NULL)
			delete [] ctl2;
		delete [] ctl;
		delete [] tbl;
	}
}


bool CN64SoundListToolDlg::GetFolder(CString& folderpath, const char* szCaption, HWND hOwner)
{
	bool retVal = false;

	// The BROWSEINFO struct tells the shell 
	// how it should display the dialog.
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags   = BIF_USENEWUI;
	bi.hwndOwner = hOwner;
	bi.lpszTitle = szCaption;

	// must call this if using BIF_USENEWUI
	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the selected folder.
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

	if(pIDL != NULL)
	{
		// Create a buffer to store the path, then get the path.
		char buffer[_MAX_PATH] = {'\0'};
		if(::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			folderpath = buffer;
			retVal = true;
		}		

		// free the item id list
		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();

	return retVal;
}

void CN64SoundListToolDlg::OnBnClickedButtonrip()
{
	if (alBankCurrent != NULL)
	{
		CString outputFolder;
		if (GetFolder(outputFolder, "Folder To Save To", NULL))
		{
			float sampleRate = alBankCurrent->samplerate;

			if (alBankCurrent->percussion != NULL)
			{
				for (int y = 0; y < alBankCurrent->percussion->soundCount; y++)
				{
					if (alBankCurrent->samplerate != 0)
						sampleRate = (alBankCurrent->samplerate);
					CString tempExportNameStr;
					if (mExportShortFilename.GetCheck())
						tempExportNameStr.Format("%s\\B%02XP%02X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
					else
						tempExportNameStr.Format("%s\\BANK_%02X_PERC_%04X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
					n64AudioLibrary.ExtractPercussion(mainFolder, alBankCurrent, y, tempExportNameStr, sampleRate, mHalfSamplingRate.GetCheck());
				}
			}

			for (int y = 0; y < alBankCurrent->countEADPercussion; y++)
			{
				if (alBankCurrent->samplerate != 0)
					sampleRate = (alBankCurrent->samplerate);
				CString tempExportNameStr;
				if (mExportShortFilename.GetCheck())
					tempExportNameStr.Format("%s\\B%02XP%02X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
				else
					tempExportNameStr.Format("%s\\BANK_%02X_PERC_%04X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
				n64AudioLibrary.ExtractEADPercussion(alBankCurrent, y, tempExportNameStr, sampleRate, mHalfSamplingRate.GetCheck());
			}

			for (int y = 0; y < alBankCurrent->countSfx; y++)
			{
				if (alBankCurrent->samplerate != 0)
					sampleRate = (alBankCurrent->samplerate);
				CString tempExportNameStr;
				if (mExportShortFilename.GetCheck())
					tempExportNameStr.Format("%s\\B%02XF%02X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
				else
					tempExportNameStr.Format("%s\\BANK_%02X_SFX_%04X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
				n64AudioLibrary.ExtractSfx(alBankCurrent, y, tempExportNameStr, sampleRate, mHalfSamplingRate.GetCheck());
			}

			for (int x = 0; x < alBankCurrent->count; x++)
			{
				for (int y = 0; y < alBankCurrent->inst[x]->soundCount; y++)
				{
					if (alBankCurrent->inst[x]->samplerate != 0)
						sampleRate = (alBankCurrent->inst[x]->samplerate);

					CString tempExportNameStr;
					if (alBankCurrent->soundBankFormat == MP3)
					{
						if (mExportShortFilename.GetCheck())
							tempExportNameStr.Format("%s\\B%02XI%02XS%02X.mp3", outputFolder, mSoundBankIndex.GetCurSel(), x, y);
						else
							tempExportNameStr.Format("%s\\BANK_%02X_INSTR_%04X_SND_%04X.mp3", outputFolder, mSoundBankIndex.GetCurSel(), x, y);
					}
					else
					{
						if (mExportShortFilename.GetCheck())
							tempExportNameStr.Format("%s\\B%02XI%02XS%02X.wav", outputFolder, mSoundBankIndex.GetCurSel(), x, y);
						else
							tempExportNameStr.Format("%s\\BANK_%02X_INSTR_%04X_SND_%04X.wav", outputFolder, mSoundBankIndex.GetCurSel(), x, y);
					}

					float sampleRatePrimary = sampleRate;
					if (mUseT1.GetCheck())
					{
						if (mT1Index.GetCount() > 0)
						{
							if (t1BankCurrent != NULL)
							{
								int bankIndex = mSoundBankIndex.GetCurSel();

								for (int t = 0; t < t1BankCurrent->t1Entries.size(); t++)
								{
									if (
										(t1BankCurrent->t1Entries[t].bankNumber == bankIndex)
										&& (t1BankCurrent->t1Entries[t].instrumentNumber == x)
										&& (t1BankCurrent->t1Entries[t].soundNumber == y)
										)
									{
										sampleRatePrimary = t1BankCurrent->t1Entries[t].frequency;
										break;
									}
								}
							}
						}
					}

					n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, x, y, tempExportNameStr, sampleRatePrimary, PRIMARY, mHalfSamplingRate.GetCheck());
					if ((alBankCurrent->inst[x] != NULL) && (alBankCurrent->inst[x]->sounds[y] != NULL))
					{
						if (alBankCurrent->inst[x]->sounds[y]->hasWavePrevious)
							n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, x, y, tempExportNameStr + "Prev.wav", sampleRate, PREVIOUS, mHalfSamplingRate.GetCheck());
						if (alBankCurrent->inst[x]->sounds[y]->hasWaveSecondary)
							n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, x, y, tempExportNameStr + "Sec.wav", sampleRate, SECONDARY, mHalfSamplingRate.GetCheck());
					}
				}
			}
		}
	}
}
void CN64SoundListToolDlg::OnBnClickedButtontest()
{
	if (alBankCurrent != NULL)
	{
		CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
			if (inFile == NULL)
			{
				MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
				return;
			}
			fclose(inFile);

			int instrSel = 0x0;
			int soundChoice = 0x50;

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			unsigned long samplingRate;
			if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, primSel, false, false))
				return;

			float sampleRateRef = alBankCurrent->samplerate;
			if (alBankCurrent->inst[instrSel]->samplerate != 0)
				sampleRateRef = alBankCurrent->inst[instrSel]->samplerate;

			if (sampleRateRef != samplingRate)
			{
				CString tempStr;
				tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
				//MessageBox(tempStr, "Warning");
			}

			for (int x = 0x51; x < 0x70; x++)
			{
				if (x != soundChoice)
					alBankCurrent->inst[instrSel]->sounds[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice];
					
			}

			OnCbnSelchangeCombosound2();
		}
	}
}

unsigned short CN64SoundListToolDlg::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CN64SoundListToolDlg::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CN64SoundListToolDlg::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned long CN64SoundListToolDlg::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}
void CN64SoundListToolDlg::OnFileQuit()
{
	OnOK();
}

BOOL CN64SoundListToolDlg::DestroyWindow()
{
	for (int x = 0; x < t1results.size(); x++)
	{
		n64AudioLibrary.DisposeT1Bank(t1results[x].bank);
	}

	t1results.clear();

	for (int x = 0; x < numberResults; x++)
	{
		n64AudioLibrary.DisposeALBank(results[x].bank);
	}

	results.clear();

	if (ROM != NULL)
	{
		delete [] ROM;
		ROM = NULL;
	}

	return CDialog::DestroyWindow();
}

void CN64SoundListToolDlg::OnFileOpenromexplicit()
{
	if (!OpenROMPlusDlg())
		return;


	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < t1results.size(); x++)
		{
			n64AudioLibrary.DisposeT1Bank(t1results[x].bank);
		}

		t1results.clear();

		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		results.clear();

		alBankCurrent = NULL;
		t1BankCurrent = NULL;
		subBankCurrent = -1;

		HRSRC hResource = NULL;
		hResource = FindResource( initiallyLoadedResource, MAKEINTRESOURCE( IDD_DIALOGSPECIFYLOCATION ), RT_DIALOG );
		if (hResource == NULL)
		{
			AfxSetResourceHandle(mainExeResource);
		}

		COpenSpecificCtlTbl tempDlg = new COpenSpecificCtlTbl();
		if (tempDlg.DoModal() != IDCANCEL)
		{
			unsigned long ctl = tempDlg.GetCtl();
			unsigned long tbl = tempDlg.GetTbl();

			numberResults = 0;

			results.resize(numberResults+1);

			results[numberResults].ctlOffset = ctl;
			results[numberResults].tblOffset = tbl;
			if (tbl > ctl)
			{
				results[numberResults].ctlSize = (results[numberResults].tblOffset - results[numberResults].ctlOffset);
				results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
			}
			else
			{
				results[numberResults].ctlSize = (romSize - results[numberResults].ctlOffset);
				results[numberResults].tblSize = (results[numberResults].ctlOffset - results[numberResults].tblOffset);
			}
			results[numberResults].bank = n64AudioLibrary.ReadAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, 0);

			numberResults++;

			for (int x = 0; x < numberResults; x++)
			{
				CString tempStr;
				tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);
				mSoundBankIndex.AddString(tempStr);
			}

			mSoundBankIndex.SetCurSel(0);

			OnCbnSelchangeCombosoundbank();
		}
	}
	catch (char * str)
	{
		CString tempErrStr;
		tempErrStr.Format("%s", str);
		MessageBox(tempErrStr, LoadResourceText(IDS_STRINGERRORPROCESSING));

		numberResults = 0;
	}

	AfxSetResourceHandle(initiallyLoadedResource);
}

void CN64SoundListToolDlg::OnBnClickedButtonimportpredictors()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
			{
				MessageBox("StarFox Special Sound Format unsupported");
				return;
			}

			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrumentSel == -1)
				return;

			if (soundSel == -1)
				return;

			if (alBankCurrent->inst[instrumentSel]->sounds[soundSel] == NULL)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "Predictors.bin", OFN_HIDEREADONLY, "Predictors (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				n64AudioLibrary.ImportPredictors(alBankCurrent, instrumentSel, soundSel, m_ldFile.GetPathName(), primSel);
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
			{
				MessageBox("StarFox Special Sound Format unsupported");
				return;
			}

			int soundSel = mSoundChoice.GetCurSel();
			if (soundSel == -1)
				return;

			if (alBankCurrent->percussion->sounds[soundSel] == NULL)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "Predictors.bin", OFN_HIDEREADONLY, "Predictors (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				n64AudioLibrary.ImportPercussionPredictors(alBankCurrent, soundSel, m_ldFile.GetPathName());
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
			{
				MessageBox("StarFox Special Sound Format unsupported");
				return;
			}

			if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
			{
				MessageBox("StarFox Special Sound Format unsupported");
				return;
			}

			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			if (alBankCurrent->eadPercussion == NULL)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "Predictors.bin", OFN_HIDEREADONLY, "Predictors (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				n64AudioLibrary.ImportEADPercussionPredictors(alBankCurrent, percussionSel, m_ldFile.GetPathName());
			}
		}
		else if (percussionMode == SFX)
		{
			if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
			{
				MessageBox("StarFox Special Sound Format unsupported");
				return;
			}

			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			if (alBankCurrent->alSfx == NULL)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "Predictors.bin", OFN_HIDEREADONLY, "Predictors (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				n64AudioLibrary.ImportSfxPredictors(alBankCurrent, sfxSel, m_ldFile.GetPathName());
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonexportpreditors()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrumentSel == -1)
				return;

			if (soundSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "Predictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();
			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;
				

				if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
				{
					MessageBox("StarFox Special Sound Format unsupported");
					return;
				}

				n64AudioLibrary.ExportPredictors(alBankCurrent, instrumentSel, soundSel, m_svFile.GetPathName(), primSel);
			}
		}
		else if (percussionMode == NORMAL)
		{
			int soundSel = mSoundChoice.GetCurSel();

			if (soundSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "Predictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();
			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				n64AudioLibrary.ExportPercussionPredictors(alBankCurrent, soundSel, m_svFile.GetPathName());
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "Predictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();
			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				n64AudioLibrary.ExportEADPercussionPredictors(alBankCurrent, percussionSel, m_svFile.GetPathName());
			}
		}
		else if (percussionMode == SFX)
		{
			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "Predictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();
			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				n64AudioLibrary.ExportSfxPredictors(alBankCurrent, sfxSel, m_svFile.GetPathName());
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonimport16bitraw()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrumentSel == -1)
				return;

			if (soundSel == -1)
				return;

			if (alBankCurrent->inst[instrumentSel]->sounds[soundSel] == NULL)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "RawSound.bin", OFN_HIDEREADONLY, "Raw Bin Sound (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				n64AudioLibrary.ImportRawData(alBankCurrent, instrumentSel, soundSel, m_ldFile.GetPathName(), primSel);
				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundSel = mSoundChoice.GetCurSel();

			if (soundSel == -1)
				return;

			if (alBankCurrent->percussion->sounds[soundSel] == NULL)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "RawSound.bin", OFN_HIDEREADONLY, "Raw Bin Sound (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				n64AudioLibrary.ImportPercussionRawData(alBankCurrent, soundSel, m_ldFile.GetPathName());
				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundSel = mSoundChoice.GetCurSel();

			if (soundSel == -1)
				return;

			if (alBankCurrent->percussion->sounds[soundSel] == NULL)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "RawSound.bin", OFN_HIDEREADONLY, "Raw Bin Sound (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				n64AudioLibrary.ImportPercussionRawData(alBankCurrent, soundSel, m_ldFile.GetPathName());
				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "RawSound.bin", OFN_HIDEREADONLY, "Raw Bin Sound (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				n64AudioLibrary.ImportEADRawPercussionData(alBankCurrent, percussionSel, m_ldFile.GetPathName());
				OnCbnSelchangeCombopercussion();
			}
		}
		else if (percussionMode == SFX)
		{
			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "RawSound.bin", OFN_HIDEREADONLY, "Raw Bin Sound (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				n64AudioLibrary.ImportRawSfxData(alBankCurrent, sfxSel, m_ldFile.GetPathName());
				OnCbnSelchangeCombosfx();
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonexportraw()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrumentSel == -1)
				return;

			if (soundSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "RawSound.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Raw Bin Sound (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();

			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				n64AudioLibrary.ExportRawData(alBankCurrent, instrumentSel, soundSel, m_svFile.GetPathName(), primSel);

			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "RawSound.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Raw Bin Sound (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();

			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				n64AudioLibrary.ExportEADRawPercussionData(alBankCurrent, percussionSel, m_svFile.GetPathName());
			}
		}
		else if (percussionMode == SFX)
		{
			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "RawSound.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Raw Bin Sound (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();

			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				n64AudioLibrary.ExportRawSfxData(alBankCurrent, sfxSel, m_svFile.GetPathName());
			}
		}
	}
}

void CN64SoundListToolDlg::OpenROMSpecific(SoundGameConfig gameConfig)
{
	CString tempGameName = gameConfig.gameName;
	tempGameName.Replace(" Speech", "");
	tempGameName.Replace(" Raw", "");
	tempGameName.Replace(" MP3", "");
	tempGameName.Replace(" Music", "");
	tempGameName.Replace(" DEAN", "");
	tempGameName.Replace(" DEAN Individual", "");
	tempGameName.Replace(" Individual", "");
	tempGameName.Replace(" Sfx", "");
	tempGameName.Replace(" SAM", "");
	tempGameName.Replace(" SFX", "");
	tempGameName.Replace(" SNG", "");
	tempGameName.Replace(" (Compressed Banks)", "");
	tempGameName.Replace(" Compressed", "");
	tempGameName.Replace(" Instruments", "");
	tempGameName.Replace(" - With Copying IPL ROM from 0x140000 to 0x3F4000 to this", "");
	tempGameName.Replace("iQue Menu - ", "");
	if (!OpenROMPlusDlg(tempGameName + " [!].z64"))
		return;

	currentGameConfig = gameConfig;

	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < t1results.size(); x++)
		{
			n64AudioLibrary.DisposeT1Bank(t1results[x].bank);
		}

		t1results.clear();

		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		results.clear();

		alBankCurrent = NULL;
		t1BankCurrent = NULL;
		subBankCurrent = -1;

		CN64SoundToolReader::ReadSoundbanks(ROM, romSize, gameConfig, numberResults, results, t1results);

		/*unsigned char* geTempRom = new unsigned char[0xC00000];
		FILE * inGE = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\Goldeneye.rom", "rb");
		fread(geTempRom, 1, 0xC00000, inGE);
		fclose(inGE);*/

		/*unsigned char* geTempRom = new unsigned char[0x2000000];
		FILE * inGE = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\Perfect_dark.v64", "rb");
		fread(geTempRom, 1, 0x2000000, inGE);
		fclose(inGE);
		ALBank * GEBank = n64AudioLibrary.ReadAudio(&geTempRom[0xCFBF30], 0xA060, 0x0, &geTempRom[0xD05F90]);*/

		// 0x4B sounds
		/*ALBank * GEBank = n64AudioLibrary.ReadAudioRawAllowed(geTempRom, &geTempRom[0x3B4450], 0x43A0, 0x0, &geTempRom[0x3B87F0], 0x0, 0xFFFFFFFF, 0);

		CString errors = n64AudioLibrary.CompareALBanks(results[1].bank, GEBank);

		for (int x = 0; x < GEBank->count; x++)
		{
			results[1].bank->inst[x] = GEBank->inst[x];
		}
		delete [] geTempRom;*/
		

		for (int x = 0; x < numberResults; x++)
		{
			CString tempStr;
			tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);


			mSoundBankIndex.AddString(tempStr);
		}

		mSoundBankIndex.SetCurSel(0);

		OnCbnSelchangeCombosoundbank();
	}
	catch (char * str)
	{
		CString tempErrStr;
		tempErrStr.Format("%s", str);
		MessageBox(tempErrStr, LoadResourceText(IDS_STRINGERRORPROCESSING));

		numberResults = 0;
	}
}


BOOL CN64SoundListToolDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	if ((nID >= STARTMENUOPTIONS) && (nID < (STARTMENUOPTIONS + countGames)))
	{
		if (nCode == 0)
		{
			int selGame = (nID - STARTMENUOPTIONS);
			OpenROMSpecific(gameConfig[selGame]);
			return TRUE;
		}
	}
 
    // otherwise, do default handling

	return CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CN64SoundListToolDlg::OnFileExtractallknowngames()
{
	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	

	for (int y = 0x0; y < countGames; y++)
	{
		if (!OpenROMFilename((tempPath + gameConfig[y].gameName + " [!].z64"), true))
			continue;


		CString outputFolder = (tempPath + gameConfig[y].gameName + "\\");
		
		::CreateDirectory(outputFolder, NULL);

		mSoundBankIndex.ResetContent();

		try
		{
			for (int x = 0; x < t1results.size(); x++)
			{
				n64AudioLibrary.DisposeT1Bank(t1results[x].bank);
			}

			t1results.clear();

			for (int x = 0; x < numberResults; x++)
			{
				n64AudioLibrary.DisposeALBank(results[x].bank);
			}

			alBankCurrent = NULL;
			t1BankCurrent = NULL;
			subBankCurrent = -1;

			numberResults = 0;

			results.clear();

			results.resize(gameConfig[y].numberSoundBanks);

			if (gameConfig[y].gameType.CompareNoCase("MultiPartERZN64WavePtrV2") == 0)
			{
				unsigned char* ctlBinaryPre = new unsigned char[0x2000000];
				unsigned long  ctlSize = 0;

				for (int x = 0; x < gameConfig[y].numberSoundBanks; x++)
				{
					RncDecoder rnc;
					int fileSizeCompressed = romSize - gameConfig[y].soundBanks[x].ctl;
					if (fileSizeCompressed > 0xFE000)
						fileSizeCompressed = 0xFE000;
					unsigned char* outputDecompressed = new unsigned char[0x2000000];
					ROM[gameConfig[y].soundBanks[x].ctl] = 0x52;
					ROM[gameConfig[y].soundBanks[x].ctl+1] = 0x4E;
					ROM[gameConfig[y].soundBanks[x].ctl+2] = 0x43;
					int fileSize = rnc.unpackM1(&ROM[gameConfig[y].soundBanks[x].ctl], outputDecompressed, 0, fileSizeCompressed);
					memcpy(&ctlBinaryPre[ctlSize], outputDecompressed, fileSize);
					ctlSize += fileSize;
					delete [] outputDecompressed;
				}

				unsigned char* ctlBinary = new unsigned char[ctlSize - gameConfig[y].soundBanks[0].mask];
				memcpy(ctlBinary, &ctlBinaryPre[gameConfig[y].soundBanks[0].mask], (ctlSize - gameConfig[y].soundBanks[0].mask));
				delete [] ctlBinaryPre;
				ctlSize = ctlSize - gameConfig[y].soundBanks[0].mask;

				results[numberResults].ctlOffset = gameConfig[y].soundBanks[0].ctl;
				results[numberResults].ctlSize = ctlSize;
				results[numberResults].tblOffset = gameConfig[y].soundBanks[0].tbl;
				results[numberResults].tblSize = romSize - results[0].tblOffset;
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinary, ctlSize, 0, &ROM[results[0].tblOffset]);
				delete [] ctlBinary;
				numberResults = 1;
			}
			else if (gameConfig[y].gameType.CompareNoCase("MultiPartTigWavePtrV2") == 0)
			{
				unsigned char* ctlBinaryPre = new unsigned char[0x2000000];
				unsigned long  ctlSize = 0;

				int currentIndex = -1;

				int start = 0;

				for (int x = 0; x < gameConfig[y].numberSoundBanks; x++)
				{
					if (gameConfig[y].soundBanks[x].numberInstruments < currentIndex)
					{
						results[numberResults].ctlOffset = gameConfig[y].soundBanks[start].ctl;
						results[numberResults].ctlSize = ctlSize;
						results[numberResults].tblOffset = gameConfig[y].soundBanks[0].tbl;
						results[numberResults].tblSize = romSize - results[start].tblOffset;
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinaryPre, ctlSize, 0, &ROM[results[start].tblOffset]);
						delete [] ctlBinaryPre;
						numberResults++;

						start = x;
						// start a new one

						ctlBinaryPre = new unsigned char[0x2000000];
						ctlSize = 0;
					}

					TigDecoder tig;
					int fileSizeCompressed = romSize - gameConfig[y].soundBanks[x].ctl;
					if (fileSizeCompressed > 0xFE000)
						fileSizeCompressed = 0xFE000;
					unsigned char* outputDecompressed = new unsigned char[0x2000000];
					
					unsigned long address = gameConfig[y].soundBanks[x].ctl;
					unsigned long compressedsize = ((((((ROM[address+3] << 8) | ROM[address+2]) << 8) | ROM[address+1]) << 8) | ROM[address+0]);
					unsigned char type = ROM[address+4];

					if (type == 1) // no compression
					{
						memcpy(&ctlBinaryPre[ctlSize], &ROM[address+8], compressedsize);
						ctlSize += compressedsize;
					}
					else if (type == 0) 
					{
						int fileSize = tig.dec(&ROM[address+8], fileSizeCompressed, outputDecompressed);
						memcpy(&ctlBinaryPre[ctlSize], outputDecompressed, fileSize);
						ctlSize += fileSize;
					}

					delete [] outputDecompressed;
				}

				results[numberResults].ctlOffset = gameConfig[y].soundBanks[start].ctl;
				results[numberResults].ctlSize = ctlSize;
				results[numberResults].tblOffset = gameConfig[y].soundBanks[0].tbl;
				results[numberResults].tblSize = romSize - results[start].tblOffset;
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinaryPre, ctlSize, 0, &ROM[results[start].tblOffset]);
				delete [] ctlBinaryPre;
				numberResults++;
			}
			else
			{
				for (int x = 0; x < gameConfig[y].numberSoundBanks; x++)
				{
					results[numberResults].ctlOffset = gameConfig[y].soundBanks[x].ctl;
					results[numberResults].tblOffset = gameConfig[y].soundBanks[x].tbl;

					if (gameConfig[y].soundBanks[x].ctl > gameConfig[y].soundBanks[x].tbl)
					{
						results[numberResults].ctlSize = (romSize - results[numberResults].ctlOffset);
						results[numberResults].tblSize = (results[numberResults].ctlOffset - results[numberResults].tblOffset);
					}
					else
					{
						results[numberResults].ctlSize = (results[numberResults].tblOffset - results[numberResults].ctlOffset);
						results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
					}
					if (gameConfig[y].gameType.CompareNoCase("RawTest") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRawTest(&ROM[0], results[numberResults].ctlSize);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SF64Uncompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioStarFox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, &ROM[0], romSize, gameConfig[y].offsetZeldaCtlTable, gameConfig[y].offsetZeldaTblTable, gameConfig[y].startZeldaCtlData, gameConfig[y].startZeldaTblData, numberResults, gameConfig[y].isCompressedZeldaCtlTblTables, gameConfig[y].compressedZeldaCtlTblTableStart, gameConfig[y].compressedZeldaCtlTblTableEnd);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ZeldaUncompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZelda(results, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, &ROM[0], romSize, gameConfig[y].offsetZeldaCtlTable, gameConfig[y].offsetZeldaTblTable, gameConfig[y].startZeldaCtlData, gameConfig[y].startZeldaTblData, numberResults, gameConfig[y].isCompressedZeldaCtlTblTables, gameConfig[y].compressedZeldaCtlTblTableStart, gameConfig[y].compressedZeldaCtlTblTableEnd, gameConfig[y].isZLib);
					}
					else if (gameConfig[y].gameType.CompareNoCase("TurokUncompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTurok(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, &ROM[0], 0);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ArmyMenCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, false);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SargeArmyMenCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, true);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ArmyMenAirCombatCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], false, false);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SnoWave") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioSno(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("RNCCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRNC(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("RNCDean") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRNCDean(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("DeanTable") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioDeanTable(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("DeanIndividual") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioDeanIndividual(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Clayfighter") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioClayfighter(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("RNXCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRNX(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MegaManN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMegamanN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("N64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("PaperMario") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioPaperMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("B0") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioB0(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("DuckDodgers") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioDuckDodgers(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SydneyWavePtr") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioSydney(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("NinDec") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioNinDec(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("TazHuffmanWavePtr") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTazN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ExciteBikeSAM") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioExciteBikeSAM(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("KobeSAM") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioKobeSAM(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("WDCSFX") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioWDCSFX(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("WDCInstruments") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioWDCInstruments(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ExciteBikeSFX") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioExciteBikeSFX(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ExciteBikeSNG") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioExciteBikeSNG(&ROM[0], romSize, results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SouthParkRally") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioSouthParkRally(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("IndividualSouthParkRally") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioIndividualSouthParkRally(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("TwistedEdgeSnowboarding") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTwistedEdgeSnowboarding(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("TwistedEdgeMusic") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTwistedEdgeMusic(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("FightingForce") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioFightingForce(&ROM[0], romSize, results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("RNCN64Wave") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64Ptr(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("AVL_0Wave") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioAVL_0Ptr(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MP3") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMP3(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("RNCN64OffsetWave") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64PtrOffset(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SeparatedBnkB") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioSeparatedBnkB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MaddenBnkB") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMaddenBnkB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Madden64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMadden64(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("BnkB") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioBnkB(&ROM[0], romSize, results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("N64WavePtrTableV1") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV1(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("H20Raw816") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioH20Raw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("TetrisphereRaw816") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTetrisphereRaw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("8BitPCM") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudio8BitPCM(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("16BitPCM") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudio16BitPCM(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Titus") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64Titus(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MKMythologies") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64MKMythologies(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("YAY0N64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2YAY0(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("QuakeN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableQuake2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SnowboardKidsN64WavePtrTable") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableSnowboardKids(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ViewpointN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2Viewpoints(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("BlitzN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableBlitz(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}	
					else if (gameConfig[y].gameType.CompareNoCase("ZLibN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2ZLIB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ZLIBSN64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64Zlib(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SN64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("SuperMario64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MarioKart64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("StandardRawAllowed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRawAllowed(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, 0);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ASMStandardRawAllowed") == 0)
					{
						unsigned long ctlOffset = n64AudioLibrary.ReadAddiuAddress(ROM, results[numberResults].ctlOffset, results[numberResults].tblOffset);
						unsigned long tblOffset = n64AudioLibrary.ReadAddiuAddress(ROM, gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
						unsigned long ctlSize = tblOffset - ctlOffset;

						results[numberResults].ctlOffset = ctlOffset;
						results[numberResults].tblOffset = tblOffset;
						
						results[numberResults].ctlSize = tblOffset - ctlOffset;
				
						if (numberResults < (gameConfig[y].numberSoundBanks - 1))
						{
							unsigned long ctlOffsetNext = n64AudioLibrary.ReadAddiuAddress(ROM, gameConfig[y].soundBanks[x+1].ctl, gameConfig[y].soundBanks[x+1].tbl);
							results[numberResults].tblSize = ctlOffsetNext - tblOffset;
						}
						else
						{
							results[numberResults].tblSize = romSize - tblOffset;
						}

						results[numberResults].bank = n64AudioLibrary.ReadAudioRawAllowed(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0, 0);
					}
					else if (gameConfig[y].gameType.CompareNoCase("KonamiCtl") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioKonami(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, gameConfig[y].soundBanks[x].extra, gameConfig[y].soundBanks[x].extra2);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Konami8Ctl") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioKonami8(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, gameConfig[y].soundBanks[x].extra, gameConfig[y].soundBanks[x].extra2);
					}
					else if (gameConfig[y].gameType.CompareNoCase("64dd") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudio64dd(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("vox") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioVox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Wav") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioWav(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MORT") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMORT(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Kobe2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioKobe2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("NamcoMuseum") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioNamcoMuseum(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("TopGearHyperBike") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTopGearHyperBike(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("TopGearOverdrive") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTopGearOverdrive(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset);
					}
					else if (gameConfig[y].gameType.CompareNoCase("BanjoTooie") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioBanjoTooie(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Conker") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioConker(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, gameConfig[y].soundBanks[x].numberInstruments, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("LzMusyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioLzMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ZLib78DAMusyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx78DA(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("ZLibMusyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, GAUNTLETLEGENDS);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MusyxREZLib") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, RESIDENTEVIL2);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MusyxSmallZlib") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyxSmall(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("Musyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.CompareNoCase("BlastCorps") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioBlastCorps(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.CompareNoCase("MarioParty2E") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMarioParty2E(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0);
					}
					else
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig[y].soundBanks[x].numberInstruments);
					}

					results[numberResults].halfSamplingRate = gameConfig[y].soundBanks[x].halfSamplingRate;

					if (results[numberResults].bank != NULL)
					{
						for (int c = 0; c < gameConfig[y].soundBanks[x].instrumentNames.size(); c++)
						{
							if (c <= results[numberResults].bank->count)
							{
								if (results[numberResults].bank->inst[c] != NULL)
								{
									if (results[numberResults].bank->inst[c]->name == "")
										results[numberResults].bank->inst[c]->name = gameConfig[y].soundBanks[x].instrumentNames[c];
								}
							}
							else
								break;
						}
					}
					numberResults++;
				}
			}

			for (int x = 0; x < gameConfig[y].t1Config.size(); x++)
			{
				if ((gameConfig[y].t1Config[x].t1Start != 0) && (gameConfig[y].t1Config[x].t1End != 0))
				{
					T1Bank* t1Bank = n64AudioLibrary.ReadT1Bank(ROM, gameConfig[y].t1Config[x].t1Start, gameConfig[y].t1Config[x].t1End);
					if (t1Bank != NULL)
					{
						t1Result resultT1;
						resultT1.bank = t1Bank;
						resultT1.t1Start = gameConfig[y].t1Config[x].t1Start;
						resultT1.t1End = gameConfig[y].t1Config[x].t1End;
						resultT1.bankStart = gameConfig[y].t1Config[x].bankReferenceStart;
						resultT1.bankEnd = gameConfig[y].t1Config[x].bankReferenceEnd;
						t1results.push_back(resultT1);
					}
				}
			}

			for (int x = 0; x < numberResults; x++)
			{
					CString tempStr;
				tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);
				
				/*FILE* outFile = fopen("C:\\temp\\predictorset.bin", "ab");

				for (int y = 0; y < results[x].bank->count; y++)
				{
					if (results[x].bank->inst[y] != NULL)
					{
						for (int z = 0; z < results[x].bank->inst[y]->soundCount; z++)
						{
							if (results[x].bank->inst[y]->sounds[z] != NULL)
							{
								if (results[x].bank->inst[y]->sounds[z]->wav.adpcmWave != NULL)
								{
									if (results[x].bank->inst[y]->sounds[z]->wav.adpcmWave->book != NULL)
									{
										if (results[x].bank->inst[y]->sounds[z]->wav.adpcmWave->book->predictors != NULL)
										{
											for (int r = 0; r < (results[x].bank->inst[y]->sounds[z]->wav.adpcmWave->book->npredictors * results[x].bank->inst[y]->sounds[z]->wav.adpcmWave->book->order * 8); r++)
											{
												unsigned short tempShort = (((results[x].bank->inst[y]->sounds[z]->wav.adpcmWave->book->predictors[r] >> 8) & 0xFF)  | ((results[x].bank->inst[y]->sounds[z]->wav.adpcmWave->book->predictors[r] << 8) & 0xFF00));
												fwrite(&tempShort, 1, 2, outFile);
											}
										}
									}
								}
							}
						}
					}
				}

				fclose(outFile);

				FILE* inFile2 = fopen("C:\\temp\\predictorset.bin", "rb");
				fseek(inFile2, 0, SEEK_END);
				int size = ftell(inFile2);
				fseek(inFile2, 0, SEEK_SET);
				unsigned char* buffer2 = new unsigned char[size];
				fread(buffer2, 1, (size), inFile2);
				
				fclose(inFile2);
				

				FILE* outFile2 = fopen("C:\\temp\\predictorset.bin", "wb");
				for (int x = 0; x < size; x+=0x20)
				{
					bool write = true;
					for (int y = 0; y < x; y+=0x20)
					{
						bool found = true;
						for (int z = 0; z < 0x20; z++)
						{
							if (buffer2[x+z] != buffer2[y+z])
								found = false;
						}
						
						if (found)
						{
							write = false;
							break;
						}
					}

					if (write)
					{
						fwrite(&buffer2[x], 1, 0x20, outFile2);
					}
				}
				fclose(outFile2);

				delete [] buffer2;*/

				mSoundBankIndex.AddString(tempStr);
			}

			mSoundBankIndex.SetCurSel(0);

			OnCbnSelchangeCombosoundbank();

			for (int x = 0; x < numberResults; x++)
			{
				float sampleRate = results[x].bank->samplerate;
				for (int r = 0; r < results[x].bank->count; r++)
				{
					for (int z = 0; z < results[x].bank->inst[r]->soundCount; z++)
					{
						if (results[x].bank->inst[r]->samplerate != 0)
							sampleRate = results[x].bank->inst[r]->samplerate;
						CString tempExportNameStr;
						if (mExportShortFilename.GetCheck())
							tempExportNameStr.Format("%s\\B%02XI%02XS%02X.wav", outputFolder, x, r, z);
						else
							tempExportNameStr.Format("%s\\BANK_%02X_INSTR_%04X_SND_%04X.wav", outputFolder, x, r, z);

						n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr, sampleRate, PRIMARY, mHalfSamplingRate.GetCheck());
						if ((results[x].bank->inst[r] != NULL) && (results[x].bank->inst[r]->sounds[z] != NULL))
						{
							if (results[x].bank->inst[r]->sounds[z]->hasWavePrevious)
								n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Prev.wav", sampleRate, PREVIOUS, mHalfSamplingRate.GetCheck());
							if (results[x].bank->inst[r]->sounds[z]->hasWaveSecondary)
								n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Sec.wav", sampleRate, SECONDARY, mHalfSamplingRate.GetCheck());
						}
					}
				}
			}
		}
		catch (char * str)
		{
			CString tempErrStr;
			tempErrStr.Format("%s", str);
			MessageBox(tempErrStr, LoadResourceText(IDS_STRINGERRORPROCESSING));

			numberResults = 0;
		}	
	}
}
void CN64SoundListToolDlg::OnFileOpenromn64ptrwavetable2()
{
	if (!OpenROMPlusDlg())
		return;


	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		alBankCurrent = NULL;
		t1BankCurrent = NULL;
		subBankCurrent = -1;

		COpenSpecificCtlTbl tempDlg = new COpenSpecificCtlTbl();
		if (tempDlg.DoModal() != IDCANCEL)
		{
			unsigned long ctl = tempDlg.GetCtl();
			unsigned long tbl = tempDlg.GetTbl();

			numberResults = 0;

			results[numberResults].ctlOffset = ctl;
			results[numberResults].tblOffset = tbl;
			if (tbl > ctl)
			{
				results[numberResults].ctlSize = (results[numberResults].tblOffset - results[numberResults].ctlOffset);
				results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
			}
			else
			{
				results[numberResults].ctlSize = (romSize - results[numberResults].ctlOffset);
				results[numberResults].tblSize = (results[numberResults].ctlOffset - results[numberResults].tblOffset);
			}
			results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);

			numberResults++;

			for (int x = 0; x < numberResults; x++)
			{
				CString tempStr;
				tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);
				mSoundBankIndex.AddString(tempStr);
			}

			mSoundBankIndex.SetCurSel(0);

			OnCbnSelchangeCombosoundbank();
		}
	}
	catch (char * str)
	{
		CString tempErrStr;
		tempErrStr.Format("%s", str);
		MessageBox(tempErrStr, LoadResourceText(IDS_STRINGERRORPROCESSING));

		numberResults = 0;
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonrip3()
{
	CString outputFolder;
	if (GetFolder(outputFolder, "Folder To Save To", NULL))
	{
		for (int x = 0; x < numberResults; x++)
		{
			float sampleRate = (results[x].bank->samplerate);

			if (results[x].bank->percussion != NULL)
			{
				for (int z = 0; z < results[x].bank->percussion->soundCount; z++)
				{
					if (results[x].bank->samplerate != 0)
						sampleRate = (alBankCurrent->samplerate);

					CString tempExportNameStr;
					if (mExportShortFilename.GetCheck())
						tempExportNameStr.Format("%s\\B%02XP%02X.wav", outputFolder, x, z);
					else
						tempExportNameStr.Format("%s\\BANK_%02X__PERC_%04X.wav", outputFolder, x, z);
					n64AudioLibrary.ExtractPercussion(mainFolder, results[x].bank, z, tempExportNameStr, sampleRate, results[x].halfSamplingRate);
				}
			}

			for (int z = 0; z < results[x].bank->countEADPercussion; z++)
			{
				if (results[x].bank->samplerate != 0)
					sampleRate = (alBankCurrent->samplerate);

				CString tempExportNameStr;
				if (mExportShortFilename.GetCheck())
					tempExportNameStr.Format("%s\\B%02XP%02X.wav", outputFolder, x, z);
				else
					tempExportNameStr.Format("%s\\BANK_%02X__PERC_%04X.wav", outputFolder, x, z);
				n64AudioLibrary.ExtractEADPercussion(results[x].bank, z, tempExportNameStr, sampleRate, results[x].halfSamplingRate);
			}

			for (int z = 0; z < results[x].bank->countSfx; z++)
			{
				if (results[x].bank->samplerate != 0)
					sampleRate = (alBankCurrent->samplerate);

				CString tempExportNameStr;
				if (mExportShortFilename.GetCheck())
					tempExportNameStr.Format("%s\\B%02XF%02X.wav", outputFolder, x, z);
				else
					tempExportNameStr.Format("%s\\BANK_%02X__SFX_%04X.wav", outputFolder, x, z);
				n64AudioLibrary.ExtractSfx(results[x].bank, z, tempExportNameStr, sampleRate, results[x].halfSamplingRate);
			}

			for (int r = 0; r < results[x].bank->count; r++)
			{
				for (int z = 0; z < results[x].bank->inst[r]->soundCount; z++)
				{
					if (results[x].bank->inst[r]->samplerate != 0)
						sampleRate = (alBankCurrent->inst[r]->samplerate);

					CString tempExportNameStr;
					if (alBankCurrent->soundBankFormat == MP3)
					{
						if (mExportShortFilename.GetCheck())
							tempExportNameStr.Format("%s\\B%02XI%02XS%02X.mp3", outputFolder, x, r, z);
						else
							tempExportNameStr.Format("%s\\BANK_%02X_INSTR_%04X_SND_%04X.mp3", outputFolder, x, r, z);
					}
					else
					{
						if (mExportShortFilename.GetCheck())
							tempExportNameStr.Format("%s\\B%02XI%02XS%02X.wav", outputFolder, x, r, z);
						else
							tempExportNameStr.Format("%s\\BANK_%02X_INSTR_%04X_SND_%04X.wav", outputFolder, x, r, z);
					}

					float sampleRatePrimary = sampleRate;
					if (mUseT1.GetCheck())
					{
						if (mT1Index.GetCount() > 0)
						{
							if (t1BankCurrent != NULL)
							{
								int bankIndex = mSoundBankIndex.GetCurSel();

								for (int t = 0; t < t1BankCurrent->t1Entries.size(); t++)
								{
									if (
										(t1BankCurrent->t1Entries[t].bankNumber == bankIndex)
										&& (t1BankCurrent->t1Entries[t].instrumentNumber == r)
										&& (t1BankCurrent->t1Entries[t].soundNumber == z)
										)
									{
										sampleRatePrimary = t1BankCurrent->t1Entries[t].frequency;
										break;
									}
								}
							}
						}
					}

					//if (results[x].bank->inst[r]->sounds[z]->wav.adpcmWave->loop != NULL)
						n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr, sampleRatePrimary, PRIMARY, results[x].halfSamplingRate);
					if ((results[x].bank->inst[r] != NULL) && (results[x].bank->inst[r]->sounds[z] != NULL))
					{
						if (results[x].bank->inst[r]->sounds[z]->hasWavePrevious)
							//if (results[x].bank->inst[r]->sounds[z]->wavPrevious.adpcmWave->loop != NULL)
								n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Prev.wav", sampleRate, PREVIOUS, results[x].halfSamplingRate);
							if (results[x].bank->inst[r]->sounds[z]->hasWaveSecondary)
								//if (results[x].bank->inst[r]->sounds[z]->wavSecondary.adpcmWave->loop != NULL)
									n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Sec.wav", sampleRate, SECONDARY, results[x].halfSamplingRate);
					}
				}
			}
		}
	}
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosoundsubsound()
{
	CString tempStr;

	if (percussionMode == NORMAL)
	{
		CString subSoundStr;
		mSubSound.GetWindowText(subSoundStr);

		byte primSel = PRIMARY;
		if (subSoundStr == "Primary")
			primSel = PRIMARY;
		else if (subSoundStr == "Previous")
			primSel = PREVIOUS;
		else if (subSoundStr == "Secondary")
			primSel = SECONDARY;


		if (dontupdateitall)
		{
			dontupdateitall = false;

			int instrSel = mInstrumentChoice.GetCurSel();
			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
			{
				dontupdateitall = true;
				return;
			}

			if (alBankCurrent->inst[instrSel]->sounds[soundChoice] != NULL)
			{
				if (primSel == PRIMARY)
				{
					if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_MADDENBNKB)
						mHalfVADPCMPrecision.SetCheck(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.wavFlags == 0x01);
					else
						mHalfVADPCMPrecision.SetCheck(((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.wavFlags & 0x30) == 0x30));
					mCheckUnknownEADFlag.SetCheck(((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.wavFlags & 0x02) == 0x02));
					tempStr.Format("%02X", ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.wavFlags & 0xC) >> 2));
					mTableIndex.SetWindowText(tempStr);

					if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_ADPCM_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop != NULL));
						mLoopingEnabled.EnableWindow(true);

						if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(true);
						m_importPredictors.EnableWindow(true);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_RAW16_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop != NULL));

						mLoopingEnabled.EnableWindow(true);
						if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave != NULL) &&(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.rawWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_VOX_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_WAV)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_MORT_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_KOBE2_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_NAMCOMUSEUM)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_TOPGEARHYPERBIKE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_ACCLAIM_DEAN)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_ACCLAIM_MARK)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_STARFOX_SOUND)
					{
						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);
						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_MP3)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if (
						(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_EXCITEBIKE_SAM)
						||
						(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_EXCITEBIKE_SFX)
						||
						(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_EXCITEBIKE_SNG)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_WDC)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_SOUTHPARKRALLY)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_TWISTEDEDGESNOWBOARDING)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_TWISTEDEDGEMUSIC)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if (
						(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_MADDENBNKB)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if (
						(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_MADDEN64)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_MUSYX_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);

						if (
							((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop != NULL))
							&&
							(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->start != alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->end)
							)
						{
							mLoopingEnabled.SetCheck((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop != NULL));
							mLoopingEnabled.EnableWindow(true);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);	
						}
						else
						{
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);
						}

						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}

					CString tempStr;
					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.base);
					mBase.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.len);
					mLength.SetWindowText(tempStr);
				}
				else if (primSel == PREVIOUS)
				{
					mHalfVADPCMPrecision.SetCheck(((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.wavFlags & 0x30) == 0x30));
					mCheckUnknownEADFlag.SetCheck(((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.wavFlags & 0x02) == 0x02));
					
					tempStr.Format("%02X", ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.wavFlags & 0xC) >> 2));
					mTableIndex.SetWindowText(tempStr);
					if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.type == AL_ADPCM_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop != NULL));
						mLoopingEnabled.EnableWindow(true);

						if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(true);
						m_importPredictors.EnableWindow(true);
					}
					else
					{
						mLoopingEnabled.SetCheck((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave->loop != NULL));

						mLoopingEnabled.EnableWindow(true);
						if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}

					CString tempStr;
					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.base);
					mBase.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.len);
					mLength.SetWindowText(tempStr);
				}
				else if (primSel == SECONDARY)
				{
					mHalfVADPCMPrecision.SetCheck(((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.wavFlags & 0x30) == 0x30));
					mCheckUnknownEADFlag.SetCheck(((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.wavFlags & 0x02) == 0x02));
					tempStr.Format("%02X", ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.wavFlags & 0xC) >> 2));
					mTableIndex.SetWindowText(tempStr);
					if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.type == AL_ADPCM_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop != NULL));
						mLoopingEnabled.EnableWindow(true);

						if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(true);
						m_importPredictors.EnableWindow(true);
					}
					else
					{
						mLoopingEnabled.SetCheck((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave != NULL) && (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave->loop != NULL));

						mLoopingEnabled.EnableWindow(true);
						if ((alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave != NULL) &&(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}

					CString tempStr;
					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.base);
					mBase.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.len);
					mLength.SetWindowText(tempStr);
				}
			}

			dontupdateitall = true;
		}
	}
	else if (percussionMode == PERCUSSION)
	{
		CString subSoundStr;
		mSubSound.GetWindowText(subSoundStr);

		byte primSel = PRIMARY;
		if (subSoundStr == "Primary")
			primSel = PRIMARY;
		else if (subSoundStr == "Previous")
			primSel = PREVIOUS;
		else if (subSoundStr == "Secondary")
			primSel = SECONDARY;


		if (dontupdateitall)
		{
			dontupdateitall = false;

			int soundChoice = mSoundChoice.GetCurSel();

			if (soundChoice == -1)
			{
				dontupdateitall = true;
				return;
			}

			if (alBankCurrent->percussion->sounds[soundChoice] != NULL)
			{
				if (primSel == PRIMARY)
				{
					mHalfVADPCMPrecision.SetCheck(((alBankCurrent->percussion->sounds[soundChoice]->wav.wavFlags & 0x30) == 0x30));
					mCheckUnknownEADFlag.SetCheck(((alBankCurrent->percussion->sounds[soundChoice]->wav.wavFlags & 0x02) == 0x02));
					tempStr.Format("%02X", ((alBankCurrent->percussion->sounds[soundChoice]->wav.wavFlags & 0xC) >> 2));
					mTableIndex.SetWindowText(tempStr);

					if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_ADPCM_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->percussion->sounds[soundChoice]->wav.adpcmWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wav.adpcmWave->loop != NULL));
						mLoopingEnabled.EnableWindow(true);

						if ((alBankCurrent->percussion->sounds[soundChoice]->wav.adpcmWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wav.adpcmWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.adpcmWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.adpcmWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.adpcmWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(true);
						m_importPredictors.EnableWindow(true);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_RAW16_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->percussion->sounds[soundChoice]->wav.rawWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wav.rawWave->loop != NULL));

						mLoopingEnabled.EnableWindow(true);
						if ((alBankCurrent->percussion->sounds[soundChoice]->wav.rawWave != NULL) &&(alBankCurrent->percussion->sounds[soundChoice]->wav.rawWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.rawWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.rawWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wav.rawWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_VOX_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_WAV)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_MORT_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_KOBE2_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_NAMCOMUSEUM)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_TOPGEARHYPERBIKE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_ACCLAIM_DEAN)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_ACCLAIM_MARK)
					{
						mLoopCount.ShowWindow(SW_SHOW);
						mLoopEnd.ShowWindow(SW_SHOW);
						mLoopStart.ShowWindow(SW_SHOW);
						mL1Text.ShowWindow(SW_SHOW);
						mL2Text.ShowWindow(SW_SHOW);
						mL3Text.ShowWindow(SW_SHOW);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_STARFOX_SOUND)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_MP3)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if (
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_EXCITEBIKE_SAM)
						||
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_EXCITEBIKE_SFX)
						||
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_EXCITEBIKE_SNG)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if (alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_WDC)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if (
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_SOUTHPARKRALLY)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if (
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_TWISTEDEDGESNOWBOARDING)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if (
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_TWISTEDEDGEMUSIC)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if (
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_MADDENBNKB)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if (
						(alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_MADDEN64)
						)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
					else if ( alBankCurrent->percussion->sounds[soundChoice]->wav.type == AL_MUSYX_WAVE)
					{
						mLoopCount.ShowWindow(SW_HIDE);
						mLoopEnd.ShowWindow(SW_HIDE);
						mLoopStart.ShowWindow(SW_HIDE);
						mL1Text.ShowWindow(SW_HIDE);
						mL2Text.ShowWindow(SW_HIDE);
						mL3Text.ShowWindow(SW_HIDE);

						mImportLoopPredictors.ShowWindow(SW_HIDE);
						mExportLoopPredictors.ShowWindow(SW_HIDE);
					}
				}
				else if (primSel == PREVIOUS)
				{
					mHalfVADPCMPrecision.SetCheck(((alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.wavFlags & 0x30) == 0x30));
					mCheckUnknownEADFlag.SetCheck(((alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.wavFlags & 0x02) == 0x02));
					tempStr.Format("%02X", ((alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.wavFlags & 0xC) >> 2));
					mTableIndex.SetWindowText(tempStr);

					if ( alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.type == AL_ADPCM_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.adpcmWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.adpcmWave->loop != NULL));
						mLoopingEnabled.EnableWindow(true);

						if ((alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.adpcmWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.adpcmWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.adpcmWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.adpcmWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.adpcmWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(true);
						m_importPredictors.EnableWindow(true);
					}
					else
					{
						mLoopingEnabled.SetCheck((alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.rawWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.rawWave->loop != NULL));

						mLoopingEnabled.EnableWindow(true);
						if ((alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.rawWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.rawWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.rawWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.rawWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavPrevious.rawWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
				}
				else if (primSel == SECONDARY)
				{
					mHalfVADPCMPrecision.SetCheck(((alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.wavFlags & 0x30) == 0x30));
					mCheckUnknownEADFlag.SetCheck(((alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.wavFlags & 0x02) == 0x02));
					tempStr.Format("%02X", ((alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.wavFlags & 0xC) >> 2));
					mTableIndex.SetWindowText(tempStr);

					if ( alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.type == AL_ADPCM_WAVE)
					{
						mLoopingEnabled.SetCheck((alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.adpcmWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.adpcmWave->loop != NULL));
						mLoopingEnabled.EnableWindow(true);

						if ((alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.adpcmWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.adpcmWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.adpcmWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.adpcmWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.adpcmWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(true);
						m_importPredictors.EnableWindow(true);
					}
					else
					{
						mLoopingEnabled.SetCheck((alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.rawWave != NULL) && (alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.rawWave->loop != NULL));

						mLoopingEnabled.EnableWindow(true);
						if ((alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.rawWave != NULL) &&(alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.rawWave->loop != NULL))
						{
							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.rawWave->loop->start);
							mLoopStart.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.rawWave->loop->end);
							mLoopEnd.SetWindowText(tempStr);

							tempStr.Format("%08X", alBankCurrent->percussion->sounds[soundChoice]->wavSecondary.rawWave->loop->count);
							mLoopCount.SetWindowText(tempStr);

							mLoopCount.ShowWindow(SW_SHOW);
							mLoopEnd.ShowWindow(SW_SHOW);
							mLoopStart.ShowWindow(SW_SHOW);
							mL1Text.ShowWindow(SW_SHOW);
							mL2Text.ShowWindow(SW_SHOW);
							mL3Text.ShowWindow(SW_SHOW);

							mImportLoopPredictors.ShowWindow(SW_SHOW);
							mExportLoopPredictors.ShowWindow(SW_SHOW);
						}
						else
						{
							mLoopCount.ShowWindow(SW_HIDE);
							mLoopEnd.ShowWindow(SW_HIDE);
							mLoopStart.ShowWindow(SW_HIDE);
							mL1Text.ShowWindow(SW_HIDE);
							mL2Text.ShowWindow(SW_HIDE);
							mL3Text.ShowWindow(SW_HIDE);

							mImportLoopPredictors.ShowWindow(SW_HIDE);
							mExportLoopPredictors.ShowWindow(SW_HIDE);
						}

						m_exportPredictors.EnableWindow(false);
						m_importPredictors.EnableWindow(false);
					}
				}
			}

			dontupdateitall = true;
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonimportlooppredictors()
{
	if (percussionMode == NORMAL)
	{
		CFileDialog m_ldFile(TRUE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY, "Loop Predictors (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			int fileSize = n64AudioLibrary.GetSizeFile(fileName);
			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);
			
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrumentSel == -1)
				return;

			if (soundSel == -1)
				return;

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavSecondary;
			}

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* inFile = fopen(fileName, "rb");
					if (inFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}

					if (fileSize != 0x20)
					{
						MessageBox(LoadResourceText(IDS_STRINGERRORTOOSMALLFILE), LoadResourceText(IDS_STRINGERROR));
						fclose(inFile);
						return;
					}

					unsigned char* buffer = new unsigned char[fileSize];
					fread(buffer, 1, fileSize, inFile);
					fclose(inFile);

					for (int z = 0; z < 0x10; z++)
					{
						alWave->adpcmWave->loop->state[z] = CharArrayToShort(&buffer[(z * 2)]);
					}

					delete [] buffer;
				}
			}
		}
	}
	else if (percussionMode == PERCUSSION)
	{
		CFileDialog m_ldFile(TRUE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY, "Loop Predictors (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			int fileSize = n64AudioLibrary.GetSizeFile(fileName);
			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);
			
			int soundSel = mSoundChoice.GetCurSel();

			if (soundSel == -1)
				return;

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBankCurrent->percussion->sounds[soundSel]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBankCurrent->percussion->sounds[soundSel]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBankCurrent->percussion->sounds[soundSel]->wavSecondary;
			}

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* inFile = fopen(fileName, "rb");
					if (inFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}

					if (fileSize != 0x20)
					{
						MessageBox(LoadResourceText(IDS_STRINGERRORTOOSMALLFILE), LoadResourceText(IDS_STRINGERROR));
						fclose(inFile);
						return;
					}

					unsigned char* buffer = new unsigned char[fileSize];
					fread(buffer, 1, fileSize, inFile);
					fclose(inFile);

					for (int z = 0; z < 0x10; z++)
					{
						alWave->adpcmWave->loop->state[z] = CharArrayToShort(&buffer[(z * 2)]);
					}

					delete [] buffer;
				}
			}
		}
	}
	else if (percussionMode == EADPERCUSSION)
	{
		CFileDialog m_ldFile(TRUE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY, "Loop Predictors (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			int fileSize = n64AudioLibrary.GetSizeFile(fileName);
			
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			ALWave* alWave = &alBankCurrent->eadPercussion[percussionSel].wav;

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* inFile = fopen(fileName, "rb");
					if (inFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}

					if (fileSize != 0x20)
					{
						MessageBox(LoadResourceText(IDS_STRINGERRORTOOSMALLFILE), LoadResourceText(IDS_STRINGERROR));
						fclose(inFile);
						return;
					}

					unsigned char* buffer = new unsigned char[fileSize];
					fread(buffer, 1, fileSize, inFile);
					fclose(inFile);

					for (int z = 0; z < 0x10; z++)
					{
						alWave->adpcmWave->loop->state[z] = CharArrayToShort(&buffer[(z * 2)]);
					}

					delete [] buffer;
				}
			}
		}
	}
	else if (percussionMode == SFX)
	{
		CFileDialog m_ldFile(TRUE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY, "Loop Predictors (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_ldFile.DoModal();
		CString fileName = m_ldFile.GetPathName();
		if (statusFileOpen == IDOK)
		{
			int fileSize = n64AudioLibrary.GetSizeFile(fileName);
			
			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			ALWave* alWave = alBankCurrent->alSfx[sfxSel];

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* inFile = fopen(fileName, "rb");
					if (inFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}

					if (fileSize != 0x20)
					{
						MessageBox(LoadResourceText(IDS_STRINGERRORTOOSMALLFILE), LoadResourceText(IDS_STRINGERROR));
						fclose(inFile);
						return;
					}

					unsigned char* buffer = new unsigned char[fileSize];
					fread(buffer, 1, fileSize, inFile);
					fclose(inFile);

					for (int z = 0; z < 0x10; z++)
					{
						alWave->adpcmWave->loop->state[z] = CharArrayToShort(&buffer[(z * 2)]);
					}

					delete [] buffer;
				}
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonexportlooppredictors()
{
	if (percussionMode == NORMAL)
	{
		CFileDialog m_svFile(FALSE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();
		if ((statusFileOpen == IDOK) && (fileName != ""))
		{

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);
			
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrumentSel == -1)
				return;

			if (soundSel == -1)
				return;

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavSecondary;
			}

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
					if (outFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}


					for (int z = 0; z < 0x10; z++)
					{
						unsigned short tempShort = Flip16Bit(alWave->adpcmWave->loop->state[z]);
						fwrite(&tempShort, 1, 2, outFile);
					}

					fclose(outFile);
				}
			}
		}	
	}
	else if (percussionMode == PERCUSSION)
	{
		CFileDialog m_svFile(FALSE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();
		if ((statusFileOpen == IDOK) && (fileName != ""))
		{

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);
			
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrumentSel == -1)
				return;

			if (soundSel == -1)
				return;

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			ALWave* alWave = NULL;
			if (primSel == PRIMARY)
			{
				alWave = &alBankCurrent->percussion->sounds[soundSel]->wav;
			}
			else if (primSel == PREVIOUS)
			{
				alWave = &alBankCurrent->percussion->sounds[soundSel]->wavPrevious;
			}
			else if (primSel == SECONDARY)
			{
				alWave = &alBankCurrent->percussion->sounds[soundSel]->wavSecondary;
			}

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
					if (outFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}


					for (int z = 0; z < 0x10; z++)
					{
						unsigned short tempShort = Flip16Bit(alWave->adpcmWave->loop->state[z]);
						fwrite(&tempShort, 1, 2, outFile);
					}

					fclose(outFile);
				}
			}
		}	
	}
	else if (percussionMode == EADPERCUSSION)
	{
		CFileDialog m_svFile(FALSE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();
		if ((statusFileOpen == IDOK) && (fileName != ""))
		{

			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			ALWave* alWave = &alBankCurrent->eadPercussion[percussionSel].wav;

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
					if (outFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}


					for (int z = 0; z < 0x10; z++)
					{
						unsigned short tempShort = Flip16Bit(alWave->adpcmWave->loop->state[z]);
						fwrite(&tempShort, 1, 2, outFile);
					}

					fclose(outFile);
				}
			}
		}	
	}
	else if (percussionMode == SFX)
	{
		CFileDialog m_svFile(FALSE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();
		if ((statusFileOpen == IDOK) && (fileName != ""))
		{

			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			ALWave* alWave = alBankCurrent->alSfx[sfxSel];

			if (alWave->type == AL_ADPCM_WAVE)
			{
				if (alWave->adpcmWave->loop != NULL)
				{
					FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
					if (outFile == NULL)
					{
						MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
						return;
					}


					for (int z = 0; z < 0x10; z++)
					{
						unsigned short tempShort = Flip16Bit(alWave->adpcmWave->loop->state[z]);
						fwrite(&tempShort, 1, 2, outFile);
					}

					fclose(outFile);
				}
			}
		}	
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddprev()
{
	if (percussionMode == NORMAL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		if (soundChoice == -1)
		{
			return;
		}
		


		if (alBankCurrent->inst[instrSel]->sounds[soundChoice] != NULL)
		{
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWavePrevious = true;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave = new ALADPCMWaveInfo();
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop = new ALADPCMloop();
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->count = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->count;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->end = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->end;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->start = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->start;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->unknown1 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->unknown1;
			for (int x = 0; x < 16; x++)
				alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop->state[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->state[x];

			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book = new ALADPCMBook();
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->npredictors = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->book->npredictors;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->order = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->book->order;

			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->predictors = new signed short[(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->npredictors * alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->order * 8)];
			for (int x = 0; x < (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->npredictors * alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->order * 8); x++)
					alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->predictors[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->book->predictors[x];

			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.base = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.base;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.flags = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.flags;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.len = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.len;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.rawWave = NULL;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.sampleRateNotInDefaultNintendoSpec = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.sampleRateNotInDefaultNintendoSpec;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.type = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.unknown1 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown1;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.unknown2 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown2;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.unknown3 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown3;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.unknown4 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown4;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.wavData = new unsigned char[alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.len];
			for (int x = 0; x < alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.len; x++)
			{
				alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.wavData[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.wavData[x];
			}
		}

		OnCbnSelchangeCombosound2();
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonremoveprev()
{
	if (percussionMode == NORMAL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		if (soundChoice == -1)
		{
			return;
		}

		if (alBankCurrent->inst[instrSel]->sounds[soundChoice] != NULL)
		{
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWavePrevious = false;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->loop;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book->predictors;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave->book;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.adpcmWave;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavPrevious.wavData;
		}

		OnCbnSelchangeCombosound2();
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddprev2()
{
	if (percussionMode == NORMAL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		if (soundChoice == -1)
		{
			return;
		}
		


		if (alBankCurrent->inst[instrSel]->sounds[soundChoice] != NULL)
		{
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWaveSecondary = true;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave = new ALADPCMWaveInfo();
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop = new ALADPCMloop();
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->count = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->count;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->end = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->end;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->start = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->start;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->unknown1 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->unknown1;
			for (int x = 0; x < 16; x++)
				alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop->state[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->loop->state[x];

			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book = new ALADPCMBook();
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->npredictors = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->book->npredictors;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->order = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->book->order;

			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->predictors = new signed short[(alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->npredictors * alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->order * 8)];
			for (int x = 0; x < (alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->npredictors * alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->order * 8); x++)
					alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->predictors[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.adpcmWave->book->predictors[x];

			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.base = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.base;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.flags = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.flags;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.len = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.len;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.rawWave = NULL;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.sampleRateNotInDefaultNintendoSpec = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.sampleRateNotInDefaultNintendoSpec;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.type = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.unknown1 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown1;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.unknown2 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown2;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.unknown3 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown3;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.unknown4 = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.unknown4;
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.wavData = new unsigned char[alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.len];
			for (int x = 0; x < alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.len; x++)
			{
				alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.wavData[x] = alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.wavData[x];
			}
		}

		OnCbnSelchangeCombosound2();
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonremoveprev2()
{
	if (percussionMode == NORMAL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		if (soundChoice == -1)
		{
			return;
		}

		if (alBankCurrent->inst[instrSel]->sounds[soundChoice] != NULL)
		{
			alBankCurrent->inst[instrSel]->sounds[soundChoice]->hasWaveSecondary = false;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->loop;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book->predictors;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave->book;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.adpcmWave;
			delete [] alBankCurrent->inst[instrSel]->sounds[soundChoice]->wavSecondary.wavData;
		}

		OnCbnSelchangeCombosound2();
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonstopsound()
{
	::PlaySound(NULL, 0, 0);
}

CString CN64SoundListToolDlg::LoadResourceText(int resourceTextId)
{
	CString tempStr;
	tempStr.LoadString(resourceTextId);
	return tempStr;
}
void CN64SoundListToolDlg::OnEnChangeEditbendrange()
{
	UpdateData();
}

void CN64SoundListToolDlg::OnCbnSelchangeCombopercussion()
{
	if (dontupdateitall)
	{
		dontupdateitall = false;

		int percussionSel = mPercussionChoice.GetCurSel();

		if (percussionSel == -1)
		{
			dontupdateitall = true;
			return;
		}

		CString tempStr;
		mSoundChoice.ResetContent();

		int percussionChoice = mPercussionChoice.GetCurSel();

		if (
			(alBankCurrent->soundBankFormat == STANDARDFORMAT)
			|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == CLAYFIGHTER)
			|| (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED)
			|| (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == ZLIBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BLASTCORPSZLBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BANJOTOOIEAUDIO)
			|| (alBankCurrent->soundBankFormat == TUROKFORMAT)
			|| (alBankCurrent->soundBankFormat == MKMYTHOLOGIES)
			|| (alBankCurrent->soundBankFormat == TITUS)
			|| (alBankCurrent->soundBankFormat == NINDEC)
			|| (alBankCurrent->soundBankFormat == KONAMICTL)
			|| (alBankCurrent->soundBankFormat == KONAMI8CTL)
			)
		{
			if (alBankCurrent->percussion->samplerate != 0)
			{
				tempStr.Format("%d", alBankCurrent->percussion->samplerate);
				mSamplingRate.SetWindowText(tempStr);

				UpdateSamplingRateKeyBaseList();
			}

		
			tempStr.Format("%02X", alBankCurrent->percussion->volume);
			mVolume.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->pan);
			mPan.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->priority);
			mPriority.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->tremType);
			mTremType.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->tremRate);
			mTremRate.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->tremDepth);
			mTremDepth.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->tremDelay);
			mTremDelay.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->vibType);
			mVibType.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->vibRate);
			mVibRate.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->vibDepth);
			mVibDepth.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->percussion->vibDelay);
			mVibDelay.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->percussion->vibDelay);
			mBendRange.SetWindowText(tempStr);

			mSoundChoice.ResetContent();

			for (int x = 0; x < alBankCurrent->percussion->soundCount; x++)
			{
				CString tempStr;
				tempStr.Format("%04X", x);
				mSoundChoice.AddString(tempStr);
			}

			mSoundChoice.SetCurSel(0);

			percussionMode = PERCUSSION;
			ShowPercussionControls();

			dontupdateitall = true;

			OnCbnSelchangeCombosound2();
		}
		else
		{
			mHalfVADPCMPrecision.SetCheck(((alBankCurrent->eadPercussion[percussionChoice].wav.wavFlags & 0x30) == 0x30));
			mCheckUnknownEADFlag.SetCheck(((alBankCurrent->eadPercussion[percussionChoice].wav.wavFlags & 0x02) == 0x02));
			tempStr.Format("%02X", ((alBankCurrent->eadPercussion[percussionChoice].wav.wavFlags & 0xC) >> 2));
			mTableIndex.SetWindowText(tempStr);

			tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.base);
			mBase.SetWindowText(tempStr);

			tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.len);
			mLength.SetWindowText(tempStr);

			tempStr.Format("%f", *reinterpret_cast<float*> (&alBankCurrent->eadPercussion[percussionChoice].keyBase));
			mReleaseTime.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->eadPercussion[percussionChoice].pan);
			mAttackVolume.SetWindowText(tempStr);

			if ( alBankCurrent->eadPercussion[percussionChoice].wav.type == AL_ADPCM_WAVE)
			{
				mLoopingEnabled.SetCheck((alBankCurrent->eadPercussion[percussionChoice].wav.adpcmWave != NULL) && (alBankCurrent->eadPercussion[percussionChoice].wav.adpcmWave->loop != NULL));
				mLoopingEnabled.EnableWindow(true);

				if ((alBankCurrent->eadPercussion[percussionChoice].wav.adpcmWave != NULL) && (alBankCurrent->eadPercussion[percussionChoice].wav.adpcmWave->loop != NULL))
				{
					tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.adpcmWave->loop->start);
					mLoopStart.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.adpcmWave->loop->end);
					mLoopEnd.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.adpcmWave->loop->count);
					mLoopCount.SetWindowText(tempStr);

					mLoopCount.ShowWindow(SW_SHOW);
					mLoopEnd.ShowWindow(SW_SHOW);
					mLoopStart.ShowWindow(SW_SHOW);
					mL1Text.ShowWindow(SW_SHOW);
					mL2Text.ShowWindow(SW_SHOW);
					mL3Text.ShowWindow(SW_SHOW);

					mImportLoopPredictors.ShowWindow(SW_SHOW);
					mExportLoopPredictors.ShowWindow(SW_SHOW);
				}
				else
				{
					mLoopCount.ShowWindow(SW_HIDE);
					mLoopEnd.ShowWindow(SW_HIDE);
					mLoopStart.ShowWindow(SW_HIDE);
					mL1Text.ShowWindow(SW_HIDE);
					mL2Text.ShowWindow(SW_HIDE);
					mL3Text.ShowWindow(SW_HIDE);

					mImportLoopPredictors.ShowWindow(SW_HIDE);
					mExportLoopPredictors.ShowWindow(SW_HIDE);
				}

				m_exportPredictors.EnableWindow(true);
				m_importPredictors.EnableWindow(true);
			}
			else if ( alBankCurrent->eadPercussion[percussionChoice].wav.type == AL_RAW16_WAVE)
			{
				mLoopingEnabled.SetCheck((alBankCurrent->eadPercussion[percussionChoice].wav.rawWave != NULL) && (alBankCurrent->eadPercussion[percussionChoice].wav.rawWave->loop != NULL));

				mLoopingEnabled.EnableWindow(true);
				if ((alBankCurrent->eadPercussion[percussionChoice].wav.rawWave != NULL) &&(alBankCurrent->eadPercussion[percussionChoice].wav.rawWave->loop != NULL))
				{
					tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.rawWave->loop->start);
					mLoopStart.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.rawWave->loop->end);
					mLoopEnd.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->eadPercussion[percussionChoice].wav.rawWave->loop->count);
					mLoopCount.SetWindowText(tempStr);

					mLoopCount.ShowWindow(SW_SHOW);
					mLoopEnd.ShowWindow(SW_SHOW);
					mLoopStart.ShowWindow(SW_SHOW);
					mL1Text.ShowWindow(SW_SHOW);
					mL2Text.ShowWindow(SW_SHOW);
					mL3Text.ShowWindow(SW_SHOW);

					mImportLoopPredictors.ShowWindow(SW_SHOW);
					mExportLoopPredictors.ShowWindow(SW_SHOW);
				}
				else
				{
					mLoopCount.ShowWindow(SW_HIDE);
					mLoopEnd.ShowWindow(SW_HIDE);
					mLoopStart.ShowWindow(SW_HIDE);
					mL1Text.ShowWindow(SW_HIDE);
					mL2Text.ShowWindow(SW_HIDE);
					mL3Text.ShowWindow(SW_HIDE);

					mImportLoopPredictors.ShowWindow(SW_HIDE);
					mExportLoopPredictors.ShowWindow(SW_HIDE);
				}

				m_exportPredictors.EnableWindow(false);
				m_importPredictors.EnableWindow(false);
			}

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[0]);
			mSamplePan.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[1]);
			mSampleVol.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[2]);
			mVelMin.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[3]);
			mVelMax.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[4]);
			mKeyMin.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[5]);
			mKeyMax.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[6]);
			mKeyBase.SetWindowText(tempStr);

			tempStr.Format("%04X", alBankCurrent->eadPercussion[percussionChoice].adsrEAD[7]);
			mDetune.SetWindowText(tempStr);

			percussionMode = EADPERCUSSION;
			ShowEADPercussionControls();

			dontupdateitall = true;
		}

		UpdateSamplingRateSelection();

		dontupdateitall = true;
	}
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosfx()
{
	if (dontupdateitall)
	{
		dontupdateitall = false;

		int sfxSel = mSfxChoice.GetCurSel();

		if (sfxSel == -1)
		{
			dontupdateitall = true;
			return;
		}

		CString tempStr;
		mSoundChoice.ResetContent();

		int sfxChoice = mSfxChoice.GetCurSel();

		if (alBankCurrent->alSfx[sfxChoice] != NULL)
		{
			tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->base);
			mBase.SetWindowText(tempStr);

			tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->len);
			mLength.SetWindowText(tempStr);

			tempStr.Format("%f", *reinterpret_cast<float*> (&alBankCurrent->alSfx[sfxChoice]->unknown1));
			mReleaseTime.SetWindowText(tempStr);

			mHalfVADPCMPrecision.SetCheck(((alBankCurrent->alSfx[sfxChoice]->wavFlags & 0x30) == 0x30));
			mCheckUnknownEADFlag.SetCheck(((alBankCurrent->alSfx[sfxChoice]->wavFlags & 0x02) == 0x02));
			tempStr.Format("%02X", ((alBankCurrent->alSfx[sfxChoice]->wavFlags & 0xC) >> 2));
			mTableIndex.SetWindowText(tempStr);

			if ( alBankCurrent->alSfx[sfxChoice]->type == AL_ADPCM_WAVE)
			{
				mLoopingEnabled.SetCheck((alBankCurrent->alSfx[sfxChoice]->adpcmWave != NULL) && (alBankCurrent->alSfx[sfxChoice]->adpcmWave->loop != NULL));
				mLoopingEnabled.EnableWindow(true);

				if ((alBankCurrent->alSfx[sfxChoice]->adpcmWave != NULL) && (alBankCurrent->alSfx[sfxChoice]->adpcmWave->loop != NULL))
				{
					tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->adpcmWave->loop->start);
					mLoopStart.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->adpcmWave->loop->end);
					mLoopEnd.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->adpcmWave->loop->count);
					mLoopCount.SetWindowText(tempStr);

					mLoopCount.ShowWindow(SW_SHOW);
					mLoopEnd.ShowWindow(SW_SHOW);
					mLoopStart.ShowWindow(SW_SHOW);
					mL1Text.ShowWindow(SW_SHOW);
					mL2Text.ShowWindow(SW_SHOW);
					mL3Text.ShowWindow(SW_SHOW);

					mImportLoopPredictors.ShowWindow(SW_SHOW);
					mExportLoopPredictors.ShowWindow(SW_SHOW);
				}
				else
				{
					mLoopCount.ShowWindow(SW_HIDE);
					mLoopEnd.ShowWindow(SW_HIDE);
					mLoopStart.ShowWindow(SW_HIDE);
					mL1Text.ShowWindow(SW_HIDE);
					mL2Text.ShowWindow(SW_HIDE);
					mL3Text.ShowWindow(SW_HIDE);

					mImportLoopPredictors.ShowWindow(SW_HIDE);
					mExportLoopPredictors.ShowWindow(SW_HIDE);
				}

				m_exportPredictors.EnableWindow(true);
				m_importPredictors.EnableWindow(true);
			}
			else if ( alBankCurrent->alSfx[sfxChoice]->type == AL_RAW16_WAVE)
			{
				mLoopingEnabled.SetCheck((alBankCurrent->alSfx[sfxChoice]->rawWave != NULL) && (alBankCurrent->alSfx[sfxChoice]->rawWave->loop != NULL));

				mLoopingEnabled.EnableWindow(true);
				if ((alBankCurrent->alSfx[sfxChoice]->rawWave != NULL) &&(alBankCurrent->alSfx[sfxChoice]->rawWave->loop != NULL))
				{
					tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->rawWave->loop->start);
					mLoopStart.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->rawWave->loop->end);
					mLoopEnd.SetWindowText(tempStr);

					tempStr.Format("%08X", alBankCurrent->alSfx[sfxChoice]->rawWave->loop->count);
					mLoopCount.SetWindowText(tempStr);

					mLoopCount.ShowWindow(SW_SHOW);
					mLoopEnd.ShowWindow(SW_SHOW);
					mLoopStart.ShowWindow(SW_SHOW);
					mL1Text.ShowWindow(SW_SHOW);
					mL2Text.ShowWindow(SW_SHOW);
					mL3Text.ShowWindow(SW_SHOW);

					mImportLoopPredictors.ShowWindow(SW_SHOW);
					mExportLoopPredictors.ShowWindow(SW_SHOW);
				}
				else
				{
					mLoopCount.ShowWindow(SW_HIDE);
					mLoopEnd.ShowWindow(SW_HIDE);
					mLoopStart.ShowWindow(SW_HIDE);
					mL1Text.ShowWindow(SW_HIDE);
					mL2Text.ShowWindow(SW_HIDE);
					mL3Text.ShowWindow(SW_HIDE);

					mImportLoopPredictors.ShowWindow(SW_HIDE);
					mExportLoopPredictors.ShowWindow(SW_HIDE);
				}

				m_exportPredictors.EnableWindow(false);
				m_importPredictors.EnableWindow(false);
			}
		}
		else
		{
			tempStr.Format("%08X", 0);
			mBase.SetWindowText(tempStr);

			tempStr.Format("%08X", 0);
			mLength.SetWindowText(tempStr);

			mHalfVADPCMPrecision.SetCheck(false);
			mCheckUnknownEADFlag.SetCheck(false);
			mTableIndex.SetWindowText(0);

			mLoopCount.ShowWindow(SW_HIDE);
			mLoopEnd.ShowWindow(SW_HIDE);
			mLoopStart.ShowWindow(SW_HIDE);
			mL1Text.ShowWindow(SW_HIDE);
			mL2Text.ShowWindow(SW_HIDE);
			mL3Text.ShowWindow(SW_HIDE);

			mImportLoopPredictors.ShowWindow(SW_HIDE);
			mExportLoopPredictors.ShowWindow(SW_HIDE);

			m_exportPredictors.EnableWindow(false);
			m_importPredictors.EnableWindow(false);
		}

		percussionMode = SFX;
		ShowSfxControls();

		dontupdateitall = true;
	}
}
void CN64SoundListToolDlg::OnBnClickedCheckhalfsamplingrate()
{
	if (alBankCurrent != NULL)
	{
		if (dontupdateitall)
		{
			dontupdateitall = false;

			for (int x = 0; x < results.size(); x++)
			{
				results[x].halfSamplingRate = mHalfSamplingRate.GetCheck();
			}

			dontupdateitall = true;
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedCheckoverriderate()
{
	if (alBankCurrent != NULL)
	{
		if (dontupdateitall)
		{
			dontupdateitall = false;

			for (int x = 0; x < results.size(); x++)
			{
				results[x].overrideSamplingRate = mOverrideRate.GetCheck();
				if (results[x].overrideSamplingRate)
				{
					CString tempStr;
					tempStr.Format("%d", results[x].samplingRate);
					mSamplingRate.SetWindowText(tempStr);
					results[x].overrideSamplingRate = true;
				}
				else
				{
					CString tempStr;
					tempStr.Format("%d", alBankCurrent->samplerate);
					mSamplingRate.SetWindowText(tempStr);
					results[x].overrideSamplingRate = false;
				}
			}

			UpdateSamplingRateKeyBaseList();

			dontupdateitall = true;
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddinstrument()
{
	if (alBankCurrent != NULL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();

		if (instrSel == -1)
			return;

		if (n64AudioLibrary.AddInstrument(alBankCurrent))
		{
			percussionMode = NORMAL;

			mInstrumentChoice.ResetContent();

			for (int x = 0; x < alBankCurrent->count; x++)
			{
				CString tempStr;
				tempStr.Format("%04X", x);
				mInstrumentChoice.AddString(tempStr);
			}

			mInstrumentChoice.SetCurSel(alBankCurrent->count-1);

			OnCbnSelchangeCombosound();
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeleteinstrument()
{
	if (alBankCurrent != NULL)
	{
		int instrSel = mInstrumentChoice.GetCurSel();

		if (instrSel == -1)
			return;

		if (alBankCurrent->count <= 1)
		{
			MessageBox("Must have one instrument");
			return;
		}

		if (n64AudioLibrary.DeleteInstrument(alBankCurrent, instrSel))
		{
			percussionMode = NORMAL;

			mInstrumentChoice.ResetContent();

			for (int x = 0; x < alBankCurrent->count; x++)
			{
				CString tempStr;
				tempStr.Format("%04X", x);
				mInstrumentChoice.AddString(tempStr);
			}

			if (instrSel >= alBankCurrent->count)
				mInstrumentChoice.SetCurSel(alBankCurrent->count-1);
			else
				mInstrumentChoice.SetCurSel(instrSel);

			OnCbnSelchangeCombosound();
		}
	}
}

void CN64SoundListToolDlg::ImportVadpcmSGIWav(int order)
{
	
	CString aifcToolsDir = mainFolder;
	CString soxDir = mainFolder + "SOX.EXE";
	CString tabledesignDir = mainFolder + "tabledesign.exe";
	CString adpcmencDir = mainFolder + "ADPCMENC.EXE";

	if (!IsFileExist(soxDir.GetBuffer(0)))
		return;

	if (!IsFileExist(tabledesignDir.GetBuffer(0)))
		return;

	if (!IsFileExist(adpcmencDir.GetBuffer(0)))
		return;

	CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

	int statusFileOpen = (int) m_ldFile.DoModal();
	CString fileName = m_ldFile.GetPathName();
	if (statusFileOpen == IDOK)
	{
		FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
		if (inFile == NULL)
		{
			MessageBox("Error opening", "Error");
			return;
		}
		fclose(inFile);

		::DeleteFile(aifcToolsDir + m_ldFile.GetFileName());
		::CopyFile(m_ldFile.GetPathName(), aifcToolsDir + m_ldFile.GetFileName(), false);

		//sox %1.wav %1.aiff

		CString ext = m_ldFile.GetFileExt();
		CString minusWav = m_ldFile.GetFileTitle();
		CString path = m_ldFile.GetFolderPath();

		CString batchScriptParamsSox = "\"" + soxDir + "\" \"" + m_ldFile.GetPathName() + "\" \"" + mainFolder + minusWav + ".aiff\"";

		::SetCurrentDirectory(mainFolder);
		if (!hiddenExec(_T(batchScriptParamsSox.GetBuffer()), mainFolder))
		{
			MessageBox("Error converting sox");
			return;
		}

		int aiffSize = CSharedFunctions::GetSizeFile(aifcToolsDir + minusWav + ".aiff");
		if (aiffSize == 0)
		{
			MessageBox("No file spit out aiff");
			return;
		}
		bool hasLoopData = false;
		unsigned char keyBase = 0x3C;
		unsigned long loopStart = 0x00000000;
		unsigned long loopEnd = 0x00000000;
		unsigned long loopCount = 0x00000000;

		unsigned char* wavPCMData;
		int rawLength;
		unsigned long samplingRate;

		if (!n64AudioLibrary.ReadWavData(m_ldFile.GetFileName(), wavPCMData, rawLength, samplingRate, hasLoopData, keyBase, loopStart, loopEnd, loopCount))
		{
			MessageBox("Error reading wav");
			return;
		}

		delete [] wavPCMData;

		if (hasLoopData)
		{
			FILE* inTempAiff = fopen(aifcToolsDir + minusWav + ".aiff", "rb");
			if (inTempAiff == NULL)
			{
				MessageBox("Can't open aiff in file");
				return;
			}
			unsigned char* aifTempFile = new unsigned char[aiffSize];
			fread(aifTempFile, 1, aiffSize, inTempAiff);
			fclose(inTempAiff);

			FILE *outTempAiffFile = fopen(aifcToolsDir + minusWav + ".aiff", "wb");
			if (outTempAiffFile == NULL)
			{
				delete [] aifTempFile;
				MessageBox("Can't open aiff file");
				return;
			}

			fwrite(&aifTempFile[0], 1, 4, outTempAiffFile);

			unsigned long sizeAiffInternal = CharArrayToLong(&aifTempFile[4]);
			sizeAiffInternal += 0x22;

			unsigned long AIFCCOMTsize = CharArrayToLong(&aifTempFile[0x10]) + 8;
			sizeAiffInternal -= AIFCCOMTsize;

			sizeAiffInternal += 0x1C; // INST addition

			sizeAiffInternal = Flip32Bit(sizeAiffInternal);
			fwrite(&sizeAiffInternal, 1, 4, outTempAiffFile);

			// Write AIFF
			fwrite(&aifTempFile[8], 1, 4, outTempAiffFile);

			// write comm section
			unsigned long startCommOffset = 0xC + AIFCCOMTsize;
			unsigned long sizeComm = CharArrayToLong(&aifTempFile[startCommOffset+4]) + 8;

			fwrite(&aifTempFile[startCommOffset], 1, sizeComm, outTempAiffFile);


			// Write MARK section

			unsigned long numberSamples = (((aiffSize-(startCommOffset + sizeComm))+8) / 2);
			if (numberSamples < loopStart)
			{
				MessageBox("Not enough samples for loop start");
				fclose(outTempAiffFile);
				return;
			}

			if (numberSamples < loopEnd)
			{
				fclose(outTempAiffFile);
				MessageBox("Not enough samples for loop end");
				return;
			}

			unsigned long tempWrite = Flip32Bit(0x4D41524B);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x0000001A);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x00020001);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(loopStart);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x044C6F6F);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x70000002);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(loopEnd);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x044C6F6F);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			unsigned short tempShort = Flip16Bit(0x7000);
			fwrite(&tempShort, 1, 2, outTempAiffFile);

			tempWrite = Flip32Bit(0x494E5354);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x00000014);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x3C00007F);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x007F0001);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x00010001);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x00020000);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			tempWrite = Flip32Bit(0x00000000);
			fwrite(&tempWrite, 1, 4, outTempAiffFile);

			// Write rest of file, ignoring the name COMT
			fwrite(&aifTempFile[(startCommOffset + sizeComm)], 1, (aiffSize-(startCommOffset + sizeComm)), outTempAiffFile);

			delete [] aifTempFile;
			fclose(outTempAiffFile);
		}

		//tabledesign -o 2 -s %2 %1.aiff > temp.tb
		//adpcmenc -c temp.tb %1.aiff %1.aifc
		//rem del %1.aiff
		//del temp.tb
		CString orderStr;
		orderStr.Format("%d", order);
		
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;  

		HANDLE h = CreateFile(_T(mainFolder + "temp.tb"),
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			&sa,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL );

		CString batchScriptParamsTblDesign = "\"" + tabledesignDir + "\" -o 2 -s " + orderStr + " \"" + mainFolder + minusWav + ".aiff\"";

		::SetCurrentDirectory(mainFolder);
		if (!hiddenExec(_T(batchScriptParamsTblDesign.GetBuffer()), mainFolder, h))
		{
			CloseHandle(h);

			MessageBox("Error converting table design");
			return;
		}

		CloseHandle(h);

		CString batchScriptParamsAdpcmEnc = "\"" + adpcmencDir + "\" -c \"" + mainFolder + "temp.tb\" \"" + mainFolder + minusWav + ".aiff\" \"" + mainFolder + minusWav + ".aifc\"";

		::SetCurrentDirectory(mainFolder);
		if (!hiddenExec(_T(batchScriptParamsAdpcmEnc.GetBuffer()), mainFolder))
		{
			MessageBox("Error converting adpcm enc");
			return;
		}

		::DeleteFile("temp.tb");

		int aifcSize = CSharedFunctions::GetSizeFile(aifcToolsDir + minusWav + ".aifc");
		if (aifcSize == 0)
		{
			MessageBox("No file spit out aifc");
			return;
		}
		FILE* inFile2 = fopen(aifcToolsDir + minusWav + ".aifc", "rb");
		if (inFile2 == NULL)
		{
			MessageBox("No file spit out aifc");
			return;
		}

		unsigned char* aifcFile = new unsigned char[aifcSize];

		fread(aifcFile, 1, aifcSize, inFile2);
		fclose(inFile2);

		

		FILE* outPred = fopen(mainFolder + "temppred.bin", "wb");
		if (outPred == NULL)
		{
			MessageBox("Can't open temp pred file");
			delete [] aifcFile;
			return;
		}
		order = aifcFile[0x6D];
		int count = aifcFile[0x6F];
		unsigned long tempLong = Flip32Bit(order);
		fwrite(&tempLong, 1, 4, outPred);
		tempLong = Flip32Bit(count);
		fwrite(&tempLong, 1, 4, outPred);
		fwrite(&aifcFile[0x70], 1, (order * count * 0x10), outPred);
		fclose(outPred);

		int startData = 0x70 + (order * count * 0x10) + 0x10;
		unsigned long sizeSsndhunk = CharArrayToLong(&aifcFile[0x70 + (order * count * 0x10) + 4]) - 0x8;

		FILE* outSsnd = fopen(mainFolder + "tempssnd.bin", "wb");
		if (outSsnd == NULL)
		{
			MessageBox("Can't open temp ssnd file");
			return;
		}

		fwrite(&aifcFile[startData], 1, sizeSsndhunk, outSsnd);
		fclose(outSsnd);

		if (alBankCurrent != NULL)
		{
			if (percussionMode == NORMAL)
			{
				int instrumentSel = mInstrumentChoice.GetCurSel();
				int soundSel = mSoundChoice.GetCurSel();

				if (instrumentSel == -1)
					return;

				if (soundSel == -1)
					return;

				if (alBankCurrent->inst[instrumentSel]->sounds[soundSel] == NULL)
					return;
		
				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
				{
					primSel = PRIMARY;
				}
				else if (subSoundStr == "Previous")
				{
					primSel = PREVIOUS;
				}
				else if (subSoundStr == "Secondary")
				{
					primSel = SECONDARY;
				}

				n64AudioLibrary.ImportRawData(alBankCurrent, instrumentSel, soundSel, mainFolder + "tempssnd.bin", primSel);
				n64AudioLibrary.ImportPredictors(alBankCurrent, instrumentSel, soundSel, mainFolder + "temppred.bin", primSel);

				if (subSoundStr == "Primary")
				{
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.wavFlags = alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wav.wavFlags & (0xFF ^ 0x30);
				}
				else if (subSoundStr == "Previous")
				{
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavPrevious.wavFlags = alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavPrevious.wavFlags & (0xFF ^ 0x30);
				}
				else if (subSoundStr == "Secondary")
				{
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavSecondary.wavFlags = alBankCurrent->inst[instrumentSel]->sounds[soundSel]->wavSecondary.wavFlags & (0xFF ^ 0x30);
				}

				if ((startData + sizeSsndhunk) != aifcSize)
				{
					// Has loop
					unsigned long startLoopData = (startData + sizeSsndhunk) + 0x1C;
					unsigned long loopStart = CharArrayToLong(&aifcFile[startLoopData]);
					unsigned long loopEnd = CharArrayToLong(&aifcFile[startLoopData+4]);
					unsigned long loopCount = CharArrayToLong(&aifcFile[startLoopData+8]);

					ALSound* alSound = alBankCurrent->inst[instrumentSel]->sounds[soundSel];
					
					if (alSound->wav.type == AL_RAW16_WAVE)
					{
						if (alSound->wav.rawWave->loop == NULL)
						{
							alSound->wav.rawWave->loop = new ALRawLoop();
						}

						alSound->wav.rawWave->loop->start = loopStart;
						alSound->wav.rawWave->loop->end = loopEnd;
						//alSound->wav.rawWave->loop->count = loopCount;
					}
					else if (alSound->wav.type == AL_ADPCM_WAVE)
					{
						if (alSound->wav.adpcmWave->loop == NULL)
						{
							alSound->wav.adpcmWave->loop = new ALADPCMloop();
						}

						alSound->wav.adpcmWave->loop->start = loopStart;
						alSound->wav.adpcmWave->loop->end = loopEnd;
						//alSound->wav.adpcmWave->loop->count = loopCount;

						for (int z = 0; z < 0x10; z++)
						{
							alSound->wav.adpcmWave->loop->state[z] = CharArrayToShort(&aifcFile[startLoopData+0xC+(z * 2)]);
						}
					}
				}
			}
			else if (percussionMode == PERCUSSION)
			{
				int soundSel = mSoundChoice.GetCurSel();
				if (soundSel == -1)
					return;

				if (alBankCurrent->percussion->sounds[soundSel] == NULL)
					return;

				n64AudioLibrary.ImportPercussionRawData(alBankCurrent, soundSel, mainFolder + "tempssnd.bin");
				n64AudioLibrary.ImportPercussionPredictors(alBankCurrent, soundSel, mainFolder + "temppred.bin");

				if ((startData + sizeSsndhunk) != aifcSize)
				{
					// Has loop
					unsigned long startLoopData = (startData + sizeSsndhunk) + 0x1C;
					unsigned long loopStart = CharArrayToLong(&aifcFile[startLoopData]);
					unsigned long loopEnd = CharArrayToLong(&aifcFile[startLoopData+4]);
					unsigned long loopCount = CharArrayToLong(&aifcFile[startLoopData+8]);

					ALSound* alSound = alBankCurrent->percussion->sounds[soundSel];
					
					alSound->wav.wavFlags = alSound->wav.wavFlags & (0xFF ^ 0x30);

					if (alSound->wav.type == AL_RAW16_WAVE)
					{
						if (alSound->wav.rawWave->loop == NULL)
						{
							alSound->wav.rawWave->loop = new ALRawLoop();
						}

						alSound->wav.rawWave->loop->start = loopStart;
						alSound->wav.rawWave->loop->end = loopEnd;
						//alSound->wav.rawWave->loop->count = loopCount;
					}
					else if (alSound->wav.type == AL_ADPCM_WAVE)
					{
						if (alSound->wav.adpcmWave->loop == NULL)
						{
							alSound->wav.adpcmWave->loop = new ALADPCMloop();
						}

						alSound->wav.adpcmWave->loop->start = loopStart;
						alSound->wav.adpcmWave->loop->end = loopEnd;
						//alSound->wav.adpcmWave->loop->count = loopCount;

						for (int z = 0; z < 0x10; z++)
						{
							alSound->wav.adpcmWave->loop->state[z] = CharArrayToShort(&aifcFile[startLoopData+0xC+(z * 2)]);
						}
					}
				}
			}
			else if (percussionMode == EADPERCUSSION)
			{
				int percussionSel = mPercussionChoice.GetCurSel();

				if (percussionSel == -1)
					return;

				if (alBankCurrent->eadPercussion == NULL)
					return;


				n64AudioLibrary.ImportEADRawPercussionData(alBankCurrent, percussionSel, mainFolder + "tempssnd.bin");
				n64AudioLibrary.ImportEADPercussionPredictors(alBankCurrent, percussionSel, mainFolder + "temppred.bin");

				alBankCurrent->eadPercussion[percussionSel].wav.wavFlags = alBankCurrent->eadPercussion[percussionSel].wav.wavFlags & (0xFF ^ 0x30);

				if ((startData + sizeSsndhunk) != aifcSize)
				{
					// Has loop
					unsigned long startLoopData = (startData + sizeSsndhunk) + 0x1C;
					unsigned long loopStart = CharArrayToLong(&aifcFile[startLoopData]);
					unsigned long loopEnd = CharArrayToLong(&aifcFile[startLoopData+4]);
					unsigned long loopCount = CharArrayToLong(&aifcFile[startLoopData+8]);

					ALWave* alWave = &alBankCurrent->eadPercussion[percussionSel].wav;
					
					if (alWave->type == AL_RAW16_WAVE)
					{
						if (alWave->rawWave->loop == NULL)
						{
							alWave->rawWave->loop = new ALRawLoop();
						}

						alWave->rawWave->loop->start = loopStart;
						alWave->rawWave->loop->end = loopEnd;
						//alWave->rawWave->loop->count = loopCount;
					}
					else if (alWave->type == AL_ADPCM_WAVE)
					{
						if (alWave->adpcmWave->loop == NULL)
						{
							alWave->adpcmWave->loop = new ALADPCMloop();
						}

						alWave->adpcmWave->loop->start = loopStart;
						alWave->adpcmWave->loop->end = loopEnd;
						//alWave->adpcmWave->loop->count = loopCount;

						for (int z = 0; z < 0x10; z++)
						{
							alWave->adpcmWave->loop->state[z] = CharArrayToShort(&aifcFile[startLoopData+0xC+(z * 2)]);
						}
					}
				}
			}
			else if (percussionMode == SFX)
			{
				int sfxSel = mSfxChoice.GetCurSel();

				if (sfxSel == -1)
					return;

				if (alBankCurrent->alSfx == NULL)
					return;

				n64AudioLibrary.ImportRawSfxData(alBankCurrent, sfxSel, mainFolder + "tempssnd.bin");
				n64AudioLibrary.ImportSfxPredictors(alBankCurrent, sfxSel, mainFolder + "temppred.bin");

				alBankCurrent->alSfx[sfxSel]->wavFlags = alBankCurrent->alSfx[sfxSel]->wavFlags & (0xFF ^ 0x30);

				if ((startData + sizeSsndhunk) != aifcSize)
				{
					// Has loop
					unsigned long startLoopData = (startData + sizeSsndhunk) + 0x1C;
					unsigned long loopStart = CharArrayToLong(&aifcFile[startLoopData]);
					unsigned long loopEnd = CharArrayToLong(&aifcFile[startLoopData+4]);
					unsigned long loopCount = CharArrayToLong(&aifcFile[startLoopData+8]);

					ALWave* alWave = alBankCurrent->alSfx[sfxSel];
					
					if (alWave->type == AL_RAW16_WAVE)
					{
						if (alWave->rawWave->loop == NULL)
						{
							alWave->rawWave->loop = new ALRawLoop();
						}

						alWave->rawWave->loop->start = loopStart;
						alWave->rawWave->loop->end = loopEnd;
						//alWave->rawWave->loop->count = loopCount;
					}
					else if (alWave->type == AL_ADPCM_WAVE)
					{
						if (alWave->adpcmWave->loop == NULL)
						{
							alWave->adpcmWave->loop = new ALADPCMloop();
						}

						alWave->adpcmWave->loop->start = loopStart;
						alWave->adpcmWave->loop->end = loopEnd;
						//alWave->adpcmWave->loop->count = loopCount;

						for (int z = 0; z < 0x10; z++)
						{
							alWave->adpcmWave->loop->state[z] = CharArrayToShort(&aifcFile[startLoopData+0xC+(z * 2)]);
						}
					}
				}
			}
		}

		delete [] aifcFile;

		::DeleteFile(aifcToolsDir + minusWav + ".aifc");
		::DeleteFile(aifcToolsDir + minusWav + ".aiff");

		::SetCurrentDirectory(mainFolder);
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonimportsdktools()
{
	ImportVadpcmSGIWav(0);
}

void CN64SoundListToolDlg::OnBnClickedButtonkonamiadsrbutton()
{
	CKonamiADSRDlg dlg;
	dlg.alBankCurrent = alBankCurrent;
	dlg.DoModal();
}

void CN64SoundListToolDlg::OnBnClickedButtonvadpcmimportsamepred()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int instrSel = mInstrumentChoice.GetCurSel();
				int soundChoice = mSoundChoice.GetCurSel();

				if (instrSel == -1)
					return;

				if (soundChoice == -1)
					return;

				if (alBankCurrent->inst[instrSel]->sounds[soundChoice] == NULL)
					return;

				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				bool decode8 = false;
				if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
				{
					decode8 = mHalfVADPCMPrecision.GetCheck();
				}

				if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
				{
					MessageBox("StarFox Special Sound Format unsupported");
					return;
				}

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, primSel, decode8, true))
					return;

				float sampleRateRef = alBankCurrent->samplerate;
				if (alBankCurrent->inst[instrSel]->samplerate != 0)
					sampleRateRef = alBankCurrent->inst[instrSel]->samplerate;
				
				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int instrSel = mInstrumentChoice.GetCurSel();
				int soundChoice = mSoundChoice.GetCurSel();
				if (alBankCurrent->percussion->sounds[soundChoice] == NULL)
					return;


				if (soundChoice == -1)
					return;

				CString subSoundStr;
				mSubSound.GetWindowText(subSoundStr);

				byte primSel = PRIMARY;
				if (subSoundStr == "Primary")
					primSel = PRIMARY;
				else if (subSoundStr == "Previous")
					primSel = PREVIOUS;
				else if (subSoundStr == "Secondary")
					primSel = SECONDARY;

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplacePercussionWithWavData(alBankCurrent, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, true))
					return;

				float sampleRateRef = alBankCurrent->samplerate;
				if (alBankCurrent->percussion->samplerate != 0)
					sampleRateRef = alBankCurrent->percussion->samplerate;
				
				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosound2();
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int percussionSel = mPercussionChoice.GetCurSel();

				if (percussionSel == -1)
					return;

				if (alBankCurrent->eadPercussion == NULL)
					return;

				if (percussionSel == -1)
					return;

				bool decode8 = false;
				if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
				{
					decode8 = mHalfVADPCMPrecision.GetCheck();
				}

				if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
				{
					MessageBox("StarFox Special Sound Format unsupported");
					return;
				}

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplaceEADPercussionWithWavData(alBankCurrent, percussionSel, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, decode8, true))
					return;

				float sampleRateRef = alBankCurrent->samplerate;

				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombopercussion();
			}
		}
		else if (percussionMode == SFX)
		{
			CFileDialog m_ldFile(TRUE, "wav", "ExtractedSound.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				FILE* inFile = fopen(m_ldFile.GetPathName(), "r");
				if (inFile == NULL)
				{
					MessageBox(LoadResourceText(IDS_STRINGERROROPENING), LoadResourceText(IDS_STRINGERROR));
					return;
				}
				fclose(inFile);

				int sfxSel = mSfxChoice.GetCurSel();

				if (sfxSel == -1)
					return;

				if (alBankCurrent->alSfx[sfxSel] == NULL)
					return;

				bool decode8 = false;
				if ((alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
				{
					decode8 = mHalfVADPCMPrecision.GetCheck();
				}

				if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) && mCheckRaw.GetCheck())
				{
					MessageBox("StarFox Special Sound Format unsupported");
					return;
				}

				unsigned long samplingRate;
				if (!n64AudioLibrary.ReplaceSfxWithWavData(alBankCurrent, sfxSel, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, decode8, true))
					return;

				float sampleRateRef = alBankCurrent->samplerate;

				if (sampleRateRef != samplingRate)
				{
					CString tempStr;
					tempStr.Format("Warning sampling rate of wav %d is not equal to sampling rate of %d", samplingRate, alBankCurrent->samplerate);
					//MessageBox(tempStr, "Warning");
				}

				OnCbnSelchangeCombosfx();
			}
		}
	}
}
void CN64SoundListToolDlg::OnBnClickedButtondeletepercussionbank()
{
	if (alBankCurrent != NULL)
	{
		int percussionSel = mPercussionChoice.GetCurSel();

		if (percussionSel == -1)
			return;

		if (
			(alBankCurrent->soundBankFormat == STANDARDFORMAT)
			|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == CLAYFIGHTER)
			|| (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED)
			|| (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == ZLIBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BLASTCORPSZLBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BANJOTOOIEAUDIO)
			|| (alBankCurrent->soundBankFormat == TUROKFORMAT)
			|| (alBankCurrent->soundBankFormat == MKMYTHOLOGIES)
			|| (alBankCurrent->soundBankFormat == TITUS)
			|| (alBankCurrent->soundBankFormat == NINDEC)
			|| (alBankCurrent->soundBankFormat == KONAMICTL)
			|| (alBankCurrent->soundBankFormat == KONAMI8CTL)
			)
		{
			if (alBankCurrent->percussion != NULL)
			{
				n64AudioLibrary.DisposeALInst(alBankCurrent->percussion);

				alBankCurrent->percussion = NULL;

				percussionMode = NORMAL;

				OnCbnSelchangeCombosoundbank();
			}
		}
		else if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
		{
			if (alBankCurrent->countEADPercussion == 0)
			{
				return;
			}
			else if (alBankCurrent->countEADPercussion == 1)
			{
				n64AudioLibrary.DeleteEADPercussion(alBankCurrent, percussionSel);

				percussionMode = NORMAL;

				OnCbnSelchangeCombosoundbank();
			}
			else
			{
				n64AudioLibrary.DeleteEADPercussion(alBankCurrent, percussionSel);
			
				mPercussionChoice.ResetContent();

				for (int x = 0; x < alBankCurrent->countEADPercussion; x++)
				{
					CString tempStr;
					tempStr.Format("%04X", x);
					mPercussionChoice.AddString(tempStr);
				}

				if (percussionSel >= alBankCurrent->countEADPercussion)
					mPercussionChoice.SetCurSel(alBankCurrent->countEADPercussion-1);
				else
					mPercussionChoice.SetCurSel(percussionSel);
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddpercussionbank()
{
	if (alBankCurrent != NULL)
	{
		int percussionSel = mPercussionChoice.GetCurSel();

		if (
			(alBankCurrent->soundBankFormat == STANDARDFORMAT)
			|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == CLAYFIGHTER)
			|| (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED)
			|| (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)
			|| (alBankCurrent->soundBankFormat == ZLIBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BLASTCORPSZLBSTANDARD)
			|| (alBankCurrent->soundBankFormat == BANJOTOOIEAUDIO)
			|| (alBankCurrent->soundBankFormat == TUROKFORMAT)
			|| (alBankCurrent->soundBankFormat == MKMYTHOLOGIES)
			|| (alBankCurrent->soundBankFormat == TITUS)
			|| (alBankCurrent->soundBankFormat == NINDEC)
			|| (alBankCurrent->soundBankFormat == KONAMICTL)
			|| (alBankCurrent->soundBankFormat == KONAMI8CTL)
			)
		{
			if (alBankCurrent->percussion == NULL)
			{
				alBankCurrent->percussion = new ALInst();

				alBankCurrent->percussion->soundCount = 0;
				alBankCurrent->percussion->sounds = NULL;

				OnCbnSelchangeCombosoundbank();

				percussionMode = PERCUSSION;

				OnCbnSelchangeCombopercussion();
			}
		}
		else if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
		{
			if ((percussionSel == -1) && (alBankCurrent->countEADPercussion > 0))
				return;

			if (n64AudioLibrary.AddEADPercussion(alBankCurrent))
			{
				if (alBankCurrent->countEADPercussion == 1)
				{
					OnCbnSelchangeCombosoundbank();

					percussionMode = EADPERCUSSION;

					OnCbnSelchangeCombopercussion();
				}
				else
				{
					mPercussionChoice.ResetContent();

					for (int x = 0; x < alBankCurrent->countEADPercussion; x++)
					{
						CString tempStr;
						tempStr.Format("%04X", x);
						mPercussionChoice.AddString(tempStr);
					}

					mPercussionChoice.SetCurSel(alBankCurrent->countEADPercussion - 1);

					OnCbnSelchangeCombopercussion();
				}
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeletesfxbank()
{
	if (alBankCurrent != NULL)
	{
		int sfxSel = mSfxChoice.GetCurSel();

		if (sfxSel == -1)
			return;

		if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
		{
			if (alBankCurrent->countSfx == 0)
			{
				return;
			}
			else if (alBankCurrent->countSfx == 1)
			{
				n64AudioLibrary.DeleteSfx(alBankCurrent, sfxSel);

				percussionMode = NORMAL;

				OnCbnSelchangeCombosoundbank();
			}
			else
			{
				n64AudioLibrary.DeleteSfx(alBankCurrent, sfxSel);
			
				mSfxChoice.ResetContent();

				for (int x = 0; x < alBankCurrent->countSfx; x++)
				{
					CString tempStr;
					tempStr.Format("%04X", x);
					mSfxChoice.AddString(tempStr);
				}

				if (sfxSel >= alBankCurrent->countSfx)
					mSfxChoice.SetCurSel(alBankCurrent->countSfx-1);
				else
					mSfxChoice.SetCurSel(sfxSel);
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddsfxbank()
{
	if (alBankCurrent != NULL)
	{
		int sfxSel = mSfxChoice.GetCurSel();

		if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
				|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT))
		{
			if ((sfxSel == -1) && (alBankCurrent->countSfx > 0))
				return;

			if (n64AudioLibrary.AddSfx(alBankCurrent))
			{
				if (alBankCurrent->countSfx == 1)
				{
					OnCbnSelchangeCombosoundbank();

					percussionMode = SFX;

					OnCbnSelchangeCombosfx();
				}
				else
				{
					mSfxChoice.ResetContent();

					for (int x = 0; x < alBankCurrent->countSfx; x++)
					{
						CString tempStr;
						tempStr.Format("%04X", x);
						mSfxChoice.AddString(tempStr);
					}

					mSfxChoice.SetCurSel(alBankCurrent->countSfx - 1);

					OnCbnSelchangeCombosfx();
				}
			}
		}
	}
}
void CN64SoundListToolDlg::OnCbnSelchangeCombot1indexcombo()
{
	if (mT1Index.GetCurSel() != -1)
	{
		if (t1BankCurrent != NULL)
		{
			CString tempStr;
			mT1Index.GetWindowText(tempStr);

			int t1Index = CSharedFunctions::StringToUnsignedShort(tempStr);

			if (t1Index < t1BankCurrent->t1Entries.size())
			{
				dontupdateitall = false;

				tempStr.Format("%02X", t1BankCurrent->t1Entries[t1Index].bankNumber);
				mT1SubBank.SelectString(0, tempStr);

				tempStr.Format("%02X", t1BankCurrent->t1Entries[t1Index].instrumentNumber);
				mT1Instrument.SelectString(0, tempStr);

				tempStr.Format("%02X", t1BankCurrent->t1Entries[t1Index].soundNumber);
				mT1Sound.SelectString(0, tempStr);

				tempStr.Format("%d", t1BankCurrent->t1Entries[t1Index].frequency);
				mT1Frequency.SetWindowText(tempStr);

				tempStr.Format("%02X", t1BankCurrent->t1Entries[t1Index].flags);
				mT1Flags.SetWindowText(tempStr);

				dontupdateitall = true;
			}
		}
	}
}

void CN64SoundListToolDlg::OnEnChangeEditt1frequency()
{
	if (t1BankCurrent != NULL)
	{
		CString tempStr;
		mT1Index.GetWindowText(tempStr);

		int t1Index = CSharedFunctions::StringToUnsignedShort(tempStr);

		if (t1Index < t1BankCurrent->t1Entries.size())
		{
			if (dontupdateitall)
			{
				dontupdateitall = false;

				mT1Frequency.GetWindowText(tempStr);
				t1BankCurrent->t1Entries[t1Index].frequency = atoi(tempStr);

				dontupdateitall = true;
			}
		}
	}
}

void CN64SoundListToolDlg::OnCbnSelchangeCombot1subbank()
{
	if (t1BankCurrent != NULL)
	{
		CString tempStr;
		mT1Index.GetWindowText(tempStr);

		int t1Index = CSharedFunctions::StringToUnsignedShort(tempStr);

		if (t1Index < t1BankCurrent->t1Entries.size())
		{
			if (dontupdateitall)
			{
				dontupdateitall = false;

				mT1SubBank.GetWindowText(tempStr);
				t1BankCurrent->t1Entries[t1Index].bankNumber = CSharedFunctions::StringToUnsignedChar(tempStr);

				dontupdateitall = true;
			}
		}
	}
}

void CN64SoundListToolDlg::OnCbnSelchangeCombot1subbank2()
{
	if (t1BankCurrent != NULL)
	{
		CString tempStr;
		mT1Index.GetWindowText(tempStr);

		int t1Index = CSharedFunctions::StringToUnsignedShort(tempStr);

		if (t1Index < t1BankCurrent->t1Entries.size())
		{
			if (dontupdateitall)
			{
				dontupdateitall = false;

				mT1Instrument.GetWindowText(tempStr);
				t1BankCurrent->t1Entries[t1Index].instrumentNumber = CSharedFunctions::StringToUnsignedChar(tempStr);

				dontupdateitall = true;
			}
		}
	}
}

void CN64SoundListToolDlg::OnCbnSelchangeCombot1sound()
{
	if (t1BankCurrent != NULL)
	{
		CString tempStr;
		mT1Index.GetWindowText(tempStr);

		int t1Index = CSharedFunctions::StringToUnsignedShort(tempStr);

		if (t1Index < t1BankCurrent->t1Entries.size())
		{
			if (dontupdateitall)
			{
				dontupdateitall = false;

				mT1Sound.GetWindowText(tempStr);
				t1BankCurrent->t1Entries[t1Index].soundNumber = CSharedFunctions::StringToUnsignedChar(tempStr);

				dontupdateitall = true;
			}
		}
	}
}

void CN64SoundListToolDlg::OnEnChangeEditt1flags()
{
	if (t1BankCurrent != NULL)
	{
		CString tempStr;
		mT1Index.GetWindowText(tempStr);

		int t1Index = CSharedFunctions::StringToUnsignedShort(tempStr);

		if (t1Index < t1BankCurrent->t1Entries.size())
		{
			if (dontupdateitall)
			{
				dontupdateitall = false;

				mT1Flags.GetWindowText(tempStr);
				t1BankCurrent->t1Entries[t1Index].flags = CSharedFunctions::StringToUnsignedChar(tempStr);

				dontupdateitall = true;
			}
		}
	}
}

BOOL CN64SoundListToolDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	CWnd *pWndChild = ChildWindowFromPoint(point, CWP_ALL);

	if (pWndChild != NULL)
	{
		if (pWndChild->m_hWnd != m_hWnd)
		{
			if (pWndChild->GetDlgCtrlID() == IDC_BUTTONPLAYSND)
			{
				int curSel = mSoundChoice.GetCurSel();
				if (curSel != -1)
				{
					int newSel = curSel + (int)(-zDelta / 120);

					if (newSel < 0)
					{
						mSoundChoice.SetCurSel(0);		
					}
					else if (newSel >= mSoundChoice.GetCount())
					{
						mSoundChoice.SetCurSel(mSoundChoice.GetCount() - 1);
					}
					else
					{
						mSoundChoice.SetCurSel(newSel);
					}

					OnCbnSelchangeCombosound2();
				}
			}
		}
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CN64SoundListToolDlg::OnBnClickedButtonplaysnd2()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == NORMAL)
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (instrSel == -1)
				return;

			if (soundSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (alBankCurrent->inst[instrSel]->samplerate)
				sampleRate = alBankCurrent->inst[instrSel]->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}
			else if (mUseT1.GetCheck())
			{
				if (mT1Index.GetCount() > 0)
				{
					if (t1BankCurrent != NULL)
					{
						int bankIndex = mSoundBankIndex.GetCurSel();

						for (int t = 0; t < t1BankCurrent->t1Entries.size(); t++)
						{
							if (
								(t1BankCurrent->t1Entries[t].bankNumber == bankIndex)
								&& (t1BankCurrent->t1Entries[t].instrumentNumber == instrSel)
								&& (t1BankCurrent->t1Entries[t].soundNumber == soundSel)
								)
							{
								sampleRate = t1BankCurrent->t1Entries[t].frequency;
								break;
							}
						}
					}
				}
			}

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			if (n64AudioLibrary.ExtractLoopSound(mainFolder, alBankCurrent, instrSel, soundSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, primSel, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC | SND_LOOP);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
		else if (percussionMode == PERCUSSION)
		{
			int soundSel = mSoundChoice.GetCurSel();

			if (soundSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (alBankCurrent->percussion->samplerate)
				sampleRate = alBankCurrent->percussion->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			if (n64AudioLibrary.ExtractLoopPercussion(mainFolder, alBankCurrent, soundSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC | SND_LOOP);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
		else if (percussionMode == EADPERCUSSION)
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}

			if (n64AudioLibrary.ExtractLoopEADPercussion(alBankCurrent, percussionSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC | SND_LOOP);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
		else if (percussionMode == SFX)
		{
			int sfxSel = mSfxChoice.GetCurSel();

			if (sfxSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (mOverrideRate.GetCheck())
			{
				CString tempStr;
				mSamplingRate.GetWindowText(tempStr);
				sampleRate = atoi(tempStr);
			}

			if (n64AudioLibrary.ExtractLoopSfx(alBankCurrent, sfxSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC | SND_LOOP);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
	}
}