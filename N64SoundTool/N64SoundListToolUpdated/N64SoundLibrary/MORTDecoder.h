// Reverse Engineered by SubDrag from Pokemon Stadium
#pragma once

#include <vector>

class CMORTDecoder
{
public:
	CMORTDecoder(void);
	~CMORTDecoder(void);

public:
	bool Decode(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, std::vector<unsigned short>& pcmSamples);

	static unsigned short CharArrayToShort(unsigned char* currentSpot);
	static unsigned long CharArrayToLong(unsigned char* currentSpot);
	static void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data);
	static void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data);
	static unsigned short Flip16Bit(unsigned short ShortValue);
	static unsigned long Flip32Bit(unsigned long inLong);
	static void WriteLongToFile(FILE* Buffer, unsigned long data);
	static void WriteShortToFile(FILE* Buffer, unsigned short data);

private:
	unsigned char* buffer800D2940Predictor;
	unsigned long buffer800D2940Subtraction;
	//800D2AD0 Pointer to function 800455DC (pull data?)
	// Need evaluate if these are part of buffer or not and the rest around there
	unsigned long variable800D2AD0PullDataPointer;
	unsigned long variable800D2AD4ROMAddressMORTData;
	unsigned char* buffer800D2AD8IntermediateValue;
	unsigned long buffer800D2AD8Subtraction;
	unsigned char* buffer800D3ED8MORTRawInputDataBuffer; // TWINE 800E1C60
	unsigned long buffer800D3ED8Subtraction;
	unsigned short variable800D4ED8Amountofsoundleft1_FirstWord; // TWINE 800E2C60
	unsigned short variable800D4EDA_3E80FirstWord;
	unsigned long variable800D4EDCAmountofCompressedWordsLeft_SecondWord;
	unsigned long variable800D4EE0;
	unsigned long variable800D4EE4;
	unsigned long variable800D4EE8;
	unsigned long variable800D4EEC;
	unsigned long variable800D4EF0;
	unsigned long variable800D4EF4;
	unsigned long variable800D4EFC;
	unsigned long variable800D4F00InputChunkUsed; // TWINE 800E2C8C
	unsigned long variable800D4F04InputChunkAmountLeft; // TWINE 800E2C90
	unsigned long variable800D4F08;
	unsigned long variable800D4F0C; //// TWINE 800E2C98
	unsigned char variable800D4F10Status1; // TWINE 800E2C9C
	unsigned char variable800D4F11Status2; //01 loading, 02 loaded, 03 playing, 04 ended 
	unsigned char variable800D4F12Status3; // TWINE 800E2C9E
	unsigned char* buffer800D4F20OutputBuffer;
	unsigned long buffer800D4F20Subtraction;

	unsigned long variable800FCED0;
	unsigned long variable800FCEDCPredictorPointer800D2940;
	unsigned long variable800FCEE4IntermediateValuePointer800D2AD8;
	unsigned long variable800FCEE8;
	unsigned long variable800FCEECCounter;
	unsigned char variable800FCEF0MORTStatus1; //Status of sound, 00 = Loading, 02 = none, 04 = Playing
	unsigned char variable800FCEF1MORTStatus2; //Status of sound, 00 = Loading, 02 = none, 04 = Playing
	unsigned long variable800FCEFCROMMORTSoundAddress;
	unsigned long variable800FCF04;
	unsigned long variable800FCF0CVolume;
	unsigned char variable800FCF20;
	unsigned char variable800FCF21;
	unsigned long variable800FCF2COutputBufferPointer800D4F20;
	unsigned long variable800FCF30CounterPlay1;
	unsigned long variable800FCF34CounterPlay2;
	unsigned long variable800FCF38CounterIncrementVariable1;
	unsigned long variable800FCF3CCounterIncrementVariable2;

	void Function800456D0(unsigned long A0Param);
	void Function80045780(unsigned char* ROM, unsigned long A0Param);
	void Function800459E0(unsigned long A0Param, unsigned long A1Param, unsigned long& V0);
	void Function8005E3A0(unsigned long A1Param, unsigned long A2Param);
	void Function80057FD0(unsigned long A0Param, unsigned long A1Param);
	void Function80059120(unsigned long A0Param, unsigned long A1Param);
	void Function800455DC(unsigned char* ROM, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param);
	void Function8005E2F0(unsigned long A0Param, unsigned long A1Param);
	void Function8005E0F0();
	void Function80056AD0(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long& V0);
	void Function80062A90(unsigned long& V0);
	void Function80045FF0(unsigned long A0Param, unsigned long A1Param);
	void Function80045C78(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned long T8Param, unsigned char stackBuffer[0x100]);
	void Function80048590(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long T8Param, unsigned char stackBuffer[0x100], unsigned char stackBuffer2[0x50]);
	void Function80048684(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned char stackBuffer2[0x50]);
	void Function80045A80(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned char stackBuffer[0x100]);
	void Function80048904(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param);
	void CallT380048XXXFunction(unsigned long& T2, unsigned long T3, unsigned long& T4);
	void Function80048AFC(unsigned long& T2, unsigned long& T4);
	void Function80048B14(unsigned long& T2, unsigned long& T4);
	void Function80048B24(unsigned long& T2, unsigned long& T4);
	void Function80048B3C(unsigned long& T2, unsigned long& T4);
	void Function80048740(unsigned long T0Param, unsigned long T1Param, unsigned long T3Param, unsigned long T6Param, unsigned long T7Param, unsigned long T8Param, unsigned long T9Param, unsigned long& A3, unsigned long& S0, unsigned long& S1, unsigned long& S2, unsigned long& S3, unsigned long& S4, unsigned long& S5, unsigned long& S6, unsigned long& S7);
	void Function80048A58(unsigned long A0Param, unsigned long T3Param, unsigned long& T6, unsigned long& T7, unsigned long& T8, unsigned long& T9);
	void Function80045A48(unsigned long A0Param);
	void Function80045A68(unsigned long A0Param);

	FILE* outDebug;
	bool setInputChunk;
};
