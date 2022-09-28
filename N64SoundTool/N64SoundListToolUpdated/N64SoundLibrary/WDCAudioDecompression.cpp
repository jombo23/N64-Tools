#include "StdAfx.h"
#include "WDCAudioDecompression.h"
#include <math.h>

void CWDCAudioDecompression::ZSortBOSS_Audio1(unsigned char* RAM, unsigned char* DMEM, unsigned long _w0, unsigned long _w1 )
{
	unsigned long addr = _w1 & 0xFFFFFF;
	unsigned long val = CSharedFunctions::Flip32Bit(((unsigned long*)DMEM)[(_w0 & 0xfff) >> 2]);
	((unsigned long*)DMEM)[0] = CSharedFunctions::Flip32Bit(val);

	memcpy(RAM+addr, DMEM, 0x8);
}

void CWDCAudioDecompression::ZSortBOSS_Audio2(unsigned char* RAM, unsigned char* DMEM, unsigned long _w0, unsigned long _w1, int lengthOverride, int DMEMDST, int DMEMSPECIALVALUE)
{
	int len = _w1 >> 24;
	if (lengthOverride != -1)
		len = lengthOverride;

	// Written by previous ZSortBOSS_MoveWord
	unsigned long dst = CSharedFunctions::Flip32Bit(((unsigned long*)DMEM)[0x10>>2]);

	float f1 = (float)((_w0>>16) & 0xff) + (float)(_w0 & 0xffff) / 65536.f;
	float f2 = (float)((_w1>>16) & 0xff) + (float)(_w1 & 0xffff) / 65536.f;

	// Written by previous ZSortBOSS_MoveWord
	unsigned short v11[2];
	v11[0] = CSharedFunctions::Flip16Bit(((unsigned short*)DMEM)[(DMEMSPECIALVALUE>>1)]);
	v11[1] = CSharedFunctions::Flip16Bit(((unsigned short*)DMEM)[((DMEMSPECIALVALUE+2)>>1)]);

	for(int i = 0; i < len; i+=4) {

		for(int j = 0; j < 4; j++) {

			float intpart, fractpart;
			float val = i*f1 + j*f1 + f2;

			fractpart = fabsf(modff(val, &intpart));
			int index = ((int)intpart) << 1;

			signed short v9 = CSharedFunctions::Flip16Bit(((signed short*)DMEM)[((DMEMDST+index)>>1)]);
			signed short v10 = CSharedFunctions::Flip16Bit(((signed short*)DMEM)[((DMEMDST + 2+index)>>1)]);
			signed short v12 = v10 - v9;
			signed short v13 = CSharedFunctions::Flip16Bit(((signed short*)DMEM)[(dst>>1)]);

			for(int k = 0; k < 2; k++) {

				signed long res1 = v12 * (unsigned short)(fractpart * 65536.f);
				signed long res2 = v9 << 16;
				signed short res3 = (res2 + res1) >> 16;

				res1 = v11[k] * res3;
				res2 = v13 << 16;
				res3 = (res2 + res1) >> 16;

				((signed short*)DMEM)[(dst>>1)] = CSharedFunctions::Flip16Bit(res3);
				dst+=2;
			}
		}
	}
	dst = dst;
}

void CWDCAudioDecompression::ZSortBOSS_Audio3(unsigned char* RAM, unsigned char* DMEM, ZSortBOSSState& gstate, unsigned long _w0, unsigned long _w1 )
{

	unsigned long addr = _w0 & 0xFFFFFF;

	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			gstate.table[i][j] = CSharedFunctions::Flip16Bit(((signed short*)RAM)[((addr+(i<<4)+(j<<1))>>1)]);
		}
	}

	addr = _w1 & 0xFFFFFF;

	// What is this?
	memcpy(DMEM, (RAM + addr), 0x8);
	addr = CSharedFunctions::Flip32Bit(addr) & 0xFFFFFF;
	memcpy((DMEM+8), &addr, sizeof(addr));

}

