// BY ZOINKITY

#include "StdAfx.h"
#include "VPK0Decoder.h"
#include <iostream>

using namespace std;

CVPK0Decoder::CVPK0Decoder(void)
{
}

CVPK0Decoder::~CVPK0Decoder(void)
{
}

int  CVPK0Decoder::_flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, int compressedSize)
{
	if (flagsLeft == 0)
	{
		if (inputPosition >= compressedSize)
			return 0;
		flagsLeft = 8;
		flags = inputBuffer[inputPosition++];
	}

    int value = ((flags&0x80)>0);
    flags<<=1;
	flagsLeft--;
	return value;
}

unsigned long CVPK0Decoder::_nbits(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, int num, int compressedSize)
{
    //"""Returns a value equal to the next num bits in stream.
    //itr should point to the self._flags() method above."""
    unsigned long v=0;
    for (int i = 0; i < num; i++)
	{
        v <<=1;
		bool flagRet = _flags(inputBuffer, inputPosition, flags, flagsLeft, compressedSize);;
        v|=flagRet;
	}
    return v;
}

int CVPK0Decoder::header(unsigned char* data, int& mode, unsigned long& dec_s)
{
    //"""Returns tuple: mode and compressed size""" 
    unsigned long name = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | (data[3]);
    mode = data[3] - 48; 
    dec_s= (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | (data[7]);
    if ((name & 0xFFFFFF00) != 0x76706B00)
        return -1;
    else 
        return mode;
}

void CVPK0Decoder::_tblbuild(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, Triplet*& tbl, int& tripletCount, int compressedSize)
{
    //"""Builds a table of ints and references from a bitsequence. 
    //itr should be a pointer to _flags(itr) or something similiar.""" 
	tripletCount = 0;
    tbl = new Triplet[0x100000];
    unsigned char* buf = new unsigned char[0x100000];
	int bufSize = 0;
    //# Current index and final index, respectively. 
    int idx = 0;
    //## convenience, instead of looking it up all the time 
    int fin = 0;


    while (true)
	{
		if (_flags(inputBuffer, inputPosition, flags, flagsLeft, compressedSize))
		{
            //# If idx == 0, the table is finished 
            if (idx<2)
                break;
            //## reference 
            tbl[tripletCount++] = Triplet(buf[idx-2], buf[idx-1], 0);
            buf[idx-2] = fin ;
            fin+=1;
            idx-=1;
		}
        else
		{
            //## integer entry 
            unsigned char v = _nbits(inputBuffer, inputPosition, flags, flagsLeft, 8, compressedSize);
            tbl[tripletCount++] = Triplet(0,0,v);
            if (bufSize<=idx)
                buf[bufSize++] = fin;
            else 
                buf[idx] = fin;
            fin+=1;
            idx+=1;
		}
	}

	delete [] buf;
}

int CVPK0Decoder::_tblsel(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, Triplet*& tbl, int& tripletCount, int compressedSize, int idx)
{
        //"""Uses bitflags to iterate the table until non-reference entry found. 
        //Returns an int with the width given by the table entry. 
        //itr should be a pointer to _flags(itr) or something similiar.""" 
        //# idx is set to final entry by default; override only if you're doing something special. 
        if (idx<0)
            idx = (tripletCount-1);
        if (idx<0)
            return 0; 

        //# Iterates from end fo the list to the beginning; final entry always a reference. 
		while (!tbl[idx].two)
		{
            if (_flags(inputBuffer, inputPosition, flags, flagsLeft, compressedSize))
				idx = tbl[idx].one;
            else
				idx = tbl[idx].zero;
		}
		return _nbits(inputBuffer, inputPosition, flags, flagsLeft, tbl[idx].two, compressedSize);
}

int CVPK0Decoder::dec(unsigned char* inputBuffer, int compressedSize, int dec_s, unsigned char* output)
{

	int inputPosition = 0;
	int outputPosition = 0;

	unsigned char flags = 0;
	int flagsLeft = 0;

    //"""Decompresses vpk0 data to output. 
    //If header not present, reads it from the file. 
    //In the case of an error or incompatible format, 
    //    returns an empty bytearray.""" 
    //# initialize the data stream 
	unsigned long f = 0;

    //# Retrieve sample length 
    int sl = _nbits(inputBuffer, inputPosition, flags, flagsLeft, 8, compressedSize);

	Triplet* tbl1 = NULL;
	Triplet* tbl2 = NULL;
	int tbl1Size;
	int tbl2Size;

    //# Build table 1 
	_tblbuild(inputBuffer, inputPosition, flags, flagsLeft, tbl1, tbl1Size, compressedSize);

    //# Build table 2 
    _tblbuild(inputBuffer, inputPosition, flags, flagsLeft, tbl2, tbl2Size, compressedSize);

    while (outputPosition < dec_s)
	{
        if (_flags(inputBuffer, inputPosition, flags, flagsLeft, compressedSize))
		{
			int p = 0;
            //# Copy bytes from output 
            if (sl)
			{
                //## two-sample backtrack lengths 
                int l = 0;
                int u = _tblsel(inputBuffer, inputPosition, flags, flagsLeft, tbl1, tbl1Size, compressedSize);
                if (u<3)
				{
                    l = u+1;
                    u = _tblsel(inputBuffer, inputPosition, flags, flagsLeft, tbl1, tbl1Size, compressedSize);
				}
                p = outputPosition - (u<<2) - l + 8;
			}
            else
			{
                //## one-sample backtrack lengths 
                p = outputPosition - _tblsel(inputBuffer, inputPosition, flags, flagsLeft, tbl1, tbl1Size, compressedSize);
			}
            //# Grab #bytes to copy 
            int n = _tblsel(inputBuffer, inputPosition, flags, flagsLeft, tbl2, tbl2Size, compressedSize);
            //# Append n bytes from p to output. 
            //# Do it byte-by-byte to allow replication. 
            for (int i = p; i < p+n; i++)
                output[outputPosition++] = output[i];
		}
        else
		{
            //# Push next byte to output 
            output[outputPosition++] = _nbits(inputBuffer, inputPosition, flags, flagsLeft, 8, compressedSize);
		}
	}

	delete [] tbl1;
	delete [] tbl2;
    return outputPosition;
}

BOOL CVPK0Decoder::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
{
	STARTUPINFO si;
	PROCESS_INFORMATION processInfo;
	ZeroMemory(&si, sizeof(si));
	si.cb           = sizeof(si);
	si.dwFlags      = STARTF_USESHOWWINDOW;
	si.wShowWindow  = SW_HIDE;
	ZeroMemory(&processInfo, sizeof(processInfo));
	/*return */CreateProcess(0, pCmdLine, 0, 0, FALSE, 0, 0, currentDirectory, &si, &processInfo);
	WaitForSingleObject(processInfo.hProcess, 60000);
	DWORD exitCode;
	if (GetExitCodeProcess(processInfo.hProcess, &exitCode))
	{
		if (exitCode == STILL_ACTIVE)
		{
			cout << "For some reason VPK0 Failed" << endl;
			TerminateProcess(processInfo.hProcess, exitCode);
			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);
			return false;
		}
	}
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
	return true;
};

