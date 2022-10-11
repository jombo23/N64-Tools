#include "StdAfx.h"
#include "MORTDecoder.h"

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
	// Setup before figuring out for now
	buffer800D2940Subtraction = 0x800D2940;
	buffer800D2940Predictor = new unsigned char[0x190];
	for (int x = 0; x < 0x190; x++)
		buffer800D2940Predictor[x] = 0x00;

	buffer800D2AD8Subtraction = 0x800D2AD8;
	buffer800D2AD8IntermediateValue = new unsigned char[0x1400];
	for (int x = 0; x < 0x1400; x++)
		buffer800D2AD8IntermediateValue[x] = 0x00;

	buffer800D3ED8Subtraction = 0x800D3ED8;
	buffer800D3ED8MORTRawInputDataBuffer = new unsigned char[0x1000];
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
	variable800D4EFC = 0x00000000;

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
	variable800FCF0CVolume = 0x0000007E;

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
	variable800FCEE4IntermediateValuePointer800D2AD8 = 0x800D2AD8;

	variable800D2AD4ROMAddressMORTData = address;
	variable800D2AD0PullDataPointer = 0x00000000;
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

				if (outDebug != NULL)
					fprintf(outDebug, "80044F0C: A0 %08X V1 %08X\n", A0, V1);

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
				A0 = variable800FCEDCPredictorPointer800D2940;
				//80044F9C
				Function80045780(ROM, A0, started);

				//80044FA4
				T8 = 0x800FCEDC;//44(SP);
				AT = 2;
		
				A0 = variable800FCEDCPredictorPointer800D2940;
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
					A0 = variable800FCF2COutputBufferPointer800D4F20; //0(S4);
					S2 = T9 + T3;

					//80044FD0
					Function80059120(A0, A1);

					//80044FD8
					T4 = 0x800FCEDC; //44(SP)
					A0 = variable800FCEDCPredictorPointer800D2940; //0(T4)

					//80044FDC
					Function80045A48(A0);

					T5 = 1;

					AT = 0x80100000;
					T6 = 0x800FCEDC; //44(SP)

					variable800FCED0 = T5;

					if (T9 == 0)
						variable800FCF20 = 0;
					else
						variable800FCF21 = 0;

					A0 = variable800FCEDCPredictorPointer800D2940;

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
							Function80045A68(A0);
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

						A0 = variable800FCEE4IntermediateValuePointer800D2AD8;
						A1 = 0x1400;

						Function80057FD0(A0, A1);

						A0 = variable800FCF2COutputBufferPointer800D4F20;
						A1 = 0x1140;

						Function80057FD0(A0, A1);

						// 8004507C
						T8 = 0x800FCEDC; //44(SP);
					}
				}

label80045084:
				// 80045084
				A1 = S7;
				Function800459E0(A0, A1, V0);

				//8004508C
				T9 = 0x800FCEDC; //44(SP)
				T3 = variable800FCEDCPredictorPointer800D2940;
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
						T4 = variable800FCEE4IntermediateValuePointer800D2AD8;
						//80045198 MFHI Remainder
						T7 = V1 % S6;
						T9 = T7 << 1;
						T3 = T4 + T9;

						T8 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3 - buffer800D2AD8Subtraction]);
						T6 = variable800FCF04; //0(S2)
						T7 = variable800FCF3CCounterIncrementVariable2; // 0(S3)

						//800451AC
						A0 = S1 << 1;

						unsigned __int64 tempRegister = T6 * T8;

						//800451B4
						T4 = T7 + A0;
						T9 = T4 + 1;

						T5 = variable800FCF2COutputBufferPointer800D4F20;//0(S4)

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
						T8 = T5 + T6;

						//800451F8
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T8 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "800451F8: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T8);
						if (started)
							pcmSamples.push_back((unsigned short)V0);

						T4 = variable800FCF3CCounterIncrementVariable2; // 0(S3)
						T7 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)

						//80045204
						S1 = S1 + 4;
						T9 = T4 + A0;
						T3 = T9 % S5;
						T5 = T3 << 1;

						T6 = T7 + T5;
						//80045220
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T6 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "80045220: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T6);
						if (started)
							pcmSamples.push_back((unsigned short)V0);
						T8 = variable800FCEE4IntermediateValuePointer800D2AD8; //0(S0)
						T4 = T0 % S6;

						//8004522C
						T9 = T4 << 1;

						T3 = T8 + T9;
						
						//80045234
						T7 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3 - buffer800D2AD8Subtraction]);
						T5 = variable800FCF04; //0(S2)
						T4 = variable800FCF3CCounterIncrementVariable2; // 0(S3)
						T6 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)

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

						T7 = T6 + T5;
						//8004528C
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T7 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "8004528C: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T7);
						if (started)
							pcmSamples.push_back((unsigned short)V0);
						T8 = variable800FCF3CCounterIncrementVariable2; // 0(S3)
						T4 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						//800452A4
						T9 = T8 + A1;
						T3 = T9 % S5;
						T6 = T3 << 1;
						T5 = T4 + T6;
						
						//800452BC
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T5 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "800452BC: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T5);
						if (started)
							pcmSamples.push_back((unsigned short)V0);
						
						T7 = variable800FCEE4IntermediateValuePointer800D2AD8; //0(S0)
						T8 = T1 % S6;

						//800452C8
						T9 = T8 << 1;
						T3 = T7 + T9;

						T4 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3 - buffer800D2AD8Subtraction]);
						T6 = variable800FCF04; //0(S2)
						T8 = variable800FCF3CCounterIncrementVariable2; // 0(S3)
						T5 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)

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
						T4 = T5 + T6;

						//80045328
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T4 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "80045328: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T4);
						if (started)
							pcmSamples.push_back((unsigned short)V0);
						T7= variable800FCF3CCounterIncrementVariable2; // 0(S3)
						T8 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)
			
						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						//80045340
						T9 = T7 + A2;
						T3 = T9 % S5;

						T5 = T3 << 1;
						T6 = T8 + T5;
						
						//80045358
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T6 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "80045358: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T6);
						if (started)
							pcmSamples.push_back((unsigned short)V0);
						T4 = variable800FCEE4IntermediateValuePointer800D2AD8; //0(S0)
						T7 = T2 % S6;

						//80045364
						T9 = T7 << 1;
						T3 = T4 + T9;

						T8 = (signed short)CharArrayToShort(&buffer800D2AD8IntermediateValue[T3 - buffer800D2AD8Subtraction]);
						T5 = variable800FCF04; //0(S2)
						T7 = variable800FCF3CCounterIncrementVariable2; // 0(S3)
						T6 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)

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
						T8 = T6 + T5;

						//800453C4
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T8 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "800453C4: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T8);
						if (started)
							pcmSamples.push_back((unsigned short)V0);

						//800453C8
						T4 = variable800FCF3CCounterIncrementVariable2; // 0(S3)
						T7 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)

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

						T5 = T7 + T6;

						if (S5 == 0)
						{
							// ERROR OUT
							return false;
						}

						//80045408
						WriteShortToBuffer(buffer800D4F20OutputBuffer, (T5 - buffer800D4F20Subtraction), (unsigned short)V0);
						if (outDebug != NULL)
							fprintf(outDebug, "800453C4: OUTPUT VALUE WRITE %04X to %08X\n", (unsigned short)(V0 & 0xFFFF), T5);
						if (started)
							pcmSamples.push_back((unsigned short)V0);

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
					A0 = variable800FCF2COutputBufferPointer800D4F20; // 0(S4)

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
					Function80059120(A0, A1);

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
							A0 = variable800FCEDCPredictorPointer800D2940;//0(T5);

							Function800456D0(A0);

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

	delete [] buffer800D2940Predictor;
	delete [] buffer800D2AD8IntermediateValue;
	delete [] buffer800D3ED8MORTRawInputDataBuffer;
	delete [] buffer800D4F20OutputBuffer;

	return true;
}

