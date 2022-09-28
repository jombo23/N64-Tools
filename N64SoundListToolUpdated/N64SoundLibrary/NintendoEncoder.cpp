// By Zoinkity!
/*#-------------------------------------------------------------------------------
# Nintendo standard compression follows

"""
Utility for handling Nintendo's MIO, Yaz, and Yay compression types.
The number following the compression format denotes the PI domain
    of the source file, which for cartridges is '0' and disk '1'.
    This is only used by disk games and expansions, so the default
    mode for all operations is cartridge ('0').

There is a simple commandline interface.  See help(Nintendo.main) for details.

decode() will read the type from the file,
    returning a decompressed binary for any of the types.

encode() compresses files to Yay0 by default, returning a bytes object.
    This can encode any Yax or MIx type
encode_external() calls yay.exe to encode instead of using pure python.
    Note the external program can only create Yax files, not MIx.

sizeof() returns the actual compressed size of the file,
    before padding is applied.  This can be rather helpful for REs.

Yaz is transformed into 3-table format when read.
Admittantly, Yaz could be interpretted as a stream and doesn't rely on filesize.
However, by doing this conversion allows one decompression routine and the support code.

Yax0r() converts between stream mode (Yaz/MIL) and block mode (Yay/MIO).
    Can't find a Yaz encoder?  Encode as Yay and run through Yax0r.

This targets the N64, so no complaints about a lack of Rarc suppport ;*)
"""*/

#include "StdAfx.h"
#include "NintendoEncoder.h"
#include <vector>
#include <algorithm>

CNintendoEncoder::CNintendoEncoder(void)
{
	Ya.push_back("Yaz0");
	Ya.push_back("Yay0");
	Ya.push_back("Yaz1");
	Ya.push_back("Yay1");
	MI.push_back("MIO0");
	MI.push_back("MIL0");
	MI.push_back("MIO1");
	MI.push_back("MIL1");
	stream.push_back("Yaz0");
	stream.push_back("Yaz1");
	stream.push_back("MIL0");
	stream.push_back("MIL1");
	block.push_back("Yay0");
	block.push_back("Yay1");
	block.push_back("MIO0");
	block.push_back("MIO1");
	valid.push_back("MIO0");
	valid.push_back("MIL0");
	valid.push_back("MIO1");
	valid.push_back("MIL1");
	valid.push_back("Yaz0");
	valid.push_back("Yay0");
	valid.push_back("Yaz1");
	valid.push_back("Yay1");
}

CNintendoEncoder::~CNintendoEncoder(void)
{
}


void CNintendoEncoder::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

void CNintendoEncoder::WriteShortToBuffer(unsigned char* Buffer, unsigned long address, unsigned short data)
{
	Buffer[address] = ((data >> 8) & 0xFF);
	Buffer[address+1] = ((data) & 0xFF);	
}

std::vector<bool> CNintendoEncoder::_next_bit(std::vector<unsigned char> ctl)
{
	std::vector<bool> b;

    for (int y = 0; y < ctl.size(); y++)
	{
		unsigned char bits = ctl[y];

		for (int x = 0; x < 8; x++)
		{
            b.push_back((bool)(bits & 0x80));
            bits <<= 1;
		}
	}
    //else:
        //yield -1

	return b;
}

std::vector<unsigned short> CNintendoEncoder::_next_cpy(std::vector<unsigned char> back)
{
	std::vector<unsigned short> c;
	for (int x = 0; x < (back.size() & (0xFFFFFFFE)); x += 2)
	{
		c.push_back((back[x] << 8) | back[x+1]);
	}
    //back = struct.unpack(">{:d}H".format(len(back)>>1), back)
    //yield from back
//##    for i in range(0, len(back), 2):
//##        yield int.from_bytes(back[i:i+2], byteorder='big')
	return c;
}

