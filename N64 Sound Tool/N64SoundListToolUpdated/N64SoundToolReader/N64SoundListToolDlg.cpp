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
	DDX_Control(pDX, IDC_CHECK1, mIgnoreKeyBase);
	DDX_Control(pDX, IDC_CHECKHALFSAMPLINGRATE, mHalfSamplingRate);
	DDX_Control(pDX, IDC_BUTTONSTOPSOUND, m_stopButton);
	DDX_Control(pDX, IDC_STATICBENDRANGE, m_InstrBendRangeStatic);
	DDX_Control(pDX, IDC_EDITBENDRANGE, mBendRange);
	DDX_Control(pDX, IDC_COMBOPERCUSSION, mPercussionChoice);
	DDX_Control(pDX, IDC_PERCUSSIONLABEL, mPercussionLabel);
	DDX_Control(pDX, IDC_BUTTONUP, mUpArrowButton);
	DDX_Control(pDX, IDC_BUTTONDOWN, mDownArrowButton);
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
END_MESSAGE_MAP()

// CN64SoundListToolDlg message handlers

BOOL CN64SoundListToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	percussionMode = false;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Quick test of compression
	/*CNinDec ninDec;
	FILE* inTemp = fopen("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\Nintama Rantarou 64 Game Gallery (J) [!].z64", "rb");
	unsigned char* tempChar = new unsigned char[0x800000];
	fread(tempChar, 1, 0x800000, inTemp);
	fclose(inTemp);

	unsigned char* outputBuffer = new unsigned char[0x1000000];
	int dcSize = ninDec.dec(&tempChar[0x3FDBE0], 0x100000, outputBuffer);

	FILE* out = fopen("C:\\temp\\temp.bin", "wb");
	fwrite(outputBuffer, 1, dcSize, out);
	fclose(out);

	delete [] tempChar;
	delete [] outputBuffer;*/

	dontupdateitall = true;
	romName = "";

	ROM = NULL;
	romSize = 0;
	results.clear();
	numberResults = 0;
	alBankCurrent = NULL;

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

	if ((CharArrayToLong(&ROM[0]) == 0x37804012) || (filename.Find(".u2") != -1) || (filename.Find(".u3") != -1) || (filename.Find(".u4") != -1))
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
	CFileDialog m_ldFile(TRUE, NULL, filename, OFN_HIDEREADONLY, "N64 ROM(*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin)|*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin|", this);

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

	/*unsigned char* geTempRom = new unsigned char[0xC00000];
	FILE * inGE = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\Goldeneye.rom", "rb");
	fread(geTempRom, 1, 0xC00000, inGE);
	fclose(inGE);
	ALBank * GEBank = n64AudioLibrary.ReadAudio(&geTempRom[0x3B4450], 0x43A0, 0x0, &geTempRom[0x3B87F0], 0x0, 0xFFFFFFFF, 0x4B);*/

	/*unsigned char* geTempRom = new unsigned char[0x2000000];
	FILE * inGE = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\Perfect_dark.v64", "rb");
	fread(geTempRom, 1, 0x2000000, inGE);
	fclose(inGE);
	ALBank * GEBank = n64AudioLibrary.ReadAudio(&geTempRom[0xCFBF30], 0xA060, 0x0, &geTempRom[0xD05F90]);*/


	if (!OpenROMPlusDlg())
		return;


	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		results.clear();

		alBankCurrent = NULL;

		numberResults = n64AudioLibrary.ReadCtlTblLocations(ROM, romSize, results);

		for (int x = 0; x < numberResults; x++)
		{
			CString tempStr;
			tempStr.Format("%02X - Ctl %08X (%08X) Tbl %08X (%08X)", x, results[x].ctlOffset, results[x].ctlSize, results[x].tblOffset, results[x].tblSize);
			mSoundBankIndex.AddString(tempStr);
		}




		/*for (int x = 0; x < GEBank->count; x++)
		{
			results[1].bank->inst[x] = GEBank->inst[x];
		}
		*/






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
	for (int x = 0; x < countGames; x++)
	{
		delete [] gameConfig[x].soundBanks;
	}

	delete [] gameConfig;

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

			if (percussionMode == false)
			{
				int instrumentSel = mInstrumentChoice.GetCurSel();


				mSamplingRate.GetWindowText(tempStr);
				alBankCurrent->samplerate = atoi(tempStr);

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
					|| (alBankCurrent->soundBankFormat == MARIOKART64FORMAT))
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
				else if ((alBankCurrent->soundBankFormat == ZELDAFORMAT)
					|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
					|| (alBankCurrent->soundBankFormat == FZEROFORMAT))
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
					|| (alBankCurrent->soundBankFormat == MARIOKART64FORMAT))
				{

					mKeyMin.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->unknown1 = CSharedFunctions::StringHexToLong(tempStr);

					mKeyMax.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->key.keymax = CSharedFunctions::StringToSignedChar(tempStr);

					mKeyBase.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.attackTime = CSharedFunctions::StringHexToLong(tempStr);

					mDetune.GetWindowText(tempStr);
					alBankCurrent->inst[instrumentSel]->sounds[soundSel]->env.decayTime = CSharedFunctions::StringHexToLong(tempStr);
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
			else
			{
				int percussionSel = mPercussionChoice.GetCurSel();

				if (percussionSel == -1)
					return;

				ALWave* alWave = &alBankCurrent->alPercussion[percussionSel].wav;

				mReleaseTime.GetWindowText(tempStr);
				float unknown2 = atof(tempStr);
				alBankCurrent->alPercussion[percussionSel].unknown2 = *reinterpret_cast<unsigned long*> (&unknown2);

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
			
			dontupdateitall = true;
		}
	}
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

	if (alBankCurrent->soundBankFormat == STANDARDFORMAT)
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
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
	else if (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED)
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_SHOW);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		m_add16BitRaw.ShowWindow(SW_SHOW);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
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
		m_sampleReleaseTimeStatic.SetWindowText("Float Key");
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == N64DD)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == CONKER)
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
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
	else if ((alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2) || (alBankCurrent->soundBankFormat == RNCCOMPRESSEDN64PTR))
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
		m_importGroupBox.ShowWindow(SW_SHOW);
		m_addToEndGroupBox.ShowWindow(SW_SHOW);
		m_deleteButton.ShowWindow(SW_SHOW);
		m_miscGroupBox.ShowWindow(SW_HIDE);
		m_tremGroupBox.ShowWindow(SW_HIDE);
		m_vibrGroupBox.ShowWindow(SW_HIDE);
		mVolume.ShowWindow(SW_SHOW);
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
		mDetune.ShowWindow(SW_HIDE);
		m_instrVolStatic.ShowWindow(SW_SHOW);
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
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == TAZHUFFMAN)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == SNOW)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == MEGAMAN64PTRV2)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == ARMYMENFORMAT)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == TITUS)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == NINDEC)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
	{
		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if ((alBankCurrent->soundBankFormat == H20RAW816) || (alBankCurrent->soundBankFormat == TETRISPHERERAW816))
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_sampleAttackTimeStatic.SetWindowText("Key Prev");
		m_sampleDecayTimeStatic.SetWindowText("Key");
		m_sampleReleaseTimeStatic.SetWindowText("Key Sec");

		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
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
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
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
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == FZEROFORMAT)
	{
		m_sampleAttackTimeStatic.SetWindowText("Key Prev");
		m_sampleDecayTimeStatic.SetWindowText("Key");
		m_sampleReleaseTimeStatic.SetWindowText("Key Sec");

		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	else if (alBankCurrent->soundBankFormat == TUROKFORMAT)
	{
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
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
		m_sampleAttackTimeStatic.SetWindowText("Key Prev");
		m_sampleDecayTimeStatic.SetWindowText("Key");
		m_sampleReleaseTimeStatic.SetWindowText("Key Sec");

		m_injectInPlaceButton.ShowWindow(SW_SHOW);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_SHOW);
		m_import16BitRawValues.ShowWindow(SW_SHOW);
		m_importPredictors.ShowWindow(SW_SHOW);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_SHOW);
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
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
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
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
	else if (alBankCurrent->soundBankFormat == MARIOKART64FORMAT)
	{
		m_sampleAttackTimeStatic.SetWindowText("Key Prev");
		m_sampleDecayTimeStatic.SetWindowText("Key");
		m_sampleReleaseTimeStatic.SetWindowText("Key Sec");

		m_injectInPlaceButton.ShowWindow(SW_HIDE);
		m_playButton.ShowWindow(SW_SHOW);
		m_stopButton.ShowWindow(SW_SHOW);
		m_saveButton.ShowWindow(SW_SHOW);
		m_import16BitRaw.ShowWindow(SW_HIDE);
		m_import16BitADPCM.ShowWindow(SW_HIDE);
		m_import16BitRawValues.ShowWindow(SW_HIDE);
		m_importPredictors.ShowWindow(SW_HIDE);
		m_add16BitRaw.ShowWindow(SW_HIDE);
		m_add16BitADPCM.ShowWindow(SW_HIDE);
		m_importGroupBox.ShowWindow(SW_HIDE);
		m_addToEndGroupBox.ShowWindow(SW_HIDE);
		m_deleteButton.ShowWindow(SW_HIDE);
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
		mSamplePan.ShowWindow(SW_HIDE);
		mSampleVol.ShowWindow(SW_HIDE);
		mAttackTime.ShowWindow(SW_SHOW);
		mDecayTime.ShowWindow(SW_SHOW);
		mReleaseTime.ShowWindow(SW_SHOW);
		mAttackVolume.ShowWindow(SW_SHOW);
		mDecayVolume.ShowWindow(SW_HIDE);
		mVelMin.ShowWindow(SW_HIDE);
		mVelMax.ShowWindow(SW_HIDE);
		mKeyMin.ShowWindow(SW_HIDE);
		mKeyMax.ShowWindow(SW_HIDE);
		mKeyBase.ShowWindow(SW_HIDE);
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
		m_sampleDecayVolumeStatic.ShowWindow(SW_HIDE);
	}
}

