#include "StdAfx.h"
#include "N64MidiToolReader.h"
#include "..\N64MidiLibrary\SupermanDecoder.h"
#include "..\N64MidiLibrary\rnc_deco.h"
#include "..\N64MidiLibrary\H20Decoder.h"
#include "..\N64MidiLibrary\TetrisphereDecoder.h"
#include "..\N64MidiLibrary\GECompression.h"
#include "..\N64MidiLibrary\MidwayDecoder.h"
#include "..\N64MidiLibrary\BlitzDecoder.h"
#include "..\N64MidiLibrary\MarioTennisDecoder.h"
#include "..\N64MidiLibrary\yay0.h"
#include "..\N64MidiLibrary\VigilanteDecoder.h"
#include "..\N64MidiLibrary\FLA2Decoder.h"
#include "..\N64MidiLibrary\RugratsDecoder.h"
#include "..\N64MidiLibrary\flzh_rn.h"
#include "..\N64MidiLibrary\n643docompression.h"
#include "..\N64MidiLibrary\LZARIDecoder.h"
#include "..\N64MidiLibrary\SnowDecoder.h"
#include "..\N64MidiLibrary\ASMICDecoder.h"
#include "..\N64MidiLibrary\NaganoDecoder.h"
#include "..\N64MidiLibrary\QuakeDecoder.h"
#include "..\N64MidiLibrary\ViewpointDecoder.h"
#include "..\N64MidiLibrary\HexenDecoder.h"

CMidiParse CN64MidiToolReader::midiParse;
GECompression CN64MidiToolReader::compress;

CN64MidiToolReader::CN64MidiToolReader(void)
{
}

CN64MidiToolReader::~CN64MidiToolReader(void)
{
}

int CN64MidiToolReader::HexToInt(char inChar)
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

unsigned long CN64MidiToolReader::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned long CN64MidiToolReader::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned short CN64MidiToolReader::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}