// TWINE 8009D5D4
void CMORTDecoder::Function80045780(unsigned char* ROM, unsigned long A0Param, bool& started)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
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

	T6 = variable800D4F10Status1; //25D0(A0)
	S0 = A0; // 0x800D2940
	S2 = variable800D4F12Status3;

	//800457A4
	A0 = A0 + 0x25A0; // TWINE 11AC
	if (T6 != 0)
	{
		//800457AC
		A1 = 0;
		A2 = 0;

		//800457B0
		Function80056AD0(A0, A1, A2, V0);

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
					fprintf(outDebug, "800457D4: INPUT VALUE %08X\n", T9);
				T0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[8]);
				if (outDebug != NULL)
					fprintf(outDebug, "800457D8: INPUT VALUE %08X\n", T0);

				variable800D4EFC = T2;

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
					T9 = variable800D2AD0PullDataPointer; //0190(S0);
					T8 = variable800D2AD4ROMAddressMORTData; //0194(S0);
		
					T1 = T0 << 3;
					A1 = S0 + T1;

					variable800D4F10Status1 = (unsigned char)S3;

					A1 = A1 + 0x1598;
					A3 = S0 + 0x25A0;

					// Index into ROM buffer
					A0 = T8 + V1;
					
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
					T8 = S1 % AT;
					T0 = T8 >> 2;
					T1 = T0 << 3;

					A1 = S0 + T1;
					A1 = A1 + 0x198;
					A0 = S0;

					//80045928 // TWINE 8009D7C0
					// Call 80045FF0 // TWINE calls 8009DF60
					Function80045FF0(A0, A1);

					//80045930  // TWINE 8009D7C8
					T9 = variable800D4EFC;
					T4 = variable800D4F0C;
					T8 = variable800D4F08;
					T6 = variable800D4ED8Amountofsoundleft1_FirstWord; // TWINE it's T8

					if (outDebug != NULL)
						fprintf(outDebug, "80045930: T9: %08X T4: %08X T8: %08X T6: %08X\n", T9, T4, T8, T6);
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
	fwrite(buffer800D2940Predictor, 1, 0x2000, pred);
	fflush(pred);
	fclose(pred);*/

	T4 = variable800D4F11Status2;
	if (T4 != S3)
	{
		// Set RA
	}
	else
	{
		T5 = 2;
		if (V1 < 0x8C0)
		{
			
		}
		else
		{
			// Start playing
			variable800D4F11Status2 = (unsigned char)T5; // 2
			started = true;
		}
	}
}

void CMORTDecoder::Function800459E0(unsigned long A0Param, unsigned long A1Param, unsigned long& V0)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
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

void CMORTDecoder::Function800456D0(unsigned long A0Param)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
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

	A1 = 0x190;

	Function80057FD0(A0, A1);

	//800456EC
	A3 = variable800FCEDCPredictorPointer800D2940; //18(SP) Maybe from Function
	T6 = 0x28;

	A2 = 0x1;
	// Why, isn't this part of buffer??
	WriteShortToBuffer(buffer800D2940Predictor, (A3 + 0x166 - buffer800D2940Subtraction), (unsigned short)T6);

	T7 = 0x800455DC; //20(SP); Maybe from Function

	A0 = A3 + 0x25A0;
	A1 = A3 + 0x25B8;

	//80045708
	// Why, isn't this part of buffer??
	//WriteLongToBuffer(buffer800D2940Predictor, (A3 + 0x190 - buffer800D2940Subtraction), T7);
	variable800D2AD0PullDataPointer = T7;

	T8 = variable800D2AD4ROMAddressMORTData; //1C(SP); Maybe from Function

	// T8 to 194(A3)
	variable800D2AD4ROMAddressMORTData = variable800FCEFCROMMORTSoundAddress;

	Function8005E3A0(A1, A2);

	T9 = variable800D2AD4ROMAddressMORTData; //1C(SP) Maybe from Function
	A3 = variable800FCEDCPredictorPointer800D2940; //18(SP)

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

		variable800D4EFC = 0;
		variable800D4F04InputChunkAmountLeft = 0;
		variable800D4F00InputChunkUsed = 0;
		variable800D4F0C = 0;
		variable800D4F08 = 0;

		variable800D4F12Status3 = (unsigned char)T2; //2
	}
}

