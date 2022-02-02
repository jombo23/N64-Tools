#include "StdAfx.h"
#include "SharedFunctions.h"
#include "MidiParse.h"
#include <sys/stat.h>
#include "SupermanDecoder.h"
#include "rnc_deco.h"
#include "H20Decoder.h"
#include "TetrisphereDecoder.h"
#include "GECompression.h"
#include "MidwayDecoder.h"
#include "BlitzDecoder.h"
#include "TigDecoder.h"
#include "MarioTennisDecoder.h"
#include "yay0.h"
#include "VigilanteDecoder.h"
#include "FLA2Decoder.h"
#include "RugratsDecoder.h"
#include "flzh_rn.h"
#include "n643docompression.h"
#include "LZARIDecoder.h"
#include "SnowDecoder.h"
#include "ASMICDecoder.h"
#include "NaganoDecoder.h"
#include "yaz0.h"
#include "AidynDecoder.h"
#include "AidynToDCMConvertor.h"
#include "QuakeDecoder.h"
#include "ViewpointDecoder.h"
#include "HexenDecoder.h"
#include "MKMythologiesDecode.h"

#include <algorithm>
#include <map>
#include <math.h>

void CMidiParse::fprintfIfDebug(FILE* outFileDebug, char* format,...) 
{
	va_list args;
    va_start( args, format );
	if (outFileDebug != NULL)
		vfprintf(outFileDebug, format, args );
    va_end( args );
}

unsigned long CMidiParse::GetVLBytes(byte* vlByteArray, int& offset, unsigned long& original, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats)
{
    unsigned long VLVal = 0; //Vlength Value.
    byte TempByte; //Byte value read.

    for (; ; )
    {
		if (altPattern != NULL)
		{
			TempByte = altPattern[altOffset];
			altOffset++;

			if (altOffset == altLength)
			{
				delete [] altPattern;
				altPattern = NULL;
				altOffset = 0;
				altLength = 0;
			}
		}
		else
		{
			TempByte = vlByteArray[offset];
			offset++;

			if ((TempByte == 0xFE) && (vlByteArray[offset] != 0xFE) && includeFERepeats)
			{
				byte repeatFirstByte = vlByteArray[offset];
				offset++;

				unsigned short repeatDistanceFromBeginningMarker = ((repeatFirstByte << 8) | vlByteArray[offset]);
				offset++;
				byte repeatCount = vlByteArray[offset];
				offset++;

				altPattern = new byte[repeatCount];
				for (int copy = ((offset - 4) - repeatDistanceFromBeginningMarker); copy < (((offset - 4) - repeatDistanceFromBeginningMarker) + repeatCount); copy++)
				{
					altPattern[copy - ((offset - 4) - repeatDistanceFromBeginningMarker)] = vlByteArray[copy];
				}
				altOffset = 0;
				altLength = repeatCount;

				TempByte = altPattern[altOffset];
				altOffset++;
			}
			else if ((TempByte == 0xFE) && (vlByteArray[offset] == 0xFE) && includeFERepeats)
			{
				// skip duplicate FEs
				offset++;
			}

			if ((altOffset == altLength) && (altPattern != NULL))
			{
				delete [] altPattern;
				altPattern = NULL;
				altOffset = 0;
				altLength = 0;
			}
		}
        if ((TempByte >> 7) == 0x1)
        {
            VLVal += TempByte;
            VLVal = VLVal << 8; //Shift to next byte in VLVal.
        }
        else
        {
            VLVal += TempByte;
            break;
        } 
    }
    
	original = VLVal;

	unsigned long Vlength = 0;

	for (int c = 0, a = 0; ;c += 8, a+= 7)
    {
       
        Vlength += (((VLVal >> c) & 0x7F) << a);
		if (c == 24)
			break;
    }
    return Vlength;
}




void CMidiParse::WriteVLBytes(FILE* outFile, unsigned long value, unsigned long length, bool includeFERepeats)
{
	byte tempByte;
	if (length == 1)
	{
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else if (length == 2)
	{
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else if (length == 3)
	{
		tempByte = (value >> 16) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
	else
	{
		tempByte = (value >> 24) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = (value >> 8) & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
		tempByte = value & 0xFF;
		fwrite(&tempByte, 1, 1, outFile);
	}
}

unsigned long ReturnVLBytes(unsigned long value, unsigned long& length)
{
	byte subValue1 = (value >> 21) & 0x7F;
	byte subValue2 = (value >> 14) & 0x7F;
	byte subValue3 = (value >> 7) & 0x7F;
	byte subValue4 = (value >> 0) & 0x7F;

	if (subValue1 > 0)
	{
		unsigned long newValue = 0x80808000;
		newValue |= (subValue1 << 24);
		newValue |= (subValue2 << 16);
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 4;
		return newValue;
	}
	else if (subValue2 > 0)
	{
		unsigned long newValue = 0x00808000;
		newValue |= (subValue2 << 16);
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 3;
		return newValue;
	}
	else if (subValue3 > 0)
	{
		unsigned long newValue = 0x00008000;
		newValue |= (subValue3 << 8);
		newValue |= subValue4;
		length = 2;
		return newValue;
	}
	else
	{
		length = 1;
		return value;
	}
}

byte CMidiParse::ReadMidiByte(byte* vlByteArray, int& offset, byte*& altPattern, byte& altOffset, byte& altLength, bool includeFERepeats)
{
	byte returnByte;
	if (altPattern != NULL)
	{
		returnByte = altPattern[altOffset];
		altOffset++;
	}
	else
	{
		returnByte = vlByteArray[offset];
		offset++;

		if ((returnByte == 0xFE) && (vlByteArray[offset] != 0xFE) && includeFERepeats)
		{
			byte repeatFirstByte = vlByteArray[offset];
			offset++;

			unsigned long repeatDistanceFromBeginningMarker = ((repeatFirstByte << 8) | vlByteArray[offset]);
			offset++;
			byte repeatCount = vlByteArray[offset];
			offset++;

			altPattern = new byte[repeatCount];
			for (int copy = ((offset - 4) - repeatDistanceFromBeginningMarker); copy < (((offset - 4) - repeatDistanceFromBeginningMarker) + repeatCount); copy++)
			{
				altPattern[copy - ((offset - 4) - repeatDistanceFromBeginningMarker)] = vlByteArray[copy];
			}
			altOffset = 0;
			altLength = repeatCount;

			returnByte = altPattern[altOffset];
			altOffset++;
		}
		else if ((returnByte == 0xFE) && (vlByteArray[offset] == 0xFE) && includeFERepeats)
		{
			// skip duplicate FEs
			offset++;
		}
	}

	if ((altOffset == altLength) && (altPattern != NULL))
	{
		delete [] altPattern;
		altPattern = NULL;
		altOffset = 0;
		altLength = 0;
	}
	
	return returnByte;
}

CMidiParse::CMidiParse(void)
{
	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);
	mainFolder.Format("%s\\", tempFolder);

	compress = new GECompression();

	for (int i = 0; i < 0x20; i++)
		trackEventCount[i] = 0;
	trackEvents = new TrackEvent *[0x20];
	for (unsigned int x = 0; x < 0x20; x++)
	{
		trackEvents[x] = new TrackEvent[0x30000];
		for (int y = 0; y < 0x30000; y++)
			trackEvents[x][y].contentSize = NULL;
	}
}

CMidiParse::~CMidiParse(void)
{
	for (unsigned int x = 0; x < 0x20; x++)
	{
		for (int y = 0; y < 0x30000; y++)
		{
			if (trackEvents[x][y].contents != NULL)
			{
				delete[] trackEvents[x][y].contents;
				trackEvents[x][y].contents = NULL;
			}
		}
		delete[] trackEvents[x];
	}
	delete [] trackEvents;

	delete compress;
}



void CMidiParse::DuckDodgersMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool extendTracksToHighest, bool usePitchBendSensitity, int pitchBendSensitity, unsigned long division)
{
	numberInstruments = 0;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		unsigned long lengthHeader = 0;

		// parse midi
		
		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000001); // Type 0, 1 track
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		int counterTrack = 0;

		int highestTrackLength = 0;

		int numberType0Controllers = 1;

		//for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			unsigned long absoluteTime = 0;

			unsigned long offset = 0;
			
			int position = offset;	

			//if (position != 0)
			{
				int previousEventValue = 0;

				std::map<int, int> loopEndsWithCount;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;

				while ((position < inputSize) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					absoluteTime += timeTag;

					if (absoluteTime > highestTrackLength)
						highestTrackLength = absoluteTime;


					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
							if ((loopCount == 0xFF) || (loopCount == 0x00))
							{
								break;
							}
							else
							{
								std::map<int, int>::iterator it = loopEndsWithCount.find(position);
								if (it != loopEndsWithCount.end())
								{
									int countLeft = it->second;

									if (countLeft == 0)
									{
										loopEndsWithCount.erase(it);
									}
									else
									{
										loopEndsWithCount[position] = (countLeft - 1);

										if (repeatPattern == NULL)
										{
											position = position - offsetToBeginningLoop;
										}
										else
										{
											loopEndsWithCount.erase(it);
										}
									}
								}
								else
								{
									loopEndsWithCount[position] = loopCount - 1;

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
						}
						else if (subType == 0x2F)
						{
							endFlag = true;
						}

						// Not set to previous event value
						//if (!statusBit)
							//previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						if (eventVal >= 0x90 && eventVal < 0xA0)
						{
							if (numberType0Controllers < ((eventVal & 0xF) + 1))
								numberType0Controllers = ((eventVal & 0xF) + 1);
						}
						if (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0))
						{
							if (numberType0Controllers < ((previousEventValue & 0xF) + 1))
								numberType0Controllers = ((previousEventValue & 0xF) + 1);
						}

						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (velocity == 0)
						{
							// Turn off
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						
					}
				}
			}
		}

		//for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			unsigned long absoluteTime = 0;

			int trackEventCountSub = 0;
			TrackEvent* trackEventsSub = new TrackEvent[0x30000];

			for (int j = 0; j < 0x30000; j++)
			{
				trackEventsSub[j].contents = NULL;
				trackEventsSub[j].obsoleteEvent = false;
				trackEventsSub[j].deltaTime = 0;
				trackEventsSub[j].absoluteTime = 0;

			}

			unsigned long offset = 0; //CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			//if (position != 0)
			{
				tempLong = Flip32Bit(0x4D54726B);
				fwrite(&tempLong, 1 ,4 , outFile);
	
				int previousEventValue = 0;

				std::map<int, int> loopEndsWithCount;
				std::vector<int> loopNumbers;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;

				if (usePitchBendSensitity)
				{
					//https://www.midikits.net/midi_analyser/pitch_bend.htm

					for (int iii = 0; iii < numberType0Controllers; iii++)
					{
						trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						
						trackEventsSub[trackEventCountSub].contents[0] = 0x64;
						trackEventsSub[trackEventCountSub].contents[1] = 0x00;

						trackEventCountSub++;


						trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						
						trackEventsSub[trackEventCountSub].contents[0] = 0x65;
						trackEventsSub[trackEventCountSub].contents[1] = 0x00;

						trackEventCountSub++;


						trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						
						trackEventsSub[trackEventCountSub].contents[0] = 0x06;
						if (pitchBendSensitity > 0x18)
							pitchBendSensitity = 0x18;
						trackEventsSub[trackEventCountSub].contents[1] = pitchBendSensitity;

						trackEventCountSub++;

						trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						
						trackEventsSub[trackEventCountSub].contents[0] = 0x64;
						trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

						trackEventCountSub++;


						trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						
						trackEventsSub[trackEventCountSub].contents[0] = 0x65;
						trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

						trackEventCountSub++;
					}
				}

				while ((position < inputSize) && !endFlag)
				{
					if (extendTracksToHighest)
					{
						if (absoluteTime >= highestTrackLength)
						{
							trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
							trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;

							break;
						}
					}


					if (trackEventCountSub >= 0x30000)
					{
						for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
						{
							if (trackEventsSub[eventCount].contents != NULL)
							{
								delete [] trackEventsSub[eventCount].contents;
								trackEventsSub[eventCount].contents = NULL;
							}
						}

						delete [] trackEventsSub;
						return;
					}

					int timePosition = position;

					unsigned long original;
					// trackEventsSub[trackEventCountSub].deltaTime is for loops
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					if (extendTracksToHighest)
					{
						if ((absoluteTime + timeTag) > highestTrackLength)
						{
							trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
							trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;

							break;
						}
					}

					trackEventsSub[trackEventCountSub].deltaTime += timeTag;

					absoluteTime += timeTag;
					trackEventsSub[trackEventCountSub].absoluteTime = absoluteTime;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 5;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x51;
							trackEventsSub[trackEventCountSub].contents[1] = 0x3;
							trackEventsSub[trackEventCountSub].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
							trackEventsSub[trackEventCountSub].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
							trackEventsSub[trackEventCountSub].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

							trackEventCountSub++;

							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							int loopNumber = 0;
							if (loopNumbers.size() > 0)
							{
								loopNumber = loopNumbers.back();
								loopNumbers.pop_back();
							}

							// Fake loop end, controller 103
							for (int iii = 0; iii < numberType0Controllers; iii++)
							{
								trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
								trackEventsSub[trackEventCountSub].contentSize = 2;
								trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
								trackEventsSub[trackEventCountSub].contents[0] = 103;
								trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
								trackEventCountSub++;
							}

							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
							if ((loopCount == 0xFF) || (loopCount == 0x00))
							{
								hasLoopPoint = true;
								loopEnd = absoluteTime;

								if (extendTracksToHighest)
								{
									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
								}
							}
							else
							{
								std::map<int, int>::iterator it = loopEndsWithCount.find(position);
								if (it != loopEndsWithCount.end())
								{
									int countLeft = it->second;

									if (countLeft == 0)
									{
										loopEndsWithCount.erase(it);
									}
									else
									{
										loopEndsWithCount[position] = (countLeft - 1);

										if (repeatPattern == NULL)
										{
											position = position - offsetToBeginningLoop;
										}
										else
										{
											loopEndsWithCount.erase(it);
										}
									}
								}
								else
								{
									loopEndsWithCount[position] = loopCount - 1;

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
							hasLoopPoint = true;
							loopStart = absoluteTime;

							for (int iii = 0; iii < numberType0Controllers; iii++)
							{
								// Fake loop start, controller 102
								trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
								trackEventsSub[trackEventCountSub].contentSize = 2;
								trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
								trackEventsSub[trackEventCountSub].contents[0] = 102;
								trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
								trackEventCountSub++;
							}



							loopNumbers.push_back(loopNumber);
						}
						else if (subType == 0x2F)
						{
							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;
						}

						// In Duck Dodgers not set for FF
						//if (!statusBit)
							//previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							trackEventsSub[trackEventCountSub].type = previousEventValue;
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							trackEventsSub[trackEventCountSub].type = eventVal;
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (velocity == 0)
						{
							bool notFound = true;
							// Turn off
							for (int check = (trackEventCountSub - 1); check >= 0; check--)
							{
								if (trackEventsSub[check].contents[0] == noteNumber)
								{
									trackEventsSub[check].durationTime = absoluteTime - trackEventsSub[check].durationTime;
									notFound = false;
									break;
								}
							}

							if (notFound)
							{
								notFound = notFound;
							}
						}
						else
						{
							trackEventsSub[trackEventCountSub].durationTime = 0; // to be filled in
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							trackEventsSub[trackEventCountSub].contents[0] = noteNumber;
							trackEventsSub[trackEventCountSub].contents[1] = velocity;

							trackEventCountSub++;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = controllerType;
						trackEventsSub[trackEventCountSub].contents[1] = controllerValue;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}

						trackEventsSub[trackEventCountSub].contentSize = 1;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = instrument;
						if (instrument >= numberInstruments)
							numberInstruments = (instrument + 1);

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}

						trackEventsSub[trackEventCountSub].contentSize = 1;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = amount;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = valueLSB;
						trackEventsSub[trackEventCountSub].contents[1] = valueMSB;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
					}

				}

				for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
				{
					if (trackEventCountSub >= 0x30000)
					{
						fclose(outFile);

						for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
						{
							if (trackEventsSub[eventCount].contents != NULL)
							{
								delete [] trackEventsSub[eventCount].contents;
								trackEventsSub[eventCount].contents = NULL;
							}
						}

						delete [] trackEventsSub;
						return;
					}

					TrackEvent trackEvent = trackEventsSub[eventCount];
					if ((trackEvent.type >= 0x90) && (trackEvent.type < 0xA0))
					{
						// need to split out
						if (trackEvent.durationTime > 0)
						{

							unsigned long shutoffTime = (trackEvent.absoluteTime + trackEvent.durationTime);

							if (eventCount != (trackEventCountSub - 1))
							{

								for (int i = (eventCount+1); i < trackEventCountSub; i++)
								{
									if ((trackEventsSub[i].absoluteTime >= shutoffTime) && (i != (trackEventCountSub - 1)))
									{
										for (int j = (trackEventCountSub - 1); j >= i; j--)
										{
											trackEventsSub[j+1].absoluteTime = trackEventsSub[j].absoluteTime;
											trackEventsSub[j+1].contentSize = trackEventsSub[j].contentSize;
											if (trackEventsSub[j+1].contents != NULL)
											{
												delete [] trackEventsSub[j+1].contents;
												trackEventsSub[j+1].contents = NULL;
											}
											trackEventsSub[j+1].contents = new byte[trackEventsSub[j].contentSize];
											for (int r = 0; r < trackEventsSub[j].contentSize; r++)
											{
												trackEventsSub[j+1].contents[r] = trackEventsSub[j].contents[r];
											}
											trackEventsSub[j+1].deltaTime = trackEventsSub[j].deltaTime;
											trackEventsSub[j+1].durationTime = trackEventsSub[j].durationTime;
											trackEventsSub[j+1].obsoleteEvent = trackEventsSub[j].obsoleteEvent;
											trackEventsSub[j+1].type = trackEventsSub[j].type;
										}

										trackEventsSub[i].type = trackEventsSub[eventCount].type;
										trackEventsSub[i].absoluteTime = shutoffTime;
										trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i-1].absoluteTime);
										trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
										trackEventsSub[i].durationTime = 0;


										if (trackEventsSub[i].contents != NULL)
										{
											delete [] trackEventsSub[i].contents;
										}

										trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
										trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
										trackEventsSub[i].contents[1] = 0;

										trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);

										if (trackEventsSub[i].deltaTime > 0xFF000000)
										{
											int a =1;
										}

										trackEventCountSub++;
										break;
									}
									else if (i == (trackEventCountSub - 1))
									{
										trackEventsSub[i+1].absoluteTime = shutoffTime; // move end to end
										trackEventsSub[i+1].contentSize = trackEventsSub[i].contentSize;
										if (trackEventsSub[i+1].contents != NULL)
										{
											delete [] trackEventsSub[i+1].contents;
											trackEventsSub[i+1].contents = NULL;
										}
										trackEventsSub[i+1].contents = new byte[trackEventsSub[i].contentSize];
										for (int r = 0; r < trackEventsSub[i].contentSize; r++)
										{
											trackEventsSub[i+1].contents[r] = trackEventsSub[i].contents[r];
										}
										trackEventsSub[i+1].deltaTime = trackEventsSub[i].deltaTime;
										trackEventsSub[i+1].durationTime = trackEventsSub[i].durationTime;
										trackEventsSub[i+1].obsoleteEvent = trackEventsSub[i].obsoleteEvent;
										trackEventsSub[i+1].type = trackEventsSub[i].type;


										trackEventsSub[i].type = trackEventsSub[eventCount].type;
										trackEventsSub[i].absoluteTime = shutoffTime;
										trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i - 1].absoluteTime);
										trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
										trackEventsSub[i].durationTime = 0;

										if (trackEventsSub[i].contents != NULL)
										{
											delete [] trackEventsSub[i].contents;
										}

										trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
										trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
										trackEventsSub[i].contents[1] = 0;

										trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);

										trackEventCountSub++;
										break;
									}
								}
							}
							else
							{
								trackEventsSub[eventCount+1].absoluteTime = shutoffTime; // move end to end
								trackEventsSub[eventCount+1].contentSize = trackEventsSub[eventCount].contentSize;
								if (trackEventsSub[eventCount+1].contents != NULL)
								{
									delete [] trackEventsSub[eventCount+1].contents;
									trackEventsSub[eventCount+1].contents = NULL;
								}
								trackEventsSub[eventCount+1].contents = new byte[trackEventsSub[eventCount].contentSize];
								for (int r = 0; r < trackEventsSub[eventCount].contentSize; r++)
								{
									trackEventsSub[eventCount+1].contents[r] = trackEventsSub[eventCount].contents[r];
								}
								trackEventsSub[eventCount+1].deltaTime = trackEventsSub[eventCount].deltaTime;
								trackEventsSub[eventCount+1].durationTime = trackEventsSub[eventCount].durationTime;
								trackEventsSub[eventCount+1].obsoleteEvent = trackEventsSub[eventCount].obsoleteEvent;
								trackEventsSub[eventCount+1].type = trackEventsSub[eventCount].type;


								trackEventsSub[eventCount].type = trackEventsSub[eventCount].type;
								trackEventsSub[eventCount].absoluteTime = shutoffTime;
								if ((trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime) > 0xFF000000)
								{
									int a =1;
								}
								trackEventsSub[eventCount].deltaTime = (trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime);
								trackEventsSub[eventCount].contentSize = trackEventsSub[eventCount].contentSize;
								trackEventsSub[eventCount].durationTime = 0;
								trackEventsSub[eventCount].contents = new byte[trackEventsSub[eventCount].contentSize];
								trackEventsSub[eventCount].contents[0] = trackEventsSub[eventCount].contents[0];
								trackEventsSub[eventCount].contents[1] = 0;

								trackEventsSub[eventCount+1].deltaTime = (trackEventsSub[eventCount+1].absoluteTime - trackEventsSub[eventCount].absoluteTime);
								if (trackEventsSub[eventCount].deltaTime > 0xFF000000)
								{
									int a =1;
								}
								trackEventCountSub++;
							}
						}
					}
				}

				
				unsigned long timeOffset = 0;

				unsigned long sizeData = 0;
				byte previousTrackEvent = 0x0;

				
				for (int j = 0; j < trackEventCountSub; j++)
				{
					TrackEvent trackEvent =  trackEventsSub[j];
					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;

						sizeData += lengthTimeDelta;


						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							sizeData += 1;
						}

						sizeData += trackEvent.contentSize;

						previousTrackEvent = trackEvent.type;
					}
				}

				tempLong = Flip32Bit(sizeData);
				fwrite(&tempLong,1, 4, outFile);

				timeOffset = 0;
				previousTrackEvent = 0x0;
				for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
				{
					TrackEvent trackEvent = trackEventsSub[eventCount];

					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;
						WriteVLBytes(outFile, timeDelta, lengthTimeDelta, true);

						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							fwrite(&trackEvent.type, 1, 1, outFile);
						}

						fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

						previousTrackEvent = trackEvent.type;
					}
				}
			}

			for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
			{
				if (trackEventsSub[eventCount].contents != NULL)
				{
					delete [] trackEventsSub[eventCount].contents;
					trackEventsSub[eventCount].contents = NULL;
				}
			}

			counterTrack++;

			delete [] trackEventsSub;
		}



	

		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

void CMidiParse::GEMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool extendTracksToHighest, bool usePitchBendSensitity, int pitchBendSensitity)
{
	numberInstruments = 0;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		unsigned long lengthHeader = 0x44;

		// parse midi
		
		int trackSize = 0;
		for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
		{
			unsigned long offset = CharArrayToLong(&inputMID[i]);
			if (offset != 0)
				trackSize++;
		}

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | trackSize);
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned long division = CharArrayToLong(&inputMID[0x40]);

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		int counterTrack = 0;

		int highestTrackLength = 0;

		for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			unsigned long absoluteTime = 0;

			unsigned long offset = CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			if (position != 0)
			{
				int previousEventValue = 0;

				std::map<int, int> loopEndsWithCount;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;

				while ((position < inputSize) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					absoluteTime += timeTag;

					if (absoluteTime > highestTrackLength)
						highestTrackLength = absoluteTime;


					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
							if ((loopCount == 0xFF) || (loopCount == 0x00))
							{
								break;
							}
							else
							{
								std::map<int, int>::iterator it = loopEndsWithCount.find(position);
								if (it != loopEndsWithCount.end())
								{
									int countLeft = it->second;

									if (countLeft == 0)
									{
										loopEndsWithCount.erase(it);
									}
									else
									{
										loopEndsWithCount[position] = (countLeft - 1);

										if (repeatPattern == NULL)
										{
											position = position - offsetToBeginningLoop;
										}
										else
										{
											loopEndsWithCount.erase(it);
										}
									}
								}
								else
								{
									loopEndsWithCount[position] = loopCount - 1;

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
						}
						else if (subType == 0x2F)
						{
							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						
					}
				}
			}
		}

		for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			unsigned long absoluteTime = 0;

			int trackEventCountSub = 0;
			TrackEvent* trackEventsSub = new TrackEvent[0x30000];

			for (int j = 0; j < 0x30000; j++)
			{
				trackEventsSub[j].contents = NULL;
				trackEventsSub[j].obsoleteEvent = false;
				trackEventsSub[j].deltaTime = 0;
				trackEventsSub[j].absoluteTime = 0;

			}

			unsigned long offset = CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			if (position != 0)
			{
				tempLong = Flip32Bit(0x4D54726B);
				fwrite(&tempLong, 1 ,4 , outFile);
	
				int previousEventValue = 0;

				std::map<int, int> loopEndsWithCount;
				std::vector<int> loopNumbers;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;

				if (usePitchBendSensitity)
				{
					//https://www.midikits.net/midi_analyser/pitch_bend.htm

					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x64;
					trackEventsSub[trackEventCountSub].contents[1] = 0x00;

					trackEventCountSub++;


					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x65;
					trackEventsSub[trackEventCountSub].contents[1] = 0x00;

					trackEventCountSub++;


					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x06;
					if (pitchBendSensitity > 0x18)
						pitchBendSensitity = 0x18;
					trackEventsSub[trackEventCountSub].contents[1] = pitchBendSensitity;

					trackEventCountSub++;

					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x64;
					trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

					trackEventCountSub++;


					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x65;
					trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

					trackEventCountSub++;
				}

				while ((position < inputSize) && !endFlag)
				{
					if (extendTracksToHighest)
					{
						if (absoluteTime >= highestTrackLength)
						{
							trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
							trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;

							break;
						}
					}


					if (trackEventCountSub >= 0x30000)
					{
						for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
						{
							if (trackEventsSub[eventCount].contents != NULL)
							{
								delete [] trackEventsSub[eventCount].contents;
								trackEventsSub[eventCount].contents = NULL;
							}
						}

						delete [] trackEventsSub;
						return;
					}

					int timePosition = position;

					unsigned long original;
					// trackEventsSub[trackEventCountSub].deltaTime is for loops
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					if (extendTracksToHighest)
					{
						if ((absoluteTime + timeTag) > highestTrackLength)
						{
							trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
							trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;

							break;
						}
					}

					trackEventsSub[trackEventCountSub].deltaTime += timeTag;

					absoluteTime += timeTag;
					trackEventsSub[trackEventCountSub].absoluteTime = absoluteTime;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 5;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x51;
							trackEventsSub[trackEventCountSub].contents[1] = 0x3;
							trackEventsSub[trackEventCountSub].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
							trackEventsSub[trackEventCountSub].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
							trackEventsSub[trackEventCountSub].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

							trackEventCountSub++;

							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							int loopNumber = 0;
							if (loopNumbers.size() > 0)
							{
								loopNumber = loopNumbers.back();
								loopNumbers.pop_back();
							}

							// Fake loop end, controller 103
							trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							trackEventsSub[trackEventCountSub].contents[0] = 103;
							trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
							trackEventCountSub++;

							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
							if ((loopCount == 0xFF) || (loopCount == 0x00))
							{
								hasLoopPoint = true;
								loopEnd = absoluteTime;

								if (extendTracksToHighest)
								{
									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
								}
							}
							else
							{
								std::map<int, int>::iterator it = loopEndsWithCount.find(position);
								if (it != loopEndsWithCount.end())
								{
									int countLeft = it->second;

									if (countLeft == 0)
									{
										loopEndsWithCount.erase(it);
									}
									else
									{
										loopEndsWithCount[position] = (countLeft - 1);

										if (repeatPattern == NULL)
										{
											position = position - offsetToBeginningLoop;
										}
										else
										{
											loopEndsWithCount.erase(it);
										}
									}
								}
								else
								{
									loopEndsWithCount[position] = loopCount - 1;

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
							hasLoopPoint = true;
							loopStart = absoluteTime;

							// Fake loop start, controller 102
							trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							trackEventsSub[trackEventCountSub].contents[0] = 102;
							trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
							trackEventCountSub++;



							loopNumbers.push_back(loopNumber);
						}
						else if (subType == 0x2F)
						{
							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							trackEventsSub[trackEventCountSub].type = previousEventValue;
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							trackEventsSub[trackEventCountSub].type = eventVal;
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						

						trackEventsSub[trackEventCountSub].durationTime = timeDuration; // to be filled in
						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = noteNumber;
						trackEventsSub[trackEventCountSub].contents[1] = velocity;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = controllerType;
						trackEventsSub[trackEventCountSub].contents[1] = controllerValue;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}

						trackEventsSub[trackEventCountSub].contentSize = 1;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = instrument;
						if (instrument >= numberInstruments)
							numberInstruments = (instrument + 1);

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}

						trackEventsSub[trackEventCountSub].contentSize = 1;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = amount;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = valueLSB;
						trackEventsSub[trackEventCountSub].contents[1] = valueMSB;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
					}

				}

				for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
				{
					if (trackEventCountSub >= 0x30000)
					{
						fclose(outFile);

						for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
						{
							if (trackEventsSub[eventCount].contents != NULL)
							{
								delete [] trackEventsSub[eventCount].contents;
								trackEventsSub[eventCount].contents = NULL;
							}
						}

						delete [] trackEventsSub;
						return;
					}

					TrackEvent trackEvent = trackEventsSub[eventCount];
					if ((trackEvent.type >= 0x90) && (trackEvent.type < 0xA0))
					{
						// need to split out
						if (trackEvent.durationTime > 0)
						{

							unsigned long shutoffTime = (trackEvent.absoluteTime + trackEvent.durationTime);

							if (eventCount != (trackEventCountSub - 1))
							{

								for (int i = (eventCount+1); i < trackEventCountSub; i++)
								{
									if ((trackEventsSub[i].absoluteTime >= shutoffTime) && (i != (trackEventCountSub - 1)))
									{
										for (int j = (trackEventCountSub - 1); j >= i; j--)
										{
											trackEventsSub[j+1].absoluteTime = trackEventsSub[j].absoluteTime;
											trackEventsSub[j+1].contentSize = trackEventsSub[j].contentSize;
											if (trackEventsSub[j+1].contents != NULL)
											{
												delete [] trackEventsSub[j+1].contents;
												trackEventsSub[j+1].contents = NULL;
											}
											trackEventsSub[j+1].contents = new byte[trackEventsSub[j].contentSize];
											for (int r = 0; r < trackEventsSub[j].contentSize; r++)
											{
												trackEventsSub[j+1].contents[r] = trackEventsSub[j].contents[r];
											}
											trackEventsSub[j+1].deltaTime = trackEventsSub[j].deltaTime;
											trackEventsSub[j+1].durationTime = trackEventsSub[j].durationTime;
											trackEventsSub[j+1].obsoleteEvent = trackEventsSub[j].obsoleteEvent;
											trackEventsSub[j+1].type = trackEventsSub[j].type;
										}

										trackEventsSub[i].type = trackEventsSub[eventCount].type;
										trackEventsSub[i].absoluteTime = shutoffTime;
										trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i-1].absoluteTime);
										trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
										trackEventsSub[i].durationTime = 0;


										if (trackEventsSub[i].contents != NULL)
										{
											delete [] trackEventsSub[i].contents;
										}

										trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
										trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
										trackEventsSub[i].contents[1] = 0;

										trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);

										if (trackEventsSub[i].deltaTime > 0xFF000000)
										{
											int a =1;
										}

										trackEventCountSub++;
										break;
									}
									else if (i == (trackEventCountSub - 1))
									{
										trackEventsSub[i+1].absoluteTime = shutoffTime; // move end to end
										trackEventsSub[i+1].contentSize = trackEventsSub[i].contentSize;
										if (trackEventsSub[i+1].contents != NULL)
										{
											delete [] trackEventsSub[i+1].contents;
											trackEventsSub[i+1].contents = NULL;
										}
										trackEventsSub[i+1].contents = new byte[trackEventsSub[i].contentSize];
										for (int r = 0; r < trackEventsSub[i].contentSize; r++)
										{
											trackEventsSub[i+1].contents[r] = trackEventsSub[i].contents[r];
										}
										trackEventsSub[i+1].deltaTime = trackEventsSub[i].deltaTime;
										trackEventsSub[i+1].durationTime = trackEventsSub[i].durationTime;
										trackEventsSub[i+1].obsoleteEvent = trackEventsSub[i].obsoleteEvent;
										trackEventsSub[i+1].type = trackEventsSub[i].type;


										trackEventsSub[i].type = trackEventsSub[eventCount].type;
										trackEventsSub[i].absoluteTime = shutoffTime;
										trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i - 1].absoluteTime);
										trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
										trackEventsSub[i].durationTime = 0;

										if (trackEventsSub[i].contents != NULL)
										{
											delete [] trackEventsSub[i].contents;
										}

										trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
										trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
										trackEventsSub[i].contents[1] = 0;

										trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);

										trackEventCountSub++;
										break;
									}
								}
							}
							else
							{
								trackEventsSub[eventCount+1].absoluteTime = shutoffTime; // move end to end
								trackEventsSub[eventCount+1].contentSize = trackEventsSub[eventCount].contentSize;
								if (trackEventsSub[eventCount+1].contents != NULL)
								{
									delete [] trackEventsSub[eventCount+1].contents;
									trackEventsSub[eventCount+1].contents = NULL;
								}
								trackEventsSub[eventCount+1].contents = new byte[trackEventsSub[eventCount].contentSize];
								for (int r = 0; r < trackEventsSub[eventCount].contentSize; r++)
								{
									trackEventsSub[eventCount+1].contents[r] = trackEventsSub[eventCount].contents[r];
								}
								trackEventsSub[eventCount+1].deltaTime = trackEventsSub[eventCount].deltaTime;
								trackEventsSub[eventCount+1].durationTime = trackEventsSub[eventCount].durationTime;
								trackEventsSub[eventCount+1].obsoleteEvent = trackEventsSub[eventCount].obsoleteEvent;
								trackEventsSub[eventCount+1].type = trackEventsSub[eventCount].type;


								trackEventsSub[eventCount].type = trackEventsSub[eventCount].type;
								trackEventsSub[eventCount].absoluteTime = shutoffTime;
								if ((trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime) > 0xFF000000)
								{
									int a =1;
								}
								trackEventsSub[eventCount].deltaTime = (trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime);
								trackEventsSub[eventCount].contentSize = trackEventsSub[eventCount].contentSize;
								trackEventsSub[eventCount].durationTime = 0;
								trackEventsSub[eventCount].contents = new byte[trackEventsSub[eventCount].contentSize];
								trackEventsSub[eventCount].contents[0] = trackEventsSub[eventCount].contents[0];
								trackEventsSub[eventCount].contents[1] = 0;

								trackEventsSub[eventCount+1].deltaTime = (trackEventsSub[eventCount+1].absoluteTime - trackEventsSub[eventCount].absoluteTime);
								if (trackEventsSub[eventCount].deltaTime > 0xFF000000)
								{
									int a =1;
								}
								trackEventCountSub++;
							}
						}
					}
				}

				
				unsigned long timeOffset = 0;

				unsigned long sizeData = 0;
				byte previousTrackEvent = 0x0;

				
				for (int j = 0; j < trackEventCountSub; j++)
				{
					TrackEvent trackEvent =  trackEventsSub[j];
					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;

						sizeData += lengthTimeDelta;


						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							sizeData += 1;
						}

						sizeData += trackEvent.contentSize;

						previousTrackEvent = trackEvent.type;
					}
				}

				tempLong = Flip32Bit(sizeData);
				fwrite(&tempLong,1, 4, outFile);

				timeOffset = 0;
				previousTrackEvent = 0x0;
				for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
				{
					TrackEvent trackEvent = trackEventsSub[eventCount];

					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;
						WriteVLBytes(outFile, timeDelta, lengthTimeDelta, true);

						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							fwrite(&trackEvent.type, 1, 1, outFile);
						}

						fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

						previousTrackEvent = trackEvent.type;
					}
				}
			}
			else
			{
				
			}

			for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
			{
				if (trackEventsSub[eventCount].contents != NULL)
				{
					delete [] trackEventsSub[eventCount].contents;
					trackEventsSub[eventCount].contents = NULL;
				}
			}

			counterTrack++;

			delete [] trackEventsSub;
		}



	

		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

void CMidiParse::MIDxMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments)
{
	numberInstruments = 1;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		// parse midi
		
		int trackSize = inputSize;

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | 0x0001); // num tracks
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned long division = 0x01E0;

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		unsigned long absoluteTime = 0;

		int trackEventCount = 0;
		TrackEvent* trackEvents = new TrackEvent[0x30000];

		for (int j = 0; j < 0x30000; j++)
			trackEvents[j].contents = NULL;

		int position = 4;	

		tempLong = Flip32Bit(0x4D54726B);
		fwrite(&tempLong, 1 ,4 , outFile);

		byte* repeatPattern = NULL;
		byte altOffset = 0;
		byte altLength = 0;

		bool endFlag = false;
		while ((position < inputSize) && !endFlag)
		{
			int timePosition = position;

			unsigned long original;
			unsigned long timeTag;
			timeTag = inputMID[position];
			position++;
			if (timeTag > 0x7F)
				timeTag = (timeTag & 0x7F);
			
			trackEvents[trackEventCount].deltaTime = timeTag;
			trackEvents[trackEventCount].obsoleteEvent = false;

			absoluteTime += timeTag;
			trackEvents[trackEventCount].absoluteTime = absoluteTime;

			int vlLength = 0;
			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			if ((eventVal >= 0xF0) && (eventVal <= 0xFF)) // meta event
			{
				byte  subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x51) // tempo
				{
					int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));

					trackEvents[trackEventCount].type = 0xFF;
					trackEvents[trackEventCount].contentSize = 5;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					
					trackEvents[trackEventCount].contents[0] = 0x51;
					trackEvents[trackEventCount].contents[1] = 0x3;
					trackEvents[trackEventCount].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
					trackEvents[trackEventCount].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
					trackEvents[trackEventCount].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

					trackEventCount++;

					
					int MICROSECONDS_PER_MINUTE = 60000000;
					float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
				}
				else if (subType == 0x2D) // end loop
				{
					byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
				}
				else if (subType == 0x2E) // start loop
				{
					byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					if (endLoop != 0xFF)
					{
						// is this used?
					}
				}
				else if (subType == 0x2F)
				{
					trackEvents[trackEventCount].type = 0xFF;
					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					
					trackEvents[trackEventCount].contents[0] = 0x2F;
					trackEvents[trackEventCount].contents[1] = 0x0;

					trackEventCount++;

					endFlag = true;
				}
				else if (subType == 0xF8)
				{
					// ?
				}
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90))
			{
				byte curEventVal;

				byte noteNumber;
				
				trackEvents[trackEventCount].type = eventVal;
				noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				curEventVal = eventVal;

				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);


				trackEvents[trackEventCount].durationTime = 0; // to be filled in
				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = noteNumber;
				trackEvents[trackEventCount].contents[1] = velocity;

				trackEventCount++;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0))
			{
				byte curEventVal;

				byte noteNumber;
				
				trackEvents[trackEventCount].type = eventVal;
				noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				curEventVal = eventVal;

				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);


				trackEvents[trackEventCount].durationTime = 0; // to be filled in
				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = noteNumber;
				trackEvents[trackEventCount].contents[1] = velocity;

				trackEventCount++;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))) // controller change
			{
				CString controllerTypeText = "";
				byte controllerType;
				
				controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = controllerType;
				trackEvents[trackEventCount].contents[1] = controllerValue;

				trackEventCount++;

			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0))) // change instrument
			{
				byte instrument;
				
				instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;

				trackEvents[trackEventCount].contentSize = 1;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = instrument;
				if (instrument >= numberInstruments)
					numberInstruments = (instrument + 1);

				trackEventCount++;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))) // channel aftertouch
			{
				byte amount;
				
				amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;

				trackEvents[trackEventCount].contentSize = 1;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = amount;

				trackEventCount++;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))) // pitch bend
			{
				byte valueLSB;
				
				valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				trackEvents[trackEventCount].type = eventVal;
				
				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = valueLSB;
				trackEvents[trackEventCount].contents[1] = valueMSB;

				trackEventCount++;
			}
			else if (eventVal == 0xFE) // repeat operation
			{
				// should not be here...

				// no prev event set
			}
			else
			{
				fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
			}
		}

		
		unsigned long timeOffset = 0;

		unsigned long sizeData = 0;
		byte previousTrackEvent = 0x0;

		
		for (int j = 0; j < trackEventCount; j++)
		{
			TrackEvent trackEvent =  trackEvents[j];
			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;

				sizeData += lengthTimeDelta;


				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
				{
					sizeData += 1;
				}

				sizeData += trackEvent.contentSize;

				previousTrackEvent = trackEvent.type;
			}
		}

		tempLong = Flip32Bit(sizeData);
		fwrite(&tempLong,1, 4, outFile);

		timeOffset = 0;
		previousTrackEvent = 0x0;
		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			TrackEvent trackEvent = trackEvents[eventCount];

			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;
				WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
				{
					fwrite(&trackEvent.type, 1, 1, outFile);
				}

				fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

				previousTrackEvent = trackEvent.type;
			}
		}

		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			if (trackEvents[eventCount].contents != NULL)
			{
				delete [] trackEvents[eventCount].contents;
				trackEvents[eventCount].contents = NULL;
			}
		}

		delete [] trackEvents;

		// just one track
		
		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

int CMidiParse::GetSngVariableLength(unsigned char* buffer, unsigned long offset)
{
	if (buffer[offset] & 0x80)
	{
		int value = (((buffer[offset] & 0x7F) << 8) | buffer[offset + 1]);
		return value;
	}
	else
	{
		return buffer[offset];
	}
}

int CMidiParse::WriteSngVariableLength(unsigned char* buffer, int& offset, int value)
{
	int writeValue = value;

	while (writeValue > 0x7FFE)
	{
		writeValue -= 0x7FFE;
	}

	if (writeValue < 0x80)
		buffer[offset++] = writeValue;
	else
	{
		unsigned short dataValue = 0x8000 | writeValue;
		buffer[offset++] = ((dataValue >> 8) & 0xFF);
		buffer[offset++] = ((dataValue) & 0xFF);
	}

	return (value - writeValue);
}

void CMidiParse::ParseSngTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, unsigned short* instrumentLookup, unsigned long adsrPointer, unsigned long drumPointer, std::vector<SngTimeValue> volumeByAbsoluteTime, std::vector<SngTimeValue> pitchBendByAbsoluteTime, SngStyle sngStyle, int& noteUniqueId, int totalInstruments)
{
	std::vector<SngNoteInfo> trackOutputNotes;
	unsigned char command = 0x00;
	unsigned long spot = offset;

	bool useDefinedLength = false;
	int definedLength = 0;

	bool useDefinedVelocity = false;
	int definedVelocity = 0x7F;

	bool useDefinedReleaseTime = false;
	int definedReleaseTime = 0x01;

	if ((sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::Bfx) || (sngStyle == SngStyle::PtrBfx))
	{
		useDefinedVelocity = true;
		definedVelocity = 0x7F;
	}


	unsigned long absoluteTime = 0;

	unsigned char currentPan = 0x80;
	unsigned short currentInstrument = 0x00;
	unsigned char currentVolume = 0x7F;
	signed char currentPitchBend = 0x40;
	int currentDrumOffset = -1;

	int currentTranspose = 0;
	int currentEffect = 0;

	bool isWobbling = false;
	unsigned char wobbleOffset = 0x00;
	unsigned char wobbleTransposeCounter = 0x00;
	unsigned char wobbleNormalCounter = 0x00;

	unsigned long currentTempo = (unsigned long)(60000000.0 / (float)120.0);

	while ((command != 0x80) && (spot < end))
	{
		// Not Master Track
		if (trackNumber >= 0)
		{
			currentTempo = (unsigned long)(60000000.0 / (float)120.0);

			// Mini not using tempo?
			if (sngStyle == SngStyle::Normal || ((sngStyle == SngStyle::Old) && (trackNumber > 0)) || ((sngStyle == SngStyle::HexenSng) && (trackNumber > 0)) || ((sngStyle == SngStyle::OldDD) && (trackNumber > 0)))
			{
				for (int y = 0; y < tempoPositions.size(); y++)
				{
					if (tempoPositions[y].absoluteTime <= absoluteTime)
					{
						currentTempo = tempoPositions[y].value;
					}
					else
					{
						break;
					}
				}
			}
		}

		command = buffer[spot];
		
		//fprintf(outFile, "%08X Time: %08X Command: %02X ", spot, absoluteTime, command);
		spot++;

		if (command < 0x80) // Note or Rest
		{
			unsigned char note = command;

			if (note == 0x60)
			{
				//fprintf(outFile, " Rest");
			}
			else if (note < 0x60)
			{
				//fprintf(outFile, " Note %02X", note);
			}
			else
			{
				//fprintf(outFile, " UNKNOWN Note %02X", note);
			}

			int velocity;
			if (useDefinedVelocity)
			{
				velocity = definedVelocity;
				//fprintf(outFile, " [Velocity %02X (%d)]", definedVelocity, definedVelocity);
			}
			else
			{
				//fprintf(outFile, " Velocity %02X (%d)", buffer[spot] & 0x7F, buffer[spot] & 0x7F);

				if (buffer[spot] & 0x80)
				{
					useDefinedVelocity = true;
					definedVelocity = buffer[spot] & 0x7F;
					//fprintf(outFile, " and Repeat");
				}

				velocity = (buffer[spot] & 0x7F);

				spot++;
			}

			int noteLength;

			if (useDefinedLength)
			{
				noteLength = definedLength;

				//fprintf(outFile, " [Length %02X (%d)]", definedLength, definedLength);
			}
			else
			{
				noteLength = GetSngVariableLength(buffer, spot);
				
				if (noteLength < 0x80)
				{
					//fprintf(outFile, " Length %02X (%d)", buffer[spot], length);
					spot++;
				}
				else
				{
					//fprintf(outFile, " Length %02X%02X (%d)", buffer[spot], buffer[spot+1], length);
					spot += 2;
				}
			}

			// Add note to batch
			
			if (note < 0x60)
			{
				if (currentDrumOffset != -1)
				{
					SngNoteInfo songNoteInfo;
					songNoteInfo.originalTrack = trackNumber;
					songNoteInfo.startAbsoluteTime = absoluteTime;
					songNoteInfo.originalNoteUniqueId = noteUniqueId++;

					unsigned long drumNoteOffset = drumPointer + (currentDrumOffset * 0x6) + (note * 0x6);

					unsigned char instrumentIndex = buffer[drumNoteOffset + 1];
					unsigned char adsrIndex = buffer[drumNoteOffset + 3];
					unsigned char pan = buffer[drumNoteOffset + 4];
					unsigned char note = buffer[drumNoteOffset + 5];

					int instrument;
					if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng) || (sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::PtrBfx))
					{
						instrument = instrumentIndex;
						if (instrument > numberInstruments)
							numberInstruments = instrument + 1;
					}
					else
						instrument = instrumentLookup[instrumentIndex];

					songNoteInfo.effect = currentEffect;

					songNoteInfo.noteNumber = note + 0xC + currentTranspose;
					songNoteInfo.velocity = velocity;

					// From Master Track
					songNoteInfo.tempo = currentTempo;

					// TODO multiple banks
					songNoteInfo.instrument = instrument;
					
					songNoteInfo.pan = ((pan >> 1) & 0xFF);

					songNoteInfo.pitchBend = currentPitchBend;
					songNoteInfo.volume = currentVolume;

					if (useDefinedReleaseTime)
					{
						songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + definedReleaseTime;
					}
					else
					{
						songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;
					}

					trackOutputNotes.push_back(songNoteInfo);
				}
				else
				{
					if (isWobbling)
					{
						int totalLengthTime;
						if (useDefinedReleaseTime)
						{
							totalLengthTime = definedReleaseTime;
						}
						else
						{
							totalLengthTime = noteLength;
						}

						int wobbleCounter = 0;
						while (wobbleCounter < totalLengthTime)
						{
							int left = totalLengthTime - (int)wobbleCounter;
							if (left <= 0)
								break;

							if (left > wobbleTransposeCounter)
								left = wobbleTransposeCounter;

							SngNoteInfo songNoteInfoWobble;
							songNoteInfoWobble.originalTrack = trackNumber;
							songNoteInfoWobble.originalNoteUniqueId = noteUniqueId++;
							songNoteInfoWobble.startAbsoluteTime = absoluteTime + wobbleCounter;
							songNoteInfoWobble.endAbsoluteTime = songNoteInfoWobble.startAbsoluteTime + left;
							songNoteInfoWobble.noteNumber = note + 0xC + currentTranspose + wobbleOffset;
							songNoteInfoWobble.velocity = velocity;

							songNoteInfoWobble.effect = currentEffect;

							// From Master Track
							songNoteInfoWobble.tempo = currentTempo;

							songNoteInfoWobble.instrument = currentInstrument;
							
							songNoteInfoWobble.pan = ((currentPan >> 1) & 0xFF);;

							songNoteInfoWobble.pitchBend = currentPitchBend;
							songNoteInfoWobble.volume = currentVolume;

							trackOutputNotes.push_back(songNoteInfoWobble);

							wobbleCounter += left;



							left = totalLengthTime - (int)wobbleCounter;
							if (left <= 0)
								break;

							if (left > wobbleNormalCounter)
								left = wobbleNormalCounter;



							SngNoteInfo songNoteInfoNormal;
							songNoteInfoNormal.originalTrack = trackNumber;
							songNoteInfoNormal.originalNoteUniqueId = noteUniqueId++;
							songNoteInfoNormal.startAbsoluteTime = absoluteTime + wobbleCounter;
							songNoteInfoNormal.endAbsoluteTime = songNoteInfoNormal.startAbsoluteTime + left;
							songNoteInfoNormal.noteNumber = note + 0xC + currentTranspose;
							songNoteInfoNormal.velocity = velocity;

							songNoteInfoNormal.effect = currentEffect;

							// From Master Track
							songNoteInfoNormal.tempo = currentTempo;

							songNoteInfoNormal.instrument = currentInstrument;
							
							songNoteInfoNormal.pan = ((currentPan >> 1) & 0xFF);;

							songNoteInfoNormal.pitchBend = currentPitchBend;
							songNoteInfoNormal.volume = currentVolume;

							trackOutputNotes.push_back(songNoteInfoNormal);

							wobbleCounter += left;
						}
					}
					else
					{
						SngNoteInfo songNoteInfo;
						songNoteInfo.originalTrack = trackNumber;
						songNoteInfo.originalNoteUniqueId = noteUniqueId++;
						songNoteInfo.startAbsoluteTime = absoluteTime;
						songNoteInfo.noteNumber = note + 0xC + currentTranspose;
						songNoteInfo.velocity = velocity;

						songNoteInfo.effect = currentEffect;

						// From Master Track
						songNoteInfo.tempo = currentTempo;

						songNoteInfo.instrument = currentInstrument;
						
						songNoteInfo.pan = ((currentPan >> 1) & 0xFF);;

						songNoteInfo.pitchBend = currentPitchBend;
						songNoteInfo.volume = currentVolume;

						if (useDefinedReleaseTime)
						{
							songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + definedReleaseTime;
						}
						else
						{
							songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;
						}

						trackOutputNotes.push_back(songNoteInfo);
					}
				}
			}

			absoluteTime += noteLength;
		}
		else
		{
			if (command == 0x80)
			{
				//End?
				//fprintf(outFile, " End");
				break;
			}
			else if (command == 0x81)
			{
				//XX, Instrument # In List
				//fprintf(outFile, " Instrument");
				//fprintf(outFile, " %02X (Looked up %02X)", buffer[spot], instrumentLookup[buffer[spot]]);

				if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng) || (sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::PtrBfx))
				{
					currentInstrument = GetSngVariableLength(buffer, spot);
					if (currentInstrument > numberInstruments)
						numberInstruments = currentInstrument + 1;

					if (currentInstrument < 0x80)
					{
						spot++;
					}
					else
					{
						spot += 2;
					}
				}
				else if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Bfx) || (sngStyle == SngStyle::OldDD))
				{
					int instrumentLookupValue = GetSngVariableLength(buffer, spot);

					if (instrumentLookupValue < 0x80)
					{
						spot++;
					}
					else
					{
						spot += 2;
					}

					if (instrumentLookupValue >= totalInstruments)
					{
						currentInstrument = instrumentLookupValue;
					}
					else
					{
						currentInstrument = instrumentLookup[instrumentLookupValue];
					}
				}
			}
			else if (command == 0x82)
			{
				if ((sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::Bfx) || (sngStyle == SngStyle::PtrBfx))
				{
					//??
					//fprintf(outFile, " ?");
				}
				else
				{
					//Set Slide
					//fprintf(outFile, " Set Slide");
					//fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
			}
			else if (command == 0x83)
			{
				// ?
			}
			else if (command == 0x84)
			{
				// ADSR Set
				spot += 7;
			}
			else if (command == 0x85)
			{
				//XX Master Track Tempo, 0x28 = 40, 0x78 = 120
				//fprintf(outFile, " Master Track Tempo");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				unsigned char tempo = buffer[spot];

				currentTempo = (unsigned long)(60000000.0 / (float)tempo);

				if (((sngStyle == SngStyle::Normal) && (trackNumber == -1)) || ((sngStyle == SngStyle::Old) && (trackNumber == 0))  || ((sngStyle == SngStyle::HexenSng) && (trackNumber == 0)) || ((sngStyle == SngStyle::OldDD) && (trackNumber == 0)))// Master Track
				{
					tempoPositions.push_back(TimeAndValue(absoluteTime, currentTempo));
				}

				spot++;
			}
			else if (command == 0x86)
			{
				//XXXX Set Release Time for upcoming notes
				//fprintf(outFile, " Set Release Time");
				//fprintf(outFile, " %04X (%d)", CharArrayToShort(&buffer[spot]), CharArrayToShort(&buffer[spot]));

				useDefinedReleaseTime = true;
				definedReleaseTime = CharArrayToShort(&buffer[spot]);

				spot+=2;
			}
			else if (command == 0x87)
			{
				//XX 01 Set Release Time back to full note (01)
				//fprintf(outFile, " Set Release Time Back to Full");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				useDefinedReleaseTime = false;

				spot++;
			}
			else if (command == 0x88)
			{
				//XX YY ZZ Vibrato Up - Delay, Amount, Speed
				//fprintf(outFile, " Vibrato Up");
				//fprintf(outFile, " Delay %02X (%d) Amount %02X (%d) Speed %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot+=3;
			}
			else if (command == 0x89)
			{
				//XX YY ZZ Vibrato Down - Delay, Amount, Speed
				//fprintf(outFile, " Vibrato Down");
				//fprintf(outFile, " Delay %02X (%d) Amount %02X (%d) Speed %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot+=3;
			}
			else if (command == 0x89)
			{
				//?
			}
			else if (command == 0x8A)
			{
				//Solo
				//fprintf(outFile, " Solo");
			}
			else if (command == 0x8B)
			{
				//LL Set Length notes following and disable in list (variable time)
				//fprintf(outFile, " Set Length Notes and Disable in Note Format");
				
				definedLength = GetSngVariableLength(buffer, spot);

				// Probably just old style, uses 0x00 to turn off?
				if (definedLength == 0x00)
				{
					useDefinedLength = false;
					spot++;
				}
				else if (definedLength < 0x80)
				{
					useDefinedLength = true;
					//fprintf(outFile, " Length %02X (%d)", buffer[spot], definedLength);
					spot++;
				}
				else
				{
					useDefinedLength = true;
					//fprintf(outFile, " Length %02X%02X (%d)", buffer[spot], buffer[spot+1], definedLength);
					spot += 2;
				}
			}
			else if (command == 0x8D)
			{
				//XX Transpose, 00 default, 01, etc
				//fprintf(outFile, " Transpose");
				//fprintf(outFile, " %02X (%d)", buffer[spot], (signed char)buffer[spot]);

				currentTranspose = (signed char)buffer[spot];

				spot++;
			}
			else if (command == 0x8F)
			{
				//XX Detune (signed)
				//fprintf(outFile, " Detune");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0x90)
			{
				//XX ADSR SubIndex
				//fprintf(outFile, " ADSR Index");
				//fprintf(outFile, " %02X", buffer[spot]);
				//fprintf(outFile, " Rate %02X (%d)", buffer[adsrPointer + buffer[spot] * 7], buffer[adsrPointer + buffer[spot] * 7]);
				//fprintf(outFile, " Start Level %02X (%d)", buffer[adsrPointer + buffer[spot] * 7 + 1], buffer[adsrPointer + buffer[spot] * 7 + 1]);
				//fprintf(outFile, " Attack Rate %02X (%d)", buffer[adsrPointer + buffer[spot] * 7 + 2], buffer[adsrPointer + buffer[spot] * 7 + 2]);
				//fprintf(outFile, " Peak Level %02X (%d)", buffer[adsrPointer + buffer[spot] * 7 + 3], buffer[adsrPointer + buffer[spot] * 7 + 3]);
				//fprintf(outFile, " Decay Rate %02X (%d)", buffer[adsrPointer + buffer[spot] * 7 + 4], buffer[adsrPointer + buffer[spot] * 7 + 4]);
				//fprintf(outFile, " Sustain Level %02X (%d)", buffer[adsrPointer + buffer[spot] * 7 + 5], buffer[adsrPointer + buffer[spot] * 7 + 5]);
				//fprintf(outFile, " Release Rate %02X (%d)", buffer[adsrPointer + buffer[spot] * 7 + 6], buffer[adsrPointer + buffer[spot] * 7 + 6]);
				spot++;
			}
			else if (command == 0x91)
			{
				//Envelope Trigger Off
				//fprintf(outFile, " Envelope Trigger Off");
			}
			else if (command == 0x92)
			{
				//Envelope Trigger On
				//fprintf(outFile, " Envelope Trigger On");
			}
			else if (command == 0x93)
			{
				//Sample Trigger Off
				//fprintf(outFile, " Sample Trigger Off");
			}
			else if (command == 0x94)
			{
				//Sample Trigger On
				//fprintf(outFile, " Sample Trigger On");
			}
			else if (command == 0x95)
			{
				//XX Loop Start FF Count?
				//fprintf(outFile, " Loop Start");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0x96)
			{
				//Loop
				//fprintf(outFile, " Loop Back");
				break;
			}
			else if (command == 0x97)
			{
				//XX YY ZZ Wobble - Offset, Transpose, Normal
				//fprintf(outFile, " Wobble");
				//fprintf(outFile, " Offset %02X (%d) Transpose %02X (%d) Normal %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				
				wobbleOffset = buffer[spot];
				wobbleTransposeCounter = buffer[spot + 1];
				wobbleNormalCounter = buffer[spot + 2];

				/*if ((wobbleOffset != 0) && ((wobbleTransposeCounter != 0) || (wobbleNormalCounter != 0)))
					isWobbling = true;
				else
					isWobbling = false;*/

				// Disable for now
				isWobbling = false;

				spot+=3;
			}
			else if (command == 0x98)
			{
				//?
			}
			else if (command == 0x99)
			{
				useDefinedVelocity = false;
				//Reenable velocity on notes
				//fprintf(outFile, " Reenable Velocity on Notes Format");
			}
			else if (command == 0x9A) // Old Style Only
			{
				// Enable Velocity Defined in Notes
				useDefinedVelocity = true;
				//fprintf(outFile, " Enable Velocity Defined in Notes");
			}
			else if (command == 0x9B) // Old Style Only
			{
				// Set Repeat Velocity
				//fprintf(outFile, " Set Repeat Velocity");
				//fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				definedVelocity = buffer[spot];
				useDefinedVelocity = true;
				spot++;
			}
			else if (command == 0x9C)
			{
				//XX Pan, 0x00
				//fprintf(outFile, " Pan");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				currentPan = buffer[spot];

				spot++;
			}
			else if (command == 0x9E)
			{
				//XX Drum map on
				//fprintf(outFile, " Drum Map On");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				currentDrumOffset = buffer[spot];
				spot++;
			}
			else if (command == 0x9F)
			{
				//Drum map off
				//fprintf(outFile, " Drum Map Off");

				currentDrumOffset = -1;
			}
			else if (command == 0xA2)
			{
				//XX Effect, 0x00
				//fprintf(outFile, " Effect");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				currentEffect = buffer[spot];
				spot++;
			}
			else if (command == 0xA3)
			{
				//XX YY Set Min/Max Transpose
				//fprintf(outFile, " Set Min/Max Transpose");
				//fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command == 0xA4)
			{
				//XX YY Set Min/Max Volume
				//fprintf(outFile, " Set Min/Max Volume");
				//fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command == 0xA5)
			{
				//XX YY Set Min/Max Pan
				//fprintf(outFile, " Set Min/Max Pan");
				//fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command == 0xA6)
			{
				// Set Note Volume
				//fprintf(outFile, " ?");
				currentVolume = buffer[spot];
				spot++;
			}
			else if (command == 0xA7)
			{
				//LL ?
				//fprintf(outFile, " Load Track #");
				//fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);

				int length = GetSngVariableLength(buffer, spot);

				if (length < 0x80)
				{
					//fprintf(outFile, " Length %02X (%d)\n", inputMID[spot], length);
					spot++;
				}
				else
				{
					//fprintf(outFile, " Length %02X%02X (%d)\n", inputMID[spot], inputMID[spot+1], length);
					spot += 2;
				}
			}
			else if (command == 0xA8)
			{
				//XX Pitch Bend
				//fprintf(outFile, " Pitch Bend");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				currentPitchBend = buffer[spot];
				spot++;
			}
			else if (command == 0xA9)
			{
				//XX Stereo Sweep Speed
				//fprintf(outFile, " Stereo Sweep Speed");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xAA)
			{
				//XX Master Track Effect 00 = None, 01 = Small Room, 02 = Big Room, 03 = Chorus, 04 = Flange, 05 = Echo
				//fprintf(outFile, " Master Track Effect");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				if (buffer[spot] == 0)
				{
					//fprintf(outFile, "None");
				}
				else if (buffer[spot] == 1)
				{
					//fprintf(outFile, "Small Room");
				}
				else if (buffer[spot] == 2)
				{
					//fprintf(outFile, "Big Room");
				}
				else if (buffer[spot] == 3)
				{
					//fprintf(outFile, "Chorus");
				}
				else if (buffer[spot] == 4)
				{
					//fprintf(outFile, "Flange");
				}
				else if (buffer[spot] == 5)
				{
					//fprintf(outFile, "Echo");
				}
				spot++;
			}
			else if (command == 0xAB)
			{
				//XX Master Track Sync - 1
				//fprintf(outFile, " Master Track Sync");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xAC)
			{
				// Reenable length on notes
				useDefinedLength = false;
				//fprintf(outFile, " Reenable length on notes");
			}
			else if (command == 0xB0)
			{
				//XX ?
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else
			{
				CString tempStr;
				tempStr.Format("Unknown Command %02X at Spot %04X", command, spot);
				MessageBox(NULL, tempStr, "Error", NULL);

				//fprintf(outFile, " UNKNOWN");
				return;
			}
		}

		//fprintf(outFile, "\n");
	}



	// Add in Volume
	for (int x = 0; x < trackOutputNotes.size(); x++)
	{
		for (int y = 0; y < volumeByAbsoluteTime.size(); y++)
		{
			// Volume To Left/Equal Ending in Middle
			if (
				(trackOutputNotes[x].startAbsoluteTime >= volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (trackOutputNotes[x].volume != volumeByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = trackOutputNotes[x];

					trackOutputNotes[x].startAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.volume = volumeByAbsoluteTime[y].value;

					trackOutputNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//Volume In Middle, Ending to Right
			else if (
				(trackOutputNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime <= volumeByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (trackOutputNotes[x].volume != volumeByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = trackOutputNotes[x];

					trackOutputNotes[x].endAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.volume = volumeByAbsoluteTime[y].value;

					trackOutputNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// Volume Completely Inside
			else if (
				(trackOutputNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (trackOutputNotes[x].volume != volumeByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = trackOutputNotes[x];
					SngNoteInfo newNoteInfo2 = trackOutputNotes[x];

					trackOutputNotes[x].endAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.volume = volumeByAbsoluteTime[y].value;

					trackOutputNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;

					trackOutputNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// Volume Completely Outside
			else if (
				(trackOutputNotes[x].startAbsoluteTime >= volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime <= volumeByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				trackOutputNotes[x].volume = volumeByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}
	


	// Add in Pitch Bend
	for (int x = 0; x < trackOutputNotes.size(); x++)
	{
		for (int y = 0; y < pitchBendByAbsoluteTime.size(); y++)
		{
			// PitchBend To Left/Equal Ending in Middle
			if (
				(trackOutputNotes[x].startAbsoluteTime >= pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (trackOutputNotes[x].pitchBend != pitchBendByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = trackOutputNotes[x];

					trackOutputNotes[x].startAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.pitchBend = pitchBendByAbsoluteTime[y].value;

					trackOutputNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//PitchBend In Middle, Ending to Right
			else if (
				(trackOutputNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime <= pitchBendByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (trackOutputNotes[x].pitchBend != pitchBendByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = trackOutputNotes[x];

					trackOutputNotes[x].endAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.pitchBend = pitchBendByAbsoluteTime[y].value;

					trackOutputNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// PitchBend Completely Inside
			else if (
				(trackOutputNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (trackOutputNotes[x].pitchBend != pitchBendByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = trackOutputNotes[x];
					SngNoteInfo newNoteInfo2 = trackOutputNotes[x];

					trackOutputNotes[x].endAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.pitchBend = pitchBendByAbsoluteTime[y].value;

					trackOutputNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;

					trackOutputNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// PitchBend Completely Outside
			else if (
				(trackOutputNotes[x].startAbsoluteTime >= pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (trackOutputNotes[x].endAbsoluteTime <= pitchBendByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				trackOutputNotes[x].pitchBend = pitchBendByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}

	// Add to end
	for (int x = 0; x < trackOutputNotes.size(); x++)
	{
		outputNotes.push_back(trackOutputNotes[x]);
	}

}

void CMidiParse::SngToDebugTextFile(CString gameName, unsigned long address, CString midiFile, CString textFileOut, unsigned long extra)
{
	CString filepath = midiFile;
	
	FILE* inFile = fopen(filepath, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Can't read input file " + filepath, "Error", NULL);
		return;
	}

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	rewind(inFile);

	unsigned char* inputMID = new unsigned char[inputSize];

	fread(inputMID, 1, inputSize, inFile);
	fclose(inFile);

	SngToDebugTextFile(gameName, address, inputMID, inputSize, textFileOut, extra);

	delete [] inputMID;
}

void CMidiParse::SngToDebugTextFile(CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, unsigned long extra)
{	
	CString addressStr;
	addressStr.Format("%08X", address);

	if (inputMID[0x0] == 0x80) // Ptr Sng
	{
		unsigned long startDataNegative = (signed long)inputSize;
		unsigned long initialPointer = extra;

		unsigned long numberTracks = 0;
		
		int pointerSpot = 0;
		while (inputMID[pointerSpot] == 0x80)
		{
			numberTracks++;
			pointerSpot += 4;
		}


		unsigned long adsrPointer = 0x00000000;
		unsigned long drumPointer = 0x00000000;

		unsigned long instrumentPointer = 0x00000000;
		
		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
			return;
		}

		fprintf(outFile, gameName + " - " + addressStr + "\n");
		fprintf(outFile, "Ptr BFX Song Style\n\n");

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToLong(&inputMID[(x * 4)]) - initialPointer;
			
			unsigned long trackEnd = 0x00000000;
			if (x < (numberTracks - 1))
			{
				trackEnd = CharArrayToLong(&inputMID[((x + 1) * 4)]) - initialPointer;
			}
			else
			{
				trackEnd = address;
			}


			fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X (%08X)\n------------------------------------------------------\n", x + 1, trackDataPointer + address + (signed long)inputSize, CharArrayToLong(&inputMID[(x * 4)]));

			fprintf(outFile, "\nTrack Data\n");

			SngTrackToDebugTextFile(outFile, inputMID + (signed long)startDataNegative, trackDataPointer, trackEnd, instrumentPointer, adsrPointer, drumPointer, SngStyle::PtrBfx, -1);
		}

		fclose(outFile);
	}
	// Second check to avoid false positive of bfx, if count is 0x215, Mario Golf E
	else if ((CharArrayToLong(&inputMID[0x0]) == 0x00000215) && (CharArrayToLong(&inputMID[0x4]) != 0x00000215)) // Binary Sng style
	{
		unsigned long numberTracks = CharArrayToLong(&inputMID[0x4]);
		unsigned long totalInstruments = CharArrayToLong(&inputMID[0x8]);
		unsigned long trackPointer = CharArrayToLong(&inputMID[0xC]);
		unsigned long volumePointer = CharArrayToLong(&inputMID[0x10]);
		unsigned long pitchBendPointer = CharArrayToLong(&inputMID[0x14]);
		unsigned long adsrPointer = CharArrayToLong(&inputMID[0x18]);
		unsigned long drumPointer = CharArrayToLong(&inputMID[0x1C]);

		unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x20]);
		unsigned long masterTrackPointer = CharArrayToLong(&inputMID[0x24]);

		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
			return;
		}

		fprintf(outFile, gameName + " - " + addressStr + "\n");
		fprintf(outFile, "Binary Song Style\n\n");

		fprintf(outFile, "Instruments: %08X\n", instrumentPointer);
		for (int x = 0; x < totalInstruments; x++)
		{
			fprintf(outFile, "%02X: %04X\n", x, CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]));
		}

		fprintf(outFile, "\n\n---------------------------\nMaster Track: %08X\n---------------------------\n", masterTrackPointer);

		unsigned long trackDataPointerFirst = CharArrayToLong(&inputMID[trackPointer]);
		SngTrackToDebugTextFile(outFile, inputMID, masterTrackPointer, trackDataPointerFirst, instrumentPointer, adsrPointer, drumPointer, SngStyle::Normal, totalInstruments);


		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
			unsigned long volumeEnd = 0;

			if (volumeDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
			
					if (volumeDataPointerTest != 0)
					{
						volumeEnd = volumeDataPointerTest;
						break;
					}
				}

				if (volumeEnd == 0)
				{
					volumeEnd = masterTrackPointer;
				}
			}

			unsigned long pitchBendDataPointer = CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
			unsigned long pitchBendEnd = 0;

			if (pitchBendDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
			
					if (pitchBendDataPointerTest != 0)
					{
						pitchBendEnd = pitchBendDataPointerTest;
						break;
					}
				}

				if (pitchBendEnd == 0)
				{
					pitchBendEnd = masterTrackPointer;
				}
			}

			unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);
			unsigned long trackEnd = inputSize;
			if (x < (numberTracks - 1))
				trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);



			fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

			if (volumeDataPointer != 0)
			{
				fprintf(outFile, "\nVolume\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = volumeDataPointer;
				while (spot < volumeEnd)
				{
					unsigned char volume = inputMID[spot];
					bool singleLength = !(volume & 0x80);

					volume = volume & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
					}

					absoluteTime += length;
				}
			}



			if (pitchBendDataPointer != 0)
			{
				fprintf(outFile, "\nPitch Bend\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = pitchBendDataPointer;
				while (spot < pitchBendEnd)
				{
					unsigned char pitchBend = inputMID[spot];
					bool singleLength = !(pitchBend & 0x80);

					pitchBend = pitchBend & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
					}

					absoluteTime += length;
				}
			}


			fprintf(outFile, "\nTrack Data\n");

			SngTrackToDebugTextFile(outFile, inputMID, trackDataPointer, trackEnd, instrumentPointer, adsrPointer, drumPointer, SngStyle::Normal, totalInstruments);
		}

		fclose(outFile);
	}
	else if (CharArrayToLong(&inputMID[0x4]) == 0x00000000) // Pokemon Stadium
	{
		unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
		unsigned long volumePointer = CharArrayToLong(&inputMID[0x8]);
		unsigned long trackPointer = CharArrayToLong(&inputMID[0xC]);
		unsigned long adsrPointer = 0x00000000;
		unsigned long drumPointer = 0x00000000;

		unsigned long instrumentPointer = 0x00000000;
		
		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
			return;
		}

		fprintf(outFile, gameName + " - " + addressStr + "\n");
		fprintf(outFile, "Mini Song Style\n\n");

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);
			
			unsigned long trackEnd = 0x00000000;
			if (trackDataPointer != 0x00000000)
			{
				if (x < (numberTracks - 1))
				{
					trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

					if (trackEnd == 0x00000000)
						trackEnd = inputSize;
				}
				else
				{
					trackEnd = inputSize;
				}
			}

			unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);


			fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Priority Start %08X Priority Data %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumePointer + (x * 4), volumeDataPointer);

			fprintf(outFile, "\nTrack Data\n");

			SngTrackToDebugTextFile(outFile, inputMID, trackDataPointer, trackEnd, instrumentPointer, adsrPointer, drumPointer, SngStyle::OldBfx, -1);
		}
	}
	else if (CharArrayToLong(&inputMID[0xC]) == 0x00000000) // BFX
	{
		unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
		unsigned long trackPointer = 0x18;

		unsigned long adsrPointer = 0x00000000;
		unsigned long drumPointer = 0x00000000;

		unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x14]);
		int totalInstruments = (inputSize - instrumentPointer) / 0x4;

		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
			return;
		}

		fprintf(outFile, "Instruments: %08X\n", instrumentPointer);
		for (int x = 0; x < totalInstruments; x++)
		{
			fprintf(outFile, "%02X: %04X\n", x, CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]));
		}

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 8)]);
			
			unsigned long trackEnd = 0x00000000;
			if (trackDataPointer != 0x00000000)
			{
				if (x < (numberTracks - 1))
				{
					trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 8)]);

					if (trackEnd == 0x00000000)
						trackEnd = instrumentPointer;
				}
				else
				{
					trackEnd = instrumentPointer;
				}
			}

			unsigned long volumeDataPointer = CharArrayToLong(&inputMID[trackDataPointer + (x * 8) + 4]);



			fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Priority Start %08X Priority Data %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, trackDataPointer + (x * 8) + 4, volumeDataPointer);

			fprintf(outFile, "\nTrack Data\n");

			SngTrackToDebugTextFile(outFile, inputMID, trackDataPointer, trackEnd, instrumentPointer, adsrPointer, drumPointer, SngStyle::Bfx, totalInstruments);
		}

		fclose(outFile);
	}
	else if (CharArrayToLong(&inputMID[0x8]) == 0x00000020) // N64 DD Style
	{
		unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
		unsigned long totalInstruments = CharArrayToLong(&inputMID[0x4]);
		unsigned long trackPointer = CharArrayToLong(&inputMID[0x8]);
		unsigned long volumePointer = CharArrayToLong(&inputMID[0xC]);
		unsigned long pitchBendPointer = CharArrayToLong(&inputMID[0x10]);
		unsigned long adsrPointer = CharArrayToLong(&inputMID[0x14]);
		unsigned long drumPointer = CharArrayToLong(&inputMID[0x18]);
		unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x1C]);

		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
			return;
		}

		fprintf(outFile, "Instruments: %08X\n", instrumentPointer);
		for (int x = 0; x < totalInstruments; x++)
		{
			fprintf(outFile, "%02X: %04X\n", x, CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]));
		}

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);

			unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
			unsigned long volumeEnd = 0;

			if (volumeDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
			
					if (volumeDataPointerTest != 0)
					{
						volumeEnd = volumeDataPointerTest;
						break;
					}
				}

				if (volumeEnd == 0)
				{
					volumeEnd = CharArrayToLong(&inputMID[trackPointer]);
				}
			}

			unsigned long pitchBendDataPointer = CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
			unsigned long pitchBendEnd = 0;

			if (pitchBendDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
			
					if (pitchBendDataPointerTest != 0)
					{
						pitchBendEnd = pitchBendDataPointerTest;
						break;
					}
				}

				if (pitchBendEnd == 0)
				{
					pitchBendEnd = CharArrayToLong(&inputMID[trackPointer]);;
				}
			}

			
			unsigned long trackEnd = 0x00000000;
			if (trackDataPointer != 0x00000000)
			{
				if (x < (numberTracks - 1))
				{
					trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

					if (trackEnd == 0x00000000)
						trackEnd = inputSize;
				}
				else
				{
					trackEnd = inputSize;
				}
			}



			fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

			if (volumeDataPointer != 0)
			{
				fprintf(outFile, "\nVolume\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = volumeDataPointer;
				while (spot < volumeEnd)
				{
					unsigned char volume = inputMID[spot];
					bool singleLength = !(volume & 0x80);

					volume = volume & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
					}

					absoluteTime += length;
				}
			}



			if (pitchBendDataPointer != 0)
			{
				fprintf(outFile, "\nPitch Bend\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = pitchBendDataPointer;
				while (spot < pitchBendEnd)
				{
					unsigned char pitchBend = inputMID[spot];
					bool singleLength = !(pitchBend & 0x80);

					pitchBend = pitchBend & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
					}

					absoluteTime += length;
				}
			}


			fprintf(outFile, "\nTrack Data\n");

			SngTrackToDebugTextFile(outFile, inputMID, trackDataPointer, trackEnd, instrumentPointer, adsrPointer, drumPointer, SngStyle::OldDD, totalInstruments);
		}

		fclose(outFile);
	}
	else if ((CharArrayToLong(&inputMID[0x0]) == 0x00000010) && (CharArrayToLong(&inputMID[0x4]) == 0x00000050) && (CharArrayToLong(&inputMID[0x8]) == 0x00000094)) // HexenSng Style
	{
		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
			return;
		}

		fprintf(outFile, gameName + " - " + addressStr + "\n");
		fprintf(outFile, "HexenSng Song Style\n\n");

		unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
		//unsigned long numberInstruments = CharArrayToLong(&inputMID[0x8]);
		unsigned long trackPointer = 0x10;
		unsigned long volumePointer = CharArrayToLong(&inputMID[0x4]);
		unsigned long pitchBendPointer = 0;
		unsigned long drumPointer = CharArrayToLong(&inputMID[0xC]);
		unsigned long adsrPointer = CharArrayToLong(&inputMID[0x8]);

		//unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x20]);
		unsigned long instrumentPointer = 0x00000000;
		
		//FILE* outFile = fopen("C:\\temp\\trackparse.txt", "w");

		/*//fprintf(outFile, "Instruments: %08X\n", instrumentPointer);
		for (int x = 0; x < numberInstruments; x++)
		{
			//fprintf(outFile, "%02X: %04X\n", x, CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]));
		}

		//fprintf(outFile, "\n\n---------------------------\nMaster Track: %08X\n---------------------------\n", masterTrackPointer);

		unsigned long trackDataPointerFirst = CharArrayToLong(&inputMID[trackPointer]);
		SngTrackToDebugTextFile(outFile, inputMID, masterTrackPointer, trackDataPointerFirst, instrumentPointer, adsrPointer, drumPointer);
		*/

		unsigned long tempo = 0;

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);

			unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
			unsigned long volumeEnd = 0;

			unsigned long pitchBendDataPointer = 0; //CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
			unsigned long pitchBendEnd = 0;

			std::vector<SngTimeValue> volumeByAbsoluteTime;
			if (volumeDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
			
					if (volumeDataPointerTest != 0)
					{
						volumeEnd = volumeDataPointerTest;
						break;
					}
				}

				if (volumeEnd == 0)
				{
					if (pitchBendDataPointer != 0)
					{
						for (int y = 0; y < numberTracks; y++)
						{
							unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);

							if (pitchBendDataPointerTest != 0)
							{
								volumeEnd = pitchBendDataPointerTest;
								break;
							}
						}
					}

					if (volumeEnd == 0)
					{
						// First track
						volumeEnd = CharArrayToLong(&inputMID[trackPointer]);;
					}
				}
			}

			std::vector<SngTimeValue> pitchBendByAbsoluteTime;
			if (pitchBendDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
			
					if (pitchBendDataPointerTest != 0)
					{
						pitchBendEnd = pitchBendDataPointerTest;
						break;
					}
				}

				if (pitchBendEnd == 0)
				{
					pitchBendEnd = CharArrayToLong(&inputMID[trackPointer]);
				}
			}

			
			unsigned long trackEnd = 0x00000000;
			if (trackDataPointer != 0x00000000)
			{
				if (x < (numberTracks - 1))
				{
					trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

					if (trackEnd == 0x00000000)
						trackEnd = inputSize;
				}
				else
				{
					trackEnd = inputSize;
				}
			}


			fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

			if (volumeDataPointer != 0)
			{
				//fprintf(outFile, "\nVolume\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = volumeDataPointer;
				while (spot < volumeEnd)
				{
					unsigned char volume = inputMID[spot];
					bool singleLength = !(volume & 0x80);

					volume = volume & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
					}

					SngTimeValue volumePair;
					volumePair.value = volume;
					volumePair.startAbsoluteTime = absoluteTime;
					volumePair.endAbsoluteTime = absoluteTime + length;
					volumeByAbsoluteTime.push_back(volumePair);

					absoluteTime += length;
				}
			}



			if (pitchBendDataPointer != 0)
			{
				fprintf(outFile, "\nPitch Bend\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = pitchBendDataPointer;
				while (spot < pitchBendEnd)
				{
					unsigned char pitchBend = inputMID[spot];
					bool singleLength = !(pitchBend & 0x80);

					pitchBend = pitchBend & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
					}

					SngTimeValue pitchBendPair;
					pitchBendPair.value = pitchBend;
					pitchBendPair.startAbsoluteTime = absoluteTime;
					pitchBendPair.endAbsoluteTime = pitchBendPair.startAbsoluteTime + length;
					pitchBendByAbsoluteTime.push_back(pitchBendPair);

					absoluteTime += length;
				}
			}


			fprintf(outFile, "\nTrack Data\n");

			SngTrackToDebugTextFile(outFile, inputMID, trackDataPointer, trackEnd, instrumentPointer, adsrPointer, drumPointer, SngStyle::HexenSng, -1);
		}

		fclose(outFile);
	}
	else // Old Style
	{
		unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
		//unsigned long numberInstruments = CharArrayToLong(&inputMID[0x8]);
		unsigned long trackPointer = CharArrayToLong(&inputMID[0x4]);
		unsigned long volumePointer = CharArrayToLong(&inputMID[0x8]);
		unsigned long pitchBendPointer = CharArrayToLong(&inputMID[0xC]);
		unsigned long adsrPointer = CharArrayToLong(&inputMID[0x10]);
		unsigned long drumPointer = CharArrayToLong(&inputMID[0x14]);

		//unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x20]);
		unsigned long instrumentPointer = 0x00000000;
		
		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
			return;
		}

		fprintf(outFile, gameName + " - " + addressStr + "\n");
		fprintf(outFile, "Old Song Style\n\n");

		/*fprintf(outFile, "Instruments: %08X\n", instrumentPointer);
		for (int x = 0; x < numberInstruments; x++)
		{
			fprintf(outFile, "%02X: %04X\n", x, CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]));
		}

		fprintf(outFile, "\n\n---------------------------\nMaster Track: %08X\n---------------------------\n", masterTrackPointer);

		unsigned long trackDataPointerFirst = CharArrayToLong(&inputMID[trackPointer]);
		SngTrackToDebugTextFile(outFile, inputMID, masterTrackPointer, trackDataPointerFirst, instrumentPointer, adsrPointer, drumPointer);
		*/

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);

			unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
			unsigned long volumeEnd = 0;

			unsigned long pitchBendDataPointer = CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
			unsigned long pitchBendEnd = 0;

			if (volumeDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
			
					if (volumeDataPointerTest != 0)
					{
						volumeEnd = volumeDataPointerTest;
						break;
					}
				}

				if (volumeEnd == 0)
				{
					if (pitchBendDataPointer != 0)
					{
						for (int y = 0; y < numberTracks; y++)
						{
							unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);

							if (pitchBendDataPointerTest != 0)
							{
								volumeEnd = pitchBendDataPointerTest;
								break;
							}
						}
					}

					if (volumeEnd == 0)
					{
						// First track
						volumeEnd = CharArrayToLong(&inputMID[trackPointer]);;
					}
				}
			}

			if (pitchBendDataPointer != 0)
			{
				for (int y = (x + 1); y < numberTracks; y++)
				{
					unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
			
					if (pitchBendDataPointerTest != 0)
					{
						pitchBendEnd = pitchBendDataPointerTest;
						break;
					}
				}

				if (pitchBendEnd == 0)
				{
					pitchBendEnd = CharArrayToLong(&inputMID[trackPointer]);
				}
			}

			
			unsigned long trackEnd = 0x00000000;
			if (trackDataPointer != 0x00000000)
			{
				if (x < (numberTracks - 1))
				{
					trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

					if (trackEnd == 0x00000000)
						trackEnd = inputSize;
				}
				else
				{
					trackEnd = inputSize;
				}
			}


			fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

			if (volumeDataPointer != 0)
			{
				fprintf(outFile, "\nVolume\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = volumeDataPointer;
				while (spot < volumeEnd)
				{
					unsigned char volume = inputMID[spot];
					bool singleLength = !(volume & 0x80);

					volume = volume & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
					}

					absoluteTime += length;
				}
			}



			if (pitchBendDataPointer != 0)
			{
				fprintf(outFile, "\nPitch Bend\n");
				unsigned long absoluteTime = 0;
				unsigned long spot = pitchBendDataPointer;
				while (spot < pitchBendEnd)
				{
					unsigned char pitchBend = inputMID[spot];
					bool singleLength = !(pitchBend & 0x80);

					pitchBend = pitchBend & 0x7F;
					spot++;

					int length = 1;
					if (!singleLength)
					{
						length = GetSngVariableLength(inputMID, spot) + 2;

						if (length < 0x82)
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
							spot++;
						}
						else
						{
							fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
							spot += 2;
						}
					}
					else
					{
						fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
					}

					absoluteTime += length;
				}
			}


			fprintf(outFile, "\nTrack Data\n");

			SngTrackToDebugTextFile(outFile, inputMID, trackDataPointer, trackEnd, instrumentPointer, adsrPointer, drumPointer, SngStyle::Old, -1);
		}

		fclose(outFile);
	}
}

void CMidiParse::SngTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, unsigned long instrumentPointer, unsigned long adsrPointer, unsigned long drumPointer, SngStyle sngStyle, int totalInstruments)
{
	unsigned char command = 0x00;
	unsigned long spot = offset;

	bool useDefinedLength = false;
	int definedLength = 0;

	bool useDefinedVelocity = false;
	int definedVelocity;

	if ((sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::Bfx) || (sngStyle == SngStyle::PtrBfx))
	{
		useDefinedVelocity = true;
		definedVelocity = 0x7F;
	}

	int drumBankOffset = -1;

	unsigned long absoluteTime = 0;

	while ((command != 0x80) && (spot < end))
	{
		command = inputMID[spot];
		
		fprintf(outFile, "%08X Time: %08X Command: %02X ", spot, absoluteTime, command);
		spot++;

		if (command < 0x80) // Note or Rest
		{
			unsigned char note = command;

			if (drumBankOffset == -1)
			{
				if (note == 0x60)
					fprintf(outFile, " Rest");
				else if (note < 0x60)
					fprintf(outFile, " Note %02X", note);
				else
					fprintf(outFile, " UNKNOWN Note %02X", note);
			}
			else
			{
				if (note == 0x60)
					fprintf(outFile, " Rest");
				else
					fprintf(outFile, " Drum SubIndex %02X", note);
			}

			if (useDefinedVelocity)
			{
				fprintf(outFile, " [Velocity %02X (%d)]", definedVelocity, definedVelocity);
			}
			else
			{
				fprintf(outFile, " Velocity %02X (%d)", inputMID[spot] & 0x7F, inputMID[spot] & 0x7F);

				if (inputMID[spot] & 0x80)
				{
					useDefinedVelocity = true;
					definedVelocity = inputMID[spot] & 0x7F;
					fprintf(outFile, " and Repeat");
				}

				spot++;
			}

			if (useDefinedLength)
			{
				fprintf(outFile, " [Length %02X (%d)]", definedLength, definedLength);
				absoluteTime += definedLength;
			}
			else
			{
				int length = GetSngVariableLength(inputMID, spot);
				
				if (length < 0x80)
				{
					fprintf(outFile, " Length %02X (%d)", inputMID[spot], length);
					spot++;
				}
				else
				{
					fprintf(outFile, " Length %02X%02X (%d)", inputMID[spot], inputMID[spot+1], length);
					spot += 2;
				}

				absoluteTime += length;
			}

			if ((drumBankOffset != -1) && (note != 0x60))
			{
				unsigned long drumNoteOffset = drumPointer + (drumBankOffset * 6) + (note * 6);

				unsigned char instrumentIndex = inputMID[drumNoteOffset + 1];
				unsigned char adsrIndex = inputMID[drumNoteOffset + 3];
				unsigned char pan = inputMID[drumNoteOffset + 4];
				unsigned char note = inputMID[drumNoteOffset + 5];

				if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					fprintf(outFile, " Drum Instrument %02X Address %08X Pan %02X Note %02X", instrumentIndex, drumNoteOffset, pan, note);
				else
					fprintf(outFile, " Drum Instrument %02X (Looked up %02X) Address %08X Pan %02X Note %02X", instrumentIndex, CharArrayToShort(&inputMID[instrumentPointer + (instrumentIndex * 2)]), drumNoteOffset, pan, note);
				fprintf(outFile, " ADSR Rate %02X (%d)", inputMID[adsrPointer + adsrIndex * 7], inputMID[adsrPointer + adsrIndex * 7]);
				fprintf(outFile, " ADSR Start Level %02X (%d)", inputMID[adsrPointer + adsrIndex * 7 + 1], inputMID[adsrPointer + adsrIndex * 7 + 1]);
				fprintf(outFile, " ADSR Attack Rate %02X (%d)", inputMID[adsrPointer + adsrIndex * 7 + 2], inputMID[adsrPointer + adsrIndex * 7 + 2]);
				fprintf(outFile, " ADSR Peak Level %02X (%d)", inputMID[adsrPointer + adsrIndex * 7 + 3], inputMID[adsrPointer + adsrIndex * 7 + 3]);
				fprintf(outFile, " ADSR Decay Rate %02X (%d)", inputMID[adsrPointer + adsrIndex * 7 + 4], inputMID[adsrPointer + adsrIndex * 7 + 4]);
				fprintf(outFile, " ADSR Sustain Level %02X (%d)", inputMID[adsrPointer + adsrIndex * 7 + 5], inputMID[adsrPointer + adsrIndex * 7 + 5]);
				fprintf(outFile, " ADSR Release Rate %02X (%d)", inputMID[adsrPointer + adsrIndex * 7 + 6], inputMID[adsrPointer + adsrIndex * 7 + 6]);
			}
		}
		else
		{
			if (command == 0x80)
			{
				//End?
				fprintf(outFile, " End");
				break;
			}
			else if (command == 0x81)
			{
				//XX, Instrument # In List
				fprintf(outFile, " Instrument");
				int instrument = GetSngVariableLength(inputMID, spot);
				if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng) || (sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::PtrBfx))
				{
					if (instrument < 0x80)
					{
						fprintf(outFile, " Instrument %02X (%d)", inputMID[spot], instrument);
						spot++;
					}
					else
					{
						fprintf(outFile, " Instrument %02X%02X (%d)", inputMID[spot], inputMID[spot+1], instrument);
						spot += 2;
					}
				}
				else if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Bfx) || (sngStyle == SngStyle::OldDD))
				{
					if (instrument >= totalInstruments)
					{
						if (instrument < 0x80)
						{
							fprintf(outFile, " Instrument %02X (%d) (TOO BIG Looked up)", inputMID[spot], instrument);
							spot++;
						}
						else
						{
							fprintf(outFile, " Instrument %02X%02X (%d) (TOO BIG Looked up)", inputMID[spot], inputMID[spot+1], instrument);
							spot += 2;
						}
					}
					else
					{
						if (instrument < 0x80)
						{
							fprintf(outFile, " Instrument %02X (%d) (Looked up %02X)", inputMID[spot], instrument, CharArrayToShort(&inputMID[instrumentPointer + (inputMID[spot] * 2)]));
							spot++;
						}
						else
						{
							fprintf(outFile, " Instrument %02X%02X (%d) (Looked up %02X)", inputMID[spot], inputMID[spot+1], instrument, CharArrayToShort(&inputMID[instrumentPointer + (inputMID[spot] * 2)]));
							spot += 2;
						}
					}
				}
			}
			else if (command == 0x82)
			{
				if ((sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::Bfx) || (sngStyle == SngStyle::PtrBfx))
				{
					//??
					fprintf(outFile, " ?");
				}
				else
				{
					//Set Slide XX
					fprintf(outFile, " Set Slide");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
			}
			else if (command == 0x83)
			{
				// ?
				fprintf(outFile, " ?");
			}
			else if (command == 0x84)
			{
				//XX ADSR SubIndex
				fprintf(outFile, " ADSR Data");
				fprintf(outFile, " Rate %02X (%d)", inputMID[spot], inputMID[spot]);
				fprintf(outFile, " Start Level %02X (%d)", inputMID[spot + 1], inputMID[spot + 1]);
				fprintf(outFile, " Attack Rate %02X (%d)", inputMID[spot + 2], inputMID[spot + 2]);
				fprintf(outFile, " Peak Level %02X (%d)", inputMID[spot + 3], inputMID[spot + 3]);
				fprintf(outFile, " Decay Rate %02X (%d)", inputMID[spot + 4], inputMID[spot + 4]);
				fprintf(outFile, " Sustain Level %02X (%d)", inputMID[spot + 5], inputMID[spot + 5]);
				fprintf(outFile, " Release Rate %02X (%d)", inputMID[spot + 6], inputMID[spot + 6]);
				spot += 7;
			}
			else if (command == 0x85)
			{
				//XX Master Track Tempo, 0x28 = 40, 0x78 = 120
				fprintf(outFile, " Master Track Tempo");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0x86)
			{
				//XXXX Set Release Time for upcoming notes
				fprintf(outFile, " Set Release Time");
				fprintf(outFile, " %04X (%d)", CharArrayToShort(&inputMID[spot]), CharArrayToShort(&inputMID[spot]));
				spot+=2;
			}
			else if (command == 0x87)
			{
				//XX 01 Set Release Time back to full note (01)
				fprintf(outFile, " Set Release Time Back to Full");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0x88)
			{
				//XX YY ZZ Vibrato Up - Delay, Amount, Speed
				fprintf(outFile, " Vibrato Up");
				fprintf(outFile, " Delay %02X (%d) Amount %02X (%d) Speed %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot+=3;
			}
			else if (command == 0x89)
			{
				//XX YY ZZ Vibrato Down - Delay, Amount, Speed
				fprintf(outFile, " Vibrato Down");
				fprintf(outFile, " Delay %02X (%d) Amount %02X (%d) Speed %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot+=3;
			}
			else if (command == 0x8A)
			{
				//Solo
				fprintf(outFile, " Solo");
			}
			else if (command == 0x8B)
			{
				//LL Set Length notes following and disable in list (variable time)
				if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng) || (sngStyle == SngStyle::OldDD))
				{
					fprintf(outFile, " Set Length Notes and Disable in Note Format");
					
					definedLength = GetSngVariableLength(inputMID, spot);
					if (definedLength == 0x00)
					{
						fprintf(outFile, " Disabled %02X (%d)", inputMID[spot], definedLength);
						useDefinedLength = false;
						spot++;
					}
					else if (definedLength < 0x80)
					{
						useDefinedLength = true;

						fprintf(outFile, " Length %02X (%d)", inputMID[spot], definedLength);
						spot++;
					}
					else
					{
						useDefinedLength = true;

						fprintf(outFile, " Length %02X%02X (%d)", inputMID[spot], inputMID[spot+1], definedLength);
						spot += 2;
					}
				}
				else
				{
					fprintf(outFile, " Set Length Notes and Disable in Note Format");
					useDefinedLength = true;
					definedLength = GetSngVariableLength(inputMID, spot);
					if (definedLength < 0x80)
					{
						fprintf(outFile, " Length %02X (%d)", inputMID[spot], definedLength);
						spot++;
					}
					else
					{
						fprintf(outFile, " Length %02X%02X (%d)", inputMID[spot], inputMID[spot+1], definedLength);
						spot += 2;
					}
				}
			}
			else if (command == 0x8D)
			{
				//XX Transpose, 00 default, 01, etc
				fprintf(outFile, " Transpose");
				fprintf(outFile, " %02X (%d)", inputMID[spot], (signed char)inputMID[spot]);
				spot++;
			}
			else if (command == 0x8F)
			{
				//XX Detune (signed)
				fprintf(outFile, " Detune");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0x90)
			{
				//XX ADSR SubIndex
				fprintf(outFile, " ADSR Index");
				fprintf(outFile, " %02X", inputMID[spot]);
				fprintf(outFile, " Rate %02X (%d)", inputMID[adsrPointer + inputMID[spot] * 7], inputMID[adsrPointer + inputMID[spot] * 7]);
				fprintf(outFile, " Start Level %02X (%d)", inputMID[adsrPointer + inputMID[spot] * 7 + 1], inputMID[adsrPointer + inputMID[spot] * 7 + 1]);
				fprintf(outFile, " Attack Rate %02X (%d)", inputMID[adsrPointer + inputMID[spot] * 7 + 2], inputMID[adsrPointer + inputMID[spot] * 7 + 2]);
				fprintf(outFile, " Peak Level %02X (%d)", inputMID[adsrPointer + inputMID[spot] * 7 + 3], inputMID[adsrPointer + inputMID[spot] * 7 + 3]);
				fprintf(outFile, " Decay Rate %02X (%d)", inputMID[adsrPointer + inputMID[spot] * 7 + 4], inputMID[adsrPointer + inputMID[spot] * 7 + 4]);
				fprintf(outFile, " Sustain Level %02X (%d)", inputMID[adsrPointer + inputMID[spot] * 7 + 5], inputMID[adsrPointer + inputMID[spot] * 7 + 5]);
				fprintf(outFile, " Release Rate %02X (%d)", inputMID[adsrPointer + inputMID[spot] * 7 + 6], inputMID[adsrPointer + inputMID[spot] * 7 + 6]);
				spot++;
			}
			else if (command == 0x91)
			{
				//Envelope Trigger Off
				fprintf(outFile, " Envelope Trigger Off");
			}
			else if (command == 0x92)
			{
				//Envelope Trigger On
				fprintf(outFile, " Envelope Trigger On");
			}
			else if (command == 0x93)
			{
				//Sample Trigger Off
				fprintf(outFile, " Sample Trigger Off");
			}
			else if (command == 0x94)
			{
				//Sample Trigger On
				fprintf(outFile, " Sample Trigger On");
			}
			else if (command == 0x95)
			{
				//XX Loop Start FF Count?
				fprintf(outFile, " Loop Start");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0x96)
			{
				//Loop
				fprintf(outFile, " Loop Back");
				break;
			}
			else if (command == 0x97)
			{
				//XX YY ZZ Wobble - Offset, Transpose, Normal
				fprintf(outFile, " Wobble");
				fprintf(outFile, " Offset %02X (%d) Transpose %02X (%d) Normal %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot+=3;
			}
			else if (command == 0x98)
			{
				// ?
				fprintf(outFile, " ?");
			}
			else if (command == 0x99)
			{
				useDefinedVelocity = false;
				//Reenable velocity on notes
				fprintf(outFile, " Reenable Velocity on Notes Format");
			}
			else if (command == 0x9A) // Old Style Only
			{
				// ?
				useDefinedVelocity = true;
				fprintf(outFile, " Enable Velocity Defined in Notes");
			}
			else if (command == 0x9B) // Old Style Only
			{
				// ?
				fprintf(outFile, " Set Repeat Velocity");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				definedVelocity = inputMID[spot];
				useDefinedVelocity = true;
				spot++;
			}
			else if (command == 0x9C)
			{
				//XX Pan, 0x00
				fprintf(outFile, " Pan");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0x9E)
			{
				//XX Drum map on
				fprintf(outFile, " Drum Map Start Index");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);

				drumBankOffset = inputMID[spot];
				spot++;
			}
			else if (command == 0x9F)
			{
				//Drum map off
				drumBankOffset = -1;
				fprintf(outFile, " Drum Map Off");
			}
			else if (command == 0xA2)
			{
				//XX Effect, 0x00
				fprintf(outFile, " Effect");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xA3)
			{
				//XX YY ?
				fprintf(outFile, " ?");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot += 2;
			}
			else if (command == 0xA3)
			{
				//XX YY Set Min/Max Transpose
				fprintf(outFile, " Set Min/Max Tranpose");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command == 0xA4)
			{
				//XX YY Set Min/Max Volume
				fprintf(outFile, " Set Min/Max Volume");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command == 0xA5)
			{
				//XX YY Set Min/Max Pan
				fprintf(outFile, " Set Min/Max Pan");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command == 0xA6)
			{
				//XX Set Note Volume (Mini Style)
				fprintf(outFile, " Set Note Volume (Mini Style)");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xA7)
			{
				//LL ?
				fprintf(outFile, " Load Track #");
				int length = GetSngVariableLength(inputMID, spot);

				if (length < 0x80)
				{
					fprintf(outFile, " %02X (%d)\n", inputMID[spot], length);
					spot++;
				}
				else
				{
					fprintf(outFile, " %02X%02X (%d)\n", inputMID[spot], inputMID[spot+1], length);
					spot += 2;
				}
			}
			else if (command == 0xA8)
			{
				//XX Pitch Bend
				fprintf(outFile, " Pitch Bend");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xA9)
			{
				//XX Stereo Sweep Speed
				fprintf(outFile, " Stereo Sweep Speed");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xAA)
			{
				//XX Master Track Effect 00 = None, 01 = Small Room, 02 = Big Room, 03 = Chorus, 04 = Flange, 05 = Echo
				fprintf(outFile, " Master Track Effect");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				if (inputMID[spot] == 0)
					fprintf(outFile, "None");
				else if (inputMID[spot] == 1)
					fprintf(outFile, "Small Room");
				else if (inputMID[spot] == 2)
					fprintf(outFile, "Big Room");
				else if (inputMID[spot] == 3)
					fprintf(outFile, "Chorus");
				else if (inputMID[spot] == 4)
					fprintf(outFile, "Flange");
				else if (inputMID[spot] == 5)
					fprintf(outFile, "Echo");
				spot++;
			}
			else if (command == 0xAB)
			{
				//XX Master Track Sync - 1
				fprintf(outFile, " Master Track Sync");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xAC)
			{
				// Reenable length on notes
				useDefinedLength = false;
				fprintf(outFile, " Reenable length on notes");
			}
			else if (command == 0xB0)
			{
				//XX ?
				fprintf(outFile, " ?");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else
			{
				fprintf(outFile, " UNKNOWN");
				return;
			}
		}

		fprintf(outFile, "\n");
	}
}

void CMidiParse::KonamiTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, ExtraGameMidiInfo extraGameMidiInfo, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength)
{
	unsigned char command = 0x00;
	unsigned long spot = offset;

	unsigned char lastDuration = 0x00;
	unsigned char lastLength = 0x00;

	unsigned long absoluteTime = 0;

	unsigned char loopAmountsLeft = 0x00;
	unsigned long loopSpot = 0;
	unsigned long loopEndSpot = 0;

	unsigned char loopNestedAmountsLeft = 0x00;
	unsigned long loopNestedSpot = 0;
	unsigned long loopNestedEndSpot = 0;

	unsigned long masterLoopMarkerStartOffset;
	unsigned long masterLoopMarkerEndOffset;

	int totalLoopsToOutputLeft = 0;
	if (writeOutLoops)
		totalLoopsToOutputLeft = loopWriteCount;

	int currentEDOffset = 0;
	int currentEEEndOffset = 0;
	int eeCountOverall = 0;

	while ((command != 0x80) && (spot < end))
	{
		if (extendTracksToHighest)
		{
			if (absoluteTime >= highestTrackLength)
				break;
		}

		command = inputMID[spot];
		
		if (command < 0xD0)
		{
			if (command >= 0x68)
			{
				fprintf(outFile, "%08X Time: %08X NOTE   : %02X*", spot, absoluteTime, (command - 0x68));
			}
			else
			{
				fprintf(outFile, "%08X Time: %08X NOTE   : %02X ", spot, absoluteTime, command);
			}
		}
		else
			fprintf(outFile, "%08X Time: %08X Command: %02X ", spot, absoluteTime, command);
		spot++;

		if (command < 0xD0)
		{
			if ((command == 0x67) || (command == (0x67 + 0x68)))
			{
				fprintf(outFile, " Drum Instrument Lookup %02X (%d)",  inputMID[spot], inputMID[spot]);
				spot++;
			}

			if (command >= 0x68)
			{
				command -= 0x68;

				fprintf(outFile, " [Repeated Duration %02X (%d)]",  lastDuration, lastDuration);
			}
			else
			{
				fprintf(outFile, " Duration %02X (%d)",  inputMID[spot], inputMID[spot]);
				lastDuration = inputMID[spot];
				spot++;
			}

			if (inputMID[spot] < 0x80)
			{
				fprintf(outFile, " Length %02X (%d) Velocity %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				lastLength = inputMID[spot];
				spot += 2;
			}
			else
			{
				fprintf(outFile, " [Repeated Length %02X (%d)] Velocity %02X* (%d)", lastLength, lastLength, inputMID[spot] & 0x7F, inputMID[spot] & 0x7F);
				spot++;
			}

			if (command < 0x48)
			{
				fprintf(outFile, " Keyboard");
			}
			else
			{
				if (command == 0x67)
				{
					fprintf(outFile, " Drums Instrument");
				}
				else
				{
					fprintf(outFile, " Drums");
				}
			}

			absoluteTime += lastDuration;
		}
		else
		{
			if (command == 0xD0)
			{
				fprintf(outFile, " ((Tempo))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xD1)
			{
				fprintf(outFile, " ((Tempo Change Fade))");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot += 2;
			}
			else if (command == 0xD2)
			{
				fprintf(outFile, " ((Instrument Change))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xD3)
			{
				fprintf(outFile, " ((Instrument/Volume Change))");
				fprintf(outFile, " Volume %02X (%d) Instrument %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot += 2;
			}
			else if (command == 0xD4)
			{
				fprintf(outFile, " ((Instrument/Volume/Pan Initial))");
				fprintf(outFile, " Volume %02X (%d) Instrument %02X (%d) Pan %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot += 3;
			}
			else if (command == 0xD5)
			{
				fprintf(outFile, " ((Volume))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xD6)
			{
				fprintf(outFile, " ((Volume))");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot += 2;
			}
			else if (command == 0xD7)
			{
				fprintf(outFile, " ((Fade Out))");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot += 2;
			}
			else if (command == 0xD8)
			{
				fprintf(outFile, " ((Staccato))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xD9)
			{
				fprintf(outFile, " ((Section Hold Note))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xDA)
			{
				fprintf(outFile, " ((Reverb))");
				fprintf(outFile, " Type/Separation %02X (%d) Amount %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot += 2;
			}
			else if (command == 0xDB)
			{
				fprintf(outFile, " UNKNOWN (Invalid)");
				break;
			}
			else if (command == 0xDC)
			{
				fprintf(outFile, " UNKNOWN (Invalid)");
				break;
			}
			else if (command == 0xDD)
			{
				fprintf(outFile, " ((Pan))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xDE)
			{
				fprintf(outFile, " Stereo Pan");
				fprintf(outFile, " Left %02X (%d) to Right %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot += 2;
			}
			else if (command == 0xDF)
			{
				fprintf(outFile, " ((Coarse Tune))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xE0)
			{
				fprintf(outFile, " ((Fine Tune))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xE1)
			{
				fprintf(outFile, " ((Tremolo))");
				fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot += 3;
			}
			else if (command == 0xE2) //Jikkyou Powerful Pro Yakyuu 2000 (J) (V1.0)
			{
				fprintf(outFile, " ?");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xE3) // Start new game and let go to hit
			{
				fprintf(outFile, " ?");
				fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot += 3;
			}
			// E4??
			else if (command == 0xE4) // Something weird about this, seems like a followup to a note, trying this for now
			{
				fprintf(outFile, " ((Pitch Bend Previous Note))");
				fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot += 3;
			}
			else if (command == 0xE5) // No. 47 music
			{
				fprintf(outFile, " ((Pitch Ascend Next Note))");
				fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				spot += 3;
			}
			else if (command == 0xE6)
			{
				fprintf(outFile, " ((Slide Notes in Section))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xE7)
			{
				loopSpot = spot - 1;
				fprintf(outFile, " ((Marker))");
			}
			else if (command == 0xE8)
			{
				fprintf(outFile, " ((Loop from Marker))");
				fprintf(outFile, " %02X (%d) Times %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				unsigned char readAmount = inputMID[spot];
				spot += 3;

				if (readAmount > 0x01)
				{
					if (loopEndSpot != spot)
					{
						loopEndSpot = spot;
						spot = loopSpot;

						loopAmountsLeft = readAmount - 1;
					}
					else
					{
						loopAmountsLeft--;

						if (loopAmountsLeft > 0)
						{
							spot = loopSpot;
						}
						else
						{
							// Reset
							loopEndSpot = 0;
						}
					}
				}
				else if (readAmount == 0x00)
				{
					// Similar to master loop

					if (extendTracksToHighest)
					{
						fprintf(outFile, "...Extending Tracks to Highest, Going to %08X", loopSpot);
						spot = loopSpot;
					}
					else if (totalLoopsToOutputLeft > 0)
					{
						fprintf(outFile, "...Master Loop, Going to %08X", loopSpot);
						spot = loopSpot;
						totalLoopsToOutputLeft--;
					}
				}
			}
			else if (command == 0xE9)
			{
				fprintf(outFile, " ((Nested Loop Marker))");
				loopNestedSpot = spot - 1;
			}
			else if (command == 0xEA)
			{
				fprintf(outFile, " ((Nested Loop from Marker))");
				fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				unsigned char readAmount = inputMID[spot];
				spot += 3;

				if (readAmount > 0x01)
				{
					if (loopNestedEndSpot != spot)
					{
						loopNestedEndSpot = spot;
						spot = loopNestedSpot;

						loopNestedAmountsLeft = readAmount - 1;
					}
					else
					{
						loopNestedAmountsLeft--;

						if (loopNestedAmountsLeft > 0)
						{
							spot = loopNestedSpot;
						}
						else
						{
							// Reset
							loopNestedEndSpot = 0;
						}
					}
				}
				else if (readAmount == 0x00)
				{
					// Similar to master loop

					if (extendTracksToHighest)
					{
						fprintf(outFile, "...Extending Tracks to Highest, Going to %08X", loopNestedSpot);
						spot = loopNestedSpot;
					}
					else if (totalLoopsToOutputLeft > 0)
					{
						fprintf(outFile, "...Master Loop, Going to %08X", loopNestedSpot);
						spot = loopNestedSpot;
						totalLoopsToOutputLeft--;
					}
				}
			}
			else if (command == 0xEB)
			{
				fprintf(outFile, " ((Master Loop Start))");

				masterLoopMarkerStartOffset = spot;
			}
			else if (command == 0xEC)
			{
				fprintf(outFile, " ((Master Loop End))");

				masterLoopMarkerEndOffset = spot - 1;

				if (extendTracksToHighest)
				{
					fprintf(outFile, "...Extending Tracks to Highest, Going to %08X", masterLoopMarkerStartOffset);
					spot = masterLoopMarkerStartOffset;
				}
				else if (totalLoopsToOutputLeft > 0)
				{
					fprintf(outFile, "...Master Loop, Going to %08X", masterLoopMarkerStartOffset);
					spot = masterLoopMarkerStartOffset;
					totalLoopsToOutputLeft--;
				}
			}
			else if (command == 0xED)
			{
				fprintf(outFile, " ((Loop Skip Start))");
				currentEDOffset = spot;
				currentEEEndOffset = 0;

				eeCountOverall = 0;
			}
			else if (command == 0xEE)
			{
				fprintf(outFile, " ((Loop Skip Middle))");

				if (eeCountOverall == 0)
				{
					if (currentEEEndOffset != 0x00000000)
					{
						fprintf(outFile, "...Going to %08X", currentEEEndOffset);
						spot = currentEEEndOffset;
					}
					
					eeCountOverall = 1;
				}
				else if (eeCountOverall == 1)
				{
					currentEEEndOffset = spot;

					spot = currentEDOffset;

					eeCountOverall = 0;

					fprintf(outFile, "...Going to %08X", currentEDOffset);
				}
			}
			else if (command == 0xEF)
			{
				fprintf(outFile, " ((Effect Delay))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xF0)
			{
				fprintf(outFile, " ((Bank Select))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xF1)
			{
				fprintf(outFile, " ?");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xF2)
			{
				fprintf(outFile, " ((Delay Note))");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);

				absoluteTime += inputMID[spot];
				spot++;
			}
			else if (command == 0xF3)
			{
				fprintf(outFile, " ((Previous Note Hold))");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);

				unsigned long noteDuration = inputMID[spot];;
				unsigned long noteLength = inputMID[spot+1];

				absoluteTime += noteDuration;

				spot += 2;
			}
			// FA/FB Goemon's Great Adventure (U) - 0053D780, not sure if legit 
			else if (command == 0xFA)
			{
				fprintf(outFile, " ?");
				fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xFB)
			{
				fprintf(outFile, " ?");
				fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command >= 0xF4)
			{
				fprintf(outFile, " End");
				break;
			}
			else
			{
				fprintf(outFile, " UNKNOWN");
				return;
			}
		}

		fprintf(outFile, "\n");
	}
}

void CMidiParse::KonamiToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra)
{
	CString filepath = midiFile;
	
	FILE* inFile = fopen(filepath, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Can't read input file " + filepath, "Error", NULL);
		return;
	}

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	rewind(inFile);

	unsigned char* inputMID = new unsigned char[inputSize];

	fread(inputMID, 1, inputSize, inFile);
	fclose(inFile);

	KonamiToDebugTextFile(ROM, romSize, gameName, address, inputMID, inputSize, textFileOut, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);

	delete [] inputMID;
}

void CMidiParse::KonamiToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra)
{
	CString addressStr;
	addressStr.Format("%08X", address);

	int numberTracks = 0x0;

	if (numberTracks == 0x0)
	{
		numberTracks = CharArrayToShort(&inputMID[0]) / 2;
	}

	FILE* outFile = fopen(textFileOut, "w");
	if (outFile == NULL)
	{
		MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
		return;
	}

	fprintf(outFile, gameName + " - " + addressStr + "\n");

	int highestTrackLength = 0;

	for (int x = 0; x < numberTracks; x++)
	{
		unsigned long trackDataPointer = CharArrayToShort(&inputMID[(x * 2)]);

		if (trackDataPointer >= inputSize)
		{
			numberTracks = x;
			break;
		}

		// Fake track
		if ((x > 0) && (inputMID[trackDataPointer-1] != 0xFF))
		{
			break;
		}

		unsigned long trackEnd;

		/*if (x < (numberTracks - 1))
		{
			trackEnd = CharArrayToShort(&inputMID[((x + 1) * 2)]);
		}
		else*/
		{
			trackEnd = inputSize;
		}

		int trackLength = FindHighestKonamiLengthTrack(x, inputMID, trackDataPointer, trackEnd);
		if (trackLength > highestTrackLength)
			highestTrackLength = trackLength;
	}

	for (int x = 0; x < numberTracks; x++)
	{
		unsigned long trackDataPointer = CharArrayToShort(&inputMID[(x * 2)]);

		if (trackDataPointer >= inputSize)
		{
			numberTracks = x;
			break;
		}

		// Fake track
		if ((x > 0) && (inputMID[trackDataPointer-1] != 0xFF))
		{
			numberTracks = x;
			break;
		}

		unsigned long trackEnd;
		/*if (x < (numberTracks - 1))
			trackEnd = CharArrayToShort(&inputMID[((x + 1) * 2)]);
		else*/
			trackEnd = inputSize;

		fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X\n------------------------------------------------------\n", x + 1, trackDataPointer);



		fprintf(outFile, "\nTrack Data\n");

		KonamiTrackToDebugTextFile(outFile, inputMID, trackDataPointer, trackEnd, extraGameMidiInfo, writeOutLoops, loopWriteCount, extendTracksToHighest, highestTrackLength);
	}

	fclose(outFile);
}

void CMidiParse::SSEQTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned long end, ExtraGameMidiInfo extraGameMidiInfo, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength)
{
	unsigned char command = 0x00;
	int spot = offset;
	int loopSpot = 0;

	unsigned long absoluteTime = 0;

	while (spot < end)
	{
		if (extendTracksToHighest)
		{
			if (absoluteTime >= highestTrackLength)
				break;
		}

		int startSpot = spot;

		unsigned long original;
		unsigned char* altPattern = NULL;
		byte altOffset = 0;
		byte altLength = 0;
		unsigned long deltaTime = GetVLBytes(inputMID, spot, original, altPattern, altOffset, altLength, false);

		absoluteTime += deltaTime;

		command = inputMID[spot];

		fprintf(outFile, "%08X:Absolute Time: %08X (Delta %08X) Command %02X", startSpot, absoluteTime, deltaTime, command);
		if (command == 0x7)
		{
			fprintf(outFile, " Instrument %02X%02X [Instrument Change]", inputMID[spot + 2], inputMID[spot + 1]);
		}
		else if (command == 0x11)
		{
			fprintf(outFile, " Note %02X Velocity %02X [Note On]", inputMID[spot + 1], inputMID[spot + 2]);
		}
		else if (command == 0x12)
		{
			fprintf(outFile, " Note %02X [Note Off]", inputMID[spot + 1]);
		}
		else if (command == 0x20)
		{
			fprintf(outFile, " %02X %02X [Loop]", inputMID[spot + 1], inputMID[spot + 2]);

			if (extendTracksToHighest)
			{
				fprintf(outFile, "...Extending Tracks to Highest, Going to %08X", loopSpot);
				spot = loopSpot;
			}
		}
		else if (command == 0x22)
		{
			fprintf(outFile, " [End]\n");
			break;
		}
		else if (command == 0x23)
		{
			loopSpot = spot;
			fprintf(outFile, " [Loop Start]");
		}
		else
		{
			for (int x = 1; x < sseqCommandSizes[command]; x++)
			{
				fprintf(outFile, " %02X", inputMID[spot + x]);
			}
		}

		
		spot += sseqCommandSizes[command];

		
		fprintf(outFile, "\n");
	}
}

void CMidiParse::SSEQToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra)
{
	CString filepath = midiFile;
	
	FILE* inFile = fopen(filepath, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Can't read input file " + filepath, "Error", NULL);
		return;
	}

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	rewind(inFile);

	unsigned char* inputMID = new unsigned char[inputSize];

	fread(inputMID, 1, inputSize, inFile);
	fclose(inFile);

	SSEQToDebugTextFile(ROM, romSize, gameName, address, inputMID, inputSize, textFileOut, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);

	delete [] inputMID;
}

void CMidiParse::SSEQToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra)
{
	numberTracks = extra;

	CString addressStr;
	addressStr.Format("%08X", address);

	FILE* outFile = fopen(textFileOut, "w");
	if (outFile == NULL)
	{
		MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
		return;
	}

	fprintf(outFile, gameName + " - " + addressStr + "\n");

	int noteUniqueId = 0;
	std::vector<TimeAndValue> tempoPositions;

	
	try
	{
		std::vector<SngNoteInfo> sngNoteList;

		int trackRealLength = 0;
		
		int loopStart = 0;
		int loopEnd = 0;
		int maxTrackLength = 0;

		int highestTrackLength = 0;

		int trackLength = FindHighestSSEQLengthTrack(inputMID, inputSize, numberTracks);
		if (trackLength > highestTrackLength)
			highestTrackLength = trackLength;

		unsigned long offsetData = 0;
		for (int track = 0; track < numberTracks; track++)
		{
			unsigned long division = CharArrayToLong(&inputMID[offsetData + 8]);
			unsigned short division2 = CharArrayToShort(&inputMID[offsetData + 0xC]);
			unsigned short extraFlag = CharArrayToShort(&inputMID[offsetData + 0xE]);

			unsigned long sizeTrack = CharArrayToLong(&inputMID[offsetData + 0x10]);
			
			fprintf(outFile, "Track %X - %08X Division %08X Division2 %04X", track, offsetData, division, division2);

			if (extraFlag)
			{
				fprintf(outFile, " Extra %08X", CharArrayToLong(&inputMID[offsetData + 0x14]));
				offsetData += 4;
			}

			fprintf(outFile, "\n");
			offsetData += 0x14;

			SSEQTrackToDebugTextFile(outFile, inputMID, offsetData, inputSize, extraGameMidiInfo, writeOutLoops, loopWriteCount, extendTracksToHighest, highestTrackLength);

			offsetData += sizeTrack;
		}
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
	
	fclose(outFile);
}

void CMidiParse::WriteSngToMidiTrack(FILE* outFile, FILE* outDebug, std::vector<TrackEvent> trackEvents)
{
	unsigned long timeOffset = 0;

	unsigned long sizeData = 0;
	byte previousTrackEvent = 0x0;

	
	for (int eventCount = 0; eventCount < trackEvents.size(); eventCount++)
	{
		TrackEvent trackEvent =  trackEvents[eventCount];
		if (trackEvent.obsoleteEvent)
		{
			timeOffset += trackEvent.deltaTime;
		}
		else
		{
			unsigned long lengthTimeDelta = 0;
			unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
			timeOffset = 0;

			sizeData += lengthTimeDelta;


			if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
			{
				sizeData += 1;
			}

			sizeData += trackEvent.contentSize;

			previousTrackEvent = trackEvent.type;
		}
	}

	unsigned long tempLong = Flip32Bit(0x4D54726B);
	fwrite(&tempLong, 1 ,4 , outFile);

	tempLong = Flip32Bit(sizeData);
	fwrite(&tempLong,1, 4, outFile);

	timeOffset = 0;
	previousTrackEvent = 0x0;

	int currentPos = 0;
	for (int eventCount = 0; eventCount < trackEvents.size(); eventCount++)
	{
		TrackEvent trackEvent = trackEvents[eventCount];

		if (trackEvent.obsoleteEvent)
		{
			timeOffset += trackEvent.deltaTime;
		}
		else
		{
			unsigned long lengthTimeDelta = 0;
			unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
			timeOffset = 0;
			WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

			if (outDebug != NULL)
				fprintf(outDebug, "Offset %08X Time Length %02X Time Value %08X Delta Time %d ", currentPos, lengthTimeDelta, timeDelta, trackEvent.deltaTime);

			currentPos += lengthTimeDelta;

			if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
			{
				fwrite(&trackEvent.type, 1, 1, outFile);

				if (outDebug != NULL)
					fprintf(outDebug, " Write Type %02X", trackEvent.type);
				currentPos++;
			}

			fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

			if (outDebug != NULL)
				fprintf(outDebug, " Content Size %02X ", trackEvent.contentSize);

			if (outDebug != NULL)
			{
				for (int r = 0; r < trackEvent.contentSize; r++)
				{
					fprintf(outDebug, "%02X", trackEvent.contents[r]);
				}
			}

			if (outDebug != NULL)
				fprintf(outDebug, "\n");

			currentPos += trackEvent.contentSize;

			previousTrackEvent = trackEvent.type;
		}
	}
}

void CMidiParse::SngToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, unsigned long extra)
{
	numberInstruments = 1;
	int noteUniqueId = 0;
	std::vector<TimeAndValue> tempoPositions;

	try
	{
		std::vector<SngNoteInfo> sngNoteList;
		
		if (inputMID[0x0] == 0x80) // Ptr Sng
		{
			unsigned long startDataNegative = (signed long)inputSize;
			unsigned long initialPointer = extra;

			unsigned long numberTracks = 0;
			
			int pointerSpot = 0;
			while (inputMID[pointerSpot] == 0x80)
			{
				numberTracks++;
				pointerSpot += 4;
			}


			unsigned long adsrPointer = 0x00000000;
			unsigned long drumPointer = 0x00000000;

			unsigned long instrumentPointer = 0x00000000;

			for (int x = 0; x < numberTracks; x++)
			{
				unsigned long trackDataPointer = CharArrayToLong(&inputMID[(x * 4)]) - initialPointer;
				
				unsigned long trackEnd = 0x00000000;
				if (x < (numberTracks - 1))
				{
					trackEnd = CharArrayToLong(&inputMID[((x + 1) * 4)]) - initialPointer;
				}
				else
				{
					trackEnd = inputSize;
				}

				std::vector<SngTimeValue> volumeByAbsoluteTime;
				std::vector<SngTimeValue> pitchBendByAbsoluteTime;
				ParseSngTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID + (signed long)startDataNegative, trackDataPointer, trackEnd, NULL, adsrPointer, drumPointer, volumeByAbsoluteTime, pitchBendByAbsoluteTime, SngStyle::PtrBfx, noteUniqueId, -1);
			}
		}
		// Second check to avoid false positive of bfx, if count is 0x215, Mario Golf E
		else if ((CharArrayToLong(&inputMID[0x0]) == 0x00000215) && (CharArrayToLong(&inputMID[0x4]) != 0x00000215)) // Binary Sng style
		{
			// parse sng
			

			unsigned long numberTracks = CharArrayToLong(&inputMID[0x4]);
			unsigned long totalInstruments = CharArrayToLong(&inputMID[0x8]);
			unsigned long trackPointer = CharArrayToLong(&inputMID[0xC]);
			unsigned long volumePointer = CharArrayToLong(&inputMID[0x10]);
			unsigned long pitchBendPointer = CharArrayToLong(&inputMID[0x14]);
			unsigned long adsrPointer = CharArrayToLong(&inputMID[0x18]);
			unsigned long drumPointer = CharArrayToLong(&inputMID[0x1C]);

			unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x20]);
			unsigned long masterTrackPointer = CharArrayToLong(&inputMID[0x24]);

			unsigned short* instrumentLookup = new unsigned short[totalInstruments];
			for (int x = 0; x < totalInstruments; x++)
			{
				instrumentLookup[x] = CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]);

				if (instrumentLookup[x] > numberInstruments)
				{
					numberInstruments = instrumentLookup[x] + 1;
				}
			}

			if (calculateInstrumentCountOnly)
			{
				delete [] instrumentLookup;
				return;
			}

			
			std::vector<SngTimeValue> masterVolumeByAbsoluteTime;
			std::vector<SngTimeValue> masterPitchBendByAbsoluteTime;

			unsigned long trackDataPointerFirst = CharArrayToLong(&inputMID[trackPointer]);
			ParseSngTrack(-1, numberInstruments, tempoPositions, sngNoteList, inputMID, masterTrackPointer, trackDataPointerFirst, instrumentLookup, adsrPointer, drumPointer, masterVolumeByAbsoluteTime, masterPitchBendByAbsoluteTime, SngStyle::Normal, noteUniqueId, totalInstruments);








			for (int x = 0; x < numberTracks; x++)
			{
				unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
				unsigned long volumeEnd = 0;

				if (volumeDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
				
						if (volumeDataPointerTest != 0)
						{
							volumeEnd = volumeDataPointerTest;
							break;
						}
					}

					if (volumeEnd == 0)
					{
						volumeEnd = masterTrackPointer;
					}
				}

				unsigned long pitchBendDataPointer = CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
				unsigned long pitchBendEnd = 0;

				if (pitchBendDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
				
						if (pitchBendDataPointerTest != 0)
						{
							pitchBendEnd = pitchBendDataPointerTest;
							break;
						}
					}

					if (pitchBendEnd == 0)
					{
						pitchBendEnd = masterTrackPointer;
					}
				}

				unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);
				unsigned long trackEnd = inputSize;
				if (x < (numberTracks - 1))
					trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);



				//fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

				std::vector<SngTimeValue> volumeByAbsoluteTime;
				if (volumeDataPointer != 0)
				{
					//fprintf(outFile, "\nVolume\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = volumeDataPointer;
					while (spot < volumeEnd)
					{
						unsigned char volume = inputMID[spot];
						bool singleLength = !(volume & 0x80);

						volume = volume & 0x7F;
						spot++;

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x80)
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
						}

						SngTimeValue volumePair;
						volumePair.value = volume;
						volumePair.startAbsoluteTime = absoluteTime;
						volumePair.endAbsoluteTime = absoluteTime + length;
						volumeByAbsoluteTime.push_back(volumePair);

						absoluteTime += length;
					}
				}


				std::vector<SngTimeValue> pitchBendByAbsoluteTime;
				if (pitchBendDataPointer != 0)
				{
					//fprintf(outFile, "\nPitch Bend\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = pitchBendDataPointer;
					while (spot < pitchBendEnd)
					{
						unsigned char pitchBend = inputMID[spot];
						bool singleLength = !(pitchBend & 0x80);

						pitchBend = pitchBend & 0x7F;
						spot++;

						

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x80)
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
						}

						SngTimeValue pitchBendPair;
						pitchBendPair.value = pitchBend;
						pitchBendPair.startAbsoluteTime = absoluteTime;
						pitchBendPair.endAbsoluteTime = pitchBendPair.startAbsoluteTime + length;
						pitchBendByAbsoluteTime.push_back(pitchBendPair);

						absoluteTime += length;
					}
				}


				//fprintf(outFile, "\nTrack Data\n");

				ParseSngTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataPointer, trackEnd, instrumentLookup, adsrPointer, drumPointer, volumeByAbsoluteTime, pitchBendByAbsoluteTime, SngStyle::Normal, noteUniqueId, totalInstruments);
			}

			delete [] instrumentLookup;
		}
		else if (CharArrayToLong(&inputMID[0x4]) == 0x00000000) // Pokemon Stadium, some Sngs for some reason
		{
			unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
			unsigned long volumePointer = CharArrayToLong(&inputMID[0x8]);
			unsigned long trackPointer = CharArrayToLong(&inputMID[0xC]);
			unsigned long adsrPointer = 0x00000000;
			unsigned long drumPointer = 0x00000000;

			unsigned long instrumentPointer = 0x00000000;
			
			//FILE* outFile = fopen("C:\\temp\\trackparse.txt", "w");

			for (int x = 0; x < numberTracks; x++)
			{
				unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);
				
				unsigned long trackEnd = 0x00000000;
				if (trackDataPointer != 0x00000000)
				{
					if (x < (numberTracks - 1))
					{
						trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

						if (trackEnd == 0x00000000)
							trackEnd = inputSize;
					}
					else
					{
						trackEnd = inputSize;
					}
				}

				unsigned long priorityDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);


				//fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume Data %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumePointer + (x * 4), volumeDataPointer);

				//fprintf(outFile, "\nTrack Data\n");

				std::vector<SngTimeValue> volumeByAbsoluteTime;
				std::vector<SngTimeValue> pitchBendByAbsoluteTime;
				ParseSngTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataPointer, trackEnd, NULL, adsrPointer, drumPointer, volumeByAbsoluteTime, pitchBendByAbsoluteTime, SngStyle::OldBfx, noteUniqueId, -1);
			}
		}
		else if (CharArrayToLong(&inputMID[0xC]) == 0x00000000) // BFX
		{
			unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
			unsigned long trackPointer = 0x18;

			unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x14]);
			int totalInstruments = (inputSize - instrumentPointer) / 0x4;
			unsigned long adsrPointer = 0x00000000;
			unsigned long drumPointer = 0x00000000;

			unsigned short* instrumentLookup = new unsigned short[totalInstruments];
			for (int x = 0; x < totalInstruments; x++)
			{
				instrumentLookup[x] = CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]);

				if (instrumentLookup[x] > numberInstruments)
				{
					numberInstruments = instrumentLookup[x] + 1;
				}
			}

			if (calculateInstrumentCountOnly)
			{
				delete [] instrumentLookup;
				return;
			}
			//FILE* outFile = fopen("C:\\temp\\trackparse.txt", "w");

			for (int x = 0; x < numberTracks; x++)
			{
				unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 8)]);
				
				unsigned long trackEnd = 0x00000000;
				if (trackDataPointer != 0x00000000)
				{
					if (x < (numberTracks - 1))
					{
						trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 8)]);

						if (trackEnd == 0x00000000)
							trackEnd = inputSize;
					}
					else
					{
						trackEnd = inputSize;
					}
				}

				unsigned long priorityDataPointer = CharArrayToLong(&inputMID[trackDataPointer + (x * 8) + 4]);


				//fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume Data %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumePointer + (x * 4), volumeDataPointer);

				//fprintf(outFile, "\nTrack Data\n");

				std::vector<SngTimeValue> volumeByAbsoluteTime;
				std::vector<SngTimeValue> pitchBendByAbsoluteTime;
				ParseSngTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataPointer, trackEnd, instrumentLookup, adsrPointer, drumPointer, volumeByAbsoluteTime, pitchBendByAbsoluteTime, SngStyle::Bfx, noteUniqueId, totalInstruments);
			}

			delete [] instrumentLookup;
		}
		else if (CharArrayToLong(&inputMID[0x8]) == 0x00000020) // N64 DD Style
		{
			unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
			unsigned long totalInstruments = CharArrayToLong(&inputMID[0x4]);
			unsigned long trackPointer = CharArrayToLong(&inputMID[0x8]);
			unsigned long volumePointer = CharArrayToLong(&inputMID[0xC]);
			unsigned long pitchBendPointer = CharArrayToLong(&inputMID[0x10]);
			unsigned long adsrPointer = CharArrayToLong(&inputMID[0x14]);
			unsigned long drumPointer = CharArrayToLong(&inputMID[0x18]);
			unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x1C]);

			unsigned short* instrumentLookup = new unsigned short[totalInstruments];
			for (int x = 0; x < totalInstruments; x++)
			{
				instrumentLookup[x] = CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]);

				if (instrumentLookup[x] > numberInstruments)
				{
					numberInstruments = instrumentLookup[x] + 1;
				}
			}

			if (calculateInstrumentCountOnly)
			{
				delete [] instrumentLookup;
				return;
			}

			for (int x = 0; x < numberTracks; x++)
			{
				unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
				unsigned long volumeEnd = 0;

				if (volumeDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
				
						if (volumeDataPointerTest != 0)
						{
							volumeEnd = volumeDataPointerTest;
							break;
						}
					}

					if (volumeEnd == 0)
					{
						volumeEnd = CharArrayToLong(&inputMID[trackPointer]);
					}
				}

				unsigned long pitchBendDataPointer = CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
				unsigned long pitchBendEnd = 0;

				if (pitchBendDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
				
						if (pitchBendDataPointerTest != 0)
						{
							pitchBendEnd = pitchBendDataPointerTest;
							break;
						}
					}

					if (pitchBendEnd == 0)
					{
						pitchBendEnd = CharArrayToLong(&inputMID[trackPointer]);
					}
				}

				unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);
				unsigned long trackEnd = 0x00000000;
				if (trackDataPointer != 0x00000000)
				{
					if (x < (numberTracks - 1))
					{
						trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

						if (trackEnd == 0x00000000)
							trackEnd = inputSize;
					}
					else
					{
						trackEnd = inputSize;
					}
				}



				//fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

				std::vector<SngTimeValue> volumeByAbsoluteTime;
				if (volumeDataPointer != 0)
				{
					//fprintf(outFile, "\nVolume\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = volumeDataPointer;
					while (spot < volumeEnd)
					{
						unsigned char volume = inputMID[spot];
						bool singleLength = !(volume & 0x80);

						volume = volume & 0x7F;
						spot++;

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x80)
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
						}

						SngTimeValue volumePair;
						volumePair.value = volume;
						volumePair.startAbsoluteTime = absoluteTime;
						volumePair.endAbsoluteTime = absoluteTime + length;
						volumeByAbsoluteTime.push_back(volumePair);

						absoluteTime += length;
					}
				}


				std::vector<SngTimeValue> pitchBendByAbsoluteTime;
				if (pitchBendDataPointer != 0)
				{
					//fprintf(outFile, "\nPitch Bend\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = pitchBendDataPointer;
					while (spot < pitchBendEnd)
					{
						unsigned char pitchBend = inputMID[spot];
						bool singleLength = !(pitchBend & 0x80);

						pitchBend = pitchBend & 0x7F;
						spot++;

						

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x80)
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
						}

						SngTimeValue pitchBendPair;
						pitchBendPair.value = pitchBend;
						pitchBendPair.startAbsoluteTime = absoluteTime;
						pitchBendPair.endAbsoluteTime = pitchBendPair.startAbsoluteTime + length;
						pitchBendByAbsoluteTime.push_back(pitchBendPair);

						absoluteTime += length;
					}
				}


				//fprintf(outFile, "\nTrack Data\n");

				ParseSngTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataPointer, trackEnd, instrumentLookup, adsrPointer, drumPointer, volumeByAbsoluteTime, pitchBendByAbsoluteTime, SngStyle::Normal, noteUniqueId, totalInstruments);
			}

			delete [] instrumentLookup;
		}
		else if ((CharArrayToLong(&inputMID[0x0]) == 0x00000010) && (CharArrayToLong(&inputMID[0x4]) == 0x00000050) && (CharArrayToLong(&inputMID[0x8]) == 0x00000094)) // HexenSng Style
		{
			unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
			//unsigned long numberInstruments = CharArrayToLong(&inputMID[0x8]);
			unsigned long trackPointer = 0x10;
			unsigned long volumePointer = CharArrayToLong(&inputMID[0x4]);
			unsigned long pitchBendPointer = 0;
			unsigned long drumPointer = CharArrayToLong(&inputMID[0xC]);
			unsigned long adsrPointer = CharArrayToLong(&inputMID[0x8]);

			//unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x20]);
			unsigned long instrumentPointer = 0x00000000;
			
			//FILE* outFile = fopen("C:\\temp\\trackparse.txt", "w");

			/*//fprintf(outFile, "Instruments: %08X\n", instrumentPointer);
			for (int x = 0; x < numberInstruments; x++)
			{
				//fprintf(outFile, "%02X: %04X\n", x, CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]));
			}

			//fprintf(outFile, "\n\n---------------------------\nMaster Track: %08X\n---------------------------\n", masterTrackPointer);

			unsigned long trackDataPointerFirst = CharArrayToLong(&inputMID[trackPointer]);
			SngTrackToDebugTextFile(outFile, inputMID, masterTrackPointer, trackDataPointerFirst, instrumentPointer, adsrPointer, drumPointer);
			*/

			unsigned long tempo = 0;

			for (int x = 0; x < numberTracks; x++)
			{
				unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);

				unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
				unsigned long volumeEnd = 0;

				unsigned long pitchBendDataPointer = 0; //CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
				unsigned long pitchBendEnd = 0;

				std::vector<SngTimeValue> volumeByAbsoluteTime;
				if (volumeDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
				
						if (volumeDataPointerTest != 0)
						{
							volumeEnd = volumeDataPointerTest;
							break;
						}
					}

					if (volumeEnd == 0)
					{
						if (pitchBendDataPointer != 0)
						{
							for (int y = 0; y < numberTracks; y++)
							{
								unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);

								if (pitchBendDataPointerTest != 0)
								{
									volumeEnd = pitchBendDataPointerTest;
									break;
								}
							}
						}

						if (volumeEnd == 0)
						{
							// First track
							volumeEnd = CharArrayToLong(&inputMID[trackPointer]);;
						}
					}
				}

				std::vector<SngTimeValue> pitchBendByAbsoluteTime;
				if (pitchBendDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
				
						if (pitchBendDataPointerTest != 0)
						{
							pitchBendEnd = pitchBendDataPointerTest;
							break;
						}
					}

					if (pitchBendEnd == 0)
					{
						pitchBendEnd = CharArrayToLong(&inputMID[trackPointer]);
					}
				}

				
				unsigned long trackEnd = 0x00000000;
				if (trackDataPointer != 0x00000000)
				{
					if (x < (numberTracks - 1))
					{
						trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

						if (trackEnd == 0x00000000)
							trackEnd = inputSize;
					}
					else
					{
						trackEnd = inputSize;
					}
				}


				//fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

				if (volumeDataPointer != 0)
				{
					//fprintf(outFile, "\nVolume\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = volumeDataPointer;
					while (spot < volumeEnd)
					{
						unsigned char volume = inputMID[spot];
						bool singleLength = !(volume & 0x80);

						volume = volume & 0x7F;
						spot++;

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x82)
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
						}

						SngTimeValue volumePair;
						volumePair.value = volume;
						volumePair.startAbsoluteTime = absoluteTime;
						volumePair.endAbsoluteTime = absoluteTime + length;
						volumeByAbsoluteTime.push_back(volumePair);

						absoluteTime += length;
					}
				}



				if (pitchBendDataPointer != 0)
				{
					//fprintf(outFile, "\nPitch Bend\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = pitchBendDataPointer;
					while (spot < pitchBendEnd)
					{
						unsigned char pitchBend = inputMID[spot];
						bool singleLength = !(pitchBend & 0x80);

						pitchBend = pitchBend & 0x7F;
						spot++;

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x82)
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
						}

						SngTimeValue pitchBendPair;
						pitchBendPair.value = pitchBend;
						pitchBendPair.startAbsoluteTime = absoluteTime;
						pitchBendPair.endAbsoluteTime = pitchBendPair.startAbsoluteTime + length;
						pitchBendByAbsoluteTime.push_back(pitchBendPair);

						absoluteTime += length;
					}
				}


				//fprintf(outFile, "\nTrack Data\n");

				ParseSngTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataPointer, trackEnd, NULL, adsrPointer, drumPointer, volumeByAbsoluteTime, pitchBendByAbsoluteTime, SngStyle::HexenSng, noteUniqueId, -1);
			}

			//fclose(outFile);
		}
		else // Old Style
		{
			unsigned long numberTracks = CharArrayToLong(&inputMID[0x0]);
			//unsigned long numberInstruments = CharArrayToLong(&inputMID[0x8]);
			unsigned long trackPointer = CharArrayToLong(&inputMID[0x4]);
			unsigned long volumePointer = CharArrayToLong(&inputMID[0x8]);
			unsigned long pitchBendPointer = CharArrayToLong(&inputMID[0xC]);
			unsigned long drumPointer = CharArrayToLong(&inputMID[0x10]);
			unsigned long adsrPointer = CharArrayToLong(&inputMID[0x14]);

			//unsigned long instrumentPointer = CharArrayToLong(&inputMID[0x20]);
			unsigned long instrumentPointer = 0x00000000;
			
			//FILE* outFile = fopen("C:\\temp\\trackparse.txt", "w");

			/*//fprintf(outFile, "Instruments: %08X\n", instrumentPointer);
			for (int x = 0; x < numberInstruments; x++)
			{
				//fprintf(outFile, "%02X: %04X\n", x, CharArrayToShort(&inputMID[instrumentPointer + (2 * x)]));
			}

			//fprintf(outFile, "\n\n---------------------------\nMaster Track: %08X\n---------------------------\n", masterTrackPointer);

			unsigned long trackDataPointerFirst = CharArrayToLong(&inputMID[trackPointer]);
			SngTrackToDebugTextFile(outFile, inputMID, masterTrackPointer, trackDataPointerFirst, instrumentPointer, adsrPointer, drumPointer);
			*/

			unsigned long tempo = 0;

			for (int x = 0; x < numberTracks; x++)
			{
				unsigned long trackDataPointer = CharArrayToLong(&inputMID[trackPointer + (x * 4)]);

				unsigned long volumeDataPointer = CharArrayToLong(&inputMID[volumePointer + (x * 4)]);
				unsigned long volumeEnd = 0;

				unsigned long pitchBendDataPointer = CharArrayToLong(&inputMID[pitchBendPointer + (x * 4)]);
				unsigned long pitchBendEnd = 0;

				std::vector<SngTimeValue> volumeByAbsoluteTime;
				if (volumeDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long volumeDataPointerTest = CharArrayToLong(&inputMID[volumePointer + (y * 4)]);
				
						if (volumeDataPointerTest != 0)
						{
							volumeEnd = volumeDataPointerTest;
							break;
						}
					}

					if (volumeEnd == 0)
					{
						if (pitchBendDataPointer != 0)
						{
							for (int y = 0; y < numberTracks; y++)
							{
								unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);

								if (pitchBendDataPointerTest != 0)
								{
									volumeEnd = pitchBendDataPointerTest;
									break;
								}
							}
						}

						if (volumeEnd == 0)
						{
							// First track
							volumeEnd = CharArrayToLong(&inputMID[trackPointer]);;
						}
					}
				}

				std::vector<SngTimeValue> pitchBendByAbsoluteTime;
				if (pitchBendDataPointer != 0)
				{
					for (int y = (x + 1); y < numberTracks; y++)
					{
						unsigned long pitchBendDataPointerTest = CharArrayToLong(&inputMID[pitchBendPointer + (y * 4)]);
				
						if (pitchBendDataPointerTest != 0)
						{
							pitchBendEnd = pitchBendDataPointerTest;
							break;
						}
					}

					if (pitchBendEnd == 0)
					{
						pitchBendEnd = CharArrayToLong(&inputMID[trackPointer]);
					}
				}

				
				unsigned long trackEnd = 0x00000000;
				if (trackDataPointer != 0x00000000)
				{
					if (x < (numberTracks - 1))
					{
						trackEnd = CharArrayToLong(&inputMID[trackPointer + ((x + 1) * 4)]);

						if (trackEnd == 0x00000000)
							trackEnd = inputSize;
					}
					else
					{
						trackEnd = inputSize;
					}
				}


				//fprintf(outFile, "\n\n------------------------------------------------------\nTrack %d: %08X Volume Start %08X Volume End %08X\n------------------------------------------------------\n", x + 1, trackDataPointer, volumeDataPointer, volumeEnd);

				if (volumeDataPointer != 0)
				{
					//fprintf(outFile, "\nVolume\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = volumeDataPointer;
					while (spot < volumeEnd)
					{
						unsigned char volume = inputMID[spot];
						bool singleLength = !(volume & 0x80);

						volume = volume & 0x7F;
						spot++;

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x82)
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X Volume %02X Length %02X%02X (%d)\n", spot-1, absoluteTime, volume, inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X Volume %02X Length 01*\n", spot-1, absoluteTime, volume);
						}

						SngTimeValue volumePair;
						volumePair.value = volume;
						volumePair.startAbsoluteTime = absoluteTime;
						volumePair.endAbsoluteTime = absoluteTime + length;
						volumeByAbsoluteTime.push_back(volumePair);

						absoluteTime += length;
					}
				}



				if (pitchBendDataPointer != 0)
				{
					//fprintf(outFile, "\nPitch Bend\n");
					unsigned long absoluteTime = 0;
					unsigned long spot = pitchBendDataPointer;
					while (spot < pitchBendEnd)
					{
						unsigned char pitchBend = inputMID[spot];
						bool singleLength = !(pitchBend & 0x80);

						pitchBend = pitchBend & 0x7F;
						spot++;

						int length = 1;
						if (!singleLength)
						{
							length = GetSngVariableLength(inputMID, spot) + 2;

							if (length < 0x82)
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], length);
								spot++;
							}
							else
							{
								//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length %02X%02X (%d)\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40), inputMID[spot], inputMID[spot+1], length);
								spot += 2;
							}
						}
						else
						{
							//fprintf(outFile, "%08X Time: %08X PitchBend %02X (%d) Length 01*\n", spot-1, absoluteTime, pitchBend, (signed char)(pitchBend - 0x40));
						}

						SngTimeValue pitchBendPair;
						pitchBendPair.value = pitchBend;
						pitchBendPair.startAbsoluteTime = absoluteTime;
						pitchBendPair.endAbsoluteTime = pitchBendPair.startAbsoluteTime + length;
						pitchBendByAbsoluteTime.push_back(pitchBendPair);

						absoluteTime += length;
					}
				}


				//fprintf(outFile, "\nTrack Data\n");

				ParseSngTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataPointer, trackEnd, NULL, adsrPointer, drumPointer, volumeByAbsoluteTime, pitchBendByAbsoluteTime, SngStyle::Old, noteUniqueId, -1);
			}

			//fclose(outFile);
		}

		WriteSngList(sngNoteList, tempoPositions, outFileName, separateByInstrument, 0x0030, false, 24);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

bool CMidiParse::IsOverlap(float x1, float x2, float y1, float y2)
{
	return x2 > y1 && y2 > x1;
}

int CMidiParse::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

bool CMidiParse::MidiToPaperMarioSngList(CString input, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfoMidiImport> channels[MAXCHANNELS], int& numChannels, std::vector<int>& instruments, int& lowestAbsoluteTime, int& highestAbsoluteTime, bool loop, unsigned long loopPoint, unsigned short & division)
{
	numChannels = 0;
	lowestAbsoluteTime = 0x7FFFFFFF;
	highestAbsoluteTime = 0;

	int noteUniqueId = 0;

	numberTracks = 0;

	


	CString tempFileName = input;

	struct stat results;
	stat(tempFileName, &results);		

	FILE* inFile1 = fopen(tempFileName, "rb");
	if (inFile1 == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	unsigned long header = CharArrayToLong(&inputMID[0]);

	if (header != 0x4D546864)
	{
		delete [] inputMID;
		MessageBox(NULL, "Invalid midi hdr", "Error", NULL);
		return false;
	}

	unsigned long headerLength = CharArrayToLong(&inputMID[4]);

	unsigned short type = CharArrayToShort(&inputMID[8]);
	unsigned short numTracks = CharArrayToShort(&inputMID[0xA]);
	division = CharArrayToShort(&inputMID[0xC]);

	if (numTracks > 0x10)
	{
		delete [] inputMID;
		MessageBox(NULL, "Invalid, can only support 16 tracks, first 1 only tempo", "Error", NULL);
		return false;
	}

	float noteTimeDivisor = division / 0x30;

	loopPoint = (float)loopPoint / noteTimeDivisor;

	if (type == 0)
	{
		
	}
	else if (type == 1)
	{

	}
	else
	{
		delete [] inputMID;

		MessageBox(NULL, "Invalid midi type", "Error", NULL);
		return false;
	}



	int position = 0xE;

	byte* repeatPattern = NULL;
	byte altOffset = 0;
	byte altLength = 0;

	bool unknownsHit = false;
	for (int trackNum = 0; trackNum < numTracks; trackNum++)
	{
		std::vector<SngNoteInfoMidiImport> pendingNoteList;

		int currentSegment[0x10];
		unsigned char currentPan[0x10];
		unsigned char currentVolume[0x10];
		unsigned char currentReverb[0x10];
		signed char currentPitchBend[0x10];

		unsigned char currentMSBBank[0x10];
		unsigned char currentLSBBank[0x10];
		unsigned char currentInstrument[0x10];

		// Controllers defaults
		for (int x = 0; x < 0x10; x++)
		{
			currentPan[x] = 0x40;
			currentVolume[x] = 0x7F;
			currentReverb[x] = 0x00;
			currentInstrument[x] = 0x00;
			currentPitchBend[x] = 0x40;

			currentMSBBank[x] = 0x00;
			currentLSBBank[x] = 0x00;
			currentSegment[x] = 0x00;
		}

		unsigned long absoluteTime = 0;
		float absoluteTimeFloat = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			delete [] inputMID;

			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTimeFloat += (float)timeTag / noteTimeDivisor;
			
			absoluteTime = absoluteTimeFloat;

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					endFlag = true;

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					unsigned char byteData[3];
					byteData[0] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byteData[1] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byteData[2] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					unsigned long microSecondsSinceQuarterNote = ((((byteData[0] << 8) | byteData[1]) << 8) | byteData[2]);
					unsigned long tempTempo = 60000000.0 / microSecondsSinceQuarterNote;

					if (tempTempo > 255)
						tempTempo = 255;
					else if (tempTempo < 1)
						tempTempo = 1;
					else
						tempTempo = (unsigned char)tempTempo;

					bool matchTempo = false;
					for (int y = 0; y < tempoPositions.size(); y++)
					{
						if (tempoPositions[y].absoluteTime == absoluteTime)
						{
							matchTempo = true;
						}
					}

					if (!matchTempo)
					{
						tempoPositions.push_back(TimeAndValue(absoluteTime, tempTempo));
					}
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					//newTrackEvent->type = 0xFF;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					//newTrackEvent->type = 0xFF;
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
				}

				previousEventValue = eventVal;
			}
			// Note off
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				int controller = (curEventVal & 0xF);

				for (int p = 0; p < pendingNoteList.size(); p++)
				{
					if (pendingNoteList[p].originalController != (controller))
						continue;

					// Go backwards in list
					if (pendingNoteList[p].noteNumber == noteNumber)
					{
						pendingNoteList[p].endAbsoluteTime = absoluteTime;

						// Promote to regular
						channels[trackNum].push_back(pendingNoteList[p]);

						pendingNoteList.erase(pendingNoteList.begin() + p);
						break;
					}
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				int controller = (curEventVal & 0xF);

				if (velocity == 0)
				{
					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller))
							continue;

						// Go backwards in list
						if (pendingNoteList[p].noteNumber == noteNumber)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;

							// Promote to regular
							channels[trackNum].push_back(pendingNoteList[p]);

							pendingNoteList.erase(pendingNoteList.begin() + p);
							break;
						}
					}
				}
				else
				{
					// If wasn't shut off, turn it off from before, then start new note
					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller))
							continue;

						// Go backwards in list
						if (pendingNoteList[p].noteNumber == noteNumber)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;

							// Promote to regular
							channels[trackNum].push_back(pendingNoteList[p]);

							pendingNoteList.erase(pendingNoteList.begin() + p);
							break;
						}
					}

					SngNoteInfoMidiImport newSongInfo;
					newSongInfo.originalController = controller;
					newSongInfo.originalTrack = trackNum;
					newSongInfo.originalNoteUniqueId = noteUniqueId++;
					newSongInfo.noteNumber = noteNumber;
					newSongInfo.velocity = velocity;
					// Apply tempo later as master track
					//newSongInfo.tempo = currentTempo;
					newSongInfo.pan = currentPan[controller];
					newSongInfo.volume = currentVolume[controller];
					newSongInfo.effect = currentReverb[controller];
					newSongInfo.instrument = currentInstrument[controller] + (currentLSBBank[controller] * 0x80) + (currentMSBBank[controller] * 0x8000);
					newSongInfo.segmentNumber = currentSegment[controller];
					newSongInfo.pitchBend = currentPitchBend[controller];
					newSongInfo.startAbsoluteTime = absoluteTime;
					pendingNoteList.push_back(newSongInfo);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				unsigned char curEventVal;

				if (statusBit)
				{
					controllerType = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				int controller = (curEventVal & 0xF);

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (controllerType == 0) // MSB Instrument Bank
				{
					currentMSBBank[controller] = controllerValue;	
				}
				else if (controllerType == 7) // Volume
				{
					if (controllerValue != currentVolume[controller])
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								channels[trackNum].push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].volume = controllerValue;
						}
					}

					currentVolume[controller] = controllerValue;	
				}
				else if (controllerType == 10) // Pan
				{
					if (controllerValue != currentPan[controller])
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								channels[trackNum].push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].pan = controllerValue;
						}
					}

					currentPan[controller] = controllerValue;
				}
				else if (controllerType == 32) // LSB Instrument Bank
				{
					currentLSBBank[controller] = controllerValue;	
				}
				else if (controllerType == 91) // Reverb
				{
					if (controllerValue != currentReverb[controller])
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								channels[trackNum].push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].effect = controllerValue;
						}
					}

					currentReverb[controller] = controllerValue;
				}
				else if (controllerType == 104) // Segment
				{
					if (controllerValue >= currentSegment[controller])
					{
						currentSegment[controller] = controllerValue;
					}
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				unsigned char curEventVal;

				if (statusBit)
				{
					instrument = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				if ((eventVal & 0xF) == 9) // Drums in GM
					instrument = instrument;
				else
					instrument = instrument;

				int controller = (curEventVal & 0xF);

				unsigned short tempInstrument = instrument + (currentLSBBank[controller] * 0x80) + (currentMSBBank[controller] * 0x8000);
				
				for (int p = 0; p < pendingNoteList.size(); p++)
				{
					if (pendingNoteList[p].originalController != (controller))
						continue;

					if (pendingNoteList[p].instrument != tempInstrument)
					{
						// Reopen
						if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;
				
							// Promote to regular
							channels[trackNum].push_back(pendingNoteList[p]);

							// Reset
							pendingNoteList[p].startAbsoluteTime = absoluteTime;
							pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
						}

						pendingNoteList[p].instrument = tempInstrument;
					}
				}

				currentInstrument[controller] = instrument;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				unsigned char curEventVal;
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			// Pitch Bend
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				byte valueLSB;

				unsigned char curEventVal;
				if (statusBit)
				{
					valueLSB = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				int controller = (curEventVal & 0xF);

				if (currentPitchBend[controller] != valueMSB)
				{
					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller))
							continue;

						// Reopen
						if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;
				
							// Promote to regular
							channels[trackNum].push_back(pendingNoteList[p]);

							// Reset
							pendingNoteList[p].startAbsoluteTime = absoluteTime;
							pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
						}
						
						pendingNoteList[p].pitchBend = valueMSB;
					}
				}

				currentPitchBend[controller] = valueMSB;

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
			{
				unsigned char curEventVal = eventVal;
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
			}
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}
		}

		for (int p = 0; p < pendingNoteList.size(); p++)
		{
			pendingNoteList[p].endAbsoluteTime = absoluteTime;
			channels[trackNum].push_back(pendingNoteList[p]);
		}

		// Clear empty notes
		for (int x = (channels[trackNum].size() - 1); x >= 0; x--)
		{
			if (channels[trackNum][x].startAbsoluteTime == channels[trackNum][x].endAbsoluteTime)
				channels[trackNum].erase(channels[trackNum].begin() + x);
		}

		for (int x = 0; x < channels[trackNum].size(); x++)
		{
			SngNoteInfoMidiImport tempNoteInfo = channels[trackNum][x];

			if (tempNoteInfo.endAbsoluteTime > highestAbsoluteTime)
				highestAbsoluteTime = tempNoteInfo.endAbsoluteTime;

			if (tempNoteInfo.startAbsoluteTime < lowestAbsoluteTime)
				lowestAbsoluteTime = tempNoteInfo.startAbsoluteTime;
		}
	}
	
	delete [] inputMID;







	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	if (outDebug != NULL)
	{
		for (int trackNum = 0; trackNum < numTracks; trackNum++)
		{
			for (int x = 0; x < channels[trackNum].size(); x++)
			{
				fprintf(outDebug, "Start %08X End %08X Instrument %02X Note %02X Volume %02X Pitch Bend %02X Pan %02X Velocity %02X\n", channels[trackNum][x].startAbsoluteTime, channels[trackNum][x].endAbsoluteTime, channels[trackNum][x].instrument, channels[trackNum][x].noteNumber, channels[trackNum][x].volume, channels[trackNum][x].pitchBend, channels[trackNum][x].pan, channels[trackNum][x].velocity);
			}
		}
	}

	if (outDebug != NULL)
		fclose(outDebug);

	numChannels = numTracks;

	if (numChannels == 0)
	{
		MessageBox(NULL, "No Channels", "Error", NULL);
		return false;
	}

	if (loop && (loopPoint > highestAbsoluteTime))
	{

		MessageBox(NULL, "Error, loop point is beyond end of midi", "Error", NULL);
		return false;
	}

	std::sort(tempoPositions.begin(), tempoPositions.end(), timeAndValueSortByTime());

	for (int x = 0; x < numChannels; x++)
	{
		bool renumberedLoop = false;
		int renumberSegment = -1;

		// Separate
		if (loop && (loopPoint != 0))
		{
			for (int y = (channels[x].size() - 1); y >= 0; y--)
			{
				SngNoteInfoMidiImport noteMidiImport = channels[x][y];
				if ((loopPoint > noteMidiImport.startAbsoluteTime) && (loopPoint < noteMidiImport.endAbsoluteTime))
				{
					// Need to split
					channels[x][y].endAbsoluteTime = loopPoint;

					noteMidiImport.startAbsoluteTime = loopPoint;
					channels[x].push_back(noteMidiImport);

					renumberedLoop = true;
					renumberSegment = noteMidiImport.segmentNumber;
				}
			}
		}

		std::sort(channels[x].begin(), channels[x].end(), sngSortByStartTime());

		if (renumberedLoop)
		{
			for (int y = 0; y < channels[x].size(); y++)
			{
				if (channels[x][y].segmentNumber == renumberSegment)
				{
					if (channels[x][y].startAbsoluteTime >= loopPoint)
					{
						channels[x][y].segmentNumber = -1;
					}
				}
			}
		}
	}

	return true;
}

bool CMidiParse::MidiToSngList(CString input, std::vector<SngNoteInfoMidiImport>& sngNoteList, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfoMidiImport> channels[MAXCHANNELS], int& numChannels, std::vector<int>& instruments, int& lowestAbsoluteTime, int& highestAbsoluteTime, bool loop, unsigned long& loopPoint)
{
	numChannels = 0;
	lowestAbsoluteTime = 0x7FFFFFFF;
	highestAbsoluteTime = 0;

	int noteUniqueId = 0;

	numberTracks = 0;

	

	std::vector<CString> inputMidiNames;
	inputMidiNames.push_back(input);

	int checkAdditional = 1;
	while (true)
	{
		CString midiNameStr = input;
		bool isMidiExtension = false;

		int indexEnd = midiNameStr.ReverseFind('.');
		if (indexEnd == -1)
			break;

		CString extension = midiNameStr.Mid(indexEnd);
		if (extension == ".midi")
		{
			midiNameStr = midiNameStr.Mid(0, indexEnd);
			isMidiExtension = true;
		}
		else if (extension == ".mid")
		{
			midiNameStr = midiNameStr.Mid(0, indexEnd);
		}
		else
		{
			break;
		}

		CString tempStr;
		tempStr.Format("%d", checkAdditional);
		
		midiNameStr += "_AdditionalPart" + tempStr;
		if (isMidiExtension)
			midiNameStr += ".midi";
		else
			midiNameStr += ".mid";

		if (GetSizeFile(midiNameStr) == 0)
			break;
		else if (checkAdditional == 1)
		{
			int iResults = MessageBox(NULL, "Do you want to incorporate the additional midi portions automatically found using the _AdditionalPart[#].mid", "Do you want to use additional midi portions?", MB_YESNO);
			if (iResults == IDNO)
				break;
		}

		inputMidiNames.push_back(midiNameStr);
		checkAdditional++;
	}

	int trackOffset = 0;

	for (int portion = 0; portion < inputMidiNames.size(); portion++)
	{
		CString tempFileName = inputMidiNames[portion];

		struct stat results;
		stat(tempFileName, &results);		

		FILE* inFile1 = fopen(tempFileName, "rb");
		if (inFile1 == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}	

		byte* inputMID = new byte[results.st_size];
		fread(inputMID, 1, results.st_size, inFile1);
		fclose(inFile1);

		unsigned long header = CharArrayToLong(&inputMID[0]);

		if (header != 0x4D546864)
		{
			delete [] inputMID;
			MessageBox(NULL, "Invalid midi hdr", "Error", NULL);
			return false;
		}

		unsigned long headerLength = CharArrayToLong(&inputMID[4]);

		unsigned short type = CharArrayToShort(&inputMID[8]);
		unsigned short numTracks = CharArrayToShort(&inputMID[0xA]);
		unsigned short division = CharArrayToShort(&inputMID[0xC]);

		float noteTimeDivisor = division / 0x30;

		loopPoint = (float)loopPoint / noteTimeDivisor;

		if (type == 0)
		{
			
		}
		else if (type == 1)
		{

		}
		else
		{
			delete [] inputMID;

			MessageBox(NULL, "Invalid midi type", "Error", NULL);
			return false;
		}



		int position = 0xE;

		byte* repeatPattern = NULL;
		byte altOffset = 0;
		byte altLength = 0;

		bool unknownsHit = false;
		for (int trackNum = 0; trackNum < numTracks; trackNum++)
		{
			std::vector<SngNoteInfoMidiImport> pendingNoteList;

			unsigned char currentPan[0x10];
			unsigned char currentVolume[0x10];
			unsigned char currentReverb[0x10];
			signed char currentPitchBend[0x10];

			unsigned char currentMSBBank[0x10];
			unsigned char currentLSBBank[0x10];
			unsigned char currentInstrument[0x10];

			// Controllers defaults
			for (int x = 0; x < 0x10; x++)
			{
				currentPan[x] = 0x40;
				currentVolume[x] = 0x7F;
				currentReverb[x] = 0x00;
				currentInstrument[x] = 0x00;
				currentPitchBend[x] = 0x40;

				currentMSBBank[x] = 0x00;
				currentLSBBank[x] = 0x00;
				currentInstrument[x] = 0x00;
			}

			unsigned long absoluteTime = 0;
			float absoluteTimeFloat = 0;

			unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
			if (trackHeader != 0x4D54726B)
			{
				delete [] inputMID;

				MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
				return false;
			}
			
			unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

			position += 8;

			byte previousEventValue = 0xFF;

			bool endFlag = false;

			while (!endFlag && (position < results.st_size))
			{
				unsigned long original;
				unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				absoluteTimeFloat += (float)timeTag / noteTimeDivisor;
				
				absoluteTime = absoluteTimeFloat;

				byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				bool statusBit = false;

				if (eventVal <= 0x7F)
				{
					// continuation
					statusBit = true;
				}
				else
				{
					statusBit = false;
				}

				if (eventVal == 0xFF) // meta event
				{
					byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					if (subType == 0x2F) //End of Track Event.
					{
						endFlag = true;

						unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
					}
					else if (subType == 0x51) //Set Tempo Event.
					{
						unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

						unsigned char byteData[3];
						byteData[0] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						byteData[1] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						byteData[2] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

						unsigned long microSecondsSinceQuarterNote = ((((byteData[0] << 8) | byteData[1]) << 8) | byteData[2]);
						unsigned long tempTempo = 60000000.0 / microSecondsSinceQuarterNote;

						if (tempTempo > 255)
							tempTempo = 255;
						else if (tempTempo < 1)
							tempTempo = 1;
						else
							tempTempo = (unsigned char)tempTempo;

						bool matchTempo = false;
						for (int y = 0; y < tempoPositions.size(); y++)
						{
							if (tempoPositions[y].absoluteTime == absoluteTime)
							{
								matchTempo = true;
							}
						}

						if (!matchTempo)
						{
							tempoPositions.push_back(TimeAndValue(absoluteTime, tempTempo));
						}
					}
					//Various Unused Meta Events.
					else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
					{
						//newTrackEvent->type = 0xFF;
						unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

						for (int i = 0; i < length; i++)
							ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
					else if (subType == 0x7F) //Unused Sequencer Specific Event.
					{
						//newTrackEvent->type = 0xFF;
						int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
						// subtract length
						for (int i = 0; i < length; i++)
						{
							ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						}
					}

					previousEventValue = eventVal;
				}
				// Note off
				else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
				{
					byte curEventVal;

					byte noteNumber;
					if (statusBit)
					{
						noteNumber = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						curEventVal = eventVal;
					}
					byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					int controller = (curEventVal & 0xF);

					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
							continue;

						// Go backwards in list
						if (pendingNoteList[p].noteNumber == noteNumber)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;

							// Promote to regular
							sngNoteList.push_back(pendingNoteList[p]);

							pendingNoteList.erase(pendingNoteList.begin() + p);
							break;
						}
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
				{
					byte curEventVal;

					byte noteNumber;
					if (statusBit)
					{
						noteNumber = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						curEventVal = eventVal;
					}
					byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					int controller = (curEventVal & 0xF);

					if (velocity == 0)
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
								continue;

							// Go backwards in list
							if (pendingNoteList[p].noteNumber == noteNumber)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;

								// Promote to regular
								sngNoteList.push_back(pendingNoteList[p]);

								pendingNoteList.erase(pendingNoteList.begin() + p);
								break;
							}
						}
					}
					else
					{
						// If wasn't shut off, turn it off from before, then start new note
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
								continue;

							// Go backwards in list
							if (pendingNoteList[p].noteNumber == noteNumber)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;

								// Promote to regular
								sngNoteList.push_back(pendingNoteList[p]);

								pendingNoteList.erase(pendingNoteList.begin() + p);
								break;
							}
						}

						SngNoteInfoMidiImport newSongInfo;
						newSongInfo.originalController = controller + (portion * 0x10);
						newSongInfo.originalTrack = trackNum + trackOffset;
						newSongInfo.originalNoteUniqueId = noteUniqueId++;
						newSongInfo.noteNumber = noteNumber;
						newSongInfo.velocity = velocity;
						// Apply tempo later as master track
						//newSongInfo.tempo = currentTempo;
						newSongInfo.pan = currentPan[controller];
						newSongInfo.volume = currentVolume[controller];
						newSongInfo.effect = currentReverb[controller];
						newSongInfo.instrument = currentInstrument[controller] + (currentLSBBank[controller] * 0x80) + (currentMSBBank[controller] * 0x8000);
						newSongInfo.pitchBend = currentPitchBend[controller];
						newSongInfo.startAbsoluteTime = absoluteTime;
						pendingNoteList.push_back(newSongInfo);
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
				{
					byte controllerType;
					unsigned char curEventVal;

					if (statusBit)
					{
						controllerType = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						curEventVal = eventVal;
					}

					int controller = (curEventVal & 0xF);

					byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					if (controllerType == 0) // MSB Instrument Bank
					{
						currentMSBBank[controller] = controllerValue;	
					}
					else if (controllerType == 7) // Volume
					{
						if (controllerValue != currentVolume[controller])
						{
							for (int p = 0; p < pendingNoteList.size(); p++)
							{
								if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
									continue;

								// Reopen
								if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
								{
									pendingNoteList[p].endAbsoluteTime = absoluteTime;
						
									// Promote to regular
									sngNoteList.push_back(pendingNoteList[p]);

									// Reset
									pendingNoteList[p].startAbsoluteTime = absoluteTime;
									pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
								}
								
								pendingNoteList[p].volume = controllerValue;
							}
						}

						currentVolume[controller] = controllerValue;	
					}
					else if (controllerType == 10) // Pan
					{
						if (controllerValue != currentPan[controller])
						{
							for (int p = 0; p < pendingNoteList.size(); p++)
							{
								if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
									continue;

								// Reopen
								if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
								{
									pendingNoteList[p].endAbsoluteTime = absoluteTime;
						
									// Promote to regular
									sngNoteList.push_back(pendingNoteList[p]);

									// Reset
									pendingNoteList[p].startAbsoluteTime = absoluteTime;
									pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
								}
								
								pendingNoteList[p].pan = controllerValue;
							}
						}

						currentPan[controller] = controllerValue;
					}
					else if (controllerType == 32) // LSB Instrument Bank
					{
						currentLSBBank[controller] = controllerValue;	
					}
					else if (controllerType == 91) // Reverb
					{
						if (controllerValue != currentReverb[controller])
						{
							for (int p = 0; p < pendingNoteList.size(); p++)
							{
								if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
									continue;

								// Reopen
								if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
								{
									pendingNoteList[p].endAbsoluteTime = absoluteTime;
						
									// Promote to regular
									sngNoteList.push_back(pendingNoteList[p]);

									// Reset
									pendingNoteList[p].startAbsoluteTime = absoluteTime;
									pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
								}
								
								pendingNoteList[p].effect = controllerValue;
							}
						}

						currentReverb[controller] = controllerValue;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
				{
					byte instrument;
					unsigned char curEventVal;

					if (statusBit)
					{
						instrument = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						curEventVal = eventVal;
					}

					if ((eventVal & 0xF) == 9) // Drums in GM
						instrument = instrument;
					else
						instrument = instrument;

					int controller = (curEventVal & 0xF);

					unsigned short tempInstrument = instrument + (currentLSBBank[controller] * 0x80) + (currentMSBBank[controller] * 0x8000);
					
					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
							continue;

						if (pendingNoteList[p].instrument != tempInstrument)
						{
							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								sngNoteList.push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}

							pendingNoteList[p].instrument = tempInstrument;
						}
					}

					currentInstrument[controller] = instrument;
					
					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
				{
					unsigned char curEventVal;
					byte amount;
					if (statusBit)
					{
						amount = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						curEventVal = eventVal;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				// Pitch Bend
				else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
				{
					byte valueLSB;

					unsigned char curEventVal;
					if (statusBit)
					{
						valueLSB = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
						curEventVal = eventVal;
					}

					byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					int controller = (curEventVal & 0xF);

					if (currentPitchBend[controller] != valueMSB)
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller + (portion * 0x10)))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								sngNoteList.push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].pitchBend = valueMSB;
						}
					}

					currentPitchBend[controller] = valueMSB;

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (eventVal == 0xF0 || eventVal == 0xF7)
				{
					unsigned char curEventVal = eventVal;
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
				}
				else
				{
					if (!unknownsHit)
					{
						MessageBox(NULL, "Invalid midi character found", "Error", NULL);
						unknownsHit = true;
					}
				}
			}

			for (int p = 0; p < pendingNoteList.size(); p++)
			{
				pendingNoteList[p].endAbsoluteTime = absoluteTime;
				sngNoteList.push_back(pendingNoteList[p]);
			}
		}

		trackOffset += numTracks;
		
		delete [] inputMID;
	}

	// Clear empty notes
	for (int x = (sngNoteList.size() - 1); x >= 0; x--)
	{
		if (sngNoteList[x].startAbsoluteTime == sngNoteList[x].endAbsoluteTime)
			sngNoteList.erase(sngNoteList.begin() + x);
	}







	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	if (outDebug != NULL)
	{
		for (int x = 0; x < sngNoteList.size(); x++)
		{
			fprintf(outDebug, "Start %08X End %08X Instrument %02X Note %02X Volume %02X Pitch Bend %02X Pan %02X Velocity %02X\n", sngNoteList[x].startAbsoluteTime, sngNoteList[x].endAbsoluteTime, sngNoteList[x].instrument, sngNoteList[x].noteNumber, sngNoteList[x].volume, sngNoteList[x].pitchBend, sngNoteList[x].pan, sngNoteList[x].velocity);
		}
	}

	if (outDebug != NULL)
		fclose(outDebug);

	// Now assign to tracks
	for (int x = 0; x < sngNoteList.size(); x++)
	{
		SngNoteInfoMidiImport tempNoteInfo = sngNoteList[x];

		if (tempNoteInfo.endAbsoluteTime > highestAbsoluteTime)
			highestAbsoluteTime = tempNoteInfo.endAbsoluteTime;

		if (tempNoteInfo.startAbsoluteTime < lowestAbsoluteTime)
			lowestAbsoluteTime = tempNoteInfo.startAbsoluteTime;

		if(std::find(instruments.begin(), instruments.end(), tempNoteInfo.instrument) == instruments.end()) 
		{
			instruments.push_back(tempNoteInfo.instrument);
		}

		int appliedChannel = -1;
		for (int channel = 0; channel < MAXCHANNELS; channel++)
		{
			bool allowOnChannel = true;

			for (int y = 0; y < channels[channel].size(); y++)
			{
				// Check if is valid
				SngNoteInfo matchNoteInfo = channels[channel][y];

				// Any overlap
				if (IsOverlap(tempNoteInfo.startAbsoluteTime, tempNoteInfo.endAbsoluteTime, matchNoteInfo.startAbsoluteTime, matchNoteInfo.endAbsoluteTime))
				{
					allowOnChannel = false;
					break;
				}

			}

			if (allowOnChannel)
			{
				appliedChannel = channel;
				break;
			}
		}

		if (appliedChannel == -1)
		{

			MessageBox(NULL, "Error, too many channels for midi", "Error", NULL);
			return false;
		}
		else
		{
			if ((appliedChannel + 1) > numChannels)
				numChannels = (appliedChannel + 1);
			channels[appliedChannel].push_back(tempNoteInfo);
		}
	}

	if (numChannels == 0)
	{

		MessageBox(NULL, "No Channels", "Error", NULL);
		return false;
	}

	if (loop && (loopPoint > highestAbsoluteTime))
	{

		MessageBox(NULL, "Error, loop point is beyond end of midi", "Error", NULL);
		return false;
	}

	std::sort(tempoPositions.begin(), tempoPositions.end(), timeAndValueSortByTime());

	for (int x = 0; x < numChannels; x++)
	{
		std::sort(channels[x].begin(), channels[x].end(), sngSortByStartTime());
	}

	return true;
}

bool CMidiParse::MidiToKonami(CString input, CString output, bool loop, unsigned long loopPoint, int& numberTracks, unsigned char separationAmount, unsigned char echoLength)
{
	try
	{
		std::vector<SngNoteInfoMidiImport> sngNoteList;
		std::vector<TimeAndValue> tempoPositions;
		std::vector<SngNoteInfoMidiImport> channels[MAXCHANNELS];
		int numChannels = 0;
		std::vector<int> instruments;
		int lowestAbsoluteTime = 0x7FFFFFFF;
		int highestAbsoluteTime = 0;


		if (!MidiToSngList(input, sngNoteList, tempoPositions, channels, numChannels, instruments, lowestAbsoluteTime, highestAbsoluteTime, loop, loopPoint))
			return false;

		for (int x = 0; x < sngNoteList.size(); x++)
		{
			if (sngNoteList[x].noteNumber < 0)
				sngNoteList[x].noteNumber = 0x00;
			else if (sngNoteList[x].noteNumber > 0x47)
				sngNoteList[x].noteNumber = 0x47;
		}
		for (int x = (numChannels - 1); x >= 0; x--)
		{
			// Remove 0 sized
			if (channels[x].size() == 0)
			{
				channels[x].erase(channels[x].begin() + x);
			}
		}

		int outputPosition = 0;

		unsigned char* outputBuffer = new unsigned char[0x100000];

		for (int x = 0; x < 0x100000; x++)
			outputBuffer[x] = 0x00;

		unsigned long trackOffsetsPointersStart = 0x00000000;


		for (int x = 0; x < numChannels; x++)
		{
			// Merge notes with same everything except volume and pitch bend
			for (int y = 0; y < channels[x].size(); y++)
			{
				SngNoteInfoMidiImport tempSongNote = channels[x][y];

				if (y == (channels[x].size() - 1))
					break;

				SngNoteInfoMidiImport tempSongNoteNext = channels[x][y + 1];

				if (
					(tempSongNote.endAbsoluteTime == tempSongNoteNext.startAbsoluteTime)
					&& (tempSongNote.effect == tempSongNoteNext.effect)
					&& (tempSongNote.instrument == tempSongNoteNext.instrument)
					&& (tempSongNote.noteNumber == tempSongNoteNext.noteNumber)
					&& (tempSongNote.pan == tempSongNoteNext.pan)
					&& (tempSongNote.velocity == tempSongNoteNext.velocity)
					&& (tempSongNote.pitchBend == tempSongNoteNext.pitchBend)
					&& (tempSongNote.volume == tempSongNoteNext.volume)
					&& (tempSongNote.originalTrack == tempSongNoteNext.originalTrack)
					&& (tempSongNote.originalController == tempSongNoteNext.originalController)
					&& (tempSongNote.originalNoteUniqueId == tempSongNoteNext.originalNoteUniqueId)
					)
				{
					// Merge
					channels[x][y].endAbsoluteTime = tempSongNoteNext.endAbsoluteTime;
					channels[x].erase(channels[x].begin() + y + 1);

					// Redo Note
					y--;
				}
			}
		}

		// Skip Track Headers
		outputPosition += (numChannels * 0x2);

		for (int x = 0; x < numChannels; x++)
		{
			WriteShortToBuffer(outputBuffer, (x * 2), outputPosition);

			bool wroteChannelLoop = false;
			unsigned long absoluteTime = 0;

			if (loop && !wroteChannelLoop && (loopPoint == 0))
			{
				outputBuffer[outputPosition++] = 0xEB;
				wroteChannelLoop = true;
			}

			unsigned short currentInstrument = 0xFFFF;
			int currentPan = -1;
			int currentEffect = -1;
			int currentVolume = -1;
			unsigned char currentBank = 0x00;
			signed char currentCoarseTune = 0x00;

			int lastLength = 0xFF;
			int lastDuration = 0xFF;

			int tempoIndex = 0;
			unsigned char currentTempo = 0x00;

			bool wroteTempo = false;
			for (int y = 0; y < channels[x].size(); y++)
			{
				if ((x == 0) && (y == 0))
				{
					if ((separationAmount != 0) || (echoLength != 0))
					{
						outputBuffer[outputPosition++] = 0xDA;
						outputBuffer[outputPosition++] = separationAmount;
						outputBuffer[outputPosition++] = echoLength;
					}
				}

				if (tempoPositions.size() > 0)
				{
					for (int z = tempoIndex; z < tempoPositions.size(); z++)
					{
						if ((tempoPositions[z].absoluteTime >= absoluteTime) && (tempoPositions[z].absoluteTime <= channels[x][y].startAbsoluteTime))
						{
							if (loop && !wroteChannelLoop && ((loopPoint >= absoluteTime) && (loopPoint <= tempoPositions[z].absoluteTime)))
							{
								unsigned long delta = (loopPoint - absoluteTime);

								while (delta > 0)
								{
									outputBuffer[outputPosition++] = 0xF2;

									if (delta < 0x80)
									{
										outputBuffer[outputPosition++] = delta;
										absoluteTime += delta;
										delta = 0;
									}
									else
									{
										outputBuffer[outputPosition++] = 0x7F;
										absoluteTime += 0x7F;
										delta -= 0x7F;
									}
								}

								outputBuffer[outputPosition++] = 0xEB;
								wroteChannelLoop = true;

								lastLength = 0xFF;
								lastDuration = 0xFF;

								currentInstrument = 0xFFFF;
								currentPan = -1;
								currentEffect = -1;
								currentVolume = -1;

								outputBuffer[outputPosition++] = 0xF0;
								outputBuffer[outputPosition++] = currentBank;

								outputBuffer[outputPosition++] = 0xDF;
								outputBuffer[outputPosition++] = currentCoarseTune;

								outputBuffer[outputPosition++] = 0xD0;
								outputBuffer[outputPosition++] = currentTempo;
							}

							unsigned long delta = (tempoPositions[z].absoluteTime - absoluteTime);

							while (delta > 0)
							{
								outputBuffer[outputPosition++] = 0xF2;

								if (delta < 0x80)
								{
									outputBuffer[outputPosition++] = delta;
									absoluteTime += delta;
									delta = 0;
								}
								else
								{
									outputBuffer[outputPosition++] = 0x7F;
									absoluteTime += 0x7F;
									delta -= 0x7F;
								}
							}

							if (!wroteTempo)
							{
								outputBuffer[outputPosition++] = 0xD0;
								outputBuffer[outputPosition++] = tempoPositions[z].value;

								currentTempo = tempoPositions[z].value;

								wroteTempo = true;
							}
							else
							{
								/*outputBuffer[outputPosition++] = 0xD1;
								outputBuffer[outputPosition++] = 0x60;
								outputBuffer[outputPosition++] = tempoPositions[z].value;*/

								outputBuffer[outputPosition++] = 0xD0;
								outputBuffer[outputPosition++] = tempoPositions[z].value;

								currentTempo = tempoPositions[z].value;
							}
							tempoIndex = z + 1;
						}
						else if ((tempoPositions[z].absoluteTime >= absoluteTime) && (tempoPositions[z].absoluteTime >= channels[x][y].startAbsoluteTime) && (tempoPositions[z].absoluteTime < channels[x][y].endAbsoluteTime))
						{
							// Split note
							SngNoteInfoMidiImport tempSongInfoCopy = channels[x][y];	
							channels[x][y].endAbsoluteTime = tempoPositions[z].absoluteTime;
							tempSongInfoCopy.startAbsoluteTime = tempoPositions[z].absoluteTime;

							channels[x].insert(channels[x].begin() + y + 1, tempSongInfoCopy);
						}
						else
						{
							break;
						}
					}
				}

				// Split note out
				if (loop && !wroteChannelLoop && ((loopPoint > absoluteTime) && (loopPoint > channels[x][y].startAbsoluteTime) && (loopPoint < channels[x][y].endAbsoluteTime)))
				{
					SngNoteInfoMidiImport tempSongInfoCopy = channels[x][y];	
					channels[x][y].endAbsoluteTime = loopPoint;
					tempSongInfoCopy.startAbsoluteTime = loopPoint;

					channels[x].insert(channels[x].begin() + y + 1, tempSongInfoCopy);
				}


				if (loop && !wroteChannelLoop && ((loopPoint >= absoluteTime) && (loopPoint <= channels[x][y].startAbsoluteTime)))
				{
					unsigned long delta = (loopPoint - absoluteTime);

					while (delta > 0)
					{
						outputBuffer[outputPosition++] = 0xF2;

						if (delta < 0x80)
						{
							outputBuffer[outputPosition++] = delta;
							absoluteTime += delta;
							delta = 0;
						}
						else
						{
							outputBuffer[outputPosition++] = 0x7F;
							absoluteTime += 0x7F;
							delta -= 0x7F;
						}
					}

					outputBuffer[outputPosition++] = 0xEB;
					wroteChannelLoop = true;

					lastLength = 0xFF;
					lastDuration = 0xFF;

					currentInstrument = 0xFFFF;
					currentPan = -1;
					currentEffect = -1;
					currentVolume = -1;

					outputBuffer[outputPosition++] = 0xF0;
					outputBuffer[outputPosition++] = currentBank;

					outputBuffer[outputPosition++] = 0xDF;
					outputBuffer[outputPosition++] = currentCoarseTune;

					outputBuffer[outputPosition++] = 0xD0;
					outputBuffer[outputPosition++] = currentTempo;
				}

				// Write initial rest
				if (channels[x][y].startAbsoluteTime > absoluteTime)
				{
					// Write rest	
					unsigned long delta = channels[x][y].startAbsoluteTime - absoluteTime;

					while (delta > 0)
					{
						outputBuffer[outputPosition++] = 0xF2;

						if (delta < 0x80)
						{
							outputBuffer[outputPosition++] = delta;
							absoluteTime += delta;
							delta = 0;
						}
						else
						{
							outputBuffer[outputPosition++] = 0x7F;
							absoluteTime += 0x7F;
							delta -= 0x7F;
						}
					}
				}

				SngNoteInfoMidiImport tempSongInfo = channels[x][y];	

				bool isDrum = (tempSongInfo.instrument >= 0x8000);
				if (tempSongInfo.instrument != currentInstrument)
				{
					if (!isDrum) // Skip Drums
					{
						unsigned char newBank = ((tempSongInfo.instrument >> 8) & 0xFF);

						if (currentBank != newBank)
						{
							outputBuffer[outputPosition++] = 0xF0;
							outputBuffer[outputPosition++] = newBank;
							currentBank = newBank;
						}
					}
				}

				if (
					(tempSongInfo.instrument != currentInstrument)
					&& (tempSongInfo.pan != currentPan)
					&& (tempSongInfo.volume != currentVolume)
					&& (!isDrum)
					)
				{
					currentVolume = tempSongInfo.volume;
					currentInstrument = tempSongInfo.instrument;
					currentPan = tempSongInfo.pan;

					outputBuffer[outputPosition++] = 0xD4;
					outputBuffer[outputPosition++] = currentVolume * 2;
					outputBuffer[outputPosition++] = currentInstrument;
					outputBuffer[outputPosition++] = currentPan;
					
				}
				else if (
					(tempSongInfo.instrument != currentInstrument)
					&& (tempSongInfo.volume != currentVolume)
					&& (!isDrum)
					)
				{
					currentVolume = tempSongInfo.volume;
					currentInstrument = tempSongInfo.instrument;

					outputBuffer[outputPosition++] = 0xD3;
					outputBuffer[outputPosition++] = currentVolume * 2;
					outputBuffer[outputPosition++] = currentInstrument;	
				}
				else
				{
					if (
					(tempSongInfo.instrument != currentInstrument)
					&& (!isDrum)
					)
					{
						currentInstrument = tempSongInfo.instrument;

						outputBuffer[outputPosition++] = 0xD2;
						outputBuffer[outputPosition++] = currentInstrument;
					}

					if (
						(tempSongInfo.volume != currentVolume)
						)
					{
						currentVolume = tempSongInfo.volume;

						outputBuffer[outputPosition++] = 0xD5;
						outputBuffer[outputPosition++] = currentVolume * 2;
					}
					
					if (
						(tempSongInfo.pan != currentPan)
						)
					{
						currentPan = tempSongInfo.pan;

						outputBuffer[outputPosition++] = 0xDD;
						outputBuffer[outputPosition++] = currentPan;
					}
				}

				if (tempSongInfo.effect != currentEffect)
				{
					currentEffect = tempSongInfo.effect;

					outputBuffer[outputPosition++] = 0xEF;
					outputBuffer[outputPosition++] = tempSongInfo.effect;
				}


				/*int noteLength = tempSongInfo.endAbsoluteTime - tempSongInfo.startAbsoluteTime;

				int noteDuration;
				if (y == (channels[x].size() - 1))
					noteDuration = highestAbsoluteTime - tempSongInfo.startAbsoluteTime;
				else
					noteDuration = channels[x][y+1].startAbsoluteTime - tempSongInfo.startAbsoluteTime;

				if (noteLength > 0x7F)
				{
					unsigned char noteNumber = tempSongInfo.noteNumber;
					if (isDrum)
					{
						signed char newCoarseTune = noteNumber - 0x3C;
						if (currentCoarseTune != newCoarseTune)
						{
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;
						}

						unsigned char drumInstrument = (tempSongInfo.instrument & 0xFF);

						if (drumInstrument >= 0x1F)
						{
							noteNumber = 0x67;
							outputBuffer[outputPosition++] = noteNumber;
							outputBuffer[outputPosition++] = drumInstrument;
						}
						else
						{
							noteNumber = drumInstrument + 0x48;
							outputBuffer[outputPosition++] = noteNumber;
						}
					}
					else
					{
						int writtenNote = noteNumber - currentCoarseTune;

						if (writtenNote > 0x47)
						{
							int newCoarseTune = noteNumber - 0x48;
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;

							writtenNote = noteNumber - currentCoarseTune;
						}

						outputBuffer[outputPosition++] = writtenNote;
					}

					outputBuffer[outputPosition++] = 0x7F;
					lastDuration = 0x7F;

					outputBuffer[outputPosition++] = 0x00;
					lastLength = 0x00;

					outputBuffer[outputPosition++] = tempSongInfo.velocity;

					noteLength -= 0x7F;
					absoluteTime += 0x7F;
					noteDuration -= 0x7F;

					while (noteLength > 0)
					{
						outputBuffer[outputPosition++] = 0xF3;
						if (noteLength > 0x7F)
						{
							outputBuffer[outputPosition++] = 0x7F;
							outputBuffer[outputPosition++] = 0x00;
							
							noteLength -= 0x7F;
							absoluteTime += 0x7F;
							noteDuration -= 0x7F;
						}
						else
						{
							if (noteDuration > 0x7F)
							{
								outputBuffer[outputPosition++] = 0x7F;
								outputBuffer[outputPosition++] = noteLength;

								absoluteTime += 0x7F;
								noteDuration -= 0x7F;
							}
							else
							{
								outputBuffer[outputPosition++] = noteDuration;
								outputBuffer[outputPosition++] = noteLength;
								absoluteTime += noteDuration;
								noteDuration = 0;
							}

							noteLength = 0;
						}
					}
				}
				else
				{
					if (noteDuration > 0x7F)
						noteDuration = 0x7F;

					unsigned char noteNumber = tempSongInfo.noteNumber;
					if (isDrum)
					{
						signed char newCoarseTune = noteNumber - 0x3C;
						if (currentCoarseTune != newCoarseTune)
						{
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;
						}

						unsigned char drumInstrument = (tempSongInfo.instrument & 0xFF);

						if (drumInstrument >= 0x1F)
						{
							noteNumber = 0x67;
							if (lastDuration == noteDuration)
							{
								outputBuffer[outputPosition++] = noteNumber + 0x68;
								outputBuffer[outputPosition++] = drumInstrument;
							}
							else
							{
								outputBuffer[outputPosition++] = noteNumber;
								outputBuffer[outputPosition++] = drumInstrument;
								outputBuffer[outputPosition++] = noteDuration;
								lastDuration = noteDuration;
							}

							outputBuffer[outputPosition++] = drumInstrument;
						}
						else
						{
							noteNumber = drumInstrument + 0x48;
							if (lastDuration == noteDuration)
							{
								outputBuffer[outputPosition++] = noteNumber + 0x68;
							}
							else
							{
								outputBuffer[outputPosition++] = noteNumber;
								outputBuffer[outputPosition++] = noteDuration;
								lastDuration = noteDuration;
							}
						}
					}
					else
					{
						int writtenNote = noteNumber - currentCoarseTune;

						if (writtenNote > 0x47)
						{
							int newCoarseTune = noteNumber - 0x47;
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;

							writtenNote = noteNumber - currentCoarseTune;
						}

						if (lastDuration == noteDuration)
						{
							outputBuffer[outputPosition++] = writtenNote + 0x68;
						}
						else
						{
							outputBuffer[outputPosition++] = writtenNote;
							outputBuffer[outputPosition++] = noteDuration;
							lastDuration = noteDuration;
						}
					}

					if (lastLength == noteLength)
					{
						outputBuffer[outputPosition++] = tempSongInfo.velocity + 0x80;
					}
					else
					{
						outputBuffer[outputPosition++] = noteLength;
						outputBuffer[outputPosition++] = tempSongInfo.velocity;
					}

					lastLength = noteLength;

					absoluteTime += noteDuration;
				}*/


				int noteDuration = tempSongInfo.endAbsoluteTime - tempSongInfo.startAbsoluteTime;

				if (noteDuration > 0x7F)
				{
					unsigned char noteNumber = tempSongInfo.noteNumber;
					if (isDrum)
					{
						signed char newCoarseTune = noteNumber - 0x3C;
						if (currentCoarseTune != newCoarseTune)
						{
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;
						}

						unsigned char drumInstrument = (tempSongInfo.instrument & 0xFF);

						if (drumInstrument >= 0x1F)
						{
							noteNumber = 0x67;
							outputBuffer[outputPosition++] = noteNumber;
							outputBuffer[outputPosition++] = drumInstrument;
						}
						else
						{
							noteNumber = drumInstrument + 0x48;
							outputBuffer[outputPosition++] = noteNumber;
						}
					}
					else
					{
						int writtenNote = noteNumber - currentCoarseTune;

						if (writtenNote > 0x47)
						{
							int newCoarseTune = noteNumber - 0x48;
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;

							writtenNote = noteNumber - currentCoarseTune;
						}

						outputBuffer[outputPosition++] = writtenNote;
					}

					outputBuffer[outputPosition++] = 0x7F;
					lastDuration = 0x7F;

					outputBuffer[outputPosition++] = 0x00;
					lastLength = 0x00;

					outputBuffer[outputPosition++] = tempSongInfo.velocity;

					absoluteTime += 0x7F;
					noteDuration -= 0x7F;

					while (noteDuration > 0)
					{
						outputBuffer[outputPosition++] = 0xF3;
						if (noteDuration > 0x7F)
						{
							outputBuffer[outputPosition++] = 0x7F;
							outputBuffer[outputPosition++] = 0x00;
							
							absoluteTime += 0x7F;
							noteDuration -= 0x7F;
						}
						else
						{
							outputBuffer[outputPosition++] = noteDuration;
							outputBuffer[outputPosition++] = 0x7F;
							absoluteTime += noteDuration;
							noteDuration = 0;
						}
					}
				}
				else
				{
					unsigned char noteNumber = tempSongInfo.noteNumber;
					if (isDrum)
					{
						signed char newCoarseTune = noteNumber - 0x3C;
						if (currentCoarseTune != newCoarseTune)
						{
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;
						}

						unsigned char drumInstrument = (tempSongInfo.instrument & 0xFF);

						if (drumInstrument >= 0x1F)
						{
							noteNumber = 0x67;
							if (lastDuration == noteDuration)
							{
								outputBuffer[outputPosition++] = noteNumber + 0x68;
								outputBuffer[outputPosition++] = drumInstrument;
							}
							else
							{
								outputBuffer[outputPosition++] = noteNumber;
								outputBuffer[outputPosition++] = drumInstrument;
								outputBuffer[outputPosition++] = noteDuration;
								lastDuration = noteDuration;
							}

							outputBuffer[outputPosition++] = drumInstrument;
						}
						else
						{
							noteNumber = drumInstrument + 0x48;
							if (lastDuration == noteDuration)
							{
								outputBuffer[outputPosition++] = noteNumber + 0x68;
							}
							else
							{
								outputBuffer[outputPosition++] = noteNumber;
								outputBuffer[outputPosition++] = noteDuration;
								lastDuration = noteDuration;
							}
						}
					}
					else
					{
						int writtenNote = noteNumber - currentCoarseTune;

						if (writtenNote > 0x47)
						{
							int newCoarseTune = noteNumber - 0x47;
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = newCoarseTune;
							currentCoarseTune = newCoarseTune;

							writtenNote = noteNumber - currentCoarseTune;
						}

						if (lastDuration == noteDuration)
						{
							outputBuffer[outputPosition++] = writtenNote + 0x68;
						}
						else
						{
							outputBuffer[outputPosition++] = writtenNote;
							outputBuffer[outputPosition++] = noteDuration;
							lastDuration = noteDuration;
						}
					}

					if (lastLength == 0x7F)
					{
						outputBuffer[outputPosition++] = tempSongInfo.velocity + 0x80;
					}
					else
					{
						outputBuffer[outputPosition++] = 0x7F;
						outputBuffer[outputPosition++] = tempSongInfo.velocity;
					}

					lastLength = 0x7F;

					absoluteTime += noteDuration;
				}


			
				if (
					(tempSongInfo.pitchBend != 0x40)
					)
				{
					if (tempSongInfo.pitchBend >= 0x7F)
					{
						if (tempSongInfo.noteNumber < 0x46)
						{
							outputBuffer[outputPosition++] = 0xE4;
							outputBuffer[outputPosition++] = 0x00;
							outputBuffer[outputPosition++] = 0x04;

							outputBuffer[outputPosition++] = tempSongInfo.noteNumber + 2;
						}
					}
					else if (tempSongInfo.pitchBend >= 0x60)
					{
						if (tempSongInfo.noteNumber < 0x47)
						{
							outputBuffer[outputPosition++] = 0xE4;
							outputBuffer[outputPosition++] = 0x00;
							outputBuffer[outputPosition++] = 0x04;

							outputBuffer[outputPosition++] = tempSongInfo.noteNumber + 1;
						}
					}
					else if (tempSongInfo.pitchBend <= 0x00)
					{
						if (tempSongInfo.noteNumber > 1)
						{
							outputBuffer[outputPosition++] = 0xE4;
							outputBuffer[outputPosition++] = 0x00;
							outputBuffer[outputPosition++] = 0x04;

							outputBuffer[outputPosition++] = tempSongInfo.noteNumber - 2;
						}
					}
					else if (tempSongInfo.pitchBend <= 0x20)
					{
						if (tempSongInfo.noteNumber > 0)
						{
							outputBuffer[outputPosition++] = 0xE4;
							outputBuffer[outputPosition++] = 0x00;
							outputBuffer[outputPosition++] = 0x04;

							outputBuffer[outputPosition++] = tempSongInfo.noteNumber - 1;
						}
					}
				}
			}

			if (loop && (!wroteChannelLoop))
			{
				unsigned long delta = (loopPoint - absoluteTime);

				while (delta > 0)
				{
					outputBuffer[outputPosition++] = 0xF2;

					if (delta < 0x80)
					{
						outputBuffer[outputPosition++] = delta;
						absoluteTime += delta;
						delta = 0;
					}
					else
					{
						outputBuffer[outputPosition++] = 0x7F;
						absoluteTime += 0x7F;
						delta -= 0x7F;
					}
				}

				outputBuffer[outputPosition++] = 0xEB;

				wroteChannelLoop = true;

				lastLength = 0xFF;
				lastDuration = 0xFF;

				currentInstrument = 0xFFFF;
				currentPan = -1;
				currentEffect = -1;
				currentVolume = -1;

				outputBuffer[outputPosition++] = 0xF0;
				outputBuffer[outputPosition++] = currentBank;

				outputBuffer[outputPosition++] = 0xDF;
				outputBuffer[outputPosition++] = currentCoarseTune;

				outputBuffer[outputPosition++] = 0xD0;
				outputBuffer[outputPosition++] = currentTempo;
			}

			unsigned long delta = (highestAbsoluteTime - absoluteTime);

			while (delta > 0)
			{
				outputBuffer[outputPosition++] = 0xF2;

				if (delta < 0x80)
				{
					outputBuffer[outputPosition++] = delta;
					absoluteTime += delta;
					delta = 0;
				}
				else
				{
					outputBuffer[outputPosition++] = 0x7F;
					absoluteTime += 0x7F;
					delta -= 0x7F;
				}
			}

			if (wroteChannelLoop)
			{
				outputBuffer[outputPosition++] = 0xEC;
			}
			
			outputBuffer[outputPosition++] = 0xFF;
		}

		numberTracks = numChannels;

		FILE* outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			delete [] outputBuffer;

			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return false;
		}

		fwrite(outputBuffer, 1, outputPosition, outFile);

		delete [] outputBuffer;
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error converting", "Error", NULL);
		return false;
	}
	return true;
}

bool CMidiParse::MidiToSng(CString input, CString output, bool loop, unsigned long loopPoint, SngStyle sngStyle, unsigned char masterTrackEffect)
{
	try
	{
		std::vector<SngNoteInfoMidiImport> sngNoteList;
		std::vector<TimeAndValue> tempoPositions;
		std::vector<SngNoteInfoMidiImport> channels[MAXCHANNELS];
		int numChannels = 0;
		std::vector<int> instruments;
		int lowestAbsoluteTime = 0x7FFFFFFF;
		int highestAbsoluteTime = 0;


		if (!MidiToSngList(input, sngNoteList, tempoPositions, channels, numChannels, instruments, lowestAbsoluteTime, highestAbsoluteTime, loop, loopPoint))
			return false;



		std::vector<TimeAndValue> volumeData[MAXCHANNELS];
		std::vector<TimeAndValue> pitchBendData[MAXCHANNELS];
		for (int x = 0; x < numChannels; x++)
		{
			unsigned char currentVolume = 0x7F;
			unsigned char currentPitchBend = 0x40;

			for (int y = 0; y < channels[x].size(); y++)
			{
				SngNoteInfoMidiImport tempNoteInfo = channels[x][y];

				if (tempNoteInfo.volume != currentVolume)
				{
					if ((volumeData[x].size() == 0) || (volumeData[x].back().absoluteTime < tempNoteInfo.startAbsoluteTime))
					{
						volumeData[x].push_back(TimeAndValue(tempNoteInfo.startAbsoluteTime, tempNoteInfo.volume));
					}
					else
					{
						volumeData[x][volumeData[x].size()-1] = TimeAndValue(tempNoteInfo.startAbsoluteTime, tempNoteInfo.volume);
					}

					currentVolume = tempNoteInfo.volume;
				}

				if (tempNoteInfo.pitchBend != currentPitchBend)
				{
					if ((pitchBendData[x].size() == 0) || (pitchBendData[x].back().absoluteTime < tempNoteInfo.startAbsoluteTime))
					{
						pitchBendData[x].push_back(TimeAndValue(tempNoteInfo.startAbsoluteTime, tempNoteInfo.pitchBend));
					}
					else
					{
						pitchBendData[x][pitchBendData[x].size()-1] = TimeAndValue(tempNoteInfo.startAbsoluteTime, tempNoteInfo.pitchBend);
					}

					currentPitchBend = tempNoteInfo.pitchBend;
				}
			}
		}

		int outputPosition = 0;

		unsigned char* outputBuffer = new unsigned char[0x100000];

		for (int x = 0; x < 0x100000; x++)
			outputBuffer[x] = 0x00;

		unsigned long trackOffsetsPointersStart = 0x00000000;
		unsigned long volumePointersStart = 0x00000000;
		unsigned long pitchBendPointersStart = 0x00000000;

		if (sngStyle == SngStyle::Normal)
		{
			WriteLongToBuffer(outputBuffer, outputPosition, 0x00000215);
			outputPosition += 4;

			WriteLongToBuffer(outputBuffer, outputPosition, numChannels);
			outputPosition += 4;

			WriteLongToBuffer(outputBuffer, outputPosition, instruments.size());
			outputPosition += 4;

			trackOffsetsPointersStart = 0x00000038;
			WriteLongToBuffer(outputBuffer, outputPosition, trackOffsetsPointersStart);
			outputPosition += 4;


			outputPosition = 0x38;

			// Track Offsets, 0x00000038
			outputPosition += (numChannels * 0x4);

			// Write Volume tracks
			WriteLongToBuffer(outputBuffer, 0x10, outputPosition);
			
			volumePointersStart = outputPosition;
			outputPosition += (numChannels * 0x4);

			// Write PitchBend tracks
			WriteLongToBuffer(outputBuffer, 0x14, outputPosition);

			pitchBendPointersStart = outputPosition;
			outputPosition += (numChannels * 0x4);

			// Write Instruments
			WriteLongToBuffer(outputBuffer, 0x20, outputPosition);

			unsigned long instrumentsPointersStart = outputPosition;

			for (int x = 0; x < instruments.size(); x++)
			{
				WriteShortToBuffer(outputBuffer, outputPosition, instruments[x]);
				outputPosition += 2;
			}

			// Skip Drums
			WriteLongToBuffer(outputBuffer, 0x1C, outputPosition);

			

			// Skip ADSR
			WriteLongToBuffer(outputBuffer, 0x18, outputPosition);

			// ADSR Rate
			outputBuffer[outputPosition++] = 0x01;

			// ADSR Start Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Attack Rate
			outputBuffer[outputPosition++] = 0x01;

			// ADSR Peak Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Decay Rate
			outputBuffer[outputPosition++] = 0x20;

			// ADSR Sustain Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Release Rate
			outputBuffer[outputPosition++] = 0x03;
		}
		else if (sngStyle == SngStyle::Old)
		{
			if ((numChannels % 8) != 0)
				numChannels = numChannels + (8 - (numChannels % 8));

			WriteLongToBuffer(outputBuffer, 0x0, numChannels);
			outputPosition += 4;

			// Track Offset
			trackOffsetsPointersStart = 0x00000018;
			WriteLongToBuffer(outputBuffer, 0x4, trackOffsetsPointersStart);

			outputPosition = 0x18;
			outputPosition += (numChannels * 0x4);

			// Write Volume tracks
			WriteLongToBuffer(outputBuffer, 0x8, outputPosition);

			volumePointersStart = outputPosition;
			outputPosition += (numChannels * 0x4);

			// Write PitchBend tracks
			WriteLongToBuffer(outputBuffer, 0xC, outputPosition);

			pitchBendPointersStart = outputPosition;
			outputPosition += (numChannels * 0x4);

			// Skip Drums
			WriteLongToBuffer(outputBuffer, 0x14, outputPosition);

			// Skip ADSR
			WriteLongToBuffer(outputBuffer, 0x10, outputPosition);

			// ADSR Rate
			outputBuffer[outputPosition++] = 0x01;

			// ADSR Start Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Attack Rate
			outputBuffer[outputPosition++] = 0x01;

			// ADSR Peak Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Decay Rate
			outputBuffer[outputPosition++] = 0x20;

			// ADSR Sustain Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Release Rate
			outputBuffer[outputPosition++] = 0x03;
		}
		else if (sngStyle == SngStyle::HexenSng)
		{
			if ((numChannels % 0x10) != 0)
				numChannels = numChannels + (0x10 - (numChannels % 0x10));

			WriteLongToBuffer(outputBuffer, 0x0, numChannels);
			outputPosition += 4;

			// Track Offset
			trackOffsetsPointersStart = 0x00000010;

			outputPosition = 0x10;
			outputPosition += (numChannels * 0x4);

			// Write Volume tracks
			WriteLongToBuffer(outputBuffer, 0x4, outputPosition);

			volumePointersStart = outputPosition;
			outputPosition += (numChannels * 0x4);

			// Write PitchBend tracks
			//WriteLongToBuffer(outputBuffer, 0xC, outputPosition);

			pitchBendPointersStart = -1;
			//outputPosition += (numChannels * 0x4);

			// Skip Drums
			WriteLongToBuffer(outputBuffer, 0xC, outputPosition);

			// Empty drums
			WriteLongToBuffer(outputBuffer, outputPosition, 0);
			outputPosition += 4;

			// Skip ADSR
			WriteLongToBuffer(outputBuffer, 0x8, outputPosition);

			// ADSR Rate
			outputBuffer[outputPosition++] = 0x01;

			// ADSR Start Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Attack Rate
			outputBuffer[outputPosition++] = 0x01;

			// ADSR Peak Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Decay Rate
			outputBuffer[outputPosition++] = 0x20;

			// ADSR Sustain Level
			outputBuffer[outputPosition++] = 0x7F;

			// ADSR Release Rate
			outputBuffer[outputPosition++] = 0x03;
		}
		else if (sngStyle == SngStyle::OldBfx)
		{
			WriteLongToBuffer(outputBuffer, 0x0, numChannels);
			WriteLongToBuffer(outputBuffer, 0x4, 0x00000000);

			// Write Volume tracks
			volumePointersStart = 0x00000010;
			WriteLongToBuffer(outputBuffer, 0x8, volumePointersStart);

			outputPosition = 0x10;

			for (int x = 0; x < numChannels; x++)
			{
				if (x == 0)
					WriteLongToBuffer(outputBuffer, 0x4, 0x00000000);
				else
					WriteLongToBuffer(outputBuffer, 0x4, 0x00000064);
				outputPosition += 4;
			}

			// Track Offset
			trackOffsetsPointersStart = outputPosition;
			WriteLongToBuffer(outputBuffer, 0xC, trackOffsetsPointersStart);

			outputPosition += (numChannels * 0x4);
		}
		else if (sngStyle == SngStyle::OldBfx)
		{
			WriteLongToBuffer(outputBuffer, 0x0, numChannels);
			WriteLongToBuffer(outputBuffer, 0x4, numChannels);
			WriteLongToBuffer(outputBuffer, 0x8, numChannels);
			WriteLongToBuffer(outputBuffer, 0xC, 0x00000000);
			WriteLongToBuffer(outputBuffer, 0x10, 0x00000000);

			outputPosition = 0x18;

			trackOffsetsPointersStart = outputPosition;

			outputPosition += 4;
			for (int x = 0; x < numChannels; x++)
			{
				WriteLongToBuffer(outputBuffer, 0x4, 0x00000064);
				outputPosition += 8;
			}
		}
		else if (sngStyle == SngStyle::PtrBfx)
		{
			return false;
		}

		

		// Data starts now
		unsigned long absoluteTime = 0;

		if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
		{
			for (int x = 0; x < numChannels; x++)
			{
				unsigned long volumeDataOffset = outputPosition;

				if (sngStyle == SngStyle::HexenSng)
				{
					if (volumeData[x].size() == 0)
					{
						volumeData[x].push_back(TimeAndValue(0, 0x7F));
					}
				}

				if (volumeData[x].size() == 0)
					WriteLongToBuffer(outputBuffer, (volumePointersStart + (x * 0x4)), 0x00000000);
				else
					WriteLongToBuffer(outputBuffer, (volumePointersStart + (x * 0x4)), volumeDataOffset);

				if (volumeData[x].size() > 0)
				{
					if (volumeData[x][0].absoluteTime > 0)
					{
						// Write initial length
						int totalVolumeLength = volumeData[x][0].absoluteTime;

						while (totalVolumeLength > 0)
						{
							// Default
							outputBuffer[outputPosition] = 0x7F;
					
							int volumeLength = totalVolumeLength;

							if (volumeLength > 0x8000)
								volumeLength = 0x8000;

							if (volumeLength == 1)
							{
								outputPosition++;
							}
							else
							{
								outputBuffer[outputPosition] |= 0x80;
								outputPosition++;
								WriteSngVariableLength(outputBuffer, outputPosition, (volumeLength - 2));
							}

							totalVolumeLength -= volumeLength;
						}
					}
				}

				for (int y = 0; y < volumeData[x].size(); y++)
				{
					outputBuffer[outputPosition] = volumeData[x][y].value;
					int totalVolumeLength;
					if (y != (volumeData[x].size()-1))
					{
						totalVolumeLength = volumeData[x][y+1].absoluteTime - volumeData[x][y].absoluteTime;
					}
					else
					{
						totalVolumeLength = highestAbsoluteTime;
					}

					while (totalVolumeLength > 0)
					{
						int volumeLength = totalVolumeLength;

						if (volumeLength > 0x8000)
							volumeLength = 0x8000;

						if (volumeLength == 1)
						{
							outputPosition++;
						}
						else
						{
							// Stored 0 value = 2, etc
							outputBuffer[outputPosition] |= 0x80;
							outputPosition++;
							WriteSngVariableLength(outputBuffer, outputPosition, (volumeLength - 2));
						}

						totalVolumeLength -= volumeLength;
					}
				}
			}





			if (sngStyle != SngStyle::HexenSng)		
			{
				absoluteTime = 0;

				for (int x = 0; x < numChannels; x++)
				{
					unsigned long pitchBendDataOffset = outputPosition;

					if (pitchBendData[x].size() == 0)
						WriteLongToBuffer(outputBuffer, (pitchBendPointersStart + (x * 0x4)), 0x00000000);
					else				
						WriteLongToBuffer(outputBuffer, (pitchBendPointersStart + (x * 0x4)), pitchBendDataOffset);

					if (pitchBendData[x].size() > 0)
					{
						if (pitchBendData[x][0].absoluteTime > 0)
						{
							// Write initial length
							int totalPitchBendLength = pitchBendData[x][0].absoluteTime;

							while (totalPitchBendLength > 0)
							{
								// Default
								outputBuffer[outputPosition] = 0x40;
						
								int pitchBendLength = totalPitchBendLength;

								if (pitchBendLength > 0x8000)
									pitchBendLength = 0x8000;

								if (pitchBendLength == 1)
								{
									outputPosition++;
								}
								else
								{
									outputBuffer[outputPosition] |= 0x80;
									outputPosition++;
									WriteSngVariableLength(outputBuffer, outputPosition, (pitchBendLength - 2));
								}

								totalPitchBendLength -= pitchBendLength;
							}
						}
					}

					for (int y = 0; y < pitchBendData[x].size(); y++)
					{
						outputBuffer[outputPosition] = pitchBendData[x][y].value;
						int totalPitchBendLength;
						if (y != (pitchBendData[x].size()-1))
						{
							totalPitchBendLength = pitchBendData[x][y+1].absoluteTime - pitchBendData[x][y].absoluteTime;
						}
						else
						{
							totalPitchBendLength = highestAbsoluteTime;
						}

						while (totalPitchBendLength > 0)
						{
							int pitchBendLength = totalPitchBendLength;

							if (pitchBendLength > 0x8000)
								pitchBendLength = 0x8000;

							if (pitchBendLength == 1)
							{
								outputPosition++;
							}
							else
							{
								outputBuffer[outputPosition] |= 0x80;
								outputPosition++;
								WriteSngVariableLength(outputBuffer, outputPosition, (pitchBendLength - 2));
							}

							totalPitchBendLength -= pitchBendLength;
						}
					}
				}
			}
		}
	


		if (sngStyle == SngStyle::Normal)
		{
			// Master Track Pointer
			WriteLongToBuffer(outputBuffer, 0x24, outputPosition);

			unsigned long masterTrackIndex = outputPosition;

			bool wroteMasterLoop = false;
			bool sharedMasterVelocity = false;

			unsigned long currentTempo = 0xFFFFFFFF;

			int countMasterTempoChanges = 0;

			for (int x = 0; x < tempoPositions.size(); x++)
			{
				if (currentTempo == tempoPositions[x].value)
					continue;

				countMasterTempoChanges++;
				currentTempo = tempoPositions[x].value;
			}

			currentTempo = 0xFFFFFFFF;

			absoluteTime = 0;

			if (masterTrackEffect != 0x00)
			{
				outputBuffer[outputPosition++] = 0xAA;
				outputBuffer[outputPosition++] = masterTrackEffect;
			}

			// Write Master Track
			for (int x = 0; x < tempoPositions.size(); x++)
			{
				if (currentTempo == tempoPositions[x].value)
					continue;

				if (loop && !wroteMasterLoop && ((loopPoint >= absoluteTime) && (loopPoint <= tempoPositions[x].absoluteTime)))
				{
					unsigned long delta = (loopPoint - absoluteTime);

					absoluteTime += delta;

					while (delta > 0)
					{
						outputBuffer[outputPosition++] = 0x60;
						if (!sharedMasterVelocity)
						{
							outputBuffer[outputPosition] = 0x00;
							if (countMasterTempoChanges > 3)
							{
								outputBuffer[outputPosition] |= 0x80;
								sharedMasterVelocity = true;
							}
							outputPosition++;
						}
							
						delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
					}

					outputBuffer[outputPosition++] = 0x95;
					outputBuffer[outputPosition++] = 0xFF;

					sharedMasterVelocity = false;
					outputBuffer[outputPosition++] = 0x99;

					wroteMasterLoop = true;
				}

				unsigned long delta = (tempoPositions[x].absoluteTime - absoluteTime);

				absoluteTime += delta;

				while (delta > 0)
				{
					outputBuffer[outputPosition++] = 0x60;
					if (!sharedMasterVelocity)
					{
						outputBuffer[outputPosition] = 0x00;
						if (countMasterTempoChanges > 3)
						{
							outputBuffer[outputPosition] |= 0x80;
							sharedMasterVelocity = true;
						}
						outputPosition++;
					}
							
					delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
				}


				outputBuffer[outputPosition++] = 0x85;
				outputBuffer[outputPosition++] = tempoPositions[x].value;

				currentTempo = tempoPositions[x].value;
			}

			if (loop && (!wroteMasterLoop))
			{
				unsigned long delta = (loopPoint - absoluteTime);

				absoluteTime += delta;

				while (delta > 0)
				{
					outputBuffer[outputPosition++] = 0x60;
					if (!sharedMasterVelocity)
					{
						outputBuffer[outputPosition] = 0x00;
						if (countMasterTempoChanges > 3)
						{
							outputBuffer[outputPosition] |= 0x80;
							sharedMasterVelocity = true;
						}
						outputPosition++;
					}
							
					delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
				}

				outputBuffer[outputPosition++] = 0x95;
				outputBuffer[outputPosition++] = 0xFF;

				sharedMasterVelocity = false;
				outputBuffer[outputPosition++] = 0x99;

				wroteMasterLoop = true;
			}

			unsigned long delta = (highestAbsoluteTime - absoluteTime);

			absoluteTime += delta;

			while (delta > 0)
			{
				outputBuffer[outputPosition++] = 0x60;
				if (!sharedMasterVelocity)
				{
					outputBuffer[outputPosition] = 0x00;
					if (countMasterTempoChanges > 3)
					{
						outputBuffer[outputPosition] |= 0x80;
						sharedMasterVelocity = true;
					}
					outputPosition++;
				}
							
				delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
			}

			// Seems like even if not turned on, turned off
			sharedMasterVelocity = false;
			outputBuffer[outputPosition++] = 0x99;


			if (wroteMasterLoop)
			{
				outputBuffer[outputPosition++] = 0x96;
			}
			else
			{
				outputBuffer[outputPosition++] = 0x80;
			}
		}


		for (int x = 0; x < numChannels; x++)
		{
			// Merge notes with same everything except volume and pitch bend
			for (int y = 0; y < channels[x].size(); y++)
			{
				SngNoteInfoMidiImport tempSongNote = channels[x][y];

				if (y == (channels[x].size() - 1))
					break;

				SngNoteInfoMidiImport tempSongNoteNext = channels[x][y + 1];

				if (
					(tempSongNote.endAbsoluteTime == tempSongNoteNext.startAbsoluteTime)
					&& (tempSongNote.effect == tempSongNoteNext.effect)
					&& (tempSongNote.instrument == tempSongNoteNext.instrument)
					&& (tempSongNote.noteNumber == tempSongNoteNext.noteNumber)
					&& (tempSongNote.pan == tempSongNoteNext.pan)
					&& (tempSongNote.velocity == tempSongNoteNext.velocity)
					&& (tempSongNote.originalTrack == tempSongNoteNext.originalTrack)
					&& (tempSongNote.originalController == tempSongNoteNext.originalController)
					&& (tempSongNote.originalNoteUniqueId == tempSongNoteNext.originalNoteUniqueId)
					)
				{
					// Merge
					channels[x][y].endAbsoluteTime = tempSongNoteNext.endAbsoluteTime;
					channels[x].erase(channels[x].begin() + y + 1);

					// Redo Note
					y--;
				}
			}
		}

		unsigned long currentOldSngTempo = 0xFFFFFFFF;

		for (int x = 0; x < numChannels; x++)
		{
			if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
			{
				if (channels[x].size() == 0)
				{
					WriteLongToBuffer(outputBuffer, trackOffsetsPointersStart + (x * 4), 0x00000000);	

					continue;
				}

				if (x == 0)
				{
					if (masterTrackEffect != 0x00)
					{
						outputBuffer[outputPosition++] = 0xAA;
						outputBuffer[outputPosition++] = masterTrackEffect;
					}
				}
			}

			if (sngStyle == SngStyle::Bfx)
			{
				WriteLongToBuffer(outputBuffer, trackOffsetsPointersStart + (x * 8), outputPosition);	
			}
			else if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng) || (sngStyle == SngStyle::OldBfx))
			{
				WriteLongToBuffer(outputBuffer, trackOffsetsPointersStart + (x * 4), outputPosition);	
			}

			absoluteTime = 0;
			
			if (channels[x].size() == 0)
			{
				bool sharedChannelVelocity = false;

				if ((sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::Bfx))
					sharedChannelVelocity = true;

				if (loop)
				{
					if (loopPoint == 0)
					{
						outputBuffer[outputPosition++] = 0x95;
						outputBuffer[outputPosition++] = 0xFF;

						if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							sharedChannelVelocity = false;
							outputBuffer[outputPosition++] = 0x99;
						}
					}
					else
					{
						unsigned long delta = loopPoint;

						absoluteTime += delta;

						while (delta > 0)
						{
							if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
							{
								if (!sharedChannelVelocity)
								{
									sharedChannelVelocity = true;
									outputBuffer[outputPosition++] = 0x9B;
									outputBuffer[outputPosition++] = 0x00;

									outputBuffer[outputPosition++] = 0x9A;
								}
							}

							outputBuffer[outputPosition++] = 0x60;
							if (sngStyle == SngStyle::Normal)
							{
								if (!sharedChannelVelocity)
								{
									sharedChannelVelocity = true;
									outputBuffer[outputPosition++] = 0x80;
								}
							}
							
							delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
						}

						outputBuffer[outputPosition++] = 0x95;
						outputBuffer[outputPosition++] = 0xFF;

						if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							sharedChannelVelocity = false;
							outputBuffer[outputPosition++] = 0x99;
						}
					}

					unsigned long delta = (highestAbsoluteTime - absoluteTime);

					absoluteTime += delta;

					while (delta > 0)
					{
						if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							if (!sharedChannelVelocity)
							{
								sharedChannelVelocity = true;

								outputBuffer[outputPosition++] = 0x9B;
								outputBuffer[outputPosition++] = 0x00;

								outputBuffer[outputPosition++] = 0x9A;
							}
						}

						outputBuffer[outputPosition++] = 0x60;
					
						if (!sharedChannelVelocity)
						{
							if (sngStyle == SngStyle::Normal)
							{
								outputBuffer[outputPosition++] = 0x80;
								sharedChannelVelocity = true;
							}
						}
							
						delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
					}

					if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						if (sharedChannelVelocity)
						{
							outputBuffer[outputPosition++] = 0x99;
							sharedChannelVelocity = false;
						}
					}

					outputBuffer[outputPosition++] = 0x96;
				}
				else
				{
					outputBuffer[outputPosition++] = 0x80;
				}
			}
			else
			{
				bool sharedChannelVelocity = false;
				unsigned char currentVelocity = 0x7F;

				bool setLength = false;
				int currentLength = 0;

				bool wroteChannelLoop = false;

				if (loop && !wroteChannelLoop && (loopPoint == 0))
				{
					outputBuffer[outputPosition++] = 0x95;
					outputBuffer[outputPosition++] = 0xFF;

					if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						sharedChannelVelocity = false;
						outputBuffer[outputPosition++] = 0x99;
					}

					if (sngStyle == SngStyle::Normal)
					{
						setLength = false;
						outputBuffer[outputPosition++] = 0xAC;
					}
					else if (sngStyle == SngStyle::Old)
					{
						setLength = false;
						outputBuffer[outputPosition++] = 0x8B;
						outputBuffer[outputPosition++] = 0x00;
					}

					wroteChannelLoop = true;
				}

				// Not sure what this does
				outputBuffer[outputPosition++] = 0x82;
				outputBuffer[outputPosition++] = 0x00;

				// Set Release Time Back to Full 01
				outputBuffer[outputPosition++] = 0x87;
				outputBuffer[outputPosition++] = 0x01;

				if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
				{
					// Reenable velocity on notes
					sharedChannelVelocity = false;
					outputBuffer[outputPosition++] = 0x99;

					if (sngStyle == SngStyle::Normal)
					{
						setLength = false;
						outputBuffer[outputPosition++] = 0xAC;
					}
					else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						setLength = false;
						outputBuffer[outputPosition++] = 0x8B;
						outputBuffer[outputPosition++] = 0x00;
					}
				}

				// Default Effect 00
				outputBuffer[outputPosition++] = 0xA2;
				outputBuffer[outputPosition++] = 0x00;

				// Transpose 00
				outputBuffer[outputPosition++] = 0x8D;
				outputBuffer[outputPosition++] = 0x00;

				if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
				{
					outputBuffer[outputPosition++] = 0x90;
					outputBuffer[outputPosition++] = 0x00;
				}
				else if ((sngStyle == SngStyle::Bfx) || (sngStyle == SngStyle::OldBfx))
				{
					outputBuffer[outputPosition++] = 0x84;
					outputBuffer[outputPosition++] = 0x01;
					outputBuffer[outputPosition++] = 0x7F;
					outputBuffer[outputPosition++] = 0x01;
					outputBuffer[outputPosition++] = 0x7F;
					outputBuffer[outputPosition++] = 0x01;
					outputBuffer[outputPosition++] = 0x7F;
					outputBuffer[outputPosition++] = 0x10;
				}

				unsigned short currentInstrument = 0xFFFF;
				unsigned char currentPan = 0x40;
				unsigned char currentEffect = 0xFF;

				int tempoIndex = 0;

				for (int y = 0; y < channels[x].size(); y++)
				{
					if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						if (x == 0)
						{
							if (tempoPositions.size() > 0)
							{
								for (int z = tempoIndex; z < tempoPositions.size(); z++)
								{
									if ((tempoPositions[z].absoluteTime >= absoluteTime) && (tempoPositions[z].absoluteTime <= channels[x][y].startAbsoluteTime))
									{
										if (loop && !wroteChannelLoop && ((loopPoint >= absoluteTime) && (loopPoint <= tempoPositions[z].absoluteTime)))
										{
											unsigned long delta = (loopPoint - absoluteTime);

											if (setLength)
											{
												if (delta != currentLength)
												{
													if (sngStyle == SngStyle::Normal)
													{
														setLength = false;
														outputBuffer[outputPosition++] = 0xAC;
													}
													else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
													{
														setLength = false;
														outputBuffer[outputPosition++] = 0x8B;
														outputBuffer[outputPosition++] = 0x00;
													}
												}
											}

											absoluteTime += delta;

											while (delta > 0)
											{
												outputBuffer[outputPosition++] = 0x60;

												if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
												{
													// Don't try and share, but if is, is ok
													if (!sharedChannelVelocity)
													{
														outputBuffer[outputPosition++] = 0x00;
														currentVelocity = 0x00;
													}
												}
												
												if (!setLength)
												{
													delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
												}
												else
												{
													delta = 0;
												}
											}

											outputBuffer[outputPosition++] = 0x95;
											outputBuffer[outputPosition++] = 0xFF;

											wroteChannelLoop = true;
										}

										unsigned long delta = (tempoPositions[z].absoluteTime - absoluteTime);

										if (setLength)
										{
											if (delta != currentLength)
											{
												if (sngStyle == SngStyle::Normal)
												{
													setLength = false;
													outputBuffer[outputPosition++] = 0xAC;
												}
												else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
												{
													setLength = false;
													outputBuffer[outputPosition++] = 0x8B;
													outputBuffer[outputPosition++] = 0x00;
												}
											}
										}

										absoluteTime += delta;

										while (delta > 0)
										{
											outputBuffer[outputPosition++] = 0x60;

											if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
											{
												// Don't try and share, but if is, is ok
												if (!sharedChannelVelocity)
												{
													outputBuffer[outputPosition++] = 0x00;
													currentVelocity = 0x00;
												}
											}
											
											if (!setLength)
											{
												delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
											}
											else
											{
												delta = 0;
											}
										}


										outputBuffer[outputPosition++] = 0x85;
										outputBuffer[outputPosition++] = tempoPositions[z].value;

										currentOldSngTempo = tempoPositions[z].value;

										tempoIndex = z + 1;
									}
									else if ((tempoPositions[z].absoluteTime >= absoluteTime) && (tempoPositions[z].absoluteTime >= channels[x][y].startAbsoluteTime) && (tempoPositions[z].absoluteTime < channels[x][y].endAbsoluteTime))
									{
										// Split note
										SngNoteInfoMidiImport tempSongInfoCopy = channels[x][y];	
										channels[x][y].endAbsoluteTime = tempoPositions[z].absoluteTime;
										tempSongInfoCopy.startAbsoluteTime = tempoPositions[z].absoluteTime;

										channels[x].insert(channels[x].begin() + y + 1, tempSongInfoCopy);
									}
									else
									{
										break;
									}
								}
							}
						}
					}

					// Split note out
					if (loop && !wroteChannelLoop && ((loopPoint > absoluteTime) && (loopPoint > channels[x][y].startAbsoluteTime) && (loopPoint < channels[x][y].endAbsoluteTime)))
					{
						SngNoteInfoMidiImport tempSongInfoCopy = channels[x][y];	
						channels[x][y].endAbsoluteTime = loopPoint;
						tempSongInfoCopy.startAbsoluteTime = loopPoint;

						channels[x].insert(channels[x].begin() + y + 1, tempSongInfoCopy);
					}

					if (loop && !wroteChannelLoop && ((loopPoint >= absoluteTime) && (loopPoint <= channels[x][y].startAbsoluteTime)))
					{
						unsigned long delta = (loopPoint - absoluteTime);

						if (setLength)
						{
							if (delta != currentLength)
							{
								if (sngStyle == SngStyle::Normal)
								{
									setLength = false;
									outputBuffer[outputPosition++] = 0xAC;
								}
								else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
								{
									setLength = false;
									outputBuffer[outputPosition++] = 0x8B;
									outputBuffer[outputPosition++] = 0x00;
								}
							}
						}

						absoluteTime += delta;

						while (delta > 0)
						{
							outputBuffer[outputPosition++] = 0x60;

							if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
							{
								// Don't try and share, but if is, is ok
								if (!sharedChannelVelocity)
								{
									outputBuffer[outputPosition++] = 0x00;
									currentVelocity = 0x00;
								}
							}
							
							if (!setLength)
							{
								delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
							}
							else
							{
								delta = 0;
							}
						}

						outputBuffer[outputPosition++] = 0x95;
						outputBuffer[outputPosition++] = 0xFF;

						if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							sharedChannelVelocity = false;
							outputBuffer[outputPosition++] = 0x99;

							if (sngStyle == SngStyle::Normal)
							{
								setLength = false;
								outputBuffer[outputPosition++] = 0xAC;
							}
							else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
							{
								setLength = false;
								outputBuffer[outputPosition++] = 0x8B;
								outputBuffer[outputPosition++] = 0x00;
							}
						}

						wroteChannelLoop = true;
					}

					// Write initial rest
					if (channels[x][y].startAbsoluteTime > absoluteTime)
					{
						int restLength = channels[x][y].startAbsoluteTime - absoluteTime;
						if (setLength)
						{
							if (restLength != currentLength)
							{
								int countSameLength = 1;
								unsigned long tempAbsoluteTime = absoluteTime;

								for (int z = y; z < channels[x].size(); z++)
								{
									if (
										((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == restLength)
										||
										((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == 0)
										)
									{
										
									}
									else
										break;

									tempAbsoluteTime = channels[x][z].startAbsoluteTime;

									if ((channels[x][z].endAbsoluteTime - tempAbsoluteTime) == restLength)
										countSameLength++;
									else
										break;

									tempAbsoluteTime = channels[x][z].endAbsoluteTime;
								}

								if ((countSameLength >= 3) && (restLength <= 0x7FFE))
								{
									outputBuffer[outputPosition++] = 0x8B;
									WriteSngVariableLength(outputBuffer, outputPosition, restLength);

									setLength = true;
									currentLength = restLength;
								}
								else
								{
									if (sngStyle == SngStyle::Normal)
									{
										outputBuffer[outputPosition++] = 0xAC;
									}
									else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
									{
										outputBuffer[outputPosition++] = 0x8B;
										outputBuffer[outputPosition++] = 0x00;
									}

									setLength = false;
								}
							}
						}
						else
						{
							int countSameLength = 1;
							unsigned long tempAbsoluteTime = absoluteTime;

							for (int z = y; z < channels[x].size(); z++)
							{
								if (
									((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == restLength)
									||
									((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == 0)
									)
								{
									
								}
								else
									break;

								tempAbsoluteTime = channels[x][z].startAbsoluteTime;

								if ((channels[x][z].endAbsoluteTime - tempAbsoluteTime) == restLength)
									countSameLength++;
								else
									break;

								tempAbsoluteTime = channels[x][z].endAbsoluteTime;
							}

							if ((countSameLength >= 3) && (restLength <= 0x7FFE))
							{
								outputBuffer[outputPosition++] = 0x8B;
								WriteSngVariableLength(outputBuffer, outputPosition, restLength);

								setLength = true;
								currentLength = restLength;
							}
						}
						
						int countSameVelocity = 0;
						for (int z = (y + 1); z < channels[x].size(); z++)
						{
							if ((channels[x][y].velocity) == channels[x][z].velocity)
								countSameVelocity++;
							else
								break;
						}

						if (!sharedChannelVelocity)
						{
							if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
							{
								if (countSameVelocity >= 3)
								{
									sharedChannelVelocity = true;
									currentVelocity = channels[x][y].velocity;

									outputBuffer[outputPosition++] = 0x9B;
									outputBuffer[outputPosition++] = currentVelocity;

									outputBuffer[outputPosition++] = 0x9A;
								}
							}
						}

						// Write rest
						outputBuffer[outputPosition++] = 0x60;
					
						if (!sharedChannelVelocity)
						{
							if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
							{
								// Should be same as previous velocity, the rest velocity
								outputBuffer[outputPosition] = channels[x][y].velocity;
								if (sngStyle == SngStyle::Normal)
								{
									if (countSameVelocity >= 3)
									{
										sharedChannelVelocity = true;
										currentVelocity = channels[x][y].velocity;
										outputBuffer[outputPosition] |= 0x80;
									}
								}
								outputPosition++;
							}
						}

						if (!setLength)
						{
							WriteSngVariableLength(outputBuffer, outputPosition, restLength);
						}

						absoluteTime += restLength;
					}

					SngNoteInfoMidiImport tempSongInfo = channels[x][y];	

					if (tempSongInfo.instrument != currentInstrument)
					{
						if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Bfx))
						{
							for (int z = 0; z < instruments.size(); z++)
							{
								if (instruments[z] == tempSongInfo.instrument)
								{
									outputBuffer[outputPosition++] = 0x81;
									outputBuffer[outputPosition++] = z;

									currentInstrument = instruments[z];

									break;
								}
							}
						}
						else if ((sngStyle == SngStyle::OldBfx) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							outputBuffer[outputPosition++] = 0x81;

							WriteSngVariableLength(outputBuffer, outputPosition, tempSongInfo.instrument);

							currentInstrument = tempSongInfo.instrument;
						}
					}

					int noteLength = tempSongInfo.endAbsoluteTime - tempSongInfo.startAbsoluteTime;
					if (setLength)
					{
						if (noteLength != currentLength)
						{
							int countSameLength = 1;
							unsigned long tempAbsoluteTime = absoluteTime;

							for (int z = (y + 1); z < channels[x].size(); z++)
							{
								if (
									((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == noteLength)
									||
									((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == 0)
									)
								{
									
								}
								else
									break;

								tempAbsoluteTime = channels[x][z].startAbsoluteTime;

								if ((channels[x][z].endAbsoluteTime - tempAbsoluteTime) == noteLength)
									countSameLength++;
								else
									break;

								tempAbsoluteTime = channels[x][z].endAbsoluteTime;
							}

							if ((countSameLength >= 3) && (noteLength <= 0x7FFE))
							{
								outputBuffer[outputPosition++] = 0x8B;
								WriteSngVariableLength(outputBuffer, outputPosition, noteLength);

								setLength = true;
								currentLength = noteLength;
							}
							else
							{
								if (sngStyle == SngStyle::Normal)
								{
									outputBuffer[outputPosition++] = 0xAC;
								}
								else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
								{
									outputBuffer[outputPosition++] = 0x8B;
									outputBuffer[outputPosition++] = 0x00;
								}
								setLength = false;
							}
						}
					}
					else
					{
						int countSameLength = 1;
						unsigned long tempAbsoluteTime = absoluteTime;

						for (int z = (y + 1); z < channels[x].size(); z++)
						{
							if (
								((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == noteLength)
								||
								((channels[x][z].startAbsoluteTime - tempAbsoluteTime) == 0)
								)
							{
								
							}
							else
								break;

							tempAbsoluteTime = channels[x][z].startAbsoluteTime;

							if ((channels[x][z].endAbsoluteTime - tempAbsoluteTime) == noteLength)
								countSameLength++;
							else
								break;

							tempAbsoluteTime = channels[x][z].endAbsoluteTime;
						}

						if ((countSameLength >= 3) && (noteLength <= 0x7FFE))
						{
							outputBuffer[outputPosition++] = 0x8B;
							WriteSngVariableLength(outputBuffer, outputPosition, noteLength);

							setLength = true;
							currentLength = noteLength;
						}
					}

					int countSameVelocity = 1;
					for (int z = (y + 1); z < channels[x].size(); z++)
					{
						if ((channels[x][z].velocity) == tempSongInfo.velocity)
							countSameVelocity++;
						else
							break;
					}

					if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						if (sharedChannelVelocity)
						{
							if (currentVelocity != tempSongInfo.velocity)
							{
								outputBuffer[outputPosition++] = 0x99;
								sharedChannelVelocity = false;
							}
						}
					}

					if (!sharedChannelVelocity)
					{
						if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							if (countSameVelocity >= 3)
							{
								sharedChannelVelocity = true;
								currentVelocity = channels[x][y].velocity;

								outputBuffer[outputPosition++] = 0x9B;
								outputBuffer[outputPosition++] = currentVelocity;

								outputBuffer[outputPosition++] = 0x9A;
							}
						}
					}

					if (tempSongInfo.effect != currentEffect)
					{
						outputBuffer[outputPosition++] = 0xA2;
						outputBuffer[outputPosition++] = tempSongInfo.effect;

						currentEffect = tempSongInfo.effect;
					}

					if (tempSongInfo.pan != currentPan)
					{
						outputBuffer[outputPosition++] = 0x9C;
						if (tempSongInfo.pan == 0x7F)
							outputBuffer[outputPosition++] = 255;
						else if (tempSongInfo.pan == 0x40)
							outputBuffer[outputPosition++] = 127;
						else
							outputBuffer[outputPosition++] = tempSongInfo.pan * 2;

						currentPan = tempSongInfo.pan;
					}

					// Write note
					if (tempSongInfo.noteNumber <= 0xC)
						outputBuffer[outputPosition++] = 0;
					else if (tempSongInfo.noteNumber >= 0x6B)
						outputBuffer[outputPosition++] = 0x5F;
					else
						outputBuffer[outputPosition++] = tempSongInfo.noteNumber - 0xC;

					if (!sharedChannelVelocity)
					{
						if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							outputBuffer[outputPosition] = tempSongInfo.velocity;
							if (sngStyle == SngStyle::Normal)
							{
								if (countSameVelocity >= 3)
								{
									sharedChannelVelocity = true;
									outputBuffer[outputPosition] |= 0x80;
								}
							}
							outputPosition++;
						}
					}

					if (!setLength)
					{
						WriteSngVariableLength(outputBuffer, outputPosition, noteLength);
					}

					absoluteTime += noteLength;
				}

				if (setLength)
				{
					if (sngStyle == SngStyle::Normal)
					{
						outputBuffer[outputPosition++] = 0xAC;
					}
					else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						outputBuffer[outputPosition++] = 0x8B;
						outputBuffer[outputPosition++] = 0x00;
					}
					setLength = false;
				}


				if (loop && (!wroteChannelLoop))
				{
					unsigned long delta = (loopPoint - absoluteTime);

					if (setLength)
					{
						if (delta != currentLength)
						{
							if (sngStyle == SngStyle::Normal)
							{
								setLength = false;
								outputBuffer[outputPosition++] = 0xAC;
							}
							else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
							{
								setLength = false;
								outputBuffer[outputPosition++] = 0x8B;
								outputBuffer[outputPosition++] = 0x00;
							}
						}
					}

					absoluteTime += delta;

					while (delta > 0)
					{
						outputBuffer[outputPosition++] = 0x60;
						if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							if (!sharedChannelVelocity)
							{
								outputBuffer[outputPosition++] = 0x00;
								currentVelocity = 0x00;
							}
						}
							
						if (!setLength)
						{
							delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
						}
						else
						{
							delta = 0;
						}
					}

					outputBuffer[outputPosition++] = 0x95;
					outputBuffer[outputPosition++] = 0xFF;

					if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						sharedChannelVelocity = false;
						outputBuffer[outputPosition++] = 0x99;

						if (sngStyle == SngStyle::Normal)
						{
							setLength = false;
							outputBuffer[outputPosition++] = 0xAC;
						}
						else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							setLength = false;
							outputBuffer[outputPosition++] = 0x8B;
							outputBuffer[outputPosition++] = 0x00;
						}
					}

					wroteChannelLoop = true;
				}

				unsigned long delta = (highestAbsoluteTime - absoluteTime);

				if (setLength)
				{
					if (delta != currentLength)
					{
						if (sngStyle == SngStyle::Normal)
						{
							setLength = false;
							outputBuffer[outputPosition++] = 0xAC;
						}
						else if ((sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
						{
							setLength = false;
							outputBuffer[outputPosition++] = 0x8B;
							outputBuffer[outputPosition++] = 0x00;
						}
					}
				}

				absoluteTime += delta;

				while (delta > 0)
				{
					outputBuffer[outputPosition++] = 0x60;
					if ((sngStyle == SngStyle::Normal) || (sngStyle == SngStyle::Old) || (sngStyle == SngStyle::HexenSng))
					{
						if (!sharedChannelVelocity)
						{
							outputBuffer[outputPosition++] = 0x00;
							currentVelocity = 0x00;
						}
					}
							
					if (!setLength)
					{
						delta = WriteSngVariableLength(outputBuffer, outputPosition, delta);
					}
					else
					{
						delta = 0;
					}
				}

				if (wroteChannelLoop)
				{
					outputBuffer[outputPosition++] = 0x96;
				}
				else
				{
					outputBuffer[outputPosition++] = 0x80;
				}
			}
		}

		if (sngStyle == SngStyle::Bfx)
		{
			WriteLongToBuffer(outputBuffer, 0x14, outputPosition);

			unsigned long instrumentsPointersStart = outputPosition;

			for (int x = 0; x < instruments.size(); x++)
			{
				WriteShortToBuffer(outputBuffer, outputPosition, instruments[x]);
				outputPosition += 2;
			}
		}

		FILE* outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			delete [] outputBuffer;

			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return false;
		}

		fwrite(outputBuffer, 1, outputPosition, outFile);

		delete [] outputBuffer;
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error converting", "Error", NULL);
		return false;
	}
	return true;
}

void CMidiParse::BTMidiToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool extendTracksToHighest, bool usePitchBendSensitity, int pitchBendSensitity)
{
	numberInstruments = 0;
	try
	{
		FILE* outFile = fopen(outFileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		// parse midi
		
		int trackSize = CharArrayToLong(&inputMID[0x4]);

		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | trackSize);
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned long division = CharArrayToLong(&inputMID[0x0]);

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		int counterTrack = 0;

		int highestTrackLength = 0;

		for (int iii = 0; iii < trackSize; iii++)
		{
			unsigned long absoluteTime = 0;

			unsigned long offset = CharArrayToLong(&inputMID[(iii * 4) + 0x8]);
			
			int position = offset;	

			
			if (position != 0)
			{
				int previousEventValue = 0;

				std::map<int, int> loopEndsWithCount;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;

				while ((position < inputSize) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					absoluteTime += timeTag;

					if (absoluteTime > highestTrackLength)
						highestTrackLength = absoluteTime;


					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
							if ((loopCount == 0xFF) || (loopCount == 0x00))
							{
								
							}
							else
							{
								std::map<int, int>::iterator it = loopEndsWithCount.find(position);
								if (it != loopEndsWithCount.end())
								{
									int countLeft = it->second;

									if (countLeft == 0)
									{
										loopEndsWithCount.erase(it);
									}
									else
									{
										loopEndsWithCount[position] = (countLeft - 1);

										if (repeatPattern == NULL)
										{
											position = position - offsetToBeginningLoop;
										}
										else
										{
											loopEndsWithCount.erase(it);
										}
									}
								}
								else
								{
									loopEndsWithCount[position] = loopCount - 1;

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
						}
						else if (subType == 0x2F)
						{
							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						
					}
				}
			}
		}

		for (int iii = 0; iii < trackSize; iii++)
		{
			unsigned long absoluteTime = 0;

			int trackEventCountSub = 0;
			TrackEvent* trackEventsSub = new TrackEvent[0x30000];

			for (int j = 0; j < 0x30000; j++)
			{
				trackEventsSub[j].contents = NULL;
				trackEventsSub[j].contentSize = 0;
				trackEventsSub[j].obsoleteEvent = false;
				trackEventsSub[j].deltaTime = 0;
				trackEventsSub[j].absoluteTime = 0;
			}

			unsigned long offset = CharArrayToLong(&inputMID[(iii * 4) + 0x8]);
			
			int position = offset;	

			if (position != 0)
			{
				tempLong = Flip32Bit(0x4D54726B);
				fwrite(&tempLong, 1 ,4 , outFile);
	
				int previousEventValue = 0;

				std::vector<int> loopNumbers;
				std::map<int, int> loopEndsWithCount;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				if (usePitchBendSensitity)
				{
					//https://www.midikits.net/midi_analyser/pitch_bend.htm

					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x64;
					trackEventsSub[trackEventCountSub].contents[1] = 0x00;

					trackEventCountSub++;


					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x65;
					trackEventsSub[trackEventCountSub].contents[1] = 0x00;

					trackEventCountSub++;


					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x06;
					if (pitchBendSensitity > 0x18)
						pitchBendSensitity = 0x18;
					trackEventsSub[trackEventCountSub].contents[1] = pitchBendSensitity;

					trackEventCountSub++;

					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x64;
					trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

					trackEventCountSub++;


					trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
					trackEventsSub[trackEventCountSub].contentSize = 2;
					trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
					
					trackEventsSub[trackEventCountSub].contents[0] = 0x65;
					trackEventsSub[trackEventCountSub].contents[1] = 0x7F;

					trackEventCountSub++;
				}

				bool endFlag = false;
				while ((position < inputSize) && !endFlag)
				{
					if (extendTracksToHighest)
					{
						if (absoluteTime >= highestTrackLength)
						{
							trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
							trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;

							break;
						}
					}

					int timePosition = position;

					unsigned long original;
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					if (extendTracksToHighest)
					{
						if ((absoluteTime + timeTag) > highestTrackLength)
						{
							trackEventsSub[trackEventCountSub].absoluteTime = highestTrackLength;
							trackEventsSub[trackEventCountSub].deltaTime = (highestTrackLength - absoluteTime);

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;

							break;
						}
					}

					trackEventsSub[trackEventCountSub].deltaTime += timeTag;

					absoluteTime += timeTag;
					trackEventsSub[trackEventCountSub].absoluteTime = absoluteTime;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 5;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x51;
							trackEventsSub[trackEventCountSub].contents[1] = 0x3;
							trackEventsSub[trackEventCountSub].contents[2] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
							trackEventsSub[trackEventCountSub].contents[3] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
							trackEventsSub[trackEventCountSub].contents[4] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

							trackEventCountSub++;

							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));

							int loopNumber = 0;
							if (loopNumbers.size() > 0)
							{
								loopNumber = loopNumbers.back();
								loopNumbers.pop_back();
							}

							// Fake loop end, controller 103
							trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							trackEventsSub[trackEventCountSub].contents[0] = 103;
							trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
							trackEventCountSub++;

							if ((loopCount == 0xFF) || (loopCount == 0x00))
							{
								hasLoopPoint = true;
								loopEnd = absoluteTime;

								if (extendTracksToHighest)
								{
									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
								}
							}
							else
							{
								std::map<int, int>::iterator it = loopEndsWithCount.find(position);
								if (it != loopEndsWithCount.end())
								{
									int countLeft = it->second;

									if (countLeft == 0)
									{
										loopEndsWithCount.erase(it);
									}
									else
									{
										loopEndsWithCount[position] = (countLeft - 1);

										if (repeatPattern == NULL)
										{
											position = position - offsetToBeginningLoop;
										}
										else
										{
											loopEndsWithCount.erase(it);
										}
									}
								}
								else
								{
									loopEndsWithCount[position] = loopCount - 1;

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
							
							// Fake loop start, controller 102
							trackEventsSub[trackEventCountSub].type = 0xB0 | ((iii / 4) & 0xF);
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							trackEventsSub[trackEventCountSub].contents[0] = 102;
							trackEventsSub[trackEventCountSub].contents[1] = loopNumber;
							trackEventCountSub++;

							hasLoopPoint = true;
							loopStart = absoluteTime;

							loopNumbers.push_back(loopNumber);
						}
						else if (subType == 0x2F)
						{
							trackEventsSub[trackEventCountSub].type = 0xFF;
							trackEventsSub[trackEventCountSub].contentSize = 2;
							trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
							
							trackEventsSub[trackEventCountSub].contents[0] = 0x2F;
							trackEventsSub[trackEventCountSub].contents[1] = 0x0;

							trackEventCountSub++;

							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							trackEventsSub[trackEventCountSub].type = previousEventValue;
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							trackEventsSub[trackEventCountSub].type = eventVal;
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						

						trackEventsSub[trackEventCountSub].durationTime = timeDuration; // to be filled in
						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = noteNumber;
						trackEventsSub[trackEventCountSub].contents[1] = velocity;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = controllerType;
						trackEventsSub[trackEventCountSub].contents[1] = controllerValue;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}

						trackEventsSub[trackEventCountSub].contentSize = 1;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = instrument;
						if (instrument >= numberInstruments)
							numberInstruments = (instrument + 1);

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}

						trackEventsSub[trackEventCountSub].contentSize = 1;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = amount;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							trackEventsSub[trackEventCountSub].type = previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEventsSub[trackEventCountSub].type = eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEventsSub[trackEventCountSub].contentSize = 2;
						trackEventsSub[trackEventCountSub].contents = new byte[trackEventsSub[trackEventCountSub].contentSize];
						trackEventsSub[trackEventCountSub].contents[0] = valueLSB;
						trackEventsSub[trackEventCountSub].contents[1] = valueMSB;

						trackEventCountSub++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
					}

				}

				for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
				{
					TrackEvent trackEvent = trackEventsSub[eventCount];
					if ((trackEvent.type >= 0x90) && (trackEvent.type < 0xA0))
					{
						// need to split out
						if (trackEvent.durationTime > 0)
						{

							unsigned long shutoffTime = (trackEvent.absoluteTime + trackEvent.durationTime);

							if (eventCount != (trackEventCountSub - 1))
							{

								for (int i = (eventCount+1); i < trackEventCountSub; i++)
								{
									if ((trackEventsSub[i].absoluteTime >= shutoffTime) && (i != (trackEventCountSub - 1)))
									{
										for (int j = (trackEventCountSub - 1); j >= i; j--)
										{
											trackEventsSub[j+1].absoluteTime = trackEventsSub[j].absoluteTime;
											trackEventsSub[j+1].contentSize = trackEventsSub[j].contentSize;
 											if (trackEventsSub[j+1].contents != NULL)
											{
												delete [] trackEventsSub[j+1].contents;
												trackEventsSub[j+1].contents = NULL;
											}
											trackEventsSub[j+1].contents = new byte[trackEventsSub[j].contentSize];
											for (int r = 0; r < trackEventsSub[j].contentSize; r++)
											{
												trackEventsSub[j+1].contents[r] = trackEventsSub[j].contents[r];
											}
											trackEventsSub[j+1].deltaTime = trackEventsSub[j].deltaTime;
											trackEventsSub[j+1].durationTime = trackEventsSub[j].durationTime;
											trackEventsSub[j+1].obsoleteEvent = trackEventsSub[j].obsoleteEvent;
											trackEventsSub[j+1].type = trackEventsSub[j].type;
										}

										trackEventsSub[i].type = trackEventsSub[eventCount].type;
										trackEventsSub[i].absoluteTime = shutoffTime;
										trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i-1].absoluteTime);
										trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
										trackEventsSub[i].durationTime = 0;
										trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
										trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
										trackEventsSub[i].contents[1] = 0;

										trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);

										if (trackEventsSub[i].deltaTime > 0xFF000000)
										{
											int a =1;
										}

										trackEventCountSub++;
										break;
									}
									else if (i == (trackEventCountSub - 1))
									{
										trackEventsSub[i+1].absoluteTime = shutoffTime; // move end to end
										trackEventsSub[i+1].contentSize = trackEventsSub[i].contentSize;
										if (trackEventsSub[i+1].contents != NULL)
										{
											delete [] trackEventsSub[i+1].contents;
											trackEventsSub[i+1].contents = NULL;
										}
										trackEventsSub[i+1].contents = new byte[trackEventsSub[i].contentSize];
										for (int r = 0; r < trackEventsSub[i].contentSize; r++)
										{
											trackEventsSub[i+1].contents[r] = trackEventsSub[i].contents[r];
										}
										trackEventsSub[i+1].deltaTime = trackEventsSub[i].deltaTime;
										trackEventsSub[i+1].durationTime = trackEventsSub[i].durationTime;
										trackEventsSub[i+1].obsoleteEvent = trackEventsSub[i].obsoleteEvent;
										trackEventsSub[i+1].type = trackEventsSub[i].type;


										trackEventsSub[i].type = trackEventsSub[eventCount].type;
										trackEventsSub[i].absoluteTime = shutoffTime;
										trackEventsSub[i].deltaTime = (trackEventsSub[i].absoluteTime - trackEventsSub[i - 1].absoluteTime);
										trackEventsSub[i].contentSize = trackEventsSub[eventCount].contentSize;
										trackEventsSub[i].durationTime = 0;
										trackEventsSub[i].contents = new byte[trackEventsSub[i].contentSize];
										trackEventsSub[i].contents[0] = trackEventsSub[eventCount].contents[0];
										trackEventsSub[i].contents[1] = 0;

										trackEventsSub[i+1].deltaTime = (trackEventsSub[i+1].absoluteTime - trackEventsSub[i].absoluteTime);
										if (trackEventsSub[i].deltaTime > 0xFF000000)
										{
											int a =1;
										}
										trackEventCountSub++;
										break;
									}
								}
							}
							else
							{
								trackEventsSub[eventCount+1].absoluteTime = shutoffTime; // move end to end
								trackEventsSub[eventCount+1].contentSize = trackEventsSub[eventCount].contentSize;
								if (trackEventsSub[eventCount+1].contents != NULL)
								{
									delete [] trackEventsSub[eventCount+1].contents;
									trackEventsSub[eventCount+1].contents = NULL;
								}
								trackEventsSub[eventCount+1].contents = new byte[trackEventsSub[eventCount].contentSize];
								for (int r = 0; r < trackEventsSub[eventCount].contentSize; r++)
								{
									trackEventsSub[eventCount+1].contents[r] = trackEventsSub[eventCount].contents[r];
								}
								trackEventsSub[eventCount+1].deltaTime = trackEventsSub[eventCount].deltaTime;
								trackEventsSub[eventCount+1].durationTime = trackEventsSub[eventCount].durationTime;
								trackEventsSub[eventCount+1].obsoleteEvent = trackEventsSub[eventCount].obsoleteEvent;
								trackEventsSub[eventCount+1].type = trackEventsSub[eventCount].type;


								trackEventsSub[eventCount].type = trackEventsSub[eventCount].type;
								trackEventsSub[eventCount].absoluteTime = shutoffTime;
								if ((trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime) > 0xFF000000)
								{
									int a =1;
								}
								trackEventsSub[eventCount].deltaTime = (trackEventsSub[eventCount].absoluteTime - trackEventsSub[eventCount - 1].absoluteTime);
								trackEventsSub[eventCount].contentSize = trackEventsSub[eventCount].contentSize;
								trackEventsSub[eventCount].durationTime = 0;
								trackEventsSub[eventCount].contents = new byte[trackEventsSub[eventCount].contentSize];
								trackEventsSub[eventCount].contents[0] = trackEventsSub[eventCount].contents[0];
								trackEventsSub[eventCount].contents[1] = 0;

								trackEventsSub[eventCount+1].deltaTime = (trackEventsSub[eventCount+1].absoluteTime - trackEventsSub[eventCount].absoluteTime);
								if (trackEventsSub[eventCount].deltaTime > 0xFF000000)
								{
									int a =1;
								}
								trackEventCountSub++;
							}
						}
					}
				}

				
				unsigned long timeOffset = 0;

				unsigned long sizeData = 0;
				byte previousTrackEvent = 0x0;

				
				for (int j = 0; j < trackEventCountSub; j++)
				{
					TrackEvent trackEvent =  trackEventsSub[j];
					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;

						sizeData += lengthTimeDelta;


						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							sizeData += 1;
						}

						sizeData += trackEvent.contentSize;

						previousTrackEvent = trackEvent.type;
					}
				}

				tempLong = Flip32Bit(sizeData);
				fwrite(&tempLong,1, 4, outFile);

				timeOffset = 0;
				previousTrackEvent = 0x0;
				for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
				{
					TrackEvent trackEvent = trackEventsSub[eventCount];

					if (trackEvent.obsoleteEvent)
					{
						timeOffset += trackEvent.deltaTime;
					}
					else
					{
						unsigned long lengthTimeDelta = 0;
						unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
						timeOffset = 0;
						WriteVLBytes(outFile, timeDelta, lengthTimeDelta, true);

						if ((trackEvent.type != previousTrackEvent) || (trackEvent.type == 0xFF))
						{
							fwrite(&trackEvent.type, 1, 1, outFile);
						}

						fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

						previousTrackEvent = trackEvent.type;
					}
				}

				for (int eventCount = 0; eventCount < trackEventCountSub; eventCount++)
				{
					if (trackEventsSub[eventCount].contents != NULL)
					{
						delete [] trackEventsSub[eventCount].contents;
						trackEventsSub[eventCount].contents = NULL;
					}
				}
			}
			else
			{
				
			}

			counterTrack++;
			delete [] trackEventsSub;
		}



	

		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}


bool CMidiParse::MidiToBTFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters)
{
	numberTracks = 0;
	for (unsigned int x = 0; x < 0x20; x++)
	{
		for (int y = 0; y < 0x30000; y++)
		{
			if (trackEvents[x][y].contents != NULL)
			{
				delete[] trackEvents[x][y].contents;
				trackEvents[x][y].contents = NULL;
			}
		}
		trackEventCount[x] = 0;
	}

	CString tempOutput = (output + "temp.bin");

	unsigned short numberTracks = 0x20;

	if (MidiToBTFormatStageOne(input, tempOutput, loop, loopPoint, useRepeaters, numberTracks))
	{
		FILE* inFile = fopen(tempOutput, "rb");
		if (inFile == NULL)
		{
			MessageBox(NULL, "Error opening temp file", "Error", NULL);
			return false;
		}

		fseek(inFile, 0, SEEK_END);
		int sizeOut = ftell(inFile);
		rewind(inFile);
		byte* inArray = new byte[sizeOut];
		fread(inArray, 1, sizeOut, inFile);
		fclose(inFile);

		::DeleteFile(tempOutput);

		FILE* outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			delete [] inArray;
			MessageBox(NULL, "Error opening output", "Error", NULL);
			return false;
		}

		unsigned long speed = CharArrayToLong(&inArray[0x80]);

		

		unsigned long offsetheader[0x20];
		
		for (int x = 0; x < 0x80; x+=4)
		{
			offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
			if (offsetheader[x/4] == 0)
			{
				break;
			}
		}

		unsigned long lessOffset = 0x84 - ((numberTracks * 0x4) + 0x8);

		unsigned long tempLong = Flip32Bit(speed);
		fwrite(&tempLong, 1, 4, outFile);

		tempLong = Flip32Bit((unsigned long)numberTracks);
		fwrite(&tempLong, 1, 4, outFile);

		for (int x = 0; x < numberTracks; x++)
		{
			tempLong = Flip32Bit(((unsigned long)offsetheader[x] - lessOffset));
			fwrite(&tempLong, 1, 4, outFile);
		}

		for (int x = 0x84; x < sizeOut; x++)
		{
			fwrite(&inArray[x], 1, 1, outFile);
		}

		fclose(outFile);
	}
	else
	{
		return false;
	}

	return true;
}

bool CMidiParse::AddLoopGEFormat(byte* inputMID, CString output, int inputSize, bool loop, unsigned long loopPoint, bool useRepeaters)
{
	unsigned long highestAbsoluteTime = 0;
	unsigned long highestAbsoluteTimeByTrack[0x10];
	for (int x = 0; x < 0x10; x++)
		highestAbsoluteTimeByTrack[x] = 0;

	unsigned long division = CharArrayToLong(&inputMID[0x40]);

	int numberInstruments = 0;
	numberTracks = 0x10;

	for (unsigned int x = 0; x < 0x20; x++)
	{
		for (int y = 0; y < 0x30000; y++)
		{
			if (trackEvents[x][y].contents != NULL)
			{
				delete[] trackEvents[x][y].contents;
				trackEvents[x][y].contents = NULL;
			}
		}
		trackEventCount[x] = 0;
	}

	try
	{
		unsigned long lengthHeader = 0x44;

		// parse midi
		
		int trackSize = 0;
		for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
		{
			unsigned long offset = CharArrayToLong(&inputMID[i]);
			if (offset != 0)
				trackSize++;
		}




		for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			int trackNum = iii / 4;

			unsigned long absoluteTime = 0;

			unsigned long offset = CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			if (position != 0)
			{
				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;
				while ((position < inputSize) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					// trackEvents[trackEventCount].deltaTime is for loops
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					absoluteTime += timeTag;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							if (endLoop != 0xFF)
							{
								// is this used?
							}
						}
						else if (subType == 0x2F)
						{
							endFlag = true;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						
					}
				}
			}

			if (absoluteTime > highestAbsoluteTime)
			{
				highestAbsoluteTime = absoluteTime;
			}

			if (absoluteTime > highestAbsoluteTimeByTrack[trackNum])
			{
				highestAbsoluteTimeByTrack[trackNum] = absoluteTime;
			}
		}



		int counterTrack = 0;

		for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
		{
			counterTrack = iii / 4;
			unsigned long absoluteTime = 0;

			trackEventCount[counterTrack] = 0;

			for (int j = 0; j < 0x30000; j++)
			{
				trackEvents[counterTrack][j].contents = NULL;
				trackEvents[counterTrack][j].obsoleteEvent = false;
				trackEvents[counterTrack][j].deltaTime = 0;
				trackEvents[counterTrack][j].absoluteTime = 0;

			}

			unsigned long offset = CharArrayToLong(&inputMID[iii]);
			
			int position = offset;	

			if (position != 0)
			{
				bool didLoop = false;

				if ((loop) && (loopPoint == 0) && (highestAbsoluteTimeByTrack[counterTrack] > 0))
				{
					TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[counterTrack][trackEventCount[counterTrack]]);
					newTrackEvent->type = 0xFF;
					newTrackEvent->absoluteTime = 0;
					newTrackEvent->contentSize = 3;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = 0x2E;
					newTrackEvent->contents[1] = 0x00;
					newTrackEvent->contents[2] = 0xFF;
					newTrackEvent->deltaTime = 0;
					newTrackEvent->obsoleteEvent = false;
					
					trackEventCount[counterTrack]++;
					didLoop = true;
				}

				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				bool endFlag = false;
				while ((position < inputSize) && !endFlag)
				{
					if (trackEventCount[counterTrack] >= 0x30000)
						return false;

					int timePosition = position;

					unsigned long original;
					// trackEvents[counterTrack][trackEventCount[counterTrack]].deltaTime is for loops
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					trackEvents[counterTrack][trackEventCount[counterTrack]].deltaTime += timeTag;

					absoluteTime += timeTag;
					trackEvents[counterTrack][trackEventCount[counterTrack]].absoluteTime = absoluteTime;

					int vlLength = 0;
					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
					}
					else
					{
						statusBit = false;
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (subType == 0x51) // tempo
						{
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

							trackEvents[counterTrack][trackEventCount[counterTrack]].type = 0xFF;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 4;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
							
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = 0x51;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = ((microsecondsSinceQuarterNote >> 16) & 0xFF);
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[2] = ((microsecondsSinceQuarterNote >> 8) & 0xFF);
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[3] = ((microsecondsSinceQuarterNote >> 0) & 0xFF);

							trackEventCount[counterTrack]++;

							
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;
						}
						else if (subType == 0x2D) // end loop
						{
							byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
							
						}
						else if (subType == 0x2E) // start loop
						{
							byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							if (endLoop != 0xFF)
							{
								// is this used?
							}
						}
						else if (subType == 0x2F)
						{
							TrackEvent* newTrackEvent = ((TrackEvent*)&(trackEvents[counterTrack][trackEventCount[counterTrack]]));

							endFlag = true;

							if (trackEventCount[counterTrack] > 0)
							{
								if (loop)
								{

									TrackEvent* prevEvent = ((TrackEvent*)&(trackEvents[counterTrack][trackEventCount[counterTrack]-1]));

									newTrackEvent->type = 0xFF;
									if (highestAbsoluteTime > (prevEvent->absoluteTime))
									{
										newTrackEvent->deltaTime = (highestAbsoluteTime - (prevEvent->absoluteTime));
										newTrackEvent->absoluteTime = highestAbsoluteTime;
									}
									else
									{
										newTrackEvent->deltaTime = 0;
										newTrackEvent->absoluteTime = prevEvent->absoluteTime;
									}

									newTrackEvent->contentSize = 7;
									newTrackEvent->contents = new byte[newTrackEvent->contentSize];
									newTrackEvent->contents[0] = 0x2D;
									newTrackEvent->contents[1] = 0xFF;
									newTrackEvent->contents[2] = 0xFF;
									newTrackEvent->contents[3] = 0x0; // todo write location
									newTrackEvent->contents[4] = 0x0;
									newTrackEvent->contents[5] = 0x0;
									newTrackEvent->contents[6] = 0x0;
									newTrackEvent->obsoleteEvent = false;
									
									trackEventCount[counterTrack]++;
								
									endFlag = true;
								}
								else
								{
									newTrackEvent->contentSize = 7;
									newTrackEvent->contents = new byte[newTrackEvent->contentSize];
									newTrackEvent->contents[0] = 0x2D;
									newTrackEvent->contents[1] = 0xFF;
									newTrackEvent->contents[2] = 0xFF;
									newTrackEvent->contents[3] = 0x0; // todo write location
									newTrackEvent->contents[4] = 0x0;
									newTrackEvent->contents[5] = 0x0;
									newTrackEvent->contents[6] = 0x0;
									newTrackEvent->obsoleteEvent = false;
									
									trackEventCount[counterTrack]++;
								}
							}

							trackEvents[counterTrack][trackEventCount[counterTrack]].type = 0xFF;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
							
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = 0x2F;
							trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = 0x0;
							trackEventCount[counterTrack]++;
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte curEventVal;

						byte noteNumber;
						if (statusBit)
						{
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
							noteNumber = eventVal;
							curEventVal = previousEventValue;
						}
						else
						{
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							curEventVal = eventVal;
						}
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

						

						trackEvents[counterTrack][trackEventCount[counterTrack]].durationTime = timeDuration; // to be filled in
						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = noteNumber;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = velocity;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						
						if (statusBit)
						{
							controllerType = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = controllerType;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = controllerValue;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
						{
							instrument = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 1;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = instrument;
						if (instrument >= numberInstruments)
							numberInstruments = (instrument + 1);

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
						{
							amount = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 1;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = amount;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte valueLSB;
						if (statusBit)
						{
							valueLSB = eventVal;
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = previousEventValue;
						}
						else
						{
							valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
							trackEvents[counterTrack][trackEventCount[counterTrack]].type = eventVal;
						}
						
						byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize = 2;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents = new byte[trackEvents[counterTrack][trackEventCount[counterTrack]].contentSize];
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[0] = valueLSB;
						trackEvents[counterTrack][trackEventCount[counterTrack]].contents[1] = valueMSB;

						trackEventCount[counterTrack]++;

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						
					}
				}
			}
		}

	}
	catch(...)
	{
		MessageBox(NULL, "Error processing inpt file", "Error", NULL);
		return false;
	}


		FILE* outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return false;
		}


		unsigned long timeOffset = 0;

	unsigned long startPosition = 0x44;

	// get offsets
	for (int i = 0; i < numberTracks; i++)
	{
		unsigned long sizeData = 0;
		int loopStartPosition = 0;
		bool foundLoopStart = false;
		byte previousTrackEvent = 0x0;

		if (trackEventCount[i] > 0)
		{
			unsigned long tempLong = Flip32Bit(startPosition);
			fwrite(&tempLong, 1, 4, outFile);

			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);

				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2E))
					{
						foundLoopStart = true;
						loopStartPosition = (startPosition + sizeData + 1 + trackEvent->contentSize + lengthTimeDelta);
					}
					
					timeOffset = 0;

					sizeData += lengthTimeDelta;

					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2D))
					{
						unsigned long offsetBack = ((startPosition + sizeData) - loopStartPosition + 8);
						trackEvent->contents[3] = ((offsetBack >> 24) & 0xFF);
						trackEvent->contents[4] = ((offsetBack >> 16) & 0xFF);
						trackEvent->contents[5] = ((offsetBack >> 8) & 0xFF);
						trackEvent->contents[6] = ((offsetBack >> 0) & 0xFF);
					}

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						sizeData += 1;
					}

					sizeData += trackEvent->contentSize;

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes);

						sizeData += lengthDurationBytes;
					}

					previousTrackEvent = trackEvent->type;
				}
			}
			startPosition += sizeData;
		}
		else
		{
			unsigned long zero = 0;
			fwrite(&zero, 1, 4, outFile);
		}
	}

	for (int i = numberTracks; i < 16; i++)
	{
		unsigned long zero = 0;
		fwrite(&zero, 1, 4, outFile);
	}

	unsigned long divisionFlipped = Flip32Bit((unsigned long)division);
	fwrite(&divisionFlipped, 1, 4, outFile);

	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	for (int i = 0; i < numberTracks; i++)
	{
		if (trackEventCount[i] > 0)
		{
			if (outDebug != NULL) 
				fprintf(outDebug, "Track %X\n", i);
			byte previousTrackEvent = 0x0;
			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					unsigned long lengthTimeDelta = 0;
					unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);
					timeOffset = 0;
					WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

					for (int ii = 0; ii < lengthTimeDelta; ii++)
						if (outDebug != NULL) fprintf(outDebug, "%02X", ((timeDelta >> ((lengthTimeDelta * 8) - 8 - (ii * 8))) & 0xFF));

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						fwrite(&trackEvent->type, 1, 1, outFile);
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->type);
					}

					fwrite(trackEvent->contents, 1, trackEvent->contentSize, outFile);
					for (int ii = 0; ii < trackEvent->contentSize; ii++)
					{
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->contents[ii]);
					}

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes); // todo real trackevent
						WriteVLBytes(outFile, duration, lengthDurationBytes, false);
						for (int ii = 0; ii < lengthDurationBytes; ii++)
							if (outDebug != NULL) fprintf(outDebug, "%02X", ((duration >> ((lengthDurationBytes * 8) - 8 - (ii * 8))) & 0xFF));
					}

					if (outDebug != NULL) fprintf(outDebug, "\n");
					previousTrackEvent = trackEvent->type;
				}
				
			}
		}

		for (int j = 0; j < trackEventCount[i]; j++)
		{
			if (trackEvents[i][j].contents != NULL)
			{
				delete [] trackEvents[i][j].contents;
				trackEvents[i][j].contents = NULL;
			}
		}
	}

	fflush(outDebug);

	fclose(outFile);

	// write FEFE in case see FE
	outFile = fopen(output, "rb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	
	fseek(outFile, 0, SEEK_END);
	int sizeOut = ftell(outFile);
	rewind(outFile);
	byte* inArray = new byte[sizeOut];
	fread(inArray, 1, sizeOut, outFile);
	fclose(outFile);

	unsigned long offsetheader[0x10];
	int extraOffsets[0x10];
	for (int x = 0; x < 0x40; x+=4)
	{
		offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		extraOffsets[x/4] = 0x00000000;
	}
	
	for (int x = 0; x < sizeOut; x++)
	{
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
			{
				for (int y = 0; y < numberTracks; y++)
				{
					if (offsetheader[y] > x)
					{
						extraOffsets[y]++;
					}
				}
			}
		}
	}

	outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		delete [] inArray;
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}

	for (int x = 0; x < 0x10; x++)
	{
		WriteLongToBuffer(inArray, x*4, offsetheader[x] + extraOffsets[x]);
	}
	for (int x = 0; x < sizeOut; x++)
	{
		fwrite(&inArray[x], 1, 1, outFile);
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
				fwrite(&inArray[x], 1, 1, outFile);
		}
	}
	fclose(outFile);
	delete [] inArray;

	//return true;

	if (useRepeaters)
	{
		// "compress"
		outFile = fopen(output, "rb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}	
		fseek(outFile, 0, SEEK_END);
		int sizeIn = ftell(outFile);
		rewind(outFile);
		inArray = new byte[sizeIn];
		fread(inArray, 1, sizeIn, outFile);
		fclose(outFile);

		unsigned long offset[0x10];
		for (int x = 0; x < 0x40; x+=4)
		{
			offset[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		}
		unsigned quarternote = ((((((inArray[0x40] << 8) | inArray[0x41]) << 8) | inArray[0x42]) << 8) | inArray[0x43]);

		unsigned char* outArray = new unsigned char[sizeIn * 4];
		for (int x = 0x0; x < (sizeIn * 4); x++)
		{
			outArray[x] = 0x0;
		}

		unsigned long offsetNew[0x10];
		for (int x = 0; x < 0x10; x++)
		{
			offsetNew[x] = 0;
		}


		int outputSpot = 0x44;
		

		for (int x = 0x0; x < 0x10; x++)
		{
			if (offset[x] != 0)
			{
				offsetNew[x] = outputSpot;

				int outputStart = outputSpot;

				int endSpot = sizeIn;

				if (x < 0xF)
				{
					if (offset[x+1] != 0)
					{
						endSpot = offset[x+1];
					}
				}

				int y = offset[x];
				// loop till end of file
				while (y < endSpot)
				{
					int bestMatchOffset = -1;
					int bestMatchLoopCount = -1;


					// check from past to now for results
					for (int z = outputStart; z < outputSpot; z++)
					{
						int match = 0;
						int matchOffset = 0;

						// one spot match start here
						while  ((outArray[z+matchOffset] == inArray[y+matchOffset]) && ((y+matchOffset) < endSpot) && (outArray[z+matchOffset] != 0xFE) && (outArray[z+matchOffset] != 0xFF) && ((z+matchOffset) < outputSpot))
						{
							bool seeAnFF = false;
							for (int checkFF = y+matchOffset; ((checkFF < endSpot) && (checkFF < (y+matchOffset + 5))); checkFF++)
							{
								if (inArray[checkFF] == 0xFF)
									seeAnFF = true;
							}

							if (seeAnFF)
							{
								break;
							}

							matchOffset++;
						}

						if ((matchOffset > bestMatchLoopCount) && (matchOffset > 6))
						{
							bestMatchLoopCount = matchOffset;
							bestMatchOffset = z;
						}
					}

					int loopCheck = ((y - offset[x]) / 2);
					if (loopCheck > 0xFD)
						loopCheck = 0xFD;

					if (bestMatchLoopCount > 6)
					{
						if (bestMatchLoopCount > 0xFD)
                            bestMatchLoopCount = 0xFD;

						outArray[outputSpot++] = 0xFE;
						int distBack = ((outputSpot - bestMatchOffset) - 1);

						outArray[outputSpot++] = ((distBack >> 8) & 0xFF);
						outArray[outputSpot++] = (distBack & 0xFF);
						outArray[outputSpot++] = bestMatchLoopCount;

						y += bestMatchLoopCount;
					}
					else
					{
						// write one
						outArray[outputSpot++] = inArray[y];

						y++;
					}
				}
			}
			else
				break;

			if ((outputSpot % 4) != 0)
				outputSpot += (4 - (outputSpot % 4));
		}

		// correct loops
		for (int x = 0x0; x < 0x10; x++)
		{
			if (offsetNew[x] != 0)
			{
				int outputStart = offsetNew[x];

				int endSpot = outputSpot;

				if (x < 0xF)
				{
					if (offsetNew[x+1] != 0)
					{
						endSpot = offsetNew[x+1];
					}
				}

				int y = offsetNew[x];
				// loop till end of file

				bool foundStart = false;
				int startPos = 0;
				while (y < endSpot)
				{
					if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2E) && (outArray[y+2] == 0x00) && (outArray[y+3] == 0xFF)) 
					{
						foundStart = true;
						startPos = y + 4;
						y+=4;
					}
					else if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2D) && (outArray[y+2] == 0xFF) && (outArray[y+3] == 0xFF)) 
					{
						if (foundStart)
						{
							int distance = ((y + 8) - startPos);

							WriteLongToBuffer(outArray, y+4, distance);
							foundStart = false;
						}

						y+=8;
					}
					else
					{
						y++;
					}

					
				}
			}
		}

		for (int x = 0x0; x < 0x10; x++)
		{
			WriteLongToBuffer(outArray, x*4, offsetNew[x]);
		}
		WriteLongToBuffer(outArray, 0x40, quarternote);


		outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			delete [] inArray;
			delete [] outArray;
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}

		for (int x = 0; x < outputSpot; x++)
		{
			fwrite(&outArray[x], 1, 1, outFile);
		}
		fclose(outFile);

		delete [] inArray;
		delete [] outArray;






	}

	return true;
}

bool CMidiParse::MidiToBTFormatStageOne(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters, unsigned short& numTracks)
{
	try
	{

	CString tempFileName = input;

	struct stat results;
	stat(tempFileName, &results);		

	FILE* inFile1 = fopen(input, "rb");
	if (inFile1 == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	unsigned long header = CharArrayToLong(&inputMID[0]);

	if (header != 0x4D546864)
	{
		MessageBox(NULL, "Invalid midi hdr", "Error", NULL);
		return false;
	}

	unsigned long headerLength = CharArrayToLong(&inputMID[4]);

	unsigned short type = CharArrayToShort(&inputMID[8]);
	numTracks = CharArrayToShort(&inputMID[0xA]);
	unsigned short tempo = CharArrayToShort(&inputMID[0xC]);

	if (numTracks > 32)
	{
		MessageBox(NULL, "Too many tracks, truncated to 32", "Warning", NULL);
		numTracks = 32;
	}

	unsigned long lessOffset = 0x84 - ((numTracks * 0x4) + 0x8);

	

	FILE* outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error outputting file", "Error", NULL);
		return false;
	}


	numberTracks = numTracks;

	if (type == 0)
	{
		
	}
	else if (type == 1)
	{

	}
	else
	{
		fclose(outFile);
		MessageBox(NULL, "Invalid midi type", "Error", NULL);
		return false;
	}



	int position = 0xE;

	byte* repeatPattern = NULL;
	byte altOffset = 0;
	byte altLength = 0;

	bool unknownsHit = false;

	unsigned long highestAbsoluteTime = 0;
	unsigned long highestAbsoluteTimeByTrack[0x20];
	for (int x = 0; x < 0x20; x++)
		highestAbsoluteTimeByTrack[x] = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

	

		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					// remove time till end
					absoluteTime -= timeTag;

					endFlag = true;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}
		}

		if (absoluteTime > highestAbsoluteTime)
		{
			highestAbsoluteTime = absoluteTime;
		}

		if (absoluteTime > highestAbsoluteTimeByTrack[trackNum])
		{
			highestAbsoluteTimeByTrack[trackNum] = absoluteTime;
		}
	}


	position = 0xE;

	repeatPattern = NULL;
	altOffset = 0;
	altLength = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

		bool didLoop = false;

		if ((loop) && (loopPoint == 0) && (highestAbsoluteTimeByTrack[trackNum] > 0))
		{
			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->type = 0xFF;
			newTrackEvent->absoluteTime = 0;
			newTrackEvent->contentSize = 3;
			newTrackEvent->contents = new byte[newTrackEvent->contentSize];
			newTrackEvent->contents[0] = 0x2E;
			newTrackEvent->contents[1] = 0x00;
			newTrackEvent->contents[2] = 0xFF;
			newTrackEvent->deltaTime = 0;
			newTrackEvent->obsoleteEvent = false;
			
			trackEventCount[trackNum]++;
			didLoop = true;
		}

		
		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->deltaTime = timeTag;
			newTrackEvent->obsoleteEvent = false;
			newTrackEvent->contents = NULL;
			newTrackEvent->absoluteTime = absoluteTime;

			if ((loop) && (!didLoop) && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
			{
				if (absoluteTime == loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = absoluteTime;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					newTrackEventLoop->deltaTime = timeTag;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = 0;
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
				else if (absoluteTime > loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = loopPoint;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					if (trackEventCount[trackNum] > 0)
						newTrackEventLoop->deltaTime = (loopPoint - ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]))->absoluteTime);
					else
						newTrackEventLoop->deltaTime = loopPoint;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = (absoluteTime - loopPoint);
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
			}

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					endFlag = true;

					if (loop && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
					{

						TrackEvent* prevEvent = ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]));
						if (
							(prevEvent->type == 0xFF)
							&& (prevEvent->contentSize > 0)
							&& (prevEvent->contents[0] == 0x2E)
							)
						{
							newTrackEvent = prevEvent;

							delete [] newTrackEvent->contents;
							newTrackEvent->type = 0xFF;
							newTrackEvent->contentSize = 1;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2F;
						}
						else
						{
							TrackEvent* newTrackEventLast = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]+1]);
							newTrackEventLast->absoluteTime = highestAbsoluteTime;
							newTrackEventLast->deltaTime = 0;
							newTrackEventLast->durationTime = newTrackEvent->durationTime;
							newTrackEventLast->obsoleteEvent = newTrackEvent->obsoleteEvent;

							newTrackEventLast->type = 0xFF;
							newTrackEventLast->contentSize = 1;
							newTrackEventLast->contents = new byte[newTrackEventLast->contentSize];
							newTrackEventLast->contents[0] = 0x2F;

							newTrackEvent->type = 0xFF;
							if (highestAbsoluteTime > (prevEvent->absoluteTime + prevEvent->durationTime))
							{
								newTrackEvent->deltaTime = (highestAbsoluteTime - (prevEvent->absoluteTime + prevEvent->durationTime));
								newTrackEvent->absoluteTime = highestAbsoluteTime;
							}
							else
							{
								newTrackEvent->deltaTime = 0;
								newTrackEvent->absoluteTime = prevEvent->absoluteTime;
							}

							newTrackEvent->contentSize = 7;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2D;
							newTrackEvent->contents[1] = 0xFF;
							newTrackEvent->contents[2] = 0xFF;
							newTrackEvent->contents[3] = 0x0; // todo write location
							newTrackEvent->contents[4] = 0x0;
							newTrackEvent->contents[5] = 0x0;
							newTrackEvent->contents[6] = 0x0;
							newTrackEvent->obsoleteEvent = false;
							
							trackEventCount[trackNum]++;
						}
					}
					else
					{
						newTrackEvent->type = 0xFF;
						newTrackEvent->contentSize = 1;
						newTrackEvent->contents = new byte[newTrackEvent->contentSize];
						newTrackEvent->contents[0] = 0x2F;
					}

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					newTrackEvent->type = 0xFF;
					newTrackEvent->contentSize = 4;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = 0x51;

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					newTrackEvent->contents[1] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[2] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[3] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					newTrackEvent->type = 0xFF;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					newTrackEvent->obsoleteEvent = true;
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					newTrackEvent->type = 0xFF;
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
					
					newTrackEvent->obsoleteEvent = true;
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
				{
					if ((trackEvents[trackNum][testBackwards].type == (0x90 | (curEventVal & 0xF))) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
					{
						if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
						{
							trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
							break;
						}
					}
				}


				newTrackEvent->durationTime = 0; 
				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = noteNumber;
				newTrackEvent->contents[1] = velocity;
				newTrackEvent->obsoleteEvent = true;

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (velocity == 0)
				{
					// simulate note off
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0;
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
					newTrackEvent->obsoleteEvent = true;
				}
				else
				{
					// check if no note off received, if so, turn it off and restart note
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								if (trackEvents[trackNum][testBackwards].durationTime == 0) // means unfinished note
									trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0; // to be filled in
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = controllerType;
				newTrackEvent->contents[1] = controllerValue;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				if ((eventVal & 0xF) == 9) // drums in GM
					instrument = instrument;
				else
					instrument = instrument;

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = instrument;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				newTrackEvent->type = eventVal;
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = amount;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				newTrackEvent->type = eventVal;
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = valueLSB;
				newTrackEvent->contents[1] = valueMSB;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
                newTrackEvent->type = eventVal;
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				newTrackEvent->obsoleteEvent = true;
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}

			trackEventCount[trackNum]++;
		}
	}

	unsigned long timeOffset = 0;

	unsigned long startPosition = 0x84;

	// get offsets
	for (int i = 0; i < numberTracks; i++)
	{
		unsigned long sizeData = 0;
		int loopStartPosition = 0;
		bool foundLoopStart = false;
		byte previousTrackEvent = 0x0;

		if (trackEventCount[i] > 0)
		{
			unsigned long tempLong = Flip32Bit(startPosition);
			fwrite(&tempLong, 1, 4, outFile);

			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);

				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2E))
					{
						foundLoopStart = true;
						loopStartPosition = (startPosition + sizeData + 1 + trackEvent->contentSize + lengthTimeDelta);
					}
					
					timeOffset = 0;

					sizeData += lengthTimeDelta;

					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2D))
					{
						unsigned long offsetBack = ((startPosition + sizeData) - loopStartPosition + 8);
						trackEvent->contents[3] = ((offsetBack >> 24) & 0xFF);
						trackEvent->contents[4] = ((offsetBack >> 16) & 0xFF);
						trackEvent->contents[5] = ((offsetBack >> 8) & 0xFF);
						trackEvent->contents[6] = ((offsetBack >> 0) & 0xFF);
					}

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						sizeData += 1;
					}

					sizeData += trackEvent->contentSize;

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes);

						sizeData += lengthDurationBytes;
					}

					previousTrackEvent = trackEvent->type;
				}
			}
			startPosition += sizeData;
		}
		else
		{
			unsigned long zero = 0;
			fwrite(&zero, 1, 4, outFile);
		}
	}

	for (int i = numberTracks; i < 32; i++)
	{
		unsigned long zero = 0;
		fwrite(&zero, 1, 4, outFile);
	}

	unsigned long divisionFlipped = Flip32Bit((unsigned long)tempo);
	fwrite(&divisionFlipped, 1, 4, outFile);

	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	for (int i = 0; i < numberTracks; i++)
	{
		if (trackEventCount[i] > 0)
		{
			if (outDebug != NULL) fprintf(outDebug, "Track %X\n", i);
			byte previousTrackEvent = 0x0;
			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					unsigned long lengthTimeDelta = 0;
					unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);
					timeOffset = 0;
					WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

					for (int ii = 0; ii < lengthTimeDelta; ii++)
						if (outDebug != NULL) fprintf(outDebug, "%02X", ((timeDelta >> ((lengthTimeDelta * 8) - 8 - (ii * 8))) & 0xFF));

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						fwrite(&trackEvent->type, 1, 1, outFile);
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->type);
					}

					fwrite(trackEvent->contents, 1, trackEvent->contentSize, outFile);
					for (int ii = 0; ii < trackEvent->contentSize; ii++)
					{
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->contents[ii]);
					}

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes); // todo real trackevent
						WriteVLBytes(outFile, duration, lengthDurationBytes, false);
						for (int ii = 0; ii < lengthDurationBytes; ii++)
							if (outDebug != NULL) fprintf(outDebug, "%02X", ((duration >> ((lengthDurationBytes * 8) - 8 - (ii * 8))) & 0xFF));
					}

					if (outDebug != NULL) fprintf(outDebug, "\n");
					previousTrackEvent = trackEvent->type;
				}
				
			}
		}

		for (int j = 0; j < trackEventCount[i]; j++)
		{
			if (trackEvents[i][j].contents != NULL)
			{
				delete [] trackEvents[i][j].contents;
				trackEvents[i][j].contents = NULL;
			}
		}
	}

	fflush(outDebug);

	fclose(outFile);

	// write FEFE in case see FE
	outFile = fopen(output, "rb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	
	fseek(outFile, 0, SEEK_END);
	int sizeOut = ftell(outFile);
	rewind(outFile);
	byte* inArray = new byte[sizeOut];
	fread(inArray, 1, sizeOut, outFile);
	fclose(outFile);

	unsigned long offsetheader[0x20];
	int extraOffsets[0x20];
	for (int x = 0; x < 0x80; x+=4)
	{
		offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		extraOffsets[x/4] = 0x00000000;
	}
	
	for (int x = 0; x < sizeOut; x++)
	{
		if (x > 0x84)
		{
			if (inArray[x] == 0xFE) // need to write twice
			{
				for (int y = 0; y < numberTracks; y++)
				{
					if (offsetheader[y] > x)
					{
						extraOffsets[y]++;
					}
				}
			}
		}
	}

	outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		delete [] inArray;
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}

	for (int x = 0; x < 0x20; x++)
	{
		WriteLongToBuffer(inArray, x*4, offsetheader[x] + extraOffsets[x]);
	}
	for (int x = 0; x < sizeOut; x++)
	{
		fwrite(&inArray[x], 1, 1, outFile);
		if (x > 0x84)
		{
			if (inArray[x] == 0xFE) // need to write twice
				fwrite(&inArray[x], 1, 1, outFile);
		}
	}
	fclose(outFile);
	delete [] inArray;

	//return true;

	if (useRepeaters)
	{
		// "compress"
		outFile = fopen(output, "rb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}	
		fseek(outFile, 0, SEEK_END);
		int sizeIn = ftell(outFile);
		rewind(outFile);
		inArray = new byte[sizeIn];
		fread(inArray, 1, sizeIn, outFile);
		fclose(outFile);

		unsigned long offset[0x20];
		for (int x = 0; x < 0x80; x+=4)
		{
			offset[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		}
		unsigned quarternote = ((((((inArray[0x80] << 8) | inArray[0x81]) << 8) | inArray[0x82]) << 8) | inArray[0x83]);

		unsigned char* outArray = new unsigned char[sizeIn * 4];
		for (int x = 0x0; x < (sizeIn * 4); x++)
		{
			outArray[x] = 0x0;
		}

		unsigned long offsetNew[0x20];
		for (int x = 0; x < 0x20; x++)
		{
			offsetNew[x] = 0;
		}


		int outputSpot = 0x84;
		

		for (int x = 0x0; x < 0x20; x++)
		{
			if (offset[x] != 0)
			{
				offsetNew[x] = outputSpot;

				int outputStart = outputSpot;

				int endSpot = sizeIn;

				if (x < 0xF)
				{
					if (offset[x+1] != 0)
					{
						endSpot = offset[x+1];
					}
				}

				int y = offset[x];
				// loop till end of file
				while (y < endSpot)
				{
					int bestMatchOffset = -1;
					int bestMatchLoopCount = -1;


					// check from past to now for results
					for (int z = outputStart; z < outputSpot; z++)
					{
						int match = 0;
						int matchOffset = 0;

						// one spot match start here
						while  ((outArray[z+matchOffset] == inArray[y+matchOffset]) && ((y+matchOffset) < endSpot) && (outArray[z+matchOffset] != 0xFE) && (outArray[z+matchOffset] != 0xFF) && ((z+matchOffset) < outputSpot))
						{
							bool seeAnFF = false;
							for (int checkFF = y+matchOffset; ((checkFF < endSpot) && (checkFF < (y+matchOffset + 5))); checkFF++)
							{
								if (inArray[checkFF] == 0xFF)
									seeAnFF = true;
							}

							if (seeAnFF)
							{
								break;
							}

							matchOffset++;
						}

						if ((matchOffset > bestMatchLoopCount) && (matchOffset > 6))
						{
							bestMatchLoopCount = matchOffset;
							bestMatchOffset = z;
						}
					}

					int loopCheck = ((y - offset[x]) / 2);
					if (loopCheck > 0xFD)
						loopCheck = 0xFD;

					if (bestMatchLoopCount > 6)
					{
						if (bestMatchLoopCount > 0xFD)
                            bestMatchLoopCount = 0xFD;

						outArray[outputSpot++] = 0xFE;
						int distBack = ((outputSpot - bestMatchOffset) - 1);

						outArray[outputSpot++] = ((distBack >> 8) & 0xFF);
						outArray[outputSpot++] = (distBack & 0xFF);
						outArray[outputSpot++] = bestMatchLoopCount;

						y += bestMatchLoopCount;
					}
					else
					{
						// write one
						outArray[outputSpot++] = inArray[y];

						y++;
					}
				}
			}
			else
				break;

			if ((outputSpot % 4) != 0)
				outputSpot += (4 - (outputSpot % 4));
		}

		// correct loops
		for (int x = 0x0; x < 0x20; x++)
		{
			if (offsetNew[x] != 0)
			{
				int outputStart = offsetNew[x];

				int endSpot = outputSpot;

				if (x < 0xF)
				{
					if (offsetNew[x+1] != 0)
					{
						endSpot = offsetNew[x+1];
					}
				}

				int y = offsetNew[x];
				// loop till end of file

				bool foundStart = false;
				int startPos = 0;
				while (y < endSpot)
				{
					if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2E) && (outArray[y+2] == 0x00) && (outArray[y+3] == 0xFF)) 
					{
						foundStart = true;
						startPos = y + 4;
						y+=4;
					}
					else if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2D) && (outArray[y+2] == 0xFF) && (outArray[y+3] == 0xFF)) 
					{
						if (foundStart)
						{
							int distance = ((y + 8) - startPos);

							WriteLongToBuffer(outArray, y+4, distance);
							foundStart = false;
						}

						y+=8;
					}
					else
					{
						y++;
					}

					
				}
			}
		}

		for (int x = 0x0; x < 0x20; x++)
		{
			WriteLongToBuffer(outArray, x*4, offsetNew[x]);
		}
		WriteLongToBuffer(outArray, 0x80, quarternote);


		outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			delete [] inArray;
			delete [] outArray;
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}

		for (int x = 0; x < outputSpot; x++)
		{
			fwrite(&outArray[x], 1, 1, outFile);
		}
		fclose(outFile);

		delete [] inArray;
		delete [] outArray;
	}
}
catch (...)
{
	MessageBox(NULL, "Error converting", "Error", NULL);
	return false;
}
	return true;
}

bool CMidiParse::MidiToGEFormat(CString input, CString output, bool loop, unsigned long loopPoint, bool useRepeaters)
{
	try
	{

	numberTracks = 0;
	for (unsigned int x = 0; x < 0x20; x++)
	{
		for (int y = 0; y < 0x30000; y++)
		{
			if (trackEvents[x][y].contents != NULL)
			{
				delete[] trackEvents[x][y].contents;
				trackEvents[x][y].contents = NULL;
			}
		}
		trackEventCount[x] = 0;
	}

	CString tempFileName = input;

	struct stat results;
	stat(tempFileName, &results);		

	FILE* inFile1 = fopen(input, "rb");
	if (inFile1 == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	unsigned long header = CharArrayToLong(&inputMID[0]);

	if (header != 0x4D546864)
	{
		MessageBox(NULL, "Invalid midi hdr", "Error", NULL);
		return false;
	}

	unsigned long headerLength = CharArrayToLong(&inputMID[4]);

	unsigned short type = CharArrayToShort(&inputMID[8]);
	unsigned short numTracks = CharArrayToShort(&inputMID[0xA]);
	unsigned short tempo = CharArrayToShort(&inputMID[0xC]);


	if (numTracks > 16)
	{
		MessageBox(NULL, "Too many tracks, truncated to 16", "Warning", NULL);
		numTracks = 16;
	}

	

	FILE* outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error outputting file", "Error", NULL);
		return false;
	}


	numberTracks = numTracks;

	if (type == 0)
	{
		
	}
	else if (type == 1)
	{

	}
	else
	{
		fclose(outFile);
		MessageBox(NULL, "Invalid midi type", "Error", NULL);
		return false;
	}



	int position = 0xE;

	byte* repeatPattern = NULL;
	byte altOffset = 0;
	byte altLength = 0;

	bool unknownsHit = false;

	unsigned long highestAbsoluteTime = 0;
	unsigned long highestAbsoluteTimeByTrack[0x10];
	for (int x = 0; x < 0x10; x++)
		highestAbsoluteTimeByTrack[x] = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

	

		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					// remove time till end
					absoluteTime -= timeTag;

					endFlag = true;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}
		}

		if (absoluteTime > highestAbsoluteTime)
		{
			highestAbsoluteTime = absoluteTime;
		}

		if (absoluteTime > highestAbsoluteTimeByTrack[trackNum])
		{
			highestAbsoluteTimeByTrack[trackNum] = absoluteTime;
		}
	}


	position = 0xE;

	repeatPattern = NULL;
	altOffset = 0;
	altLength = 0;

	for (int trackNum = 0; trackNum < numberTracks; trackNum++)
	{
		unsigned long absoluteTime = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

		bool didLoop = false;

		if ((loop) && (loopPoint == 0) && (highestAbsoluteTimeByTrack[trackNum] > 0))
		{
			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->type = 0xFF;
			newTrackEvent->absoluteTime = 0;
			newTrackEvent->contentSize = 3;
			newTrackEvent->contents = new byte[newTrackEvent->contentSize];
			newTrackEvent->contents[0] = 0x2E;
			newTrackEvent->contents[1] = 0x00;
			newTrackEvent->contents[2] = 0xFF;
			newTrackEvent->deltaTime = 0;
			newTrackEvent->obsoleteEvent = false;
			
			trackEventCount[trackNum]++;
			didLoop = true;
		}

		
		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTime += timeTag;

			TrackEvent* newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
			newTrackEvent->deltaTime = timeTag;
			newTrackEvent->obsoleteEvent = false;
			newTrackEvent->contents = NULL;
			newTrackEvent->absoluteTime = absoluteTime;

			if ((loop) && (!didLoop) && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
			{
				if (absoluteTime == loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = absoluteTime;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					newTrackEventLoop->deltaTime = timeTag;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = 0;
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
				else if (absoluteTime > loopPoint)
				{
					TrackEvent* newTrackEventLoop = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEventLoop->type = 0xFF;
					newTrackEventLoop->absoluteTime = loopPoint;
					newTrackEventLoop->contentSize = 3;
					newTrackEventLoop->contents = new byte[newTrackEventLoop->contentSize];
					newTrackEventLoop->contents[0] = 0x2E;
					newTrackEventLoop->contents[1] = 0x00;
					newTrackEventLoop->contents[2] = 0xFF;
					if (trackEventCount[trackNum] > 0)
						newTrackEventLoop->deltaTime = (loopPoint - ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]))->absoluteTime);
					else
						newTrackEventLoop->deltaTime = loopPoint;
					newTrackEventLoop->obsoleteEvent = false;
					
					trackEventCount[trackNum]++;

					newTrackEvent = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]]);
					newTrackEvent->deltaTime = (absoluteTime - loopPoint);
					newTrackEvent->obsoleteEvent = false;
					newTrackEvent->contents = NULL;
					newTrackEvent->absoluteTime = absoluteTime;

					didLoop = true;
				}
			}

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					endFlag = true;

					if (loop && (highestAbsoluteTimeByTrack[trackNum] > loopPoint))
					{

						TrackEvent* prevEvent = ((TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]-1]));
						if (
							(prevEvent->type == 0xFF)
							&& (prevEvent->contentSize > 0)
							&& (prevEvent->contents[0] == 0x2E)
							)
						{
							newTrackEvent = prevEvent;

							delete [] newTrackEvent->contents;
							newTrackEvent->type = 0xFF;
							newTrackEvent->contentSize = 1;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2F;
						}
						else
						{
							TrackEvent* newTrackEventLast = (TrackEvent*)&(trackEvents[trackNum][trackEventCount[trackNum]+1]);
							newTrackEventLast->absoluteTime = highestAbsoluteTime;
							newTrackEventLast->deltaTime = 0;
							newTrackEventLast->durationTime = newTrackEvent->durationTime;
							newTrackEventLast->obsoleteEvent = newTrackEvent->obsoleteEvent;

							newTrackEventLast->type = 0xFF;
							newTrackEventLast->contentSize = 1;
							newTrackEventLast->contents = new byte[newTrackEventLast->contentSize];
							newTrackEventLast->contents[0] = 0x2F;

							newTrackEvent->type = 0xFF;
							if (highestAbsoluteTime > (prevEvent->absoluteTime + prevEvent->durationTime))
							{
								newTrackEvent->deltaTime = (highestAbsoluteTime - (prevEvent->absoluteTime + prevEvent->durationTime));
								newTrackEvent->absoluteTime = highestAbsoluteTime;
							}
							else
							{
								newTrackEvent->deltaTime = 0;
								newTrackEvent->absoluteTime = prevEvent->absoluteTime;
							}

							newTrackEvent->contentSize = 7;
							newTrackEvent->contents = new byte[newTrackEvent->contentSize];
							newTrackEvent->contents[0] = 0x2D;
							newTrackEvent->contents[1] = 0xFF;
							newTrackEvent->contents[2] = 0xFF;
							newTrackEvent->contents[3] = 0x0; // todo write location
							newTrackEvent->contents[4] = 0x0;
							newTrackEvent->contents[5] = 0x0;
							newTrackEvent->contents[6] = 0x0;
							newTrackEvent->obsoleteEvent = false;
							
							trackEventCount[trackNum]++;
						}
					}
					else
					{
						newTrackEvent->type = 0xFF;
						newTrackEvent->contentSize = 1;
						newTrackEvent->contents = new byte[newTrackEvent->contentSize];
						newTrackEvent->contents[0] = 0x2F;
					}

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					newTrackEvent->type = 0xFF;
					newTrackEvent->contentSize = 4;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = 0x51;

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
										
					newTrackEvent->contents[1] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[2] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->contents[3] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					newTrackEvent->type = 0xFF;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					newTrackEvent->obsoleteEvent = true;
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					newTrackEvent->type = 0xFF;
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
					
					newTrackEvent->obsoleteEvent = true;
				}

				previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
				{
					if ((trackEvents[trackNum][testBackwards].type == (0x90 | (curEventVal & 0xF))) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
					{
						if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
						{
							trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
							break;
						}
					}
				}


				newTrackEvent->durationTime = 0; 
				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = noteNumber;
				newTrackEvent->contents[1] = velocity;
				newTrackEvent->obsoleteEvent = true;

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					newTrackEvent->type = previousEventValue;
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					newTrackEvent->type = eventVal;
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (velocity == 0)
				{
					// simulate note off
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0;
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
					newTrackEvent->obsoleteEvent = true;
				}
				else
				{
					// check if no note off received, if so, turn it off and restart note
					for (int testBackwards = (trackEventCount[trackNum] - 1); testBackwards >= 0; testBackwards--)
					{
						if (((trackEvents[trackNum][testBackwards].type == curEventVal)) && !(trackEvents[trackNum][testBackwards].obsoleteEvent))
						{
							if (trackEvents[trackNum][testBackwards].contents[0] == noteNumber)
							{
								if (trackEvents[trackNum][testBackwards].durationTime == 0) // means unfinished note
									trackEvents[trackNum][testBackwards].durationTime = (absoluteTime - trackEvents[trackNum][testBackwards].absoluteTime);
								break;
							}
						}
					}

					newTrackEvent->durationTime = 0; // to be filled in
					newTrackEvent->contentSize = 2;
					newTrackEvent->contents = new byte[newTrackEvent->contentSize];
					newTrackEvent->contents[0] = noteNumber;
					newTrackEvent->contents[1] = velocity;
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				if (statusBit)
				{
					controllerType = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = controllerType;
				newTrackEvent->contents[1] = controllerValue;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				if (statusBit)
				{
					instrument = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				if ((eventVal & 0xF) == 9) // Drums in GM
					instrument = instrument;
				else
					instrument = instrument;

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = instrument;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				newTrackEvent->type = eventVal;
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				newTrackEvent->contentSize = 1;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = amount;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				newTrackEvent->type = eventVal;
				byte valueLSB;
				if (statusBit)
				{
					valueLSB = eventVal;
					newTrackEvent->type = previousEventValue;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					newTrackEvent->type = eventVal;
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				newTrackEvent->contentSize = 2;
				newTrackEvent->contents = new byte[newTrackEvent->contentSize];
				newTrackEvent->contents[0] = valueLSB;
				newTrackEvent->contents[1] = valueMSB;
				//newTrackEvent->obsoleteEvent = true; // temporary?

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
            {
                newTrackEvent->type = eventVal;
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}

				newTrackEvent->obsoleteEvent = true;
            }
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}

			trackEventCount[trackNum]++;
		}
	}

	unsigned long timeOffset = 0;

	unsigned long startPosition = 0x44;

	// get offsets
	for (int i = 0; i < numberTracks; i++)
	{
		unsigned long sizeData = 0;
		int loopStartPosition = 0;
		bool foundLoopStart = false;
		byte previousTrackEvent = 0x0;

		if (trackEventCount[i] > 0)
		{
			unsigned long tempLong = Flip32Bit(startPosition);
			fwrite(&tempLong, 1, 4, outFile);

			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);

				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2E))
					{
						foundLoopStart = true;
						loopStartPosition = (startPosition + sizeData + 1 + trackEvent->contentSize + lengthTimeDelta);
					}
					
					timeOffset = 0;

					sizeData += lengthTimeDelta;

					if ((trackEvent->type == 0xFF) && (trackEvent->contents[0] == 0x2D))
					{
						unsigned long offsetBack = ((startPosition + sizeData) - loopStartPosition + 8);
						trackEvent->contents[3] = ((offsetBack >> 24) & 0xFF);
						trackEvent->contents[4] = ((offsetBack >> 16) & 0xFF);
						trackEvent->contents[5] = ((offsetBack >> 8) & 0xFF);
						trackEvent->contents[6] = ((offsetBack >> 0) & 0xFF);
					}

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						sizeData += 1;
					}

					sizeData += trackEvent->contentSize;

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes);

						sizeData += lengthDurationBytes;
					}

					previousTrackEvent = trackEvent->type;
				}
			}
			startPosition += sizeData;
		}
		else
		{
			unsigned long zero = 0;
			fwrite(&zero, 1, 4, outFile);
		}
	}

	for (int i = numberTracks; i < 16; i++)
	{
		unsigned long zero = 0;
		fwrite(&zero, 1, 4, outFile);
	}

	unsigned long divisionFlipped = Flip32Bit((unsigned long)tempo);
	fwrite(&divisionFlipped, 1, 4, outFile);

	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	for (int i = 0; i < numberTracks; i++)
	{
		if (trackEventCount[i] > 0)
		{
			if (outDebug != NULL) fprintf(outDebug, "Track %X\n", i);
			byte previousTrackEvent = 0x0;
			for (int j = 0; j < trackEventCount[i]; j++)
			{
				TrackEvent* trackEvent =  &(trackEvents[i][j]);
				if (trackEvent->obsoleteEvent)
				{
					timeOffset += trackEvent->deltaTime;
				}
				else
				{
					unsigned long lengthTimeDelta = 0;
					unsigned long timeDelta = ReturnVLBytes((trackEvent->deltaTime + timeOffset), lengthTimeDelta);
					timeOffset = 0;
					WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

					for (int ii = 0; ii < lengthTimeDelta; ii++)
						if (outDebug != NULL) fprintf(outDebug, "%02X", ((timeDelta >> ((lengthTimeDelta * 8) - 8 - (ii * 8))) & 0xFF));

					if ((trackEvent->type != previousTrackEvent) || (trackEvent->type == 0xFF))
					{
						fwrite(&trackEvent->type, 1, 1, outFile);
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->type);
					}

					fwrite(trackEvent->contents, 1, trackEvent->contentSize, outFile);
					for (int ii = 0; ii < trackEvent->contentSize; ii++)
					{
						if (outDebug != NULL) fprintf(outDebug, "%02X", trackEvent->contents[ii]);
					}

					if ((trackEvent->type >= 0x90) && (trackEvent->type < 0xA0))
					{
						unsigned long lengthDurationBytes = 0;
						unsigned long duration = ReturnVLBytes(trackEvent->durationTime, lengthDurationBytes); // todo real trackevent
						WriteVLBytes(outFile, duration, lengthDurationBytes, false);
						for (int ii = 0; ii < lengthDurationBytes; ii++)
							if (outDebug != NULL) fprintf(outDebug, "%02X", ((duration >> ((lengthDurationBytes * 8) - 8 - (ii * 8))) & 0xFF));
					}

					if (outDebug != NULL) fprintf(outDebug, "\n");
					previousTrackEvent = trackEvent->type;
				}
				
			}
		}

		for (int j = 0; j < trackEventCount[i]; j++)
		{
			if (trackEvents[i][j].contents != NULL)
			{
				delete [] trackEvents[i][j].contents;
				trackEvents[i][j].contents = NULL;
			}
		}
	}

	fflush(outDebug);

	fclose(outFile);

	// write FEFE in case see FE
	outFile = fopen(output, "rb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	
	fseek(outFile, 0, SEEK_END);
	int sizeOut = ftell(outFile);
	rewind(outFile);
	byte* inArray = new byte[sizeOut];
	fread(inArray, 1, sizeOut, outFile);
	fclose(outFile);

	unsigned long offsetheader[0x10];
	int extraOffsets[0x10];
	for (int x = 0; x < 0x40; x+=4)
	{
		offsetheader[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		extraOffsets[x/4] = 0x00000000;
	}
	
	for (int x = 0; x < sizeOut; x++)
	{
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
			{
				for (int y = 0; y < numberTracks; y++)
				{
					if (offsetheader[y] > x)
					{
						extraOffsets[y]++;
					}
				}
			}
		}
	}

	outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		delete [] inArray;
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}

	for (int x = 0; x < 0x10; x++)
	{
		WriteLongToBuffer(inArray, x*4, offsetheader[x] + extraOffsets[x]);
	}
	for (int x = 0; x < sizeOut; x++)
	{
		fwrite(&inArray[x], 1, 1, outFile);
		if (x > 0x44)
		{
			if (inArray[x] == 0xFE) // need to write twice
				fwrite(&inArray[x], 1, 1, outFile);
		}
	}
	fclose(outFile);
	delete [] inArray;

	//return true;

	if (useRepeaters)
	{
		// "compress"
		outFile = fopen(output, "rb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}	
		fseek(outFile, 0, SEEK_END);
		int sizeIn = ftell(outFile);
		rewind(outFile);
		inArray = new byte[sizeIn];
		fread(inArray, 1, sizeIn, outFile);
		fclose(outFile);

		unsigned long offset[0x10];
		for (int x = 0; x < 0x40; x+=4)
		{
			offset[x/4] = ((((((inArray[x] << 8) | inArray[x+1]) << 8) | inArray[x+2]) << 8) | inArray[x+3]);
		}
		unsigned quarternote = ((((((inArray[0x40] << 8) | inArray[0x41]) << 8) | inArray[0x42]) << 8) | inArray[0x43]);

		unsigned char* outArray = new unsigned char[sizeIn * 4];
		for (int x = 0x0; x < (sizeIn * 4); x++)
		{
			outArray[x] = 0x0;
		}

		unsigned long offsetNew[0x10];
		for (int x = 0; x < 0x10; x++)
		{
			offsetNew[x] = 0;
		}


		int outputSpot = 0x44;
		

		for (int x = 0x0; x < 0x10; x++)
		{
			if (offset[x] != 0)
			{
				offsetNew[x] = outputSpot;

				int outputStart = outputSpot;

				int endSpot = sizeIn;

				if (x < 0xF)
				{
					if (offset[x+1] != 0)
					{
						endSpot = offset[x+1];
					}
				}

				int y = offset[x];
				// loop till end of file
				while (y < endSpot)
				{
					int bestMatchOffset = -1;
					int bestMatchLoopCount = -1;


					// check from past to now for results
					for (int z = outputStart; z < outputSpot; z++)
					{
						int match = 0;
						int matchOffset = 0;

						// one spot match start here
						while  ((outArray[z+matchOffset] == inArray[y+matchOffset]) && ((y+matchOffset) < endSpot) && (outArray[z+matchOffset] != 0xFE) && (outArray[z+matchOffset] != 0xFF) && ((z+matchOffset) < outputSpot))
						{
							bool seeAnFF = false;
							for (int checkFF = y+matchOffset; ((checkFF < endSpot) && (checkFF < (y+matchOffset + 5))); checkFF++)
							{
								if (inArray[checkFF] == 0xFF)
									seeAnFF = true;
							}

							if (seeAnFF)
							{
								break;
							}

							matchOffset++;
						}

						if ((matchOffset > bestMatchLoopCount) && (matchOffset > 6))
						{
							bestMatchLoopCount = matchOffset;
							bestMatchOffset = z;
						}
					}

					int loopCheck = ((y - offset[x]) / 2);
					if (loopCheck > 0xFD)
						loopCheck = 0xFD;

					if (bestMatchLoopCount > 6)
					{
						if (bestMatchLoopCount > 0xFD)
							bestMatchLoopCount = 0xFD;

						outArray[outputSpot++] = 0xFE;
						int distBack = ((outputSpot - bestMatchOffset) - 1);

						outArray[outputSpot++] = ((distBack >> 8) & 0xFF);
						outArray[outputSpot++] = (distBack & 0xFF);
						outArray[outputSpot++] = bestMatchLoopCount;

						y += bestMatchLoopCount;
					}
					else
					{
						// write one
						outArray[outputSpot++] = inArray[y];

						y++;
					}
				}
			}
			else
				break;

			if ((outputSpot % 4) != 0)
				outputSpot += (4 - (outputSpot % 4));
		}

		// correct loops
		for (int x = 0x0; x < 0x10; x++)
		{
			if (offsetNew[x] != 0)
			{
				int outputStart = offsetNew[x];

				int endSpot = outputSpot;

				if (x < 0xF)
				{
					if (offsetNew[x+1] != 0)
					{
						endSpot = offsetNew[x+1];
					}
				}

				int y = offsetNew[x];
				// loop till end of file

				bool foundStart = false;
				int startPos = 0;
				while (y < endSpot)
				{
					if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2E) && (outArray[y+2] == 0x00) && (outArray[y+3] == 0xFF)) 
					{
						foundStart = true;
						startPos = y + 4;
						y+=4;
					}
					else if ((outArray[y] == 0xFF) && (outArray[y+1] == 0x2D) && (outArray[y+2] == 0xFF) && (outArray[y+3] == 0xFF)) 
					{
						if (foundStart)
						{
							int distance = ((y + 8) - startPos);

							WriteLongToBuffer(outArray, y+4, distance);
							foundStart = false;
						}

						y+=8;
					}
					else
					{
						y++;
					}

					
				}
			}
		}

		for (int x = 0x0; x < 0x10; x++)
		{
			WriteLongToBuffer(outArray, x*4, offsetNew[x]);
		}
		WriteLongToBuffer(outArray, 0x40, quarternote);


		outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			delete [] inArray;
			delete [] outArray;
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return false;
		}

		for (int x = 0; x < outputSpot; x++)
		{
			fwrite(&outArray[x], 1, 1, outFile);
		}
		fclose(outFile);

		delete [] inArray;
		delete [] outArray;
	}
}
catch (...)
{
	MessageBox(NULL, "Error converting", "Error", NULL);
	return false;
}
	return true;
}


unsigned long CMidiParse::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}


void CMidiParse::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CMidiParse::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}



unsigned short CMidiParse::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CMidiParse::Flip16bit(unsigned short tempShort)
{
	return (((tempShort & 0xFF00) >> 8) | ((tempShort & 0x00FF) << 8));
}


unsigned long CMidiParse::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}



unsigned char CMidiParse::StringToUnsignedChar(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 2)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (2-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 2; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((1-x)*4);
	}
	return (unsigned char) tempLong;
}

unsigned long CMidiParse::StringHexToLong(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 8)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (8-tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0;x < 8; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt<<((7-x)*4);
	}
	return tempLong;
}

void CMidiParse::GEMidiToDebugTextFile(byte* inputMID, int inputSize, CString textFileOut, bool extendTracksToHighest)
{
	FILE* outFile = fopen(textFileOut, "w");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error outputting file", "Error", NULL);
		return;
	}

	unsigned long lengthHeader = 0x44;

	fprintf(outFile, "Offset Start Midi Events: %08X \n", lengthHeader);

	fprintf(outFile, "Tracks\n");

	// parse midi
	
	int counterTrack = 0;

	int highestTrackLength = 0;

	for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
	{
		unsigned long absoluteTime = 0;

		unsigned long offset = CharArrayToLong(&inputMID[iii]);
		
		int position = offset;	

		if (position != 0)
		{
			int previousEventValue = 0;

			std::map<int, int> loopEndsWithCount;

			byte* repeatPattern = NULL;
			byte altOffset = 0;
			byte altLength = 0;

			bool endFlag = false;

			while ((position < inputSize) && !endFlag)
			{
				int timePosition = position;

				unsigned long original;
				unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

				absoluteTime += timeTag;

				if (absoluteTime > highestTrackLength)
					highestTrackLength = absoluteTime;


				int vlLength = 0;
				byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

				bool statusBit = false;

				if (eventVal < 0x80)
				{
					// continuation
					statusBit = true;
				}
				else
				{
					statusBit = false;
				}

				if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
				{
					byte subType;
					if (statusBit)
						subType = eventVal;
					else
						subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (subType == 0x51) // tempo
					{
						int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

					}
					else if (subType == 0x2D) // end loop
					{
						byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
						
						if ((loopCount == 0xFF) || (loopCount == 0x00))
						{
							break;
						}
						else
						{
							std::map<int, int>::iterator it = loopEndsWithCount.find(position);
							if (it != loopEndsWithCount.end())
							{
								int countLeft = it->second;

								if (countLeft == 0)
								{
									loopEndsWithCount.erase(it);
								}
								else
								{
									loopEndsWithCount[position] = (countLeft - 1);

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
							else
							{
								loopEndsWithCount[position] = loopCount - 1;

								if (repeatPattern == NULL)
								{
									position = position - offsetToBeginningLoop;
								}
								else
								{
									loopEndsWithCount.erase(it);
								}
							}
						}
					}
					else if (subType == 0x2E) // start loop
					{
						byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
					}
					else if (subType == 0x2F)
					{
						endFlag = true;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
				{
					byte curEventVal;

					byte noteNumber;
					if (statusBit)
					{
						noteNumber = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						curEventVal = eventVal;
					}
					byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
				{
					CString controllerTypeText = "";
					byte controllerType;
					
					if (statusBit)
					{
						controllerType = eventVal;
						previousEventValue;
					}
					else
					{
						controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}
					byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
				{
					byte instrument;
					if (statusBit)
					{
						instrument = eventVal;
						previousEventValue;
					}
					else
					{
						instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
				{
					byte amount;
					if (statusBit)
					{
						amount = eventVal;
						previousEventValue;
					}
					else
					{
						amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
				{
					byte valueLSB;
					if (statusBit)
					{
						valueLSB = eventVal;
						previousEventValue;
					}
					else
					{
						valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}
					
					byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (eventVal == 0xFE) // repeat operation
				{
					// should not be here...

					// no prev event set
				}
				else
				{
					
				}
			}
		}
	}

	for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
	{
		unsigned long offset = CharArrayToLong(&inputMID[i]);
		fprintf(outFile, "Track %X Offset %X: Track Offset %08X\n", counterTrack, i, offset);

		int position = offset;	

		

		if (position != 0)
		{
			int previousEventValue = 0;

			byte* repeatPattern = NULL;
			byte altOffset = 0;
			byte altLength = 0;

			std::map<int, int> loopEndsWithCount;

			int timeAbsolute = 0;
			bool endFlag = false;
			while ((position < inputSize) && !endFlag)
			{
				int timePosition = position;

				if (extendTracksToHighest)
				{
					if (timeAbsolute >= highestTrackLength)
					{
						endFlag = true;

						break;
					}
				}
				unsigned long original;
				unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

				if (extendTracksToHighest)
				{
					if ((timeAbsolute + timeTag) > highestTrackLength)
					{
						endFlag = true;

						break;
					}
				}

				timeAbsolute += timeTag;

				byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

				bool statusBit = false;

				if (eventVal < 0x80)
				{
					// continuation
					statusBit = true;
					fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X)  -   ", timePosition, timeTag, timeAbsolute, original);
				}
				else
				{
					statusBit = false;
					fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X) -   ", timePosition, timeTag, timeAbsolute, original);
				}

				if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
				{
					byte subType;
					if (statusBit)
						subType = eventVal;
					else
						subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (subType == 0x51) // tempo
					{
						int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
						int MICROSECONDS_PER_MINUTE = 60000000;
						float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;

						if (statusBit)
							fprintf(outFile, "!%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
						else
							fprintf(outFile, "%02X%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", eventVal, subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
					}
					else if (subType == 0x2D) // end loop
					{
						
						byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));

						if (statusBit)
							fprintf(outFile, "!%02X%02X%02X%08X Count %u LoopCount %u OffsetBeginning %u (%04X)", subType, loopCount, currentLoopCount, offsetToBeginningLoop, loopCount, currentLoopCount, offsetToBeginningLoop, (position - offsetToBeginningLoop));
						else
							fprintf(outFile, "%02X%02X%02X%02X%08X Count %u LoopCount %u OffsetBeginning %u (%04X)", eventVal, subType, loopCount, currentLoopCount, offsetToBeginningLoop, loopCount, currentLoopCount, offsetToBeginningLoop, (position - offsetToBeginningLoop));
						 //meta status byte (0xFF), a loop end subtype byte (0x2D), a loop count byte (0-255), a current loop count (should be the same as the loop count byte), and four bytes that specify the number of bytes difference between the end of the loop end event, and the begining of the loop start event. (note that if this value is calculated before the pattern matching compression takes place, this value will have to be adjusted to compensate for any compression of data that takes place between the loop end and the loop start.) The loop count value should be a zero to loop forever, otherwise it should be set to one less than the number of times the section should repeat. (i.e. to hear a section eight times, you would set the loop count to seven.)

						if ((loopCount == 0xFF) || (loopCount == 0x00))
						{
							if (extendTracksToHighest)
							{
								if (repeatPattern == NULL)
								{
									position = position - offsetToBeginningLoop;

									fprintf(outFile, " ...Going to %08X", position);
								}
							}
						}
						else
						{
							std::map<int, int>::iterator it = loopEndsWithCount.find(position);
							if (it != loopEndsWithCount.end())
							{
								int countLeft = it->second;

								if (countLeft == 0)
								{
									loopEndsWithCount.erase(it);
								}
								else
								{
									loopEndsWithCount[position] = (countLeft - 1);

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;

										fprintf(outFile, " ...Going to %08X for count left of %02X", position, loopEndsWithCount[position]);
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
							else
							{
								loopEndsWithCount[position] = loopCount - 1;

								if (repeatPattern == NULL)
								{
									position = position - offsetToBeginningLoop;

									fprintf(outFile, " ...Going to %08X for count left of %02X", position, loopEndsWithCount[position]);
								}
								else
								{
									loopEndsWithCount.erase(it);
								}
							}
						}

						fprintf(outFile, "\n");
					}
					else if (subType == 0x2E) // start loop
					{
						byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						if (statusBit)
							fprintf(outFile, "!%02X%02X%02X Loop #%u\n", subType, loopNumber, endLoop, loopNumber);
						else
							fprintf(outFile, "%02X%02X%02X%02X Loop #%u\n", eventVal, subType, loopNumber, endLoop, loopNumber);
					}
					else if (subType == 0x2F)
					{
						if (statusBit)
							fprintf(outFile, "!%02X End of Track\n", subType);
						else
							fprintf(outFile, "%02X%02X End of Track\n", eventVal, subType);
						endFlag = true;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
				{
					byte noteNumber;
					if (statusBit)
						noteNumber = eventVal;
					else
						noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
					byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					if (statusBit)
						fprintf(outFile, "!%02X%02X Duration (%X) - Midi Channel %d NoteNumber %d Velocity %d Duration %d\n", noteNumber, velocity, timeDuration, (previousEventValue&0xF), noteNumber, velocity, timeDuration);
					else
						fprintf(outFile, "%02X%02X%02X Duration (%X) - Midi Channel %d NoteNumber %d Velocity %d Duration %d\n", eventVal, noteNumber, velocity, timeDuration, (eventVal&0xF), noteNumber, velocity, timeDuration);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
				{
					CString controllerTypeText = "";
					byte controllerType;
					if (statusBit)
						controllerType = eventVal;
					else
						controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
					byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (controllerType == 0x00) controllerTypeText = "BankSelect";
					else if (controllerType == 0x01) controllerTypeText = "Modulation";
					else if (controllerType == 0x02) controllerTypeText = "BreathController";
					else if (controllerType == 0x04) controllerTypeText = "FootController";
					else if (controllerType == 0x05) controllerTypeText = "PortamentoTime";
					else if (controllerType == 0x06) controllerTypeText = "DataEntryMSB";
					else if (controllerType == 0x07) controllerTypeText = "MainVolume";
					else if (controllerType == 0x08) controllerTypeText = "Balance";
					else if (controllerType == 0x0A) controllerTypeText = "Pan";
					else if (controllerType == 0x0B) controllerTypeText = "ExpressionController";
					else if (controllerType == 0x0C) controllerTypeText = "EffectControl1";
					else if (controllerType == 0x0D) controllerTypeText = "EffectControl2";
					else if ((controllerType >= 0x10) && (controllerType <= 0x13)) controllerTypeText = "General-PurposeControllers01/04/09";
					else if ((controllerType >= 0x20) && (controllerType <= 0x3F)) controllerTypeText = "LSBforcontrollers0-31";
					else if (controllerType == 0x40) controllerTypeText = "Damperpedalsustain";
					else if (controllerType == 0x41) controllerTypeText = "Portamento";
					else if (controllerType == 0x42) controllerTypeText = "Sostenuto";
					else if (controllerType == 0x43) controllerTypeText = "SoftPedal";
					else if (controllerType == 0x44) controllerTypeText = "LegatoFootswitch";
					else if (controllerType == 0x45) controllerTypeText = "Hold2";
					else if (controllerType == 0x46) controllerTypeText = "SoundController1default:TimberVariation";
					else if (controllerType == 0x47) controllerTypeText = "SoundController2default:Timber/HarmonicContent";
					else if (controllerType == 0x48) controllerTypeText = "SoundController3default:ReleaseTime";
					else if (controllerType == 0x49) controllerTypeText = "SoundController4default:AttackTime";
					else if ((controllerType >= 0x4A) && (controllerType <= 0x4F)) controllerTypeText = "SoundController06/10/09";
					else if ((controllerType >= 0x50) && (controllerType <= 0x53)) controllerTypeText = "General-PurposeControllers05/08/09";
					else if (controllerType == 0x54) controllerTypeText = "PortamentoControl";
					else if (controllerType == 0x5B) controllerTypeText = "Effects1DepthformerlyExternalEffectsDepth";
					else if (controllerType == 0x5C) controllerTypeText = "Effects2DepthformerlyTremoloDepth";
					else if (controllerType == 0x5D) controllerTypeText = "Effects3DepthformerlyChorusDepth";
					else if (controllerType == 0x5E) controllerTypeText = "Effects4DepthformerlyCelesteDetune";
					else if (controllerType == 0x5F) controllerTypeText = "Effects5DepthformerlyPhaserDepth";
					else if (controllerType == 0x60) controllerTypeText = "DataIncrement";
					else if (controllerType == 0x61) controllerTypeText = "DataDecrement";
					else if (controllerType == 0x62) controllerTypeText = "Non-RegisteredParameterNumberLSB";
					else if (controllerType == 0x63) controllerTypeText = "Non-RegisteredParameterNumberMSB";
					else if (controllerType == 0x64) controllerTypeText = "RegisteredParameterNumberLSB";
					else if (controllerType == 0x65) controllerTypeText = "RegisteredParameterNumberMSB";
					else if ((controllerType >= 0x79) && (controllerType <= 0x7F)) controllerTypeText = "ModeMessages";

					if (statusBit)
						fprintf(outFile, "!%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", controllerType, controllerValue, (previousEventValue&0xF), controllerType, controllerTypeText, controllerValue);
					else
						fprintf(outFile, "%02X%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", eventVal, controllerType, controllerValue, (eventVal&0xF), controllerType, controllerTypeText, controllerValue);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
				{
					byte instrument;
					if (statusBit)
						instrument = eventVal;
					else
						instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (statusBit)
					{
						if ((previousEventValue & 0xF) == 9)
							fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (previousEventValue&0xF), instrument);
						else
							fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (previousEventValue&0xF), instrument);
					}
					else
					{
						if ((eventVal & 0xF) == 9)
							fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
						else
							fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
				{
					byte amount;
					if (statusBit)
						amount = eventVal;
					else
						amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (statusBit)
					{
						if ((previousEventValue & 0xF) == 9)
							fprintf(outFile, "!%02X - Midi Channel %d Amount %d\n", amount, (previousEventValue&0xF), amount);
						else
							fprintf(outFile, "!%02X - Midi Channel %d Amount %d\n", amount, (previousEventValue&0xF), amount);
					}
					else
					{
						if ((eventVal & 0xF) == 9)
							fprintf(outFile, "%02X%02X - Midi Channel %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
						else
							fprintf(outFile, "%02X%02X - Midi Channel %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
				{
					byte lsb;
					if (statusBit)
						lsb = eventVal;
					else
						lsb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					byte msb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (statusBit)
					{
						if ((previousEventValue & 0xF) == 9)
							fprintf(outFile, "!%02X%02X - Midi Channel %d lsb %d msb %d\n", lsb, msb, (previousEventValue&0xF), lsb, msb);
						else
							fprintf(outFile, "!%02X%02X - Midi Channel %d lsb %d msb %d\n", lsb, msb, (previousEventValue&0xF), lsb, msb);
					}
					else
					{
						if ((eventVal & 0xF) == 9)
							fprintf(outFile, "%02X%02X%02X - Midi Channel %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
						else
							fprintf(outFile, "%02X%02X%02X - Midi Channel %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (eventVal == 0xFE) // repeat operation
				{
					// should not be here...

					// no prev event set
				}
				else
				{
					fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
				}
			}
		}
		else
		{
			fprintf(outFile, "No Track Data\n");
		}

		fprintf(outFile, "\n");
		counterTrack++;
	}


	fclose(outFile);
}

void CMidiParse::GEMidiToDebugTextFile(CString midiFile, CString textFileOut, bool extendTracksToHighest)
{
	CString filepath = midiFile;
	
	FILE* inFile = fopen(filepath, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Can't read input file " + filepath, "Error", NULL);
		return;
	}

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	rewind(inFile);

	unsigned char* inputMID = new unsigned char[inputSize];

	fread(inputMID, 1, inputSize, inFile);
	fclose(inFile);

	GEMidiToDebugTextFile(inputMID, inputSize, textFileOut, extendTracksToHighest);

	delete [] inputMID;
}


void CMidiParse::DuckDodgersMidiToDebugTextFile(CString midiFile, CString textFileOut, bool extendTracksToHighest)
{
	CString filepath = midiFile;
	
	FILE* inFile = fopen(filepath, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Can't read input file " + filepath, "Error", NULL);
		return;
	}

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	rewind(inFile);

	unsigned char* inputMID = new unsigned char[inputSize];

	fread(inputMID, 1, inputSize, inFile);
	fclose(inFile);

	DuckDodgersMidiToDebugTextFile(inputMID, inputSize, textFileOut, extendTracksToHighest);

	delete [] inputMID;
}

void CMidiParse::DuckDodgersMidiToDebugTextFile(byte* inputMID, int inputSize, CString textFileOut, bool extendTracksToHighest)
{
	FILE* outFile = fopen(textFileOut, "w");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Error outputting file", "Error", NULL);
		return;
	}

	unsigned long lengthHeader = 0;

	fprintf(outFile, "Offset Start Midi Events: %08X \n", 0);

	fprintf(outFile, "Tracks\n");

	// parse midi
	
	int counterTrack = 0;

	int highestTrackLength = 0;

	//for (int iii = 0; iii < (lengthHeader - 4); iii+=4) // ignore last 00000180
	{
		unsigned long absoluteTime = 0;

		unsigned long offset = 0;
		
		int position = offset;	

		//if (position != 0)
		{
			int previousEventValue = 0;

			std::map<int, int> loopEndsWithCount;

			byte* repeatPattern = NULL;
			byte altOffset = 0;
			byte altLength = 0;

			bool endFlag = false;

			while ((position < inputSize) && !endFlag)
			{
				int timePosition = position;

				unsigned long original;
				unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

				absoluteTime += timeTag;

				if (absoluteTime > highestTrackLength)
					highestTrackLength = absoluteTime;


				int vlLength = 0;
				byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

				bool statusBit = false;

				if (eventVal < 0x80)
				{
					// continuation
					statusBit = true;
				}
				else
				{
					statusBit = false;
				}

				if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
				{
					byte subType;
					if (statusBit)
						subType = eventVal;
					else
						subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (subType == 0x51) // tempo
					{
						int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));

					}
					else if (subType == 0x2D) // end loop
					{
						byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));
						
						if ((loopCount == 0xFF) || (loopCount == 0x00))
						{
							break;
						}
						else
						{
							std::map<int, int>::iterator it = loopEndsWithCount.find(position);
							if (it != loopEndsWithCount.end())
							{
								int countLeft = it->second;

								if (countLeft == 0)
								{
									loopEndsWithCount.erase(it);
								}
								else
								{
									loopEndsWithCount[position] = (countLeft - 1);

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
							else
							{
								loopEndsWithCount[position] = loopCount - 1;

								if (repeatPattern == NULL)
								{
									position = position - offsetToBeginningLoop;
								}
								else
								{
									loopEndsWithCount.erase(it);
								}
							}
						}
					}
					else if (subType == 0x2E) // start loop
					{
						byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true); // Always FF
					}
					else if (subType == 0x2F)
					{
						endFlag = true;
					}

					// Not Duck Dodgers
					//if (!statusBit)
						//previousEventValue = eventVal;
				}
				else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
				{
					byte curEventVal;

					byte noteNumber;
					if (statusBit)
					{
						noteNumber = eventVal;
						curEventVal = previousEventValue;
					}
					else
					{
						noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						curEventVal = eventVal;
					}
					byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					//unsigned long timeDuration = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
				{
					CString controllerTypeText = "";
					byte controllerType;
					
					if (statusBit)
					{
						controllerType = eventVal;
						previousEventValue;
					}
					else
					{
						controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}
					byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
				{
					byte instrument;
					if (statusBit)
					{
						instrument = eventVal;
						previousEventValue;
					}
					else
					{
						instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
				{
					byte amount;
					if (statusBit)
					{
						amount = eventVal;
						previousEventValue;
					}
					else
					{
						amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
				{
					byte valueLSB;
					if (statusBit)
					{
						valueLSB = eventVal;
						previousEventValue;
					}
					else
					{
						valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						eventVal;
					}
					
					byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (eventVal == 0xFE) // repeat operation
				{
					// should not be here...

					// no prev event set
				}
				else
				{
					
				}
			}
		}
	}

	//for (int i = 0; i < (lengthHeader - 4); i+=4) // ignore last 00000180
	{
		unsigned long offset = 0;
		fprintf(outFile, "Track %X Offset %X: Track Offset %08X\n", counterTrack, 0, offset);

		int position = offset;	

		

		if (position != 0)
		{
			int previousEventValue = 0;

			byte* repeatPattern = NULL;
			byte altOffset = 0;
			byte altLength = 0;

			std::map<int, int> loopEndsWithCount;

			int timeAbsolute = 0;
			bool endFlag = false;
			while ((position < inputSize) && !endFlag)
			{
				int timePosition = position;

				if (extendTracksToHighest)
				{
					if (timeAbsolute >= highestTrackLength)
					{
						endFlag = true;

						break;
					}
				}
				unsigned long original;
				unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);

				if (extendTracksToHighest)
				{
					if ((timeAbsolute + timeTag) > highestTrackLength)
					{
						endFlag = true;

						break;
					}
				}

				timeAbsolute += timeTag;

				byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

				bool statusBit = false;

				if (eventVal < 0x80)
				{
					// continuation
					statusBit = true;
					fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X)  -   ", timePosition, timeTag, timeAbsolute, original);
				}
				else
				{
					statusBit = false;
					fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X) -   ", timePosition, timeTag, timeAbsolute, original);
				}

				if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
				{
					byte subType;
					if (statusBit)
						subType = eventVal;
					else
						subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (subType == 0x51) // tempo
					{
						int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
						int MICROSECONDS_PER_MINUTE = 60000000;
						float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;

						if (statusBit)
							fprintf(outFile, "!%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
						else
							fprintf(outFile, "%02X%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", eventVal, subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
					}
					else if (subType == 0x2D) // end loop
					{
						
						byte loopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte currentLoopCount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						unsigned long offsetToBeginningLoop = ((((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false));

						if (statusBit)
							fprintf(outFile, "!%02X%02X%02X%08X Count %u LoopCount %u OffsetBeginning %u (%04X)", subType, loopCount, currentLoopCount, offsetToBeginningLoop, loopCount, currentLoopCount, offsetToBeginningLoop, (position - offsetToBeginningLoop));
						else
							fprintf(outFile, "%02X%02X%02X%02X%08X Count %u LoopCount %u OffsetBeginning %u (%04X)", eventVal, subType, loopCount, currentLoopCount, offsetToBeginningLoop, loopCount, currentLoopCount, offsetToBeginningLoop, (position - offsetToBeginningLoop));
						 //meta status byte (0xFF), a loop end subtype byte (0x2D), a loop count byte (0-255), a current loop count (should be the same as the loop count byte), and four bytes that specify the number of bytes difference between the end of the loop end event, and the begining of the loop start event. (note that if this value is calculated before the pattern matching compression takes place, this value will have to be adjusted to compensate for any compression of data that takes place between the loop end and the loop start.) The loop count value should be a zero to loop forever, otherwise it should be set to one less than the number of times the section should repeat. (i.e. to hear a section eight times, you would set the loop count to seven.)

						if ((loopCount == 0xFF) || (loopCount == 0x00))
						{
							if (extendTracksToHighest)
							{
								if (repeatPattern == NULL)
								{
									position = position - offsetToBeginningLoop;

									fprintf(outFile, " ...Going to %08X", position);
								}
							}
						}
						else
						{
							std::map<int, int>::iterator it = loopEndsWithCount.find(position);
							if (it != loopEndsWithCount.end())
							{
								int countLeft = it->second;

								if (countLeft == 0)
								{
									loopEndsWithCount.erase(it);
								}
								else
								{
									loopEndsWithCount[position] = (countLeft - 1);

									if (repeatPattern == NULL)
									{
										position = position - offsetToBeginningLoop;

										fprintf(outFile, " ...Going to %08X for count left of %02X", position, loopEndsWithCount[position]);
									}
									else
									{
										loopEndsWithCount.erase(it);
									}
								}
							}
							else
							{
								loopEndsWithCount[position] = loopCount - 1;

								if (repeatPattern == NULL)
								{
									position = position - offsetToBeginningLoop;

									fprintf(outFile, " ...Going to %08X for count left of %02X", position, loopEndsWithCount[position]);
								}
								else
								{
									loopEndsWithCount.erase(it);
								}
							}
						}

						fprintf(outFile, "\n");
					}
					else if (subType == 0x2E) // start loop
					{
						byte loopNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte endLoop = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						if (statusBit)
							fprintf(outFile, "!%02X%02X%02X Loop #%u\n", subType, loopNumber, endLoop, loopNumber);
						else
							fprintf(outFile, "%02X%02X%02X%02X Loop #%u\n", eventVal, subType, loopNumber, endLoop, loopNumber);
					}
					else if (subType == 0x2F)
					{
						if (statusBit)
							fprintf(outFile, "!%02X End of Track\n", subType);
						else
							fprintf(outFile, "%02X%02X End of Track\n", eventVal, subType);
						endFlag = true;
					}

					// Not Duck Dodgers
					//if (!statusBit)
						//previousEventValue = eventVal;
				}
				else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
				{
					byte noteNumber;
					if (statusBit)
						noteNumber = eventVal;
					else
						noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
					byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (statusBit)
						fprintf(outFile, "!%02X%02X - Midi Channel %d NoteNumber %d Velocity %d\n", noteNumber, velocity, (previousEventValue&0xF), noteNumber, velocity);
					else
						fprintf(outFile, "%02X%02X%02X - Midi Channel %d NoteNumber %d Velocity %d\n", eventVal, noteNumber, velocity, (eventVal&0xF), noteNumber, velocity);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
				{
					CString controllerTypeText = "";
					byte controllerType;
					if (statusBit)
						controllerType = eventVal;
					else
						controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
					byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (controllerType == 0x00) controllerTypeText = "BankSelect";
					else if (controllerType == 0x01) controllerTypeText = "Modulation";
					else if (controllerType == 0x02) controllerTypeText = "BreathController";
					else if (controllerType == 0x04) controllerTypeText = "FootController";
					else if (controllerType == 0x05) controllerTypeText = "PortamentoTime";
					else if (controllerType == 0x06) controllerTypeText = "DataEntryMSB";
					else if (controllerType == 0x07) controllerTypeText = "MainVolume";
					else if (controllerType == 0x08) controllerTypeText = "Balance";
					else if (controllerType == 0x0A) controllerTypeText = "Pan";
					else if (controllerType == 0x0B) controllerTypeText = "ExpressionController";
					else if (controllerType == 0x0C) controllerTypeText = "EffectControl1";
					else if (controllerType == 0x0D) controllerTypeText = "EffectControl2";
					else if ((controllerType >= 0x10) && (controllerType <= 0x13)) controllerTypeText = "General-PurposeControllers01/04/09";
					else if ((controllerType >= 0x20) && (controllerType <= 0x3F)) controllerTypeText = "LSBforcontrollers0-31";
					else if (controllerType == 0x40) controllerTypeText = "Damperpedalsustain";
					else if (controllerType == 0x41) controllerTypeText = "Portamento";
					else if (controllerType == 0x42) controllerTypeText = "Sostenuto";
					else if (controllerType == 0x43) controllerTypeText = "SoftPedal";
					else if (controllerType == 0x44) controllerTypeText = "LegatoFootswitch";
					else if (controllerType == 0x45) controllerTypeText = "Hold2";
					else if (controllerType == 0x46) controllerTypeText = "SoundController1default:TimberVariation";
					else if (controllerType == 0x47) controllerTypeText = "SoundController2default:Timber/HarmonicContent";
					else if (controllerType == 0x48) controllerTypeText = "SoundController3default:ReleaseTime";
					else if (controllerType == 0x49) controllerTypeText = "SoundController4default:AttackTime";
					else if ((controllerType >= 0x4A) && (controllerType <= 0x4F)) controllerTypeText = "SoundController06/10/09";
					else if ((controllerType >= 0x50) && (controllerType <= 0x53)) controllerTypeText = "General-PurposeControllers05/08/09";
					else if (controllerType == 0x54) controllerTypeText = "PortamentoControl";
					else if (controllerType == 0x5B) controllerTypeText = "Effects1DepthformerlyExternalEffectsDepth";
					else if (controllerType == 0x5C) controllerTypeText = "Effects2DepthformerlyTremoloDepth";
					else if (controllerType == 0x5D) controllerTypeText = "Effects3DepthformerlyChorusDepth";
					else if (controllerType == 0x5E) controllerTypeText = "Effects4DepthformerlyCelesteDetune";
					else if (controllerType == 0x5F) controllerTypeText = "Effects5DepthformerlyPhaserDepth";
					else if (controllerType == 0x60) controllerTypeText = "DataIncrement";
					else if (controllerType == 0x61) controllerTypeText = "DataDecrement";
					else if (controllerType == 0x62) controllerTypeText = "Non-RegisteredParameterNumberLSB";
					else if (controllerType == 0x63) controllerTypeText = "Non-RegisteredParameterNumberMSB";
					else if (controllerType == 0x64) controllerTypeText = "RegisteredParameterNumberLSB";
					else if (controllerType == 0x65) controllerTypeText = "RegisteredParameterNumberMSB";
					else if ((controllerType >= 0x79) && (controllerType <= 0x7F)) controllerTypeText = "ModeMessages";

					if (statusBit)
						fprintf(outFile, "!%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", controllerType, controllerValue, (previousEventValue&0xF), controllerType, controllerTypeText, controllerValue);
					else
						fprintf(outFile, "%02X%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", eventVal, controllerType, controllerValue, (eventVal&0xF), controllerType, controllerTypeText, controllerValue);

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
				{
					byte instrument;
					if (statusBit)
						instrument = eventVal;
					else
						instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (statusBit)
					{
						if ((previousEventValue & 0xF) == 9)
							fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (previousEventValue&0xF), instrument);
						else
							fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (previousEventValue&0xF), instrument);
					}
					else
					{
						if ((eventVal & 0xF) == 9)
							fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
						else
							fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
				{
					byte amount;
					if (statusBit)
						amount = eventVal;
					else
						amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (statusBit)
					{
						if ((previousEventValue & 0xF) == 9)
							fprintf(outFile, "!%02X - Midi Channel %d Amount %d\n", amount, (previousEventValue&0xF), amount);
						else
							fprintf(outFile, "!%02X - Midi Channel %d Amount %d\n", amount, (previousEventValue&0xF), amount);
					}
					else
					{
						if ((eventVal & 0xF) == 9)
							fprintf(outFile, "%02X%02X - Midi Channel %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
						else
							fprintf(outFile, "%02X%02X - Midi Channel %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
				{
					byte lsb;
					if (statusBit)
						lsb = eventVal;
					else
						lsb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					byte msb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					if (statusBit)
					{
						if ((previousEventValue & 0xF) == 9)
							fprintf(outFile, "!%02X%02X - Midi Channel %d lsb %d msb %d\n", lsb, msb, (previousEventValue&0xF), lsb, msb);
						else
							fprintf(outFile, "!%02X%02X - Midi Channel %d lsb %d msb %d\n", lsb, msb, (previousEventValue&0xF), lsb, msb);
					}
					else
					{
						if ((eventVal & 0xF) == 9)
							fprintf(outFile, "%02X%02X%02X - Midi Channel %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
						else
							fprintf(outFile, "%02X%02X%02X - Midi Channel %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
					}

					if (!statusBit)
						previousEventValue = eventVal;
				}
				else if (eventVal == 0xFE) // repeat operation
				{
					// should not be here...

					// no prev event set
				}
				else
				{
					fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
				}
			}
		}
		else
		{
			fprintf(outFile, "No Track Data\n");
		}

		fprintf(outFile, "\n");
		counterTrack++;
	}


	fclose(outFile);
}

void CMidiParse::MidiToDebugTextFile(CString midiFile, CString textFileOut)
{
	CString tempFileName = midiFile;

		struct stat results;
		stat(tempFileName, &results);		


		
		FILE* inFile1 = fopen(midiFile, "rb");
		if (inFile1 == NULL)
		{
			MessageBox(NULL, "Error reading file", "Error", NULL);
			return;
		}

		byte* inputMID = new byte[results.st_size];
		fread(inputMID, 1, results.st_size, inFile1);
		fclose(inFile1);


		FILE* outFile = fopen(textFileOut, "w");
		if (outFile == NULL)
		{
			delete [] inputMID;
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}


		if (CharArrayToLong(&inputMID[0]) != 0x4D546864)
		{
			delete [] inputMID;
			fflush(outFile);
			fclose(outFile);
			MessageBox(NULL, "Invalid Midi MThd header", "Error", NULL);
			return;
		}

		int lengthHeader = CharArrayToShort(&inputMID[6]);
		int numTracks = CharArrayToShort(&inputMID[0xA]);
		int division = CharArrayToShort(&inputMID[0xC]);

		fprintf(outFile, "MThd Header Size %08X, Format %02X, # Tracks %02X, Division %04X\n", lengthHeader, CharArrayToShort(&inputMID[8]), numTracks, division);

		

		fprintf(outFile, "Tracks\n");

		// parse midi
		int truePosition = 8 + lengthHeader;
		
		int counterTrack = 0;

		for (int i = 0; i < numTracks; i++)
		{
			if (CharArrayToLong(&inputMID[truePosition]) != 0x4D54726B)
			{
				delete [] inputMID;
				fclose(outFile);
				MessageBox(NULL, "Invalid Midi MTrk header", "Error", NULL);
				return;
			}

			unsigned long midiTrackSize = CharArrayToLong(&inputMID[truePosition + 4]);

			if ((truePosition + 8 + midiTrackSize) > results.st_size)
			{
				delete [] inputMID;
				MessageBox(NULL, "Too big midi track", "Error", NULL);
				fclose(outFile);
				return;
			}

			unsigned long offset = truePosition + 8;

			fprintf(outFile, "Track %X Offset %X: Track Offset %08X\n", counterTrack, i, offset);

			int position = offset;	

			

			if (position != 0)
			{
				int previousEventValue = 0;

				byte* repeatPattern = NULL;
				byte altOffset = 0;
				byte altLength = 0;

				int timeAbsolute = 0;
				bool endFlag = false;
				while ((position < results.st_size) && !endFlag)
				{
					int timePosition = position;

					unsigned long original;
					unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, true);
					timeAbsolute += timeTag;

					byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

					bool statusBit = false;

					if (eventVal < 0x80)
					{
						// continuation
						statusBit = true;
						fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X)  -   ", timePosition, timeTag, timeAbsolute, original);
					}
					else
					{
						statusBit = false;
						fprintf(outFile, "Offset: %08X - Event Delta Time: %d -Abs %d (%08X) -   ", timePosition, timeTag, timeAbsolute, original);
					}

					if ((eventVal == 0xFF) || (statusBit && (previousEventValue == 0xFF))) // meta event
					{
						byte subType;
						if (statusBit)
							subType = eventVal;
						else
							subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);


						if (subType == 0x2F) //End of Track Event.
						{
							unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
						
							if (statusBit)
								fprintf(outFile, "!%02X End of Track\n", subType);
							else
								fprintf(outFile, "%02X%02X End of Track\n", eventVal, subType);
							endFlag = true;
						}
						else if (subType == 0x51) //Set Tempo Event.
						{
							unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 
												
							int microsecondsSinceQuarterNote = ((((ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true)) << 8) | ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true));
							int MICROSECONDS_PER_MINUTE = 60000000;
							float beatsPerMinute = (float)MICROSECONDS_PER_MINUTE / (float)microsecondsSinceQuarterNote;

							if (statusBit)
								fprintf(outFile, "!%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
							else
								fprintf(outFile, "%02X%02X%06X - MicroSecondSinceQuarterNote %d (BPM: %f)\n", eventVal, subType, microsecondsSinceQuarterNote, microsecondsSinceQuarterNote, beatsPerMinute);
						}
						//Various Unused Meta Events.
						else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
						{
							unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

							if (statusBit)
								fprintf(outFile, "!%02X Unused Meta Events ", subType);
							else
								fprintf(outFile, "%02X%02X Unused Meta Events ", eventVal, subType);
						
							for (int i = 0; i < length; i++)
							{
								unsigned char tempByte = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
								fprintf(outFile, "%02X", tempByte);
							}

							fprintf(outFile, "\n");
						}
						else if (subType == 0x7F) //Unused Sequencer Specific Event.
						{
							int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);

							if (statusBit)
								fprintf(outFile, "!%02X Unused Sequencer Specific Event ", subType);
							else
								fprintf(outFile, "%02X%02X Unused Sequencer Specific Event ", eventVal, subType);

							// subtract length
							for (int i = 0; i < length; i++)
							{
								unsigned char tempByte = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
								fprintf(outFile, "%02X", tempByte);
							}

							fprintf(outFile, "\n");
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80) && (previousEventValue < 0x90)))
					{
						byte noteNumber;
						if (statusBit)
							noteNumber = eventVal;
						else
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
							fprintf(outFile, "!%02X%02X - Midi Channel OFF %d NoteNumber %d Velocity %d\n", noteNumber, velocity, (previousEventValue&0xF), noteNumber, velocity);
						else
							fprintf(outFile, "%02X%02X%02X - Midi Channel OFF %d NoteNumber %d Velocity %d\n", eventVal, noteNumber, velocity, (eventVal&0xF), noteNumber, velocity);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90) && (previousEventValue < 0xA0)))
					{
						byte noteNumber;
						if (statusBit)
							noteNumber = eventVal;
						else
							noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
							fprintf(outFile, "!%02X%02X - Midi Channel ON %d NoteNumber %d Velocity %d\n", noteNumber, velocity, (previousEventValue&0xF), noteNumber, velocity);
						else
							fprintf(outFile, "%02X%02X%02X - Midi Channel ON %d NoteNumber %d Velocity %d\n", eventVal, noteNumber, velocity, (eventVal&0xF), noteNumber, velocity);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xB0) && (eventVal < 0xC0)) || (statusBit && (previousEventValue >= 0xB0) && (previousEventValue < 0xC0))) // controller change
					{
						CString controllerTypeText = "";
						byte controllerType;
						if (statusBit)
							controllerType = eventVal;
						else
							controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);
						byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (controllerType == 0x00) controllerTypeText = "BankSelect";
						else if (controllerType == 0x01) controllerTypeText = "Modulation";
						else if (controllerType == 0x02) controllerTypeText = "BreathController";
						else if (controllerType == 0x04) controllerTypeText = "FootController";
						else if (controllerType == 0x05) controllerTypeText = "PortamentoTime";
						else if (controllerType == 0x06) controllerTypeText = "DataEntryMSB";
						else if (controllerType == 0x07) controllerTypeText = "MainVolume";
						else if (controllerType == 0x08) controllerTypeText = "Balance";
						else if (controllerType == 0x0A) controllerTypeText = "Pan";
						else if (controllerType == 0x0B) controllerTypeText = "ExpressionController";
						else if (controllerType == 0x0C) controllerTypeText = "EffectControl1";
						else if (controllerType == 0x0D) controllerTypeText = "EffectControl2";
						else if ((controllerType >= 0x10) && (controllerType <= 0x13)) controllerTypeText = "General-PurposeControllers01/04/09";
						else if ((controllerType >= 0x20) && (controllerType <= 0x3F)) controllerTypeText = "LSBforcontrollers0-31";
						else if (controllerType == 0x40) controllerTypeText = "Damperpedalsustain";
						else if (controllerType == 0x41) controllerTypeText = "Portamento";
						else if (controllerType == 0x42) controllerTypeText = "Sostenuto";
						else if (controllerType == 0x43) controllerTypeText = "SoftPedal";
						else if (controllerType == 0x44) controllerTypeText = "LegatoFootswitch";
						else if (controllerType == 0x45) controllerTypeText = "Hold2";
						else if (controllerType == 0x46) controllerTypeText = "SoundController1default:TimberVariation";
						else if (controllerType == 0x47) controllerTypeText = "SoundController2default:Timber/HarmonicContent";
						else if (controllerType == 0x48) controllerTypeText = "SoundController3default:ReleaseTime";
						else if (controllerType == 0x49) controllerTypeText = "SoundController4default:AttackTime";
						else if ((controllerType >= 0x4A) && (controllerType <= 0x4F)) controllerTypeText = "SoundController06/10/09";
						else if ((controllerType >= 0x50) && (controllerType <= 0x53)) controllerTypeText = "General-PurposeControllers05/08/09";
						else if (controllerType == 0x54) controllerTypeText = "PortamentoControl";
						else if (controllerType == 0x5B) controllerTypeText = "Effects1DepthformerlyExternalEffectsDepth";
						else if (controllerType == 0x5C) controllerTypeText = "Effects2DepthformerlyTremoloDepth";
						else if (controllerType == 0x5D) controllerTypeText = "Effects3DepthformerlyChorusDepth";
						else if (controllerType == 0x5E) controllerTypeText = "Effects4DepthformerlyCelesteDetune";
						else if (controllerType == 0x5F) controllerTypeText = "Effects5DepthformerlyPhaserDepth";
						else if (controllerType == 0x60) controllerTypeText = "DataIncrement";
						else if (controllerType == 0x61) controllerTypeText = "DataDecrement";
						else if (controllerType == 0x62) controllerTypeText = "Non-RegisteredParameterNumberLSB";
						else if (controllerType == 0x63) controllerTypeText = "Non-RegisteredParameterNumberMSB";
						else if (controllerType == 0x64) controllerTypeText = "RegisteredParameterNumberLSB";
						else if (controllerType == 0x65) controllerTypeText = "RegisteredParameterNumberMSB";
						else if ((controllerType >= 0x79) && (controllerType <= 0x7F)) controllerTypeText = "ModeMessages";

						if (statusBit)
							fprintf(outFile, "!%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", controllerType, controllerValue, (previousEventValue&0xF), controllerType, controllerTypeText, controllerValue);
						else
							fprintf(outFile, "%02X%02X%02X - Midi Channel %d ControllerType %d (%s) Value %d\n", eventVal, controllerType, controllerValue, (eventVal&0xF), controllerType, controllerTypeText, controllerValue);

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0) && (previousEventValue < 0xD0))) // change instrument
					{
						byte instrument;
						if (statusBit)
							instrument = eventVal;
						else
							instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
						{
							if ((previousEventValue & 0xF) == 9)
								fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (previousEventValue&0xF), instrument);
							else
								fprintf(outFile, "!%02X - Midi Channel %d Instrument %d\n", instrument, (previousEventValue&0xF), instrument);
						}
						else
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
							else
								fprintf(outFile, "%02X%02X - Midi Channel %d Instrument %d\n", eventVal, instrument, (eventVal&0xF), instrument);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xD0) && (eventVal < 0xE0)) || (statusBit && (previousEventValue >= 0xD0) && (previousEventValue < 0xE0))) // channel aftertouch
					{
						byte amount;
						if (statusBit)
							amount = eventVal;
						else
							amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
						{
							if ((previousEventValue & 0xF) == 9)
								fprintf(outFile, "!%02X - Channel Aftertouch %d Amount %d\n", amount, (previousEventValue&0xF), amount);
							else
								fprintf(outFile, "!%02X - Channel Aftertouch %d Amount %d\n", amount, (previousEventValue&0xF), amount);
						}
						else
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "%02X%02X - Channel Aftertouch %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
							else
								fprintf(outFile, "%02X%02X - Channel Aftertouch %d Amount %d\n", eventVal, amount, (eventVal&0xF), amount);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (((eventVal >= 0xE0) && (eventVal < 0xF0)) || (statusBit && (previousEventValue >= 0xE0) && (previousEventValue < 0xF0))) // pitch bend
					{
						byte lsb;
						if (statusBit)
							lsb = eventVal;
						else
							lsb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						byte msb = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, true);

						if (statusBit)
						{
							if ((previousEventValue & 0xF) == 9)
								fprintf(outFile, "!%02X%02X - Pitch Bend %d lsb %d msb %d\n", lsb, msb, (previousEventValue&0xF), lsb, msb);
							else
								fprintf(outFile, "!%02X%02X - Pitch Bend %d lsb %d msb %d\n", lsb, msb, (previousEventValue&0xF), lsb, msb);
						}
						else
						{
							if ((eventVal & 0xF) == 9)
								fprintf(outFile, "%02X%02X%02X - Pitch Bend %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
							else
								fprintf(outFile, "%02X%02X%02X - Pitch Bend %d lsb %d msb %d\n", eventVal, lsb, msb, (eventVal&0xF), lsb, msb);
						}

						if (!statusBit)
							previousEventValue = eventVal;
					}
					else if (eventVal == 0xFE) // repeat operation
					{
						// should not be here...

						// no prev event set
					}
					else
					{
						fprintf(outFile, "%02X ERROR MISSING PARSE OF TYPE\n", eventVal);
					}
				}
			}
			else
			{
				fprintf(outFile, "No Track Data\n");
			}

			fprintf(outFile, "\n");
			counterTrack++;

			truePosition = truePosition + 8 + midiTrackSize;
		}

		
		fclose(outFile);
}

void CMidiParse::GenerateTestPattern(int type, CString outputFile)
{
	try
	{
		FILE* outFile = fopen(outputFile, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return;
		}

		// parse midi
		
		unsigned long tempLong = Flip32Bit(0x4D546864);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00000006);
		fwrite(&tempLong, 1 ,4 , outFile);
		tempLong = Flip32Bit(0x00010000 | 0x0001); // num tracks
		fwrite(&tempLong, 1 ,4 , outFile);

		unsigned long division = 0x01E0;

		unsigned short tempShort = division;
		tempShort = Flip16Bit(tempShort);
		fwrite(&tempShort, 1 ,2 , outFile);

		unsigned long absoluteTime = 0;

		int trackEventCount = 0;
		TrackEvent* trackEvents = new TrackEvent[0x30000];

		for (int j = 0; j < 0x30000; j++)
			trackEvents[j].contents = NULL;

		int position = 4;	

		tempLong = Flip32Bit(0x4D54726B);
		fwrite(&tempLong, 1 ,4 , outFile);


		int trackNumber = 1;
		int instrumentNumber = 0;

		float tempo = 120;
		unsigned long tempoVal = 60000000.0 / tempo;

		trackEvents[trackEventCount].type = 0xFF;
		trackEvents[trackEventCount].contentSize = 5;
		trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
		
		trackEvents[trackEventCount].contents[0] = 0x51;
		trackEvents[trackEventCount].contents[1] = 0x3;
		trackEvents[trackEventCount].contents[2] = (tempoVal >> 16) & 0xFF;
		trackEvents[trackEventCount].contents[3] = (tempoVal >> 8) & 0xFF;
		trackEvents[trackEventCount].contents[4] = (tempoVal) & 0xFF;

		trackEventCount++;

		


		trackEvents[trackEventCount].type = 0xC0 | trackNumber;

		trackEvents[trackEventCount].contentSize = 1;
		trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
		trackEvents[trackEventCount].contents[0] = instrumentNumber;
	
		trackEventCount++;


		byte controllerType = 0x07;
		byte controllerValue = 0x7F;

		trackEvents[trackEventCount].type = 0xB0 | trackNumber;
		trackEvents[trackEventCount].contentSize = 2;
		trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
		trackEvents[trackEventCount].contents[0] = controllerType;
		trackEvents[trackEventCount].contents[1] = controllerValue;

		trackEventCount++;



		if ((type == 0) || (type == 1) || (type == 2))
		{
			for (unsigned char note = 0x18; note < 0x78; note++)
			{
				if (type == 0)
				{
					trackEvents[trackEventCount].deltaTime = 0;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x90 | trackNumber;
					
					byte velocity = 0x7F;

					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = note;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;


					trackEvents[trackEventCount].deltaTime = 10;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x90 | trackNumber;
					
					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = note - 5;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;






					trackEvents[trackEventCount].deltaTime = 10;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x80 | trackNumber;
					
					velocity = 0x2F;


					trackEvents[trackEventCount].durationTime = 0; // to be filled in
					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = note;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;




					trackEvents[trackEventCount].deltaTime = 10;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x80 | trackNumber;
					
					velocity = 0x2F;


					trackEvents[trackEventCount].durationTime = 0; // to be filled in
					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = note - 5;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;
				}
				else if (type == 1)
				{
					trackEvents[trackEventCount].deltaTime = 200;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x90 | trackNumber;
					
					byte velocity = 0x7F;

					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = note;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;






					trackEvents[trackEventCount].deltaTime = 200;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x80 | trackNumber;
					
					velocity = 0x2F;


					trackEvents[trackEventCount].durationTime = 0; // to be filled in
					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = note;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;
				}
				else if (type == 2)
				{
					trackEvents[trackEventCount].deltaTime = 200;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x90 | trackNumber;
					
					byte velocity = 0x7F;

					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = 0x78;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;






					trackEvents[trackEventCount].deltaTime = 200;
					trackEvents[trackEventCount].obsoleteEvent = false;

					absoluteTime += trackEvents[trackEventCount].deltaTime;
					trackEvents[trackEventCount].absoluteTime = absoluteTime;

					trackEvents[trackEventCount].type = 0x80 | trackNumber;
					
					velocity = 0x2F;


					trackEvents[trackEventCount].durationTime = 0; // to be filled in
					trackEvents[trackEventCount].contentSize = 2;
					trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
					trackEvents[trackEventCount].contents[0] = 0x78;
					trackEvents[trackEventCount].contents[1] = velocity;

					trackEventCount++;
				}
			}
		}
		else if (type == 3)
		{
			int note = 0x78;
			for (int x = 0; x < 2000; x+=10)
			{
				trackEvents[trackEventCount].deltaTime = 0;
				trackEvents[trackEventCount].obsoleteEvent = false;

				absoluteTime += trackEvents[trackEventCount].deltaTime;
				trackEvents[trackEventCount].absoluteTime = absoluteTime;

				trackEvents[trackEventCount].type = 0x90 | trackNumber;
				
				byte velocity = 0x7F; // seems ignored on import

				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = note;
				trackEvents[trackEventCount].contents[1] = velocity;

				trackEventCount++;







				trackEvents[trackEventCount].deltaTime = x;
				trackEvents[trackEventCount].obsoleteEvent = false;

				absoluteTime += trackEvents[trackEventCount].deltaTime;
				trackEvents[trackEventCount].absoluteTime = absoluteTime;

				trackEvents[trackEventCount].type = 0x80 | trackNumber;


				trackEvents[trackEventCount].durationTime = 0; // to be filled in
				trackEvents[trackEventCount].contentSize = 2;
				trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
				trackEvents[trackEventCount].contents[0] = note;
				trackEvents[trackEventCount].contents[1] = velocity;

				trackEventCount++;
			}

			trackEvents[trackEventCount].deltaTime = 0;
			trackEvents[trackEventCount].obsoleteEvent = false;

			absoluteTime += trackEvents[trackEventCount].deltaTime;
			trackEvents[trackEventCount].absoluteTime = absoluteTime;

			trackEvents[trackEventCount].type = 0x90 | trackNumber;
			
			byte velocity = 0x7F; // seems ignored on import

			trackEvents[trackEventCount].contentSize = 2;
			trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
			trackEvents[trackEventCount].contents[0] = note;
			trackEvents[trackEventCount].contents[1] = velocity;

			trackEventCount++;







			trackEvents[trackEventCount].deltaTime = 48;
			trackEvents[trackEventCount].obsoleteEvent = false;

			absoluteTime += trackEvents[trackEventCount].deltaTime;
			trackEvents[trackEventCount].absoluteTime = absoluteTime;

			trackEvents[trackEventCount].type = 0x80 | trackNumber;


			trackEvents[trackEventCount].durationTime = 0; // to be filled in
			trackEvents[trackEventCount].contentSize = 2;
			trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
			trackEvents[trackEventCount].contents[0] = note;
			trackEvents[trackEventCount].contents[1] = velocity;

			trackEventCount++;
		}

		trackEvents[trackEventCount].type = 0xFF;
		trackEvents[trackEventCount].contentSize = 2;
		trackEvents[trackEventCount].contents = new byte[trackEvents[trackEventCount].contentSize];
		
		trackEvents[trackEventCount].contents[0] = 0x2F;
		trackEvents[trackEventCount].contents[1] = 0x0;

		trackEventCount++;
			
		
		unsigned long timeOffset = 0;

		unsigned long sizeData = 0;
		byte previousTrackEvent = 0x0;

		
		for (int j = 0; j < trackEventCount; j++)
		{
			TrackEvent trackEvent =  trackEvents[j];
			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;

				sizeData += lengthTimeDelta;


				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
				{
					sizeData += 1;
				}

				sizeData += trackEvent.contentSize;

				previousTrackEvent = trackEvent.type;
			}
		}


		tempLong = Flip32Bit(sizeData);
		fwrite(&tempLong,1, 4, outFile);

		timeOffset = 0;
		previousTrackEvent = 0x0;
		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			TrackEvent trackEvent = trackEvents[eventCount];

			if (trackEvent.obsoleteEvent)
			{
				timeOffset += trackEvent.deltaTime;
			}
			else
			{
				unsigned long lengthTimeDelta = 0;
				unsigned long timeDelta = ReturnVLBytes((trackEvent.deltaTime + timeOffset), lengthTimeDelta);
				timeOffset = 0;
				WriteVLBytes(outFile, timeDelta, lengthTimeDelta, false);

				if ((trackEvent.type != previousTrackEvent) || (trackEvent.type >= 0xF0))
				{
					fwrite(&trackEvent.type, 1, 1, outFile);
				}

				fwrite(trackEvent.contents, 1, trackEvent.contentSize, outFile);

				previousTrackEvent = trackEvent.type;
			}
		}

		for (int eventCount = 0; eventCount < trackEventCount; eventCount++)
		{
			if (trackEvents[eventCount].contents != NULL)
			{
				delete [] trackEvents[eventCount].contents;
				trackEvents[eventCount].contents = NULL;
			}
		}

		delete [] trackEvents;

		// just one track
		
		fflush(outFile);	
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

int CMidiParse::HexToInt(char inChar)
{
	switch(inChar)
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	case 'A':
		return 10;
	case 'a':
		return 10;
	case 'B':
		return 11;
	case 'b':
		return 11;
	case 'C':
		return 12;
	case 'c':
		return 12;
	case 'D':
		return 13;
	case 'd':
		return 13;
	case 'E':
		return 14;
	case 'e':
		return 14;
	case 'F':
		return 15;
	case 'f':
		return 15;
	default:
		return 0;
	}
}

unsigned short CMidiParse::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

void CMidiParse::ExportToBin(CString gameName, unsigned char* buffer, unsigned long address, unsigned long size, CString fileName, bool& compressed, unsigned long extra)
{
	gameName.Trim();

	if (compressed)
	{
		if (gameName.CompareNoCase("MidiLZSSWilliams") == 0)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed, "WILLIAMS");

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("LZSS_0BSng") == 0)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address+4], fileSizeCompressed, outputDecompressed, "LZSS_0");
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;

		}
		else if (gameName.CompareNoCase("LZSS_0Sng") == 0)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed, "LZSS_0");
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;

		}
		else if (gameName.CompareNoCase("AVL_0Sng") == 0)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed, "LZSS");
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;

		}
		else if (gameName.CompareNoCase("VigilanteSng") == 0)
		{
			int fileSizeCompressed = size;
			CVigilanteDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("ViewpointSng") == 0)
		{
			int fileSizeCompressed = size;
			CViewpointDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], outputDecompressed, fileSizeCompressed, extra);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("RugratsSng") == 0)
		{
			int fileSizeCompressed = size;
			CRugratsDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("RNCSng") == 0)
		{
			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.unpackM1(&buffer[address], outputDecompressed, 0x0000, fileSizeCompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("HexenSng") == 0)
		{
			int fileSizeCompressed = -1;
			CHexenDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.decode(&buffer[address], outputDecompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("ASMICSng") == 0)
		{
			int fileSizeCompressed = -1;
			CASMICDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("SnowSng") == 0)
		{
			int fileSizeCompressed = size;
			CSnowDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("ArmySargeSng") == 0)
		{
			int fileSizeCompressed = -1;
			n643docompression decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.uncompressedSize(&buffer[address], true);

			expectedSize = decode.dec(&buffer[address], expectedSize, outputDecompressed, fileSizeCompressed, true, false);
			

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("ArmySarge2Sng") == 0)
		{
			int fileSizeCompressed = -1;
			n643docompression decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.uncompressedSize(&buffer[address], true);

			expectedSize = decode.dec(&buffer[address], expectedSize, outputDecompressed, fileSizeCompressed, true, true);
			

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("Quake2Sng") == 0)
		{
			int fileSizeCompressed = -1;
			CQuakeDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];

			int decompressedSize = decode.aridecode(&buffer[address], 0x50000, outputDecompressed, decompressedSize);
			

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < decompressedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("TazSng") == 0)
		{
			CString inTempFileName;
			inTempFileName.Format("tempASAS%08X.mus", address);

			CString outTempFileName;
			outTempFileName.Format("tempASAS%08X.musb", address);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			FILE* outTempIn = fopen(inTempFileName, "wb");
			if (outTempIn == NULL)
			{
				return;
			}

			int musSize = size;

			unsigned long expectedSize = CharArrayToLong(&buffer[address+4]) - 0x14;
			fwrite(&expectedSize, 1, 4, outTempIn);
			fwrite(&buffer[address+0x14], 1, musSize-0x14, outTempIn);
			fflush(outTempIn);
			fclose(outTempIn);

			flzh huffman;
			huffman.infile = fopen(inTempFileName, "rb");
			if (huffman.infile == NULL)
			{
				::DeleteFile(inTempFileName);
				return;
			}

			huffman.outfile = fopen(outTempFileName, "wb");
			if (huffman.outfile == NULL)
			{
				return;
			}

			huffman.Decode();

			fflush(huffman.outfile);
			fclose(huffman.infile);
			fclose(huffman.outfile);

			musSize = CSharedFunctionsMidi::GetSizeFile(outTempFileName);

			FILE* inTempIn = fopen(outTempFileName, "rb");
			if (inTempIn == NULL)
			{
				return;
			}
			
			unsigned char* outputDecompressed = new unsigned char[musSize];
			fread(outputDecompressed, 1, musSize, inTempIn);

			fclose(inTempIn);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				delete [] outputDecompressed;
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("ARISng") == 0)
		{
			CString inTempFileName;
			inTempFileName.Format("tempASAS%08X.mus", address);

			CString outTempFileName;
			outTempFileName.Format("tempASAS%08X.musb", address);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			FILE* outTempIn = fopen(inTempFileName, "wb");
			if (outTempIn == NULL)
			{
				return;
			}

			int musSize = size;

			unsigned long expectedSize = CharArrayToLong(&buffer[address-4]);
			fwrite(&expectedSize, 1, 4, outTempIn);
			fwrite(&buffer[address], 1, musSize, outTempIn);
			fflush(outTempIn);
			fclose(outTempIn);

			CLZARIDecoder lzari;
			lzari.infile = fopen(inTempFileName, "rb");
			if (lzari.infile == NULL)
			{
				::DeleteFile(inTempFileName);
				return;
			}

			lzari.outfile = fopen(outTempFileName, "wb");
			if (lzari.outfile == NULL)
			{
				return;
			}

			lzari.Decode();

			fflush(lzari.outfile);
			fclose(lzari.infile);
			fclose(lzari.outfile);

			musSize = CSharedFunctionsMidi::GetSizeFile(outTempFileName);

			FILE* inTempIn = fopen(outTempFileName, "rb");
			if (inTempIn == NULL)
			{
				return;
			}
			
			unsigned char* outputDecompressed = new unsigned char[musSize];
			fread(outputDecompressed, 1, musSize, inTempIn);

			fclose(inTempIn);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				delete [] outputDecompressed;
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("FLA2Sng") == 0)
		{
			int fileSizeCompressed = -1;
			CFLA2Decoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];

			int expectedSizeUncompressed = Flip32Bit(CharArrayToLong(&buffer[address+4]));
			int expectedSize = decode.decFLA2(&buffer[address+8], fileSizeCompressed, expectedSizeUncompressed, outputDecompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("RNCSngOffset") == 0)
		{
			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.unpackM1(&buffer[address], outputDecompressed, 0x0000, fileSizeCompressed);

			unsigned long offset = CharArrayToLong(&outputDecompressed[0]);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize - offset; x++)
			{
				fwrite(&outputDecompressed[offset + x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("Yay0Sng") == 0)
		{
			int fileSizeCompressed = -1;
			YAY0 decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.decodeAll(&buffer[address], outputDecompressed, fileSizeCompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("BlitzSng") == 0)
		{
			int fileSizeCompressed = -1;
			CBlitzDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], outputDecompressed, fileSizeCompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("MarioTennisSng") == 0)
		{
			int fileSizeCompressed = -1;
			CMarioTennisDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else if (gameName.CompareNoCase("MultiPartTigSng") == 0)
		{
			MessageBox(NULL, "Can't write Tig to bin, not supported", "Error", NULL);
		}
		else if (gameName.CompareNoCase("Konami") == 0)
		{
			int fileSizeCompressed = size;
			CNaganoDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&buffer[address], fileSizeCompressed, outputDecompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;

		}
		else
		{
			DecompressToFile(&buffer[address], size, fileName);
		}
	}
	
	else
	{
		FILE* outFile = fopen(fileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Cannot Write File", "Error", NULL);
			return;
		}
		for (int x = 0; x < size; x++)
		{
			fwrite(&buffer[address+x], 1, 1, outFile);
		}
		fclose(outFile);
	}
}

void CMidiParse::ExportToMidi(CString gameName, unsigned char* gamebuffer, int gamebufferSize, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division, bool& compressed, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool calculateInstrumentCountOnly, bool separateByInstrument, bool generateDebugTextFile, unsigned long extra, unsigned long extra2, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, bool isPreview)
{
	ExportToMidi(gameName, gamebuffer, gamebufferSize, address, size, fileName, gameType, numberInstruments, division, compressed, hasLoopPoint, loopStart, loopEnd, calculateInstrumentCountOnly, separateByInstrument, generateDebugTextFile, extra, extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false, 4, isPreview);
}

void CMidiParse::ExportToMidi(CString gameName, unsigned char* gamebuffer, int gamebufferSize, unsigned long address, unsigned long size, CString fileName, CString gameType, int& numberInstruments, unsigned long division, bool& compressed, bool& hasLoopPoint, int& loopStart, int& loopEnd, bool calculateInstrumentCountOnly, bool separateByInstrument, bool generateDebugTextFile, unsigned long extra, unsigned long extra2, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, bool usePitchBendSensitity, int pitchBendSensitity, bool isPreview)
{
	gameName.Trim();
	gameType.Trim();
	if (gameType.CompareNoCase("BanjoTooie") == 0)
	{
		if (compressed)
		{
			compress->SetGame(BANJOTOOIE);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				BTMidiToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity);
				delete [] outputBuffer;
			}
		}
		else
		{
			BTMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity);
		}
	}
	else if (gameType.CompareNoCase("MIDx") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			MIDxMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments);
		}
	}
	else if (gameType.CompareNoCase("Sng") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			SngToMidi(&gamebuffer[address], size, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, &gamebuffer[address], size, fileName + " TrackParseDebug.txt", extra);
		}
	}
	else if (gameType.CompareNoCase("Konami") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CNaganoDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed);

			KonamiToMidi(gamebuffer, gamebufferSize, outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
			if (generateDebugTextFile)
				KonamiToDebugTextFile(gamebuffer, gamebufferSize, gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);

			delete [] outputDecompressed;
		}
		else
		{
			KonamiToMidi(gamebuffer, gamebufferSize, &gamebuffer[address], size, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
			if (generateDebugTextFile)
				KonamiToDebugTextFile(gamebuffer, gamebufferSize, gameName, address, &gamebuffer[address], size, fileName + " TrackParseDebug.txt", writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
		}
	}
	else if (gameType.CompareNoCase("ZLIBSSEQ") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			GECompression gedecompress;
			gedecompress.SetGame(MORTALKOMBAT);
			gedecompress.SetCompressedBuffer(&gamebuffer[address], size);
			int fileSizeUncompressed;
			unsigned char* outputDecompressedSSEQ = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

			SSEQToMidi(gamebuffer, gamebufferSize, outputDecompressedSSEQ, fileSizeUncompressed, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
			if (generateDebugTextFile)
				SSEQToDebugTextFile(gamebuffer, gamebufferSize, gameName, address, &gamebuffer[address], size, fileName + " TrackParseDebug.txt", writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);

			delete [] outputDecompressedSSEQ;
		}
		else
		{
			SSEQToMidi(gamebuffer, gamebufferSize, &gamebuffer[address], size, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
			if (generateDebugTextFile)
				SSEQToDebugTextFile(gamebuffer, gamebufferSize, gameName, address, &gamebuffer[address], size, fileName + " TrackParseDebug.txt", writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
		}
	}
	else if (gameType.CompareNoCase("SSEQ") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			SSEQToMidi(gamebuffer, gamebufferSize, &gamebuffer[address], size, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
			if (generateDebugTextFile)
				SSEQToDebugTextFile(gamebuffer, gamebufferSize, gameName, address, &gamebuffer[address], size, fileName + " TrackParseDebug.txt", writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
		}
	}
	else if (gameType.CompareNoCase("PaperMario") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			std::vector<SngInstrumentPaperMario> usedInstruments;
			std::vector<SngDrumPaperMario> usedPercussionSet;
			std::vector<int> usedExtraInstruments;
			
			PaperMarioToMidi(gamebuffer, gamebufferSize, &gamebuffer[address], size, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, usedInstruments, usedPercussionSet, usedExtraInstruments, extendTracksToHighest);
			if (generateDebugTextFile)
				PaperMarioToDebugTextFile(gamebuffer, gamebufferSize, gameName, address, &gamebuffer[address], size, fileName + " TrackParseDebug.txt", writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);
		}
	}
	else if (gameType.CompareNoCase("RNCSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.unpackM1(&gamebuffer[address], outputDecompressed, 0x0000, fileSizeCompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("HexenSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			CHexenDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.decode(&gamebuffer[address], outputDecompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("ASMICSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			CASMICDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("SnowSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CSnowDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("ArmySargeSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			n643docompression decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.uncompressedSize(&gamebuffer[address], true);

			expectedSize = decode.dec(&gamebuffer[address], expectedSize, outputDecompressed, fileSizeCompressed, true, false);
			

			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{

		}
	}
	else if (gameType.CompareNoCase("ArmySarge2Sng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			n643docompression decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.uncompressedSize(&gamebuffer[address], true);

			expectedSize = decode.dec(&gamebuffer[address], expectedSize, outputDecompressed, fileSizeCompressed, true, true);
			

			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);


			delete [] outputDecompressed;
		}
		else
		{

		}
	}
	else if (gameType.CompareNoCase("Quake2Sng") == 0)
	{
		if (compressed)
		{
			CQuakeDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];

			int decompressedSize = decode.aridecode(&gamebuffer[address], 0x50000, outputDecompressed, decompressedSize);

			SngToMidi(outputDecompressed, decompressedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, decompressedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{

		}
	}
	else if (gameType.CompareNoCase("TazSng") == 0)
	{
		if (compressed)
		{
			CString inTempFileName;
			inTempFileName.Format("tempASAS%08X.mus", address);

			CString outTempFileName;
			outTempFileName.Format("tempASAS%08X.musb", address);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			FILE* outTempIn = fopen(inTempFileName, "wb");
			if (outTempIn == NULL)
			{
				return;
			}

			int musSize = size;

			unsigned long expectedSize = CharArrayToLong(&gamebuffer[address+4]) - 0x14;
			fwrite(&expectedSize, 1, 4, outTempIn);
			fwrite(&gamebuffer[address+0x14], 1, musSize-0x14, outTempIn);
			fflush(outTempIn);
			fclose(outTempIn);

			flzh huffman;
			huffman.infile = fopen(inTempFileName, "rb");
			if (huffman.infile == NULL)
			{
				::DeleteFile(inTempFileName);
				return;
			}

			huffman.outfile = fopen(outTempFileName, "wb");
			if (huffman.outfile == NULL)
			{
				return;
			}

			huffman.Decode();

			fflush(huffman.outfile);
			fclose(huffman.infile);
			fclose(huffman.outfile);

			musSize = CSharedFunctionsMidi::GetSizeFile(outTempFileName);

			FILE* inTempIn = fopen(outTempFileName, "rb");
			if (inTempIn == NULL)
			{
				return;
			}
			
			unsigned char* outputDecompressed = new unsigned char[musSize];
			fread(outputDecompressed, 1, musSize, inTempIn);

			fclose(inTempIn);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("ARISng") == 0)
	{
		if (compressed)
		{
			CString inTempFileName;
			inTempFileName.Format("tempASAS%08X.mus", address);

			CString outTempFileName;
			outTempFileName.Format("tempASAS%08X.musb", address);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			FILE* outTempIn = fopen(inTempFileName, "wb");
			if (outTempIn == NULL)
			{
				return;
			}

			int musSize = size;

			unsigned long expectedSize = CharArrayToLong(&gamebuffer[address-4]);
			fwrite(&expectedSize, 1, 4, outTempIn);
			fwrite(&gamebuffer[address], 1, musSize, outTempIn);
			fflush(outTempIn);
			fclose(outTempIn);

			CLZARIDecoder lzari;
			lzari.infile = fopen(inTempFileName, "rb");
			if (lzari.infile == NULL)
			{
				::DeleteFile(inTempFileName);
				return;
			}

			lzari.outfile = fopen(outTempFileName, "wb");
			if (lzari.outfile == NULL)
			{
				return;
			}

			lzari.Decode();

			fflush(lzari.outfile);
			fclose(lzari.infile);
			fclose(lzari.outfile);

			musSize = CSharedFunctionsMidi::GetSizeFile(outTempFileName);

			FILE* inTempIn = fopen(outTempFileName, "rb");
			if (inTempIn == NULL)
			{
				return;
			}
			
			unsigned char* outputDecompressed = new unsigned char[musSize];
			fread(outputDecompressed, 1, musSize, inTempIn);

			fclose(inTempIn);

			::DeleteFile(inTempFileName);
			::DeleteFile(outTempFileName);

			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("FLA2Sng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			CFLA2Decoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];

			int expectedSizeUncompressed = Flip32Bit(CharArrayToLong(&gamebuffer[address+4]));
			int expectedSize =  decode.decFLA2(&gamebuffer[address+8], fileSizeCompressed, expectedSizeUncompressed, outputDecompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("RNCSngOffset") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.unpackM1(&gamebuffer[address], outputDecompressed, 0x0000, fileSizeCompressed);
			
			unsigned long offset = CharArrayToLong(&outputDecompressed[0]);

			SngToMidi(&outputDecompressed[offset], expectedSize - offset, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, &outputDecompressed[offset], expectedSize - offset, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("Yay0Sng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			YAY0 decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.decodeAll(&gamebuffer[address], outputDecompressed, fileSizeCompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("BlitzSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			CBlitzDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], outputDecompressed, fileSizeCompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("MarioTennisSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			CMarioTennisDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("ZLibSng") == 0)
	{
		if (compressed)
		{
			compress->SetGame(NOHEADER);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				SngToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
				if (generateDebugTextFile)
					SngToDebugTextFile(gameName, address, outputBuffer, decompressedSize, fileName + " TrackParseDebug.txt", extra);

				delete [] outputBuffer;
			}
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("Factor5Zlb") == 0)
	{
		if ((gamebuffer[address] == 0x78) && (gamebuffer[address+1] == 0xDA))
		{
			compress->SetGame(STUNTRACER64);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				Factor5ToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, true);
				if (generateDebugTextFile)
					Factor5ToDebugTextFile(gameName, address, outputBuffer, decompressedSize, fileName + " TrackParseDebug.txt", true);
				
				delete [] outputBuffer;
			}
		}
		else if ((gamebuffer[address] == 0x68) && (gamebuffer[address+1] == 0xDE))
		{
			compress->SetGame(RESIDENTEVIL2);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				Factor5ToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, true);
				if (generateDebugTextFile)
					Factor5ToDebugTextFile(gameName, address, outputBuffer, decompressedSize, fileName + " TrackParseDebug.txt", true);
				
				delete [] outputBuffer;
			}
		}
		else
		{
			Factor5ToMidi(&gamebuffer[address], size, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, true);
			if (generateDebugTextFile)
				Factor5ToDebugTextFile(gameName, address, gamebuffer, size, fileName + " TrackParseDebug.txt", true);
		}
	}
	else if (gameType.CompareNoCase("Factor5ZlbGCStyle") == 0)
	{
		if ((gamebuffer[address] == 0x78) && (gamebuffer[address+1] == 0xDA))
		{
			compress->SetGame(STUNTRACER64);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				Factor5ToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, false);
				if (generateDebugTextFile)
					Factor5ToDebugTextFile(gameName, address, outputBuffer, decompressedSize, fileName + " TrackParseDebug.txt", false);
				
				delete [] outputBuffer;
			}
		}
		else if ((gamebuffer[address] == 0x68) && (gamebuffer[address+1] == 0xDE))
		{
			compress->SetGame(RESIDENTEVIL2);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				Factor5ToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, false);
				if (generateDebugTextFile)
					Factor5ToDebugTextFile(gameName, address, outputBuffer, decompressedSize, fileName + " TrackParseDebug.txt", false);
				
				delete [] outputBuffer;
			}
		}
		else
		{
			Factor5ToMidi(&gamebuffer[address], size, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, false);
			if (generateDebugTextFile)
				Factor5ToDebugTextFile(gameName, address, gamebuffer, size, fileName + " TrackParseDebug.txt", false);
		}
	}
	else if (gameType.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0)
	{
		compress->SetGame(NOHEADER);
		int decompressedSize = 0;	
		int compressedSize = -1;
		byte* outputBuffer = Decompress(&gamebuffer[address], size, decompressedSize, compressedSize);
		if (outputBuffer != NULL)
		{
			Factor5ToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, false);
			if (generateDebugTextFile)
				Factor5ToDebugTextFile(gameName, address, outputBuffer, decompressedSize, fileName + " TrackParseDebug.txt", false);
			
			delete [] outputBuffer;
		}
	}
	else if (gameType.CompareNoCase("Factor5LZGCStyle") == 0)
	{
		CMidwayDecoder decode;
		unsigned char* outputBuffer = new unsigned char[0x50000];
		int fileSizeCompressed = size;
		int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputBuffer, "MIDWAY");

		if (outputBuffer != NULL)
		{
			Factor5ToMidi(outputBuffer, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, false);
			if (generateDebugTextFile)
				Factor5ToDebugTextFile(gameName, address, outputBuffer, expectedSize, fileName + " TrackParseDebug.txt", false);

			delete [] outputBuffer;
		}		
	}
	else if (gameType.CompareNoCase("ZLibIndexedSng") == 0)
	{
		if (compressed)
		{
			compress->SetGame(NOHEADER);
			int decompressedSize = 0;	
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], gamebufferSize - address, decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				unsigned long offset = CharArrayToLong(&outputBuffer[4 + (size * 4)]);
				unsigned long offsetEnd = CharArrayToLong(&outputBuffer[4 + (size * 4) + 4]);
				if (offsetEnd == 0)
					offsetEnd = decompressedSize;

				SngToMidi(&outputBuffer[offset], (offsetEnd - offset), fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
				if (generateDebugTextFile)
					SngToDebugTextFile(gameName, address, outputBuffer, (offsetEnd - offset), fileName + " TrackParseDebug.txt", extra);

				delete [] outputBuffer;
			}
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("MultiPartTigSng") == 0)
	{
		if (compressed)
		{
			TigDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x90000];
			unsigned char* sngBinaryPre = new unsigned char[0x100000];
			unsigned long  sngSize = 0;

			int totalCompressedSize = 0;
			unsigned long tempAddress = address;
			while (totalCompressedSize < (size-1))
			{
				unsigned long compressedsize = Flip32Bit((((((gamebuffer[tempAddress] << 8) | gamebuffer[tempAddress+1]) << 8) | gamebuffer[tempAddress+2]) << 8) | gamebuffer[tempAddress+3]);
				unsigned char type = gamebuffer[tempAddress+4];

				if (type == 1)  // no compression
				{
					memcpy(&sngBinaryPre[sngSize], &gamebuffer[tempAddress+8], compressedsize-8);
					sngSize += compressedsize-8;
				}
				else if (type == 0)
				{
					int fileSize = decode.dec(&gamebuffer[tempAddress+8], compressedsize, outputDecompressed);
					if (fileSize > 0x1000)
						fileSize = 0x1008;
					memcpy(&sngBinaryPre[sngSize], outputDecompressed, fileSize-8);
					sngSize += fileSize-8;
				}
				
				tempAddress += compressedsize;
				totalCompressedSize += compressedsize;

				if ((gamebuffer[tempAddress] == 0x00) || ((Flip32Bit(CharArrayToLong(&gamebuffer[tempAddress]))) > 0x1010))
				{
					tempAddress++;
					totalCompressedSize++;
				}
			}

			SngToMidi(sngBinaryPre, sngSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, sngSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
			delete [] sngBinaryPre;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("LZSS_0BSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address+4], fileSizeCompressed, outputDecompressed, "LZSS_0");
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("LZSS_0Sng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed, "LZSS_0", 0);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("AVL_0Sng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed, "LZSS");
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("VigilanteSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CVigilanteDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("ViewpointSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CViewpointDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[extra];
			int expectedSize = decode.dec(&gamebuffer[address], outputDecompressed, fileSizeCompressed, extra);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("RugratsSng") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CRugratsDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed);
			
			SngToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
			if (generateDebugTextFile)
				SngToDebugTextFile(gameName, address, outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extra);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("MidiLZSSWilliams") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = size;
			CMidwayDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, outputDecompressed, "WILLIAMS");
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
	}
	else if (gameType.CompareNoCase("Midi") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < size; x++)
			{
				fwrite(&gamebuffer[address+x], 1, 1, outFile);
			}
			fclose(outFile);
		}
	}
	else if (gameType.CompareNoCase("Yaz0EADZelda") == 0)
	{
		YAZ0 yaz0;
		int fileSizeCompressed = -1;
		int sizeUncompressed = yaz0.yaz0_get_size(&gamebuffer[address]);
		unsigned char* outputDecompressed = new unsigned char[sizeUncompressed];
		unsigned long sizeDecompressed = yaz0.yaz0_decode(&gamebuffer[address], outputDecompressed, fileSizeCompressed);
		if (sizeDecompressed == 0)
		{
			delete [] outputDecompressed;
			MessageBox(NULL, "Error decompressing", "Error", NULL);
			return;
		}

		unsigned long audioSeqIndex = extra2;
		unsigned long audioSeqOffset = size;

		unsigned long numberMidi = CharArrayToShort(&outputDecompressed[audioSeqIndex]);

		unsigned long start = CharArrayToLong(&outputDecompressed[audioSeqIndex + 0x10 + (extra * 0x10)]);
		unsigned long length = CharArrayToLong(&outputDecompressed[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);

		int pitchBendRange = outputDecompressed[audioSeqIndex + 0x10 + (extra * 0x10) + 9] * 6;

		EADMusicToMidi(gameName, EADMUSICSTYLEZELDA, gamebuffer, audioSeqOffset + start, length, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra, generateDebugTextFile, fileName + " TrackParseDebug.txt", pitchBendRange, isPreview);

		delete [] outputDecompressed;
	}
	else if (gameType.CompareNoCase("ZLibEADZelda") == 0)
	{
		int fileSizeCompressed = -1;

		GECompression gedecompress;
		gedecompress.SetGame(MORTALKOMBAT);
		gedecompress.SetCompressedBuffer(&gamebuffer[address], size);
		int fileSizeUncompressed;
		unsigned char* outputDecompressed = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

		if (outputDecompressed == NULL)
		{
			delete [] outputDecompressed;
			MessageBox(NULL, "Error decompressing", "Error", NULL);
			return;
		}

		unsigned long audioSeqIndex = extra2;
		unsigned long audioSeqOffset = size;

		unsigned long numberMidi = CharArrayToShort(&outputDecompressed[audioSeqIndex]);

		unsigned long start = CharArrayToLong(&outputDecompressed[audioSeqIndex + 0x10 + (extra * 0x10)]);
		unsigned long length = CharArrayToLong(&outputDecompressed[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);

		int pitchBendRange = outputDecompressed[audioSeqIndex + 0x10 + (extra * 0x10) + 9] * 6;

		EADMusicToMidi(gameName, EADMUSICSTYLEZELDA, gamebuffer, audioSeqOffset + start, length, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra, generateDebugTextFile, fileName + " TrackParseDebug.txt", pitchBendRange, isPreview);

		delete [] outputDecompressed;
	}
	else if (
		(gameType.CompareNoCase("EADZelda") == 0)
		||
		(gameType.CompareNoCase("Seq64Zelda") == 0)
		)
	{
		if (compressed)
		{
			
		}
		else
		{
			unsigned long audioSeqIndex = address;
			unsigned long audioSeqOffset = size;

			unsigned long numberMidi = CharArrayToShort(&gamebuffer[audioSeqIndex]);

			unsigned long start = CharArrayToLong(&gamebuffer[audioSeqIndex + 0x10 + (extra * 0x10)]);
			unsigned long length = CharArrayToLong(&gamebuffer[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);

			int pitchBendRange = gamebuffer[audioSeqIndex + 0x10 + (extra * 0x10) + 9] * 6;

			EADMusicToMidi(gameName, EADMUSICSTYLEZELDA, gamebuffer, audioSeqOffset + start, length, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra, generateDebugTextFile, fileName + " TrackParseDebug.txt", pitchBendRange, isPreview);
		}
	}
	else if (
		(gameType.CompareNoCase("EADMario") == 0)
		||
		(gameType.CompareNoCase("Seq64Mario") == 0)
		)
	{
		if (compressed)
		{
			
		}
		else
		{
			unsigned long audioSeqIndex = address;
			unsigned long audioSeqOffset = size;

			unsigned long numberMidi = CharArrayToShort(&gamebuffer[audioSeqIndex]);

			unsigned long start = CharArrayToLong(&gamebuffer[audioSeqIndex + 4 + (extra * 8)]);
			unsigned long length = CharArrayToLong(&gamebuffer[audioSeqIndex + 4 + (extra * 8) + 4]);

			EADMusicToMidi(gameName, EADMUSICSTYLEMARIO, gamebuffer, audioSeqOffset + start, length, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra, generateDebugTextFile, fileName + " TrackParseDebug.txt", 6, isPreview);
		}
	}
	else if (gameType.CompareNoCase("EADStarFox") == 0)
	{
		if (compressed)
		{
			
		}
		else
		{
			unsigned long audioSeqIndex = address;
			unsigned long audioSeqOffset = size;

			unsigned long numberMidi = CharArrayToShort(&gamebuffer[audioSeqIndex]);

			unsigned long start = CharArrayToLong(&gamebuffer[audioSeqIndex + 0x10 + (extra * 0x10)]);
			unsigned long length = CharArrayToLong(&gamebuffer[audioSeqIndex + 0x10 + (extra * 0x10) + 4]);

			int pitchBendRange = gamebuffer[audioSeqIndex + 0x10 + (extra * 0x10) + 9] * 6;

			EADMusicToMidi(gameName, EADMUSICSTYLESTARFOX, gamebuffer, audioSeqOffset + start, length, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra, generateDebugTextFile, fileName + " TrackParseDebug.txt", pitchBendRange, isPreview);
		}
	}
	else if (gameType.CompareNoCase("Seq64Mario") == 0)
	{
		CString tempROMStr = mainFolder + "TempASEQ64ROM.rom";
		FILE* outTempROM = fopen(tempROMStr, "wb");
		if (outTempROM == NULL)
		{
			MessageBox(NULL, "Cannot Write temp ROM File", "Error", NULL);
			return;
		}

		fwrite(gamebuffer, 1, gamebufferSize, outTempROM);
		fclose(outTempROM);

		CString tempStr;
		::DeleteFile(fileName);
		tempStr.Format("seq64.exe --rom=\"%s\" --romdesc=\"%s\" --export_midi=%d --output=\"%s\"", tempROMStr, mainFolder + "romdesc\\" + gameName + ".xml", extra, fileName);
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));

		::DeleteFile(tempROMStr);
	}
	else if (gameType.CompareNoCase("Seq64Zelda") == 0)
	{
		CString tempROMStr = mainFolder + "TempASEQ64ROM.rom";
		FILE* outTempROM = fopen(tempROMStr, "wb");
		if (outTempROM == NULL)
		{
			MessageBox(NULL, "Cannot Write temp ROM File", "Error", NULL);
			return;
		}

		fwrite(gamebuffer, 1, gamebufferSize, outTempROM);
		fclose(outTempROM);

		CString tempStr;
		::DeleteFile(fileName);
		tempStr.Format("seq64.exe --rom=\"%s\" --romdesc=\"%s\" --export_midi=%d --output=\"%s\"", tempROMStr, mainFolder + "romdesc\\" + gameName + ".xml", extra, fileName);
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));

		::DeleteFile(tempROMStr);
	}
	else if (gameType.CompareNoCase("Yaz0Seq64") == 0)
	{
		CString tempROMStr = mainFolder + "TempASEQ64ROM.rom";

		unsigned char* tempGameBuffer = new unsigned char[gamebufferSize];
		memcpy(tempGameBuffer, gamebuffer, gamebufferSize);

		YAZ0 yaz0;
		int fileSizeCompressed = -1;
		int sizeUncompressed = yaz0.yaz0_get_size(&tempGameBuffer[extra]);
		unsigned char* outputDecompressed = new unsigned char[sizeUncompressed];
		unsigned long sizeDecompressed = yaz0.yaz0_decode(&tempGameBuffer[extra], outputDecompressed, fileSizeCompressed);
		if (sizeDecompressed == 0)
		{
			delete [] outputDecompressed;
			delete [] tempGameBuffer;
			MessageBox(NULL, "Error decompressing", "Error", NULL);
			return;
		}

		FILE* outTempROM = fopen(tempROMStr, "wb");
		if (outTempROM == NULL)
		{
			delete [] outputDecompressed;
			delete [] tempGameBuffer;
			MessageBox(NULL, "Cannot Write temp ROM File", "Error", NULL);
			return;
		}

		memcpy(&tempGameBuffer[extra], outputDecompressed, sizeDecompressed);
		delete [] outputDecompressed;

		fwrite(tempGameBuffer, 1, gamebufferSize, outTempROM);
		fclose(outTempROM);

		CString tempStr;
		::DeleteFile(fileName);
		tempStr.Format("seq64.exe --rom=\"%s\" --romdesc=\"%s\" --export_midi=%d --output=\"%s\"", tempROMStr, mainFolder + "romdesc\\" + gameName + ".xml", extra, fileName);
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));

		::DeleteFile(tempROMStr);

		delete [] tempGameBuffer;
	}
	else if (gameType.CompareNoCase("ZipSng") == 0)
	{
		CString tempROMStr = mainFolder + "TempASEQ64ROM.rom";
		FILE* outTempROM = fopen(tempROMStr, "wb");
		if (outTempROM == NULL)
		{
			MessageBox(NULL, "Cannot Write temp ROM File", "Error", NULL);
			return;
		}

		fwrite(gamebuffer, 1, gamebufferSize, outTempROM);
		fclose(outTempROM);

		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;  

		::SetCurrentDirectory(mainFolder);
		HANDLE h = CreateFile(_T(mainFolder + "templist.txt"),
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			&sa,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL );

		CString tempStr;
		::DeleteFile(fileName);
		tempStr.Format("7z.exe l \"%s\" -o\"%s\" *.bin -r -y", tempROMStr, mainFolder);
		if (!hiddenExec(_T(tempStr.GetBuffer()), mainFolder, h))
		{
			CloseHandle(h);

			return;
		}

		CloseHandle(h);


		FILE* inFile = fopen(mainFolder + "templist.txt", "r");

		if (inFile == NULL)
		{
			return;
		}

		std::vector<CString> lines;
		while (!feof(inFile))
		{
			char currentLine[1000];
			fgets(currentLine, 1000, inFile);
			lines.push_back(currentLine);
		}
		fclose(inFile);

		int counter = 0;
		for (int x = 0; x < lines.size(); x++)
		{
			if (lines[x].Find("Sound\\") != -1)
			{
				if (counter == address)
				{
					int fileNameSpot = lines[x].Find("Sound\\") + 6;
					CString fileNamePull = lines[x].Mid(fileNameSpot);
					fileNamePull.Trim();

					tempStr.Format("7z.exe e \"%s\" -o\"%s\" %s -r -y", tempROMStr, mainFolder, fileNamePull);
					if (!hiddenExec(_T(tempStr.GetBuffer()), (mainFolder)))
						return;

					FILE* inFileSng = fopen(mainFolder + fileNamePull, "rb");
					if (inFileSng == NULL)
						return;

			
					fseek(inFile, 0, SEEK_END);
					int inputSize = ftell(inFile);
					rewind(inFile);

					unsigned char* inputMID = new unsigned char[inputSize];

					fread(inputMID, 1, inputSize, inFile);
					fclose(inFile);

					fclose(inFileSng);

					SngToMidi(inputMID, inputSize, fileName, numberInstruments, calculateInstrumentCountOnly, separateByInstrument, extra);
					if (generateDebugTextFile)
						SngToDebugTextFile(gameName, address, &gamebuffer[address], inputSize, fileName + " TrackParseDebug.txt", extra);

					delete [] inputMID;
					::DeleteFile(mainFolder + fileNamePull);
				}
				counter++;
			}
		}

		::DeleteFile("templist.txt");

		::DeleteFile(tempROMStr);
	}
	else if (gameType.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		if (compressed)
		{
			compress->SetGame(STUNTRACER64);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}

			unsigned long compressedMainSize = CharArrayToLong(&gamebuffer[address]);
			unsigned long uncompressedMainSize = CharArrayToLong(&gamebuffer[address+4]);
			unsigned long step = CharArrayToLong(&gamebuffer[address+0x8]);

			int readSize = 0;
			while ((readSize < compressedMainSize) && (CharArrayToLong(&gamebuffer[address+0xC+readSize]) != 0))
			{
				int decompressedSize = 0;
				int compressedSize = -1;
				byte* outputBuffer = Decompress(&gamebuffer[address+0xC+readSize], size, decompressedSize, compressedSize);

				for (int x = 0; x < decompressedSize; x++)
				{
					fwrite(&outputBuffer[x], 1, 1, outFile);
				}

				readSize += step;

				if (((address+0xC+readSize) % 2) != 0)
				{
					readSize += (2-((address+0xC+readSize) % 2));
				}

				step = CharArrayToLong(&gamebuffer[address+0xC+readSize]);

				readSize += 4;

				delete [] outputBuffer;
			}
			fclose(outFile);

		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("DCM") == 0)
	{
		if (compressed)
		{
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}

			CH20DecoderMidiTool h20dec;
			int compressedSize = -1;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			unsigned char* cleanDecompressed = new unsigned char[0x50000];

			int decSize = h20dec.decPolaris(&gamebuffer[address], compressedSize, outputDecompressed);
			decSize = h20dec.decPolaris(&gamebuffer[address], compressedSize, cleanDecompressed);

			for (int x = 14; x < (14 + (outputDecompressed[5] * 0x10)); x+=0x10)
			{
				unsigned char tempValue1 = outputDecompressed[x];
				unsigned char tempValue2 = outputDecompressed[x+1];

				outputDecompressed[x] = outputDecompressed[x+3];
				outputDecompressed[x+1] = outputDecompressed[x+2];
				outputDecompressed[x+2] = tempValue2;
				outputDecompressed[x+3] = tempValue1;

				tempValue1 = outputDecompressed[x+4];
				tempValue2 = outputDecompressed[x+5];

				outputDecompressed[x+4] = outputDecompressed[x+7];
				outputDecompressed[x+5] = outputDecompressed[x+6];
				outputDecompressed[x+6] = tempValue2;
				outputDecompressed[x+7] = tempValue1;

				tempValue1 = outputDecompressed[x+8];
				tempValue2 = outputDecompressed[x+9];

				outputDecompressed[x+8] = outputDecompressed[x+11];
				outputDecompressed[x+9] = outputDecompressed[x+10];
				outputDecompressed[x+10] = tempValue2;
				outputDecompressed[x+11] = tempValue1;

				unsigned char tempValue = outputDecompressed[x+12];
				outputDecompressed[x+12] = outputDecompressed[x+13];
				outputDecompressed[x+13] = tempValue;

				tempValue = outputDecompressed[x+14];
				outputDecompressed[x+14] = outputDecompressed[x+15];
				outputDecompressed[x+15] = tempValue;
			}

			int position = (14 + (outputDecompressed[5] * 0x10));

			if (cleanDecompressed[position] == 0x00)
			{
				unsigned long length = CharArrayToLong(&cleanDecompressed[position]);
				if (length > decSize)
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					CTetrisphereDecoder tetDec;
					unsigned char* outputLz = new unsigned char[0x100000];
					int returnSize = tetDec.decompressLZ(&cleanDecompressed[position+4], length, outputLz, true);
					returnSize = returnSize;

					fwrite(outputLz, 1, length, outFile);

					delete [] outputLz;
				}
				else
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					length = (decSize - (position + 4));
					fwrite(&cleanDecompressed[position+4], 1, length, outFile);
				}
			}
			else
			{
				fwrite(cleanDecompressed, 1, position, outFile);

				int length = (decSize - (position));
				fwrite(&cleanDecompressed[position], 1, length, outFile);
			}

			for (int x = 0; x < outputDecompressed[5]; x++)
			{
				unsigned short instrumentNumber = CharArrayToShort(&outputDecompressed[14+(16*(x))+14]);

				unsigned long sampleSize = CharArrayToLong(&outputDecompressed[14+(16*(x))]);
				unsigned short flags = CharArrayToShort(&outputDecompressed[14+(16*(x))+12]);

				if (flags & 1)
				{
					sampleSize = sampleSize * 2;
				}
				unsigned long location = address + size + CharArrayToLong(&gamebuffer[address + size + (instrumentNumber * 4) + 2]);

				unsigned char* outputDecompressedInstrument = new unsigned char[0x50000];

				int decSizeInstrument = h20dec.decPolaris(&gamebuffer[location], compressedSize, outputDecompressedInstrument);
				fwrite(outputDecompressedInstrument, 1, sampleSize, outFile);

				delete [] outputDecompressedInstrument;
			}

			delete [] outputDecompressed;
			delete [] cleanDecompressed;

			fclose(outFile);
		}
		else
		{

		}
	}
	else if (gameType.CompareNoCase("Aidyn") == 0)
	{
		FILE* outFile = fopen(fileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Cannot Write File", "Error", NULL);
			return;
		}

		int decompressedSize = -1;
		int fileTable = size;
		int fileID = address;
		int fileTableData = extra;
		unsigned char* tempInput = CAidynDecoder::DecompressAidynFile(gamebuffer, fileTable, fileTableData, fileID, decompressedSize);

		if (tempInput != NULL)
		{
			std::map<int, fileData> smp;
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			int dcmSize = -1;
			CAidynToDCMConvertor::convert(tempInput, gamebuffer, fileTable, fileTableData, smp, "", outputDecompressed, dcmSize, 0);
			fwrite(outputDecompressed, 1, dcmSize, outFile);

			if (outputDecompressed)
			{
				delete [] outputDecompressed;
				outputDecompressed = NULL;
			}
			delete [] tempInput;
		}

		fclose(outFile);
	}
	else if (gameType.CompareNoCase("SmpOffsetAidDebug") == 0)
	{
		FILE* outFile = fopen(fileName, "wb");
		if (outFile == NULL)
		{
			MessageBox(NULL, "Cannot Write File", "Error", NULL);
			return;
		}

		int decompressedSize = -1;
		int fileTable = size;
		int fileID = address;
		int fileTableData = extra;
		unsigned char* tempInput = CAidynDecoder::DecompressAidynFile(gamebuffer, fileTable, fileTableData, fileID, decompressedSize);

		if (tempInput != NULL)
		{
			std::map<int, fileData> smp;
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			int dcmSize = -1;
			CAidynToDCMConvertor::convert(tempInput, gamebuffer, fileTable, fileTableData, smp, "", outputDecompressed, dcmSize, 1);
			fwrite(outputDecompressed, 1, dcmSize, outFile);

			if (outputDecompressed)
			{
				delete [] outputDecompressed;
				outputDecompressed = NULL;
			}
			delete [] tempInput;
		}

		fclose(outFile);
	}
	else if (gameType.CompareNoCase("LZSamplesDCM") == 0)
	{
		if (compressed)
		{
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}

			unsigned long decSize = ((((((gamebuffer[address+0x9] << 8) | gamebuffer[address+0x8]) << 8) | gamebuffer[address+0x7]) << 8) | gamebuffer[address+0x6]);
			
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			unsigned char* cleanDecompressed = new unsigned char[0x50000];
			memcpy(outputDecompressed, &gamebuffer[address], decSize + 4 + (14 + (outputDecompressed[5] * 0x10)));
			memcpy(cleanDecompressed, &gamebuffer[address], decSize + 4 + (14 + (outputDecompressed[5] * 0x10)));

			for (int x = 14; x < (14 + (outputDecompressed[5] * 0x10)); x+=0x10)
			{
				unsigned char tempValue1 = outputDecompressed[x];
				unsigned char tempValue2 = outputDecompressed[x+1];

				outputDecompressed[x] = outputDecompressed[x+3];
				outputDecompressed[x+1] = outputDecompressed[x+2];
				outputDecompressed[x+2] = tempValue2;
				outputDecompressed[x+3] = tempValue1;

				tempValue1 = outputDecompressed[x+4];
				tempValue2 = outputDecompressed[x+5];

				outputDecompressed[x+4] = outputDecompressed[x+7];
				outputDecompressed[x+5] = outputDecompressed[x+6];
				outputDecompressed[x+6] = tempValue2;
				outputDecompressed[x+7] = tempValue1;

				tempValue1 = outputDecompressed[x+8];
				tempValue2 = outputDecompressed[x+9];

				outputDecompressed[x+8] = outputDecompressed[x+11];
				outputDecompressed[x+9] = outputDecompressed[x+10];
				outputDecompressed[x+10] = tempValue2;
				outputDecompressed[x+11] = tempValue1;

				unsigned char tempValue = outputDecompressed[x+12];
				outputDecompressed[x+12] = outputDecompressed[x+13];
				outputDecompressed[x+13] = tempValue;

				tempValue = outputDecompressed[x+14];
				outputDecompressed[x+14] = outputDecompressed[x+15];
				outputDecompressed[x+15] = tempValue;
			}

			int position = (14 + (outputDecompressed[5] * 0x10));

			if (cleanDecompressed[position] == 0x00)
			{
				unsigned long length = CharArrayToLong(&cleanDecompressed[position]);
				if (length > decSize)
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					CTetrisphereDecoder tetDec;
					unsigned char* outputLz = new unsigned char[0x100000];
					int returnSize = tetDec.decompressLZ(&cleanDecompressed[position+4], (decSize - 4), outputLz, true);
					returnSize = returnSize;

					fwrite(outputLz, 1, length, outFile);

					delete [] outputLz;

					position += returnSize;
				}
				else
				{
					cleanDecompressed[6] = (length & 0xFF);
					cleanDecompressed[7] = ((length >> 8) & 0xFF);
					cleanDecompressed[8] = ((length >> 16) & 0xFF);
					cleanDecompressed[9] = ((length >> 24) & 0xFF);

					fwrite(cleanDecompressed, 1, position, outFile);

					length = (decSize - (position + 4));
					fwrite(&cleanDecompressed[position+4], 1, length, outFile);

					position += length;
				}
			}
			else
			{
				fwrite(cleanDecompressed, 1, position, outFile);

				int length = decSize;
				fwrite(&cleanDecompressed[position], 1, length, outFile);

				position += length;
			}

			for (int x = 0; x < outputDecompressed[5]; x++)
			{
				unsigned short instrumentNumber = CharArrayToShort(&outputDecompressed[14+(16*(x))+14]);

				unsigned long sampleSize = CharArrayToLong(&outputDecompressed[14+(16*(x))]);
				unsigned short flags = CharArrayToShort(&outputDecompressed[14+(16*(x))+12]);

				if (flags & 1)
				{
					sampleSize = sampleSize * 2;
				}
				unsigned long location = address + size + CharArrayToLong(&gamebuffer[address + size + (instrumentNumber * 4) + 2]);

				unsigned char* outputDecompressedInstrument = new unsigned char[0x50000];

				CTetrisphereDecoder tetDec;
				tetDec.sphereDecompress(&gamebuffer[location], outputDecompressedInstrument);
				fwrite(outputDecompressedInstrument, 1, sampleSize, outFile);

				position += sampleSize;

				delete [] outputDecompressedInstrument;
			}

			delete [] outputDecompressed;
			delete [] cleanDecompressed;

			fclose(outFile);
		}
		else
		{

		}
	}
	else if (gameType.CompareNoCase("TitusMidi") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			SupermanDecoder decode;
			int expectedSize = decode.header(&gamebuffer[address], fileSizeCompressed);
			unsigned char* outputDecompressed = new unsigned char[expectedSize];
			
			decode.dec(&gamebuffer[address+0x11], fileSizeCompressed, expectedSize, outputDecompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			delete [] outputDecompressed;
		}
		else
		{
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < size; x++)
			{
				fwrite(&gamebuffer[address+x], 1, 1, outFile);
			}
			fclose(outFile);
		}
	}
	else if (gameType.CompareNoCase("RNCMidi") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.unpackM1(&gamebuffer[address], outputDecompressed, 0x0000, fileSizeCompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			GEMidiToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity);
			if (generateDebugTextFile)
				GEMidiToDebugTextFile(outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extendTracksToHighest);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("RobotechN64Midi") == 0)
	{
		if (compressed)
		{
			int fileSizeCompressed = -1;
			CMKMythologiesDecode decode;
			unsigned char* outputDecompressed = new unsigned char[0x50000];
			int expectedSize = decode.dec(&gamebuffer[address], fileSizeCompressed, extra, outputDecompressed);
			
			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = 0; x < expectedSize; x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			GEMidiToMidi(outputDecompressed, expectedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity);
			if (generateDebugTextFile)
				GEMidiToDebugTextFile(outputDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extendTracksToHighest);

			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("RNCSeq") == 0)
	{
		if (compressed)
		{
			int realSpot = size;

			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			int expectedSize = decode.unpackM1(&gamebuffer[address], outputDecompressed, 0x0000, fileSizeCompressed);
			
			unsigned long realStart = CharArrayToLong(&outputDecompressed[0x8]) + CharArrayToLong(&outputDecompressed[realSpot]);
			unsigned long realSize = CharArrayToLong(&outputDecompressed[realSpot + 4]) - CharArrayToLong(&outputDecompressed[realSpot]);

			FILE* outFile = fopen(fileName, "wb");
			if (outFile == NULL)
			{
				MessageBox(NULL, "Cannot Write File", "Error", NULL);
				return;
			}
			for (int x = realStart; x < (realStart + realSize); x++)
			{
				fwrite(&outputDecompressed[x], 1, 1, outFile);
			}
			fclose(outFile);

			unsigned char* tempDecompressed = new unsigned char[realSize];
			memcpy(tempDecompressed, &outputDecompressed[realStart], realSize);

			GEMidiToMidi(tempDecompressed, expectedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity);
			if (generateDebugTextFile)
				GEMidiToDebugTextFile(tempDecompressed, expectedSize, fileName + " TrackParseDebug.txt", extendTracksToHighest);

			delete [] tempDecompressed;
			delete [] outputDecompressed;
		}
		else
		{
			
		}
	}
	else if (gameType.CompareNoCase("DuckDodgers") == 0)
	{
		DuckDodgersMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity, extra);
		if (generateDebugTextFile)
			DuckDodgersMidiToDebugTextFile(&gamebuffer[address], size, fileName + " TrackParseDebug.txt", extendTracksToHighest);
	}
	else
	{
		if (compressed)
		{
			if (gameType.CompareNoCase("GoldenEye") == 0)
			{
				compress->SetGame(GOLDENEYE);
			}
			else if (gameType.CompareNoCase("PerfectDark") == 0)
			{
				compress->SetGame(PD);
			}
			else if (gameType.CompareNoCase("BanjoKazooie") == 0)
			{
				compress->SetGame(BANJOKAZOOIE);
			}
			else if (gameType.CompareNoCase("BanjoTooie") == 0)
			{
				compress->SetGame(BANJOTOOIE);
			}
			else if (gameType.CompareNoCase("DonkeyKong") == 0)
			{
				compress->SetGame(DONKEYKONG64);
			}

			int decompressedSize = 0;
			int compressedSize = -1;
			byte* outputBuffer = Decompress(&gamebuffer[address], (size + 20000), decompressedSize, compressedSize);
			if (outputBuffer != NULL)
			{
				if ((outputBuffer[0] == 0x0) && (outputBuffer[1] == 0x0)
					&& (outputBuffer[2] == 0x0) && (outputBuffer[3] == 0x44))
				{
					GEMidiToMidi(outputBuffer, decompressedSize, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity);
					if (generateDebugTextFile)
						GEMidiToDebugTextFile(outputBuffer, decompressedSize, fileName + " TrackParseDebug.txt", extendTracksToHighest);
				}
				delete [] outputBuffer;
			}
		}
		else
		{
			GEMidiToMidi(&gamebuffer[address], size, fileName, numberInstruments, hasLoopPoint, loopStart, loopEnd, extendTracksToHighest, usePitchBendSensitity, pitchBendSensitity);
			if (generateDebugTextFile)
				GEMidiToDebugTextFile(&gamebuffer[address], size, fileName + " TrackParseDebug.txt", extendTracksToHighest);
		}
	}
}

byte* CMidiParse::Decompress(unsigned char* Buffer, unsigned long size, int& fileSize, int& compressedSize)
{
	compress->SetCompressedBuffer(Buffer, size);
	
	fileSize = 0; // is by reference, overwritten
	byte* outputDecompressed = compress->OutputDecompressedBuffer(fileSize, compressedSize);

	if (outputDecompressed == NULL)
	{
		MessageBox(NULL, "Error Decompressing", "Error", NULL);
		return NULL;
	}

	return outputDecompressed;
}

bool CMidiParse::DecompressToFile(unsigned char* Buffer, unsigned long size, CString outputFile)
{
	int fileSize = 0; // is by reference, overwritten
	int compressedSize = -1;
	byte* outputDecompressed = Decompress(Buffer, size, fileSize, compressedSize);

	if (outputDecompressed == NULL)
	{
		MessageBox(NULL, "Error Decompressing", "Error", NULL);
		return false;
	}

	FILE* outFile = fopen(outputFile, "wb");
	if (outFile == NULL)
	{
		MessageBox(NULL, "Cannot Write File", "Error", NULL);
		return false;
	}
	for (int x = 0; x < fileSize; x++)
	{
		fwrite(&outputDecompressed[x], 1, 1, outFile);
	}
	fclose(outFile);
	
	delete [] outputDecompressed;
	return false;
}


void CMidiParse::ParseKonamiTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, int& noteUniqueId, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength)
{
	std::vector<SngNoteInfo> trackOutputNotes;
	unsigned char command = 0x00;
	unsigned long spot = offset;


	unsigned long absoluteTime = 0;

	unsigned char currentPan = 0x40;
	unsigned long currentInstrument = 0x00;
	unsigned char currentVolume = 0x7F;

	unsigned char currentBank = 0x00;

	int currentEffect = 0;

	unsigned char lastDuration = 0xFF;
	unsigned char lastLength = 0xFF;

	unsigned long currentTempo = (unsigned long)(60000000.0 / (float)120.0);

	signed long currentCoarseTune = 0x00;

	unsigned char loopAmountsLeft = 0x00;
	unsigned long loopSpot = 0;
	unsigned long loopEndSpot = 0;

	unsigned char loopNestedAmountsLeft = 0x00;
	unsigned long loopNestedSpot = 0;
	unsigned long loopNestedEndSpot = 0;

	unsigned char lastNoteValue = 0x00;

	unsigned long masterLoopMarkerStartOffset;
	unsigned long masterLoopMarkerEndOffset;

	int totalLoopsToOutputLeft = 0;
	if (writeOutLoops)
		totalLoopsToOutputLeft = loopWriteCount;

	int currentEDOffset = 0;
	int currentEEEndOffset = 0;
	int eeCountOverall = 0;
	
	bool lastNote00Length = false;
	unsigned char lastNote = 0xFF;
	unsigned char lastVelocity = 0xFF;

	int previousCmd = -1;
	
	unsigned char sectionNoteHold = 0x00;

	while ((command != 0xFF) && (spot < end))
	{
		if (extendTracksToHighest)
		{
			if (absoluteTime >= highestTrackLength)
				break;
		}

		if (trackNumber >= 0)
		{
			currentTempo = (unsigned long)(60000000.0 / (float)120.0);

			if (trackNumber > 0)
			{
				for (int y = 0; y < tempoPositions.size(); y++)
				{
					if (tempoPositions[y].absoluteTime <= absoluteTime)
					{
						currentTempo = tempoPositions[y].value;
					}
					else
					{
						break;
					}
				}
			}
		}

		command = buffer[spot];
		
		spot++;

		if (command < 0xD0) // Note or Rest
		{
			unsigned char note = command;

			unsigned char drumInstrumentValue = 0x00;
			if ((command == 0x67) || (command == (0x67 + 0x68))) // Rest?
			{
				drumInstrumentValue = buffer[spot];
				spot++;
			}

			if (note >= 0x68)
			{
				note -= 0x68;
			}
			else
			{
				lastDuration = buffer[spot];
				spot++;
			}

			bool firstSetNote00Length = false;

			if (buffer[spot] < 0x80)
			{
				lastLength = buffer[spot];
				spot++;

				if (lastLength == 0)
					lastNote00Length = true;
				else
					lastNote00Length = false;

				firstSetNote00Length = true;
			}
			else
			{
				if (lastLength == 0)
					lastNote00Length = true;
				else
					lastNote00Length = false;

				if ((lastLength == 0) && (previousCmd == 0xF2))
				{
					firstSetNote00Length = true;
				}
				else
				{
					firstSetNote00Length = false;
				}
			}

			unsigned char velocity = buffer[spot] & 0x7F;
			spot++;

			unsigned long noteDuration = lastDuration;
			unsigned long noteLength = lastLength;

			if (noteLength == 0)
				noteLength = noteDuration;
			else if (noteLength > noteDuration)
				noteLength = noteDuration;
			else if (noteLength <= noteDuration)
			{
				if (noteLength == 0x7F)
					noteLength = noteDuration;
				//else if ((noteLength == 0x01) && (sectionNoteHold != 0x00) && (sectionNoteHold < noteDuration))
					//noteLength = sectionNoteHold;
				else
					noteLength = noteLength;
			}

			if (!firstSetNote00Length && lastNote00Length && (lastLength == 0x00))
			{
				// Set to same as previous, this is an extension
				noteUniqueId--;
			}
			
			if (note < 0x48)
			{
				SngNoteInfo songNoteInfo;
				songNoteInfo.originalTrack = trackNumber;
				songNoteInfo.originalNoteUniqueId = noteUniqueId++;
				songNoteInfo.startAbsoluteTime = absoluteTime;
				songNoteInfo.noteNumber = note + currentCoarseTune;

				lastNoteValue = note;
				
				songNoteInfo.instrument = currentInstrument + (currentBank * 0x100);

				songNoteInfo.velocity = velocity;

				songNoteInfo.effect = currentEffect;
				songNoteInfo.tempo = currentTempo;
				songNoteInfo.pan = currentPan & 0x7F;

				songNoteInfo.pitchBend = 0x40;
				songNoteInfo.volume = ((currentVolume >> 1) & 0x7F);


				songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

				trackOutputNotes.push_back(songNoteInfo);
			}
			else if (note < 0x67)
			{
				// Drums
				SngNoteInfo songNoteInfo;
				songNoteInfo.originalTrack = trackNumber;
				songNoteInfo.originalNoteUniqueId = noteUniqueId++;
				songNoteInfo.startAbsoluteTime = absoluteTime;
				songNoteInfo.noteNumber = 0x3C + currentCoarseTune;
				songNoteInfo.velocity = velocity;

				lastNoteValue = 0x3C;

				songNoteInfo.effect = currentEffect;
				songNoteInfo.tempo = currentTempo;

				unsigned long drumInstrumentLookedUp = (note - 0x48);
				songNoteInfo.instrument = drumInstrumentLookedUp + 0x8000;

				songNoteInfo.pan = currentPan & 0x7F;

				songNoteInfo.pitchBend = 0x40;
				songNoteInfo.volume = ((currentVolume >> 1) & 0x7F);


				songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

				trackOutputNotes.push_back(songNoteInfo);
			}
			else if (note == 0x67)
			{
				// Drum Instrument
				SngNoteInfo songNoteInfo;
				songNoteInfo.originalTrack = trackNumber;
				songNoteInfo.originalNoteUniqueId = noteUniqueId++;
				songNoteInfo.startAbsoluteTime = absoluteTime;
				songNoteInfo.noteNumber = 0x3C + currentCoarseTune;
				songNoteInfo.velocity = velocity;

				lastNoteValue = 0x3C;

				songNoteInfo.effect = currentEffect;
				songNoteInfo.tempo = currentTempo;

				unsigned long drumInstrumentLookedUp = (drumInstrumentValue - 0x48);

				songNoteInfo.instrument = drumInstrumentLookedUp + 0x8000;
				songNoteInfo.pan = currentPan & 0x7F;
				songNoteInfo.pitchBend = 0x40;
				songNoteInfo.volume = ((currentVolume >> 1) & 0x7F);


				songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

				trackOutputNotes.push_back(songNoteInfo);
			}

			lastVelocity = velocity;
			lastNote = note;

			absoluteTime += noteDuration;
		}
		else
		{
			if (command == 0xD0)
			{
				//fprintf(outFile, " ((Tempo))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				unsigned char tempo = buffer[spot];

				currentTempo = (unsigned long)(60000000.0 / (float)tempo);

				if (trackNumber == 0)
				{
					tempoPositions.push_back(TimeAndValue(absoluteTime, currentTempo));
				}

				spot++;
			}
			else if (command == 0xD1)
			{
				//fprintf(outFile, " ((Tempo Change Fade))");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				unsigned char tempo = buffer[spot+1];

				currentTempo = (unsigned long)(60000000.0 / (float)tempo);

				if (trackNumber == 0)
				{
					tempoPositions.push_back(TimeAndValue(absoluteTime, currentTempo));
				}

				spot += 2;
			}
			else if (command == 0xD2)
			{
				//fprintf(outFile, " ((Instrument Change))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				currentInstrument = buffer[spot];

				if (currentInstrument >= numberInstruments)
					numberInstruments = currentInstrument + 1;
				spot++;
			}
			else if (command == 0xD3)
			{
				//fprintf(outFile, " ((Instrument/Volume Change))");
				//fprintf(outFile, " Volume %02X (%d) Instrument %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				currentVolume = buffer[spot];
				currentInstrument = buffer[spot+1];

				if (currentInstrument >= numberInstruments)
					numberInstruments = currentInstrument + 1;

				spot += 2;
			}
			else if (command == 0xD4)
			{
				//fprintf(outFile, " ((Instrument/Volume/Pan Initial))");
				//fprintf(outFile, " Volume %02X (%d) Instrument %02X (%d) Pan %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);

				currentVolume = buffer[spot];
				currentInstrument = buffer[spot+1];

				if (currentInstrument >= numberInstruments)
					numberInstruments = currentInstrument + 1;

				currentPan = buffer[spot+2];
				spot += 3;
			}
			else if (command == 0xD5)
			{
				//fprintf(outFile, " ((Volume))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				currentVolume = buffer[spot];
				spot++;
			}
			else if (command == 0xD6)
			{
				//fprintf(outFile, " ((Fade In/Out))");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				currentVolume = buffer[spot + 1];

				spot += 2;
			}
			else if (command == 0xD7)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
				spot += 2;
			}
			else if (command == 0xD8)
			{
				//fprintf(outFile, " ((Release Time))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xD9)
			{
				//fprintf(outFile, " ((Section Hold Note))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				sectionNoteHold = buffer[spot];

				spot++;
			}
			else if (command == 0xDA)
			{
				//fprintf(outFile, " ((Reverb))");
				//fprintf(outFile, " Type/Separation %02X (%d) Amount %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				spot += 2;
			}
			else if (command == 0xDB)
			{
				//fprintf(outFile, " UNKNOWN (Invalid)");
				break;
			}
			else if (command == 0xDC)
			{
				//fprintf(outFile, " UNKNOWN (Invalid)");
				break;
			}
			else if (command == 0xDD)
			{
				//fprintf(outFile, " ((Pan))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				currentPan = buffer[spot];
				spot++;
			}
			else if (command == 0xDE)
			{
				//fprintf(outFile, " Stereo Pan");
				//fprintf(outFile, " Left %02X (%d) to Right %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
				spot += 2;
			}
			else if (command == 0xDF)
			{
				//fprintf(outFile, " ((Coarse Tune))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				currentCoarseTune = (signed char)buffer[spot];
				spot++;
			}
			else if (command == 0xE0)
			{
				//fprintf(outFile, " ((Fine Tune))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xE1)
			{
				//fprintf(outFile, " ((Tremolo))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot += 3;
			}
			else if (command == 0xE2) // Jikkyou Powerful Pro Yakyuu 2000 (J) (V1.0) [!]
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xE3) // Start new game and let go to hit
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot += 3;
			}
			// E4??
			else if (command == 0xE4) // Something weird about this, seems like a followup to a note, trying this for now
			{
				//fprintf(outFile, " ((Pitch Bend Previous Note))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);

				/*if (trackOutputNotes.size() > 0)
				{
					unsigned char endPitchBend = buffer[spot+2];

					int pitchBendChange = endPitchBend - lastNoteValue;

					if (pitchBendChange >= 2)
						trackOutputNotes.back().pitchBend = 0x7F;
					else if (pitchBendChange >= 1)
						trackOutputNotes.back().pitchBend = 0x60;
					else if (pitchBendChange <= -2)
						trackOutputNotes.back().pitchBend = 0x00;
					else if (pitchBendChange <= -1)
						trackOutputNotes.back().pitchBend = 0x20;
					else
						trackOutputNotes.back().pitchBend = 0x40;
				}*/

				spot += 3;
			}
			else if (command == 0xE5) // No. 47 music
			{
				//fprintf(outFile, " ((Pitch Ascend Next Note))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot += 3;
			}
			else if (command == 0xE6)
			{
				//fprintf(outFile, " ((Slide Notes in Section))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xE7)
			{
				//fprintf(outFile, " ((Marker))");
				loopSpot = spot - 1;
			}
			else if (command == 0xE8)
			{
				//fprintf(outFile, " ((Loop from Marker))");
				//fprintf(outFile, " %02X (%d) Times %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				unsigned char readAmount = buffer[spot];
				spot += 3;

				if (readAmount > 0x01)
				{
					if (loopEndSpot != spot)
					{
						loopEndSpot = spot;
						spot = loopSpot;

						loopAmountsLeft = readAmount - 1;
					}
					else
					{
						loopAmountsLeft--;

						if (loopAmountsLeft > 0)
						{
							spot = loopSpot;
						}
						else
						{
							// Reset
							loopEndSpot = 0;
						}
					}
				}
				else if (readAmount == 0x00)
				{
					// Similar to master loop

					if (extendTracksToHighest)
					{
						spot = loopSpot;
					}
					else if (totalLoopsToOutputLeft > 0)
					{
						spot = loopSpot;
						totalLoopsToOutputLeft--;
					}
				}
			}
			else if (command == 0xE9)
			{
				//fprintf(outFile, " ((Nested Loop Marker))");
				loopNestedSpot = spot - 1;
			}
			else if (command == 0xEA)
			{
				//fprintf(outFile, " ((Nested Loop from Marker))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				unsigned char readAmount = buffer[spot];
				spot += 3;

				if (readAmount > 0x01)
				{
					if (loopNestedEndSpot != spot)
					{
						loopNestedEndSpot = spot;
						spot = loopNestedSpot;

						loopNestedAmountsLeft = readAmount - 1;
					}
					else
					{
						loopNestedAmountsLeft--;

						if (loopNestedAmountsLeft > 0)
						{
							spot = loopNestedSpot;
						}
						else
						{
							// Reset
							loopNestedEndSpot = 0;
						}
					}
				}
				else if (readAmount == 0x00)
				{
					// Similar to master loop

					if (extendTracksToHighest)
					{
						spot = loopNestedSpot;
					}
					else if (totalLoopsToOutputLeft > 0)
					{
						spot = loopNestedSpot;
						totalLoopsToOutputLeft--;
					}
				}
			}
			else if (command == 0xEB)
			{
				//fprintf(outFile, " ((Master Loop Start))");

				masterLoopMarkerStartOffset = spot;
			}
			else if (command == 0xEC)
			{
				//fprintf(outFile, " ((Master Loop End))");

				masterLoopMarkerEndOffset = spot - 1;

				if (extendTracksToHighest)
				{
					spot = masterLoopMarkerStartOffset;
				}
				else if (totalLoopsToOutputLeft > 0)
				{
					spot = masterLoopMarkerStartOffset;
					totalLoopsToOutputLeft--;
				}
			}
			else if (command == 0xED)
			{
				//fprintf(outFile, " ((Loop Skip Start))");
				currentEDOffset = spot;
				currentEEEndOffset = 0;

				eeCountOverall = 0;
			}
			else if (command == 0xEE)
			{
				//fprintf(outFile, " ((Loop Skip Middle))");

				if (eeCountOverall == 0)
				{
					if (currentEEEndOffset != 0x00000000)
					{
						spot = currentEEEndOffset;
					}
					
					eeCountOverall = 1;
				}
				else if (eeCountOverall == 1)
				{
					currentEEEndOffset = spot;

					spot = currentEDOffset;

					eeCountOverall = 0;
				}
			}
			else if (command == 0xEF)
			{
				//fprintf(outFile, " ((Effect Delay))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				currentEffect = buffer[spot];

				spot++;
			}
			else if (command == 0xF0)
			{
				currentBank = buffer[spot];
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xF1)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xF2)
			{
				//fprintf(outFile, " ((Delay Note))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				absoluteTime += buffer[spot];

				lastNote00Length = false;

				spot++;
			}
			else if (command == 0xF3)
			{
				//fprintf(outFile, " ((Previous Note Hold))");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				lastNote00Length = false;

				unsigned long noteDuration = buffer[spot];;
				unsigned long noteLength = buffer[spot+1];
				if (noteLength == 0)
					noteLength = noteDuration;
				else if (noteLength > noteDuration)
					noteLength = noteDuration;
				else if (noteLength <= noteDuration)
				{
					if (noteLength == 0x7F)
						noteLength = noteDuration;
					else
						noteLength = noteLength;
				}

				if (trackOutputNotes.size() > 0)
				{
					trackOutputNotes.back().endAbsoluteTime += noteLength;
				}

				absoluteTime += noteDuration;

				spot += 2;
			}
			// FA/FB Goemon's Great Adventure (U) - 0053D780, not sure if legit 
			else if (command == 0xFA)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xFB)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command >= 0xF4)
			{
				//fprintf(outFile, " End");
				break;
			}
			else
			{
				CString tempStr;
				tempStr.Format("Unknown Command %02X at Spot %04X", command, spot);
				MessageBox(NULL, tempStr, "Error", NULL);
				return;
			}
		}

		previousCmd = command;
	}

	// Add to end
	for (int x = 0; x < trackOutputNotes.size(); x++)
	{
		outputNotes.push_back(trackOutputNotes[x]);
	}
}

void CMidiParse::ParseSSEQTrack(int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* inputMID, unsigned long offset, unsigned long end, int& noteUniqueId, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, int highestTrackLength)
{
	std::vector<SngNoteInfo> trackOutputNotes;
	unsigned char command = 0x00;
	int spot = offset;


	unsigned long absoluteTime = 0;

	unsigned char currentPan = 0x40;
	unsigned long currentInstrument = 0x00;
	unsigned char currentVolume = 0x7F;

	unsigned char currentBank = 0x00;

	int currentEffect = 0;

	unsigned long currentTempo = (unsigned long)(60000000.0 / (float)120.0);

	signed long currentCoarseTune = 0x00;

	unsigned char loopAmountsLeft = 0x00;
	unsigned long loopSpot = 0;
	unsigned long loopEndSpot = 0;

	unsigned long masterLoopMarkerStartOffset;
	unsigned long masterLoopMarkerEndOffset;

	int totalLoopsToOutputLeft = 0;
	if (writeOutLoops)
		totalLoopsToOutputLeft = loopWriteCount;

	unsigned char sectionNoteHold = 0x00;

	std::map<int, SngNoteInfo> activeSngNotes;

	while (spot < end)
	{
		if (extendTracksToHighest)
		{
			if (absoluteTime >= highestTrackLength)
				break;
		}

		int startSpot = spot;

		unsigned long original;
		unsigned char* altPattern = NULL;
		byte altOffset = 0;
		byte altLength = 0;
		unsigned long deltaTime = GetVLBytes(inputMID, spot, original, altPattern, altOffset, altLength, false);

		absoluteTime += deltaTime;

		command = inputMID[spot];

		if (command == 0x7)
		{
			currentInstrument = ((inputMID[spot + 2] << 8) | inputMID[spot + 1]);
			//fprintf(outFile, " Instrument %02X%02X [Instrument Change]", inputMID[spot + 2], inputMID[spot + 1]);

			if (currentInstrument > numberInstruments)
				numberInstruments = currentInstrument + 1;
		}
		else if (command == 0xC)
		{
			currentVolume = inputMID[spot + 1];
		}
		else if (command == 0xD)
		{
			currentPan = inputMID[spot + 1];
		}
		else if (command == 0x11)
		{
			//fprintf(outFile, " Note %02X Velocity %02X [Note On]", inputMID[spot + 1], inputMID[spot + 2]);
			unsigned char note = inputMID[spot + 1];
			unsigned char velocity = inputMID[spot + 2];

			SngNoteInfo songNoteInfo;
			songNoteInfo.originalTrack = trackNumber;
			songNoteInfo.originalNoteUniqueId = noteUniqueId++;
			songNoteInfo.startAbsoluteTime = absoluteTime;
			songNoteInfo.noteNumber = note + currentCoarseTune;

			songNoteInfo.instrument = currentInstrument;

			songNoteInfo.velocity = velocity;

			songNoteInfo.effect = currentEffect;
			songNoteInfo.tempo = currentTempo;
			songNoteInfo.pan = currentPan & 0x7F;

			songNoteInfo.pitchBend = 0x40;
			songNoteInfo.volume = currentVolume & 0x7F;

			songNoteInfo.endAbsoluteTime = 0;
			activeSngNotes[note] = songNoteInfo;
		}
		else if (command == 0x12)
		{
			//fprintf(outFile, " Note %02X [Note Off]", inputMID[spot + 1]);
			unsigned char note = inputMID[spot + 1];
			if (activeSngNotes.find(note) != activeSngNotes.end())
			{
				activeSngNotes[note].endAbsoluteTime = absoluteTime;

				trackOutputNotes.push_back(activeSngNotes[note]);

				activeSngNotes.erase(activeSngNotes.find(note));
			}
		}
		else if (command == 0x20)
		{
			//fprintf(outFile, " %02X %02X [Loop]", inputMID[spot + 1], inputMID[spot + 2]);
			if (extendTracksToHighest)
			{
				spot = loopSpot;
			}
		}
		else if (command == 0x22)
		{
			//fprintf(outFile, " [End]\n");
			break;
		}
		else if (command == 0x23)
		{
			//fprintf(outFile, " [Loop Start]");
			loopSpot = spot;
		}
		else
		{
			
		}

		
		spot += sseqCommandSizes[command];
	}
	

	// Add to end
	for (int x = 0; x < trackOutputNotes.size(); x++)
	{
		outputNotes.push_back(trackOutputNotes[x]);
	}
}

void CMidiParse::WriteSngList(std::vector<SngNoteInfo> sngNoteList, std::vector<TimeAndValue> tempoPositions, CString outFileName, bool separateByInstrument, unsigned short division, bool expandBendRange, int bendRange)
{
	// Convert SngList to Tracks, hopefully
	int numChannels = 0;
	std::vector<SngNoteInfo> channels[MAXCHANNELS];

	// Combine Controllers
	/*for (int x = 0; x < sngNoteList.size(); x++)
	{
		SngNoteInfo tempNoteInfo = sngNoteList[x];

		int appliedChannel = -1;
		for (int channel = 0; channel < MAXCHANNELS; channel++)
		{
			bool allowOnChannel = true;

			for (int y = 0; y < channels[channel].size(); y++)
			{
				// Check if is valid
				SngNoteInfo matchNoteInfo = channels[channel][y];

				// Any overlap
				if (IsOverlap(tempNoteInfo.startAbsoluteTime, tempNoteInfo.endAbsoluteTime, matchNoteInfo.startAbsoluteTime, matchNoteInfo.endAbsoluteTime))
				{
					if (
						(tempNoteInfo.instrument != matchNoteInfo.instrument)
						|| (tempNoteInfo.pan != matchNoteInfo.pan)
						|| (tempNoteInfo.pitchBend != matchNoteInfo.pitchBend)
						|| (tempNoteInfo.tempo != matchNoteInfo.tempo)
						|| (tempNoteInfo.volume != matchNoteInfo.volume)
						|| (tempNoteInfo.effect != matchNoteInfo.effect)
						|| (tempNoteInfo.vibrato != matchNoteInfo.vibrato)
						|| (tempNoteInfo.noteNumber == matchNoteInfo.noteNumber)
						)
					{
						allowOnChannel = false;
						break;
					}
				}

			}

			if (allowOnChannel)
			{
				appliedChannel = channel;
				break;
			}
		}

		if (appliedChannel == -1)
		{
			MessageBox(NULL, "Error, too many channels for midi", "Error", NULL);
			delete [] instrumentLookup;
			return;
		}
		else
		{
			if ((appliedChannel + 1) > numChannels)
				numChannels = (appliedChannel + 1);
			channels[appliedChannel].push_back(tempNoteInfo);
		}
	}*/

	// Fix Transpose issues
	for (int x = 0; x < sngNoteList.size(); x++)
	{
		if (sngNoteList[x].noteNumber < 0)
			sngNoteList[x].noteNumber = 0x00;
		else if (sngNoteList[x].noteNumber > 0x7F)
			sngNoteList[x].noteNumber = 0x7F;

		if (sngNoteList[x].pan > 0x7F)
			sngNoteList[x].pan = 0x7F;
		else if (sngNoteList[x].pan < 0)
			sngNoteList[x].pan = 0x0;

		if (sngNoteList[x].volume > 0x7F)
			sngNoteList[x].volume = 0x7F;
		else if (sngNoteList[x].volume < 0)
			sngNoteList[x].volume = 0x0;

		if (sngNoteList[x].effect > 0x7F)
			sngNoteList[x].effect = 0x7F;

		if (sngNoteList[x].pitchBend > 0x7F)
			sngNoteList[x].pitchBend = 0x7F;

		if (sngNoteList[x].vibrato > 0x7F)
			sngNoteList[x].vibrato = 0x7F;
	}
	if (separateByInstrument)
	{
		for (int x = 0; x < sngNoteList.size(); x++)
		{
			SngNoteInfo tempNoteInfo = sngNoteList[x];

			int appliedChannel = -1;

			for (int channel = 0; channel < MAXCHANNELS; channel++)
			{
				for (int y = 0; y < channels[channel].size(); y++)
				{
					// Check if is valid
					SngNoteInfo matchNoteInfo = channels[channel][y];

					if (tempNoteInfo.instrument == matchNoteInfo.instrument)
					{
						appliedChannel = channel;
						break;
					}
				}

				if (appliedChannel != -1)
				{
					break;
				}
			}

			if (appliedChannel == -1)
			{
				if (numChannels == (MAXCHANNELS - 1))
				{
					MessageBox(NULL, "Error, too many channels for midi", "Error", NULL);
					return;
				}

				appliedChannel = numChannels;
			}

			channels[appliedChannel].push_back(tempNoteInfo);
			if ((appliedChannel + 1) > numChannels)
				numChannels = (appliedChannel + 1);
		}
	}
	else
	{
		for (int x = 0; x < sngNoteList.size(); x++)
		{
			SngNoteInfo tempNoteInfo = sngNoteList[x];

			int appliedChannel = tempNoteInfo.originalTrack;

			if (appliedChannel == -1)
				appliedChannel = 0;

			if (appliedChannel > MAXCHANNELS)
			{
				MessageBox(NULL, "Error, too many channels for midi", "Error", NULL);
				return;
			}

			channels[appliedChannel].push_back(tempNoteInfo);

			if ((appliedChannel + 1) > numChannels)
				numChannels = (appliedChannel + 1);
		}
	}

	for (int x = 0; x < numChannels; x++)
	{
		std::sort(channels[x].begin(), channels[x].end(), sngSortByStartTime());
	}

	std::vector<SngNoteInfo> overlappingNotes;

	// Check for note overlaps
	for (int channel = 0; channel < numChannels; channel++)
	{
		for (int x = 0; x < channels[channel].size(); x++)
		{
			for (int y = (x + 1); y < channels[channel].size(); y++)
			{
				SngNoteInfo tempNoteInfo = channels[channel][x];
				SngNoteInfo matchNoteInfo = channels[channel][y];

				if (IsOverlap(tempNoteInfo.startAbsoluteTime, tempNoteInfo.endAbsoluteTime, matchNoteInfo.startAbsoluteTime, matchNoteInfo.endAbsoluteTime))
				{
					if (
						(tempNoteInfo.instrument != matchNoteInfo.instrument)
						|| (tempNoteInfo.pan != matchNoteInfo.pan)
						|| (tempNoteInfo.pitchBend != matchNoteInfo.pitchBend)
						|| (tempNoteInfo.volume != matchNoteInfo.volume)
						|| (tempNoteInfo.effect != matchNoteInfo.effect)
						|| (tempNoteInfo.vibrato != matchNoteInfo.vibrato)
						)
					{
						overlappingNotes.push_back(channels[channel][y]);
						channels[channel].erase(channels[channel].begin() + y);
						y--;
					}
				}
			}
		}
	}

	std::sort(overlappingNotes.begin(), overlappingNotes.end(), sngSortByStartTime());

	int startChannel = numChannels;

	for (int x = 0; x < overlappingNotes.size(); x++)
	{
		int appliedChannel = -1;

		for (int z = startChannel; z < MAXCHANNELS; z++)
		{
			if (separateByInstrument && (channels[z].size() > 0))
			{
				if (channels[z][0].instrument != overlappingNotes[x].instrument)
					continue;
			}

			bool overlapped = false;
			for (int y = 0; y < channels[z].size(); y++)
			{
				SngNoteInfo tempNoteInfo = overlappingNotes[x];
				SngNoteInfo matchNoteInfo = channels[z][y];
				
				if (IsOverlap(tempNoteInfo.startAbsoluteTime, tempNoteInfo.endAbsoluteTime, matchNoteInfo.startAbsoluteTime, matchNoteInfo.endAbsoluteTime))
				{
					if (
						(tempNoteInfo.instrument != matchNoteInfo.instrument)
						|| (tempNoteInfo.pan != matchNoteInfo.pan)
						|| (tempNoteInfo.pitchBend != matchNoteInfo.pitchBend)
						|| (tempNoteInfo.volume != matchNoteInfo.volume)
						|| (tempNoteInfo.effect != matchNoteInfo.effect)
						|| (tempNoteInfo.vibrato != matchNoteInfo.vibrato)
						)
					{
						overlapped = true;
						break;
					}
				}
			}

			if (!overlapped)
			{
				appliedChannel = z;
				break;
			}
		}

		if ((appliedChannel > MAXCHANNELS) || (appliedChannel == -1))
		{
			MessageBox(NULL, "Error, too many channels for midi", "Error", NULL);
			return;
		}

		channels[appliedChannel].push_back(overlappingNotes[x]);

		if ((appliedChannel + 1) > numChannels)
			numChannels = (appliedChannel + 1);
	}

	if (numChannels == 0)
	{
		// No Notes
		::DeleteFile(outFileName);
		return;
	}

	FILE* outFile = NULL;

	for (int channel = 0; channel < numChannels; channel++)
	{
		FILE* outDebug = NULL; 
		CString tempChannelStr;
		tempChannelStr.Format("%02X", channel);
		//FILE* outDebug = fopen("C:\\temp\\outDebug_" + tempChannelStr + ".txt", "w");

		/*if (outDebug != NULL)
		{
			for (int x = 0; x < channels[channel].size(); x++)
			{
				fprintf(outDebug, "Note %02X Start %08X End %08X\n", channels[channel][x].noteNumber, channels[channel][x].startAbsoluteTime, channels[channel][x].endAbsoluteTime);
			}
		}*/

		if ((channel % 0x10) == 0)
		{
			if (outFile != NULL)
			{
				// just one track
				
				fflush(outFile);	
				fclose(outFile);

				outFile = NULL;
			}

			int midiNumber = channel / 0x10;

			if (midiNumber == 0)
			{
				outFile = fopen(outFileName, "wb");

				if (outFile == NULL)
				{
					MessageBox(NULL, "Cannot open file for output midi " + outFileName, "Error", NULL);
					return;
				}
			}
			else
			{
				bool midiName = (outFileName.Find(".midi") != -1);
				CString tempMidiNumber;
				tempMidiNumber.Format("%s_AdditionalPart%d", outFileName, midiNumber);
				tempMidiNumber.Replace(".midi", "");
				tempMidiNumber.Replace(".mid", "");
				if (midiName)
					tempMidiNumber += ".midi";
				else
					tempMidiNumber += ".mid";
				outFile = fopen(tempMidiNumber, "wb");

				if (outFile == NULL)
				{
					MessageBox(NULL, "Cannot open file for output midi " + tempMidiNumber, "Error", NULL);
					return;
				}
			}

			
			

			// 1 For Tempo Positions
			int numTracks = 0x11;
			if ((numChannels - channel) < 0x10)
			{
				numTracks = (numChannels - channel) + 1;
			}

			unsigned long tempLong = Flip32Bit(0x4D546864);
			fwrite(&tempLong, 1 ,4 , outFile);
			tempLong = Flip32Bit(0x00000006);
			fwrite(&tempLong, 1 ,4 , outFile);
			tempLong = Flip32Bit(0x00010000 | numTracks); // num tracks
			fwrite(&tempLong, 1 ,4 , outFile);

			unsigned short tempShort = division;
			tempShort = Flip16Bit(tempShort);
			fwrite(&tempShort, 1 ,2 , outFile);

			unsigned long absoluteTimeTempo = 0;


			std::vector<TrackEvent> trackEventsTempo;

			// Write tempo track
			for (int tempoIndex = 0; tempoIndex < tempoPositions.size(); tempoIndex++)
			{
				TrackEvent trackEventNewTempo;

				unsigned long deltaTimeTempo = tempoPositions[tempoIndex].absoluteTime - absoluteTimeTempo;

				trackEventNewTempo.deltaTime = deltaTimeTempo;
				absoluteTimeTempo += deltaTimeTempo;

				trackEventNewTempo.obsoleteEvent = false;

				trackEventNewTempo.absoluteTime = absoluteTimeTempo;

				trackEventNewTempo.type = 0xFF;
				trackEventNewTempo.contentSize = 5;
				trackEventNewTempo.contents = new byte[trackEventNewTempo.contentSize];
				
				trackEventNewTempo.contents[0] = 0x51;
				trackEventNewTempo.contents[1] = 0x3;
				trackEventNewTempo.contents[2] = ((tempoPositions[tempoIndex].value >> 16) & 0xFF);
				trackEventNewTempo.contents[3] = ((tempoPositions[tempoIndex].value >> 8) & 0xFF);
				trackEventNewTempo.contents[4] = ((tempoPositions[tempoIndex].value >> 0) & 0xFF);

				trackEventsTempo.push_back(trackEventNewTempo);
			}



			TrackEvent trackEventEndTempo;
			trackEventEndTempo.deltaTime = 0;
			trackEventEndTempo.obsoleteEvent = false;
			trackEventEndTempo.absoluteTime = absoluteTimeTempo;

			trackEventEndTempo.type = 0xFF;
			trackEventEndTempo.contentSize = 2;
			trackEventEndTempo.contents = new byte[trackEventEndTempo.contentSize];
			
			trackEventEndTempo.contents[0] = 0x2F;
			trackEventEndTempo.contents[1] = 0x0;

			trackEventsTempo.push_back(trackEventEndTempo);

			WriteSngToMidiTrack(outFile, outDebug, trackEventsTempo);

			for (int eventCount = 0; eventCount < trackEventsTempo.size(); eventCount++)
			{
				if (trackEventsTempo[eventCount].contents != NULL)
				{
					delete [] trackEventsTempo[eventCount].contents;
					trackEventsTempo[eventCount].contents = NULL;
				}
			}
		}

		unsigned long absoluteTime = 0;
		std::vector<TrackEvent> trackEvents;
		
		// MTrk

		unsigned char currentPan = 0x40;
		unsigned long currentInstrument = 0xFF;
		unsigned char currentVolume = 0x7F;
		unsigned char currentPitchBend = 0x40;
		int currentEffect = 0;
		unsigned char currentVibrato = 0x00;

		int controller = channel % 0x10;

		int currentSegmentNumber = -1;

		if (channels[channel].size() > 0)
		{
			if (expandBendRange && (bendRange > 0))
			{
				if (bendRange > 24)
					bendRange = 24;
				if (bendRange < 1)
					bendRange = 1;

				{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = 0;
				trackEventNew.obsoleteEvent = false;
				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 101;
				trackEventNew.contents[1] = 0;
				
				trackEvents.push_back(trackEventNew);
				}

				{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = 0;
				trackEventNew.obsoleteEvent = false;
				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 100;
				trackEventNew.contents[1] = 0;
				
				trackEvents.push_back(trackEventNew);
				}

				{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = 0;
				trackEventNew.obsoleteEvent = false;
				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 6;
				trackEventNew.contents[1] = bendRange;
				
				trackEvents.push_back(trackEventNew);
				}

				{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = 0;
				trackEventNew.obsoleteEvent = false;
				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 38;
				trackEventNew.contents[1] = 0;
				
				trackEvents.push_back(trackEventNew);
				}
			}
		}

		std::vector<SngNoteInfo> pendingShutOffNotes;
		for (int x = 0; x < channels[channel].size(); x++)
		{
			SngNoteInfo songNoteInfo = channels[channel][x];
			bool errorStartEnd = (songNoteInfo.endAbsoluteTime <= songNoteInfo.startAbsoluteTime);
			if (outDebug != NULL)
				fprintf(outDebug, "Time: %08X ON %02X %04X: Start %08X End %08X Error %d\n", absoluteTime, songNoteInfo.noteNumber, x, songNoteInfo.startAbsoluteTime, songNoteInfo.endAbsoluteTime, errorStartEnd);

			while (pendingShutOffNotes.size() > 0)
			{
				std::sort(pendingShutOffNotes.begin(), pendingShutOffNotes.end(), sngSortByEndTime());

				if (pendingShutOffNotes[0].endAbsoluteTime < songNoteInfo.startAbsoluteTime)
				{
					if (outDebug != NULL)
						fprintf(outDebug, "Time: %08X Note %02X Shut off: Start %08X End %08X Error %d\n", absoluteTime, pendingShutOffNotes[0].noteNumber, pendingShutOffNotes[0].startAbsoluteTime, pendingShutOffNotes[0].endAbsoluteTime, errorStartEnd);

					unsigned long deltaTime = pendingShutOffNotes[0].endAbsoluteTime - absoluteTime;

					TrackEvent trackEventNewNoteOff;
					trackEventNewNoteOff.deltaTime = deltaTime;
					trackEventNewNoteOff.obsoleteEvent = false;
					trackEventNewNoteOff.absoluteTime = absoluteTime;
					trackEventNewNoteOff.type = 0x80 | controller;

					trackEventNewNoteOff.durationTime = 0; // to be filled in
					trackEventNewNoteOff.contentSize = 2;
					trackEventNewNoteOff.contents = new byte[trackEventNewNoteOff.contentSize];
					trackEventNewNoteOff.contents[0] = pendingShutOffNotes[0].noteNumber;
					trackEventNewNoteOff.contents[1] = pendingShutOffNotes[0].velocity;
					trackEvents.push_back(trackEventNewNoteOff);

					absoluteTime += deltaTime;

					pendingShutOffNotes.erase(pendingShutOffNotes.begin());
				}
				else if (pendingShutOffNotes[0].endAbsoluteTime == songNoteInfo.startAbsoluteTime)
				{
					bool skipOff = false;

					for (int y = x; y < channels[channel].size(); y++)
					{
						SngNoteInfo tempSongNoteInfo = channels[channel][y];
						if (
							(pendingShutOffNotes[0].endAbsoluteTime == tempSongNoteInfo.startAbsoluteTime)
							&& (pendingShutOffNotes[0].noteNumber == tempSongNoteInfo.noteNumber)
							&& (pendingShutOffNotes[0].instrument == tempSongNoteInfo.instrument)
							&& (pendingShutOffNotes[0].originalTrack == tempSongNoteInfo.originalTrack)
							&& (pendingShutOffNotes[0].originalNoteUniqueId == tempSongNoteInfo.originalNoteUniqueId)
							)
						{
							if (outDebug != NULL)
								fprintf(outDebug, "Time: %08X Merged Note %02X : Start %08X End %08X Start %08X End %08X Error %d\n", absoluteTime, pendingShutOffNotes[0].noteNumber, pendingShutOffNotes[0].startAbsoluteTime, pendingShutOffNotes[0].endAbsoluteTime, tempSongNoteInfo.startAbsoluteTime, tempSongNoteInfo.endAbsoluteTime, errorStartEnd);

							pendingShutOffNotes[0] = tempSongNoteInfo;
							channels[channel][y].ignoreNoteOn = true;

							if (y == x)
								songNoteInfo.ignoreNoteOn = true;

							skipOff = true;
							break;
						}
						else if (pendingShutOffNotes[0].endAbsoluteTime < tempSongNoteInfo.startAbsoluteTime)
						{
							break;
						}
					}

					if (!skipOff)
					{
						unsigned long deltaTime = pendingShutOffNotes[0].endAbsoluteTime - absoluteTime;

						TrackEvent trackEventNewNoteOff;
						trackEventNewNoteOff.deltaTime = deltaTime;
						trackEventNewNoteOff.obsoleteEvent = false;
						trackEventNewNoteOff.absoluteTime = absoluteTime;
						trackEventNewNoteOff.type = 0x80 | controller;

						trackEventNewNoteOff.durationTime = 0; // to be filled in
						trackEventNewNoteOff.contentSize = 2;
						trackEventNewNoteOff.contents = new byte[trackEventNewNoteOff.contentSize];
						trackEventNewNoteOff.contents[0] = pendingShutOffNotes[0].noteNumber;
						trackEventNewNoteOff.contents[1] = pendingShutOffNotes[0].velocity;
						trackEvents.push_back(trackEventNewNoteOff);

						absoluteTime += deltaTime;

						if (outDebug != NULL)
							fprintf(outDebug, "Time: %08X Note %02X Shut off Couldn't Skip: Start %08X End %08X Error %d\n", absoluteTime, pendingShutOffNotes[0].noteNumber, pendingShutOffNotes[0].startAbsoluteTime, pendingShutOffNotes[0].endAbsoluteTime, errorStartEnd);

						pendingShutOffNotes.erase(pendingShutOffNotes.begin());
					}
				}
				else
				{
					break;
				}
			}
			

			if (!separateByInstrument)
			{
				if (songNoteInfo.segmentNumber != currentSegmentNumber)
				{
					// Pending should be clear cause new segment
					if (pendingShutOffNotes.size() > 0)
					{
						// Error
						pendingShutOffNotes = pendingShutOffNotes;
					}

					TrackEvent trackEventNew;

					trackEventNew.deltaTime = 0;
					trackEventNew.obsoleteEvent = false;
					trackEventNew.absoluteTime = absoluteTime;

					trackEventNew.type = 0xB0 | controller;
					trackEventNew.contentSize = 2;
					trackEventNew.contents = new byte[trackEventNew.contentSize];
					trackEventNew.contents[0] = 104;
					trackEventNew.contents[1] = songNoteInfo.segmentNumber;
					
					trackEvents.push_back(trackEventNew);

					currentSegmentNumber = songNoteInfo.segmentNumber;
				}
			}

			unsigned long deltaTime = songNoteInfo.startAbsoluteTime - absoluteTime;

			// Changed to master track
			/*if (songNoteInfo.tempo != currentTempo)
			{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNew.obsoleteEvent = false;

				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xFF;
				trackEventNew.contentSize = 5;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				
				trackEventNew.contents[0] = 0x51;
				trackEventNew.contents[1] = 0x3;
				trackEventNew.contents[2] = ((songNoteInfo.tempo >> 16) & 0xFF);
				trackEventNew.contents[3] = ((songNoteInfo.tempo >> 8) & 0xFF);
				trackEventNew.contents[4] = ((songNoteInfo.tempo >> 0) & 0xFF);

				trackEvents.push_back(trackEventNew);

				currentTempo = songNoteInfo.tempo;
			}*/

			if (songNoteInfo.effect != currentEffect)
			{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNew.obsoleteEvent = false;

				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 91; // reverb
				trackEventNew.contents[1] = songNoteInfo.effect;

				trackEvents.push_back(trackEventNew);

				currentEffect = songNoteInfo.effect;
			}

			if (songNoteInfo.instrument != currentInstrument)
			{
				TrackEvent trackEventNewMSB;

				trackEventNewMSB.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNewMSB.obsoleteEvent = false;

				trackEventNewMSB.absoluteTime = absoluteTime;

				trackEventNewMSB.type = 0xB0 | controller;
				trackEventNewMSB.contentSize = 2;
				trackEventNewMSB.contents = new byte[trackEventNewMSB.contentSize];
				trackEventNewMSB.contents[0] = 00;
				trackEventNewMSB.contents[1] = songNoteInfo.instrument / 0x8000;

				trackEvents.push_back(trackEventNewMSB);






				
				TrackEvent trackEventNewLSB;

				trackEventNewLSB.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNewLSB.obsoleteEvent = false;

				trackEventNewLSB.absoluteTime = absoluteTime;

				trackEventNewLSB.type = 0xB0 | controller;
				trackEventNewLSB.contentSize = 2;
				trackEventNewLSB.contents = new byte[trackEventNewLSB.contentSize];
				trackEventNewLSB.contents[0] = 32;
				trackEventNewLSB.contents[1] = songNoteInfo.instrument / 0x80;

				trackEvents.push_back(trackEventNewLSB);



				TrackEvent trackEventNew;

				trackEventNew.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNew.obsoleteEvent = false;

				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xC0 | controller;
				trackEventNew.contentSize = 1;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = songNoteInfo.instrument & 0x7F;

				trackEvents.push_back(trackEventNew);

				currentInstrument = songNoteInfo.instrument;
			}

			if (songNoteInfo.pan != currentPan)
			{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNew.obsoleteEvent = false;

				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 10;
				trackEventNew.contents[1] = songNoteInfo.pan;

				trackEvents.push_back(trackEventNew);

				currentPan = songNoteInfo.pan;
			}

			if (songNoteInfo.volume != currentVolume)
			{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNew.obsoleteEvent = false;

				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 0x07;
				trackEventNew.contents[1] = songNoteInfo.volume;

				trackEvents.push_back(trackEventNew);

				currentVolume = songNoteInfo.volume;
			}

			if (songNoteInfo.vibrato != currentVibrato)
			{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNew.obsoleteEvent = false;

				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xB0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 0x01;
				trackEventNew.contents[1] = songNoteInfo.vibrato;

				trackEvents.push_back(trackEventNew);

				currentVibrato = songNoteInfo.vibrato;
			}

			if (songNoteInfo.pitchBend != currentPitchBend)
			{
				TrackEvent trackEventNew;

				trackEventNew.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNew.obsoleteEvent = false;

				trackEventNew.absoluteTime = absoluteTime;

				trackEventNew.type = 0xE0 | controller;
				trackEventNew.contentSize = 2;
				trackEventNew.contents = new byte[trackEventNew.contentSize];
				trackEventNew.contents[0] = 0x00; // LSB
				trackEventNew.contents[1] = songNoteInfo.pitchBend; // MSB

				trackEvents.push_back(trackEventNew);

				currentPitchBend = songNoteInfo.pitchBend;
			}

			if (!songNoteInfo.ignoreNoteOn)
			{
				// Note On
				TrackEvent trackEventNewNoteOn;

				trackEventNewNoteOn.deltaTime = deltaTime;
				absoluteTime += deltaTime;
				deltaTime = 0;

				trackEventNewNoteOn.obsoleteEvent = false;

				trackEventNewNoteOn.absoluteTime = absoluteTime;

				trackEventNewNoteOn.type = 0x90 | controller;

				trackEventNewNoteOn.contentSize = 2;
				trackEventNewNoteOn.contents = new byte[trackEventNewNoteOn.contentSize];
				trackEventNewNoteOn.contents[0] = songNoteInfo.noteNumber;
				trackEventNewNoteOn.contents[1] = songNoteInfo.velocity;

				trackEvents.push_back(trackEventNewNoteOn);

				pendingShutOffNotes.push_back(songNoteInfo);
			}
		}

		if (pendingShutOffNotes.size() > 0)
		{
			std::sort(pendingShutOffNotes.begin(), pendingShutOffNotes.end(), sngSortByEndTime());
			while (pendingShutOffNotes.size() > 0)
			{
				unsigned long deltaTime = pendingShutOffNotes[0].endAbsoluteTime - absoluteTime;


				TrackEvent trackEventNewNoteOff;
				trackEventNewNoteOff.deltaTime = deltaTime;
				trackEventNewNoteOff.obsoleteEvent = false;
				trackEventNewNoteOff.absoluteTime = absoluteTime;
				trackEventNewNoteOff.type = 0x80 | controller;

				trackEventNewNoteOff.durationTime = 0; // to be filled in
				trackEventNewNoteOff.contentSize = 2;
				trackEventNewNoteOff.contents = new byte[trackEventNewNoteOff.contentSize];
				trackEventNewNoteOff.contents[0] = pendingShutOffNotes[0].noteNumber;
				trackEventNewNoteOff.contents[1] = pendingShutOffNotes[0].velocity;
				trackEvents.push_back(trackEventNewNoteOff);

				absoluteTime += deltaTime;

				pendingShutOffNotes.erase(pendingShutOffNotes.begin());
			}
		}

		TrackEvent trackEventEnd;
		trackEventEnd.deltaTime = 0;
		trackEventEnd.obsoleteEvent = false;
		trackEventEnd.absoluteTime = absoluteTime;

		trackEventEnd.type = 0xFF;
		trackEventEnd.contentSize = 2;
		trackEventEnd.contents = new byte[trackEventEnd.contentSize];
		
		trackEventEnd.contents[0] = 0x2F;
		trackEventEnd.contents[1] = 0x0;

		trackEvents.push_back(trackEventEnd);



		WriteSngToMidiTrack(outFile, outDebug, trackEvents);

		if (outDebug != NULL)
			fclose(outDebug);

		for (int eventCount = 0; eventCount < trackEvents.size(); eventCount++)
		{
			if (trackEvents[eventCount].contents != NULL)
			{
				delete [] trackEvents[eventCount].contents;
				trackEvents[eventCount].contents = NULL;
			}
		}
	}

	// just one track
	
	if (outFile != NULL)
	{
		fflush(outFile);	
		fclose(outFile);

		outFile = NULL;
	}
}

int CMidiParse::FindHighestKonamiLengthTrack(int trackNumber, unsigned char* buffer, unsigned long offset, unsigned long end)
{
	unsigned char command = 0x00;
	unsigned long spot = offset;


	unsigned long absoluteTime = 0;

	unsigned char lastDuration = 0xFF;
	unsigned char lastLength = 0xFF;

	unsigned char loopAmountsLeft = 0x00;
	unsigned long loopSpot = 0;
	unsigned long loopEndSpot = 0;

	unsigned char loopNestedAmountsLeft = 0x00;
	unsigned long loopNestedSpot = 0;
	unsigned long loopNestedEndSpot = 0;

	unsigned char lastNoteValue = 0x00;

	int currentEDOffset = 0;
	int currentEEEndOffset = 0;
	int eeCountOverall = 0;
	
	bool lastNote00Length = false;
	unsigned char lastNote = 0xFF;
	unsigned char lastVelocity = 0xFF;

	int previousCmd = -1;
	

	while ((command != 0xFF) && (spot < end))
	{
		command = buffer[spot];
		
		spot++;

		if (command < 0xD0) // Note or Rest
		{
			unsigned char note = command;

			unsigned char drumInstrumentValue = 0x00;
			if ((command == 0x67) || (command == (0x67 + 0x68))) // Rest?
			{
				drumInstrumentValue = buffer[spot];
				spot++;
			}

			if (note >= 0x68)
			{
				note -= 0x68;
			}
			else
			{
				lastDuration = buffer[spot];
				spot++;
			}

			bool firstSetNote00Length = false;

			if (buffer[spot] < 0x80)
			{
				lastLength = buffer[spot];
				spot++;

				if (lastLength == 0)
					lastNote00Length = true;
				else
					lastNote00Length = false;

				firstSetNote00Length = true;
			}
			else
			{
				if (lastLength == 0)
					lastNote00Length = true;
				else
					lastNote00Length = false;

				if ((lastLength == 0) && (previousCmd == 0xF2))
				{
					firstSetNote00Length = true;
				}
				else
				{
					firstSetNote00Length = false;
				}
			}

			unsigned char velocity = buffer[spot] & 0x7F;
			spot++;

			unsigned long noteDuration = lastDuration;
			unsigned long noteLength = lastLength;

			if (noteLength == 0)
				noteLength = noteDuration;
			else if (noteLength > noteDuration)
				noteLength = noteDuration;
			else if (noteLength <= noteDuration)
			{
				if (noteLength == 0x7F)
					noteLength = noteDuration;
				else
					noteLength = noteLength;
			}

			lastVelocity = velocity;
			lastNote = note;

			absoluteTime += noteDuration;
		}
		else
		{
			if (command == 0xD0)
			{
				//fprintf(outFile, " ((Tempo))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				spot++;
			}
			else if (command == 0xD1)
			{
				//fprintf(outFile, " ((Tempo Change Fade))");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				spot += 2;
			}
			else if (command == 0xD2)
			{
				//fprintf(outFile, " ((Instrument Change))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xD3)
			{
				//fprintf(outFile, " ((Instrument/Volume Change))");
				//fprintf(outFile, " Volume %02X (%d) Instrument %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
		
				spot += 2;
			}
			else if (command == 0xD4)
			{
				//fprintf(outFile, " ((Instrument/Volume/Pan Initial))");
				//fprintf(outFile, " Volume %02X (%d) Instrument %02X (%d) Pan %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);

				spot += 3;
			}
			else if (command == 0xD5)
			{
				//fprintf(outFile, " ((Volume))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xD6)
			{
				//fprintf(outFile, " ((Fade In/Out))");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				spot += 2;
			}
			else if (command == 0xD7)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
				spot += 2;
			}
			else if (command == 0xD8)
			{
				//fprintf(outFile, " ((Release Time))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xD9)
			{
				//fprintf(outFile, " ((Section Hold Note))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xDA)
			{
				//fprintf(outFile, " ((Reverb))");
				//fprintf(outFile, " Type/Separation %02X (%d) Amount %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				spot += 2;
			}
			else if (command == 0xDB)
			{
				//fprintf(outFile, " UNKNOWN (Invalid)");
				break;
			}
			else if (command == 0xDC)
			{
				//fprintf(outFile, " UNKNOWN (Invalid)");
				break;
			}
			else if (command == 0xDD)
			{
				//fprintf(outFile, " ((Pan))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				spot++;
			}
			else if (command == 0xDE)
			{
				//fprintf(outFile, " Stereo Pan");
				//fprintf(outFile, " Left %02X (%d) to Right %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
				spot += 2;
			}
			else if (command == 0xDF)
			{
				//fprintf(outFile, " ((Coarse Tune))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				spot++;
			}
			else if (command == 0xE0)
			{
				//fprintf(outFile, " ((Fine Tune))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xE1)
			{
				//fprintf(outFile, " ((Tremolo))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot += 3;
			}
			else if (command == 0xE2) // Jikkyou Powerful Pro Yakyuu 2000 (J) (V1.0) [!]
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xE3) // Start new game and let go to hit
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot += 3;
			}
			// E4??
			else if (command == 0xE4) // Something weird about this, seems like a followup to a note, trying this for now
			{
				//fprintf(outFile, " ((Pitch Bend Previous Note))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);

				/*if (trackOutputNotes.size() > 0)
				{
					unsigned char endPitchBend = buffer[spot+2];

					int pitchBendChange = endPitchBend - lastNoteValue;

					if (pitchBendChange >= 2)
						trackOutputNotes.back().pitchBend = 0x7F;
					else if (pitchBendChange >= 1)
						trackOutputNotes.back().pitchBend = 0x60;
					else if (pitchBendChange <= -2)
						trackOutputNotes.back().pitchBend = 0x00;
					else if (pitchBendChange <= -1)
						trackOutputNotes.back().pitchBend = 0x20;
					else
						trackOutputNotes.back().pitchBend = 0x40;
				}*/

				spot += 3;
			}
			else if (command == 0xE5) // No. 47 music
			{
				//fprintf(outFile, " ((Pitch Ascend Next Note))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				spot += 3;
			}
			else if (command == 0xE6)
			{
				//fprintf(outFile, " ((Slide Notes in Section))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xE7)
			{
				//fprintf(outFile, " ((Marker))");
				loopSpot = spot - 1;
			}
			else if (command == 0xE8)
			{
				//fprintf(outFile, " ((Loop from Marker))");
				//fprintf(outFile, " %02X (%d) Times %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				unsigned char readAmount = buffer[spot];
				spot += 3;

				if (readAmount > 0x01)
				{
					if (loopEndSpot != spot)
					{
						loopEndSpot = spot;
						spot = loopSpot;

						loopAmountsLeft = readAmount - 1;
					}
					else
					{
						loopAmountsLeft--;

						if (loopAmountsLeft > 0)
						{
							spot = loopSpot;
						}
						else
						{
							// Reset
							loopEndSpot = 0;
						}
					}
				}
			}
			else if (command == 0xE9)
			{
				//fprintf(outFile, " ((Nested Loop Marker))");
				loopNestedSpot = spot - 1;
			}
			else if (command == 0xEA)
			{
				//fprintf(outFile, " ((Nested Loop from Marker))");
				//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
				unsigned char readAmount = buffer[spot];
				spot += 3;

				if (readAmount > 0x01)
				{
					if (loopNestedEndSpot != spot)
					{
						loopNestedEndSpot = spot;
						spot = loopNestedSpot;

						loopNestedAmountsLeft = readAmount - 1;
					}
					else
					{
						loopNestedAmountsLeft--;

						if (loopNestedAmountsLeft > 0)
						{
							spot = loopNestedSpot;
						}
						else
						{
							// Reset
							loopNestedEndSpot = 0;
						}
					}
				}
			}
			else if (command == 0xEB)
			{
				//fprintf(outFile, " ((Master Loop Start))");
			}
			else if (command == 0xEC)
			{
				//fprintf(outFile, " ((Master Loop End))");
			}
			else if (command == 0xED)
			{
				//fprintf(outFile, " ((Loop Skip Start))");
				currentEDOffset = spot;
				currentEEEndOffset = 0;

				eeCountOverall = 0;
			}
			else if (command == 0xEE)
			{
				//fprintf(outFile, " ((Loop Skip Middle))");

				if (eeCountOverall == 0)
				{
					if (currentEEEndOffset != 0x00000000)
					{
						spot = currentEEEndOffset;
					}
					
					eeCountOverall = 1;
				}
				else if (eeCountOverall == 1)
				{
					currentEEEndOffset = spot;

					spot = currentEDOffset;

					eeCountOverall = 0;
				}
			}
			else if (command == 0xEF)
			{
				//fprintf(outFile, " ((Effect Delay))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

				spot++;
			}
			else if (command == 0xF0)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xF1)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				spot++;
			}
			else if (command == 0xF2)
			{
				//fprintf(outFile, " ((Delay Note))");
				//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
				absoluteTime += buffer[spot];

				lastNote00Length = false;

				spot++;
			}
			else if (command == 0xF3)
			{
				//fprintf(outFile, " ((Previous Note Hold))");
				//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

				lastNote00Length = false;

				unsigned long noteDuration = buffer[spot];;
				unsigned long noteLength = buffer[spot+1];
				if (noteLength == 0)
					noteLength = noteDuration;
				else if (noteLength > noteDuration)
					noteLength = noteDuration;
				else if (noteLength <= noteDuration)
				{
					if (noteLength == 0x7F)
						noteLength = noteDuration;
					else
						noteLength = noteLength;
				}

				absoluteTime += noteDuration;

				spot += 2;
			}
			// FA/FB Goemon's Great Adventure (U) - 0053D780, not sure if legit 
			else if (command == 0xFA)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
				spot++;
			}
			else if (command == 0xFB)
			{
				//fprintf(outFile, " ?");
				//fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
				spot+=2;
			}
			else if (command >= 0xF4)
			{
				//fprintf(outFile, " End");
				break;
			}
			else
			{
				CString tempStr;
				tempStr.Format("Unknown Command %02X at Spot %04X", command, spot);
				MessageBox(NULL, tempStr, "Error", NULL);
				return 0;
			}
		}

		previousCmd = command;
	}

	return absoluteTime;
}

unsigned char CMidiParse::sseqCommandSizes[0x24] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
    0x02, 0x03, 0x02, 0x04, 0x05, 0x05, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x01, 0x01, 0x03, 0x03, 
    0x03, 0x01, 0x01, 0x01,
};

int CMidiParse::FindHighestSSEQLengthTrack(unsigned char* inputMID, int endSize, int numberTracks)
{
	int highestTime = 0;

	unsigned long offsetData = 0;
	for (int track = 0; track < numberTracks; track++)
	{
		unsigned short extraFlag = CharArrayToShort(&inputMID[offsetData + 0xE]);
		unsigned long sizeTrack = CharArrayToLong(&inputMID[offsetData + 0x10]);
		
		if (extraFlag)
		{
			offsetData += 4;
		}

		offsetData += 0x14;

		unsigned char command = 0x00;
		int spot = offsetData;

		unsigned long absoluteTime = 0;

		while (spot < endSize)
		{
			unsigned long original;
			unsigned char* altPattern = NULL;
			byte altOffset = 0;
			byte altLength = 0;
			unsigned long deltaTime = GetVLBytes(inputMID, spot, original, altPattern, altOffset, altLength, false);

			absoluteTime += deltaTime;

			command = inputMID[spot];

			if (command == 0x22)
			{
				break;
			}

			spot += sseqCommandSizes[command];
		}

		if (absoluteTime > highestTime)
			highestTime = absoluteTime;

		offsetData += sizeTrack;
	}	

	return highestTime;
}

void CMidiParse::ParsePaperMarioTrack(unsigned char* ROM, int romSize, int trackNumber, int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes, unsigned char* buffer, unsigned long offset, unsigned long end, int& noteUniqueId, unsigned long drumDataPointer, int numberDrum, unsigned long instrumentDataPointer, int numberInst, unsigned short trackFlags, std::vector<int>& usedExtraInstruments, unsigned char& currentPan, unsigned long& currentInstrument, unsigned char& currentVolume, int& currentEffect, unsigned long& currentTempo, signed long& currentCoarseTune, bool& setReverb, bool& setVolume, bool& setPan, int& absoluteTimeStart, int subSegmentCounter)
{
	bool isDrumTrack = ((trackFlags & 0x80) == 0x80);

	std::vector<SngNoteInfo> trackOutputNotes;

	unsigned char command = 0xFF;
	unsigned long spot = offset;

	unsigned long absoluteTime = absoluteTimeStart;

	std::vector<int> subroutineJumpSpots;
	std::vector<int> subroutineEndSpot;

	unsigned long randomJumpBackSpot = 0;
	int randomJumpOffsetBackCounter = -1;
	unsigned long absoluteTimeBeforeRandomJump = 0;
	int numberSets = 0;

	while (command != 0x00)
	{
		if (trackNumber >= 0)
		{
			if (trackNumber > 0)
			{
				for (int y = 0; y < tempoPositions.size(); y++)
				{
					if (tempoPositions[y].absoluteTime <= absoluteTime)
					{
						currentTempo = tempoPositions[y].value;
					}
					else
					{
						break;
					}
				}
			}
		}

		if (subroutineJumpSpots.size() > 0)
		{
			if (subroutineEndSpot[subroutineEndSpot.size()-1] == spot)
			{
				//fprintf(outFile, "...((SubroutineEnd, going to %04X)) \n", subroutineJumpSpots[subroutineJumpSpots.size()-1]);

				spot = subroutineJumpSpots[subroutineJumpSpots.size()-1];

				subroutineJumpSpots.pop_back();
				subroutineEndSpot.pop_back();
			}
		}

		command = buffer[spot];
		//fprintf(outFile, "%08X Time: %08X", spot, absoluteTime);

		spot++;

		if (command < 0x80)
		{
			if (command == 0x00)
			{
				if (randomJumpOffsetBackCounter == -1)
				{
					//fprintf(outFile, " 00 ((End)) \n");
					break;
				}
				else
				{
					command = 0xFC;
					//fprintf(outFile, "...((Random Jump End, going back to %04X)) \n", randomJumpBackSpot);

					spot = randomJumpBackSpot;
				}
			}
			else if (command < 0x78)
			{
				//fprintf(outFile, " ((Delay))");
				//fprintf(outFile, " %02X (%d)", command, command);

				absoluteTime += command;
			}
			else
			{
				unsigned long value = 0x78 + buffer[spot] + ((command & 0x7) << 8);

				//fprintf(outFile, " ((Delay))");
				//fprintf(outFile, " %02X (%d) %02X - %04X (%d)", command, command, buffer[spot], value, value);
				spot++;

				absoluteTime += value;
			}
		}
		else
		{
			// 0x80 to 0xD4
			if (command < 0xD4)
			{
				//fprintf(outFile, " Note:   ");
				unsigned char note = command & 0x7F;
				unsigned char velocity = buffer[spot];
				
				unsigned long length;
				if (buffer[spot+1] < 0xC0)
				{
					length = buffer[spot+1];

					//fprintf(outFile, " %02X (%d) [Really %02X (%0d)] Velocity %02X (%d) Length %02X (%d)", note, note, command, command, buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

					spot += 2;
				}
				else
				{
					length = 0xC0 + ((buffer[spot+1] & 0x3F) << 8) + buffer[spot+2];

					//fprintf(outFile, " %02X (%d) [Really %02X (%0d)] Velocity %02X (%d) Length %02X%02X - Means %04X (%d)", note, note, command, command, buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+2], length, length);

					spot += 3;
				}

				if (!isDrumTrack)
				{
					SngNoteInfo songNoteInfo;
					songNoteInfo.originalTrack = trackNumber;
					songNoteInfo.originalNoteUniqueId = noteUniqueId++;
					songNoteInfo.startAbsoluteTime = absoluteTime;
					songNoteInfo.noteNumber = note + currentCoarseTune + 0xC;

					
					songNoteInfo.instrument = currentInstrument;

					songNoteInfo.velocity = velocity;

					songNoteInfo.effect = currentEffect;
					songNoteInfo.tempo = currentTempo;
					songNoteInfo.pan = currentPan & 0x7F;

					songNoteInfo.pitchBend = 0x40;
					songNoteInfo.volume = ((currentVolume) & 0x7F);


					songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + length;
					songNoteInfo.segmentNumber = subSegmentCounter;

					trackOutputNotes.push_back(songNoteInfo);
				}
				else
				{
					SngNoteInfo songNoteInfo;
					songNoteInfo.originalTrack = trackNumber;
					songNoteInfo.originalNoteUniqueId = noteUniqueId++;
					songNoteInfo.startAbsoluteTime = absoluteTime;

					
					if (note < 0x48)
					{
						songNoteInfo.noteNumber = note + 0xC;
						songNoteInfo.instrument = (2 * 0x80);
					}
					else
					{
						songNoteInfo.noteNumber = 0x3C + currentCoarseTune + 0xC;
						songNoteInfo.instrument = (note - 0x48) + (1 * 0x80);

						int actualInstrument = buffer[drumDataPointer + ((note - 0x48) * 0xC) + 1];

						if (!setVolume)
						{
							currentVolume = buffer[drumDataPointer + ((note - 0x48) * 0xC) + 4];
							if (currentVolume > 0x7F)
								currentVolume = 0x7F;
						}

						if (!setPan)
						{
							currentPan = (buffer[drumDataPointer + ((note - 0x48) * 0xC) + 5] & 0x7F);
						}

						if (!setReverb)
						{
							currentEffect = buffer[drumDataPointer + ((note - 0x48) * 0xC) + 6];
						}
					}

					songNoteInfo.velocity = velocity;

					songNoteInfo.effect = currentEffect;
					songNoteInfo.tempo = currentTempo;
					songNoteInfo.pan = currentPan & 0x7F;

					songNoteInfo.pitchBend = 0x40;
					songNoteInfo.volume = ((currentVolume) & 0x7F);


					songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + length;

					songNoteInfo.segmentNumber = subSegmentCounter;
					trackOutputNotes.push_back(songNoteInfo);
				}
			}
			else
			{
				//fprintf(outFile, " Command: %02X ", command);
				if (command == 0xE0)
				{
					//fprintf(outFile, " ((Master Tempo))");
					//fprintf(outFile, " %04X (%d)", CharArrayToShort(&buffer[spot]), CharArrayToShort(&buffer[spot]));

					unsigned short tempo = (buffer[spot] << 8) | buffer[spot+1];

					currentTempo = (unsigned long)(60000000.0 / (float)tempo);

					if (trackNumber == 0)
					{
						tempoPositions.push_back(TimeAndValue(absoluteTime, currentTempo));
					}

					spot += 2;
				}
				else if (command == 0xE1)
				{
					//fprintf(outFile, " ((Master Volume))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
					spot++;
				}
				else if (command == 0xE2)
				{
					//fprintf(outFile, " ((Master Transpose))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], (signed char)buffer[spot]);
					spot++;
				}
				else if (command == 0xE3)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
					spot++;
				}
				else if (command == 0xE4)
				{
					//fprintf(outFile, " ((Master Change Tempo))");
					//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2], buffer[spot+3], buffer[spot+3]);

					unsigned short tempo = (buffer[spot+2] << 8) | buffer[spot+3];

					currentTempo = (unsigned long)(60000000.0 / (float)tempo);

					if (trackNumber == 0)
					{
						tempoPositions.push_back(TimeAndValue(absoluteTime, currentTempo));
					}

					spot += 4;
				}
				else if (command == 0xE5)
				{
					//fprintf(outFile, " ((Master Volume Fade In))");
					//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
					spot += 3;
				}
				else if (command == 0xE6)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
					spot += 2;
				}
				else if (command == 0xE7)
				{
					//fprintf(outFile, " ((?))");
				}
				else if (command == 0xE8)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);

					currentInstrument = buffer[spot+1] + (10 * 0x80);

					if (std::find(usedExtraInstruments.begin(), usedExtraInstruments.end(), buffer[spot+1]) == usedExtraInstruments.end()) 
					{
						usedExtraInstruments.push_back(buffer[spot+1]);
					}

					spot += 2;
				}
				else if (command == 0xE9)
				{
					//fprintf(outFile, " ((Track Volume))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

					currentVolume = buffer[spot];
					if (currentVolume > 0x7F)
						currentVolume = 0x7F;
					spot++;

					setVolume = true;
				}
				else if (command == 0xEA)
				{
					//fprintf(outFile, " ((Track Pan))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

					currentPan = buffer[spot] & 0x7F;
					spot++;

					setPan = true;
				}
				else if (command == 0xEB)
				{
					//fprintf(outFile, " ((Reverb))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
					currentEffect = buffer[spot];
					spot++;

					setReverb = true;
				}
				else if (command == 0xEC)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
					spot++;
				}
				else if (command == 0xED)
				{
					//fprintf(outFile, " ((Coarse Tune))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
			
					currentCoarseTune = (signed char)buffer[spot];

					spot++;
				}
				else if (command == 0xEE)
				{
					//fprintf(outFile, " ((Fine Tune))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
					spot++;
				}
				else if (command == 0xEF)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
					spot += 2;
				}
				else if (command == 0xF0)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
					spot += 3;
				}
				else if (command == 0xF1)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
					spot++;
				}
				else if (command == 0xF2)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

					spot++;
				}
				else if (command == 0xF3)
				{
					//fprintf(outFile, " ((?))");
				}
				else if (command == 0xF4)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1]);
					spot += 2;
				}
				else if (command == 0xF5)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);

					if (!isDrumTrack)
					{
						currentInstrument = buffer[spot];

						int actualInstrument = buffer[instrumentDataPointer + (currentInstrument * 8) + 1];
						if (!setVolume)
						{
							currentVolume = buffer[instrumentDataPointer + (currentInstrument * 8) + 2];
							if (currentVolume > 0x7F)
								currentVolume = 0x7F;
						}

						if (!setPan)
						{
							currentPan = (buffer[instrumentDataPointer + (currentInstrument * 8) + 3] & 0x7F);
						}

						if (!setReverb)
						{
							currentEffect = buffer[instrumentDataPointer + (currentInstrument * 8) + 4];
						}
					}

					/*if (currentInstrument < numberInst)
					{
						currentInstrument = buffer[instrumentDataPointer + (currentInstrument * 0x8) + 1];
					}*/
					spot++;
				}
				else if (command == 0xF6)
				{
					//fprintf(outFile, " ((Fade out/in volume of track))");
					//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);

					currentVolume = buffer[spot+2];
					spot += 3;
				}
				else if (command == 0xF7)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d)", buffer[spot], buffer[spot]);
					spot++;
				}
				else if (command == 0xF8)
				{
					//fprintf(outFile, " ((?))");
				}
				else if (command == 0xF9)
				{
					//fprintf(outFile, " ((?))");
				}
				else if (command == 0xFA)
				{
					//fprintf(outFile, " ((?))");
				}
				else if (command == 0xFB)
				{
					//fprintf(outFile, " ((?))");
				}
				else if (command == 0xFC)
				{
					unsigned short offsetsAt = ((buffer[spot] << 8) | buffer[spot+1]);
					numberSets = buffer[spot+2];

					if (randomJumpBackSpot != (spot - 1))
					{
						absoluteTimeBeforeRandomJump = absoluteTime;
						randomJumpBackSpot = spot - 1;
						randomJumpOffsetBackCounter = 0;
					}

					
					
					if (randomJumpOffsetBackCounter == 0)
					{
						//fprintf(outFile, " ((Jump from set))");
						//fprintf(outFile, " Offset %02X%02X # Random Offsets %02X (%d) Entry #%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2], randomJumpOffsetBackCounter);

						unsigned short jumpOffset = ((buffer[offsetsAt + (randomJumpOffsetBackCounter * 3)] << 8) |  buffer[offsetsAt + (randomJumpOffsetBackCounter * 3) + 1]);
						//fprintf(outFile, " #%02X Offset %02X%02X %02X", randomJumpOffsetBackCounter, inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3)], inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3) + 1], inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3) + 2]);

						spot = jumpOffset;
						randomJumpOffsetBackCounter++;
					}
					else
					{
						randomJumpOffsetBackCounter = -1;

						spot += 3;
					}
				}
				else if (command == 0xFD)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
					spot += 3;
				}
				else if (command == 0xFE)
				{
					//fprintf(outFile, " ((Subroutine))");
					//fprintf(outFile, " %02X (%d) %02X (%d) Length %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);

					unsigned short subroutineOffset = ((buffer[spot] << 8) | buffer[spot+1]);
					int subroutineLengthLeft = buffer[spot + 2];
					
					spot += 3;

					subroutineJumpSpots.push_back(spot);
					subroutineEndSpot.push_back(subroutineOffset + subroutineLengthLeft);

					spot = subroutineOffset;
				}
				else if (command == 0xFF)
				{
					//fprintf(outFile, " ((?))");
					//fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", buffer[spot], buffer[spot], buffer[spot+1], buffer[spot+1], buffer[spot+2], buffer[spot+2]);
					spot += 3;
				}
				else
				{
					//fprintf(outFile, " UNKNOWN\n");
					return;
				}
			}
		}

		//fprintf(outFile, "\n");
	}

	
	// Add to end
	for (int x = 0; x < trackOutputNotes.size(); x++)
	{
		outputNotes.push_back(trackOutputNotes[x]);
	}

	absoluteTimeStart = absoluteTime;
}

void CMidiParse::PaperMarioToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra)
{
	CString filepath = midiFile;
	
	FILE* inFile = fopen(filepath, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Can't read input file " + filepath, "Error", NULL);
		return;
	}

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	rewind(inFile);

	unsigned char* inputMID = new unsigned char[inputSize];

	fread(inputMID, 1, inputSize, inFile);
	fclose(inFile);

	PaperMarioToDebugTextFile(ROM, romSize, gameName, address, inputMID, inputSize, textFileOut, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, extra);

	delete [] inputMID;
}

void CMidiParse::PaperMarioToDebugTextFile(unsigned char* ROM, int romSize, CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra)
{
	CString addressStr;
	addressStr.Format("%08X", address);

	FILE* outFile = fopen(textFileOut, "w");
	if (outFile == NULL)
	{
		MessageBox(NULL,"Can't open output file " + textFileOut, "Error", NULL);
		return;
	}

	fprintf(outFile, gameName + " - " + addressStr + "\n");

	CString bgmName = ((char*)&inputMID[8]);
	fprintf(outFile, "Header %02X %02X - Name %s\n", inputMID[6], inputMID[7], bgmName);

	int numberSegments = inputMID[0x10];

	fprintf(outFile, "# Segments %02X\n", numberSegments);
	fprintf(outFile, "\n");

	unsigned long drumDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (0 * 2)]) << 2;
	int numberDrums = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (1 * 2)]);
	unsigned long instrumentDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (2 * 2)]) << 2;
	int numberInstruments = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (3 * 2)]);

	for (int x = 0; x < numberDrums; x++)
	{
		unsigned long tempOffset = drumDataPointer + (x * 0xC);
		fprintf(outFile, "Drum #%02X: Bank Start %02X Instrument %02X Coarse Tune %02X Fine Tune %02X Volume %02X Pan %02X Reverb %02X %02X %02X %02X %02X %02X\n", x, inputMID[tempOffset+0], inputMID[tempOffset+1], inputMID[tempOffset+2], inputMID[tempOffset+3], inputMID[tempOffset+4], inputMID[tempOffset+5], inputMID[tempOffset+6], inputMID[tempOffset+7], inputMID[tempOffset+8], inputMID[tempOffset+9], inputMID[tempOffset+0xA], inputMID[tempOffset+0xB]);
	}

	fprintf(outFile, "\n");

	for (int x = 0; x < numberInstruments; x++)
	{
		unsigned long tempOffset = instrumentDataPointer + (x * 0x8);
		fprintf(outFile, "Instrument #%02X: Bank Start %02X Instrument %02X Volume %02X Pan %02X Reverb %02X Coarse Tune %02X Fine Tune %02X %02X\n", x, inputMID[tempOffset+0], inputMID[tempOffset+1], inputMID[tempOffset+2], inputMID[tempOffset+3], inputMID[tempOffset+4], inputMID[tempOffset+5], inputMID[tempOffset+6], inputMID[tempOffset+7]);
	}

	fprintf(outFile, "\n");

	for (int x = 0; x < numberSegments; x++)
	{
		unsigned long segmentDataPointer = CharArrayToShort(&inputMID[0x14 + (x * 2)]) << 2;

		if (segmentDataPointer == 0x0000)
			continue;

		fprintf(outFile, "-----------------------------------------------------------\n");
		fprintf(outFile, "Segment #%02X Data Offset %08X\n", x, segmentDataPointer);
		fprintf(outFile, "-----------------------------------------------------------\n");
		fprintf(outFile, "\n");

		unsigned long segmentTempDataPointer = segmentDataPointer;
		int segmentCounter = 0;

		while (true)
		{
			if ((inputMID[segmentTempDataPointer] & 0x70) == 0x10)
			{
				unsigned long segmentDataOffset = segmentDataPointer + (CharArrayToShort(&inputMID[segmentTempDataPointer + 2]) << 2);
		
				fprintf(outFile, "\n**********************************************\n");
				fprintf(outFile, "Sub-Segment #%02X: Data Offset %08X\n", segmentCounter, segmentDataOffset);
				fprintf(outFile, "**********************************************\n");

				for (int trackNumber = 0; trackNumber < 0x10; trackNumber++)
				{
					if (CharArrayToLong(&inputMID[segmentDataOffset + (trackNumber * 4)]) != 0x00000000)
					{
						unsigned long trackDataOffset = segmentDataOffset + (CharArrayToShort(&inputMID[segmentDataOffset + (trackNumber * 4)]));
						unsigned short trackFlags = (CharArrayToShort(&inputMID[segmentDataOffset + (trackNumber * 4 + 2)]));
						CString extraTrackFlagsInfo;
						if (trackFlags & 0x80)
							extraTrackFlagsInfo + " Drum Track";
						fprintf(outFile, "\nTrack #%02X: Data Offset %08X Track Flags %04X%s\n", trackNumber, trackDataOffset, trackFlags, extraTrackFlagsInfo);

						PaperMarioTrackToDebugTextFile(outFile, inputMID, trackDataOffset, trackFlags);
					}
				}
			}
			else if ((inputMID[segmentTempDataPointer] & 0x70) == 0x00)
			{
				break;
			}

			segmentTempDataPointer += 4;
			segmentCounter++;
		}

		fprintf(outFile, "\n\n");
	}

	fclose(outFile);
}

void CMidiParse::PaperMarioTrackToDebugTextFile(FILE* outFile, unsigned char* inputMID, unsigned long offset, unsigned short trackFlags)
{
	bool isDrumTrack = ((trackFlags & 0x80) == 0x80);
	unsigned char command = 0xFF;
	unsigned long spot = offset;

	unsigned long absoluteTime = 0;

	std::vector<int> subroutineJumpSpots;
	std::vector<int> subroutineEndSpot;

	unsigned long randomJumpBackSpot = 0;
	int randomJumpOffsetBackCounter = -1;
	unsigned long absoluteTimeBeforeRandomJump = 0;

	int numberSets = 0;

	while (command != 0x00)
	{
		if (subroutineJumpSpots.size() > 0)
		{
			if (subroutineEndSpot[subroutineEndSpot.size()-1] == spot)
			{
				fprintf(outFile, "...((SubroutineEnd, going back to %04X)) \n", subroutineJumpSpots[subroutineJumpSpots.size()-1]);

				spot = subroutineJumpSpots[subroutineJumpSpots.size()-1];

				subroutineJumpSpots.pop_back();
				subroutineEndSpot.pop_back();
			}
		}

		command = inputMID[spot];
		fprintf(outFile, "%08X Time: %08X (%d)", spot, absoluteTime, absoluteTime);

		spot++;

		if (command < 0x80)
		{
			if (command == 0x00)
			{
				if (randomJumpOffsetBackCounter == -1)
				{
					fprintf(outFile, " 00 ((End)) \n");
					break;
				}
				else
				{
					command = 0xFC;
					fprintf(outFile, "...((Random Jump End, going back to %04X)) \n", randomJumpBackSpot);

					spot = randomJumpBackSpot;

					if (randomJumpOffsetBackCounter < numberSets)
					{
						absoluteTime = absoluteTimeBeforeRandomJump;
					}
				}
			}
			else if (command < 0x78)
			{
				fprintf(outFile, " ((Delay))");
				fprintf(outFile, " %02X (%d)", command, command);

				absoluteTime += command;
			}
			else
			{
				unsigned long value = 0x78 + inputMID[spot] + ((command & 0x7) << 8);

				fprintf(outFile, " ((Delay))");
				fprintf(outFile, " %02X (%d) %02X - %04X (%d)", command, command, inputMID[spot], value, value);
				spot++;

				absoluteTime += value;
			}
		}
		else
		{
			// 0x80 to 0xD4
			if (command < 0xD4)
			{
				if (!isDrumTrack)
				{
					fprintf(outFile, " Note:   ");
				}
				else
				{
					fprintf(outFile, " Drum:   ");
				}

				
				unsigned char note = command & 0x7F;
				unsigned char velocity = inputMID[spot];
				
				unsigned long length;
				if (inputMID[spot+1] < 0xC0)
				{
					length = inputMID[spot+1];

					fprintf(outFile, " %02X (%d) [Really %02X (%0d)] Velocity %02X (%d) Length %02X (%d)", note, note, command, command, inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);

					spot += 2;
				}
				else
				{
					length = 0xC0 + ((inputMID[spot+1] & 0x3F) << 8) | inputMID[spot+2];

					fprintf(outFile, " %02X (%d) [Really %02X (%0d)] Velocity %02X (%d) Length %02X%02X - Means %04X (%d)", note, note, command, command, inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+2], length, length);

					spot += 3;
				}

				if (isDrumTrack)
				{
					if (note < 0x48)
					{
						fprintf(outFile, " Percussion Bank # %02X ", note);
					}
					else
					{
						fprintf(outFile, " Drum Bank Instrument # %02X ", note - 0x48);
					}
				}
			}
			else
			{
				fprintf(outFile, " Command: %02X ", command);
				if (command == 0xE0)
				{
					fprintf(outFile, " ((Master Tempo))");
					fprintf(outFile, " %04X (%d)", CharArrayToShort(&inputMID[spot]), CharArrayToShort(&inputMID[spot]));
					spot += 2;
				}
				else if (command == 0xE1)
				{
					fprintf(outFile, " ((Master Volume))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xE2)
				{
					fprintf(outFile, " ((Master Transpose))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], (signed char)inputMID[spot]);
					spot++;
				}
				else if (command == 0xE3)
				{
					fprintf(outFile, " ((?))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xE4)
				{
					fprintf(outFile, " ((Master Change Tempo))");
					fprintf(outFile, " Delay %02X%02X (%d) Tempo %02X%02X (%d)", inputMID[spot], inputMID[spot+1], ((inputMID[spot] << 8) | inputMID[spot+1]), inputMID[spot+2], inputMID[spot+3], ((inputMID[spot+2] << 8) | inputMID[spot+3]));
					spot += 4;
				}
				else if (command == 0xE5)
				{
					fprintf(outFile, " ((Master Volume Fade In))");
					fprintf(outFile, " Delay %02X%02X (%d) Volume %02X (%d)", inputMID[spot], inputMID[spot+1], ((inputMID[spot] << 8) | inputMID[spot+1]), inputMID[spot+2], inputMID[spot+2]);
					spot += 3;
				}
				else if (command == 0xE6)
				{
					fprintf(outFile, " ((Master Effect))");
					fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
					spot += 2;
				}
				else if (command == 0xE7)
				{
					fprintf(outFile, " ((?))");
				}
				else if (command == 0xE8)
				{
					fprintf(outFile, " ((Load Instrument))");
					fprintf(outFile, " Bank Type %02X (%d) Instrument %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
					spot += 2;
				}
				else if (command == 0xE9)
				{
					fprintf(outFile, " ((Track Volume))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xEA)
				{
					fprintf(outFile, " ((Track Pan))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xEB)
				{
					fprintf(outFile, " ((Track Reverb))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xEC)
				{
					fprintf(outFile, " ((Segment Track Volume))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xED)
				{
					fprintf(outFile, " ((Coarse Tune))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xEE)
				{
					fprintf(outFile, " ((Fine Tune))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xEF)
				{
					fprintf(outFile, " ((Coarse/Fine Tune Track))");
					fprintf(outFile, " Coarse %02X (%d) Fine %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
					spot += 2;
				}
				else if (command == 0xF0)
				{
					fprintf(outFile, " ((Tremolo))");
					fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
					spot += 3;
				}
				else if (command == 0xF1)
				{
					fprintf(outFile, " ((?))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xF2)
				{
					fprintf(outFile, " ((?))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xF3)
				{
					fprintf(outFile, " ((Stop Tremolo))");
				}
				else if (command == 0xF4)
				{
					fprintf(outFile, " ((?))");
					fprintf(outFile, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
					spot += 2;
				}
				else if (command == 0xF5)
				{
					fprintf(outFile, " ((Instrument))");
					fprintf(outFile, " Instrument %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xF6)
				{
					fprintf(outFile, " ((Fade out/in volume of track))");
					fprintf(outFile, " Delay %02X%02X (%d) Volume %02X (%d)", inputMID[spot], inputMID[spot+1], ((inputMID[spot] << 8) | inputMID[spot+1]), inputMID[spot+2], inputMID[spot+2]);
					spot += 3;
				}
				else if (command == 0xF7)
				{
					fprintf(outFile, " ((Track Reverb Type))");
					fprintf(outFile, " %02X (%d)", inputMID[spot], inputMID[spot]);
					spot++;
				}
				else if (command == 0xF8)
				{
					fprintf(outFile, " ((?))");
				}
				else if (command == 0xF9)
				{
					fprintf(outFile, " ((?))");
				}
				else if (command == 0xFA)
				{
					fprintf(outFile, " ((?))");
				}
				else if (command == 0xFB)
				{
					fprintf(outFile, " ((?))");
				}
				else if (command == 0xFC)
				{
					unsigned short offsetsAt = ((inputMID[spot] << 8) | inputMID[spot+1]);
					numberSets = inputMID[spot+2];

					if (randomJumpBackSpot != (spot - 1))
					{
						absoluteTimeBeforeRandomJump = absoluteTime;
						randomJumpBackSpot = spot - 1;
						randomJumpOffsetBackCounter = 0;
					}

					
					
					if (randomJumpOffsetBackCounter <  numberSets)
					{
						fprintf(outFile, " ((Jump from set))");
						fprintf(outFile, " Offset %02X%02X # Random Offsets %02X (%d) Entry #%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2], randomJumpOffsetBackCounter);

						unsigned short jumpOffset = ((inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3)] << 8) |  inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3) + 1]);
						fprintf(outFile, " #%02X Offset %02X%02X %02X", randomJumpOffsetBackCounter, inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3)], inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3) + 1], inputMID[offsetsAt + (randomJumpOffsetBackCounter * 3) + 2]);

						spot = jumpOffset;
						randomJumpOffsetBackCounter++;
					}
					else
					{
						randomJumpOffsetBackCounter = -1;

						spot += 3;
					}
				}
				else if (command == 0xFD)
				{
					fprintf(outFile, " ((?))");
					fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
					spot += 3;
				}
				else if (command == 0xFE)
				{
					fprintf(outFile, " ((Subroutine))");
					fprintf(outFile, " Offset %02X%02X Length %02X (%d)", inputMID[spot], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);

					unsigned short subroutineOffset = ((inputMID[spot] << 8) | inputMID[spot+1]);
					int subroutineLengthLeft = inputMID[spot + 2];
					
					spot += 3;

					subroutineJumpSpots.push_back(spot);
					subroutineEndSpot.push_back(subroutineOffset + subroutineLengthLeft);

					spot = subroutineOffset;
				}
				else if (command == 0xFF)
				{
					fprintf(outFile, " ((?))");
					fprintf(outFile, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
					spot += 3;
				}
				else
				{
					fprintf(outFile, " UNKNOWN\n");
					return;
				}
			}
		}

		fprintf(outFile, "\n");
	}
}

void CMidiParse::WritePaperMarioDelay(unsigned long delay, unsigned char* outputBuffer, int& outputPosition)
{
	while (delay > 0)
	{
		if (delay < 0x78)
		{
			outputBuffer[outputPosition++] = delay;
			delay = 0;
		}
		else
		{
			delay = delay - 0x78;
			unsigned long maskLowExtra = delay >> 8;

			if (maskLowExtra > 7)
				maskLowExtra = 7;

			outputBuffer[outputPosition++] = 0x78 | maskLowExtra;

			delay = delay - (maskLowExtra << 8);

			unsigned long extraByte = 0;
			if (delay > 0)
			{
				if (delay > 0x78)
					extraByte = 0x78;
				else
					extraByte = delay;
			}

			outputBuffer[outputPosition++] = extraByte;

			delay = delay - extraByte;
		}
	}
}

bool CMidiParse::MidiToPaperMario(CString input[4], CString output, bool loop, unsigned long& loopPoint, unsigned long name)
{
	SngSegmentInfoPaperMario sngSegments[4];
	for (int segmentNumber = 0; segmentNumber < 4; segmentNumber++)
	{
		if (input[segmentNumber] == "")
			continue;

		std::vector<TimeAndValue> tempoPositions;
		std::vector<SngNoteInfoMidiImport> channels[0x10];
		int numChannels = 0;
		std::vector<int> instruments;
		int lowestAbsoluteTime = 0x7FFFFFFF;
		int highestAbsoluteTime = 0;

		unsigned short division = 0x30;
		if (!MidiToPaperMarioSngList(input[segmentNumber], tempoPositions, channels, numChannels, instruments, lowestAbsoluteTime, highestAbsoluteTime, loop, loopPoint, division))
			return false;

		std::vector<int> subSegments;

		for (int trackNum = 0; trackNum < 0x10; trackNum++)
		{
			for (int x = 0; x < channels[trackNum].size(); x++)
			{
				if (std::find(subSegments.begin(), subSegments.end(), channels[trackNum][x].segmentNumber) == subSegments.end())
				{
					subSegments.push_back(channels[trackNum][x].segmentNumber);
				}
			}
		}

		float noteTimeDivisor = division / 0x30;

		unsigned long loopPointReal = (float)loopPoint / noteTimeDivisor;

		// Renumber segments
		for (int trackNum = 0; trackNum < 0x10; trackNum++)
		{
			for (int x = 0; x < channels[trackNum].size(); x++)
			{
				int oldSegment = channels[trackNum][x].segmentNumber;

				bool notFoundSeg = false;
				for (int s = 0; s < subSegments.size(); s++)
				{
					if (oldSegment == subSegments[s])
					{
						int newSegmentNumber = s;
						if (loop && (channels[trackNum][x].startAbsoluteTime >= loopPointReal))
							newSegmentNumber++;

						channels[trackNum][x].segmentNumber = newSegmentNumber;
						notFoundSeg = true;
						break;
					}
				}

				if (!notFoundSeg)
				{
					// Should never happen
					channels[trackNum][x].segmentNumber = 0;
				}
			}
		}

		subSegments.clear();

		for (int trackNum = 0; trackNum < 0x10; trackNum++)
		{
			for (int x = 0; x < channels[trackNum].size(); x++)
			{
				if (std::find(subSegments.begin(), subSegments.end(), channels[trackNum][x].segmentNumber) == subSegments.end())
				{
					subSegments.push_back(channels[trackNum][x].segmentNumber);
				}
			}
		}


		std::vector<int> subSegmentStarts;
		std::vector<int> subSegmentEnds;
		subSegmentStarts.resize(subSegments.size());
		subSegmentEnds.resize(subSegments.size());

		for (int subSegment = 0; subSegment < subSegments.size(); subSegment++)
		{
			int startSegment = 0x7FFFFFFF;
			int endSegment = 0;
			for (int trackNum = 0; trackNum < 0x10; trackNum++)
			{
				for (int x = 0; x < channels[trackNum].size(); x++)
				{
					if (subSegment == channels[trackNum][x].segmentNumber)
					{
						if (channels[trackNum][x].startAbsoluteTime < startSegment)
							startSegment = channels[trackNum][x].startAbsoluteTime;

						if (channels[trackNum][x].endAbsoluteTime > endSegment)
							endSegment = channels[trackNum][x].endAbsoluteTime;
					}
				}
			}

			if (subSegment == 0)
			{
				subSegmentStarts[subSegment] = 0;
			}
			else
			{
				subSegmentStarts[subSegment] = startSegment;
			}
			subSegmentEnds[subSegment] = endSegment;
		}

		for (int x = 0; x < subSegments.size(); x++)
		{
			for (int y = 0; y < subSegments.size(); y++)
			{
				if (x != y)
				{
					if (IsOverlap(subSegmentStarts[x], subSegmentEnds[x], subSegmentStarts[y], subSegmentEnds[y]))
					{
						int iResults = MessageBox(NULL, "Warning Overlap of sub-segments", "Do you want to continue?", MB_YESNO);

						if (iResults == IDNO)
							return false;
						
						x = subSegments.size();
						y = subSegments.size();
						break;
					}
				}
			}
		}
		
		for (int trackNum = 0; trackNum < 0x10; trackNum++)
		{
			for (int x = 0; x < channels[trackNum].size(); x++)
			{
				sngSegments[segmentNumber].sngSegmentTracks[trackNum].sngNoteList.push_back(channels[trackNum][x]);
			}
		}

		sngSegments[segmentNumber].tempoPositions = tempoPositions;
	}
	std::vector<SngDrumPaperMario> drumsList;
	std::vector<SngInstrumentPaperMario> instrumentsList;

	return MidiToPaperMario(output, drumsList, instrumentsList, sngSegments, name, loop);
}

bool CMidiParse::MidiToPaperMario(CString output, std::vector<SngDrumPaperMario> drums, std::vector<SngInstrumentPaperMario> instruments, SngSegmentInfoPaperMario sngSegments[4], unsigned long name, bool loop)
{
	/*{
	FILE* testoutFile = fopen("C:\\temp\\a.txt", "w");
	for (int x = 1; x < 32151; x++)
	{
		unsigned char* testoutputBuffer = new unsigned char[0x110];
		for (int x = 0; x < 0x110; x++)
			testoutputBuffer[x] = 0;

		int testPos = 0;
		WritePaperMarioDelay(x, testoutputBuffer, testPos);

		fprintf(testoutFile, "%d,", x);
		for (int y = 0; y < testPos; y++)
			fprintf(testoutFile, "%02X", testoutputBuffer[y]);

		fprintf(testoutFile, ",");
		unsigned long value = 0;

		int newPos = 0;
		while (testoutputBuffer[newPos] != 0)
		{
			if (testoutputBuffer[newPos] < 0x78)
			{
				fprintf(testoutFile, " ((Delay))");
				fprintf(testoutFile, " %02X (%d)", testoutputBuffer[newPos], testoutputBuffer[newPos]);
				value += testoutputBuffer[newPos];
				newPos++;
			}
			else
			{
				value += 0x78 + testoutputBuffer[newPos + 1] + ((testoutputBuffer[newPos] & 0x7) << 8);

				fprintf(testoutFile, " ((Delay))");
				fprintf(testoutFile, " %02X (%d) %02X - %04X (%d)", testoutputBuffer[newPos], testoutputBuffer[newPos], testoutputBuffer[newPos + 1], value, value);
				newPos += 2;
			}
		}

		if (value != x)
		{
			value = value;
		}

		fprintf(testoutFile, ",Match %d \n", value);
	}
	fclose(testoutFile);
	}*/

	int outputPosition = 0;

	unsigned char* outputBuffer = new unsigned char[0x100000];

	for (int x = 0; x < 0x100000; x++)
		outputBuffer[x] = 0x00;

	WriteLongToBuffer(outputBuffer, outputPosition, 0x42474D20);
	outputPosition += 4;
	
	int finalSizePosition = outputPosition;

	// Size here
	outputPosition += 4;

	WriteLongToBuffer(outputBuffer, outputPosition, name);
	outputPosition += 4;

	WriteLongToBuffer(outputBuffer, outputPosition, 0x00000000);
	outputPosition += 4;

	int numberSegments = 4;
	outputBuffer[outputPosition] = 0x04;
	outputBuffer[outputPosition + 1] = 0x00;
	outputBuffer[outputPosition + 2] = 0x00;
	outputBuffer[outputPosition + 3] = 0x00;
	outputPosition += 4;

	int segmentOffsetPosition = outputPosition;
	outputPosition += (2 * numberSegments);

	int drumOffsetPosition = outputPosition;
	outputPosition += 2;
	WriteShortToBuffer(outputBuffer, outputPosition, drums.size());
	outputPosition += 2;

	int instrumentOffsetPosition = outputPosition;
	outputPosition += 2;
	WriteShortToBuffer(outputBuffer, outputPosition, instruments.size());
	outputPosition += 2;

	if (drums.size() > 0)
		WriteShortToBuffer(outputBuffer, drumOffsetPosition, (outputPosition >> 2));
	else
		WriteShortToBuffer(outputBuffer, drumOffsetPosition, 0);

	for (int x = 0; x < drums.size(); x++)
	{
		outputBuffer[outputPosition++] = drums[x].flags;
		outputBuffer[outputPosition++] = drums[x].instrument;
		outputBuffer[outputPosition++] = drums[x].unknown2;
		outputBuffer[outputPosition++] = drums[x].unknown3;
		outputBuffer[outputPosition++] = drums[x].volume;
		outputBuffer[outputPosition++] = drums[x].pan;
		outputBuffer[outputPosition++] = drums[x].effect;
		outputBuffer[outputPosition++] = drums[x].unknown7;
		outputBuffer[outputPosition++] = drums[x].unknown8;
		outputBuffer[outputPosition++] = drums[x].unknown9;
		outputBuffer[outputPosition++] = drums[x].unknownA;
		outputBuffer[outputPosition++] = drums[x].unknownB;
	}
	
	if (instruments.size() > 0)
		WriteShortToBuffer(outputBuffer, instrumentOffsetPosition, (outputPosition >> 2));
	else
		WriteShortToBuffer(outputBuffer, instrumentOffsetPosition, 0x0000);

	for (int x = 0; x < instruments.size(); x++)
	{
		outputBuffer[outputPosition++] = instruments[x].flags;
		outputBuffer[outputPosition++] = instruments[x].instrument;
		outputBuffer[outputPosition++] = instruments[x].volume;
		outputBuffer[outputPosition++] = instruments[x].pan;
		outputBuffer[outputPosition++] = instruments[x].effect;
		outputBuffer[outputPosition++] = instruments[x].unknown5;
		outputBuffer[outputPosition++] = instruments[x].unknown6;
		outputBuffer[outputPosition++] = instruments[x].unknown7;
	}

	int segmentOffsetPointers[4];
	std::vector<int> subSegments[4];
	for (int segment = 0; segment < numberSegments; segment++)
	{
		bool hasNote = false;

		for (int trackNumber = 0; trackNumber < 0x10; trackNumber++)
		{
			if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList.size() > 0)
			{
				hasNote = true;
				break;
			}
		}

		if (hasNote)
		{
			WriteShortToBuffer(outputBuffer, segmentOffsetPosition + (segment * 2), (outputPosition >> 2));
			segmentOffsetPointers[segment] = outputPosition;
		}
		else
		{
			WriteShortToBuffer(outputBuffer, segmentOffsetPosition + (segment * 2), 0x0000);
			segmentOffsetPointers[segment] = 0x00000000;
			continue;
		}

		for (int trackNumber = 0; trackNumber < 0x10; trackNumber++)
		{
			for (int y = 0; y < sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList.size(); y++)
			{
				if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].segmentNumber != -1)
				{
					if (std::find(subSegments[segment].begin(), subSegments[segment].end(), sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].segmentNumber) == subSegments[segment].end())
					{
						subSegments[segment].push_back(sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].segmentNumber);
					}
				}
			}
		}

		std::sort(subSegments[segment].begin(), subSegments[segment].end());

		bool didSpecial30 = false;

		if (subSegments[segment].size() > 0)
		{
			for (int x = 0; x < (subSegments[segment][subSegments[segment].size() - 1] + 1); x++)
			{
				// Skip one
				if (std::find(subSegments[segment].begin(), subSegments[segment].end(), x) == subSegments[segment].end())
				{
					WriteLongToBuffer(outputBuffer, outputPosition, 0x30000000);
					didSpecial30 = true;
				}
				outputPosition += 4;
			}
		}

		if (didSpecial30)
		{
			WriteLongToBuffer(outputBuffer, outputPosition, 0x50000000);
			outputPosition += 4;
		}
		// Last 00000000 terminator
		outputPosition += 4;
	}

	for (int segment = 0; segment < numberSegments; segment++)
	{
		if (subSegments[segment].size() > 0)
		{
			bool isAfterLoopSubSegment = false;
			for (int x = 0; x < (subSegments[segment][subSegments[segment].size() - 1] + 1); x++)
			{
				int maxAbsoluteTime = 0;

				if (std::find(subSegments[segment].begin(), subSegments[segment].end(), x) != subSegments[segment].end())
				{
					for (int trackNumber = 0; trackNumber < 0x10; trackNumber++)
					{
						for (int y = 0; y < sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList.size(); y++)
				 		{
							if (x == sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].segmentNumber)
							{
								if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].endAbsoluteTime > maxAbsoluteTime)
								{
									maxAbsoluteTime = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].endAbsoluteTime;
								}
							}
						}
					}

					if (sngSegments[segment].sngSubSegmentInfo.find(x) != sngSegments[segment].sngSubSegmentInfo.end())
					{
						if (maxAbsoluteTime < sngSegments[segment].sngSubSegmentInfo[x].endSegmentTime)
						{
							maxAbsoluteTime = sngSegments[segment].sngSubSegmentInfo[x].endSegmentTime;
						}
					}

					bool isDrumTrack = false;

					WriteLongToBuffer(outputBuffer, segmentOffsetPointers[segment] + (x * 4), 0x10000000 | ((outputPosition - segmentOffsetPointers[segment]) >> 2));

					int trackPointerOffset = outputPosition;

					outputPosition += 0x40;

					for (int trackNumber = 0; trackNumber < 0x10; trackNumber++)
					{
						if ((trackNumber == 0) || (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList.size() > 0))
						{
							// Flags
							//0x0080 Drum Track
							if (isDrumTrack)
							{
								WriteShortToBuffer(outputBuffer, trackPointerOffset + (trackNumber * 0x4), ((outputPosition - trackPointerOffset)));
								// Flags
								WriteShortToBuffer(outputBuffer, trackPointerOffset + (trackNumber * 0x4) + 2, 0xE080);
							}
							else
							{
								WriteShortToBuffer(outputBuffer, trackPointerOffset + (trackNumber * 0x4), ((outputPosition - trackPointerOffset)));
								// Flags
								WriteShortToBuffer(outputBuffer, trackPointerOffset + (trackNumber * 0x4) + 2, 0xA000);
							}
						}
						else
						{
							WriteShortToBuffer(outputBuffer, trackPointerOffset + (trackNumber * 0x4), 0x0000);
							WriteShortToBuffer(outputBuffer, trackPointerOffset + (trackNumber * 0x4) + 2, 0x0000);
							continue;
						}

						int absoluteTime = 0;
						int startAbsoluteTime = 0;

						if (sngSegments[segment].sngSubSegmentInfo.find(x) != sngSegments[segment].sngSubSegmentInfo.end())
						{
							absoluteTime = sngSegments[segment].sngSubSegmentInfo[x].startSegmentTime;
							startAbsoluteTime = sngSegments[segment].sngSubSegmentInfo[x].startSegmentTime;;
						}

						if (trackNumber == 0)
						{
							// Tempo only 
							for (int t = 0; t < sngSegments[segment].tempoPositions.size(); t++)
							{
								if (sngSegments[segment].tempoPositions[t].absoluteTime >= startAbsoluteTime)
								{
									if (sngSegments[segment].tempoPositions[t].absoluteTime >= maxAbsoluteTime)
										break;

									if ((t > 0) && (sngSegments[segment].tempoPositions[t].value == sngSegments[segment].tempoPositions[t - 1].value))
										continue;

									if (sngSegments[segment].tempoPositions[t].absoluteTime > absoluteTime)
									{
										WritePaperMarioDelay((sngSegments[segment].tempoPositions[t].absoluteTime - absoluteTime), outputBuffer, outputPosition);
										absoluteTime = sngSegments[segment].tempoPositions[t].absoluteTime;
									}

									unsigned short tempoValue = sngSegments[segment].tempoPositions[t].value;

									outputBuffer[outputPosition++] = 0xE0;
									WriteShortToBuffer(outputBuffer, outputPosition, tempoValue);
									outputPosition += 2;

									if (t == 0)
									{
										// Write Master Volume
										outputBuffer[outputPosition++] = 0xE1;
										outputBuffer[outputPosition++] = 0x64;

										// Write Effect
										outputBuffer[outputPosition++] = 0xE6;
										WriteShortToBuffer(outputBuffer, outputPosition, 0x0001);
										outputPosition += 2;
									}
								}
							}
						}
						else
						{
							int currentInstrument = -1;
							int currentVolume = -1;
							int currentPan = -1;
							int currentReverb = -1;

							for (int y = 0; y < sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList.size(); y++)
				 			{
								if (x == sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].segmentNumber)
								{
									if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].startAbsoluteTime > absoluteTime)
									{
										WritePaperMarioDelay((sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].startAbsoluteTime - absoluteTime), outputBuffer, outputPosition);
										absoluteTime = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].startAbsoluteTime;
									}

									if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].effect != currentReverb)
									{
										outputBuffer[outputPosition++] = 0xEB;
										outputBuffer[outputPosition++] = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].effect;
										currentReverb = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].effect;
									}

									if (isDrumTrack)
									{
										if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].volume != currentVolume)
										{
											outputBuffer[outputPosition++] = 0xE9;
											outputBuffer[outputPosition++] = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].volume;

											currentVolume = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].volume;
										}

										if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].pan != currentPan)
										{
											outputBuffer[outputPosition++] = 0xEA;
											outputBuffer[outputPosition++] = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].pan;

											currentPan = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].pan;
										}

										// TODO do this
									}
									else
									{
										if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].instrument != currentInstrument)
										{
											bool foundInstrument = false;
											for (int i = 0; i < instruments.size(); i++)
											{
												if (instruments[i].instrument == sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].instrument)
												{
													foundInstrument = true;
													outputBuffer[outputPosition++] = 0xF5;
													outputBuffer[outputPosition++] = instruments[i].instrument;

													currentInstrument = instruments[i].instrument;
													break;
												}
											}

											if (!foundInstrument)
											{
												outputBuffer[outputPosition++] = 0xE8;
												outputBuffer[outputPosition++] = 0x30;
												outputBuffer[outputPosition++] = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].instrument;

												currentInstrument = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].instrument;
											}
										}

										if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].volume != currentVolume)
										{
											outputBuffer[outputPosition++] = 0xE9;
											outputBuffer[outputPosition++] = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].volume;

											currentVolume = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].volume;
										}

										if (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].pan != currentPan)
										{
											outputBuffer[outputPosition++] = 0xEA;
											outputBuffer[outputPosition++] = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].pan;

											currentPan = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].pan;
										}

										unsigned char noteNumber = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].noteNumber - 0xC;
										if (noteNumber > 0x53)
											noteNumber = 0x53;

										unsigned long noteLength = (sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].endAbsoluteTime - sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].startAbsoluteTime);

										int originalLength = noteLength;
										if (noteLength > 0xD3FF)
											noteLength = 0xD3FF;

										outputBuffer[outputPosition++] = noteNumber + 0x80;
										outputBuffer[outputPosition++] = sngSegments[segment].sngSegmentTracks[trackNumber].sngNoteList[y].velocity;
										
										if (noteLength < 0xC0)
										{
											outputBuffer[outputPosition++] = noteLength;
											noteLength = 0;
										}
										else
										{
											noteLength = noteLength - 0xC0;

											unsigned long maskLowExtra = noteLength >> 8;

											if (maskLowExtra > 0x3F)
												maskLowExtra = 0x3F;

											outputBuffer[outputPosition++] = 0xC0 | maskLowExtra;

											noteLength = noteLength - (maskLowExtra << 8);

											unsigned long extraByte = 0;
											if (noteLength > 0)
											{
												if (noteLength > 0xFF)
													extraByte = 0xFF;
												else
													extraByte = noteLength;
											}

											outputBuffer[outputPosition++] = extraByte;

											noteLength = noteLength - extraByte;
										}
									}
								}
							}
						}

						if (absoluteTime < maxAbsoluteTime)
						{
							WritePaperMarioDelay((maxAbsoluteTime - absoluteTime), outputBuffer, outputPosition);
							absoluteTime = maxAbsoluteTime;
						}

						// End
						outputBuffer[outputPosition++] = 0x00;

						if ((outputPosition % 4) != 0)
						{
							int pad = 4 - (outputPosition % 4);
							for (int p = 0; p < pad; p++)
								outputBuffer[outputPosition++] = 0x00;
						}
					}

					isAfterLoopSubSegment = false;

				}
				else
				{
					isAfterLoopSubSegment = true;
				}
			}
		}
	}

	WriteLongToBuffer(outputBuffer, finalSizePosition, outputPosition);

	FILE* outFile = fopen(output, "wb");
	if (outFile == NULL)
	{
		delete [] outputBuffer;

		MessageBox(NULL, "Error outputting file", "Error", NULL);
		return false;
	}

	fwrite(outputBuffer, 1, outputPosition, outFile);

	delete [] outputBuffer;
	fclose(outFile);

	return true;
}

void CMidiParse::TestPaperMarioToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, std::vector<SngInstrumentPaperMario>& usedInstruments, std::vector<SngDrumPaperMario>& usedPercussionSet, std::vector<int>& usedExtraInstruments, bool combineSegments)
{
	numberInstruments = 1;

	int numberSegments = inputMID[0x10];

	unsigned long name = CharArrayToLong(&inputMID[8]);

	unsigned long drumDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (0 * 2)]) << 2;
	int numberDrum = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (1 * 2)]);
	unsigned long instrumentDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (2 * 2)]) << 2;
	int numberInst = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (3 * 2)]);

	// Test Output
	std::vector<SngDrumPaperMario> drums;
	for (int x = 0; x < numberDrum; x++)
	{
		SngDrumPaperMario drumInfo;

		drumInfo.flags = inputMID[drumDataPointer + (x * 0xC) + 0x0];
		drumInfo.instrument = inputMID[drumDataPointer + (x * 0xC) + 0x1];
		drumInfo.unknown2 = inputMID[drumDataPointer + (x * 0xC) + 0x2];
		drumInfo.unknown3 = inputMID[drumDataPointer + (x * 0xC) + 0x3];
		drumInfo.volume = inputMID[drumDataPointer + (x * 0xC) + 0x4];
		drumInfo.pan = inputMID[drumDataPointer + (x * 0xC) + 0x5];
		drumInfo.effect = inputMID[drumDataPointer + (x * 0xC) + 0x6];
		drumInfo.unknown7 = inputMID[drumDataPointer + (x * 0xC) + 0x7];
		drumInfo.unknown8 = inputMID[drumDataPointer + (x * 0xC) + 0x8];
		drumInfo.unknown9 = inputMID[drumDataPointer + (x * 0xC) + 0x9];
		drumInfo.unknownA = inputMID[drumDataPointer + (x * 0xC) + 0xA];
		drumInfo.unknownB = inputMID[drumDataPointer + (x * 0xC) + 0xB];

		drums.push_back(drumInfo);
		usedPercussionSet.push_back(drumInfo);
	}

	std::vector<SngInstrumentPaperMario> instruments;
	for (int x = 0; x < numberInst; x++)
	{
		SngInstrumentPaperMario instrumentInfo;

		instrumentInfo.flags = inputMID[instrumentDataPointer + (x * 0x8) + 0x0];
		instrumentInfo.instrument = inputMID[instrumentDataPointer + (x * 0x8) + 0x1];
		instrumentInfo.volume = inputMID[instrumentDataPointer + (x * 0x8) + 0x2];
		instrumentInfo.pan = inputMID[instrumentDataPointer + (x * 0x8) + 0x3];
		instrumentInfo.effect = inputMID[instrumentDataPointer + (x * 0x8) + 0x4];
		instrumentInfo.unknown5 = inputMID[instrumentDataPointer + (x * 0x8) + 0x5];
		instrumentInfo.unknown6 = inputMID[instrumentDataPointer + (x * 0x8) + 0x6];
		instrumentInfo.unknown7 = inputMID[instrumentDataPointer + (x * 0x8) + 0x7];

		instruments.push_back(instrumentInfo);
		usedInstruments.push_back(instrumentInfo);
	}

	SngSegmentInfoPaperMario sngSegments[4];

	for (int x = 0; x < numberSegments; x++)
	{
		unsigned long segmentDataPointer = CharArrayToShort(&inputMID[0x14 + (x * 2)]) << 2;

		if (segmentDataPointer == 0x0000)
			continue;

		unsigned long segmentTempDataPointer = segmentDataPointer;
		int segmentCounter = 0;

		unsigned char currentPan = 0x40;
		unsigned long currentInstrument = 0x00;
		unsigned char currentVolume = 0x7F;

		int currentEffect = 0;

		unsigned long currentTempo = (unsigned long)(60000000.0 / (float)120.0);

		signed long currentCoarseTune = 0x00;

		bool setReverb = false;
		bool setVolume = false;
		bool setPan = false;

		std::vector<SngNoteInfo> sngNoteList;
		int noteUniqueId = 0;
		std::vector<TimeAndValue> tempoPositions;

		int absoluteTimeStart[0x10];
		for (int a = 0; a < 0x10; a++)
			absoluteTimeStart[a] = 0;

		while (true)
		{
			if ((inputMID[segmentTempDataPointer] & 0x70) == 0x10)
			{
				unsigned long segmentDataOffset = segmentDataPointer + (CharArrayToShort(&inputMID[segmentTempDataPointer + 2]) << 2);

				unsigned long previousTempo = currentTempo;

				sngSegments[x].sngSubSegmentInfo[segmentCounter].startSegmentTime = absoluteTimeStart[0];

				for (int trackNumber = 0; trackNumber < 0x10; trackNumber++)
				{
					if (CharArrayToLong(&inputMID[segmentDataOffset + (trackNumber * 4)]) != 0x00000000)
					{
						unsigned long trackDataOffset = segmentDataOffset + (CharArrayToShort(&inputMID[segmentDataOffset + (trackNumber * 4)]));
						unsigned short trackFlags = (CharArrayToShort(&inputMID[segmentDataOffset + (trackNumber * 4 + 2)]));

						SngSegmentInfoPaperMario* sngSegmentInfo = &(sngSegments[x]);
						
						sngSegmentInfo->sngSubSegmentInfo[segmentCounter].trackFlags = trackFlags;

						SngSegmentTrackInfoPaperMario* sngSegmentTrackInfo = &(sngSegmentInfo->sngSegmentTracks[trackNumber]);
						ParsePaperMarioTrack(ROM, romSize, trackNumber, numberInstruments, tempoPositions, sngSegmentTrackInfo->sngNoteList, inputMID, trackDataOffset, inputSize, noteUniqueId, drumDataPointer, numberDrum, instrumentDataPointer, numberInst, trackFlags, usedExtraInstruments, currentPan, currentInstrument, currentVolume, currentEffect, currentTempo, currentCoarseTune, setReverb, setVolume, setPan, absoluteTimeStart[trackNumber], segmentCounter);
						sngSegmentTrackInfo->sngNoteList = sngSegmentTrackInfo->sngNoteList;
					}
				}

				sngSegments[x].sngSubSegmentInfo[segmentCounter].endSegmentTime = absoluteTimeStart[0];

				if (tempoPositions.size() == 0)
				{
					tempoPositions.push_back(TimeAndValue(0, currentTempo));
				}

				if (tempoPositions[0].absoluteTime != 0)
				{
					tempoPositions.insert(tempoPositions.begin(), TimeAndValue(0, previousTempo));	
				}

				sngSegments[x].tempoPositions = tempoPositions;
			}
			else if ((inputMID[segmentTempDataPointer] & 0x70) == 0x00)
			{
				break;
			}

			segmentTempDataPointer += 4;
			segmentCounter++;
		}
	}

	MidiToPaperMario("C:\\GoldeneyeStuff\\N64Hack\\ROMs\\GoodSet\\papermariooutput.bin", drums, instruments, sngSegments, name, false);
}

void CMidiParse::PaperMarioToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, std::vector<SngInstrumentPaperMario>& usedInstruments, std::vector<SngDrumPaperMario>& usedPercussionSet, std::vector<int>& usedExtraInstruments, bool combineSegments)
{
	numberInstruments = 1;

	int numberSegments = inputMID[0x10];

	unsigned long drumDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (0 * 2)]) << 2;
	int numberDrum = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (1 * 2)]);
	unsigned long instrumentDataPointer = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (2 * 2)]) << 2;
	int numberInst = CharArrayToShort(&inputMID[0x14 + (numberSegments * 2) + (3 * 2)]);

	std::vector<SngDrumPaperMario> drums;
	for (int x = 0; x < numberDrum; x++)
	{
		SngDrumPaperMario drumInfo;

		drumInfo.flags = inputMID[drumDataPointer + (x * 0xC) + 0x0];
		drumInfo.instrument = inputMID[drumDataPointer + (x * 0xC) + 0x1];
		drumInfo.unknown2 = inputMID[drumDataPointer + (x * 0xC) + 0x2];
		drumInfo.unknown3 = inputMID[drumDataPointer + (x * 0xC) + 0x3];
		drumInfo.volume = inputMID[drumDataPointer + (x * 0xC) + 0x4];
		drumInfo.pan = inputMID[drumDataPointer + (x * 0xC) + 0x5];
		drumInfo.effect = inputMID[drumDataPointer + (x * 0xC) + 0x6];
		drumInfo.unknown7 = inputMID[drumDataPointer + (x * 0xC) + 0x7];
		drumInfo.unknown8 = inputMID[drumDataPointer + (x * 0xC) + 0x8];
		drumInfo.unknown9 = inputMID[drumDataPointer + (x * 0xC) + 0x9];
		drumInfo.unknownA = inputMID[drumDataPointer + (x * 0xC) + 0xA];
		drumInfo.unknownB = inputMID[drumDataPointer + (x * 0xC) + 0xB];

		usedPercussionSet.push_back(drumInfo);
	}

	std::vector<SngInstrumentPaperMario> instruments;
	for (int x = 0; x < numberInst; x++)
	{
		SngInstrumentPaperMario instrumentInfo;

		instrumentInfo.flags = inputMID[instrumentDataPointer + (x * 0x8) + 0x0];
		instrumentInfo.instrument = inputMID[instrumentDataPointer + (x * 0x8) + 0x1];
		instrumentInfo.volume = inputMID[instrumentDataPointer + (x * 0x8) + 0x2];
		instrumentInfo.pan = inputMID[instrumentDataPointer + (x * 0x8) + 0x3];
		instrumentInfo.effect = inputMID[instrumentDataPointer + (x * 0x8) + 0x4];
		instrumentInfo.unknown5 = inputMID[instrumentDataPointer + (x * 0x8) + 0x5];
		instrumentInfo.unknown6 = inputMID[instrumentDataPointer + (x * 0x8) + 0x6];
		instrumentInfo.unknown7 = inputMID[instrumentDataPointer + (x * 0x8) + 0x7];

		usedInstruments.push_back(instrumentInfo);
	}

	for (int x = 0; x < numberSegments; x++)
	{
		unsigned long segmentDataPointer = CharArrayToShort(&inputMID[0x14 + (x * 2)]) << 2;

		if (segmentDataPointer == 0x0000)
			continue;

		unsigned long segmentTempDataPointer = segmentDataPointer;
		int segmentCounter = 0;

		unsigned char currentPan = 0x40;
		unsigned long currentInstrument = 0x00;
		unsigned char currentVolume = 0x7F;

		int currentEffect = 0;

		unsigned long currentTempo = (unsigned long)(60000000.0 / (float)120.0);

		signed long currentCoarseTune = 0x00;

		bool setReverb = false;
		bool setVolume = false;
		bool setPan = false;

		std::vector<SngNoteInfo> sngNoteList;
		int noteUniqueId = 0;
		std::vector<TimeAndValue> tempoPositions;

		int absoluteTimeStartTempo = 0;

		while (true)
		{
			if ((inputMID[segmentTempDataPointer] & 0x70) == 0x10)
			{
				unsigned long segmentDataOffset = segmentDataPointer + (CharArrayToShort(&inputMID[segmentTempDataPointer + 2]) << 2);

				if (!combineSegments)
				{
					sngNoteList.clear();
					noteUniqueId = 0;
					tempoPositions.clear();
				}

				unsigned long previousTempo = absoluteTimeStartTempo;

				int currentAbsoluteStart = absoluteTimeStartTempo;
				for (int trackNumber = 0; trackNumber < 0x10; trackNumber++)
				{
					if (CharArrayToLong(&inputMID[segmentDataOffset + (trackNumber * 4)]) != 0x00000000)
					{
						unsigned long trackDataOffset = segmentDataOffset + (CharArrayToShort(&inputMID[segmentDataOffset + (trackNumber * 4)]));
						unsigned short trackFlags = (CharArrayToShort(&inputMID[segmentDataOffset + (trackNumber * 4 + 2)]));
							
						int absoluteTimeStart;
						if (!combineSegments)
							absoluteTimeStart = 0;
						else
							absoluteTimeStart = previousTempo;

						ParsePaperMarioTrack(ROM, romSize, trackNumber, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataOffset, inputSize, noteUniqueId, drumDataPointer, numberDrum, instrumentDataPointer, numberInst, trackFlags, usedExtraInstruments, currentPan, currentInstrument, currentVolume, currentEffect, currentTempo, currentCoarseTune, setReverb, setVolume, setPan, absoluteTimeStart, segmentCounter);

						if (trackNumber == 0)
							absoluteTimeStartTempo = absoluteTimeStart;
					}
				}

				if (tempoPositions.size() == 0)
				{
					tempoPositions.push_back(TimeAndValue(0, currentTempo));
				}

				if (tempoPositions[0].absoluteTime != 0)
				{
					tempoPositions.insert(tempoPositions.begin(), TimeAndValue(0, previousTempo));	
				}

				if (!combineSegments)
				{
					// Adjust for empty master track
					for (int x = 0; x < sngNoteList.size(); x++)
					{
						if (sngNoteList[x].originalTrack > 0)
							sngNoteList[x].originalTrack--;
					}
				}

				if (!combineSegments)
				{
					bool midiName = (outFileName.Find(".midi") != -1);
					CString tempMidiNumber;
					tempMidiNumber.Format("%s_Segment%d_SubSegment%d", outFileName, x, segmentCounter);
					tempMidiNumber.Replace(".midi", "");
					tempMidiNumber.Replace(".mid", "");
					if (midiName)
						tempMidiNumber += ".midi";
					else
						tempMidiNumber += ".mid";

					WriteSngList(sngNoteList, tempoPositions, tempMidiNumber, separateByInstrument, 0x0030, false, 24);
				}
			}
			else if ((inputMID[segmentTempDataPointer] & 0x70) == 0x00)
			{
				break;
			}

			segmentTempDataPointer += 4;
			segmentCounter++;
		}

		if (combineSegments)
		{
			// Adjust for empty master track
			for (int x = 0; x < sngNoteList.size(); x++)
			{
				if (sngNoteList[x].originalTrack > 0)
					sngNoteList[x].originalTrack--;
			}

			bool midiName = (outFileName.Find(".midi") != -1);
			CString tempMidiNumber;
			tempMidiNumber.Format("%s_Segment%d", outFileName, x);
			tempMidiNumber.Replace(".midi", "");
			tempMidiNumber.Replace(".mid", "");
			if (midiName)
				tempMidiNumber += ".midi";
			else
				tempMidiNumber += ".mid";

			WriteSngList(sngNoteList, tempoPositions, tempMidiNumber, separateByInstrument, 0x0030, false, 24);
		}
	}
}

bool CMidiParse::MidiToEADMusicSngList(CString input, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfoMidiImport> channels[0x10], int& numChannels, int& lowestAbsoluteTime, int& highestAbsoluteTime, bool loop, unsigned long loopPoint, unsigned short & division)
{
	numChannels = 0;
	lowestAbsoluteTime = 0x7FFFFFFF;
	highestAbsoluteTime = 0;

	int noteUniqueId = 0;

	numberTracks = 0;

	


	CString tempFileName = input;

	struct stat results;
	stat(tempFileName, &results);		

	FILE* inFile1 = fopen(tempFileName, "rb");
	if (inFile1 == NULL)
	{
		MessageBox(NULL, "Error reading file", "Error", NULL);
		return false;
	}	

	byte* inputMID = new byte[results.st_size];
	fread(inputMID, 1, results.st_size, inFile1);
	fclose(inFile1);

	unsigned long header = CharArrayToLong(&inputMID[0]);

	if (header != 0x4D546864)
	{
		delete [] inputMID;
		MessageBox(NULL, "Invalid midi hdr", "Error", NULL);
		return false;
	}

	unsigned long headerLength = CharArrayToLong(&inputMID[4]);

	unsigned short type = CharArrayToShort(&inputMID[8]);
	unsigned short numTracks = CharArrayToShort(&inputMID[0xA]);
	division = CharArrayToShort(&inputMID[0xC]);

	float noteTimeDivisor = division / 0x30;

	loopPoint = (float)loopPoint / noteTimeDivisor;

	if (type == 0)
	{
		
	}
	else if (type == 1)
	{

	}
	else
	{
		delete [] inputMID;

		MessageBox(NULL, "Invalid midi type", "Error", NULL);
		return false;
	}



	int position = 0xE;

	byte* repeatPattern = NULL;
	byte altOffset = 0;
	byte altLength = 0;

	bool unknownsHit = false;
	for (int trackNum = 0; trackNum < numTracks; trackNum++)
	{
		std::vector<SngNoteInfoMidiImport> pendingNoteList;

		unsigned char currentPan[0x10];
		unsigned char currentVolume[0x10];
		unsigned char currentReverb[0x10];
		signed char currentPitchBend[0x10];
		unsigned char currentVibrato[0x10];

		unsigned char currentMSBBank[0x10];
		unsigned char currentLSBBank[0x10];
		unsigned char currentInstrument[0x10];

		// Controllers defaults
		for (int x = 0; x < 0x10; x++)
		{
			currentPan[x] = 0x40;
			currentVolume[x] = 0x7F;
			currentReverb[x] = 0x00;
			currentInstrument[x] = 0x00;
			currentPitchBend[x] = 0x40;
			currentVibrato[x] = 0x00;

			currentMSBBank[x] = 0x00;
			currentLSBBank[x] = 0x00;
		}

		unsigned long absoluteTime = 0;
		float absoluteTimeFloat = 0;

		unsigned long trackHeader = ((((((inputMID[position] << 8) | inputMID[position+1]) << 8) | inputMID[position+2]) << 8) | inputMID[position+3]);
		if (trackHeader != 0x4D54726B)
		{
			delete [] inputMID;

			MessageBox(NULL, "Invalid track midi hdr", "Error", NULL);
			return false;
		}
		
		unsigned long trackLength = ((((((inputMID[position+4] << 8) | inputMID[position+5]) << 8) | inputMID[position+6]) << 8) | inputMID[position+7]);

		position += 8;

		byte previousEventValue = 0xFF;

		bool endFlag = false;

		while (!endFlag && (position < results.st_size))
		{
			unsigned long original;
			unsigned long timeTag = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
			absoluteTimeFloat += (float)timeTag / noteTimeDivisor;
			
			absoluteTime = absoluteTimeFloat;

			byte eventVal = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

			bool statusBit = false;

			if (eventVal <= 0x7F)
			{
				// continuation
				statusBit = true;
			}
			else
			{
				statusBit = false;
			}

			if (eventVal == 0xFF) // meta event
			{
				byte subType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (subType == 0x2F) //End of Track Event.
				{
					endFlag = true;

					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);  // end 00 in real mid
				}
				else if (subType == 0x51) //Set Tempo Event.
				{
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					unsigned char byteData[3];
					byteData[0] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byteData[1] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					byteData[2] = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

					unsigned long microSecondsSinceQuarterNote = ((((byteData[0] << 8) | byteData[1]) << 8) | byteData[2]);
					unsigned long tempTempo = 60000000.0 / microSecondsSinceQuarterNote;

					if (tempTempo > 255)
						tempTempo = 255;
					else if (tempTempo < 1)
						tempTempo = 1;
					else
						tempTempo = (unsigned char)tempTempo;

					bool matchTempo = false;
					for (int y = 0; y < tempoPositions.size(); y++)
					{
						if (tempoPositions[y].absoluteTime == absoluteTime)
						{
							matchTempo = true;
						}
					}

					if (!matchTempo)
					{
						tempoPositions.push_back(TimeAndValue(absoluteTime, tempTempo));
					}
				}
				//Various Unused Meta Events.
				else if ((subType < 0x7F) && !(subType == 0x51 || subType == 0x2F))
				{
					//newTrackEvent->type = 0xFF;
					unsigned long length = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false); 

					for (int i = 0; i < length; i++)
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
				else if (subType == 0x7F) //Unused Sequencer Specific Event.
				{
					//newTrackEvent->type = 0xFF;
					int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
					// subtract length
					for (int i = 0; i < length; i++)
					{
						ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					}
				}

				previousEventValue = eventVal;
			}
			// Note off
			else if ((eventVal >= 0x80 && eventVal < 0x90) || (statusBit && (previousEventValue >= 0x80 && previousEventValue < 0x90)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				int controller = (curEventVal & 0xF);

				for (int p = 0; p < pendingNoteList.size(); p++)
				{
					if (pendingNoteList[p].originalController != (controller))
						continue;

					// Go backwards in list
					if (pendingNoteList[p].noteNumber == noteNumber)
					{
						pendingNoteList[p].endAbsoluteTime = absoluteTime;

						// Promote to regular
						channels[controller].push_back(pendingNoteList[p]);

						pendingNoteList.erase(pendingNoteList.begin() + p);
						break;
					}
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if ((eventVal >= 0x90 && eventVal < 0xA0) || (statusBit && (previousEventValue >= 0x90 && previousEventValue < 0xA0)))
			{
				byte curEventVal;

				byte noteNumber;
				if (statusBit)
				{
					noteNumber = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					noteNumber = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}
				byte velocity = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				int controller = (curEventVal & 0xF);

				if (velocity == 0)
				{
					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller))
							continue;

						// Go backwards in list
						if (pendingNoteList[p].noteNumber == noteNumber)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;

							// Promote to regular
							channels[controller].push_back(pendingNoteList[p]);

							pendingNoteList.erase(pendingNoteList.begin() + p);
							break;
						}
					}
				}
				else
				{
					// If wasn't shut off, turn it off from before, then start new note
					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller))
							continue;

						// Go backwards in list
						if (pendingNoteList[p].noteNumber == noteNumber)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;

							// Promote to regular
							channels[controller].push_back(pendingNoteList[p]);

							pendingNoteList.erase(pendingNoteList.begin() + p);
							break;
						}
					}

					SngNoteInfoMidiImport newSongInfo;
					newSongInfo.originalController = controller;
					newSongInfo.originalTrack = trackNum;
					newSongInfo.originalNoteUniqueId = noteUniqueId++;
					newSongInfo.noteNumber = noteNumber;
					newSongInfo.velocity = velocity;
					// Apply tempo later as master track
					//newSongInfo.tempo = currentTempo;
					newSongInfo.pan = currentPan[controller];
					newSongInfo.volume = currentVolume[controller];
					newSongInfo.vibrato = currentVibrato[controller];
					newSongInfo.effect = currentReverb[controller];
					newSongInfo.instrument = currentInstrument[controller] + (currentLSBBank[controller] * 0x80) + (currentMSBBank[controller] * 0x8000);
					newSongInfo.pitchBend = currentPitchBend[controller];
					newSongInfo.startAbsoluteTime = absoluteTime;
					pendingNoteList.push_back(newSongInfo);
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xB0) && (eventVal < 0xC0))  || (statusBit && (previousEventValue >= 0xB0 && previousEventValue < 0xC0))) // controller change
			{
				byte controllerType;
				unsigned char curEventVal;

				if (statusBit)
				{
					controllerType = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					controllerType = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				int controller = (curEventVal & 0xF);

				byte controllerValue = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				if (controllerType == 0) // MSB Instrument Bank
				{
					currentMSBBank[controller] = controllerValue;	
				}
				else if (controllerType == 1) // Mod Wheel (Vibrato)
				{
					if (controllerValue != currentVibrato[controller])
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								channels[controller].push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].vibrato = controllerValue;
						}
					}

					currentVibrato[controller] = controllerValue;	
				}
				else if (controllerType == 7) // Volume
				{
					if (controllerValue != currentVolume[controller])
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								channels[controller].push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].volume = controllerValue;
						}
					}

					currentVolume[controller] = controllerValue;	
				}
				else if (controllerType == 10) // Pan
				{
					if (controllerValue != currentPan[controller])
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								channels[controller].push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].pan = controllerValue;
						}
					}

					currentPan[controller] = controllerValue;
				}
				else if (controllerType == 32) // LSB Instrument Bank
				{
					currentLSBBank[controller] = controllerValue;	
				}
				else if (controllerType == 91) // Reverb
				{
					if (controllerValue != currentReverb[controller])
					{
						for (int p = 0; p < pendingNoteList.size(); p++)
						{
							if (pendingNoteList[p].originalController != (controller))
								continue;

							// Reopen
							if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
							{
								pendingNoteList[p].endAbsoluteTime = absoluteTime;
					
								// Promote to regular
								channels[controller].push_back(pendingNoteList[p]);

								// Reset
								pendingNoteList[p].startAbsoluteTime = absoluteTime;
								pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
							}
							
							pendingNoteList[p].effect = controllerValue;
						}
					}

					currentReverb[controller] = controllerValue;
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xC0) && (eventVal < 0xD0)) || (statusBit && (previousEventValue >= 0xC0 && previousEventValue < 0xD0))) // change instrument
			{
				byte instrument;
				unsigned char curEventVal;

				if (statusBit)
				{
					instrument = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					instrument = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				if ((eventVal & 0xF) == 9) // Drums in GM
					instrument = instrument;
				else
					instrument = instrument;

				int controller = (curEventVal & 0xF);

				unsigned short tempInstrument = instrument + (currentLSBBank[controller] * 0x80) + (currentMSBBank[controller] * 0x8000);
				
				for (int p = 0; p < pendingNoteList.size(); p++)
				{
					if (pendingNoteList[p].originalController != (controller))
						continue;

					if (pendingNoteList[p].instrument != tempInstrument)
					{
						// Reopen
						if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;
				
							// Promote to regular
							channels[controller].push_back(pendingNoteList[p]);

							// Reset
							pendingNoteList[p].startAbsoluteTime = absoluteTime;
							pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
						}

						pendingNoteList[p].instrument = tempInstrument;
					}
				}

				currentInstrument[controller] = instrument;
				
				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (((eventVal >= 0xD0) && (eventVal < 0xE0))  || (statusBit && (previousEventValue >= 0xD0 && previousEventValue < 0xE0))) // channel aftertouch
			{
				unsigned char curEventVal;
				byte amount;
				if (statusBit)
				{
					amount = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					amount = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				if (!statusBit)
					previousEventValue = eventVal;
			}
			// Pitch Bend
			else if (((eventVal >= 0xE0) && (eventVal < 0xF0))  || (statusBit && (previousEventValue >= 0xE0 && previousEventValue < 0xF0))) // pitch bend
			{
				byte valueLSB;

				unsigned char curEventVal;
				if (statusBit)
				{
					valueLSB = eventVal;
					curEventVal = previousEventValue;
				}
				else
				{
					valueLSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
					curEventVal = eventVal;
				}

				byte valueMSB = ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);

				int controller = (curEventVal & 0xF);

				if (currentPitchBend[controller] != valueMSB)
				{
					for (int p = 0; p < pendingNoteList.size(); p++)
					{
						if (pendingNoteList[p].originalController != (controller))
							continue;

						// Reopen
						if (pendingNoteList[p].startAbsoluteTime != absoluteTime)
						{
							pendingNoteList[p].endAbsoluteTime = absoluteTime;
				
							// Promote to regular
							channels[controller].push_back(pendingNoteList[p]);

							// Reset
							pendingNoteList[p].startAbsoluteTime = absoluteTime;
							pendingNoteList[p].endAbsoluteTime = 0xFFFFFFFF;
						}
						
						pendingNoteList[p].pitchBend = valueMSB;
					}
				}

				currentPitchBend[controller] = valueMSB;

				if (!statusBit)
					previousEventValue = eventVal;
			}
			else if (eventVal == 0xF0 || eventVal == 0xF7)
			{
				unsigned char curEventVal = eventVal;
				int length = GetVLBytes(inputMID, position, original, repeatPattern, altOffset, altLength, false);
				// subtract length
				for (int i = 0; i < length; i++)
				{
					ReadMidiByte(inputMID, position, repeatPattern, altOffset, altLength, false);
				}
			}
			else
			{
				if (!unknownsHit)
				{
					MessageBox(NULL, "Invalid midi character found", "Error", NULL);
					unknownsHit = true;
				}
			}
		}

		for (int p = 0; p < pendingNoteList.size(); p++)
		{
			pendingNoteList[p].endAbsoluteTime = absoluteTime;
			channels[pendingNoteList[p].originalController].push_back(pendingNoteList[p]);
		}
	}

	numChannels = 0x10;

	for (int y = 0; y < numChannels; y++)
	{
		// Clear empty notes
		for (int x = (channels[y].size() - 1); x >= 0; x--)
		{
			if (channels[y][x].startAbsoluteTime == channels[y][x].endAbsoluteTime)
				channels[y].erase(channels[y].begin() + x);
		}

		for (int x = 0; x < channels[y].size(); x++)
		{
			SngNoteInfoMidiImport tempNoteInfo = channels[y][x];

			if (tempNoteInfo.endAbsoluteTime > highestAbsoluteTime)
				highestAbsoluteTime = tempNoteInfo.endAbsoluteTime;

			if (tempNoteInfo.startAbsoluteTime < lowestAbsoluteTime)
				lowestAbsoluteTime = tempNoteInfo.startAbsoluteTime;
		}
	}
	
	delete [] inputMID;







	//FILE* outDebug = fopen("C:\\GoldeneyeStuff\\GE Editor Source\\debug.txt", "w");
	FILE* outDebug = NULL;

	if (outDebug != NULL)
	{
		for (int channel = 0; channel < numChannels; channel++)
		{
			for (int x = 0; x < channels[channel].size(); x++)
			{
				fprintf(outDebug, "Start %08X End %08X Instrument %02X Note %02X Volume %02X Vibrato %02X Pitch Bend %02X Pan %02X Velocity %02X\n", channels[channel][x].startAbsoluteTime, channels[channel][x].endAbsoluteTime, channels[channel][x].instrument, channels[channel][x].noteNumber, channels[channel][x].volume, channels[channel][x].vibrato, channels[channel][x].pitchBend, channels[channel][x].pan, channels[channel][x].velocity);
			}
		}
	}

	if (outDebug != NULL)
		fclose(outDebug);

	

	if (loop && (loopPoint > highestAbsoluteTime))
	{

		MessageBox(NULL, "Error, loop point is beyond end of midi", "Error", NULL);
		return false;
	}

	std::sort(tempoPositions.begin(), tempoPositions.end(), timeAndValueSortByTime());

	// Weed out equal tempos
	for (int t = 0; t < tempoPositions.size(); t++)
	{
		if ((t + 1) < tempoPositions.size())
		{
			if (tempoPositions[t].value == tempoPositions[t+1].value)
			{
				tempoPositions.erase(tempoPositions.begin() + t + 1);
				t--;
			}
		}
	}

	for (int x = 0; x < numChannels; x++)
	{
		// Separate
		if (loop && (loopPoint != 0))
		{
			for (int y = (channels[x].size() - 1); y >= 0; y--)
			{
				SngNoteInfoMidiImport noteMidiImport = channels[x][y];
				if ((loopPoint > noteMidiImport.startAbsoluteTime) && (loopPoint < noteMidiImport.endAbsoluteTime))
				{
					// Need to split
					channels[x][y].endAbsoluteTime = loopPoint;

					noteMidiImport.startAbsoluteTime = loopPoint;
					channels[x].push_back(noteMidiImport);

				}
			}
		}

		std::sort(channels[x].begin(), channels[x].end(), sngSortByStartTime());
	}

	return true;
}

bool CMidiParse::MidiToEADMusic(int gameStyle, CString input, CString output, bool loop, unsigned long loopPoint)
{
	try
	{
		std::vector<TimeAndValue> tempoPositions;
		std::vector<SngNoteInfoMidiImport> channels[0x10];
		int numChannels = 0;
		int lowestAbsoluteTime = 0x7FFFFFFF;
		int highestAbsoluteTime = 0;

		unsigned short division = 0x30;
		if (!MidiToEADMusicSngList(input, tempoPositions, channels, numChannels, lowestAbsoluteTime, highestAbsoluteTime, loop, loopPoint, division))
			return false;

		for (int x = 0; x < numChannels; x++)
		{
			for (int y = 0; y < channels[x].size(); y++)
			{
				// Max in Zelda is FF
				channels[x][y].effect *= 2;

				// Fix Note Range
				if (channels[x][y].noteNumber <= 0x15)
					channels[x][y].noteNumber = 0x00;
				else if (channels[x][y].noteNumber >= 0x54)
					channels[x][y].noteNumber = 0x3F;
				else
					channels[x][y].noteNumber = channels[x][y].noteNumber - 0x15;

				// Adjust Pitch Bend from 00-7F to Signed Char
				channels[x][y].pitchBend = ((signed char)channels[x][y].pitchBend - 0x40) * 2;
				if (channels[x][y].pitchBend < 0)
					channels[x][y].pitchBend = 0;
				if (channels[x][y].pitchBend > 0x7F)
					channels[x][y].pitchBend = 0x7F;
			}
		}
		

		for (int x = 0; x < numChannels; x++)
		{
			for (int y = 0; y < channels[x].size(); y++)
			{
				SngNoteInfoMidiImport tempSongNote = channels[x][y];

				if (y == (channels[x].size() - 1))
					break;

				SngNoteInfoMidiImport tempSongNoteNext = channels[x][y + 1];

				if (
					(tempSongNote.endAbsoluteTime == tempSongNoteNext.startAbsoluteTime)
					&& (tempSongNote.effect == tempSongNoteNext.effect)
					&& (tempSongNote.instrument == tempSongNoteNext.instrument)
					&& (tempSongNote.noteNumber == tempSongNoteNext.noteNumber)
					&& (tempSongNote.pan == tempSongNoteNext.pan)
					&& (tempSongNote.velocity == tempSongNoteNext.velocity)
					&& (tempSongNote.pitchBend == tempSongNoteNext.pitchBend)
					&& (tempSongNote.volume == tempSongNoteNext.volume)
					&& (tempSongNote.vibrato == tempSongNoteNext.vibrato)
					&& (tempSongNote.originalTrack == tempSongNoteNext.originalTrack)
					&& (tempSongNote.originalController == tempSongNoteNext.originalController)
					&& (tempSongNote.originalNoteUniqueId == tempSongNoteNext.originalNoteUniqueId)
					)
				{
					// Merge
					channels[x][y].endAbsoluteTime = tempSongNoteNext.endAbsoluteTime;
					channels[x].erase(channels[x].begin() + y + 1);

					// Redo Note
					y--;
				}
			}
		}


		float noteTimeDivisor = division / 0x30;

		unsigned long loopPointReal = (float)loopPoint / noteTimeDivisor;

		int outputPosition = 0;

		unsigned char* outputBuffer = new unsigned char[0x100000];

		for (int x = 0; x < 0x100000; x++)
			outputBuffer[x] = 0x00;

		// Sequence Format
		outputBuffer[outputPosition++] = 0xD3;
		outputBuffer[outputPosition++] = 0x20;

		// Sequence Type
		outputBuffer[outputPosition++] = 0xD5;
		outputBuffer[outputPosition++] = 0x32;

		unsigned short channelsEnabled = 0;

		for (int x = 0; x < numChannels; x++)
		{
			if (channels[x].size() > 0)
			{
				channelsEnabled |= (1 << x);
			}
		}

		// Enable Channels
		outputBuffer[outputPosition++] = 0xD7;
		outputBuffer[outputPosition++] = ((channelsEnabled >> 8) & 0xFF);
		outputBuffer[outputPosition++] = ((channelsEnabled) & 0xFF);

		int loopCounter = 1;
		if (loop && (loopPointReal != 0))
		{
			loopCounter = 2;
		}

		int loopPointByteOffset = 0;

		int currentTempo = 0x78;

		int currentInstrument = -1;
		int currentEffect = 0;
		int currentPan = 0x40;
		int currentVolume = 0x7F;
		int currentVibrato = 0x00;
		int currentPitchBend = -1;


		// Write out the sequence first
		int absoluteTempoTime = 0;
		int absoluteTimeSequence = 0;

		int absoluteChanPointerLocations[2][0x10];

		for (int currentGroup = 0; currentGroup < loopCounter; currentGroup++)
		{
			int previousAbsoluteTimeSequence = absoluteTimeSequence;

			if (loop)
			{
				if (currentGroup == (loopCounter - 1))
					loopPointByteOffset = outputPosition;
			}


			std::vector<TimeAndValue> subTempoPositions;
			std::vector<SngNoteInfoMidiImport> subChannels[0x10];

			if (loopCounter == 1)
			{
				subTempoPositions = tempoPositions;
				for (int x = 0; x < numChannels; x++)
				{
					subChannels[x] = channels[x];
				}
			}
			else
			{
				if (currentGroup == 0)
				{
					for (int t = 0; t < tempoPositions.size(); t++)
					{
						if (tempoPositions[t].absoluteTime < loopPointReal)
						{
							subTempoPositions.push_back(tempoPositions[t]);
						}
					}
				}
				else if (currentGroup == 1)
				{
					for (int t = 0; t < tempoPositions.size(); t++)
					{
						if (tempoPositions[t].absoluteTime >= loopPointReal)
						{
							subTempoPositions.push_back(tempoPositions[t]);
						}
					}
				}

				for (int x = 0; x < numChannels; x++)
				{
					for (int y = 0; y < channels[x].size(); y++)
					{
						if (currentGroup == 0)
						{
							if (channels[x][y].startAbsoluteTime < loopPointReal)
							{
								subChannels[x].push_back(channels[x][y]);
							}
						}
						else if (currentGroup == 1)
						{
							if (channels[x][y].startAbsoluteTime >= loopPointReal)
							{
								subChannels[x].push_back(channels[x][y]);
							}
						}
					}
				}
			}
			




			// Placeholder channel offsets
			for (int x = 0; x < numChannels; x++)
			{
				if (subChannels[x].size() > 0)
				{
					outputBuffer[outputPosition++] = 0x90 | x;
					absoluteChanPointerLocations[currentGroup][x] = outputPosition;

					// Placeholder
					outputBuffer[outputPosition++] = 0x00;
					outputBuffer[outputPosition++] = 0x00;
				}
				else
				{
					absoluteChanPointerLocations[currentGroup][x] = -1;
				}
			}

			// Write Master Volume
			outputBuffer[outputPosition++] = 0xDB;
			outputBuffer[outputPosition++] = 0x46;


			if ((subTempoPositions.size() == 0) || (subTempoPositions[0].absoluteTime > previousAbsoluteTimeSequence))
			{
				outputBuffer[outputPosition++] = 0xDD;
				outputBuffer[outputPosition++] = currentTempo;
			}

			

			// Write Tempos
			for (int t = 0; t < subTempoPositions.size(); t++)
			{
				while (subTempoPositions[t].absoluteTime > absoluteTempoTime)
				{
					int delta = (subTempoPositions[t].absoluteTime - absoluteTempoTime);
					outputBuffer[outputPosition++] = 0xFD;
					WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

					absoluteTempoTime += delta;
				}

				outputBuffer[outputPosition++] = 0xDD;
				outputBuffer[outputPosition++] = subTempoPositions[t].value;

				currentTempo = subTempoPositions[t].value;
			}

			// Timestamp channel to get to end of section
			if ((loop == true) && (loopPointReal != 0))
			{
				if (currentGroup == 0)
				{
					while (loopPointReal > absoluteTimeSequence)
					{
						int delta = (loopPointReal - absoluteTimeSequence);
						outputBuffer[outputPosition++] = 0xFD;
						WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

						absoluteTimeSequence += delta;
					}
				}
				else if (currentGroup == 1)
				{
					while (highestAbsoluteTime > absoluteTimeSequence)
					{
						int delta = (highestAbsoluteTime - absoluteTimeSequence);
						outputBuffer[outputPosition++] = 0xFD;
						WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

						absoluteTimeSequence += delta;
					}
				}
			}
			else
			{
				while (highestAbsoluteTime > absoluteTimeSequence)
				{
					int delta = (highestAbsoluteTime - absoluteTimeSequence);
					outputBuffer[outputPosition++] = 0xFD;
					WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

					absoluteTimeSequence += delta;
				}
			}


			if (currentGroup == (loopCounter - 1))
			{
				if (loop)
				{
					outputBuffer[outputPosition++] = 0xFB;
					outputBuffer[outputPosition++] = ((loopPointByteOffset >> 8) & 0xFF);
					outputBuffer[outputPosition++] = ((loopPointByteOffset) & 0xFF);
				}

				// Disable Channels
				outputBuffer[outputPosition++] = 0xD6;
				outputBuffer[outputPosition++] = ((channelsEnabled >> 8) & 0xFF);
				outputBuffer[outputPosition++] = ((channelsEnabled) & 0xFF);

				// Terminator Sequence
				outputBuffer[outputPosition++] = 0xFF;
			}

			if (loop && (loopPoint != 0))
			{
				absoluteTimeSequence = loopPointReal;
			}
			else
			{
				absoluteTimeSequence = highestAbsoluteTime;
			}
		}

		absoluteTimeSequence = 0;

		for (int currentGroup = 0; currentGroup < loopCounter; currentGroup++)
		{
			int previousAbsoluteTimeSequence = absoluteTimeSequence;

			std::vector<SngNoteInfoMidiImport> subChannels[0x10];

			if (loopCounter == 1)
			{
				for (int x = 0; x < numChannels; x++)
				{
					subChannels[x] = channels[x];
				}
			}
			else
			{
				for (int x = 0; x < numChannels; x++)
				{
					for (int y = 0; y < channels[x].size(); y++)
					{
						if (currentGroup == 0)
						{
							if (channels[x][y].startAbsoluteTime < loopPointReal)
							{
								subChannels[x].push_back(channels[x][y]);
							}
						}
						else if (currentGroup == 1)
						{
							if (channels[x][y].startAbsoluteTime >= loopPointReal)
							{
								subChannels[x].push_back(channels[x][y]);
							}
						}
					}
				}
			}

			bool warnedMissingNotes = false;
			for (int x = 0; x < numChannels; x++)
			{
				if (subChannels[x].size() > 0)
				{
					int absoluteTimeChannel = previousAbsoluteTimeSequence;
					int previousAbsoluteTimeChannel = absoluteTimeChannel;

					// Write channel location
					WriteShortToBuffer(outputBuffer, absoluteChanPointerLocations[currentGroup][x], outputPosition);

					std::vector<SngNoteInfoMidiImport> tracks[0x10];

					int maxTracks = 0;

					std::vector<TimeAndValueAndType> instrumentByAbsoluteTime;
					std::vector<TimeAndValueAndType> effectByAbsoluteTime;
					std::vector<TimeAndValueAndType> volumeByAbsoluteTime;
					std::vector<TimeAndValueAndType> panByAbsoluteTime;
					std::vector<TimeAndValueAndType> pitchBendByAbsoluteTime;
					std::vector<TimeAndValueAndType> vibratoByAbsoluteTime;

					for (int y = 0; y < subChannels[x].size(); y++)
					{
						instrumentByAbsoluteTime.push_back(TimeAndValueAndType(subChannels[x][y].startAbsoluteTime, subChannels[x][y].instrument, INSTRUMENTTYPE));
						effectByAbsoluteTime.push_back(TimeAndValueAndType(subChannels[x][y].startAbsoluteTime, subChannels[x][y].effect, EFFECTTYPE));
						volumeByAbsoluteTime.push_back(TimeAndValueAndType(subChannels[x][y].startAbsoluteTime, subChannels[x][y].volume, VOLUMETYPE));
						panByAbsoluteTime.push_back(TimeAndValueAndType(subChannels[x][y].startAbsoluteTime, subChannels[x][y].pan, PANTYPE));
						pitchBendByAbsoluteTime.push_back(TimeAndValueAndType(subChannels[x][y].startAbsoluteTime, subChannels[x][y].pitchBend, PITCHBENDTYPE));
						vibratoByAbsoluteTime.push_back(TimeAndValueAndType(subChannels[x][y].startAbsoluteTime, subChannels[x][y].vibrato, VIBRATOTYPE));
					}

					std::sort(instrumentByAbsoluteTime.begin(), instrumentByAbsoluteTime.end(), timeAndValueAndTypeSortByTime());
					std::sort(effectByAbsoluteTime.begin(), effectByAbsoluteTime.end(), timeAndValueAndTypeSortByTime());
					std::sort(volumeByAbsoluteTime.begin(), volumeByAbsoluteTime.end(), timeAndValueAndTypeSortByTime());
					std::sort(panByAbsoluteTime.begin(), panByAbsoluteTime.end(), timeAndValueAndTypeSortByTime());
					std::sort(pitchBendByAbsoluteTime.begin(), pitchBendByAbsoluteTime.end(), timeAndValueAndTypeSortByTime());
					std::sort(vibratoByAbsoluteTime.begin(), vibratoByAbsoluteTime.end(), timeAndValueAndTypeSortByTime());

					for (int y = 0; y < instrumentByAbsoluteTime.size(); y++)
					{
						int size = instrumentByAbsoluteTime.size();
						if ((y + 1) < (instrumentByAbsoluteTime.size()))
						{
							if (instrumentByAbsoluteTime[y].value == instrumentByAbsoluteTime[y+1].value)
							{
								instrumentByAbsoluteTime.erase(instrumentByAbsoluteTime.begin() + y + 1);
								y--;
							}
						}
					}

					// Not sure what to do if no instrument and beginning
					if (currentGroup > 0)
					{
						if (currentInstrument != -1)
						{
							if ((instrumentByAbsoluteTime.size() == 0) || (instrumentByAbsoluteTime[0].absoluteTime > previousAbsoluteTimeSequence))
							{
								instrumentByAbsoluteTime.insert(instrumentByAbsoluteTime.begin(), TimeAndValueAndType(previousAbsoluteTimeSequence, currentInstrument, INSTRUMENTTYPE));
							}
						}
					}
					

					for (int y = 0; y < effectByAbsoluteTime.size(); y++)
					{
						if ((y + 1) < (effectByAbsoluteTime.size()))
						{
							if (effectByAbsoluteTime[y].value == effectByAbsoluteTime[y+1].value)
							{
								effectByAbsoluteTime.erase(effectByAbsoluteTime.begin() + y + 1);
								y--;
							}
						}
					}

					if ((effectByAbsoluteTime.size() == 0) || (effectByAbsoluteTime[0].absoluteTime > previousAbsoluteTimeSequence))
					{
						if ((effectByAbsoluteTime.size() > 0) && (effectByAbsoluteTime[0].value == currentEffect))
							effectByAbsoluteTime[0].absoluteTime = previousAbsoluteTimeSequence;
						else
							effectByAbsoluteTime.insert(effectByAbsoluteTime.begin(), TimeAndValueAndType(previousAbsoluteTimeSequence, currentEffect, EFFECTTYPE));
					}

					for (int y = 0; y < volumeByAbsoluteTime.size(); y++)
					{
						if ((y + 1) < (volumeByAbsoluteTime.size()))
						{
							if (volumeByAbsoluteTime[y].value == volumeByAbsoluteTime[y+1].value)
							{
								volumeByAbsoluteTime.erase(volumeByAbsoluteTime.begin() + y + 1);
								y--;
							}
						}
					}

					if ((volumeByAbsoluteTime.size() == 0) || (volumeByAbsoluteTime[0].absoluteTime > previousAbsoluteTimeSequence))
					{
						if ((volumeByAbsoluteTime.size() > 0) && (volumeByAbsoluteTime[0].value == currentVolume))
							volumeByAbsoluteTime[0].absoluteTime = previousAbsoluteTimeSequence;
						else
							volumeByAbsoluteTime.insert(volumeByAbsoluteTime.begin(), TimeAndValueAndType(previousAbsoluteTimeSequence, currentVolume, VOLUMETYPE));
					}

					for (int y = 0; y < panByAbsoluteTime.size(); y++)
					{
						if ((y + 1) < (panByAbsoluteTime.size()))
						{
							if (panByAbsoluteTime[y].value == panByAbsoluteTime[y+1].value)
							{
								panByAbsoluteTime.erase(panByAbsoluteTime.begin() + y + 1);
								y--;
							}
						}
					}

					if ((panByAbsoluteTime.size() == 0) || (panByAbsoluteTime[0].absoluteTime > previousAbsoluteTimeSequence))
					{
						if ((panByAbsoluteTime.size() > 0) && (panByAbsoluteTime[0].value == currentPan))
							panByAbsoluteTime[0].absoluteTime = previousAbsoluteTimeSequence;
						else
							panByAbsoluteTime.insert(panByAbsoluteTime.begin(), TimeAndValueAndType(previousAbsoluteTimeSequence, currentPan, PANTYPE));
					}

					for (int y = 0; y < pitchBendByAbsoluteTime.size(); y++)
					{
						if ((y + 1) < (pitchBendByAbsoluteTime.size()))
						{
							if (pitchBendByAbsoluteTime[y].value == pitchBendByAbsoluteTime[y+1].value)
							{
								pitchBendByAbsoluteTime.erase(pitchBendByAbsoluteTime.begin() + y + 1);
								y--;
							}
						}
					}

					if ((currentPitchBend == -1) && (pitchBendByAbsoluteTime.size() == 1) && (pitchBendByAbsoluteTime[0].value == 0x40))
					{
						// Clear if 0x40, defaulted
						pitchBendByAbsoluteTime.clear();
					}
					
					// Pitch Bend probably doesn't need 0 value
					if (currentGroup > 0)
					{
						if (currentPitchBend != -1)
						{
							if ((pitchBendByAbsoluteTime.size() == 0) || (pitchBendByAbsoluteTime[0].absoluteTime > previousAbsoluteTimeSequence))
							{
								pitchBendByAbsoluteTime.insert(pitchBendByAbsoluteTime.begin(), TimeAndValueAndType(previousAbsoluteTimeSequence, currentPitchBend, PITCHBENDTYPE));
							}
						}
					}

					for (int y = 0; y < vibratoByAbsoluteTime.size(); y++)
					{
						if ((y + 1) < (vibratoByAbsoluteTime.size()))
						{
							if (vibratoByAbsoluteTime[y].value == vibratoByAbsoluteTime[y+1].value)
							{
								vibratoByAbsoluteTime.erase(vibratoByAbsoluteTime.begin() + y + 1);
								y--;
							}
						}
					}

					if ((currentVibrato == 0x00) && (vibratoByAbsoluteTime.size() == 1) && (vibratoByAbsoluteTime[0].value == 0x00))
					{
						// Clear if 0x00, defaulted
						vibratoByAbsoluteTime.clear();
					}
					
					// Vibrato  doesn't need 0 value
					if (currentGroup > 0)
					{
						if (currentVibrato != 0x00)
						{
							if ((vibratoByAbsoluteTime.size() == 0) || (vibratoByAbsoluteTime[0].absoluteTime > previousAbsoluteTimeSequence))
							{
								vibratoByAbsoluteTime.insert(vibratoByAbsoluteTime.begin(), TimeAndValueAndType(previousAbsoluteTimeSequence, currentVibrato, VIBRATOTYPE));
							}
						}
					}

					// Merge all
					std::vector<TimeAndValueAndType> mergedChannelActions;

					for (int y = 0; y < instrumentByAbsoluteTime.size(); y++)
					{
						mergedChannelActions.push_back(instrumentByAbsoluteTime[y]);
					}

					for (int y = 0; y < effectByAbsoluteTime.size(); y++)
					{
						mergedChannelActions.push_back(effectByAbsoluteTime[y]);
					}

					for (int y = 0; y < volumeByAbsoluteTime.size(); y++)
					{
						mergedChannelActions.push_back(volumeByAbsoluteTime[y]);
					}

					for (int y = 0; y < panByAbsoluteTime.size(); y++)
					{
						mergedChannelActions.push_back(panByAbsoluteTime[y]);
					}

					for (int y = 0; y < pitchBendByAbsoluteTime.size(); y++)
					{
						mergedChannelActions.push_back(pitchBendByAbsoluteTime[y]);
					}

					for (int y = 0; y < vibratoByAbsoluteTime.size(); y++)
					{
						mergedChannelActions.push_back(vibratoByAbsoluteTime[y]);
					}

					std::sort(mergedChannelActions.begin(), mergedChannelActions.end(), timeAndValueAndTypeSortByTime());








					// Merge notes now that had same of those values that are part of channel
					for (int y = 0; y < subChannels[x].size(); y++)
					{
						SngNoteInfoMidiImport tempSongNote = subChannels[x][y];

						for (int z = 0; z < subChannels[x].size(); z++)
						{
							if (y == z)
								continue;

							SngNoteInfoMidiImport tempSongNoteNext = subChannels[x][z];

							if (
								(tempSongNote.endAbsoluteTime == tempSongNoteNext.startAbsoluteTime)
								&& (tempSongNote.instrument == tempSongNoteNext.instrument)
								&& (tempSongNote.noteNumber == tempSongNoteNext.noteNumber)
								&& (tempSongNote.velocity == tempSongNoteNext.velocity)
								&& (tempSongNote.originalTrack == tempSongNoteNext.originalTrack)
								&& (tempSongNote.originalController == tempSongNoteNext.originalController)
								&& (tempSongNote.originalNoteUniqueId == tempSongNoteNext.originalNoteUniqueId)
								)
							{
								// Merge
								subChannels[x][y].endAbsoluteTime = tempSongNoteNext.endAbsoluteTime;
								subChannels[x].erase(subChannels[x].begin() + z);

								// Redo Note
								y--;
								break;
							}
						}
					}

					for (int y = 0; y < subChannels[x].size(); y++)
					{
						int selectedTrack = -1;
						int maxAvailableTracks = 4;
						if (gameStyle == EADMUSICSTYLEZELDA)
							maxAvailableTracks = 4;
						else if (gameStyle == EADMUSICSTYLESTARFOX)
							maxAvailableTracks = 0x10;
						else if (gameStyle == EADMUSICSTYLEMARIO)
							maxAvailableTracks = 4;
						for (int t = 0; t < maxAvailableTracks; t++)
						{
							bool overlap = false;
							for (int z = 0; z < tracks[t].size(); z++)
							{
								if (IsOverlap(subChannels[x][y].startAbsoluteTime, subChannels[x][y].endAbsoluteTime, tracks[t][z].startAbsoluteTime, tracks[t][z].endAbsoluteTime))
								{
									overlap = true;
									break;
								}
							}

							if (!overlap)
							{
								selectedTrack = t;
								break;
							}
						}

						if (selectedTrack == -1)
						{
							// Skip
							if (!warnedMissingNotes)
							{
								warnedMissingNotes = true;
								CString trackStr;
								trackStr.Format("%02X", x);
								MessageBox(NULL, "Too many simultaneous notes on track " + trackStr, "Error", NULL);
							}
						}
						else
						{
							if ((selectedTrack + 1) > maxTracks)
								maxTracks = selectedTrack + 1;
							tracks[selectedTrack].push_back(subChannels[x][y]);
						}
					}






					// Channel Start
					outputBuffer[outputPosition++] = 0xC4;

					int absoluteTrackOffset[0x10];
					for (int y = 0; y < maxTracks; y++)
					{
						// Placeholder
						if (gameStyle == EADMUSICSTYLEZELDA)
						{
							// Max 4
							outputBuffer[outputPosition++] = 0x88 + y;
						}
						else if (gameStyle == EADMUSICSTYLEMARIO)
						{
							// Max 4
							outputBuffer[outputPosition++] = 0x90 + y;
						}
						else if (gameStyle == EADMUSICSTYLESTARFOX)
						{
							// Max 0x10
							outputBuffer[outputPosition++] = 0x90 + y;
						}
						absoluteTrackOffset[y] = outputPosition;

						outputBuffer[outputPosition++] = 0x00;
						outputBuffer[outputPosition++] = 0x00;
					}

					// Priority default
					outputBuffer[outputPosition++] = 0xE9;
					outputBuffer[outputPosition++] = 0x02;

					// Now write the events
					for (int y = 0; y < mergedChannelActions.size(); y++)
					{
						while (mergedChannelActions[y].absoluteTime > absoluteTimeChannel)
						{
							int delta = (mergedChannelActions[y].absoluteTime - absoluteTimeChannel);
							outputBuffer[outputPosition++] = 0xFD;
							WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

							absoluteTimeChannel += delta;
						}

						if (mergedChannelActions[y].type == INSTRUMENTTYPE)
						{
							outputBuffer[outputPosition++] = 0xC1;
							outputBuffer[outputPosition++] = mergedChannelActions[y].value;

							currentInstrument = mergedChannelActions[y].value;
						}
						else if (mergedChannelActions[y].type == EFFECTTYPE)
						{
							outputBuffer[outputPosition++] = 0xD4;
							outputBuffer[outputPosition++] = mergedChannelActions[y].value;

							currentEffect = mergedChannelActions[y].value;
						}
						else if (mergedChannelActions[y].type == VOLUMETYPE)
						{
							outputBuffer[outputPosition++] = 0xDF;
							outputBuffer[outputPosition++] = mergedChannelActions[y].value;

							currentVolume = mergedChannelActions[y].value;
						}
						else if (mergedChannelActions[y].type == PANTYPE)
						{
							outputBuffer[outputPosition++] = 0xDD;
							outputBuffer[outputPosition++] = mergedChannelActions[y].value;

							currentPan = mergedChannelActions[y].value;
						}
						else if (mergedChannelActions[y].type == PITCHBENDTYPE)
						{
							outputBuffer[outputPosition++] = 0xD3;
							outputBuffer[outputPosition++] = mergedChannelActions[y].value;

							currentPitchBend = mergedChannelActions[y].value;
						}
						else if (mergedChannelActions[y].type == VIBRATOTYPE)
						{
							outputBuffer[outputPosition++] = 0xD8;
							outputBuffer[outputPosition++] = mergedChannelActions[y].value;

							currentVibrato = mergedChannelActions[y].value;
						}
					}

					// Timestamp channel to get to end of section
					if ((loop == true) && (loopPointReal != 0))
					{
						if (currentGroup == 0)
						{
							while (loopPointReal > absoluteTimeChannel)
							{
								int delta = (loopPointReal - absoluteTimeChannel);
								outputBuffer[outputPosition++] = 0xFD;
								WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

								absoluteTimeChannel += delta;
							}
						}
						else if (currentGroup == 1)
						{
							while (highestAbsoluteTime > absoluteTimeChannel)
							{
								int delta = (highestAbsoluteTime - absoluteTimeChannel);
								outputBuffer[outputPosition++] = 0xFD;
								WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

								absoluteTimeChannel += delta;
							}
						}
					}
					else
					{
						while (highestAbsoluteTime > absoluteTimeChannel)
						{
							int delta = (highestAbsoluteTime - absoluteTimeChannel);
							outputBuffer[outputPosition++] = 0xFD;
							WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

							absoluteTimeChannel += delta;
						}
					}

					// Terminator Sequence Channel
					outputBuffer[outputPosition++] = 0xFF;




					

					// Now write tracks
					for (int z = 0; z < maxTracks; z++)
					{
						// Wite offset
						WriteShortToBuffer(outputBuffer, absoluteTrackOffset[z], outputPosition);

						int absoluteTimeTrack = previousAbsoluteTimeChannel;

						int lastNoteTimeStamp = -1;

						for (int y = 0; y < tracks[z].size(); y++)
						{
							while (tracks[z][y].startAbsoluteTime > absoluteTimeTrack)
							{
								int delta = (tracks[z][y].startAbsoluteTime - absoluteTimeTrack);
								outputBuffer[outputPosition++] = 0xC0;
								WriteEADMusicTimeValue(outputBuffer, outputPosition, delta);			

								absoluteTimeTrack += delta;
							}

							//  1   -   NoteLength               =    (((duration / 256))
							//	       	    /
							//	      TimeTillNextNote

							int noteLength = tracks[z][y].endAbsoluteTime - tracks[z][y].startAbsoluteTime;
							int timeStamp = noteLength;
							int ratio = -1;
							bool useDuration = false;
							if ((y + 1) < (tracks[z].size()))
							{
								int nextNote = tracks[z][y+1].startAbsoluteTime - tracks[z][y].startAbsoluteTime;
								if (nextNote == noteLength)
								{
									useDuration = false;
								}
								else
								{
									double fractionalRatio = ((1.0 - ((double)noteLength / (double)nextNote)) * 256.0);
									ratio = ceil(fractionalRatio);
									int noteLengthCalculated = (nextNote - ((ratio * nextNote) >> 8));

									if ((ratio >= 0x100) || (ratio <= 0))
									{
										useDuration = false;
									}
									else if (noteLengthCalculated != noteLength)
									{
										useDuration = false;
									}
									else
									{
										useDuration = true;
										
										timeStamp = nextNote;
									}
								}
							}

							if ((!useDuration) || ((y + 1) == (tracks[z].size())))
							{
								// Do TV always
								outputBuffer[outputPosition++] = 0x40 | tracks[z][y].noteNumber;
								WriteEADMusicTimeValue(outputBuffer, outputPosition, timeStamp);
								outputBuffer[outputPosition++] = tracks[z][y].velocity;
								
							}
							else if (timeStamp == lastNoteTimeStamp)
							{
								// VG
								outputBuffer[outputPosition++] = 0x80 | tracks[z][y].noteNumber;
								outputBuffer[outputPosition++] = tracks[z][y].velocity;
								outputBuffer[outputPosition++] = (unsigned char)ratio;
							}
							else
							{
								// Do TVG
								outputBuffer[outputPosition++] = tracks[z][y].noteNumber;
								WriteEADMusicTimeValue(outputBuffer, outputPosition, timeStamp);
								outputBuffer[outputPosition++] = tracks[z][y].velocity;
								outputBuffer[outputPosition++] = (unsigned char)ratio;
							}

							lastNoteTimeStamp = timeStamp;

							absoluteTimeTrack += timeStamp;
						}
						outputBuffer[outputPosition++] = 0xFF;
					}
				}
			}

			if (loop && (loopPoint != 0))
			{
				absoluteTimeSequence = loopPointReal;
			}
			else
			{
				absoluteTimeSequence = highestAbsoluteTime;
			}
		}

		FILE* outFile = fopen(output, "wb");
		if (outFile == NULL)
		{
			delete [] outputBuffer;

			MessageBox(NULL, "Error outputting file", "Error", NULL);
			return false;
		}

		fwrite(outputBuffer, 1, outputPosition, outFile);

		delete [] outputBuffer;
		fclose(outFile);
	}
	catch (...)
	{
		MessageBox(NULL, "Error converting", "Error", NULL);
		return false;
	}
	return true;
}

int CMidiParse::IncrementSpot(unsigned long& spot, int count, unsigned char* coverage)
{
	if (coverage)
	{
		for (int x = spot; x < spot + count; x++)
		{
			coverage[x] = 0x00;
		}
	}

	spot += count;
	return spot;
}

int CMidiParse::ReadEADMusicTimeValue(unsigned char* inputMID, unsigned long& offset, unsigned char* coverage)
{
	if (inputMID[offset] & 0x80)
	{
		int timevalue = (((inputMID[offset] & 0x7F) << 8) | inputMID[offset+1]);
		IncrementSpot(offset, 2, coverage);
		return timevalue;
	}
	else
	{
		int timevalue = inputMID[offset];
		IncrementSpot(offset, 1, coverage);
		return timevalue;
	}
}

int CMidiParse::WriteEADMusicTimeValue(unsigned char* outputMID, int& offset, int value)
{
	int delta = value;

	if (delta > 0x7FFF)
		delta = 0x7FFF;

	if (delta < 0x80)
	{
		outputMID[offset++] = delta;
	}
	else
	{
		outputMID[offset++] = 0x80 | (delta >> 8);
		outputMID[offset++] = (delta & 0xFF);
	}

	return delta;
}

void CMidiParse::ParseEADMusicTrackLayer(int gameStyle, FILE* outFileDebug, unsigned char* inputMID, int end, int offset, int channel, int layer, unsigned long absoluteTime,
							   int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& trackOutputNotes,
							   int& noteUniqueId,
							   std::vector<unsigned long>& jumpStarts, std::vector<unsigned long>& jumpsTaken, bool attemptJumps, unsigned char* coverage, bool channelStarted, bool isPreview)
{
	fprintfIfDebug(outFileDebug, "-------------------Track Chn %X Layer %X-------------------\n", channel, layer);

	unsigned char command = 0x00;
	unsigned long spot = offset;

	std::vector<int> returnBackOffset;
	std::vector<int> returnBackAlternateChoices;

	int currentCoarseTune = 0;
	int lastTimestamp = 0x0;

loopAgain:
	while ((command != 0xFF) && (spot < end))
	{
		command = inputMID[spot];

		IncrementSpot(spot, 1, coverage);

		if (command >= 0xC0)
		{
			fprintfIfDebug(outFileDebug, "TRK %08X Time: %08X CMD:      %02X", spot-1, absoluteTime, command);

			if (command == 0xC0)
			{
				fprintfIfDebug(outFileDebug, " Timestamp");
				
				int timestamp = ReadEADMusicTimeValue(inputMID, spot, coverage);
				if (timestamp < 0x80)
					fprintfIfDebug(outFileDebug, " %02X (%d)", timestamp, timestamp);
				else
					fprintfIfDebug(outFileDebug, " %02X%02X - %04X (%d)", inputMID[spot-2], inputMID[spot+1-2], timestamp, timestamp);

				absoluteTime += timestamp;
			}
			else if (command == 0xC1)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xC2)
			{
				fprintfIfDebug(outFileDebug, " Coarse Tune");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

				currentCoarseTune = (signed char)inputMID[spot];
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xC3)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xC4)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xC5)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xC6)
			{
				fprintfIfDebug(outFileDebug, " SFX Id");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xC7)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
				IncrementSpot(spot, 3, coverage);
			}
			else if (command == 0xC8)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xC9)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xCA)
			{
				fprintfIfDebug(outFileDebug, " SFX Pan");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xCB)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X%02X %02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
				
				IncrementSpot(spot, 3, coverage);
			}
			else if (command == 0xCC)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xCD)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xCE)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xCF)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xD0)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD1)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD2)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD3)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD4)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD5)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD6)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD7)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD8)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xD9)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xDA)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xDB)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xDC)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xDD)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xDE)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xDF)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE0)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE1)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE2)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE3)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE4)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE5)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE6)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE7)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE8)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xE9)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xEA)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xEB)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xEC)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xED)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xEE)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xEF)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xF0)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xF1)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xF2)
			{
				fprintfIfDebug(outFileDebug, " Go to Relative Offset if Play Value < 0");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

				int relativeOffset = (signed char)inputMID[spot];
				IncrementSpot(spot, 1, coverage);

				if (attemptJumps)
				{
					if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
					{
						fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
						jumpStarts.push_back(spot);
						returnBackAlternateChoices.push_back(spot - 2);
					}
					else
					{
						if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
						{
							fprintfIfDebug(outFileDebug, "...Taking\n");
							jumpsTaken.push_back(relativeOffset);
							spot = spot + relativeOffset;
						}
					}
				}
			}
			else if (command == 0xF3)
			{
				fprintfIfDebug(outFileDebug, " Go to Relative Offset if Play Value == 0");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

				int relativeOffset = (signed char)inputMID[spot];
				IncrementSpot(spot, 1, coverage);

				if (attemptJumps)
				{
					if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
					{
						fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
						jumpStarts.push_back(spot);
						returnBackAlternateChoices.push_back(spot - 2);
					}
					else
					{			
						if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
						{
							fprintfIfDebug(outFileDebug, "...Taking\n");
							jumpsTaken.push_back(relativeOffset);
							spot = spot + relativeOffset;
						}
					}
				}
			}
			else if (command == 0xF4)
			{
				fprintfIfDebug(outFileDebug, " Go to  relative offset");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], (signed char)inputMID[spot]);

				int relativeOffset = (signed char)inputMID[spot];
				IncrementSpot(spot, 1, coverage);

				if (attemptJumps)
				{
					if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
					{
						fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
						jumpStarts.push_back(spot);
						returnBackAlternateChoices.push_back(spot - 2);
						break;
					}
					else
					{
						if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
						{
							fprintfIfDebug(outFileDebug, "...Taking\n");
							jumpsTaken.push_back(relativeOffset);
							spot = spot + relativeOffset;
						}
						else
						{
							break;
						}
					}
				}
			}
			else if (command == 0xF5)
			{
				fprintfIfDebug(outFileDebug, " Go to absolute offset if Play Value >= 0");
				fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

				int absoluteOffset = CharArrayToShort(&inputMID[spot]);
				IncrementSpot(spot, 2, coverage);

				if (attemptJumps)
				{
					if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
					{
						fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
						jumpStarts.push_back(spot);
						returnBackAlternateChoices.push_back(spot - 3);
					}
					else
					{
						if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
						{
							fprintfIfDebug(outFileDebug, "...Taking\n");
							jumpsTaken.push_back(absoluteOffset);
							spot =  absoluteOffset;
						}
					}
				}
			}
			else if (command == 0xF6)
			{
				fprintfIfDebug(outFileDebug, " Restart Song");

				if (attemptJumps)
				{
					int startSpot = 0;

					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), startSpot) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(startSpot);
						offset = startSpot;
					}
				}
			}
			else if (command == 0xF7)
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
			else if (command == 0xF8)
			{
				fprintfIfDebug(outFileDebug, " ?");
				fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
				IncrementSpot(spot, 1, coverage);
			}
			else if (command == 0xF9)
			{
				fprintfIfDebug(outFileDebug, " Go to Absolute Offset if Play Value < 0");
				fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

				int absoluteOffset = CharArrayToShort(&inputMID[spot]);
				IncrementSpot(spot, 2, coverage);

				if (attemptJumps)
				{
					if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
					{
						fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
						jumpStarts.push_back(spot);
						returnBackAlternateChoices.push_back(spot - 3);
					}
					else
					{
						if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
						{
							fprintfIfDebug(outFileDebug, "...Taking\n");
							jumpsTaken.push_back(absoluteOffset);
							offset =  absoluteOffset;
						}
					}
				}
			}
			else if (command == 0xFA)
			{
				fprintfIfDebug(outFileDebug, " Go to Absolute Offset if Play Value == 0");
				fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

				int absoluteOffset = CharArrayToShort(&inputMID[spot]);
				IncrementSpot(spot, 2, coverage);

				if (attemptJumps)
				{
					if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
					{
						fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
						jumpStarts.push_back(spot);
						returnBackAlternateChoices.push_back(spot - 3);
					}
					else
					{
						if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
						{
							fprintfIfDebug(outFileDebug, "...Taking\n");
							jumpsTaken.push_back(absoluteOffset);
							spot = absoluteOffset;
						}
					}
				}
			}
			else if (command == 0xFB)
			{
				fprintfIfDebug(outFileDebug, " Go to absolute offset");
				fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
				
				int absoluteOffset = CharArrayToShort(&inputMID[spot]);
				IncrementSpot(spot, 2, coverage);

				if (attemptJumps)
				{
					if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
					{
						fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
						jumpStarts.push_back(spot);
						returnBackAlternateChoices.push_back(spot - 3);
						break;
					}
					else
					{
						if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
						{
							fprintfIfDebug(outFileDebug, "...Taking\n");
							jumpsTaken.push_back(absoluteOffset);
							spot = absoluteOffset;
						}
						else
						{
							break;
						}
					}
				}
			}
			else if (command == 0xFC)
			{
				unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
				fprintfIfDebug(outFileDebug, " Jump and Link");
				fprintfIfDebug(outFileDebug, " Offset %04X", offset);

				IncrementSpot(spot, 2, coverage);
				returnBackOffset.push_back(spot);

				spot = offset;
			}
			else if (command == 0xFD)
			{
				fprintfIfDebug(outFileDebug, " Delay");
				
				int timestamp = ReadEADMusicTimeValue(inputMID, spot, coverage);
				if (timestamp < 0x80)
					fprintfIfDebug(outFileDebug, " %02X (%d)", timestamp, timestamp);
				else
					fprintfIfDebug(outFileDebug, " %02X%02X - %04X (%d)", inputMID[spot-2], inputMID[spot+1-2], timestamp, timestamp);

				absoluteTime += timestamp;
			}
			else if (command == 0xFE)
			{
				fprintfIfDebug(outFileDebug, " RETURN");
			}
			else if (command == 0xFF)
			{
				fprintfIfDebug(outFileDebug, " END");
			}
			else
			{
				fprintfIfDebug(outFileDebug, " UNKNOWN");
			}
		}
		else if (!channelStarted)
		{
			// Note + Time
			int preTimestampSpot = spot;

			int timestamp = ReadEADMusicTimeValue(inputMID, spot, coverage);
		
			lastTimestamp = timestamp;

			fprintfIfDebug(outFileDebug, "TRK %08X Time: %08X Note T:  %02X Note %02X", preTimestampSpot-1, absoluteTime, command, command);

			if (timestamp < 0x80)
				fprintfIfDebug(outFileDebug, " Length %02X (%d)", timestamp, timestamp);
			else
				fprintfIfDebug(outFileDebug, " Length %02X%02X - %04X (%d)", inputMID[spot-2], inputMID[spot+1-2], timestamp, timestamp);

			int note = command & 0x3F;
			int velocity = inputMID[spot];
			int noteLength = timestamp;

			SngNoteInfo songNoteInfo;
			songNoteInfo.originalTrack = channel;
			songNoteInfo.originalNoteUniqueId = noteUniqueId++;
			songNoteInfo.startAbsoluteTime = absoluteTime;
			songNoteInfo.noteNumber = note + currentCoarseTune + 0x15;

			songNoteInfo.instrument = 0x00;
			songNoteInfo.velocity = 0x7F;

			songNoteInfo.effect = 0x00;
			songNoteInfo.tempo = 0x78;
			songNoteInfo.pan = 0x40;
			songNoteInfo.pitchBend = 0x40;
			songNoteInfo.vibrato = 0x00;
			songNoteInfo.volume = 0x7F;

			songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

			trackOutputNotes.push_back(songNoteInfo);

			absoluteTime += timestamp;
		}
		else if (command & 0x40)
		{
			int preTimestampSpot = spot;

			int timestamp = ReadEADMusicTimeValue(inputMID, spot, coverage);

			lastTimestamp = timestamp;

			fprintfIfDebug(outFileDebug, "TRK %08X Time: %08X Note TV:  %02X Note %02X", preTimestampSpot-1, absoluteTime, command, command & 0x3F);

			if (timestamp < 0x80)
				fprintfIfDebug(outFileDebug, " Length %02X (%d)", timestamp, timestamp);
			else
				fprintfIfDebug(outFileDebug, " Length %02X%02X - %04X (%d)", inputMID[spot-2], inputMID[spot+1-2], timestamp, timestamp);

			fprintfIfDebug(outFileDebug, " Velocity %02X (%d) [True Note Length %04X]", inputMID[spot], inputMID[spot], timestamp);

			int note = command & 0x3F;
			int velocity = inputMID[spot];
			int noteLength = timestamp;

			SngNoteInfo songNoteInfo;
			songNoteInfo.originalTrack = channel;
			songNoteInfo.originalNoteUniqueId = noteUniqueId++;
			songNoteInfo.startAbsoluteTime = absoluteTime;
			songNoteInfo.noteNumber = note + currentCoarseTune + 0x15;

			songNoteInfo.instrument = 0x00;
			songNoteInfo.velocity = velocity;

			songNoteInfo.effect = 0x00;
			songNoteInfo.tempo = 0x78;
			songNoteInfo.pan = 0x40;
			songNoteInfo.pitchBend = 0x40;
			songNoteInfo.vibrato = 0x00;
			songNoteInfo.volume = 0x7F;

			songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

			trackOutputNotes.push_back(songNoteInfo);

			absoluteTime += timestamp;

			IncrementSpot(spot, 1, coverage);
		}
		else if (command & 0x80)
		{
			fprintfIfDebug(outFileDebug, "TRK %08X Time: %08X Note VD:  %02X Note %02X", spot-1, absoluteTime, command, command & 0x3F);
			
			int duration = inputMID[spot+1];
			fprintfIfDebug(outFileDebug, " [Timestamp %04X] Velocity %02X (%d) Duration %02X (%d) [True Note Length %04X]", lastTimestamp, inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], (lastTimestamp - ((duration * lastTimestamp) >> 8)));

			int note = command & 0x3F;
			int velocity = inputMID[spot];
			int noteLength = (lastTimestamp - ((duration * lastTimestamp) >> 8));

			SngNoteInfo songNoteInfo;
			songNoteInfo.originalTrack = channel;
			songNoteInfo.originalNoteUniqueId = noteUniqueId++;
			songNoteInfo.startAbsoluteTime = absoluteTime;
			songNoteInfo.noteNumber = note + currentCoarseTune + 0x15;

			songNoteInfo.instrument = 0x00;
			songNoteInfo.velocity = velocity;

			songNoteInfo.effect = 0x00;
			songNoteInfo.tempo = 0x78;
			songNoteInfo.pan = 0x40;
			songNoteInfo.pitchBend = 0x40;
			songNoteInfo.vibrato = 0x00;
			songNoteInfo.volume = 0x7F;

			songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

			trackOutputNotes.push_back(songNoteInfo);

			absoluteTime += lastTimestamp;

			IncrementSpot(spot, 2, coverage);
		}
		else
		{
			int preTimestampSpot = spot;

			int timestamp = ReadEADMusicTimeValue(inputMID, spot, coverage);
			lastTimestamp = timestamp;

			fprintfIfDebug(outFileDebug, "TRK %08X Time: %08X Note TVD: %02X Note %02X", preTimestampSpot-1, absoluteTime, command, command & 0x3F);

			if (timestamp < 0x80)
				fprintfIfDebug(outFileDebug, " Length %02X (%d)", timestamp, timestamp);
			else
				fprintfIfDebug(outFileDebug, " Length %02X%02X - %04X (%d)", inputMID[spot-2], inputMID[spot+1-2], timestamp, timestamp);

			int duration = inputMID[spot+1];
			fprintfIfDebug(outFileDebug, " Velocity %02X (%d) Duration %02X (%d) [True Note Length %04X]", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], (timestamp - ((duration * timestamp) >> 8)));

			int note = command & 0x3F;
			int velocity = inputMID[spot];
			int noteLength = (timestamp - ((duration * timestamp) >> 8));

			SngNoteInfo songNoteInfo;
			songNoteInfo.originalTrack = channel;
			songNoteInfo.originalNoteUniqueId = noteUniqueId++;
			songNoteInfo.startAbsoluteTime = absoluteTime;
			songNoteInfo.noteNumber = note + currentCoarseTune + 0x15;

			songNoteInfo.instrument = 0x00;
			songNoteInfo.velocity = velocity;

			songNoteInfo.effect = 0x00;
			songNoteInfo.tempo = 0x78;
			songNoteInfo.pan = 0x40;
			songNoteInfo.pitchBend = 0x40;
			songNoteInfo.vibrato = 0x00;
			songNoteInfo.volume = 0x7F;

			songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

			trackOutputNotes.push_back(songNoteInfo);

			absoluteTime += timestamp;

			//Time %02X Velocity %02X Duration %02X
			//, inputMID[spot], inputMID[spot+1], inputMID[spot+2]
			IncrementSpot(spot, 2, coverage);

			// This goes backwards or something odd...figure out later, command was 00FFFF00 1080 Snowboarding Midi 0
			/*if (duration == 0)
			{
				fprintfIfDebug(outFileDebug, "\n");
				break;
			}*/
		}

		fprintfIfDebug(outFileDebug, "\n");
	}

	if (returnBackAlternateChoices.size() > 0)
	{
		// Return from Jump and Link
		spot = returnBackAlternateChoices.back();
		returnBackAlternateChoices.pop_back();
		command = 0x00;
		
		fprintfIfDebug(outFileDebug, "... Taking Alternate Path to %08X\n", spot);
		goto loopAgain;
	}

	if (returnBackOffset.size() > 0)
	{
		// Return from Jump and Link
		spot = returnBackOffset.back();
		returnBackOffset.pop_back();
		command = 0x00;
		
		fprintfIfDebug(outFileDebug, "... Returning from Subroutine to %08X\n", spot);
		goto loopAgain;
	}
}

void CMidiParse::ParseEADMusicChannel(int gameStyle, FILE* outFileDebug, unsigned char* inputMID, int end, int offset, int channel, unsigned long absoluteTime,
							int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes,
							int& noteUniqueId, int currentInstrument[0x10], int currentPan[0x10], int currentVolume[0x10], int currentPitchBend[0x10], int currentEffect[0x10],
							std::vector<unsigned long>& jumpStarts, std::vector<unsigned long>& jumpsTaken, bool attemptJumps, unsigned char* coverage, bool channelStarted[0x10],
							int currentVibrato[0x10], bool isPreview)
{
	fprintfIfDebug(outFileDebug, "-------------------Channel %X-------------------\n", channel);
	unsigned long startAbsoluteTime = absoluteTime;

	unsigned char command = 0x00;
	unsigned long spot = offset;

	std::vector<int> returnBackOffset;
	std::vector<int> returnBackAlternateChoices;

	std::vector<SngNoteInfo> pendingChannelNotes;

	std::vector<SngTimeValue> instrumentByAbsoluteTime;
	std::vector<SngTimeValue> effectByAbsoluteTime;
	std::vector<SngTimeValue> volumeByAbsoluteTime;
	std::vector<SngTimeValue> panByAbsoluteTime;
	std::vector<SngTimeValue> pitchBendByAbsoluteTime;
	std::vector<SngTimeValue> vibratoByAbsoluteTime;

loopAgain:
	while ((command != 0xFF) && (spot < end))
	{
		command = inputMID[spot];

		fprintfIfDebug(outFileDebug, "CHN %08X Time: %08X CMD:      %02X", spot, absoluteTime, command);

		IncrementSpot(spot, 1, coverage);

		if (command == 0xFF)
		{
			fprintfIfDebug(outFileDebug, " END\n");
			break;
		}
		else if (command == 0xFE)
		{
			fprintfIfDebug(outFileDebug, " RETURN\n");
			break;
		}
		else if ((command >= 0x0) && (command < 0x10))
		{
			if ((gameStyle == EADMUSICSTYLEZELDA) || (gameStyle == EADMUSICSTYLEMARIO))
			{
				fprintfIfDebug(outFileDebug, " Delay");
				fprintfIfDebug(outFileDebug, " %02X (%d)", command, command);

				absoluteTime += command;
			}
			else
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
		}
		else if ((command >= 0x10) && (command < 0x20))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x20) && (command < 0x30))
		{
			// 1080
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);

			fprintfIfDebug(outFileDebug, " Change Channel %02X to Offset?", command & 0xF);
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);

			int callChannel = command & 0xF;

			fprintfIfDebug(outFileDebug, "\n");
			ParseEADMusicChannel(gameStyle, outFileDebug, inputMID, end, offset, callChannel, absoluteTime, numberInstruments, tempoPositions, outputNotes, noteUniqueId, currentInstrument, currentPan, currentVolume, currentPitchBend, currentEffect, jumpStarts, jumpsTaken, attemptJumps, coverage, channelStarted, currentVibrato, isPreview);
		}
		else if ((command >= 0x30) && (command < 0x40))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x40) && (command < 0x50))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x50) && (command < 0x60))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x60) && (command < 0x70))
		{
			if (gameStyle == EADMUSICSTYLESTARFOX)
			{
				fprintfIfDebug(outFileDebug, " Delay");
				fprintfIfDebug(outFileDebug, " %02X (%d) [Real delay %02X (%d)]", command, command, command & 0xF, command & 0xF);

				absoluteTime += (command & 0xF);
			}
			else
			{
				fprintfIfDebug(outFileDebug, " ?");
			}
		}
		else if ((command >= 0x70) && (command < 0x78))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x78) && (command < 0x7C))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			int layer = command & 0x3;
			fprintfIfDebug(outFileDebug, " Track Layer Offset %02X", (command & 0x3));
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			fprintfIfDebug(outFileDebug, "\n");

			IncrementSpot(spot, 2, coverage);

			if (offset > 0)
			{
				std::vector<unsigned long> jumpStartsTrack;
				std::vector<unsigned long> jumpsTakenTrack;

				ParseEADMusicTrackLayer(gameStyle, outFileDebug, inputMID, end, spot + offset, channel, layer, absoluteTime, numberInstruments, tempoPositions, pendingChannelNotes, noteUniqueId, jumpStartsTrack, jumpsTakenTrack, attemptJumps, coverage, channelStarted[channel], isPreview);
			}
		}
		else if ((command >= 0x7C) && (command < 0x80))
		{
			// Looks like placeholder, defaults back to 0 in Zelda
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			int layer = 0;
			fprintfIfDebug(outFileDebug, " Track Layer Offset %02X", layer);
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			fprintfIfDebug(outFileDebug, "\n");

			IncrementSpot(spot, 2, coverage);

			if (offset > 0)
			{
				std::vector<unsigned long> jumpStartsTrack;
				std::vector<unsigned long> jumpsTakenTrack;

				ParseEADMusicTrackLayer(gameStyle, outFileDebug, inputMID, end, spot + offset, channel, layer, absoluteTime, numberInstruments, tempoPositions, pendingChannelNotes, noteUniqueId, jumpStartsTrack, jumpsTakenTrack, attemptJumps, coverage, channelStarted[channel], isPreview);
			}
		}
		else if ((command >= 0x80) && (command < 0x84))
		{
			// & 0x3 Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x84) && (command < 0x88))
		{
			// Default 0 placeholder Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((gameStyle == EADMUSICSTYLEZELDA) && ((command >= 0x88) && (command < 0x8C)))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			int layer = command & 0x3;
			fprintfIfDebug(outFileDebug, " Track Layer %02X", (command & 0x3));
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			fprintfIfDebug(outFileDebug, "\n");

			std::vector<unsigned long> jumpStartsTrack;
			std::vector<unsigned long> jumpsTakenTrack;

			ParseEADMusicTrackLayer(gameStyle, outFileDebug, inputMID, end, offset, channel, layer, absoluteTime, numberInstruments, tempoPositions, pendingChannelNotes, noteUniqueId, jumpStartsTrack, jumpsTakenTrack, attemptJumps, coverage, channelStarted[channel], isPreview);

			IncrementSpot(spot, 2, coverage);
		}
		else if ((gameStyle == EADMUSICSTYLEZELDA) && ((command >= 0x8C) && (command < 0x90)))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			int layer = 0;
			fprintfIfDebug(outFileDebug, " Track Layer %02X", 0);
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			fprintfIfDebug(outFileDebug, "\n");

			std::vector<unsigned long> jumpStartsTrack;
			std::vector<unsigned long> jumpsTakenTrack;

			ParseEADMusicTrackLayer(gameStyle, outFileDebug, inputMID, end, offset, channel, layer, absoluteTime, numberInstruments, tempoPositions, pendingChannelNotes, noteUniqueId, jumpStartsTrack, jumpsTakenTrack, attemptJumps, coverage, channelStarted[channel], isPreview);

			IncrementSpot(spot, 2, coverage);
		}
		else if ((gameStyle == EADMUSICSTYLEMARIO) && ((command >= 0x88) && (command < 0x90)))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if ((gameStyle == EADMUSICSTYLESTARFOX) && ((command >= 0x88) && (command < 0x90)))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if ((gameStyle == EADMUSICSTYLEZELDA) && ((command >= 0x90) && (command < 0x94)))
		{
			// & 0x3 Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((gameStyle == EADMUSICSTYLEZELDA) && ((command >= 0x94) && (command < 0x98)))
		{
			// Default 0 Placeholder Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}

		else if ((gameStyle == EADMUSICSTYLEMARIO) && ((command >= 0x90) && (command < 0x94)))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			int layer = command & 0x3;
			fprintfIfDebug(outFileDebug, " Track Layer %02X", (command & 0x3));
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			fprintfIfDebug(outFileDebug, "\n");

			std::vector<unsigned long> jumpStartsTrack;
			std::vector<unsigned long> jumpsTakenTrack;

			ParseEADMusicTrackLayer(gameStyle, outFileDebug, inputMID, end, offset, channel, layer, absoluteTime, numberInstruments, tempoPositions, pendingChannelNotes, noteUniqueId, jumpStartsTrack, jumpsTakenTrack, attemptJumps, coverage, channelStarted[channel], isPreview);

			IncrementSpot(spot, 2, coverage);
		}
		else if ((gameStyle == EADMUSICSTYLEMARIO) && ((command >= 0x94) && (command < 0x98)))
		{
			// Not sure just put in
			IncrementSpot(spot, 2, coverage);
		}
		else if ((gameStyle == EADMUSICSTYLEMARIO) && ((command >= 0x98) && (command < 0x9C)))
		{
			// & 0x3 Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((gameStyle == EADMUSICSTYLEZELDA) && ((command >= 0x98) && (command < 0x9C)))
		{
			// & 0x3 Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((gameStyle == EADMUSICSTYLEMARIO) && ((command >= 0x9C) && (command < 0xA0)))
		{
			// Default 0 Placeholder Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((gameStyle == EADMUSICSTYLEZELDA) && ((command >= 0x9C) && (command < 0xA0)))
		{
			// Default 0 Placeholder Track Layer
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((gameStyle == EADMUSICSTYLESTARFOX) && ((command >= 0x90) && (command < 0xA0)))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			int layer = command & 0xF;
			fprintfIfDebug(outFileDebug, " Track Layer %02X", (command & 0xF));
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			fprintfIfDebug(outFileDebug, "\n");

			std::vector<unsigned long> jumpStartsTrack;
			std::vector<unsigned long> jumpsTakenTrack;

			ParseEADMusicTrackLayer(gameStyle, outFileDebug, inputMID, end, offset, channel, layer, absoluteTime, numberInstruments, tempoPositions, pendingChannelNotes, noteUniqueId, jumpStartsTrack, jumpsTakenTrack, attemptJumps, coverage, channelStarted[channel], isPreview);

			IncrementSpot(spot, 2, coverage);
		}
		else if ((command >= 0xA0) && (command < 0xB0))
		{
			// NOOP in Zelda
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xB0)
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			fprintfIfDebug(outFileDebug, " Add to 80100000 and ?");
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			IncrementSpot(spot, 2, coverage);

			break;
		}
		else if (command == 0xB1)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xB2)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xB3)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xB4)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xB5)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xB6)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xB7)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xB8)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xB9)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xBA)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xBB)
		{
			fprintfIfDebug(outFileDebug, " Control Change?");
			fprintfIfDebug(outFileDebug, " %02X %02X%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xBC)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xBD)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X %02X%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2], inputMID[spot+3]);
			
			IncrementSpot(spot, 4, coverage);
		}
		else if (command == 0xBE)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xBF)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xC0)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xC1)
		{
			fprintfIfDebug(outFileDebug, " Instrument");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			if (isPreview)
			{
				if (inputMID[spot] >= 0x7F)
				{
					// Prevent preview issues with soundfont in soundbank tool
					break;
				}
			}

			currentInstrument[channel] = inputMID[spot];

			SngTimeValue pair;
			pair.value = currentInstrument[channel];
			pair.startAbsoluteTime = absoluteTime;
			pair.endAbsoluteTime = absoluteTime;
			instrumentByAbsoluteTime.push_back(pair);

			if (currentInstrument[channel] >= numberInstruments)
				numberInstruments = (currentInstrument[channel] + 1);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xC2)
		{
			if (outFileDebug)
			{
				fflush(outFileDebug);
			}
			// Really uses play values and external to set...can probably not ever get this right sadly.
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);

			std::vector<unsigned short> values;

			fprintfIfDebug(outFileDebug, " Set Jump Table Channel");
			fprintfIfDebug(outFileDebug, " Offset with Data %02X%02X", inputMID[spot], inputMID[spot+1]);

			unsigned short previousValueTemp = 0;
			while (true)
			{
				unsigned short  valueTemp = CharArrayToShort(&inputMID[offset + (values.size() * 2)]);

				if ((valueTemp >= end) || (valueTemp <= previousValueTemp))
					break;

				fprintfIfDebug(outFileDebug, " Value %02X %04X", (values.size()), valueTemp);
				values.push_back(valueTemp);

				previousValueTemp = valueTemp;
			}

			IncrementSpot(spot, 2, coverage);

			fprintfIfDebug(outFileDebug, "\n");

			for (int v = 0; v < values.size(); v++)
			{
				if ((offset + (v * 2 + 1)) < end)
				{
					coverage[offset + (v * 2)] = 0x00;
					coverage[offset + (v * 2) + 1] = 0x00;

					ParseEADMusicChannel(gameStyle, outFileDebug, inputMID, end, values[v], channel, absoluteTime, numberInstruments, tempoPositions, outputNotes, noteUniqueId, currentInstrument, currentPan, currentVolume, currentPitchBend, currentEffect, jumpStarts, jumpsTaken, attemptJumps, coverage, channelStarted, currentVibrato, isPreview);
				}
			}
		}
		else if (command == 0xC3)
		{
			fprintfIfDebug(outFileDebug, " Channel End (Disable Velocity/Duration on Notes)");
			channelStarted[channel] = false;
		}
		else if (command == 0xC4)
		{
			fprintfIfDebug(outFileDebug, " Channel Start (Enable Velocity/Duration on Notes)");
			channelStarted[channel] = true;
		}
		else if (command == 0xC5)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xC6)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xC7)
		{
			fprintfIfDebug(outFileDebug, " Write Value to Memory");
			fprintfIfDebug(outFileDebug, " %02X %02X%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xC8)
		{
			fprintfIfDebug(outFileDebug, " Subtract Play Value - Value");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xC9)
		{
			fprintfIfDebug(outFileDebug, " And Play Value and Value");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xCA)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xCB)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xCC)
		{
			fprintfIfDebug(outFileDebug, " Set Play Value");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xCD)
		{
			fprintfIfDebug(outFileDebug, " ? [Value is channel number]");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xCE)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xCF)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xD0)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD1)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD2)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD3)
		{
			fprintfIfDebug(outFileDebug, " Pitch Bend");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			// signed char to 00-7F range
			currentPitchBend[channel] = (signed char)inputMID[spot];
			if (currentPitchBend[channel] == 0)
				currentPitchBend[channel] = 0x40;
			else if (currentPitchBend[channel] < 0)
				currentPitchBend[channel] = currentPitchBend[channel] / 2 + 0x40;
			else if (currentPitchBend[channel] > 0)
				currentPitchBend[channel] = currentPitchBend[channel] / 2 + 0x40;
			
			SngTimeValue pair;
			pair.value = currentPitchBend[channel];
			pair.startAbsoluteTime = absoluteTime;
			pair.endAbsoluteTime = absoluteTime;
			pitchBendByAbsoluteTime.push_back(pair);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD4)
		{
			fprintfIfDebug(outFileDebug, " Reverb");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			currentEffect[channel] = inputMID[spot] / 2;

			SngTimeValue pair;
			pair.value = currentEffect[channel];
			pair.startAbsoluteTime = absoluteTime;
			pair.endAbsoluteTime = absoluteTime;
			effectByAbsoluteTime.push_back(pair);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD5)
		{
			fprintfIfDebug(outFileDebug, " Reverb");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD6)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD7)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD8)
		{
			fprintfIfDebug(outFileDebug, " Vibrato");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			currentVibrato[channel] = inputMID[spot];
			
			SngTimeValue pair;
			pair.value = currentVibrato[channel];
			pair.startAbsoluteTime = absoluteTime;
			pair.endAbsoluteTime = absoluteTime;
			vibratoByAbsoluteTime.push_back(pair);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD9)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xDA)
		{
			// Absolute offset
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xDB)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xDC)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xDD)
		{
			fprintfIfDebug(outFileDebug, " Pan");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			currentPan[channel] = inputMID[spot];

			SngTimeValue pair;
			pair.value = currentPan[channel];
			pair.startAbsoluteTime = absoluteTime;
			pair.endAbsoluteTime = absoluteTime;
			panByAbsoluteTime.push_back(pair);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xDE)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xDF)
		{
			fprintfIfDebug(outFileDebug, " Volume");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			currentVolume[channel] = inputMID[spot];

			SngTimeValue pair;
			pair.value = currentVolume[channel];
			pair.startAbsoluteTime = absoluteTime;
			pair.endAbsoluteTime = absoluteTime;
			volumeByAbsoluteTime.push_back(pair);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE0)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE1)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);

			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xE2)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X %02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xE3)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE4)
		{
			fprintfIfDebug(outFileDebug, " Use Play Value to Offset into Jump Table (from last Command C2)");
		}
		else if (command == 0xE5)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE6)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE7)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xE8)
		{
			// NUD-DMGJ
			//E800000E003F7F0000
			fprintfIfDebug(outFileDebug, " Envelope");
			fprintfIfDebug(outFileDebug, " %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d)", 
			inputMID[spot], inputMID[spot],
			inputMID[spot+1], inputMID[spot+1],
			inputMID[spot+2], inputMID[spot+2],
			inputMID[spot+3], inputMID[spot+3],
			inputMID[spot+4], inputMID[spot+4],
			inputMID[spot+5], inputMID[spot+5],
			inputMID[spot+6], inputMID[spot+6],
			inputMID[spot+7], inputMID[spot+7]
			);

			IncrementSpot(spot, 8, coverage);
		}
		else if (command == 0xE9)
		{
			fprintfIfDebug(outFileDebug, " Priority");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xEA)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xEB)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xEC)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xED)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xEE)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xEF)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X %02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xF0)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xF1)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xF2)
		{
			fprintfIfDebug(outFileDebug, " Go to Relative Offset if Play Value < 0");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			int relativeOffset = (signed char)inputMID[spot];
			IncrementSpot(spot, 1, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 2);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(relativeOffset);
						spot = spot + relativeOffset;
					}
				}
			}
		}
		else if (command == 0xF3)
		{
			fprintfIfDebug(outFileDebug, " Go to Relative Offset if Play Value == 0");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			int relativeOffset = (signed char)inputMID[spot];
			IncrementSpot(spot, 1, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 2);
				}
				else
				{			
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(relativeOffset);
						spot = spot + relativeOffset;
					}
				}
			}
		}
		else if (command == 0xF4)
		{
			fprintfIfDebug(outFileDebug, " Go to  relative offset");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], (signed char)inputMID[spot]);

			int relativeOffset = (signed char)inputMID[spot];
			IncrementSpot(spot, 1, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 2);
					break;
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(relativeOffset);
						spot = spot + relativeOffset;
					}
					else
					{
						break;
					}
				}
			}
		}
		else if (command == 0xF5)
		{
			fprintfIfDebug(outFileDebug, " Go to absolute offset if Play Value >= 0");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot =  absoluteOffset;
					}
				}
			}
		}
		else if (command == 0xF6)
		{
			fprintfIfDebug(outFileDebug, " Restart Song");

			if (attemptJumps)
			{
				int startSpot = 0;

				if (std::find(jumpsTaken.begin(), jumpsTaken.end(), startSpot) == jumpsTaken.end())
				{
					fprintfIfDebug(outFileDebug, "...Taking\n");
					jumpsTaken.push_back(startSpot);
					spot = startSpot;
				}
			}
		}
		else if (command == 0xF7)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xF8)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xF9)
		{
			fprintfIfDebug(outFileDebug, " Go to Absolute Offset if Play Value < 0");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot =  absoluteOffset;
					}
				}
			}
		}
		else if (command == 0xFA)
		{
			fprintfIfDebug(outFileDebug, " Go to Absolute Offset if Play Value == 0");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot = absoluteOffset;
					}
				}
			}
		}
		else if (command == 0xFB)
		{
			fprintfIfDebug(outFileDebug, " Go to absolute offset");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			
			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
					break;
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot = absoluteOffset;
					}
					else
					{
						break;
					}
				}
			}
		}
		else if (command == 0xFC)
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			fprintfIfDebug(outFileDebug, " Jump and Link");
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			IncrementSpot(spot, 2, coverage);
			returnBackOffset.push_back(spot);

			spot = offset;
		}
		else if (command == 0xFD)
		{
			fprintfIfDebug(outFileDebug, " Delay");
			
			int timestamp = ReadEADMusicTimeValue(inputMID, spot, coverage);
			if (timestamp < 0x80)
				fprintfIfDebug(outFileDebug, " %02X (%d)", timestamp, timestamp);
			else
				fprintfIfDebug(outFileDebug, " %02X%02X - %04X (%d)", inputMID[spot-2], inputMID[spot+1-2], timestamp, timestamp);

			absoluteTime += timestamp;
		}
		else
		{
			fprintfIfDebug(outFileDebug, " UNKNOWN");
			return;
		}

		fprintfIfDebug(outFileDebug, "\n");
	}

	if (returnBackAlternateChoices.size() > 0)
	{
		// Return from Jump and Link
		spot = returnBackAlternateChoices.back();
		returnBackAlternateChoices.pop_back();
		command = 0x00;
		
		fprintfIfDebug(outFileDebug, "... Taking Alternate Path to %08X\n", spot);
		goto loopAgain;
	}

	if (returnBackOffset.size() > 0)
	{
		// Return from Jump and Link
		spot = returnBackOffset.back();
		returnBackOffset.pop_back();
		command = 0x00;
		
		fprintfIfDebug(outFileDebug, "... Returning from Subroutine to %08X\n", spot);
		goto loopAgain;
	}

	int maxAbsoluteTime = 0;
	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		if (pendingChannelNotes[x].endAbsoluteTime > maxAbsoluteTime)
			maxAbsoluteTime = pendingChannelNotes[x].endAbsoluteTime;
	}

	std::sort(pendingChannelNotes.begin(), pendingChannelNotes.end(), sngSortByStartTime());

	if (pendingChannelNotes.size() > 0)
	{
		if (instrumentByAbsoluteTime.size() == 0)
		{
			SngTimeValue pair;
			pair.value = currentInstrument[channel];
			pair.startAbsoluteTime = startAbsoluteTime;
			pair.endAbsoluteTime = startAbsoluteTime;
			instrumentByAbsoluteTime.push_back(pair);
		}

		if (effectByAbsoluteTime.size() == 0)
		{
			SngTimeValue pair;
			pair.value = currentEffect[channel];
			pair.startAbsoluteTime = startAbsoluteTime;
			pair.endAbsoluteTime = startAbsoluteTime;
			effectByAbsoluteTime.push_back(pair);
		}

		if (volumeByAbsoluteTime.size() == 0)
		{
			SngTimeValue pair;
			pair.value = currentVolume[channel];
			pair.startAbsoluteTime = startAbsoluteTime;
			pair.endAbsoluteTime = startAbsoluteTime;
			volumeByAbsoluteTime.push_back(pair);
		}

		if (panByAbsoluteTime.size() == 0)
		{
			SngTimeValue pair;
			pair.value = currentPan[channel];
			pair.startAbsoluteTime = startAbsoluteTime;
			pair.endAbsoluteTime = startAbsoluteTime;
			panByAbsoluteTime.push_back(pair);
		}

		if (pitchBendByAbsoluteTime.size() == 0)
		{
			SngTimeValue pair;
			pair.value = currentPitchBend[channel];
			pair.startAbsoluteTime = startAbsoluteTime;
			pair.endAbsoluteTime = startAbsoluteTime;
			pitchBendByAbsoluteTime.push_back(pair);
		}

		if (vibratoByAbsoluteTime.size() == 0)
		{
			SngTimeValue pair;
			pair.value = currentVibrato[channel];
			pair.startAbsoluteTime = startAbsoluteTime;
			pair.endAbsoluteTime = startAbsoluteTime;
			vibratoByAbsoluteTime.push_back(pair);
		}
	}

	if (instrumentByAbsoluteTime.size() > 0)
	{
		if (instrumentByAbsoluteTime.size() > 1)
		{
			for (int x = (instrumentByAbsoluteTime.size() - 2); x >= 0; x--)
			{
				instrumentByAbsoluteTime[x].endAbsoluteTime = instrumentByAbsoluteTime[x+1].startAbsoluteTime;
			}
		}

		instrumentByAbsoluteTime[instrumentByAbsoluteTime.size() - 1].endAbsoluteTime = maxAbsoluteTime;
	}

	if (effectByAbsoluteTime.size() > 0)
	{
		if (effectByAbsoluteTime.size() > 1)
		{
			for (int x = (effectByAbsoluteTime.size() - 2); x >= 0; x--)
			{
				effectByAbsoluteTime[x].endAbsoluteTime = effectByAbsoluteTime[x+1].startAbsoluteTime;
			}
		}

		effectByAbsoluteTime[effectByAbsoluteTime.size() - 1].endAbsoluteTime = maxAbsoluteTime;
	}

	if (volumeByAbsoluteTime.size() > 0)
	{
		if (volumeByAbsoluteTime.size() > 1)
		{
			for (int x = (volumeByAbsoluteTime.size() - 2); x >= 0; x--)
			{
				volumeByAbsoluteTime[x].endAbsoluteTime = volumeByAbsoluteTime[x+1].startAbsoluteTime;
			}
		}

		volumeByAbsoluteTime[volumeByAbsoluteTime.size() - 1].endAbsoluteTime = maxAbsoluteTime;
	}

	if (panByAbsoluteTime.size() > 0)
	{
		if (panByAbsoluteTime.size() > 1)
		{
			for (int x = (panByAbsoluteTime.size() - 2); x >= 0; x--)
			{
				panByAbsoluteTime[x].endAbsoluteTime = panByAbsoluteTime[x+1].startAbsoluteTime;
			}
		}

		panByAbsoluteTime[panByAbsoluteTime.size() - 1].endAbsoluteTime = maxAbsoluteTime;
	}

	if (pitchBendByAbsoluteTime.size() > 0)
	{
		if (pitchBendByAbsoluteTime.size() > 1)
		{
			for (int x = (pitchBendByAbsoluteTime.size() - 2); x >= 0; x--)
			{
				pitchBendByAbsoluteTime[x].endAbsoluteTime = pitchBendByAbsoluteTime[x+1].startAbsoluteTime;
			}
		}

		pitchBendByAbsoluteTime[pitchBendByAbsoluteTime.size() - 1].endAbsoluteTime = maxAbsoluteTime;
	}

	if (vibratoByAbsoluteTime.size() > 0)
	{
		if (vibratoByAbsoluteTime.size() > 1)
		{
			for (int x = (vibratoByAbsoluteTime.size() - 2); x >= 0; x--)
			{
				vibratoByAbsoluteTime[x].endAbsoluteTime = vibratoByAbsoluteTime[x+1].startAbsoluteTime;
			}
		}

		vibratoByAbsoluteTime[vibratoByAbsoluteTime.size() - 1].endAbsoluteTime = maxAbsoluteTime;
	}



	// Add in Volume
	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		for (int y = 0; y < volumeByAbsoluteTime.size(); y++)
		{
			// Volume To Left/Equal Ending in Middle
			if (
				(pendingChannelNotes[x].startAbsoluteTime >= volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (pendingChannelNotes[x].volume != volumeByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].startAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.volume = volumeByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//Volume In Middle, Ending to Right
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= volumeByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (pendingChannelNotes[x].volume != volumeByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.volume = volumeByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// Volume Completely Inside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (pendingChannelNotes[x].volume != volumeByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];
					SngNoteInfo newNoteInfo2 = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = volumeByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.volume = volumeByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = volumeByAbsoluteTime[y].endAbsoluteTime;

					pendingChannelNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// Volume Completely Outside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime >= volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < volumeByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > volumeByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= volumeByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				pendingChannelNotes[x].volume = volumeByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}
	


	// Add in Pitch Bend
	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		for (int y = 0; y < pitchBendByAbsoluteTime.size(); y++)
		{
			// PitchBend To Left/Equal Ending in Middle
			if (
				(pendingChannelNotes[x].startAbsoluteTime >= pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (pendingChannelNotes[x].pitchBend != pitchBendByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].startAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.pitchBend = pitchBendByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//PitchBend In Middle, Ending to Right
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= pitchBendByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (pendingChannelNotes[x].pitchBend != pitchBendByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.pitchBend = pitchBendByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// PitchBend Completely Inside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (pendingChannelNotes[x].pitchBend != pitchBendByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];
					SngNoteInfo newNoteInfo2 = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = pitchBendByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.pitchBend = pitchBendByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = pitchBendByAbsoluteTime[y].endAbsoluteTime;

					pendingChannelNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// PitchBend Completely Outside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime >= pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < pitchBendByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > pitchBendByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= pitchBendByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				pendingChannelNotes[x].pitchBend = pitchBendByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}

	// Add in Pan
	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		for (int y = 0; y < panByAbsoluteTime.size(); y++)
		{
			// Pan To Left/Equal Ending in Middle
			if (
				(pendingChannelNotes[x].startAbsoluteTime >= panByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < panByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > panByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > panByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (pendingChannelNotes[x].pan != panByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].startAbsoluteTime = panByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = panByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.pan = panByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//Pan In Middle, Ending to Right
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < panByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < panByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > panByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= panByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (pendingChannelNotes[x].pan != panByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = panByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = panByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.pan = panByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// Pan Completely Inside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < panByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < panByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > panByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > panByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (pendingChannelNotes[x].pan != panByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];
					SngNoteInfo newNoteInfo2 = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = panByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = panByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = panByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.pan = panByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = panByAbsoluteTime[y].endAbsoluteTime;

					pendingChannelNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// Pan Completely Outside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime >= panByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < panByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > panByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= panByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				pendingChannelNotes[x].pan = panByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}

	// Add in Effect
	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		for (int y = 0; y < effectByAbsoluteTime.size(); y++)
		{
			// Effect To Left/Equal Ending in Middle
			if (
				(pendingChannelNotes[x].startAbsoluteTime >= effectByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < effectByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > effectByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > effectByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (pendingChannelNotes[x].effect != effectByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].startAbsoluteTime = effectByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = effectByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.effect = effectByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//Effect In Middle, Ending to Right
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < effectByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < effectByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > effectByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= effectByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (pendingChannelNotes[x].effect != effectByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = effectByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = effectByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.effect = effectByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// Effect Completely Inside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < effectByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < effectByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > effectByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > effectByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (pendingChannelNotes[x].effect != effectByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];
					SngNoteInfo newNoteInfo2 = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = effectByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = effectByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = effectByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.effect = effectByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = effectByAbsoluteTime[y].endAbsoluteTime;

					pendingChannelNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// Effect Completely Outside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime >= effectByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < effectByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > effectByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= effectByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				pendingChannelNotes[x].effect = effectByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}

	// Add in Instrument
	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		for (int y = 0; y < instrumentByAbsoluteTime.size(); y++)
		{
			// Instrument To Left/Equal Ending in Middle
			if (
				(pendingChannelNotes[x].startAbsoluteTime >= instrumentByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < instrumentByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > instrumentByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > instrumentByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (pendingChannelNotes[x].instrument != instrumentByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].startAbsoluteTime = instrumentByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = instrumentByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.instrument = instrumentByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//Instrument In Middle, Ending to Right
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < instrumentByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < instrumentByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > instrumentByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= instrumentByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (pendingChannelNotes[x].instrument != instrumentByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = instrumentByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = instrumentByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.instrument = instrumentByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// Instrument Completely Inside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < instrumentByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < instrumentByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > instrumentByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > instrumentByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (pendingChannelNotes[x].instrument != instrumentByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];
					SngNoteInfo newNoteInfo2 = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = instrumentByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = instrumentByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = instrumentByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.instrument = instrumentByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = instrumentByAbsoluteTime[y].endAbsoluteTime;

					pendingChannelNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// Instrument Completely Outside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime >= instrumentByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < instrumentByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > instrumentByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= instrumentByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				pendingChannelNotes[x].instrument = instrumentByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}

	// Add in Vibrato
	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		for (int y = 0; y < vibratoByAbsoluteTime.size(); y++)
		{
			// Vibrato To Left/Equal Ending in Middle
			if (
				(pendingChannelNotes[x].startAbsoluteTime >= vibratoByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < vibratoByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > vibratoByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > vibratoByAbsoluteTime[y].startAbsoluteTime)
				)
			{
				//Split at Middle, Apply to Beginning

				if (pendingChannelNotes[x].vibrato != vibratoByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].startAbsoluteTime = vibratoByAbsoluteTime[y].endAbsoluteTime;
					
					newNoteInfo.endAbsoluteTime = vibratoByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.vibrato = vibratoByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
				
			}
			//Vibrato In Middle, Ending to Right
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < vibratoByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < vibratoByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > vibratoByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= vibratoByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				//Split at Middle, Apply to End
				if (pendingChannelNotes[x].vibrato != vibratoByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = vibratoByAbsoluteTime[y].startAbsoluteTime;
					
					newNoteInfo.startAbsoluteTime = vibratoByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.vibrato = vibratoByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);

					x--;
					break;
				}
			}
			// Vibrato Completely Inside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime < vibratoByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < vibratoByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > vibratoByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > vibratoByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Split at Mid-Left and Mid-Right
				if (pendingChannelNotes[x].vibrato != vibratoByAbsoluteTime[y].value)
				{
					SngNoteInfo newNoteInfo = pendingChannelNotes[x];
					SngNoteInfo newNoteInfo2 = pendingChannelNotes[x];

					pendingChannelNotes[x].endAbsoluteTime = vibratoByAbsoluteTime[y].startAbsoluteTime;


					

					newNoteInfo.startAbsoluteTime = vibratoByAbsoluteTime[y].startAbsoluteTime;
					newNoteInfo.endAbsoluteTime = vibratoByAbsoluteTime[y].endAbsoluteTime;
					newNoteInfo.vibrato = vibratoByAbsoluteTime[y].value;

					pendingChannelNotes.push_back(newNoteInfo);
					
					newNoteInfo2.startAbsoluteTime = vibratoByAbsoluteTime[y].endAbsoluteTime;

					pendingChannelNotes.push_back(newNoteInfo2);

					x--;
					break;
				}
			}
			// Vibrato Completely Outside
			else if (
				(pendingChannelNotes[x].startAbsoluteTime >= vibratoByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].startAbsoluteTime < vibratoByAbsoluteTime[y].endAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime > vibratoByAbsoluteTime[y].startAbsoluteTime)
				&& (pendingChannelNotes[x].endAbsoluteTime <= vibratoByAbsoluteTime[y].endAbsoluteTime)
				)
			{
				// Apply to whole
				pendingChannelNotes[x].vibrato = vibratoByAbsoluteTime[y].value;
			}
			// No Overlap
			else
			{

			}
		}
	}

	for (int x = 0; x < pendingChannelNotes.size(); x++)
	{
		outputNotes.push_back(pendingChannelNotes[x]);
	}
}

void CMidiParse::ParseEADMusicSequence(int gameStyle, FILE* outFileDebug, unsigned char* inputMID, int offset ,int end, unsigned long& absoluteTime, std::vector<unsigned long>& jumpStarts, std::vector<unsigned long>& jumpsTaken, bool attemptJumps,
							 int& numberInstruments, std::vector<TimeAndValue>& tempoPositions, std::vector<SngNoteInfo>& outputNotes,
							 int& noteUniqueId,
							int currentInstrument[0x10], int currentPan[0x10], int currentVolume[0x10], int currentPitchBend[0x10], int currentReverb[0x10], bool& hitConditional, unsigned char* coverage, bool channelStarted[0x10],
							int currentVibrato[0x10], bool isPreview
							 )
{
	unsigned char command = 0x00;
	unsigned long spot = offset;

	fprintfIfDebug(outFileDebug, "-------------------SEQUENCE %08X-------------------\n", offset);

	std::vector<int> returnBackOffset;
	std::vector<int> returnBackAlternateChoices;

loopAgain:
	while ((command != 0xFF) && (spot < end))
	{
		command = inputMID[spot];

		fprintfIfDebug(outFileDebug, "SEQ %08X Time: %08X CMD:      %02X", spot, absoluteTime, command);

		IncrementSpot(spot, 1, coverage);

		if (command == 0xFF)
		{
			fprintfIfDebug(outFileDebug, " END\n");
			break;
		}
		else if (command == 0xFE)
		{
			fprintfIfDebug(outFileDebug, " RETURN\n");
			break;
		}
		else if ((command >= 0x0) && (command < 0x10))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x10) && (command < 0x40))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x40) && (command < 0x50))
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if ((command >= 0x50) && (command < 0x60))
		{
			fprintfIfDebug(outFileDebug, " Set Play Value to (Play Value - Channel Value %02X)", command & 0xF);
		}
		else if ((command >= 0x60) && (command < 0x70))
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);
		}
		else if ((command >= 0x70) && (command < 0x80))
		{
			fprintfIfDebug(outFileDebug, " Set Channel Value %02X to Play Value", command & 0xF);
		}
		else if ((command >= 0x80) && (command < 0x90))
		{
			// READMusic Values Set 1
			// Write to Values Set 2 if >= 2
			// else Write FFFF (-1)

			fprintfIfDebug(outFileDebug, " Set Play Value to Channel Value %02X If Channel >= 2 Else -1", command & 0xF);
		}
		else if ((command >= 0x90) && (command < 0xA0))
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			unsigned char channel = command & 0xF;
			fprintfIfDebug(outFileDebug, " Channel Pointer Channel %X", command & 0xF);
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			IncrementSpot(spot, 2, coverage);

			fprintfIfDebug(outFileDebug, "\n");

			std::vector<unsigned long> jumpStartsChannel;
			std::vector<unsigned long> jumpsTakenChannel;

			ParseEADMusicChannel(gameStyle, outFileDebug, inputMID, end, offset, channel, absoluteTime, numberInstruments, tempoPositions, outputNotes, noteUniqueId, currentInstrument, currentPan, currentVolume, currentPitchBend, currentReverb, jumpStartsChannel, jumpsTakenChannel, attemptJumps, coverage, channelStarted, currentVibrato, isPreview);
		}
		else if ((command >= 0xA0) && (command < 0xB0))
		{
			// Not sure about this
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			unsigned char channel = command & 0xF;
			fprintfIfDebug(outFileDebug, " Channel Pointer Offset Channel %X", command & 0xF);
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			
			IncrementSpot(spot, 2, coverage);

			fprintfIfDebug(outFileDebug, "\n");

			std::vector<unsigned long> jumpStartsChannel;
			std::vector<unsigned long> jumpsTakenChannel;

			ParseEADMusicChannel(gameStyle, outFileDebug, inputMID, end, spot + offset, channel, absoluteTime, numberInstruments, tempoPositions, outputNotes, noteUniqueId, currentInstrument, currentPan, currentVolume, currentPitchBend, currentReverb, jumpStartsChannel, jumpsTakenChannel, attemptJumps, coverage, channelStarted, currentVibrato, isPreview);
		}
		else if ((command >= 0xB0) && (command < 0xC0))
		{
			unsigned char index = inputMID[spot];
			unsigned short offset = ((inputMID[spot + 1] << 8) | inputMID[spot+2]);

			fprintfIfDebug(outFileDebug, " Load And Goto Sequence at Offset");
			fprintfIfDebug(outFileDebug, " %02X At %02X%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			
			IncrementSpot(spot, 3, coverage);

			fprintfIfDebug(outFileDebug, "\n");

			break;
		}
		else if (command == 0xC0)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xC1)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xC2)
		{
			// Really uses play values and external to set...can probably not ever get this right sadly.
			//DMGJ 0x10
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);

			std::vector<unsigned short> values;

			fprintfIfDebug(outFileDebug, " Set Jump Table Sequence");
			fprintfIfDebug(outFileDebug, " Offset with Data %02X%02X", inputMID[spot], inputMID[spot+1]);

			unsigned short previousValueTemp = 0;
			while (true)
			{
				unsigned short  valueTemp = CharArrayToShort(&inputMID[offset + (values.size() * 2)]);

				if ((valueTemp >= end) || (valueTemp <= previousValueTemp))
					break;

				fprintfIfDebug(outFileDebug, " Value %02X %04X", (values.size()), valueTemp);
				values.push_back(valueTemp);

				previousValueTemp = valueTemp;
			}

			IncrementSpot(spot, 2, coverage);

			fprintfIfDebug(outFileDebug, "\n");

			for (int v = 0; v < values.size(); v++)
			{
				if ((offset + (v * 2 + 1)) < end)
				{
					coverage[offset + (v * 2)] = 0x00;
					coverage[offset + (v * 2) + 1] = 0x00;

					ParseEADMusicSequence(gameStyle, outFileDebug, inputMID, values[v], end, absoluteTime, jumpStarts, jumpsTaken, attemptJumps, numberInstruments, tempoPositions, outputNotes, noteUniqueId, currentInstrument, currentPan, currentVolume, currentPitchBend, currentReverb, hitConditional, coverage, channelStarted, currentVibrato, isPreview);
				}
			}
		}
		else if (command == 0xC3)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xC4)
		{
			fprintfIfDebug(outFileDebug, " DONE?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			
			IncrementSpot(spot, 2, coverage);

			fprintfIfDebug(outFileDebug, "\n");
			break;
		}
		else if (command == 0xC5)
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			
			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xC6)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xC7)
		{
			fprintfIfDebug(outFileDebug, " ? Pitch Related");
			fprintfIfDebug(outFileDebug, " %02X%02X%02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xC8)
		{
			fprintfIfDebug(outFileDebug, " Subtract Play Value - Value");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xC9)
		{
			fprintfIfDebug(outFileDebug, " And Play Value and Value");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xCA)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xCB)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xCC)
		{
			fprintfIfDebug(outFileDebug, " Set Play Value");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xCD)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			fprintfIfDebug(outFileDebug, "\n");

			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xCE)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xCF)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xD0)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD1)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD2)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xD3)
		{
			fprintfIfDebug(outFileDebug, " Begin Sequence");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD4)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xD5)
		{
			fprintfIfDebug(outFileDebug, " Sequence Type");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD6)
		{
			fprintfIfDebug(outFileDebug, " Channel Disable");
			fprintfIfDebug(outFileDebug, " %02X%02X Channels", inputMID[spot], inputMID[spot+1]);

			unsigned short channelEnables = ((inputMID[spot] << 8) | inputMID[spot + 1]);
			for (int x = 0; x < 0x10; x++)
			{
				if (channelEnables & 0x1)
					fprintfIfDebug(outFileDebug, " %X", x);

				channelEnables >>= 1;
			}

			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xD7)
		{
			fprintfIfDebug(outFileDebug, " Channel Enable");
			fprintfIfDebug(outFileDebug, " %02X%02X Channels", inputMID[spot], inputMID[spot+1]);

			unsigned short channelEnables = ((inputMID[spot] << 8) | inputMID[spot + 1]);
			for (int x = 0; x < 0x10; x++)
			{
				if (channelEnables & 0x1)
					fprintfIfDebug(outFileDebug, " %X", x);

				channelEnables >>= 1;
			}

			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xD8)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xD9)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xDA)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d) %02X (%d) %02X (%d)", inputMID[spot], inputMID[spot], inputMID[spot+1], inputMID[spot+1], inputMID[spot+2], inputMID[spot+2]);
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xDB)
		{
			fprintfIfDebug(outFileDebug, " Master Volume");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xDC)
		{
			fprintfIfDebug(outFileDebug, " SFX Volume");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xDD)
		{
			fprintfIfDebug(outFileDebug, " Tempo");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			unsigned char tempo = inputMID[spot];
			unsigned long currentTempo = (unsigned long)(60000000.0 / (float)tempo);

			tempoPositions.push_back(TimeAndValue(absoluteTime, currentTempo));
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xDE)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			IncrementSpot(spot, 2, coverage);
		}
		else if (command == 0xDF)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE0)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE1)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X %02X %02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xE2)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X %02X %02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xE3)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE4)
		{
			fprintfIfDebug(outFileDebug, " Use Play Value to Offset into Jump Table (from last Command C2)");
		}
		else if (command == 0xE5)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE6)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xE7)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xE8)
		{
			// NUD-DMGJ
			//E800000E003F7F0000
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d) %02X (%d)", 
			inputMID[spot], inputMID[spot],
			inputMID[spot+1], inputMID[spot+1],
			inputMID[spot+2], inputMID[spot+2],
			inputMID[spot+3], inputMID[spot+3],
			inputMID[spot+4], inputMID[spot+4],
			inputMID[spot+5], inputMID[spot+5],
			inputMID[spot+6], inputMID[spot+6],
			inputMID[spot+7], inputMID[spot+7]
			);

			IncrementSpot(spot, 8, coverage);
		}
		else if (command == 0xE9)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xEA)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xEB)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xEC)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xED)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xEE)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xEF)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X%02X %02X", inputMID[spot], inputMID[spot+1], inputMID[spot+2]);
			IncrementSpot(spot, 3, coverage);
		}
		else if (command == 0xF0)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xF1)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xF2)
		{
			hitConditional = true;
			fprintfIfDebug(outFileDebug, " Go to Relative Offset if Play Value < 0");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			int relativeOffset = (signed char)inputMID[spot];
			IncrementSpot(spot, 1, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 2);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(relativeOffset);
						spot = spot + relativeOffset;
					}
				}
			}
		}
		else if (command == 0xF3)
		{
			hitConditional = true;
			fprintfIfDebug(outFileDebug, " Go to Relative Offset if Play Value == 0");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);

			int relativeOffset = (signed char)inputMID[spot];
			IncrementSpot(spot, 1, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 2);
				}
				else
				{			
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(relativeOffset);
						spot = spot + relativeOffset;
					}
				}
			}
		}
		else if (command == 0xF4)
		{
			hitConditional = true;
			fprintfIfDebug(outFileDebug, " Go to  relative offset");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], (signed char)inputMID[spot]);

			int relativeOffset = (signed char)inputMID[spot];
			IncrementSpot(spot, 1, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 2);
					break;
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), relativeOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(relativeOffset);
						spot = spot + relativeOffset;
					}
					else
					{
						break;
					}
				}
			}
		}
		else if (command == 0xF5)
		{
			hitConditional = true;
			fprintfIfDebug(outFileDebug, " Go to absolute offset if Play Value >= 0");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot =  absoluteOffset;
					}
				}
			}
		}
		else if (command == 0xF6)
		{
			fprintfIfDebug(outFileDebug, " Restart Song");

			if (attemptJumps)
			{
				int startSpot = 0;

				if (std::find(jumpsTaken.begin(), jumpsTaken.end(), startSpot) == jumpsTaken.end())
				{
					fprintfIfDebug(outFileDebug, "...Taking\n");
					jumpsTaken.push_back(startSpot);
					spot = startSpot;
				}
			}
		}
		else if (command == 0xF7)
		{
			fprintfIfDebug(outFileDebug, " ?");
		}
		else if (command == 0xF8)
		{
			fprintfIfDebug(outFileDebug, " ?");
			fprintfIfDebug(outFileDebug, " %02X (%d)", inputMID[spot], inputMID[spot]);
			IncrementSpot(spot, 1, coverage);
		}
		else if (command == 0xF9)
		{
			hitConditional = true;
			fprintfIfDebug(outFileDebug, " Go to Absolute Offset if Play Value < 0");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot =  absoluteOffset;
					}
				}
			}
		}
		else if (command == 0xFA)
		{
			hitConditional = true;
			fprintfIfDebug(outFileDebug, " Go to Absolute Offset if Play Value == 0");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);

			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot = absoluteOffset;
					}
				}
			}
		}
		else if (command == 0xFB)
		{
			fprintfIfDebug(outFileDebug, " Go to absolute offset");
			fprintfIfDebug(outFileDebug, " %02X%02X", inputMID[spot], inputMID[spot+1]);
			
			int absoluteOffset = CharArrayToShort(&inputMID[spot]);
			IncrementSpot(spot, 2, coverage);

			// Assume if next Channel Disable, is actually end of song for a real loop
			if (inputMID[spot] == 0xD6)
			{
				if (!hitConditional)
				{
					fprintfIfDebug(outFileDebug, "...Assuming End of Song\n");
					break;
				}
			}

			if (attemptJumps)
			{
				if (std::find(jumpStarts.begin(), jumpStarts.end(), spot) == jumpStarts.end())
				{
					fprintfIfDebug(outFileDebug, "...Skipping First Time\n");
					jumpStarts.push_back(spot);
					returnBackAlternateChoices.push_back(spot - 3);
					break;
				}
				else
				{
					if (std::find(jumpsTaken.begin(), jumpsTaken.end(), absoluteOffset) == jumpsTaken.end())
					{
						fprintfIfDebug(outFileDebug, "...Taking\n");
						jumpsTaken.push_back(absoluteOffset);
						spot = absoluteOffset;
					}
					else
					{
						break;
					}
				}
			}
		}
		else if (command == 0xFC)
		{
			unsigned short offset = ((inputMID[spot] << 8) | inputMID[spot+1]);
			fprintfIfDebug(outFileDebug, " Jump and Link");
			fprintfIfDebug(outFileDebug, " Offset %04X", offset);

			IncrementSpot(spot, 2, coverage);
			returnBackOffset.push_back(spot);

			spot = offset;
		}
		else if (command == 0xFD)
		{
			fprintfIfDebug(outFileDebug, " Delay");
			
			int timestamp = ReadEADMusicTimeValue(inputMID, spot, coverage);
			if (timestamp < 0x80)
				fprintfIfDebug(outFileDebug, " %02X (%d)", timestamp, timestamp);
			else
				fprintfIfDebug(outFileDebug, " %02X%02X - %04X (%d)", inputMID[spot-2], inputMID[spot+1-2], timestamp, timestamp);

			absoluteTime += timestamp;
		}
		else
		{
			fprintfIfDebug(outFileDebug, " UNKNOWN");
			return;
		}

		fprintfIfDebug(outFileDebug, "\n");
	}

	if (returnBackAlternateChoices.size() > 0)
	{
		// Return from Jump and Link
		spot = returnBackAlternateChoices.back();
		returnBackAlternateChoices.pop_back();
		command = 0x00;
		
		fprintfIfDebug(outFileDebug, "... Taking Alternate Path to %08X\n", spot);
		goto loopAgain;
	}

	if (returnBackOffset.size() > 0)
	{
		// Return from Jump and Link
		spot = returnBackOffset.back();
		returnBackOffset.pop_back();
		command = 0x00;
		
		fprintfIfDebug(outFileDebug, "... Returning from Subroutine to %08X\n", spot);
		goto loopAgain;
	}
}

void CMidiParse::EADMusicToMidi(CString gameName, int gameStyle, byte* inputMID, int address, int inputSize, CString outFileName, 
				 int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, unsigned long extra,
				 bool writeDebug, CString debugFilename, int pitchBendRange, bool isPreview)
{
	numberInstruments = 1;
	int noteUniqueId = 0;
	std::vector<TimeAndValue> tempoPositions;

	try
	{
		std::vector<SngNoteInfo> sngNoteList;

		numberInstruments = 1;
		int noteUniqueId = 0;
		std::vector<TimeAndValue> tempoPositions;

		int currentInstrument[0x10];
		int currentPan[0x10];
		int currentVolume[0x10];
		int currentPitchBend[0x10];
		int currentReverb[0x10];
		bool channelStarted[0x10];
		int currentVibrato[0x10];
		for (int x = 0; x < 0x10; x++)
		{
			currentInstrument[x] = 0x00;
			currentPan[x] = 0x40;
			currentVolume[x] = 0x7F;
			currentPitchBend[x] = 0x40;
			currentReverb[x] = 0x00;
			channelStarted[x] = false;
			currentVibrato[x] = 0x00;
		}

		FILE* outFileDebug = NULL;
		if (writeDebug)
		{
			outFileDebug = fopen(debugFilename, "w");

			CString addressStr;
			addressStr.Format("%08X", address);

			fprintf(outFileDebug, gameName + " - " + addressStr + "\n");
			fprintf(outFileDebug, "EAD Music\n\n");
		}

		unsigned long absoluteTime = 0;
		std::vector<unsigned long> jumpStarts;
		std::vector<unsigned long> jumpsTaken;
		bool attemptJumps = true;
		bool hitConditional = false;

		unsigned char* coverage = new unsigned char[inputSize];
		memcpy(coverage, &inputMID[address], inputSize);

		ParseEADMusicSequence(gameStyle, outFileDebug, &inputMID[address], 0, inputSize, absoluteTime, jumpStarts, jumpsTaken, attemptJumps, numberInstruments, tempoPositions, sngNoteList, noteUniqueId, currentInstrument, currentPan, currentVolume, currentPitchBend, currentReverb, hitConditional, coverage, channelStarted, currentVibrato, isPreview);

		WriteSngList(sngNoteList, tempoPositions, outFileName, separateByInstrument, 0x0030, true, pitchBendRange);

		if (outFileDebug)
		{
			fflush(outFileDebug);
			fclose(outFileDebug);
			outFileDebug = NULL;

			FILE* outDebugCoverage = fopen(debugFilename + "Coverage.bin", "wb");
			if (outDebugCoverage)
			{
				fwrite(coverage, 1, inputSize, outDebugCoverage);
			}
			fclose(outDebugCoverage);

			delete [] coverage;
			
		}
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

void CMidiParse::Factor5ToDebugTextFile(CString gameName, unsigned long address, CString midiFile, CString textFileOut, bool isRogueStyle)
{
	CString filepath = midiFile;
	
	FILE* inFile = fopen(filepath, "rb");
	if (inFile == NULL)
	{
		MessageBox(NULL, "Can't read input file " + filepath, "Error", NULL);
		return;
	}

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	rewind(inFile);

	unsigned char* inputMID = new unsigned char[inputSize];

	fread(inputMID, 1, inputSize, inFile);
	fclose(inFile);

	Factor5ToDebugTextFile(gameName, address, inputMID, inputSize, textFileOut, isRogueStyle);

	delete [] inputMID;
}

void CMidiParse::Factor5ToDebugTextFile(CString gameName, unsigned long address, byte* inputMID, int inputSize, CString textFileOut, bool isRogueStyle)
{
	FILE* outFile = fopen(textFileOut, "w");
		
	unsigned long instrumentOffset = CharArrayToLong(&inputMID[0x8]);

	fprintf(outFile, "Instruments\n");
	for (int x = 0; x < 0x40; x++)
	{
		if (inputMID[instrumentOffset + x] != 0xFF)
			fprintf(outFile, "%02X:%02X\n", x, inputMID[instrumentOffset + x]);
	}

	fprintf(outFile, "\n");
	fprintf(outFile, "Initial Tempo %08X", CharArrayToLong(&inputMID[0x10]));

	fprintf(outFile, "\n");

	unsigned long tempoOffset = CharArrayToLong(&inputMID[0xC]);
	if (tempoOffset != 0x00000000)
	{
		fprintf(outFile, "Tempos\n");
		int tempoIndex = 0;
		while (true)
		{
			if (CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8)]) == 0xFFFFFFFF)
				break;
			else
			{
				fprintf(outFile, "%08X:%08X\n", CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8)]), CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8) + 4]));
				tempoIndex++;
			}
		}
	}

	fprintf(outFile, "\n");

	fprintf(outFile, "Channels\n");
	fprintf(outFile, "-----------------------------------------\n");

	unsigned long channelOffsets = CharArrayToLong(&inputMID[0x0]);
	unsigned long trackOffsets = CharArrayToLong(&inputMID[0x4]);

	int numberChannels = 0;
	while (true)
	{
		if (
				(CharArrayToLong(&inputMID[channelOffsets + (numberChannels * 0x4)]) == 0x00000000) && (!isRogueStyle)
				)
			{
				numberChannels++;
				
				if (numberChannels == 64)
					break;
				else
					continue;
			}
		else if (CharArrayToLong(&inputMID[channelOffsets + (numberChannels * 0x4)]) == 0x00000000)
		{
			break;
		}
		else
		{
			unsigned long channelOffset = CharArrayToLong(&inputMID[channelOffsets + (numberChannels * 0x4)]);
			fprintf(outFile, "Channel #%02X Offset: %08X\n", numberChannels, channelOffset);
			fprintf(outFile, "-----------------------------------------\n");

			while ((inputMID[channelOffset+5] != 0x00) && ((signed short)CharArrayToShort(&inputMID[channelOffset+8]) >= 0))
			{
				fprintf(outFile, "Start Tick: %08X\n", CharArrayToLong(&inputMID[channelOffset]));
				fprintf(outFile, "Continuation: %08X\n", CharArrayToLong(&inputMID[channelOffset+4]));
				fprintf(outFile, "Track Index: %04X\n", CharArrayToShort(&inputMID[channelOffset+8]));
				fprintf(outFile, "Flags?: %04X\n", CharArrayToShort(&inputMID[channelOffset+0xA]));
				fprintf(outFile, "\n");

				unsigned long startTick = CharArrayToLong(&inputMID[channelOffset]);
				unsigned short trackIndex = CharArrayToShort(&inputMID[channelOffset+8]);

				unsigned long trackOffset = CharArrayToLong(&inputMID[trackOffsets + (trackIndex * 0x4)]);

				fprintf(outFile, "Track %02X Offset: %08X\n", trackIndex, trackOffset);
				fprintf(outFile, "-----------------------------------------\n");
				unsigned long headerSize = CharArrayToLong(&inputMID[trackOffset]);
				fprintf(outFile, "Header Size: %08X\n", headerSize);
				if (CharArrayToLong(&inputMID[trackOffset+4]) != NULL)
				{
					fprintf(outFile, "Pitch Wheel Offset: %08X\n", CharArrayToLong(&inputMID[trackOffset+4]));
				}
				else
					fprintf(outFile, "No Pitch Wheel\n");

				if (CharArrayToLong(&inputMID[trackOffset+8]) != NULL)
				{
					fprintf(outFile, "Mod Wheel Offset: %08X\n", CharArrayToLong(&inputMID[trackOffset+8]));
				}
				else
					fprintf(outFile, "No Mod Wheel\n");

				trackOffset += 4 + headerSize;

				fprintf(outFile, "\n");
				unsigned long absoluteTime = startTick;
				while (true)
				{
					if (isRogueStyle)
					{
						absoluteTime = startTick + CharArrayToLong(&inputMID[trackOffset]);
						trackOffset += 4;

						if ((CharArrayToLong(&inputMID[trackOffset]) & 0x0000FFFF) == 0x0000FFFF)
						{
							fprintf(outFile, "%08X Time %08X End\n", trackOffset, absoluteTime);
							trackOffset += 4;
							break;
						}

						if (((inputMID[trackOffset]) & 0x80) == 0x00)
						{
							if ((inputMID[trackOffset+2] & 0x80) == 0x00)
							{
								unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset]);
								unsigned char note = (inputMID[trackOffset+2] & 0x7F);
								unsigned char velocity = (inputMID[trackOffset + 3] & 0x7F);

								fprintf(outFile, "%08X Time %08X Length %04X Note %02X Velocity %02X\n", trackOffset, absoluteTime, noteLength, note, velocity);
							}
							else
							{
								unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset]);
								unsigned char value = (inputMID[trackOffset+2] & 0x7F);
								unsigned char command = (inputMID[trackOffset + 3] & 0x7F);

								fprintf(outFile, "%08X Time %08X Length %04X Cmd %02X Value %02X\n", trackOffset, absoluteTime, noteLength, command, value);
							}

							trackOffset += 4;
						}
						else if (((inputMID[trackOffset]) & 0x80) == 0x80)
						{
							unsigned char value = (inputMID[trackOffset] & 0x7F);
							unsigned char controller = (inputMID[trackOffset + 1] & 0x7F);

							fprintf(outFile, "%08X Time %08X Controller %02X Value %02X\n", trackOffset, absoluteTime, controller, value);

							trackOffset += 2;
						}
					}
					else
					{
						if ((CharArrayToLong(&inputMID[trackOffset]) & 0x0000FFFF) == 0x0000FFFF)
						{
							fprintf(outFile, "%08X Time %08X End\n", trackOffset, absoluteTime);
							trackOffset += 4;
							break;
						}

						absoluteTime += DecodeFactor5DeltaTimeRLE(inputMID, trackOffset);

						if (((inputMID[trackOffset]) & 0x80) == 0x00)
						{
						
							unsigned char note = (inputMID[trackOffset] & 0x7F);
							unsigned char velocity = (inputMID[trackOffset+1] & 0x7F);
							unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset+2]);

							fprintf(outFile, "%08X Time %08X Note %02X Velocity %02X Length %04X \n", trackOffset, absoluteTime, note, velocity, noteLength);

							trackOffset += 4;
						}
						else
						{
							unsigned char value = (inputMID[trackOffset] & 0x7F);
							unsigned char controller = (inputMID[trackOffset + 1] & 0x7F);

							fprintf(outFile, "%08X Time %08X Controller %02X Value %02X\n", trackOffset, absoluteTime, controller, value);

							trackOffset += 2;
						}
					}
				}

				fprintf(outFile, "\n");

				channelOffset += 0xC;
			}

			fprintf(outFile, "Start Tick: %08X\n", CharArrayToLong(&inputMID[channelOffset]));
			fprintf(outFile, "Continuation: %08X (End)\n", CharArrayToLong(&inputMID[channelOffset+4]));
			fprintf(outFile, "Track Index: %04X\n", CharArrayToShort(&inputMID[channelOffset+8]));
			fprintf(outFile, "Flags?: %04X\n", CharArrayToShort(&inputMID[channelOffset+0xA]));
			fprintf(outFile, "\n");

			channelOffset += 0xC;

			fprintf(outFile, "\n");
		}

		numberChannels++;
	}

	fprintf(outFile, "\n\n\n\n\n");

	fprintf(outFile, "Tracks Individually\n");
	fprintf(outFile, "-----------------------------------------\n");

	int numberTracks = 0;
	while (true)
	{
		unsigned long trackOffset = CharArrayToLong(&inputMID[trackOffsets + (numberTracks * 0x4)]);
		if (
			(CharArrayToLong(&inputMID[trackOffsets + (numberTracks * 0x4)]) == 0x00000000)
			|| (CharArrayToLong(&inputMID[trackOffsets + (numberTracks * 0x4)]) == 0x00000008)
			)
		{
			break;
		}
		else
		{
			fprintf(outFile, "Track %02X Offset: %08X\n", numberTracks, trackOffset);
			fprintf(outFile, "-----------------------------------------\n");
			unsigned long headerSize = CharArrayToLong(&inputMID[trackOffset]);
			fprintf(outFile, "Header Size: %08X\n", headerSize);
			if (CharArrayToLong(&inputMID[trackOffset+4]) != NULL)
			{
				fprintf(outFile, "Pitch Wheel Offset: %08X\n", CharArrayToLong(&inputMID[trackOffset+4]));
			}
			else
				fprintf(outFile, "No Pitch Wheel\n");

			if (CharArrayToLong(&inputMID[trackOffset+8]) != NULL)
			{
				fprintf(outFile, "Mod Wheel Offset: %08X\n", CharArrayToLong(&inputMID[trackOffset+8]));
			}
			else
				fprintf(outFile, "No Mod Wheel\n");

			trackOffset += 4 + headerSize;

			fprintf(outFile, "\n");
			unsigned long absoluteTime = 0x00000000;
			while (true)
			{
				if (isRogueStyle)
				{
					absoluteTime = CharArrayToLong(&inputMID[trackOffset]);
					trackOffset += 4;

					if ((CharArrayToLong(&inputMID[trackOffset]) & 0x0000FFFF) == 0x0000FFFF)
					{
						fprintf(outFile, "%08X Time %08X End\n", trackOffset, absoluteTime);
						trackOffset += 4;
						break;
					}

					if (((inputMID[trackOffset]) & 0x80) == 0x00)
					{
						if ((inputMID[trackOffset+2] & 0x80) == 0x00)
						{
							unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset]);
							unsigned char note = (inputMID[trackOffset+2] & 0x7F);
							unsigned char velocity = (inputMID[trackOffset + 3] & 0x7F);

							fprintf(outFile, "%08X Time %08X Length %04X Note %02X Velocity %02X\n", trackOffset, absoluteTime, noteLength, note, velocity);
						}
						else
						{
							unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset]);
							unsigned char value = (inputMID[trackOffset+2] & 0x7F);
							unsigned char command = (inputMID[trackOffset + 3] & 0x7F);

							fprintf(outFile, "%08X Time %08X Length %04X Cmd %02X Value %02X\n", trackOffset, absoluteTime, noteLength, command, value);
						}

						trackOffset += 4;
					}
					else if (((inputMID[trackOffset]) & 0x80) == 0x80)
					{
						unsigned char value = (inputMID[trackOffset] & 0x7F);
						unsigned char controller = (inputMID[trackOffset + 1] & 0x7F);

						fprintf(outFile, "%08X Time %08X Controller %02X Value %02X\n", trackOffset, absoluteTime, controller, value);

						trackOffset += 2;
					}
				}
				else
				{
					if ((CharArrayToLong(&inputMID[trackOffset]) & 0x0000FFFF) == 0x0000FFFF)
					{
						fprintf(outFile, "%08X Time %08X End\n", trackOffset, absoluteTime);
						trackOffset += 4;
						break;
					}

					unsigned short timeDelta = DecodeFactor5DeltaTimeRLE(inputMID, trackOffset);
					absoluteTime += timeDelta;

					if (((inputMID[trackOffset]) & 0x80) == 0x00)
					{
					
						unsigned char note = (inputMID[trackOffset] & 0x7F);
						unsigned char velocity = (inputMID[trackOffset+1] & 0x7F);
						unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset+2]);

						fprintf(outFile, "%08X Time %08X Note %02X Velocity %02X Length %04X \n", trackOffset, absoluteTime, note, velocity, noteLength);

						trackOffset += 4;
					}
					else
					{
						unsigned char value = (inputMID[trackOffset] & 0x7F);
						unsigned char controller = (inputMID[trackOffset + 1] & 0x7F);

						fprintf(outFile, "%08X Time %08X Controller %02X Value %02X\n", trackOffset, absoluteTime, controller, value);

						trackOffset += 2;
					}
				}
			}

			fprintf(outFile, "\n\n");
		}

		numberTracks++;
	}

	fprintf(outFile, "\n");

	fclose(outFile);
}

int CMidiParse::DecodeFactor5DeltaTimeRLE(unsigned char* input, unsigned long& offset)
{
    int total = 0;
    while (true)
	{
		int term = CharArrayToShort(&input[offset]);
		offset += 2;
        if (term == 0xffff)
		{
            total += 0xffff;
            int dummy = CharArrayToShort(&input[offset]);
			offset += 2;
            continue;
		}
        total += term;
        return total;
	}
}

void CMidiParse::Factor5ToMidi(byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool isRogueStyle)
{
	// Contributions to figuring out this format were thanks to the GameCube work of Jack Anderson
	// http://www.metroid2002.com/retromodding/wiki/CSNG_(File_Format)
	try
	{
		numberInstruments = 1;
		int noteUniqueId = 0;
		std::vector<TimeAndValue> tempoPositions;
		std::vector<SngNoteInfo> sngNoteList;

		unsigned long instrumentOffset = CharArrayToLong(&inputMID[0x8]);

		//fprintf(outFile, "\n");
		//fprintf(outFile, "Initial Something %08X", CharArrayToLong(&inputMID[0x10]));

		//fprintf(outFile, "\n");

		unsigned long tempoOffset = CharArrayToLong(&inputMID[0xC]);
		if (tempoOffset != 0x00000000)
		{
			//fprintf(outFile, "Tempos\n");
			int tempoIndex = 0;
			while (true)
			{
				if (CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8)]) == 0xFFFFFFFF)
					break;
				else
				{
					unsigned long tempo = CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8) + 4]);
					tempo = (unsigned long)(60000000.0 / (float)tempo);
					tempoPositions.push_back(TimeAndValue(CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8)]), tempo));
					//fprintf(outFile, "%08X:%08X\n", CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8)]), CharArrayToLong(&inputMID[tempoOffset + (tempoIndex * 0x8) + 4]));
					tempoIndex++;
				}
			}
		}
		else
		{
			unsigned long tempo = CharArrayToLong(&inputMID[0x10]);
			tempo = (unsigned long)(60000000.0 / (float)tempo);
			tempoPositions.push_back(TimeAndValue(0x00000000, tempo));
		}

		//fprintf(outFile, "\n");

		//fprintf(outFile, "Channels\n");
		//fprintf(outFile, "-----------------------------------------\n");

		unsigned long trackOffsets = CharArrayToLong(&inputMID[0x4]);

		unsigned long channelOffsets = CharArrayToLong(&inputMID[0x0]);
		int numberChannels = 0;
		while (true)
		{
			if (
				(CharArrayToLong(&inputMID[channelOffsets + (numberChannels * 0x4)]) == 0x00000000) && (!isRogueStyle)
				)
			{
				numberChannels++;

				if (numberChannels == 64)
					break;
				else
					continue;
			}
			else if (CharArrayToLong(&inputMID[channelOffsets + (numberChannels * 0x4)]) == 0x00000000)
			{
				break;
			}
			else
			{
				unsigned long channelOffset = CharArrayToLong(&inputMID[channelOffsets + (numberChannels * 0x4)]);
				unsigned short currentInstrument = inputMID[instrumentOffset + numberChannels];

				if (currentInstrument > numberInstruments)
					numberInstruments = currentInstrument + 1;

				//fprintf(outFile, "Channel #%02X Offset: %08X\n", numberChannels, channelOffset);
				//fprintf(outFile, "-----------------------------------------\n");

				while ((inputMID[channelOffset+5] != 0x00) && ((signed short)CharArrayToShort(&inputMID[channelOffset+8]) >= 0))
				{
					//fprintf(outFile, "Start Tick: %08X\n", CharArrayToLong(&inputMID[channelOffset]));
					//fprintf(outFile, "Continuation: %08X\n", CharArrayToLong(&inputMID[channelOffset+4]));
					//fprintf(outFile, "Track Index: %04X\n", CharArrayToShort(&inputMID[channelOffset+8]));
					//fprintf(outFile, "Flags?: %04X\n", CharArrayToShort(&inputMID[channelOffset+0xA]));
					//fprintf(outFile, "\n");


					unsigned long startTick = CharArrayToLong(&inputMID[channelOffset]);
					unsigned short trackIndex = CharArrayToShort(&inputMID[channelOffset+8]);

					unsigned char currentPan = 0x40;
					unsigned char currentVolume = 0x7F;
					signed char currentPitchBend = 0x40;

					int currentTranspose = 0;
					int currentEffect = 0;


					unsigned long trackOffset = CharArrayToLong(&inputMID[trackOffsets + (trackIndex * 0x4)]);
					//fprintf(outFile, "Track %02X Offset: %08X\n", numberTracks, trackOffset);
					//fprintf(outFile, "-----------------------------------------\n");
					unsigned long headerSize = CharArrayToLong(&inputMID[trackOffset]);
					//fprintf(outFile, "Header Size: %08X\n", headerSize);
					if (CharArrayToLong(&inputMID[trackOffset+4]) != NULL)
					{
						unsigned long pitchWheelOffset = CharArrayToLong(&inputMID[trackOffset+4]);
						//fprintf(outFile, "Pitch Wheel Offset: %08X\n", CharArrayToLong(&inputMID[trackOffset+4]));
					}
					//else
						//fprintf(outFile, "No Pitch Wheel\n");

					if (CharArrayToLong(&inputMID[trackOffset+8]) != NULL)
					{
						unsigned long modWheelOffset = CharArrayToLong(&inputMID[trackOffset+8]);
						//fprintf(outFile, "Mod Wheel Offset: %08X\n", CharArrayToLong(&inputMID[trackOffset+8]));
					}
					//else
						//fprintf(outFile, "No Mod Wheel\n");

					trackOffset += 4 + headerSize;

					//fprintf(outFile, "\n");
					unsigned long absoluteTime = startTick;
					while (true)
					{
						unsigned long currentTempo = (unsigned long)(60000000.0 / (float)120.0);

						for (int y = 0; y < tempoPositions.size(); y++)
						{
							if (tempoPositions[y].absoluteTime <= absoluteTime)
							{
								currentTempo = tempoPositions[y].value;
							}
							else
							{
								break;
							}
						}

						if (isRogueStyle)
						{
							absoluteTime = startTick + CharArrayToLong(&inputMID[trackOffset]);
							trackOffset += 4;

							if ((CharArrayToLong(&inputMID[trackOffset]) & 0x0000FFFF) == 0x0000FFFF)
							{
								//fprintf(outFile, "%08X Time %08X End\n", trackOffset, absoluteTime);
								trackOffset += 4;
								break;
							}
							else if (((inputMID[trackOffset]) & 0x80) == 0x00)
							{
								if ((inputMID[trackOffset+2] & 0x80) == 0x00)
								{
									unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset]);
									unsigned char note = (inputMID[trackOffset+2] & 0x7F);
									unsigned char velocity = (inputMID[trackOffset + 3] & 0x7F);

									//fprintf(outFile, "%08X Time %08X Length %04X Note %02X Velocity %02X\n", trackOffset, absoluteTime, noteLength, note, velocity);

									SngNoteInfo songNoteInfo;
									songNoteInfo.originalTrack = numberChannels;
									songNoteInfo.originalNoteUniqueId = noteUniqueId++;
									songNoteInfo.startAbsoluteTime = absoluteTime;
									songNoteInfo.noteNumber = note + currentTranspose;

									
									songNoteInfo.instrument = numberChannels;

									songNoteInfo.velocity = velocity;

									songNoteInfo.effect = currentEffect;
									songNoteInfo.tempo = currentTempo;
									songNoteInfo.pan = currentPan;

									songNoteInfo.pitchBend = 0x40;
									songNoteInfo.volume = currentVolume;


									songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

									sngNoteList.push_back(songNoteInfo);
								}
								else
								{
									unsigned short length = CharArrayToShort(&inputMID[trackOffset]);
									unsigned char value = (inputMID[trackOffset+2] & 0x7F);
									unsigned char command = (inputMID[trackOffset + 3] & 0x7F);

									//fprintf(outFile, "%08X Time %08X Length %04X Cmd %02X Value %02X\n", trackOffset, absoluteTime, noteLength, command, value);
									if (command == 7)
										currentVolume = value & 0x7F;
									else if (command == 10)
										currentPan = value & 0x7F;
									else if (command == 91)
										currentEffect = value & 0x7F;
								}

								trackOffset += 4;
							}
							else if (((inputMID[trackOffset]) & 0x80) == 0x80)
							{
								unsigned char value = (inputMID[trackOffset] & 0x7F);
								unsigned char controller = (inputMID[trackOffset + 1] & 0x7F);

								//fprintf(outFile, "%08X Time %08X Controller %02X Value %02X\n", trackOffset, absoluteTime, controller, value);

								trackOffset += 2;
							}
						}
						else
						{
							if ((CharArrayToLong(&inputMID[trackOffset]) & 0x0000FFFF) == 0x0000FFFF)
							{
								//fprintf(outFile, "%08X Time %08X End\n", trackOffset, absoluteTime);
								trackOffset += 4;
								break;
							}

							absoluteTime += DecodeFactor5DeltaTimeRLE(inputMID, trackOffset);

							if (((inputMID[trackOffset]) & 0x80) == 0x00)
							{
							
								unsigned char note = (inputMID[trackOffset] & 0x7F);
								unsigned char velocity = (inputMID[trackOffset+1] & 0x7F);
								unsigned short noteLength = CharArrayToShort(&inputMID[trackOffset+2]);

								//fprintf(outFile, "%08X Time %08X Note %02X Velocity %02X Length %04X \n", trackOffset, absoluteTime, note, velocity, noteLength);

								SngNoteInfo songNoteInfo;
								songNoteInfo.originalTrack = numberChannels;
								songNoteInfo.originalNoteUniqueId = noteUniqueId++;
								songNoteInfo.startAbsoluteTime = absoluteTime;
								songNoteInfo.noteNumber = note + currentTranspose;

								
								songNoteInfo.instrument = numberChannels;

								songNoteInfo.velocity = velocity;

								songNoteInfo.effect = currentEffect;
								songNoteInfo.tempo = currentTempo;
								songNoteInfo.pan = currentPan;

								songNoteInfo.pitchBend = 0x40;
								songNoteInfo.volume = currentVolume;


								songNoteInfo.endAbsoluteTime = songNoteInfo.startAbsoluteTime + noteLength;

								sngNoteList.push_back(songNoteInfo);

								trackOffset += 4;
							}
							else
							{
								unsigned char value = (inputMID[trackOffset] & 0x7F);
								unsigned char controller = (inputMID[trackOffset + 1] & 0x7F);

								//fprintf(outFile, "%08X Time %08X Controller %02X Value %02X\n", trackOffset, absoluteTime, controller, value);

								trackOffset += 2;
							}
						}
					}

					channelOffset += 0xC;
				}

				//fprintf(outFile, "Start Tick: %08X\n", CharArrayToLong(&inputMID[channelOffset]));
				//fprintf(outFile, "Continuation: %08X (End)\n", CharArrayToLong(&inputMID[channelOffset+4]));
				//fprintf(outFile, "Track Index: %04X\n", CharArrayToShort(&inputMID[channelOffset+8]));
				//fprintf(outFile, "Flags?: %04X\n", CharArrayToShort(&inputMID[channelOffset+0xA]));
				//fprintf(outFile, "\n");

				channelOffset += 0xC;

				//fprintf(outFile, "\n");
			}

			numberChannels++;
		}

		//fprintf(outFile, "\n");

		WriteSngList(sngNoteList, tempoPositions, outFileName, separateByInstrument, 0x0180, false, 24);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

void CMidiParse::KonamiToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long extra)
{
	int numberTracks = 0x0;
	
	/*int numberTracksCompare = 0x0;
	if (!extraGameMidiInfo.naganoCompressed && !extraGameMidiInfo.zlbCompressed)
	{
		int trackCountOffset = extraGameMidiInfo.trackOffset + (extraGameMidiInfo.trackIncrement * extra);

		numberTracksCompare = ROM[trackCountOffset];
	}
	else if (extraGameMidiInfo.naganoCompressed)
	{
		int fileSizeCompressed = extraGameMidiInfo.compressedFileEndOffset - extraGameMidiInfo.compressedFileOffset;
		CNaganoDecoder decode;
		unsigned char* outputDecompressed = new unsigned char[0x200000];
		int expectedSize = decode.dec(&ROM[extraGameMidiInfo.compressedFileOffset], fileSizeCompressed, outputDecompressed);
		
		int trackCountOffset = extraGameMidiInfo.trackOffset + (extraGameMidiInfo.trackIncrement * extra);

		numberTracksCompare = outputDecompressed[trackCountOffset];

		delete [] outputDecompressed;
	}
	else if (extraGameMidiInfo.zlbCompressed)
	{
		compress->SetGame(STUNTRACER64);
		int decompressedSize = 0;	
		int compressedSize = -1;
		byte* outputBuffer = Decompress(&ROM[extraGameMidiInfo.compressedFileOffset], romSize - extraGameMidiInfo.compressedFileOffset, decompressedSize, compressedSize);
		if (outputBuffer != NULL)
		{
			int trackCountOffset = extraGameMidiInfo.trackOffset + (extraGameMidiInfo.trackIncrement * extra);

			numberTracksCompare = outputBuffer[trackCountOffset];

			delete [] outputBuffer;
		}
	}*/

	if (numberTracks == 0x0)
	{
		numberTracks = CharArrayToShort(&inputMID[0]) / 2;
	}

	numberInstruments = 1;
	int noteUniqueId = 0;
	std::vector<TimeAndValue> tempoPositions;

	
	try
	{
		std::vector<SngNoteInfo> sngNoteList;

		int trackRealLength = 0;
		std::vector<int> tracksEDEE;

		int loopStart = 0;
		int loopEnd = 0;
		int maxTrackLength = 0;

		int highestTrackLength = 0;

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToShort(&inputMID[(x * 2)]);

			if (trackDataPointer >= inputSize)
			{
				numberTracks = x;
				break;
			}

			// Fake track
			if ((x > 0) && (inputMID[trackDataPointer-1] != 0xFF))
			{
				break;
			}

			unsigned long trackEnd;

			/*if (x < (numberTracks - 1))
			{
				trackEnd = CharArrayToShort(&inputMID[((x + 1) * 2)]);
			}
			else*/
			{
				trackEnd = inputSize;
			}

			int trackLength = FindHighestKonamiLengthTrack(x, inputMID, trackDataPointer, trackEnd);
			if (trackLength > highestTrackLength)
				highestTrackLength = trackLength;
		}

		/*if (numberTracks != numberTracksCompare)
		{
			FILE* temp = fopen("C:\\temp\\a.txt", "a");
			fprintf(temp, "%s %08X Specified %02X Real %02X %s\n", &ROM[0x20], extraGameMidiInfo.compressedFileOffset, numberTracksCompare, numberTracks, outFileName);
			fclose(temp);
		}*/

		for (int x = 0; x < numberTracks; x++)
		{
			unsigned long trackDataPointer = CharArrayToShort(&inputMID[(x * 2)]);

			if (trackDataPointer >= inputSize)
			{
				numberTracks = x;
				break;
			}

			// Fake track
			if ((x > 0) && (inputMID[trackDataPointer-1] != 0xFF))
			{
				break;
			}

			unsigned long trackEnd;

			/*if (x < (numberTracks - 1))
			{
				trackEnd = CharArrayToShort(&inputMID[((x + 1) * 2)]);
			}
			else*/
			{
				trackEnd = inputSize;
			}

			ParseKonamiTrack(x, numberInstruments, tempoPositions, sngNoteList, inputMID, trackDataPointer, trackEnd, noteUniqueId, writeOutLoops, loopWriteCount, extendTracksToHighest, highestTrackLength);
		}

		WriteSngList(sngNoteList, tempoPositions, outFileName, separateByInstrument, 0x0030, false, 24);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

void CMidiParse::SSEQToMidi(unsigned char* ROM, int romSize, byte* inputMID, int inputSize, CString outFileName, int& numberInstruments, bool calculateInstrumentCountOnly, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo, unsigned long numberTracks)
{
	numberInstruments = 1;
	int noteUniqueId = 0;
	std::vector<TimeAndValue> tempoPositions;

	
	try
	{
		std::vector<SngNoteInfo> sngNoteList;

		int trackRealLength = 0;
		
		int loopStart = 0;
		int loopEnd = 0;
		int maxTrackLength = 0;

		int highestTrackLength = 0;

		int trackLength = FindHighestSSEQLengthTrack(inputMID, inputSize, numberTracks);
		if (trackLength > highestTrackLength)
			highestTrackLength = trackLength;

		unsigned long division = 0x1E0;

		unsigned long offsetData = 0;
		for (int track = 0; track < numberTracks; track++)
		{
			unsigned short extraFlag = CharArrayToShort(&inputMID[offsetData + 0xE]);
			unsigned long sizeTrack = CharArrayToLong(&inputMID[offsetData + 0x10]);
			division = CharArrayToLong(&inputMID[offsetData + 8]);

			if (extraFlag)
			{
				offsetData += 4;
			}

			offsetData += 0x14;

			ParseSSEQTrack(track, numberInstruments, tempoPositions, sngNoteList, inputMID, offsetData, inputSize, noteUniqueId, writeOutLoops, loopWriteCount, extendTracksToHighest, highestTrackLength);

			offsetData += sizeTrack;
		}

		WriteSngList(sngNoteList, tempoPositions, outFileName, separateByInstrument, division, false, 24);
	}
	catch (...)
	{
		MessageBox(NULL, "Error exporting", "Error", NULL);
	}
}

BOOL CMidiParse::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESHOWWINDOW;
   si.wShowWindow  = SW_HIDE;
   ZeroMemory(&processInfo, sizeof(processInfo));
   /*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox(NULL, "For some reason GZip Failed", "Error", NULL);
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
}

BOOL CMidiParse::hiddenExec (PTSTR pCmdLine, CString currentDirectory, HANDLE out)
{
	::SetCurrentDirectory(currentDirectory);
   STARTUPINFO si;
   PROCESS_INFORMATION processInfo;
   ZeroMemory(&si, sizeof(si));
   si.cb           = sizeof(si);
   si.dwFlags      = STARTF_USESTDHANDLES;
   //si.wShowWindow  = SW_HIDE;
   si.hStdOutput = out;
   //si.hStdInput = out;
   //si.hStdError = out;

   ZeroMemory(&processInfo, sizeof(processInfo));


   if (currentDirectory.ReverseFind('\\') == (currentDirectory.GetLength()-1))
   {
		currentDirectory = currentDirectory.Mid(0, (currentDirectory.GetLength()-1));
   }

   /*return */CreateProcess(0, pCmdLine, 0, 0, TRUE, CREATE_DEFAULT_ERROR_MODE, 0, currentDirectory, &si, &processInfo);
   WaitForSingleObject(processInfo.hProcess, 20000);
   DWORD exitCode;
   if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
   {
        if (exitCode == STILL_ACTIVE)
		{
			MessageBox(NULL, "For some reason Zip Failed", "Error", NULL);
			
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
}