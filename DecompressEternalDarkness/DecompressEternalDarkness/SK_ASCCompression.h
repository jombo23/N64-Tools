#pragma once
// Eternal Darkness Decompression
// Ghidra was the basis

class CSK_ASCCompression
{
public:
	CSK_ASCCompression(void);
	~CSK_ASCCompression(void);

	int DecompressFile(CString compressedFilename, CString outputFilename);
};
