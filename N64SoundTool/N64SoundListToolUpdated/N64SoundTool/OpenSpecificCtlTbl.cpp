// OpenSpecificCtlTbl.cpp : implementation file
//

#include "stdafx.h"
#include "N64SoundListTool.h"
#include "OpenSpecificCtlTbl.h"


// COpenSpecificCtlTbl dialog

IMPLEMENT_DYNAMIC(COpenSpecificCtlTbl, CDialog)

COpenSpecificCtlTbl::COpenSpecificCtlTbl(CWnd* pParent /*=NULL*/)
	: CDialog(COpenSpecificCtlTbl::IDD, pParent)
{

}

COpenSpecificCtlTbl::~COpenSpecificCtlTbl()
{
}

void COpenSpecificCtlTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITCTL, mCtl);
	DDX_Control(pDX, IDC_EDITTBL, mTbl);
}


BEGIN_MESSAGE_MAP(COpenSpecificCtlTbl, CDialog)
	ON_BN_CLICKED(IDOK, &COpenSpecificCtlTbl::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &COpenSpecificCtlTbl::OnBnClickedCancel)
END_MESSAGE_MAP()


// COpenSpecificCtlTbl message handlers

void COpenSpecificCtlTbl::OnBnClickedOk()
{
	CString tempStr;
	mCtl.GetWindowText(tempStr);
	ctl = CSharedFunctions::StringHexToLong(tempStr);
	mTbl.GetWindowText(tempStr);
	tbl = CSharedFunctions::StringHexToLong(tempStr);
	OnOK();
}



void COpenSpecificCtlTbl::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


unsigned long COpenSpecificCtlTbl::GetCtl()
{
	
	return ctl;
}

unsigned long COpenSpecificCtlTbl::GetTbl()
{
	
	return tbl;
}