BOOL CVPK0Decoder::IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}

bool CVPK0Decoder::CompressVPK0File(CString mainFolder, CString inputFile, CString outputFile)
{
	return CompressVPK0File(mainFolder, inputFile, outputFile, "2");
}

bool CVPK0Decoder::CompressVPK0File(CString mainFolder, CString inputFile, CString outputFile, CString level)
{
	CString gzipFileName = (mainFolder + "nvpktool.exe");
	char tempFileExistName[1000];
	strcpy(tempFileExistName, (mainFolder + "nvpktool.exe"));
	if (IsFileExist(tempFileExistName) == false)
	{
		cout << "nvpktool.exe not found!" << endl;
		return false;
	}

	strcpy(tempFileExistName, inputFile);
	if (IsFileExist(tempFileExistName))
	{
		FILE* tempInputFile = fopen(inputFile, "rb");
		fseek(tempInputFile, 0, SEEK_END);
		unsigned long size = ftell(tempInputFile);

		unsigned char* tempBuffer;
		tempBuffer = new unsigned char[size];
		
		fseek(tempInputFile, 0, SEEK_SET);
		fread(tempBuffer, 1, size, tempInputFile);

		FILE* tempOutputFile = fopen((mainFolder+"temp\\tempghv.bin"), "wb");
		if (tempOutputFile == 0)
		{
			fclose(tempInputFile);
			delete [] tempBuffer;
			cout << "Cannot Write Temporary File" << endl;
			return false;
		}

		fwrite(tempBuffer, 1, size, tempOutputFile);	

		fclose(tempInputFile);
		fclose(tempOutputFile);

		delete [] tempBuffer;

		::SetCurrentDirectory(mainFolder);
		CString tempStr;
		tempStr.Format("nvpktool.exe -c -level %s -lzwindow 30000 -zsize 30000 -method 1 -i temp\\tempghv.bin -o temp\\TEMPGHV.BIZ", level);
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));
		CString outputGZippedName = (mainFolder+"temp\\TEMPGHV.BIZ");

		DeleteFile((mainFolder+"temp\\tempghv.bin"));

		strcpy(tempFileExistName, outputGZippedName);
		if (IsFileExist(tempFileExistName))
		{
			FILE* inputFileName = fopen(outputGZippedName, "rb");
			int sizeNew = 0;
			fseek(inputFileName, 0, SEEK_END);
			sizeNew = ftell(inputFileName);
			fseek(inputFileName, 0, SEEK_SET);

			int sizePadded = sizeNew;
			if ((sizePadded % 4) != 0)
			{
				sizePadded = sizePadded + (4 - (sizePadded % 4));
			}

			unsigned char* tempBufferNew;
			tempBufferNew = new unsigned char[sizePadded];
			for (int x = 0; x < sizePadded; x++)
				tempBufferNew[x] = 0x00;

			fread(tempBufferNew, 1, sizeNew, inputFileName);

			fclose(inputFileName);
			DeleteFile((mainFolder+"temp\\TEMPGHV.BIZ"));
			FILE* outputFileName = fopen(outputFile, "wb");
			if (outputFileName == NULL)
			{
				delete [] tempBufferNew;
				cout << "Error opening temp output file" << endl;
				return false;
			}

			fwrite(&tempBufferNew[0], 1, (sizePadded), outputFileName);

			delete [] tempBufferNew;
			fflush(outputFileName);
			fclose(outputFileName);
			return true;
		}
		else
		{
			cout << "Error Compressing - nvpktool didn't spit out a file" << endl;
			return false;
		}
	}
	cout << "Error - could not read/find input file" << endl; // this should never happen - something prevented the input file from being read
	return false;
}