int CNintendoEncoder::_dec(int dec_s, std::vector<unsigned char> ctl, std::vector<unsigned char> back, std::vector<unsigned char> vals, unsigned char* output, CString mode="Yay")
{
    //"""
    //Decompresses archive of type mode using data tables and size.
    //Mode is presumed to be Yax,
        //although honestly only MIO is tested for.
    //If MIO or MIL, copy bytes receive different treatment.
    //"""
    //## initialize iterators
	std::vector<bool> b = _next_bit(ctl);
	std::vector<unsigned char> v = vals;
	std::vector<unsigned short> c = _next_cpy(back);

    int outputPosition = 0;

    try
	{
        while (outputPosition < dec_s)
		{
			bool nextB = b[0];
			b.erase(b.begin());

            if (nextB)
			{
                output[outputPosition++] = v[0];
				v.erase(v.begin());
			}
            else
			{
                unsigned long off = c[0];
				c.erase(c.begin());

                unsigned long cnt = (off>>12)&0xF;
                off&= 0xFFF;
                off+= 1;
                if (std::find(MI.begin(), MI.end(), mode) != MI.end())
				{
                    cnt+= 3;
				}
                else
				{
                    if (cnt==0)
					{
                        cnt = v[0] + 16;
						v.erase(v.begin());
					}
                    cnt+= 2;
				}

                for (int i = 0; i < cnt; i++)
				{
                    unsigned char val = output[outputPosition-off];
                    output[outputPosition++] = val;
				}
			}
		}
	}
	catch (...)
	{
		return 0;
	}

	return outputPosition;
}

int CNintendoEncoder::_z_from_tables(std::vector<unsigned char> ctl, std::vector<unsigned char> back, std::vector<unsigned char> values, unsigned char* output, int& outputPosition, int dec_s=-1, CString mode="Yaz0")
{
    //"""
    //Reencodes tables in stream format.
    //(makes a Yaz datastream from tables)
	//
    //If the decompressed size is not given,
    //    will run until out of control bits.
    //The side-effect is upwards of 7 additional erroneous copy commands.
	//
    //For the heck of it, also wrote in a mode to spit out MIL files,
    //    the theoretical stream version of MIO
    //"""
    //## hack: directly manage the values
	std::vector<unsigned char> b = ctl;
	std::vector<unsigned char> v = values;
	std::vector<unsigned short> c = _next_cpy(back);

    unsigned long bit = 0x10000;
    //## if dec_s declared, will keep accurate track
    while (dec_s > 0)
	{
        //## get next bit
        if (bit > 0xFFFF)
		{
            bit = b[0];
			b.erase(b.begin());

            output[outputPosition++] = bit & 0xFF;

            bit |= 0x100;
		}

        //## catch end of control commands
        if (bit & 0x80)
		{
            output[outputPosition++] = v[0];
			v.erase(v.begin());

            dec_s-=1;
		}
        else
		{
            unsigned short val = c[0];
			c.erase(c.begin());

			output[outputPosition++] = ((val >> 8) & 0xFF);
			output[outputPosition++] = ((val) & 0xFF);

            //## decrement dec_s accurately with length
            val>>=12;
            val&=0xF;
            if (std::find(MI.begin(), MI.end(), mode) != MI.end())
			{
                dec_s-=1;
			}
            else if (val == 0)
			{
                val = v[0];
				v.erase(v.begin());

                output[outputPosition++] = val;
                val+=16;
			}
            dec_s-= val+2;
		}
        bit<<=1;
	}

    return outputPosition;
}

int CNintendoEncoder::_build_file(CString name, int dec_s, std::vector<unsigned char> ctl, std::vector<unsigned char> back, std::vector<unsigned char> values, unsigned char* output)
{
    //"""
    //Builds a file of given name (type) using given tables and size.
	//
    //Note you don't give the original format name, but the type to be built against.
    //Also note no conversion is done, so this will not magically
    //    transform MIO into Yax or some nonsense.
    //"""
    //## catch anything we can't write
	if (std::find(valid.begin(), valid.end(), name) == valid.end())
        return 0;

    //from struct import Struct
    //pat = Struct(">4s3L")
    //## write a header based on type

	if (std::find(stream.begin(), stream.end(), name) != stream.end())
	{
		char* modeStr = name.GetBuffer();
		output[0] = (unsigned char)modeStr[0];
		output[1] = (unsigned char)modeStr[1];
		output[2] = (unsigned char)modeStr[2];
		output[3] = (unsigned char)modeStr[3];
		name.ReleaseBuffer();

		WriteLongToBuffer(output, 4, dec_s);
		WriteLongToBuffer(output, 8, 0);
		WriteLongToBuffer(output, 12, 0);

		int outputPosition = 0;
		_z_from_tables(ctl, back, values, &output[16], outputPosition, dec_s, name);

		return outputPosition;
	}
    else
	{
        //## MIO and Yay need offsets to data tables
        //## ctl is word-aligned
       // align = bytes((4 - len(ctl)) & 3)
        int om = 16 + ctl.size();
        int oc = om + back.size();

		char* modeStr = name.GetBuffer();
		output[0] = (unsigned char)modeStr[0];
		output[1] = (unsigned char)modeStr[1];
		output[2] = (unsigned char)modeStr[2];
		output[3] = (unsigned char)modeStr[3];
		name.ReleaseBuffer();

		WriteLongToBuffer(output, 4, dec_s);
		WriteLongToBuffer(output, 8, om);
		WriteLongToBuffer(output, 12, oc);

		int outputPosition = 16;

		for (int x = 0; x < ctl.size(); x++)
		{
			output[outputPosition++] = ctl[x];
		}

		if ((ctl.size() % 4) != 0)
		{
			for (int x = 0; x < (4 - (ctl.size() % 4)); x++)
			{
				output[outputPosition++] = 0;
			}
		}

		for (int x = 0; x < back.size(); x++)
		{
			output[outputPosition++] = back[x];
		}

		for (int x = 0; x < values.size(); x++)
		{
			output[outputPosition++] = values[x];
		}

        //## write in control codes, copy commands, then values
        //return b''.join((pat.pack(buf, dec_s, om, oc), ctl, align, back, values))
	}
}

