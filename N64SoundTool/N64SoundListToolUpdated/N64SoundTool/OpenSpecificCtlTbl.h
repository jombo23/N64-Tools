#pragma once
#include "afxwin.h"
#include "..\N64SoundLibrary\SharedFunctions.h"

// COpenSpecificCtlTbl dialog

class COpenSpecificCtlTbl : public CDialog
{
	DECLARE_DYNAMIC(COpenSpecificCtlTbl)

public:
	COpenSpecificCtlTbl(CWnd* pParent = NULL);   // standard constructor
	virtual ~COpenSpecificCtlTbl();

// Dialog Data
	enum { IDD = IDD_DIALOGSPECIFYLOCATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit mCtl;
	CEdit mTbl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	unsigned long GetCtl();
	unsigned long ctl;
	unsigned long GetTbl();
	unsigned long tbl;
};
