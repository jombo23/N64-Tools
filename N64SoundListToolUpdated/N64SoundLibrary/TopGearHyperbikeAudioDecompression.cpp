#include "StdAfx.h"
#include "TopGearHyperbikeAudioDecompression.h"
#include "SharedFunctions.h"

CTopGearHyperbikeAudioDecompression::CTopGearHyperbikeAudioDecompression(void)
{
}

CTopGearHyperbikeAudioDecompression::~CTopGearHyperbikeAudioDecompression(void)
{
}


void CTopGearHyperbikeAudioDecompression::FUN_000481fc(unsigned char* RAM, unsigned long param_1, int param_2)
{
	//param_1 short *
  byte bVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  unsigned int uVar9;
  __int64 lVar10;
  int iVar11;
  unsigned int uVar12;
  unsigned int uVar13;
  unsigned long psVar14; //short *
  unsigned long psVar15; //short *
  int iVar16;
  int iVar17;
  unsigned long puVar18; //int*
  int iVar19;
  int iVar20;


  iVar19 = 0;
  puVar18 = 0x8009a0e0;
  do {
	  if ((CSharedFunctions::CharArrayToChar(RAM, puVar18 + 0x2b * 4) & 3) == 3) {
      //FUN_0005db50(*puVar18,0x2c0);
    }
    iVar19 = iVar19 + 1;
    puVar18 = puVar18 + 0x2c * 4;
  } while (iVar19 < 8);
  iVar19 = 0;
  iVar20 = 0;

  // hacked in
  iVar19 = 3;
  iVar20 = iVar20 + 0xb0 * 3;
  // end hacked in

  do {
	  iVar4 = CSharedFunctions::CharArrayToShort(RAM, 0x8009a0f0 + iVar20) * 0x18;
	  if ((CSharedFunctions::CharArrayToChar(RAM, 0x8009a18c + iVar20) & 2) != 0) {
      if ((CSharedFunctions::CharArrayToChar(RAM, 0x8009a18c + iVar20) & 1) != 0) {
		lVar10 = (__int64)(short)CSharedFunctions::CharArrayToShort(RAM, 0x8009a188 + iVar20);
		iVar7 = (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a184 + iVar20);
        iVar3 = (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e0 + iVar20);
        iVar17 = (int)CSharedFunctions::CharArrayToLong(RAM, iVar4 + 0x800C6514) -
                 ((int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e4 + iVar20) >> 10 & 0xfffffffcU);
        if (iVar19 == 0) {
			bVar1 = CSharedFunctions::CharArrayToChar(RAM, 0x8009a18e + iVar20);
          iVar6 = (int)CSharedFunctions::CharArrayToLong(RAM, 0x800742c4);
        }
        else {
          bVar1 = CSharedFunctions::CharArrayToChar(RAM, 0x8009a18e + iVar20);
          iVar6 = (int)CSharedFunctions::CharArrayToLong(RAM, 0x800742c0);
        }
        bVar2 = CSharedFunctions::CharArrayToChar(RAM, 0x8009a18f + iVar20);
        uVar12 = (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e4 + iVar20) >> 10 & 3;
        uVar13 = CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e4 + iVar20) & 0x3ff;
        if (0 < param_2) {
          psVar14 = param_1 + 1 * 2;
          iVar8 = iVar7 * 2;
          iVar11 = iVar7;
          psVar15 = param_1;
          iVar16 = param_2;
          do {
			  iVar7 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar15) + ((iVar8 >> 1) * ((int)((unsigned int)bVar1 * iVar6) >> 8) >> 8);
            iVar8 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, psVar14) + ((iVar8 >> 1) * ((int)((unsigned int)bVar2 * iVar6) >> 8) >> 8);

            if (iVar7 < (int)0x8000) {
              if (iVar7 < (int)(short)-0x8000) {
				  CSharedFunctions::WriteShortToBuffer(RAM, psVar15, (short)-0x8000);
              }
              else {
                CSharedFunctions::WriteShortToBuffer(RAM, psVar15, (short)iVar7);
              }
            }
            else {
              CSharedFunctions::WriteShortToBuffer(RAM, psVar15, (short)0x7fff);
            }
            /*if (iVar8 < (int)0x8000) {
              if (iVar8 < (int)(short)-0x8000) {
                CSharedFunctions::WriteShortToBuffer(RAM, psVar14, (short)-0x8000);
              }
              else {
                CSharedFunctions::WriteShortToBuffer(RAM, psVar14, (short)iVar8);
              }
            }
            else {
              CSharedFunctions::WriteShortToBuffer(RAM, psVar14, 0x7fff);
            }*/
            psVar14 = psVar14 + 2 * 2;
            psVar15 = psVar15 + 1 * 2; //2 * 2;
            iVar7 = iVar11;
			for (uVar13 = uVar13 + (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e8 + iVar20); 0x3ff < (int)uVar13; uVar13 = uVar13 - 0x400) {
              iVar8 = (int)lVar10;
              if (iVar17 <= (int)uVar12) {
                iVar7 = 0;
                if ((int)CSharedFunctions::CharArrayToLong(RAM, iVar4 + -0x7ff39ae8) == -1) {
					CSharedFunctions::WriteCharToBuffer(RAM, 0x8009a18c + iVar20, CSharedFunctions::CharArrayToChar(RAM, 0x8009a18c + iVar20) & 0xfd);
                  break;
                }
				CSharedFunctions::WriteLongToBuffer(RAM,  0x8009a0e4 + iVar20, 
                     (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e4 + iVar20) +
                     ((int)CSharedFunctions::CharArrayToLong(RAM, iVar4 + -0x7ff39aec) - (int)CSharedFunctions::CharArrayToLong(RAM, iVar4 + -0x7ff39ae8)) * -0x400);
                iVar7 = (int)CSharedFunctions::CharArrayToLong(RAM, iVar4 + -0x7ff39ae0);
				iVar8 = (int)(short)CSharedFunctions::CharArrayToShort(RAM, iVar4 + -0x7ff39ae2);
                uVar12 = (short)CSharedFunctions::CharArrayToShort(RAM, 0x8009a18a + iVar20) * 2 +
                         (CSharedFunctions::CharArrayToLong(RAM, iVar4 + -0x7ff39ae8) & 3);
                iVar17 = uVar12 + ((int)CSharedFunctions::CharArrayToLong(RAM, iVar4 + -0x7ff39aec) - (int)CSharedFunctions::CharArrayToLong(RAM, iVar4 + -0x7ff39ae8)) ;
              }
			  iVar5 = (unsigned int)CSharedFunctions::CharArrayToChar(RAM, iVar3 + ((int)uVar12 >> 1)) << 0x18;
              uVar9 = iVar5 >> 0x18;
              if ((uVar12 & 1) == 0) {
                uVar9 = iVar5 >> 0x1c;
              }
              iVar5 = (unsigned int)(short)CSharedFunctions::CharArrayToShort(RAM, iVar8 * 2 + 0x800742D4) << 0x10;
              iVar5 = (iVar5 >> 0x10 & (int)(uVar9 << 0x1d) >> 0x1f) +
                      (iVar5 >> 0x11 & (int)(uVar9 << 0x1e) >> 0x1f) +
                      (iVar5 >> 0x12 & (int)(uVar9 << 0x1f) >> 0x1f) + (iVar5 >> 0x13);
              if ((uVar9 & 8) != 0) {
                iVar5 = -iVar5;
              }
              iVar7 = iVar7 + iVar5;
              uVar9 = iVar8 + (short)CSharedFunctions::CharArrayToShort(RAM, (uVar9 & 7) * 2 + 0x80074378);
              lVar10 = (__int64)(int)(uVar9 & (int)~uVar9 >> 0x1f);
              if (0x50 < lVar10) {
                lVar10 = 0x50;
              }
              uVar12 = uVar12 + 1;
            }
            iVar16 = iVar16 + -1;
            iVar8 = iVar7 + iVar11;
            iVar11 = iVar7;
          } while (0 < iVar16);
        }
		CSharedFunctions::WriteShortToBuffer(RAM, (0x8009a188 + iVar20), (short)lVar10);
        CSharedFunctions::WriteLongToBuffer(RAM, (0x8009a184 + iVar20), iVar7);
        CSharedFunctions::WriteLongToBuffer(RAM, (0x8009a0e4 + iVar20), (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e4 + iVar20) + (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0e8 + iVar20) * param_2);
      }
      CSharedFunctions::WriteLongToBuffer(RAM, (0x8009a0e8 + iVar20), (int)CSharedFunctions::CharArrayToLong(RAM, 0x8009a0ec + iVar20));
	  if ((CSharedFunctions::CharArrayToChar(RAM, 0x8009a18c + iVar20) & 2) != 0) {
	    // Pulls new data
        //FUN_00047980(0x8009a0e0 + iVar20);
      }
	  CSharedFunctions::WriteCharToBuffer(RAM, 0x8009a18c + iVar20, CSharedFunctions::CharArrayToChar(RAM, 0x8009a18c + iVar20) | 1);
    }
    iVar19 = iVar19 + 1;
    iVar20 = iVar20 + 0xb0;
    if (7 < iVar19) {
      return;
    }

	return; // hacked in ignore the other sound slots
  } while( true );
}

