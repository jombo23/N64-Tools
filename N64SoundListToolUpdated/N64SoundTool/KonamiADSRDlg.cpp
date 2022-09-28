// KonamiADSRDlg.cpp : implementation file
//

#include "stdafx.h"
#include "N64SoundListTool.h"
#include "KonamiADSRDlg.h"
#include "..\N64SoundLibrary\SharedFunctions.h"

// CKonamiADSRDlg dialog

IMPLEMENT_DYNAMIC(CKonamiADSRDlg, CDialog)

CKonamiADSRDlg::CKonamiADSRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKonamiADSRDlg::IDD, pParent)
{
	
}

CKonamiADSRDlg::~CKonamiADSRDlg()
{
}

void CKonamiADSRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITCOARSETUNEINSTRUMENT, mInstrumentCoarseTune);
	DDX_Control(pDX, IDC_EDITFINETUNEINSTRUMENT2, mInstrumentFineTune);
	DDX_Control(pDX, IDC_EDITATTACKTIMEINSTRUMENT, mInstrumentAttackTime);
	DDX_Control(pDX, IDC_EDITDECAYTIMEINSTRUMENT, mInstrumentDecayTime);
	DDX_Control(pDX, IDC_EDITRELEASETIMEINSTRUMENT, mInstrumentReleaseTime);
	DDX_Control(pDX, IDC_EDITUNKNOWN1INSTRUMENT, mInstrumentUnknown1);
	DDX_Control(pDX, IDC_EDITUNKNOWN2INSTRUMENT, mInstrumentUnknown2);
	DDX_Control(pDX, IDC_LABELCOARSEDRUM, mDrumCoarseTuneLabel);
	DDX_Control(pDX, IDC_LABELFINETUNEDRUM, mDrumFineTuneLabel);
	DDX_Control(pDX, IDC_LABELATTACKDRUM, mDrumAttackTimeLabel);
	DDX_Control(pDX, IDC_LABELDECAYTIMEDRUM, mDrumDecayTimeLabel);
	DDX_Control(pDX, IDC_LABELRELEASETIMEDRUM, mDrumReleaseTimeLabel);
	DDX_Control(pDX, IDC_LABELUNKNOWNDRUM, mDrumUnknown1Label);
	DDX_Control(pDX, IDC_LABELUNKNOWNDRUM2, mDrumUnknown2Label);
	DDX_Control(pDX, IDC_EDITCOARSETUNEDRUM, mDrumCoarseTune);
	DDX_Control(pDX, IDC_EDITFINETUNEDRUM, mDrumFineTune);
	DDX_Control(pDX, IDC_EDITATTACKTIMEDRUM, mDrumAttackTime);
	DDX_Control(pDX, IDC_EDITDECAYTIMEDRUM, mDrumDecayTime);
	DDX_Control(pDX, IDC_EDITRELEASETIMEDRUM, mDrumReleaseTime);
	DDX_Control(pDX, IDC_EDITUNKNOWN1DRUM, mDrumUnknown1);
	DDX_Control(pDX, IDC_EDITUNKNOWN2DRUM, mDrumUnknown2);
	DDX_Control(pDX, IDC_LABELCOARSETUNEINSTRUMENT, mInstrumentCoarseTuneLabel);
	DDX_Control(pDX, IDC_LABELFINETUNEINSTRUMENT, mInstrumentFineTuneLabel);
	DDX_Control(pDX, IDC_LABELATTACKINSTRUMENT, mInstrumentAttackTimeLabel);
	DDX_Control(pDX, IDC_LABELDECAYTIMEINSTRUMENT, mInstrumentDecayTimeLabel);
	DDX_Control(pDX, IDC_LABELRELEASETIMEINSTRUMENT, mInstrumentReleaseTimeLabel);
	DDX_Control(pDX, IDC_LABELUNKNOWNINSTRUMENT1, mInstrumentUnknown1Label);
	DDX_Control(pDX, IDC_LABELUNKNOWNINSTRUMENT2, mInstrumentUnknown2Label);
	DDX_Control(pDX, IDC_COMBOINSTRUMENT, mInstrument);
	DDX_Control(pDX, IDC_COMBODRUM, mDrum);
	DDX_Control(pDX, IDC_LABELDRUMINSTRUMENT, mDrumInstrumentLabel);
	DDX_Control(pDX, IDC_EDITDRUMINSTRUMENT, mDrumInstrument);
	DDX_Control(pDX, IDC_DRUMLABEL, mDrumLabel);
	DDX_Control(pDX, IDC_INSTRUMENTLABEL, mInstrumentLabel);
}


