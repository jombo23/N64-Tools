#pragma once
#include "afxwin.h"
#include "StartupParameters.h"

class CCustomCommandLineInfo :
	public CCommandLineInfo
{
public:
	CCustomCommandLineInfo(void);
	~CCustomCommandLineInfo(void);
	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);

	CStartupParameters startupParameters;
};
