// N64MidiTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CN64MidiToolApp:
// See N64MidiTool.cpp for the implementation of this class
//

class CN64MidiToolApp : public CWinAppEx
{
public:
	CN64MidiToolApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CN64MidiToolApp theApp;