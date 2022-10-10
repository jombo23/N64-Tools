
// N64SoundbankToolDlg.cpp : implementation file
//

#include "stdafx.h"

#include "DLSFile.h"

#include "N64SoundbankTool.h"
#include "N64SoundbankToolDlg.h"
#include <vector>
#include <algorithm>
#include "FolderDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CN64SoundbankToolDlg dialog




CN64SoundbankToolDlg::CN64SoundbankToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CN64SoundbankToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CN64SoundbankToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOGAME, m_game);
	DDX_Control(pDX, IDC_COMBOMIDI, mMidi);
	DDX_Control(pDX, IDC_EDITTIMEMULTIPLIER, mTimeMultiplier);
	DDX_Control(pDX, IDC_COMBOSOUND, mSoundBankNumber);
	DDX_Control(pDX, IDC_CHECKHALFSAMPLINGRATE, mHalfSamplingRate);
	DDX_Control(pDX, IDC_CHECKLOOP, mLoopPlayback);
	DDX_Control(pDX, IDC_BUTTONPREVIEWMIDI, mPreviewButton);
	DDX_Control(pDX, IDC_BUTTONPREVIEWMIDI2, mStopButton);
	DDX_Control(pDX, IDC_EDITSAMPLINGRATE, mSamplingRate);
	DDX_Control(pDX, IDC_LABELGAMEINFO, mGameDescription);
	DDX_Control(pDX, IDC_CHECKSEPARATEBYISNTRUMENT, mSeparateByInstrument);
	DDX_Control(pDX, IDC_EDITOUTPUTLOOPCOUNT, mOutputLoopCount);
	DDX_Control(pDX, IDC_CHECKOUTPUTLOOP, mOutputLoop);
	DDX_Control(pDX, IDC_BUTTONWRITEDLSSOUNDFONT3, mWriteDLSCombinedBanksButton);
	DDX_Control(pDX, IDC_CHECKMERGEPREVIEWBANKS, mMergePreviewBanksCheck);
	DDX_Control(pDX, IDC_CHECKEXTENDLOOPSTOHIGHESTTRACK, mExtendSmallerTracksToEnd);
	DDX_Control(pDX, IDC_CHECKUSET1, mUseT1SamplingRate);
	DDX_Control(pDX, IDC_CHECKDEBUG, mDebug);
	DDX_Control(pDX, IDC_COMBOSONGID, mComboSongId);
}

BEGIN_MESSAGE_MAP(CN64SoundbankToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBOGAME, &CN64SoundbankToolDlg::OnCbnSelchangeCombogame)
	ON_BN_CLICKED(IDC_BUTTONLOADROM, &CN64SoundbankToolDlg::OnBnClickedButtonloadrom)
	ON_BN_CLICKED(IDC_BUTTONWRITEDLSSOUNDFONT, &CN64SoundbankToolDlg::OnBnClickedButtonwritedlssoundfont)
	ON_BN_CLICKED(IDC_BUTTONPREVIEWMIDI, &CN64SoundbankToolDlg::OnBnClickedButtonpreviewmidi)
	ON_BN_CLICKED(IDC_BUTTONPREVIEWMIDI2, &CN64SoundbankToolDlg::OnBnClickedButtonpreviewmidi2)
	ON_CBN_SELCHANGE(IDC_COMBOSOUND, &CN64SoundbankToolDlg::OnCbnSelchangeCombosound)
	ON_BN_CLICKED(IDC_BUTTONWRITEDLSSOUNDFONT2, &CN64SoundbankToolDlg::OnBnClickedButtonwritedlssoundfont2)
	ON_BN_CLICKED(IDC_BUTTONWRITEMIDISTOFOLDER, &CN64SoundbankToolDlg::OnBnClickedButtonwritemidistofolder)
	ON_BN_CLICKED(IDC_BUTTONEXPORTMIDI, &CN64SoundbankToolDlg::OnBnClickedButtonexportmidi)
	ON_BN_CLICKED(IDC_BUTTONWRITEDLSSOUNDFONT3, &CN64SoundbankToolDlg::OnBnClickedButtonwritedlssoundfont3)
	ON_BN_CLICKED(IDC_CHECKEXTENDLOOPSTOHIGHESTTRACK, &CN64SoundbankToolDlg::OnBnClickedCheckextendloopstohighesttrack)
	ON_BN_CLICKED(IDC_CHECKOUTPUTLOOP, &CN64SoundbankToolDlg::OnBnClickedCheckoutputloop)
END_MESSAGE_MAP()

// CN64SoundbankToolDlg message handlers

BOOL CN64SoundbankToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowText("N64 Soundbank Tool by SubDrag	" __DATE__);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	mSeparateByInstrument.SetCheck(true);
	mTimeMultiplier.SetWindowText("0.000001");
	mOutputLoop.SetCheck(false);
	mOutputLoopCount.SetWindowText("1");
	mUseT1SamplingRate.SetCheck(true);

	compressed = false;
	romSize = 0;
	buffer = NULL;

	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	mainFolder.Format("%s\\", tempFolder);

	DWORD portCount = 0;
	MIDIINFO midiInfo;

	midiPlayer = new CMidiPlayer();
	HRESULT hr = midiPlayer->Startup(mainFolder, this->m_hWnd);
	
	midiInfos.clear();

	bool setDefault = false;

	if (hr == S_OK)
	{ 
		while (midiPlayer->GetNextPort(portCount, midiInfo) == S_OK)
		{
			if (midiInfo.dwClass == DMUS_PC_OUTPUTCLASS)
			{
				//if (midiInfo.dwFlags & DMUS_PC_DLS2)
				//if (midiInfo.dwFlags & DMUS_PC_DLS)
				{
					if (midiInfo.dwFlags & DMUS_PC_SOFTWARESYNTH)
					{
						midiInfos.push_back(midiInfo);
						if (!setDefault)
						{
							HRESULT hr = midiPlayer->ChoosePort(midiInfo);
							if (hr == S_OK)
							{
								setDefault = true;
							}
						}
					}
				}
			}

			portCount++;
		}
	}

	InitializeSpecificGames();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CN64SoundbankToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CN64SoundbankToolDlg::OnPaint()
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
HCURSOR CN64SoundbankToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CN64SoundbankToolDlg::FilterGameName(CString gameName)
{
	gameName.Replace(" - With Copying IPL ROM from 0x140000 to 0x3F4000 to this", "");
	gameName = gameName.Trim();

	return gameName;
}