void CWDCAudioDecompression::ZSortBOSS_Audio4(unsigned char* RAM, unsigned char* DMEM, ZSortBOSSState& gstate, unsigned long _w0, unsigned long _w1, int inputSizeOverride, int DMEMDST )
{

	unsigned long addr = _w1 & 0xFFFFFF;

	unsigned long src = ((_w0 & 0xf000) >> 12) + addr;
	signed short * dst = (signed short*)(DMEM+DMEMDST);
	int len = (_w0 & 0xfff);
	if (inputSizeOverride != -1)
		len = inputSizeOverride;

	// Written by previous ZSortBOSS_MoveWord
	signed short v1 = CSharedFunctions::Flip16Bit(((signed short*)DMEM)[(0>>1)]);
	signed short v2 = CSharedFunctions::Flip16Bit(((signed short*)DMEM)[(2>>1)]);

	for(int l1 = len; l1 != 0; l1-=9) {

		unsigned long r9 = ((unsigned char*)RAM)[(src++)];
		int index = (r9 & 0xf) << 1;

		if(index > 6) {
			//LOG(LOG_VERBOSE, "ZSortBOSS_Audio4: Index out of bound");
			break;
		}

		signed short c1 = 1 << ((r9>>4) & 0x1f);
		signed short c2 = 0x20;

		for(int l2 = 0; l2 < 2; l2++) {

			signed long a = ((char*)RAM)[(src++)];
			signed long b = ((char*)RAM)[(src++)];
			signed long c = ((char*)RAM)[(src++)];
			signed long d = ((char*)RAM)[(src++)];

			signed short coeff[8];
			coeff[0] = a>>4;
			coeff[1] = (a<<28)>>28;
			coeff[2] = b>>4;
			coeff[3] = (b<<28)>>28;
			coeff[4] = c>>4;
			coeff[5] = (c<<28)>>28;
			coeff[6] = d>>4;
			coeff[7] = (d<<28)>>28;

			int i, j, k;

			for(i = 0; i < 8; i++) {
				signed long res1 = 0;

				for(j = 0, k = i; j < i; j++, k--)
					res1 += gstate.table[index+1][k-1] * coeff[j];

				res1 += coeff[i] * (signed short)0x0800;
				res1 *= c1;

				signed long res2 = v1*gstate.table[index][i] + v2*gstate.table[index+1][i];

				dst[i] = CSharedFunctions::Flip16Bit((res1*c2 + res2*c2) >> 16);
			}
			v1 = CSharedFunctions::Flip16Bit(dst[6]);
			v2 = CSharedFunctions::Flip16Bit(dst[7]);
			dst += 8;
		}
	}
}

bool CWDCAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
{
	float samplingRateFloat = samplingRate;

	int flags = 1; //(waveType == 5); //0 = 8bit, 1=16bit

	FILE* outFileTempRaw = fopen(outputFilename, "wb");
	if (outFileTempRaw == NULL)
	{
		MessageBox(NULL, "Cannot open temporary file", "Error", NULL);
		return false;
	}

	unsigned char wavHeader[0x28];

	wavHeader[0x0] = 0x52;
	wavHeader[0x1] = 0x49;
	wavHeader[0x2] = 0x46;
	wavHeader[0x3] = 0x46;
	unsigned long chunkSize = 0x28 + processedWavDataLength;
	wavHeader[0x4] = ((chunkSize >> 0) & 0xFF);
	wavHeader[0x5] = ((chunkSize >> 8) & 0xFF);
	wavHeader[0x6] = ((chunkSize >> 16) & 0xFF);
	wavHeader[0x7] = ((chunkSize >> 24) & 0xFF);
	wavHeader[0x8] = 0x57;
	wavHeader[0x9] = 0x41;
	wavHeader[0xA] = 0x56;
	wavHeader[0xB] = 0x45;
	wavHeader[0xC] = 0x66;
	wavHeader[0xD] = 0x6D;
	wavHeader[0xE] = 0x74;
	wavHeader[0xF] = 0x20;
	wavHeader[0x10] = 0x10;
	wavHeader[0x11] = 0x00;
	wavHeader[0x12] = 0x00;
	wavHeader[0x13] = 0x00;
	wavHeader[0x14] = 0x01;
	wavHeader[0x15] = 0x00;
	wavHeader[0x16] = 0x01;
	wavHeader[0x17] = 0x00;
	wavHeader[0x18] = (((unsigned long)samplingRateFloat >> 0) & 0xFF);
	wavHeader[0x19] = (((unsigned long)samplingRateFloat >> 8) & 0xFF);
	wavHeader[0x1A] = (((unsigned long)samplingRateFloat >> 16) & 0xFF);
	wavHeader[0x1B] = (((unsigned long)samplingRateFloat >> 24) & 0xFF);
	wavHeader[0x1C] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 0) & 0xFF);
	wavHeader[0x1D] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 8) & 0xFF);
	wavHeader[0x1E] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 16) & 0xFF);
	wavHeader[0x1F] = ((((unsigned long)samplingRateFloat * (1 + (flags & 1))) >> 24) & 0xFF);
	wavHeader[0x20] = (1 + (flags & 1));
	wavHeader[0x21] = 0x00;
	wavHeader[0x22] = (((flags & 1) + 1) * 8);
	wavHeader[0x23] = 0x00;
	wavHeader[0x24] = 0x64;
	wavHeader[0x25] = 0x61;
	wavHeader[0x26] = 0x74;
	wavHeader[0x27] = 0x61;

	fwrite(wavHeader, 1, 0x28, outFileTempRaw );

	unsigned long length = processedWavDataLength;
	fwrite(&length, 1, 4, outFileTempRaw);

	if ((flags & 1) == 1)
	{
		fwrite(processedWavData, 1, processedWavDataLength, outFileTempRaw);
	}
	else
	{
		for (int r = 0; r < processedWavDataLength; r++)
		{
			int value = (signed char)processedWavData[r];
			value += 128;
			fwrite(&value, 1, 1, outFileTempRaw);

		}
	}

	fflush(outFileTempRaw);
	fclose(outFileTempRaw);

	return true;
}

