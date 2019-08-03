#include "StdAfx.h"
#include "AidynToDCMConvertor.h"

CAidynToDCMConvertor::CAidynToDCMConvertor(void)
{
}

CAidynToDCMConvertor::~CAidynToDCMConvertor(void)
{
}

unsigned short CAidynToDCMConvertor::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CAidynToDCMConvertor::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

int CAidynToDCMConvertor::GetSizeFile(CString filename)
{
	FILE* inFile = fopen(filename, "rb");
	
	if (inFile == 0)
		return 0;

	fseek(inFile, 0, SEEK_END);
	int fileSize = ftell(inFile);
	fclose(inFile);
	return fileSize;
}

void CAidynToDCMConvertor::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CAidynToDCMConvertor::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

unsigned short CAidynToDCMConvertor::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned long CAidynToDCMConvertor::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}



bool CAidynToDCMConvertor::convert(unsigned char* data, unsigned char* ROM, int fileTable, int fileTableDataStart, std::map<int, fileData>& smp, CString kind, unsigned char* output, int& outputPosition)
{
     //"""Converts Aidyn DCMs into single-file standalone DCM-lite files (DCM1).
     //[data] can be either a bytes-like object or a filename for the dch.
     //If provided, [smp] should be a mapping (dict or similar) of ID numbers (ints!)
     //    and either the binaries themselves (a bytes-like object) or filenames for them.
     //If not provided, will look in the current working directory for a file matching that ID
     //    in the form 'XXXX'--the value in hex with no extension.
     //Header file format will be determined at runtime unless [kind] is given, which can be:
     //    'big'     E3 and retail Aidyn
     //    'little'  pre-E3 protos
     //    'lite'    DCM-lite, which won't be converted for obvious reasons
     //    'org'     The original DCM format which may have never been released
     //Returns a bytes object for the built DCM-lite file."""

     //inf = struct.Struct(">3Ll")
     //DCMEntry = struct.Struct("<3LHH")

     //# smp should be a mapping of IDs and binaries or something.

     //# This just happens to work in all retail examples.
     //# It could, just as well, lead to false-positives.
     if (kind == "")
	 {
         if (CharArrayToLong(&data[0]) == 0x44434D30) //DCM0
             kind = "org";
         else if (CharArrayToLong(&data[0]) == 0x44434D31) //DCM1
             kind = "lite";
         else if (CharArrayToLong(&data[24]) == 0x20)
             kind = "big";
		 else if (CharArrayToLong(&data[16]) == 0x18)
             kind = "little";
	 }
     if (kind == "")
	 {
        MessageBox(NULL, "Not a known DCM format.", "", NULL);
		return false;
	 }
     else if (kind == "org")
	 {
		MessageBox(NULL, "DCM0 not supported.", "", NULL);
		return false;
	 }
     else if (kind == "lite")
	 {
		//# Uh...whatever.
		MessageBox(NULL, "Why are you trying to convert this file into its own format?", "", NULL);
		return false;
	 }

	 unsigned long head[6];
	 unsigned long v;
	 std::vector<unsigned long> tbl;
     if (kind == "big")
	 {
		 for (int x = 0; x < 6; x++)
			 head[x] = CharArrayToLong(&data[8 + (x * 4)]);
         v = CharArrayToLong(&data[0]);
		 for (int x = 0; x < head[1]; x++)
			 tbl.push_back(CharArrayToLong(&data[v + (x * 4)]));
	 }
     else
	 {
         for (int x = 0; x < 6; x++)
			 head[x] = CharArrayToLong(&data[(x * 4)]);
         tbl.clear();
	 }
     unsigned long channels = head[0];
     //# Never used(?), but guessing it's the restart byte offset in music stream.
     unsigned long offset = head[3];
	 std::vector<inf> si;
	 for (int i = head[4]; i < head[5]; i += 16)
	 {
		inf tempInf;
		tempInf.ul[0] = CharArrayToLong(&data[i]);
		tempInf.ul[1] = CharArrayToLong(&data[i + 4]);
		tempInf.ul[2] = CharArrayToLong(&data[i + 8]);
		tempInf.i[0] = (int)CharArrayToLong(&data[i + 0xC]);
		si.push_back(tempInf);
	 }

     unsigned long p = head[2];
     //# Should probably check if this is compressed...
	 int streamLength = p;
     unsigned char* stream = &data[head[5]];
     //# Okay, problem here is that flags are embedded in the samples.
     //# Aegh...
     //# That requires loading them if you're going to do this "right".
     //# Double entry for duplicate samples is required.  Lite samples are LE, others BE.
     //# At any rate, convert the order of sampleinfo entries to DCM-lite.
	 std::vector<DCMEntry> e;
	 std::vector<unsigned char> s;
     for (int i = 0; i < si.size(); i++)
	 {
		 unsigned long ID = si[i].ul[0];
		 unsigned long start = si[i].ul[1];
		 unsigned long end = si[i].ul[2];
         int length = 0;
         if (tbl.size() > 0)
		 {
             ID = tbl[i];
		 }
         int flags = 0;
         if (end != 0x00000000)
		 {
			 if (si[i].i[0] != 0x00000000)
				 flags |= 4;
			 else
				 flags |= 8;
		 }
		 fileData smpValue;
		 if (smp.find(ID) != smp.end())
			 smpValue = smp[ID];
		 else
		 {
			int decompressedSize = -1;
			fileData tempData;
			tempData.fileSize = decompressedSize;
			tempData.data = CAidynDecoder::DecompressAidynFile(ROM, fileTable, fileTableDataStart, ID, decompressedSize);
			smp[ID] = tempData;
             
			if (smp.find(ID) == smp.end())
			{
                MessageBox(NULL, "Unable to open sample. Place it in the working directory", "Error", NULL);
				return false;
			}

			smpValue = smp[ID];
		 }

         //# Read the header, set the flags, byteswap if necessary, copy the sample.
		 inf k;
		 k.ul[0] = CharArrayToLong(&smpValue.data[0]);
		 k.ul[1] = CharArrayToLong(&smpValue.data[4]);
		 k.ul[2] = CharArrayToLong(&smpValue.data[8]);
		 k.i[0] = CharArrayToLong(&smpValue.data[0xC]);
		 flags |= k.ul[0] >> 3;
		 length = k.ul[2];

		 if (k.ul[0] & 8)
		 {
             //# 16bit, BE -> LE
			 std::vector<unsigned short> tempHalfwords;
			 for (int y = 0; y < k.ul[2]; y++)
			 {
				 tempHalfwords.push_back(CharArrayToShort(&(smpValue.data[k.i[0] + (y * 2)])));
			 }

			 for (int y = 0; y < k.ul[2]; y++)
			 {
				 // Little Endian
				 s.push_back(tempHalfwords[y] & 0xFF);
				 s.push_back((tempHalfwords[y] >> 8) & 0xFF);
			 }
		 }
         else
		 {
             //# 8bit
			 for (int y = k.i[0]; y < (k.i[0]+k.ul[1]); y++)
			 {
				s.push_back(smpValue.data[y]);
			 }
		 }
		 DCMEntry tempEntry;
		 tempEntry.ul[0] = length;
		 tempEntry.ul[1] = start;
		 tempEntry.ul[2] = end;
		 tempEntry.us[0] = flags;
		 tempEntry.us[1] = (ID & 0xFFFF);
		 e.push_back(tempEntry);
	 }

     //# Generate a header (dch).
	 outputPosition = 0;
	 WriteLongToBuffer(output, outputPosition, 0x44434D31);
	 outputPosition += 4;

	 output[outputPosition++] = channels;
	 output[outputPosition++] = e.size();
	 WriteLongToBuffer(output, outputPosition, Flip32Bit(streamLength));
	 outputPosition += 4;
	 WriteLongToBuffer(output, outputPosition, Flip32Bit(offset));
	 outputPosition += 4;

	 for (int x = 0; x < e.size(); x++)
	 {
		WriteLongToBuffer(output, outputPosition, Flip32Bit(e[x].ul[0]));
		outputPosition += 4;
		WriteLongToBuffer(output, outputPosition, Flip32Bit(e[x].ul[1]));
		outputPosition += 4;
		WriteLongToBuffer(output, outputPosition, Flip32Bit(e[x].ul[2]));
		outputPosition += 4;
		
		WriteShortToBuffer(output, outputPosition, Flip16Bit(e[x].us[0]));
		outputPosition += 2;
		WriteShortToBuffer(output, outputPosition, Flip16Bit(e[x].us[1]));
		outputPosition += 2;
	 }

     //# Hope against hope it produces a DCM that works.
	 memcpy(&output[outputPosition], stream, streamLength);
	 outputPosition += streamLength;

	 for (int x = 0; x < s.size(); x++)
		output[outputPosition++] = s[x];
     
	 return true;
}