void CMORTDecoder::Function80057FD0(unsigned long A0Param, unsigned long A1Param)
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
		if ((A0 >= 0x800D2940) && (A0 < 0x800D2AD0))
		{
			// Predictors
			buffer800D2940Predictor[A0 + x - buffer800D2940Subtraction] = 0;
		}
		else if ((A0 >= 0x800D2AD8) && (A0 < 0x800D3ED8))
		{
			// Intermediate Value
			buffer800D2AD8IntermediateValue[A0 + x - buffer800D2AD8Subtraction] = 0;
		}
		else if (A0 == 0x800D4F20)
		{
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
	memcpy(&buffer800D3ED8MORTRawInputDataBuffer[A1Param - buffer800D3ED8Subtraction], &ROM[A0Param], A2Param);
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

void CMORTDecoder::Function80056AD0(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long& V0)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
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

//TWINE 8009DF60
void CMORTDecoder::Function80045FF0(unsigned long A0Param, unsigned long A1Param)
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

	//800AA3E0
	unsigned char stackBuffer[0x100];
	for (int x = 0; x < 0x100; x++)
	{
		stackBuffer[x] = 0x00;
	}

	//S0 to 28(SP) 800D2940
	WriteLongToBuffer(stackBuffer, 0xFC, A1); //A1 to FC(SP)

	T0 = variable800D4EFC;

	S0 = A0; //800D2940 Predictor Buffer pointer
	T6 = (int)T0 >> 5;

	T7 = T6 & 0x3FF;

	T8 = T7 << 2;
	T9 = A0 + T8;

	//80046018
	T8 = (signed short)CharArrayToShort(&buffer800D2940Predictor[A0 + 0x18A - buffer800D2940Subtraction]);

	//8004601C
	T6 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T9 + 0x1598 - buffer800D3ED8Subtraction]);
	if (outDebug != NULL)
		fprintf(outDebug, "8004601C: INPUT VALUE %08X\n", T6);

	T7 = 0x20;
	A2 = T0 & 0x1F;

	A3 = T7 - A2;
	V1 = A3;

	/*FILE* aa = fopen("C:\\temp\\aa.bin", "wb");
	fwrite(buffer800D2940Predictor, 1, 0x2000, aa);
	fflush(aa);
	fclose(aa);*/
	//80046030
	V0 = T6 >> A2;
	if (T8 == 0)
	{
		T9 = (signed short)CharArrayToShort(&buffer800D2940Predictor[A0 + 0x18C - buffer800D2940Subtraction]);

		if (T9 != 0)
		{
			T8 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18C - buffer800D2940Subtraction]);
		}
		else
		{
			A0 = 1;
			//80046048
			if ((int)A3 < 2)
			{
				//80046060
				T6 = 1;
				T7 = T6 << V1;
				T0 = T0 + 0x20;
				T9 = (int)T0 >> 5;
				T8 = T7 - 1;
				T6 = T9 & 0x3FF;
				T7 = T6 << 2;
				T3 = T8 & V0;
				//80046080
				T8 = S0 + T7;

				V0 = CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046084: INPUT VALUE %08X\n", V0);
				//80046084
				if (V1 != A0)
				{
					//8004608C
					T9 = A0 - V1;
					T6 = 1;
					T7 = T6 << T9;
					T8 = T7 - 1;
					T6 = V0 & T8;
					T9 = T6 << V1;
					T3 = T3 | T9;
				}

				//800460A8
				T7 = A0 - V1;
				V0 = V0 >> T7;
				V1 = V1 + 0x1F;
			}
			else
			{
				//80046050
				T3 = V0 & 0x1;
				V0 = V0 >> 1;
				V1 = A3 - 1;
			}

			//800460B4
			AT = (int)V1 < 8;
			if (T3 != 0)
			{
				//800460BC
				AT = (int)V1 < 5;

				A1 = 4;

				if (AT == 0)
				{
					T8 = V0 & 0xF;
					WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18C - buffer800D2940Subtraction, (unsigned short)T8);

					V0 = V0 >> 4;
				
					V1 = V1 - 4;
				}
				else
				{
					//800460DC
					T6 = 1;
					T9 = T6 << V1;
					T7 = T9 - 1;
					T0 = T0 + 0x20;
					T6 = (int)T0 >> 5;
					T8 = T7 & V0;
					T9 = T6 & 0x3FF;
					T7 = T9 << 2;
					//800460FC
					WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18C - buffer800D2940Subtraction, (unsigned short)T8);
					T8 = S0 + T7;

					//80046104
					V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
					if (outDebug != NULL)
						fprintf(outDebug, "80046104: INPUT VALUE %08X\n", V0);
					if (V1 != A1)
					{
						T9 = A1 - V1;
						T7 = 1;
						T8 = T7 << T9;
						T6 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18C - buffer800D2940Subtraction]);
						T7 = T8 - 1;
						T9 = V0 & T7;
						T8 = T9 << V1;
						T7 = T6 | T8;
						WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18C - buffer800D2940Subtraction, (unsigned short)T7);
					}

					//80046130
					T9 = A1 - V1;
					V0 = V0 >> T9;
					V1 = V1 + 0x1C;
				}

				//8004613C
				T6 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18C - buffer800D2940Subtraction]);
				T8 = T6 + 1;
				WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18C - buffer800D2940Subtraction, (unsigned short)T8);
			}
			else
			{
				//8004614C
				AT = (int)V1 < 8;

				T5 = 7;
				if (AT == 0)
				{
					T7 = V0 & 0x7F;
					WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18A - buffer800D2940Subtraction, (unsigned short)T7);
					V0 = V0 >> 7;
					V1 = V1 - 7;
				}
				else
				{
					//80046168
					T9 = 1;
					T6 = T9 << V1;
					T8 = T6 - 1;
					T0 = T0 + 0x20; 
					T9 = T0 >> 5;
					T7 = T8 & V0;
					T6 = T9 & 0x3FF;
					T8 = T6 << 2;
					WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18A - buffer800D2940Subtraction, (unsigned short)T7);
					T7 = S0 + T8;
					//80046190
					V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
					if (outDebug != NULL)
						fprintf(outDebug, "80046190: INPUT VALUE %08X\n", V0);
					if (V1 != T5)
					{
						//80046198
						T6 = T5 - V1;
						T8 = 1;
						T7 = T8 << T6;
						T9 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18A - buffer800D2940Subtraction]);
						if (outDebug != NULL)
							fprintf(outDebug, "800461A4: PREDICTOR VALUE READ %04X from %08X\n", (unsigned short)(T9 & 0xFFFF), (S0 + 0x18A));
						T8 = T7 - 1;
						T6 = V0 & T8;
						T7 = T6 << V1;
						T8 = T9 | T7;
						WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18A - buffer800D2940Subtraction, (unsigned short)T8);
					}

					//800461BC
					T6 = T5 - V1;
					V0 = V0 >> T6;
					V1 = V1 + 0x19;
				}

				//800461C8
				T9 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18A - buffer800D2940Subtraction]);
				T7 = T9 + 1;
				WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18A - buffer800D2940Subtraction, (unsigned short)T7);
				T8 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18C - buffer800D2940Subtraction]);
			}
		}
	}
	else
	{
		T8 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18C - buffer800D2940Subtraction]);
	}

	//800461D8
	A1 = 4;
	T5 = 7;

	AT = (int)V1 < 7;
	if (T8 != 0)
	{
		A0 = CharArrayToLong(&stackBuffer[0xFC]); //FC(SP);
		A1 = 0x140;
		
		//V1 to 58(SP);
		//T0 to 50(SP);

		//800461F4
		Function80057FD0(A0, A1);

		//800461FC
		T6 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18C - buffer800D2940Subtraction]);
		//V1 = 58(SP);
		//T0 = 50(SP);

		T9 = T6 - 1;

		//8004620C
		WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18C - buffer800D2940Subtraction, (unsigned short)T9);
	}
	else
	{
		unsigned long RA = 0;
		unsigned long SP = 0;

		//80046214
		A2 = SP + 0xE0;
		if (AT == 0)
		{
			T7 = V0 & 0x3F;
			WriteShortToBuffer(stackBuffer, SP + 0xE8, (unsigned short)T7); //T7 to E8(SP)
			V0 = V0 >> 6;
			V1 = V1 - 6;

			T4 = 6;
		}
		else
		{
			//80046234
			T8 = 1;
			T6 = T8 << V1;
			T9 = T6 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T6 = T8 & 0x3FF;
			T7 = T9 & V0;
			T9 = T6 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xE8, (unsigned short)T7); //T7 to E8(SP);
			T4 = 6;

			//8004625C
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "8004625C: INPUT VALUE %08X\n", V0);

			if (V1 != T4)
			{
				//80046268
				T6 = T4 - V1;
				T9 = 1;
				T8 = T9 << T6;
				T9 = T8 - 1;
				T6 = V0 & T9;
				T8 = T6 << V1;
				T6 = T7;
				T7 = T6 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xE8, (unsigned short)T7); //T7 to E8(SP)
			}

			//8004628C
			T9 = T4 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1A;
		}

		//80046298
		A3 = SP + 0xD0;
		AT = ((int)V1 < 7);

		//8004629C
		if (AT == 0)
		{
			T6 = V0 & 0x3F;
			WriteShortToBuffer(stackBuffer, SP + 0xEA, (unsigned short)T6); //T6 to EA(SP)
			V0 = V0 >> 6;
			V1 = V1 - 6;
		}
		else
		{
			//800462B8
			T8 = 1;
			T7 = T8 << V1;
			T9 = T7 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T7 = T8 & 0x3FF;
			T6 = T9 & V0;
			T9 = T7 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xEA, (unsigned short)T6); //T6 to EA(SP)
			//800462E0
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "800462E0: INPUT VALUE %08X\n", V0);
			if (V1 != T4)
			{
				T7 = T4 - V1;
				T9 = 1;
				T8 = T9 << T7;
				T9 = T8 - 1;
				T7 = V0 & T9;
				T8 = T7 << V1;
				T7 = T6;
				T6 = T7 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xEA, (unsigned short)T6); //T6 to EA(SP)
			}

			//8004630C
			T9 = T4 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1A;
		}

		//80046318
		AT = (int)V1 < 6;

		//8004631C
		T1 = SP + 0xD8;
		if (AT == 0)
		{
			//80046324
			T7 = V0 & 0x1F;
			WriteShortToBuffer(stackBuffer, SP + 0xEC, (unsigned short)T7); //T7 to EC(SP)
			V0 = V0 >> 5;
			V1 = V1 - 5;
			A0 = 5;
		}
		else
		{
			//8004633C
			T8 = 1;
			T6 = T8 << V1;
			T9 = T6 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T6 = T8 & 0x3FF;
			T7 = T9 & V0;
			T9 = T6 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xEC, (unsigned short)T7); //T7 to EC(SP)
			A0 = 5;

			//80046368
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "80046368: INPUT VALUE %08X\n", V0);
			if (V1 != A0)
			{
				T6 = A0 - V1;
				T9 = 1;
				T8 = T9 << T6;
				T9 = T8 - 1;
				T6 = V0 & T9;
				T8 = T6 << V1;
				T6 = T7;
				T7 = T6 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xEC, (unsigned short)T7); //T7 to EC(SP)
			}

			//80046394
			T9 = A0 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1B;
		}

		//800463A0
		AT = (int)V1 < 6;
		T2 = SP + 0xC8;

		//800463A4
		if (AT == 0)
		{
			T6 = V0 & 0x1F;
			WriteShortToBuffer(stackBuffer, SP + 0xEE, (unsigned short)T6); //T6 to EE(SP)
			V0 = V0 >> 5;
			V1 = V1 - 5;

		}
		else
		{
			//800463C0
			T8 = 1;
			T7 = T8 << V1;
			T9 = T7 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T7 = T8 & 0x3FF;
			T6 = T9 & V0;
			T9 = T7 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xEE, (unsigned short)T6); //T6 to EE(SP)

			//800463E8
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "800463E8: INPUT VALUE %08X\n", V0);

			if (V1 != A0)
			{
				T7 = A0 - V1;
				T9 = 1;
				T8 = T9 << T7;
				T9 = T8 - 1;
				T7 = V0 & T9;
				T8 = T7 << V1;
				T7 = T6;
				T6 = T7 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xEE, (unsigned short)T6); //T6 to EE(SP)
			}

			//80046414
			T9 = A0 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1B;
		}

		//80046420
		AT = (int)V1 < 5;

		//80046424
		A0 = SP + 0x60;
		if (AT == 0)
		{
			T7 = V0 & 0xF;
			WriteShortToBuffer(stackBuffer, SP + 0xF0, (unsigned short)T7); //T7 to F0(SP)
			V0 = V0 >> 4;
			V1 = V1 - 4;
		}
		else
		{
			//80046440
			T8 = 1;
			T6 = T8 << V1;
			T9 = T6 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T6 = T8 & 0x3FF;
			T7 = T9 & V0;
			T9 = T6 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xF0, (unsigned short)T7); //T7 to F0(SP)

			//80046464
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "80046464: INPUT VALUE %08X\n", V0);

			if (V1 != A1)
			{
				T6 = A1 - V1;
				T9 = 1;
				T8 = T9 << T6;
				T9 = T8 - 1;
				T6 = V0 & T9;
				T8 = T6 << V1;
				T6 = T7;
				T7 = T6 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xF0, (unsigned short)T7); //T7 to F0(SP)
			}

			//80046494
			T9 = A1 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1C;
		}

		//800464A0
		AT = (int)V1 < 5;

		//800464A4
		RA = SP + 0xC8;
		if (AT == 0)
		{
			T6 = V0 & 0xF;
			WriteShortToBuffer(stackBuffer, SP + 0xF2, (unsigned short)T6); //T6 to F2(SP)
			V0 = V0 >> 4;
			V1 = V1 - 4;
		}
		else
		{
			//800464C0
			T8 = 1;
			T7 = T8 << V1;
			T9 = T7 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T7 = T8 & 0x3FF;
			T6 = T9 & V0;
			T9 = T7 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xF2, (unsigned short)T6); //T6 to F2(SP)

			//800463E8
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "800463E8: INPUT VALUE %08X\n", V0);

			if (V1 != A1)
			{
				T7 = A1 - V1;
				T9 = 1;
				T8 = T9 << T7;
				T9 = T8 - 1;
				T7 = V0 & T9;
				T8 = T7 << V1;
				T7 = T6;
				T6 = T7 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xF2, (unsigned short)T6); //T6 to F2(SP)
			}

			//80046514
			T9 = A1 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1C;
		}

		//80046520
		AT = (int)V1 < 4;

		T3 = 2;
		if (AT == 0)
		{
			T7 = V0 & 0x7;
			WriteShortToBuffer(stackBuffer, SP + 0xF4, (unsigned short)T7); //T7 to F4(SP)
			V0 = V0 >> 3;
			V1 = V1 - 3;

			A1 = 3;
		}
		else
		{
			//80046544
			T8 = 1;
			T6 = T8 << V1;
			T9 = T6 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T6 = T8 & 0x3FF;
			T7 = T9 & V0;
			T9 = T6 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xF4, (unsigned short)T7); //T7 to F4(SP)

			A1 = 3;

			//80046568
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "8004601C: INPUT VALUE %08X\n", V0);

			if (V1 != A1)
			{
				T6 = A1 - V1;
				T9 = 1;
				T8 = T9 << T6;
				T9 = T8 - 1;
				T6 = V0 & T9;
				T8 = T6 << V1;
				T6 = T7;
				T7 = T6 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xF4, (unsigned short)T7); //T7 to F4(SP)
			}

			//8004659C
			T9 = A1 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1D;
		}

		//800465A8
		AT = ((int)V1 < 4);

		//800465AC
		T8 = 1;
		if (AT == 0)
		{
			T6 = V0 & 0x7;
			WriteShortToBuffer(stackBuffer, SP + 0xF6, (unsigned short)T6); //T6 to F6(SP)
			V0 = V0 >> 3;
			V1 = V1 - 3;

		}
		else
		{
			//800465C8
			T7 = T8 << V1;
			T9 = T7 - 1;
			T0 = T0 + 0x20;
			T8 = T0 >> 5;
			T7 = T8 & 0x3FF;
			T6 = T9 & V0;
			T9 = T7 << 2;
			T8 = S0 + T9;
			WriteShortToBuffer(stackBuffer, SP + 0xF6, (unsigned short)T6); //T6 to F6(SP)

			//800465EC
			V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
			if (outDebug != NULL)
				fprintf(outDebug, "800465EC: INPUT VALUE %08X\n", V0);

			if (V1 != A1)
			{
				T7 = A1 - V1;
				T9 = 1;
				T8 = T9 << T7;
				T9 = T8 - 1;
				T7 = V0 & T9;
				T8 = T7 << V1;
				T7 = T6;
				T6 = T7 | T8;
				WriteShortToBuffer(stackBuffer, SP + 0xF6, (unsigned short)T6); //T6 to F6(SP)
			}

			//80046618
			T9 = A1 - V1;
			V0 = V0 >> T9;
			V1 = V1 + 0x1D;
		}

		do
		{
			//80046624
			AT = (int)V1 < 8;

			T8 = 1;
			if (AT == 0)
			{
				T7 = V0 & 0x7F;
				WriteShortToBuffer(stackBuffer, A2 + 0x0, (unsigned short)T7); //T7 to 0(A2)
				V0 = V0 >> 7;
				V1 = V1 - 7;
			}
			else
			{
				//80046644
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				T8 = S0 + T9;
				WriteShortToBuffer(stackBuffer, A2 + 0x0, (unsigned short)T7); //T7 to 0(A2)
				T7 = S0 + T9;


				//80046668
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046668: INPUT VALUE %08X\n", V0);

				if (V1 != T5)
				{
					T6 = T5 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A2 + 0x0]); //0(A2)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A2 + 0x0, (unsigned short)T9); //T9 to 0(A2)
				}

				//80046694
				T6 = T5 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x19;
			}

			//800466A0
			AT = (int)V1 < 3;

			A2 = A2 + 2;

			if (AT == 0)
			{
				T8 = V0 & 0x3;
				WriteShortToBuffer(stackBuffer, A3 + 0x0, (unsigned short)T8); //T8 to 0(A3)
				V0 = V0 >> 2;
				V1 = V1 - 2;
			}
			else
			{
				//800466C0
				T7 = 1;
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, A3 + 0x0, (unsigned short)T8); //T8 to 0(A3)
				T8 = S0 + T6;

				//800466E8
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "800466E8: INPUT VALUE %08X\n", V0);

				if (V1 != T3)
				{
					T9 = T3 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[A3 + 0x0]); //0(A3)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, A3 + 0x0, (unsigned short)T6); //T6 to 0(A3)
				}

				//80046714
				T9 = T3 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1E;
			}
			//80046720
			AT = (int)V1 < 3;

			A3 = A3 + 2;

			if (AT == 0)
			{
				T7 = V0 & 0x3;
				WriteShortToBuffer(stackBuffer, T1 + 0x0, (unsigned short)T7); //T7 to 0(T1)
				V0 = V0 >> 2;
				V1 = V1 - 2;
			}
			else
			{
				//80046740
				T8 = 1;
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				WriteShortToBuffer(stackBuffer, T1 + 0x0, (unsigned short)T7); //T7 to 0(T1)
				T7 = S0 + T9;

				//80046768
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046768: INPUT VALUE %08X\n", V0);

				if (V1 != T3)
				{
					T6 = T3 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[T1 + 0x0]); //0(T1)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, T1 + 0x0, (unsigned short)T9); //T9 to 0(T1)
				}

				//80046794
				T6 = T3 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1E;
			}

			//800467A0
			AT = (int)V1 < 7;

			T1 = T1 + 2;

			if (AT == 0)
			{
				T8 = V0 & 0x3F;
				WriteShortToBuffer(stackBuffer, T2 + 0x0, (unsigned short)T8); //T8 to 0(T2)
				V0 = V0 >> 6;
				V1 = V1 - 6;
			}
			else
			{
				//800467C0
				T7 = 1;
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, T2 + 0x0, (unsigned short)T8); //T8 to 0(T2)
				T8 = S0 + T6;

				//800467E4
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "800467E4: INPUT VALUE %08X\n", V0);

				if (V1 != T4)
				{
					T9 = T4 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[T2 + 0x0]); //0(T2)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, T2 + 0x0, (unsigned short)T6); //T6 to 0(T2)
				}

				//80046814
				T9 = T4 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1A;
			}

			//80046820
			AT = ((int)V1 < 4);

			T8 = 1;

			if (AT == 0)
			{
				T7 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x0, (unsigned short)T7); //T7 to 0(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046840
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x0, (unsigned short)T7); //T7 to 0(A0)
				T7 = S0 + T9;

				//80046864
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046864: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T6 = A1 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x0]); //0(A0)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A0 + 0x0, (unsigned short)T9); //T9 to 0(A0)
				}

				//80046890
				T6 = A1 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1D;
			}
			
			//8004689C
			AT = ((int)V1 < 4);

			T7 = 1;

			if (AT == 0)
			{
				T8 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x2, (unsigned short)T8); //T8 to 2(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//800468BC
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x2, (unsigned short)T8); //T8 to 2(A0)
				T8 = S0 + T6;

				//800468E0
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "800468E0: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T9 = A1 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x2]); //2(A0)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, A0 + 0x2, (unsigned short)T6); //T6 to 2(A0)
				}

				//8004690C
				T9 = A1 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1D;
			}

			//80046918
			AT = ((int)V1 < 4);

			//8004691C
			T8 = 1;

			if (AT == 0)
			{
				T7 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x4, (unsigned short)T7); //T7 to 4(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046938
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x4, (unsigned short)T7); //T7 to 4(A0)
				T7 = S0 + T9;

				//8004695C
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "8004695C: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T6 = A1 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x4]); //4(A0)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A0 + 0x4, (unsigned short)T9); //T9 to 4(A0)
				}

				//
				T6 = A1 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1D;
			}

			//80046994
			AT = ((int)V1 < 4);

			//80046998
			T7 = 1;

			if (AT == 0)
			{
				T8 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x6, (unsigned short)T8); //T8 to 6(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//800469B4
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x6, (unsigned short)T8); //T8 to 6(A0)
				T8 = S0 + T6;

				//800469D8
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "800469D8: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T9 = A1 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x6]); //6(A0)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, A0 + 0x6, (unsigned short)T6); //T6 to 6(A0)
				}

				//80046A04
				T9 = A1 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1D;
			}

			//80046A10
			AT = ((int)V1 < 4);

			//80046A14
			if (AT == 0)
			{
				T7 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x8, (unsigned short)T7); //T7 to 8(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046A30
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x8, (unsigned short)T7); //T7 to 8(A0)
				T7 = S0 + T9;

				//80046A54
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046A54: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T6 = A1 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x8]); //8(A0)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A0 + 0x8, (unsigned short)T9); //T9 to 8(A0)
				}

				//80046A80
				T6 = A1 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1D;
			}

			//80046A8C
			AT = ((int)V1 < 4);

			T7 = 1;

			if (AT == 0)
			{
				T8 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0xA, (unsigned short)T8); //T8 to A(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046AAC
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0xA, (unsigned short)T8); //T8 to A(A0)
				T8 = S0 + T6;

				//80046AD0
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046AD0: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T9 = A1 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0xA]); //A(A0)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, A0 + 0xA, (unsigned short)T6); //T6 to A(A0)
				}

				//80046AFC
				T9 = A1 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1D;
			}

			//80046B08
			AT = ((int)V1 < 4);

			T8 = 1;

			if (AT == 0)
			{
				T7 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0xC, (unsigned short)T7); //T7 to C(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046B28
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0xC, (unsigned short)T7); //T7 to C(A0)
				T7 = S0 + T9;

				//80046B4C
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046B4C: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T6 = A1 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0xC]); //8(A0)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A0 + 0xC, (unsigned short)T9); //T9 to C(A0)
				}

				//80046B78
				T6 = A1 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1D;
			}

			//80046B84
			AT = ((int)V1 < 4);

			T7 = 1;

			if (AT == 0)
			{
				T8 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0xE, (unsigned short)T8); //T8 to E(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046BA4
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0xE, (unsigned short)T8); //T8 to E(A0)
				T8 = S0 + T6;

				//80046BC8
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "8004601C: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T9 = A1 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0xE]); //E(A0)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, A0 + 0xE, (unsigned short)T6); //T6 to E(A0)
				}

				//80046BF4
				T9 = A1 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1D;
			}

			//80046C00
			AT = ((int)V1 < 4);

			T8 = 1;

			if (AT == 0)
			{
				T7 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x10, (unsigned short)T7); //T7 to 10(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046C20
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x10, (unsigned short)T7); //T7 to 10(A0)
				T7 = S0 + T9;

				//80046C44
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046C44: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T6 = A1 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x10]); //10(A0)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A0 + 0x10, (unsigned short)T9); //T9 to 10(A0)
				}

				//80046C70
				T6 = A1 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1D;
			}

			//80046C7C
			AT = ((int)V1 < 4);

			T7 = 1;

			if (AT == 0)
			{
				T8 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x12, (unsigned short)T8); //T8 to 12(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046C9C
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x12, (unsigned short)T8); //T8 to 12(A0)
				T8 = S0 + T6;

				//80046CC0
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046CC0: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T9 = A1 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x12]); //12(A0)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, A0 + 0x12, (unsigned short)T6); //T6 to 12(A0)
				}

				//80046CEC
				T9 = A1 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1D;
			}

			//80046CF8
			AT = ((int)V1 < 4);

			T8 = 1;

			if (AT == 0)
			{
				T7 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x14, (unsigned short)T7); //T7 to 14(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046D18
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2; 
				WriteShortToBuffer(stackBuffer, A0 + 0x14, (unsigned short)T7); //T7 to 14(A0)
				T7 = S0 + T9;

				//80046D3C
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046D3C: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T6 = A1 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x14]); //14(A0)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A0 + 0x14, (unsigned short)T9); //T9 to 14(A0)
				}

				//80046D68
				T6 = A1 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1D;
			}

			//80046D74
			AT = ((int)V1 < 4);

			T7 = 1;

			if (AT == 0)
			{
				T8 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x16, (unsigned short)T8); //T8 to 16(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046D94
				T9 = T7 << V1;
				T6 = T9 - 1;
				T0 = T0 + 0x20;
				T7 = T0 >> 5;
				T8 = T6 & V0;
				T9 = T7 & 0x3FF;
				T6 = T9 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x16, (unsigned short)T8); //T8 to 16(A0)
				T8 = S0 + T6;

				//80046DB8
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T8 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046DB8: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T9 = A1 - V1;
					T6 = 1;
					T8 = T6 << T9;
					T7 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x16]); //16(A0)
					T6 = T8 - 1;
					T9 = V0 & T6;
					T8 = T9 << V1;
					T6 = T7 | T8;
					WriteShortToBuffer(stackBuffer, A0 + 0x16, (unsigned short)T6); //T6 to 16(A0)
				}

				//80046DE4
				T9 = A1 - V1;
				V0 = V0 >> T9;
				V1 = V1 + 0x1D;
			}
			
			//80046DF0
			AT = ((int)V1 < 4);

			T8 = 1;

			if (AT == 0)
			{
				T7 = V0 & 0x7;
				WriteShortToBuffer(stackBuffer, A0 + 0x18, (unsigned short)T7); //T7 to 18(A0)
				V0 = V0 >> 3;
				V1 = V1 - 3;
			}
			else
			{
				//80046E10
				T6 = T8 << V1;
				T9 = T6 - 1;
				T0 = T0 + 0x20;
				T8 = T0 >> 5;
				T7 = T9 & V0;
				T6 = T8 & 0x3FF;
				T9 = T6 << 2;
				WriteShortToBuffer(stackBuffer, A0 + 0x18, (unsigned short)T7); //T7 to 18(A0)
				T7 = S0 + T9;

				//80046E34
				V0 = (signed long)CharArrayToLong(&buffer800D3ED8MORTRawInputDataBuffer[T7 + 0x1598 - buffer800D3ED8Subtraction]);
				if (outDebug != NULL)
					fprintf(outDebug, "80046E34: INPUT VALUE %08X\n", V0);

				if (V1 != A1)
				{
					T6 = A1 - V1;
					T9 = 1;
					T7 = T9 << T6;
					T8 = (signed short)CharArrayToShort(&stackBuffer[A0 + 0x18]); //18(A0)
					T9 = T7 - 1;
					T6 = V0 & T9;
					T7 = T6 << V1;
					T9 = T8 | T7;
					WriteShortToBuffer(stackBuffer, A0 + 0x18, (unsigned short)T9); //T9 to 18(A0)
				}

				//80046E60
				T6 = A1 - V1;
				V0 = V0 >> T6;
				V1 = V1 + 0x1D;
			}

			//80046E6C
			A0 = A0 + 0x1A;

			//80046E70
			T2 = T2 + 2;
		}
		while(A0 != RA);

		//80046E78
		T8 = (signed short)CharArrayToShort(&buffer800D2940Predictor[S0 + 0x18A - buffer800D2940Subtraction]);
		T9 = SP + 0xD8;
		T6 = SP + 0xC8;

		T7 = T8 - 1;
		WriteShortToBuffer(buffer800D2940Predictor, S0 + 0x18A - buffer800D2940Subtraction, (unsigned short)T7);

		T7 = CharArrayToLong(&stackBuffer[0xFC]); //FC(SP);
		T8 = SP + 0x60;

		WriteLongToBuffer(stackBuffer, SP + 0x18, T8); //T8 to 0x18(SP)
		WriteLongToBuffer(stackBuffer, SP + 0x50, T0); //T0 to 0x50(SP)
		WriteLongToBuffer(stackBuffer, SP + 0x58, V1); //V1 to 0x58(SP)
		WriteLongToBuffer(stackBuffer, SP + 0x14, T6); //T6 to 0x14(SP)
		WriteLongToBuffer(stackBuffer, SP + 0x10, T9); //T9 to 0x10(SP)

		//80046EA8
		A0 = S0; //800D2940

		A1 = SP + 0xE8;
		A2 = SP + 0xE0;
		A3 = SP + 0xD0;
		WriteLongToBuffer(stackBuffer, SP + 0x1C, T7); //T7 to 1C(SP)

		/*FILE* out = fopen("C:\\temp\\a.bin", "wb");
		fwrite(stackBuffer, 1, 0x100, out);
		fflush(out);
		fclose(out);*/

		//80046EB8
		Function80045C78(A0, A1, A2, A3, T8, stackBuffer);

		//80046EC0
		V1 = (signed long)CharArrayToLong(&stackBuffer[SP + 0x58]); // 58(SP)
		T0 = (signed long)CharArrayToLong(&stackBuffer[SP + 0x50]); //T0 = 50(SP);
	}