bool CTopGearHyperbikeAudioDecompression::DecompressSound(unsigned char* ROM, int offset, int numberUncompressedSamples, CString outputFilename, float samplingRateFloat)
{
	int sizeRAM = 0x1000000;
	unsigned char* RAM = new unsigned char[sizeRAM];
	for (int x = 0; x < sizeRAM; x++)
		RAM[x] = 0x00;

	/*FILE* inRAM = fopen("C:\\GoldeneyeStuff\\N64Hack\\Save\\prehyperbikebeta.bin", "rb");
	fread(RAM, 1, sizeRAM, inRAM);
	fclose(inRAM);*/

	CSharedFunctions::WriteLongToBuffer(RAM, 0x800C688C, numberUncompressedSamples);

	int compressedBufferSpot = 0x800CEDE0;

	CSharedFunctions::WriteLongToBuffer(RAM, 0x000742C0, 0x0000007F);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x0009A2F0, compressedBufferSpot);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x0009A2F4, 0x00000000);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x0009A2F8, 0x00000200);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x0009A2FC, 0x00000200);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x0009A394, 0x00000000);
	CSharedFunctions::WriteLongToBuffer(RAM, 0x000C6890, 0xFFFFFFFF);

	CSharedFunctions::WriteShortToBuffer(RAM, 0x0009A300, 0x0025);
	CSharedFunctions::WriteShortToBuffer(RAM, 0x0009A398, 0x0000);

	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A18C, 0x03);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A23C, 0x00);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A2EC, 0x00);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A39C, 0x03);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A39E, 0x7F);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A39F, 0x7F);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A44C, 0x00);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A4FC, 0x00);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A5AC, 0x00);
	CSharedFunctions::WriteCharToBuffer(RAM, 0x0009A65C, 0x00);

	CSharedFunctions::StringToByteArray(&RAM[0x800742D4 & 0xFFFFFF], "001000110013001500170019001C001F002200250029002D00320037003C00420049005000580061006B00760082008F009D00AD00BE00D100E600FD0117013301510173019801C101EE02200256029202D4031C036C03C30424048E05020583061006AB0756081208E009C30ABD0BD00CFF0E4C0FBA114C130714EE170619541BDC1EA521B6251528CA2CDF315B364B3BB941B248444F7E5756603969CE74627FFF0000FFFFFFFFFFFFFFFF0002000400060008");

	int lengthData = numberUncompressedSamples + 1; //0x468; // said 465 in memory as last one? might be 0x466 with 0 pad, not sure
	int ramOutput = 0x80200000;
	for (int x = (ramOutput & 0xFFFFFF); x < sizeRAM; x++)
		RAM[x] = 0x00;

	int romLocation = offset; //0x00A7B458;
	memcpy(&RAM[compressedBufferSpot & 0xFFFFFF], &ROM[romLocation], lengthData / 2);

	//FUN_000481fc(RAM, 0x80099450, 0x00000240);
	FUN_000481fc(RAM, ramOutput, lengthData * 2);

	/*FILE* a = fopen("C:\\temp\\ram.bin", "wb");
	fwrite(RAM, 1, sizeRAM, a);
	fclose(a);*/

	bool result = SaveWav(&RAM[ramOutput & 0xFFFFFF], lengthData * 4, 1, samplingRateFloat, outputFilename);

	delete [] RAM;
	return result;
}

