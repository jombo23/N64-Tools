#include "StdAfx.h"
#include "NinDec.h"

CNinDec::CNinDec(void)
{
}

CNinDec::~CNinDec(void)
{
}

int CNinDec::dec(unsigned char* inputBuffer, int compressedSize, unsigned char* outputBuffer)
{
	int inputPosition = 4;
	int outputPosition = 0;

     unsigned long cmd = 0x10000;

     int out_sz = ((((((inputBuffer[3] << 8) | inputBuffer[2]) << 8) | inputBuffer[1]) << 8) | inputBuffer[0]);

     while (outputPosition<out_sz)
	 {
         if (cmd>0xFFFF)
		 {
             cmd = 0x100 | inputBuffer[inputPosition++];
		 }
         if (cmd & 0x80)
		 {
			 unsigned long l = 0;

             unsigned long p = inputBuffer[inputPosition++];
             if (p&0x80)
			 {
                 l = (p>>4)&7;
                 l += 1;
                 p <<= 8;
                 p |= inputBuffer[inputPosition++];
                 p &= 0xFFF;
			 }
             else
			 {
                 l = 2;
			 }
             p+=1;
             for (int i = 0; i < l; i++)
			 {
                 outputBuffer[outputPosition++] = outputBuffer[outputPosition-p];
			 }
		 }
         else
		 {
             outputBuffer[outputPosition++] = inputBuffer[inputPosition++];
		 }
         cmd <<= 1;
	 }

     //# Amusingly, they overshoot sizes and cut at the final size.
     //# Simply allowing the decompression and resizing should be okay though.
     return out_sz;

}

bool CNinDec::Compare(unsigned char* array, unsigned char* needle, int needleLen, int startIndex)
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

int CNinDec::Find(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
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

int CNinDec::RFind(unsigned char* array, unsigned char* needle, int needleLen, int startIndex, int sourceLength)
{
	int resultIndex = -1;
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
            return resultIndex;

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
			resultIndex = index;
            // needle was found
        }

        // continue to search for needle
        //sourceLength -= (index - startIndex + 1);
        startIndex = index + 1;
    }
    return resultIndex;
}

/*
def _search(data, pos, sz, cap, window, minl):
     ml = min(cap, sz - pos)
     if ml < minl:
         return 0, 0
     mp = max(0, pos - window)
     hitp, hitl = 0, minl
     if mp < pos:
         hl = data[mp:pos+hitl].index(data[pos:pos+hitl])
         while hl < (pos - mp):
             while (hitl < ml) and (data[pos + hitl] == data[mp + hl +
hitl]):
                 hitl += 1
             mp += hl
             hitp = mp
             if hitl == ml:
                 return pos - hitp, hitl
             mp += 1
             hitl += 1
             if mp >= pos:
                 break
             hl = data[mp:pos+hitl].index(data[pos:pos+hitl])
     # If length less than minimum, return miss.  Else, get nearest index, not farthest.
     hitl -= 1
     if hitl < minl:
         hitl = 0
     #elif hitl == minl:
     else:
         hitp = data[max(0, pos - window):pos+hitl-1].rfind(data[pos:pos+hitl]) + max(0, pos - window)
     return pos - hitp, hitl
	 */