BEGIN_MESSAGE_MAP(CKonamiADSRDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBOINSTRUMENT, &CKonamiADSRDlg::OnCbnSelchangeComboinstrument)
	ON_CBN_SELCHANGE(IDC_COMBODRUM, &CKonamiADSRDlg::OnCbnSelchangeCombodrum)
	ON_EN_CHANGE(IDC_EDITCOARSETUNEINSTRUMENT, &CKonamiADSRDlg::OnEnChangeEditcoarsetuneinstrument)
	ON_EN_CHANGE(IDC_EDITFINETUNEINSTRUMENT2, &CKonamiADSRDlg::OnEnChangeEditfinetuneinstrument2)
	ON_EN_CHANGE(IDC_EDITATTACKTIMEINSTRUMENT, &CKonamiADSRDlg::OnEnChangeEditattacktimeinstrument)
	ON_EN_CHANGE(IDC_EDITDECAYTIMEINSTRUMENT, &CKonamiADSRDlg::OnEnChangeEditdecaytimeinstrument)
	ON_EN_CHANGE(IDC_EDITRELEASETIMEINSTRUMENT, &CKonamiADSRDlg::OnEnChangeEditreleasetimeinstrument)
	ON_EN_CHANGE(IDC_EDITUNKNOWN1INSTRUMENT, &CKonamiADSRDlg::OnEnChangeEditunknown1instrument)
	ON_EN_CHANGE(IDC_EDITUNKNOWN2INSTRUMENT, &CKonamiADSRDlg::OnEnChangeEditunknown2instrument)
	ON_EN_CHANGE(IDC_EDITDRUMINSTRUMENT, &CKonamiADSRDlg::OnEnChangeEditdruminstrument)
	ON_EN_CHANGE(IDC_EDITCOARSETUNEDRUM, &CKonamiADSRDlg::OnEnChangeEditcoarsetunedrum)
	ON_EN_CHANGE(IDC_EDITFINETUNEDRUM, &CKonamiADSRDlg::OnEnChangeEditfinetunedrum)
	ON_EN_CHANGE(IDC_EDITATTACKTIMEDRUM, &CKonamiADSRDlg::OnEnChangeEditattacktimedrum)
	ON_EN_CHANGE(IDC_EDITDECAYTIMEDRUM, &CKonamiADSRDlg::OnEnChangeEditdecaytimedrum)
	ON_EN_CHANGE(IDC_EDITRELEASETIMEDRUM, &CKonamiADSRDlg::OnEnChangeEditreleasetimedrum)
	ON_EN_CHANGE(IDC_EDITUNKNOWN1DRUM, &CKonamiADSRDlg::OnEnChangeEditunknown1drum)
	ON_EN_CHANGE(IDC_EDITUNKNOWN2DRUM, &CKonamiADSRDlg::OnEnChangeEditunknown2drum)
END_MESSAGE_MAP()


// CKonamiADSRDlg message handlers

