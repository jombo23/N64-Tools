// BY ZOINKITY

#pragma once

struct Triplet
{
	unsigned char zero;
	unsigned char one;
	unsigned char two;

	Triplet() : zero(0), one(0), two(0) { };
	Triplet(unsigned char zeroIn, unsigned char oneIn, unsigned char twoIn) : zero(zeroIn), one(oneIn), two(twoIn){ };
};

class CVPK0Decoder
{
public:
	CVPK0Decoder(void);
	~CVPK0Decoder(void);
	int  _flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, int compressedSize);
	unsigned long _nbits(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, int num, int compressedSize);
	int dec(unsigned char* inputBuffer, int compressedSize, int dec_s, unsigned char* output);
	int _tblsel(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, Triplet*& tbl, int& tripletCount, int compressedSize, int idx=-1);
	void _tblbuild(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, Triplet*& tbl, int& tripletCount, int compressedSize);
	int header(unsigned char* data, int& mode, unsigned long& dec_s);

	bool CompressVPK0File(CString mainFolder, CString inputFile, CString outputFile);
	bool CompressVPK0File(CString mainFolder, CString inputFile, CString outputFile, CString level);
	BOOL IsFileExist(LPSTR lpszFilename);
	BOOL hiddenExec (PTSTR pCmdLine, CString currentDirectory);
};