//80046EC8
	AT = -0x20;
	T9 = T0 & AT;
	T6 = T9 - V1;
	T8 = T6 + 0x20;
	
	variable800D4EFC = T8;

	// RA = 2C(SP);
	// S0 = 28(SP);
	//SP = SP + 0xF8;
	V0 = 0;
}

//TWINE 8009DBE8
void CMORTDecoder::Function80045C78(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned long T8Param, unsigned char stackBuffer[0x100])
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
	unsigned long A3 = A3Param;
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
	unsigned long T8 = T8Param;
	unsigned long T9 = 0;

	//SP = SP - 0x98;
	S1 = A2;
	S2 = A3;
	S8 = A0;

	//RA to 3C(SP)
	//A1 to 9C(SP)

	S0 = 0;
	S3 = 0xC8; //AC(SP)
	S4 = 0xD8; //A8(SP)
	S5 = 0x60; //B0(SP);
	//S6 = SP + 0x44;
	S7 = 4;

	//800AA38C
	unsigned char stackBuffer2[0x50];
	for (int x = 0; x < 0x50; x++)
	{
		stackBuffer2[x] = 0x00;
	}

	do
	{
		//80045CCC
		A0 = (signed short)CharArrayToShort(&stackBuffer[S3]); //0(S3);
		A1 = (signed short)CharArrayToShort(&stackBuffer[S4]); //0(S4);
		A2 = S5;

		A3 = S6;
		//80045CD8
		Function80048590(A0, A1, A2, T8, stackBuffer, stackBuffer2);

		/*FILE* out = fopen("C:\\temp\\b.bin", "wb");
		fwrite(stackBuffer2, 1, 0x50, out);
		fflush(out);
		fclose(out);*/

		//80045CE0
		A0 = S8;
		A1 = (signed short)CharArrayToShort(&stackBuffer[S1]); //0(S1);
		A2 = (signed short)CharArrayToShort(&stackBuffer[S2]); //0(S2);

		A3 = S6;
		//80045CEC
		Function80048684(A0, A1, A2, A3, stackBuffer2);

		S0 = S0 + 1;
		S3 = S3 + 2;
		S2 = S2 + 2;
		S1 = S1 + 2;
		S4 = S4 + 2;


		//80045D08
		S5 = S5 + 0x1A;
	} while (S0 != S7);

	//80045D10
	//S5 to B0(SP);
	//S4 to A8(SP);
	//S3 to AC(SP);

	A0 = S8;
	A1 = 0xE8; //9C(SP);
	A2 = S8;

	A3 = CharArrayToLong(&stackBuffer[0x1C]); //B4(SP)
	Function80045A80(A0, A1, A2, A3, stackBuffer);
}