BOOL CKonamiADSRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int x = 0; x < alBankCurrent->konamiADSR.size(); x++)
	{
		CString tempStr;
		tempStr.Format("%02X", x);
		mInstrument.AddString(tempStr);

		mInstrument.SetCurSel(0);

		OnCbnSelchangeComboinstrument();
	}

	if (alBankCurrent->konamiDrumsADSR.size() > 0)
	{
		for (int x = 0; x < alBankCurrent->konamiDrumsADSR.size(); x++)
		{
			CString tempStr;
			tempStr.Format("%02X", x);
			mDrum.AddString(tempStr);
		}

		mDrum.SetCurSel(0);

		mDrumLabel.ShowWindow(SW_SHOW);
		mDrum.ShowWindow(SW_SHOW);

		OnCbnSelchangeCombodrum();
	}
	else
	{
		mDrumCoarseTuneLabel.ShowWindow(SW_HIDE);
		mDrumFineTuneLabel.ShowWindow(SW_HIDE);
		mDrumAttackTimeLabel.ShowWindow(SW_HIDE);
		mDrumDecayTimeLabel.ShowWindow(SW_HIDE);
		mDrumReleaseTimeLabel.ShowWindow(SW_HIDE);
		mDrumUnknown1Label.ShowWindow(SW_HIDE);
		mDrumUnknown2Label.ShowWindow(SW_HIDE);
		mDrumInstrumentLabel.ShowWindow(SW_HIDE);
		mDrumCoarseTune.ShowWindow(SW_HIDE);
		mDrumFineTune.ShowWindow(SW_HIDE);
		mDrumAttackTime.ShowWindow(SW_HIDE);
		mDrumDecayTime.ShowWindow(SW_HIDE);
		mDrumReleaseTime.ShowWindow(SW_HIDE);
		mDrumUnknown1.ShowWindow(SW_HIDE);
		mDrumUnknown2.ShowWindow(SW_HIDE);
		mDrumInstrument.ShowWindow(SW_HIDE);
		mDrumLabel.ShowWindow(SW_HIDE);
		mDrum.ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CKonamiADSRDlg::OnCbnSelchangeComboinstrument()
{
	int instrumentNumber = mInstrument.GetCurSel();

	CString tempStr;

	dontEdit = true;

	tempStr.Format("%02X", alBankCurrent->konamiADSR[instrumentNumber].coarseTune & 0xFF);
	mInstrumentCoarseTune.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiADSR[instrumentNumber].fineTune & 0xFF);
	mInstrumentFineTune.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiADSR[instrumentNumber].attackTime);
	mInstrumentAttackTime.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiADSR[instrumentNumber].decayTime);
	mInstrumentDecayTime.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiADSR[instrumentNumber].releaseTime);
	mInstrumentReleaseTime.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiADSR[instrumentNumber].unknownOne);
	mInstrumentUnknown1.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiADSR[instrumentNumber].unknownTwo);
	mInstrumentUnknown2.SetWindowText(tempStr);

	dontEdit = false;
}
void CKonamiADSRDlg::OnCbnSelchangeCombodrum()
{
	int drumNumber = mDrum.GetCurSel();

	dontEdit = true;

	CString tempStr;
	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].instrumentLookup);
	mDrumInstrument.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].coarseTune & 0xFF);
	mDrumCoarseTune.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].fineTune & 0xFF);
	mDrumFineTune.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].attackTime);
	mDrumAttackTime.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].decayTime);
	mDrumDecayTime.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].releaseTime);
	mDrumReleaseTime.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].unknownOne);
	mDrumUnknown1.SetWindowText(tempStr);

	tempStr.Format("%02X", alBankCurrent->konamiDrumsADSR[drumNumber].unknownTwo);
	mDrumUnknown2.SetWindowText(tempStr);

	dontEdit = false;

	mDrumCoarseTuneLabel.ShowWindow(SW_SHOW);
	mDrumFineTuneLabel.ShowWindow(SW_SHOW);
	mDrumAttackTimeLabel.ShowWindow(SW_SHOW);
	mDrumDecayTimeLabel.ShowWindow(SW_SHOW);
	mDrumReleaseTimeLabel.ShowWindow(SW_SHOW);
	mDrumUnknown1Label.ShowWindow(SW_SHOW);
	mDrumUnknown2Label.ShowWindow(SW_SHOW);
	mDrumInstrumentLabel.ShowWindow(SW_SHOW);
	mDrumCoarseTune.ShowWindow(SW_SHOW);
	mDrumFineTune.ShowWindow(SW_SHOW);
	mDrumAttackTime.ShowWindow(SW_SHOW);
	mDrumDecayTime.ShowWindow(SW_SHOW);
	mDrumReleaseTime.ShowWindow(SW_SHOW);
	mDrumUnknown1.ShowWindow(SW_SHOW);
	mDrumUnknown2.ShowWindow(SW_SHOW);
	mDrumInstrument.ShowWindow(SW_SHOW);
}

