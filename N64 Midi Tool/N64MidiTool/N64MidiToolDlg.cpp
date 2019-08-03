// N64MidiToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "N64MidiTool.h"
#include "N64MidiToolDlg.h"
#include "FolderDialog.h"
#include <sys/stat.h>
#include "..\N64MidiLibrary\SharedFunctions.h"
#include "..\N64MidiToolReader\N64MidiToolReader.h"
#include "..\N64MidiLibrary\NaganoDecoder.h"
#include "..\N64MidiLibrary\NintendoEncoder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CN64MidiToolDlg dialog




CN64MidiToolDlg::CN64MidiToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CN64MidiToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CN64MidiToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOGAME, m_game);
	DDX_Control(pDX, IDC_COMBOMIDI, m_spot);
	DDX_Control(pDX, IDC_CHECKLOOP, m_loop);
	DDX_Control(pDX, IDC_EDITNUMINSTRUMENTS, m_numberInstruments);
	DDX_Control(pDX, IDC_EDITLOOPPOINT, mLoopPoint);
	DDX_Control(pDX, IDC_CHECKLOOP2, mNoRepeaters);
	DDX_Control(pDX, IDC_CHECKSEPARATEBYISNTRUMENT, mSeparateByInstrument);
	DDX_Control(pDX, IDC_CHECKDEBUGTEXTFILE, mDebugTextFile);
	DDX_Control(pDX, IDC_CHECKSHOWBFX, mShowEffectsGames);
	DDX_Control(pDX, IDC_MASTERTRACKEFFECTLABEL, mMasterTrackEffectLabel);
	DDX_Control(pDX, IDC_COMBOMASTERTRACKEFFECT, mMasterTrackEffect);
	DDX_Control(pDX, IDC_CHECKOUTPUTLOOP, mOutputLoop);
	DDX_Control(pDX, IDC_EDITOUTPUTLOOPCOUNT, mOutputLoopCount);
	DDX_Control(pDX, IDC_CHECKEXTENDLOOPSTOHIGHESTTRACK, mExtendSmallerTracksToEnd);
	DDX_Control(pDX, IDC_MIDITYPE, mMidiType);
	DDX_Control(pDX, IDC_GAMENAME, mGameName);
	DDX_Control(pDX, IDC_COMBOMASTERTRACKEFFECT2, mMasterTrackEffect2);
	DDX_Control(pDX, IDC_MASTERTRACKEFFECTLABEL2, mMasterTrackEffectLabel2);
	DDX_Control(pDX, IDC_GROUPEFFECTS, mEffectGroupBox);
	DDX_Control(pDX, IDC_MASTERTRACKEFFECTLABEL3, mPitchBendSensitivityLabel);
	DDX_Control(pDX, IDC_COMBOPITCHBENDSENSITIVITY, mPitchBendSensitivity);
}

BEGIN_MESSAGE_MAP(CN64MidiToolDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONLOADROM, &CN64MidiToolDlg::OnBnClickedButtonloadrom)
	ON_CBN_SELCHANGE(IDC_COMBOMIDI, &CN64MidiToolDlg::OnCbnSelchangeCombomidi)
	ON_BN_CLICKED(IDC_BUTTONExportBin, &CN64MidiToolDlg::OnBnClickedButtonexportbin)
	ON_BN_CLICKED(IDC_BUTTONExportMidi, &CN64MidiToolDlg::OnBnClickedButtonexportmidi)
	ON_BN_CLICKED(IDC_BUTTONImportBin, &CN64MidiToolDlg::OnBnClickedButtonimportbin)
	ON_BN_CLICKED(IDC_BUTTONImportMidi, &CN64MidiToolDlg::OnBnClickedButtonimportmidi)
	ON_BN_CLICKED(IDC_BUTTONEXPORTALLTOMIDI, &CN64MidiToolDlg::OnBnClickedButtonexportalltomidi)
	ON_BN_CLICKED(IDC_BUTTONEXPORTALLTORAWBIN, &CN64MidiToolDlg::OnBnClickedButtonexportalltorawbin)
	ON_BN_CLICKED(IDC_BUTTONWriteROM, &CN64MidiToolDlg::OnBnClickedButtonwriterom)
	ON_BN_CLICKED(IDC_BUTTONPLAYMIDI, &CN64MidiToolDlg::OnBnClickedButtonplaymidi)
	ON_BN_CLICKED(IDC_BUTTONSCANDIRFORMIDIS, &CN64MidiToolDlg::OnBnClickedButtonscandirformidis)
	ON_BN_CLICKED(IDC_BUTTONRIPROMS, &CN64MidiToolDlg::OnBnClickedButtonriproms)
	ON_BN_CLICKED(IDC_BUTTONLOOPDEBUG, &CN64MidiToolDlg::OnBnClickedLoopDebug)
	ON_BN_CLICKED(IDC_BUTTONGENMIDIS, &CN64MidiToolDlg::OnBnClickedButtongenmidis)
	ON_BN_CLICKED(IDC_CHECKSHOWBFX, &CN64MidiToolDlg::OnBnClickedCheckshowbfx)
	ON_BN_CLICKED(IDC_CHECKEXTENDLOOPSTOHIGHESTTRACK, &CN64MidiToolDlg::OnBnClickedCheckextendloopstohighesttrack)
	ON_BN_CLICKED(IDC_CHECKOUTPUTLOOP, &CN64MidiToolDlg::OnBnClickedCheckoutputloop)
END_MESSAGE_MAP()


// CN64MidiToolDlg message handlers

