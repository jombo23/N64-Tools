#include "StdAfx.h"
#include "MORTDecoder.h"
#include <algorithm>

CMORTDecoder::CMORTDecoder(void)
{
	outDebug = NULL;
	//outDebug = fopen("C:\\temp\\debugmort.txt", "w");	
}

CMORTDecoder::~CMORTDecoder(void)
{
	if (outDebug != NULL)
	{
		fflush(outDebug);
		fclose(outDebug);
	}
}

unsigned short CMORTDecoder::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CMORTDecoder::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

unsigned short CMORTDecoder::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CMORTDecoder::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

void CMORTDecoder::WriteLongToFile(FILE* outFile, unsigned long data)
{
	unsigned long tempLong = Flip32Bit(data);
	fwrite(&tempLong, 1, 4, outFile);
}

void CMORTDecoder::WriteShortToFile(FILE* outFile, unsigned short data)
{
	unsigned short tempLong = Flip16Bit(data);
	fwrite(&tempLong, 1, 2, outFile);
}

void CMORTDecoder::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CMORTDecoder::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

bool CMORTDecoder::Decode(unsigned char* ROM, int romSize, unsigned long address, unsigned long length, std::vector<unsigned short>& pcmSamples)
{
	if (outDebug != NULL)
		fprintf(outDebug, "-----STARTING PARSING SOUND-----\n");
	// Setup before figuring out for now
	buffer800D2940Subtraction = 0x800D2940;
	for (int x = 0; x < 0xA0; x++)
		buffer800D2940Predictor[x] = 0x0000;
	numberSkipResetPredictorCheck = 0; // 2-bytes buffer800D2940Predictor[0x18A]
	numberResetPredictor = 0; // 2-bytes buffer800D2940Predictor[0x18C]
	lastPredictorUpdateBase = 0;
	lastSampleValue = 0;
	for (int x = 0; x < 8; x++)
	{
		sampleBuffer[x] = 0;
	}

	currentSmootherPredictor = 0;

	for (int x = 0; x < 8; x++)
	{
		smootherPredictorA[x] = 0x0000;
		smootherPredictorB[x] = 0x0000;
	}
	buffer800D2AD8Subtraction = 0x800D2AD8;
	for (int x = 0; x < 0x1400; x++)
		buffer800D2AD8IntermediateValue[x] = 0x00;

	for (int x = 0; x < 0x1000; x++)
		buffer800D3ED8MORTRawInputDataBuffer[x] = 0x00;

	buffer800D4F20Subtraction = 0x800D4F20;
	buffer800D4F20OutputBuffer = new unsigned char[0x5000]; // Unsure size
	for (int x = 0; x < 0x5000; x++)
		buffer800D4F20OutputBuffer[x] = 0x00;

	variable800D4ED8Amountofsoundleft1_FirstWord = 0x0000;
	variable800D4EDA_3E80FirstWord = 0x0000;
	variable800D4EDCAmountofCompressedWordsLeft_SecondWord = 0x00000000;
	variable800D4EE0 = 0x00000000;
	variable800D4EE4 = 0x00000000;
	variable800D4EE8 = 0x00000000;
	variable800D4EEC = 0x00000000;
	variable800D4EF0 = 0x00000000;
	variable800D4EF4 = 0x00000000;
	variable800D4EFCInputChunkCurrentReadBitPosition = 0x00000000;

	variable800D4F00InputChunkUsed = 0x00000000;
	variable800D4F04InputChunkAmountLeft = 0x00000000;
	variable800D4F08 = 0x00000000;
	variable800D4F0C = 0x00000000; 
	variable800FCF2COutputBufferPointer800D4F20 = 0x800D4F20;
	variable800D4F10Status1 = 0x00;
	variable800D4F11Status2 = 0x04;
	variable800D4F12Status3 = 0x02;

	variable800FCED0 = 0x00000000;

	variable800FCEE8 = 0x000000B8;
	variable800FCEECCounter = 0x00000194;

	variable800FCEF0MORTStatus1 = 0x02;
	// About to play this one
	variable800FCEF1MORTStatus2 = 0x00;

	variable800FCF04 = 0x00000000;

	// Hardcoded passed in
	variable800FCF0CVolume = 0x0000007F;

	// Not sure about this check
	variable800FCF20 = 0x01;
	variable800FCF21 = 0x01;

	// Nemu
	//variable800FCF38CounterIncrementVariable1 = 0x00000728;
	//variable800FCF3CCounterIncrementVariable2 = 0x00000728;

	// PJ64
	//variable800FCF38CounterIncrementVariable1 = 0x000005B8;
	//variable800FCF3CCounterIncrementVariable2 = 0x000005B8;

	// PJ64 Rogue 0014
	variable800FCF38CounterIncrementVariable1 = 0x000001F8;
	variable800FCF3CCounterIncrementVariable2 = 0x000001F8;

	variable800FCEDCPredictorPointer800D2940 = 0x800D2940;

	variable800D2AD4ROMAddressMORTData = address;
	variable800FCEFCROMMORTSoundAddress = address;

	//Function called from 8003CD54 Main Loop

	//80044EA4
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	A1 = 0x800FCEF0;
	S5 = 0x8A0;
	S6 = 0xA00;
	V0 = 0;

	// PJ64
	variable800FCF30CounterPlay1 = 0xFFFFFFFF;
	variable800FCF34CounterPlay2 = 0xFFFFFFFF;

	bool started = false;
	int endedLoopAmount = -1;
	int loops = 0;

	std::vector<unsigned long> counterPJ64;

	// Original Decode
	/*counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);*/

	// Rogue 0014
	/*counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000228);
	counterPJ64.push_back(0x00000450);
	counterPJ64.push_back(0x00000678);
	counterPJ64.push_back(0x00000000);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x000005C0);
	counterPJ64.push_back(0x000007E8);
	counterPJ64.push_back(0x00000170);
	counterPJ64.push_back(0x00000398);
	counterPJ64.push_back(0x00000508);
	counterPJ64.push_back(0x00000730);
	counterPJ64.push_back(0x000000B8);
	counterPJ64.push_back(0x000002E0);
	counterPJ64.push_back(0x00000508);*/


	// Later do better...
	while (true)
	{
		//// Extra done elsewhere for main counter? Later this will be different..

		if (counterPJ64.size() > 0)
		{
			variable800FCF30CounterPlay1 = counterPJ64[0];
			variable800FCF34CounterPlay2 = counterPJ64[0];

			counterPJ64.erase(counterPJ64.begin());
		}
		else
		{
			if (variable800FCF30CounterPlay1 == 0xFFFFFFFF)
			{
				// PJ64
				variable800FCF30CounterPlay1 = 0xB8;
				variable800FCF34CounterPlay2 = 0xB8;

				// Nemu
				//variable800FCF30CounterPlay1 = 0x398;
				//variable800FCF34CounterPlay2 = 0x398;
			}
			else
			{
				for (int x = 0; x < 3; x++)
				{
					if (variable800FCF30CounterPlay1 >= 0x8A0)
					{
						variable800FCF30CounterPlay1 = 0xB8;
						variable800FCF34CounterPlay2 = 0xB8;
					}
					else
					{
						variable800FCF30CounterPlay1 += 0xB8;
						variable800FCF34CounterPlay2 += 0xB8;
					}
				}
			}
		}


		// 80044EE4 TWINE // 800886B4
		int soundIndex = 1;
		//// Commented out just for speed skipping the dual play
		//for (int soundIndex = 0; soundIndex < 2; soundIndex++)
		{
			if (soundIndex == 0)
			{
				T6 = variable800FCEF0MORTStatus1;
			}
			else if (soundIndex == 1)
			{
				T6 = variable800FCEF1MORTStatus2;
			}

			S8 = V0 << 2;
			// 80044EEC
			// Commented out so keeps playing when thinks done
			if (T6 != 2) // One playing
			{
				//80044EF4

				//80044F0C
				A0 = variable800FCF3CCounterIncrementVariable2;
				V1 = variable800FCF34CounterPlay2;

				//if (outDebug != NULL)
					//fprintf(outDebug, "80044F0C: A0 %08X V1 %08X\n", A0, V1);

				// T9/800FCEDC to 44(SP)

				S1 = 0;
				// V0 to 70(SP)

				//800 44F38
				// A1 to 5C(SP)
				if (V1 < A0)
				{
					V0 = variable800FCEECCounter;

					T4 = (A0 - V1);

					//80044F54
					if (T4 < V0)
					{
						S7 = V0;
					}
					else
					{
						S7 = variable800FCEE8;
					}
				}
				else
				{
					// 80044F6C
					V0 = variable800FCEECCounter;
					T6 = V1 - A0;
					
					T5 = S5 - V0;
					if (T5 < T6)
					{
						S7 = V0;
					}
					else
					{
						S7 = variable800FCEE8;
					}
				}

				//80044F98
				//80044F9C
				Function80045780(ROM, started, pcmSamples);

				//80044FA4
				T8 = 0x800FCEDC;//44(SP);

				T3 = 0x800FCF20;

				A1 = 0x1140;

				V1 = variable800D4F11Status2;
				T9 = soundIndex; // 70(SP)

				//80044FC4
				if (V1 != 2)
				{
					T7 = 0x800FCEF0 + soundIndex; //5C(SP)
				}
				else
				{
					//80044FCC
					S2 = T9 + T3;

					//80044FD0
					//Function80059120(A0, A1);

					//80044FD8
					T4 = 0x800FCEDC; //44(SP)

					//80044FDC
					Function80045A48();

					AT = 0x80100000;
					T6 = 0x800FCEDC; //44(SP)

					variable800FCED0 = 1;

					if (T9 == 0)
						variable800FCF20 = 0;
					else
						variable800FCF21 = 0;

					//80044FF8
					goto label80045084;
				}

				//80045004
				if (soundIndex == 0)
				{
					V0 = variable800FCEF0MORTStatus1;
				}
				else if (soundIndex == 1)
				{
					V0 = variable800FCEF1MORTStatus2;
				}

				//80045008
				if ((V0 == 0) || (V0 == 3))
				{
					
				}
				else
				{
					T8 = soundIndex; // 70(SP)
					if (V1 != 4)
					{
						
					}
					else
					{
						//80045020
						T9 = 0x800FCF20;
						S2 = T8 + T9;

						if (soundIndex == 0)
						{
							T4 = variable800FCF20;
						}
						else if (soundIndex == 1)
						{
							T4 = variable800FCF21;
						}

						T3 = 0x800FCEE0;

						S0 = S8 + T3;

						//80045038
						if (T4 == 0)
						{
							Function80045A68();
						}

						T5 = 1;

						if (soundIndex == 0)
						{
							variable800FCF20 = (unsigned char)T5;
						}
						else if (soundIndex == 1)
						{
							variable800FCF21 = (unsigned char)T5;
						}

						//80045050
						T7 = 0x800FCEF0 + soundIndex;

						variable800FCED0 = 0;
						T6 = 2;

						if (soundIndex == 0)
						{
							variable800FCEF0MORTStatus1 = (unsigned char)T6;
						}
						else
						{
							variable800FCEF1MORTStatus2 = (unsigned char)T6;
						}

						A1 = 0x1400;
						ClearBuffer_Function80057FD0(0x800D2AD8, A1);
						ClearBuffer_Function80057FD0(0x800D4F20, A1);

						// 8004507C
						T8 = 0x800FCEDC; //44(SP);
					}
				}

label80045084:
				// 80045084
				A1 = S7;
				Function800459E0(A1, V0);

				//8004508C
				T9 = 0x800FCEDC; //44(SP)
				T4 = variable800D4F08;

				//80045098
				A2 = T4 - S7;
				if (S7 != 0)
				{
					T6 = 0x800FCF00;
					T5 = 0x800FCEE0;

					V0 = S7 & 0x3;
					A1 = V0;

					S0 = S8 + T5;

					S2 = S8 + T6;

					//800450BC
					if (V0 != 0)
					{
						// TODO 800450C4
					}
					else
					{
						// GOTO 8004517C
					}
					
					//8004517C
					V1 = A2 + S1;
					T0 = V1 + 1;
					T1 = V1 + 2;
					T2 = V1 + 3;

					do
					{
						S0 = 0x800FCEE4;//0(S0)
						//80045198 MFHI Remainder
						T7 = V1 % S6;
						T9 = T7 << 1;
						T3 = T9;

						T8 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3]);
						//if (outDebug != NULL)
							//fprintf(outDebug, "80045198: INTERMEDIATE VALUE READ %04X from %08X\n", (unsigned short)(T8 & 0xFFFF), T3);
						T6 = variable800FCF04; //0(S2)
						T7 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						//800451AC
						A0 = S1 << 1;

						unsigned __int64 tempRegister = T6 * T8;

						//800451B4
						T4 = T7 + A0;
						T9 = T4 + 1;

						//800451C0
						A1 = S1 + 1;
						A1 = A1 << 1;
						A2 = S1 + 2;
						A2 = A2 << 1;
						A3 = S1 + 3;

						A3 = A3 << 1;

						V0 = (unsigned long)(tempRegister & 0xFFFFFFFF);

						//800451DC
						V0 = V0 >> 7;
						V0 = V0 << 0x10;

						T3 = T9 % S5;
						T6 = T3 << 1;

						V0 = (int)V0 >> 0x10;
						T8 = T6;

						//800451F8
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T8, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "800451F8: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T8);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);

						T4 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						//80045204
						S1 = S1 + 4;
						T9 = T4 + A0;
						T3 = T9 % S5;
						T5 = T3 << 1;

						T6 = T5;
						//80045220
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T6, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "80045220: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T6);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);
						T4 = T0 % S6;

						//8004522C
						T9 = T4 << 1;

						T3 = T9;
						
						//80045234
						T7 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3]);
						//if (outDebug != NULL)
							//fprintf(outDebug, "80045234: INTERMEDIATE VALUE READ %04X from %08X\n", (unsigned short)(T7 & 0xFFFF), T3);
						T5 = variable800FCF04; //0(S2)
						T4 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						T8 = T4 + A1;
						T9 = T8 + 1;

						if (S6 == 0)
						{
							// ERROR OUT
							return false;
						}

						T0 = T0 + 4;

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						//8004526C
						tempRegister = T5 * T7;
						V0 = tempRegister & 0xFFFFFFFF;

						V0 = V0 >> 7;
						V0 = V0 << 0x10;

						T3 = T9 % S5;

						T5 = T3 << 1;
						V0 = (int)V0 >> 0x10;

						T7 = T5;
						//8004528C
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T7, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "8004528C: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T7);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);
						T8 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						//800452A4
						T9 = T8 + A1;
						T3 = T9 % S5;
						T6 = T3 << 1;
						T5 = T6;
						
						//800452BC
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T5, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "800452BC: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T5);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);
						
						T8 = T1 % S6;

						//800452C8
						T9 = T8 << 1;
						T3 = T9;

						T4 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3]);
						//if (outDebug != NULL)
							//fprintf(outDebug, "800452C8: INTERMEDIATE VALUE READ %04X from %08X\n", (unsigned short)(T4 & 0xFFFF), T3);
						T6 = variable800FCF04; //0(S2)
						T8 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						T7 = T8 + A2;
						T9 = T7 + 1;

						if (S6 == 0)
						{
							// ERROR OUT
							return false;
						}

						T1 = T1 + 4;

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						V0 = T6 * T4;

						//8004530C
						V0 = V0 >> 7;
						V0 = V0 << 0x10;

						T3 = T9 % S5;

						T6 = T3 << 1;
						V0 = (int)V0 >> 0x10;
						T4 = T6;

						//80045328
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T4, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "80045328: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T4);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);
						T7= variable800FCF3CCounterIncrementVariable2; // 0(S3)
			
						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						//80045340
						T9 = T7 + A2;
						T3 = T9 % S5;

						T5 = T3 << 1;
						T6 = T5;
						
						//80045358
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T6, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "80045358: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T6);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);
						T7 = T2 % S6;

						//80045364
						T9 = T7 << 1;
						T3 = T9;

						T8 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3]);
						//if (outDebug != NULL)
							//fprintf(outDebug, "80045364: INTERMEDIATE VALUE READ %04X from %08X\n", (unsigned short)(T8 & 0xFFFF), T3);
						T5 = variable800FCF04; //0(S2)
						T7 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						//8004537C
						tempRegister = T5 * T8;
						T4 = T7 + A3;
						T9 = T4 + 1;

						if (S6 == 0)
						{
							// ERROR OUT
							return false;
						}

						T2 = T2 + 4;

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}
				
						//800453A4
						V0 = tempRegister & 0xFFFFFFFF;
						V0 = V0 >> 7;
						V0 = V0 << 0x10;

						T3 = T9 % S5;

						T5 = T3 << 1;
						V0 = (int)V0 >> 0x10;
						T8 = T5;

						//800453C4
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T8, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "800453C4: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T8);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);

						//800453C8
						T4 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						T9 = T4 + A3;

						T3 = T9 % S5;
						T6 = T3 << 1;

						if (S6 == 0)
						{
							// ERROR OUT
							return false;
						}

						T5 = T6;

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						//80045408
						WriteShortToBuffer(buffer800D4F20OutputBuffer, T5, (unsigned short)V0);
						//if (outDebug != NULL)
							//fprintf(outDebug, "80045408: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T5);
						//if (started)
							//pcmSamples.push_back((unsigned short)V0);

						V1 = V1 + 4;

						//8004541C
					}
					while (S1 != S7);

					/*FILE* a = fopen("C:\\temp\\d.bin", "ab");
					fwrite(buffer800D4F20OutputBuffer, 1, 0x2000, a);
					fflush(a);
					fclose(a);*/