void CN64SoundbankToolDlg::InitializeSpecificGames()
{
	char folder[8000];
	strcpy(folder, mainFolder);
	strcat(folder, "gameconfig.ini");

	FILE* inIni = fopen(folder, "r");
	if (inIni == NULL)
	{
		MessageBox("Error reading gameconfig.ini, file must exist", "Error");
		OnCancel();
	}

	CString lastGame = "";

	countGames = 0;

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

		if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", &currentLine[1]);
		}

		if (lastGameNew != lastGame)
		{
			countGames++;
			lastGame = lastGameNew;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	rewind(inIni);

	gameConfig = new GameConfig[countGames];
	countGames = 0;
	lastGame = "";
	
	int soundBankCount = 0;


	CString gameType = "Uncompressed";
	while (!feof(inIni))
	{
		
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;
		
		CString curLineStr;
		curLineStr.Format("%s", currentLine);

		if (currentLine[0] == '/')
			continue;
		else if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", curLineStr.Mid(1, (spot - 1)));

			gameType = "Uncompressed";
			if (curLineStr.Find(":type=") != -1)
			{
				gameType = curLineStr.Mid((curLineStr.Find(":type=") + 6), (curLineStr.GetLength() - curLineStr.Find(":type=") - 6));
			}
		}
		else if (currentLine[0] == '{')
		{

		}
		else
		{
			soundBankCount++;
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				countGames++;
			}
			else
			{
				gameConfig[countGames].gameName = lastGameNew;
			}

			soundBankCount = 0;
			lastGame = lastGameNew;
			gameConfig[countGames].gameName = lastGameNew;
			gameConfig[countGames].gameType = gameType;
		}
	}

	if (lastGame != "")
	{
		gameConfig[countGames].gameType = gameType;
		countGames++;
	}


	rewind(inIni);

	countGames = 0;
	lastGame = "";

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

		if (currentLine[0] == '/')
			continue;
		else if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", &currentLine[1]);
		}
		else if (currentLine[0] == '{')
		{
			CString line;
			line.Format("%s", currentLine);

			line.Replace("{", "");
			line.Replace("}", "");
			line.Replace(" ", "");

			if ((line.Find("HalfSampling")) != -1)
			{
				gameConfig[countGames].halfSamplingRate = true;
			}
			else if (line.Find("TimeMultiplier") != -1)
			{
				line.Replace("TimeMultiplier", "");
				line.Replace(":", "");
				gameConfig[countGames].timeMultiplier = atof(line);
			}
			else if (line.Find("SamplingRate") != -1)
			{
				line.Replace("SamplingRate", "");
				line.Replace(":", "");
				gameConfig[countGames].overrideSamplingRate = true;
				gameConfig[countGames].samplingRate = atoi(line);
			}
			else if (line.Find("SkipInstrument") != -1)
			{
				line.Replace("SkipInstrument", "");
				line.Replace(":", "");
				gameConfig[countGames].skipInstruments.push_back(atoi(line));
			}
			else if (line.Find("PercussionOffset") != -1)
			{
				line.Replace("PercussionOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].percussionOffset = CN64SoundToolReader::StringHexToLong(line);
			}
			else if (line.Find("ProjOffset") != -1)
			{
				line.Replace("ProjOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].projOffset = CN64SoundToolReader::StringHexToLong(line);
			}
			else if (line.Find("AbsoluteOffsets") != -1)
			{
				line.Replace("AbsoluteOffsets", "");
				line.Replace(":", "");
				gameConfig[countGames].absoluteOffsets = true;
			}
			else if (line.Find("PoolOffset") != -1)
			{
				line.Replace("PoolOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].poolOffset = CN64SoundToolReader::StringHexToLong(line);
			}
			else if (line.Find("ProjSize") != -1)
			{
				line.Replace("ProjSize", "");
				line.Replace(":", "");
				gameConfig[countGames].projSize = CN64SoundToolReader::StringHexToLong(line);
			}
			else if (line.Find("PoolSize") != -1)
			{
				line.Replace("PoolSize", "");
				line.Replace(":", "");
				gameConfig[countGames].poolSize = CN64SoundToolReader::StringHexToLong(line);
			}
			else if (line.Find("SDirOffset") != -1)
			{
				line.Replace("SDirOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].sDirOffset = CN64SoundToolReader::StringHexToLong(line);
			}
			else if (line.Find("SDirSize") != -1)
			{
				line.Replace("SDirSize", "");
				line.Replace(":", "");
				gameConfig[countGames].sDirSize = CN64SoundToolReader::StringHexToLong(line);
			}
		}
		else
		{
			CString line;
			line.Format("%s", currentLine);
			line.Replace("\r", "");
			line.Replace("\n", "");
			line.Replace("\r\n", "");
			line.Replace(" ", "");

			SoundfontBank soundfontBank;

			if (line.Find(":") != -1)
			{
				soundfontBank.soundBankNumber = CN64SoundToolReader::StringHexToLong(line.Mid(0, line.Find(":")));

				CString midis = line.Mid(line.Find(":")+1);
				CString sToken=_T("");
				int i = 0;
				while (AfxExtractSubString(sToken, midis, i,','))
				{   
					if (sToken != "")
					{
						soundfontBank.midiNumbers.push_back(CN64SoundToolReader::StringHexToLong(sToken));
					}

				   i++;
				}
			}
			else
			{
				soundfontBank.soundBankNumber = CN64SoundToolReader::StringHexToLong(line);
			}
			gameConfig[countGames].soundfontBank.push_back(soundfontBank);
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				countGames++;
			}

			lastGame = lastGameNew;

			soundBankCount = 0;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	fclose(inIni);

	countGamesMidi = 0;
	gameConfigMidi = NULL;

	char folderMidi[8000];
	strcpy(folderMidi, mainFolder);
	strcat(folderMidi, "gameconfigmidi.ini");

	CN64MidiToolReader::InitializeSpecificGames(folderMidi, countGamesMidi, gameConfigMidi);

	countGamesSound = 0;
	gameConfigSound = NULL;

	char folderSound[8000];
	strcpy(folderSound, mainFolder);
	strcat(folderSound, "gameconfigsound.ini");

	CN64SoundToolReader::InitializeSpecificGames(folderSound, countGamesSound, gameConfigSound);

	m_game.ResetContent();
	for (int x = 0; x < countGames; x++)
	{
		bool foundSound = false;
		for (int y = 0; y < countGamesSound; y++)
		{
			if (FilterGameName(gameConfigSound[y].gameName) == FilterGameName(gameConfig[x].gameName))
			{
				foundSound = true;
				break;
			}
		}

		bool foundMidi = false;
		for (int y = 0; y < countGamesMidi; y++)
		{
			if (FilterGameName(gameConfigMidi[y].gameName) == FilterGameName(gameConfig[x].gameName))
			{
				foundMidi = true;
				break;
			}
		}

		if (foundSound)
			m_game.AddString(FilterGameName(gameConfig[x].gameName));
	}

	m_game.SetCurSel(0);
	
	t1BankList.clear();
	alBankList.clear();

	OnCbnSelchangeCombogame();
}

CN64SoundbankToolDlg::~CN64SoundbankToolDlg()
{
	for (int x = 0; x < countGamesSound; x++)
	{
		delete [] gameConfigSound[x].soundBanks;
	}
	delete [] gameConfigSound;

	for (int x = 0; x < countGamesMidi; x++)
	{
		if (gameConfigMidi[x].midiNames != NULL)
			delete [] gameConfigMidi[x].midiNames;

		if (gameConfigMidi[x].midiBanks != NULL)
			delete [] gameConfigMidi[x].midiBanks;
	}
	delete [] gameConfigMidi;

	delete [] gameConfig;

	for (int x = 0; x < alBankList.size(); x++)
	{
		n64audio.DisposeALBank(alBankList[x]);
	}
	alBankList.clear();

	for (int x = 0; x < t1BankList.size(); x++)
	{
		n64audio.DisposeT1Bank(t1BankList[x].bank);
	}
	t1BankList.clear();

	delete midiPlayer;

	if (buffer)
	{
		delete [] buffer;
		buffer = NULL;
	}
}

void CN64SoundbankToolDlg::OnClose()
{

	CDialog::OnClose();
}

void CN64SoundbankToolDlg::OnCbnSelchangeCombogame()
{
	
}

int CN64SoundbankToolDlg::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void CN64SoundbankToolDlg::OnBnClickedButtonloadrom()
{
	if (m_game.GetCurSel() == -1)
		return;

	mSoundBankNumber.ResetContent();

	CString gameName;
	m_game.GetWindowText(gameName);

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	for (int x = 0; x < alBankList.size(); x++)
	{
		n64audio.DisposeALBank(alBankList[x]);
	}
	alBankList.clear();

	for (int x = 0; x < t1BankList.size(); x++)
	{
		n64audio.DisposeT1Bank(t1BankList[x].bank);
	}
	t1BankList.clear();

	SoundGameConfig gameConfigSoundFound;
	for (int y = 0; y < countGamesSound; y++)
	{
		if (FilterGameName(gameConfigSound[y].gameName) == FilterGameName(gameConfigBank.gameName))
		{
			gameConfigSoundFound = gameConfigSound[y];
			break;
		}
	}

	CString tempStr;
	m_game.GetWindowText(tempStr);
	tempStr.Trim();

	CFileDialog m_ldFile(TRUE, NULL, tempStr + " [!].z64", OFN_HIDEREADONLY, "GE ROM(*.v64;*.z64;*.rom;*.n64)|*.v64;*.z64;*.rom;*.n64|", this);

	int statusFileOpen = (int) m_ldFile.DoModal();

	CString romName = m_ldFile.GetPathName();

	//int statusFileOpen = IDOK;
	//romName = "C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\GoldenEye 007 (U) [!].z64";
	
	

	if ((statusFileOpen == IDCANCEL) || (romName == ""))
		return;

	if (statusFileOpen == FALSE)
		return;	


	romSize = GetSizeFile(romName);

	if (romSize == 0)
	{
		MessageBox("Invalid ROM");
		return;
	}
	else if (statusFileOpen == IDOK)
	{
		m_game.GetWindowText(gameName);
		gameName.Trim();


		FILE* inROM = fopen(romName, "rb");

		if (inROM == NULL)
		{
			MessageBox("Error opening ROM");
			return;
		}
		
		if (buffer)
		{
			delete [] buffer;
			buffer = NULL;
		}

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

		mHalfSamplingRate.SetCheck(gameConfigBank.halfSamplingRate);
	
		tempStr.Format("%f", gameConfigBank.timeMultiplier);
		mTimeMultiplier.SetWindowText(tempStr);

		int numberResults;
		std::vector<ctlTblResult> results;
		std::vector<t1Result> t1results;

		CN64SoundToolReader::ReadSoundbanks(buffer, romSize, gameConfigSoundFound, numberResults, results, t1results);

		t1BankList = t1results;

		for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
		{
			if (gameConfigBank.soundfontBank[x].soundBankNumber >= numberResults)
			{
				mComboSongId.ShowWindow(SW_HIDE);
				MessageBox("Invalid sound bank # from ini for game or error loading");
				delete [] buffer;
				buffer = NULL;
				romSize = 0;
				return;
			}
		}

		if (gameConfigBank.soundfontBank.size() > 0)
		{
			for (int x = 0; x < numberResults; x++)
			{
				alBankList.push_back(results[x].bank);

				for (int y = 0; y < gameConfigBank.soundfontBank.size(); y++)
				{
					if (x == gameConfigBank.soundfontBank[y].soundBankNumber)
					{
						tempStr.Format("%02X", gameConfigBank.soundfontBank[y]);

						tempStr += " " + results[x].bank->bankName;

						mSoundBankNumber.AddString(tempStr);

						break;
					}
				}
			}

			MidiGameConfig gameConfigMidiFound;
			for (int y = 0; y < countGamesMidi; y++)
			{
				if (FilterGameName(gameConfigMidi[y].gameName).CompareNoCase(gameName) == 0)
				{
					gameConfigMidiFound = gameConfigMidi[y];
					break;
				}
			}

			if (
				(gameConfigMidiFound.gameType.CompareNoCase("Factor5Zlb") == 0)
					|| (gameConfigMidiFound.gameType.CompareNoCase("Factor5ZlbGCStyle") == 0)
					|| (gameConfigMidiFound.gameType.CompareNoCase("Factor5LZGCStyle") == 0)
					|| (gameConfigMidiFound.gameType.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0)
					)
			{
				bool absoluteOffsets = gameConfigBank.absoluteOffsets;
				mComboSongId.ResetContent();

				int projOffset = gameConfigBank.projOffset;
				int poolOffset = gameConfigBank.poolOffset;
				int projSize = gameConfigBank.projSize;
				int poolSize = gameConfigBank.poolSize;

				unsigned char* proj = NULL;
				unsigned char* pool = NULL;

				bool isCompressed = false;
				int zlibGame = 0;
				if (buffer[poolOffset] == 0x78)
				{
					isCompressed = true;
					zlibGame = STUNTRACER64;
				}
				else if (gameConfigSoundFound.gameType == "Musyx")
				{
					isCompressed = false;
				}
				else if (gameConfigSoundFound.gameType == "ZLibMusyx")
				{
					isCompressed = true;
					zlibGame = GAUNTLETLEGENDS;
				}
				else if (gameConfigSoundFound.gameType == "ZLib78DAMusyx")
				{
					isCompressed = true;
					zlibGame = STUNTRACER64;
				}
				else if (gameConfigSoundFound.gameType == "MusyxREZLib")
				{
					isCompressed = true;
					zlibGame = STUNTRACER64;
				}
				else if (gameConfigSoundFound.gameType == "MusyxSmallZlib")
				{
					isCompressed = true;
					zlibGame = STUNTRACER64;
				}
				else if (gameConfigSoundFound.gameType == "LzMusyx")
				{
					isCompressed = true;
				}
				if (!isCompressed)
				{
					proj = new unsigned char[projSize];
					memcpy(proj, &buffer[projOffset], projSize);
					pool = new unsigned char[poolSize];
					memcpy(pool, &buffer[poolOffset], poolSize);
				}
				else
				{
					projSize = 0;
					poolSize = 0;

					if (gameConfigSoundFound.gameType == "LzMusyx")
					{
						proj = new unsigned char[0x100000];
						pool = new unsigned char[0x100000];
						MidwayLZ midwayLz;
						projSize = midwayLz.dec(&buffer[projOffset], romSize - projOffset, proj);
						poolSize = midwayLz.dec(&buffer[poolOffset], romSize - poolOffset, pool);
						/*FILE* projDebugFile = fopen("C:\\temp\\proj.bin", "wb");
						if (projDebugFile)
						{
							fwrite(proj, 1, projSize, projDebugFile);
							fflush(projDebugFile);
							fclose(projDebugFile);
						}
						FILE* poolDebugFile = fopen("C:\\temp\\pool.bin", "wb");
						if (poolDebugFile)
						{
							fwrite(pool, 1, poolSize, poolDebugFile);
							fflush(poolDebugFile);
							fclose(poolDebugFile);
						}*/
					}
					else
					{
						GECompression compression;
						compression.SetPath(mainFolder);
						if ((buffer[poolOffset] == 0x78) && (buffer[poolOffset+1] == 0xDA))
						{
							compression.SetGame(STUNTRACER64);
						}
						else
						{
							compression.SetGame(zlibGame);
						}

						compression.SetCompressedBuffer(&buffer[projOffset], romSize - projOffset);
						projSize = 0;
						int projCompressedSize = 0;
						proj = compression.OutputDecompressedBuffer(projSize, projCompressedSize);

						compression.SetCompressedBuffer(&buffer[poolOffset], romSize - poolOffset);
						poolSize = 0;
						int poolCompressedSize = 0;
						pool = compression.OutputDecompressedBuffer(poolSize, poolCompressedSize);
					}
				}
				

				std::vector<int> songIds;

				int groupOffset = 0;
				int groupCounter = 0;

				while (groupOffset < projSize)
				{
					unsigned int groupEndOff = CharArrayToLong(&proj[groupOffset + 0]);

					if (groupEndOff == 0xFFFFFFFF)
						break;

					unsigned short groupId = CharArrayToShort(&proj[groupOffset + 4]);
					int type = CharArrayToShort(&proj[groupOffset + 6]);
					unsigned int soundMacroIdsOff = CharArrayToLong(&proj[groupOffset + 8]);
					unsigned int samplIdsOff = CharArrayToLong(&proj[groupOffset + 0xC]);
					unsigned int tableIdsOff = CharArrayToLong(&proj[groupOffset + 0x10]);
					unsigned int keymapIdsOff = CharArrayToLong(&proj[groupOffset + 0x14]);
					unsigned int layerIdsOff = CharArrayToLong(&proj[groupOffset + 0x18]);
					unsigned int pageTableOff = CharArrayToLong(&proj[groupOffset + 0x1C]);
					unsigned int sfxTableOff = CharArrayToLong(&proj[groupOffset + 0x1C]);
					unsigned int drumTableOff = CharArrayToLong(&proj[groupOffset + 0x20]);
					unsigned int midiSetupsOff = CharArrayToLong(&proj[groupOffset + 0x24]);

					unsigned long subDataOff = absoluteOffsets ? groupOffset : groupOffset + 8;

					CString typeStr;
					if (type == 0)
					{
						typeStr = "SONG";
						//fprintf(outProj, "#%02X %s Group Offset: %08X Group End Offset: %08X - Group Id %04X Type %04X Sound Macro Ids Offset %08X Samples Id Offset %08X Table Ids Offset %08X Key Map Offset %08X Layer Ids %08X Page Table %08X Drum Page Table %08X Midi Setups %08X\n", groupCounter, typeStr, subDataOff, subDataOff + groupEndOff, groupId, type, subDataOff + soundMacroIdsOff, subDataOff + samplIdsOff, subDataOff + tableIdsOff, subDataOff + keymapIdsOff, subDataOff + layerIdsOff, subDataOff + pageTableOff, subDataOff + drumTableOff, subDataOff + midiSetupsOff);
					}
					else
					{
						typeStr = "SFX ";
						//fprintf(outProj, "#%02X %s Group Offset: %08X Group End Offset: %08X - Group Id %04X Type %04X Sound Macro Ids Offset %08X Samples Id Offset %08X Table Ids Offset %08X Key Map Offset %08X Layer Ids %08X SFX Table Offset %08X \n", groupCounter, typeStr, subDataOff, subDataOff + groupEndOff, groupId, type, subDataOff + soundMacroIdsOff, subDataOff + samplIdsOff, subDataOff + tableIdsOff, subDataOff + keymapIdsOff, subDataOff + layerIdsOff, subDataOff + pageTableOff);
					}
					
					if (type == 0) // Song
					{
						unsigned long tempMidiSetupData = midiSetupsOff;
						if (!absoluteOffsets)
							tempMidiSetupData += subDataOff;

						if (absoluteOffsets)
						{
							int songId = CharArrayToShort(&proj[tempMidiSetupData]);
							if (std::find(songIds.begin(), songIds.end(), songId) == songIds.end())
								songIds.push_back(songId);
						}
						else
						{
							//fprintf(outProj, "\nSong Info\n");
							while (CharArrayToLong(&proj[tempMidiSetupData]) != 0xFFFFFFFF)
							{
								int songId = CharArrayToShort(&proj[tempMidiSetupData]);
								if (std::find(songIds.begin(), songIds.end(), songId) == songIds.end())
									songIds.push_back(songId);

								tempMidiSetupData += 8 * 16 + 4;
							}
						}
					}

					
					if (absoluteOffsets)
						groupOffset = groupEndOff;
					else
						groupOffset += groupEndOff;
					groupCounter++;
				}

				for (int x = 0; x < songIds.size(); x++)
				{
					CString tempSongIdStr;
					tempSongIdStr.Format("%02X", songIds[x]);
					mComboSongId.AddString(tempSongIdStr);
				}

				delete [] pool;
				delete [] proj;

				mComboSongId.SetCurSel(0);
				mComboSongId.ShowWindow(SW_SHOW);
			}
			else
			{
				mComboSongId.ShowWindow(SW_HIDE);
			}

			mSoundBankNumber.SetCurSel(0);
		}
		else
		{
			mComboSongId.ShowWindow(SW_HIDE);
			MessageBox("Invalid sound bank # from ini for game or error loading");
			delete [] buffer;
			buffer = NULL;
			romSize = 0;
			return;
		}

		OnCbnSelchangeCombosound();
	}
}

std::string CN64SoundbankToolDlg::CStringToString(CString cstring)
{
	std::string newStr = cstring.GetBuffer(0);
	cstring.ReleaseBuffer();
	return newStr;
}

void CN64SoundbankToolDlg::fprintfIfDebug(FILE* outFileDebug, char* format,...) 
{
	va_list args;
    va_start( args, format );
	if (outFileDebug != NULL)
		vfprintf(outFileDebug, format, args );
    va_end( args );
}

bool CN64SoundbankToolDlg::WriteDLS(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, bool combineBanks, FILE* outFileDebug)
{
	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	SoundGameConfig gameConfigSoundFound;
	for (int y = 0; y < countGamesSound; y++)
	{
		if (FilterGameName(gameConfigSound[y].gameName) == FilterGameName(gameConfigBank.gameName))
		{
			gameConfigSoundFound = gameConfigSound[y];
			break;
		}
	}

	MidiGameConfig gameConfigMidiFound;
	for (int y = 0; y < countGamesMidi; y++)
	{
		if (FilterGameName(gameConfigMidi[y].gameName).CompareNoCase(gameName) == 0)
		{
			gameConfigMidiFound = gameConfigMidi[y];
			break;
		}
	}

	fprintfIfDebug(outFileDebug, "Soundbank Game %s\n", gameConfigBank.gameName);
	fprintfIfDebug(outFileDebug, "Sound Type %s\n", gameConfigSoundFound.gameType);
	fprintfIfDebug(outFileDebug, "Midi Type %s\n", gameConfigMidiFound.gameType);

	CString tempStr;
	mTimeMultiplier.GetWindowText(tempStr);

	float timeMultiplier = atof(tempStr);
	
	DLSFile file("Soundfont Generated by N64 Soundbank Tool (GoldenEye Vault)");

	int overallSoundCount = 0;

	for (int writeBank = 0; writeBank < alBanks.size(); writeBank++)
	{
		int soundBankStartSpot = 0;
		if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("Konami") == 0))
			soundBankStartSpot = writeBank;
		else if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("PaperMario") == 0))
			soundBankStartSpot = writeBank;

		ALBank* alBankCurrent = alBanks[writeBank];
		int soundBankSelected = soundBankIndexes[writeBank];

		fprintfIfDebug(outFileDebug, "Soundbank %02X\n", soundBankSelected);
		
		if (alBankCurrent != NULL)
		{
			fprintfIfDebug(outFileDebug, "Number Instruments %02X\n", alBankCurrent->count);
			for (int x = 0; x < alBankCurrent->count; x++)
			{
				if (alBankCurrent->inst[x]->soundCount > 0)
				{
					fprintfIfDebug(outFileDebug, "\nInstrument %02X\n", x);

					if (outFileDebug != NULL)
					{
						if (
								(alBankCurrent->soundBankFormat == STANDARDFORMAT)
								|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
								|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							fprintfIfDebug(outFileDebug, "[Not in DLS] Bend Range %02X\n", alBankCurrent->inst[x]->bendRange);
							fprintfIfDebug(outFileDebug, "[Not in DLS] Volume %02X\n", alBankCurrent->inst[x]->volume);
							fprintfIfDebug(outFileDebug, "[Not in DLS] Pan %02X\n", alBankCurrent->inst[x]->pan);
							fprintfIfDebug(outFileDebug, "[Not in DLS] Priority %02X\n", alBankCurrent->inst[x]->priority);
							if ((alBankCurrent->inst[x]->tremType != 0) || (alBankCurrent->inst[x]->tremDelay != 0) || (alBankCurrent->inst[x]->tremDepth != 0) || (alBankCurrent->inst[x]->tremRate != 0))
							{
								fprintfIfDebug(outFileDebug, "[Not in DLS] Trem Type %02X\n", alBankCurrent->inst[x]->tremType);
								fprintfIfDebug(outFileDebug, "[Not in DLS] Trem Delay %02X\n", alBankCurrent->inst[x]->tremDelay);
								fprintfIfDebug(outFileDebug, "[Not in DLS] Trem Depth %02X\n", alBankCurrent->inst[x]->tremDepth);
								fprintfIfDebug(outFileDebug, "[Not in DLS] Trem Rate %02X\n", alBankCurrent->inst[x]->tremRate);
							}

							if ((alBankCurrent->inst[x]->vibType != 0) || (alBankCurrent->inst[x]->vibDelay != 0) || (alBankCurrent->inst[x]->vibDepth != 0) || (alBankCurrent->inst[x]->vibRate != 0))
							{
								fprintfIfDebug(outFileDebug, "[Not in DLS] Vib Type %02X\n", alBankCurrent->inst[x]->vibType);
								fprintfIfDebug(outFileDebug, "[Not in DLS] Vib Delay %02X\n", alBankCurrent->inst[x]->vibDelay);
								fprintfIfDebug(outFileDebug, "[Not in DLS] Vib Depth %02X\n", alBankCurrent->inst[x]->vibDepth);
								fprintfIfDebug(outFileDebug, "[Not in DLS] Vib Rate %02X\n", alBankCurrent->inst[x]->vibRate);
							}
						}
					}

					int bankNumber = floor((float)x / (float)0x80);
					if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("Konami") == 0))
						bankNumber += (soundBankStartSpot * 2);
					else if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("PaperMario") == 0))
						bankNumber = floor((float)(x + (soundBankStartSpot * 0x10)) / (float)0x80);

					if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("Konami") == 0))
						tempStr.Format("Instr_%02X", x + (soundBankStartSpot * 0x100));
					else if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("PaperMario") == 0))
						tempStr.Format("Instr_%02X", x + (soundBankStartSpot * 0x10));
					else
						tempStr.Format("Instr_%02X", x);

					if (x < gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames.size())
					{
						tempStr = gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames[x];
					}

					if (std::find(gameConfigBank.skipInstruments.begin(), gameConfigBank.skipInstruments.end(), x) != gameConfigBank.skipInstruments.end())
						continue;

					int instrumentNumber = (x % 0x80);
					if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("PaperMario") == 0))
						instrumentNumber = ((x + (soundBankSelected * 0x10)) % 0x80);

					DLSInstr* instr = file.AddInstr(bankNumber, instrumentNumber, CStringToString(tempStr));

					fprintfIfDebug(outFileDebug, "Number Sounds %04X\n", alBankCurrent->inst[x]->soundCount);
					for (int y = 0; y < alBankCurrent->inst[x]->soundCount; y++)
					{
						fprintfIfDebug(outFileDebug, "Sound %04X\n", y);

						CString outputWavFile;
						outputWavFile.Format("%stempABC%02X_%04X_%04X.bin", mainFolder, writeBank, x, y);
						if (!n64audio.ExtractRawPCMData(mainFolder, alBankCurrent, x, y, outputWavFile, PRIMARY))
						{
							//MessageBox("Could not do soundbank, error extracting a sound");
							continue;
						}

						int sizePcm = GetSizeFile(outputWavFile);

						FILE* inPcmData = fopen(outputWavFile, "rb");
						if (inPcmData == NULL)
						{
							MessageBox("Could not do soundbank, error reading a written pcm sound");
							return false;
						}

						unsigned char* pcmData = new unsigned char[sizePcm];
						fread(pcmData, 1, sizePcm, inPcmData);
						fclose(inPcmData);

						float samplingRate = (float)alBankCurrent->samplerate;

						if (alBankCurrent->soundBankFormat == N64DD)
						{
							samplingRate = alBankCurrent->inst[x]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec;
						}

						mSamplingRate.GetWindowText(tempStr);
						samplingRate = atoi(tempStr);

						if (mUseT1SamplingRate.GetCheck())
						{
							CString tempStr;
							mSoundBankNumber.GetWindowText(tempStr);

							if (tempStr.Find(" ") != -1)
								tempStr = tempStr.Mid(0, tempStr.Find(" "));

							int soundBankSelected = CN64SoundToolReader::StringHexToLong(tempStr);

							for (int b = 0; b < t1BankList.size(); b++)
							{
								for (int t = 0; t < t1BankList[b].bank->t1Entries.size(); t++)
								{
									if (
										(t1BankList[b].bank->t1Entries[t].bankNumber == soundBankSelected)
										&& (t1BankList[b].bank->t1Entries[t].instrumentNumber == x)
										&& (t1BankList[b].bank->t1Entries[t].soundNumber == y)
										)
									{
										samplingRate = t1BankList[b].bank->t1Entries[t].frequency;
										break;
									}
								}
							}
						}

						if (mHalfSamplingRate.GetCheck())
							samplingRate /= 2.0;

						fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

						if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("Konami") == 0))
							tempStr.Format("Instr_%02X_Smp_%02X", x + (soundBankStartSpot * 0x100), y);
						else if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("PaperMario") == 0))
							tempStr.Format("Instr_%02X_Smp_%02X", x + (soundBankStartSpot * 0x10), y);
						else
							tempStr.Format("Instr_%02X_Smp_%02X", x, y);

						DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
						
						::DeleteFile(outputWavFile);

						DLSRgn* region = instr->AddRgn();
						DLSArt* art = region->AddArt();

						long tcAttack = 0;
						long ptSustain = 0;
						long tcRelease = 0;
						long tcDecay = 0;

						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							float attackPercentage = (float)(alBankCurrent->inst[x]->sounds[y]->env.attackVolume) / (float)0x7F;
							float decayPercentage = (float)(alBankCurrent->inst[x]->sounds[y]->env.decayVolume) / (float)alBankCurrent->inst[x]->sounds[y]->env.attackVolume;

							tcAttack = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->inst[x]->sounds[y]->env.attackTime * timeMultiplier);
							
							if (((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == -1) || ((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == 0))
								tcDecay = 0x7FFFFFFF;
							else
								tcDecay = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime * timeMultiplier);

							double attenInDB;
							if (decayPercentage == 0)
								attenInDB = 96;
							else if (decayPercentage >= 0.99)
								attenInDB = 0;
							else
								attenInDB = 20*log10((1.0/(1 - decayPercentage)));

							if ((attenInDB == 96) && (((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == -1) || ((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == 0)))
								ptSustain = 0x03e80000;
							else
								ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
							tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->inst[x]->sounds[y]->env.releaseTime * timeMultiplier);

							art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);

							fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X (Attack Volume %02X Decay Volume %02X)\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->inst[x]->sounds[y]->env.attackVolume, alBankCurrent->inst[x]->sounds[y]->env.decayVolume);
						}
						else if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
							)
						{

							if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] < 0))
								tcAttack = 0;
							else
								tcAttack = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] / 1000 * 7.5));

							if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] < 0))
							{
								tcDecay = 0x7FFFFFFF;
								double attenInDB = 0;
								ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
							}
							else
							{
								tcDecay = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] / 1000 * 7.5));
								double attenInDB = 96;
								ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
							}
							if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
								tcRelease = CMidiPlayer::TimeSecondsToCents(midiPlayer->EADGameReleaseTimeSeconds[alBankCurrent->inst[x]->sounds[y]->unknown1 & 0xFF]);
							else
								tcRelease = CMidiPlayer::TimeSecondsToCents(0.5);

							art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);

							fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X - %04X %04X %04X %04X %04X %04X %04X %04X %08X\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->inst[x]->sounds[y]->adsrEAD[0], alBankCurrent->inst[x]->sounds[y]->adsrEAD[1], alBankCurrent->inst[x]->sounds[y]->adsrEAD[2], alBankCurrent->inst[x]->sounds[y]->adsrEAD[3], alBankCurrent->inst[x]->sounds[y]->adsrEAD[4], alBankCurrent->inst[x]->sounds[y]->adsrEAD[5], alBankCurrent->inst[x]->sounds[y]->adsrEAD[6], alBankCurrent->inst[x]->sounds[y]->adsrEAD[7], alBankCurrent->inst[x]->sounds[y]->unknown1);
						}

						signed long pan = 0;

						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
						
							if (alBankCurrent->inst[x]->sounds[y]->samplePan == 0x40)
								pan = 0;
							else if (alBankCurrent->inst[x]->sounds[y]->samplePan == 0x7F)
								pan = 0x1F40000;
							else if (alBankCurrent->inst[x]->sounds[y]->samplePan == 0x00)
								pan = -0x1F40000;
							else
							{
								float panFloat = (float)alBankCurrent->inst[x]->sounds[y]->samplePan / (float)0x7F;
								panFloat = (panFloat - 0.5f) * 2.0f;
								pan = (panFloat * (float)0x1F40000);
							}

							if (pan != 0)
							{
								art->AddPan(pan);
								fprintfIfDebug(outFileDebug, "Pan %02X (%08X)\n", alBankCurrent->inst[x]->sounds[y]->samplePan, pan);
							}
						}

						if (alBankCurrent->inst[x]->vibType != 0x00)
						{
							//long vibratoFrequency = CMidiPlayer::PitchHzToCents((signed long)alBankCurrent->inst[x]->vibRate * timeMultiplier);
							//long vibratoDelay = ((float)alBankCurrent->inst[x]->vibDelay / (float)0x7F); 

							//art->AddVibrato(vibratoFrequency, vibratoDelay);
						}

						char cSampleLoops = 0;
						ULONG ulLoopStart = 0;
						ULONG ulLoopLength = 0;
						ULONG ulLoopType = 0;

						if (alBankCurrent->inst[x]->sounds[y]->wav.type == AL_RAW16_WAVE)
						{
							if ((alBankCurrent->inst[x]->sounds[y]->wav.rawWave != NULL)
								&& (alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop != NULL))
							{
								if (alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->count != 0)
								{
									cSampleLoops = 1;
									ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->start;
									ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->start);
									ulLoopType = 0; // FORWARD

									fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
								}
							}
						}
						else if (alBankCurrent->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE)
						{
							if ((alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave != NULL)
								&& (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL))
							{
								if (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->count != 0)
								{
									cSampleLoops = 1;
									ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start;
									ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
									ulLoopType = 0; // FORWARD

									fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
								}
							}
						}
						else if (alBankCurrent->inst[x]->sounds[y]->wav.type == AL_MUSYX_WAVE)
						{
							if ((alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave != NULL)
								&& (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL))
							{
								if (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start != alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->end)
								{
									cSampleLoops = 1;
									ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start;
									ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
									ulLoopType = 0; // FORWARD

									fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
								}
							}
						}

						DLSWsmp* wsmp = region->AddWsmp();
						//DLSWsmp* wsmp = new DLSWsmp();
						//wave->Wsmp = wsmp;

						long volumeAttenuation = 0;
						unsigned char keyBase = 0x3C;
						signed short fineTune = 0;
						float keybaseFloat = 1.0;
						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							float volumePercentage = (float)alBankCurrent->inst[x]->sounds[y]->sampleVolume / (float)0x7F;
							if (alBankCurrent->inst[x]->sounds[y]->sampleVolume >= 0x7F)
							{
								volumeAttenuation = 0x00000000;
							}
							else if (volumePercentage != 0)
							{
								double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
								volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
							}
							else
							{
								volumeAttenuation = 0x7FFFFF00;
							}

							//if ((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == -1)
								//volumeAttenuation = 0;

							keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
							fineTune = (signed short)alBankCurrent->inst[x]->sounds[y]->key.detune;

							if ((alBankCurrent->soundBankFormat == KONAMICTL) || (alBankCurrent->soundBankFormat == KONAMI8CTL))
							{
								if (alBankCurrent->konamiADSR.size() > 0)
								{
									if (x < alBankCurrent->konamiADSR.size())
									{
										keyBase -= (signed char)alBankCurrent->konamiADSR[x].coarseTune;
										fineTune = (signed char)alBankCurrent->konamiADSR[x].fineTune;
									}
								}
							}
						}
						else if (
								(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
								|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
								|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
								)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBankCurrent->inst[x]->sounds[y]->floatKeyBase);
							keyBase = n64audio.ConvertEADGameValueToKeyBase(keybaseFloat);
						}
						else if (
								(alBankCurrent->soundBankFormat == MUSYX)
								|| (alBankCurrent->soundBankFormat == MUSYXLZ)
								|| (alBankCurrent->soundBankFormat == MUSYXZLIB)
								|| (alBankCurrent->soundBankFormat == MUSYXSMALLZLIB)
								)
						{
							keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
						}
						else if (
								(alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2)
								|| (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
								|| (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
								|| (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
								|| (alBankCurrent->soundBankFormat == RNCCOMPRESSEDN64PTR)
								|| (alBankCurrent->soundBankFormat == SNOW)
								|| (alBankCurrent->soundBankFormat == SYDNEY)
								|| (alBankCurrent->soundBankFormat == ARMYMENFORMAT)
								|| (alBankCurrent->soundBankFormat == MEGAMAN64PTRV2)
								|| (alBankCurrent->soundBankFormat == TAZHUFFMAN)
								)
						{
							keyBase = 0x3C - (signed char)alBankCurrent->inst[x]->volume;
							fineTune = (signed char)alBankCurrent->inst[x]->pan;
							//volume is coarse tune, pan is fine tune
						}
						else if (
							(alBankCurrent->soundBankFormat == N64DD)
							)
						{
							keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
							fineTune = (signed short)alBankCurrent->inst[x]->sounds[y]->key.detune;
						}
						else if (
								((alBankCurrent->soundBankFormat == SN64) || (alBankCurrent->soundBankFormat == ZLIBSN64))
								)
						{
							keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
						}

						wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
						fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X Pitch %.17e\n", keyBase, fineTune, volumeAttenuation, keybaseFloat);
						wsmp->cSampleLoops = cSampleLoops;
						wsmp->ulLoopType = ulLoopType;
						wsmp->ulLoopStart = ulLoopStart;
						wsmp->ulLoopLength = ulLoopLength;

						
						region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, overallSoundCount);

						unsigned short usKeyHigh = 0x7F;
						unsigned short usKeyLow = 0x00;
						unsigned short usVelHigh = 0x7F;
						unsigned short usVelLow = 0x00;

						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							usKeyHigh = alBankCurrent->inst[x]->sounds[y]->key.keymax;
							usKeyLow = alBankCurrent->inst[x]->sounds[y]->key.keymin;

							usVelHigh = alBankCurrent->inst[x]->sounds[y]->key.velocitymax;
							usVelLow = alBankCurrent->inst[x]->sounds[y]->key.velocitymin;
						}
						else if (
								(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
								|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
								|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
								)
						{
							unsigned char checkPrevious = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 16) & 0xFF);
							unsigned char checkSecondary = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 8) & 0xFF);

							usKeyHigh = checkSecondary + 0x15;
							usKeyLow = checkPrevious + 0x15;
						}

						region->usKeyHigh = usKeyHigh;
						region->usKeyLow = usKeyLow;

						region->usVelHigh = usVelHigh;
						region->usVelLow = usVelLow;

						fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

						overallSoundCount++;







						if (alBankCurrent->inst[x]->sounds[y]->hasWavePrevious)
						{
							fprintfIfDebug(outFileDebug, "Sound Previous %04X\n", y);

							outputWavFile.Format("%stempABC%02X_%04X_%04X_Prev.bin", mainFolder, writeBank, x, y);
							if (!n64audio.ExtractRawPCMData(mainFolder, alBankCurrent, x, y, outputWavFile, PREVIOUS))
							{
								MessageBox("Could not do soundbank, error extracting a sound");
								return false;
							}

							int sizePcm = GetSizeFile(outputWavFile);

							FILE* inPcmData = fopen(outputWavFile, "rb");
							if (inPcmData == NULL)
							{
								MessageBox("Could not do soundbank, error reading a written pcm sound");
								return false;
							}

							unsigned char* pcmData = new unsigned char[sizePcm];
							fread(pcmData, 1, sizePcm, inPcmData);
							fclose(inPcmData);

							float samplingRate = (float)alBankCurrent->samplerate;
							mSamplingRate.GetWindowText(tempStr);
							samplingRate = atoi(tempStr);

							if (mHalfSamplingRate.GetCheck())
								samplingRate /= 2.0;

							fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

							if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("Konami") == 0))
								tempStr.Format("Instr_%02X_Smp_%02X_Prev", x + (soundBankStartSpot * 0x100), y);
							else if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("PaperMario") == 0))
								tempStr.Format("Instr_%02X_Smp_%02X_Prev", x + (soundBankStartSpot * 0x10), y);
							else
								tempStr.Format("Instr_%02X_Smp_%02X_Prev", x, y);
							DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
							
							::DeleteFile(outputWavFile);

							DLSRgn* region = instr->AddRgn();
							DLSArt* art = region->AddArt();

							long tcAttack = 0;
							long ptSustain = 0;
							long tcRelease = 0;
							long tcDecay = 0;

							if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
							)
							{

								if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] < 0))
									tcAttack = 0;
								else
									tcAttack = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] / 1000 * 7.5));

								if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] < 0))
								{
									tcDecay = 0x7FFFFFFF;
									double attenInDB = 0;
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								}
								else
								{
									tcDecay = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] / 1000 * 7.5));
									double attenInDB = 96;
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								}
								if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									tcRelease = CMidiPlayer::TimeSecondsToCents(midiPlayer->EADGameReleaseTimeSeconds[alBankCurrent->inst[x]->sounds[y]->unknown1 & 0xFF]);
								else
									tcRelease = CMidiPlayer::TimeSecondsToCents(0.5);

								art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);
								fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X - %04X %04X %04X %04X %04X %04X %04X %04X %08X\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->inst[x]->sounds[y]->adsrEAD[0], alBankCurrent->inst[x]->sounds[y]->adsrEAD[1], alBankCurrent->inst[x]->sounds[y]->adsrEAD[2], alBankCurrent->inst[x]->sounds[y]->adsrEAD[3], alBankCurrent->inst[x]->sounds[y]->adsrEAD[4], alBankCurrent->inst[x]->sounds[y]->adsrEAD[5], alBankCurrent->inst[x]->sounds[y]->adsrEAD[6], alBankCurrent->inst[x]->sounds[y]->adsrEAD[7], alBankCurrent->inst[x]->sounds[y]->unknown1);
							}

							signed long pan = 0;
							if (pan != 0)
							{
								art->AddPan(pan);
								fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
							}

							char cSampleLoops = 0;
							ULONG ulLoopStart = 0;
							ULONG ulLoopLength = 0;
							ULONG ulLoopType = 0;

							if (alBankCurrent->inst[x]->sounds[y]->wavPrevious.type == AL_RAW16_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wavPrevious.rawWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wavPrevious.rawWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wavPrevious.rawWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wavPrevious.rawWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wavPrevious.rawWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wavPrevious.rawWave->loop->start);
										ulLoopType = 0; // FORWARD

										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}
							else if (alBankCurrent->inst[x]->sounds[y]->wavPrevious.type == AL_ADPCM_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start);
										ulLoopType = 0; // FORWARD

										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}
							else if (alBankCurrent->inst[x]->sounds[y]->wavPrevious.type == AL_MUSYX_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start != alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wavPrevious.adpcmWave->loop->start);
										ulLoopType = 0; // FORWARD

										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}

							DLSWsmp* wsmp = region->AddWsmp();
							//DLSWsmp* wsmp = new DLSWsmp();
							//wave->Wsmp = wsmp;

							long volumeAttenuation = 0;
							unsigned char keyBase = 0x3C;
							signed short fineTune = 0;
							float keybaseFloat = 1.0f;
							if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
									)
							{
								keybaseFloat = *reinterpret_cast<float*> (&alBankCurrent->inst[x]->sounds[y]->floatKeyBasePrev);
								keyBase = n64audio.ConvertEADGameValueToKeyBase(keybaseFloat);
							}

							fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X Pitch %.17e\n", keyBase, fineTune, volumeAttenuation, keybaseFloat);

							wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
							wsmp->cSampleLoops = cSampleLoops;
							wsmp->ulLoopType = ulLoopType;
							wsmp->ulLoopStart = ulLoopStart;
							wsmp->ulLoopLength = ulLoopLength;

							
							region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, overallSoundCount);

							unsigned short usKeyHigh = 0x7F;
							unsigned short usKeyLow = 0x00;
							unsigned short usVelHigh = 0x7F;
							unsigned short usVelLow = 0x00;

							if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
									)
							{
								unsigned char checkPrevious = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 16) & 0xFF);
								unsigned char checkSecondary = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 8) & 0xFF);
								
								if (checkPrevious != 0x00)
								{
									usKeyHigh = checkPrevious - 1 + 0x15;
									usKeyLow = 0;
								}
								else
								{
									usKeyHigh = 0;
									usKeyLow = 0;
								}
							}

							region->usKeyHigh = usKeyHigh;
							region->usKeyLow = usKeyLow;

							region->usVelHigh = usVelHigh;
							region->usVelLow = usVelLow;

							fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

							overallSoundCount++;
						}

						if (alBankCurrent->inst[x]->sounds[y]->hasWaveSecondary)
						{
							fprintfIfDebug(outFileDebug, "Sound Secondary %04X\n", y);

							outputWavFile.Format("%stempABC%02X_%04X_%04X_Sec.bin", mainFolder, writeBank, x, y);
							if (!n64audio.ExtractRawPCMData(mainFolder, alBankCurrent, x, y, outputWavFile, SECONDARY))
							{
								MessageBox("Could not do soundbank, error extracting a sound");
								return false;
							}

							int sizePcm = GetSizeFile(outputWavFile);

							FILE* inPcmData = fopen(outputWavFile, "rb");
							if (inPcmData == NULL)
							{
								MessageBox("Could not do soundbank, error reading a written pcm sound");
								return false;
							}

							unsigned char* pcmData = new unsigned char[sizePcm];
							fread(pcmData, 1, sizePcm, inPcmData);
							fclose(inPcmData);

							float samplingRate = (float)alBankCurrent->samplerate;
							mSamplingRate.GetWindowText(tempStr);
							samplingRate = atoi(tempStr);

							if (mHalfSamplingRate.GetCheck())
								samplingRate /= 2.0;

							fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

							if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("Konami") == 0))
								tempStr.Format("Instr_%02X_Smp_%02X_Sec", x + (soundBankStartSpot * 0x100), y);
							else if (combineBanks && (gameConfigMidiFound.gameType.CompareNoCase("PaperMario") == 0))
								tempStr.Format("Instr_%02X_Smp_%02X_Sec", x + (soundBankStartSpot * 0x10), y);
							else
								tempStr.Format("Instr_%02X_Smp_%02X_Sec", x, y);
							DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
							
							::DeleteFile(outputWavFile);

							DLSRgn* region = instr->AddRgn();
							DLSArt* art = region->AddArt();

							long tcAttack = 0;
							long ptSustain = 0;
							long tcRelease = 0;
							long tcDecay = 0;

							if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
									)
							{

								if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] < 0))
									tcAttack = 0;
								else
									tcAttack = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] / 1000 * 7.5));

								if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] < 0))
								{
									tcDecay = 0x7FFFFFFF;
									double attenInDB = 0;
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								}
								else
								{
									tcDecay = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] / 1000 * 7.5));
									double attenInDB = 96;
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								}
								if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									tcRelease = CMidiPlayer::TimeSecondsToCents(midiPlayer->EADGameReleaseTimeSeconds[alBankCurrent->inst[x]->sounds[y]->unknown1 & 0xFF]);
								else
									tcRelease = CMidiPlayer::TimeSecondsToCents(0.5);

								art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);
								fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X - %04X %04X %04X %04X %04X %04X %04X %04X %08X\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->inst[x]->sounds[y]->adsrEAD[0], alBankCurrent->inst[x]->sounds[y]->adsrEAD[1], alBankCurrent->inst[x]->sounds[y]->adsrEAD[2], alBankCurrent->inst[x]->sounds[y]->adsrEAD[3], alBankCurrent->inst[x]->sounds[y]->adsrEAD[4], alBankCurrent->inst[x]->sounds[y]->adsrEAD[5], alBankCurrent->inst[x]->sounds[y]->adsrEAD[6], alBankCurrent->inst[x]->sounds[y]->adsrEAD[7], alBankCurrent->inst[x]->sounds[y]->unknown1);
							}

							signed long pan = 0;
							if (pan != 0)
							{
								art->AddPan(pan);
								fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
							}

							char cSampleLoops = 0;
							ULONG ulLoopStart = 0;
							ULONG ulLoopLength = 0;
							ULONG ulLoopType = 0;

							if (alBankCurrent->inst[x]->sounds[y]->wavSecondary.type == AL_RAW16_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wavSecondary.rawWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wavSecondary.rawWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wavSecondary.rawWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wavSecondary.rawWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wavSecondary.rawWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wavSecondary.rawWave->loop->start);
										ulLoopType = 0; // FORWARD
									}
								}
							}
							else if (alBankCurrent->inst[x]->sounds[y]->wavSecondary.type == AL_ADPCM_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start);
										ulLoopType = 0; // FORWARD

										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}
							else if (alBankCurrent->inst[x]->sounds[y]->wavSecondary.type == AL_MUSYX_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wavSecondary.adpcmWave->loop->start);
										ulLoopType = 0; // FORWARD

										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}

							DLSWsmp* wsmp = region->AddWsmp();
							//DLSWsmp* wsmp = new DLSWsmp();
							//wave->Wsmp = wsmp;

							long volumeAttenuation = 0;
							unsigned char keyBase = 0x3C;
							signed short fineTune = 0;
							float keybaseFloat = 1.0f;
							if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
									)
							{
								keybaseFloat = *reinterpret_cast<float*> (&alBankCurrent->inst[x]->sounds[y]->floatKeyBaseSec);
								keyBase = n64audio.ConvertEADGameValueToKeyBase(keybaseFloat);
							}

							fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X Pitch %.17e\n", keyBase, fineTune, volumeAttenuation, keybaseFloat);

							wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
							wsmp->cSampleLoops = cSampleLoops;
							wsmp->ulLoopType = ulLoopType;
							wsmp->ulLoopStart = ulLoopStart;
							wsmp->ulLoopLength = ulLoopLength;

							
							region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, overallSoundCount);

							unsigned short usKeyHigh = 0x7F;
							unsigned short usKeyLow = 0x00;
							unsigned short usVelHigh = 0x7F;
							unsigned short usVelLow = 0x00;

							if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
									)
							{
								unsigned char checkPrevious = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 16) & 0xFF);
								unsigned char checkSecondary = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 8) & 0xFF);
								
								if (checkSecondary != 0x7F)
								{
									usKeyHigh = 127;
									usKeyLow = checkSecondary + 1 + 0x15;
								}
								else
								{
									usKeyHigh = 127;
									usKeyLow = 127;
								}
							}

							region->usKeyHigh = usKeyHigh;
							region->usKeyLow = usKeyLow;

							region->usVelHigh = usVelHigh;
							region->usVelLow = usVelLow;

							fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

							overallSoundCount++;
						}
					}
				}
			}

			// Write drums as MSB
			if (
				((alBankCurrent->soundBankFormat == KONAMICTL) && (alBankCurrent->konamiDrumsADSR.size() > 0))
				||
				((alBankCurrent->soundBankFormat == KONAMI8CTL) && (alBankCurrent->konamiDrumsADSR.size() > 0))
				)
			{
				for (int d = 0; d < alBankCurrent->konamiDrumsADSR.size(); d++)
				{
					fprintfIfDebug(outFileDebug, "Konami Drum %04X\n", d);

					int x = alBankCurrent->konamiDrumsADSR[d].instrumentLookup;

					if (alBankCurrent->inst[x]->soundCount > 0)
					{
						// To MSB
						int bankNumber = floor((float)d / (float)0x80) + 0x100;

						tempStr.Format("Drum_%02X", d );

						if (x < gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames.size())
						{
							tempStr = gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames[x];
						}

						if (std::find(gameConfigBank.skipInstruments.begin(), gameConfigBank.skipInstruments.end(), x) != gameConfigBank.skipInstruments.end())
							continue;

						DLSInstr* instr = file.AddInstr((bankNumber), (d % 0x80), CStringToString(tempStr));

						fprintfIfDebug(outFileDebug, "Number Konami Drum %04X\n", alBankCurrent->inst[x]->soundCount);

						for (int y = 0; y < alBankCurrent->inst[x]->soundCount; y++)
						{
							CString outputWavFile;
							outputWavFile.Format("%stempABCD%02X_%04X_%04X.bin", mainFolder, writeBank, d, y);
							if (!n64audio.ExtractRawPCMData(mainFolder, alBankCurrent, x, y, outputWavFile, PRIMARY))
							{
								//MessageBox("Could not do soundbank, error extracting a sound");
								continue;
							}

							int sizePcm = GetSizeFile(outputWavFile);

							FILE* inPcmData = fopen(outputWavFile, "rb");
							if (inPcmData == NULL)
							{
								MessageBox("Could not do soundbank, error reading a written pcm sound");
								return false;
							}

							unsigned char* pcmData = new unsigned char[sizePcm];
							fread(pcmData, 1, sizePcm, inPcmData);
							fclose(inPcmData);

							float samplingRate = (float)alBankCurrent->samplerate;

							if (alBankCurrent->soundBankFormat == N64DD)
							{
								samplingRate = alBankCurrent->inst[x]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec;
							}

							mSamplingRate.GetWindowText(tempStr);
							samplingRate = atoi(tempStr);

							if (mHalfSamplingRate.GetCheck())
								samplingRate /= 2.0;

							fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

							tempStr.Format("Drum_%02X_Smp_%02X", d, y);
							DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
							
							::DeleteFile(outputWavFile);

							DLSRgn* region = instr->AddRgn();
							DLSArt* art = region->AddArt();

							long tcAttack = 0;
							long ptSustain = 0;
							long tcRelease = 0;
							long tcDecay = 0;

							if (
								(alBankCurrent->soundBankFormat == STANDARDFORMAT)
								|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
								|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
								float attackPercentage = (float)(alBankCurrent->inst[x]->sounds[y]->env.attackVolume) / (float)0x7F;
								float decayPercentage = (float)(alBankCurrent->inst[x]->sounds[y]->env.decayVolume) / (float)alBankCurrent->inst[x]->sounds[y]->env.attackVolume;

								tcAttack = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->inst[x]->sounds[y]->env.attackTime * timeMultiplier);
								
								if (((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == -1) || ((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == 0))
									tcDecay = 0x7FFFFFFF;
								else
									tcDecay = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime * timeMultiplier);

								double attenInDB;
								if (decayPercentage == 0)
									attenInDB = 96;
								else if (decayPercentage >= 0.99)
									attenInDB = 0;
								else
									attenInDB = 20*log10((1.0/(1 - decayPercentage)));

								if ((attenInDB == 96) && (((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == -1) || ((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == 0)))
									ptSustain = 0x03e80000;
								else
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->inst[x]->sounds[y]->env.releaseTime * timeMultiplier);

								art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);
								fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->inst[x]->sounds[y]->env.attackVolume, alBankCurrent->inst[x]->sounds[y]->env.decayVolume);
							}
							else if (
										(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
										|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
										|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
								)
							{

								if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] < 0))
									tcAttack = 0;
								else
									tcAttack = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[0] / 1000 * 7.5));

								if ((alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] < 0))
								{
									tcDecay = 0x7FFFFFFF;
									double attenInDB = 0;
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								}
								else
								{
									tcDecay = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->inst[x]->sounds[y]->adsrEAD[2] / 1000 * 7.5));
									double attenInDB = 96;
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								}
								if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									tcRelease = CMidiPlayer::TimeSecondsToCents(midiPlayer->EADGameReleaseTimeSeconds[alBankCurrent->inst[x]->sounds[y]->unknown1 & 0xFF]);
								else
									tcRelease = CMidiPlayer::TimeSecondsToCents(0.5);

								art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);
								fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X - %04X %04X %04X %04X %04X %04X %04X %04X %08X\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->inst[x]->sounds[y]->adsrEAD[0], alBankCurrent->inst[x]->sounds[y]->adsrEAD[1], alBankCurrent->inst[x]->sounds[y]->adsrEAD[2], alBankCurrent->inst[x]->sounds[y]->adsrEAD[3], alBankCurrent->inst[x]->sounds[y]->adsrEAD[4], alBankCurrent->inst[x]->sounds[y]->adsrEAD[5], alBankCurrent->inst[x]->sounds[y]->adsrEAD[6], alBankCurrent->inst[x]->sounds[y]->adsrEAD[7], alBankCurrent->inst[x]->sounds[y]->unknown1);
							}

							signed long pan = 0;

							if (
								(alBankCurrent->soundBankFormat == STANDARDFORMAT)
								|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
								|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							
								if (alBankCurrent->inst[x]->sounds[y]->samplePan == 0x40)
									pan = 0;
								else if (alBankCurrent->inst[x]->sounds[y]->samplePan == 0x7F)
									pan = 0x1F40000;
								else if (alBankCurrent->inst[x]->sounds[y]->samplePan == 0x00)
									pan = -0x1F40000;
								else
								{
									float panFloat = (float)alBankCurrent->inst[x]->sounds[y]->samplePan / (float)0x7F;
									panFloat = (panFloat - 0.5f) * 2.0f;
									pan = (panFloat * (float)0x1F40000);
								}

								if (pan != 0)
								{
									art->AddPan(pan);
									fprintfIfDebug(outFileDebug, "Pan %02X (%08X)\n", alBankCurrent->inst[x]->sounds[y]->samplePan, pan);
								}
							}

							if (alBankCurrent->inst[x]->vibType != 0x00)
							{
								//long vibratoFrequency = CMidiPlayer::PitchHzToCents((signed long)alBankCurrent->inst[x]->vibRate * timeMultiplier);
								//long vibratoDelay = ((float)alBankCurrent->inst[x]->vibDelay / (float)0x7F); 

								//art->AddVibrato(vibratoFrequency, vibratoDelay);
							}

							char cSampleLoops = 0;
							ULONG ulLoopStart = 0;
							ULONG ulLoopLength = 0;
							ULONG ulLoopType = 0;

							if (alBankCurrent->inst[x]->sounds[y]->wav.type == AL_RAW16_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wav.rawWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wav.rawWave->loop->start);
										ulLoopType = 0; // FORWARD
										
										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}
							else if (alBankCurrent->inst[x]->sounds[y]->wav.type == AL_ADPCM_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
										ulLoopType = 0; // FORWARD
										
										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}
							else if (alBankCurrent->inst[x]->sounds[y]->wav.type == AL_MUSYX_WAVE)
							{
								if ((alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave != NULL)
									&& (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop != NULL))
								{
									if (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[y]->wav.adpcmWave->loop->start);
										ulLoopType = 0; // FORWARD
										
										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}

							DLSWsmp* wsmp = region->AddWsmp();
							//DLSWsmp* wsmp = new DLSWsmp();
							//wave->Wsmp = wsmp;

							long volumeAttenuation = 0;
							unsigned char keyBase = 0x3C;
							signed short fineTune = 0;
							float keybaseFloat = 1.0f;
							if (
								(alBankCurrent->soundBankFormat == STANDARDFORMAT)
								|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
								|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
								float volumePercentage = (float)alBankCurrent->inst[x]->sounds[y]->sampleVolume / (float)0x7F;
								if (alBankCurrent->inst[x]->sounds[y]->sampleVolume >= 0x7F)
								{
									volumeAttenuation = 0x00000000;
								}
								else if (volumePercentage != 0)
								{
									double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
									volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
								}
								else
								{
									volumeAttenuation = 0x7FFFFF00;
								}

								//if ((signed long)alBankCurrent->inst[x]->sounds[y]->env.decayTime == -1)
									//volumeAttenuation = 0;

								keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
								fineTune = (signed short)alBankCurrent->inst[x]->sounds[y]->key.detune;

								keyBase -= (signed char)alBankCurrent->konamiDrumsADSR[d].coarseTune;
								fineTune = (signed char)alBankCurrent->konamiDrumsADSR[d].fineTune;
							}
							else if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
									)
							{
								keybaseFloat = *reinterpret_cast<float*> (&alBankCurrent->inst[x]->sounds[y]->floatKeyBase);
								keyBase = n64audio.ConvertEADGameValueToKeyBase(keybaseFloat);
							}
							else if (
									(alBankCurrent->soundBankFormat == MUSYX)
									|| (alBankCurrent->soundBankFormat == MUSYXLZ)
									|| (alBankCurrent->soundBankFormat == MUSYXZLIB)
									|| (alBankCurrent->soundBankFormat == MUSYXSMALLZLIB)
									)
							{
								keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
							}
							else if (
									(alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2)
									|| (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2YAY0)
									|| (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2ZLIB)
									|| (alBankCurrent->soundBankFormat == N64PTRWAVETABLETABLEV2BLITZ)
									|| (alBankCurrent->soundBankFormat == RNCCOMPRESSEDN64PTR)
									|| (alBankCurrent->soundBankFormat == SNOW)
									|| (alBankCurrent->soundBankFormat == SYDNEY)
									|| (alBankCurrent->soundBankFormat == ARMYMENFORMAT)
									|| (alBankCurrent->soundBankFormat == MEGAMAN64PTRV2)
									|| (alBankCurrent->soundBankFormat == TAZHUFFMAN)
									)
							{
								keyBase = 0x3C - (signed char)alBankCurrent->inst[x]->volume;
								fineTune = (signed char)alBankCurrent->inst[x]->pan;
								//volume is coarse tune, pan is fine tune
							}
							else if (
								(alBankCurrent->soundBankFormat == N64DD)
								)
							{
								keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
								fineTune = (signed short)alBankCurrent->inst[x]->sounds[y]->key.detune;
							}
							else if (
								((alBankCurrent->soundBankFormat == SN64) || (alBankCurrent->soundBankFormat == ZLIBSN64))
								)
							{
								keyBase = alBankCurrent->inst[x]->sounds[y]->key.keybase;
							}

							fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X Pitch %.17e\n", keyBase, fineTune, volumeAttenuation, keybaseFloat);

							wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
							wsmp->cSampleLoops = cSampleLoops;
							wsmp->ulLoopType = ulLoopType;
							wsmp->ulLoopStart = ulLoopStart;
							wsmp->ulLoopLength = ulLoopLength;

							
							region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, overallSoundCount);

							unsigned short usKeyHigh = 0x7F;
							unsigned short usKeyLow = 0x00;
							unsigned short usVelHigh = 0x7F;
							unsigned short usVelLow = 0x00;

							if (
								(alBankCurrent->soundBankFormat == STANDARDFORMAT)
								|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
								|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
								usKeyHigh = alBankCurrent->inst[x]->sounds[y]->key.keymax;
								usKeyLow = alBankCurrent->inst[x]->sounds[y]->key.keymin;

								usVelHigh = alBankCurrent->inst[x]->sounds[y]->key.velocitymax;
								usVelLow = alBankCurrent->inst[x]->sounds[y]->key.velocitymin;
							}
							else if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
									)
							{
								unsigned char checkPrevious = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 16) & 0xFF);
								unsigned char checkSecondary = ((alBankCurrent->inst[x]->sounds[y]->unknown1 >> 8) & 0xFF);

								usKeyHigh = checkSecondary + 0x15;
								usKeyLow = checkPrevious + 0x15;
							}

							region->usKeyHigh = usKeyHigh;
							region->usKeyLow = usKeyLow;

							region->usVelHigh = usVelHigh;
							region->usVelLow = usVelLow;

							fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

							overallSoundCount++;
						}
					}
				}
			}

			if (alBankCurrent->percussion != NULL)
			{
				if (alBankCurrent->percussion->soundCount > 0)
				{
					fprintfIfDebug(outFileDebug, "Percussion\n");

					tempStr.Format("Percussion");

					DLSInstr* instr = file.AddInstr(F_INSTRUMENT_DRUMS, 0, CStringToString(tempStr));

					for (int y = 0; y < alBankCurrent->percussion->soundCount; y++)
					{
						CString outputWavFile;
						outputWavFile.Format("%stempABC%02X_%04X.bin", mainFolder, writeBank, y);
						if (!n64audio.ExtractPercussionRawPCMData(mainFolder, alBankCurrent, y, outputWavFile))
						{
							//MessageBox("Could not do soundbank, error extracting a sound");
							continue;
						}

						int sizePcm = GetSizeFile(outputWavFile);

						FILE* inPcmData = fopen(outputWavFile, "rb");
						if (inPcmData == NULL)
						{
							MessageBox("Could not do soundbank, error reading a written pcm sound");
							return false;
						}

						unsigned char* pcmData = new unsigned char[sizePcm];
						fread(pcmData, 1, sizePcm, inPcmData);
						fclose(inPcmData);

						float samplingRate = (float)alBankCurrent->samplerate;

						if (alBankCurrent->soundBankFormat == N64DD)
						{
							samplingRate = alBankCurrent->percussion->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec;
						}

						mSamplingRate.GetWindowText(tempStr);
						samplingRate = atoi(tempStr);

						if (mHalfSamplingRate.GetCheck())
							samplingRate /= 2.0;

						fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

						tempStr.Format("Percussion_Smp_%02X_%02X", writeBank, y);
						DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
						
						::DeleteFile(outputWavFile);

						DLSRgn* region = instr->AddRgn();
						DLSArt* art = region->AddArt();

						long tcAttack = 0;
						long ptSustain = 0;
						long tcRelease = 0;
						long tcDecay = 0;

						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							float attackPercentage = (float)(alBankCurrent->percussion->sounds[y]->env.attackVolume) / (float)0x7F;
							float decayPercentage = (float)(alBankCurrent->percussion->sounds[y]->env.decayVolume) / (float)alBankCurrent->percussion->sounds[y]->env.attackVolume;

							tcAttack = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->percussion->sounds[y]->env.attackTime * timeMultiplier);
							
							if (((signed long)alBankCurrent->percussion->sounds[y]->env.decayTime == -1) || ((signed long)alBankCurrent->percussion->sounds[y]->env.decayTime == 0))
								tcDecay = 0x7FFFFFFF;
							else
								tcDecay = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->percussion->sounds[y]->env.decayTime * timeMultiplier);

							double attenInDB;
							if (decayPercentage == 0)
								attenInDB = 96;
							else if (decayPercentage >= 0.99)
								attenInDB = 0;
							else
								attenInDB = 20*log10((1.0/(1 - decayPercentage)));

							if ((attenInDB == 96) && (((signed long)alBankCurrent->percussion->sounds[y]->env.decayTime == -1) || ((signed long)alBankCurrent->percussion->sounds[y]->env.decayTime == 0)))
								ptSustain = 0x03e80000;
							else
								ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
							tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)alBankCurrent->percussion->sounds[y]->env.releaseTime * timeMultiplier);

							art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);
							fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->percussion->sounds[y]->env.attackVolume, alBankCurrent->percussion->sounds[y]->env.decayVolume);
						}
						else if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
							)
						{

							if ((alBankCurrent->percussion->sounds[y]->adsrEAD[0] <= 0x0002) || ((signed short)alBankCurrent->percussion->sounds[y]->adsrEAD[0] < 0))
								tcAttack = 0;
							else
								tcAttack = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->percussion->sounds[y]->adsrEAD[0] / 1000 * 7.5));

							if ((alBankCurrent->percussion->sounds[y]->adsrEAD[2] == 0x0001) || ((signed short)alBankCurrent->percussion->sounds[y]->adsrEAD[2] < 0))
							{
								tcDecay = 0x7FFFFFFF;
								double attenInDB = 0;
								ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
							}
							else
							{
								tcDecay = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->percussion->sounds[y]->adsrEAD[2] / 1000 * 7.5));
								double attenInDB = 96;
								ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
							}
							if (alBankCurrent->soundBankFormat == ZELDAFORMAT)
								tcRelease = CMidiPlayer::TimeSecondsToCents(midiPlayer->EADGameReleaseTimeSeconds[alBankCurrent->percussion->sounds[y]->unknown1 & 0xFF]);
							else
								tcRelease = CMidiPlayer::TimeSecondsToCents(0.5);

							art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);
							fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X\n", tcAttack, tcDecay, ptSustain, tcRelease);
						}

						signed long pan = 0;

						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
						
							if (alBankCurrent->percussion->sounds[y]->samplePan == 0x40)
								pan = 0;
							else if (alBankCurrent->percussion->sounds[y]->samplePan == 0x7F)
								pan = 0x1F40000;
							else if (alBankCurrent->percussion->sounds[y]->samplePan == 0x00)
								pan = -0x1F40000;
							else
							{
								float panFloat = (float)alBankCurrent->percussion->sounds[y]->samplePan / (float)0x7F;
								panFloat = (panFloat - 0.5f) * 2.0f;
								pan = (panFloat * (float)0x1F40000);
							}

							if (pan != 0)
							{
								art->AddPan(pan);
								fprintfIfDebug(outFileDebug, "Pan %02X (%08X)\n", alBankCurrent->percussion->sounds[y]->samplePan, pan);
							}
						}

						if (alBankCurrent->percussion->vibType != 0x00)
						{
							//long vibratoFrequency = CMidiPlayer::PitchHzToCents((signed long)alBankCurrent->percussion->vibRate * timeMultiplier);
							//long vibratoDelay = ((float)alBankCurrent->percussion->vibDelay / (float)0x7F); 

							//art->AddVibrato(vibratoFrequency, vibratoDelay);
						}

						char cSampleLoops = 0;
						ULONG ulLoopStart = 0;
						ULONG ulLoopLength = 0;
						ULONG ulLoopType = 0;

						if (alBankCurrent->percussion->sounds[y]->wav.type == AL_RAW16_WAVE)
						{
							if ((alBankCurrent->percussion->sounds[y]->wav.rawWave != NULL)
								&& (alBankCurrent->percussion->sounds[y]->wav.rawWave->loop != NULL))
							{
								if (alBankCurrent->percussion->sounds[y]->wav.rawWave->loop->count != 0)
								{
									cSampleLoops = 1;
									ulLoopStart = alBankCurrent->percussion->sounds[y]->wav.rawWave->loop->start;
									ulLoopLength = (alBankCurrent->percussion->sounds[y]->wav.rawWave->loop->end - alBankCurrent->percussion->sounds[y]->wav.rawWave->loop->start);
									ulLoopType = 0; // FORWARD

									fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
								}
							}
						}
						else if (alBankCurrent->percussion->sounds[y]->wav.type == AL_ADPCM_WAVE)
						{
							if ((alBankCurrent->percussion->sounds[y]->wav.adpcmWave != NULL)
								&& (alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop != NULL))
							{
								if (alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->count != 0)
								{
									cSampleLoops = 1;
									ulLoopStart = alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->start;
									ulLoopLength = (alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->start);
									ulLoopType = 0; // FORWARD

									fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
								}
							}
						}
						else if (alBankCurrent->percussion->sounds[y]->wav.type == AL_MUSYX_WAVE)
						{
							if ((alBankCurrent->percussion->sounds[y]->wav.adpcmWave != NULL)
								&& (alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop != NULL))
							{
								if (alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->count != 0)
								{
									cSampleLoops = 1;
									ulLoopStart = alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->start;
									ulLoopLength = (alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->percussion->sounds[y]->wav.adpcmWave->loop->start);
									ulLoopType = 0; // FORWARD

									fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
								}
							}
						}

						DLSWsmp* wsmp = region->AddWsmp();
						//DLSWsmp* wsmp = new DLSWsmp();
						//wave->Wsmp = wsmp;

						long volumeAttenuation = 0;
						unsigned char keyBase = 0x3C;
						signed short fineTune = 0;
						float keybaseFloat = 1.0f;
						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							float volumePercentage = (float)alBankCurrent->percussion->sounds[y]->sampleVolume / (float)0x7F;
							if (alBankCurrent->percussion->sounds[y]->sampleVolume >= 0x7F)
							{
								volumeAttenuation = 0x00000000;
							}
							else if (volumePercentage != 0)
							{
								double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
								volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
							}
							else
							{
								volumeAttenuation = 0x7FFFFF00;
							}

							//if ((signed long)alBankCurrent->percussion->sounds[y]->env.decayTime == -1)
								//volumeAttenuation = 0;

							keyBase = alBankCurrent->percussion->sounds[y]->key.keybase;
							fineTune = (signed short)alBankCurrent->percussion->sounds[y]->key.detune;
						}
						else if (
								(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
								|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
								|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
								)
						{
							keybaseFloat = *reinterpret_cast<float*> (&alBankCurrent->percussion->sounds[y]->floatKeyBase);
							keyBase = n64audio.ConvertEADGameValueToKeyBase(keybaseFloat);
						}
						else if (
								(alBankCurrent->soundBankFormat == MUSYX)
								|| (alBankCurrent->soundBankFormat == MUSYXLZ)
								|| (alBankCurrent->soundBankFormat == MUSYXZLIB)
								|| (alBankCurrent->soundBankFormat == MUSYXSMALLZLIB)
								)
						{
							keyBase = alBankCurrent->percussion->sounds[y]->key.keybase;
						}
						else if (
							(alBankCurrent->soundBankFormat == N64DD)
							)
						{
							keyBase = alBankCurrent->percussion->sounds[y]->key.keybase;
							fineTune = (signed short)alBankCurrent->percussion->sounds[y]->key.detune;
						}
						else if (
								((alBankCurrent->soundBankFormat == SN64) || (alBankCurrent->soundBankFormat == ZLIBSN64))
								)
						{
							keyBase = alBankCurrent->percussion->sounds[y]->key.keybase;
						}

						fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X Pitch %.17e\n", keyBase, fineTune, volumeAttenuation, keybaseFloat);
						wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
						wsmp->cSampleLoops = cSampleLoops;
						wsmp->ulLoopType = ulLoopType;
						wsmp->ulLoopStart = ulLoopStart;
						wsmp->ulLoopLength = ulLoopLength;

						
						region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, overallSoundCount);

						unsigned short usKeyHigh = 0x7F;
						unsigned short usKeyLow = 0x00;
						unsigned short usVelHigh = 0x7F;
						unsigned short usVelLow = 0x00;

						if (
							(alBankCurrent->soundBankFormat == STANDARDFORMAT)
							|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
							|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
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
							usKeyHigh = alBankCurrent->percussion->sounds[y]->key.keymax;
							usKeyLow = alBankCurrent->percussion->sounds[y]->key.keymin;

							usVelHigh = alBankCurrent->percussion->sounds[y]->key.velocitymax;
							usVelLow = alBankCurrent->percussion->sounds[y]->key.velocitymin;
						}
						else if (
								(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
								|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
								|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
								)
						{
							unsigned char checkPrevious = ((alBankCurrent->percussion->sounds[y]->unknown1 >> 16) & 0xFF);
							unsigned char checkSecondary = ((alBankCurrent->percussion->sounds[y]->unknown1 >> 8) & 0xFF);

							usKeyHigh = checkSecondary + 0x15;
							usKeyLow = checkPrevious + 0x15;
						}

						region->usKeyHigh = usKeyHigh;
						region->usKeyLow = usKeyLow;

						region->usVelHigh = usVelHigh;
						region->usVelLow = usVelLow;

						fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

						overallSoundCount++;
					}
				}
			}

			if ((alBankCurrent->countEADPercussion > 0) && (alBankCurrent->eadPercussion != NULL))
			{
				fprintfIfDebug(outFileDebug, "Number EAD Percussion %04X\n", alBankCurrent->countEADPercussion);

				int bankNumber = 0;

				tempStr.Format("Perc_%02X", bankNumber);
				DLSInstr* instr = file.AddInstr(F_INSTRUMENT_DRUMS, bankNumber, CStringToString(tempStr));	
				for (int y = 0; y < alBankCurrent->countEADPercussion; y++)
				{
					fprintfIfDebug(outFileDebug, "EAD Percussion %02X\n", y);

					CString outputWavFile;
					outputWavFile.Format("%stempABC%04X.bin", mainFolder, y);
					if (!n64audio.ExtractEADPercussionRawPCMData(mainFolder, alBankCurrent, y, outputWavFile))
					{
						//MessageBox("Could not do soundbank, error extracting a percussion");
						continue;
					}
					
					int sizePcm = GetSizeFile(outputWavFile);

					FILE* inPcmData = fopen(outputWavFile, "rb");
					if (inPcmData == NULL)
					{
						MessageBox("Could not do soundbank, error reading a written pcm sound");
						return false;
					}

					unsigned char* pcmData = new unsigned char[sizePcm];
					fread(pcmData, 1, sizePcm, inPcmData);
					fclose(inPcmData);

					float samplingRate = (float)alBankCurrent->samplerate;
					mSamplingRate.GetWindowText(tempStr);
					samplingRate = atoi(tempStr);

					if (mHalfSamplingRate.GetCheck())
						samplingRate /= 2.0;

					fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

					tempStr.Format("Perc_%02X_Smp_%02X", bankNumber, y);
					DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
					
					::DeleteFile(outputWavFile);

					DLSRgn* region = instr->AddRgn();
					DLSArt* art = region->AddArt();

					long tcAttack = 0;
					long ptSustain = 0;
					long tcRelease = 0;
					long tcDecay = 0;

					if (
									(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
									|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
									|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
							)
					{

						if ((alBankCurrent->eadPercussion[y].adsrEAD[0] <= 0x0002) || ((signed short)alBankCurrent->eadPercussion[y].adsrEAD[0] < 0))
							tcAttack = 0;
						else
							tcAttack = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->eadPercussion[y].adsrEAD[0] / 1000 * 7.5));

						if ((alBankCurrent->eadPercussion[y].adsrEAD[2] == 0x0001) || ((signed short)alBankCurrent->eadPercussion[y].adsrEAD[2] < 0))
						{
							tcDecay = 0x7FFFFFFF;
							double attenInDB = 0;
							ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
						}
						else
						{
							tcDecay = CMidiPlayer::TimeSecondsToCents(((float)alBankCurrent->eadPercussion[y].adsrEAD[2] / 1000 * 7.5));
							double attenInDB = 96;
							ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
						}
						tcRelease = CMidiPlayer::TimeSecondsToCents(0.1);

						art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);
						fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X - %04X %04X %04X %04X %04X %04X %04X %04X\n", tcAttack, tcDecay, ptSustain, tcRelease, alBankCurrent->eadPercussion[y].adsrEAD[0], alBankCurrent->eadPercussion[y].adsrEAD[1], alBankCurrent->eadPercussion[y].adsrEAD[2], alBankCurrent->eadPercussion[y].adsrEAD[3], alBankCurrent->eadPercussion[y].adsrEAD[4], alBankCurrent->eadPercussion[y].adsrEAD[5], alBankCurrent->eadPercussion[y].adsrEAD[6], alBankCurrent->eadPercussion[y].adsrEAD[7]);
					}

					signed long pan = 0;
					if (alBankCurrent->eadPercussion[y].pan == 0x40)
						pan = 0;
					else if (alBankCurrent->eadPercussion[y].pan == 0x7F)
						pan = 0x1F40000;
					else if (alBankCurrent->eadPercussion[y].pan == 0x00)
						pan = -0x1F40000;
					else
					{
						float panFloat = (float)alBankCurrent->eadPercussion[y].pan / (float)0x7F;
						panFloat = (panFloat - 0.5f) * 2.0f;
						pan = (panFloat * (float)0x1F40000);
					}

					if (pan != 0)
					{
						art->AddPan(pan);
						fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
					}

					char cSampleLoops = 0;
					ULONG ulLoopStart = 0;
					ULONG ulLoopLength = 0;
					ULONG ulLoopType = 0;

					if (alBankCurrent->eadPercussion[y].wav.type == AL_RAW16_WAVE)
					{
						if ((alBankCurrent->eadPercussion[y].wav.rawWave != NULL)
							&& (alBankCurrent->eadPercussion[y].wav.rawWave->loop != NULL))
						{
							if (alBankCurrent->eadPercussion[y].wav.rawWave->loop->count != 0)
							{
								cSampleLoops = 1;
								ulLoopStart = alBankCurrent->eadPercussion[y].wav.rawWave->loop->start;
								ulLoopLength = (alBankCurrent->eadPercussion[y].wav.rawWave->loop->end - alBankCurrent->eadPercussion[y].wav.rawWave->loop->start);
								ulLoopType = 0; // FORWARD

								fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
							}
						}
					}
					else if (alBankCurrent->eadPercussion[y].wav.type == AL_ADPCM_WAVE)
					{
						if ((alBankCurrent->eadPercussion[y].wav.adpcmWave != NULL)
							&& (alBankCurrent->eadPercussion[y].wav.adpcmWave->loop != NULL))
						{
							if (alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->count != 0)
							{
								cSampleLoops = 1;
								ulLoopStart = alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->start;
								ulLoopLength = (alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->end - alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->start);
								ulLoopType = 0; // FORWARD

								fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
							}
						}
					}
					else if (alBankCurrent->eadPercussion[y].wav.type == AL_MUSYX_WAVE)
					{
						if ((alBankCurrent->eadPercussion[y].wav.adpcmWave != NULL)
							&& (alBankCurrent->eadPercussion[y].wav.adpcmWave->loop != NULL))
						{
							if (alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->count != 0)
							{
								cSampleLoops = 1;
								ulLoopStart = alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->start;
								ulLoopLength = (alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->end - alBankCurrent->eadPercussion[y].wav.adpcmWave->loop->start);
								ulLoopType = 0; // FORWARD

								fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
							}
						}
					}

					DLSWsmp* wsmp = region->AddWsmp();
					//DLSWsmp* wsmp = new DLSWsmp();
					//wave->Wsmp = wsmp;

					long volumeAttenuation = 0;
					unsigned char keyBase = 0x3C;
					signed short fineTune = 0;
					float keybaseFloat = 1.0f;
					if (
							(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
							|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
							|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
							)
					{
						keybaseFloat = *reinterpret_cast<float*> (&alBankCurrent->eadPercussion[y].keyBase);
						keyBase = n64audio.ConvertEADGameValueToKeyBase(keybaseFloat);
					}

					fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X Pitch %.17e\n", keyBase, fineTune, volumeAttenuation, keybaseFloat);
					wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
					wsmp->cSampleLoops = cSampleLoops;
					wsmp->ulLoopType = ulLoopType;
					wsmp->ulLoopStart = ulLoopStart;
					wsmp->ulLoopLength = ulLoopLength;

					
					region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, overallSoundCount);

					unsigned short usKeyHigh = y + 0x15;
					unsigned short usKeyLow = y + 0x15;
					unsigned short usVelHigh = 0x7F;
					unsigned short usVelLow = 0x00;

					region->usKeyHigh = usKeyHigh;
					region->usKeyLow = usKeyLow;

					region->usVelHigh = usVelHigh;
					region->usVelLow = usVelLow;

					fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

					overallSoundCount++;
				}
			}
		}
		else
		{
			return false;
		}
	}

	bool saved = file.SaveDLSFile(pathNameStr);

	if (!saved)
	{
		MessageBox("Can't save file, bad filename " + pathNameStr);
		return false;
	}

	return true;
}