BOOL CN64MidiToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText("N64 Midi Tool V2 by SubDrag " __DATE__);

	// TEST CODE
	/*CNintendoEncoder ninEnc;
	
	CString sourceTestSr = "C:\\temp\\yay.bin";
	int sizeSource = GetSizeFile(sourceTestSr);
	FILE* inSource = fopen(sourceTestSr, "rb");
	unsigned char* sourceTest = new unsigned char[sizeSource];
	fread(sourceTest, 1, sizeSource, inSource);
	fclose(inSource);

	unsigned char* outputBuffer = new unsigned char[0x500000];
	int decodeSize = ninEnc.decode(sourceTest, sizeSource, outputBuffer);

	CString inputEncodedStr = "C:\\temp\\re.test";
	FILE* outRe = fopen(inputEncodedStr, "wb");
	fwrite(outputBuffer, 1, decodeSize, outRe);
	fclose(outRe);*/

	/*CString sourceTestSr = "C:\\temp\\mio0.bin.bin";
	int decodeSize = GetSizeFile(sourceTestSr);
	FILE* inSource = fopen(sourceTestSr, "rb");
	unsigned char* outputBuffer = new unsigned char[decodeSize];
	fread(outputBuffer, 1, decodeSize, inSource);
	fclose(inSource);

	unsigned char* recompressed = new unsigned char[0x500000];
	int encodedSize = ninEnc.encode(outputBuffer, decodeSize, recompressed, "Yay0");

	CString reEncodedStr = "C:\\temp\\re.bin";
	FILE* outRe2 = fopen(reEncodedStr, "wb");
	fwrite(recompressed, 1, encodedSize, outRe2);
	fclose(outRe2);

	delete [] outputBuffer;
	delete [] recompressed;*/
	//delete [] sourceTest;

	/*CNaganoDecoder naganoDecoder;

	CString sourceTestSr = "C:\\temp\\KonamiSong.bin";
	int sizeSource = GetSizeFile(sourceTestSr);
	FILE* inSource = fopen(sourceTestSr, "rb");
	unsigned char* sourceTest = new unsigned char[sizeSource];
	fread(sourceTest, 1, sizeSource, inSource);
	fclose(inSource);
	
	unsigned char* outputBuffer = new unsigned char[0x500000];
	int compressedSize = -1;
	int compSize = naganoDecoder.EncodeKonamiLZW(sourceTest, sizeSource, outputBuffer, 0x500000, compressedSize, 3);

	CString inputEncodedStr = "C:\\temp\\re.test";
	FILE* outRe = fopen(inputEncodedStr, "wb");
	fwrite(outputBuffer, 1, compressedSize, outRe);
	fclose(outRe);

	int sizeInput = GetSizeFile(inputEncodedStr);
	FILE* inInput = fopen(inputEncodedStr, "rb");
	unsigned char* compressedTest = new unsigned char[sizeInput];
	fread(compressedTest, 1, sizeInput, inInput);
	fclose(inInput);

	unsigned char* output = new unsigned char[0x500000];
	int decompressedSize = naganoDecoder.dec(compressedTest, sizeInput, output);

	FILE* outFile = fopen("C:\\temp\\dec.bin", "wb");
	fwrite(output, 1, decompressedSize, outFile);
	fclose(outFile);

	delete [] compressedTest;
	delete [] output;

	delete [] sourceTest;*/

	

	// END TEST
	

	mSeparateByInstrument.SetCheck(false);
	mOutputLoop.SetCheck(false);
	mOutputLoopCount.SetWindowText("1");

	mExtendSmallerTracksToEnd.SetCheck(false);

	mPitchBendSensitivity.SelectString(0, "4");
	mLoopPoint.SetWindowText("0");
	int iniOption;
	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	mainFolder.Format("%s\\", tempFolder);


	/*FILE* inTemp = fopen("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\WCW-nWo Revenge (U) [!].z64", "rb");
	unsigned char* tempChar = new unsigned char[0x1000000];
	fread(tempChar, 1, 0x1000000, inTemp);
	fclose(inTemp);

	unsigned long data[0x1DEA];
	int increment = 0;


CASMICDecoder decoder;
for (int x = 0; x < increment - 1; x++)
{
		unsigned char* outputDecompressed = new unsigned char[0x1000000];
		unsigned long address = data[x];
		
		int fileSize = decoder.dec(&tempChar[address], (data[x+1] - data[x]), outputDecompressed);

		if (fileSize > 0)
		{
			CString tempStr;
			tempStr.Format("%08X.bin", address);
			FILE* outFile = fopen("C:\\temp\\wcwnwor\\" + tempStr, "wb");
			for (int x = 0; x < fileSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);	
		}

		delete [] outputDecompressed;
}
		delete [] tempChar;*/
	

	/*GECompression compress;
	compress.SetGame(NOHEADER);
	compress.SetPath(mainFolder);

	FILE* inTemp = fopen("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\Starshot - Space Circus Fever (E) (M3) [!].z64", "rb");
	unsigned char* tempChar = new unsigned char[0xC00000];
	fread(tempChar, 1, 0xC00000, inTemp);
	fclose(inTemp);

	for (int x = 0x000B55A0; x < 0x000B55A0 + 0x28B18; x+=0x18)
	{
		unsigned long offset = CharArrayToLong(&tempChar[x + 0xC]) + 0xB5590;
		unsigned long uncompressedSize = CharArrayToLong(&tempChar[x + 0x0]);
		unsigned long compressedSize = CharArrayToLong(&tempChar[x + 0x4]);
		unsigned long offsetName = CharArrayToLong(&tempChar[x + 0x8]) + 0xB5590 + 2;

		CString tempStr;
		tempStr.Format("%08X_%s", offset, &tempChar[offsetName]);

		if (tempChar[x + 0x10] == 0x04)
		{
			compress.SetCompressedBuffer(&tempChar[offset], compressedSize);

			
			int fileSize = -1;
			int fileSizeCompressed = -1;
			unsigned char* outputDecompressed = compress.OutputDecompressedBuffer(fileSize, fileSizeCompressed);

			if (fileSize > 0)
			{
				FILE* outFile = fopen("C:\\temp\\starshot\\" + tempStr, "wb");
				for (int x = 0; x < fileSize; x++)
				{
					fwrite(&outputDecompressed[x], 1, 1, outFile);
				}
				fclose(outFile);	

				delete [] outputDecompressed;
			}
		}
		else
		{
			FILE* outFile = fopen("C:\\temp\\starshot\\" + tempStr, "wb");
			for (int x = 0; x < uncompressedSize; x++)
			{
				fwrite(&tempChar[offset + x], 1, 1, outFile);
			}
			fclose(outFile);	
		}
	}

	delete [] tempChar;*/


	buffer = NULL;
	compressed = false;
	startSpot = 0;
	endSpot = 0;
	lastRomName = "";
	romSize = 0;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_game.ResetContent();

	InitializeSpecificGames();
	
	m_game.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CN64MidiToolDlg::InitializeSpecificGames()	
{
	char folder[8000];
	strcpy(folder, mainFolder);
	strcat(folder, "gameconfigmidi.ini");

	CN64MidiToolReader::InitializeSpecificGames(folder, countGames, gameConfig);

	OnBnClickedCheckshowbfx();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CN64MidiToolDlg::OnPaint()
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
HCURSOR CN64MidiToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


unsigned long CN64MidiToolDlg::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned long CN64MidiToolDlg::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CN64MidiToolDlg::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CN64MidiToolDlg::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

int CN64MidiToolDlg::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void CN64MidiToolDlg::OnBnClickedButtonloadrom()
{
	CString tempStr;
	m_game.GetWindowText(tempStr);
	tempStr.Replace(" (Binary FX)", "");
	tempStr.Replace(" RNC", "");
	tempStr.Replace(" AVL_0", "");

	CFileDialog m_ldFile(TRUE, NULL, tempStr + " [!].z64", OFN_HIDEREADONLY, "N64 ROM(*.v64;*.z64;*.rom;*.n64;*.ndd)|*.v64;*.z64;*.rom;*.n64;*.ndd|", this);
	
	int statusFileOpen = (int) m_ldFile.DoModal();

	CString romName = m_ldFile.GetPathName();

	//int statusFileOpen = IDOK;
	//romName = "C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\GoldenEye 007 (U) [!].z64";

	if ((statusFileOpen == IDCANCEL) || (romName == ""))
		return;

	if (statusFileOpen == FALSE)
		return;	


	romSize = GetSizeFile(romName);

	if ((statusFileOpen == IDOK) && (romSize > 0))
	{
		

		m_game.GetWindowText(gameName);


		FILE* inROM = fopen(romName, "rb");

		if (inROM == NULL)
		{
			MessageBox("Error opening ROM");
			return;
		}
		if (buffer != NULL)
		{
			delete [] buffer;
			buffer = NULL;
		}

		lastRomName = romName;

		buffer = new unsigned char[romSize];
		fread(buffer, 1, romSize, inROM);
		fclose(inROM);

		bool byteFlippedROM = false;
		if (buffer[0] == 0x37)
			byteFlippedROM = true;
		else if (buffer[0] == 0x80)
			byteFlippedROM = false;
		else
		{
			//MessageBox("Not a valid ROM", "Error");
			//return;
		}

		if (byteFlippedROM == true)
		{
			for (int x = 0; x < romSize; x=x+2)
			{
				unsigned char tempSpot = buffer[x];
				buffer[x] = buffer[x+1];
				buffer[x+1] = tempSpot;
			}
		}	
			

		m_spot.ResetContent();

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		mGameName.ShowWindow(SW_SHOW);
		mMidiType.ShowWindow(SW_SHOW);
		mGameName.SetWindowText(gameConfig[gameNumber].gameName);
		mMidiType.SetWindowText(gameConfig[gameNumber].gameType);

		std::vector<CString> addMidiStrings;
		int numberMidiStrings = 0;
		int numberInstruments = 0;
		CN64MidiToolReader::ProcessMidis(gameConfig, gameNumber, addMidiStrings, numberMidiStrings, numberInstruments, compressed, buffer, romSize, startSpot, endSpot, true, mSeparateByInstrument.GetCheck(), mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);

		for (int x = 0; x < numberMidiStrings; x++)
			m_spot.AddString(addMidiStrings[x]);

		CString tempInstrStr;
		tempInstrStr.Format("%02X", numberInstruments);
		m_numberInstruments.SetWindowText(tempInstrStr);

		if (gameConfig[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
		{
			mExtendSmallerTracksToEnd.SetWindowText("Combine Sub-segments");
			mPitchBendSensitivityLabel.ShowWindow(SW_HIDE);
			mPitchBendSensitivity.ShowWindow(SW_HIDE);
		}
		else
		{
			mExtendSmallerTracksToEnd.SetWindowText("Extend Short Tracks");
		}
		if ((gameConfig[gameNumber].gameType.CompareNoCase("Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("BlitzSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("RNCSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ASMICSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("SnowSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ARISng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ArmySargeSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ArmySarge2Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Quake2Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("TazSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("FLA2Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("RNCSngOffset") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("LZSS_0BSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("AVL_0Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("VigilanteSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("RugratsSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("MultiPartTigSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ZLibSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ZLibIndexedSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("MarioTennisSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Yay0Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ZipSng") == 0)
			)
		{
			mMasterTrackEffect.SetCurSel(0);
			mMasterTrackEffectLabel.SetWindowText("Master Track Effect");
			mMasterTrackEffect.ResetContent();
			mMasterTrackEffect.AddString("00 = None");
			mMasterTrackEffect.AddString("01 = Small Room");
			mMasterTrackEffect.AddString("02 = Big Room");
			mMasterTrackEffect.AddString("03 = Chorus");
			mMasterTrackEffect.AddString("04 = Flange");
			mMasterTrackEffect.AddString("05 = Echo");
			mEffectGroupBox.ShowWindow(SW_SHOW);
			mMasterTrackEffectLabel.ShowWindow(SW_SHOW);
			mMasterTrackEffectLabel2.ShowWindow(SW_HIDE);
			mMasterTrackEffect.ShowWindow(SW_SHOW);
			mMasterTrackEffect2.ShowWindow(SW_HIDE);
			mSeparateByInstrument.ShowWindow(SW_SHOW);
			mPitchBendSensitivityLabel.ShowWindow(SW_HIDE);
			mPitchBendSensitivity.ShowWindow(SW_HIDE);
		}
		else if ((gameConfig[gameNumber].gameType.CompareNoCase("Konami") == 0))
		{
			mEffectGroupBox.ShowWindow(SW_SHOW);
			mMasterTrackEffectLabel.ShowWindow(SW_SHOW);
			mMasterTrackEffectLabel.SetWindowText("Separation Amt");
			mMasterTrackEffectLabel2.ShowWindow(SW_SHOW);
			mMasterTrackEffectLabel2.SetWindowText("Echo Length");

			mMasterTrackEffect.ResetContent();
			mMasterTrackEffect2.ResetContent();

			for (int x = 0; x < 0x100; x++)
			{
				CString tempStr;
				tempStr.Format("%02X", x);
				mMasterTrackEffect.AddString(tempStr);
				mMasterTrackEffect2.AddString(tempStr);
			}

			mMasterTrackEffect.SetCurSel(0);
			mMasterTrackEffect.ShowWindow(SW_SHOW);
			mMasterTrackEffect2.SetCurSel(0);
			mMasterTrackEffect2.ShowWindow(SW_SHOW);
			
			mSeparateByInstrument.ShowWindow(SW_SHOW);
			mPitchBendSensitivityLabel.ShowWindow(SW_HIDE);
			mPitchBendSensitivity.ShowWindow(SW_HIDE);
		}
		else if (gameConfig[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
		{
			mEffectGroupBox.ShowWindow(SW_SHOW);
			mMasterTrackEffectLabel.ShowWindow(SW_HIDE);
			mMasterTrackEffectLabel2.ShowWindow(SW_HIDE);
			mMasterTrackEffect.ShowWindow(SW_HIDE);
			mMasterTrackEffect2.ShowWindow(SW_HIDE);
			mSeparateByInstrument.ShowWindow(SW_SHOW);
			mPitchBendSensitivityLabel.ShowWindow(SW_HIDE);
			mPitchBendSensitivity.ShowWindow(SW_HIDE);
		}
		else if ((gameConfig[gameNumber].gameType.CompareNoCase("Factor5Zlb") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Factor5ZlbGCStyle") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Factor5LZGCStyle") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0))
		{
			mEffectGroupBox.ShowWindow(SW_HIDE);
			mMasterTrackEffectLabel.ShowWindow(SW_HIDE);
			mMasterTrackEffectLabel2.ShowWindow(SW_HIDE);
			mMasterTrackEffect.ShowWindow(SW_HIDE);
			mMasterTrackEffect2.ShowWindow(SW_HIDE);
			mSeparateByInstrument.ShowWindow(SW_SHOW);
			mPitchBendSensitivityLabel.ShowWindow(SW_HIDE);
			mPitchBendSensitivity.ShowWindow(SW_HIDE);
		}
		else if (
			(gameConfig[gameNumber].gameType.CompareNoCase("BanjoKazooie") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("BanjoTooie") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("DonkeyKong") == 0)
			)
		{
			mEffectGroupBox.ShowWindow(SW_HIDE);
			mMasterTrackEffectLabel.ShowWindow(SW_HIDE);
			mMasterTrackEffectLabel2.ShowWindow(SW_HIDE);
			mMasterTrackEffect.ShowWindow(SW_HIDE);
			mMasterTrackEffect2.ShowWindow(SW_HIDE);
			mSeparateByInstrument.ShowWindow(SW_HIDE);
			mPitchBendSensitivityLabel.ShowWindow(SW_SHOW);
			mPitchBendSensitivity.ShowWindow(SW_SHOW);
		}
		else
		{
			mEffectGroupBox.ShowWindow(SW_HIDE);
			mMasterTrackEffectLabel.ShowWindow(SW_HIDE);
			mMasterTrackEffectLabel2.ShowWindow(SW_HIDE);
			mMasterTrackEffect.ShowWindow(SW_HIDE);
			mMasterTrackEffect2.ShowWindow(SW_HIDE);
			mSeparateByInstrument.ShowWindow(SW_HIDE);
			mPitchBendSensitivityLabel.ShowWindow(SW_HIDE);
			mPitchBendSensitivity.ShowWindow(SW_HIDE);
		}

		if (gameConfig[gameNumber].gameType.CompareNoCase("Konami") == 0)
		{
			OnBnClickedCheckextendloopstohighesttrack();
			OnBnClickedCheckoutputloop();

			mOutputLoop.ShowWindow(SW_SHOW);
			mOutputLoopCount.ShowWindow(SW_SHOW);

			mExtendSmallerTracksToEnd.ShowWindow(SW_SHOW);
		}
		else if ((gameConfig[gameNumber].gameType.CompareNoCase("Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("BlitzSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("RNCSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ASMICSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("SnowSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ARISng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ArmySargeSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ArmySarge2Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Quake2Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("TazSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("FLA2Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("RNCSngOffset") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("LZSS_0BSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("AVL_0Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("VigilanteSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("RugratsSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("MultiPartTigSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ZLibSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ZLibIndexedSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("MarioTennisSng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Yay0Sng") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ZipSng") == 0)
			)
		{
			mOutputLoop.ShowWindow(SW_HIDE);
			mOutputLoopCount.ShowWindow(SW_HIDE);

			mExtendSmallerTracksToEnd.ShowWindow(SW_HIDE);
		}
		else if (
			(gameConfig[gameNumber].gameType.CompareNoCase("Yaz0Seq64") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Yaz0EADZelda") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("ZLibEADZelda") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("EADZelda") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("EADMario") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("EADStarFox") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Seq64Zelda") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Seq64Mario") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Midi") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("MidiLZSSWilliams") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("MIDx") == 0)
			|| (gameConfig[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
			)
		{
			mOutputLoop.ShowWindow(SW_HIDE);
			mOutputLoopCount.ShowWindow(SW_HIDE);

			mExtendSmallerTracksToEnd.ShowWindow(SW_SHOW);
		}
		else
		{
			mOutputLoop.ShowWindow(SW_HIDE);
			mOutputLoopCount.ShowWindow(SW_HIDE);

			mExtendSmallerTracksToEnd.ShowWindow(SW_SHOW);
		}
		
		

		if (m_spot.GetCount() > 0)
		{
			m_spot.SetCurSel(0);
			OnCbnSelchangeCombomidi();
		}
	}
}

CN64MidiToolDlg::~CN64MidiToolDlg()
{
	if (gameConfig != NULL)
	{
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].midiBanks != NULL)
				delete [] gameConfig[x].midiBanks;

			if (gameConfig[x].midiNames != NULL)
				delete [] gameConfig[x].midiNames;
		}

		delete [] gameConfig;
	}

	if (buffer != NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}
}
void CN64MidiToolDlg::OnCbnSelchangeCombomidi()
{

	
}

void CN64MidiToolDlg::OnBnClickedButtonexportbin()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	unsigned long extra = 0;
	unsigned long extra2 = 0;
	
	int originalSize = spotStr.GetLength();
	CString tempColonReplacedStr = spotStr;
	int countColons = tempColonReplacedStr.Replace(":", "");

	if (countColons == 3)
		sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
	else if (countColons == 2)
		sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
	else
		sscanf(spotStr, "%08X:%08X", &address, &size);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (gameConfig[gameNumber].gameType.CompareNoCase("EADZelda") == 0)
	{
		unsigned long audioSeqIndex = address;
		unsigned long audioSeqOffset = size;

		unsigned long numberMidi = CharArrayToShort(&buffer[audioSeqIndex]);

		address = audioSeqOffset + CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10)]);
		size = CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("EADStarFox") == 0)
	{
		unsigned long audioSeqIndex = address;
		unsigned long audioSeqOffset = size;

		unsigned long numberMidi = CharArrayToShort(&buffer[audioSeqIndex]);

		address = audioSeqOffset + CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10)]);
		size = CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("EADMario") == 0)
	{
		unsigned long audioSeqIndex = address;
		unsigned long audioSeqOffset = size;

		unsigned long numberMidi = CharArrayToShort(&buffer[audioSeqIndex]);

		address = audioSeqOffset + CharArrayToLong(&buffer[audioSeqIndex + 4 + (extra * 8)]);
		size = CharArrayToLong(&buffer[audioSeqIndex + 4 + (extra * 8) + 4]);
	}


	
	if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		CFileDialog m_svFile(FALSE, "xm", "FastTracker2.xm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Xm File (*.xm)|*.xm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", "song.dcm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", "song.dcm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", "song.dcm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", "song.dcm", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else if ((gameConfig[gameNumber].gameType.CompareNoCase("RNCSeq") == 0) && compressed)
	{
		CFileDialog m_svFile(FALSE, "bin", "Midi.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		if (compressed)
		{
			int realSpot = size;

			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			int expectedSize = decode.unpackM1(&buffer[address], outputDecompressed, 0x0000, fileSizeCompressed);
			
			unsigned long realStart = CharArrayToLong(&outputDecompressed[0x8]) + CharArrayToLong(&outputDecompressed[realSpot]);
			unsigned long realSize = CharArrayToLong(&outputDecompressed[realSpot + 4]) - CharArrayToLong(&outputDecompressed[realSpot]);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox("Cannot Write File", "Error");
				return;
			}
			for (int x = realStart; x < (realStart + realSize); x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);
		}
	}
	else if ((gameConfig[gameNumber].gameType.CompareNoCase("MidiLZSSWilliams") == 0) && compressed)
	{
		CFileDialog m_svFile(FALSE, "bin", "Midi.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int fileSizeCompressed = size;
		CMidwayDecoder decode;
		unsigned char* outputDecompressed = new unsigned char[0x50000];
		int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed, "WILLIAMS");

		FILE* outFile = fopen(fileName, "wb");
		if (outFile == NULL)
		{
			MessageBox("Cannot Write File", "Error");
			return;
		}
		for (int x = 0; x < expectedSize; x++)
		{
			fwrite(&outputDecompressed[x], 1, 1, outFile);
		}
		fclose(outFile);
	}
	else
	{
		CFileDialog m_svFile(FALSE, "bin", "Midi.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File (*.bin)|*.bin|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		CN64MidiToolReader::midiParse.ExportToBin(gameConfig[gameNumber].gameType, buffer, address, size, fileName, compressed);
	}
}

void CN64MidiToolDlg::OnBnClickedButtonexportmidi()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	unsigned long extra = 0;
	unsigned long extra2 = 0;
	
	int originalSize = spotStr.GetLength();
	CString tempColonReplacedStr = spotStr;
	int countColons = tempColonReplacedStr.Replace(":", "");

	if (countColons == 3)
		sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
	else if (countColons == 2)
		sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
	else
		sscanf(spotStr, "%08X:%08X", &address, &size);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	int midiIndex = m_spot.GetCurSel();

	CString outputName;
	if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		outputName.Format("%s%s %08X %08X.xm", "", gameName, midiIndex, address);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
	{
		outputName.Format("%s%s %08X %08X.dcm", "", gameName, midiIndex, address);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
	{
		outputName.Format("%s%s %08X %08X.dcm", "", gameName, midiIndex, address);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
	{
		outputName.Format("%s%s %08X %08X.dcm", "", gameName, midiIndex, address);
	}
	else
	{
		if (midiIndex < gameConfig[gameNumber].numberMidiNames)
		{
			CString midiName = gameConfig[gameNumber].midiNames[midiIndex];
			midiName.Trim();
			midiName.Replace("\n", "");
			midiName.Replace("\r", "");
			midiName.Replace("*", "");
			midiName.Replace("?", "");
			midiName.Replace("<", "");
			midiName.Replace(">", "");
			midiName.Replace("|", "");

			outputName.Format("%s%s %08X %08X %s.mid", "", gameName, midiIndex, address, midiName);
		}
		else
		{
			outputName.Format("%s%s %08X %08X.mid", "", gameName, midiIndex, address);
		}
	}

	bool generateDebugTextFile = mDebugTextFile.GetCheck();
	if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		CFileDialog m_svFile(FALSE, "xm", outputName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Xm File (*.xm)|*.xm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else if ((gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0))
	{
		CFileDialog m_svFile(FALSE, "dcm", outputName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
	{
		CFileDialog m_svFile(FALSE, "dcm", outputName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Dcm File (*.dcm)|*.dcm|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
	}
	else
	{
		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CFileDialog m_svFile(FALSE, "mid", outputName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Midi File (*.mid)|*.mid|", this);

		int statusFileOpen = (int) m_svFile.DoModal();

		if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
			return;

		if (statusFileOpen == FALSE)
			return;

		CString fileName;
		if (statusFileOpen == IDOK)
		{
			fileName = m_svFile.GetPathName();
		}
		else
		{
			return;
		}

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString gameName;
		m_game.GetWindowText(gameName);

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		bool usePitchBendSensitivity =  (mPitchBendSensitivity.IsWindowVisible());
		CString pitchBendValueStr;
		mPitchBendSensitivity.GetWindowText(pitchBendValueStr);
		int pitchBendSensitivity = atoi(pitchBendValueStr);
		if (pitchBendSensitivity == 4)
			usePitchBendSensitivity = false;

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo, usePitchBendSensitivity, pitchBendSensitivity);
	}


}

void CN64MidiToolDlg::KillWMPlayer()
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

void CN64MidiToolDlg::ConvertIntoSpot(CString inputFile)
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	unsigned long extra = 0;
	unsigned long extra2 = 0;
	
	int originalSize = spotStr.GetLength();
	CString tempColonReplacedStr = spotStr;
	int countColons = tempColonReplacedStr.Replace(":", "");

	if (countColons == 3)
		sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
	else if (countColons == 2)
		sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
	else
		sscanf(spotStr, "%08X:%08X", &address, &size);

	int fileSize = GetSizeFile(inputFile);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (compressed)
	{
		if (gameConfig[gameNumber].gameType == "Konami")
		{
			int sizeInputFile = GetSizeFile(inputFile);

			FILE* inFile = fopen(inputFile, "rb");
			if (inFile == NULL)
			{
				MessageBox("Error reading file");
				return;
			}

			unsigned char* sourceBuffer = new unsigned char[sizeInputFile];
			fread(sourceBuffer, 1, sizeInputFile, inFile);
			fclose(inFile);

			CNaganoDecoder naganoDecoder;
			unsigned char* outputBuffer = new unsigned char[0x500000];
			int compressedSize = -1;
			int compressedRealSize = -1;
			int compSize = naganoDecoder.EncodeKonamiLZW(sourceBuffer, sizeInputFile, outputBuffer, 0x500000, compressedSize, compressedRealSize, 3);

			if (compSize <= 0)
			{
				MessageBox("Unsuccessful compression");
				delete [] outputBuffer;
				return;
			}

			unsigned char* testRe = new unsigned char[0x500000];
			int reSize = naganoDecoder.dec(outputBuffer, compressedSize, testRe);

			if (reSize == 0)
			{
				delete [] sourceBuffer;
				delete [] outputBuffer;
				delete [] testRe;
				MessageBox("Invalid compression, can't reoutput");
				return;
			}

			for (int x = 0; x < sizeInputFile; x++)
			{
				if (testRe[x] != sourceBuffer[x])
				{
					delete [] sourceBuffer;
					delete [] outputBuffer;
					delete [] testRe;
					MessageBox("Invalid compression, not equal to start");
					return;
				}
			}
			delete [] testRe;
			delete [] sourceBuffer;
			
			signed long change = (compSize - size);

			unsigned long fileAddress = gameConfig[gameNumber].extraGameMidiInfo.trackAddressOffset + ((extra) * 4);
			unsigned long fileAddressData = CharArrayToLong(&buffer[fileAddress]);

			unsigned long nextAddress = gameConfig[gameNumber].extraGameMidiInfo.trackAddressOffset + ((extra + 1) * 4);
			unsigned long nextEndAddress = gameConfig[gameNumber].extraGameMidiInfo.trackAddressEndsOffset + ((extra + 1) * 4);

			unsigned long lastAddress = gameConfig[gameNumber].extraGameMidiInfo.trackAddressOffset + ((gameConfig[gameNumber].numberMidis) * 4);

			
			unsigned long lastEndAddress = gameConfig[gameNumber].extraGameMidiInfo.trackAddressEndsOffset + ((gameConfig[gameNumber].numberMidis) * 4);

			unsigned long endSpot = gameConfig[gameNumber].extraGameMidiInfo.tracksEnd;

			if (compSize == size)
			{

			}
			else if (compSize < size)
			{
				for (unsigned long addressOffset = nextAddress; addressOffset < lastAddress; addressOffset += 4)
				{
					unsigned long address = CharArrayToLong(&buffer[addressOffset]);

					CN64MidiToolReader::midiParse.WriteLongToBuffer(buffer, addressOffset, address + change);
				}

				memcpy(&buffer[address + compSize], &buffer[address + size], (endSpot - (address + size)));
				for (int x = (address + compSize + (endSpot - (address + size))); x < endSpot; x++)
				{
					buffer[x] = 0x00;
				}

				for (unsigned long addressOffset = nextEndAddress; addressOffset < lastEndAddress; addressOffset += 4)
				{
					unsigned long address = CharArrayToLong(&buffer[addressOffset]);

					CN64MidiToolReader::midiParse.WriteLongToBuffer(buffer, addressOffset, (signed long)address + change);
				}
			}
			else// if (compSize > size)
			{
				unsigned long trueEnd = CharArrayToLong(&buffer[lastEndAddress - 4]);

				if (((signed long)trueEnd + change) > gameConfig[gameNumber].extraGameMidiInfo.tracksEnd)
				{
					delete [] outputBuffer;
					MessageBox("Too big, songs would overtake end of song data");
					return;
				}

				for (unsigned long addressOffset = nextAddress; addressOffset < lastAddress; addressOffset += 4)
				{
					unsigned long address = CharArrayToLong(&buffer[addressOffset]);

					CN64MidiToolReader::midiParse.WriteLongToBuffer(buffer, addressOffset, (signed long)address + change);
				}

				for (int x = ((signed long)trueEnd + change); x >= (fileAddressData + compSize); x--)
				{
					buffer[x] = buffer[x-change];
				}

				for (unsigned long addressOffset = nextEndAddress; addressOffset < lastEndAddress; addressOffset += 4)
				{
					unsigned long address = CharArrayToLong(&buffer[addressOffset]);

					CN64MidiToolReader::midiParse.WriteLongToBuffer(buffer, addressOffset, (signed long)address + change);
				}
			}

			memcpy(&buffer[address], outputBuffer, compSize);

			unsigned long startOffsetWritten = gameConfig[gameNumber].extraGameMidiInfo.trackAddressOffset + (extra * 4);
			unsigned long currentStartOffset = CharArrayToLong(&buffer[startOffsetWritten]);

			unsigned long endOffsetWritten = gameConfig[gameNumber].extraGameMidiInfo.trackAddressEndsOffset + (extra * 4);

			unsigned long newEndOffset = (currentStartOffset + compSize);
			CN64MidiToolReader::midiParse.WriteLongToBuffer(buffer, endOffsetWritten, newEndOffset);

			delete [] outputBuffer;

			int spotSel = m_spot.GetCurSel();

			m_spot.ResetContent();
			for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
			{
				unsigned long startOffsetWritten = gameConfig[gameNumber].extraGameMidiInfo.trackAddressOffset + (gameConfig[gameNumber].midiBanks[x].extra * 4);
				unsigned long start = CharArrayToLong(&buffer[startOffsetWritten]);

				unsigned long endOffsetWritten = gameConfig[gameNumber].extraGameMidiInfo.trackAddressEndsOffset + (gameConfig[gameNumber].midiBanks[x].extra * 4);
				unsigned long end = CharArrayToLong(&buffer[endOffsetWritten]);

				if ((end % 0x10) != 0)
				{
					end = end + (0x10 - (end % 0x10));
				}

				CString tempSpotStr;
				if ((gameConfig[gameNumber].midiBanks[x].extra != 0) && (gameConfig[gameNumber].midiBanks[x].extra2 != 0))
					tempSpotStr.Format("%08X:%08X:%08X:%08X", start, (end - start), gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2);
				else if (gameConfig[gameNumber].midiBanks[x].extra != 0)
					tempSpotStr.Format("%08X:%08X:%08X", start, (end - start), gameConfig[gameNumber].midiBanks[x].extra);
				else
					tempSpotStr.Format("%08X:%08X", start, (end - start));
				m_spot.AddString(tempSpotStr);
			}

			m_spot.SetCurSel(spotSel);
		}
		else if (gameConfig[gameNumber].gameType == "Yay0Sng")
		{
			int sizeInputFile = GetSizeFile(inputFile);

			FILE* inFile = fopen(inputFile, "rb");
			if (inFile == NULL)
			{
				MessageBox("Error reading file");
				return;
			}

			unsigned char* sourceBuffer = new unsigned char[sizeInputFile];
			fread(sourceBuffer, 1, sizeInputFile, inFile);
			fclose(inFile);

			CNintendoEncoder ninEnc;
			unsigned char* outputBuffer = new unsigned char[0x500000];

			int sizeCompressed = ninEnc.encode(sourceBuffer, sizeInputFile, outputBuffer, "Yay0");

			if (sizeCompressed <= 0)
			{
				MessageBox("Unsuccessful compression");
				delete [] sourceBuffer;
				delete [] outputBuffer;
				return;
			}

			if (sizeCompressed > size)
			{
				CString errorStr;
				errorStr.Format("Cannot import, %08X is larger than %08X", sizeCompressed, size);
				MessageBox(errorStr);
				delete [] sourceBuffer;
				delete [] outputBuffer;
				return;
			}

			memcpy(&buffer[address], outputBuffer, sizeCompressed);

			delete [] sourceBuffer;
			delete [] outputBuffer;
		}
		else if (
			(gameConfig[gameNumber].gameType == "RNCSng") ||
			(gameConfig[gameNumber].gameType == "ASMICSng") ||
			(gameConfig[gameNumber].gameType == "SnowSng") ||
			(gameConfig[gameNumber].gameType == "ArmySargeSng") ||
			(gameConfig[gameNumber].gameType == "ArmySarge2Sng") ||
			(gameConfig[gameNumber].gameType == "Quake2Sng") ||
			(gameConfig[gameNumber].gameType == "TazSng") ||
			(gameConfig[gameNumber].gameType == "ARISng") ||
			(gameConfig[gameNumber].gameType == "FLA2Sng") ||
			(gameConfig[gameNumber].gameType == "RNCSngOffset") ||
			(gameConfig[gameNumber].gameType == "BlitzSng") ||
			(gameConfig[gameNumber].gameType == "MarioTennisSng") ||
			(gameConfig[gameNumber].gameType == "ZLibSng") ||
			(gameConfig[gameNumber].gameType == "Factor5Zlb") ||
			(gameConfig[gameNumber].gameType == "Factor5ZlbGCStyle") ||
			(gameConfig[gameNumber].gameType == "Factor5LZGCStyle") ||
			(gameConfig[gameNumber].gameType == "Factor5ZlbNoHeaderGCStyle") ||
			(gameConfig[gameNumber].gameType == "ZLibIndexedSng") ||
			(gameConfig[gameNumber].gameType == "MultiPartTigSng") ||
			(gameConfig[gameNumber].gameType == "LZSS_0BSng") ||
			(gameConfig[gameNumber].gameType == "AVL_0Sng") ||
			(gameConfig[gameNumber].gameType == "VigilanteSng") ||
			(gameConfig[gameNumber].gameType == "RugratsSng") ||
			(gameConfig[gameNumber].gameType == "MidiLZSSWilliams") ||
			(gameConfig[gameNumber].gameType == "TitusMidi") ||
			(gameConfig[gameNumber].gameType == "RNCMidi") ||
			(gameConfig[gameNumber].gameType == "RNCSeq") ||
			(gameConfig[gameNumber].gameType == "MultipartZLibXMFastTracker2") ||
			(gameConfig[gameNumber].gameType == "DCM") ||
			(gameConfig[gameNumber].gameType == "Aidyn") ||
			(gameConfig[gameNumber].gameType == "LZSamplesDCM")
			)

		{
			MessageBox("Unsupported Compression");
			return;
		}
		else
		{
			CompressGZipFile(inputFile, mainFolder + "tempAS734esa.gz", false, (gameConfig[gameNumber].gameType));

			int sizeCompressed = GetSizeFile(mainFolder + "tempAS734esa.gz");

			if (sizeCompressed > size)
			{
				CString errorStr;
				errorStr.Format("Cannot import, %08X is larger than %08X", sizeCompressed, size);
				MessageBox(errorStr);
				return;
			}

			FILE* inFile = fopen((mainFolder + "tempAS734esa.gz"), "rb");
			if (inFile == NULL)
			{
				MessageBox("Error reading file");
				return;
			}
			fread(&buffer[address], 1, sizeCompressed, inFile);
			fclose(inFile);
			::DeleteFile(mainFolder + "tempAS734esa.gz");
		}
	}
	else
	{
		if (
			(gameConfig[gameNumber].gameType == "EADZelda")
			||
			(gameConfig[gameNumber].gameType == "Seq64Zelda")
			)
		{
			unsigned long audioSeqIndex = address;
			unsigned long audioSeqOffset = size;

			unsigned long numberMidi = CharArrayToShort(&buffer[audioSeqIndex]);

			address = audioSeqOffset + CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10)]);
			size = CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);
		}
		else if (
			(gameConfig[gameNumber].gameType == "EADMario")
			||
			(gameConfig[gameNumber].gameType == "Seq64Mario")
			)
		{
			unsigned long audioSeqIndex = address;
			unsigned long audioSeqOffset = size;

			unsigned long numberMidi = CharArrayToShort(&buffer[audioSeqIndex]);

			address = audioSeqOffset + CharArrayToLong(&buffer[audioSeqIndex + 4 + (extra * 8)]);
			size = CharArrayToLong(&buffer[audioSeqIndex + 4 + (extra * 8) + 4]);
		}
		else if (gameConfig[gameNumber].gameType == "EADStarFox")
		{
			unsigned long audioSeqIndex = address;
			unsigned long audioSeqOffset = size;

			unsigned long numberMidi = CharArrayToShort(&buffer[audioSeqIndex]);

			address = audioSeqOffset + CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10)]);
			size = CharArrayToLong(&buffer[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);
		}

		int sizeOnDisk = GetSizeFile(inputFile);

		if (sizeOnDisk > size)
		{
			CString errorStr;
			errorStr.Format("Cannot import, %08X is larger than %08X", sizeOnDisk, size);
			MessageBox(errorStr);
			return;
		}

		FILE* inFile = fopen(inputFile, "rb");
		if (inFile == NULL)
		{
			MessageBox("Error reading file");
			return;
		}
		fread(&buffer[address], 1, sizeOnDisk, inFile);
		fclose(inFile);

	}
}

void CN64MidiToolDlg::OnBnClickedButtonimportbin()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CFileDialog m_ldFile(TRUE, NULL, "Midi.bin", OFN_HIDEREADONLY, "Bin (*.bin)|*.bin|", this);

	int isFileOpened = m_ldFile.DoModal();

	if (isFileOpened == IDCANCEL)
		return;
	
	
	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	unsigned long extra = 0;
	unsigned long extra2 = 0;
	
	int originalSize = spotStr.GetLength();
	CString tempColonReplacedStr = spotStr;
	int countColons = tempColonReplacedStr.Replace(":", "");

	if (countColons == 3)
		sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
	else if (countColons == 2)
		sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
	else
		sscanf(spotStr, "%08X:%08X", &address, &size);

	ConvertIntoSpot(m_ldFile.GetPathName());
}

BOOL CN64MidiToolDlg::IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}


BOOL CN64MidiToolDlg::normalExec (PTSTR pCmdLine, CString currentDirectory)
{
	STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.wShowWindow  = SW_SHOW;
   ZeroMemory(&processInfo, sizeof(processInfo));
   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, NORMAL_PRIORITY_CLASS, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);

   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason GZip Failed", "Error");
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
}

BOOL CN64MidiToolDlg::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESHOWWINDOW;
   si.wShowWindow  = SW_HIDE;
   ZeroMemory(&processInfo, sizeof(processInfo));
   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox("For some reason GZip Failed", "Error");
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
};

bool CN64MidiToolDlg::CompressGZipFile(CString inputFile, CString outputFile, bool byteFlipCompressed, CString gameType)
{
	CString gzipFileName = (mainFolder + "gzip.exe");
	char tempFileExistName[1000];
	strcpy(tempFileExistName, (mainFolder + "gzip.exe"));
	if (IsFileExist(tempFileExistName) == false)
	{
		MessageBox("gzip.exe not found!", "Error");
		return false;
	}

	strcpy(tempFileExistName, inputFile);
	if (IsFileExist(tempFileExistName))
	{
		FILE* tempInputFile = fopen(inputFile, "rb");
		fseek(tempInputFile, 0, SEEK_END);
		unsigned long size = ftell(tempInputFile);

		unsigned char* tempBuffer;
		tempBuffer = new unsigned char[size + 0x10];
		for (int x = 0; x < (size + 0x10); x++)
			tempBuffer[x] = 0;

		fseek(tempInputFile, 0, SEEK_SET);


		fread(tempBuffer, 1, size, tempInputFile);


		FILE* tempOutputFile = fopen((mainFolder+"tempgh9.bin"), "wb");
		if (tempOutputFile == 0)
		{
			delete [] tempBuffer;
			fclose(tempInputFile);
			MessageBox("Cannot Write Temporary File", "Error");
			return false;
		}

		unsigned long compressedSize = size;

		if (((compressedSize % 0x10) != 0))
			compressedSize = ((compressedSize - (compressedSize % 0x10)) + 0x10);

		fwrite(tempBuffer, 1, compressedSize, tempOutputFile);	

		fclose(tempInputFile);
		fclose(tempOutputFile);

		delete [] tempBuffer;

		::SetCurrentDirectory(mainFolder);
		CString tempStr = ("gzip.exe -f -q -9 tempgh9.bin");
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));
		CString outputGZippedName = (mainFolder+"TEMPGH9.BIZ");

		strcpy(tempFileExistName, outputGZippedName);
		if (!IsFileExist(tempFileExistName))
		{
			outputGZippedName = (mainFolder+"tempgh9.bin.gz");;
		}

		strcpy(tempFileExistName, outputGZippedName);
		if (IsFileExist(tempFileExistName))
		{
			FILE* inputFileName = fopen(outputGZippedName, "rb");
			int sizeNew = 0;
			fseek(inputFileName, 0, SEEK_END);
			sizeNew = ftell(inputFileName);
			fseek(inputFileName, 0, SEEK_SET);

			unsigned char* tempBufferNew;
			tempBufferNew = new unsigned char[sizeNew];
			fread(tempBufferNew, 1, sizeNew, inputFileName);

			fclose(inputFileName);
			DeleteFile((mainFolder+"TEMPGH9.BIZ"));
			FILE* outputFileName = fopen(outputFile, "wb");
			if (outputFileName == NULL)
			{
				delete [] tempBufferNew;
				MessageBox("Error opening temp output file", "Error");
				return false;
			}

			unsigned long start = 0x16;
			if ((gameType.CompareNoCase("GoldenEye") == 0) || (gameType.CompareNoCase("Killer Instinct") == 0))
			{
				start = start - 2;
				tempBufferNew[start] = 0x11;
				tempBufferNew[start+1] = 0x72;
			}
			else if (gameType.CompareNoCase("PerfectDark") == 0)
			{
				start = start - 5;
				tempBufferNew[start] = 0x11;
				tempBufferNew[start+1] = 0x73;
				tempBufferNew[start+2] = ((size >> 16) & 0xFF);
				tempBufferNew[start+3] = ((size >> 8) & 0xFF);
				tempBufferNew[start+4] = ((size) & 0xFF);
			}
			else if (gameType.CompareNoCase("BanjoKazooie") == 0)
			{
				start = start - 6;
				tempBufferNew[start] = 0x11;
				tempBufferNew[start+1] = 0x72;
				tempBufferNew[start+2] = ((size >> 24) & 0xFF);
				tempBufferNew[start+3] = ((size >> 16) & 0xFF);
				tempBufferNew[start+4] = ((size >> 8) & 0xFF);
				tempBufferNew[start+5] = ((size) & 0xFF);
			}
			else if (gameType.CompareNoCase("DonkeyKong") == 0)
			{
				start = start - 0xA;
				tempBufferNew[start] = 0x1F;
				tempBufferNew[start+1] = 0x8B;
				tempBufferNew[start+2] = 0x08;
				tempBufferNew[start+3] = 0x00;
				tempBufferNew[start+4] = 0x00;
				tempBufferNew[start+5] = 0x00;
				tempBufferNew[start+6] = 0x00;
				tempBufferNew[start+7] = 0x00;
				tempBufferNew[start+8] = 0x02;
				tempBufferNew[start+9] = 0x03;
			}
			else if (gameType.CompareNoCase("BlastCorps") == 0)
			{
				start = start;
			}
			else if (gameType.CompareNoCase("Donkey Kong Demo") == 0)
			{
				start = start - 0x12;
				tempBufferNew[start] = 0x1F;
				tempBufferNew[start+1] = 0x8B;
				tempBufferNew[start+2] = 0x08;
				tempBufferNew[start+3] = 0x08;
				tempBufferNew[start+4] = 0x03;
				tempBufferNew[start+5] = 0x45;
				tempBufferNew[start+6] = 0x75;
				tempBufferNew[start+7] = 0x37;
				tempBufferNew[start+8] = 0x00;
				tempBufferNew[start+9] = 0x03;
				tempBufferNew[start+0xA] = 0x74;
				tempBufferNew[start+0xB] = 0x6D;
				tempBufferNew[start+0xC] = 0x70;
				tempBufferNew[start+0xD] = 0x2E;
				tempBufferNew[start+0xE] = 0x62;
				tempBufferNew[start+0xF] = 0x69;
				tempBufferNew[start+0x10] = 0x6E;
				tempBufferNew[start+0x11] = 0x00;
			}
			else if (gameType.CompareNoCase("BanjoTooie") == 0)
			{

				start = start - 2;
				// is this checksum, not sure?
				tempBufferNew[start] = 0x0;
				tempBufferNew[start+1] = 0x15;
			}
			else if (gameType.CompareNoCase("Conker") == 0)
			{
				start = start - 4;
				tempBufferNew[start] = ((size >> 24) & 0xFF);
				tempBufferNew[start+1] = ((size >> 16) & 0xFF);
				tempBufferNew[start+2] = ((size >> 8) & 0xFF);
				tempBufferNew[start+3] = ((size) & 0xFF);
			}



			if (byteFlipCompressed == true)
			{
				if ((sizeNew%2) == 1)
				{
					tempBufferNew[sizeNew-0x8] = 0;
					sizeNew++;
				}

				for (int x = 0; x < sizeNew; x=x+2)
				{
					unsigned char tempSpot = tempBufferNew[x];
					tempBufferNew[x] = tempBufferNew[x+1];
					tempBufferNew[x+1] = tempSpot;
				}
			}



			fwrite(&tempBufferNew[start], 1, (sizeNew-(start + 8)), outputFileName);	

			unsigned long fileSizeNew = (sizeNew-(start + 8));

			// don't pad
			/*if ((fileSizeNew % 0x8) != 0)
			{
				for (int x = 0; x < (0x8-(fileSizeNew % 0x8)); x++)
				{
					if ((gameName == "BanjoKazooie") || (gameName == "BanjoTooie"))
					{
						unsigned char tempZero = 0xAA;
						fwrite(&tempZero, 1, 1, outputFileName);
					}
					else
					{
						unsigned char tempZero = 0;
						fwrite(&tempZero, 1, 1, outputFileName);
					}
				}
			}*/

			delete [] tempBufferNew;
			fflush(outputFileName);
			fclose(outputFileName);
			return true;
		}
		else
		{
			MessageBox("Error Compressing - GZip didn't spit out a file", "Error");
			return false;
		}
	}

	return false;
}
void CN64MidiToolDlg::OnBnClickedButtonimportmidi()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;
	
	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	CString midiName = "Midi.mid";
	if (gameConfig[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
		midiName.Format("MidiSegment0.mid");
	CFileDialog m_ldFile(TRUE, NULL, midiName, OFN_HIDEREADONLY, "Midi (*.mid)|*.mid|", this);

	int isFileOpened = m_ldFile.DoModal();

	if (isFileOpened == IDCANCEL)
		return;

	CString outputFilename = (mainFolder + "ASDxs34sMIDI.bin");
	if (gameConfig[gameNumber].gameType.CompareNoCase("BanjoTooie") == 0)
	{
		if (!CN64MidiToolReader::midiParse.MidiToBTFormat(m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), 0, mNoRepeaters.GetCheck()))
			return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("MIDx") == 0)
	{
		MessageBox("Unsupported MIDx import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		MessageBox("Unsupported Fast Tracker import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
	{
		MessageBox("Unsupported DCM import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
	{
		MessageBox("Unsupported Aidyn import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
	{
		MessageBox("Unsupported LZSamplesDCM import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("MidiLZSSWilliams") == 0)
	{
		MessageBox("Unsupported Midi LZSS Williams Compressed import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSS_0BSng") == 0)
	{
		MessageBox("Unsupported LZSS_0BSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("AVL_0Sng") == 0)
	{
		MessageBox("Unsupported AVL_0Sng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("VigilanteSng") == 0)
	{
		MessageBox("Unsupported VigilanteSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("RugratsSng") == 0)
	{
		MessageBox("Unsupported RugratsSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("RNCSng") == 0)
	{
		MessageBox("Unsupported RNCSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ASMICSng") == 0)
	{
		MessageBox("Unsupported ASMICSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("SnowSng") == 0)
	{
		MessageBox("Unsupported SnowSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ARISng") == 0)
	{
		MessageBox("Unsupported ARISng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ArmySargeSng") == 0)
	{
		MessageBox("Unsupported ArmySargeSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Quake2Sng") == 0)
	{
		MessageBox("Unsupported Quake2Sng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ArmySarge2Sng") == 0)
	{
		MessageBox("Unsupported ArmySarge2Sng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("TazSng") == 0)
	{
		MessageBox("Unsupported TazSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("FLA2Sng") == 0)
	{
		MessageBox("Unsupported FLA2Sng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("RNCSngOffset") == 0)
	{
		MessageBox("Unsupported RNCSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("BlitzSng") == 0)
	{
		MessageBox("Unsupported BlitzSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("MarioTennisSng") == 0)
	{
		MessageBox("Unsupported MarioTennisSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ZLibSng") == 0)
	{
		MessageBox("Unsupported ZLibSng import");
		return;
	}
	else if ((gameConfig[gameNumber].gameType.CompareNoCase("Factor5Zlb") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Factor5ZlbGCStyle") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Factor5LZGCStyle") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0))
	{
		MessageBox("Unsupported Factor5Zlb import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ZLibIndexedSng") == 0)
	{
		MessageBox("Unsupported ZLibIndexedSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("MultiPartTigSng") == 0)
	{
		MessageBox("Unsupported MultiPartTigSng import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Midi") == 0)
	{
		outputFilename = m_ldFile.GetPathName();
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Yaz0EADZelda") == 0)
	{
		MessageBox("Unsupported Compressed import - you must use Seq64 GUI to do this");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ZLibEADZelda") == 0)
	{
		MessageBox("Unsupported Compressed import");
		return;
	}
	else if (
		(gameConfig[gameNumber].gameType.CompareNoCase("EADZelda") == 0)
		||
		(gameConfig[gameNumber].gameType.CompareNoCase("Seq64Zelda") == 0)
		)
	{
		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		int loop = atoi(tempStr);

		int numberTracks = 0;
		if (!CN64MidiToolReader::midiParse.MidiToEADMusic(EADMUSICSTYLEZELDA, m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), loop))
			return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("EADStarFox") == 0)
	{
		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		int loop = atoi(tempStr);

		int numberTracks = 0;
		if (!CN64MidiToolReader::midiParse.MidiToEADMusic(EADMUSICSTYLESTARFOX, m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), loop))
			return;
	}
	else if (
		(gameConfig[gameNumber].gameType.CompareNoCase("EADMario") == 0)
		||
		(gameConfig[gameNumber].gameType.CompareNoCase("Seq64Mario") == 0)
		)
	{
		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		int loop = atoi(tempStr);

		int numberTracks = 0;
		if (!CN64MidiToolReader::midiParse.MidiToEADMusic(EADMUSICSTYLEMARIO, m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), loop))
			return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Yaz0Seq64") == 0)
	{
		MessageBox("Unsupported Compressed import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("ZipSng") == 0)
	{
		MessageBox("Unsupported Compressed import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("TitusMidi") == 0)
	{
		MessageBox("Unsupported Titus Midi Compressed import");
		return;
	}

	else if (gameConfig[gameNumber].gameType.CompareNoCase("RNCMidi") == 0)
	{
		MessageBox("Unsupported RNCMidi Compressed import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("RNCSeq") == 0)
	{
		MessageBox("Unsupported RNCSeq Compressed import");
		return;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Konami") == 0)
	{
		if (gameConfig[gameNumber].extraGameMidiInfo.naganoCompressed || gameConfig[gameNumber].extraGameMidiInfo.zlbCompressed)
		{
			MessageBox("Unsupported as of yet setting track numbers, so can't import");
			return;
		}

		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		int loop = atoi(tempStr);

		int numberTracks = 0;
		if (!CN64MidiToolReader::midiParse.MidiToKonami(m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), loop, numberTracks, mMasterTrackEffect.GetCurSel(), mMasterTrackEffect2.GetCurSel()))
			return;

		unsigned long trackCountOffset = gameConfig[gameNumber].extraGameMidiInfo.trackOffset + (extra * gameConfig[gameNumber].extraGameMidiInfo.trackIncrement);
		buffer[trackCountOffset] = numberTracks & 0xFF;
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
	{
		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		unsigned long loop = atoi(tempStr);

		CString input[4];
		input[0] = m_ldFile.GetPathName();
		input[1] = "";
		input[2] = "";
		input[3] = "";

		for (int x = 1; x < 4; x++)
		{
			CString midiSegmentStr;
			midiSegmentStr.Format("MidiSegment%X.mid", x);
			CFileDialog m_ldFileSegment(TRUE, NULL, midiSegmentStr, OFN_HIDEREADONLY, "Midi (*.mid)|*.mid|", this);

			int isFileOpenedSegment = m_ldFileSegment.DoModal();

			if (isFileOpenedSegment == IDCANCEL)
				break;

			input[x] = m_ldFileSegment.GetPathName();
		}

		unsigned long name = CharArrayToLong(&buffer[address + 8]);

		if (!CN64MidiToolReader::midiParse.MidiToPaperMario(input, outputFilename, m_loop.GetCheck(), loop, name))
			return;
	}
	else if (
		(gameConfig[gameNumber].gameType.CompareNoCase("Sng") == 0)
		|| (gameConfig[gameNumber].gameType.CompareNoCase("Yay0Sng") == 0)
		)
	{
		if (buffer == NULL)
			return;

		if (m_spot.GetCount() == 0)
			return;

		CString spotStr;
		m_spot.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CMidiParse::SngStyle sngStyle;
		// Second check to avoid false positive of bfx, if count is 0x215, Mario Golf E
		if (buffer[address+0x0] == 0x80) // Ptr
		{
			sngStyle = CMidiParse::SngStyle::PtrBfx;

			MessageBox("Cannot import Binary Effects");
			return;
		}
		else if ((CharArrayToLong(&buffer[address+0x0]) == 0x00000215) && (CharArrayToLong(&buffer[address+0x4]) != 0x00000215)) // Binary Sng style
		{
			sngStyle = CMidiParse::SngStyle::Normal;
		}
		else if (CharArrayToLong(&buffer[address]) == 0x00000000) // Pokemon Stadium Old Bfx
		{
			sngStyle = CMidiParse::SngStyle::OldBfx;

			MessageBox("Cannot import Binary Effects");
			return;
		}
		else if (CharArrayToLong(&buffer[address + 0xC]) == 0x00000000) // Bfx
		{
			sngStyle = CMidiParse::SngStyle::Bfx;

			MessageBox("Cannot import Binary Effects");
			return;
		}
		else if (CharArrayToLong(&buffer[address + 0x8]) == 0x00000020) // Bfx
		{
			sngStyle = CMidiParse::SngStyle::OldDD;

			MessageBox("Cannot import DD Style");
			return;
		}
		else
		{
			sngStyle = CMidiParse::SngStyle::Old;
		}

		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		int loop = atoi(tempStr);

		if (!CN64MidiToolReader::midiParse.MidiToSng(m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), loop, sngStyle, mMasterTrackEffect.GetCurSel()))
			return;

		//if (mDebugTextFile.GetCheck())
			//CN64MidiToolReader::midiParse.SngToDebugTextFile(gameConfig[gameNumber].gameName, 0, outputFilename, m_ldFile.GetPathName() + " TrackParseDebug.txt");
	}
	else
	{
		CString tempStr;
		mLoopPoint.GetWindowText(tempStr);
		int loop = atoi(tempStr);

		if (!CN64MidiToolReader::midiParse.MidiToGEFormat(m_ldFile.GetPathName(), outputFilename, m_loop.GetCheck(), loop, mNoRepeaters.GetCheck()))
			return;
	}
	

	
	ConvertIntoSpot(outputFilename);
	//::DeleteFile((mainFolder+"ASDxs34sMIDI.bin"));
}

void CN64MidiToolDlg::OnBnClickedButtonexportalltomidi()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	for (int x = 0; x < m_spot.GetCount(); x++)
	{
		CString spotStr;
		m_spot.GetLBText(x, spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString gameName;
		m_game.GetWindowText(gameName);

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		if ((size > 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Seq64Mario") == 0)  || (gameConfig[gameNumber].gameType.CompareNoCase("Seq64Zelda") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("Yaz0Seq64") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("ZipSng") == 0) || (gameConfig[gameNumber].gameType.CompareNoCase("ZLibIndexedSng") == 0))
		{
			CString outputName;
			if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
			{
				outputName.Format("%s%s %08X %08X.xm", tempPath, gameName, x, address);
			}
			else if (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
			{
				outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			}
			else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
			{
				outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			}
			else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
			{
				outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			}
			else
			{
				if (x < gameConfig[gameNumber].numberMidiNames)
				{
					CString midiName = gameConfig[gameNumber].midiNames[x];
					midiName.Trim();
					midiName.Replace("\n", "");
					midiName.Replace("\r", "");
					midiName.Replace("*", "");
					midiName.Replace("?", "");
					midiName.Replace("<", "");
					midiName.Replace(">", "");
					midiName.Replace("|", "");

					outputName.Format("%s%s %08X %08X %s.mid", tempPath, gameName, x, address, midiName);
				}
				else
				{
					outputName.Format("%s%s %08X %08X.mid", tempPath, gameName, x, address);
				}
			}
			try
			{
				CString gameName;
				m_game.GetWindowText(gameName);

				int gameNumber = 0;
				for (int x = 0; x < countGames; x++)
				{
					if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
					{
						gameNumber = x;
						break;
					}
				}

				int numberInstruments;
				
				bool usePitchBendSensitivity =  (mPitchBendSensitivity.IsWindowVisible());
				CString pitchBendValueStr;
				mPitchBendSensitivity.GetWindowText(pitchBendValueStr);
				int pitchBendSensitivity = atoi(pitchBendValueStr);
				if (pitchBendSensitivity == 4)
					usePitchBendSensitivity = false;

				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo, usePitchBendSensitivity, pitchBendSensitivity);
			}
			catch (...)
			{
				MessageBox("Exception processing midi, maybe bad auto-read");
			}

		}
		else
		{
			int a =1;
		}
	}
}

void CN64MidiToolDlg::OnBnClickedButtonexportalltorawbin()
{
	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	for (int x = 0; x < m_spot.GetCount(); x++)
	{
		CString spotStr;
		m_spot.GetLBText(x, spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		unsigned long extra = 0;
		unsigned long extra2 = 0;
		
		int originalSize = spotStr.GetLength();
		CString tempColonReplacedStr = spotStr;
		int countColons = tempColonReplacedStr.Replace(":", "");

		if (countColons == 3)
			sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
		else if (countColons == 2)
			sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
		else
			sscanf(spotStr, "%08X:%08X", &address, &size);

		CString gameName;
		m_game.GetWindowText(gameName);

		int gameNumber = 0;
		for (int x = 0; x < countGames; x++)
		{
			if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;

		CString outputName;
		if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
		{
			outputName.Format("%s%s %08X %08X.xm", tempPath, gameName, x, address);
			int numberInstruments;
			CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		}
		else if (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
		{
			outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			int numberInstruments;
			CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		}
		else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
		{
			outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			int numberInstruments;
			CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		}
		else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
		{
			outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			int numberInstruments;
			CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, outputName, gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		}
		else if ((gameConfig[gameNumber].gameType.CompareNoCase("RNCSeq") == 0) && compressed)
		{
			if (compressed)
			{
				int realSpot = size;

				int fileSizeCompressed = -1;
				RncDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x100000];
				int expectedSize = decode.unpackM1(&buffer[address], outputDecompressed, 0x0000, fileSizeCompressed);
				
				unsigned long realStart = CharArrayToLong(&outputDecompressed[0x8]) + CharArrayToLong(&outputDecompressed[realSpot]);
				unsigned long realSize = CharArrayToLong(&outputDecompressed[realSpot + 4]) - CharArrayToLong(&outputDecompressed[realSpot]);

				outputName.Format("%s%s %08X %08X.bin", tempPath, gameName, x, address);

				FILE* outFile = fopen(outputName, "wb");
				if (outFile == NULL)
				{
					MessageBox("Cannot Write File", "Error");
					return;
				}
				for (int x = realStart; x < (realStart + realSize); x++)
				{
					fwrite(&outputDecompressed[x], 1, 1, outFile);
				}
				fclose(outFile);
			}
		}
		else
		{
			outputName.Format("%s%s %08X %08X.bin", tempPath, gameName, x, address);
			CN64MidiToolReader::midiParse.ExportToBin(gameConfig[gameNumber].gameType, buffer, address, size, outputName, compressed);
		}
	}
}

void CN64MidiToolDlg::OnBnClickedButtonwriterom()
{
	CFileDialog m_svFileOutROM(FALSE, NULL, lastRomName + ".rom", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ROM(*.v64;*.z64;*.rom;*.n64;*.ndd)|*.v64;*.z64;*.rom;*.n64;*.ndd|", this);

	int isFileOpened2 = m_svFileOutROM.DoModal();

	if ((isFileOpened2 != IDCANCEL) && (m_svFileOutROM.GetFileName() != ""))
	{
		CString romOutName = m_svFileOutROM.GetPathName();
		FILE* outFile = fopen(romOutName, "wb");
		if (outFile == NULL)
		{
			MessageBox("Bad output file");
			return;
		}

		fwrite(buffer, 1, romSize, outFile);
		fclose(outFile);

		if (GetSizeFile((mainFolder + "rn64crc.exe")) == 0)
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

void CN64MidiToolDlg::OnBnClickedButtonplaymidi()
{
	KillWMPlayer();

	if (buffer == NULL)
		return;

	if (m_spot.GetCount() == 0)
		return;

	CString spotStr;
	m_spot.GetWindowText(spotStr);

	unsigned long address = 0;
	unsigned long size = 0;
	unsigned long extra = 0;
	unsigned long extra2 = 0;
	
	int originalSize = spotStr.GetLength();
	CString tempColonReplacedStr = spotStr;
	int countColons = tempColonReplacedStr.Replace(":", "");

	if (countColons == 3)
		sscanf(spotStr, "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
	else if (countColons == 2)
		sscanf(spotStr, "%08X:%08X:%08X", &address, &size, &extra);
	else
		sscanf(spotStr, "%08X:%08X", &address, &size);

	CString gameName;
	m_game.GetWindowText(gameName);

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}
	int numberInstruments;

	bool hasLoopPoint = false;
	int loopStart = 0;
	int loopEnd = 0;

	if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, mainFolder + "tempAS123123as.xm", gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), false, extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		CString sParameter;
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.xm"), mainFolder, SW_SHOWNORMAL );
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
	{
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, mainFolder + "tempAS123123as.dcm", gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), false, extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		CString sParameter;
		normalExec((mainFolder + "MikIT.exe " + (mainFolder + "tempAS123123as.dcm ") + (mainFolder + "tempAS123123as.wav ") + "300").GetBuffer(), mainFolder);
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.wav"), mainFolder, SW_SHOWNORMAL );
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("Aidyn") == 0)
	{
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, mainFolder + "tempAS123123as.dcm", gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), false, extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		CString sParameter;
		normalExec((mainFolder + "MikIT.exe " + (mainFolder + "tempAS123123as.dcm ") + (mainFolder + "tempAS123123as.wav ") + "300").GetBuffer(), mainFolder);
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.wav"), mainFolder, SW_SHOWNORMAL );
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
	{
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, mainFolder + "tempAS123123as.dcm", gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), false, extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		CString sParameter;
		normalExec((mainFolder + "MikIT.exe " + (mainFolder + "tempAS123123as.dcm ") + (mainFolder + "tempAS123123as.wav ") + "300").GetBuffer(), mainFolder);
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.wav"), mainFolder, SW_SHOWNORMAL );
	}
	else
	{
		CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, romSize, address, size, mainFolder + "tempAS123123as.mid", gameConfig[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), false, extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[gameNumber].extraGameMidiInfo);
		CString sParameter;
		ShellExecute( GetSafeHwnd(), "open", "explorer.exe", (mainFolder + "tempAS123123as.mid"), mainFolder, SW_SHOWNORMAL );
	}


}

int CN64MidiToolDlg::WorkOnFilesIntoADirectory(char* pszFiles, CString fileNames[0x1000])
{
	int numberFiles = 0;

	WIN32_FIND_DATA files;
	HANDLE handle = FindFirstFile(pszFiles, &files);

	if(handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (files.dwFileAttributes & 0x10)
			{
				
			}
			else
			{
				fileNames[numberFiles] = files.cFileName;
				numberFiles++;
			}
		} while(FindNextFile(handle, &files));
	}

	return numberFiles;
}

void CN64MidiToolDlg::OnBnClickedButtonscandirformidis()
{
	/*FILE* readFile = fopen("C:\\Temp\\Midwaydec\\filelists\\Taz Express-NTXE.txt", "r");

	std::vector<CString> lines;
	while (!feof(readFile))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, readFile);
		lines.push_back(currentLine);
	}
	fclose(readFile);

	FILE* outMusic = fopen("C:\\temp\\mus.txt", "wb");
	for (int x = 0; x < lines.size(); x++)
	{
		if (lines[x].Find("bfx.mus") != -1)
		{
			CString name = lines[x].Mid(lines[x].Find("\t") + 1);
			name = name.Mid(name.Find("\t") + 1);
			lines[x] = lines[x].Mid(0, lines[x].Find("\t"));

			lines[x].Replace("\r\n", "");
			lines[x].Replace("\r", "");
			lines[x].Replace("\n", "");
			lines[x].Replace("0x", "");

			name.Replace("\r\n", "");
			name.Replace("\r", "");
			name.Replace("\n", "");

			if (x < (lines.size() - 1))
			{
				CString nextLine = lines[x+1];
				if (nextLine.Find("\t") != -1)
				{
					nextLine = nextLine.Mid(0, nextLine.Find("\t"));

					nextLine.Replace("\r\n", "");
					nextLine.Replace("\r", "");
					nextLine.Replace("\n", "");
					nextLine.Replace("0x", "");

					fprintf(outMusic, "%s,%s\n", lines[x], nextLine);
					fprintf(outMusic, "|%s\n", name);
				}
			}
		}
	}
	fclose(outMusic);


	return;*/

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	char tempPathChar[1000];
	strcpy(tempPathChar, tempPath);
	strcat(tempPathChar, "*.*");

	CFileDialog m_svFile(FALSE, "txt", "midiReport.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Txt File (*.txt)|*.txt|", this);

	int statusFileOpen = (int) m_svFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	CString fileName;
	if (statusFileOpen == IDOK)
	{

		FILE* outFile = fopen(m_svFile.GetPathName(), "w");
		FILE* outFileAlt = fopen(m_svFile.GetPathName() + "alt.txt", "w");

		CString fileNames[0x10000];
		int count = WorkOnFilesIntoADirectory(tempPathChar, fileNames);

		for (int y = 0; y < count; y++)
		{
			
			struct stat results;
			stat((tempPath + fileNames[y]), &results);	

			if (results.st_size == 0)
				continue;

			FILE* inFile = fopen((tempPath + fileNames[y]), "rb");
			if (inFile == NULL)
				continue;
			
			unsigned char* binary = new unsigned char[results.st_size];
			fread(binary, 1, results.st_size, inFile);

			if (CharArrayToLong(&binary[0]) == 0x37804012)
			{
				for (int r = 0; r < results.st_size; r+=2)
				{
					unsigned char spot = binary[r];
					binary[r] = binary[r+1];
					binary[r+1] = spot;
				}
			}

			CString prev = "";

			if (results.st_size > 10000)
			{
				bool wroteNameHeader = false;
				for (unsigned long x = 0; x < (results.st_size - 0x44); x+=4)
				{
					//if ((CharArrayToLong(&binary[x]) == 0x00000044) && (CharArrayToLong(&binary[x + 0x40]) == 0x00000180))
					//if (CharArrayToLong(&binary[x]) == 0x42310001)
					//if (CharArrayToLong(&binary[x]) == 0x59617930) // yay0				
					//if (CharArrayToLong(&binary[x]) == 0x59617A30) // yaz0				
					//if (CharArrayToLong(&binary[x]) == 0x4D494F30) // mio0
					//if ((CharArrayToLong(&binary[x]) == 0x20696E66) && (CharArrayToLong(&binary[x+4]) == 0x6C617465)) // inflate
					//if (CharArrayToLong(&binary[x]) == 0xFFF330C0) // mp3
					//if (CharArrayToLong(&binary[x]) == 0x534E3634) // SN64
					//if (((CharArrayToLong(&binary[x]) & 0xFFFFFF00) == 0x42310000) && (binary[x+4] == 0) && (binary[x+3] > 0x01)) // Ctl multiple banks
					//if (CharArrayToLong(&binary[x]) == 0x4D546864) // Mthd
					//if (CharArrayToLong(&binary[x]) == 0x424F4653) // BOFS
					//if (CharArrayToLong(&binary[x]) == 0x464C4132) //FLA2
					/*if (
						((CharArrayToLong(&binary[x]) == 0x4E363420) && (CharArrayToLong(&binary[x+4]) == 0x50545220))
						|| 
						((CharArrayToLong(&binary[x]) == 0x6279206D) && (CharArrayToLong(&binary[x+4]) == 0x61747375))
						)*/
					//if (CharArrayToLong(&binary[x]) == 0x76706B30) //VPK0
					//if (
						//(CharArrayToLong(&binary[x]) == 0x36346464)
						//&& (CharArrayToLong(&binary[x+4]) == 0x2D4D756C)
						//&& (CharArrayToShort(&binary[x+8]) == 0x7469)
						//)//64dd-Multi
					//if (CharArrayToLong(&binary[x]) == 0x36344444) //64dd
					/*if ((
						(CharArrayToLong(&binary[x]) == 0x2D6C7A73) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C7A34) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C7A35) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6830) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6831) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6832) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6833) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6834) ||

						(CharArrayToLong(&binary[x]) == 0x2D6C6835))
						&& (binary[x+4] == 0x2D)) // lz/lh*/
					//if (CharArrayToLong(&binary[x]) == 0x4D4F5254) //MORT
					if (CharArrayToLong(&binary[x]) == 0x53424E4B) //SBNK
					//if ((CharArrayToLong(&binary[x]) == 0x53444632) && ((x % 0x10) == 0))
					//if ((CharArrayToShort(&binary[x]) == 0x5331) && ((CharArrayToShort(&binary[x+2]) < 0x500)) && (CharArrayToShort(&binary[x+4]) == 0x0000) && (CharArrayToShort(&binary[x+6]) != 0x0000) && (CharArrayToShort(&binary[x+8]) == 0x0000) && (CharArrayToShort(&binary[x+0xA]) != 0x0000) && ((x % 0x10) == 0))
						//if ((CharArrayToLong(&binary[x]) == 0x00000215) && (CharArrayToShort(&binary[x+4]) == 0x0000) && (CharArrayToShort(&binary[x+6]) > 0x0000) && (CharArrayToShort(&binary[x+6]) <= 0x0040 && (CharArrayToLong(&binary[x + 0x28]) == 0x00000000) && (CharArrayToLong(&binary[x + 0x2C]) == 0x00000000)))
					/*if ((CharArrayToLong(&binary[x]) <= 0x00000040) && (CharArrayToLong(&binary[x]) >= 0x00000008)
						 && (CharArrayToLong(&binary[x+4]) >= 0x00000008)
						  && (CharArrayToLong(&binary[x+8]) >= 0x00000008)
						   && (CharArrayToLong(&binary[x+0xC]) >= 0x00000008)
						    && (CharArrayToLong(&binary[x+0x10]) >= 0x00000008)

							&& (CharArrayToLong(&binary[x+4]) <= 0x00001000)
						  && (CharArrayToLong(&binary[x+8]) <= 0x00001000)
						   && (CharArrayToLong(&binary[x+0xC]) <= 0x00001000)
						    && (CharArrayToLong(&binary[x+0x10]) <= 0x00001000)

							 && (CharArrayToLong(&binary[x-8]) != 0x00000215)
							 && (CharArrayToLong(&binary[x-4]) != 0x00000215)
							 && (CharArrayToLong(&binary[x+4]) == 0x00000018)
							&&

						(CharArrayToLong(&binary[x+8]) - (CharArrayToLong(&binary[x+4])) == ((CharArrayToLong(&binary[x+0xC]) - (CharArrayToLong(&binary[x+8])))))
						&& ((CharArrayToLong(&binary[x+8]) - (CharArrayToLong(&binary[x+4]))) == (4 * CharArrayToLong(&binary[x+0])))
						&& ((CharArrayToLong(&binary[x+8]) - (CharArrayToLong(&binary[x+4]))) > 0x20)

						)*/
						

					/*if (
						(CharArrayToLong(&binary[x]) == 0x4E363420)

						&& (
						(CharArrayToLong(&binary[x+4]) == 0x90A20000)
						|| (CharArrayToLong(&binary[x+8]) == 0x24AE0001)
						)
						

						&&
						(CharArrayToLong(&binary[x+0xC]) == 0x03E00008)

						)*/

					/*if (
						(CharArrayToLong(&binary[x]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+4]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+8]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+0xC]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+0x10]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+0x14]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+0x18]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+0x1C]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+0x20]) == 0x00000064)
						&& (CharArrayToLong(&binary[x+0x24]) == 0x00000064)
						)*/
					//if (((CharArrayToShort(&binary[x]) == 0x5431) || (CharArrayToShort(&binary[x]) == 0x5432)))
					{
						/*int sizeT1 = CharArrayToShort(&binary[x + 2]);
						if ((sizeT1 <= 0x30) || (sizeT1 > 0x1000))
							continue;
						if ((x % 4) != 0)
							continue;

						bool continueNext = false;
						for (int r = 0; r < (sizeT1 - 0x30); r += 8)
						{
							if (
								(binary[x + 4 + (r * 8)] == 0x00)
								&&
								(
								(
								(CharArrayToLong(&binary[x + 4 + (r * 8) + 4]) > 0x400)
								&&
								(CharArrayToLong(&binary[x + 4 + (r * 8) + 4]) < 0x8000)
								)
								)
								)
							{
								
							}
							else
							{
								continueNext = true;
								break;
							}
						}
						if (continueNext)
							continue;*/


						/*bool findLoopHead = false;
						bool findLoopEnd = false;
						for (int r = 0; r < 0x1000; r++)
						{
							if (binary[x+r] == 0x96)
								findLoopHead = true;

							if ((binary[x+r] == 0x95) && (binary[x+r+1] == 0xFF))
								findLoopEnd = true;
						}

						if (findLoopHead && findLoopEnd)*/


						/*unsigned long trueStart = 0;
						for (int r = x; ((r >= 4) && (r >= (x - 0x1800))); r-=4)
						{
							if (
								(CharArrayToLong(&binary[r]) == 0x00000000))
							{
								trueStart = r - 0x14;
								break;
							}
						}

						unsigned long instrumentOffset = CharArrayToLong(&binary[trueStart + 0x14]);
						if ((instrumentOffset != 0) && (instrumentOffset < 0x50000) && (instrumentOffset > 0x50))
						{
							unsigned long end = trueStart + instrumentOffset;

							while (
								(((CharArrayToShort(&binary[end]) != 0x0000)
								|| (CharArrayToShort(&binary[end+2]) != 0x0000)))
								&&
								(CharArrayToShort(&binary[end]) <  0x0400)
								)
							{
								end += 2;
							}*/

							if (!wroteNameHeader)
							{
								CString tempFileName = fileNames[y];
								tempFileName.Replace(" [!]", "");
								tempFileName.Replace(".z64", "");
								tempFileName.Replace(".v64", "");
								tempFileName.Replace(".rom", "");
								wroteNameHeader = true;
								fprintf(outFile, "[%s Speech]:type=MORT\n", tempFileName);
								fprintf(outFileAlt, "[%s Speech]:type=MORT\n", tempFileName);
							}
							fprintf(outFile, "%08X,%08X,%d", x, x + (CharArrayToLong(&binary[x + 8]) << 2), CharArrayToShort(&binary[x + 2]));
							fprintf(outFileAlt, "%08X,%08X,%d", x, x + (CharArrayToLong(&binary[x + 8]) << 2), (CharArrayToShort(&binary[x + 2]) / 2));
							//fprintf(outFile, "%08X,%08X",trueStart,end);
							//for (int r = 0; r < 0x8; r++)
								//fprintf(outFile, " %08X", CharArrayToLong(&binary[x + (4 * r)]));
							fprintf(outFile, "\n");
							fprintf(outFileAlt, "\n");

							/*x += instrumentOffset;
						}*/
					}

					/*if (!wroteNameHeader)
					{
						wroteNameHeader = true;
						fprintf(outFile, "[%s]\n", fileNames[y]);
					}
					if ((CharArrayToLong(&binary[x]) == 0x4E363420) && (CharArrayToLong(&binary[x+4]) == 0x57617665)) // n64 wave
					{
						//fprintf(outFile, "%08X\n", x);
						//continue;
						if (prev == "ctl")
							fprintf(outFile, "%08X\n", x);
						else if (prev == "tbl")
							fprintf(outFile, "DTBL%08X\n", x);
						else
							fprintf(outFile, "FIRSTTBL\n%08X\n", x);
						prev = "tbl";
					}
					else if ((CharArrayToLong(&binary[x]) == 0x4E363420) && (CharArrayToLong(&binary[x+4]) == 0x50747254)) // n64 Ptr
					{
						//fprintf(outFile, "%08X,\n", x);
						//continue;
 						if (prev == "ctl")
							fprintf(outFile, "DCTL%08X,\n", x);
						else if (prev == "tbl")
							fprintf(outFile, "%08X,", x);
						else
							fprintf(outFile, "%08X,", x);
						prev = "ctl";
					}*/
					/*if ((CharArrayToLong(&binary[x]) & 0xFFFF0000) == 0x53310000) // S1 Midi Format
					{
						unsigned short numberMidis = CharArrayToShort(&binary[x+2]);
						unsigned long firstOffset = x + CharArrayToLong(&binary[x+4]);
						if (firstOffset < (results.st_size - 0x44))
						{
							if (CharArrayToLong(&binary[firstOffset]) == 0x00000044)
							{
								unsigned long lastOffset = x + CharArrayToLong(&binary[x+(8 * (numberMidis-1))]);
								unsigned long lastSize = CharArrayToLong(&binary[x+(8 * (numberMidis-1)) + 4]);
								fprintf(outFile, "[%s]\n", fileNames[y]);
								fprintf(outFile, "%08X,%08X\n", firstOffset, (lastOffset+lastSize));
								fflush(outFile);
							}
						}
					}*/
				}
			}

			delete [] binary;
			

			fclose(inFile);
		}

		fclose(outFile);
		fclose(outFileAlt);

		/*FILE* rereadFile = fopen(m_svFile.GetPathName(), "r");

		std::vector<CString> lines;
		while (!feof(rereadFile))
		{
			char currentLine[1000];
			fgets(currentLine, 1000, rereadFile);
			lines.push_back(currentLine);
		}
		fclose(rereadFile);

		FILE* outRe = fopen(m_svFile.GetPathName(), "wb");
		for (int x = 0; x < lines.size(); x++)
		{
			lines[x].Replace("\r\n", "");
			lines[x].Replace("\r", "");
			lines[x].Replace("\n", "");

			if (lines[x].Find(",") != -1)
			{
				if (x < (lines.size() - 1))
				{
					CString nextLine = lines[x+1];
					nextLine.Replace("\r\n", "");
					nextLine.Replace("\r", "");
					nextLine.Replace("\n", "");
					if (nextLine.Find(",") != -1)
					{
						nextLine.Replace(",", "");
						fprintf(outRe, "%s%s\n", lines[x], nextLine);
					}
					else
					{
						fprintf(outRe, "%s\n", lines[x]);
					}
				}
				else
				{
					fprintf(outRe, "%s\n", lines[x]);
				}
			}
			else
			{
				fprintf(outRe, "%s:type=Sng\n", lines[x]);
			}
		}
		fclose(outRe);*/
	}
}

void CN64MidiToolDlg::OnBnClickedButtonriproms()
{
	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	

	for (int y = 0x0; y < countGames; y++)
	{
		CString gameName = (tempPath + gameConfig[y].gameName + " [!].z64");

		//if (gameConfig[y].gameType.Find("Konami") == -1)
			//continue;

		//if (gameConfig[y].gameType.Find("Sng") == -1)
			//continue;

		CString romName = gameName;
		romName.Replace(" (Binary FX)", "");
		romName.Replace(" RNC", "");
		romName.Replace(" AVL_0", "");
		
		int gameROMSize = GetSizeFile(romName);

		if (gameROMSize <= 0)
			continue;

		CString outputFolder = (tempPath + gameConfig[y].gameName + "\\");
		
		::CreateDirectory(outputFolder, NULL);

		try
		{
			
			FILE* inROM = fopen(romName, "rb");

			if (inROM == NULL)
			{
				continue;
			}
			
			unsigned char* gamebuffer = new unsigned char[gameROMSize];
			fread(gamebuffer, 1, gameROMSize, inROM);
			fclose(inROM);

			bool byteFlippedROM = false;
			if (gamebuffer[0] == 0x37)
				byteFlippedROM = true;
			else if (gamebuffer[0] == 0x80)
				byteFlippedROM = false;
			else
			{
				//MessageBox("Not a valid ROM", "Error");
				//return;
			}

			if (byteFlippedROM == true)
			{
				for (int x = 0; x < gameROMSize; x=x+2)
				{
					unsigned char tempSpot = gamebuffer[x];
					gamebuffer[x] = gamebuffer[x+1];
					gamebuffer[x+1] = tempSpot;
				}
			}

			std::vector<CString> addMidiStrings;
			int numberMidiStrings = 0;
			int numberInstruments = 0;
			CN64MidiToolReader::ProcessMidis(gameConfig, y, addMidiStrings, numberMidiStrings, numberInstruments, compressed, gamebuffer, gameROMSize, startSpot, endSpot, true, mSeparateByInstrument.GetCheck(), mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[y].extraGameMidiInfo);

			for (int x = 0; x < numberMidiStrings; x++)
			{
				unsigned long address = 0;
				unsigned long size = 0;
				unsigned long extra = 0;
				unsigned long extra2 = 0;

				int originalSize = addMidiStrings[x].GetLength();
				CString tempColonReplacedStr = addMidiStrings[x];
				int countColons = tempColonReplacedStr.Replace(":", "");

				if (countColons == 3)
					sscanf(addMidiStrings[x], "%08X:%08X:%08X:%08X", &address, &size, &extra, &extra2);
				else if (countColons == 2)
					sscanf(addMidiStrings[x], "%08X:%08X:%08X", &address, &size, &extra);
				else
					sscanf(addMidiStrings[x], "%08X:%08X", &address, &size);

				CString tempOutFileNameStr;
				tempOutFileNameStr.Format("%s%02X_%08X_%08X.mid",outputFolder,x,address,size);
				int numberInstruments;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				CN64MidiToolReader::midiParse.ExportToMidi(gameConfig[y].gameName, gamebuffer, gameROMSize, address, size, tempOutFileNameStr, gameConfig[y].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebugTextFile.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfig[y].extraGameMidiInfo);
			}
			

			delete [] gamebuffer;

		}
		catch (char * str)
		{
			CString tempErrStr;
			tempErrStr.Format("%s", str);
			MessageBox(tempErrStr, "Error processing ");
		}	
	}
}

void CN64MidiToolDlg::OnBnClickedLoopDebug()
{
	CFileDialog m_ldFile(TRUE, NULL, "GEMidi.bin", OFN_HIDEREADONLY, "Bin (*.bin)|*.bin|", this);

	int isFileOpened = m_ldFile.DoModal();

	if (isFileOpened == IDCANCEL)
		return;

	CFileDialog m_svFile(FALSE, "bin", "Midi.bin", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Bin File (*.bin)|*.bin|", this);

	int statusFileOpen = (int) m_svFile.DoModal();

	if ((statusFileOpen == IDCANCEL) || (m_svFile.GetFileName() == ""))
		return;

	if (statusFileOpen == FALSE)
		return;

	CString fileName;
	if (statusFileOpen == IDOK)
	{
		fileName = m_svFile.GetPathName();
	}
	else
	{
		return;
	}

	struct stat results;
	stat(m_ldFile.GetPathName(), &results);		

	FILE* inFile1 = fopen(m_ldFile.GetPathName(), "rb");
	if (inFile1 == NULL)
	{
		MessageBox("Error reading file", "Error");
		return;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	CN64MidiToolReader::midiParse.AddLoopGEFormat(inputMID, fileName, results.st_size, true, 0, true);

	delete [] inputMID;
}

void CN64MidiToolDlg::OnBnClickedButtongenmidis()
{
	/*CN64MidiToolReader::midiParse.GenerateTestPattern(0, "\\\\mitchellnas\\Goldeneye\\TempShare\\a.mid");
	CN64MidiToolReader::midiParse.GenerateTestPattern(1, "\\\\mitchellnas\\Goldeneye\\TempShare\\b.mid");
	CN64MidiToolReader::midiParse.GenerateTestPattern(2, "\\\\mitchellnas\\Goldeneye\\TempShare\\c.mid");
	CN64MidiToolReader::midiParse.GenerateTestPattern(3, "\\\\mitchellnas\\Goldeneye\\TempShare\\d.mid");*/

	CN64MidiToolReader::midiParse.GenerateTestPattern(0, "C:\\temp\\a.mid");
	CN64MidiToolReader::midiParse.GenerateTestPattern(1, "C:\\temp\\b.mid");
	CN64MidiToolReader::midiParse.GenerateTestPattern(2, "C:\\temp\\c.mid");
	CN64MidiToolReader::midiParse.GenerateTestPattern(3, "C:\\temp\\d.mid");
}

void CN64MidiToolDlg::OnBnClickedCheckshowbfx()
{
	m_game.ResetContent();
	bool showEffects = mShowEffectsGames.GetCheck();
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.Find(" (Binary FX)") != -1)
		{
			if (!showEffects)
				continue;
		}

		m_game.AddString(gameConfig[x].gameName);
	}

	m_game.SetCurSel(0);
}

int CN64MidiToolDlg::CEditControlToDecimalValue(CEdit* editControl)
{
	CString tempStr;
	editControl->GetWindowText(tempStr);

	return atoi(tempStr);
}
void CN64MidiToolDlg::OnBnClickedCheckextendloopstohighesttrack()
{
	if (mExtendSmallerTracksToEnd.GetCheck())
		mOutputLoop.EnableWindow(false);
	else
		mOutputLoop.EnableWindow(true);
}

void CN64MidiToolDlg::OnBnClickedCheckoutputloop()
{
	if (mOutputLoop.GetCheck())
		mExtendSmallerTracksToEnd.EnableWindow(false);
	else
		mExtendSmallerTracksToEnd.EnableWindow(true);
}