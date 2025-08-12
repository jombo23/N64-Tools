
// RipConkerFontsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RipConkerFonts.h"
#include "RipConkerFontsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRipConkerFontsDlg dialog




CRipConkerFontsDlg::CRipConkerFontsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRipConkerFontsDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRipConkerFontsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRipConkerFontsDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONRIP, &CRipConkerFontsDlg::OnBnClickedButtonrip)
	ON_BN_CLICKED(IDC_BUTTONIMPORT, &CRipConkerFontsDlg::OnBnClickedButtonimport)
END_MESSAGE_MAP()


// CRipConkerFontsDlg message handlers

BOOL CRipConkerFontsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	directory.Format("%s\\", tempFolder);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRipConkerFontsDlg::OnPaint()
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
HCURSOR CRipConkerFontsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void SaveGreyscaleFontToBitmap(int w, int h, unsigned char* indexes, CString filename, int pad)
{
	int Width = w;
	int Height = h;
	int Bits = 0x20;

	FILE * Shiznit;
	CString Path = filename;
	Shiznit = fopen(Path, "wb");

	if (Shiznit == NULL)
	{
		CreateDirectory("Fonts", 0);
		Shiznit = fopen(Path, "wb");
	}
	

	if (Shiznit != NULL)
	{

		int sized = (Width*Height)*4;

		BITMAPFILEHEADER bmphdr;
		BITMAPINFOHEADER bmpihdr;

		bmphdr.bfType = 0x4D42;	//0x00 short
		bmphdr.bfSize = ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sized );	//0x02 long
		bmphdr.bfOffBits = ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ); // 0x0a long
		bmphdr.bfReserved1 = 0;	//0x06 short
		bmphdr.bfReserved2 = 0;	//0x08 short
		bmpihdr.biBitCount = Bits;	//0x1c
		bmpihdr.biClrImportant = 0;
		bmpihdr.biClrUsed = 0;
		bmpihdr.biCompression = BI_RGB;

		bmpihdr.biHeight = Height;

		bmpihdr.biPlanes = 1;	//0x1a
		bmpihdr.biSize = sizeof(BITMAPINFOHEADER);
		bmpihdr.biSizeImage = sized;
		
		int trueWidth = bmpihdr.biWidth = Width;
		//if ((Width % pad) != 0)
			//Width = ((pad - (Width % pad)) + Width);
		
		
		//bmpihdr.biWidth = Width; // bitmaps dont support these non-8 sizes? They should...

		bmpihdr.biXPelsPerMeter = 0x0b12;	// Taken from a bitmap created in Photoshop
		bmpihdr.biYPelsPerMeter = 0x0b12;; // Taken from a bitmap created in Photoshop

		fwrite((void*)&bmphdr, 1, sizeof(BITMAPFILEHEADER), Shiznit);
		fwrite((void*)&bmpihdr, 1, sizeof(BITMAPINFOHEADER), Shiznit);

		for (int V0 = 0; V0 < bmpihdr.biHeight; V0++)
		{
			for (int V1 = 0; V1 < bmpihdr.biWidth/*trueWidth*/; V1++)
			{	
				unsigned char tempChar = (indexes[((bmpihdr.biHeight - V0 - 1) * Width) + V1]);
				fwrite(&tempChar, 1, 1, Shiznit);
				fwrite(&tempChar, 1, 1, Shiznit);
				fwrite(&tempChar, 1, 1, Shiznit);
				tempChar = 0xFF;
				fwrite(&tempChar, 1, 1, Shiznit);
			}
		}

		//fwrite(outthangs, 2, (Width * Height), Shiznit);
		fclose(Shiznit);

	}

}

int GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void CRipConkerFontsDlg::OnBnClickedButtonrip()
{
	//FILE* conkerRom = fopen("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\Conker's Bad Fur Day (U) [!].z64", "rb");
	CFileDialog m_ldFile(TRUE, NULL, "Conker's Bad Fur Day (U) [!].z64", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ROM (*.v64;*.z64;*.rom;*.bin;*.zlb)|*.v64;*.z64;*.rom;*.bin;*.zlb|", this);
	int didRead = m_ldFile.DoModal();
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	if (GetSizeFile(m_ldFile.GetPathName()) != 0x4000000)
	{
		MessageBox("Invalid rom size");
		return;
	}

	FILE* conkerRom = fopen(m_ldFile.GetPathName(), "rb");
	if (conkerRom == NULL)
	{
		MessageBox("Invalid rom size");
		return;
	}

	unsigned char* buffer = new unsigned char[0x4000000];
	fread(buffer, 1, 0x4000000, conkerRom);
	fclose(conkerRom);

	unsigned long spot;
	int size;
	int numberEntries;

	if (buffer[0x25] == 0x45) // ECTS
	{
		spot = ECTSOFFSET;
		size = ECTSSIZE;
		numberEntries = ECTSENTRIES;
	}
	else if ((buffer[0x20] == 0x43) && (buffer[0x2B] == 0x20) && (buffer[0x3E] == 0x45)) // retail NTSC
	{
		spot = NTSCOFFSET;
		size = NTSCPALSIZE;
		numberEntries = NTSCPALENTRIES;
	}
	else if ((buffer[0x20] == 0x43) && (buffer[0x2B] == 0x44)) // Debug Conker
	{
		spot = DEBUGOFFSET;
		size = NTSCPALSIZE;
		numberEntries = NTSCPALENTRIES;
	}
	else if (buffer[0x3E] == 0x50) // PAL
	{
		spot = PALOFFSET;
		size = NTSCPALSIZE;
		numberEntries = NTSCPALENTRIES;
	}
	else
	{
		MessageBox("Unknown Conker ROM");
		return;
	}

	unsigned char digit = 0x30;

	while (digit < (0x30 + numberEntries))
	{
		unsigned char width = buffer[spot];
		unsigned char height = buffer[spot+1];

		unsigned char* outPixels = new unsigned char[width*height];
		unsigned short totalCharSize = (((((buffer[spot+4] << 8) | buffer[spot+5]) << 8) | buffer[spot+6]) << 8) | buffer[spot+7];

		int fontSpot = 0;
		int fontCount = 0;
		while (fontCount < (width * height))
		{
			unsigned char grey = buffer[spot + fontSpot + 8] & 0xF0;
			unsigned char count = (buffer[spot + fontSpot + 8] & 0xF) + 1;

			for (int x = 0; x < count; x++)
			{
				outPixels[fontCount++] = grey;
			}

			fontSpot++;
		}

		CString tempStr;
		/*tempStr.Format(directory + "Fonts\\Conker%02XRaw_%02X_%02X.bin", (digit), buffer[spot+2], buffer[spot+3]);

		FILE* temp = fopen(tempStr, "wb");
		fwrite(outPixels, 1, fontCount, temp);
		fclose(temp);*/

		tempStr.Format(directory + "Fonts\\Conker%02X.bmp", (digit++));

		SaveGreyscaleFontToBitmap(width, height, outPixels, tempStr, 0);

		delete [] outPixels;

		spot += totalCharSize;
	}

	delete [] buffer;
}

bool ReplaceBitmap(CString filename, unsigned char*& outPixels, int& width, int& height)
{
	outPixels = NULL;

	FILE* bitmap = fopen(filename, "rb");
	if (bitmap == NULL)
	{
		MessageBox(NULL, "Could not open Bitmap", "Error", NULL);
		return false;
	}
	unsigned short temp[0x18];
	fread(temp, 0x18, 2, bitmap);
	fclose(bitmap);
	width = temp[9];
	height = temp[0xb];
	unsigned char bitAmount = temp[0xe];

	if (bitAmount != 0x20)
	{
		MessageBox(NULL, "Must be 24-bit RGB or 32-bit ARGB bmp", "Error", NULL);
		return false;
	}

	bitmap = fopen(filename, "rb");
	if (bitmap == NULL)
	{
		MessageBox(NULL, "Could not open Bitmap", "Error", NULL);
		return false;
	}

	unsigned char startOffset = temp[0x5];
	fseek(bitmap, startOffset, SEEK_SET);

	outPixels = new unsigned char[width * height];

	int inCount = 0;

	for (int x = (height-1); x > -1; x--)
	{
		for (int y = 0; y < width; y++)
		{
			unsigned char temp1, temp2, temp3, alpha;
			//bgra
			fread(&temp1, 1, 1, bitmap);
			fread(&temp2, 1, 1, bitmap);
			fread(&temp3, 1, 1, bitmap);
			fread(&alpha, 1, 1, bitmap);

			outPixels[(x * width) + y] = ((temp1 + temp2 + temp3) / 3);
		}
	}

	return true;
}


BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory, int& errorCode, int waitTime)
{
	errorCode = 0;
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
   WaitForSingleObject(processInfo.hProcess, waitTime);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
	   errorCode = exitCode;
		if (exitCode == STILL_ACTIVE)
		{
			MessageBox(NULL, "For some reason Process Failed", "Error", NULL);
			TerminateProcess(processInfo.hProcess, exitCode);

			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
			return false;
		}
   }   

   CloseHandle(processInfo.hProcess);
   CloseHandle(processInfo.hThread);
   return true;
}

BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory, int& errorCode)
{
	return hiddenExec(pCmdLine, currentDirectory, errorCode, 20000);
}

BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
	int errorCode;
	return hiddenExec(pCmdLine, currentDirectory, errorCode);
};

unsigned long Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned long CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}



void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}


void CRipConkerFontsDlg::OnBnClickedButtonimport()
{
	unsigned char types1[0x100];
	unsigned char types2[0x100];

	types1[0x30] = 0x00;
	types1[0x31] = 0x00;
	types1[0x32] = 0x00;
	types1[0x33] = 0x00;
	types1[0x34] = 0x00;
	types1[0x35] = 0x00;
	types1[0x36] = 0x00;
	types1[0x37] = 0x00;
	types1[0x38] = 0x00;
	types1[0x39] = 0x00;
	types1[0x3A] = 0x00;
	types1[0x3B] = 0x00;
	types1[0x3C] = 0x00;
	types1[0x3D] = 0x00;
	types1[0x3E] = 0x00;
	types1[0x3F] = 0x00;
	types1[0x40] = 0x00;
	types1[0x41] = 0x00;
	types1[0x42] = 0x00;
	types1[0x43] = 0x00;
	types1[0x44] = 0x00;
	types1[0x45] = 0x00;
	types1[0x46] = 0x00;
	types1[0x47] = 0x00;
	types1[0x48] = 0x00;
	types1[0x49] = 0x00;
	types1[0x4A] = 0x00;
	types1[0x4B] = 0x00;
	types1[0x4C] = 0x00;
	types1[0x4D] = 0x00;
	types1[0x4E] = 0x00;
	types1[0x4F] = 0x00;
	types1[0x50] = 0x00;
	types1[0x51] = 0x00;
	types1[0x52] = 0x00;
	types1[0x53] = 0x00;
	types1[0x54] = 0x00;
	types1[0x55] = 0x00;
	types1[0x56] = 0x00;
	types1[0x57] = 0x00;
	types1[0x58] = 0x00;
	types1[0x59] = 0x00;
	types1[0x5A] = 0x00;
	types1[0x5B] = 0x00;
	types1[0x5C] = 0x00;
	types1[0x5D] = 0x01;
	types1[0x5E] = 0x00;
	types1[0x5F] = 0x00;
	types1[0x60] = 0x00;
	types1[0x61] = 0x00;
	types1[0x62] = 0x00;
	types1[0x63] = 0x00;
	types1[0x64] = 0x00;
	types1[0x65] = 0x00;
	types1[0x66] = 0x00;
	types1[0x67] = 0x00;
	types1[0x68] = 0x00;
	types1[0x69] = 0x00;
	types1[0x6A] = 0x00;
	types1[0x6B] = 0x00;
	types1[0x6C] = 0x00;
	types1[0x6D] = 0x00;
	types1[0x6E] = 0x02;
	types1[0x6F] = 0x00;
	types1[0x70] = 0x00;
	types1[0x71] = 0x00;
	types1[0x72] = 0x00;
	types1[0x73] = 0x00;
	types1[0x74] = 0x00;
	types1[0x75] = 0x00;
	types1[0x76] = 0x00;
	types1[0x77] = 0x00;
	types1[0x78] = 0x00;
	types1[0x79] = 0x00;
	types1[0x7A] = 0x00;
	types1[0x7B] = 0x00;
	types1[0x7C] = 0x00;
	types1[0x7D] = 0x00;
	types1[0x7E] = 0x00;
	types1[0x7F] = 0x00;
	types1[0x80] = 0x00;
	types1[0x81] = 0x00;
	types1[0x82] = 0x00;
	types1[0x83] = 0x00;
	types1[0x84] = 0x00;
	types1[0x85] = 0x00;
	types1[0x86] = 0x00;
	types1[0x87] = 0x00;
	types1[0x88] = 0x00;
	types1[0x89] = 0x00;
	types1[0x8A] = 0x00;
	types1[0x8B] = 0x00;
	types1[0x8C] = 0x00;
	types1[0x8D] = 0x00;
	types1[0x8E] = 0x00;

	types2[0x30] = 0x00;
	types2[0x31] = 0x00;
	types2[0x32] = 0x00;
	types2[0x33] = 0x00;
	types2[0x34] = 0x00;
	types2[0x35] = 0x00;
	types2[0x36] = 0x00;
	types2[0x37] = 0x00;
	types2[0x38] = 0x00;
	types2[0x39] = 0x00;
	types2[0x3A] = 0x01;
	types2[0x3B] = 0x00;
	types2[0x3C] = 0x00;
	types2[0x3D] = 0x01;
	types2[0x3E] = 0x00;
	types2[0x3F] = 0x00;
	types2[0x40] = 0x00;
	types2[0x41] = 0x00;
	types2[0x42] = 0x01;
	types2[0x43] = 0x01;
	types2[0x44] = 0x01;
	types2[0x45] = 0x01;
	types2[0x46] = 0x00;
	types2[0x47] = 0x01;
	types2[0x48] = 0x01;
	types2[0x49] = 0x01;
	types2[0x4A] = 0x01;
	types2[0x4B] = 0x01;
	types2[0x4C] = 0x01;
	types2[0x4D] = 0x00;
	types2[0x4E] = 0x01;
	types2[0x4F] = 0x01;
	types2[0x50] = 0x01;
	types2[0x51] = 0x01;
	types2[0x52] = 0x01;
	types2[0x53] = 0x01;
	types2[0x54] = 0x01;
	types2[0x55] = 0x01;
	types2[0x56] = 0x01;
	types2[0x57] = 0x02;
	types2[0x58] = 0x07;
	types2[0x59] = 0x01;
	types2[0x5A] = 0x01;
	types2[0x5B] = 0x03;
	types2[0x5C] = 0x01;
	types2[0x5D] = 0x06;
	types2[0x5E] = 0x03;
	types2[0x5F] = 0x01;
	types2[0x60] = 0x00;
	types2[0x61] = 0x00;
	types2[0x62] = 0x00;
	types2[0x63] = 0x03;
	types2[0x64] = 0x04;
	types2[0x65] = 0x02;
	types2[0x66] = 0x02;
	types2[0x67] = 0x00;
	types2[0x68] = 0x00;
	types2[0x69] = 0x00;
	types2[0x6A] = 0x01;
	types2[0x6B] = 0x02;
	types2[0x6C] = 0x00;
	types2[0x6D] = 0x01;
	types2[0x6E] = 0x01;
	types2[0x6F] = 0x00;
	types2[0x70] = 0x00;
	types2[0x71] = 0x00;
	types2[0x72] = 0x00;
	types2[0x73] = 0x00;
	types2[0x74] = 0x00;
	types2[0x75] = 0x00;
	types2[0x76] = 0x00;
	types2[0x77] = 0x00;
	types2[0x78] = 0x00;
	types2[0x79] = 0x00;
	types2[0x7A] = 0x00;
	types2[0x7B] = 0x00;
	types2[0x7C] = 0x00;
	types2[0x7D] = 0x00;
	types2[0x7E] = 0x00;
	types2[0x7F] = 0x00;
	types2[0x80] = 0x00;
	types2[0x81] = 0x00;
	types2[0x82] = 0x00;
	types2[0x83] = 0x00;
	types2[0x84] = 0x00;
	types2[0x85] = 0x00;
	types2[0x86] = 0x00;
	types2[0x87] = 0x00;
	types2[0x88] = 0x00;
	types2[0x89] = 0x00;
	types2[0x8A] = 0x00;
	types2[0x8B] = 0x00;
	types2[0x8C] = 0x00;
	types2[0x8D] = 0x00;
	types2[0x8E] = 0x01;

	//FILE* conkerRom = fopen("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\Conker's Bad Fur Day (U) [!].z64", "rb");
	CFileDialog m_ldFile(TRUE, NULL, "Conker's Bad Fur Day (U) [!].z64", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "ROM (*.v64;*.z64;*.rom;*.bin;*.zlb)|*.v64;*.z64;*.rom;*.bin;*.zlb|", this);
	int didRead = m_ldFile.DoModal();
	if ((didRead == IDCANCEL) || (m_ldFile.GetPathName() == ""))
		return;

	if (didRead == FALSE)
		return;

	if (GetSizeFile(m_ldFile.GetPathName()) != 0x4000000)
	{
		MessageBox("Invalid rom size");
		return;
	}

	FILE* conkerRom = fopen(m_ldFile.GetPathName(), "rb");
	if (conkerRom == NULL)
	{
		MessageBox("Invalid rom size");
		return;
	}

	unsigned char* buffer = new unsigned char[0x4000000];
	fread(buffer, 1, 0x4000000, conkerRom);
	fclose(conkerRom);

	unsigned long spot;
	int size;
	int numberEntries;

	bool compressedMainFile = true;
	unsigned long mainFileLocation;
	unsigned long mainFileFontStartOffset;
	unsigned long mainFileFontEndOffset;
	unsigned long mainFileFontCompressedSize;

	if (buffer[0x25] == 0x45) // ECTS
	{
		spot = ECTSOFFSET;
		size = ECTSSIZE;
		numberEntries = ECTSENTRIES;

		mainFileFontStartOffset = ECTSMAINFILESTARTFONT;
		mainFileFontEndOffset = ECTSMAINFILEENDFONT;

		compressedMainFile = false;
	}
	else if ((buffer[0x20] == 0x43) && (buffer[0x2B] == 0x20) && (buffer[0x3E] == 0x45)) // retail NTSC
	{
		spot = NTSCOFFSET;
		size = NTSCPALSIZE;
		numberEntries = NTSCPALENTRIES;

		mainFileLocation = NTSCMAINFILE;
		mainFileFontStartOffset = NTSCPALMAINFILESTARTFONT;
		mainFileFontEndOffset = NTSCPALMAINFILEENDFONT;
		mainFileFontCompressedSize = NTSCMAINFILECOMPRESSEDSIZE;
	}
	else if ((buffer[0x20] == 0x43) && (buffer[0x2B] == 0x44)) // Debug Conker
	{
		spot = DEBUGOFFSET;
		size = NTSCPALSIZE;
		numberEntries = NTSCPALENTRIES;

		mainFileLocation = DEBUGMAINFILE;
		mainFileFontStartOffset = DEBUGMAINFILESTARTFONT;
		mainFileFontEndOffset = DEBUGMAINFILEENDFONT;
		mainFileFontCompressedSize = DEBUGMAINFILECOMPRESSEDSIZE;
	}
	else if (buffer[0x3E] == 0x50) // PAL
	{
		spot = PALOFFSET;
		size = NTSCPALSIZE;
		numberEntries = NTSCPALENTRIES;

		mainFileLocation = PALMAINFILE;
		mainFileFontStartOffset = NTSCPALMAINFILESTARTFONT;
		mainFileFontEndOffset = NTSCPALMAINFILEENDFONT;
		mainFileFontCompressedSize = PALMAINFILECOMPRESSEDSIZE;
	}
	else
	{
		MessageBox("Unknown Conker ROM");
		return;
	}

	int dataOffset = spot;

	int subSpot = spot;

	for (int x = spot; x < (spot + size); x++)
		buffer[x] = 0x00;

	for (int x = 0x30; x < (0x30 + numberEntries); x++)
	{
		CString tempStr;
		tempStr.Format(directory + "Fonts\\Conker%02X.bmp", x);

		if (GetSizeFile(m_ldFile.GetPathName()) == 0)
		{
			MessageBox("Missing file " + tempStr, "Cannot continue");
			return;
		}
	}

	for (int x = 0x30; x < (0x30 + numberEntries); x++)
	{
		CString tempStr;
		tempStr.Format(directory + "Fonts\\Conker%02X.bmp", x);

		int width;
		int height;
		unsigned char* outPixels = NULL;

		if (!ReplaceBitmap(tempStr, outPixels, width, height))
		{
			if (outPixels != NULL)
				delete [] outPixels;

			MessageBox("Error loading image " + tempStr, "Cannot continue");
			return;
		}

		buffer[subSpot] = width;
		buffer[subSpot+1] = height;
		buffer[subSpot+2] = types1[x];
		buffer[subSpot+3] = types2[x];

		// Fill size back later
		subSpot += 8;

		int pixelCount = 0;
		while (pixelCount < (width * height))
		{
			int matchCount = 1;
			while ((pixelCount + matchCount) < (width * height))
			{
				if (((pixelCount + matchCount) % width) == 0)
					break;

				if (outPixels[pixelCount] == outPixels[pixelCount + matchCount])
				{
					matchCount++;
				}
				else
					break;

				if (matchCount == 0x10)
					break;

				if (((pixelCount + matchCount) % width) == 0)
					break;
			}

			buffer[subSpot++] = ((outPixels[pixelCount] & 0xF0) | (unsigned char)(matchCount - 1));

			pixelCount += matchCount;
		}

		unsigned long totalSize = subSpot - spot;
		buffer[spot + 0x4] = ((totalSize >> 24) & 0xFF);
		buffer[spot + 0x5] = ((totalSize >> 16) & 0xFF);
		buffer[spot + 0x6] = ((totalSize >> 8) & 0xFF);
		buffer[spot + 0x7] = ((totalSize) & 0xFF);

		spot = subSpot;
	
		if (outPixels != NULL)
		{
			delete [] outPixels;
		}
	}

	if ((spot - dataOffset) > size)
	{
		MessageBox("Too big, cannot exceed size in bytes");
		return;
	}

	if (compressedMainFile)
	{
		compression.SetGame(CONKER);
		compression.SetCompressedBuffer(&buffer[mainFileLocation], 0x4000000 - mainFileLocation);
		compression.SetPath(directory);

		int fileSize = -1;
		int compressedSize = -1;
		unsigned char* outputMainFile = compression.OutputDecompressedBuffer(fileSize, compressedSize);

		unsigned long specifiedStart = CharArrayToLong(&outputMainFile[mainFileFontStartOffset]);
		unsigned long specifiedEnd = CharArrayToLong(&outputMainFile[mainFileFontEndOffset]);

		specifiedEnd = spot;
		if ((specifiedEnd % 0x10) != 0x0)
		{
			specifiedEnd = specifiedEnd + (0x10 - (specifiedEnd % 0x10));
		}

		if (outputMainFile == NULL)
		{
			MessageBox("Error decompressing main file, cannot encode font size", "Error", NULL);
		}
		else
		{
			WriteLongToBuffer(outputMainFile, mainFileFontEndOffset, specifiedEnd);

			FILE* outTemp = fopen(directory + "tempFont.bin", "wb");
			if (outTemp == NULL)
			{
				MessageBox("Cannot write temporary font file", "Error");
			}
			else
			{
				fwrite(outputMainFile, 1, fileSize, outTemp);
				fclose(outTemp);

				if (!compression.CompressGZipFile(directory + "tempFont.bin", directory + "tempFont.conk", false))
				{
					MessageBox("Cannot write compressed font file", "Error");
				}
				else
				{
					int sizeCompressed = GetSizeFile(directory + "tempFont.conk");

					if (sizeCompressed == 0)
					{
						MessageBox("Cannot reread compressed font file", "Error");
					}
					else if (sizeCompressed > mainFileFontCompressedSize)
					{
						MessageBox("Too big compressed font file", "Error");
					}
					else
					{
						::DeleteFile(directory + "tempFont.bin");

						FILE* inCompressed = fopen(directory + "tempFont.conk", "rb");
						if (inCompressed == 0)
						{
							MessageBox("Cannot open compressed font file", "Error");
						}
						fread(&buffer[mainFileLocation], 1, sizeCompressed, inCompressed);
						fclose(inCompressed);

						::DeleteFile(directory + "tempFont.conk");
					}
				}
			}

			delete [] outputMainFile;
		}
	}
	else
	{
		unsigned long specifiedStart = CharArrayToLong(&buffer[mainFileFontStartOffset]);
		unsigned long specifiedEnd = CharArrayToLong(&buffer[mainFileFontEndOffset]);

		WriteLongToBuffer(buffer, mainFileFontEndOffset, specifiedEnd);
	}

	
	CFileDialog m_svFile(FALSE, NULL, (m_ldFile.GetPathName() + ".rom"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "OUT ROM(*.v64;*.z64;*.rom;*.bin)|*.v64;*.z64;*.rom;*.bin|", this);

	int isFileOpened2 = m_svFile.DoModal();

	if (isFileOpened2 == IDCANCEL)
		return;

	if (m_svFile.GetFileName() == "")
		return;

	FILE* outFile = fopen(m_svFile.GetPathName(), "wb");
	if (outFile == NULL)
	{
		MessageBox("Cannot open output file");
		return;
	}
	fwrite(buffer, 1, 0x4000000, outFile);
	fclose(outFile);

	delete [] buffer;

	CString RN64dir = m_svFile.GetPathName().Mid(0, (m_svFile.GetPathName().ReverseFind('\\')+1));
	CString RN64name = m_svFile.GetPathName().Mid((m_svFile.GetPathName().ReverseFind('\\')+1), (m_svFile.GetPathName().GetLength() - (m_svFile.GetPathName().ReverseFind('\\')+1)));

	if (RN64dir != directory)
		::CopyFile((directory + "rn64crc.exe"), (RN64dir + "rn64crc.exe"), false);

	::SetCurrentDirectory(RN64dir);

	CString tempStr;
	tempStr.Format("rn64crc.exe -u \"%s\"", RN64name);
	hiddenExec(_T(tempStr.GetBuffer()), RN64dir);
}
