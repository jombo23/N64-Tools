
// ObjToAn8.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CObjToAn8App:
// See ObjToAn8.cpp for the implementation of this class
//

class CObjToAn8App : public CWinAppEx
{
public:
	CObjToAn8App();

// Overrides
	public:
	virtual BOOL InitInstance();

	virtual int ExitInstance( );

	int returnExitCode;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CObjToAn8App theApp;