bool CN64SoundbankToolDlg::WriteDLSGMBankPaperMario(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, std::vector<int> instrumentIndex, std::vector<int> instrumentOffset, FILE* outFileDebug)
{
	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	GameConfig gameConfigBank;
	for (int g = 0; g < countGames; g++)
	{
		if (gameName == gameConfig[g].gameName)
		{
			gameConfigBank = gameConfig[g];
			break;
		}
	}

	SoundGameConfig gameConfigSoundFound;
	for (int y = 0; y < countGamesSound; y++)
	{
		if (FilterGameName(gameConfigSound[y].gameName) == FilterGameName(gameConfigBank.gameName))
		{
			gameConfigSoundFound = gameConfigSound[y];
			break;
		}
	}

	MidiGameConfig gameConfigMidiFound;
	for (int y = 0; y < countGamesMidi; y++)
	{
		if (FilterGameName(gameConfigMidi[y].gameName).CompareNoCase(gameName) == 0)
		{
			gameConfigMidiFound = gameConfigMidi[y];
			break;
		}
	}

	fprintfIfDebug(outFileDebug, "Soundbank Game %s\n", gameConfigBank.gameName);
	fprintfIfDebug(outFileDebug, "Sound Type %s\n", gameConfigSoundFound.gameType);
	fprintfIfDebug(outFileDebug, "Midi Type %s\n", gameConfigMidiFound.gameType);

	CString tempStr;
	mTimeMultiplier.GetWindowText(tempStr);

	float timeMultiplier = atof(tempStr);
	
	DLSFile file("Soundfont Generated by N64 Soundbank Tool (GoldenEye Vault)");

	int overallSoundCount = 0;

	for (int gmBankSpot = 0; gmBankSpot < 0x100; gmBankSpot++)
	{
		fprintfIfDebug(outFileDebug, "\nGM Bank Spot %04X\n", gmBankSpot);

		int bankNumber = floor((float)gmBankSpot / (float)0x80);
		
		tempStr.Format("Instr_%02X", gmBankSpot);

		if (std::find(gameConfigBank.skipInstruments.begin(), gameConfigBank.skipInstruments.end(), gmBankSpot) != gameConfigBank.skipInstruments.end())
			continue;

		int instrumentNumber = (gmBankSpot % 0x80);
	
		DLSInstr* instr = file.AddInstr(bankNumber, instrumentNumber, CStringToString(tempStr));

		int alBankIndex = instrumentIndex[gmBankSpot];
		// Drum placeholder
		if (alBankIndex == -1)
			continue;

		ALBank* alBankCurrent = alBanks[alBankIndex];

		if (alBankCurrent == NULL)
			continue;

		int alBankOffset = instrumentOffset[gmBankSpot];

		int y = 0;
		{
			CString outputWavFile;
			outputWavFile.Format("%stempABC%02X_%04X_%04X.bin", mainFolder, alBankIndex, alBankOffset, y);
			if (!n64audio.ExtractRawPCMData(mainFolder, alBankCurrent, alBankOffset, y, outputWavFile, PRIMARY))
			{
				//MessageBox("Could not do soundbank, error extracting a sound");
				continue;
			}

			int sizePcm = GetSizeFile(outputWavFile);

			FILE* inPcmData = fopen(outputWavFile, "rb");
			if (inPcmData == NULL)
			{
				MessageBox("Could not do soundbank, error reading a written pcm sound");
				return false;
			}

			unsigned char* pcmData = new unsigned char[sizePcm];
			fread(pcmData, 1, sizePcm, inPcmData);
			fclose(inPcmData);

			float samplingRate = (float)alBankCurrent->samplerate;

			mSamplingRate.GetWindowText(tempStr);
			samplingRate = atoi(tempStr);

			if (mHalfSamplingRate.GetCheck())
				samplingRate /= 2.0;

			fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

			tempStr.Format("Instr_%02X_Smp_%02X", gmBankSpot, y);

			DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
			
			::DeleteFile(outputWavFile);

			DLSRgn* region = instr->AddRgn();
			DLSArt* art = region->AddArt();

			long tcAttack = 0;
			long ptSustain = 0;
			long tcRelease = 0;
			long tcDecay = 0;

			signed long pan = 0;

			if (alBankCurrent->inst[alBankOffset]->vibType != 0x00)
			{
				//long vibratoFrequency = CMidiPlayer::PitchHzToCents((signed long)alBankCurrent->inst[alBankOffset]->vibRate * timeMultiplier);
				//long vibratoDelay = ((float)alBankCurrent->inst[alBankOffset]->vibDelay / (float)0x7F); 

				//art->AddVibrato(vibratoFrequency, vibratoDelay);
			}

			char cSampleLoops = 0;
			ULONG ulLoopStart = 0;
			ULONG ulLoopLength = 0;
			ULONG ulLoopType = 0;

			if (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.type == AL_RAW16_WAVE)
			{
				if ((alBankCurrent->inst[alBankOffset]->sounds[y]->wav.rawWave != NULL)
					&& (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.rawWave->loop != NULL))
				{
					if (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.rawWave->loop->count != 0)
					{
						cSampleLoops = 1;
						ulLoopStart = alBankCurrent->inst[alBankOffset]->sounds[y]->wav.rawWave->loop->start;
						ulLoopLength = (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.rawWave->loop->end - alBankCurrent->inst[alBankOffset]->sounds[y]->wav.rawWave->loop->start);
						ulLoopType = 0; // FORWARD

						fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
					}
				}
			}
			else if (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.type == AL_ADPCM_WAVE)
			{
				if ((alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave != NULL)
					&& (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop != NULL))
				{
					if (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->count != 0)
					{
						cSampleLoops = 1;
						ulLoopStart = alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->start;
						ulLoopLength = (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->start);
						ulLoopType = 0; // FORWARD

						fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
					}
				}
			}
			else if (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.type == AL_MUSYX_WAVE)
			{
				if ((alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave != NULL)
					&& (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop != NULL))
				{
					if (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->start != alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->end)
					{
						cSampleLoops = 1;
						ulLoopStart = alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->start;
						ulLoopLength = (alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->end - alBankCurrent->inst[alBankOffset]->sounds[y]->wav.adpcmWave->loop->start);
						ulLoopType = 0; // FORWARD

						fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
					}
				}
			}

			DLSWsmp* wsmp = region->AddWsmp();
			//DLSWsmp* wsmp = new DLSWsmp();
			//wave->Wsmp = wsmp;

			long volumeAttenuation = 0;
			unsigned char keyBase = 0x3C;

			unsigned short noteCenter = (alBankCurrent->inst[alBankOffset]->sounds[0]->wav.unknown1 & 0xFFFF) / 0x64;
			unsigned short fineTuneBank = (alBankCurrent->inst[alBankOffset]->sounds[0]->wav.unknown1 & 0xFFFF) % 0x64;

			keyBase = noteCenter + 0xC;
			signed short fineTune = -fineTuneBank;

			fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
			wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
			wsmp->cSampleLoops = cSampleLoops;
			wsmp->ulLoopType = ulLoopType;
			wsmp->ulLoopStart = ulLoopStart;
			wsmp->ulLoopLength = ulLoopLength;

			
			region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, overallSoundCount);

			unsigned short usKeyHigh = 0x7F;
			unsigned short usKeyLow = 0x00;
			unsigned short usVelHigh = 0x7F;
			unsigned short usVelLow = 0x00;

			region->usKeyHigh = usKeyHigh;
			region->usKeyLow = usKeyLow;

			region->usVelHigh = usVelHigh;
			region->usVelLow = usVelLow;

			fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

			overallSoundCount++;		
		}
	}

	bool saved = file.SaveDLSFile(pathNameStr);

	if (!saved)
	{
		MessageBox("Can't save file, bad filename " + pathNameStr);
		return false;
	}

	return true;
}


