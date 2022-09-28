// By Zoinkity
#include "StdAfx.h"
#include "CalcCRC.h"

CalcCRC::CalcCRC(void)
{
	cic_names.clear();
    cic_names["starf"] = 0x3F;
    cic_names["lylat"] = 0x3F;
    cic_names["mario"] = 0x3F;
    cic_names["diddy"] = 0x78;
    cic_names["aleck"] = 0xAC;
    cic_names["zelda"] = 0x91;
    cic_names["yoshi"] = 0x85;
    cic_names["ddipl"] = 0xDD;
    cic_names["dddev"] = 0xDD;

	PC = 0;
}

CalcCRC::~CalcCRC(void)
{
}

unsigned long rol(unsigned long long v, int n)
{
	return (v % (unsigned long long)0x100000000) >> n;
}

CString CalcCRC::cic2seed(CString cic, unsigned char& cicValue)
{
    /*"""Returns (name, seed) for <cic>.
    name: the normalized name of the chip
    seed: unencoded seed value.
        <cic> should be a str, matching the serial or short name of the chip.
    cic types:
        'aleck', '5101'
        'starf', '6101'
        'lylat', '7102'
        'mario', '6102', '7101'
        'diddy', '6103', '7103'
        'ddipl', '8303'
        'dddev'
        'zelda', '6105', '7105'
        'yoshi', '6106', '7106'
    """*/
    //# Normalize the names.  (kinda makes the dict pointless...)
    cic = cic.MakeLower();
    if ((cic == "6102") || (cic == "7101") || (cic == "mario"))
        cic = "mario";
    else if ((cic == "6101") || (cic == "starf"))
        cic = "starf";
    else if ((cic == "7102") || (cic == "lylat"))
        cic = "starf";
    else if ((cic == "6103") || (cic == "7103") || (cic == "diddy"))
        cic = "diddy";
    else if ((cic == "6105") || (cic == "7105") || (cic == "zelda"))
        cic = "zelda";
    else if ((cic == "6106") || (cic == "7106") || (cic == "yoshi"))
        cic = "yoshi";
    else if ((cic == "5101") || (cic == "aleck"))
        cic = "aleck";
    else if ((cic == "8303") || (cic == "ddipl"))
        cic = "ddipl";
    else if ((cic == "8302") || (cic == "dddev"))
        cic = "dddev";
    else if ((cic == "8301"))
        cic = "dddev";
    else
	{
		cicValue = 0x00;
        return "";
	}
	
	cicValue = cic_names[cic];
    return cic;
}

unsigned long Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned long CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

void WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

void setASMvalue(unsigned char* rom, int romSize, unsigned long value, unsigned long upper, unsigned long lower = 0)
{
    /*"""If <upper> and <lower> are given,
        sets the LUI+ADDIU or LUI+ORI pair to value.
        If upper is None only the lower half will be set.
    If only <upper> is given,
        sets value as the address of a J or JAL instruction."""*/
    if (lower == 0)
	{
        value>>=2;
        value&=0x3FFFFFF;
        unsigned char u = rom[upper] & 0xFC;
        value |= u << 24;
        WriteLongToBuffer(rom, upper, value);
	}
    else
	{
        unsigned short u = value >> 16;
        unsigned short l = value & 0xFFFF;
        if ((rom[lower] & 0x10) == 0)
            u += ((l&0x8000) > 0);
        if (upper != 0)
			WriteShortToBuffer(rom, upper+2, u);
		WriteShortToBuffer(rom, lower+2, l);
	}
}

