// Spinout
#pragma once

class MIO0
{
public:
	unsigned long mio0_decode ( unsigned char *src, unsigned char * destination, int& fileSizeCompressed );
	int mio0_get_size( unsigned char * src);
	bool CompressMIO0File(CString mainFolder, CString inputFile, CString outputFile);
	BOOL IsFileExist(LPSTR lpszFilename);
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
};