void CN64SoundbankToolDlg::ParseSoundMacroList(std::vector<Factor5SoundMacro> soundMacroList, std::map<int, Factor5ADSR> poolTables, int& sampleId, unsigned long& attackTime, unsigned long& decayTime, float& sustainPercentage, unsigned long& releaseTime, unsigned char& macroPan)
{
	for (int macroNumber = 0; macroNumber < soundMacroList.size(); macroNumber++)
	{
		if (soundMacroList[macroNumber].commandData[0] == 0xC) // SETADSR
		{
			unsigned short tableLookupId = (soundMacroList[macroNumber].commandData[2] << 8) | soundMacroList[macroNumber].commandData[1];
			Factor5ADSR adsrLookedUp = poolTables[tableLookupId];
			unsigned char dlsMode = soundMacroList[macroNumber].commandData[3];

			attackTime = adsrLookedUp.attackTime;
			decayTime = adsrLookedUp.decayTime;
			sustainPercentage = adsrLookedUp.sustainPercentage;
			releaseTime = adsrLookedUp.releaseTime;
		}
		else if (soundMacroList[macroNumber].commandData[0] == 0xE) // Pan
		{
			macroPan = soundMacroList[macroNumber].commandData[1];
		}
		else if (soundMacroList[macroNumber].commandData[0] == 0x10) // Start Sound
		{
			sampleId = (soundMacroList[macroNumber].commandData[2] << 8) | soundMacroList[macroNumber].commandData[1];
		}
		else if (soundMacroList[macroNumber].commandData[0] == 0x16) // SETADSRCTRL
		{
			attackTime = soundMacroList[macroNumber].commandData[1];
			decayTime = soundMacroList[macroNumber].commandData[2];
			sustainPercentage = (float)soundMacroList[macroNumber].commandData[3] / 256.0f;
			releaseTime = soundMacroList[macroNumber].commandData[4];
		}
	}
}