void CNintendoEncoder::_z_to_tables(unsigned char* data, int dataSize, std::vector<unsigned char>& ctl, std::vector<unsigned char>& back, std::vector<unsigned char>& vals, CString mode = "Yaz0")
{
    //"""
    //Decodes stream into tables (Yaz->Yay)
    //This is a convenience to recycle code with all three types.
	//
    //For the heck of it, threw in a mode to decode MIL,
    //    the theoretical MIO stream mode.
    //"""
    ctl.clear();
	vals.clear();
	back.clear();

    int i = 0;
    unsigned long bits = 0x10000;
    while (i < dataSize)
	{
        if (bits > 0xFFFF)
		{
            ctl.push_back(data[i]);
            bits = data[i] | 0x100;
            i += 1;
            if (i >= dataSize)
				break;
		}
        if (bits & 0x80)
		{
            vals.push_back(data[i]);
            i+=1;
		}
        else
		{
			for (int x = i; x < (i + 2); x++)
			{
				back.push_back(data[x]);
			}

            //## detect the 3-byte long reference
            unsigned long v = (data[i]>>4)&0xF;
            i+=2;
			if (std::find(MI.begin(), MI.end(), mode) != MI.end())
			{
				//pass
			}
            else if (v==0)
			{
                if (i >= dataSize)
					break;
                vals.push_back(data[i]);
                i += 1;
			}
		}
        bits <<= 1;
	}
    //return ctl, back, vals
}



unsigned long CNintendoEncoder::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

unsigned long CNintendoEncoder::CharArrayToLong(unsigned char* currentSpot)
{
	return Flip32Bit(*reinterpret_cast<unsigned long*> (currentSpot));
}

unsigned short CNintendoEncoder::Flip16Bit(unsigned short ShortValue)
{
	return ((ShortValue >> 8) | ((ShortValue << 8)));
}

unsigned short CNintendoEncoder::CharArrayToShort(unsigned char* currentSpot)
{
	return Flip16Bit(*reinterpret_cast<unsigned short*> (currentSpot));
}



bool CNintendoEncoder::_from_header(unsigned char* data, int dataSize, CString& name, int& dec_s, std::vector<unsigned char>& ctl, std::vector<unsigned char>& back, std::vector<unsigned char>& values, int& cmp_s)
{
    //"""
    //Returns the name(type) of archive, its decompressed size,
    //    the three files needed to parse the compression,
    //   and the actual compressed size of the file before padding.
	//
    //Note Yaz/MIL will be converted to table form from stream.
    //"""
    ctl.clear();
	values.clear();
	back.clear();
	name.Format("%s", &data[0]);


    dec_s = CharArrayToLong(&data[4]);
	if (std::find(stream.begin(), stream.end(), name) != stream.end())
	{
        //## stream variant of Yay0; decode to segments
        _z_to_tables(&data[16], (dataSize - 16), ctl, back, values, name);
        cmp_s = 16 + ctl.size() + back.size() + values.size();
	}
    else if (std::find(block.begin(), block.end(), name) != block.end())
	{
        unsigned long seg1 = CharArrayToLong(&data[8]);
        unsigned long seg2 = CharArrayToLong(&data[12]);

		for (int x = 16; x < seg1; x++)
			ctl.push_back(data[x]);
        
		for (int x = seg1; x < seg2; x++)
			back.push_back(data[x]);

		std::vector<bool> b = _next_bit(ctl);
//##            values=data[seg2:]
        //# Compute length of values from ctl + back

		std::vector<unsigned short> c = _next_cpy(back);
        int sz = 0;
		int s = 0;

        while (s < dec_s)
		{
			bool nextB = b[0];
			b.erase(b.begin());

            if (nextB)
			{
                sz += 1;
                s += 1;
			}
            else
			{
                unsigned long v = c[0];
				c.erase(c.begin());

                v = (v>>12)&0xF;
				if (std::find(MI.begin(), MI.end(), name) != MI.end())
				{
                    v+= 3;
				}
                else
				{
                    if (v==0)
					{
                        v = data[seg2+sz] + 16;
                        sz += 1;
					}
                    v+= 2;
				}
                s += v;
			}
		}
        cmp_s = seg2 + sz;

		for (int x = seg2; x < cmp_s; x++)
			values.push_back(data[x]);
	}
    else
	{
        return false;
	}
    //return name, dec_s, ctl, back, values, cmp_s
	return true;
}

