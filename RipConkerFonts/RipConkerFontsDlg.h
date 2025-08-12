
// RipConkerFontsDlg.h : header file
//

#include "GECompression.h"

#pragma once
#define NTSCOFFSET 0x40F10
#define PALOFFSET 0x41270
#define ECTSOFFSET 0x20550
#define DEBUGOFFSET 0x20D10

#define NTSCPALSIZE 5440
#define ECTSSIZE 5344
#define NTSCPALENTRIES 0x5F
#define ECTSENTRIES 0x5E

#define NTSCMAINFILE 0x00188328
#define NTSCMAINFILECOMPRESSEDSIZE 0x000144B0
#define NTSCPALMAINFILESTARTFONT 0x00000460
#define NTSCPALMAINFILEENDFONT 0x00000464

#define PALMAINFILE 0x00188688
#define PALMAINFILECOMPRESSEDSIZE 0x000144A0

#define ECTSMAINFILESTARTFONT 0x0020FF20
#define ECTSMAINFILEENDFONT 0x0020FF24

#define DEBUGMAINFILE 0x001730E0
#define DEBUGMAINFILECOMPRESSEDSIZE 0x00014BD0
#define DEBUGMAINFILESTARTFONT 0x00000460
#define DEBUGMAINFILEENDFONT 0x00000464

// CRipConkerFontsDlg dialog
class CRipConkerFontsDlg : public CDialog
{
// Construction
public:
	CRipConkerFontsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RIPCONKERFONTS_DIALOG };

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
	afx_msg void OnBnClickedButtonrip();
	afx_msg void OnBnClickedButtonimport();

	CString directory;

	GECompression compression;
};