unsigned short CN64MidiToolReader::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CN64MidiToolReader::StringHexToLong(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 8)
	{
		CString tempStr = inString;
		int tempStrLength = tempStr.GetLength();
		inString = "";
		for (int x = 0; x < (8-tempStrLength); x++)
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

void CN64MidiToolReader::InitializeSpecificGames(CString iniPath, int& countGames, MidiGameConfig*& gameConfig)
{
	FILE* inIni = fopen(iniPath, "r");
	if (inIni == NULL)
	{
		MessageBox(NULL, "Error reading gameconfigmidi.ini, file must exist", "Error", NULL);
		return;
	}

	CString lastGame = "";

	countGames = 0;

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

		if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", &currentLine[1]);
		}

		if (lastGameNew != lastGame)
		{
			countGames++;
			lastGame = lastGameNew;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	rewind(inIni);

	gameConfig = new MidiGameConfig[countGames];
	countGames = 0;
	lastGame = "";
	
	int midiCount = 0;
	int nameCount = 0;


	CString gameType = "Uncompressed";
	while (!feof(inIni))
	{
		
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;
		
		CString curLineStr;
		curLineStr.Format("%s", currentLine);

		if (currentLine[0] == '/')
			continue;
		else if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", curLineStr.Mid(1, (spot - 1)));

			gameType = "Uncompressed";
			if (curLineStr.Find(":type=") != -1)
			{
				gameType = curLineStr.Mid((curLineStr.Find(":type=") + 6), (curLineStr.GetLength() - curLineStr.Find(":type=") - 6));
				gameType.Trim();
			}
		}
		else if (currentLine[0] == '|')
		{
			nameCount++;
		}
		else if (currentLine[0] == '{')
		{
			CString line;
			line.Format("%s", currentLine);

			line.Replace("{", "");
			line.Replace("}", "");
			line.Replace(" ", "");

			if ((line.Find("TrackOffset")) != -1)
			{
				line.Replace("TrackOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.trackOffset = StringHexToLong(line);
			}
			else if ((line.Find("TrackIncrement")) != -1)
			{
				line.Replace("TrackIncrement", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.trackIncrement = StringHexToLong(line);
			}
			else if ((line.Find("CompressedFileOffset")) != -1)
			{
				line.Replace("CompressedFileOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.compressedFileOffset = StringHexToLong(line);
			}
			else if ((line.Find("CompressedFileEndOffset")) != -1)
			{
				line.Replace("CompressedFileEndOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.compressedFileEndOffset = StringHexToLong(line);
			}
			else if ((line.Find("TrackAddressOffset")) != -1)
			{
				line.Replace("TrackAddressOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.trackAddressOffset = StringHexToLong(line);
			}
			else if ((line.Find("TrackAddressEndsOffset")) != -1)
			{
				line.Replace("TrackAddressEndsOffset", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.trackAddressEndsOffset = StringHexToLong(line);
			}
			else if ((line.Find("Checksum1")) != -1)
			{
				line.Replace("Checksum1", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.checksum1 = StringHexToLong(line);
			}
			else if ((line.Find("Checksum2")) != -1)
			{
				line.Replace("Checksum2", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.checksum2 = StringHexToLong(line);
			}
			else if ((line.Find("TracksEnd")) != -1)
			{
				line.Replace("TracksEnd", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.tracksEnd = StringHexToLong(line);
			}
			
			else if ((line.Find("ZlbCompressed")) != -1)
			{
				line.Replace("ZlbCompressed", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.zlbCompressed = true;
			}
			else if ((line.Find("NaganoCompressed")) != -1)
			{
				line.Replace("NaganoCompressed", "");
				line.Replace(":", "");
				gameConfig[countGames].extraGameMidiInfo.naganoCompressed = true;
			}
		}
		else
		{
			midiCount++;
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				if (midiCount > 0)
					gameConfig[countGames].midiBanks = new MidiConfig[midiCount];
				if (nameCount > 0)
					gameConfig[countGames].midiNames = new CString[nameCount];
				countGames++;
			}
			else
			{
				gameConfig[countGames].gameName = lastGameNew;
				gameConfig[countGames].gameName.Replace("\r", "");
				gameConfig[countGames].gameName.Replace("\n", "");
				gameConfig[countGames].gameName.Trim();
			}


			midiCount = 0;
			nameCount = 0;
			lastGame = lastGameNew;
			gameConfig[countGames].gameName = lastGameNew;
			gameConfig[countGames].gameName.Replace("\r", "");
			gameConfig[countGames].gameName.Replace("\n", "");
			gameConfig[countGames].gameName.Trim();
			gameConfig[countGames].gameType = gameType;
			gameConfig[countGames].gameType.Trim();
		}
	}

	if (lastGame != "")
	{
		if (midiCount > 0)
			gameConfig[countGames].midiBanks = new MidiConfig[midiCount];
		if (nameCount > 0)
			gameConfig[countGames].midiNames = new CString[nameCount];
		gameConfig[countGames].gameType = gameType;
		gameConfig[countGames].gameType.Trim();
		countGames++;
	}

	rewind(inIni);

	countGames = 0;
	lastGame = "";

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

		if (currentLine[0] == '/')
			continue;
		else if (currentLine[0] == '[')
		{
			int spot = 0;
			while (spot < 1000)
			{
				if (currentLine[spot] == ']')
				{
					currentLine[spot] = 0x0;
					break;
				}
				spot++;
			}

			lastGameNew.Format("%s", &currentLine[1]);
		}
		else if (currentLine[0] == '|')
		{
			gameConfig[countGames].midiNames[gameConfig[countGames].numberMidiNames].Format("%s", &currentLine[1]);
			gameConfig[countGames].numberMidiNames++;
		}
		else
		{
			CString line;
			line.Format("%s", currentLine);
			if (line.Find(",") != -1)
			{
				int commaSpot = line.Find(",");
				CString first = line.Mid(0,commaSpot);
				CString second = line.Mid((commaSpot + 1), (line.GetLength() - commaSpot - 1));
				CString third = "";
				CString fourth = "";

				int commaSpot2 = second.Find(",");
				if (commaSpot2 != -1)
				{
					third = second.Mid((commaSpot2 + 1), (second.GetLength() - commaSpot2 - 1));
					second = second.Mid(0,commaSpot2);


					int commaSpot3 = third.Find(",");
					if (commaSpot3 != -1)
					{
						fourth = third.Mid((commaSpot3 + 1), (third.GetLength() - commaSpot3 - 1));
						third = third.Mid(0,commaSpot3);
					}
				}

				second.Replace("\r", "");
				second.Replace("\n", "");
				third.Replace("\r", "");
				third.Replace("\n", "");
				fourth.Replace("\r", "");
				fourth.Replace("\n", "");

				gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].start = StringHexToLong(first);
				gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].end = StringHexToLong(second);
				if (third.GetLength() > 0)
					gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].extra = StringHexToLong(third);
				else
					gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].extra = 0x00000000;
				if (fourth.GetLength() > 0)
					gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].extra2 = StringHexToLong(fourth);
				else
					gameConfig[countGames].midiBanks[gameConfig[countGames].numberMidis].extra2 = 0x00000000;
				gameConfig[countGames].numberMidis++;
				midiCount++;
			}
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				countGames++;
			}

			lastGame = lastGameNew;

			midiCount = 0;
			nameCount = 0;
			gameConfig[countGames].numberMidis = 0;
			gameConfig[countGames].numberMidiNames = 0;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	fclose(inIni);
}

int CN64MidiToolReader::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void CN64MidiToolReader::ProcessMidis(MidiGameConfig* gameConfig, int gameNumber, std::vector<CString>& addMidiStrings, int& numberMidiStrings, int& numberInstruments, bool& compressed, unsigned char* buffer, int bufferSize, unsigned long& startSpot, unsigned long& endSpot, bool calculateInstrumentCount, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo)
{
	numberInstruments = 0;

	CString gameName = gameConfig[gameNumber].gameType;
	gameName.Trim();

	if (gameName.CompareNoCase("GoldenEye") == 0)
	{
		compress.SetGame(GOLDENEYE);
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = ReadAddiuAddress(buffer, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end) + 4;

			int numberMusicData = CharArrayToShort(&buffer[start-4]);
			
			unsigned long currentSpot = start;
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber < numberMusicData)
			{
				unsigned long musicOffset = (CharArrayToLong(&buffer[currentSpot]) + (start - 4));
				unsigned long compressedSize = CharArrayToShort(&buffer[currentSpot+6]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, compressedSize);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, musicOffset, compressedSize, "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				currentBinaryNumber++;
				currentSpot += 8;
			}		
		}
	}
	else if (gameName.CompareNoCase("MarioParty") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = ReadAddiuAddress(buffer, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end) + 4;

			int numberMusicData = CharArrayToShort(&buffer[start-2]);
			
			unsigned long currentSpot = start;
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber < numberMusicData)
			{
				unsigned long musicOffset = (CharArrayToLong(&buffer[currentSpot]) + (start - 4));
				unsigned long size = CharArrayToLong(&buffer[currentSpot+4]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, size);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, musicOffset, size, "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				currentBinaryNumber++;
				currentSpot += 8;
			}		
		}
	}
	else if (gameName.CompareNoCase("MarioPartyMBF0") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = ReadAddiuAddress(buffer, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end);

			int numberMusicData = CharArrayToShort(&buffer[start+6]);
			
			unsigned long currentSpot = start + 0x40;
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber < numberMusicData)
			{
				unsigned long musicOffset = (CharArrayToLong(&buffer[currentSpot + 0x8]) + start);
				unsigned long size = (CharArrayToLong(&buffer[currentSpot + 0xC]));

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, size);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, musicOffset, size, "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				currentBinaryNumber++;
				currentSpot += 0x10;
			}		
		}
	}
	else if (gameName.CompareNoCase("Glover") == 0)
	{
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = gameConfig[gameNumber].midiBanks[x].start;
			unsigned long end = gameConfig[gameNumber].midiBanks[x].end;
			
			unsigned long countMidi = CharArrayToLong(&buffer[start]);
			unsigned long endMidi = CharArrayToLong(&buffer[start + 0x14]);
			for (int y = 0; y < countMidi; y++)
			{
				unsigned long midiLocation = CharArrayToLong(&buffer[start + 0x18 + (y * 8)]);
				unsigned long midiDivison = CharArrayToLong(&buffer[start + 0x18 + (y * 8) + 4]);
				unsigned long midiEnd;
				if (y == (countMidi - 1))
					midiEnd = endMidi;
				else
					midiEnd = CharArrayToLong(&buffer[start + 0x18 + (y * 8) + 8]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", midiLocation, (midiEnd - midiLocation));
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, (start + midiLocation), (midiEnd - midiLocation), "asdasdaw43.mid", gameName, numberInstTemp, midiDivison, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}
			}
		}
	}
	else if (gameName.CompareNoCase("PerfectDark") == 0)
	{
		compress.SetGame(PD);
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = gameConfig[gameNumber].midiBanks[x].start;
			unsigned long currentSpot = start;
			
			startSpot = (CharArrayToLong(&buffer[currentSpot]) + (start - 4));
			
			int currentBinaryNumber = 0;
			while (currentBinaryNumber <  0x77)
			{
				unsigned long musicOffset = (CharArrayToLong(&buffer[currentSpot]) + gameConfig[gameNumber].midiBanks[0].end);
				unsigned long compressedSize = CharArrayToShort(&buffer[currentSpot+6]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", musicOffset, compressedSize);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, musicOffset, compressedSize, "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				endSpot = musicOffset + compressedSize;

				currentBinaryNumber++;
				currentSpot += 8;
			}
		}
	}
	else if (gameName.CompareNoCase("BanjoKazooie") == 0)
	{
		compress.SetGame(BANJOKAZOOIE);
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
				{
				int numberInstTemp = 0;
				
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("MIDx") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("Sng") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			if (gameConfig[gameNumber].midiBanks[x].extra != 0)
				tempSpotStr.Format("%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), gameConfig[gameNumber].midiBanks[x].extra);
			else
				tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("Konami") == 0)
	{
		bool checksumMatch = true;

		if (gameConfig[gameNumber].extraGameMidiInfo.checksum1 != 0x00000000)
		{
			if (CharArrayToLong(&buffer[0x10]) != gameConfig[gameNumber].extraGameMidiInfo.checksum1)
				checksumMatch = false;
		}

		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = gameConfig[gameNumber].midiBanks[x].start;
			unsigned long end = gameConfig[gameNumber].midiBanks[x].end;

			if (!checksumMatch)
			{
				unsigned long startOffsetWritten = gameConfig[gameNumber].extraGameMidiInfo.trackAddressOffset + (gameConfig[gameNumber].midiBanks[x].extra * 4);
				start = CharArrayToLong(&buffer[startOffsetWritten]);

				unsigned long endOffsetWritten = gameConfig[gameNumber].extraGameMidiInfo.trackAddressEndsOffset + (gameConfig[gameNumber].midiBanks[x].extra * 4);
				end = CharArrayToLong(&buffer[endOffsetWritten]);
			}

			if ((end % 0x10) != 0)
			{
				end = end + (0x10 - (end % 0x10));
			}

			CString tempSpotStr;
			if ((gameConfig[gameNumber].midiBanks[x].extra != 0) || (gameConfig[gameNumber].midiBanks[x].extra2 != 0))
				tempSpotStr.Format("%08X:%08X:%08X:%08X", start, (end - start), gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2);
			else if (gameConfig[gameNumber].midiBanks[x].extra != 0)
				tempSpotStr.Format("%08X:%08X:%08X", start, (end - start), gameConfig[gameNumber].midiBanks[x].extra);
			else
				tempSpotStr.Format("%08X:%08X", start, (end - start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = end - start;

				if (gameConfig[gameNumber].midiBanks[x].extra2 == 1)
				{
					compressed = false;
					// uncompressed tracks
					midiParse.KonamiToMidi(buffer, bufferSize, &buffer[start], fileSizeCompressed, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, gameConfig[gameNumber].midiBanks[x].extra);
				}
				else
				{
					CNaganoDecoder decode;
					unsigned char* outputDecompressed = new unsigned char[0x50000];
					int expectedSize = decode.dec(&buffer[start], fileSizeCompressed, outputDecompressed);

					/*FILE* a = fopen("C:\\temp\\a.bin", "wb");
					fwrite(outputDecompressed, 1, expectedSize, a);
					fclose(a);*/
	
					midiParse.KonamiToMidi(buffer, bufferSize, outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, gameConfig[gameNumber].midiBanks[x].extra);

					delete [] outputDecompressed;
				}

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ZLIBSSEQ") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long sseqTable = gameConfig[gameNumber].midiBanks[x].start;

			int fileSizeCompressed = -1;
			GECompression gedecompress;
			gedecompress.SetGame(MORTALKOMBAT);
			gedecompress.SetCompressedBuffer(&buffer[sseqTable + 0x20], bufferSize - (sseqTable + 0x20));
			int fileSizeUncompressed;
			unsigned char* outputDecompressedSSEQ = gedecompress.OutputDecompressedBuffer(fileSizeUncompressed, fileSizeCompressed);

			/*FILE* a = fopen("C:\\temp\\mk.bin", "wb");
			fwrite(outputDecompressedSSEQ, 1, fileSizeUncompressed, a);
			fflush(a);
			fclose(a);*/

			unsigned long reportedDecompressedSize = CharArrayToLong(&buffer[sseqTable + 0x18]);
			unsigned long reportedCompressedSize = CharArrayToLong(&buffer[sseqTable + 0x14]);

			unsigned long countEntries = reportedDecompressedSize / 0x10;

			unsigned long startData = sseqTable + 0x20 + reportedCompressedSize;

			int songIndex = 0;
			while (songIndex < countEntries)
			{
				unsigned short numberTracks = CharArrayToShort(&outputDecompressedSSEQ[(songIndex * 0x10)]);
				unsigned long lengthData = CharArrayToLong(&outputDecompressedSSEQ[(songIndex * 0x10) + 4]);
				unsigned long offsetData = startData + CharArrayToLong(&outputDecompressedSSEQ[(songIndex * 0x10) + 8]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X:%08X", offsetData, lengthData, numberTracks);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, offsetData, lengthData, "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				songIndex++;
			}

			delete [] outputDecompressedSSEQ;
		}
	}
	else if (gameName.CompareNoCase("SSEQ") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long sseqTable = gameConfig[gameNumber].midiBanks[x].start;

			unsigned long countEntries = CharArrayToLong(&buffer[sseqTable + 0xC]);
			unsigned long tableSize = CharArrayToLong(&buffer[sseqTable + 0x18]);

			unsigned long startData = 0x20 + sseqTable + tableSize;

			int songIndex = 0;
			while (songIndex < countEntries)
			{
				unsigned short numberTracks = CharArrayToShort(&buffer[sseqTable + 0x20 + (songIndex * 0x10)]);
				unsigned long lengthData = CharArrayToLong(&buffer[sseqTable + 0x20 + (songIndex * 0x10) + 4]);
				unsigned long offsetData = startData + CharArrayToLong(&buffer[sseqTable + 0x20 + (songIndex * 0x10) + 8]);

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X:%08X", offsetData, lengthData, numberTracks);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, offsetData, lengthData, "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}

				songIndex++;
			}		
		}
	}
	else if (gameName.CompareNoCase("MKPROTOSSEQ") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long offsetData = gameConfig[gameNumber].midiBanks[x].start;

			while (offsetData < gameConfig[gameNumber].midiBanks[x].end)
			{
				unsigned short numberTracks = CharArrayToShort(&buffer[offsetData + 0x0]);
				offsetData += 8;

				if (numberTracks == 0)
					break;

				int startOffsetData = offsetData;

				for (int x = 0; x < numberTracks; x++)
				{
					unsigned short extraFlag = CharArrayToShort(&buffer[offsetData + 0x12]);
					unsigned long sizeTrack = CharArrayToLong(&buffer[offsetData + 0x14]);
					unsigned long division = CharArrayToLong(&buffer[offsetData + 0xC]);

					if (extraFlag)
					{
						offsetData += 4;
					}
					offsetData += 0x18;
					offsetData += sizeTrack;
				}

				int sizeTrack = offsetData - startOffsetData;
				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X:%08X", startOffsetData, sizeTrack, numberTracks);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, startOffsetData, sizeTrack, "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}
			}		
		}
	}
	else if (gameName.CompareNoCase("PaperMario") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			if ((gameConfig[gameNumber].midiBanks[x].extra != 0) && (gameConfig[gameNumber].midiBanks[x].extra2 != 0))
				tempSpotStr.Format("%08X:%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2);
			else if (gameConfig[gameNumber].midiBanks[x].extra != 0)
				tempSpotStr.Format("%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), gameConfig[gameNumber].midiBanks[x].extra);
			else
				tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("BanjoTooie") == 0)
	{
		compress.SetGame(BANJOTOOIE);
		compressed = true;

		// TODO
		//BT 13393bc
		//BT 138f044

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}

		
	}
	else if (gameName.CompareNoCase("DonkeyKong") == 0)
	{
		compress.SetGame(DONKEYKONG64);
		compressed  = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			unsigned long start = gameConfig[gameNumber].midiBanks[x].start;
			unsigned long  currentSpot = start;

			bool endFlag = false;

			while (!endFlag)
			{
				// algorithm to find each length, check latest offset, then find end from meta code

				unsigned long startCurrentSpot = currentSpot;
				unsigned long byteCheck = 0;

				// 00FF2F ends
				while ((buffer[currentSpot + byteCheck] != 0x1F) || (buffer[currentSpot + byteCheck + 1] != 0x8B))
				{
					byteCheck++;
				}

				// move ahead 1
				currentSpot++;

				while ((currentSpot <  gameConfig[gameNumber].midiBanks[x].end))
				{
					if ((buffer[currentSpot] != 0x1F) || (buffer[currentSpot + 1] != 0x8B) || (buffer[currentSpot + 2] != 0x08))
					{
						currentSpot+=1;
					}
					else
					{
						break;
					}
				}

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", startCurrentSpot, (currentSpot - startCurrentSpot));
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, startCurrentSpot, (currentSpot - startCurrentSpot), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}
				
				if (currentSpot >= gameConfig[gameNumber].midiBanks[x].end)
					endFlag = true;
			}
		}	
	}
	else if (gameName.CompareNoCase("RNCSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				RncDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.unpackM1(&buffer[gameConfig[gameNumber].midiBanks[x].start], outputDecompressed, 0x0000, fileSizeCompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("HexenSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				CHexenDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.decode(&buffer[gameConfig[gameNumber].midiBanks[x].start], outputDecompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ASMICSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				CASMICDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], fileSizeCompressed, outputDecompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("SnowSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start;
				CSnowDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], fileSizeCompressed, outputDecompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ArmySargeSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				
				int fileSizeCompressed = -1;
				n643docompression decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.uncompressedSize(&buffer[gameConfig[gameNumber].midiBanks[x].start], true);

				expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], expectedSize, outputDecompressed, fileSizeCompressed, true, false);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ArmySarge2Sng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				
				int fileSizeCompressed = -1;
				n643docompression decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.uncompressedSize(&buffer[gameConfig[gameNumber].midiBanks[x].start], true);

				expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], expectedSize, outputDecompressed, fileSizeCompressed, true, true);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("Quake2Sng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				
				CQuakeDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];

				int decompressedSize = decode.aridecode(&buffer[gameConfig[gameNumber].midiBanks[x].start], 0x50000, outputDecompressed, decompressedSize);

				midiParse.SngToMidi(outputDecompressed, decompressedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("TazSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				CString inTempFileName;
				inTempFileName.Format("tempASAS%08X.mus", gameConfig[gameNumber].midiBanks[x].start);

				CString outTempFileName;
				outTempFileName.Format("tempASAS%08X.musb", gameConfig[gameNumber].midiBanks[x].start);

				::DeleteFile(inTempFileName);
				::DeleteFile(outTempFileName);

				FILE* outTempIn = fopen(inTempFileName, "wb");
				if (outTempIn == NULL)
				{
					continue;
				}

				int musSize = gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start;

				unsigned long expectedSize = CharArrayToLong(&buffer[gameConfig[gameNumber].midiBanks[x].start+4]) - 0x14;
				fwrite(&expectedSize, 1, 4, outTempIn);
				fwrite(&buffer[gameConfig[gameNumber].midiBanks[x].start+0x14], 1, musSize-0x14, outTempIn);
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

				musSize = GetSizeFile(outTempFileName);

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


				int numberInstTemp = 0;
				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ARISng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				CString inTempFileName;
				inTempFileName.Format("tempASAS%08X.mus", gameConfig[gameNumber].midiBanks[x].start);

				CString outTempFileName;
				outTempFileName.Format("tempASAS%08X.musb", gameConfig[gameNumber].midiBanks[x].start);

				::DeleteFile(inTempFileName);
				::DeleteFile(outTempFileName);

				FILE* outTempIn = fopen(inTempFileName, "wb");
				if (outTempIn == NULL)
				{
					continue;
				}

				int musSize = gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start;

				unsigned long expectedSize = CharArrayToLong(&buffer[gameConfig[gameNumber].midiBanks[x].start-4]);
				fwrite(&expectedSize, 1, 4, outTempIn);
				fwrite(&buffer[gameConfig[gameNumber].midiBanks[x].start], 1, musSize, outTempIn);
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

				fclose(lzari.infile);
				fclose(lzari.outfile);

				musSize = GetSizeFile(outTempFileName);

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


				int numberInstTemp = 0;
				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("FLA2Sng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				CFLA2Decoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];

				int expectedSizeUncompressed = Flip32Bit(CharArrayToLong(&buffer[gameConfig[gameNumber].midiBanks[x].start+4]));

				int expectedSize = decode.decFLA2(&buffer[gameConfig[gameNumber].midiBanks[x].start+8], fileSizeCompressed, expectedSizeUncompressed, outputDecompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("RNCSngOffset") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				RncDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.unpackM1(&buffer[gameConfig[gameNumber].midiBanks[x].start], outputDecompressed, 0x0000, fileSizeCompressed);

				unsigned long offset = CharArrayToLong(&outputDecompressed[0]);

				midiParse.SngToMidi(&outputDecompressed[offset], expectedSize - offset, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("Yay0Sng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				YAY0 decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.decodeAll(&buffer[gameConfig[gameNumber].midiBanks[x].start], outputDecompressed, fileSizeCompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("BlitzSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				CBlitzDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], outputDecompressed, fileSizeCompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("MarioTennisSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = -1;
				CMarioTennisDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], fileSizeCompressed, outputDecompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ZLibSng") == 0)
	{
		compress.SetGame(NOHEADER);
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if ((gameName.CompareNoCase("Factor5Zlb") == 0) || (gameName.CompareNoCase("Factor5ZlbGCStyle") == 0) || (gameName.CompareNoCase("Factor5LZGCStyle") == 0) || (gameName.CompareNoCase("Factor5ZlbNoHeaderGCStyle") == 0))
	{
		compress.SetGame(STUNTRACER64);
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ZLibIndexedSng") == 0)
	{
		compress.SetGame(NOHEADER);
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			/*if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				midiParse.ExportToMidi(gameConfig[gameNumber].gameName, buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), "asdasdaw43.mid", gameName, numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, true, separateByInstrument, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}*/
		}
	}
	else if (gameName.CompareNoCase("MultiPartTigSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				// TODO
			}
		}
	}
	else if (gameName.CompareNoCase("LZSS_0BSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start);
				CMidwayDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start+4], fileSizeCompressed, outputDecompressed, "LZSS_0");
				
				/*FILE* a = fopen("C:\\temp\\a.bin", "wb");
				fwrite(outputDecompressed, 1, expectedSize, a);
				fclose(a);*/
				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("LZSS_0Sng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start);
				CMidwayDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], fileSizeCompressed, outputDecompressed, "LZSS_0");
				
				/*FILE* a = fopen("C:\\temp\\a.bin", "wb");
				fwrite(outputDecompressed, 1, expectedSize, a);
				fclose(a);*/
				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("AVL_0Sng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start);
				CMidwayDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], fileSizeCompressed, outputDecompressed, "LZSS");
				
				/*FILE* a = fopen("C:\\temp\\a.bin", "wb");
				fwrite(outputDecompressed, 1, expectedSize, a);
				fclose(a);*/
				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("VigilanteSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start);
				CVigilanteDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], fileSizeCompressed, outputDecompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("ViewpointSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start);
				CViewpointDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[gameConfig[gameNumber].midiBanks[x].extra];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], outputDecompressed, fileSizeCompressed, gameConfig[gameNumber].midiBanks[x].extra);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("RugratsSng") == 0)
	{
		compressed = true;

		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				int fileSizeCompressed = (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start);
				CRugratsDecoder decode;
				unsigned char* outputDecompressed = new unsigned char[0x50000];
				int expectedSize = decode.dec(&buffer[gameConfig[gameNumber].midiBanks[x].start], fileSizeCompressed, outputDecompressed);

				midiParse.SngToMidi(outputDecompressed, expectedSize, "asdasdaw43.mid", numberInstTemp, true, separateByInstrument, gameConfig[gameNumber].midiBanks[x].extra);

				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				delete [] outputDecompressed;
				::DeleteFile("asdasdaw43.mid");
			}
		}
	}
	else if (gameName.CompareNoCase("MidiLZSSWilliams") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			int numberMidi = CharArrayToShort(&buffer[gameConfig[gameNumber].midiBanks[x].start+2]);

			for (int y = 0; y < numberMidi; y++)
			{
				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start + CharArrayToLong(&buffer[gameConfig[gameNumber].midiBanks[x].start + 4 + (y * 8)]), CharArrayToLong(&buffer[gameConfig[gameNumber].midiBanks[x].start + 4 + (y * 8) + 4]));
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;
			}
		}
	}
	else if (gameName.CompareNoCase("Midi") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("ZTRK") == 0)
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("Yaz0EADZelda") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%02X:%08X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("ZLibEADZelda") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%02X:%08X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("EADZelda") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%02X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("EADMario") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%02X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("EADStarFox") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%02X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("Seq64Mario") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%02X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("Seq64Zelda") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%02X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("Yaz0Seq64") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("ZipSng") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("MultipartZLibXMFastTracker2") == 0)
	{
		compressed = true;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("DCM") == 0)
	{
		compressed = true;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("ImpulseTracker") == 0)
	{
		compressed = true;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), gameConfig[gameNumber].midiBanks[x].extra, gameConfig[gameNumber].midiBanks[x].extra2);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("Aidyn") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("SmpOffsetAidDebug") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("LZSamplesDCM") == 0)
	{
		compressed = true;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("TitusMidi") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("RNCMidi") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("RobotechN64Midi") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, (gameConfig[gameNumber].midiBanks[x].end - gameConfig[gameNumber].midiBanks[x].start), gameConfig[gameNumber].midiBanks[x].extra);
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;
		}
	}
	else if (gameName.CompareNoCase("RNCSeq") == 0)
	{
		compressed = true;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			int fileSizeCompressed = -1;
			RncDecoder decode;
			unsigned char* outputDecompressed = new unsigned char[0x100000];
			int expectedSize = decode.unpackM1(&buffer[gameConfig[gameNumber].midiBanks[x].start], outputDecompressed, 0x0000, fileSizeCompressed);

			unsigned long offsetMidis = CharArrayToLong(&outputDecompressed[0x8]);
			int numberMidis = CharArrayToLong(&outputDecompressed[offsetMidis]);

			for (int y = 0; y < numberMidis; y++)
			{
				unsigned long offsetMidiData = offsetMidis + (4 * (y + 1));
				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X", gameConfig[gameNumber].midiBanks[x].start, offsetMidiData);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;
			}

			delete [] outputDecompressed;
		}
	}
	else if (gameName.CompareNoCase("DuckDodgers") == 0)
	{
		compressed = false;
		//midiParse.ImportMidiConfig("aerofightersassault.txt");
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			for (int y = gameConfig[gameNumber].midiBanks[x].start; y < gameConfig[gameNumber].midiBanks[x].end; y += 8)
			{
				unsigned long musicFrequency = CharArrayToLong(&buffer[y]);
				unsigned long romOffset = gameConfig[gameNumber].midiBanks[x].extra + CharArrayToLong(&buffer[y+4]);
				unsigned long romOffsetNext;
				if (y < (gameConfig[gameNumber].midiBanks[x].end - 8))
					romOffsetNext = gameConfig[gameNumber].midiBanks[x].extra + CharArrayToLong(&buffer[y+4+8]);
				else
					romOffsetNext = gameConfig[gameNumber].midiBanks[x].extra2;

				int midiLength = romOffsetNext - romOffset;
				
				/*FILE* a = fopen("C:\\temp\\a.bin", "wb");
				fwrite(tempGEMidi, 1, midiLength + 0x44, a);
				fflush(a);
				fclose(a);*/

				CString tempSpotStr;
				tempSpotStr.Format("%08X:%08X:%08X", romOffset, romOffsetNext, musicFrequency);
				addMidiStrings.push_back(tempSpotStr);
				numberMidiStrings++;

				if (calculateInstrumentCount)
				{
					int numberInstTemp = 0;
					bool hasLoopPoint = false;
					int loopStart = 0;
					int loopEnd = 0;
					unsigned long extra = 0;
					unsigned long extra2 = 0;
					midiParse.ExportToMidi("Unknown", buffer, bufferSize, romOffset, midiLength, "asdasdaw43.mid", "DuckDodgers", numberInstTemp, musicFrequency, compressed, hasLoopPoint, loopStart, loopEnd, false, separateByInstrument, false, musicFrequency, 0, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
					if (numberInstTemp > numberInstruments)
						numberInstruments = numberInstTemp;
					::DeleteFile("asdasdaw43.mid");
				}
			}
		}
	}
	else
	{
		compressed = false;
		for (int x = 0; x < gameConfig[gameNumber].numberMidis; x++)
		{
			ParseUncompressedType(buffer, bufferSize, gameConfig[gameNumber].midiBanks[x].start, gameConfig[gameNumber].midiBanks[x].end, addMidiStrings, numberMidiStrings, numberInstruments, compressed, buffer, calculateInstrumentCount, separateByInstrument, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo);
		}
	}

	for (int x = 0; x < addMidiStrings.size(); x++)
	{
		if (x < gameConfig[gameNumber].numberMidiNames)
		{
			addMidiStrings[x] += " " + gameConfig[gameNumber].midiNames[x];
		}
		else
		{
			break;
		}
	}
}