//##def _inittree(seq):
//##    from itertools import repeat
//##    l = len(seq)
//##    tree = list(repeat(l, 0x100))
//##    for c,i in enumerate(seq, 1):
//##        tree[i] = l - c
//##    return tuple(tree)
//##
//##def _seqfind(data, hitl):
//##    if len(data) < hitl:
//##        return len(data)
//##    pos = len(data) - hitl
//##    tree = _inittree(data[pos:])
//##    lc, ll = hitl - 1, hitl - 1
//##    c = data[pos + lc]
//##    while True:
//##        while data[lc] != c:
//##            lc += tree[data[lc]]
//##        ll = hitl - 2
//##        lc -= 1
//##        while ll >= 0:
//##            if data[pos + ll] != data[lc]:
//##                lc += max(tree[data[lc]], (hitl - ll))
//##                break
//##            ll -= 1
//##            lc -= 1
//##        else:
//##            return (lc + 1)

bool CNintendoEncoder::Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex)
{
    // compare
    for (int i = 0, p = startIndex; i < needleLen; i++, p++)
    {
        if (array[p] != needle[i])
        {
            return false;
        }
    }
    return true;
}

int CNintendoEncoder::Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
{
    int index;

    while (startIndex < (sourceLength - needleLen + 1))
    {
        // find needle's starting element
		int index = -1;
		for (int r = startIndex; r < (sourceLength - needleLen + 1); r++)
		{
			if (array[r] == needle[0])
			{
				index = r;
				break;
			}
		}

        // if we did not find even the first element of the needle, then the search is failed
        if (index == -1)
            return -1;

        int i, p;
        // check for needle
        for (i = 0, p = index; i < needleLen; i++, p++)
        {
            if (array[p] != needle[i])
            {
                break;
            }
        }

        if (i == needleLen)
        {
            // needle was found
            return index;
        }

        // continue to search for needle
        //sourceLength -= (index - startIndex + 1);
        startIndex = index + 1;
    }
    return -1;
}

std::vector<int> CNintendoEncoder::_search(unsigned char* data, int pos, int sz, int cap = 0x111)
{
	std::vector<int> returnData;
	returnData.resize(2);

    int mp = max(0, pos - 0x1000);
    int ml = min(cap, sz - pos);
    if (ml < 3)
	{
		returnData[0] = 0;
		returnData[1] = 0;
		return returnData;
        //return 0, 0
	}
    int hitp = 0;
	int hitl = 3;
	int hl = -1;

    if (mp < pos)
	{
//##        hl = _seqfind(data[mp:pos+hitl], hitl)
		hl = Find(&data[mp], &data[pos], hitl, 0, pos + hitl - mp);
        //hl = data[mp:pos+hitl].find(data[pos:pos+hitl])
        while (hl < (pos - mp))
		{
            while ((hitl < ml) && (data[pos + hitl] == data[mp + hl + hitl]))
			{
                hitl += 1;
			}
            mp += hl;
            hitp = mp;
            if (hitl == ml)
			{
				returnData[0] = hitp;
				returnData[1] = hitl;
                //return hitp, hitl
				return returnData;
			}
            mp += 1;
            hitl += 1;
            if (mp >= pos)
                break;
//##            hl = _seqfind(data[mp:pos+hitl], hitl)
            //hl = data[mp:pos+hitl].find(data[pos:pos+hitl])
			hl = Find(&data[mp], &data[pos], hitl, 0, pos + hitl - mp);
		}
	}

    //# If length less than 4, return miss.
    if (hitl < 4)
        hitl = 1;

	returnData[0] = hitp;
	returnData[1] = hitl-1;
    //return hitp, hitl-1
	return returnData;
}