bool CTopGearHyperbikeAudioDecompression::SaveProcessedWav(unsigned char* processedWavData, int processedWavDataLength, int waveType, int samplingRate, CString outputFilename)
{
	/*FILE* a = fopen(outputFilename + ".bin", "wb");
	fwrite(wavData, 1, len, a);
	fflush(a);
	fclose(a);*/

	/*FILE* aa = fopen("C:\\temp\\smp2.bin", "wb");
	fwrite(processedWavData, 1, processedWavDataLength, aa);
	fflush(aa);
	fclose(aa);*/

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

bool CTopGearHyperbikeAudioDecompression::SaveWav(unsigned char* wavData, int len, int waveType, int samplingRate, CString outputFilename)
{
	int processedWavDataLength = len;
	unsigned char* processedWavData = new unsigned char[processedWavDataLength];
	for (int x = 0; x < len; x += 2)
	{
		int value = (int)CSharedFunctions::CharArrayToShort(&wavData[x]);

		// Little endian
		processedWavData[x + 1] = ((value >> 8) & 0xFF);
		processedWavData[x + 0] = value & 0xFF;
	}

	bool result = SaveProcessedWav(processedWavData, processedWavDataLength, waveType, samplingRate, outputFilename);
	delete [] processedWavData;
	return result;
}