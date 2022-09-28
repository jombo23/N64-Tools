
#include <shlobj.h>

class CFolderDialog  
{
public:
	int DoModal();
	bool GetFolder(CString& folderpath, const char* szCaption, HWND hOwner);
	CString GetPathName();
	CString folderpath;
};