std::vector<unsigned long> CalcCRC::calccrc(unsigned char* rom, unsigned long romSize, CString cic, bool fix, unsigned long long seed, unsigned long base, unsigned long seel)
{
	std::vector<unsigned long> returnCrc;
    /*"""Recalculates the CRC based on the CIC chip version given.
    Set fix to True to revise the crc in self.rom.

    cic types:
        'aleck', '5101'
        'starf', '6101'
        'lylat', '7102'
        'mario', '6102', '7101'
        'diddy', '6103', '7103'
        'ddipl', '8303'
        'dddev'
        'zelda', '6105', '7105'
        'yoshi', '6106', '7106'
    Setting seed overrides the normal seed value for these types.
    If you need to use one of the particular cic algorithms set the
        string version of the name in cic.  Now that the Aleck64
        CIC has been identified this will probably not be necessary
        unless some repro cic chips start using different values."""*/
    

    //# The algo slightly changes with certain cics.
	unsigned char s = 0;
	cic = cic2seed(cic, s);

    //# Pull the seed byte and generate the seed from it.
    //# Note even if seed is set you'll have to pass a cic "name" to get funky types out.
    if (seed != 0)
        s = seed;
    if ((cic == "diddy") || (cic == "yoshi") || (cic == "aleck"))
        seed = 0x6C078965 * s;
    else if (cic == "ddipl")
        seed = 0x2E90EDD * s;
    else if (cic == "dddev")
        seed = 0x260BCD5 * s;
    else
        seed = 0x5D588B65 * s;
    seed+= 1;
    seed &= 0xFFFFFFFF;
	unsigned long long r1 = seed;
	unsigned long long r2 = seed;
	unsigned long long r3 = seed;
	unsigned long long r4 = seed;
	unsigned long long r5 = seed;
	unsigned long long r6 = seed;

    //# I wish there was a less horrifying way to do this...
    if (seel == 0x0)
	{
        if (cic == "aleck")
		{
			if (PC == 0x80100400)
				seel = 0x3FE000;
			else
				seel = 0x100000;
		}
        else if ((cic == "ddipl") || (cic == "dddev"))
            seel = 0xA0000;
        else
            seel = 0x100000;
	}
    seel += base;
    unsigned long long l = min(seel, romSize);

	std::vector<unsigned long> m;
	for (int x = base; x < l; x += 4)
	{
		m.push_back(CharArrayToLong(&rom[x]));
	}

	for (int x = 0; x < (seel - l); x++)
		m.push_back(0);

	std::vector<unsigned long> n;
		
	int nIterationPosition = 0;

    //# Zelda updates the second word a different way...
    if (cic == "zelda")
	{
		// should go infinite, cycle
		for (int x = 0x750; x < 0x850; x+=4)
		{
			n.push_back(CharArrayToLong(&rom[x]));
		}
	}
    //# Read each word as an integer.
    for (int x = 0; x < m.size(); x++)
	{
		unsigned long long i = m[x];
        unsigned long long v = (r1+i) & 0xFFFFFFFF;
        if (v < r1)
		{
            if (cic == "ddipl")
			{
                r2 ^= r3;
//##                if cic == 'dddev':
//##                    pass    # invalid opcode in v1.0: 014A0000
			}
            else
			{
                r2+=1;
			}
		}
        v = i & 0x1F;
        unsigned long long a = (i<<v) | (rol(i, 0x20-v));
	    r1+=i;
        r3^=i;
        r4+=a;
        //# You have to limit the result here to 32bits.
        r1&= 0xFFFFFFFF;
        r4&= 0xFFFFFFFF;
        if (r5 < i)
            r5 ^= (r1^i);
        else
		{
//##                if cic in ('ddipl', 'dddev'):
            if (cic  == "ddipl")
                r5+=a;
            else
                r5^=a;
		}
        if (cic == "zelda")
		{
            r6 += (i ^ n[nIterationPosition]);
			nIterationPosition++;
			nIterationPosition = nIterationPosition % n.size();
		}
        else
		{
            r6+= (i ^ r4);
		}
        //# Ditto here.
        r5&= 0xFFFFFFFF;
        r6&= 0xFFFFFFFF;
	}

    //# Assemble upper and lower CRCs
    if ((cic == "ddipl") || (cic == "dddev"))
	{
        if (fix)
		{
            setASMvalue(rom, romSize, r1, 0x608, 0x60C);
            setASMvalue(rom, romSize, r2, 0x618, 0x61C);
            setASMvalue(rom, romSize, r3, 0x628, 0x62C);
            setASMvalue(rom, romSize, r4, 0x638, 0x63C);
            setASMvalue(rom, romSize, r5, 0x648, 0x64C);
            setASMvalue(rom, romSize, r6, 0x658, 0x65C);
		}
		// Not sure what this is for?
		returnCrc.push_back(r1);
		returnCrc.push_back(r2);
		returnCrc.push_back(r3);
		returnCrc.push_back(r4);
		returnCrc.push_back(r5);
		returnCrc.push_back(r6);
		return returnCrc;
	}
    if (cic == "yoshi")
	{
        r1*=r2;
        r4*=r5;
	}
    else
	{
//##            r2&= 0xFFFFFFFF
        r1^=r2;
        r4^=r5;
	}
    if ((cic == "diddy") || (cic == "yoshi") || (cic == "aleck"))
	{
        r1+=r3;
        r4+=r6;
	}
    else
	{
        r1^=r3;
        r4^=r6;
	}
    //# Make sure they fit within 4 bytes each.
    r1&= 0xFFFFFFFF;
    r4&= 0xFFFFFFFF;
    if (fix)
	{
        WriteLongToBuffer(rom, 16, (unsigned long)r1);
        WriteLongToBuffer(rom, 20, (unsigned long)r4);
	}
	returnCrc.push_back(r1);
	returnCrc.push_back(r4);
	return returnCrc;
}