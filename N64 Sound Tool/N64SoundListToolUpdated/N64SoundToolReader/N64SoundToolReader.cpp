#include "StdAfx.h"
#include "N64SoundToolReader.h"
#include "..\N64SoundLibrary\TigDecoder.h"
#include "..\N64SoundLibrary\QuakeDecoder.h"

CN64SoundToolReader::CN64SoundToolReader(void)
{
}

CN64SoundToolReader::~CN64SoundToolReader(void)
{
}

int CN64SoundToolReader::HexToInt(char inChar)
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

unsigned long CN64SoundToolReader::StringHexToLong(CString inString)
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

void CN64SoundToolReader::InitializeSpecificGames(CString iniPath, int& countGames, SoundGameConfig*& gameConfig)
{
	FILE* inIni = fopen(iniPath, "r");
	if (inIni == NULL)
	{
		MessageBox(NULL, "Error reading gameconfigsound.ini, file must exist", "Error", NULL);
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

	gameConfig = new SoundGameConfig[countGames];
	countGames = 0;
	lastGame = "";
	
	int soundBankCount = 0;
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
			}
		}
		else if (currentLine[0] == '|')
		{
			nameCount++;
		}
		else if (currentLine[0] == '{')
		{

		}
		else
		{
			soundBankCount++;
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				gameConfig[countGames].soundBanks = new CtlTblConfig[soundBankCount];
				countGames++;
			}

			soundBankCount = 0;
			nameCount = 0;
			lastGame = lastGameNew;
			gameConfig[countGames].gameName = lastGameNew;
			gameConfig[countGames].gameName.Trim();
			gameConfig[countGames].gameType = gameType;
			gameConfig[countGames].gameType.Trim();
			gameConfig[countGames].numberSoundBanks = 0;
		}
	}

	if (lastGame != "")
	{
		gameConfig[countGames].soundBanks = new CtlTblConfig[soundBankCount];
		gameConfig[countGames].gameType = gameType;
		gameConfig[countGames].gameType.Trim();
		gameConfig[countGames].numberSoundBanks = 0;
		countGames++;
	}


	rewind(inIni);

	countGames = 0;
	lastGame = "";

	bool overrideSamplingRate = false;
	int samplingRate = 22050;
	T1Config t1TempConfig;

	while (!feof(inIni))
	{
		char currentLine[1000];
		fgets(currentLine, 1000, inIni);

		CString lastGameNew = lastGame;

		if (currentLine[0] == '/')
			continue;
		else if (currentLine[0] == '[')
		{
			overrideSamplingRate = false;

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
			gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].instrumentNames.push_back(&currentLine[1]);
		}
		else if (currentLine[0] == '{')
		{
			CString line;
			line.Format("%s", currentLine);

			line.Replace("{", "");
			line.Replace("}", "");
			line.Replace(" ", "");

			if ((line.Find("HalfSampling")) != -1)
			{
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].halfSamplingRate = true;
			}
			else if (line.Find("AllSamplingRate") != -1)
			{
				line.Replace("AllSamplingRate", "");
				line.Replace(":", "");
				overrideSamplingRate = true;
				samplingRate = atoi(line);
			}
			else if (line.Find("SamplingRate") != -1)
			{
				line.Replace("SamplingRate", "");
				line.Replace(":", "");
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].overrideSamplingRate = true;
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks-1].samplingRate = atoi(line);
			}
			else if ((line.Find("OffsetZeldaCtlTable")) != -1)
			{
				line.Replace("OffsetZeldaCtlTable", "");
				line.Replace(":", "");
				gameConfig[countGames].offsetZeldaCtlTable = StringHexToLong(line);
			}
			else if ((line.Find("OffsetZeldaTblTable")) != -1)
			{
				line.Replace("OffsetZeldaTblTable", "");
				line.Replace(":", "");
				gameConfig[countGames].offsetZeldaTblTable = StringHexToLong(line);
			}
			else if ((line.Find("StartZeldaCtlData")) != -1)
			{
				line.Replace("StartZeldaCtlData", "");
				line.Replace(":", "");
				gameConfig[countGames].startZeldaCtlData = StringHexToLong(line);
			}
			else if ((line.Find("StartZeldaTblData")) != -1)
			{
				line.Replace("StartZeldaTblData", "");
				line.Replace(":", "");
				gameConfig[countGames].startZeldaTblData = StringHexToLong(line);
			}
			else if ((line.Find("EndZeldaCtlData")) != -1)
			{
				line.Replace("EndZeldaCtlData", "");
				line.Replace(":", "");
				gameConfig[countGames].endZeldaCtlData = StringHexToLong(line);
			}
			else if ((line.Find("EndZeldaTblData")) != -1)
			{
				line.Replace("EndZeldaTblData", "");
				line.Replace(":", "");
				gameConfig[countGames].endZeldaTblData = StringHexToLong(line);
			}
			else if ((line.Find("IsCompressedZeldaCtlTblTables")) != -1)
			{
				line.Replace("IsCompressedZeldaCtlTblTables", "");
				line.Replace(":", "");
				line.Trim();
				if ((line.Find("true") != -1) || (line.Find("True") != -1) || (line.Find("1") != -1))
				{
					gameConfig[countGames].isCompressedZeldaCtlTblTables = true;
				}
				else
				{
					gameConfig[countGames].isCompressedZeldaCtlTblTables = false;
				}
			}
			else if ((line.Find("IsZLib")) != -1)
			{
				line.Replace("IsZLib", "");
				line.Replace(":", "");
				line.Trim();
				if ((line.Find("true") != -1) || (line.Find("True") != -1) || (line.Find("1") != -1))
				{
					gameConfig[countGames].isZLib = true;
				}
				else
				{
					gameConfig[countGames].isZLib = false;
				}
			}
			else if ((line.Find("CompressedZeldaCtlTblTableStart")) != -1)
			{
				line.Replace("CompressedZeldaCtlTblTableStart", "");
				line.Replace(":", "");
				gameConfig[countGames].compressedZeldaCtlTblTableStart = StringHexToLong(line);
			}
			else if ((line.Find("CompressedZeldaCtlTblTableEnd")) != -1)
			{
				line.Replace("CompressedZeldaCtlTblTableEnd", "");
				line.Replace(":", "");
				gameConfig[countGames].compressedZeldaCtlTblTableEnd = StringHexToLong(line);
			}
			else if ((line.Find("T1StartBank")) != -1)
			{
				line.Replace("T1StartBank", "");
				line.Replace(":", "");
				t1TempConfig.bankReferenceStart = StringHexToLong(line);
			}
			else if ((line.Find("T1StartAddress")) != -1)
			{
				line.Replace("T1StartAddress", "");
				line.Replace(":", "");
				t1TempConfig.t1Start = StringHexToLong(line);
			}
			else if ((line.Find("T1EndAddress")) != -1)
			{
				line.Replace("T1EndAddress", "");
				line.Replace(":", "");
				t1TempConfig.t1End = StringHexToLong(line);
			}
			else if ((line.Find("T1EndBank")) != -1)
			{
				line.Replace("T1EndBank", "");
				line.Replace(":", "");
				t1TempConfig.bankReferenceEnd = StringHexToLong(line);

				gameConfig[countGames].t1Config.push_back(t1TempConfig);
			}
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
				CString fifth = "";
				CString sixth = "";

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

						int commaSpot4 = fourth.Find(",");
						if (commaSpot4 != -1)
						{
							fifth = fourth.Mid((commaSpot4 + 1), (fourth.GetLength() - commaSpot4 - 1));
							fourth = fourth.Mid(0,commaSpot4);

							int commaSpot5 = fifth.Find(",");
							if (commaSpot5 != -1)
							{
								sixth = fifth.Mid((commaSpot5 + 1), (fifth.GetLength() - commaSpot5 - 1));
								fifth = fifth.Mid(0,commaSpot5);
							}
						}
					}
				}

				second.Replace("\r", "");
				second.Replace("\n", "");
				third.Replace("\r", "");
				third.Replace("\n", "");
				fourth.Replace("\r", "");
				fourth.Replace("\n", "");
				fifth.Replace("\r", "");
				fifth.Replace("\n", "");
				sixth.Replace("\r", "");
				sixth.Replace("\n", "");

				if (overrideSamplingRate)
				{
					if (!gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].overrideSamplingRate)
					{
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].overrideSamplingRate = true;
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].samplingRate = samplingRate;
					}
				}

				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].ctl = StringHexToLong(first);
				gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].tbl = StringHexToLong(second);
				if (third.GetLength() > 0)
				{
					if (gameConfig[countGames].gameType.CompareNoCase("vox") == 0)
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = atoi(third);
					else
						gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = StringHexToLong(third);
				}
				else
				{
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].numberInstruments = 0;
				}
				if (fourth.GetLength() > 0)
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].mask = StringHexToLong(fourth);
				else
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].mask = -1;
				if (fifth.GetLength() > 0)
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].extra = StringHexToLong(fifth);
				else
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].extra = 0;
				if (sixth.GetLength() > 0)
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].extra2 = StringHexToLong(sixth);
				else
					gameConfig[countGames].soundBanks[gameConfig[countGames].numberSoundBanks].extra2 = 0;

				gameConfig[countGames].numberSoundBanks++;
				soundBankCount++;
			}
		}

		if (lastGameNew != lastGame)
		{
			if (lastGame != "")
			{
				countGames++;
			}

			lastGame = lastGameNew;

			soundBankCount = 0;
			gameConfig[countGames].numberSoundBanks = 0;
		}
	}

	if (lastGame != "")
	{
		countGames++;
	}

	fclose(inIni);
}

