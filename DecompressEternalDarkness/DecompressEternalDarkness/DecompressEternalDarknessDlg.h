
// DecompressEternalDarknessDlg.h : header file
//

#pragma once


// CDecompressEternalDarknessDlg dialog
class CDecompressEternalDarknessDlg : public CDialog
{
// Construction
public:
	CDecompressEternalDarknessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DECOMPRESSETERNALDARKNESS_DIALOG };

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
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
