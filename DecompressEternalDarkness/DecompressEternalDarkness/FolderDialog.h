#pragma once
#include <shlobj.h>

class CFolderDialog  
{
public:
	CFolderDialog();
	INT_PTR DoModal();
	bool GetFolder(CString& folderpath, const char* szCaption, HWND hOwner);
	CString GetPathName();
	CString folderpath;
};