unsigned long CN64SoundToolReader::GetRegionSize(unsigned long currentSpot, SoundGameConfig gameConfig, unsigned long romSize)
{
	unsigned long endSpot = romSize;
	for (int x = 0; x < gameConfig.numberSoundBanks; x++)
	{
		if (gameConfig.soundBanks[x].ctl > currentSpot)
		{
			if (gameConfig.soundBanks[x].ctl < endSpot)
				endSpot = gameConfig.soundBanks[x].ctl;
		}

		if (gameConfig.soundBanks[x].tbl > currentSpot)
		{
			if (gameConfig.soundBanks[x].tbl < endSpot)
				endSpot = gameConfig.soundBanks[x].tbl;
		}
	}

	return endSpot - currentSpot;
}

void CN64SoundToolReader::ReadSoundbanks(unsigned char* ROM, int romSize, SoundGameConfig gameConfig, int& numberResults, std::vector<ctlTblResult>& results, std::vector<t1Result>& t1results)
{
	numberResults = 0;
	if (gameConfig.gameType.CompareNoCase("MultiPartERZN64WavePtrV2") == 0)
	{
		unsigned char* ctlBinaryPre = new unsigned char[0x2000000];
		unsigned long  ctlSize = 0;

		for (int x = 0; x < gameConfig.numberSoundBanks; x++)
		{
			RncDecoder rnc;
			int fileSizeCompressed = romSize - gameConfig.soundBanks[x].ctl;
			if (fileSizeCompressed > 0xFE000)
				fileSizeCompressed = 0xFE000;
			unsigned char* outputDecompressed = new unsigned char[0x2000000];
			ROM[gameConfig.soundBanks[x].ctl] = 0x52;
			ROM[gameConfig.soundBanks[x].ctl+1] = 0x4E;
			ROM[gameConfig.soundBanks[x].ctl+2] = 0x43;
			int fileSize = rnc.unpackM1(&ROM[gameConfig.soundBanks[x].ctl], outputDecompressed, 0, fileSizeCompressed);
			memcpy(&ctlBinaryPre[ctlSize], outputDecompressed, fileSize);
			ctlSize += fileSize;
			delete [] outputDecompressed;
		}

		unsigned char* ctlBinary = new unsigned char[ctlSize - gameConfig.soundBanks[0].mask];
		memcpy(ctlBinary, &ctlBinaryPre[gameConfig.soundBanks[0].mask], (ctlSize - gameConfig.soundBanks[0].mask));
		delete [] ctlBinaryPre;
		ctlSize = ctlSize - gameConfig.soundBanks[0].mask;

		
		results.resize(numberResults+1);

		results[numberResults].ctlOffset = gameConfig.soundBanks[0].ctl;
		results[numberResults].ctlSize = ctlSize;
		results[numberResults].tblOffset = gameConfig.soundBanks[0].tbl;
		results[numberResults].tblSize = romSize - results[0].tblOffset;
		results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinary, ctlSize, 0, &ROM[results[0].tblOffset]);
		delete [] ctlBinary;
		numberResults = 1;
	}
	else if (gameConfig.gameType.CompareNoCase("MultiPartTigWavePtrV2") == 0)
	{
		unsigned char* ctlBinaryPre = new unsigned char[0x2000000];
		unsigned long  ctlSize = 0;

		int currentIndex = -1;

		int start = 0;

		for (int x = 0; x < gameConfig.numberSoundBanks; x++)
		{
			if ((int)gameConfig.soundBanks[x].numberInstruments < currentIndex)
			{
				results.resize(numberResults+1);

				results[numberResults].ctlOffset = gameConfig.soundBanks[start].ctl;
				results[numberResults].ctlSize = ctlSize;
				results[numberResults].tblOffset = gameConfig.soundBanks[start].tbl;
				results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinaryPre, ctlSize, 0, &ROM[results[numberResults].tblOffset]);
				delete [] ctlBinaryPre;
				numberResults++;

				start = x;
				currentIndex = 0;
				// start a new one

				ctlBinaryPre = new unsigned char[0x2000000];
				ctlSize = 0;
			}

			TigDecoder tig;
			int fileSizeCompressed = romSize - gameConfig.soundBanks[x].ctl;
			if (fileSizeCompressed > 0xFE000)
				fileSizeCompressed = 0xFE000;
			unsigned char* outputDecompressed = new unsigned char[0x2000000];
			
			unsigned long address = gameConfig.soundBanks[x].ctl;
			unsigned long compressedsize = ((((((ROM[address+3] << 8) | ROM[address+2]) << 8) | ROM[address+1]) << 8) | ROM[address+0]);
			unsigned char type = ROM[address+4];

			if (type == 1)  // no compression
			{
				memcpy(&ctlBinaryPre[ctlSize], &ROM[address+8], compressedsize-8);
				ctlSize += compressedsize-8;
			}
			else if (type == 0)
			{
				int fileSize = tig.dec(&ROM[address+8], compressedsize, outputDecompressed);
				if (fileSize > 0x1000)
					fileSize = 0x1008;
				memcpy(&ctlBinaryPre[ctlSize], outputDecompressed, fileSize-8);
				ctlSize += fileSize-8;
			}

			delete [] outputDecompressed;

			currentIndex++;
		}

		results.resize(numberResults + 1);

		results[numberResults].ctlOffset = gameConfig.soundBanks[start].ctl;
		results[numberResults].ctlSize = ctlSize;
		results[numberResults].tblOffset = gameConfig.soundBanks[start].tbl;
		results[numberResults].tblSize = romSize - results[numberResults].tblOffset;
		results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(ctlBinaryPre, ctlSize, 0, &ROM[results[numberResults].tblOffset]);
		delete [] ctlBinaryPre;
		
		numberResults++;
	}
	else
	{
		for (int x = 0; x < gameConfig.numberSoundBanks; x++)
		{
			results.resize(numberResults+1);

			results[numberResults].ctlOffset = gameConfig.soundBanks[x].ctl;
			results[numberResults].tblOffset = gameConfig.soundBanks[x].tbl;

			results[numberResults].ctlSize = GetRegionSize(results[numberResults].ctlOffset, gameConfig, romSize);
			results[numberResults].tblSize = GetRegionSize(results[numberResults].tblOffset, gameConfig, romSize);
			
			if (gameConfig.gameType.CompareNoCase("RawTest") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRawTest(&ROM[0], results[numberResults].ctlSize);
			}
			else if (gameConfig.gameType.CompareNoCase("SF64Uncompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioStarFox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, &ROM[0], romSize, gameConfig.offsetZeldaCtlTable, gameConfig.offsetZeldaTblTable, gameConfig.startZeldaCtlData, gameConfig.startZeldaTblData, numberResults, gameConfig.isCompressedZeldaCtlTblTables, gameConfig.compressedZeldaCtlTblTableStart, gameConfig.compressedZeldaCtlTblTableEnd);
			}
			else if (gameConfig.gameType.CompareNoCase("ZeldaUncompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZelda(results, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, results[numberResults].tblOffset, gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, &ROM[0], romSize, gameConfig.offsetZeldaCtlTable, gameConfig.offsetZeldaTblTable, gameConfig.startZeldaCtlData, gameConfig.startZeldaTblData, numberResults, gameConfig.isCompressedZeldaCtlTblTables, gameConfig.compressedZeldaCtlTblTableStart, gameConfig.compressedZeldaCtlTblTableEnd, gameConfig.isZLib);
			}
			else if (gameConfig.gameType.CompareNoCase("TurokUncompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioTurok(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, &ROM[0], 0);
			}
			else if (gameConfig.gameType.CompareNoCase("ArmyMenCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, false);
			}
			else if (gameConfig.gameType.CompareNoCase("SargeArmyMenCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], true, true);
			}
			else if (gameConfig.gameType.CompareNoCase("ArmyMenAirCombatCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioArmyMenCompressed(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], false, false);
			}
			else if (gameConfig.gameType.CompareNoCase("SnoWave") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioSno(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("RNCCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadRNCAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("RNXCompressed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadRNXAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("MegaManN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMegamanN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("N64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("PaperMario") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioPaperMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("B0") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioB0(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("DuckDodgers") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioDuckDodgers(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("SydneyWavePtr") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioSydney(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("NinDec") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioNinDec(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("TazHuffmanWavePtr") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioTazN64PtrWavetableV2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("RNCN64Wave") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64Ptr(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("AVL_0Wave") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioAVL_0Ptr(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("RNCN64OffsetWave") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRNCN64PtrOffset(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("BnkB") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioBnkB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("N64WavePtrTableV1") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV1(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("H20Raw816") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioH20Raw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("TetrisphereRaw816") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioTetrisphereRaw816(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("YAY0N64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2YAY0(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("QuakeN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableQuake2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("SnowboardKidsN64WavePtrTable") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableSnowboardKids(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("ViewpointN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2Viewpoints(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("Titus") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64Titus(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("MKMythologies") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64MKMythologies(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("BlitzN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableBlitz(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}	
			else if (gameConfig.gameType.CompareNoCase("ZLibN64WavePtrTableV2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64PtrWavetableV2ZLIB(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("ZLIBSN64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64Zlib(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.CompareNoCase("SN64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioN64SN64(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.CompareNoCase("SuperMario64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("MarioKart64") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMario(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], results[numberResults].tblSize, gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("StandardRawAllowed") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioRawAllowed(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, 0);
			}
			else if (gameConfig.gameType.CompareNoCase("ASMStandardRawAllowed") == 0)
			{
				unsigned long ctlOffset = n64AudioLibrary.ReadAddiuAddress(ROM, results[numberResults].ctlOffset, results[numberResults].tblOffset);
				unsigned long tblOffset = n64AudioLibrary.ReadAddiuAddress(ROM, gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
				unsigned long ctlSize = tblOffset - ctlOffset;

				results[numberResults].ctlOffset = ctlOffset;
				results[numberResults].tblOffset = tblOffset;
				results[numberResults].ctlSize = tblOffset - ctlOffset;
				
				if (numberResults < (gameConfig.numberSoundBanks - 1))
				{
					unsigned long ctlOffsetNext = n64AudioLibrary.ReadAddiuAddress(ROM, gameConfig.soundBanks[x+1].ctl, gameConfig.soundBanks[x+1].tbl);
					results[numberResults].tblSize = ctlOffsetNext - tblOffset;
				}
				else
				{
					results[numberResults].tblSize = romSize - tblOffset;
				}

				results[numberResults].bank = n64AudioLibrary.ReadAudioRawAllowed(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0, 0);
			}
			else if (gameConfig.gameType.CompareNoCase("KonamiCtl") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioKonami(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, gameConfig.soundBanks[x].extra, gameConfig.soundBanks[x].extra2);
			}
			else if (gameConfig.gameType.CompareNoCase("Konami8Ctl") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioKonami8(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, gameConfig.soundBanks[x].extra, gameConfig.soundBanks[x].extra2);
			}
			else if (gameConfig.gameType.CompareNoCase("64dd") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudio64dd(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("vox") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioVox(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("Wav") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioWav(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("MORT") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMORT(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("Kobe2") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioKobe2(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("BanjoTooie") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioBanjoTooie(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig.soundBanks[x].numberInstruments);
			}
			else if (gameConfig.gameType.CompareNoCase("Conker") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioConker(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, gameConfig.soundBanks[x].numberInstruments, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("LzMusyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioLzMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.CompareNoCase("ZLib78DAMusyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx78DA(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.CompareNoCase("ZLibMusyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, GAUNTLETLEGENDS);
			}
			else if (gameConfig.gameType.CompareNoCase("MusyxSmallZlib") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyxSmall(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.CompareNoCase("MusyxREZLib") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioZLibMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask, RESIDENTEVIL2);
			}
			else if (gameConfig.gameType.CompareNoCase("Musyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.CompareNoCase("Musyx") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMusyx(&ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], gameConfig.soundBanks[x].numberInstruments, gameConfig.soundBanks[x].mask);
			}
			else if (gameConfig.gameType.CompareNoCase("BlastCorps") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioBlastCorps(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset]);
			}
			else if (gameConfig.gameType.CompareNoCase("MarioParty2E") == 0)
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudioMarioParty2E(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0);
			}
			else
			{
				results[numberResults].bank = n64AudioLibrary.ReadAudio(ROM, &ROM[0], results[numberResults].ctlSize, results[numberResults].ctlOffset, &ROM[results[numberResults].tblOffset], 0, 0xFFFFFFFF, gameConfig.soundBanks[x].numberInstruments);
			}

			if (results[numberResults].bank != NULL)
			{
				results[numberResults].halfSamplingRate = gameConfig.soundBanks[x].halfSamplingRate;
				results[numberResults].overrideSamplingRate = gameConfig.soundBanks[x].overrideSamplingRate;
				results[numberResults].samplingRate = gameConfig.soundBanks[x].samplingRate;
				numberResults++;
			}
			else
				results.resize(numberResults);
		}

		for (int x = 0; x < gameConfig.t1Config.size(); x++)
		{
			if ((gameConfig.t1Config[x].t1Start != 0) && (gameConfig.t1Config[x].t1End != 0))
			{
				T1Bank* t1Bank = n64AudioLibrary.ReadT1Bank(ROM, gameConfig.t1Config[x].t1Start, gameConfig.t1Config[x].t1End);
				if (t1Bank != NULL)
				{
					t1Result resultT1;
					resultT1.bank = t1Bank;
					resultT1.t1Start = gameConfig.t1Config[x].t1Start;
					resultT1.t1End = gameConfig.t1Config[x].t1End;
					resultT1.bankStart = gameConfig.t1Config[x].bankReferenceStart;
					resultT1.bankEnd = gameConfig.t1Config[x].bankReferenceEnd;
					t1results.push_back(resultT1);
				}
			}
		}
	}
}