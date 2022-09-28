// N64SoundListTool.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CN64SoundListToolApp:
// See N64SoundListTool.cpp for the implementation of this class
//

class CN64SoundListToolApp : public CWinAppEx
{
public:
	CN64SoundListToolApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CN64SoundListToolApp theApp;