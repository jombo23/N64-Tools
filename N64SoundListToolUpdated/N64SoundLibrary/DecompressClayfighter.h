#pragma once

class CDecompressClayfighter
{
public:
	CDecompressClayfighter(void);
	~CDecompressClayfighter(void);
	static int Decompress(byte *output, byte *input);
};
