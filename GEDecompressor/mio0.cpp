/* MIO0 decompression */
#pragma once
#include "stdafx.h"
#include "mio0.h"

#define U32(x)  ((unsigned long)( (((unsigned char*)(x))[0]<<24)|(((unsigned char*)(x))[1]<<16)|(((unsigned char*)(x))[2]<<8)|((unsigned char*)(x))[3] ))
#define U16(x)  ((unsigned short)( ((*((unsigned char*)(x)))<<8)|((unsigned char*)(x))[1] ))
#define U8(x)	((unsigned char)((unsigned char*)(x))[0])
#define S32(x)  ((signed long)(U32(x)))
#define S16(x)  ((signed short)(U16(x)))
#define F32(x)  ( ( (____u32_f32_union____.u = U32(x)) & 0) + ____u32_f32_union____.f)


struct mio0_t
{
    unsigned long magic;
    unsigned long size;
    unsigned long compressed_offset;
    unsigned long raw_offset;
    unsigned char data[];
};

typedef struct mio0_t Mio0;

int MIO0::mio0_get_size( unsigned char * src)
{
    Mio0 *file = (Mio0*)src;
    return U32( &(file->size) );
}

unsigned long MIO0::mio0_decode ( unsigned char *src, unsigned char * destination, int& fileSizeCompressed )
{
    Mio0 * file = (Mio0*)src;
    
    unsigned MapByte = 0x80;
    unsigned CurMapByte;
    unsigned Length;
    unsigned SData, Dist;
    unsigned NumBytesOutput = 0;
    unsigned MapLoc = 0;
    unsigned CompLoc = U32( &(file->compressed_offset) );
    unsigned RawLoc = U32( &(file->raw_offset) );
    unsigned OutLoc = 0;
    unsigned OutputSize = U32( &(file->size) );
    
    unsigned char * Input = src, * Output;
    
    Output = destination;
    
    MapLoc = 0x10;
    CurMapByte = Input[MapLoc];
    
    while( NumBytesOutput < OutputSize )
    {
        /* Raw data, not compressed */
        if( CurMapByte & MapByte)
        {
            Output[OutLoc] = Input[RawLoc];
            OutLoc++;
            RawLoc++;
            NumBytesOutput++;
        }
        
        /* Compressed */
        else
        {
            SData = U16( Input + CompLoc );
            Length = (SData >> 12) + 3;
            Dist = (SData & 0xFFF) + 1;
            
            /* Sanity check: can't copy from before first byte */
            if( ((int)OutLoc - (int)Dist) < 0 )
                goto failed;
            
            int i;
            /* copy from output */
            for( i = 0; i < Length; i++ )
            {
                Output[OutLoc] = Output[OutLoc - Dist];
                OutLoc++;
                NumBytesOutput++;
                
                if( NumBytesOutput >= OutputSize )
                    break;
            }
            
            CompLoc += 2;
        }
        
        MapByte >>= 1; /* next control bit */
        
        /* If we've exhausted the current control byte, fetch another one */
        if( !MapByte )
        {
            MapByte = 0x80;
            MapLoc++;
            CurMapByte = Input[MapLoc];
            
            /* Sanity check: map pointer should never reach tihs */
            unsigned Check = CompLoc;
            
            if( RawLoc < CompLoc )
                Check = RawLoc;
            
            if( MapLoc > Check )
                goto failed;
        }
    }
  failed:
    
    return NumBytesOutput;
}


BOOL MIO0::hiddenExec (PTSTR pCmdLine, CString currentDirectory)
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
			MessageBox(NULL, "For some reason GZip Failed", "Error", NULL);
			TerminateProcess(processInfo.hProcess, exitCode);
			return false;
		}
   }   
   return true;
};

BOOL MIO0::IsFileExist(LPSTR lpszFilename)
{
	DWORD dwAttr = GetFileAttributes(lpszFilename);
	if (dwAttr == 0xffffffff)
		return FALSE;
	else 
		return TRUE;
}

bool MIO0::CompressMIO0File(CString mainFolder, CString inputFile, CString outputFile)
{
	CString gzipFileName = (mainFolder + "mio0.exe");
	char tempFileExistName[1000];
	strcpy(tempFileExistName, (mainFolder + "mio0.exe"));
	if (IsFileExist(tempFileExistName) == false)
	{
		MessageBox(NULL, "mio0.exe not found!", "Error", NULL);
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

		FILE* tempOutputFile = fopen((mainFolder+"tempgh9.bin"), "wb");
		if (tempOutputFile == 0)
		{
			fclose(tempInputFile);
			delete [] tempBuffer;
			MessageBox(NULL, "Cannot Write Temporary File", "Error", NULL);
			return false;
		}

		fwrite(tempBuffer, 1, size, tempOutputFile);	

		fclose(tempInputFile);
		fclose(tempOutputFile);

		delete [] tempBuffer;

		::SetCurrentDirectory(mainFolder);
		CString tempStr = ("mio0.exe -c tempgh9.bin TEMPGH9.BIZ");
		hiddenExec(_T(tempStr.GetBuffer()), (mainFolder));
		CString outputGZippedName = (mainFolder+"TEMPGH9.BIZ");

		DeleteFile((mainFolder+"tempgh9.bin"));

		strcpy(tempFileExistName, outputGZippedName);
		if (IsFileExist(tempFileExistName))
		{
			FILE* inputFileName = fopen(outputGZippedName, "rb");
			int sizeNew = 0;
			fseek(inputFileName, 0, SEEK_END);
			sizeNew = ftell(inputFileName);
			fseek(inputFileName, 0, SEEK_SET);

			unsigned char* tempBufferNew;
			tempBufferNew = new unsigned char[sizeNew];
			fread(tempBufferNew, 1, sizeNew, inputFileName);

			fclose(inputFileName);
			DeleteFile((mainFolder+"TEMPGH9.BIZ"));
			FILE* outputFileName = fopen(outputFile, "wb");
			if (outputFileName == NULL)
			{
				delete [] tempBufferNew;
				MessageBox(NULL, "Error opening temp output file", "Error", NULL);
				return false;
			}

			fwrite(&tempBufferNew[0], 1, (sizeNew), outputFileName);	

			delete [] tempBufferNew;
			fflush(outputFileName);
			fclose(outputFileName);
			return true;
		}
		else
		{
			MessageBox(NULL, "Error Compressing - rncencode didn't spit out a file", "Error", NULL);
			return false;
		}
	}
}