// TWINE 8009EE60
void CMORTDecoder::Function80048590(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long T8Param, unsigned char stackBuffer[0x100], unsigned char stackBuffer2[0x50])
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
	unsigned long A3 = 0; // Really would be S6, but using own buffer
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
	unsigned long T8 = T8Param;
	unsigned long T9 = 0;

	unsigned long RA = 0; //0x80045CE0

	unsigned char table8004867C[0x8];
	// NEMU FOR SOME REASON LOADS THIS WRONG
	table8004867C[0x00] = 0xC7;
	table8004867C[0x01] = 0xD7;
	table8004867C[0x02] = 0xE3;
	table8004867C[0x03] = 0xE7;
	table8004867C[0x04] = 0xF1;
	table8004867C[0x05] = 0xF3;
	table8004867C[0x06] = 0xF5;
	table8004867C[0x07] = 0xF7;

	//80048590
	AT = A0 - 7;
	if ((int)AT <= 0)
	{
		AT = RA;
		// BLTZAL
		RA = 0x800485A8;

		T7 = RA + 0xD4; //8004867C
		RA = AT;
		T7 = T7 + A0;

		//800485B4
		AT = (signed char)table8004867C[T7 - 0x8004867C]; //0(T7);

		T0 = AT >> 4;
		T1 = AT & 0x7;
	}
	else
	{
		//800485CC
		T0 = A0 - 8;
		T0 = T0 >> 3;

		T1 = A0 & 7;
	}

	//800485D8
	T4 = T1 << 0xB;
	T4 = T4 + 0x47FF;
	T2 = 6;

	//800485E4
	T2 = T2 - T0;
	T3 = 1;
	AT = T2 - 1;

	T3 = T3 << AT;
	T6 = 0xD;

	T8 = A2;

	do
	{
		//800485FC
		T5 = (signed short)CharArrayToShort(&stackBuffer[T8]); //0(T8);

		T5 = T5 << 0xD;
		T5 = T5 - 0x7000;

		T5 = T5 * T4;

		//80048614
		T5 = T5 + 0x4000;
		T5 = (int)T5 >> 0xF;

		T5 = T5 + T3;
		T5 = (int)T5 >> T2;

		WriteShortToBuffer(stackBuffer, T8, (unsigned short)T5); //T5 to 0(T8);
		T6 = T6 - 1;

		//8004862C
		T8 = T8 + 2;
	}
	while ((int)T6 > 0);

	//80048634
	T9 = A3; // Really stack at 800AA38C
	T6 = 0x28;

	do
	{
		WriteShortToBuffer(stackBuffer2, T9, 0); //0 to 0(T9);
		T6 = T6 - 1;
		
		T9 = T9 + 2;
	} while ((int)T6 > 0);

	//8004864C
	T9 = A3 + A1;
	T9 = T9 + A1;

	T8 = A2;
	T6 = 0xD;

	//8004865C
	do
	{
		AT = (signed short)CharArrayToShort(&stackBuffer[T8]); //AT = 0(T8)

		T8 = T8 + 2;
	
		WriteShortToBuffer(stackBuffer2, T9, (unsigned short)AT); // AT to 0(T9);
		T6 = T6 - 1;

		T9 = T9 + 6;
	} while ((int)T6 > 0);	
}

