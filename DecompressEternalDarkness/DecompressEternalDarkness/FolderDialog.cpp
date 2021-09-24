#include "stdafx.h"
#include "FolderDialog.h"

CString defaultFolderPathSave;

CFolderDialog::CFolderDialog()
{
	folderpath = "";
}

INT_PTR CFolderDialog::DoModal()
{
	if (GetFolder(folderpath, "Choose a Folder...", NULL))
	{
		return IDOK;
	}
	else
	{
		return IDCANCEL;
	}
}

CString CFolderDialog::GetPathName()
{
	return folderpath;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,
    UINT uMsg,LPARAM lParam,LPARAM lpData)
{
    // Look for BFFM_INITIALIZED
    if(uMsg == BFFM_INITIALIZED)
    {
        SendMessage(hwnd, BFFM_SETSELECTION,
            TRUE,(LPARAM)_T((LPCTSTR)defaultFolderPathSave));
    }
    return 0;
}


bool CFolderDialog::GetFolder(CString& folderpath, const char* szCaption, HWND hOwner)
{
	defaultFolderPathSave = folderpath;
	bool retVal = false;

	// The BROWSEINFO struct tells the shell 
	// how it should display the dialog.
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags   = BIF_USENEWUI;
	bi.hwndOwner = hOwner;
	bi.lpszTitle = szCaption;

	// Set the callback function
	bi.lpfn = BrowseCallbackProc;

	// must call this if using BIF_USENEWUI
	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the selected folder.
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

	if(pIDL != NULL)
	{
		// Create a buffer to store the path, then get the path.
		char buffer[_MAX_PATH] = {'\0'};
		if(::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			folderpath = buffer;
			retVal = true;
		}		

		// free the item id list
		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();

	return retVal;
}
