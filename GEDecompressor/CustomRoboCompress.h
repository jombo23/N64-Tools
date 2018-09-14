// BY ZOINKITY
#pragma once

class CCustomRoboCompress
{
public:
	CCustomRoboCompress(void);
	~CCustomRoboCompress(void);
	int decode(FILE * olde, int offset, CString filename, int& fileSizeCompressed);
	bool encode(CString inFileName, CString outFileName);
};
