#include "StdAfx.h"
#include "EBBigDecompression.h"

CEBBigDecompression::CEBBigDecompression(void)
{
}

CEBBigDecompression::~CEBBigDecompression(void)
{
}

unsigned short CEBBigDecompression::CharArrayToShort(unsigned char* currentSpot)
{
	return ((currentSpot[0] << 8) | currentSpot[1]);
}

unsigned long CEBBigDecompression::CharArrayToLong(unsigned char* currentSpot)
{
	return ((((((currentSpot[0] << 8) | currentSpot[1]) << 8) | currentSpot[2]) << 8) | currentSpot[3]);
}

void CEBBigDecompression::DecodeFile(unsigned char* ROM, unsigned long start, unsigned char*& data, int& dataLength)
{
	data = new unsigned char[0x100000];
	dataLength = 0;

	std::vector<EBBigEntry> bigEntries = buffering_pointers(&ROM[start]);
	for (int x = 0; x < bigEntries.size(); x++)
	{
		if (bigEntries[x].data_t == "eb-raw")
		{
			int outputSize = 0;
			dec(&ROM[start + bigEntries[x].offset], bigEntries[x].cmp_s, &data[dataLength], outputSize);
			dataLength += outputSize;
		}
		else // "bin"
		{
			memcpy(&data[dataLength], &ROM[start + bigEntries[x].offset], bigEntries[x].cmp_s);
			dataLength += bigEntries[x].cmp_s;
		}
	}
}

/*
    def __init__(self, pos, dec_s, cmp_s, data_t, name=None):
        if(name==None):
            name="{:X}.bin".format(pos)
        self.name  = name
        self.pos   = pos
        self.dec_s = dec_s
        self.cmp_s = cmp_s
        self.data_t= data_t.lower()

    def __repr__(self):
        return repr((self.name, self.pos, self.dec_s, self.cmp_s, self.data_t))

    def dec_all(data, pointers=None, base=None):
        if data==None: return
        ## generate list of pointers from archive index
        if pointers==None:
            pointers = EBBig.buffering_pointers(data, base)
        if pointers==None: return
        ## output all files in archive
        tbl = list(range(0,len(pointers)))
        EBBig.dec_files(data, tbl, pointers)

    def dec_sel(data, pointers=None, base=None):
        if data==None: return
        ## generate list of pointers from archive index
        if pointers==None:
            pointers = EBBig.buffering_pointers(data, base)
        if pointers==None: return
        ## get selection from dialog
        sel = ListBoxChoice(app, "Excitebike64 Big Contents", "Select one or more files to decompress", EBBig.list_pointers(pointers)).returnValue()
        if sel==None or sel==[]: return
        idx = []
        ## build a comparison table so index will work
        tbl = [pointers[x].name for x in range(len(pointers))]
        for i in range(len(sel)):
            idx.append(tbl.index(sel[i]))
        ## grab selected indices
        EBBig.dec_files(data, idx, pointers)

    def dec_files(data, tbl, pointers):
        ## iterate list and return individual files
        for i in range(len(tbl)):
            p = pointers[tbl[i]]
            d = data[p.pos:p.pos+p.cmp_s]
            if len(d)<=0: continue

            out = bytearray()
            ## output individual files
            try:
                cmd = app.list_t.get(p.data_t, None)
                if cmd:
                    output = cmd.command(d, p, cmd.args)
                    if output: out.extend(output)
                else:
                    app.writeToLog("\tError: {} is not mapped to a command.\n".format(p.data_t),('warning'))
            except:
                app._report_exception()

            ## output file if it exists
            if(len(out)):
                app.writeToLog("{:X}\tEB Big {}: {}\t0x{:X}\t\n".format(p.pos, i, p.data_t, len(out)))
                f_out=app.newfile(p.name, p.pos)
                if(f_out!=None):
                    f_out.write(out)
                    f_out.close()
            else:
                app.writeToLog(">>>  Error!  Unable to extract EB Big #{}: {}\n".format(i, p.name), ('warning'))
            del d
*/

int CEBBigDecompression::_flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, int compressedSize)
{
	if (flagsLeft == 0)
	{
		if (inputPosition >= compressedSize)
			return 0;
		flagsLeft = 8;
		flags = inputBuffer[inputPosition++];
	}

    int value = ((flags & 0x1)>0);
    flags>>=1;
	flagsLeft--;
	return value;
}

unsigned long CEBBigDecompression::_nbits(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagsLeft, int num, int compressedSize, unsigned long startValue)
{
    //"""Returns a value equal to the next num bits in stream.
    //itr should point to the self._flags() method above."""
    unsigned long v=startValue;
    for (int i = 0; i < num; i++)
	{
        v <<=1;
		bool flagRet = _flags(inputBuffer, inputPosition, flags, flagsLeft, compressedSize);
        v|=flagRet;
	}
    return v;
}

