// SSBFileInjector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SSBFileInjector.h"

#include "SSBMain21990Setup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

bool IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return false;
	else
		return true;
}

CString GetEditorMainFolderPath()
{
	char tempFolderModule[8000];
	DWORD length = GetModuleFileName(NULL, tempFolderModule, 8000);
	PathRemoveFileSpec(tempFolderModule);

	CString workingModule;
	workingModule.Format("%s", tempFolderModule);

	workingModule += "\\";

	char tempFolder[8000];
	::GetCurrentDirectory(8000, tempFolder);

	CString tempMainFolder;
	tempMainFolder.Format("%s", tempFolder);

	tempMainFolder += "\\";

	if (tempMainFolder == workingModule)
	{
		return tempMainFolder;
	}
	else
	{
		CString tempImagesTextPath;
		tempImagesTextPath = tempMainFolder + "SSBFileInjector.cpp";
		if (IsFileExist(tempImagesTextPath.GetBuffer(0)))
		{
			tempImagesTextPath.ReleaseBuffer();
			return tempMainFolder;
		}
		else
		{
			tempImagesTextPath.ReleaseBuffer();
			return workingModule;
		}
	}
}

bool hiddenExec(PTSTR pCmdLine, CString currentDirectory, int& errorCode, int waitTime)
{
	errorCode = 0;
	::SetCurrentDirectory(currentDirectory);
	STARTUPINFO si;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&processInfo, sizeof(processInfo));


	if (currentDirectory.ReverseFind('\\') == (currentDirectory.GetLength() - 1))
	{
		currentDirectory = currentDirectory.Mid(0, (currentDirectory.GetLength() - 1));
	}

	/*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, CREATE_NO_WINDOW, 0, currentDirectory, &si, &processInfo);
	if (waitTime == -1) // if waitTime is -1, do not wait for program to close (used for N64MidiPlayer)
		return true;

	WaitForSingleObject(processInfo.hProcess, waitTime);
	DWORD exitCode;
	if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
	{
		errorCode = exitCode;
		if (exitCode == STILL_ACTIVE)
		{
			cout << "For some reason rn64crc Process Failed" << endl;
			TerminateProcess(processInfo.hProcess, exitCode);

			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
			return false;
		}
	}
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return true;
}

bool hiddenExec(PTSTR pCmdLine, CString currentDirectory, int& errorCode)
{
	return hiddenExec(pCmdLine, currentDirectory, errorCode, 60000);
}

bool hiddenExec(PTSTR pCmdLine, CString currentDirectory)
{
	int errorCode;
	return hiddenExec(pCmdLine, currentDirectory, errorCode);
};

std::vector<CString> StringSplit(CString s, CString Delimiters)
{
	std::vector<CString> OutputArray;
	int     Hit;
	int     EarliestHit;
	int     DelimiterIndex;
	CString sExtract;
	BOOL    MoreTokens = TRUE;
	BOOL    GotToken = FALSE;
	TCHAR   CurrDelimiter;

	while (MoreTokens)
	{
		GotToken = FALSE;
		EarliestHit = s.GetLength();

		// Trawl the string looking for the leftmost (earliest) hit in
		// the list of valid separators.
		for (DelimiterIndex = 0;
			DelimiterIndex < Delimiters.GetLength();
			DelimiterIndex++)
		{
			CurrDelimiter = Delimiters[DelimiterIndex];
			Hit = s.Find(CurrDelimiter);
			if (Hit != -1)
			{
				if (Hit < EarliestHit)
				{
					EarliestHit = Hit;
				}
				GotToken = TRUE;
			}
		}

		if (GotToken)
		{
			sExtract = s.Left(EarliestHit);
			s = s.Right(s.GetLength() - EarliestHit - 1);
			OutputArray.push_back(sExtract.Trim());
		}
		else
		{
			// Trailing token.
			MoreTokens = FALSE;
			OutputArray.push_back(s.Trim());
		}
	}

	return OutputArray;
}


int HexToInt(char inChar)
{
	switch (inChar)
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

unsigned long StringHexToLong(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 8)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (8 - tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0; x < 8; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt << ((7 - x) * 4);
	}
	return tempLong;
}

unsigned short StringToUnsignedShort(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 4)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (4 - tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0; x < 4; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt << ((3 - x) * 4);
	}
	return tempLong;
}

unsigned char StringToUnsignedChar(CString inString)
{
	inString.Trim();
	int tempA = inString.GetLength();
	if (inString.GetLength() < 2)
	{
		CString tempStr = inString;
		inString = "";
		for (int x = 0; x < (2 - tempStr.GetLength()); x++)
		{
			inString = inString + "0";
		}
		inString = inString + tempStr;
	}
	char* b;
	b = inString.GetBuffer(0);
	unsigned long tempLong = 0;
	for (int x = 0; x < 2; x++)
	{
		char tempChar = b[x];
		int hexInt = HexToInt(tempChar);
		tempLong = tempLong | hexInt << ((1 - x) * 4);
	}
	return (unsigned char)tempLong;
}

std::vector<unsigned short> ReadReqFiles(CString reqFilePath)
{
	std::vector<unsigned short> reqFileData;

	char currentLine[10000];
	unsigned short tempShort;
	
	FILE* inFile = fopen(reqFilePath, "r");
	if (inFile == NULL)
	{
		cout << "Can't open req file " << reqFilePath << endl;
		return reqFileData;
	}

	

	fgets(currentLine, TEXTBUFFERREADSIZE, inFile);
	while ((!feof(inFile)) && ((currentLine[0] != 'E') || (currentLine[1] != 'N') || (currentLine[2] != 'D')) && ((currentLine[4] == ' ') || (currentLine[4] == '\r') || (currentLine[4] == '\n'))) // while not end of file, END maker not found and space/new line found
	{
		if ((!isxdigit(currentLine[0])) || (!isxdigit(currentLine[1])) || (!isxdigit(currentLine[2])) || (!isxdigit(currentLine[3]))) // if slot isn't made up of valid hex characters, show error and stop
		{
			cout << "Error: Req list file contain a non-hex character slot ID, aborting..." << endl;
			return reqFileData;
		}

		tempShort = ((((signed short)HexToInt(currentLine[0])) << 12) | (((signed short)HexToInt(currentLine[1])) << 8) | (HexToInt(currentLine[2]) << 4) | (HexToInt(currentLine[3]) << 0));
		reqFileData.push_back(tempShort);
		fgets(currentLine, TEXTBUFFERREADSIZE, inFile);
	}
	fclose(inFile);

	return reqFileData;
}

BOOL IsDots(const TCHAR* str) {
	if (_tcscmp(str, ".") && _tcscmp(str, "..")) return FALSE;
	return TRUE;
}

BOOL DeleteDirectory(const TCHAR* sPath) {
	HANDLE hFind;  // file handle
	WIN32_FIND_DATA FindFileData;

	TCHAR DirPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	_tcscpy(DirPath, sPath);
	_tcscat(DirPath, "\\*");    // searching all files
	_tcscpy(FileName, sPath);
	_tcscat(FileName, "\\");

	hFind = FindFirstFile(DirPath, &FindFileData); // find the first file
	if (hFind == INVALID_HANDLE_VALUE) return FALSE;
	_tcscpy(DirPath, FileName);

	bool bSearch = true;
	while (bSearch) { // until we finds an entry
		if (FindNextFile(hFind, &FindFileData)) {
			if (IsDots(FindFileData.cFileName)) continue;
			_tcscat(FileName, FindFileData.cFileName);
			if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

				// we have found a directory, recurse
				if (!DeleteDirectory(FileName)) {
					FindClose(hFind);
					return FALSE; // directory couldn't be deleted
				}
				RemoveDirectory(FileName); // remove the empty directory
				_tcscpy(FileName, DirPath);
			}
			else {
				//if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
//					_chmod(FileName, _S_IWRITE); // change read-only file mode
				if (!DeleteFile(FileName)) {  // delete the file
					FindClose(hFind);
					return FALSE;
				}
				_tcscpy(FileName, DirPath);
			}
		}
		else {
			if (GetLastError() == ERROR_NO_MORE_FILES) // no more files there
				bSearch = false;
			else {
				// some error occured, close the handle and return FALSE
				FindClose(hFind);
				return FALSE;
			}

		}

	}
	FindClose(hFind);  // closing file handle

	return RemoveDirectory(sPath); // remove the empty directory

}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			if (argc == 1)
			{
				cout << "Usage SSBFileInjector.exe \"csv=InjectSample.csv\" \"inputROM=ssb.rom\" \"outputROM=test.rom\"" << endl;
				nRetCode = 2;
				return nRetCode;
			}

			CString mainFolder = GetEditorMainFolderPath();
			::CreateDirectory(mainFolder + "Temp", NULL);

			CString csv;
			CString inputROM;
			CString outputROM;

			for (int x = 0; x < argc; x++)
			{
				CString tempStr = CString(argv[x]);
				if (tempStr.MakeUpper().Find(CString("csv=").MakeUpper()) == 0)
				{
					csv = CString(argv[x]).Mid(CString("csv=").GetLength()).Trim();
				}
				else if (tempStr.MakeUpper().Find(CString("inputROM=").MakeUpper()) == 0)
				{
					inputROM = CString(argv[x]).Mid(CString("inputROM=").GetLength()).Trim();
				}
				else if (tempStr.MakeUpper().Find(CString("outputROM=").MakeUpper()) == 0)
				{
					outputROM = CString(argv[x]).Mid(CString("outputROM=").GetLength()).Trim();
				}
			}

			if (csv == "")
			{
				cout << "Missing csv=" << endl;
				nRetCode = 2;
				return nRetCode;
			}

			if (inputROM == "")
			{
				cout << "Missing inputROM=" << endl;
				nRetCode = 2;
				return nRetCode;
			}

			if (outputROM == "")
			{
				cout << "Missing outputROM=" << endl;
				nRetCode = 2;
				return nRetCode;
			}

			FILE* inFile = fopen(csv, "r");
			if (inFile == NULL)
			{
				cout << "Can't read input file" << endl;
				nRetCode = 3;
				return nRetCode;
			}

			CSSBMain21990Setup mainSetup;
			mainSetup.Initialize(SSB);
			mainSetup.mainFolder = mainFolder;
			CString ROMpathdir = inputROM.Mid(0, (inputROM.ReverseFind('\\') + 1));
			CString ROMname = inputROM.Mid((inputROM.ReverseFind('\\') + 1), (inputROM.GetLength() - (inputROM.ReverseFind('\\') + 1)));

			if (!mainSetup.Initialize(ROMpathdir, ROMname))
			{
				cout << "Error starting input ROM" << endl;
				nRetCode = 3;
				return nRetCode;
			}

			while (!feof(inFile))
			{
				char currentLine[10000];
				fgets(currentLine, 10000, inFile);
				CString currentLineStr(currentLine);

				if (
					(CString(currentLineStr).MakeUpper().Find("ADD") == 0)
					||
					(CString(currentLineStr).MakeUpper().Find("MODIFY") == 0)
					)
				{
					std::vector<CString> lineSplit = StringSplit(currentLineStr, ",");
					for (int x = 0; x < lineSplit.size(); x++)
					{
						// Remove if Excel put a ' to note string
						if (lineSplit[x].Find("'") == 0)
							lineSplit[x] = lineSplit[x].Mid(1);
					}
					if (lineSplit.size() < 3)
					{
						cout << "Not enough comma entries line " + currentLineStr << endl;
						nRetCode = 5;
						return nRetCode;
					}

					int fileNumber = -1;
					if (lineSplit[1] != "")
						fileNumber = StringToUnsignedShort(lineSplit[1]);

					CString newFilePath = lineSplit[2];

					bool compressed = true;
					int internalFileTableBytes = -1;
					int internalFileResourceOffsetBytes = -1;
					CString reqFilePath = "";
					int compressionLevel = 2;

					if (lineSplit.size() > 3)
					{
						if ((CString(lineSplit[3]) == "0") || ((CString(lineSplit[3]).MakeLower() == "false")))
							compressed = false;
					}

					if (lineSplit.size() > 4)
					{
						if (lineSplit[4] != "")
							internalFileTableBytes = StringToUnsignedShort(lineSplit[4]);
					}
					
					if (lineSplit.size() > 5)
					{
						if (lineSplit[5] != "")
							internalFileResourceOffsetBytes = StringToUnsignedShort(lineSplit[5]);
					}
					if (lineSplit.size() > 6)
					{
						reqFilePath = lineSplit[6];
					}
					if (lineSplit.size() > 7)
					{
						if ((lineSplit[7] != "") && ((lineSplit[7] == "0") || (lineSplit[7] == "1") || (lineSplit[7] == "2")))
							compressionLevel = HexToInt(lineSplit[7][0]);
					}

					std::vector<unsigned short> reqFiles;
					if (reqFilePath != "")
						reqFiles = ReadReqFiles(reqFilePath);

					if (CString(currentLineStr).MakeUpper().Find("ADD") == 0)
					{
						int copySlot = mainSetup.ssbFileTable.size() - 1;
						SSBFileTableEntry newEntry;
						newEntry.compressedSizeWords = mainSetup.ssbFileTable[copySlot].compressedSizeWords;
						newEntry.dataCompressed = new unsigned char[newEntry.compressedSizeWords * 4];
						memcpy(newEntry.dataCompressed, mainSetup.ssbFileTable[copySlot].dataCompressed, (mainSetup.ssbFileTable[copySlot].compressedSizeWords * 4));
						newEntry.dataOffset = 0;
						newEntry.decompressedSizeWords = mainSetup.ssbFileTable[copySlot].decompressedSizeWords;
						newEntry.index = mainSetup.ssbFileTable.size();
						newEntry.internalResourceDataWords = mainSetup.ssbFileTable[copySlot].internalResourceDataWords;
						newEntry.internalResourceTableOffsetWords = mainSetup.ssbFileTable[copySlot].internalResourceTableOffsetWords;
						newEntry.isCompressed = mainSetup.ssbFileTable[copySlot].isCompressed;
						newEntry.reqFile = mainSetup.ssbFileTable[copySlot].reqFile;
						mainSetup.ssbFileTable.push_back(newEntry);

						fileNumber = mainSetup.ssbFileTable.size() - 1;
						cout << "Added File " << fileNumber << endl;
					}

					if (fileNumber >= mainSetup.ssbFileTable.size())
					{
						cout << "File number too large failure " + currentLineStr << endl;
						nRetCode = 6;
						return nRetCode;
					}

					mainSetup.ssbFileTable[fileNumber].isCompressed = compressed;
					if (!mainSetup.InjectFileIntoROM(newFilePath, fileNumber, internalFileTableBytes, compressionLevel, internalFileResourceOffsetBytes))
					{
						cout << "Inject failure " + currentLineStr;
						nRetCode = 6;
						return nRetCode;
					}
					mainSetup.ssbFileTable[fileNumber].reqFile = reqFiles;

					cout << "Modified File " << fileNumber << endl;
				}
				if (CString(currentLineStr).MakeUpper().Find("END") == 0)
				{
					break;
				}
			}
			fclose(inFile);

			mainSetup.Write21990File();

			FILE* outFile = fopen(outputROM, "wb");
			if (outFile == NULL)
			{
				cout << "Can't write output file" << endl;
				nRetCode = 4;
				return nRetCode;
			}
			fwrite(mainSetup.SSBROM, 1, mainSetup.romSize, outFile);
			fclose(outFile);

			CString tempStr;
			tempStr.Format("\"" + mainFolder + "rn64crc.exe\" -u \"%s\"", outputROM);
			hiddenExec(_T(tempStr.GetBuffer()), mainFolder);

			DeleteDirectory(mainFolder + "Temp");
		}

		cout << "Done" << endl;
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
