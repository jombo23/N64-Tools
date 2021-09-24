
// DecompressEternalDarknessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DecompressEternalDarkness.h"
#include "DecompressEternalDarknessDlg.h"
#include <vector>
#include "FolderDialog.h"
#include "SK_ASCCompression.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDecompressEternalDarknessDlg dialog




CDecompressEternalDarknessDlg::CDecompressEternalDarknessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDecompressEternalDarknessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDecompressEternalDarknessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDecompressEternalDarknessDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CDecompressEternalDarknessDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDecompressEternalDarknessDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDecompressEternalDarknessDlg message handlers

BOOL CDecompressEternalDarknessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Recurse(LPCTSTR pstr, std::vector<CString>& files)
{
   CFileFind finder;

   CString strWildcard(pstr);
   strWildcard += _T("\\*.*");

   BOOL bWorking = finder.FindFile(strWildcard);

   while (bWorking)
   {
      bWorking = finder.FindNextFile();

      if (finder.IsDots())
         continue;

	  CString str = finder.GetFilePath();
      if (finder.IsDirectory())
      {
         Recurse(str, files);
      }
	  else
	  {
		 files.push_back(str);
	  }
   }

   finder.Close();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDecompressEternalDarknessDlg::OnPaint()
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
HCURSOR CDecompressEternalDarknessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDecompressEternalDarknessDlg::OnBnClickedButton1()
{
	const TCHAR szFilter[] = _T("All Files (*.*)|*.*||");
	CFileDialog dlg(FALSE, NULL, NULL, NULL, szFilter, this);    
	if(dlg.DoModal() != IDOK)
	{
		return;
	}

	CString sFilePath = dlg.GetPathName();

	CFileDialog dlgsv(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);    
	if(dlgsv.DoModal() != IDOK)
	{
		return;
	}
	CString sFilePathOut = dlgsv.GetPathName();

	// length is first 4 bytes of decompressed buffer
	CSK_ASCCompression sk_ascCompression;
	sk_ascCompression.DecompressFile(sFilePath, sFilePathOut);
}

int CreateDirectoryRecursively( LPCTSTR path )
{
    return SHCreateDirectoryEx( NULL, path, NULL );
}

void CDecompressEternalDarknessDlg::OnBnClickedButton2()
{
	//CString folder = "C:\\temp\\eternaldarknessjune2002rip\\";
	//CString folderOut = "C:\\temp\\eternaldarknessjune2002decompressed\\";

	//CString folder = "C:\\temp\\eternaldarknessrip\\";
	//CString folderOut = "C:\\temp\\eternaldarknessdecompressed\\";

	//CreateDirectoryRecursively(folderOut);

	CFolderDialog tempDlg;
	int dialogbox = tempDlg.DoModal();
	if ((dialogbox == IDCANCEL) || (tempDlg.GetPathName()==""))
	{
		return;
	}

	CString folder = tempDlg.GetPathName();
	folder = (folder + "\\");

	CFolderDialog tempDlg2;
	int dialogbox2 = tempDlg2.DoModal();
	if ((dialogbox2 == IDCANCEL) || (tempDlg2.GetPathName()==""))
	{
		return;
	}

	CString folderOut = tempDlg2.GetPathName();
	folderOut = (folderOut + "\\");

	CString outReport = "";

	std::vector<CString> files;
	Recurse(folder, files);
	for (int x = 0; x < files.size(); x++)
	{
		FILE* inFile = fopen(files[x], "rb");
		if (!inFile)
		{
			outReport += "Can't open file " + files[x] + "\n";
			continue;
		}
		unsigned char tempBuffer[8];
		fread(tempBuffer, 1, 8, inFile);
		fclose(inFile);

		// *SK_ASC*
		if (
			(tempBuffer[0] == 0x2A) && (tempBuffer[1] == 0x53) && (tempBuffer[2] == 0x4B) && (tempBuffer[3] == 0x5F)
			&& (tempBuffer[4] == 0x41) && (tempBuffer[5] == 0x53) && (tempBuffer[6] == 0x43) && (tempBuffer[7] == 0x2A)
			)
		{
			CString inputFileNameStripped = files[x];
			inputFileNameStripped.Replace(folder, "");

			CString outputFileName = folderOut + inputFileNameStripped + ".decompressed";
			CString outputSubFolder = outputFileName;
			PathRemoveFileSpec(outputSubFolder.GetBuffer(0)); 
			outputSubFolder.ReleaseBuffer();

			CreateDirectoryRecursively(outputSubFolder);

			CString filename = PathFindFileName(files[x]);
			// Compressed
			CSK_ASCCompression sk_ascCompression;
			int fileSize = sk_ascCompression.DecompressFile(files[x], outputFileName);
			CString fileSizeStr;
			fileSizeStr.Format("%08X Bytes", fileSize);
			
			outReport += inputFileNameStripped + " " + fileSizeStr + "\n";
		}
	}
	
	FILE* outReportFile = fopen(folderOut + "DecompressedReport.txt", "w");
	fprintf(outReportFile, "%s", outReport); 
	fflush(outReportFile);
	fclose(outReportFile);
}