label80045424:
					//80045424
					T8 = variable800FCF3CCounterIncrementVariable2; // 0(S3)
					T4 = S7 << 1;

					T9 = T8 + T4;

					//80045434
					T7 = T9 % S5;
					variable800FCF3CCounterIncrementVariable2 = T9;
					variable800FCF3CCounterIncrementVariable2 = T7;

					T3 = T9;

					if (S5 == 0)
					{
						// ERROR OUT
						return false;
					}

					A1 = 0x1140;

					//80045458
					// Maybe this is something for output buffer to write it out for use?
					//Function80059120(A0, A1);

					//80045460
					V1 = 0x800FCEF0 + soundIndex; //5C(SP)
					if (soundIndex == 0)
					{
						T6 = variable800FCEF0MORTStatus1;
					}
					else if (soundIndex == 1)
					{
						T6 = variable800FCEF1MORTStatus2;
					}

					//80045480
					if (T6 < 6)
					{
						//T6 = T6 << 2;
						//T6 = T6 + 0x8007CC60;

						// JR T6 table
						if (T6 == 0)
						{
							//80045488

							T5 = 0x800FCEDC; //44(SP)
							T4 = 0x800FCF08;
							T8 = 0x800FCF00;

							A1 = 0x800F0000 + S8;
							A2 = 0x800455DC;

							A1 = variable800FCEFCROMMORTSoundAddress;
		
							S2 = S8 + T8;
							S0 = S8 + 4;

							//800454B8
							Function800456D0();

							//800454C0
							T3 = 0x800FCEF0 + soundIndex; //5C(SP)
							T9 = 4;

							if (soundIndex == 0)
							{
								variable800FCEF0MORTStatus1 = (unsigned char)T9;
							}
							else
							{
								variable800FCEF1MORTStatus2 = (unsigned char)T9;
							}

							T7 = variable800FCED0;
							
							if (T7 != 0)
							{
								variable800FCED0 = 0;
							}

							T6 = variable800FCF0CVolume; //0(S0)
							variable800FCF04 = T6; // 0(S2)

							goto label80045594;
						}
						else if (T6 == 1)
						{
							//TODO 800454EC
						}
						else if (T6 == 2)
						{
							goto label80045594;
						}
						else if (T6 == 3)
						{
							//TODO 8004552C

							// Think this means stopped, when song over
						}
						else if (T6 == 4)
						{
							goto label80045594;
						}
						else if (T6 == 5)
						{
							//TODO 80045550
						}
					}
					else
					{
						goto label80045594;
					}
				}
				else
				{
					goto label80045424;
				}
			}

	label80045594:
				A1 = 0x800FCEF0 + soundIndex; //5C(SP);
				V0 = soundIndex; //70(SP)
			// 8004559C
			{
				V0++;
				A1++;
				if (V0 != 2)
				{

				}
			}
		}

		// Load back S7
		S7 = S7;

		loops++;

		if (!started)
		{
			//if (variable800D4F11Status2 == 1)
				//started = true;
		}
		else
		{
			if (variable800D4F11Status2 == 4)
			{
				if (endedLoopAmount == -1)
				{
					endedLoopAmount = loops + 1000;
				}
				else
				{
					if (loops > endedLoopAmount)
					{
						break;
					}
				}
			}
			else if (loops >= 2000)
			{
				if (endedLoopAmount == -1)
				{
					endedLoopAmount = loops + 1000;
				}
				else
				{
					if (loops > endedLoopAmount)
					{
						break;
					}
				}
			}
		}
	}

	delete [] buffer800D4F20OutputBuffer;

	return true;
}

// TWINE 8009D5D4
void CMORTDecoder::Function80045780(unsigned char* ROM, bool& started, std::vector<unsigned short>& pcmSamples)
{
	unsigned long AT = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	T6 = variable800D4F10Status1; //25D0(A0)
	S2 = variable800D4F12Status3;

	//800457A4
	if (T6 != 0)
	{
		//800457AC
		A1 = 0;
		A2 = 0;

		//800457B0
		Function80056AD0(V0);

		//800457B8
		if ((int)V0 < 0)
		{
			T5 = variable800D4F10Status1;
		}
		else
		{
			//800457C0 // TWINE 8009D614
			V1 = variable800D4F04InputChunkAmountLeft;
			AT = 0x00FFFFFF;

			T2 = 0x60;

			//800457CC
			if (V1 == 0)
			{
				T9 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[4]);
				if (outDebug != NULL)
					fprintf(outDebug, "800457D4: HEADER 1 INPUT VALUE %08X\n", T9);
				T0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[8]);
				if (outDebug != NULL)
					fprintf(outDebug, "800457D8: HEADER 2 INPUT VALUE %08X\n", T0);

				variable800D4EFCInputChunkCurrentReadBitPosition = T2;

				T8 = T9 >> 0x10;
				T1 = T0 & AT;
				
				variable800D4ED8Amountofsoundleft1_FirstWord = (unsigned short)T8;
				variable800D4EDCAmountofCompressedWordsLeft_SecondWord = T1;

				V1 = variable800D4F04InputChunkAmountLeft;
				variable800D4EDA_3E80FirstWord = (unsigned short)(T9 & 0xFFFF);
			}

			//800457F8
			V0 = variable800D4EDCAmountofCompressedWordsLeft_SecondWord;
			T3 = V1 + 0x400;

			//80045800
			variable800D4F04InputChunkAmountLeft = T3;

			T4 = V0 - 0x100;
			//80045808
			if (V0 < 0x100)
			{
				variable800D4EDCAmountofCompressedWordsLeft_SecondWord = 0;
			}
			else
			{
				variable800D4EDCAmountofCompressedWordsLeft_SecondWord = T4;
			}

			variable800D4F10Status1 = 0;
		}
	}

	//80045820
	T5 = variable800D4F10Status1;
	S1 = variable800D4F0C;
	if (T5 == 0)
	{
		V0 = variable800D4F11Status2;

		S3 = 1;

		//80045838
		if ((V0 == 1) || (V0 == 2) || (V0 == 3))
		{
			V1 = variable800D4F04InputChunkAmountLeft; // TWINE 800E2C90

			//80045854
			T6 = variable800D4F00InputChunkUsed; // TWINE 800E2C8C

			T7 = V1 - T6;

			if (T7 < 0xC01)
			{
				T9 = V1 & 0xFFF;

				//80045868
				if (V1 != 0)
				{
					V0 = variable800D4EDCAmountofCompressedWordsLeft_SecondWord;

					if (V0 < 0x101)
					{
						A2 = V0 << 2;
					}
					else
					{
						A2 = 0x400;
					}
				}
				else
				{
					A2 = 0x400;
				}

				//80045890
				T0 = T9 >> 3;
				// Checking if any more compressed data to read
				if ((int)A2 <= 0)
				{
					//from 800458BC
				}
				else
				{
					//80045898
					T8 = variable800D2AD4ROMAddressMORTData; //0194(S0);
		
					T1 = T0 << 3;
					A1 = T1;

					variable800D4F10Status1 = (unsigned char)S3;

					// Index into ROM buffer
					unsigned long A0 = T8 + V1;
					
					T9 = 0x800455DC; //variable800D2AD0PullDataPointer; //0190(S0);
					// TWINE calls 80088F6C
					if (T9 == 0x800455DC)
					{
						Function800455DC(ROM, A0, A1, A2);
					}
					else
					{
						return;
					}
				}
			}
		}

		S1 = variable800D4F0C;

		//800458C0
		T2 = variable800D4F08;

		S3 = 1;

		V1 = S1 - T2;

		//800458D0
		if (V1 < 0x960)
		{
			T3 = variable800D4F11Status2;

			if (T3 == S3)
			{
				T5 = variable800D4F04InputChunkAmountLeft;
			}
			else
			{
				//800458E4

				T4 = variable800D4ED8Amountofsoundleft1_FirstWord;

				if ((int)T4 <= 0)
				{
					goto label800459AC;
				}
				else
				{
					//800458F0
					T5 = variable800D4F04InputChunkAmountLeft;
				}
			}

			//800458F4
			T6 = variable800D4F00InputChunkUsed;
			T7 = T5 - T6;

			// Pokemon Stadium was just ((int) T7 < 0x25)...Added because of TWINE to double check it was actually legit done, because < 0x25 it's not right, still has data
			//if ((int)T7 < 0x25)
			if ((int)T7 <= 0)
			{
				goto label800459AC;
			}
			else
			{
				//80045908
				AT = 0xA00;
				if ((int)S2 <= 0)
				{
					goto label800459AC;
				}
				else
				{
label80045910:
					//80045910
					// 0x140 offset in buffers a time (modulus 0x1400, so 0x10 chunks)
					T8 = S1 % AT;
					T0 = T8 >> 2;
					T1 = T0 << 3;

					A1 = T1;

					//80045928 // TWINE 8009D7C0
					// Call 80045FF0 // TWINE calls 8009DF60
					Function80045FF0(A1, pcmSamples);

					//80045930  // TWINE 8009D7C8
					T9 = variable800D4EFCInputChunkCurrentReadBitPosition;
					T4 = variable800D4F0C;
					T8 = variable800D4F08;
					T6 = variable800D4ED8Amountofsoundleft1_FirstWord; // TWINE it's T8

					//if (outDebug != NULL)
						//fprintf(outDebug, "80045930: T9: %08X T4: %08X T8: %08X T6: %08X\n", T9, T4, T8, T6);
					//80045940
					AT = -4;
					T2 = T9 >> 3;

					S1 = T4 + 0xA0;
					T3 = T2 & AT;
					V1 = S1 - T8;

					//80045958
					T7 = (int)T6 - 1;

					// Logic added...was not in Pokemon Stadium
					// Sound 0...is weird in Rogue...it is 1 value, and goes below 0 here in Pokemon Stadium rip...
					if ((int)T7 < 0)
						T7 = 0;

					variable800D4F00InputChunkUsed = T3;
					variable800D4F0C = S1;
					variable800D4ED8Amountofsoundleft1_FirstWord = (unsigned short)T7;

					S2 = S2 - 1;

					//80045968
					if (V1 < 0x960)
					{
						T0 = variable800D4F11Status2;
						T1 = T7 & 0xFFFF;

						//80045978
						if (S3 != T0)
						{
							if ((int)T1 <= 0)
							{
								goto label800459AC; 
							}
							else
							{
								T9 = variable800D4F04InputChunkAmountLeft;
							}
						}
						else
						{
							T9 = variable800D4F04InputChunkAmountLeft;
						}

						//8004598C
						T2 = variable800D4F00InputChunkUsed;
						T3 = T9 - T2;

						//80045998
						//if (T3 < 0x25)
						if ((int)T3 <= 0x0)
						{
							
						}
						else
						{
							if ((int)S2 > 0)
							{
								AT = 0xA00;
								goto label80045910;
							}
						}
					}
				}
			}
		}
	}