bool CN64SoundbankToolDlg::WriteDLSCombinedFactor5(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, unsigned char* buffer, int romSize, unsigned long projOffset, unsigned long poolOffset, int projSize, int poolSize, FILE* outFileDebug, bool absoluteOffsets)
{
	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	SoundGameConfig gameConfigSoundFound;
	for (int y = 0; y < countGamesSound; y++)
	{
		if (FilterGameName(gameConfigSound[y].gameName) == FilterGameName(gameConfigBank.gameName))
		{
			gameConfigSoundFound = gameConfigSound[y];
			break;
		}
	}

	MidiGameConfig gameConfigMidiFound;
	for (int y = 0; y < countGamesMidi; y++)
	{
		if (FilterGameName(gameConfigMidi[y].gameName).CompareNoCase(gameName) == 0)
		{
			gameConfigMidiFound = gameConfigMidi[y];
			break;
		}
	}

	fprintfIfDebug(outFileDebug, "Soundbank Game %s\n", gameConfigBank.gameName);
	fprintfIfDebug(outFileDebug, "Sound Type %s\n", gameConfigSoundFound.gameType);
	fprintfIfDebug(outFileDebug, "Midi Type %s\n", gameConfigMidiFound.gameType);

	CString tempStr;
	mTimeMultiplier.GetWindowText(tempStr);

	float timeMultiplier = atof(tempStr);

	unsigned char* proj = NULL;
	unsigned char* pool = NULL;

	bool isCompressed = false;
	int zlibGame = 0;
	if (buffer[poolOffset] == 0x78)
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameConfigSoundFound.gameType == "Musyx")
	{
		isCompressed = false;
	}
	else if (gameConfigSoundFound.gameType == "ZLibMusyx")
	{
		isCompressed = true;
		zlibGame = GAUNTLETLEGENDS;
	}
	else if (gameConfigSoundFound.gameType == "ZLib78DAMusyx")
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameConfigSoundFound.gameType == "MusyxREZLib")
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameConfigSoundFound.gameType == "MusyxSmallZlib")
	{
		isCompressed = true;
		zlibGame = STUNTRACER64;
	}
	else if (gameConfigSoundFound.gameType == "LzMusyx")
	{
		isCompressed = true;
	}
	if (!isCompressed)
	{
		proj = new unsigned char[projSize];
		memcpy(proj, &buffer[projOffset], projSize);
		pool = new unsigned char[poolSize];
		memcpy(pool, &buffer[poolOffset], poolSize);
	}
	else
	{
		projSize = 0;
		poolSize = 0;

		if (gameConfigSoundFound.gameType == "LzMusyx")
		{
			proj = new unsigned char[0x100000];
			pool = new unsigned char[0x100000];
			MidwayLZ midwayLz;
			projSize = midwayLz.dec(&buffer[projOffset], romSize - projOffset, proj);
			poolSize = midwayLz.dec(&buffer[poolOffset], romSize - poolOffset, pool);
			/*FILE* projDebugFile = fopen("C:\\temp\\proj.bin", "wb");
			if (projDebugFile)
			{
				fwrite(proj, 1, projSize, projDebugFile);
				fflush(projDebugFile);
				fclose(projDebugFile);
			}
			FILE* poolDebugFile = fopen("C:\\temp\\pool.bin", "wb");
			if (poolDebugFile)
			{
				fwrite(pool, 1, poolSize, poolDebugFile);
				fflush(poolDebugFile);
				fclose(poolDebugFile);
			}*/
		}
		else
		{
			GECompression compression;
			compression.SetPath(mainFolder);
			if ((buffer[poolOffset] == 0x78) && (buffer[poolOffset+1] == 0xDA))
			{
				compression.SetGame(STUNTRACER64);
			}
			else
			{
				compression.SetGame(zlibGame);
			}

			compression.SetCompressedBuffer(&buffer[projOffset], romSize - projOffset);
			projSize = 0;
			int projCompressedSize = 0;
			proj = compression.OutputDecompressedBuffer(projSize, projCompressedSize);

			compression.SetCompressedBuffer(&buffer[poolOffset], romSize - poolOffset);
			poolSize = 0;
			int poolCompressedSize = 0;
			pool = compression.OutputDecompressedBuffer(poolSize, poolCompressedSize);
		}
	}
	

	unsigned int soundMacrosOffset = CharArrayToLong(&pool[0]);
    unsigned int tablesOffset = CharArrayToLong(&pool[4]);
    unsigned int keymapsOffset = CharArrayToLong(&pool[8]);
    unsigned int layersOffset = CharArrayToLong(&pool[0xC]);

	std::map<int, Factor5SoundMacroList> poolMacroList; // By ObjectId
	std::map<int, Factor5ADSR> poolTables; // By ObjectId
	std::map<int, Factor5KeymapGroup> poolKeyGroups; // By ObjectId
	std::map<int, Factor5LayerGroup> poolLayers; // By ObjectId

	CString commandType[0xFF];
	commandType[0x1] = "STOP";
	commandType[0x2] = "SPLITKEY";
	commandType[0x3] = "SPLITVEL";
	commandType[0x4] = "WAIT_TICKS";
	commandType[0x5] = "LOOP";
	commandType[0x6] = "GOTO";
	commandType[0x7] = "WAIT_MS";
	commandType[0x8] = "PLAYMACRO";
	commandType[0x9] = "SENDKEYOFF";
	commandType[0xA] = "SPLITMOD";
	commandType[0xB] = "PIANOPAN";
	commandType[0xC] = "SETADSR";
	commandType[0xD] = "SCALEVOLUME";
	commandType[0xE] = "PANNING";
	commandType[0xF] = "ENVELOPE";
	commandType[0x10] = "STARTSAMPLE";
	commandType[0x11] = "STOPSAMPLE";
	commandType[0x12] = "KEYOFF";
	commandType[0x13] = "SPLITRND";
	commandType[0x14] = "FADE-IN";
	commandType[0x15] = "SPANNING";
	commandType[0x16] = "SETADSRCTRL";
	commandType[0x17] = "RNDNOTE";
	commandType[0x18] = "ADDNOTE";
	commandType[0x19] = "SETNOTE";
	commandType[0x1A] = "LASTNOTE";
	commandType[0x1B] = "PORTAMENTO";
	commandType[0x1C] = "VIBRATO";
	commandType[0x1D] = "PITCHSWEEP1";
	commandType[0x1E] = "PITCHSWEEP2";
	commandType[0x1F] = "SETPITCH";
	commandType[0x20] = "SETPITCHADSR";
	commandType[0x21] = "SCALEVOLUME DLS";
	commandType[0x22] = "MOD2VIBRANGE";
	commandType[0x23] = "SETUP TREMOLO";
	commandType[0x24] = "RETURN";
	commandType[0x25] = "GOSUB";
	commandType[0x28] = "TRAP_EVENT";
	commandType[0x29] = "UNTRAP_EVENT";
	commandType[0x2A] = "SEND_MESSAGE";
	commandType[0x2B] = "GET_MESSAGE";
	commandType[0x2C] = "GET_VID";
	commandType[0x30] = "ADDAGECOUNT";
	commandType[0x31] = "SETAGECOUNT";
	commandType[0x32] = "SENDFLAG";
	commandType[0x33] = "PITCHWHEELR";
	commandType[0x34] = "SETPRIORITY";
	commandType[0x35] = "ADDPRIORITY";
	commandType[0x36] = "AGECNTSPEED";
	commandType[0x37] = "AGECNTVEL";
	commandType[0x40] = "VOL_SELECT";
	commandType[0x41] = "PAN_SELECT";
	commandType[0x42] = "PitchW_SELECT";
	commandType[0x43] = "ModW_SELECT";
	commandType[0x44] = "PEDAL_SELECT";
	commandType[0x45] = "PORTA_SELECT";
	commandType[0x46] = "REVERB_SELECT";
	commandType[0x47] = "SPAN_SELECT";
	commandType[0x48] = "DOPPLER_SELECT";
	commandType[0x49] = "TREMOLO_SELECT";
	commandType[0x4A] = "PREA_SELECT";
	commandType[0x4B] = "PREB_SELECT";
	commandType[0x4C] = "POSTB_SELECT";
	commandType[0x4D] = "AUXAFX_SELECT";
	commandType[0x4E] = "AUXBFX_SELECT";
	commandType[0x50] = "SETUP_LFO";
	commandType[0x58] = "MODE_SELECT";
	commandType[0x59] = "SET_KEYGROUP";
	commandType[0x5A] = "SRCMODE_SELECT";
	commandType[0x60] = "ADD_VARS";
	commandType[0x61] = "SUB_VARS";
	commandType[0x62] = "MUL_VARS";
	commandType[0x63] = "DIV_VARS";
	commandType[0x64] = "ADDI_VARS";
	commandType[0x70] = "IF_EQUAL";
	commandType[0x71] = "IF_LESS";
	commandType[0x0] = "END";


	if (soundMacrosOffset != NULL)
	{
		//fprintf(outPool, "Sound Macros Offset\n");

		unsigned long tempSoundMacrosOffset = soundMacrosOffset;
		while (CharArrayToLong(&pool[tempSoundMacrosOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempSoundMacrosOffset]);
            int id = CharArrayToShort(&pool[tempSoundMacrosOffset + 4]);

			//fprintf(outPool, "%08X: Id %04X Size %08X\n", tempSoundMacrosOffset, id, size);

			Factor5SoundMacroList soundMacroList;

			unsigned long tempCommandOffset = tempSoundMacrosOffset + 8;
			while (pool[tempCommandOffset + 3] != 0x00)
			{
				Factor5SoundMacro soundMacro;

				soundMacro.commandData[0] = pool[tempCommandOffset + 3];
				soundMacro.commandData[1] = pool[tempCommandOffset + 2];
				soundMacro.commandData[2] = pool[tempCommandOffset + 1];
				soundMacro.commandData[3] = pool[tempCommandOffset + 0];

				soundMacro.commandData[4] = pool[tempCommandOffset + 7];
				soundMacro.commandData[5] = pool[tempCommandOffset + 6];
				soundMacro.commandData[6] = pool[tempCommandOffset + 5];
				soundMacro.commandData[7] = pool[tempCommandOffset + 4];

				soundMacroList.soundMacros.push_back(soundMacro);

				//fprintf(outPool, "	%08X: Command %02X %s - %02X %02X %02X %02X %02X %02X %02X\n", tempCommandOffset, commandData[0], commandType[commandData[0]], commandData[1], commandData[2], commandData[3], commandData[4], commandData[5], commandData[6], commandData[7]);
				

				tempCommandOffset += 8;
			}

			poolMacroList[id] = soundMacroList;

			tempSoundMacrosOffset += size;

		}
	}

	if (tablesOffset != NULL)
	{
		//fprintf(outPool, "\nTables Offset\n");

		unsigned long tempTablesOffset = tablesOffset;
		while (CharArrayToLong(&pool[tempTablesOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempTablesOffset]);
            int id = CharArrayToShort(&pool[tempTablesOffset + 4]);

			//fprintf(outPool, "%08X: Id %04X Size %08X", tempTablesOffset, id, size);

			/*0x0	1	Attack time (0-255 milliseconds); no multiplication is done to the value
			0x1	1	Attack time (0-65280 milliseconds); multiply value by 256
			0x2	1	Decay time (0-255 milliseconds); no multiplication is done to the value
			0x3	1	Decay time (0-65280 milliseconds); multiply value by 256
			0x4	1	Sustain (percentage); multiply value by 0.0244
			0x5	1	Sustain (percentage); multiply value by 6.25
			0x6	1	Release time (0-255 milliseconds); no multiplication is done to the value
			0x7	1	Release time (0-65280 milliseconds); multiply value by 256*/

			Factor5ADSR adsr;
			adsr.objectId = id;
			adsr.attackTime = (pool[tempTablesOffset + 8] + (pool[tempTablesOffset + 9] << 8));
			adsr.decayTime = (pool[tempTablesOffset + 0xA] + (pool[tempTablesOffset + 0xB] << 8));
			adsr.sustainPercentage = (((float)pool[tempTablesOffset + 0xC] * 0.0244) + ((float)pool[tempTablesOffset + 0xD] * 6.25));
			adsr.releaseTime = (pool[tempTablesOffset + 0xE] + (pool[tempTablesOffset + 0xF] << 8));

			//fprintf(outPool, " Attack Time %04X Decay Time %04X Sustain %f Release Time %04X\n", attackTime, decayTime, sustainPercentage, releaseTime);

			poolTables[id] = adsr;

			tempTablesOffset += size;
		}
	}
	
	if (keymapsOffset != NULL)
	{
		//fprintf(outPool, "\nKeymaps Offset\n");

		unsigned long tempKeymapsOffset = keymapsOffset;
		while (CharArrayToLong(&pool[tempKeymapsOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempKeymapsOffset]);
            int id = CharArrayToShort(&pool[tempKeymapsOffset + 4]);

			//fprintf(outPool, "%08X: Id %04X Size %08X\n", tempKeymapsOffset, id, size);

			Factor5KeymapGroup keymapGroup;
			keymapGroup.objectId = id;
			for (int y = 0; y < 128; y++)
			{
				keymapGroup.keys[y].objectId = CharArrayToShort(&pool[tempKeymapsOffset + 8 + (y * 8)]);
				keymapGroup.keys[y].transpose = pool[tempKeymapsOffset + 8 + (y * 8) + 1];
				keymapGroup.keys[y].pan = pool[tempKeymapsOffset + 8 + (y * 8) + 2];
				keymapGroup.keys[y].priorityOffset = pool[tempKeymapsOffset + 8 + (y * 8) + 3];
				//fprintf(outPool, "	%08X: Key #%02X ObjectId %04X Transpose %02X Pan %02X Priority Offset %02X\n", tempKeymapsOffset + 8 + (y * 8), y, objectId, transpose, pan, priorityOffset);
			}

			poolKeyGroups[id] = keymapGroup;
			tempKeymapsOffset += size;
		}
	}

	if (layersOffset != NULL)
	{
		//fprintf(outPool, "\nLayers Offset\n");

		unsigned long tempLayersOffset = layersOffset;
		while (CharArrayToLong(&pool[tempLayersOffset]) != 0xFFFFFFFF)
		{
			unsigned long size = CharArrayToLong(&pool[tempLayersOffset]);
            int id = CharArrayToShort(&pool[tempLayersOffset + 4]);

			unsigned long count = CharArrayToLong(&pool[tempLayersOffset + 8]);
			//fprintf(outPool, "%08X: Id %04X Size %08X Count %08X\n", tempLayersOffset, id, size, count);

			Factor5LayerGroup layerGroup;
			layerGroup.objectId = id;

			unsigned long subTempLayersOffset = tempLayersOffset + 12;
			for (int y = 0; y < count ; y++)
			{
				Factor5Layer layer;

				layer.objectId = CharArrayToShort(&pool[subTempLayersOffset]);
				layer.keyLo = pool[subTempLayersOffset + 2];
				layer.keyHi = pool[subTempLayersOffset + 3];
				layer.transpose = pool[subTempLayersOffset + 4];
				layer.volume = pool[subTempLayersOffset + 5];
				layer.priorityOffset = pool[subTempLayersOffset + 6];
				layer.span = pool[subTempLayersOffset + 7];
				layer.pan = pool[subTempLayersOffset + 8];

				layerGroup.factor5Layers.push_back(layer);

				//fprintf(outPool, "%08X: #%02X Object Id %04X Key Low %02X Key Hi %02X Transpose %02X Volume %02X Priority Offset %02X Span %02X Pan %02X\n", subTempLayersOffset, y, objectId, keyLo, keyHi, transpose, volume, priorityOffset, span, pan);
				subTempLayersOffset += 12;
			}

			poolLayers[id] = layerGroup;
			//fprintf(outPool, "\n");

			tempLayersOffset += size;
		}
	}




	int groupOffset = 0;
	int groupCounter = 0;

	while (groupOffset < projSize)
	{
		unsigned int groupEndOff = CharArrayToLong(&proj[groupOffset + 0]);

		if (groupEndOff == 0xFFFFFFFF)
			break;

		unsigned short groupId = CharArrayToShort(&proj[groupOffset + 4]);
		int type = CharArrayToShort(&proj[groupOffset + 6]);
		unsigned int soundMacroIdsOff = CharArrayToLong(&proj[groupOffset + 8]);
		unsigned int samplIdsOff = CharArrayToLong(&proj[groupOffset + 0xC]);
		unsigned int tableIdsOff = CharArrayToLong(&proj[groupOffset + 0x10]);
		unsigned int keymapIdsOff = CharArrayToLong(&proj[groupOffset + 0x14]);
		unsigned int layerIdsOff = CharArrayToLong(&proj[groupOffset + 0x18]);
		unsigned int pageTableOff = CharArrayToLong(&proj[groupOffset + 0x1C]);
		unsigned int sfxTableOff = CharArrayToLong(&proj[groupOffset + 0x1C]);
		unsigned int drumTableOff = CharArrayToLong(&proj[groupOffset + 0x20]);
		unsigned int midiSetupsOff = CharArrayToLong(&proj[groupOffset + 0x24]);

		unsigned long subDataOff = absoluteOffsets ? groupOffset : groupOffset + 8;

		CString typeStr;
		if (type == 0)
		{
			typeStr = "SONG";
			//fprintf(outProj, "#%02X %s Group Offset: %08X Group End Offset: %08X - Group Id %04X Type %04X Sound Macro Ids Offset %08X Samples Id Offset %08X Table Ids Offset %08X Key Map Offset %08X Layer Ids %08X Page Table %08X Drum Page Table %08X Midi Setups %08X\n", groupCounter, typeStr, subDataOff, subDataOff + groupEndOff, groupId, type, subDataOff + soundMacroIdsOff, subDataOff + samplIdsOff, subDataOff + tableIdsOff, subDataOff + keymapIdsOff, subDataOff + layerIdsOff, subDataOff + pageTableOff, subDataOff + drumTableOff, subDataOff + midiSetupsOff);
		}
		else
		{
			typeStr = "SFX ";
			//fprintf(outProj, "#%02X %s Group Offset: %08X Group End Offset: %08X - Group Id %04X Type %04X Sound Macro Ids Offset %08X Samples Id Offset %08X Table Ids Offset %08X Key Map Offset %08X Layer Ids %08X SFX Table Offset %08X \n", groupCounter, typeStr, subDataOff, subDataOff + groupEndOff, groupId, type, subDataOff + soundMacroIdsOff, subDataOff + samplIdsOff, subDataOff + tableIdsOff, subDataOff + keymapIdsOff, subDataOff + layerIdsOff, subDataOff + pageTableOff);
		}
		
		if (type == 0) // Song
        {
			std::map<int, Factor5Page> instrumentPages; // by program number
			std::map<int, Factor5Page> drumPages; // by program number
			std::map<int, Factor5SongArray> songInfo; // by Song id

			unsigned int tempPageTableOff = pageTableOff;
			if (!absoluteOffsets)
				tempPageTableOff += subDataOff;

			//fprintf(outProj, "\nPage Info\n");

			while (CharArrayToShort(&proj[tempPageTableOff]) != 0xFFFF)
			{
				Factor5Page page;
				page.objId = CharArrayToShort(&proj[tempPageTableOff]);
				page.priority = proj[tempPageTableOff + 2];
				page.maxVoices = proj[tempPageTableOff + 3];
				page.unk = proj[tempPageTableOff + 4];
				page.programNo = proj[tempPageTableOff + 5];
				page.pad = CharArrayToShort(&proj[tempPageTableOff + 6]);
				instrumentPages[page.programNo] = page;

				//fprintf(outProj, "%08X: Page: ObjId %04X Priority %02X Max Voices %02X Unknown %02X Program Number %02X\n", tempPageTableOff, objId, priority, maxVoices, unk, programNo);
		
				tempPageTableOff += 8;
			}

			tempPageTableOff = drumTableOff;
			if (!absoluteOffsets)
				tempPageTableOff += subDataOff;

			//fprintf(outProj, "\nDrum Page Info\n");

			while (CharArrayToShort(&proj[tempPageTableOff]) != 0xFFFF)
			{
				Factor5Page page;
				page.objId = CharArrayToShort(&proj[tempPageTableOff]);
				page.priority = proj[tempPageTableOff + 2];
				page.maxVoices = proj[tempPageTableOff + 3];
				page.unk = proj[tempPageTableOff + 4];
				page.programNo = proj[tempPageTableOff + 5];
				page.pad = CharArrayToShort(&proj[tempPageTableOff + 6]);
				drumPages[page.programNo] = page;

				//fprintf(outProj, "%08X: Drum Page: ObjId %04X Priority %02X Max Voices %02X Unknown %02X Program Number %02X\n", tempPageTableOff, objId, priority, maxVoices, unk, programNo);

				tempPageTableOff += 8;
			}

			unsigned long tempMidiSetupData = midiSetupsOff;
			if (!absoluteOffsets)
				tempMidiSetupData += subDataOff;

			//fprintf(outProj, "\nSong Info\n");
			while (CharArrayToLong(&proj[tempMidiSetupData]) != 0xFFFFFFFF)
            {
				Factor5SongArray songArray;

				songArray.songId = CharArrayToShort(&proj[tempMidiSetupData]);

				//fprintf(outProj, "%08X: Song Id %04X\n", tempMidiSetupData, songId);

				for (int y = 0; y < 16; y++)
				{
					if (absoluteOffsets)
					{
						songArray.songInfo[y].programNo = proj[tempMidiSetupData + 4 + (y * 5)];
						songArray.songInfo[y].volume = proj[tempMidiSetupData + 4 + (y * 5) + 1];
						songArray.songInfo[y].panning = proj[tempMidiSetupData + 4 + (y * 5) + 2];
						songArray.songInfo[y].reverb = proj[tempMidiSetupData + 4 + (y * 5) + 3];
						songArray.songInfo[y].chorus = proj[tempMidiSetupData + 4 + (y * 5) + 4];
					}
					else
					{
						songArray.songInfo[y].programNo = proj[tempMidiSetupData + 4 + (y * 8)];
						songArray.songInfo[y].volume = proj[tempMidiSetupData + 4 + (y * 8) + 1];
						songArray.songInfo[y].panning = proj[tempMidiSetupData + 4 + (y * 8) + 2];
						songArray.songInfo[y].reverb = proj[tempMidiSetupData + 4 + (y * 8) + 3];
						songArray.songInfo[y].chorus = proj[tempMidiSetupData + 4 + (y * 8) + 4];
					// pad 3
					}

					//fprintf(outProj, "#%02X Program Number %02X Volume %02X Panning %02X Reverb %02X Chorus %02X\n", y, programNo, volume, panning, reverb, chorus);
				}


				songInfo[songArray.songId] = songArray;

                if (absoluteOffsets)
				{
					tempMidiSetupData += 5 * 16 + 4;
					break;
				}
				else
				{
					tempMidiSetupData += 8 * 16 + 4;
				}
            }

			for (map<int, Factor5SongArray>::iterator iter = songInfo.begin(); iter != songInfo.end(); iter++)
			{
				fprintfIfDebug(outFileDebug, "\nSong %04X\n", iter->first);

				int songId = iter->first;
				Factor5SongArray songArray = iter->second;

				DLSFile file("Soundfont Generated by N64 Soundbank Tool (GoldenEye Vault)");

				CString tempStr;

				int bankNumber = 0;
				ALBank* alBankCurrent = alBanks[bankNumber];
				for (int x = 0; x < alBankCurrent->count; x++)
				{
					fprintfIfDebug(outFileDebug, "\nInstrument %04X\n", x);

					int y = 0;

					CString outputWavFile;
					outputWavFile.Format("%stempABC%02X_%04X.bin", mainFolder, x, y);
					if (!n64audio.ExtractRawPCMData(mainFolder, alBankCurrent, x, y, outputWavFile, PRIMARY))
					{
						//MessageBox("Could not do soundbank, error extracting a sound");
						continue;
					}

					int sizePcm = GetSizeFile(outputWavFile);

					FILE* inPcmData = fopen(outputWavFile, "rb");
					if (inPcmData == NULL)
					{
						MessageBox("Could not do soundbank, error reading a written pcm sound");
						return false;
					}

					unsigned char* pcmData = new unsigned char[sizePcm];
					fread(pcmData, 1, sizePcm, inPcmData);
					fclose(inPcmData);

					float samplingRate = (float)alBankCurrent->inst[x]->samplerate;

					//samplingRate = alBankCurrent->inst[x]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec;

					//mSamplingRate.GetWindowText(tempStr);
					//samplingRate = atoi(tempStr);

					if (mHalfSamplingRate.GetCheck())
						samplingRate /= 2.0;

					fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

					tempStr.Format("Instr_%02X_Smp_%02X", x, y);

					DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
				}





				for (int channel = 0; channel < 0x10; channel++)
				{
					int programNumber = songArray.songInfo[channel].programNo;
					fprintfIfDebug(outFileDebug, "\nChannel %02X\n", channel);
					int bankNumber = 0;
					Factor5Page* page;
					if (channel == 9)
					{
						if (drumPages.find(programNumber) != drumPages.end())
						{
							//bankNumber = F_INSTRUMENT_DRUMS;
							page = &drumPages[programNumber];
						}
						else
						{
							continue;
						}
					}
					else
					{
						if (instrumentPages.find(programNumber) != instrumentPages.end())
						{
							page = &instrumentPages[programNumber];
						}
						else
						{
							continue;
						}
					}
					fprintfIfDebug(outFileDebug, "Page %02X - Object Id %08X\n", programNumber, page->objId);
					
					int objectId = page->objId;

					Factor5ADSR* adsr = NULL;
					Factor5KeymapGroup* keymapGroup = NULL;
					Factor5LayerGroup* layerGroup = NULL;
					Factor5SoundMacroList* soundMacroList = NULL;

					if (poolTables.find(objectId) != poolTables.end())
					{
						adsr = &poolTables[objectId];
					}

					if (poolKeyGroups.find(objectId) != poolKeyGroups.end())
					{
						keymapGroup = &poolKeyGroups[objectId];
					}

					if (poolLayers.find(objectId) != poolLayers.end())
					{
						layerGroup = &poolLayers[objectId];
					}

					if (poolMacroList.find(objectId) != poolMacroList.end())
					{
						soundMacroList = &poolMacroList[objectId];
					}

					tempStr.Format("Channel_%02X", channel);
					int instrumentNumber = channel;
					DLSInstr* instr = file.AddInstr(bankNumber, instrumentNumber, CStringToString(tempStr));


					if (layerGroup != NULL)
					{
						for (int layer = (layerGroup->factor5Layers.size() - 1); layer >= 0; layer--)
						{
							if (layerGroup->factor5Layers[layer].objectId != 0xFFFF)
							{
								fprintfIfDebug(outFileDebug, "\nLayer %02X\n", layer);

								Factor5Layer layerData = layerGroup->factor5Layers[layer];
								
								std::vector<Factor5SoundMacro> soundMacros = poolMacroList[layerData.objectId].soundMacros;

								int sampleId = -1;
								unsigned long attackTime = 0;
								unsigned long decayTime = 0;
								float sustainPercentage = 0.0;
								unsigned long releaseTime = 0;
								unsigned char macroPan = 0x40;

								ParseSoundMacroList(soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

								int foundSampleId = -1;
								for (int s = 0; s < alBankCurrent->count; s++)
								{
									if ((alBankCurrent->inst[s] != NULL) && (alBankCurrent->inst[s]->sounds[0] != NULL))
									{
										// sfx id check
										if (alBankCurrent->inst[s]->sounds[0]->wav.unknown1 == sampleId)
										{
											foundSampleId = s;
										}
									}
								}

								fprintfIfDebug(outFileDebug, "\nSample Id %04X Lookup Index %04X\n", sampleId, foundSampleId);

								sampleId = foundSampleId;

								DLSRgn* region = instr->AddRgn();
								DLSArt* art = region->AddArt();

								long tcAttack = 0;
								long ptSustain = 0;
								long tcRelease = 0;
								long tcDecay = 0;

								float attackPercentage = 1.0f;
								float decayPercentage = sustainPercentage / 100.0f;

								tcAttack = CMidiPlayer::TimeSecondsToCents(attackTime * timeMultiplier);
								
								if (((signed long)decayTime == -1) || ((signed long)decayTime == 0))
									tcDecay = 0x7FFFFFFF;
								else
									tcDecay = CMidiPlayer::TimeSecondsToCents(decayTime * timeMultiplier);

								double attenInDB;
								if (decayPercentage == 0)
									attenInDB = 96;
								else if (decayPercentage >= 0.99)
									attenInDB = 0;
								else
									attenInDB = 20*log10((1.0/(1 - decayPercentage)));

								if ((attenInDB == 96) && (((signed long)decayTime == -1) || ((signed long)decayTime == 0)))
									ptSustain = 0x03e80000;
								else
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)releaseTime * timeMultiplier);

								fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X\n", tcAttack, tcDecay, ptSustain, tcRelease);
								art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);



								signed long pan = 0;
								macroPan = layerData.pan;

								if (macroPan == 0x40)
									pan = 0;
								else if (macroPan == 0x7F)
									pan = 0x1F40000;
								else if (macroPan == 0x00)
									pan = -0x1F40000;
								else
								{
									float panFloat = (float)macroPan / (float)0xFF;
									panFloat = (panFloat - 0.5f) * 2.0f;
									pan = (panFloat * (float)0x1F40000);
								}

								if (pan != 0)
								{
									art->AddPan(pan);
									fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
								}



								char cSampleLoops = 0;
								ULONG ulLoopStart = 0;
								ULONG ulLoopLength = 0;
								ULONG ulLoopType = 0;

								if (sampleId != -1)
								{
									if (alBankCurrent->inst[sampleId]->sounds[0]->wav.type == AL_MUSYX_WAVE)
									{
										if ((alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave != NULL)
											&& (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop != NULL))
										{
											if (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->count != 0)
											{
												cSampleLoops = 1;
												ulLoopStart = alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start;
												ulLoopLength = (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->end - alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start);
												ulLoopType = 0; // FORWARD

												fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
											}
										}
									}
								}



								DLSWsmp* wsmp = region->AddWsmp();
								//DLSWsmp* wsmp = new DLSWsmp();
								//wave->Wsmp = wsmp;

								long volumeAttenuation = 0;
								unsigned char keyBase = 0x3C;
								signed short fineTune = 0;

								int volume = layerData.volume;


								float volumePercentage = (float)volume / (float)0x7F;
								if (volume >= 0x7F)
								{
									volumeAttenuation = 0x00000000;
								}
								else if (volumePercentage != 0)
								{
									double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
									volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
								}
								else
								{
									volumeAttenuation = 0x7FFFFF00;
								}

								//if ((signed long)decayTime == -1)
									//volumeAttenuation = 0;

								if (sampleId != -1)
								{
									keyBase = alBankCurrent->inst[sampleId]->sounds[0]->key.keybase;
									keyBase -= layerData.transpose;
									//fineTune = ;//
								}

								fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
								wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
								wsmp->cSampleLoops = cSampleLoops;
								wsmp->ulLoopType = ulLoopType;
								wsmp->ulLoopStart = ulLoopStart;
								wsmp->ulLoopLength = ulLoopLength;

								
								if (sampleId != -1)
								{
									region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, sampleId);
								}

								region->usKeyHigh = layerData.keyHi;
								region->usKeyLow = layerData.keyLo;

								if (layer != 0)
								{
									if (layerGroup->factor5Layers[layer-1].keyLo == layerData.keyHi)
										region->usKeyHigh--;
								}

								region->usVelHigh = 0x7F;
								region->usVelLow = 0;

								fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", region->usKeyLow, region->usKeyHigh, region->usVelLow, region->usVelHigh);
							}
						}
					}
					else if (keymapGroup != NULL)
					{
						for (int keyNumber = 0; keyNumber < 128; keyNumber++)
						{
							if (keymapGroup->keys[keyNumber].objectId != 0xFFFF)
							{
								fprintfIfDebug(outFileDebug, "\nKey %04X\n", keyNumber);

								Factor5Keymap keyMap = keymapGroup->keys[keyNumber];
								
								std::vector<Factor5SoundMacro> soundMacros = poolMacroList[keyMap.objectId].soundMacros;

								int sampleId = -1;
								unsigned long attackTime = 0;
								unsigned long decayTime = 0;
								float sustainPercentage = 0.0;
								unsigned long releaseTime = 0;
								unsigned char macroPan = 0x40;

								ParseSoundMacroList(soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

								int foundSampleId = -1;
								for (int s = 0; s < alBankCurrent->count; s++)
								{
									if ((alBankCurrent->inst[s] != NULL) && (alBankCurrent->inst[s]->sounds[0] != NULL))
									{
										// sfx id check
										if (alBankCurrent->inst[s]->sounds[0]->wav.unknown1 == sampleId)
										{
											foundSampleId = s;
										}
									}
								}

								fprintfIfDebug(outFileDebug, "\nSample Id %04X Lookup Index %04X\n", sampleId, foundSampleId);

								sampleId = foundSampleId;

								DLSRgn* region = instr->AddRgn();
								DLSArt* art = region->AddArt();

								long tcAttack = 0;
								long ptSustain = 0;
								long tcRelease = 0;
								long tcDecay = 0;

								float attackPercentage = 1.0f;
								float decayPercentage = sustainPercentage / 100.0f;

								tcAttack = CMidiPlayer::TimeSecondsToCents(attackTime * timeMultiplier);
								
								if (((signed long)decayTime == -1) || ((signed long)decayTime == 0))
									tcDecay = 0x7FFFFFFF;
								else
									tcDecay = CMidiPlayer::TimeSecondsToCents(decayTime * timeMultiplier);

								double attenInDB;
								if (decayPercentage == 0)
									attenInDB = 96;
								else if (decayPercentage >= 0.99)
									attenInDB = 0;
								else
									attenInDB = 20*log10((1.0/(1 - decayPercentage)));

								if ((attenInDB == 96) && (((signed long)decayTime == -1) || ((signed long)decayTime == 0)))
									ptSustain = 0x03e80000;
								else
									ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
								tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)releaseTime * timeMultiplier);

								fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X\n", tcAttack, tcDecay, ptSustain, tcRelease);
								art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);



								signed long pan = 0;
								macroPan = keyMap.pan;

								if (macroPan == 0x40)
									pan = 0;
								else if (macroPan == 0x7F)
									pan = 0x1F40000;
								else if (macroPan == 0x00)
									pan = -0x1F40000;
								else
								{
									float panFloat = (float)macroPan / (float)0xFF;
									panFloat = (panFloat - 0.5f) * 2.0f;
									pan = (panFloat * (float)0x1F40000);
								}

								if (pan != 0)
								{
									art->AddPan(pan);
									fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
								}



								char cSampleLoops = 0;
								ULONG ulLoopStart = 0;
								ULONG ulLoopLength = 0;
								ULONG ulLoopType = 0;

								if (sampleId != -1)
								{
									if (alBankCurrent->inst[sampleId]->sounds[0]->wav.type == AL_MUSYX_WAVE)
									{
										if ((alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave != NULL)
											&& (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop != NULL))
										{
											if (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->count != 0)
											{
												cSampleLoops = 1;
												ulLoopStart = alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start;
												ulLoopLength = (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->end - alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start);
												ulLoopType = 0; // FORWARD

												fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
											}
										}
									}
								}



								DLSWsmp* wsmp = region->AddWsmp();
								//DLSWsmp* wsmp = new DLSWsmp();
								//wave->Wsmp = wsmp;

								long volumeAttenuation = 0;
								unsigned char keyBase = 0x3C;
								signed short fineTune = 0;

								int volume = 0x7F;


								float volumePercentage = (float)volume / (float)0x7F;
								if (volume >= 0x7F)
								{
									volumeAttenuation = 0x00000000;
								}
								else if (volumePercentage != 0)
								{
									double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
									volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
								}
								else
								{
									volumeAttenuation = 0x7FFFFF00;
								}

								//if ((signed long)decayTime == -1)
									//volumeAttenuation = 0;

								if (sampleId != -1)
								{
									keyBase = alBankCurrent->inst[sampleId]->sounds[0]->key.keybase;
									keyBase -= keyMap.transpose;
									//fineTune = ;//
								}


								fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
								wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
								wsmp->cSampleLoops = cSampleLoops;
								wsmp->ulLoopType = ulLoopType;
								wsmp->ulLoopStart = ulLoopStart;
								wsmp->ulLoopLength = ulLoopLength;

								
								if (sampleId != -1)
								{
									region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, sampleId);
								}

								region->usKeyHigh = keyNumber;
								region->usKeyLow = keyNumber;

								region->usVelHigh = 0x7F;
								region->usVelLow = 0;

								fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", region->usKeyLow, region->usKeyHigh, region->usVelLow, region->usVelHigh);
							}
						}
					}
					else if (soundMacroList != NULL)
					{
						int sampleId = -1;
						unsigned long attackTime = 0;
						unsigned long decayTime = 0;
						float sustainPercentage = 0.0;
						unsigned long releaseTime = 0;
						unsigned char macroPan = 0x40;

						ParseSoundMacroList(soundMacroList->soundMacros, poolTables, sampleId, attackTime, decayTime, sustainPercentage, releaseTime, macroPan);

						int foundSampleId = -1;
						for (int s = 0; s < alBankCurrent->count; s++)
						{
							if ((alBankCurrent->inst[s] != NULL) && (alBankCurrent->inst[s]->sounds[0] != NULL))
							{
								// sfx id check
								if (alBankCurrent->inst[s]->sounds[0]->wav.unknown1 == sampleId)
								{
									foundSampleId = s;
								}
							}
						}

						fprintfIfDebug(outFileDebug, "\nSample Id %04X Lookup Index %04X\n", sampleId, foundSampleId);

						sampleId = foundSampleId;

						DLSRgn* region = instr->AddRgn();
						DLSArt* art = region->AddArt();

						long tcAttack = 0;
						long ptSustain = 0;
						long tcRelease = 0;
						long tcDecay = 0;

						float attackPercentage = 1.0f;
						float decayPercentage = sustainPercentage / 100.0f;

						tcAttack = CMidiPlayer::TimeSecondsToCents(attackTime * timeMultiplier);
						
						if (((signed long)decayTime == -1) || ((signed long)decayTime == 0))
							tcDecay = 0x7FFFFFFF;
						else
							tcDecay = CMidiPlayer::TimeSecondsToCents(decayTime * timeMultiplier);

						double attenInDB;
						if (decayPercentage == 0)
							attenInDB = 96;
						else if (decayPercentage >= 0.99)
							attenInDB = 0;
						else
							attenInDB = 20*log10((1.0/(1 - decayPercentage)));

						if ((attenInDB == 96) && (((signed long)decayTime == -1) || ((signed long)decayTime == 0)))
							ptSustain = 0x03e80000;
						else
							ptSustain = ((96.0-attenInDB)/96.0)*0x03e80000;
						tcRelease = CMidiPlayer::TimeSecondsToCents((signed long)releaseTime * timeMultiplier);

						fprintfIfDebug(outFileDebug, "Attack %08X Decay %08X Sustain %08X Release %08X\n", tcAttack, tcDecay, ptSustain, tcRelease);
						art->AddADSR(tcAttack, 0, tcDecay, ptSustain, tcRelease, 0);



						signed long pan = 0;

						if (macroPan == 0x40)
							pan = 0;
						else if (macroPan == 0x7F)
							pan = 0x1F40000;
						else if (macroPan == 0x00)
							pan = -0x1F40000;
						else
						{
							float panFloat = (float)macroPan / (float)0xFF;
							panFloat = (panFloat - 0.5f) * 2.0f;
							pan = (panFloat * (float)0x1F40000);
						}

						if (pan != 0)
						{
							art->AddPan(pan);
							fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
						}



						char cSampleLoops = 0;
						ULONG ulLoopStart = 0;
						ULONG ulLoopLength = 0;
						ULONG ulLoopType = 0;

						if (sampleId != -1)
						{
							if (alBankCurrent->inst[sampleId]->sounds[0]->wav.type == AL_MUSYX_WAVE)
							{
								if ((alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave != NULL)
									&& (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop != NULL))
								{
									if (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->count != 0)
									{
										cSampleLoops = 1;
										ulLoopStart = alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start;
										ulLoopLength = (alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->end - alBankCurrent->inst[sampleId]->sounds[0]->wav.adpcmWave->loop->start);
										ulLoopType = 0; // FORWARD

										fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
									}
								}
							}
						}



						DLSWsmp* wsmp = region->AddWsmp();
						//DLSWsmp* wsmp = new DLSWsmp();
						//wave->Wsmp = wsmp;

						long volumeAttenuation = 0;
						unsigned char keyBase = 0x3C;
						signed short fineTune = 0;

						int volume = 0x7F;


						float volumePercentage = (float)volume / (float)0x7F;
						if (volume >= 0x7F)
						{
							volumeAttenuation = 0x00000000;
						}
						else if (volumePercentage != 0)
						{
							double attenInDBVol = 20*log10((1.0/(1.0 - volumePercentage)));
							volumeAttenuation = ((96.0-attenInDBVol)/96.0)*0x03e80000;		//the DLS envelope is a range from 0 to -96db. 
						}
						else
						{
							volumeAttenuation = 0x7FFFFF00;
						}

						//if ((signed long)decayTime == -1)
							//volumeAttenuation = 0;

						if (sampleId != -1)
						{
							keyBase = alBankCurrent->inst[sampleId]->sounds[0]->key.keybase;
							//fineTune = ;//
						}

						fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
						wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
						wsmp->cSampleLoops = cSampleLoops;
						wsmp->ulLoopType = ulLoopType;
						wsmp->ulLoopStart = ulLoopStart;
						wsmp->ulLoopLength = ulLoopLength;

						
						if (sampleId != -1)
						{
							region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, sampleId);
						}

						region->usKeyHigh = 0x7F;
						region->usKeyLow = 0;

						region->usVelHigh = 0x7F;
						region->usVelLow = 0;

						fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", region->usKeyLow, region->usKeyHigh, region->usVelLow, region->usVelHigh);
					}


					/*

					DLSRgn* region = instr->AddRgn();
					DLSArt* art = region->AddArt();

					long tcAttack = 0;
					long ptSustain = 0;
					long tcRelease = 0;
					long tcDecay = 0;

					signed long pan = 0;

					char cSampleLoops = 0;
					ULONG ulLoopStart = 0;
					ULONG ulLoopLength = 0;
					ULONG ulLoopType = 0;

					if ((alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave != NULL)
							&& (alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop != NULL))
					{
						if (alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->count != 0)
						{
							cSampleLoops = 1;
							ulLoopStart = alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->start;
							ulLoopLength = (alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->start);
							ulLoopType = 0; // FORWARD
						}
					}

					DLSWsmp* wsmp = region->AddWsmp();
					//DLSWsmp* wsmp = new DLSWsmp();
					//wave->Wsmp = wsmp;

					long volumeAttenuation = 0;
					unsigned char keyBase = 0x3C;
					signed short fineTune = 0;

					signed char coarseTuneBank = (signed char)((alBankCurrent->inst[x]->sounds[0]->wav.unknown1 >> 8) & 0xFF);
					signed char fineTuneBank = (signed char)((alBankCurrent->inst[x]->sounds[0]->wav.unknown1) & 0xFF);
					keyBase -= coarseTuneBank;
					fineTune = (signed char)fineTuneBank;

					wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
					wsmp->cSampleLoops = cSampleLoops;
					wsmp->ulLoopType = ulLoopType;
					wsmp->ulLoopStart = ulLoopStart;
					wsmp->ulLoopLength = ulLoopLength;

					
					region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, soundPositionsByIndex[overallPaperMarioCount]);

					unsigned short usKeyHigh = 0x7F;
					unsigned short usKeyLow = 0x00;
					unsigned short usVelHigh = 0x7F;
					unsigned short usVelLow = 0x00;

					region->usKeyHigh = usKeyHigh;
					region->usKeyLow = usKeyLow;

					region->usVelHigh = usVelHigh;
					region->usVelLow = usVelLow;*/
				}


	

				CString pathNameStrSub = pathNameStr;
				pathNameStrSub.Replace(".dls", "");

				/*if (songId < gameConfigMidiFound.numberMidiNames)
				{
					tempStr.Format("_%02X_%s.dls", m, gameConfigMidiFound.midiNames[m]);
				}
				else*/
				{
					tempStr.Format("_%02X.dls", songId);
				}
				pathNameStrSub += tempStr;
				pathNameStrSub.Trim();
				pathNameStrSub.Replace("\n", "");
				pathNameStrSub.Replace("\r", "");
				pathNameStrSub.Replace("*", "");
				pathNameStrSub.Replace("?", "");
				pathNameStrSub.Replace("<", "");
				pathNameStrSub.Replace(">", "");
				pathNameStrSub.Replace("|", "");

				bool saved = file.SaveDLSFile(pathNameStrSub);
				if (!saved)
				{
					MessageBox("Can't save file, bad filename " + pathNameStrSub);
					return false;
				}
			}
		}
		else if (type == 1) // SFX
		{
			//fprintf(outProj, "\SFX\n");
			unsigned int tempSfxTableOff = sfxTableOff;
			if (!absoluteOffsets)
				tempSfxTableOff += subDataOff;

			unsigned short count = CharArrayToShort(&proj[tempSfxTableOff]);

			for (int y = 0; y < count; y++)
			{
				unsigned short defineId = CharArrayToShort(&proj[tempSfxTableOff + 4 + (y * 0xC)]);
				unsigned short objectId = CharArrayToShort(&proj[tempSfxTableOff + 4 + (y * 0xC) + 2]);
				unsigned char priority = proj[tempSfxTableOff + 4 + (y * 0xC) + 4];
				unsigned char maxVoices = proj[tempSfxTableOff + 4 + (y * 0xC) + 5];
				unsigned char volume = proj[tempSfxTableOff + 4 + (y * 0xC) + 6];
				unsigned char pan = proj[tempSfxTableOff + 4 + (y * 0xC) + 7];
				unsigned char key = proj[tempSfxTableOff + 4 + (y * 0xC) + 8];
				unsigned char unk = proj[tempSfxTableOff + 4 + (y * 0xC) + 9];

				//fprintf(outProj, "%08X: Define %04X ObjectId %04X Priority %02X MaxVoices %02X Volume %02X Pan %02X Key %02X Unkn %02X\n", tempSfxTableOff + 4 + (y * 0xC), defineId, objectId, priority, maxVoices, volume, pan, key, unk);
			}
		}
		else
		{
			break;
		}

		//fprintf(outProj, "\nSound Macros Ids\n");
		unsigned long tempSoundMacroIdsOff = soundMacroIdsOff;
		if (!absoluteOffsets)
			tempSoundMacroIdsOff += subDataOff;
		while (CharArrayToShort(&proj[tempSoundMacroIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempSoundMacroIdsOff]));
			tempSoundMacroIdsOff += 4;
		}

		//fprintf(outProj, "\nTables Ids\n");
		unsigned long tempTablesIdsOff = tableIdsOff;
		if (!absoluteOffsets)
			tempTablesIdsOff += subDataOff;
		while (CharArrayToShort(&proj[tempTablesIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempTablesIdsOff]));
			tempTablesIdsOff += 4;
		}

		//fprintf(outProj, "\nKeymap Ids\n");
		unsigned long tempKeymapsIdsOff = keymapIdsOff;
		if (!absoluteOffsets)
			tempKeymapsIdsOff += subDataOff;

		while (CharArrayToShort(&proj[tempKeymapsIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempKeymapsIdsOff]));
			tempKeymapsIdsOff += 4;
		}

		//fprintf(outProj, "\nLayers Macros Ids\n");
		unsigned long tempLayersIdsOff = layerIdsOff;
		if (!absoluteOffsets)
			tempLayersIdsOff += subDataOff;

		while (CharArrayToShort(&proj[tempLayersIdsOff]) != 0xFFFF)
		{
			//fprintf(outProj, "%04X ", CharArrayToShort(&proj[tempLayersIdsOff]));
			tempLayersIdsOff += 4;
		}

		//fprintf(outProj, "\n");

		if (absoluteOffsets)
			groupOffset = groupEndOff;
		else
			groupOffset += groupEndOff;
		groupCounter++;
	}





	delete [] proj;
	delete [] pool;
	return true;
}

