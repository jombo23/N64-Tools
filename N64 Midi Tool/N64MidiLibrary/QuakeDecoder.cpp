#include "StdAfx.h"
#include "QuakeDecoder.h"
#include <vector>

CQuakeDecoder::CQuakeDecoder(void)
{
}

CQuakeDecoder::~CQuakeDecoder(void)
{
}

bool CQuakeDecoder::getbit(unsigned char* data, int& bitPosition)
{
	int byteSpot = bitPosition / 8;
	int bitSpot = bitPosition % 8;

	bool value = (bool)((data[byteSpot] >> (7 - bitSpot)) & 0x1);

	bitPosition++;
	return value;
}

void CQuakeDecoder::update(std::vector<int>& tbl1, std::vector<int>& tbl2, std::vector<int>& tbl3, std::vector<int>& tbl4, int x, int y)
 {
     while (true)
	 {
         int i = tbl4[x];
         int j = tbl4[y];
         x = tbl3[x];
         tbl4[x] = i + j;
         if (x == 1)
             break;
         int v = tbl3[x];
         y = tbl1[v];
         if (y == x)
             y = tbl2[v];
	 }
     if (tbl4[1] == 0x7D0)
	 {
		 for (int i = 1; i < 0x4EA; i++)
		 {
			int v = tbl4[i] >> 1;
            tbl4[i] = v;
		 }
	 }
 }

int CQuakeDecoder::aridecode(unsigned char* data, int dataLength, unsigned char* output, int& outputPosition)
{
	outputPosition = 0;
	std::vector<int> bitlengths;
	bitlengths.push_back(4);
	bitlengths.push_back(6);
	bitlengths.push_back(8);
	bitlengths.push_back(10);
	bitlengths.push_back(12);
	bitlengths.push_back(14);

     //# Init
	std::vector<int> tbl1;
	for (int i = 0; i < 0x4EA; i+=2)
		tbl1.push_back(i);
	std::vector<int> tbl2;
	for (int i = 1; i < 0x4EA; i+=2)
		tbl2.push_back(i);
     tbl2[0] = 0;
	 std::vector<int> tbl3;
	 for (int i = 0; i < 0x4EA; i++)
	 {
		 tbl3.push_back(i>>1);
	 }
	 std::vector<int> tbl4;
	 for (int i = 0; i < 0x4EA; i++)
	 {
		tbl4.push_back(1);
	 }
     tbl4[0] = 0;
     tbl4[1] = 0;
     //# Could just write these instead...
     int ceilValue = 0;
     std::vector<int> tbl5;
     std::vector<int> tbl6;
     for (size_t i = 0; i < bitlengths.size(); i++) // in (4, 6, 8, 10, 12, 14)
	 {
         tbl5.push_back(ceilValue);
         ceilValue += (1 << bitlengths[i]);
         tbl6.push_back(ceilValue - 1);
	 }
     tbl6.push_back(ceilValue - 1);
     ceilValue += 0x3F;
     
     //# @8008870C; length of buffer at @8008E91C
	 int overallBitPosition = 0;
     //bit = getbit(data)
	 std::vector<unsigned char> buf;
	 buf.resize(ceilValue);
     for (int i = 0; i < ceilValue; i++)
		 buf[i] = 0x00;
     //# Decode loop.
     //out = bytearray()
     int cnt = 0;
     while (true)
	 {
         //# Decode the next value.
         int c = 1;
		 int v = 0;
         while (c < 0x275)
		 {
			 v = getbit(data, overallBitPosition);
			 if (v)
				 c = tbl2[c];
			 else
				 c = tbl1[c];
		 }
         //# Update model.
         tbl4[c] += 1;
         int i = tbl3[c];
         if (i != 1)
		 {
             v = tbl1[i];
             if (v == c)
                 v = tbl2[i];
             update(tbl1, tbl2, tbl3, tbl4, c, v);
             int x = c;
             while (i != 1)
			 {
                 int j = tbl3[i];
                 int k = tbl1[j];
                 if (i == k)
                     k = tbl2[j];
                 if (tbl4[k] < tbl4[x])
				 {
                     if (tbl1[j] == i)
                         tbl2[j] = x;
                     else
                         tbl1[j] = x;
                     if (tbl1[i] == x)
					 {
                         tbl1[i] = k;
                         v = tbl2[i];
					 }
                     else
					 {
                         tbl2[i] = k;
                         v = tbl1[i];
					 }
                     tbl3[k] = i;
                     tbl3[x] = j;
                     update(tbl1, tbl2, tbl3, tbl4, k, v);
                     x = k;
				 }
                 x = tbl3[x];
                 i = tbl3[x];
			 }
		 }
         //# Handle the value!
         c -= 0x275;
         if (c == 256)
             return outputPosition;
         else if (c < 256)
		 {
             //# Literal
             output[outputPosition++] = c;
             buf[cnt] = c;
             cnt += 1;
             if (cnt == ceilValue)
                 cnt = 0;
		 }
         else
		 {
             //# Copy.  (v - 32v/62) / 32
             v = c - 0x101;
             i = v / 62;
//##            i = (-2078209981 * v) >> 32
//##            i += v
//##            i >>= 5
             //# Fetch distance from next *n getbit.
             v = 0;
             for (int j = 0; j < bitlengths[i]; j++)
			 {
                 if (getbit(data, overallBitPosition))
                     v |= (1 << j);
			 }
             int j = c - (i * 62) - 0xFE;
             v += tbl5[i];
             v += j;
             int p = cnt - v;
             if (p < 0)
                 p += ceilValue;
             //# Do this one byte at a time to handle wrapping around the ring.
             for (int i = 0; i < j; i++)
			 {
                 v = buf[p];
                 output[outputPosition++] = v;
                 buf[cnt] = v;
                 cnt += 1;
                 p += 1;
                 if (cnt == ceilValue)
					 cnt = 0;
                 if (p == ceilValue)
					 p = 0;
			 }
		 }
	 }

	return outputPosition;
}