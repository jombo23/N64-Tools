// By ZOINKITY
#include "StdAfx.h"
#include "n643docompression.h"

n643docompression::n643docompression(void)
{
}

n643docompression::~n643docompression(void)
{
}




//# -------------------------------------------------------------------------------
//# 3DO's decompression routine for N64 titles

int  n643docompression::_flags(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft)
{
	if (flagLeft == 0)
	{
		flagLeft = 8;
		flags = inputBuffer[inputPosition++];
	}

    int value = ((flags&0x80)>0);
    flags<<=1;
	flagLeft--;
	return value;
}

unsigned long n643docompression::helper(unsigned char* inputBuffer, int& inputPosition, unsigned char& flags, int& flagLeft, int num)
{
    //"""Returns a value equal to the next num bits in stream.
    //itr should point to the self._flags() method above."""
    unsigned long v=0;
    for (int i = 0; i < num; i++)
	{
        v <<=1;
		bool flagRet = _flags(inputBuffer, inputPosition, flags, flagLeft);;
        v|=flagRet;
	}
    return v;
}



int n643docompression::uncompressedSize(unsigned char* inputBuffer, bool littleEndianHeaderSize)
{
	if (littleEndianHeaderSize)
		return ((((((inputBuffer[3] << 8) | inputBuffer[2]) << 8) | inputBuffer[1]) << 8) | inputBuffer[0]);
	else
		return ((((((inputBuffer[0] << 8) | inputBuffer[1]) << 8) | inputBuffer[2]) << 8) | inputBuffer[3]);
}