// Rogue 8009D8C4
// TWINE 8009EF54
void CMORTDecoder::Function80048684(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned char stackBuffer2[0x50])
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
	unsigned long A3 = A3Param;
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

	T3 = (signed short)CharArrayToShort(&buffer800D2940Predictor[(A0 + 0x166 - buffer800D2940Subtraction)]); //0x166(A0);

	AT = A1 - 0x28;

	if ((int)AT >= 0)
	{
		AT = AT - 0x50;
		if ((int)AT <= 0)
		{
			T3 = A1;
		}
	}

	//800486A4
	WriteShortToBuffer(buffer800D2940Predictor, (A0 + 0x166 - buffer800D2940Subtraction), (unsigned short)T3); //T3 to 0x166(A0);

	AT = RA;
	//BLTZAL NOOP
	RA = 0x800486B4;

	//800486B4
	T0 = RA + 0x84; //80048738
	RA = AT;
	T0 = T0 + A2;
	T0 = T0 + A2;

	//800486C4
	T0 = (unsigned short)table80048738[(T0 - 0x80048738) / 2]; // 0(T0);
	T2 = 0x77;
	T5 = A0;

	do
	{
		//800486D0
		T4 = CharArrayToShort(&buffer800D2940Predictor[T5 + 0x50 - buffer800D2940Subtraction]); // T4 = 0x50(T5)
		T2 = T2 - 1;

		//800486D8
		WriteShortToBuffer(buffer800D2940Predictor, T5 - buffer800D2940Subtraction, (unsigned short)T4); // T4 to 0(T5);

		if (outDebug != NULL)
			fprintf(outDebug, "800486D8: PREDICTOR VALUE WRITE %04X to %08X\n", (unsigned short)(T4 & 0xFFFF), T5);

		T5 = T5 + 2;
	}
	while ((int)T2 >= 0);

	//800486E4
	T2 = 0x27;
	T6 = A0;
	T5 = A0;

	T5 = T5 - T3;
	T5 = T5 - T3;

	T7 = A3;

	//800486FC
	do
	{
		T1 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T5 + 0xF0 - buffer800D2940Subtraction]); //F0(T5);
		if (outDebug != NULL)
			fprintf(outDebug, "800486FC: PREDICTOR VALUE READ %04X from %08X\n", (unsigned short)(T1 & 0xFFFF), (T5 + 0xF0));

		T5 = T5 + 2;
		T1 = T1 * T0;

		T1 = T1 + 0x4000;
		
		T1 = (int)T1 >> 0xF;

		//80048714
		AT = (signed short)CharArrayToShort(&stackBuffer2[T7]); //0(T7);
		T7 = T7 + 2;
		T1 = T1 + AT;

		//800D48720
		WriteShortToBuffer(buffer800D2940Predictor, (T6 + 0xF0 - buffer800D2940Subtraction), (unsigned short)T1); //T1 to 0xF0(T6);
		T6 = T6 + 2;

		T2 = T2 - 1;
	} while ((int)T2 >= 0);
}