float CN64SoundbankToolDlg::Round(float value)
{
   return (value < 0.0f ? ceil((value)-0.5f) : floor((value)+0.5f));
}

bool CN64SoundbankToolDlg::WriteDLSCombinedPaperMario(CString pathNameStr, std::vector<ALBank*> alBanks, std::vector<int> soundBankIndexes, unsigned char* buffer, int romSize, unsigned long percussionOffset, std::vector<int> instrumentIndex, std::vector<int> instrumentOffset, FILE* outFileDebug)
{
	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(FilterGameName(gameConfig[x].gameName)))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	SoundGameConfig gameConfigSoundFound;
	for (int y = 0; y < countGamesSound; y++)
	{
		if (FilterGameName(gameConfigSound[y].gameName) == FilterGameName(gameConfigBank.gameName))
		{
			gameConfigSoundFound = gameConfigSound[y];
			break;
		}
	}

	MidiGameConfig gameConfigMidiFound;
	for (int y = 0; y < countGamesMidi; y++)
	{
		if (FilterGameName(gameConfigMidi[y].gameName).CompareNoCase(gameName) == 0)
		{
			gameConfigMidiFound = gameConfigMidi[y];
			break;
		}
	}

	fprintfIfDebug(outFileDebug, "Soundbank Game %s\n", gameConfigBank.gameName);
	fprintfIfDebug(outFileDebug, "Sound Type %s\n", gameConfigSoundFound.gameType);
	fprintfIfDebug(outFileDebug, "Midi Type %s\n", gameConfigMidiFound.gameType);

	CString tempStr;
	mTimeMultiplier.GetWindowText(tempStr);

	float timeMultiplier = atof(tempStr);


	for (int m = 0; m < gameConfigMidiFound.numberMidis; m++)
	{
		fprintfIfDebug(outFileDebug, "Midi Soundbank %04X\n", m);

		unsigned char* inputMID = &buffer[gameConfigMidiFound.midiBanks[m].start];

		int midSize = gameConfigMidiFound.midiBanks[m].end - gameConfigMidiFound.midiBanks[m].start;

		int numberInstruments;
		std::vector<SngInstrumentPaperMario> usedInstruments;
		std::vector<SngDrumPaperMario> usedPercussion;
		std::vector<int> usedExtraInstruments;

		midiParse.PaperMarioToMidi(buffer, romSize, inputMID, midSize, "temp123qaAS3.mid", numberInstruments, false, false, usedInstruments, usedPercussion, usedExtraInstruments, false);
		::DeleteFile("temp123qaAS3.mid");

		int numberSegments = inputMID[0x10];

		//for (int s = 0; s < numberSegments; s++)
		{
			DLSFile file("Soundfont Generated by N64 Soundbank Tool (GoldenEye Vault)");




			std::vector<int> soundPositionsByIndex;
			soundPositionsByIndex.resize(instrumentIndex.size());

			std::vector<int> usedInstrumentActual;
			usedInstrumentActual.resize(usedInstruments.size());
			for (int x = 0; x < usedInstruments.size(); x++)
			{
				usedInstrumentActual[x] = usedInstruments[x].instrument;
			}

			std::vector<int> usedPercussionActual;
			usedPercussionActual.resize(usedPercussion.size());
			for (int x = 0; x < usedPercussion.size(); x++)
			{
				usedPercussionActual[x] = buffer[percussionOffset + 0x10 + (usedPercussion[x].instrument * 0xC) + 1];
			}

			int paperMarioSoundIndex = 0;

			for (int x = 0; x < soundPositionsByIndex.size(); x++)
			{
				soundPositionsByIndex[x] = -1;

				if (
					(std::find(usedInstrumentActual.begin(), usedInstrumentActual.end(), x) == usedInstrumentActual.end())
					&& (std::find(usedPercussionActual.begin(), usedPercussionActual.end(), x) == usedPercussionActual.end())
					&& (std::find(usedExtraInstruments.begin(), usedExtraInstruments.end(), x) == usedExtraInstruments.end())
					)
				{
					// Skip unused
					continue;
				}

				if (instrumentIndex[x] != -1)
				{
					fprintfIfDebug(outFileDebug, "\nInstrument Index %04X\n", instrumentIndex[x]);

					ALBank* drumBank = alBanks[instrumentIndex[x]];
					int drumInstrumentOffset = instrumentOffset[x];

					int y = 0;

					{
						CString outputWavFile;
						outputWavFile.Format("%stempABC%02X_%04X.bin", mainFolder, instrumentIndex[x], instrumentOffset[x]);
						if (!n64audio.ExtractRawPCMData(mainFolder, drumBank, drumInstrumentOffset, y, outputWavFile, PRIMARY))
						{
							//MessageBox("Could not do soundbank, error extracting a sound");
							continue;
						}

						int sizePcm = GetSizeFile(outputWavFile);

						FILE* inPcmData = fopen(outputWavFile, "rb");
						if (inPcmData == NULL)
						{
							MessageBox("Could not do soundbank, error reading a written pcm sound");
							return false;
						}

						unsigned char* pcmData = new unsigned char[sizePcm];
						fread(pcmData, 1, sizePcm, inPcmData);
						fclose(inPcmData);

						float samplingRate = (float)drumBank->samplerate;

						samplingRate = drumBank->inst[drumInstrumentOffset]->sounds[y]->wav.sampleRateNotInDefaultNintendoSpec;

						mSamplingRate.GetWindowText(tempStr);
						samplingRate = atoi(tempStr);

						if (mHalfSamplingRate.GetCheck())
							samplingRate /= 2.0;

						fprintfIfDebug(outFileDebug, "Sampling Rate %f\n", samplingRate);

						tempStr.Format("Instr_%02X_Smp_%02X", instrumentIndex[x], instrumentOffset[x]);

						DLSWave* wave = file.AddWave(0x0001, 1, samplingRate, (16 / 8) * samplingRate * 1, (samplingRate / 8) * 1, 16, sizePcm, pcmData, CStringToString(tempStr));
						
						::DeleteFile(outputWavFile);

						soundPositionsByIndex[x] = paperMarioSoundIndex++;
					}
				}
			}





			int overallPaperMarioCount = 0;


			for (int writeBank = 0; writeBank < alBanks.size(); writeBank++)
			{
				fprintfIfDebug(outFileDebug, "\nWrite Bank %04X\n", writeBank);
				int soundBankStartSpot = 0;

				ALBank* alBankCurrent = alBanks[writeBank];
				int soundBankSelected = soundBankIndexes[writeBank];
				
				if (alBankCurrent != NULL)
				{
					for (int x = 0; x < alBankCurrent->count; x++)
					{
						if (std::find(usedExtraInstruments.begin(), usedExtraInstruments.end(), overallPaperMarioCount) == usedExtraInstruments.end())
						{
							overallPaperMarioCount++;
							continue;
						}

						fprintfIfDebug(outFileDebug, "\nInstrument %04X\n", x);

						int bankNumber = (overallPaperMarioCount / 0x80) + (10 * 0x100);

						tempStr.Format("MainInstr_%02X", overallPaperMarioCount);

						if (x < gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames.size())
						{
							tempStr = gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames[x];
						}

						if (std::find(gameConfigBank.skipInstruments.begin(), gameConfigBank.skipInstruments.end(), x) != gameConfigBank.skipInstruments.end())
							continue;

						int instrumentNumber = (overallPaperMarioCount % 0x80);

						DLSInstr* instr = file.AddInstr(bankNumber, instrumentNumber, CStringToString(tempStr));

						DLSRgn* region = instr->AddRgn();
						DLSArt* art = region->AddArt();

						long tcAttack = 0;
						long ptSustain = 0;
						long tcRelease = 0;
						long tcDecay = 0;

						signed long pan = 0;

						char cSampleLoops = 0;
						ULONG ulLoopStart = 0;
						ULONG ulLoopLength = 0;
						ULONG ulLoopType = 0;

						if ((alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave != NULL)
								&& (alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop != NULL))
						{
							if (alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->count != 0)
							{
								cSampleLoops = 1;
								ulLoopStart = alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->start;
								ulLoopLength = (alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->end - alBankCurrent->inst[x]->sounds[0]->wav.adpcmWave->loop->start);
								ulLoopType = 0; // FORWARD

								fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
							}
						}

						DLSWsmp* wsmp = region->AddWsmp();
						//DLSWsmp* wsmp = new DLSWsmp();
						//wave->Wsmp = wsmp;

						long volumeAttenuation = 0;
						unsigned char keyBase = 0x3C;

						unsigned short noteCenter = (alBankCurrent->inst[x]->sounds[0]->wav.unknown1 & 0xFFFF) / 0x64;
						unsigned short fineTuneBank = (alBankCurrent->inst[x]->sounds[0]->wav.unknown1 & 0xFFFF) % 0x64;

						keyBase = noteCenter + 0xC;

						signed short fineTune = -fineTuneBank;

						fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
						wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
						wsmp->cSampleLoops = cSampleLoops;
						wsmp->ulLoopType = ulLoopType;
						wsmp->ulLoopStart = ulLoopStart;
						wsmp->ulLoopLength = ulLoopLength;

						
						region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, soundPositionsByIndex[overallPaperMarioCount]);

						unsigned short usKeyHigh = 0x7F;
						unsigned short usKeyLow = 0x00;
						unsigned short usVelHigh = 0x7F;
						unsigned short usVelLow = 0x00;

						region->usKeyHigh = usKeyHigh;
						region->usKeyLow = usKeyLow;

						region->usVelHigh = usVelHigh;
						region->usVelLow = usVelLow;

						fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);

						overallPaperMarioCount++;
					}
				}
				else
				{
					if (std::find(usedExtraInstruments.begin(), usedExtraInstruments.end(), overallPaperMarioCount) == usedExtraInstruments.end())
					{
						overallPaperMarioCount++;
						continue;
					}

					int x = overallPaperMarioCount;

					int bankNumber = (overallPaperMarioCount / 0x80) + (10 * 0x100);

					tempStr.Format("MainInstr_%02X", overallPaperMarioCount);
					
					if (x < gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames.size())
					{
						tempStr = gameConfigSoundFound.soundBanks[soundBankSelected].instrumentNames[x];
					}

					if (std::find(gameConfigBank.skipInstruments.begin(), gameConfigBank.skipInstruments.end(), x) != gameConfigBank.skipInstruments.end())
						continue;

					int instrumentNumber = (overallPaperMarioCount % 0x80);

					DLSInstr* instr = file.AddInstr(bankNumber, instrumentNumber, CStringToString(tempStr));

					for (int r = 0; r < 0x48; r++)
					{
						fprintfIfDebug(outFileDebug, "Drum %04X\n", r);

						unsigned long percussionSpecificOffset = percussionOffset + 0x10 + (r * 0xC);

						unsigned char drumInstrumentFlag = buffer[percussionSpecificOffset];
						unsigned char drumInstrumentNumber = buffer[percussionSpecificOffset+1];
						unsigned char drumCoarseTune = buffer[percussionSpecificOffset+2];
						unsigned char drumFineTune = buffer[percussionSpecificOffset+3];
						unsigned char drumVolume = buffer[percussionSpecificOffset+4];
						unsigned char drumPan = buffer[percussionSpecificOffset+5];
						unsigned char drumReverb = buffer[percussionSpecificOffset+6];

						ALBank* drumBank = alBanks[instrumentIndex[drumInstrumentNumber]];
						int drumInstrumentOffset = instrumentOffset[drumInstrumentNumber];

						DLSRgn* region = instr->AddRgn();
						DLSArt* art = region->AddArt();

						signed long pan = 0;

						unsigned char samplePan = drumPan & 0x7F;
						if (samplePan == 0x40)
							pan = 0;
						else if (samplePan == 0x7F)
							pan = 0x1F40000;
						else if (samplePan == 0x00)
							pan = -0x1F40000;
						else
						{
							float panFloat = (float)samplePan / (float)0x7F;
							panFloat = (panFloat - 0.5f) * 2.0f;
							pan = (panFloat * (float)0x1F40000);
						}

						if (pan != 0)
						{
							art->AddPan(pan);
							fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
						}

						char cSampleLoops = 0;
						ULONG ulLoopStart = 0;
						ULONG ulLoopLength = 0;
						ULONG ulLoopType = 0;


						if ((drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave != NULL)
								&& (drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop != NULL))
						{
							if (drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->count != 0)
							{
								cSampleLoops = 1;
								ulLoopStart = drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->start;
								ulLoopLength = (drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->end - drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->start);
								ulLoopType = 0; // FORWARD

								fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
							}
						}


						DLSWsmp* wsmp = region->AddWsmp();
						//DLSWsmp* wsmp = new DLSWsmp();
						//wave->Wsmp = wsmp;

						long volumeAttenuation = 0;
						unsigned char keyBase = r + 0xC;
						signed short fineTune = 0;

						keyBase -= (signed char)drumCoarseTune;
						fineTune = (signed char)drumFineTune;

						fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
						wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
						wsmp->cSampleLoops = cSampleLoops;
						wsmp->ulLoopType = ulLoopType;
						wsmp->ulLoopStart = ulLoopStart;
						wsmp->ulLoopLength = ulLoopLength;

						
						region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, soundPositionsByIndex[drumInstrumentNumber]);

						unsigned short usKeyHigh = r + 0xC;
						unsigned short usKeyLow = r + 0xC;
						unsigned short usVelHigh = 0x7F;
						unsigned short usVelLow = 0x00;

						
						region->usKeyHigh = usKeyHigh;
						region->usKeyLow = usKeyLow;

						region->usVelHigh = usVelHigh;
						region->usVelLow = usVelLow;

						fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);
					}

					overallPaperMarioCount++;
				}
			}

			//unsigned long segmentDataPointer = CharArrayToShort(&inputMID[0x14 + (s * 2)]) << 2;

			//if (segmentDataPointer == 0x0000)
				//continue;

			unsigned long instrumentDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (2 * 2)]) << 2;
			int numberInst = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (3 * 2)]);

			int bankNumber = 0;

			for (int r = 0; r < numberInst; r++)
			{
				fprintfIfDebug(outFileDebug, "\nInstrument %04X\n", r);

				unsigned long instrumentSpecificOffset = instrumentDataPointer + (r * 0x8);

				unsigned char instrumentInstrumentFlag = inputMID[instrumentSpecificOffset];
				unsigned char instrumentInstrumentNumber = inputMID[instrumentSpecificOffset+1];
				unsigned char instrumentVolume = inputMID[instrumentSpecificOffset+2];
				unsigned char instrumentPan = inputMID[instrumentSpecificOffset+3];
				unsigned char instrumentReverb = inputMID[instrumentSpecificOffset+4];
				unsigned char instrumentCoarseTune = inputMID[instrumentSpecificOffset+5];
				unsigned char instrumentFineTune = inputMID[instrumentSpecificOffset+6];

				ALBank* instrumentBank = alBanks[instrumentIndex[instrumentInstrumentNumber]];
				int instrumentInstrumentOffset = instrumentOffset[instrumentInstrumentNumber];

				int instrumentNumber = (r % 0x80);

				tempStr.Format("Instr_%02X", r);
				DLSInstr* instr = file.AddInstr(bankNumber, instrumentNumber, CStringToString(tempStr));

				DLSRgn* region = instr->AddRgn();
				DLSArt* art = region->AddArt();

				signed long pan = 0;

				unsigned char samplePan = instrumentPan & 0x7F;
				if (samplePan == 0x40)
					pan = 0;
				else if (samplePan == 0x7F)
					pan = 0x1F40000;
				else if (samplePan == 0x00)
					pan = -0x1F40000;
				else
				{
					float panFloat = (float)samplePan / (float)0x7F;
					panFloat = (panFloat - 0.5f) * 2.0f;
					pan = (panFloat * (float)0x1F40000);
				}

				if (pan != 0)
				{
					art->AddPan(pan);
					fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
				}

				char cSampleLoops = 0;
				ULONG ulLoopStart = 0;
				ULONG ulLoopLength = 0;
				ULONG ulLoopType = 0;

				if ((instrumentBank->inst[instrumentInstrumentOffset]->sounds[0]->wav.adpcmWave != NULL)
								&& (instrumentBank->inst[instrumentInstrumentOffset]->sounds[0]->wav.adpcmWave->loop != NULL))
				{
					if (instrumentBank->inst[instrumentInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->count != 0)
					{
						cSampleLoops = 1;
						ulLoopStart = instrumentBank->inst[instrumentInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->start;
						ulLoopLength = (instrumentBank->inst[instrumentInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->end - instrumentBank->inst[instrumentInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->start);
						ulLoopType = 0; // FORWARD

						fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
					}
				}


				DLSWsmp* wsmp = region->AddWsmp();
				//DLSWsmp* wsmp = new DLSWsmp();
				//wave->Wsmp = wsmp;

				long volumeAttenuation = 0;
				unsigned char keyBase = 0x3C;
				signed short fineTune = 0;

				keyBase -= (signed char)instrumentCoarseTune;
				fineTune = (signed char)instrumentFineTune;

				fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
				wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
				wsmp->cSampleLoops = cSampleLoops;
				wsmp->ulLoopType = ulLoopType;
				wsmp->ulLoopStart = ulLoopStart;
				wsmp->ulLoopLength = ulLoopLength;

				
				region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, soundPositionsByIndex[instrumentInstrumentNumber]);

				unsigned short usKeyHigh = 0x7F;
				unsigned short usKeyLow = 0;
				unsigned short usVelHigh = 0x7F;
				unsigned short usVelLow = 0x00;

				
				region->usKeyHigh = usKeyHigh;
				region->usKeyLow = usKeyLow;

				region->usVelHigh = usVelHigh;
				region->usVelLow = usVelLow;

				fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);
			}

			unsigned long drumDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (0 * 2)]) << 2;
			int numberDrum = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (1 * 2)]);
		
			bankNumber = 1;

			for (int x = 0; x < numberDrum; x++)
			{
				fprintfIfDebug(outFileDebug, "\nDrum %04X\n", x);

				unsigned long percussionSpecificOffset = drumDataPointer + (x * 0xC);
				//fprintf(outFile, "Drum #%02X: Bank Start %02X Instrument %02X Coarse Tune %02X Fine Tune %02X Volume %02X Pan %02X Reverb %02X %02X %02X %02X %02X %02X\n", x, inputMID[tempOffset+0], inputMID[tempOffset+1], inputMID[tempOffset+2], inputMID[tempOffset+3], inputMID[tempOffset+4], inputMID[tempOffset+5], inputMID[tempOffset+6], inputMID[tempOffset+7], inputMID[tempOffset+8], inputMID[tempOffset+9], inputMID[tempOffset+0xA], inputMID[tempOffset+0xB]);

				unsigned char drumInstrumentFlag = inputMID[percussionSpecificOffset];
				unsigned char drumInstrumentNumber = inputMID[percussionSpecificOffset+1];
				unsigned char drumCoarseTune = inputMID[percussionSpecificOffset+2];
				unsigned char drumFineTune = inputMID[percussionSpecificOffset+3];
				unsigned char drumVolume = inputMID[percussionSpecificOffset+4];
				unsigned char drumPan = inputMID[percussionSpecificOffset+5];
				unsigned char drumReverb = inputMID[percussionSpecificOffset+6];

				ALBank* drumBank = alBanks[instrumentIndex[drumInstrumentNumber]];
				int drumInstrumentOffset = instrumentOffset[drumInstrumentNumber];

				tempStr.Format("Drum_%02X", x);
				DLSInstr* instr = file.AddInstr(bankNumber, x, CStringToString(tempStr));

				DLSRgn* region = instr->AddRgn();
				DLSArt* art = region->AddArt();

				signed long pan = 0;

				unsigned char samplePan = drumPan & 0x7F;
				if (samplePan == 0x40)
					pan = 0;
				else if (samplePan == 0x7F)
					pan = 0x1F40000;
				else if (samplePan == 0x00)
					pan = -0x1F40000;
				else
				{
					float panFloat = (float)samplePan / (float)0x7F;
					panFloat = (panFloat - 0.5f) * 2.0f;
					pan = (panFloat * (float)0x1F40000);
				}

				if (pan != 0)
				{
					art->AddPan(pan);
					fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
				}

				char cSampleLoops = 0;
				ULONG ulLoopStart = 0;
				ULONG ulLoopLength = 0;
				ULONG ulLoopType = 0;

				if ((drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave != NULL)
								&& (drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop != NULL))
				{
					if (drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->count != 0)
					{
						cSampleLoops = 1;
						ulLoopStart = drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->start;
						ulLoopLength = (drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->end - drumBank->inst[drumInstrumentOffset]->sounds[0]->wav.adpcmWave->loop->start);
						ulLoopType = 0; // FORWARD

						fprintfIfDebug(outFileDebug, "Loop Count %08X Start %08X Length %08X\n", cSampleLoops, ulLoopStart, ulLoopLength);
					}
				}

				DLSWsmp* wsmp = region->AddWsmp();
				//DLSWsmp* wsmp = new DLSWsmp();
				//wave->Wsmp = wsmp;

				long volumeAttenuation = 0;
				unsigned char keyBase = 0x3C;
				signed short fineTune = 0;

				keyBase -= (signed char)drumCoarseTune;
				fineTune = (signed char)drumFineTune;

				fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
				wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
				wsmp->cSampleLoops = cSampleLoops;
				wsmp->ulLoopType = ulLoopType;
				wsmp->ulLoopStart = ulLoopStart;
				wsmp->ulLoopLength = ulLoopLength;

				
				region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, soundPositionsByIndex[drumInstrumentNumber]);

				unsigned short usKeyHigh = 0x7F;
				unsigned short usKeyLow = 0x00;
				unsigned short usVelHigh = 0x7F;
				unsigned short usVelLow = 0x00;

				
				region->usKeyHigh = usKeyHigh;
				region->usKeyLow = usKeyLow;

				region->usVelHigh = usVelHigh;
				region->usVelLow = usVelLow;

				fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);
			}

			bankNumber = 2;

			tempStr.Format("DrumKit_%02X", 0);
			DLSInstr* instrDrum = file.AddInstr(bankNumber, 0, CStringToString(tempStr));

			for (int r = 0; r < 0x48; r++)
			{
				bool foundDrum = false;
				for (int z = 0; z < usedPercussion.size(); z++)
				{
					if (usedPercussion[z].instrument == r)
					{
						foundDrum = true;
						break;
					}
				}
				if (!foundDrum)
				{
					continue;
				}

				fprintfIfDebug(outFileDebug, "\nDrum %04X\n", r);

				unsigned long percussionSpecificOffset = percussionOffset + 0x10 + (r * 0xC);

				unsigned char drumInstrumentFlag = buffer[percussionSpecificOffset];
				unsigned char drumInstrumentNumber = buffer[percussionSpecificOffset+1];
				unsigned char drumCoarseTune = buffer[percussionSpecificOffset+2];
				unsigned char drumFineTune = buffer[percussionSpecificOffset+3];
				unsigned char drumVolume = buffer[percussionSpecificOffset+4];
				unsigned char drumPan = buffer[percussionSpecificOffset+5];
				unsigned char drumReverb = buffer[percussionSpecificOffset+6];

				ALBank* drumBank = alBanks[instrumentIndex[drumInstrumentNumber]];
				int drumInstrumentOffset = instrumentOffset[drumInstrumentNumber];

				DLSRgn* region = instrDrum->AddRgn();
				DLSArt* art = region->AddArt();

				signed long pan = 0;

				unsigned char samplePan = drumPan & 0x7F;
				if (samplePan == 0x40)
					pan = 0;
				else if (samplePan == 0x7F)
					pan = 0x1F40000;
				else if (samplePan == 0x00)
					pan = -0x1F40000;
				else
				{
					float panFloat = (float)samplePan / (float)0x7F;
					panFloat = (panFloat - 0.5f) * 2.0f;
					pan = (panFloat * (float)0x1F40000);
				}

				if (pan != 0)
				{
					art->AddPan(pan);
					fprintfIfDebug(outFileDebug, "Pan %08X\n", pan);
				}

				char cSampleLoops = 0;
				ULONG ulLoopStart = 0;
				ULONG ulLoopLength = 0;
				ULONG ulLoopType = 0;


				DLSWsmp* wsmp = region->AddWsmp();
				//DLSWsmp* wsmp = new DLSWsmp();
				//wave->Wsmp = wsmp;

				long volumeAttenuation = 0;
				unsigned char keyBase = r + 0xC;
				signed short fineTune = 0;

				//keyBase -= (signed char)drumCoarseTune;
				//fineTune = (signed char)drumFineTune;

				fprintfIfDebug(outFileDebug, "Key Base %02X Fine Tune %04X Volume Attenuation %08X\n", keyBase, fineTune, volumeAttenuation);
				wsmp->SetPitchInfo(keyBase, fineTune, volumeAttenuation);
				wsmp->cSampleLoops = cSampleLoops;
				wsmp->ulLoopType = ulLoopType;
				wsmp->ulLoopStart = ulLoopStart;
				wsmp->ulLoopLength = ulLoopLength;

				
				region->SetWaveLinkInfo(F_RGN_OPTION_SELFNONEXCLUSIVE, 0, 1, soundPositionsByIndex[drumInstrumentNumber]);

				unsigned short usKeyHigh = r + 0xC;
				unsigned short usKeyLow = r + 0xC;
				unsigned short usVelHigh = 0x7F;
				unsigned short usVelLow = 0x00;

				
				region->usKeyHigh = usKeyHigh;
				region->usKeyLow = usKeyLow;

				region->usVelHigh = usVelHigh;
				region->usVelLow = usVelLow;

				fprintfIfDebug(outFileDebug, "Key Low %02X Key High %02X Vel Low %02X Vel High %02X\n", usKeyLow, usKeyHigh, usVelLow, usVelHigh);
			}


			CString pathNameStrSub = pathNameStr;
			pathNameStrSub.Replace(".dls", "");
			CString tempStr;
			if (m < gameConfigMidiFound.numberMidiNames)
			{
				tempStr.Format("_%02X_%s.dls", m, gameConfigMidiFound.midiNames[m]);
			}
			else
			{
				tempStr.Format("_%02X.dls", m);
			}
			pathNameStrSub += tempStr;
			pathNameStrSub.Trim();
			pathNameStrSub.Replace("\n", "");
			pathNameStrSub.Replace("\r", "");
			pathNameStrSub.Replace("*", "");
			pathNameStrSub.Replace("?", "");
			pathNameStrSub.Replace("<", "");
			pathNameStrSub.Replace(">", "");
			pathNameStrSub.Replace("|", "");

			bool saved = file.SaveDLSFile(pathNameStrSub);
			if (!saved)
			{
				MessageBox("Can't save file, bad filename " + pathNameStrSub);
				return false;
			}
		}
	}

	return true;
}