int CEBBigDecompression::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* outputBuffer, int& outputBufferPosition)
{
	std::vector<int> offsets;
	offsets.push_back(1);
	offsets.push_back(0x21);
	offsets.push_back(0xA1);
	offsets.push_back(0x2A1);
	std::vector<int> bits;
	bits.push_back(5);
	bits.push_back(7);
	bits.push_back(9);
	bits.push_back(10);

	outputBufferPosition = 0;

    //"""Decompresses data to output until terminator is reached."""
    //# initialize the data stream
	int inputBufferPosition = 0;
    int inputBufferBitPosition = 0;
	unsigned char flags = 0;
	int flagsLeft = 0;

	int count = 0;
    //# Continue until terminator reached.
    while (true)
	{
        if (_nbits(inputBuffer, inputBufferPosition, flags, flagsLeft, 1, compressedSize))
		{
            count = _nbits(inputBuffer, inputBufferPosition, flags, flagsLeft, 1, compressedSize);
            if (count)
			{
                count = _nbits(inputBuffer, inputBufferPosition, flags, flagsLeft, 2, compressedSize);
                if (!count)
				{
                    count = _nbits(inputBuffer, inputBufferPosition, flags, flagsLeft, 4, compressedSize);
                    if (!count)
					{
                        count = inputBuffer[inputBufferPosition++];
                        if (!count)
                            break;
                        count+=0x12;
					}
                    else
					{
                        count+=3;
					}
				}
			}
            count+=2;
            //# Determine backtrack.
            int b = _nbits(inputBuffer, inputBufferPosition, flags, flagsLeft, 2, compressedSize);
            //# Slight nuance: if >8, read the next byte, NOT from the register!
            int back = offsets[b];
            b = bits[b];
            int v = 0;
            if (b>=8)
			{
                v = inputBuffer[inputBufferPosition++];
                b-=8;
			}
            back+= _nbits(inputBuffer, inputBufferPosition, flags, flagsLeft, b, compressedSize, v);
            back = outputBufferPosition - back;
            //# Copy and append count bytes from pos to output.
            for (int i = 0; i < count; i++)
			{
                v = outputBuffer[back+i];
                outputBuffer[outputBufferPosition++] = v;
			}
		}
        else
		{
            outputBuffer[outputBufferPosition++] = inputBuffer[inputBufferPosition++];
		}
	}
	return outputBufferPosition;
}

std::vector<EBBigEntry> CEBBigDecompression::buffering_pointers(unsigned char* data)
{
    //if base==None: base='output.big'
    //## pre-format name
    //if '.' in base:
        //buf = base.rsplit('.',1)
        //base = buf[0] + "-{}." + buf[1]
        //del buf
    //else: base+="-{}"
    //## retrieve number of entries from first entry
    int pos = 12;
    int num = CharArrayToLong(&data[pos]) >> 4;
    num -= pos;
    num /= 4;
	std::vector<EBBigEntry> pointers;
	for (int x = 0; x < num; x++)
	{
        int offset = CharArrayToLong(&data[pos]);
		CString data_t;
		if (offset&1)
			data_t = "eb-raw";
		else
			data_t = "bin";
        offset>>=4;
        //## determine size via offset
		int cmp_s;
        if (x == num-1)
            cmp_s = 0;
        else
            cmp_s = (CharArrayToLong(&data[pos + 4]) >> 4) - offset;
        //## generate names
        CString name;
		name.Format("%08X", offset);
        //## append a new entry
        pointers.push_back(EBBigEntry(offset, cmp_s, cmp_s, data_t, name));
        pos+=4;
	}
    return pointers;
}
/*
    def list_pointers(pointers):
        table = []
        for x in range(len(pointers)-1):
            if(pointers[x].data_t!="none"):
                table.append(pointers[x].name)
        return table

    def print_pointers(pointers):
        app.writeToLog("\n\t Offset\t  Size\tIn File\tFormat\n", ('highlight'))
        for x in range(len(pointers)-1):
            if(pointers[x].data_t!="none"): app.writeToLog("\t{:8X}\t{:6X}\t{:6X}\t{}\n".format(pointers[x].pos, pointers[x].dec_s, pointers[x].cmp_s, list_t.get(pointers[x].data_t, list_t.get('none')).name), peek=None)
            else:
                if app.superlist.get(): app.writeToLog("\t{:8X}\t{:6X}\t{:6X}\t{:12}\n".format(pointers[x].pos, pointers[x].dec_s, pointers[x].cmp_s, list_t.get(pointers[x].data_t, list_t.get('none')).name), ('warning'), peek=None)
*/