//TWINE 8009D9F0
void CMORTDecoder::Function80045A80(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param, unsigned char stackBuffer[0x100])
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
	unsigned long A3 = A3Param;
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

	T8 = (signed short)CharArrayToShort(&stackBuffer[A1]); //0(A1);
	V1 = 0x3333;

	T6 = (signed short)CharArrayToShort(&buffer800D2940Predictor[A0 + 0x188 - buffer800D2940Subtraction]); //0x188(A0);

	T9 = T8 - 0x20;
	T0 = T9 << 0xA;

	T7 = T6 << 4;
	V0 = A0 + T7;

	AT = -0x332F000;
	
	//80045AB8
	V0 = V0 + 0x168;

	A2 = A3;
	T1 = T0 * V1;

	//80045AC4
	T2 = T1 + 0x4000;
	T3 = (int)T2 >> 0xF;

	T4 = T3 << 1;
	WriteShortToBuffer(buffer800D2940Predictor, V0 - buffer800D2940Subtraction, (unsigned short)T4); //T4 to 0(V0);
	T5 = (signed short)CharArrayToShort(&stackBuffer[A1 + 0x2]); //2(A1);

	T6 = T5 - 0x20;
	T7 = T6 << 0xA;

	T8 = T7 * V1;
	T9 = T8 + 0x4000;

	T0 = (int)T9 >> 0xF; 
	T1 = T0 << 1;

	WriteShortToBuffer(buffer800D2940Predictor, V0 + 0x2 - buffer800D2940Subtraction, (unsigned short)T1); //T1 to 2(V0);
	
	//80045AF8
	T2 = (signed short)CharArrayToShort(&stackBuffer[A1 + 0x4]); //4(A1);
	T3 = T2 - 0x10;

	T4 = T3 << 0xA;
	T5 = T4 * V1;

	// TODO Check signing here
	T6 = T5 + AT;
	T7 = (int)T6 >> 0xF;

	T8 = T7 << 1;
	WriteShortToBuffer(buffer800D2940Predictor, V0 + 0x4 - buffer800D2940Subtraction, (unsigned short)T8); //T8 to 4(V0);

	//80045B1C
	T9 = (signed short)CharArrayToShort(&stackBuffer[A1 + 0x6]); //6(A1);

	AT = 0x04003C00;
	T0 = T9 - 0x10;
	T1 = T0 << 0xA;

	T2 = T1 * V1;
	T3 = T2 + AT;
	T4 = (int)T3 >> 0xF;
	T5 = T4 << 1;
	WriteShortToBuffer(buffer800D2940Predictor, V0 + 0x6 - buffer800D2940Subtraction, (unsigned short)T5); //T5 to 6(V0);

	//80045B48
	T6 = (signed short)CharArrayToShort(&stackBuffer[A1 + 0x8]); //8(A1);
	AT = 0x4B17;
	T7 = T6 - 8;
	T8 = T7 << 0xA;

	T9 = T8 * AT;
	AT = 0xFFC91B1C;
	
	T0 = T9 + AT;
	T1 = (int)T0 >> 0xF;

	T2 = T1 << 1;
	WriteShortToBuffer(buffer800D2940Predictor, V0 + 0x8 - buffer800D2940Subtraction, (unsigned short)T2); //T2 to 8(V0);

	//80045B78
	T3 = (signed short)CharArrayToShort(&stackBuffer[A1 + 0xA]); //A(A1);

	AT = 0x03BBF800;
	T4 = T3 - 8;
	T5 = T4 << 0xA;
	T6 = T5 << 4;

	T6 = T6 + T5;
	T6 = T6 << 4;

	T6 = T6 + T5;
	T6 = T6 << 4;

	//80045BA0
	T6 = T6 + T5;
	T6 = T6 << 2;

	T7 = T6 + AT;
	T8 = (int)T7 >> 0xF;

	T9 = T8 << 1;
	WriteShortToBuffer(buffer800D2940Predictor, V0 + 0xA - buffer800D2940Subtraction, (unsigned short)T9); //T9 to A(V0);

	//80045BB8
	T0 = (signed short)CharArrayToShort(&stackBuffer[A1 + 0xC]); //C(A1);
	AT = 0x147936C;
	T1 = T0 - 4;

	T2 = T1 << 0xA;
	T3 = T2 << 0x5;
	T3 = T3 - T2;
	T3 = T3 << 2;
	T3 = T3 - T2;
	T3 = T3 << 3;
	T3 = T3 - T2;
	T3 = T3 << 4;
	T3 = T3 - T2;
	T3 = T3 << 1;
	T4 = T3 + AT;
	T5 = (int)T4 >> 0xF;
	T6 = T5 << 1;
	WriteShortToBuffer(buffer800D2940Predictor, V0 + 0xC - buffer800D2940Subtraction, (unsigned short)T6); //T6 to C(V0);

	//80045C00
	T7 = (signed short)CharArrayToShort(&stackBuffer[A1 + 0xE]); //E(A1);
	AT = 0x40D6B40;
	T8 = T7 - 4;

	T9 = T8 << 0xA;
	T0 = T9 << 3;
	T0 = T0 - T9;
	T0 = T0 << 2;
	T0 = T0 + T9;
	T0 = T0 << 6;
	T0 = T0 + T9;
	T0 = T0 << 2;
	T0 = T0 - T9;
	T0 = T0 << 2;
	T1 = T0 + AT;
	T2 = (int)T1 >> 0xF;
	T3 = T2 << 1;
	WriteShortToBuffer(buffer800D2940Predictor, V0 + 0xE - buffer800D2940Subtraction, (unsigned short)T3); //T3 to E(V0);

	//80045C48
	//A0 to 20(SP);

	A1 = variable800FCEDCPredictorPointer800D2940;//28(SP);
	//80045C4C
	Function80048904(A0, A1, A2, A3);

	//A0 = 20(SP);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[A0 + 0x188 - buffer800D2940Subtraction]);
	T5 = T4 ^ 0x0001;

	//80045C60
	WriteShortToBuffer(buffer800D2940Predictor, (A0 + 0x188 - buffer800D2940Subtraction), (unsigned short)T5); //T5 to 0x188(A0);
	//RA = 1A(SP)
	//S0 = 18(SP)

	// SP = SP + 0x20;
}

void CMORTDecoder::Function80048904(unsigned long A0Param, unsigned long A1Param, unsigned long A2Param, unsigned long A3Param)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
	unsigned long A1 = A1Param;
	unsigned long A2 = A2Param;
	unsigned long A3 = A3Param;
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

	AT = A0 + 0x140; // 800D2A80
	S0 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 0 - buffer800D2940Subtraction]); //0(AT)
	S1 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 4 - buffer800D2940Subtraction]); //4(AT)
	S2 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 8 - buffer800D2940Subtraction]); //8(AT)
	S3 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 0xC - buffer800D2940Subtraction]); //C(AT)
	S4 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 0x10 - buffer800D2940Subtraction]); //10(AT)
	S5 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 0x14 - buffer800D2940Subtraction]); //14(AT)
	S6 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 0x18  - buffer800D2940Subtraction]); //18(AT)
	S7 = (signed long)CharArrayToLong(&buffer800D2940Predictor[AT + 0x1C - buffer800D2940Subtraction]); //1C(AT)
	A3 = (signed short)CharArrayToShort(&buffer800D2940Predictor[A0 + 0x164 - buffer800D2940Subtraction]); //0x164(A0)

	// Function pointer formation to 80048AFC
	T3 = 0x80048AFC; //RA + 0x190;

	// 80048970
	Function80048A58(A0, T3, T6, T7, T8, T9);

	//80048978
	T1 = A1; // 800D2940
	T0 = A2; // 800D2AD8
	T3 = 0xC;
	Function80048740(T0, T1, T3, T6, T7, T8, T9, A3, S0, S1, S2, S3, S4, S5, S6, S7);

	//80048990
	T3 = 0x80048B14; //RA + 0x184;
	
	//80048994
	Function80048A58(A0, T3, T6, T7, T8, T9);

	//8004899C
	T1 = A1 + 0x1A; // 800D2940
	T0 = A2 + 0x1A; // 800D2AD8
	T3 = 0xD;
	Function80048740(T0, T1, T3, T6, T7, T8, T9, A3, S0, S1, S2, S3, S4, S5, S6, S7);

	T3 = 0x80048B24; //RA + 0x170

	//800489B8
	Function80048A58(A0, T3, T6, T7, T8, T9);

	//800489C0
	T1 = A1 + 0x36; // 800D2940
	T0 = A2 + 0x36; // 800D2AD8
	T3 = 0xC;
	Function80048740(T0, T1, T3, T6, T7, T8, T9, A3, S0, S1, S2, S3, S4, S5, S6, S7);

	T3 = 0x80048B3C; //RA + 0x160
	//800489DC
	Function80048A58(A0, T3, T6, T7, T8, T9);

	//800489E4
	T1 = A1 + 0x50; // 800D2940
	T0 = A2 + 0x50; // 800D2AD8
	T3 = 0x77;
	Function80048740(T0, T1, T3, T6, T7, T8, T9, A3, S0, S1, S2, S3, S4, S5, S6, S7);
	
	// set stuff back
	/*FILE* a = fopen("C:\\temp\\c.bin", "w");
	fwrite(buffer800D2AD8IntermediateValue, 1, 0x1400, a);
	fflush(a);
	fclose(a);*/
	
	//800489F8
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 0 - buffer800D2940Subtraction), S0); //0(AT)
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 4 - buffer800D2940Subtraction), S1); //4(AT)
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 8 - buffer800D2940Subtraction), S2); //8(AT)
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 0xC - buffer800D2940Subtraction), S3); //C(AT)
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 0x10 - buffer800D2940Subtraction), S4); //10(AT)
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 0x14 - buffer800D2940Subtraction), S5); //14(AT)
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 0x18 - buffer800D2940Subtraction), S6); //18(AT)
	WriteLongToBuffer(buffer800D2940Predictor, (AT + 0x1C - buffer800D2940Subtraction), S7); //1C(AT)
	WriteShortToBuffer(buffer800D2940Predictor, (A0 + 0x164 - buffer800D2940Subtraction), (unsigned short)A3); //164(A0)
}