void CN64SoundbankToolDlg::OnBnClickedButtonwritedlssoundfont()
{
	if (mSoundBankNumber.GetCurSel() == -1)
		return;

	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	int gameNumber = -1;
	for (int x = 0; x < countGamesMidi; x++)
	{
		if (FilterGameName(gameConfigMidi[x].gameName).CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (gameNumber == -1)
		return;

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	CString tempStr;
	mSoundBankNumber.GetWindowText(tempStr);

	if (tempStr.Find(" ") != -1)
	{
		tempStr = tempStr.Mid(0, tempStr.Find(" "));
	}

	int soundBankSelected = CN64SoundToolReader::StringHexToLong(tempStr);

	ALBank* alBankCurrent = NULL;
	if (soundBankSelected >= alBankList.size())
	{
		return;
	}

	alBankCurrent = alBankList[soundBankSelected];

	if (alBankCurrent != NULL)
	{
		CFileDialog m_svFile(FALSE, "dls", "ExtractedSoundbank.dls", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "DLS Soundfont (*.dls)|*.dls|");

		int statusFileOpen = (int) m_svFile.DoModal();
		CString fileName = m_svFile.GetFileName();

		if ((statusFileOpen == IDOK) && (fileName != ""))
		{
			CString tempStr;
			mSoundBankNumber.GetWindowText(tempStr);

			if (tempStr.Find(" ") != -1)
			{
				tempStr = tempStr.Mid(0, tempStr.Find(" "));
			}

			int soundBankSelected = CN64SoundToolReader::StringHexToLong(tempStr);

			std::vector<ALBank*> alBanks;
			if (gameConfigMidi[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
			{

			}
			else
			{
				alBanks.push_back(alBankCurrent);
			}

			std::vector<int> bankIndexes;
			if (gameConfigMidi[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
			{

			}
			else
			{
				bankIndexes.push_back(soundBankSelected);
			}

			std::vector<int> instrumentIndex;
			std::vector<int> instrumentOffset;

			if (gameConfigMidi[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
			{
				/*CString name = alBankCurrent->bankName;

				std::vector<CString> orderInRAM;
				orderInRAM.push_back("MAR0CR");
				orderInRAM.push_back("MAR1CR");
				orderInRAM.push_back("MAR2CR");
				orderInRAM.push_back("MAR3CR");
				orderInRAM.push_back("MAR4CR");
				orderInRAM.push_back("PAR0CR");
				orderInRAM.push_back("PAR1CR");
				orderInRAM.push_back("PAR2CR");
				orderInRAM.push_back("BTL0CR");
				orderInRAM.push_back("BTL1CR");
				orderInRAM.push_back("BTL2CR");
				orderInRAM.push_back("ENV0CR");
				orderInRAM.push_back("ENV1CR");
				orderInRAM.push_back("BTL20CR");
				orderInRAM.push_back("BTL21CR");
				orderInRAM.push_back("BTL22CR");
				orderInRAM.push_back("BTL23CR");
				orderInRAM.push_back("BTL24CR");
				orderInRAM.push_back("BTL25CR");
				orderInRAM.push_back("BTL26CR");
				orderInRAM.push_back("BTL27CR");
				orderInRAM.push_back("BTL28CR");
				orderInRAM.push_back("EVT0CR");
				orderInRAM.push_back("EVT0CR");
				orderInRAM.push_back("EVT1CR");
				orderInRAM.push_back("EVT2CR");
				orderInRAM.push_back("EVT3CR");
				orderInRAM.push_back("EVT4CR");
				orderInRAM.push_back("EVT5CR");
				orderInRAM.push_back("EVT6CR");
				orderInRAM.push_back("ENE0CR");
				orderInRAM.push_back("ENE1CR");
				orderInRAM.push_back("SYS0CR");
				orderInRAM.push_back("SYS1CR");
				orderInRAM.push_back("SYS2CR");
				orderInRAM.push_back("PEA0CR");
				orderInRAM.push_back("ETC0CR");
				orderInRAM.push_back("ETC1CR");
				orderInRAM.push_back("ETC2CR");
				orderInRAM.push_back("MOB0CR");
				orderInRAM.push_back("BT29CR");
				orderInRAM.push_back("BT2ACR");
				orderInRAM.push_back("BT2BCR");
				orderInRAM.push_back("PHR1CR");
				orderInRAM.push_back("EVT7CR");
				orderInRAM.push_back("EVT8CR");
				orderInRAM.push_back("BT2CCR");
				orderInRAM.push_back("BT2DCR");
				orderInRAM.push_back("GM01CR");
				orderInRAM.push_back("GM02CR");
				orderInRAM.push_back("GM03CR");
				orderInRAM.push_back("GM04CR");
				orderInRAM.push_back("GM05CR");
				orderInRAM.push_back("GM06CR");
				orderInRAM.push_back("GM07CR");
				orderInRAM.push_back("GM08CR");
				orderInRAM.push_back("GM09CR");
				orderInRAM.push_back("GM10CR");
				orderInRAM.push_back("GM11CR");
				orderInRAM.push_back("PS01CR");
				orderInRAM.push_back("PS02CR");
				orderInRAM.push_back("PS03CR");
				orderInRAM.push_back("PS04CR");
				orderInRAM.push_back("PS05CR");*/

				int startCombo = 0;

				for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
				{
					int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

					
					if (soundBankSelected >= alBankList.size())
					{
						return;
					}

					if (alBankList[soundBankSelected]->bankName.Find("GM") == 0)
					{
						startCombo = x;
						break;
					}
				}

				CString name = alBankList[gameConfigBank.soundfontBank[startCombo].soundBankNumber]->bankName;
				CString firstTwoName = name.Mid(0,2);

				for (int x = startCombo; x < gameConfigBank.soundfontBank.size(); x++)
				{
					int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

					
					if (soundBankSelected >= alBankList.size())
					{
						return;
					}

					if (alBankList[soundBankSelected]->bankName.Find(firstTwoName) != 0)
					{
						if (firstTwoName == "GM")
						{
							// Placeholders for drum splits
							alBanks.push_back(NULL); //A6
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //A7
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //A8
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //A9
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //AA
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //AB
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //AC
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //AD
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //AE
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							alBanks.push_back(NULL); //AF
							bankIndexes.push_back(0);
							instrumentIndex.push_back(-1);
							instrumentOffset.push_back(-1);

							firstTwoName = "PS";
							x = -1;
							continue;
						}
						else
						{
							break;
						}
					}

					for (int r = 0; r < alBankList[soundBankSelected]->count; r++)
					{
						instrumentIndex.push_back(alBanks.size());
						instrumentOffset.push_back(r);
					}

					alBanks.push_back(alBankList[soundBankSelected]);

					bankIndexes.push_back(x);
				}

				FILE* outDebugFile = NULL;
				if (mDebug.GetCheck())
					outDebugFile = fopen(m_svFile.GetPathName() + "Debug.txt", "w");
				WriteDLSGMBankPaperMario(m_svFile.GetPathName(), alBanks, bankIndexes, instrumentIndex, instrumentOffset, outDebugFile);
				if (outDebugFile)
				{
					fflush(outDebugFile);
					fclose(outDebugFile);
				}
			}
			else
			{
				FILE* outDebugFile = NULL;
				if (mDebug.GetCheck())
					outDebugFile = fopen(m_svFile.GetPathName() + "Debug.txt", "w");
				WriteDLS(m_svFile.GetPathName(), alBanks, bankIndexes, false, outDebugFile);
				if (outDebugFile)
				{
					fflush(outDebugFile);
					fclose(outDebugFile);
				}
			}
		}
	}
}

void CN64SoundbankToolDlg::OnBnClickedButtonpreviewmidi()
{
	if (mSoundBankNumber.GetCurSel() == -1)
		return;

	CString tempStr;
	mSoundBankNumber.GetWindowText(tempStr);

	if (tempStr.Find(" ") != -1)
	{
		tempStr = tempStr.Mid(0, tempStr.Find(" "));
	}

	int soundBankSelected = CN64SoundToolReader::StringHexToLong(tempStr);

	ALBank* alBankCurrent = NULL;
	if (soundBankSelected >= alBankList.size())
	{
		return;
	}

	alBankCurrent = alBankList[soundBankSelected];

	if (mMidi.GetCurSel() == -1)
		return;

	if (buffer == NULL)
		return;

	if (mMidi.GetCount() == 0)
		return;

	CString spotStr;
	mMidi.GetWindowText(spotStr);

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
	gameName.Trim();

	int gameNumber = -1;
	for (int x = 0; x < countGamesMidi; x++)
	{
		if (FilterGameName(gameConfigMidi[x].gameName).CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (gameNumber == -1)
		return;

	mTimeMultiplier.GetWindowText(tempStr);

	float timeMultiplier = atof(tempStr);

	mSamplingRate.GetWindowText(tempStr);

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	bool combineBanks = false;
	std::vector<ALBank*> alBanks;
	if (mMergePreviewBanksCheck.GetCheck() && (gameConfigMidi[gameNumber].gameType.CompareNoCase("Konami") == 0))
	{
		for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
		{
			int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

			ALBank* alBankCurrent = NULL;
			if (soundBankSelected >= alBankList.size())
			{
				return;
			}

			alBanks.push_back(alBankList[soundBankSelected]);
		}

		combineBanks = true;
	}
	else if (mMergePreviewBanksCheck.GetCheck() && (gameConfigMidi[gameNumber].gameType.CompareNoCase("PaperMario") == 0))
	{
		if (mSoundBankNumber.GetCurSel() == -1)
			return;

		int startCombo = mSoundBankNumber.GetCurSel();

		for (int x = startCombo; x < gameConfigBank.soundfontBank.size(); x++)
		{
			int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

			ALBank* alBankCurrent = NULL;
			if (soundBankSelected >= alBankList.size())
			{
				return;
			}

			alBanks.push_back(alBankList[soundBankSelected]);
		}

		combineBanks = true;
	}
	else
	{
		alBanks.push_back(alBankCurrent);
	}

	if (
		(gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5Zlb") == 0)
			|| (gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5ZlbGCStyle") == 0)
			|| (gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5LZGCStyle") == 0)
			|| (gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0)
			&& (mComboSongId.GetCurSel() != -1)
			)
	{
		CString tempSongIdStr;
		mComboSongId.GetWindowText(tempSongIdStr);

		int songId = CSharedFunctions::StringToUnsignedChar(tempSongIdStr);

		SoundGameConfig gameConfigSoundFound;
		for (int y = 0; y < countGamesSound; y++)
		{
			if (FilterGameName(gameConfigSound[y].gameName) == FilterGameName(gameConfigBank.gameName))
			{
				gameConfigSoundFound = gameConfigSound[y];
				break;
			}
		}

		midiPlayer->SetupMidiSoundBankFactor5(songId, buffer, romSize, gameConfigBank.projOffset, gameConfigBank.poolOffset, gameConfigBank.projSize, gameConfigBank.poolSize, alBankCurrent, timeMultiplier, mHalfSamplingRate.GetCheck(), false, atoi(tempStr), gameConfigSoundFound.gameType, gameConfigBank.absoluteOffsets);
	}
	else if (mUseT1SamplingRate.GetCheck())
		midiPlayer->SetupMidiSoundBank(alBanks, timeMultiplier, mHalfSamplingRate.GetCheck(), false, atoi(tempStr), gameConfigBank.skipInstruments, combineBanks, gameConfigMidi[gameNumber].gameType, mUseT1SamplingRate.GetCheck(), soundBankSelected, t1BankList);
	else
		midiPlayer->SetupMidiSoundBank(alBanks, timeMultiplier, mHalfSamplingRate.GetCheck(), true, atoi(tempStr), gameConfigBank.skipInstruments, combineBanks, gameConfigMidi[gameNumber].gameType, mUseT1SamplingRate.GetCheck(), soundBankSelected, t1BankList);

	// For some reason won't play dls generated by VGMTrans...but Synthfont does ok

	tempStr.Format("%stemp.dls", mainFolder);
	
	//if (!(WriteDLS(tempStr, alBankCurrent)))
		//return;
	
	//midiPlayer->LoadDLS(tempStr.GetBuffer(0));
	//tempStr.ReleaseBuffer(); 

	int numberInstruments = 0;

	if (gameConfigMidi[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		return;
	}
	else if (gameConfigMidi[gameNumber].gameType.CompareNoCase("DCM") == 0)
	{
		return;
	}
	else if (gameConfigMidi[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
	{
		return;
	}
	else
	{	bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;

		midiParse.ExportToMidi(gameConfigMidi[gameNumber].gameName, buffer, romSize, address, size, mainFolder + "tempAS123123as.mid", gameConfigMidi[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), false, extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfigMidi[gameNumber].extraGameMidiInfo, true);

		if (numberInstruments > alBankCurrent->count)
		{
			MessageBox("Warning, not enough instruments in bank, some may be defaulted to General Midi Soundfont");
		}

		midiPlayer->LoadMidi(mainFolder + "tempAS123123as.mid");

		if (mLoopPlayback.GetCheck() && hasLoopPoint)
		{
			midiPlayer->SetLoopSpot(loopStart, loopEnd);
			midiPlayer->SetRepeat(true);
		}
		else if (mLoopPlayback.GetCheck())
		{
			midiPlayer->SetLoopSpot(0, 0);
			midiPlayer->SetRepeat(true);
		}
		else
			midiPlayer->SetRepeat(false);
		midiPlayer->Play();
	}
}

void CN64SoundbankToolDlg::OnBnClickedButtonpreviewmidi2()
{
	midiPlayer->Stop();
}

void CN64SoundbankToolDlg::OnCbnSelchangeCombosound()
{
	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	CString tempStr;
	mSoundBankNumber.GetWindowText(tempStr);

	if (tempStr.Find(" ") != -1)
		tempStr = tempStr.Mid(0, tempStr.Find(" "));

	int soundBankSelected = CN64SoundToolReader::StringHexToLong(tempStr);

	SoundfontBank soundfontBank;

	for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
	{
		if (gameConfigBank.soundfontBank[x].soundBankNumber == soundBankSelected)
		{
			soundfontBank = gameConfigBank.soundfontBank[x];
			break;
		}
	}

	ALBank* alBankCurrent = NULL;
	if (soundBankSelected >= alBankList.size())
	{
		return;
	}

	alBankCurrent = alBankList[soundBankSelected];

	CString gameDescription = "Soundfont Status: ";
	if (
		(alBankCurrent->soundBankFormat == STANDARDFORMAT)
		)
	{
		bool found = false;
		if (t1BankList.size() > 0)
		{
			for (int b = 0; b < t1BankList.size(); b++)
			{
				if ((soundBankSelected >= t1BankList[b].bankStart) && (soundBankSelected < t1BankList[b].bankEnd))
				{
					mUseT1SamplingRate.ShowWindow(SW_SHOW);
					found = true;
				}
			}
		}

		if (!found)
		{
			mUseT1SamplingRate.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		mUseT1SamplingRate.ShowWindow(SW_HIDE);
	}

	if (
		(alBankCurrent->soundBankFormat == STANDARDFORMAT)
		|| (alBankCurrent->soundBankFormat == CONKERFORMAT)
		|| (alBankCurrent->soundBankFormat == STANDARDRNCCOMPRESSED)
		|| (alBankCurrent->soundBankFormat == STANDARDFORMATRAWALLOWED)
		|| (alBankCurrent->soundBankFormat == STANDARDRNXCOMPRESSED)
		|| (alBankCurrent->soundBankFormat == ZLIBSTANDARD)
		|| (alBankCurrent->soundBankFormat == BLASTCORPSZLBSTANDARD)
		|| (alBankCurrent->soundBankFormat == BANJOTOOIEAUDIO)
		|| (alBankCurrent->soundBankFormat == TUROKFORMAT)
		|| (alBankCurrent->soundBankFormat == MKMYTHOLOGIES)
		|| (alBankCurrent->soundBankFormat == TITUS)
		|| (alBankCurrent->soundBankFormat == NINDEC)
		)
	{
		gameDescription += "Full support, except for Tremolo/Vibrato";
	}
	else if ((alBankCurrent->soundBankFormat == KONAMICTL) || (alBankCurrent->soundBankFormat == KONAMI8CTL))
	{
		gameDescription += "No ADSR yet, coarse tune is adjust key base";
	}
	else if (
			(alBankCurrent->soundBankFormat == ZELDAFORMAT)
			)
	{
		gameDescription += "Valid keybase/splits, No detune, ADSR";
	}
	else if (
			(alBankCurrent->soundBankFormat == SUPERMARIO64FORMAT)
			|| (alBankCurrent->soundBankFormat == ZELDAFORMAT)
			|| (alBankCurrent->soundBankFormat == STARFOX64FORMAT)
			)
	{
		gameDescription += "Valid keybase/splits, No ADSR";
	}
	else if (
			(alBankCurrent->soundBankFormat == MUSYX)
			|| (alBankCurrent->soundBankFormat == MUSYXLZ)
			|| (alBankCurrent->soundBankFormat == MUSYXZLIB)
			|| (alBankCurrent->soundBankFormat == MUSYXSMALLZLIB)
			)
	{
		gameDescription += "Valid keybase";
	}
	else if (
		(alBankCurrent->soundBankFormat == N64DD)
		)
	{
		gameDescription += "Valid keybase and detune";
	}
	else
	{
		gameDescription += "Sound data only";
	}

	mGameDescription.SetWindowText(gameDescription);

	if (gameConfigBank.overrideSamplingRate)
	{
		tempStr.Format("%d", gameConfigBank.samplingRate);
		mSamplingRate.SetWindowText(tempStr);
	}
	else
	{
		tempStr.Format("%d", alBankCurrent->samplerate);
		mSamplingRate.SetWindowText(tempStr);
	}

	mMidi.ResetContent();
	for (int x = 0; x < countGamesMidi; x++)
	{
		if (gameConfigMidi[x].gameType.CompareNoCase("PaperMario") == 0)
		{
			mExtendSmallerTracksToEnd.SetWindowText("Combine Sub-segments");
		}
		else
		{
			mExtendSmallerTracksToEnd.SetWindowText("Extend Smaller Tracks to End");
		}

		if (FilterGameName(gameConfigMidi[x].gameName).CompareNoCase(FilterGameName(gameConfigBank.gameName)) == 0)
		{
			if ((gameConfigMidi[x].gameType.CompareNoCase("Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("HexenSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("BlitzSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("RNCSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("HexenSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ASMICSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("SnowSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ARISng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ArmySargeSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ArmySarge2Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("TazSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("FLA2Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("RNCSngOffset") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("LZSS_0BSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("AVL_0Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("VigilanteSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ViewpointSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("RugratsSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("MultiPartTigSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZLibSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZLibIndexedSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("MarioTennisSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("Yay0Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZipSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZTRK") == 0)
			)
			{
				mSeparateByInstrument.ShowWindow(SW_SHOW);
			}
			else if ((gameConfigMidi[x].gameType.CompareNoCase("Konami") == 0))
			{
				mSeparateByInstrument.ShowWindow(SW_SHOW);
			}
			else if (gameConfigMidi[x].gameType.CompareNoCase("PaperMario") == 0)
			{
				mSeparateByInstrument.ShowWindow(SW_SHOW);
			}
			else
			{
				mSeparateByInstrument.ShowWindow(SW_HIDE);
			}

			if (gameConfigMidi[x].gameType.CompareNoCase("Konami") == 0)
			{
				OnBnClickedCheckextendloopstohighesttrack();
				OnBnClickedCheckoutputloop();

				mOutputLoop.ShowWindow(SW_SHOW);
				mOutputLoopCount.ShowWindow(SW_SHOW);
				mExtendSmallerTracksToEnd.ShowWindow(SW_SHOW);

				mWriteDLSCombinedBanksButton.ShowWindow(SW_SHOW);
				mMergePreviewBanksCheck.ShowWindow(SW_SHOW);
			}
			else if (
				(gameConfigMidi[x].gameType.CompareNoCase("Factor5Zlb") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("Factor5ZlbGCStyle") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("Factor5LZGCStyle") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0)
				)
			{
				mOutputLoop.ShowWindow(SW_HIDE);
				mOutputLoopCount.ShowWindow(SW_HIDE);
				mExtendSmallerTracksToEnd.ShowWindow(SW_SHOW);

				mWriteDLSCombinedBanksButton.ShowWindow(SW_SHOW);
				mMergePreviewBanksCheck.ShowWindow(SW_SHOW);
			}
			else if (gameConfigMidi[x].gameType.CompareNoCase("PaperMario") == 0)
			{
				mOutputLoop.ShowWindow(SW_HIDE);
				mOutputLoopCount.ShowWindow(SW_HIDE);
				mExtendSmallerTracksToEnd.ShowWindow(SW_SHOW);

				mWriteDLSCombinedBanksButton.ShowWindow(SW_SHOW);
				mMergePreviewBanksCheck.ShowWindow(SW_SHOW);
			}
			else if ((gameConfigMidi[x].gameType.CompareNoCase("Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("HexenSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("BlitzSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("RNCSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("HexenSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ASMICSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("SnowSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ARISng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ArmySargeSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ArmySarge2Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("TazSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("FLA2Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("RNCSngOffset") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("LZSS_0BSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("AVL_0Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("VigilanteSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ViewpointSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("RugratsSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("MultiPartTigSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZLibSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZLibIndexedSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("MarioTennisSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("Yay0Sng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZipSng") == 0)
			|| (gameConfigMidi[x].gameType.CompareNoCase("ZTRK") == 0)
				)
			{
				mOutputLoop.ShowWindow(SW_HIDE);
				mOutputLoopCount.ShowWindow(SW_HIDE);

				mExtendSmallerTracksToEnd.ShowWindow(SW_HIDE);

				mWriteDLSCombinedBanksButton.ShowWindow(SW_HIDE);
				mMergePreviewBanksCheck.ShowWindow(SW_HIDE);
			}
			else if (
				(gameConfigMidi[x].gameType.CompareNoCase("Yaz0Seq64") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("Seq64Mario") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("Seq64Zelda") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("Midi") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("MidiLZSSWilliams") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("LZSamplesDCM") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("DCM") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("MIDx") == 0)
				|| (gameConfigMidi[x].gameType.CompareNoCase("DuckDodgers") == 0)
			)
			{
				mOutputLoop.ShowWindow(SW_HIDE);
				mOutputLoopCount.ShowWindow(SW_HIDE);

				mExtendSmallerTracksToEnd.ShowWindow(SW_HIDE);

				mWriteDLSCombinedBanksButton.ShowWindow(SW_HIDE);
				mMergePreviewBanksCheck.ShowWindow(SW_HIDE);
			}
			else
			{
				mOutputLoop.ShowWindow(SW_HIDE);
				mOutputLoopCount.ShowWindow(SW_HIDE);
				mExtendSmallerTracksToEnd.ShowWindow(SW_SHOW);

				mWriteDLSCombinedBanksButton.ShowWindow(SW_HIDE);
				mMergePreviewBanksCheck.ShowWindow(SW_HIDE);
			}

			std::vector<CString> addMidiStrings;
			int numberMidiStrings = 0;
			int numberInstruments = 0;
			unsigned long startSpot = 0;
			unsigned long endSpot = 0;
			CN64MidiToolReader::ProcessMidis(gameConfigMidi, x, addMidiStrings, numberMidiStrings, numberInstruments, compressed, buffer, romSize, startSpot, endSpot, false, mSeparateByInstrument.GetCheck(), mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfigMidi[x].extraGameMidiInfo);

			for (int y = 0; y < numberMidiStrings; y++)
			{
				if ((soundfontBank.midiNumbers.size() == 0) || (std::find(soundfontBank.midiNumbers.begin(), soundfontBank.midiNumbers.end(), y) != soundfontBank.midiNumbers.end()))
				{
					mMidi.AddString(addMidiStrings[y]);
				}
			}
			mMidi.SetCurSel(0);
			break;
		}
	}

	if (mMidi.GetCount() > 0)
	{
		mPreviewButton.EnableWindow(true);
	}
	else
	{
		mPreviewButton.EnableWindow(false);
	}
}
void CN64SoundbankToolDlg::OnBnClickedButtonwritedlssoundfont2()
{
	if (mSoundBankNumber.GetCount() == 0)
		return;

	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString outputPath = tempDlg.GetPathName();
	outputPath = (outputPath + "\\");


	for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
	{
		int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

		ALBank* alBankCurrent = NULL;
		if (soundBankSelected >= alBankList.size())
		{
			return;
		}

		alBankCurrent = alBankList[soundBankSelected];

		if (alBankCurrent != NULL)
		{
			std::vector<ALBank*> alBanks;
			alBanks.push_back(alBankCurrent);

			std::vector<int> bankIndexes;
			bankIndexes.push_back(soundBankSelected);

			CString tempStr;
			tempStr.Format("ExtractedSoundbank_%02X.dls", soundBankSelected);

			FILE* outDebugFile = NULL;
			if (mDebug.GetCheck())
				outDebugFile = fopen(outputPath + tempStr + "Debug.txt", "w");

			WriteDLS(outputPath + tempStr, alBanks, bankIndexes, false, outDebugFile);

			if (outDebugFile)
			{
				fflush(outDebugFile);
				fclose(outDebugFile);
			}
		}
	}
}

void CN64SoundbankToolDlg::OnBnClickedButtonwritemidistofolder()
{
	if (mMidi.GetCurSel() == -1)
		return;

	if (buffer == NULL)
		return;

	if (mMidi.GetCount() == 0)
		return;

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
		return;

	CString tempPath = tempDlg.GetPathName();
	tempPath = (tempPath + "\\");

	for (int x = 0; x < mMidi.GetCount(); x++)
	{
		CString spotStr;
		mMidi.GetLBText(x, spotStr);

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
		gameName.Trim();

		int gameNumber = 0;
		for (int y = 0; y < countGamesMidi; y++)
		{
			if (gameConfigMidi[y].gameName.CompareNoCase(gameName) == 0)
			{
				gameNumber = y;
				break;
			}
		}

		if ((size > 0)  || (gameConfigMidi[gameNumber].gameType.CompareNoCase("Yaz0Seq64") == 0) || (gameConfigMidi[gameNumber].gameType.CompareNoCase("Seq64Zelda") == 0) || (gameConfigMidi[gameNumber].gameType.CompareNoCase("Seq64Mario") == 0))
		{
			CString outputName;
			if (gameConfigMidi[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
			{
				outputName.Format("%s%s %08X %08X.xm", tempPath, gameName, x, address);
			}
			else if (gameConfigMidi[gameNumber].gameType.CompareNoCase("DCM") == 0)
			{
				outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			}
			else if (gameConfigMidi[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
			{
				outputName.Format("%s%s %08X %08X.dcm", tempPath, gameName, x, address);
			}
			else
			{
				if (x < gameConfigMidi[gameNumber].numberMidiNames)
				{
					CString midiName = gameConfigMidi[gameNumber].midiNames[x];
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

				outputName.Format("%s%s %08X %08X.mid", tempPath, gameName, x, address);
			}
			try
			{
				CString gameName;
				m_game.GetWindowText(gameName);
				gameName.Trim();

				int gameNumber = 0;
				for (int y = 0; y < countGamesMidi; y++)
				{
					if (gameConfigMidi[y].gameName.CompareNoCase(gameName) == 0)
					{
						gameNumber = y;
						break;
					}
				}

				int numberInstruments;
				
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfigMidi[gameNumber].gameName, buffer, romSize, address, size, outputName, gameConfigMidi[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebug.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfigMidi[gameNumber].extraGameMidiInfo, true);
			}
			catch (...)
			{
				MessageBox("Exception processing midi, maybe bad auto-read");
			}
		}
	}
}

void CN64SoundbankToolDlg::OnBnClickedButtonexportmidi()
{
	if (buffer == NULL)
		return;

	if (mMidi.GetCount() == 0)
		return;

	CString spotStr;
	mMidi.GetWindowText(spotStr);

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
	gameName.Trim();

	int gameNumber = 0;
	for (int x = 0; x < countGames; x++)
	{
		if (gameConfig[x].gameName.CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	int midiIndex = mMidi.GetCurSel();

	CString outputName;
	if (gameConfig[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		outputName.Format("%s%s %08X %08X.xm", "", gameName, midiIndex, address);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("DCM") == 0)
	{
		outputName.Format("%s%s %08X %08X.dcm", "", gameName, midiIndex, address);
	}
	else if (gameConfig[gameNumber].gameType.CompareNoCase("LZSamplesDCM") == 0)
	{
		outputName.Format("%s%s %08X %08X.dcm", "", gameName, midiIndex, address);
	}
	else
	{
		if (midiIndex < gameConfigMidi[gameNumber].numberMidiNames)
		{
			CString midiName = gameConfigMidi[gameNumber].midiNames[midiIndex];
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

	if (gameConfigMidi[gameNumber].gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
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
		midiParse.ExportToMidi(gameConfigMidi[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfigMidi[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebug.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfigMidi[gameNumber].extraGameMidiInfo, false);
	}
	else if (gameConfigMidi[gameNumber].gameType.CompareNoCase("DCM") == 0)
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
		midiParse.ExportToMidi(gameConfigMidi[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfigMidi[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebug.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfigMidi[gameNumber].extraGameMidiInfo, false);
	}
	else
	{
		if (buffer == NULL)
			return;

		if (mMidi.GetCount() == 0)
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
		mMidi.GetWindowText(spotStr);

		unsigned long address = 0;
		unsigned long size = 0;
		sscanf(spotStr, "%08X:%08X", &address, &size);

		CString gameName;
		m_game.GetWindowText(gameName);
		gameName.Trim();

		int gameNumber = 0;
		for (int x = 0; x < countGamesMidi; x++)
		{
			if (FilterGameName(gameConfigMidi[x].gameName).CompareNoCase(gameName) == 0)
			{
				gameNumber = x;
				break;
			}
		}

		int numberInstruments;
		bool hasLoopPoint = false;
		int loopStart = 0;
		int loopEnd = 0;
		midiParse.ExportToMidi(gameConfigMidi[gameNumber].gameName, buffer, romSize, address, size, fileName, gameConfigMidi[gameNumber].gameType, numberInstruments, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, mSeparateByInstrument.GetCheck(), mDebug.GetCheck(), extra, extra2, mOutputLoop.GetCheck(), CEditControlToDecimalValue(&mOutputLoopCount), mExtendSmallerTracksToEnd.GetCheck(), gameConfigMidi[gameNumber].extraGameMidiInfo, false);
	}
}

int CN64SoundbankToolDlg::CEditControlToDecimalValue(CEdit* editControl)
{
	CString tempStr;
	editControl->GetWindowText(tempStr);

	return atoi(tempStr);
}
void CN64SoundbankToolDlg::OnBnClickedButtonwritedlssoundfont3()
{
	CString gameName;
	m_game.GetWindowText(gameName);
	gameName.Trim();

	int gameNumber = -1;
	for (int x = 0; x < countGamesMidi; x++)
	{
		if (FilterGameName(gameConfigMidi[x].gameName).CompareNoCase(gameName) == 0)
		{
			gameNumber = x;
			break;
		}
	}

	if (gameNumber == -1)
		return;

	GameConfig gameConfigBank;
	for (int x = 0; x < countGames; x++)
	{
		if (gameName == FilterGameName(gameConfig[x].gameName))
		{
			gameConfigBank = gameConfig[x];
			break;
		}
	}

	CFileDialog m_svFile(FALSE, "dls", "ExtractedSoundbankCombined.dls", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "DLS Soundfont (*.dls)|*.dls|");

	int statusFileOpen = (int) m_svFile.DoModal();
	CString fileName = m_svFile.GetFileName();

	if ((statusFileOpen == IDOK) && (fileName != ""))
	{
		std::vector<ALBank*> alBanks;
		std::vector<int> bankIndexes;
		std::vector<int> instrumentIndex;
		std::vector<int> instrumentOffset;

		if (gameConfigMidi[gameNumber].gameType.CompareNoCase("Konami") == 0)
		{
			for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
			{
				int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

				ALBank* alBankCurrent = NULL;
				if (soundBankSelected >= alBankList.size())
				{
					return;
				}

				alBanks.push_back(alBankList[soundBankSelected]);

				bankIndexes.push_back(x);
			}

			FILE* outDebugFile = NULL;
			if (mDebug.GetCheck())
				outDebugFile = fopen(m_svFile.GetPathName() + "Debug.txt", "w");
			WriteDLS(m_svFile.GetPathName(), alBanks, bankIndexes, true, outDebugFile);
			if (outDebugFile)
			{
				fflush(outDebugFile);
				fclose(outDebugFile);
			}
		}
		else if (
					(gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5Zlb") == 0)
				|| (gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5ZlbGCStyle") == 0)
				|| (gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5LZGCStyle") == 0)
				|| (gameConfigMidi[gameNumber].gameType.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0)
				)
		{
			for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
			{
				int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

				ALBank* alBankCurrent = NULL;
				if (soundBankSelected >= alBankList.size())
				{
					return;
				}

				alBanks.push_back(alBankList[soundBankSelected]);

				bankIndexes.push_back(x);
			}

			FILE* outDebugFile = NULL;
			if (mDebug.GetCheck())
				outDebugFile = fopen(m_svFile.GetPathName() + "Debug.txt", "w");
			WriteDLSCombinedFactor5(m_svFile.GetPathName(), alBanks, bankIndexes, buffer, romSize, gameConfigBank.projOffset, gameConfigBank.poolOffset, gameConfigBank.projSize, gameConfigBank.poolSize, outDebugFile, gameConfigBank.absoluteOffsets);
			if (outDebugFile)
			{
				fflush(outDebugFile);
				fclose(outDebugFile);
			}
		}
		else if (gameConfigMidi[gameNumber].gameType.CompareNoCase("PaperMario") == 0)
		{
			if (mSoundBankNumber.GetCurSel() == -1)
				return;

			int startCombo = 0;

			for (int x = 0; x < gameConfigBank.soundfontBank.size(); x++)
			{
				int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

				
				if (soundBankSelected >= alBankList.size())
				{
					return;
				}

				if (alBankList[soundBankSelected]->bankName.Find("GM") == 0)
				{
					startCombo = x;
					break;
				}
			}

			CString name = alBankList[gameConfigBank.soundfontBank[startCombo].soundBankNumber]->bankName;
			CString firstTwoName = name.Mid(0,2);

			for (int x = startCombo; x < gameConfigBank.soundfontBank.size(); x++)
			{
				int soundBankSelected = gameConfigBank.soundfontBank[x].soundBankNumber;

				
				if (soundBankSelected >= alBankList.size())
				{
					return;
				}

				if (alBankList[soundBankSelected]->bankName.Find(firstTwoName) != 0)
				{
					if (firstTwoName == "GM")
					{
						// Placeholders for drum splits
						alBanks.push_back(NULL); //A6
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //A7
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //A8
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //A9
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //AA
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //AB
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //AC
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //AD
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //AE
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						alBanks.push_back(NULL); //AF
						bankIndexes.push_back(0);
						instrumentIndex.push_back(-1);
						instrumentOffset.push_back(-1);

						firstTwoName = "PS";
						x = -1;
						continue;
					}
					else
					{
						break;
					}
				}

				for (int r = 0; r < alBankList[soundBankSelected]->count; r++)
				{
					instrumentIndex.push_back(alBanks.size());
					instrumentOffset.push_back(r);
				}

				alBanks.push_back(alBankList[soundBankSelected]);

				bankIndexes.push_back(x);
			}

			FILE* outDebugFile = NULL;
			if (mDebug.GetCheck())
				outDebugFile = fopen(m_svFile.GetPathName() + "Debug.txt", "w");
			WriteDLSCombinedPaperMario(m_svFile.GetPathName(), alBanks, bankIndexes, buffer, romSize, gameConfigBank.percussionOffset, instrumentIndex, instrumentOffset, outDebugFile);
			if (outDebugFile)
			{
				fflush(outDebugFile);
				fclose(outDebugFile);
			}
		}
	}
}

void CN64SoundbankToolDlg::OnBnClickedCheckextendloopstohighesttrack()
{
	if (mExtendSmallerTracksToEnd.GetCheck())
		mOutputLoop.EnableWindow(false);
	else
		mOutputLoop.EnableWindow(true);
}

void CN64SoundbankToolDlg::OnBnClickedCheckoutputloop()
{
	if (mOutputLoop.GetCheck())
		mExtendSmallerTracksToEnd.EnableWindow(false);
	else
		mExtendSmallerTracksToEnd.EnableWindow(true);
}

unsigned short CN64SoundbankToolDlg::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned short CN64SoundbankToolDlg::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned long CN64SoundbankToolDlg::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned long CN64SoundbankToolDlg::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}