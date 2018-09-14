// BY ZOINKITY
#pragma once

class CEDLCompress
{
public:
	CEDLCompress(void);
	~CEDLCompress(void);
	int decode(FILE * in, int offset, CString filename, int& fileSizeCompressed, CString& type);
	bool encode(CString inFileName, CString outFileName);
};