void CMORTDecoder::CallT380048XXXFunction(unsigned long& T2, unsigned long T3, unsigned long& T4)
{
	if (T3 == 0x80048AFC)
		Function80048AFC(T2, T4); // JALR RA,T3
	else if (T3 == 0x80048B14)
		Function80048B14(T2, T4); // JALR RA,T3
	else if (T3 == 0x80048B24)
		Function80048B24(T2, T4); // JALR RA,T3
	else if (T3 == 0x80048B3C)
		Function80048B3C(T2, T4); // JALR RA,T3
}

void CMORTDecoder::Function80048AFC(unsigned long& T2, unsigned long& T4)
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

	T2 = (int)T2 >> 2;
	T4 = (int)T4 >> 1;

	T2 = T2 + T4;
	T4 = (int)T4 >> 1;

	//80048B0C
	T2 = T2 + T4;

	// Branch always to 80048B3C
	Function80048B3C(T2, T4);
}

void CMORTDecoder::Function80048B14(unsigned long& T2, unsigned long& T4)
{
	//80048B14
	T2 = (int)T2 >> 1;
	T4 = (int)T4 >> 1;

	T2 = T2 + T4;

	// Branch always to 80048B3C
	Function80048B3C(T2, T4);
}

void CMORTDecoder::Function80048B24(unsigned long& T2, unsigned long& T4)
{
	T2 = (int)T2 >> 1;
	T4 = (int)T4 >> 2;

	T4 = T4 + T2;
	T2 = (int)T2 >> 1;

	T2 = T2 + T4;

	// Branch always to 80048B3C
	Function80048B3C(T2, T4);
}

void CMORTDecoder::Function80048B3C(unsigned long& T2, unsigned long& T4)
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
	T4 = 0;
	if ((int)T2 < 0)
	{
		T2 = 0 - T2;
		T4 = 1;
	}
	//80048B4C
	AT = T2 - 0x2B33;

	//80048B50
	if ((int)AT >= 0)
	{
		AT = T2 - 0x4E66;

		//80048B5C
		if ((int)AT >= 0)
		{
			AT = (int)T2 >> 2;

			AT = AT + 0x6600;
		}
		else
		{
			AT = T2 + 0x2B33;
		}
	}
	else
	{
		AT = T2 << 1;
	}

	//80048B6C
	T2 = AT - 0x7FFF;

	if ((int)T2 <= 0)
	{
		T2 = AT;
	}
	else
	{
		T2 = 0x7FFF;
	}

	//80048B7C
	if (T4 != 0)
	{
		T2 = -(signed)T2;
	}

	T2 = T2 & 0xFFFF;
}

void CMORTDecoder::Function80048740(unsigned long T0Param, unsigned long T1Param, unsigned long T3Param, unsigned long T6Param, unsigned long T7Param, unsigned long T8Param, unsigned long T9Param, unsigned long& A3, unsigned long& S0, unsigned long& S1, unsigned long& S2, unsigned long& S3, unsigned long& S4, unsigned long& S5, unsigned long& S6, unsigned long& S7)
{
	unsigned long AT = 0;
	unsigned long A0 = 0;
	unsigned long A1 = 0;
	unsigned long A2 = 0;
	unsigned long V0 = 0;
	unsigned long V1 = 0;
	unsigned long S8 = 0;
	unsigned long T0 = T0Param;
	unsigned long T1 = T1Param;
	unsigned long T2 = 0;
	unsigned long T3 = T3Param;
	unsigned long T4 = 0;
	unsigned long T5 = 0;
	unsigned long T6 = T6Param;
	unsigned long T7 = T7Param;
	unsigned long T8 = T8Param;
	unsigned long T9 = T9Param;

	//80048740
	do
	{
		T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 - buffer800D2940Subtraction]); //0(T1)
		if (outDebug != NULL)
			fprintf(outDebug, "80048740: PREDICTOR VALUE READ %04X from %08X\n", (unsigned short)(T2 & 0xFFFF), T1);

		T1 = T1 + 2;
		T5 = T9 << 0x10;
		T5 = (int)T5 >> 0x10;

		AT = S7 * T5;
		
		//80048758
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		T5 = (int)T9 >> 0x10;
		AT = S6 * T5;

		//80048770
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		AT = T5 * T2;

		//80048784
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		S7 = AT + S6;
		T5 = T8 << 0x10;
		T5 = (int)T5 >> 0x10;
		AT = S5 * T5;

		//800487A0
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		AT = T5 * T2;

		//800487B4
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		S6 = AT + S5;
		T5 = (int)T8 >> 0x10;
		AT = S4 * T5;

		//800487CC
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		AT = T5 * T2;

		//800487E0
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		S5 = AT + S4;
		T5 = T7 << 0x10;
		T5 = (int)T5 >> 0x10;
		AT = S3 * T5;

		//800487FC
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		AT = T5 * T2;

		//80048810
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		S4 = AT + S3;
		T5 = (int)T7 >> 0x10;
		AT = S2 * T5;

		//80048828
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		AT = T5 * T2;

		//8004883C
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		S3 = AT + S2;
		T5 = T6 << 0x10;
		T5 = (int)T5 >> 0x10;
		AT = S1 * T5;

		//80048858
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		AT = T5 * T2;

		//8004886C
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		S2 = AT + S1;
		T5 = (int)T6 >> 0x10;
		AT = S0 * T5;

		//80048884
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 - AT;
		AT = T5 * T2;

		//80048898
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		S1 = AT + S0;
		S0 = T2;

		AT = 0x6E14;
		AT = AT * A3;

		//800488B4
		AT = AT + 0x4000;
		AT = (int)AT >> 0xF;
		T2 = T2 + AT;
		//800488C0
		A3 = T2;
		T2 = T2 << 1;
		AT = (int)T2 >> 0xF;
		AT = AT + 1;
		AT = (int)AT >> 1;

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
		WriteShortToBuffer(buffer800D2AD8IntermediateValue, T0 + 0 - buffer800D2AD8Subtraction, (unsigned short)T2);
		if (outDebug != NULL)
			fprintf(outDebug, "800488EC: INTERMEDIATE VALUE WRITE %04X to %08X\n", (unsigned short)(T2 & 0xFFFF), T0);

		T0 = T0 + 2;

		T3 = T3 - 1;
	}
	while ((int)T3 >= 0);
}

void CMORTDecoder::Function80048A58(unsigned long A0Param, unsigned long T3Param, unsigned long& T6, unsigned long& T7, unsigned long& T8, unsigned long& T9)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
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
	unsigned long T3 = T3Param;
	unsigned long T4 = 0;
	unsigned long T5 = 0;

	AT = (signed short)CharArrayToShort(&buffer800D2940Predictor[A0 + 0x188 - buffer800D2940Subtraction]); //0x188(A0);

	T0 = A0 + 0x168;
	T1 = T0 + 0x10;

	//80048A60
	if (AT != 0)
	{
		T0 = T1;
		T1 = A0 + 0x168;
	}

	//80048A70
	//T5 = RA;

	//80048A74
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 - buffer800D2940Subtraction]); //0(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 - buffer800D2940Subtraction]); //0(T1)	
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T6 = T2 << 0x10;

	//80048A84
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 + 2 - buffer800D2940Subtraction]); //2(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 + 2 - buffer800D2940Subtraction]); //2(T1);
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T6 = T6 | T2;

	//80048A94
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 + 4 - buffer800D2940Subtraction]); //4(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 + 4 - buffer800D2940Subtraction]); //4(T1)	
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T7 = T2 << 0x10;

	//80048AA4
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 + 6 - buffer800D2940Subtraction]); //6(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 + 6 - buffer800D2940Subtraction]); //6(T1);
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T7 = T7 | T2;

	//80048AB4
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 + 8 - buffer800D2940Subtraction]); //8(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 + 8 - buffer800D2940Subtraction]); //8(T1)	
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T8 = T2 << 0x10;

	//80048AC4
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 + 0xA - buffer800D2940Subtraction]); //A(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 + 0xA - buffer800D2940Subtraction]); //A(T1);
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T8 = T8 | T2;

	//80048AD4
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 + 0xC - buffer800D2940Subtraction]); //C(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 + 0xC - buffer800D2940Subtraction]); //C(T1)	
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T9 = T2 << 0x10;

	//80048AE4
	T2 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T0 + 0xE - buffer800D2940Subtraction]); //E(T0);
	T4 = (signed short)CharArrayToShort(&buffer800D2940Predictor[T1 + 0xE - buffer800D2940Subtraction]); //E(T1);
	CallT380048XXXFunction(T2, T3, T4); // JALR RA,T3 to 80048AFC
	T9 = T9 | T2;

	//RA back to T5 original
}

void CMORTDecoder::Function80045A48(unsigned long A0Param)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
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

void CMORTDecoder::Function80045A68(unsigned long A0Param)
{
	unsigned long AT = 0;
	unsigned long A0 = A0Param;
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