void CKonamiADSRDlg::OnEnChangeEditcoarsetuneinstrument()
{
	if (!dontEdit)
	{
		int instrumentNumber = mInstrument.GetCurSel();

		CString tempStr;
		mInstrumentCoarseTune.GetWindowText(tempStr);

		alBankCurrent->konamiADSR[instrumentNumber].coarseTune = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditfinetuneinstrument2()
{
	if (!dontEdit)
	{
		int instrumentNumber = mInstrument.GetCurSel();

		CString tempStr;
		mInstrumentFineTune.GetWindowText(tempStr);

		alBankCurrent->konamiADSR[instrumentNumber].fineTune = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditattacktimeinstrument()
{
	if (!dontEdit)
	{
		int instrumentNumber = mInstrument.GetCurSel();

		CString tempStr;
		mInstrumentAttackTime.GetWindowText(tempStr);

		alBankCurrent->konamiADSR[instrumentNumber].attackTime = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditdecaytimeinstrument()
{
	if (!dontEdit)
	{
		int instrumentNumber = mInstrument.GetCurSel();

		CString tempStr;
		mInstrumentDecayTime.GetWindowText(tempStr);

		alBankCurrent->konamiADSR[instrumentNumber].decayTime = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditreleasetimeinstrument()
{
	if (!dontEdit)
	{
		int instrumentNumber = mInstrument.GetCurSel();

		CString tempStr;
		mInstrumentReleaseTime.GetWindowText(tempStr);

		alBankCurrent->konamiADSR[instrumentNumber].releaseTime = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditunknown1instrument()
{
	if (!dontEdit)
	{
		int instrumentNumber = mInstrument.GetCurSel();

		CString tempStr;
		mInstrumentUnknown1.GetWindowText(tempStr);

		alBankCurrent->konamiADSR[instrumentNumber].unknownOne = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditunknown2instrument()
{
	if (!dontEdit)
	{
		int instrumentNumber = mInstrument.GetCurSel();

		CString tempStr;
		mInstrumentUnknown2.GetWindowText(tempStr);

		alBankCurrent->konamiADSR[instrumentNumber].unknownTwo = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditdruminstrument()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumInstrument.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].instrumentLookup = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditcoarsetunedrum()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumCoarseTune.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].coarseTune = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditfinetunedrum()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumFineTune.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].fineTune = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditattacktimedrum()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumAttackTime.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].attackTime = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditdecaytimedrum()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumDecayTime.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].decayTime = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditreleasetimedrum()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumReleaseTime.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].releaseTime = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditunknown1drum()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumUnknown1.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].unknownOne = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}

void CKonamiADSRDlg::OnEnChangeEditunknown2drum()
{
	if (!dontEdit)
	{
		int drumNumber = mDrum.GetCurSel();

		CString tempStr;
		mDrumUnknown2.GetWindowText(tempStr);

		alBankCurrent->konamiDrumsADSR[drumNumber].unknownTwo = CSharedFunctions::StringToUnsignedChar(tempStr);
	}
}