std::vector<int> CNinDec::_search(unsigned char* data, int pos, int sz, int cap, int window, int minl)
{
	std::vector<int> returnData;
	returnData.resize(2);

	int ml = min(cap, sz - pos);
	if (ml < minl)
	{
		returnData[0] = 0;
		returnData[1] = 0;
		return returnData;
        //return 0, 0
	}

    int mp = max(0, pos - window);

    int hitp = 0;
	int hitl = minl;

	int hl = -1;

    if (mp < pos)
	{
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
				returnData[0] = pos - hitp;
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
    if (hitl < minl)
        hitl = 0;
	else
	{
		//hitp = data[max(0, pos - window):pos+hitl-1].rfind(data[pos:pos+hitl]) + max(0, pos - window)
		// Done a different way...just automatically scan initially for latest of the best match
	}

	returnData[0] = pos - hitp;
	returnData[1] = hitl-1;
    //return hitp, hitl-1
	return returnData;
}

unsigned long CNinDec::Flip32Bit(unsigned long inLong)
{
	return (((inLong & 0xFF000000) >> 24) | ((inLong & 0x00FF0000) >> 8) | ((inLong & 0x0000FF00) << 8) | ((inLong & 0x000000FF) << 24));
}

void CNinDec::WriteLongToBuffer(unsigned char* Buffer, unsigned long address, unsigned long data)
{
	Buffer[address] = ((data >> 24) & 0xFF);
	Buffer[address+1] = ((data >> 16) & 0xFF);
	Buffer[address+2] = ((data >> 8) & 0xFF);
	Buffer[address+3] = ((data) & 0xFF);
}

int CNinDec::encode(unsigned char* data, int dataSize, unsigned char* outputBuffer, int peek, bool header)
{
	int outputPosition = 0;

     //"""If peek is none, no peeking is performed.  If an int, will replace on hitl+peek."""
     int sz = dataSize;
     if (header)
	 {
		 //out.extend(struct.pack("<L", sz))
		WriteLongToBuffer(outputBuffer, outputPosition, Flip32Bit(sz));
		outputPosition += 4;
	 }
     int pos = 0;
	 unsigned char c = 0;
	 int cmds = outputPosition;
	 unsigned char flag = 0x80;

	 outputBuffer[outputPosition++] = 0;
     while (pos < sz)
	 {
		 std::vector<int> searchReturn;

		 int hitpbest = 0;
		 int hitlbest = 0;
		
         searchReturn = _search(data, pos, sz, 8, 4096, 2);

		 hitpbest = searchReturn[0];
		 hitlbest  = searchReturn[1];

		 if (hitpbest > 2)
		 {
			 int windowLeft = searchReturn[0] - 1;
			 while (windowLeft > 0)
			 {
				searchReturn = _search(data, pos, sz, 8, windowLeft, 2);

				if (searchReturn[1] < 2)
					break;

				if (hitlbest == searchReturn[1])
				{
					hitpbest = searchReturn[0];
					hitlbest = searchReturn[1];
				}

				windowLeft = searchReturn[0] - 1;
			 }
		 }


		 int hitp = hitpbest;
		 int hitl = hitlbest;

         if ((hitl == 2) && (hitp > 128))
		 {
             //# Short lengths are non-encodable.  Pass a literal.
             hitl = 1;
		 }
         if (hitl < 2)
		 {
             //# Push a raw if copying isn't possible.
			 outputBuffer[outputPosition++] = data[pos];
             pos += 1;
		 }
         else
		 {
             //# If the shoe fits, wear it...but check for a better fit ;*)
             if (peek != 0)
			 {
                 searchReturn = _search(data, pos+1, sz, 8, 4096, 2);

				 int tstpbest = searchReturn[0];
				 int tstlbest  = searchReturn[1];

				 if (tstpbest > 2)
				 {
					 int windowLeft = searchReturn[0] - 1;
					 while (windowLeft > 0)
					 {
						searchReturn = _search(data, pos+1, sz, 8, windowLeft, 2);

						if (searchReturn[1] < 2)
							break;

						if (tstlbest == searchReturn[1])
						{
							tstpbest = searchReturn[0];
							tstlbest = searchReturn[1];
						}

						windowLeft = searchReturn[0] - 1;
					 }
				 }

				 int tstp = tstpbest;
				 int tstl = tstlbest;


                 //# Ordinarily +1 should be more ideal because it compensates for the literal.
                 //# For whatever reason I'm getting a 20% growth in filesize when not 0...
                 //# Looks like in case of run+run long initial runs aren't as ideal as later runs.
                 //# Probably an indictor they encoded from back forward, not forward back.
                 if ((hitl + peek) < tstl)
				 {
                     //# Push it if it really is better.
					 outputBuffer[outputPosition++] = data[pos];
                     pos += 1;
                     flag >>= 1;
                     if (flag == 0)
					 {
                         outputBuffer[cmds] = c;
                         c = 0;
						 flag = 0x80;
                         cmds = outputPosition;
						 outputBuffer[outputPosition++] = 0;
					 }
                     hitl = tstl;
                     hitp = tstp;
				 }
			 }
             c |= flag;
             unsigned long e = hitp - 1;
             pos += hitl;
             if (hitl == 2)
			 {
                 outputBuffer[outputPosition++] = e & 0x7F;
			 }
             else
			 {
                 hitl -= 1;
                 e &= 0xFFF;
                 unsigned long u = e >> 8;
                 u |= (hitl & 7) << 4;
                 outputBuffer[outputPosition++] = (0x80 | u);
                 outputBuffer[outputPosition++] = (e & 0xFF);
			 }
		 }
         //# Advance the flag and refill if required.
         flag >>= 1;
         if (flag == 0)
		 {
             outputBuffer[cmds] = c;
             c = 0;
			 flag = 0x80;
             cmds = outputPosition;
             outputBuffer[outputPosition++] = 0;
		 }
	 }
     //# Update any hanging command registers.
     outputBuffer[cmds] = c;
     return outputPosition;
}