CWDCAudioDecompression::CWDCAudioDecompression(void)
{
}

CWDCAudioDecompression::~CWDCAudioDecompression(void)
{
}

bool CWDCAudioDecompression::DecompressSound(unsigned char* romSampleData, unsigned char* predictors, int inputSize, int outputSize, CString outputFilename, float samplingRateFloat)
{
	ZSortBOSSState gstate;

	int sizeRAM = 0x800000;
	unsigned char* RAM = new unsigned char[sizeRAM];
	for (int x = 0; x < sizeRAM; x++)
		RAM[x] = 0x00;


	int sizeDMEM = 0x1000000;
	unsigned char* DMEM = new unsigned char[sizeDMEM];
	for (int x = 0; x < sizeDMEM; x++)
		DMEM[x] = 0x00;

	int DMEMDST = 0x30; //0x30;
	int DMEMSPECIALVALUE = 0x100030; //0x904;
	int DMEMOUTPUTSPOT = 0x800C20; // 0xC20
	int RDRAMOUTPUTSPECIALVALUE = 0x80000010; // 0x803F4400
	int RDRAMINPUTSPOT = 0x80200000;//0x003E0798;

	memcpy(&RAM[0xCAFB0], predictors, 0x80);
	memcpy(&RAM[RDRAMINPUTSPOT & 0xFFFFFF], romSampleData, inputSize);

	CSharedFunctions::WriteLongToBuffer(DMEM, 0x10, DMEMOUTPUTSPOT);
	ZSortBOSS_Audio3(RAM, DMEM, gstate, 0x220CAFB0, RDRAMOUTPUTSPECIALVALUE);
	CSharedFunctions::WriteLongToBuffer(DMEM, DMEMSPECIALVALUE, 0x7FFF7FFF);
	CSharedFunctions::WriteLongToBuffer(DMEM, 0x0, 0x00000000);
	ZSortBOSS_Audio4(RAM, DMEM, gstate, 0x24000000, RDRAMINPUTSPOT, inputSize, DMEMDST);
	ZSortBOSS_Audio1(RAM, DMEM, 0x260000AC, RDRAMOUTPUTSPECIALVALUE);
	ZSortBOSS_Audio2(RAM, DMEM, 0x1C00B7B2, 0x80000000, outputSize, DMEMDST, DMEMSPECIALVALUE);
	//fwrite(&DMEM[DMEMOUTPUTSPOT], 1, outputSize, outFileDMEM);

	unsigned char* outputSoundData = new unsigned char[outputSize];
	for (int x = 0; x < outputSize; x += 2)
	{
		// Little Endian
		outputSoundData[x] = DMEM[DMEMOUTPUTSPOT + x + 1];
		outputSoundData[x + 1] = DMEM[DMEMOUTPUTSPOT + x];
	}

	SaveProcessedWav(outputSoundData, outputSize, 1, 44050, outputFilename);
	delete [] outputSoundData;

	delete [] RAM;
	delete [] DMEM;

	return true;
}