int CNintendoEncoder::encode(unsigned char* data, int dataSize, unsigned char* output, CString mode="Yay0")
{
    //"""Encodes data in given mode, returning a bytes object.
    //If mode is None, will return a tuple of the ctrls, backs, and vals.
    //Acceptable modes are:
    //    None, "Yaz", "Yay", "Yaz0", "Yay0", "Yaz1", "Yay1"
    //          "MIO", "MIL", "MIO0", "MIL0", "MIO1", "MIL1"
    //"""
	if (mode == "Yay")
        mode = "Yay0";
    else if (mode == "Yaz")
        mode = "Yaz0";
    if (mode == "MIO")
        mode = "MIO0";
    else if (mode == "MIL")
        mode = "MIL0";

    if (dataSize == 0)
	{
        //return b"".join((mode.encode('ascii'), bytes(12)))

		char* modeStr = mode.GetBuffer();
		output[0] = (unsigned char)modeStr[0];
		output[1] = (unsigned char)modeStr[1];
		output[2] = (unsigned char)modeStr[2];
		output[3] = (unsigned char)modeStr[3];
		mode.ReleaseBuffer();

		for (int x = 4; x < 16; x++)
			output[x] = 0x00;
		return 16;
	}
    
	if ((std::find(Ya.begin(), Ya.end(), mode) == Ya.end()) && (std::find(MI.begin(), MI.end(), mode) == MI.end()))
	{
		return 0;
	}

    //from array import array
    //import sys
    //from struct import Struct
    //pat = Struct(">4s3L")

	int cap;
	if (std::find(MI.begin(), MI.end(), mode) != MI.end())
		cap = 0x12;
	else
		cap = 0x111;

    int sz = dataSize;
	std::vector<unsigned long> cmds;
	std::vector<unsigned short> ctrl;
    
	std::vector<unsigned char> raws;
    
	cmds.push_back(0);
    unsigned long pos = 0;
	unsigned long flag = 0x80000000;

    while (pos < sz)
	{
		std::vector<int> searchReturn = _search(data, pos, sz, cap);

        int hitp = searchReturn[0];
		int hitl  = searchReturn[1];

        if (hitl < 3)
		{
            //# Push a raw if copying isn't possible.
            raws.push_back(data[pos]);
            cmds[cmds.size()-1] |= flag;
            pos += 1;
		}
        else
		{
			searchReturn = _search(data, pos+1, sz, cap);
            int tstp = searchReturn[0];
			int tstl = searchReturn[1];

            if ((hitl + 1) < tstl)
			{
                raws.push_back(data[pos]);
                cmds[cmds.size()-1] |= flag;
                pos += 1;
                flag >>= 1;
                if (flag == 0)
				{
                    flag = 0x80000000;
                    cmds.push_back(0);
				}
                hitl = tstl;
                hitp = tstp;
			}
            int e = pos - hitp - 1;
            pos += hitl;
            //# Handle MIx first, then Yax conditions.
            if (cap == 0x12)
			{
                hitl -= 3;
                ctrl.push_back((hitl << 12) | e);
			}
            else if (hitl < 0x12)
			{
                hitl -= 2;
                ctrl.push_back((hitl << 12) | e);
			}
            else
			{
                ctrl.push_back(e);
                raws.push_back(hitl - 0x12);
			}
		}
        //# Advance the flag and refill if required.
        flag >>= 1;
        if (flag == 0)
		{
            flag = 0x80000000;
            cmds.push_back(0);
		}
	}

    //# If no cmds in final word, del it.
    if (flag == 0x80000000)
	{
		cmds.erase(cmds.end()- 1);
	}

    //# These need to be big-endian.
    //if sys.byteorder == 'little':
        //cmds.byteswap()
        //ctrl.byteswap()

	if (std::find(block.begin(), block.end(), mode) != block.end())
	{
        int l = (cmds.size() << 2) + 16;
        int o = (ctrl.size() << 1) + l;


		char* modeStr = mode.GetBuffer();
		output[0] = (unsigned char)modeStr[0];
		output[1] = (unsigned char)modeStr[1];
		output[2] = (unsigned char)modeStr[2];
		output[3] = (unsigned char)modeStr[3];
		mode.ReleaseBuffer();

		WriteLongToBuffer(output, 4, sz);
		WriteLongToBuffer(output, 8, l);
		WriteLongToBuffer(output, 12, o);

		int outputPosition = 16;
		for (int x = 0; x < cmds.size(); x++)
		{
			WriteLongToBuffer(output, outputPosition, cmds[x]);
			outputPosition += 4;
		}

		for (int x = 0; x < ctrl.size(); x++)
		{
			WriteShortToBuffer(output, outputPosition, ctrl[x]);
			outputPosition += 2;
		}

		for (int x = 0; x < raws.size(); x++)
		{
			output[outputPosition++] = raws[x];
		}
		
        //return b''.join((pat.pack(mode.encode('ascii'), sz, l, o), cmds.tobytes(), ctrl.tobytes(), raws))
		return outputPosition;
	}
    else if (std::find(stream.begin(), stream.end(), mode) != stream.end())
	{
		char* modeStr = mode.GetBuffer();
		output[0] = (unsigned char)modeStr[0];
		output[1] = (unsigned char)modeStr[1];
		output[2] = (unsigned char)modeStr[2];
		output[3] = (unsigned char)modeStr[3];
		mode.ReleaseBuffer();

		WriteLongToBuffer(output, 4, sz);
		WriteLongToBuffer(output, 8, 0);
		WriteLongToBuffer(output, 12, 0);

		int outputPosition = 0;
		std::vector<unsigned char> ctl;
		std::vector<unsigned char> back;
		std::vector<unsigned char> values;

		for (int x = 0; x < cmds.size(); x++)
		{
			ctl.push_back((cmds[x] >> 24) & 0xFF);
			ctl.push_back((cmds[x] >> 16) & 0xFF);
			ctl.push_back((cmds[x] >> 8) & 0xFF);
			ctl.push_back((cmds[x]) & 0xFF);
		}

		for (int x = 0; x < ctrl.size(); x++)
		{
			back.push_back((ctrl[x] >> 8) & 0xFF);
			back.push_back((ctrl[x]) & 0xFF);
		}

		int dec_s = -1;
		_z_from_tables(ctl, back, raws, &output[16], outputPosition, dataSize, mode);

		return 16 + outputPosition;



        //# Convert to stream format.
        //return b''.join((pat.pack(mode.encode('ascii'), sz, 0, 0), _z_from_tables(cmds.tobytes(), ctrl.tobytes(), raws, sz, mode)))
	}
    else
	{
        //# If no mode specified, return the tables instead.
        //return (cmds.tobytes(), ctrl.tobytes(), raws)
	}
}