void CN64SoundListToolDlg::ShowPercussionControls()
{
	mSubSound.ShowWindow(SW_HIDE);
	mAddPrevButton.ShowWindow(SW_HIDE);
	mRemovePrevButton.ShowWindow(SW_HIDE);
	mAddSecButton.ShowWindow(SW_HIDE);
	mRemoveSecButton.ShowWindow(SW_HIDE);

	mSoundChoice.ShowWindow(SW_HIDE);
	mUpArrowButton.ShowWindow(SW_HIDE);
	mDownArrowButton.ShowWindow(SW_HIDE);

	m_injectInPlaceButton.ShowWindow(SW_HIDE);
	m_playButton.ShowWindow(SW_SHOW);
	m_stopButton.ShowWindow(SW_SHOW);
	m_saveButton.ShowWindow(SW_SHOW);
	m_import16BitRaw.ShowWindow(SW_HIDE);
	m_import16BitADPCM.ShowWindow(SW_HIDE);
	m_import16BitRawValues.ShowWindow(SW_HIDE);
	m_importPredictors.ShowWindow(SW_HIDE);
	m_add16BitRaw.ShowWindow(SW_HIDE);
	m_add16BitADPCM.ShowWindow(SW_HIDE);
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
	mReleaseTime.ShowWindow(SW_SHOW);
	mAttackVolume.ShowWindow(SW_HIDE);
	mDecayVolume.ShowWindow(SW_HIDE);
	mVelMin.ShowWindow(SW_HIDE);
	mVelMax.ShowWindow(SW_HIDE);
	mKeyMin.ShowWindow(SW_HIDE);
	mKeyMax.ShowWindow(SW_HIDE);
	mKeyBase.ShowWindow(SW_HIDE);
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
		if (alBankCurrent == NULL)
		{
			dontupdateitall = true;
			return;
		}

		if (alBankCurrent->bankName != "")
			m_bankName.SetWindowText(alBankCurrent->bankName);

		mInstrumentChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->count; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			mInstrumentChoice.AddString(tempStr);
		}

		mInstrumentChoice.SetCurSel(0);

		mPercussionChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->countPercussion; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			mPercussionChoice.AddString(tempStr);
		}

		if (alBankCurrent->countPercussion > 0)
		{
			mPercussionChoice.SetCurSel(0);
			mPercussionChoice.ShowWindow(SW_SHOW);
			mPercussionLabel.ShowWindow(SW_SHOW);
		}
		else
		{
			mPercussionChoice.ShowWindow(SW_HIDE);
			mPercussionLabel.ShowWindow(SW_HIDE);
		}

		CString tempSampleStr;
		tempSampleStr.Format("%d", alBankCurrent->samplerate);
		mSamplingRate.SetWindowText(tempSampleStr);

		dontupdateitall = true;
	}

	percussionMode = false;
	OnCbnSelchangeCombosound();
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
		}

	
		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->volume);
		mVolume.SetWindowText(tempStr);

		tempStr.Format("%02X", alBankCurrent->inst[instrSel]->pan);
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

		tempStr.Format("%04X", alBankCurrent->inst[instrSel]->vibDelay);
		mBendRange.SetWindowText(tempStr);

		mSoundChoice.ResetContent();

		for (int x = 0; x < alBankCurrent->inst[instrSel]->soundCount; x++)
		{
			CString tempStr;
			tempStr.Format("%04X", x);
			mSoundChoice.AddString(tempStr);
		}

		mSoundChoice.SetCurSel(0);

		percussionMode = false;
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

		int instrSel = mInstrumentChoice.GetCurSel();
		int soundChoice = mSoundChoice.GetCurSel();

		if (soundChoice == -1)
		{
			dontupdateitall = true;
			return;
		}
	

		if (alBankCurrent->inst[instrSel]->sounds[soundChoice] != NULL)
		{

			if (alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
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
			tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.base);
			mBase.SetWindowText(tempStr);

			tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.len);
			mLength.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->samplePan);
			mSamplePan.SetWindowText(tempStr);

			tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->sampleVolume);
			mSampleVol.SetWindowText(tempStr);


			if ((alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
				|| (alBankCurrent->soundBankFormat == MARIOKART64FORMAT))
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
			else if (
				(alBankCurrent->soundBankFormat == ZELDAFORMAT)
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
				|| (alBankCurrent->soundBankFormat == MARIOKART64FORMAT))
			{
				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->unknown1);
				mKeyMin.SetWindowText(tempStr);

				tempStr.Format("%02X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->key.keymax);
				mKeyMax.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.attackTime);
				mKeyBase.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->inst[instrSel]->sounds[soundChoice]->env.decayTime);
				mDetune.SetWindowText(tempStr);
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

		dontupdateitall = true;
	}

	mSubSound.SelectString(0, "Primary");

	OnCbnSelchangeCombosoundsubsound();
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
		if (percussionMode == false)
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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondown()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonplayoriginalsound()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
		{
			int instrSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			if (soundSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (alBankCurrent->inst[instrSel]->samplerate)
				sampleRate = alBankCurrent->inst[instrSel]->samplerate;

			CString subSoundStr;
			mSubSound.GetWindowText(subSoundStr);

			byte primSel = PRIMARY;
			if (subSoundStr == "Primary")
				primSel = PRIMARY;
			else if (subSoundStr == "Previous")
				primSel = PREVIOUS;
			else if (subSoundStr == "Secondary")
				primSel = SECONDARY;

			if (n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, instrSel, soundSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, primSel, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck()))
			{
				::PlaySound((mainFolder + "tempWav231A24r.wav"), 0, SND_ASYNC);
				//PlayWMPlayerSound((mainFolder + "tempWav231A24r.wav"));
			}
			else
			{
				MessageBox(LoadResourceText(IDS_STRINGERROREXTRACTINGSOUND));
			}
		}
		else
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			//KillWMPlayer();

			float sampleRate = alBankCurrent->samplerate;

			if (n64AudioLibrary.ExtractPercussion(alBankCurrent, percussionSel, (mainFolder + "tempWav231A24r.wav"), sampleRate, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck()))
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
		if (percussionMode == false)
		{
			CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

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
				if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_RAW16_WAVE, primSel))
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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonvadpcmimport()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
		{
			CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

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
				if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, primSel))
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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddsound()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
		{
			CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonaddsound2()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
		{
			CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeletesound()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtondeletesound2()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
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

				if (n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, instrSel, soundChoice, m_svFile.GetPathName(), alBankCurrent->samplerate, primSel, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck()))
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
				int percussionSel = mPercussionChoice.GetCurSel();

				if (percussionSel == -1)
					return;

				//KillWMPlayer();

				float sampleRate = alBankCurrent->samplerate;

				if (n64AudioLibrary.ExtractPercussion(alBankCurrent, percussionSel, m_svFile.GetPathName(), sampleRate, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck()))
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
		CFileDialog m_svFileOutROM(FALSE, "rom", (romName + ".rom"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "N64 ROM(*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin)|*.v64;*.z64;*.rom;*.n64;*.u2;*.u3;*.u4;.bin|", this);

		int isFileOpened2 = m_svFileOutROM.DoModal();

		if ((isFileOpened2 != IDCANCEL) && (m_svFileOutROM.GetFileName() != ""))
		{
			CString romOutName = m_svFileOutROM.GetPathName();

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

		n64AudioLibrary.WriteAudioToFile(alBankCurrent, m_svFileCtl.GetPathName(), m_svFileTbl.GetPathName());
	}
}

void CN64SoundListToolDlg::OnBnClickedInjectplace()
{
	if ((alBankCurrent != NULL) && (romSize > 0))
	{
		int bank = mSoundBankIndex.GetCurSel();
		if (bank == -1)
			return;

		unsigned char* ctl;
		unsigned char* tbl;
		int ctlSize, tblSize;
		if (alBankCurrent->soundBankFormat == STANDARDFORMAT)
			n64AudioLibrary.WriteAudio(alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if ((alBankCurrent->soundBankFormat == STARFOX64FORMAT) || (alBankCurrent->soundBankFormat == ZELDAFORMAT) || (alBankCurrent->soundBankFormat == FZEROFORMAT) || (alBankCurrent->soundBankFormat == TUROKFORMAT)  || (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)  || (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)|| (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED))
		{
			MessageBox("Sorry, no encoding yet for MIO0 format");
			return;
		}
		else if (alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
			n64AudioLibrary.WriteAudioSuperMario(alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2)
			n64AudioLibrary.WriteAudioN64PtrWavetableV2(alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
			n64AudioLibrary.WriteAudioN64PtrWavetableV2Blitz(mainFolder, alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV1)
			n64AudioLibrary.WriteAudioN64PtrWavetableV1(alBankCurrent, ctl, ctlSize, tbl, tblSize);
		else if (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
		{
			MessageBox("Sorry, no encoding yet for YAY0 format");
			return;
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
		else
		{
			MessageBox("Sorry, no encoding");
			return;
		}

		if (n64AudioLibrary.InjectCtlTblIntoROMArrayInPlace(ROM, ctl, ctlSize, tbl, tblSize, results[bank].ctlOffset, results[bank].tblOffset, results[bank].ctlSize, results[bank].tblSize))
		{
			/*for (int x = (results[bank].ctlOffset + ctlSize); x < (results[bank].ctlOffset + results[bank].ctlSize); x++)
			{
				ROM[x] = 0xFF;
			}*/
		}
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

			for (int y = 0; y < alBankCurrent->countPercussion; y++)
			{
				if (alBankCurrent->samplerate != 0)
					sampleRate = (alBankCurrent->samplerate);
				CString tempExportNameStr;
				if (mExportShortFilename.GetCheck())
					tempExportNameStr.Format("%s\\B%02XP%02X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
				else
					tempExportNameStr.Format("%s\\BANK_%02X_PERC_%04X.wav", outputFolder, mSoundBankIndex.GetCurSel(), y);
				n64AudioLibrary.ExtractPercussion(alBankCurrent, y, tempExportNameStr, sampleRate, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
			}

			for (int x = 0; x < alBankCurrent->count; x++)
			{
				for (int y = 0; y < alBankCurrent->inst[x]->soundCount; y++)
				{
					if (alBankCurrent->inst[x]->samplerate != 0)
						sampleRate = (alBankCurrent->inst[x]->samplerate);
					CString tempExportNameStr;
					if (mExportShortFilename.GetCheck())
						tempExportNameStr.Format("%s\\B%02XI%02XS%02X.wav", outputFolder, mSoundBankIndex.GetCurSel(), x, y);
					else
						tempExportNameStr.Format("%s\\BANK_%02X_INSTR_%04X_SND_%04X.wav", outputFolder, mSoundBankIndex.GetCurSel(), x, y);

					n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, x, y, tempExportNameStr, sampleRate, PRIMARY, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
					if ((alBankCurrent->inst[x] != NULL) && (alBankCurrent->inst[x]->sounds[y] != NULL))
					{
						if (alBankCurrent->inst[x]->sounds[y]->hasWavePrevious)
							n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, x, y, tempExportNameStr + "Prev.wav", sampleRate, PREVIOUS, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
						if (alBankCurrent->inst[x]->sounds[y]->hasWaveSecondary)
							n64AudioLibrary.ExtractRawSound(mainFolder, alBankCurrent, x, y, tempExportNameStr + "Sec.wav", sampleRate, SECONDARY, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
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
		CFileDialog m_ldFile(TRUE, "wav", "raw16.wav", OFN_HIDEREADONLY, "16-bit Raw Wave file (*.wav)|*.wav|", this);

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
			if (!n64AudioLibrary.ReplaceSoundWithWavData(alBankCurrent, instrSel, soundChoice, m_ldFile.GetPathName(), samplingRate, AL_ADPCM_WAVE, primSel))
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
		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		results.clear();

		alBankCurrent = NULL;

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
		if (percussionMode == false)
		{	
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();
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
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonexportpreditors()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
		{
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

			CFileDialog m_svFile(FALSE, "bin", "Predictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "PRedictors (*.bin)|*.bin|");

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

				n64AudioLibrary.ExportPredictors(alBankCurrent, instrumentSel, soundSel, m_svFile.GetPathName(), primSel);
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonimport16bitraw()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
		{
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

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
		else
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			CFileDialog m_ldFile(TRUE, "bin", "RawSound.bin", OFN_HIDEREADONLY, "Raw Bin Sound (*.bin)|*.bin|", this);

			int statusFileOpen = (int) m_ldFile.DoModal();
			CString fileName = m_ldFile.GetPathName();
			if (statusFileOpen == IDOK)
			{
				n64AudioLibrary.ImportRawPercussionData(alBankCurrent, percussionSel, m_ldFile.GetPathName());
				OnCbnSelchangeCombosound2();
			}
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonexportraw()
{
	if (alBankCurrent != NULL)
	{
		if (percussionMode == false)
		{
			int instrumentSel = mInstrumentChoice.GetCurSel();
			int soundSel = mSoundChoice.GetCurSel();

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
		else
		{
			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			CFileDialog m_svFile(FALSE, "bin", "RawSound.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Raw Bin Sound (*.bin)|*.bin|");

			int statusFileOpen = (int) m_svFile.DoModal();
			CString fileName = m_svFile.GetFileName();

			if ((statusFileOpen == IDOK) && (fileName != ""))
			{
				n64AudioLibrary.ExportRawPercussionData(alBankCurrent, percussionSel, m_svFile.GetPathName());
			}
		}
	}
}

void CN64SoundListToolDlg::OpenROMSpecific(SoundGameConfig gameConfig)
{
	if (!OpenROMPlusDlg(gameConfig.gameName + " [!].z64"))
		return;


	mSoundBankIndex.ResetContent();

	try
	{
		for (int x = 0; x < numberResults; x++)
		{
			n64AudioLibrary.DisposeALBank(results[x].bank);
		}

		results.clear();

		alBankCurrent = NULL;

		CN64SoundToolReader::ReadSoundbanks(ROM, romSize, gameConfig, numberResults, results);

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

	

	for (int y = 0; y < countGames; y++)
	{
		if (!OpenROMFilename((tempPath + gameConfig[y].gameName + " [!].z64"), true))
			continue;


		CString outputFolder = (tempPath + gameConfig[y].gameName + "\\");
		
		::CreateDirectory(outputFolder, NULL);

		mSoundBankIndex.ResetContent();

		try
		{
			for (int x = 0; x < numberResults; x++)
			{
				n64AudioLibrary.DisposeALBank(results[x].bank);
			}

			alBankCurrent = NULL;

			numberResults = 0;

			if (gameConfig[y].gameType.Find("MultiPartERZN64WavePtrV2") == 0)
			{
				unsigned char* ctlBinaryPre = new unsigned char[0x1000000];
				unsigned long  ctlSize = 0;

				for (int x = 0; x < gameConfig[y].numberSoundBanks; x++)
				{
					RncDecoder rnc;
					int fileSizeCompressed = romSize - gameConfig[y].soundBanks[x].ctl;
					if (fileSizeCompressed > 0xFE000)
						fileSizeCompressed = 0xFE000;
					unsigned char* outputDecompressed = new unsigned char[0x1000000];
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
			else if (gameConfig[y].gameType.Find("MultiPartTigWavePtrV2") == 0)
			{
				unsigned char* ctlBinaryPre = new unsigned char[0x1000000];
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

						ctlBinaryPre = new unsigned char[0x1000000];
						ctlSize = 0;
					}

					TigDecoder tig;
					int fileSizeCompressed = romSize - gameConfig[y].soundBanks[x].ctl;
					if (fileSizeCompressed > 0xFE000)
						fileSizeCompressed = 0xFE000;
					unsigned char* outputDecompressed = new unsigned char[0x1000000];
					
					unsigned long address = gameConfig[y].soundBanks[x].ctl;
					unsigned long compressedsize = ((((((ROM[address] << 8) | ROM[address+1]) << 8) | ROM[address+2]) << 8) | ROM[address+3]);
					unsigned char type = ROM[address+4];

					if (type == 0)
					{
						memcpy(&ctlBinaryPre[ctlSize], &ROM[address+8], compressedsize);
						ctlSize += compressedsize;
					}
					else if (type == 1) // no compression
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
					if (gameConfig[y].gameType == "RawTest")
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRawTest(&ROM[0], results[numberResults].ctlSize);
					}
					else if (gameConfig[y].gameType == "SF64Uncompressed")
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioStarFox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType == "ZeldaUncompressed")
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZelda(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, &ROM[0]);
					}
					else if (gameConfig[y].gameType.Find("TurokUncompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTurok(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, &ROM[0], 0);
					}
					else if (gameConfig[y].gameType.Find("ArmyMenCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, false);
					}
					else if (gameConfig[y].gameType.Find("SargeArmyMenCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, true);
					}
					else if (gameConfig[y].gameType.Find("ArmyMenAirCombatCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], false, false);
					}
					else if (gameConfig[y].gameType.Find("SnoWave") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioSno(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("RNCCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadRNCAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("RNXCompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadRNXAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("FZeroUncompressed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioFZero(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, &ROM[0]);
					}
					else if (gameConfig[y].gameType.Find("MegaManN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMegamanN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("N64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("SydneyWavePtr") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioSydney(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("NinDec") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioNinDec(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("TazHuffmanWavePtr") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTazN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("RNCN64Wave") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64Ptr(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("RNCN64OffsetWave") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64PtrOffset(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("BnkB") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioBnkB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("N64WavePtrTableV1") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV1(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("H20Raw816") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioH20Raw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("TetrisphereRaw816") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioTetrisphereRaw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("Titus") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64Titus(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("MKMythologies") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64MKMythologies(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("YAY0N64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2YAY0(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("BlitzN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableBlitz(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}	
					else if (gameConfig[y].gameType.Find("ZLibN64WavePtrTableV2") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2ZLIB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("ZLIBSN64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64Zlib(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.Find("SN64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.Find("SuperMario64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("MarioKart64") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, true);
					}
					else if (gameConfig[y].gameType.Find("StandardRawAllowed") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioRawAllowed(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, 0);
					}
					else if (gameConfig[y].gameType.Find("64dd") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudio64dd(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("vox") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioVox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("BanjoTooie") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioBanjoTooie(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig[y].soundBanks[x].numberInstruments);
					}
					else if (gameConfig[y].gameType.Find("Conker") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioConker(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, gameConfig[y].soundBanks[x].numberInstruments, &ROM[results[numberResults].tblOffset]);
					}
					else if (gameConfig[y].gameType.Find("LzMusyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioLzMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.Find("ZLib78DAMusyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx78DA(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.Find("ZLibMusyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, GAUNTLETLEGENDS);
					}
					else if (gameConfig[y].gameType.Find("MusyxREZLib") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask, RESIDENTEVIL2);
					}
					else if (gameConfig[y].gameType.Find("MusyxSmallZlib") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyxSmall(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.Find("Musyx") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig[y].soundBanks[x].numberInstruments, gameConfig[y].soundBanks[x].mask);
					}
					else if (gameConfig[y].gameType.Find("BlastCorps") == 0)
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudioBlastCorps(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
					}
					else
					{
						results[numberResults].bank = n64AudioLibrary.ReadAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig[y].soundBanks[x].numberInstruments);
					}

					numberResults++;
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

						n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr, sampleRate, PRIMARY, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
						if ((results[x].bank->inst[r] != NULL) && (results[x].bank->inst[r]->sounds[z] != NULL))
						{
							if (results[x].bank->inst[r]->sounds[z]->hasWavePrevious)
								n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Prev.wav", sampleRate, PREVIOUS, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
							if (results[x].bank->inst[r]->sounds[z]->hasWaveSecondary)
								n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Sec.wav", sampleRate, SECONDARY, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
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

			for (int z = 0; z < results[x].bank->countPercussion; z++)
			{
				if (results[x].bank->samplerate != 0)
					sampleRate = (alBankCurrent->samplerate);

				CString tempExportNameStr;
				if (mExportShortFilename.GetCheck())
					tempExportNameStr.Format("%s\\B%02XP%02X.wav", outputFolder, x, z);
				else
					tempExportNameStr.Format("%s\\BANK_%02X__PERC_%04X.wav", outputFolder, x, z);
				n64AudioLibrary.ExtractPercussion(results[x].bank, z, tempExportNameStr, sampleRate, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
			}

			for (int r = 0; r < results[x].bank->count; r++)
			{
				for (int z = 0; z < results[x].bank->inst[r]->soundCount; z++)
				{
					if (results[x].bank->inst[r]->samplerate != 0)
						sampleRate = (alBankCurrent->inst[r]->samplerate);

					CString tempExportNameStr;
					if (mExportShortFilename.GetCheck())
						tempExportNameStr.Format("%s\\B%02XI%02XS%02X.wav", outputFolder, x, r, z);
					else
						tempExportNameStr.Format("%s\\BANK_%02X_INSTR_%04X_SND_%04X.wav", outputFolder, x, r, z);

					//if (results[x].bank->inst[r]->sounds[z]->wav.adpcmWave->loop != NULL)
						n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr, sampleRate, PRIMARY, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
					if ((results[x].bank->inst[r] != NULL) && (results[x].bank->inst[r]->sounds[z] != NULL))
					{
						if (results[x].bank->inst[r]->sounds[z]->hasWavePrevious)
							//if (results[x].bank->inst[r]->sounds[z]->wavPrevious.adpcmWave->loop != NULL)
								n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Prev.wav", sampleRate, PREVIOUS, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
							if (results[x].bank->inst[r]->sounds[z]->hasWaveSecondary)
								//if (results[x].bank->inst[r]->sounds[z]->wavSecondary.adpcmWave->loop != NULL)
									n64AudioLibrary.ExtractRawSound(mainFolder, results[x].bank, r, z, tempExportNameStr + "Sec.wav", sampleRate, SECONDARY, mIgnoreKeyBase.GetCheck(), mHalfSamplingRate.GetCheck());
					}
				}
			}
		}
	}
}

void CN64SoundListToolDlg::OnCbnSelchangeCombosoundsubsound()
{
	CString tempStr;

	if (percussionMode == false)
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
					}
					else if ( alBankCurrent->inst[instrSel]->sounds[soundChoice]->wav.type == AL_MUSYX_WAVE)
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
				}
				else if (primSel == SECONDARY)
				{
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
				}
			}

			dontupdateitall = true;
		}
	}
}

void CN64SoundListToolDlg::OnBnClickedButtonimportlooppredictors()
{
	if (percussionMode == false)
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
	else
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

			ALWave* alWave = &alBankCurrent->alPercussion[percussionSel].wav;

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
	if (percussionMode == false)
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
	else
	{
		CFileDialog m_svFile(FALSE, "bin", "LoopPredictors.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Predictors (*.bin)|*.bin|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();
		if ((statusFileOpen == IDOK) && (fileName != ""))
		{

			int percussionSel = mPercussionChoice.GetCurSel();

			if (percussionSel == -1)
				return;

			ALWave* alWave = &alBankCurrent->alPercussion[percussionSel].wav;

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
	if (percussionMode == false)
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
	if (percussionMode == false)
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
	if (percussionMode == false)
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
	if (percussionMode == false)
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

		tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.base);
		mBase.SetWindowText(tempStr);

		tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.len);
		mLength.SetWindowText(tempStr);

		tempStr.Format("%f", *reinterpret_cast<float*> (&alBankCurrent->alPercussion[percussionChoice].unknown2));
		mReleaseTime.SetWindowText(tempStr);

		if ( alBankCurrent->alPercussion[percussionChoice].wav.type == AL_ADPCM_WAVE)
		{
			mLoopingEnabled.SetCheck((alBankCurrent->alPercussion[percussionChoice].wav.adpcmWave != NULL) && (alBankCurrent->alPercussion[percussionChoice].wav.adpcmWave->loop != NULL));
			mLoopingEnabled.EnableWindow(true);

			if ((alBankCurrent->alPercussion[percussionChoice].wav.adpcmWave != NULL) && (alBankCurrent->alPercussion[percussionChoice].wav.adpcmWave->loop != NULL))
			{
				tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.adpcmWave->loop->start);
				mLoopStart.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.adpcmWave->loop->end);
				mLoopEnd.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.adpcmWave->loop->count);
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
		else if ( alBankCurrent->alPercussion[percussionChoice].wav.type == AL_RAW16_WAVE)
		{
			mLoopingEnabled.SetCheck((alBankCurrent->alPercussion[percussionChoice].wav.rawWave != NULL) && (alBankCurrent->alPercussion[percussionChoice].wav.rawWave->loop != NULL));

			mLoopingEnabled.EnableWindow(true);
			if ((alBankCurrent->alPercussion[percussionChoice].wav.rawWave != NULL) &&(alBankCurrent->alPercussion[percussionChoice].wav.rawWave->loop != NULL))
			{
				tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.rawWave->loop->start);
				mLoopStart.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.rawWave->loop->end);
				mLoopEnd.SetWindowText(tempStr);

				tempStr.Format("%08X", alBankCurrent->alPercussion[percussionChoice].wav.rawWave->loop->count);
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

		percussionMode = true;
		ShowPercussionControls();

		dontupdateitall = true;
	}
}