label800459AC:
	/*FILE* pred = fopen("C:\\temp\\pred.bin", "wb");
	fwrite(buffer800D2940Predictor, 2, 0xA0, pred);
	fflush(pred);
	fclose(pred);*/

	T4 = variable800D4F11Status2;
	if (T4 != S3)
	{
		// Set RA
	}
	else
	{
		if (V1 < 0x8C0)
		{
			
		}
		else
		{
			// Start playing
			variable800D4F11Status2 = 2;
			started = true;
			if (outDebug != NULL)
			{
				fprintf(outDebug, "STARTED - SOUND NOW PLAYING!\n");
				fflush(outDebug);
			}
		}
	}
}

void CMORTDecoder::Function800459E0(unsigned long A1Param, unsigned long& V0)
{
	unsigned long AT = 0;
	unsigned long A1 = A1Param;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//800459E0
	T6 = variable800D4F11Status2;

	V0 = 0;

	//800459EC
	if (T6 == 3)
	{
		// 800459F4
		V1 = variable800D4F08;
		T7 = variable800D4F0C;
		V0 = T7 - V1;

		//80045A04
		if (A1 < V0)
		{
			A2 = A1;
		}
		else
		{
			T8 = variable800D4ED8Amountofsoundleft1_FirstWord;
			
			//80045A18
			A2 = V0;

			T9 = 4;

			if ((int)T8 <= 0)
			{
				variable800D4F11Status2 = (unsigned char)T9;
				V1 = variable800D4F08;
			}
		}

		//80045A30	
		T0 = V1 + A2;
		variable800D4F08 = T0;

		V0 = A2;
	}
}

void CMORTDecoder::Function80059120(unsigned long A0Param, unsigned long A1Param)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	if ((int)A1 <= 0)
	{
		return;
	}

	T3 = 0x2000;

	//80059130
	if (A1 < T3)
	{
		T0 = A0;
		T1 = A0 + A1;

		if (T0 < T1)
		{
			T2 = T0 & 0xF;
			T1 = T1 - 0x10;
			T0 = T0 - T2;

			do
			{
				// CACHE Command
				AT = T0 < T1;

				T0 = T0 + 0x10;
			}
			while(AT);
		}
		else
		{
			return;
		}

	}
	else
	{
		// TODO 80059170
	}
}

void CMORTDecoder::Function800456D0()
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;


	//ClearBuffer_Function80057FD0(0x800D2940, 0x140); // resets everything here
	for (int x = 0; x < 0xA0; x++)
		buffer800D2940Predictor[x] = 0x0000;
	numberSkipResetPredictorCheck = 0; // 2-bytes buffer800D2940Predictor[0x18A]
	numberResetPredictor = 0; // 2-bytes buffer800D2940Predictor[0x18C]
	lastPredictorUpdateBase = 0;
	lastSampleValue = 0;
	for (int x = 0; x < 8; x++)
	{
		sampleBuffer[x] = 0;
	}

	currentSmootherPredictor = 0;

	for (int x = 0; x < 8; x++)
	{
		smootherPredictorA[x] = 0x0000;
		smootherPredictorB[x] = 0x0000;
	}




	//800456EC
	lastPredictorUpdateBase = (unsigned short)0x28;

	T7 = 0x800455DC; //20(SP); Maybe from Function
	//80045708
	// Why, isn't this part of buffer??
	//WriteLongToBuffer(buffer800D2940Predictor, 0x190), T7);
	//variable800D2AD0PullDataPointer = T7;

	T8 = variable800D2AD4ROMAddressMORTData; //1C(SP); Maybe from Function

	// T8 to 194(A3)
	variable800D2AD4ROMAddressMORTData = variable800FCEFCROMMORTSoundAddress;

	Function8005E3A0(0x190, 0x1);

	T9 = variable800D2AD4ROMAddressMORTData; //1C(SP) Maybe from Function

	V0 = 0;

	T2 = 2;
	if (T9 != 0)
	{
		T0 = 0x800455DC; //20(SP);
		T1 = 1;

		if (T0 != 0)
		{
			variable800D4F11Status2 = (unsigned char)T1;
		}
		else
		{
			variable800D4F11Status2 = 0;
		}

		variable800D4F10Status1 = 0x00;
		variable800D4ED8Amountofsoundleft1_FirstWord = 0x0000;

		variable800D4EDA_3E80FirstWord = (unsigned short)V0;
		variable800D4ED8Amountofsoundleft1_FirstWord = (unsigned short)V0;

		variable800D4EFCInputChunkCurrentReadBitPosition = 0;
		variable800D4F04InputChunkAmountLeft = 0;
		variable800D4F00InputChunkUsed = 0;
		variable800D4F0C = 0;
		variable800D4F08 = 0;

		variable800D4F12Status3 = (unsigned char)T2; //2
	}
}

void CMORTDecoder::ClearBuffer_Function80057FD0(unsigned long A0Param, unsigned long A1Param)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	// I think just clears out buffers to 00
	for (unsigned long x = 0; x < A1; x++)
	{
		if ((A0 >= 0x800D2AD8) && (A0 < 0x800D3ED8)) // 0x1400 sized buffer
		{
			// Intermediate Value
			buffer800D2AD8IntermediateValue[A0 + x - buffer800D2AD8Subtraction] = 0;
		}
		else if ((A0 >= 0x800D4F20) && (A0 < 0x800D6320)) // 0x1400 sized offset
		{
			// Output Buffer
			buffer800D4F20OutputBuffer[A0 + x - buffer800D4F20Subtraction] = 0;
		}
		else
		{
			MessageBox(NULL, "Support another buffer clear!!!", "Error", NULL);
		}
	}
}

void CMORTDecoder::Function8005E3A0(unsigned long A1Param, unsigned long A2Param)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	T6 = 0x80079620;
	T7 = 0x80079620;

	variable800D4EE0 = T6;
	variable800D4EE4 = T7;
	variable800D4EE8 = 0;
	variable800D4EEC = 0;
	variable800D4EF0 = A2;
	variable800D4EF4 = A1;
}

void CMORTDecoder::Function800455DC(unsigned char* ROM, unsigned long A0Param, unsigned long A1Param, unsigned long A2Param)
{
	memcpy(&buffer800D3ED8MORTRawInputDataBuffer[A1Param], &ROM[A0Param], A2Param);
	return;

	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//800455DC
	S0 = A2; // 0x400 size to pull
	S5 = A1; // 800D3ED8 MORT RAW Input Buffer
	S2 = A0; // ROM Address 0x1AC0004

	S3 = A1; // 800D3ED8 MORT RAW Input Buffer
	if (A0 != 0)
	{
		S4 = 0x800FCF40;
		S6 = 0x800FCF24;
		//S7 = SP + 0x48;

		A0 = S5; // 800D3ED8 MORT RAW Input Buffer
		if (S0 < 0x1001)
		{
			S1 = S0;
		}
		else
		{
			S1 = 0x1000;
		}

		//80045648

		//0 to 4A(SP)
		//S4 to 4C(SP)
		//S3 to 50(SP)
		//S2 to 54(SP)
		//S1 to 58(SP)

		A1 = S0;

		//8004565C
		Function8005E2F0(A0, A1);

		//80045664
		// Not sure what these are...maybe just restoring old
		//A0 = 0x801038D0; //0(S6);
		//A1 = 0x800AA4C0; //S7;
		//A2 = 0;

		// 8005E0F0
		Function8005E0F0();

		// TODO 8005E0F8
		// ...

		// I think this is just a fancy way of copying...
	}
	else
	{
		// GOTO 80045694 
	}

	//80045694
}

void CMORTDecoder::Function8005E2F0(unsigned long A0Param, unsigned long A1Param)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	if ((int)A1 <= 0)
	{
		return;
	}

	T3 = 0x2000;

	if (A1 < T3)
	{
		T0 = A0;
		T1 = A0 + A1;

		while (T0 < T1)
		{
			T2 = T0 & 0xF;

			T1 = T1 & 0xFFF0;
			if (T2 != 0)
			{
				T0 = T0 - T2;
				// CACHE

				if (T0 < T1)
				{

				}
				else
				{
					return;
				}

				T0 = T0 + 0x10;
			}

			//8005E340
			T2 = T1 & 0xF;

			if (T2 != 0)
			{
				T1 = T1 - T2;
				// CACHE

				if (T1 < T0)
				{

				}
				else
				{
					return;
				}
			}

			//8005E360
			{
				do
				{
					// CACHE
					AT = (T0 < T1);
					T0 = T0 + 0x10;
				} while (AT != 0);
			}
		}
	}
	else
	{
		//TODO 8005E378
	}
}

void CMORTDecoder::Function8005E0F0()
{

}

void CMORTDecoder::Function80056AD0(unsigned long& V0)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//A0 to 28(SP);
	//A1 to 2C(SP);
	//A2 to 30(SP);
	//S1 to 18(SP);
	//S0 to 14(SP);

	//80056AE8
	Function80062A90(V0);

	//80056AF0
	T6 = variable800D4EE0; //28(SP)

	// For now...seems always this
	V0 = 0;
}

void CMORTDecoder::Function80062A90(unsigned long& V0)
{
	// Not sure, some kind of DMA thing
}

unsigned long CMORTDecoder::ReadBitsFrom80045FF0Buffer(int numberBits, unsigned char* buffer800D3ED8MORTRawInputDataBuffer, unsigned long& currentInputData, unsigned long& bitsleft, unsigned long& currentOverallBitPosition)
{
	if ((int)bitsleft >= (numberBits + 1))
	{
		unsigned long bitmask = (unsigned long)1 << numberBits;
		bitmask = bitmask - 1;

		unsigned long returnValue = (currentInputData & bitmask);
		currentInputData = currentInputData >> numberBits;
		bitsleft = bitsleft - numberBits;
		return (unsigned short)returnValue;
	}
	else
	{
		unsigned long T6 = (unsigned long)1 << bitsleft;
		unsigned long T9 = T6 - 1;
		currentOverallBitPosition = currentOverallBitPosition + 0x20;
		unsigned long T8 = currentOverallBitPosition >> 5;
		T6 = T8 & 0x3FF;
		unsigned long T7 = T9 & currentInputData;
		T9 = T6 << 2;
		T8 = T9;
		unsigned long returnValue = (unsigned short)T7;
		unsigned long T4 = numberBits;

		//8004625C
		currentInputData = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8]);
		if (outDebug != NULL)
			fprintf(outDebug, "8004625C: INPUT VALUE %08X\n", currentInputData);

		if (bitsleft != T4)
		{
			//80046268
			T6 = T4 - bitsleft;
			T9 = 1;
			T8 = T9 << T6;
			T9 = T8 - 1;
			T6 = currentInputData & T9;
			T8 = T6 << bitsleft;
			T6 = T7;
			T7 = T6 | T8;
			returnValue = (unsigned short)T7;
		}

		//8004628C
		T9 = T4 - bitsleft;
		currentInputData = currentInputData >> T9;
		bitsleft = bitsleft + (0x20 - numberBits);

		return returnValue;
	}
}