/*int sizeof(unsigned char* data)
{
    //"""Returns the actual compressed size of data."""
    return _from_header(data)[5]
}*/

int CNintendoEncoder::Yax0r(unsigned char* data, int dataSize, unsigned char* output)
{
    //"""
    //Switches Yaz<->Yay format.
    //Yaz is a stream format of Yay, and when reduced to tables
    //    is essentially identical.
    //This is handy, since it means half the tools are needed ;*)
    //"""
	CString name;
	int dec_s;
	std::vector<unsigned char> ctl;
	std::vector<unsigned char> back;
	std::vector<unsigned char> values;
	int cmp_s;
    _from_header(data, dataSize, name, dec_s, ctl, back, values, cmp_s);
    if ((dec_s == 0) || (name.GetLength() == 0))
        return 0;

	CString newName;
	if (std::find(Ya.begin(), Ya.end(), name) != Ya.end())
	{
        //## could just name[2]^=3, if you convert it to death
		if (name[2] == 'z')
		{
			newName = "Yay" + name[3];
		}
		else
		{
			newName = "Yaz" + name[3];
		}
	}
    else if (std::find(MI.begin(), MI.end(), name) != MI.end())
	{
		if (name[2] == 'L')
		{
			newName = "MIO" + name[3];
		}
		else
		{
			newName = "MIL" + name[3];
		}
	}
    else
	{
        return 0;
	}
    
    return _build_file(newName, dec_s, ctl, back, values, output);
}

int CNintendoEncoder::decode(unsigned char* data, int dataSize, unsigned char* output)
{
    //"""
    //Decompress Nintendo archive.
    //Reads header, generating tables for type,
    //    retrieves size, then sends data to _dec().
    //"""
	CString name;
	int dec_s;
	std::vector<unsigned char> ctl;
	std::vector<unsigned char> back;
	std::vector<unsigned char> vals;
	int cmp_s;

    _from_header(data, dataSize, name, dec_s, ctl, back, vals, cmp_s);
    if ((dec_s == 0) || (name.GetLength() == 0))
        return 0;
    return _dec(dec_s, ctl, back, vals, output, name);
}