int n643docompression::dec(unsigned char* inputBuffer, int dec_s, unsigned char* outputBuffer, int& compressedSize, bool littleEndianHeaderSize, bool sarge2style)
{
	int inputPosition = 0;
	int outputPosition = 0;

    //# initialize the data stream
	if (littleEndianHeaderSize)
		dec_s = ((((((inputBuffer[3] << 8) | inputBuffer[2]) << 8) | inputBuffer[1]) << 8) | inputBuffer[0]);
	else
		dec_s = ((((((inputBuffer[0] << 8) | inputBuffer[1]) << 8) | inputBuffer[2]) << 8) | inputBuffer[3]);
	unsigned char flags = 0;
	int flagsLeft = 0;

	inputPosition = 4;
    //#initialize registers
	unsigned long bit_reg = helper(inputBuffer, inputPosition, flags, flagsLeft, 17);
    unsigned long bit_msk = 0x20000;
    unsigned long bit_val = 0;
    unsigned long buf_p = 0xFC4;
    //# JAL 80019EFC: build initial tables
    std::vector<unsigned long> t79C0;
    for (int i = 0; i < 0x1000; i++)
	{
		if (i < buf_p)
			t79C0.push_back(32); 
		else 
			t79C0.push_back(0);
	}
    std::vector<unsigned long> t8A00;
    std::vector<unsigned long> t8EF0;
    std::vector<unsigned long> t93E0;
    for (int i = 0; i < 0x13B; i++)
	{
		if (i>1)
			t8A00.push_back(i-1);
		else
			t8A00.push_back(0);
        t8EF0.push_back(1);
		if (i<0x13B)
			t93E0.push_back(0x13A-i);
		else 
			t93E0.push_back(0);
	}
    //# t8A00.append(0)
    t8EF0[0] = 0;
    t8EF0.push_back(0);
    //# t93E0.append(0)
    std::vector<unsigned long> t98D0;
    for (int i = 0; i < 0x1001; i++)
        t98D0.push_back(0);
	for (int i = 0x1000; i > 0; i--)
	{
		unsigned long value = (unsigned long)((float)0x2710 / (float)((i+200))) +t98D0[i];
		t98D0[i-1] = value;
	}
    //# end JAL 80019EFC

    //#decompression loop
    while (outputPosition < dec_s)
	{
        //# v = JAL 8001A0E8
        unsigned long v = bit_reg - bit_val + 1;
        v*=t93E0[0];
        unsigned long s = bit_msk - bit_val;
        v-=1;
        //# v = JAL 8001A544(v/s)
        unsigned long t = (unsigned long)((float)v / (float)s);
		v = 1;
		unsigned long a2 = 0x13A;
		unsigned long a0 = t;

        while (true)
		{
            unsigned long v1 = v + a2;
            a0 = (v1<0) + v1;
            a0>>=1;
            v1 = t93E0[a0];
            if (t < v1)
                v = a0+1;
            else
                a2 = a0;
            if (a2 <= v)
                break;
		}
        // # end v = JAL 8001A544(v/s)
        unsigned long v0 = t93E0[v-1] * s;
        v0 = (unsigned long)((float)v0 / (float)t93E0[0]);
        s *= t93E0[v];
        s = (unsigned long)((float)s /  (float)t93E0[0]);
        bit_msk = v0 + bit_val;
        bit_val+= s;
        //# 8001A228
        while (true)
		{
            if (0xFFFF < bit_val)
			{
                bit_val -= 0x10000;
                bit_msk -= 0x10000;
                bit_reg -= 0x10000;
			}
            else if (( 0x7FFF<bit_val) && (0x18000>=bit_msk))
			{
                bit_val -= 0x8000;
                bit_msk -= 0x8000;
                bit_reg -= 0x8000;
			}
            else if (0x10000<bit_msk)
				break;
            bit_msk<<= 1;
            bit_val<<= 1;
            bit_reg<<= 1;
            bit_reg |= helper(inputBuffer, inputPosition, flags, flagsLeft, 1);
            bit_val &= 0xFFFFFFFF;
            bit_msk &= 0xFFFFFFFF;
            bit_reg &= 0xFFFFFFFF;
		}
        unsigned long w = v;
        v = t8A00[v];
		unsigned long a1 = 0;

        //# JAL 80019FD8[v]
        //if (t93E0[0] == 0)

		//# JAL 80019FD8[v]   8001C104 (Sarge2) tests !<7FFF, not zero
        
		if (((sarge2style) && (t93E0[0]>=0x7FFF)) | ((!sarge2style) && (t93E0[0] == 0)))
		{
            a1=0;
            for (int i = 0x13A; i > 0; i--)
			{
                t93E0[i]=a1;
                v0 = (t8EF0[i]+1)>>1;
                t8EF0[i] = v0;
                a1+=v0;
			}

			if (sarge2style)
				t93E0[0]=a1;
		}
        //# 8001A034
        for (a2 = w; a2 > 0; a2--)
		{
            if (t8EF0[a2]!=t8EF0[a2-1])
                break;
		}
        if (a2 < w)
		{
            a0=t8A00[a2];
            a1=t8A00[w];
            t8A00[w] =a0;
            t8A00[a2]=a1;
		}
        if (a2>=0)
		{
			t8EF0[a2] = t8EF0[a2] + 1;
		}
        for (int i = a2; i > 0; i--)
		{
            t93E0[i-1] = t93E0[i-1] + 1;
		}
        //# end JAL 80019FD8[v]
        //# end v = JAL 8001A0E8
        if (v<0x100) //    # write byte to stream
		{
			outputBuffer[outputPosition++] = (unsigned char)v;
            t79C0[buf_p]=(unsigned char)v;
            buf_p+=1;
            buf_p&=0xFFF;
		}
        else //   # copy run of bytes
		{
            v-=0xFD;
            //# p = JAL 8001A2FC
            unsigned long p = bit_reg - bit_val + 1;
            p*=t98D0[0];
            s = bit_msk - bit_val;
            p-=1;
            //# p = JAL 8001A598(p/s)
            t = (unsigned long)  ((float)p / (float)s);
			p = 1;
			a2 = 0x1000;
			a0 = t;

			unsigned long v1;

            while (true)
			{
                v1 = p + a2;
                a0 = (v1<0) + v1;
                a0>>=1;
                v1 = t98D0[a0];
                if (t < v1)
                    p = a0+1;
                else
                    a2= a0;
                if (a2 <= p)
                    break;
			}
            p-=1;
            //# end p = JAL 8001A598(p/s)
            v0 = t98D0[p] * s;
			float t98D0Floor = t98D0[0];
            v0 = (unsigned long)((float)v0 / t98D0Floor);
            s *= t98D0[p+1];
            s = (unsigned long)((float)s / t98D0Floor);
            bit_msk = v0 + bit_val;
            bit_val+= s;
            //# 8001A440:
            while (true)
			{
                if (0xFFFF < bit_val)
				{
                    bit_val -= 0x10000;
                    bit_msk -= 0x10000;
                    bit_reg -= 0x10000;
				}
                else if ((0x7FFF<bit_val) && (0x18000>=bit_msk))
				{
                    bit_val -= 0x8000;
                    bit_msk -= 0x8000;
                    bit_reg -= 0x8000;
				}
                else if (0x10000<bit_msk)
					break;
                bit_msk<<= 1;
                bit_val<<= 1;
                bit_reg<<= 1;
                bit_reg |= helper(inputBuffer, inputPosition, flags, flagsLeft, 1);
                bit_val &= 0xFFFFFFFF;
                bit_msk &= 0xFFFFFFFF;
                bit_reg &= 0xFFFFFFFF;
			}
            //# end p = JAL 8001A2FC
            p = (buf_p - p - 1)&0xFFF;
            for (int i = 0; i < v; i++)
			{
                unsigned char b = t79C0[p];
				outputBuffer[outputPosition++] = b;
                t79C0[buf_p] = b;
                p+=1;
                p&=0xFFF;
                buf_p+=1;
                buf_p&=0xFFF;
			}
		}
	}

	compressedSize = inputPosition;

    //# decompression only ends when output file reaches expected length
	return dec_s;
}