//TWINE 8009DF60
void CMORTDecoder::Function80045FF0(unsigned long currentIntermediateValueOffset, std::vector<unsigned short>& pcmSamples)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//800AA3E0
	unsigned short shortsSP60[4][0xD];
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 0xD; x++)
		{
			shortsSP60[y][x] = 0x0000;
		}
	}
	unsigned short shortsSPC8[0x4];
	unsigned short shortsSPD0[0x4];
	unsigned short stackBuffer2Offsets[0x4];
	unsigned short shortsSPE0[0x4];
	for (int x = 0; x < 0x4; x++)
	{
		shortsSPC8[x] = 0x0000;
		shortsSPD0[x] = 0x0000;
		stackBuffer2Offsets[x] = 0x0000;
		shortsSPE0[x] = 0x0000;
	}

	unsigned short shortsSPE8[8];
	for (int x = 0; x < 8; x++)
	{
		shortsSPE8[x] = 0x0000;
	}

	//S0 to 28(SP) 800D2940
	// A1 to (0xFC(SP), 0x1400 size buffer total, from 800D2AD8

	unsigned long currentOverallBitPosition = variable800D4EFCInputChunkCurrentReadBitPosition;

	//S0 = A0; //800D2940 Predictor Buffer pointer
	T6 = (int)currentOverallBitPosition >> 5;
	T7 = T6 & 0x3FF;
	T8 = T7 << 2;
	T9 = T8;

	//8004601C
	unsigned long currentInputWord = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T9]);
	if (outDebug != NULL)
		fprintf(outDebug, "8004601C: INPUT VALUE %08X\n", currentInputWord);

	unsigned long bitsUsed = currentOverallBitPosition & 0x1F;

	A3 = 0x20 - bitsUsed;
	unsigned long bitsleft = A3;

	/*FILE* aa = fopen("C:\\temp\\aa.bin", "wb");
	fwrite(buffer800D2940Predictor, 2, 0xA0, aa);
	fflush(aa);
	fclose(aa);*/
	//80046030
	unsigned long currentInputData = currentInputWord >> bitsUsed;

	//80046018
	if (numberSkipResetPredictorCheck == 0)
	{
		// 0x18A is periods of skip bit check predictors
		// 0x18C is periods of null predictors
		if (numberResetPredictor == 0)
		{
			T3 = ReadBitsFrom80045FF0Buffer(1, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);

			//800460B4
			if (T3 != 0)
			{
				//800460BC
				numberResetPredictor = ReadBitsFrom80045FF0Buffer(4, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);

				//8004613C
				numberResetPredictor++;

				if (numberResetPredictor != 1 && outDebug != NULL)
					fprintf(outDebug, "800461C8: NUMBER RESET PREDICTOR %04X\n", numberResetPredictor);
			}
			else
			{
				//8004614C
				numberSkipResetPredictorCheck = ReadBitsFrom80045FF0Buffer(7, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
				
				//800461C8
				numberSkipResetPredictorCheck++;

				if (numberSkipResetPredictorCheck != 1 && outDebug != NULL)
					fprintf(outDebug, "800461C8: NUMBER SKIP RESET PREDICTOR %04X\n", numberSkipResetPredictorCheck);
			}
		}
	}
	else
	{
		if (outDebug != NULL)
			fprintf(outDebug, "800461D8: SKIPPING numberSkipResetPredictorCheck %04X\n", numberSkipResetPredictorCheck);
	}

	//800461D8
	if (numberResetPredictor != 0)
	{
		if (outDebug != NULL)
			fprintf(outDebug, "800461D8: CLEARING DUE TO numberResetPredictor %04X\n", numberResetPredictor);

		A0 = currentIntermediateValueOffset; //FC(SP);
		currentIntermediateValueOffset = 0x140;
		
		//bitsleft to 58(SP);
		//currentOverallBitPosition to 50(SP);

		//800461F4
		ClearBuffer_Function80057FD0(0x800D2AD8 + A0, currentIntermediateValueOffset);

		//800461FC
		numberResetPredictor--;

		//8004620C
	}
	else
	{
		//80046214

		unsigned long readValue = ReadBitsFrom80045FF0Buffer(6, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x0] = (unsigned short)readValue; //T7 to E8(SP)

		//80046298

		readValue = ReadBitsFrom80045FF0Buffer(6, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x1] = (unsigned short)readValue; //T6 to EA(SP)

		//80046318
		readValue = ReadBitsFrom80045FF0Buffer(5, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x2] = (unsigned short)readValue; //T7 to EC(SP)

		//8004631C
		
		//800463A0

		//800463A4
		readValue = ReadBitsFrom80045FF0Buffer(5, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x3] = (unsigned short)readValue; //T6 to EE(SP)

		//80046420

		//80046424

		readValue = ReadBitsFrom80045FF0Buffer(4, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x4] = (unsigned short)readValue; //T7 to F0(SP)

		//800464A0
		readValue = ReadBitsFrom80045FF0Buffer(4, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x5] = (unsigned short)readValue; //T6 to F2(SP)

		//800464A4
		
		//80046520
		readValue = ReadBitsFrom80045FF0Buffer(3, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x6] = (unsigned short)readValue; //T7 to F4(SP)
		
		//800465A8

		//800465AC
		readValue = ReadBitsFrom80045FF0Buffer(3, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
		shortsSPE8[0x7] = (unsigned short)readValue; //T6 to F6(SP)

		if (outDebug != NULL)
			fprintf(outDebug, "800465AC: ShortsSPE8 %04X %04X %04X %04X %04X %04X %04X %04X\n", shortsSPE8[0], shortsSPE8[1], shortsSPE8[2], shortsSPE8[3], shortsSPE8[4], shortsSPE8[5], shortsSPE8[6], shortsSPE8[7]);

		for (int x = 0; x < 4; x++)
		{
			//80046624
			readValue = ReadBitsFrom80045FF0Buffer(7, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
			shortsSPE0[x] = (unsigned short)readValue; //T7 to 0(A2)

			//800466A0

			readValue = ReadBitsFrom80045FF0Buffer(2, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
			shortsSPD0[x] = (unsigned short)readValue; //T8 to 0(A3)

			//80046720

			readValue = ReadBitsFrom80045FF0Buffer(2, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
			stackBuffer2Offsets[x] = (unsigned short)readValue; //T7 to 0(T1)

			//800467A0

			readValue = ReadBitsFrom80045FF0Buffer(6, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
			shortsSPC8[x] = (unsigned short)readValue; //T8 to 0(T2)

			for (int y = 0; y < 0xD; y++)
			{
				readValue = ReadBitsFrom80045FF0Buffer(3, buffer800D3ED8MORTRawInputDataBuffer, currentInputData, bitsleft, currentOverallBitPosition);
				shortsSP60[x][y] = (unsigned short)readValue; //T7 to 0(A0)
			}

			//80046E6C

			//80046E70
		}

		//80046E78
		numberSkipResetPredictorCheck--;

		//80046EA8

		//80046EB8
		Function80045C78(currentIntermediateValueOffset, shortsSP60, shortsSPC8, shortsSPD0, stackBuffer2Offsets, shortsSPE0, shortsSPE8, pcmSamples);

		//80046EC0
	}

//80046EC8
	AT = -0x20;
	T9 = currentOverallBitPosition & AT;
	T6 = T9 - bitsleft;
	T8 = T6 + 0x20;
	
	variable800D4EFCInputChunkCurrentReadBitPosition = T8;

	// RA = 2C(SP);
	// S0 = 28(SP);
	//SP = SP + 0xF8;
	currentInputData = 0;
}

//TWINE 8009DBE8
void CMORTDecoder::Function80045C78(int currentIntermediateValueOffset, unsigned short shortsSP60[4][0xD], unsigned short shortsSPC8[0x4], unsigned short shortsSPD0[0x4], unsigned short stackBuffer2Offsets[0x4], unsigned short shortsSPE0[0x4], unsigned short shortsSPE8[8], std::vector<unsigned short>& pcmSamples)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//SP = SP - 0x98;
	//RA to 3C(SP)
	//A1 to 9C(SP)

	//S6 = SP + 0x44;

	//800AA38C
	unsigned short stackBuffer2[0x28];
	for (int x = 0; x < 0x28; x++)
	{
		stackBuffer2[x] = 0x00;
	}

	for (int x = 0; x < 4; x++)
	{
		if (outDebug != NULL)
			fprintf(outDebug, "80046E70: Index %X ShortsSPE0 %04X ShortsSPD0 %04X ShortsSPD8 %04X ShortsSPC8 %04X - ShortsSP60 %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X\n", x,
				shortsSPE0[x], shortsSPD0[x], stackBuffer2Offsets[x], shortsSPC8[x],
				shortsSP60[x][0], shortsSP60[x][1], shortsSP60[x][2], shortsSP60[x][3],
				shortsSP60[x][4], shortsSP60[x][5], shortsSP60[x][6], shortsSP60[x][7],
				shortsSP60[x][8], shortsSP60[x][9], shortsSP60[x][0xA], shortsSP60[x][0xB],
				shortsSP60[x][0xC]);

		//80045CCC
		if (outDebug != NULL)
			fprintf(outDebug, "80045CCC: Function80048590: shortsSPC8 %04X stackBuffer2Offsets %04X Index %02X\n", shortsSPC8[x], stackBuffer2Offsets[x], x);
		//80045CD8

		// Updating stackBuffer2 at stackBuffer2Offset, every 3 values, with shortsSP60 values, adjusted by table based on shortsSPC8Value
		Function80048590((signed short)shortsSPC8[x], (signed short)stackBuffer2Offsets[x], stackBuffer2, shortsSP60[x]);

		/*FILE* out = fopen("C:\\temp\\b.bin", "wb");
		fwrite(stackBuffer2, 1, 0x50, out);
		fflush(out);
		fclose(out);*/

		//80045CE0
		if (outDebug != NULL)
			fprintf(outDebug, "80045CE0: Function80048684: shortsSPE0 %04X shortsSPD0 %04X\n", shortsSPE0[x], shortsSPD0[x]);
		//80045CEC
		// Move back predictors, 0x28-0xA0 to 0x00 to 0x78
		// Read 0x28 of previous predictors, from 0x78 - shortsSPE0Value, adjusted by shortsSPD0Value value, add to stackBuffer2 value, and write to 0x78 index of predictors forward
		Function80048684((signed short)shortsSPE0[x], (signed short)shortsSPD0[x], stackBuffer2);

		//80045D08
	}

	//80045D10
	//S5 to B0(SP);
	//S4 to A8(SP);
	//S3 to AC(SP);

	Function80045A80(currentIntermediateValueOffset, shortsSPE8, pcmSamples);
}

// TWINE 8009EE60
void CMORTDecoder::Function80048590(unsigned short shortsSPC8Value, unsigned short stackBuffer2Offset, unsigned short stackBuffer2[0x28], unsigned short shortsSP60[0xD])
{
	unsigned long AT = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	// NEMU FOR SOME REASON LOADS THIS WRONG
	signed char table8004867CValue1[0x8];
	table8004867CValue1[0x00] = (signed char)0xFC;
	table8004867CValue1[0x01] = (signed char)0xFD;
	table8004867CValue1[0x02] = (signed char)0xFE;
	table8004867CValue1[0x03] = (signed char)0xFE;
	table8004867CValue1[0x04] = (signed char)0xFF;
	table8004867CValue1[0x05] = (signed char)0xFF;
	table8004867CValue1[0x06] = (signed char)0xFF;
	table8004867CValue1[0x07] = (signed char)0xFF;
	unsigned char table8004867CValue2[0x8];
	table8004867CValue2[0x00] = 0x7;
	table8004867CValue2[0x01] = 0x7;
	table8004867CValue2[0x02] = 0x3;
	table8004867CValue2[0x03] = 0x7;
	table8004867CValue2[0x04] = 0x1;
	table8004867CValue2[0x05] = 0x3;
	table8004867CValue2[0x06] = 0x5;
	table8004867CValue2[0x07] = 0x7;

	if (shortsSPC8Value < 8)
	{
		T0 = (signed long)table8004867CValue1[shortsSPC8Value];
		T1 = table8004867CValue2[shortsSPC8Value];
	}
	else
	{
		T0 = (shortsSPC8Value - 8) >> 3;
		T1 = shortsSPC8Value & 7;
	}

	T0 = 6 - T0;

	//800485D8
	T4 = (T1 * 0x800) + 0x47FF;

	//800485E4

	for (int x = 0; x < 0x28; x++)
	{
		stackBuffer2[x] = 0; //0 to 0(T9);
	}

	for (int x = 0; x < 0xD; x++)
	{
		//800485FC

		//80048614
		T5 = ((int)(((((signed short)shortsSP60[x] * 0x2000) - 0x7000) * T4) + 0x4000) >> 0xF);
		T5 = (int)(T5 + (1 << (T0 - 1))) >> T0; // Sort of round up when shifting?

		shortsSP60[x] = (unsigned short)T5; //T5 to 0(T8);

		//8004862C

		stackBuffer2[stackBuffer2Offset + (x * 3)] = (signed short)shortsSP60[x]; // AT to 0(T9);

		if (outDebug != NULL)
			fprintf(outDebug, "8004862C: STACK BUFFER2 WRITE %04X to %04X\n", shortsSP60[x], (stackBuffer2Offset + (x * 3)));
	}

	//80048634

	//8004864C

	//8004865C
}

// Rogue 8009D8C4
// TWINE 8009EF54
void CMORTDecoder::Function80048684(unsigned short shortsSPE0Value, unsigned short shortsSPD0Value, unsigned short stackBuffer2[0x28])
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	unsigned long RA = 0;

	// NEMU FOR SOME REASON LOADS THIS WRONG
	unsigned short table80048738[0x6];
	table80048738[0x00] = 0x0CCD;
	table80048738[0x01] = 0x2CCD;
	table80048738[0x02] = 0x5333;
	table80048738[0x03] = 0x7FFF;
	table80048738[0x04] = 0x852A;
	table80048738[0x05] = 0x0000;

	if ((shortsSPE0Value >= 0x28) && (((int)shortsSPE0Value) <= 0x78))
	{
		lastPredictorUpdateBase = (unsigned short)shortsSPE0Value;
	}

	//800486A4
	//BLTZAL NOOP

	//800486B4


	if (outDebug != NULL)
		fprintf(outDebug, "800486D8: PREDICTOR VALUE SHIFT 0x28\n");

	//800486C4
	// Move back predictors, 0x50-0x140 to 0x00 to 0xF0
	for (int x = 0; x < 0x78; x++)
	{
		//800486D0
		T4 = buffer800D2940Predictor[x + 0x28]; // T4 = 0x50(T5)

		//800486D8
		buffer800D2940Predictor[x] = (unsigned short)T4; // T4 to 0(T5);
	}

	//800486E4

	//800486FC
	// Read 0x50 of predictors, from F0 - (2 * passed in spot), adjusted by shortsSPD0Value value, add to stackBuffer2 value, and write to F0
	for (int x = 0; x < 0x28; x++)
	{
		T1 = (signed short)buffer800D2940Predictor[(0x78 - (signed long)lastPredictorUpdateBase + x)]; //F0(T5);

		if (outDebug != NULL)
			fprintf(outDebug, "800486FC: PREDICTOR VALUE READ %04X from %04X\n", (unsigned short)(T1 & 0xFFFF), (0x78 - (signed long)lastPredictorUpdateBase + x));


		T1 = ((int)((T1 * (unsigned short)table80048738[shortsSPD0Value]) + 0x4000) >> 0xF);

		//80048714
		if (outDebug != NULL)
			fprintf(outDebug, "80048714: STACK BUFFER2 READ %04X from %04X\n", stackBuffer2[x], x);

		//800D48720
		buffer800D2940Predictor[0x78 + x] = (unsigned short)(T1 + (signed short)stackBuffer2[x]); //T1 to 0xF0(T6);
		if (outDebug != NULL)
			fprintf(outDebug, "800486D8: PREDICTOR VALUE WRITE %04X to %04X\n", (unsigned short)(T1 + (signed short)stackBuffer2[x]), 0x78 + x);
	}
}

//TWINE 8009D9F0
void CMORTDecoder::Function80045A80(unsigned long currentIntermediateValueOffset, unsigned short shortsSPE8[8], std::vector<unsigned short>& pcmSamples)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//SP = SP - 0x20;
	//RA to 1C(SP)
	//S0 to 18(SP)
	//A2 to 28(SP)

	// Alternating 0/1
	signed short* smootherPredictor;
	if (!currentSmootherPredictor)
		smootherPredictor = smootherPredictorA;
	else
		smootherPredictor = smootherPredictorB;

	//80045AB8

	//80045AC4
	T4 = (int)(((((signed short)shortsSPE8[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
	smootherPredictor[0] = (unsigned short)T4; //T4 to 0(V0);

	T1 = (int)(((((signed short)shortsSPE8[0x1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
	smootherPredictor[1] = (unsigned short)T1; //T1 to 2(V0);
	
	//80045AF8
	T8 = (int)(((((signed short)shortsSPE8[0x2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
	smootherPredictor[2] = (unsigned short)T8; //T8 to 4(V0);

	//80045B1C
	T5 = (int)(((((signed short)shortsSPE8[0x3] - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
	smootherPredictor[3] = (unsigned short)T5; //T5 to 6(V0);

	//80045B48
	T2 = (int)(((((signed short)shortsSPE8[0x4] - 8) * (unsigned long)0x400) * (unsigned long)0x4B17) + 0xFFC91B1C) >> 0xF << 1;
	smootherPredictor[4] = (unsigned short)T2; //T2 to 8(V0);

	//80045B78
	T9 = (int)(((((signed short)shortsSPE8[0x5] - 8) * (unsigned long)0x400) * (unsigned long)0x4444) + 0x03BBF800) >> 0xF << 1;
	smootherPredictor[5] = (unsigned short)T9; //T9 to A(V0);

	//80045BB8
	T6 = (int)(((((signed short)shortsSPE8[0x6] - 4) * (unsigned long)0x400) * (unsigned long)0x7ADE) + 0x147936C) >> 0xF << 1;
	smootherPredictor[6] = (unsigned short)T6; //T6 to C(V0);

	//80045C00
	T3 = (int)((((((signed short)shortsSPE8[0x7] - 4) * (unsigned long)0x400) * (unsigned long)0x740C) + 0x40D6B40)) >> 0xF << 1;
	smootherPredictor[7] = (unsigned short)T3; //T3 to E(V0);

	//80045C48
	//A0 to 20(SP);

	//80045C4C
	Function80048904(currentIntermediateValueOffset, pcmSamples);
	//A0 = 20(SP);

	// Flip between 0/1
	currentSmootherPredictor = !currentSmootherPredictor;
	//RA = 1A(SP)
	//S0 = 18(SP)

	// SP = SP + 0x20;
}

void CMORTDecoder::Function80048904(unsigned long currentIntermediateValueOffset, std::vector<unsigned short>& pcmSamples)
{
	unsigned long AT = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//RA to 0(SP)
	//A0 to 4(SP)
	//A1 to 8(SP)
	//A2 to C(SP)
	//A3 to 10(SP)
	//S0 to 14(SP)
	//S1 to 18(SP)
	//S2 to 1C(SP)
	//S3 to 20(SP)
	//S4 to 24(SP)
	//S5 to 28(SP)
	//S6 to 2C(SP)
	//S7 to 30(SP)

	S0 = (signed long)sampleBuffer[0]; //0(AT)
	S1 = (signed long)sampleBuffer[1]; //4(AT)
	S2 = (signed long)sampleBuffer[2]; //8(AT)
	S3 = (signed long)sampleBuffer[3]; //C(AT)
	S4 = (signed long)sampleBuffer[4]; //10(AT)
	S5 = (signed long)sampleBuffer[5]; //14(AT)
	S6 = (signed long)sampleBuffer[6]; //18(AT)
	S7 = (signed long)sampleBuffer[7]; //1C(AT)
	A3 = (signed short)lastSampleValue; //0x164(A0)

	// Function pointer formation to 80048AFC

	// 80048970
	signed short adjusters[8];
	for (int x = 0; x < 8; x++)
		adjusters[x] = 0x0000;
	// T2 = (T2 / 4) + (T4 * (3 / 4)) and Smooth
	// HACK (commented out is hack)
	Function80048A58(0x80048AFC, adjusters);

	//80048978
	Function80048740(currentIntermediateValueOffset, 0, 0xD, adjusters, A3, S0, S1, S2, S3, S4, S5, S6, S7, pcmSamples);

	//80048990

	//80048994
	// T2 / 2 + T4 / 2 and Smooth
	Function80048A58(0x80048B14, adjusters);

	//8004899C
	Function80048740((currentIntermediateValueOffset + 0x1A), 0xD, 0xE, adjusters, A3, S0, S1, S2, S3, S4, S5, S6, S7, pcmSamples);

	//800489B8
	// T2 = (T2 * (3 / 4)) + (T4 / 4) and Smooth
	Function80048A58(0x80048B24, adjusters);

	//800489C0
	Function80048740((currentIntermediateValueOffset + 0x36), 0x1B, 0xD, adjusters, A3, S0, S1, S2, S3, S4, S5, S6, S7, pcmSamples);

	//800489DC
	// Smooth
	Function80048A58(0x80048B3C, adjusters);

	//800489E4
	Function80048740((currentIntermediateValueOffset + 0x50), 0x28, 0x78, adjusters, A3, S0, S1, S2, S3, S4, S5, S6, S7, pcmSamples);
	
	// set stuff back
	/*FILE* a = fopen("C:\\temp\\c.bin", "w");
	fwrite(buffer800D2AD8IntermediateValue, 1, 0x1400, a);
	fflush(a);
	fclose(a);*/
	
	//800489F8
	sampleBuffer[0] = S0; //0(AT)
	sampleBuffer[1] = S1; //4(AT)
	sampleBuffer[2] = S2; //8(AT)
	sampleBuffer[3] = S3; //C(AT)
	sampleBuffer[4] = S4; //10(AT)
	sampleBuffer[5] = S5; //14(AT)
	sampleBuffer[6] = S6; //18(AT)
	sampleBuffer[7] = S7; //1C(AT)
	lastSampleValue = (signed short)A3; //164(A0)
}

void CMORTDecoder::CallT380048XXXFunction(unsigned long& T2, unsigned long T3, unsigned long T4)
{
	if (T3 == 0x80048AFC)
		Function80048AFC(T2, T4); // JALR RA,T3
	else if (T3 == 0x80048B14)
		Function80048B14(T2, T4); // JALR RA,T3
	else if (T3 == 0x80048B24)
		Function80048B24(T2, T4); // JALR RA,T3
	else if (T3 == 0x80048B3C)
		Function80048B3C(T2); // JALR RA,T3
}

void CMORTDecoder::Function80048AFC(unsigned long& T2, unsigned long T4)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T3 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	// T2 = (T2 / 4) + (T4 * (3 / 4))
	T2 = (((int)T2 >> 2) + ((int)T4 >> 1)) + ((int)T4 >> 2);
	
	// Branch always to 80048B3C
	Function80048B3C(T2);
}

void CMORTDecoder::Function80048B14(unsigned long& T2, unsigned long T4)
{
	//80048B14
	// T2 / 2 + T4 / 2
	T2 = ((int)T2 >> 1) + ((int)T4 >> 1);

	// Branch always to 80048B3C
	Function80048B3C(T2);
}

void CMORTDecoder::Function80048B24(unsigned long& T2, unsigned long T4)
{
	// T2 = (T2 * (3 / 4)) + (T4 / 4)
	T2 = (((int)T2 >> 1) + ((int)T4 >> 2)) + ((int)T2 >> 2);

	// Branch always to 80048B3C
	Function80048B3C(T2);
}

void CMORTDecoder::Function80048B3C(unsigned long& T2)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T3 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	//80048B3C
	bool isNegative = false;
	if ((int)T2 < 0)
	{
		T2 = -(int)T2;
		isNegative = true;
	}

	if (T2 < 0x2B33)
	{
		T2 = T2 * 2;
	}
	else if (T2 < 0x4E66)
	{
		T2 = T2 + 0x2B33;
	}
	else
	{
		T2 = (int)T2 >> 2;

		T2 = T2 + 0x6600;
	}

	//80048B6C
	if (T2 > 0x7FFF)
	{
		T2 = 0x7FFF;
	}

	//80048B7C
	if (isNegative)
	{
		T2 = -(signed)T2;
	}

	T2 = T2 & 0xFFFF;
}

void CMORTDecoder::Function80048740(unsigned long intermediateValueOffset, unsigned long predictorBufferOffset, int countValues, signed short adjusters[8], unsigned long& A3, unsigned long& S0, unsigned long& S1, unsigned long& S2, unsigned long& S3, unsigned long& S4, unsigned long& S5, unsigned long& S6, unsigned long& S7, std::vector<unsigned short>& pcmSamples)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S8 = 0;
	unsigned long T2 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;

	//80048740
	for (int x = 0; x < countValues; x++)
	{
		//80048758

		T2 = (signed short)buffer800D2940Predictor[predictorBufferOffset + x]; //0(T1)
		if (outDebug != NULL)
			fprintf(outDebug, "80048740: PREDICTOR VALUE READ %04X from %04X\n", (unsigned short)(T2 & 0xFFFF), predictorBufferOffset + x);

		//80048770
		T2 = (T2 - ((int)((S7 * (int)(adjusters[7])) + 0x4000) >> 0xF));
		
		T2 = (T2 - ((int)((S6 * (int)(adjusters[6])) + 0x4000) >> 0xF));

		//80048784
		S7 = (((int)((int)(adjusters[6]) * T2 + 0x4000)) >> 0xF) + S6;

		//800487A0
		T2 = T2 - (((int)((S5 * (int)(adjusters[5])) + 0x4000)) >> 0xF);

		//800487B4
		S6 = ((((int)((int)(adjusters[5]) * T2) + 0x4000) >> 0xF) + S5);

		//800487CC
		T2 = T2 - ((int)((S4 * (int)(adjusters[4])) + 0x4000) >> 0xF);

		//800487E0
		S5 = ((int)(((int)(adjusters[4]) * T2) + 0x4000) >> 0xF) + S4;

		//800487FC
		T2 = T2 - ((int)((S3 * (int)(adjusters[3])) + 0x4000) >> 0xF);

		//80048810
		S4 = (((int)((int)(adjusters[3]) * T2) + 0x4000) >> 0xF) + S3;

		//80048828
		T2 = (T2 - ((int)((S2 * ((int)(adjusters[2]))) + 0x4000) >> 0xF));
		
		//8004883C
		S3 = ((int)(((int)(adjusters[2]) * T2) + 0x4000) >> 0xF) + S2;

		//80048858
		T2 = T2 - ((int)((S1 * ((int)(adjusters[1]))) + 0x4000) >> 0xF);

		//8004886C
		S2 = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1;

		//80048884
		T2 = T2 - ((int)((S0 * ((int)(adjusters[0]))) + 0x4000) >> 0xF);

		//80048898
		S1 = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0;

		S0 = T2;

		//800488B4
		T2 = T2 + ((int)((0x6E14 * A3) + 0x4000) >> 0xF);

		//800488C0
		A3 = T2;

		T2 = T2 << 1;

		AT = ((int)(((int)T2 >> 0xF) + 1) >> 1);
		//800488D4
		if (AT != 0)
		{
			T2 = -0x8000;

			if ((int)AT < 0)
			{
				T2 = -0x8000;
			}
			else
			{
				T2 = 0x7FFF;
			}
		}

		//800488E8
		T2 = T2 & 0xFFF8;

		//800488EC
		WriteShortToBuffer(buffer800D2AD8IntermediateValue, intermediateValueOffset + (x * 2), (unsigned short)T2);
		if (outDebug != NULL)
			fprintf(outDebug, "800488EC: INTERMEDIATE VALUE WRITE %04X to %04X\n", (unsigned short)(T2 & 0xFFFF), (intermediateValueOffset + (x * 2)));
		pcmSamples.push_back((unsigned short)T2);
	}
}

void CMORTDecoder::Function80048A58(unsigned long algorithm, signed short adjusters[8])
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = algorithm;
	unsigned long T4 = 0;
	unsigned long T5 = 0;

	signed short* smootherPredictor1;
	signed short* smootherPredictor2;
	if (!currentSmootherPredictor)
	{
		smootherPredictor1 = smootherPredictorA;
		smootherPredictor2 = smootherPredictorB;
	}
	else
	{
		smootherPredictor1 = smootherPredictorB;
		smootherPredictor2 = smootherPredictorA;
	}

	//80048A70
	//T5 = RA;

	//80048A74
	for (int x = 0; x < 8; x++)
	{
		T2 = (signed short)smootherPredictor1[x]; //0(T0);
		T4 = (signed short)smootherPredictor2[x]; //0(T1)	
		CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
		adjusters[x] = (T2 & 0xFFFF);
	}
	//RA back to T5 original
}

void CMORTDecoder::Function80045A48()
{
	unsigned long AT = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	T6 = variable800D4F11Status2;
	AT = 2;
	T7 = 3;

	if (T6 == AT)
	{
		variable800D4F11Status2 = (unsigned char)T7;
	}
}

void CMORTDecoder::Function80045A68()
{
	unsigned long AT = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long A3 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = 0;
	unsigned long T1 = 0;
	unsigned long T2 = 0;
	unsigned long T3 = 0;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = 0;
	unsigned long T7 = 0;
	unsigned long T8 = 0;
	unsigned long T9 = 0;

	T6 = 4;

	variable800D4F11Status2 = (unsigned char)T6;
}

void CMORTDecoder::WriteBitsTo80045FF0Buffer(unsigned char* buffer, int& bufferOffset, int& bufferBitOffset, int numBits, unsigned char value)
{
	int numBitsLeft = (0x20 - bufferBitOffset);
	if (numBits > numBitsLeft)
	{
		// Partial
		unsigned long valueBuffer = CharArrayToLong(&buffer[bufferOffset]);
		valueBuffer |= (value << bufferBitOffset);
		WriteLongToBuffer(buffer, bufferOffset, valueBuffer);

		numBits -= numBitsLeft;
		value = value >> numBitsLeft;
		numBitsLeft = 0x20;
		bufferBitOffset = 0;
		bufferOffset += 4;
	}

	unsigned long valueBuffer = CharArrayToLong(&buffer[bufferOffset]);
	valueBuffer |= (value << bufferBitOffset);
	WriteLongToBuffer(buffer, bufferOffset, valueBuffer);

	numBitsLeft -= numBits;
	bufferBitOffset += numBits;

	if (bufferBitOffset == 0x20)
	{
		bufferOffset += 4;
		bufferBitOffset = 0;
	}
}

bool CMORTDecoder::ReadWavData(CString rawWavFileName, unsigned char*& rawData, int& rawLength, unsigned long& samplingRate, bool& hasLoopData, unsigned char& keyBase, unsigned long& loopStart, unsigned long& loopEnd, unsigned long& loopCount)
{
	FILE* inWavFile = fopen(rawWavFileName, "rb");
	if (inWavFile == NULL)
	{
		MessageBox(NULL, "Error cannot read wav file", "Error", NULL);
		return false;
	}

	fseek(inWavFile, 0, SEEK_END);
	int fileSize = ftell(inWavFile);
	rewind(inWavFile);

	unsigned char* wavData = new unsigned char[fileSize];
	fread(wavData, 1, fileSize, inWavFile);
	fclose(inWavFile);

	if (((((((wavData[0] << 8) | wavData[1]) << 8) | wavData[2]) << 8) | wavData[3]) != 0x52494646)
	{
		delete [] wavData;
		MessageBox(NULL, "Error not RIFF wav", "Error", NULL);
		return false;
	}

	if (((((((wavData[0x8] << 8) | wavData[0x9]) << 8) | wavData[0xA]) << 8) | wavData[0xB]) != 0x57415645)
	{
		delete [] wavData;
		MessageBox(NULL, "Error not WAVE wav", "Error", NULL);
		return false;
	}

	bool endFlag = false;

	unsigned long currentOffset = 0xC;

	unsigned short channels = 0;
	samplingRate = 0;
	unsigned short bitRate = 0;

	bool returnFlag = false;

	while (!endFlag)
	{
		if ((int)currentOffset >= (fileSize - 8))
			break;

		unsigned long sectionType = ((((((wavData[currentOffset] << 8) | wavData[currentOffset + 1]) << 8) | wavData[currentOffset + 2]) << 8) | wavData[currentOffset + 3]);

		if (sectionType == 0x666D7420) // fmt
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			channels = ((wavData[currentOffset + 0xB] << 8) | wavData[currentOffset + 0xA]);

			if (channels != 0x0001)
			{
				MessageBox(NULL, "Warning: Only mono wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
			}

			samplingRate = ((((((wavData[currentOffset + 0xF] << 8) | wavData[currentOffset + 0xE]) << 8) | wavData[currentOffset + 0xD]) << 8) | wavData[currentOffset + 0xC]);
			
			bitRate = ((wavData[currentOffset + 0x17] << 8) | wavData[currentOffset + 0x16]);

			currentOffset += chunkSize + 8;
		}
		else if (sectionType == 0x64617461) // data
		{
			rawLength = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			if ((channels == 0) || (samplingRate == 0) || (bitRate == 0))
			{
				MessageBox(NULL, "Incorrect section type (missing fmt)", "Error unknown wav format", NULL);
				endFlag = true;
				returnFlag = false;
			}

			if (bitRate == 0x0010)
			{
				rawData = new unsigned char[rawLength];
				for (int x = 0; x < rawLength; x++)
				{
					rawData[x] = wavData[currentOffset + 0x8 + x];
				}
			
				returnFlag = true;
			}
			else
			{
				MessageBox(NULL, "Error only 16-bit PCM wav supported", "Error", NULL);
				endFlag = true;
				returnFlag = false;
			}

			currentOffset += rawLength + 8;
		}
		else if (sectionType == 0x736D706C) // smpl
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			unsigned long numSampleBlocks = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x24]));
			if (numSampleBlocks > 0)
			{
				hasLoopData = true;

				keyBase = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x14])) & 0xFF;
				loopStart = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x34]));
				loopEnd = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x38]));
				loopCount = Flip32Bit(CharArrayToLong(&wavData[currentOffset+0x40]));
				if (loopCount == 0)
					loopCount = 0xFFFFFFFF;
			}

			currentOffset += 8 + chunkSize;
		}
		else
		{
			unsigned long chunkSize = ((((((wavData[currentOffset + 0x7] << 8) | wavData[currentOffset + 0x6]) << 8) | wavData[currentOffset + 0x5]) << 8) | wavData[currentOffset + 0x4]);

			currentOffset += 8 + chunkSize;
		}
	}

	delete [] wavData;
	return returnFlag;
}

void CMORTDecoder::CalculateBestPredictors(std::vector<unsigned short> actualValues, int actualValueOffset, unsigned short buffer800D2940PredictorTemp[0xA0], unsigned short shortsSP60[4][0xD], unsigned short shortsSPC8[0x4], unsigned short shortsSPD0[0x4], unsigned short stackBuffer2Offsets[0x4], unsigned short shortsSPE0[0x4], unsigned long& S0, unsigned long& S1, unsigned long& S2, unsigned long& S3, unsigned long& S4, unsigned long& S5, unsigned long& S6, unsigned long& S7, signed long& A3, signed short smoothersChosen[8], signed short adjustersPrevious[8])
{
	signed char table8004867CValue1[0x8];
	table8004867CValue1[0x00] = (signed char)0xFC;
	table8004867CValue1[0x01] = (signed char)0xFD;
	table8004867CValue1[0x02] = (signed char)0xFE;
	table8004867CValue1[0x03] = (signed char)0xFE;
	table8004867CValue1[0x04] = (signed char)0xFF;
	table8004867CValue1[0x05] = (signed char)0xFF;
	table8004867CValue1[0x06] = (signed char)0xFF;
	table8004867CValue1[0x07] = (signed char)0xFF;
	unsigned char table8004867CValue2[0x8];
	table8004867CValue2[0x00] = 0x7;
	table8004867CValue2[0x01] = 0x7;
	table8004867CValue2[0x02] = 0x3;
	table8004867CValue2[0x03] = 0x7;
	table8004867CValue2[0x04] = 0x1;
	table8004867CValue2[0x05] = 0x3;
	table8004867CValue2[0x06] = 0x5;
	table8004867CValue2[0x07] = 0x7;


	unsigned short table80048738[0x6];
	table80048738[0x00] = 0x0CCD;
	table80048738[0x01] = 0x2CCD;
	table80048738[0x02] = 0x5333;
	table80048738[0x03] = 0x7FFF;
	table80048738[0x04] = 0x852A;
	table80048738[0x05] = 0x0000;

	for (int slot = 0; slot < 4; slot++)
	{
		unsigned short shortsSPC8Value = shortsSPC8[slot];
		unsigned short shortsSP60Values[0xD];
		for (int d = 0; d < 0xD; d++)
			shortsSP60Values[d] = shortsSP60[slot][d];
		unsigned short shortsSPE0Value = shortsSPE0[slot];
		unsigned short shortsSPD0Value = shortsSPD0[slot];
		unsigned short stackBuffer2OffsetValue = stackBuffer2Offsets[slot];

		unsigned long T0;
		unsigned long T1;
		if (shortsSPC8Value < 8)
		{
			T0 = (signed long)table8004867CValue1[shortsSPC8Value];
			T1 = table8004867CValue2[shortsSPC8Value];
		}
		else
		{
			T0 = (shortsSPC8Value - 8) >> 3;
			T1 = shortsSPC8Value & 7;
		}

		T0 = 6 - T0;

		//800485D8
		unsigned long T4 = (T1 * 0x800) + 0x47FF;

		//800485E4

		unsigned short stackBuffer2[0x28];
		for (int x = 0; x < 0x28; x++)
		{
			stackBuffer2[x] = 0; //0 to 0(T9);
		}

		for (int x = 0; x < 0xD; x++)
		{
			unsigned long T5 = ((int)(((((signed short)shortsSP60Values[x] * 0x2000) - 0x7000) * T4) + 0x4000) >> 0xF);
			T5 = (int)(T5 + (1 << (T0 - 1))) >> T0; // Sort of round up when shifting?

			stackBuffer2[stackBuffer2OffsetValue + (x * 3)] = (signed short)(unsigned short)T5; // AT to 0(T9);
		}

		for (int x = 0; x < 0x78; x++)
		{
			T4 = buffer800D2940PredictorTemp[x + 0x28]; // T4 = 0x50(T5)

			buffer800D2940PredictorTemp[x] = (unsigned short)T4; // T4 to 0(T5);
		}

		for (int x = 0; x < 0x28; x++)
		{
			T1 = (signed short)buffer800D2940PredictorTemp[(0x78 - (signed long)shortsSPE0Value + x)]; //F0(T5);
			T1 = ((int)((T1 * (unsigned short)table80048738[shortsSPD0Value]) + 0x4000) >> 0xF);
			buffer800D2940PredictorTemp[0x78 + x] = (unsigned short)(T1 + (signed short)stackBuffer2[x]); //T1 to 0xF0(T6);
		}
	}

	signed short adjusters[8];

	unsigned long bestDelta = 0xFFFFFFFF;
	int bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 0x40; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)testSmootherValue - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;

			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;


			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[0] = bestDeltaSmooth;

	bestDelta = 0xFFFFFFFF;
	bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 0x40; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;
			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;

			temp = (int)(((((signed short)testSmootherValue - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[1] = (signed short)temp;
			temp = (signed short)adjusters[1];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
			adjusters[1] = (signed short)temp;


			T2 = T2 - ((int)((S1Temp * ((int)(adjusters[1]))) + 0x4000) >> 0xF);

			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S1Temp = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0Temp;

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[1] = bestDeltaSmooth;

	bestDelta = 0xFFFFFFFF;
	bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 0x20; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;
			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[1] = (signed short)temp;
			temp = (signed short)adjusters[1];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
			adjusters[1] = (signed short)temp;

			temp = (int)(((((signed short)testSmootherValue - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
			adjusters[2] = (signed short)temp;
			temp = (signed short)adjusters[2];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[2]); // JALR RA,T3 to 80048AFC
			adjusters[2] = (signed short)temp;

			T2 = (T2 - ((int)((S2Temp * ((int)(adjusters[2]))) + 0x4000) >> 0xF));

			T2 = T2 - ((int)((S1Temp * ((int)(adjusters[1]))) + 0x4000) >> 0xF);
			
			S2Temp = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1Temp;

			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S1Temp = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0Temp;

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[2] = bestDeltaSmooth;

	bestDelta = 0xFFFFFFFF;
	bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 0x20; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;
			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[1] = (signed short)temp;
			temp = (signed short)adjusters[1];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
			adjusters[1] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
			adjusters[2] = (signed short)temp;
			temp = (signed short)adjusters[2];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[2]); // JALR RA,T3 to 80048AFC
			adjusters[2] = (signed short)temp;

			temp = (int)(((((signed short)testSmootherValue - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
			adjusters[3] = (signed short)temp;
			temp = (signed short)adjusters[3];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[3]); // JALR RA,T3 to 80048AFC
			adjusters[3] = (signed short)temp;

			T2 = T2 - ((int)((S3Temp * (int)(adjusters[3])) + 0x4000) >> 0xF);

			T2 = (T2 - ((int)((S2Temp * ((int)(adjusters[2]))) + 0x4000) >> 0xF));

			S3Temp = ((int)(((int)(adjusters[2]) * T2) + 0x4000) >> 0xF) + S2Temp;

			T2 = T2 - ((int)((S1Temp * ((int)(adjusters[1]))) + 0x4000) >> 0xF);
			
			S2Temp = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1Temp;

			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S1Temp = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0Temp;

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[3] = bestDeltaSmooth;

	bestDelta = 0xFFFFFFFF;
	bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 0x10; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;
			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[1] = (signed short)temp;
			temp = (signed short)adjusters[1];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
			adjusters[1] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
			adjusters[2] = (signed short)temp;
			temp = (signed short)adjusters[2];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[2]); // JALR RA,T3 to 80048AFC
			adjusters[2] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[3] - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
			adjusters[3] = (signed short)temp;
			temp = (signed short)adjusters[3];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[3]); // JALR RA,T3 to 80048AFC
			adjusters[3] = (signed short)temp;

			temp = (int)(((((signed short)testSmootherValue - 8) * (unsigned long)0x400) * (unsigned long)0x4B17) + 0xFFC91B1C) >> 0xF << 1;
			adjusters[4] = (signed short)temp;
			temp = (signed short)adjusters[4];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[4]); // JALR RA,T3 to 80048AFC
			adjusters[4] = (signed short)temp;

			T2 = T2 - ((int)((S4Temp * (int)(adjusters[4])) + 0x4000) >> 0xF);

			T2 = T2 - ((int)((S3Temp * (int)(adjusters[3])) + 0x4000) >> 0xF);

			S4Temp = (((int)((int)(adjusters[3]) * T2) + 0x4000) >> 0xF) + S3Temp;

			T2 = (T2 - ((int)((S2Temp * ((int)(adjusters[2]))) + 0x4000) >> 0xF));

			S3Temp = ((int)(((int)(adjusters[2]) * T2) + 0x4000) >> 0xF) + S2Temp;

			T2 = T2 - ((int)((S1Temp * ((int)(adjusters[1]))) + 0x4000) >> 0xF);
			
			S2Temp = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1Temp;

			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S1Temp = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0Temp;

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[4] = bestDeltaSmooth;

	bestDelta = 0xFFFFFFFF;
	bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 0x10; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;
			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[1] = (signed short)temp;
			temp = (signed short)adjusters[1];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
			adjusters[1] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
			adjusters[2] = (signed short)temp;
			temp = (signed short)adjusters[2];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[2]); // JALR RA,T3 to 80048AFC
			adjusters[2] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[3] - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
			adjusters[3] = (signed short)temp;
			temp = (signed short)adjusters[3];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[3]); // JALR RA,T3 to 80048AFC
			adjusters[3] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[4] - 8) * (unsigned long)0x400) * (unsigned long)0x4B17) + 0xFFC91B1C) >> 0xF << 1;
			adjusters[4] = (signed short)temp;
			temp = (signed short)adjusters[4];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[4]); // JALR RA,T3 to 80048AFC
			adjusters[4] = (signed short)temp;

			temp = (int)(((((signed short)testSmootherValue - 8) * (unsigned long)0x400) * (unsigned long)0x4444) + 0x03BBF800) >> 0xF << 1;
			adjusters[5] = (signed short)temp;
			temp = (signed short)adjusters[5];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[5]); // JALR RA,T3 to 80048AFC
			adjusters[5] = (signed short)temp;
		
			T2 = T2 - (((int)((S5Temp * (int)(adjusters[5])) + 0x4000)) >> 0xF);

			T2 = T2 - ((int)((S4Temp * (int)(adjusters[4])) + 0x4000) >> 0xF);
		
			S5Temp = ((int)(((int)(adjusters[4]) * T2) + 0x4000) >> 0xF) + S4Temp;

			T2 = T2 - ((int)((S3Temp * (int)(adjusters[3])) + 0x4000) >> 0xF);

			S4Temp = (((int)((int)(adjusters[3]) * T2) + 0x4000) >> 0xF) + S3Temp;

			T2 = (T2 - ((int)((S2Temp * ((int)(adjusters[2]))) + 0x4000) >> 0xF));

			S3Temp = ((int)(((int)(adjusters[2]) * T2) + 0x4000) >> 0xF) + S2Temp;

			T2 = T2 - ((int)((S1Temp * ((int)(adjusters[1]))) + 0x4000) >> 0xF);
			
			S2Temp = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1Temp;

			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S1Temp = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0Temp;

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[5] = bestDeltaSmooth;

	bestDelta = 0xFFFFFFFF;
	bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 8; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;
			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[1] = (signed short)temp;
			temp = (signed short)adjusters[1];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
			adjusters[1] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
			adjusters[2] = (signed short)temp;
			temp = (signed short)adjusters[2];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[2]); // JALR RA,T3 to 80048AFC
			adjusters[2] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[3] - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
			adjusters[3] = (signed short)temp;
			temp = (signed short)adjusters[3];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[3]); // JALR RA,T3 to 80048AFC
			adjusters[3] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[4] - 8) * (unsigned long)0x400) * (unsigned long)0x4B17) + 0xFFC91B1C) >> 0xF << 1;
			adjusters[4] = (signed short)temp;
			temp = (signed short)adjusters[4];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[4]); // JALR RA,T3 to 80048AFC
			adjusters[4] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[5] - 8) * (unsigned long)0x400) * (unsigned long)0x4444) + 0x03BBF800) >> 0xF << 1;
			adjusters[5] = (signed short)temp;
			temp = (signed short)adjusters[5];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[5]); // JALR RA,T3 to 80048AFC
			adjusters[5] = (signed short)temp;

			temp = (int)(((((signed short)testSmootherValue - 4) * (unsigned long)0x400) * (unsigned long)0x7ADE) + 0x147936C) >> 0xF << 1;
			adjusters[6] = (signed short)temp;
			temp = (signed short)adjusters[6];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[6]); // JALR RA,T3 to 80048AFC
			adjusters[6] = (signed short)temp;

			T2 = (T2 - ((int)((S6Temp * (int)(adjusters[6])) + 0x4000) >> 0xF));
		
			T2 = T2 - (((int)((S5Temp * (int)(adjusters[5])) + 0x4000)) >> 0xF);

			S6Temp = ((((int)((int)(adjusters[5]) * T2) + 0x4000) >> 0xF) + S5Temp);

			T2 = T2 - ((int)((S4Temp * (int)(adjusters[4])) + 0x4000) >> 0xF);
		
			S5Temp = ((int)(((int)(adjusters[4]) * T2) + 0x4000) >> 0xF) + S4Temp;

			T2 = T2 - ((int)((S3Temp * (int)(adjusters[3])) + 0x4000) >> 0xF);

			S4Temp = (((int)((int)(adjusters[3]) * T2) + 0x4000) >> 0xF) + S3Temp;

			T2 = (T2 - ((int)((S2Temp * ((int)(adjusters[2]))) + 0x4000) >> 0xF));

			S3Temp = ((int)(((int)(adjusters[2]) * T2) + 0x4000) >> 0xF) + S2Temp;

			T2 = T2 - ((int)((S1Temp * ((int)(adjusters[1]))) + 0x4000) >> 0xF);
			
			S2Temp = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1Temp;

			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S1Temp = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0Temp;

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[6] = bestDeltaSmooth;

	bestDelta = 0xFFFFFFFF;
	bestDeltaSmooth = 0;

	for (int testSmootherValue = 0x0; testSmootherValue < 8; testSmootherValue++)
	{
		signed long S0Temp = S0;
		signed long S1Temp = S1;
		signed long S2Temp = S2;
		signed long S3Temp = S3;
		signed long S4Temp = S4;
		signed long S5Temp = S5;
		signed long S6Temp = S6;
		signed long S7Temp = S7;
		signed long A3Temp = A3;

		unsigned long totalDelta = 0;
		for (int x = 0; x < 0xA0; x++)
		{
			unsigned long algorithm;
			if (x < 0xD)
				algorithm = 0x80048AFC;
			else if (x < 0x1B)
				algorithm = 0x80048B14;
			else if (x < 0x28)
				algorithm = 0x80048B24;
			else
				algorithm = 0x80048B3C;

			unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

			unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[0] = (signed short)temp;
			temp = (signed short)adjusters[0];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
			adjusters[0] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
			adjusters[1] = (signed short)temp;
			temp = (signed short)adjusters[1];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
			adjusters[1] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
			adjusters[2] = (signed short)temp;
			temp = (signed short)adjusters[2];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[2]); // JALR RA,T3 to 80048AFC
			adjusters[2] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[3] - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
			adjusters[3] = (signed short)temp;
			temp = (signed short)adjusters[3];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[3]); // JALR RA,T3 to 80048AFC
			adjusters[3] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[4] - 8) * (unsigned long)0x400) * (unsigned long)0x4B17) + 0xFFC91B1C) >> 0xF << 1;
			adjusters[4] = (signed short)temp;
			temp = (signed short)adjusters[4];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[4]); // JALR RA,T3 to 80048AFC
			adjusters[4] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[5] - 8) * (unsigned long)0x400) * (unsigned long)0x4444) + 0x03BBF800) >> 0xF << 1;
			adjusters[5] = (signed short)temp;
			temp = (signed short)adjusters[5];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[5]); // JALR RA,T3 to 80048AFC
			adjusters[5] = (signed short)temp;

			temp = (int)(((((signed short)smoothersChosen[6] - 4) * (unsigned long)0x400) * (unsigned long)0x7ADE) + 0x147936C) >> 0xF << 1;
			adjusters[6] = (signed short)temp;
			temp = (signed short)adjusters[6];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[6]); // JALR RA,T3 to 80048AFC
			adjusters[6] = (signed short)temp;

			temp = (int)((((((signed short)testSmootherValue - 4) * (unsigned long)0x400) * (unsigned long)0x740C) + 0x40D6B40)) >> 0xF << 1;
			adjusters[7] = (signed short)temp;
			temp = (signed short)adjusters[7];
			CallT380048XXXFunction(temp, algorithm, adjustersPrevious[7]); // JALR RA,T3 to 80048AFC
			adjusters[7] = (signed short)temp;

			T2 = (T2 - ((int)((S7Temp * (int)(adjusters[7])) + 0x4000) >> 0xF));

			T2 = (T2 - ((int)((S6Temp * (int)(adjusters[6])) + 0x4000) >> 0xF));

			S7Temp = (((int)((int)(adjusters[6]) * T2 + 0x4000)) >> 0xF) + S6Temp;
		
			T2 = T2 - (((int)((S5Temp * (int)(adjusters[5])) + 0x4000)) >> 0xF);

			S6Temp = ((((int)((int)(adjusters[5]) * T2) + 0x4000) >> 0xF) + S5Temp);

			T2 = T2 - ((int)((S4Temp * (int)(adjusters[4])) + 0x4000) >> 0xF);
		
			S5Temp = ((int)(((int)(adjusters[4]) * T2) + 0x4000) >> 0xF) + S4Temp;

			T2 = T2 - ((int)((S3Temp * (int)(adjusters[3])) + 0x4000) >> 0xF);

			S4Temp = (((int)((int)(adjusters[3]) * T2) + 0x4000) >> 0xF) + S3Temp;

			T2 = (T2 - ((int)((S2Temp * ((int)(adjusters[2]))) + 0x4000) >> 0xF));

			S3Temp = ((int)(((int)(adjusters[2]) * T2) + 0x4000) >> 0xF) + S2Temp;

			T2 = T2 - ((int)((S1Temp * ((int)(adjusters[1]))) + 0x4000) >> 0xF);
			
			S2Temp = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1Temp;

			T2 = (T2 - ((int)((S0Temp * (int)(adjusters[0])) + 0x4000) >> 0xF));

			S1Temp = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0Temp;

			S0Temp = T2;
	
			T2 = T2 + ((int)((0x6E14 * A3Temp) + 0x4000) >> 0xF);

			A3Temp = T2;
			T2 = T2 << 1;
			T2 = T2 & 0xFFF8;

			// Compare to real value and add to total delta
			unsigned long delta = abs((signed short)actualValues[actualValueOffset + x] - (signed short)T2);
			totalDelta += delta;
		}

		// See if was closer overall
		if (totalDelta < bestDelta)
		{
			bestDelta = totalDelta;
			bestDeltaSmooth = testSmootherValue;
		}
	}
	smoothersChosen[7] = bestDeltaSmooth;

	for (int x = 0; x < 0xA0; x++)
	{
		unsigned long algorithm;
		if (x < 0xD)
			algorithm = 0x80048AFC;
		else if (x < 0x1B)
			algorithm = 0x80048B14;
		else if (x < 0x28)
			algorithm = 0x80048B24;
		else
			algorithm = 0x80048B3C;

		unsigned long T2 = (signed short)buffer800D2940PredictorTemp[x];

		unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
		adjusters[0] = (signed short)temp;
		temp = (signed short)adjusters[0];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[0]); // JALR RA,T3 to 80048AFC
		adjusters[0] = (signed short)temp;

		temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
		adjusters[1] = (signed short)temp;
		temp = (signed short)adjusters[1];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[1]); // JALR RA,T3 to 80048AFC
		adjusters[1] = (signed short)temp;

		temp = (int)(((((signed short)smoothersChosen[2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
		adjusters[2] = (signed short)temp;
		temp = (signed short)adjusters[2];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[2]); // JALR RA,T3 to 80048AFC
		adjusters[2] = (signed short)temp;

		temp = (int)(((((signed short)smoothersChosen[3] - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
		adjusters[3] = (signed short)temp;
		temp = (signed short)adjusters[3];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[3]); // JALR RA,T3 to 80048AFC
		adjusters[3] = (signed short)temp;

		temp = (int)(((((signed short)smoothersChosen[4] - 8) * (unsigned long)0x400) * (unsigned long)0x4B17) + 0xFFC91B1C) >> 0xF << 1;
		adjusters[4] = (signed short)temp;
		temp = (signed short)adjusters[4];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[4]); // JALR RA,T3 to 80048AFC
		adjusters[4] = (signed short)temp;

		temp = (int)(((((signed short)smoothersChosen[5] - 8) * (unsigned long)0x400) * (unsigned long)0x4444) + 0x03BBF800) >> 0xF << 1;
		adjusters[5] = (signed short)temp;
		temp = (signed short)adjusters[5];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[5]); // JALR RA,T3 to 80048AFC
		adjusters[5] = (signed short)temp;

		temp = (int)(((((signed short)smoothersChosen[6] - 4) * (unsigned long)0x400) * (unsigned long)0x7ADE) + 0x147936C) >> 0xF << 1;
		adjusters[6] = (signed short)temp;
		temp = (signed short)adjusters[6];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[6]); // JALR RA,T3 to 80048AFC
		adjusters[6] = (signed short)temp;

		temp = (int)((((((signed short)smoothersChosen[7] - 4) * (unsigned long)0x400) * (unsigned long)0x740C) + 0x40D6B40)) >> 0xF << 1;
		adjusters[7] = (signed short)temp;
		temp = (signed short)adjusters[7];
		CallT380048XXXFunction(temp, algorithm, adjustersPrevious[7]); // JALR RA,T3 to 80048AFC
		adjusters[7] = (signed short)temp;

		T2 = (T2 - ((int)((S7 * (int)(adjusters[7])) + 0x4000) >> 0xF));

		T2 = (T2 - ((int)((S6 * (int)(adjusters[6])) + 0x4000) >> 0xF));

		S7 = (((int)((int)(adjusters[6]) * T2 + 0x4000)) >> 0xF) + S6;
	
		T2 = T2 - (((int)((S5 * (int)(adjusters[5])) + 0x4000)) >> 0xF);

		S6 = ((((int)((int)(adjusters[5]) * T2) + 0x4000) >> 0xF) + S5);

		T2 = T2 - ((int)((S4 * (int)(adjusters[4])) + 0x4000) >> 0xF);
	
		S5 = ((int)(((int)(adjusters[4]) * T2) + 0x4000) >> 0xF) + S4;

		T2 = T2 - ((int)((S3 * (int)(adjusters[3])) + 0x4000) >> 0xF);

		S4 = (((int)((int)(adjusters[3]) * T2) + 0x4000) >> 0xF) + S3;

		T2 = (T2 - ((int)((S2 * ((int)(adjusters[2]))) + 0x4000) >> 0xF));

		S3 = ((int)(((int)(adjusters[2]) * T2) + 0x4000) >> 0xF) + S2;

		T2 = T2 - ((int)((S1 * ((int)(adjusters[1]))) + 0x4000) >> 0xF);
		
		S2 = ((int)(((int)(adjusters[1]) * T2) + 0x4000) >> 0xF) + S1;

		T2 = (T2 - ((int)((S0 * (int)(adjusters[0])) + 0x4000) >> 0xF));

		S1 = ((int)(((int)(adjusters[0]) * T2) + 0x4000) >> 0xF) + S0;

		S0 = T2;

		T2 = T2 + ((int)((0x6E14 * A3) + 0x4000) >> 0xF);

		A3 = T2;
		T2 = T2 << 1;
		T2 = T2 & 0xFFF8;
	}

	unsigned long temp = (int)(((((signed short)smoothersChosen[0] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
	adjustersPrevious[0] = (signed short)temp;

	temp = (int)(((((signed short)smoothersChosen[1] - 0x20) * (unsigned long)0x400) * (unsigned long)0x3333) + (unsigned long)0x4000) >> 0xF << 1;
	adjustersPrevious[1] = (signed short)temp;

	temp = (int)(((((signed short)smoothersChosen[2] - 0x10) * (unsigned long)0x400) * (unsigned long)0x3333) + -0x332F000) >> 0xF << 0x1;
	adjustersPrevious[2] = (signed short)temp;

	temp = (int)(((((signed short)smoothersChosen[3] - 0x10) * 0x400) * (unsigned long)0x3333) + 0x04003C00) >> 0xF << 1;
	adjustersPrevious[3] = (signed short)temp;

	temp = (int)(((((signed short)smoothersChosen[4] - 8) * (unsigned long)0x400) * (unsigned long)0x4B17) + 0xFFC91B1C) >> 0xF << 1;
	adjustersPrevious[4] = (signed short)temp;

	temp = (int)(((((signed short)smoothersChosen[5] - 8) * (unsigned long)0x400) * (unsigned long)0x4444) + 0x03BBF800) >> 0xF << 1;
	adjustersPrevious[5] = (signed short)temp;

	temp = (int)(((((signed short)smoothersChosen[6] - 4) * (unsigned long)0x400) * (unsigned long)0x7ADE) + 0x147936C) >> 0xF << 1;
	adjustersPrevious[6] = (signed short)temp;

	temp = (int)((((((signed short)smoothersChosen[7] - 4) * (unsigned long)0x400) * (unsigned long)0x740C) + 0x40D6B40)) >> 0xF << 1;
	adjustersPrevious[7] = (signed short)temp;
}

void CMORTDecoder::Encode(unsigned char* data, int dataSize, unsigned char* outputBuffer, int& outputBufferSize)
{
	// TODO move to global
	signed char table8004867CValue1[0x8];
	table8004867CValue1[0x00] = (signed char)0xFC;
	table8004867CValue1[0x01] = (signed char)0xFD;
	table8004867CValue1[0x02] = (signed char)0xFE;
	table8004867CValue1[0x03] = (signed char)0xFE;
	table8004867CValue1[0x04] = (signed char)0xFF;
	table8004867CValue1[0x05] = (signed char)0xFF;
	table8004867CValue1[0x06] = (signed char)0xFF;
	table8004867CValue1[0x07] = (signed char)0xFF;
	unsigned char table8004867CValue2[0x8];
	table8004867CValue2[0x00] = 0x7;
	table8004867CValue2[0x01] = 0x7;
	table8004867CValue2[0x02] = 0x3;
	table8004867CValue2[0x03] = 0x7;
	table8004867CValue2[0x04] = 0x1;
	table8004867CValue2[0x05] = 0x3;
	table8004867CValue2[0x06] = 0x5;
	table8004867CValue2[0x07] = 0x7;

	unsigned short table80048738[0x6];
	table80048738[0x00] = 0x0CCD;
	table80048738[0x01] = 0x2CCD;
	table80048738[0x02] = 0x5333;
	table80048738[0x03] = 0x7FFF;
	table80048738[0x04] = 0x852A;
	table80048738[0x05] = 0x0000;

	outputBufferSize = 0;

	std::vector<unsigned short> intermediateValues;
	for (int x = 0; x < dataSize; x+=2)
	{
		intermediateValues.push_back(CharArrayToShort(&data[x]));
	}

	while ((intermediateValues.size() % 0xA0) != 0)
		intermediateValues.push_back(0x0000);

	std::vector<signed short> predictorValues;
	unsigned long prevValueNotDoubled = 0;
	

	std::vector<int> allZeroSpots;
	for (size_t x = 0; x < intermediateValues.size(); x += 0xA0)
	{
		bool isAllZeros = true;
		for (int y = 0; y < 0xA0; y++)
		{
			if (intermediateValues[x + y] != 0x0000)
			{
				isAllZeros = false;
				break;
			}
		}

		if (!isAllZeros)
		{
			for (int y = 0; y < 0xA0; y++)
			{
				unsigned long startValue = prevValueNotDoubled;
				unsigned long currentValue = (signed short)intermediateValues[x + y] >> 1;
				prevValueNotDoubled = currentValue;
				unsigned long deltaValue = currentValue - startValue;
				predictorValues.push_back((unsigned short)deltaValue);
			}
		}
		else
		{
			allZeroSpots.push_back(x);
			for (int y = 0; y < 0xA0; y++)
			{
				predictorValues.push_back((unsigned short)0x0000);
			}
		}
	}

	WriteLongToBuffer(outputBuffer, 0, 0x4D4F5254); // MORT
	outputBufferSize = 0xC;

	int outputBufferBitOffset = 0;

	bool isSkipResetPredictor = false;
	int numberSkipResetPredictors = 0;
	int numberResetPredictors = 0;

	std::vector<unsigned short> previousPredictorValues;
	for (int x = 0; x < 0x78; x++)
		previousPredictorValues.push_back(0x0000);

	signed short adjustersPrevious[8];
	for (int x = 0; x < 8; x++)
		adjustersPrevious[x] = 0;
	unsigned short buffer800D2940PredictorTemp[0xA0];
	for (int x = 0; x < 0xA0; x++)
		buffer800D2940PredictorTemp[x] = 0x0;
	unsigned long S0 = 0;
	unsigned long S1 = 0;
	unsigned long S2 = 0;
	unsigned long S3 = 0;
	unsigned long S4 = 0;
	unsigned long S5 = 0;
	unsigned long S6 = 0;
	unsigned long S7 = 0;
	signed long A3 = 0;

	for (size_t x = 0; x < predictorValues.size(); x += 0xA0)
	{
		bool isAllZero = false;
		if (numberResetPredictors == 0)
		{
			if (std::find(allZeroSpots.begin(), allZeroSpots.end(), x) != allZeroSpots.end())
			{
				isAllZero = true;
				numberSkipResetPredictors++;
			}
		}

		bool writeNumberResetPredictors = false;
		if (!isAllZero && (numberResetPredictors == 0))
		{
			numberResetPredictors++;
			writeNumberResetPredictors = true;
			for (size_t xx = x + 0xA0; xx < predictorValues.size(); xx += 0xA0)
			{
				if (
					(std::find(allZeroSpots.begin(), allZeroSpots.end(), xx) != allZeroSpots.end())
					|| (numberResetPredictors == 0x80)
				)
				{
					break;
				}
				numberResetPredictors++;
			}
		}

		if ((!isAllZero && (numberSkipResetPredictors != 0)) || (isAllZero && (x == (predictorValues.size() - 0xA0))))
		{
			// Write out
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 1, (int)true);
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 4, (numberSkipResetPredictors - 1));

			numberSkipResetPredictors = 0;
		}

		if (isAllZero)
		{
			continue;
		}
		else if (!isAllZero && writeNumberResetPredictors)
		{
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 1, false);
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 7, (numberResetPredictors - 1));
		}

		numberResetPredictors--;

		unsigned short shortsSP60[4][0xD];

		unsigned short shortsSPC8[0x4];
		unsigned short shortsSPD0[0x4];
		unsigned short stackBuffer2Offsets[0x4];
		unsigned short shortsSPE0[0x4];

		for (int y = 0; y < 4; y++)
		{
			unsigned short stackBuffer2[0x28];
			for (int xx = 0; xx < 0x28; xx++)
			{
				stackBuffer2[xx] = 0x00;
			}

			unsigned long bestDelta = 0xFFFFFFFF;
			signed short bestPredictors[0x28];

			//for (size_t testShortsSPE0BackPredictor = 0x28; testShortsSPE0BackPredictor < 0x78; testShortsSPE0BackPredictor++)
			int testShortsSPE0BackPredictor = 0x28;
			{
				for (int testSPD0Table = 0; testSPD0Table < 6; testSPD0Table++)
				{
					for (int testStartStackBuffer2 = 0; testStartStackBuffer2 < 4; testStartStackBuffer2++)
					{
						for (int testSPC8Table = 0; testSPC8Table < 64; testSPC8Table++)
						{
							signed short bestSP60ValuesTemp[0xD];
							signed short bestPredictorsTemp[0x28];
							unsigned short stackBuffer2test[0x28];
							for (int xx = 0; xx < 0x28; xx++)
							{
								stackBuffer2test[xx] = stackBuffer2[xx];
							}

							unsigned long T0;
							unsigned long T1;
							if (testSPC8Table < 8)
							{
								T0 = (signed long)table8004867CValue1[testSPC8Table];
								T1 = table8004867CValue2[testSPC8Table];
							}
							else
							{
								T0 = (testSPC8Table - 8) >> 3;
								T1 = testSPC8Table & 7;
							}

							T0 = 6 - T0;

							unsigned long T4 = (T1 * 0x800) + 0x47FF;

							for (int yy = 0; yy < 0xD; yy++)
							{
								unsigned long valueCompare = (signed short)predictorValues[testStartStackBuffer2 + x + (y * 0x28) + (yy * 3)];

								int bestIndex60 = 0;
								int absDelta = 0x7FFFFFFF;
								unsigned long bestT5 = 0;

								for (int d = 0; d < 8; d++)
								{
									unsigned long T5 = ((int)(((((signed short)d * 0x2000) - 0x7000) * T4) + 0x4000) >> 0xF);
									T5 = (int)(T5 + (1 << (T0 - 1))) >> T0; // Sort of round up when shifting?

									if (abs((signed long)T5 - (signed long)valueCompare) < absDelta)
									{
										bestIndex60 = d;
										absDelta = abs((int)T5 - (int)valueCompare);
										bestT5 = T5;
									}
								}

								//8004862C
								bestSP60ValuesTemp[yy] = bestIndex60;
								stackBuffer2test[testStartStackBuffer2 + (yy * 3)] = (unsigned short)bestT5; // AT to 0(T9);
							}

							unsigned long totalDeltaOffset = 0;
							for (int yy = 0; yy < 0x28; yy++)
							{
								unsigned long T1 = (signed short)previousPredictorValues[0x78 - testShortsSPE0BackPredictor + yy];
								T1 = ((int)((T1 * (unsigned short)table80048738[testSPD0Table]) + 0x4000) >> 0xF);
								bestPredictorsTemp[yy] = (unsigned short)(T1 + (signed short)stackBuffer2test[yy]);

								signed long delta = (signed short)predictorValues[x + (y * 0x28) + yy] - (signed short)bestPredictorsTemp[yy];
								unsigned long deltaAbs = abs(delta);
								totalDeltaOffset += deltaAbs;
							}

							if (totalDeltaOffset < bestDelta)
							{
								bestDelta = totalDeltaOffset;
								shortsSPE0[y] = testShortsSPE0BackPredictor;
								shortsSPD0[y] = testSPD0Table;
								stackBuffer2Offsets[y] = testStartStackBuffer2;
								shortsSPC8[y] = testSPC8Table;

								for (int bb = 0; bb < 0xD; bb++)
									shortsSP60[y][bb] = bestSP60ValuesTemp[bb];

								for (int bb = 0; bb < 0x28; bb++)
									bestPredictors[bb] = bestPredictorsTemp[bb];
							}
						}
					}
				}
			}

			for (int xx = 0; xx < 0x28; xx++)
			{
				previousPredictorValues.erase(previousPredictorValues.begin());
			}

			for (int xx = 0; xx < 0x28; xx++)
			{
				previousPredictorValues.push_back(bestPredictors[xx]);
			}
		}

		signed short shortsSPE8[8];

		CalculateBestPredictors(intermediateValues, x, buffer800D2940PredictorTemp, shortsSP60, shortsSPC8, shortsSPD0, stackBuffer2Offsets, shortsSPE0, S0, S1, S2, S3, S4, S5, S6, S7, A3, shortsSPE8, adjustersPrevious);

		// Mostly zeroed out
		/*shortsSPE8[0x00] = 0x0020;
		shortsSPE8[0x01] = 0x0020;
		shortsSPE8[0x02] = 0x0014;
		shortsSPE8[0x03] = 0x000B;
		shortsSPE8[0x04] = 0x0008;
		shortsSPE8[0x05] = 0x0008;
		shortsSPE8[0x06] = 0x0004;
		shortsSPE8[0x07] = 0x0004;*/

		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 6, (unsigned char)shortsSPE8[0x00]);
		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 6, (unsigned char)shortsSPE8[0x01]);
		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 5, (unsigned char)shortsSPE8[0x02]);
		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 5, (unsigned char)shortsSPE8[0x03]);
		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 4, (unsigned char)shortsSPE8[0x04]);
		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 4, (unsigned char)shortsSPE8[0x05]);
		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 3, (unsigned char)shortsSPE8[0x06]);
		WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 3, (unsigned char)shortsSPE8[0x07]);

		for (int xx = 0; xx < 4; xx++)
		{
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 7, (unsigned char)shortsSPE0[xx]);
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 2, (unsigned char)shortsSPD0[xx]);
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 2, (unsigned char)stackBuffer2Offsets[xx]);
			WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 6, (unsigned char)shortsSPC8[xx]);

			for (int yy = 0; yy < 0xD; yy++)
			{
				WriteBitsTo80045FF0Buffer(outputBuffer, outputBufferSize, outputBufferBitOffset, 3, (unsigned char)shortsSP60[xx][yy]);
			}
		}
	}

	if (outputBufferBitOffset != 0)
		outputBufferSize += 4;

	unsigned short number140s = dataSize / 0x140;
	
	WriteLongToBuffer(outputBuffer, 4, (number140s << 16) | 0x00003E80); // Header 1 TODO
	WriteLongToBuffer(outputBuffer, 8, outputBufferSize / 4); // Header 2 TODO
}

void CMORTDecoder::Encode(unsigned char* data, int dataSize, CString outputFile)
{
	int outputBufferSize = 0;
	unsigned char* outputBuffer = new unsigned char[0x100000];
	for (int x = 0; x < 0x100000; x++)
		outputBuffer[x] = 0x00;

	Encode(data, dataSize, outputBuffer, outputBufferSize);
	
	FILE* outFile = fopen(outputFile, "wb");
	if (!outFile)
	{
		delete [] outputBuffer;
		MessageBox(NULL, "Error opening file", "error", NULL);
		return;
	}

	fwrite(outputBuffer, 1, outputBufferSize, outFile);

	fflush(outFile);
	fclose(outFile);

	delete [] outputBuffer;
}