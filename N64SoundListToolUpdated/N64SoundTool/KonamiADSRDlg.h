#pragma once
#include "afxwin.h"
#include "..\N64SoundLibrary\N64AIFCAudio.h"

// CKonamiADSRDlg dialog

class CKonamiADSRDlg : public CDialog
{
	DECLARE_DYNAMIC(CKonamiADSRDlg)

public:
	CKonamiADSRDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKonamiADSRDlg();

// Dialog Data
	enum { IDD = IDD_KONAMIADSR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit mInstrumentCoarseTune;
	CEdit mInstrumentFineTune;
	CEdit mInstrumentAttackTime;
	CEdit mInstrumentDecayTime;
	CEdit mInstrumentReleaseTime;
	CEdit mInstrumentUnknown1;
	CEdit mInstrumentUnknown2;
	CStatic mDrumCoarseTuneLabel;
	CStatic mDrumFineTuneLabel;
	CStatic mDrumAttackTimeLabel;
	CStatic mDrumDecayTimeLabel;
	CStatic mDrumReleaseTimeLabel;
	CStatic mDrumUnknown1Label;
	CStatic mDrumUnknown2Label;
	CStatic mDrumInstrumentLabel;
	CEdit mDrumCoarseTune;
	CEdit mDrumFineTune;
	CEdit mDrumAttackTime;
	CEdit mDrumDecayTime;
	CEdit mDrumReleaseTime;
	CEdit mDrumUnknown1;
	CEdit mDrumUnknown2;
	CEdit mDrumInstrument;
	CStatic mInstrumentCoarseTuneLabel;
	CStatic mInstrumentFineTuneLabel;
	CStatic mInstrumentAttackTimeLabel;
	CStatic mInstrumentDecayTimeLabel;
	CStatic mInstrumentReleaseTimeLabel;
	CStatic mInstrumentUnknown1Label;
	CStatic mInstrumentUnknown2Label;
	CComboBox mInstrument;
	CComboBox mDrum;
	afx_msg void OnCbnSelchangeComboinstrument();
	
	virtual BOOL OnInitDialog();

	ALBank* alBankCurrent;
	CStatic mDrumLabel;
	CStatic mInstrumentLabel;
	afx_msg void OnCbnSelchangeCombodrum();

	bool dontEdit;
	afx_msg void OnEnChangeEditcoarsetuneinstrument();
	afx_msg void OnEnChangeEditfinetuneinstrument2();
	afx_msg void OnEnChangeEditattacktimeinstrument();
	afx_msg void OnEnChangeEditdecaytimeinstrument();
	afx_msg void OnEnChangeEditreleasetimeinstrument();
	afx_msg void OnEnChangeEditunknown1instrument();
	afx_msg void OnEnChangeEditunknown2instrument();
	afx_msg void OnEnChangeEditdruminstrument();
	afx_msg void OnEnChangeEditcoarsetunedrum();
	afx_msg void OnEnChangeEditfinetunedrum();
	afx_msg void OnEnChangeEditattacktimedrum();
	afx_msg void OnEnChangeEditdecaytimedrum();
	afx_msg void OnEnChangeEditreleasetimedrum();
	afx_msg void OnEnChangeEditunknown1drum();
	afx_msg void OnEnChangeEditunknown2drum();
};
