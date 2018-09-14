/*
    chksum64 V1.2, a program to calculate the ROM checksum of Nintendo64 ROMs.
    Copyright (C) 1997  Andreas Sterbenz (stan@sbox.tu-graz.ac.at)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
*/

#include "StdAfx.h"
#include ".\chksum64.h"
#include <stdio.h>
#include <string.h>
#include "chksum64.h"
#include <stdlib.h>

CheckSumN64::CheckSumN64(void)
{
}

CheckSumN64::~CheckSumN64(void)
{
}




#define max2(a, b) ( (a)>(b) ? (a) : (b) )
#define min2(a, b) ( (a)<(b) ? (a) : (b) )

#define BUFSIZE 32768

#define CHECKSUM_START 0x1000
#define CHECKSUM_LENGTH 0x100000L
#define CHECKSUM_HEADERPOS 0x10
#define CHECKSUM_END (CHECKSUM_START + CHECKSUM_LENGTH)

#define CHECKSUM_STARTVALUE 0xf8ca4ddc

#define ROL(i, b) (((i)<<(b)) | ((i)>>(32-(b))))

#define BYTES2LONG(b, s) ( (((b)[0^(s)] & 0xffL) << 24) | \
                           (((b)[1^(s)] & 0xffL) << 16) | \
                           (((b)[2^(s)] & 0xffL) <<  8) | \
                           (((b)[3^(s)] & 0xffL)) )

#define LONG2BYTES(l, b, s)  (b)[0^(s)] = ((l)>>24)&0xff; \
                             (b)[1^(s)] = ((l)>>16)&0xff; \
                             (b)[2^(s)] = ((l)>> 8)&0xff; \
                             (b)[3^(s)] = ((l)    )&0xff;

#define HEADER_MAGIC 0x80371240

int CheckSumN64::ConvertROM(CString inputFileName)
{

	FILE* tempIn = fopen(inputFileName, "rb");
	if (tempIn == NULL)
		return false;

	fseek(tempIn, 0, SEEK_END);
	unsigned long inputFileTotalSize = ftell(tempIn);
	fclose(tempIn);



  FILE *file1;
  char *fname1=NULL;
  unsigned char buffer1[BUFSIZE];
  long flen1;
  unsigned long sum1, sum2;
  int readonly = 0;
 int swapped = -1;

	fname1 = inputFileName.GetBuffer();

  if( fname1 == NULL ) return 0;

  file1 = fopen(fname1, "rb");

  fread(buffer1, 1, 12, file1);

  if( swapped == -1 ) {
    if( BYTES2LONG(buffer1, 0) == HEADER_MAGIC ) {
      swapped = 0;
    } else if( BYTES2LONG(buffer1, 1) == HEADER_MAGIC ) {
      swapped = 1;
    }
  }
  
  
  fseek(file1, 0L, SEEK_END);
  flen1 = ftell(file1);

  fseek(file1, CHECKSUM_START, SEEK_SET);
  {
    unsigned long i;
    unsigned long c1, k1, k2;
    unsigned long t1, t2, t3, t4;
    unsigned long t5, t6;
    unsigned int n;
    long clen = CHECKSUM_LENGTH;
    long rlen = flen1 - CHECKSUM_START;

    /* Below is the actual checksum calculation algorithm, which was
       reverse engineered out of Nagra's program.

       As you can see, the algorithm is total crap. Obviously it was
       designed to be difficult to guess and reverse engineer, and not
       to give a good checksum. A simple XOR and ROL 13 would give a
       just as good checksum. The ifs and the data dependent ROL are really
       extreme nonsense.
    */

    t1 = CHECKSUM_STARTVALUE;
    t2 = CHECKSUM_STARTVALUE;
    t3 = CHECKSUM_STARTVALUE;
    t4 = CHECKSUM_STARTVALUE;
    t5 = CHECKSUM_STARTVALUE;
    t6 = CHECKSUM_STARTVALUE;

    for( ;; ) {
      if( rlen > 0 ) {
        n = fread(buffer1, 1, min2(BUFSIZE, clen), file1);
        if( (n & 0x03) != 0 ) {
          n += fread(buffer1+n, 1, 4-(n&3), file1);
        }
      } else {
        n = min2(BUFSIZE, clen);
      }
      if( (n == 0) || ((n&3) != 0) ) {
        if( (clen != 0) || (n != 0) ) {
          //fprintf(stderr, "WARNING: Short read, checksum may be incorrect.\n");
        }
        break;
      }
      for( i=0; i<n; i+=4 ) {
        c1 = BYTES2LONG(&buffer1[i], swapped);
        k1 = t6 + c1;
        if( k1 < t6 ) t4++;
        t6 = k1;
        t3 ^= c1;
        k2 = c1 & 0x1f;
        k1 = ROL(c1, k2);
        t5 += k1;
        if( c1 < t2 ) {
          t2 ^= k1;
        } else {
          t2 ^= t6 ^ c1;
        }
        t1 += c1 ^ t5;
      }
      if( rlen > 0 ) {
        rlen -= n;
        if( rlen <= 0 ) memset(buffer1, 0, BUFSIZE);
      }
      clen -= n;
    }
    sum1 = t6 ^ t4 ^ t3;
    sum2 = t5 ^ t2 ^ t1;
  }
  LONG2BYTES(sum1, &buffer1[0], 0);
  LONG2BYTES(sum2, &buffer1[4], 0);
  fseek(file1, CHECKSUM_HEADERPOS, SEEK_SET);
  fread(buffer1+8, 1, 8, file1);
  /*printf("Old Checksum:        ");
  printf("%02X %02X %02X %02X  ", buffer1[ 8^swapped], buffer1[ 9^swapped], buffer1[10^swapped], buffer1[11^swapped]);
  printf("%02X %02X %02X %02X\n", buffer1[12^swapped], buffer1[13^swapped], buffer1[14^swapped], buffer1[15^swapped]);
  printf("Calculated Checksum: ");
  printf("%02X %02X %02X %02X  ", buffer1[0], buffer1[1], buffer1[2], buffer1[3]);
  printf("%02X %02X %02X %02X\n", buffer1[4], buffer1[5], buffer1[6], buffer1[7]);*/
  if( readonly == 0 ) {
	fseek(file1, CHECKSUM_HEADERPOS, SEEK_SET);
	LONG2BYTES(sum1, &buffer1[16], swapped);
	LONG2BYTES(sum2, &buffer1[20], swapped);

	unsigned char* GEROM;
	GEROM = new unsigned char[inputFileTotalSize];
	fseek(file1, 0, SEEK_SET);
	fread(GEROM, 1, inputFileTotalSize, file1);
	fclose(file1);

	file1 = fopen(inputFileName, "wb");

	fwrite(GEROM, 1, 0x10, file1);
	fwrite(buffer1+0x10, 1, 8, file1);
	
	fwrite(GEROM+0x18, 1, (inputFileTotalSize - 0x18), file1);

	fflush(file1);
	fclose(file1);

	delete [] GEROM;

	return 1;
  }
 }
