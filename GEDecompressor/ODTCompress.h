// BY ZOINKITY
#pragma once

class CODTCompress
{
public:
	CODTCompress(void);
	~CODTCompress(void);
	int decode(FILE * olde, int offset, CString filename, int& fileSizeCompressed);
	bool encode(CString inFileName, CString outFileName);
};
