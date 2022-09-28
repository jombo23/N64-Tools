// Painstakingly decoded by SubDrag
#pragma once

#include <vector>
#include <algorithm>

class CKobe2ADPCMDecoder
{
public:
	CKobe2ADPCMDecoder(void);
	~CKobe2ADPCMDecoder(void);
public:
	bool Decode(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, std::vector<unsigned short>& pcmSamples, int variable20_S2, int soundType);
	unsigned short CharArrayToShort(unsigned char* currentSpot);
	unsigned long CharArrayToLong(unsigned char* currentSpot);
	unsigned short Flip16Bit(unsigned short ShortValue);
	unsigned long Flip32Bit(unsigned long inLong);
	void WriteLongToFile(FILE* outFile, unsigned long data);
	void WriteShortToFile(FILE* outFile, unsigned short data);
	void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
private:
	void Function800839C0(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer, std::vector<unsigned short>& pcmSamples, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param);
	void Function8008102C(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer, std::vector<unsigned short>& pcmSamples, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param);
	void Function80083854(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param);
	void Function80084330(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param);
	void Function80086080(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long SPParam);
	void Function80085AD0(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long SPParam);
	void Function80085370(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long SPParam);
	void Function800859BC(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long SPParam);
	void Function80085890(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long SPParam);
	void Function80085600(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned long SPParam);
	void Function80084EB0(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long& V0, unsigned long SPParam);
	void Function80085BE0(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long SPParam);
	void Function80085220(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long& V0);
	void Function80084DB0(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long SPParam);
	void Function80084D00(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long& V0);
	void Function800857D0(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned long SPParam);
	void Function80085B70(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long& V0, unsigned long SPParam);
	void Function80085030(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned long SPParam);
	void Function80085720(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long& V0, unsigned long SPParam);
	void Function80084B2C(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long SPParam);
	void Function800861D0(unsigned char* ROM, int romSize, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned long SPParam);
	void Function80084D60(unsigned char* ROM, int romSize, unsigned char* Buffer, std::vector<unsigned short>& pcmSamples, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param);
	void Function80084400(unsigned char* ROM, int romSize, unsigned char* Buffer, std::vector<unsigned short>& pcmSamples, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long SPParam);
	void Function800840A0(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer, std::vector<unsigned short>& pcmSamples, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned long& V0);
	void Function80086180(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer, unsigned long A0Param, unsigned long A1Param, unsigned long SPParam);
	void Function80084820(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer, unsigned long SPParam);
	void Function80086048(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer, unsigned long SPParam);
	void Function80085250(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer, unsigned long SPParam);
	void Function800842F8(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, unsigned char* Buffer);

	unsigned long variable801242EC_C_S2;
	unsigned long variable801242F0_10_S2_CompressedDataSpot;
	unsigned long variable801242F8_18_S2;
	unsigned long variable80124300_20_S2;
	unsigned long variable8012430C_2C_S2;
	unsigned short variable80124320_40_S2;

	unsigned long variable_801283B4;
	unsigned long variable_80128454;
	unsigned long variable_80128458;
	unsigned long variable_801283B0_ROM_Location;
	unsigned long variable_80124338;
	unsigned long variable_8012433C;
};