unsigned long CN64MidiToolReader::ReadAddiuAddress(unsigned char* GEROM, unsigned long upperLocation, unsigned long lowerLocation)
{
	unsigned short upper = CharArrayToShort(&GEROM[upperLocation+2]);
	unsigned short lower = CharArrayToShort(&GEROM[lowerLocation+2]);
	if (lower > 0x7FFF)
	{
		return ((upper - 1) << 16) | lower;
	}
	else
	{
		return ((upper) << 16) | lower;
	}
}

void CN64MidiToolReader::ParseUncompressedType(unsigned char* gamebuffer, int gamebufferSize, unsigned long start, unsigned long endSpot, std::vector<CString>& addMidiStrings, int& numberMidiStrings, int& numberInstruments, bool& compressed, unsigned char* buffer, bool calculateInstrumentCount, bool separateByInstrument, bool writeOutLoops, int loopWriteCount, bool extendTracksToHighest, ExtraGameMidiInfo extraGameMidiInfo)
{
	unsigned long  currentSpot = start;

	bool endFlag = false;

	while (!endFlag)
	{
		// algorithm to find each length, check latest offset, then find end from meta code

		unsigned long startCurrentSpot = currentSpot;

		bool invalidMidi = false;
		unsigned long highestEnd = 0x00000044;
		for (unsigned long test = currentSpot; test < currentSpot + 0x40; test += 4)
		{
			unsigned long offsetToMidiStart = CharArrayToLong(&gamebuffer[test]);
			if (((offsetToMidiStart >> 8) & 0xFFFFFFF) == 0x00FF2F)
			{
				// blast corps 47D014 no headers, just data
				highestEnd = offsetToMidiStart + 4;
				invalidMidi = true;
				break;
			}

			unsigned long byteCheck = 0;

			if (offsetToMidiStart == 0x00000000)
				continue;

			bool endedWrongly = false;
			// FF2F ends
			while (true)
			{
				if ((gamebuffer[currentSpot + offsetToMidiStart + byteCheck] == 0xFF) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 1] == 0x2F))
				{
					byteCheck += 2;
					break;
				}
				if ((gamebuffer[currentSpot + offsetToMidiStart + byteCheck] == 0x00) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 1] == 0x00) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 2] == 0x00)  && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 3] == 0x44))
				{
					endedWrongly = true;
					break;
				}
				if ((gamebuffer[currentSpot + offsetToMidiStart + byteCheck] == 0x42) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 1] == 0x31) && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 2] == 0x00)  && (gamebuffer[currentSpot + offsetToMidiStart + byteCheck + 3] == 0x01))
				{
					// Toon Panic Hack
					endedWrongly = true;
					break;
				}
				byteCheck++;
				if (byteCheck > endSpot)
				{
					break;
				}
			}

			if ((offsetToMidiStart + byteCheck) > highestEnd)
				highestEnd = (offsetToMidiStart + byteCheck);
		}

		if ((highestEnd % 4) != 0)
			highestEnd += (4 - (highestEnd % 4));

		currentSpot += highestEnd;

		while ((currentSpot < endSpot))
		{
			if ((gamebuffer[currentSpot] != 0x00) || (gamebuffer[currentSpot + 1] != 0x00) || (gamebuffer[currentSpot + 2] != 0x0)  || (gamebuffer[currentSpot + 3] != 0x44) || (gamebuffer[currentSpot + 4] != 0x00))
			{
				currentSpot+=4;
			}
			else
			{
				break;
			}
		}

		if (!invalidMidi)
		{
			CString tempSpotStr;
			tempSpotStr.Format("%08X:%08X", startCurrentSpot, (currentSpot - startCurrentSpot));
			addMidiStrings.push_back(tempSpotStr);
			numberMidiStrings++;

			if (calculateInstrumentCount)
			{
				int numberInstTemp = 0;
				bool hasLoopPoint = false;
				int loopStart = 0;
				int loopEnd = 0;
				unsigned long extra = 0;
				unsigned long extra2 = 0;
				midiParse.ExportToMidi("Unknown", gamebuffer, gamebufferSize, startCurrentSpot, (currentSpot - startCurrentSpot), "asdasdaw43.mid", "Uncompressed", numberInstTemp, 0, compressed, hasLoopPoint, loopStart, loopEnd, false, separateByInstrument, false, extra, extra2, writeOutLoops, loopWriteCount, extendTracksToHighest, extraGameMidiInfo, false);
				if (numberInstTemp > numberInstruments)
					numberInstruments = numberInstTemp;
				::DeleteFile("asdasdaw43.mid");
			}
		}
		
		if (currentSpot >= endSpot)
			endFlag = true;
	}
	
	compressed  = false;
}