#include "stdafx.h"
#include "YAZ0.h"

#define U32(x)  ((unsigned long)( (((unsigned char*)(x))[0]<<24)|(((unsigned char*)(x))[1]<<16)|(((unsigned char*)(x))[2]<<8)|((unsigned char*)(x))[3] ))
#define U16(x)  ((unsigned short)( ((*((unsigned char*)(x)))<<8)|((unsigned char*)(x))[1] ))
#define U8(x)	((unsigned char)((unsigned char*)(x))[0])
#define S32(x)  ((signed long)(U32(x)))
#define S16(x)  ((signed short)(U16(x)))
#define F32(x)  ( ( (____unsigned long_f32_union____.u = U32(x)) & 0) + ____unsigned long_f32_union____.f)

#define SU32(addr, val)\
{\
	*((addr)+0) = (val & 0xFF000000) >> 24;\
	*((addr)+1) = (val & 0x00FF0000) >> 16;\
	*((addr)+2) = (val & 0x0000FF00) >> 8;\
	*((addr)+3) = (val & 0x000000FF);\
}

/* internal declarations */
unsigned long simpleEnc(unsigned char* src, int size, int pos, unsigned long *pMatchPos);
unsigned long nintendoEnc(unsigned char* src, int size, int pos, unsigned long *pMatchPos);
int yaz0_encode_internal(unsigned char* src, int srcSize, unsigned char * Data);
unsigned long yaz0_decode_internal (unsigned char *src, unsigned char *dst, int uncompressedSize);

int YAZ0::yaz0_get_size (unsigned char * src)
{
    return U32(src + 0x4);
}

unsigned long YAZ0::yaz0_decode (unsigned char *src, unsigned char *dst, int& fileSizeCompressed)
{
    int siz = yaz0_get_size( src );
    return yaz0_decode_internal(src + 0x10, dst, siz );
}

unsigned long
yaz0_decode_internal (unsigned char *src, unsigned char *dst, int uncompressedSize)
{
  int srcPlace = 0, dstPlace = 0; /*current read/write positions*/
  
  unsigned long validBitCount = 0; /*number of valid bits left in "code" byte*/
  unsigned char currCodeByte = 0;
  
  while(dstPlace < uncompressedSize)
  {
    /*read new "code" byte if the current one is used up*/
    if(!validBitCount)
    {
      currCodeByte = src[srcPlace];
      ++srcPlace;
      validBitCount = 8;
    }
    
    if(currCodeByte & 0x80)
    {
      /*straight copy*/
      dst[dstPlace] = src[srcPlace];
      dstPlace++;
      srcPlace++;
    }
    else
    {
      /*RLE part*/
      unsigned char byte1 = src[srcPlace];
      unsigned char byte2 = src[srcPlace + 1];
      srcPlace += 2;
      
      unsigned long dist = ((byte1 & 0xF) << 8) | byte2;
      unsigned long copySource = dstPlace - (dist + 1);

      unsigned long numBytes = byte1 >> 4;
      if(numBytes)
        numBytes += 2;
      else
      {
        numBytes = src[srcPlace] + 0x12;
        srcPlace++;
      }

      /*copy run*/
      int i;
      for(i = 0; i < numBytes; ++i)
      {
        dst[dstPlace] = dst[copySource];
        copySource++;
        dstPlace++;
      }
    }
    
    /*use next bit from "code" byte*/
    currCodeByte <<= 1;
    validBitCount-=1;    
  }

  return dstPlace;
}

unsigned long toDWORD(unsigned long d)
{
  unsigned char w1 = d & 0xFF;
  unsigned char w2 = (d >> 8) & 0xFF;
  unsigned char w3 = (d >> 16) & 0xFF;
  unsigned char w4 = d >> 24;
  return (w1 << 24) | (w2 << 16) | (w3 << 8) | w4;
}

// simple and straight encoding scheme for Yaz0
unsigned long simpleEnc(unsigned char* src, int size, int pos, unsigned long *pMatchPos)
{
  int startPos = pos - 0x1000, j, i;
  unsigned long numBytes = 1;
  unsigned long matchPos = 0;

  if (startPos < 0)
    startPos = 0;
  for (i = startPos; i < pos; i++)
  {
    for (j = 0; j < size-pos; j++)
    {
      if (src[i+j] != src[j+pos])
        break;
    }
    if (j > numBytes)
    {
      numBytes = j;
      matchPos = i;
    }
  }
  *pMatchPos = matchPos;
  if (numBytes == 2)
    numBytes = 1;
  return numBytes;
}



// a lookahead encoding scheme for ngc Yaz0
unsigned long nintendoEnc(unsigned char* src, int size, int pos, unsigned long *pMatchPos)
{
  unsigned long numBytes = 1;
  static unsigned long numBytes1;
  static unsigned long matchPos;
  static int prevFlag = 0;

  // if prevFlag is set, it means that the previous position was determined by look-ahead try.
  // so just use it. this is not the best optimization, but nintendo's choice for speed.
  if (prevFlag == 1) {
    *pMatchPos = matchPos;
    prevFlag = 0;
    return numBytes1;
  }
  prevFlag = 0;
  numBytes = simpleEnc(src, size, pos, &matchPos);
  *pMatchPos = matchPos;

  // if this position is RLE encoded, then compare to copying 1 byte and next position(pos+1) encoding
  if (numBytes >= 3) {
    numBytes1 = simpleEnc(src, size, pos+1, &matchPos);
    // if the next position encoding is +2 longer than current position, choose it.
    // this does not guarantee the best optimization, but fairly good optimization with speed.
    if (numBytes1 >= numBytes+2) {
      numBytes = 1;
      prevFlag = 1;
    }
  }
  return numBytes;
}

int yaz0_encode_internal(unsigned char* src, int srcSize, unsigned char * Data)
{
  unsigned char dst[24];    // 8 codes * 3 bytes maximum
  int dstSize = 0;
  int i;
  int pos=0;
  int srcPos=0, dstPos=0;
  
  unsigned long validBitCount = 0; //number of valid bits left in "code" byte
  unsigned char currCodeByte = 0;
  while(srcPos < srcSize)
  {
    unsigned long numBytes;
    unsigned long matchPos;
    unsigned long srcPosBak;

    numBytes = nintendoEnc(src, srcSize, srcPos, &matchPos);
    if (numBytes < 3)
    {
      //straight copy
      dst[dstPos] = src[srcPos];
      dstPos++;
      srcPos++;
      //set flag for straight copy
      currCodeByte |= (0x80 >> validBitCount);
    }
    else 
    {
      //RLE part
      unsigned long dist = srcPos - matchPos - 1; 
      unsigned char byte1, byte2, byte3;

      if (numBytes >= 0x12)  // 3 byte encoding
      {
        byte1 = 0 | (dist >> 8);
        byte2 = dist & 0xff;
        dst[dstPos++] = byte1;
        dst[dstPos++] = byte2;
        // maximum runlength for 3 byte encoding
        if (numBytes > 0xff+0x12)
          numBytes = 0xff+0x12;
        byte3 = numBytes - 0x12;
        dst[dstPos++] = byte3;
      } 
      else  // 2 byte encoding
      {
        byte1 = ((numBytes - 2) << 4) | (dist >> 8);
        byte2 = dist & 0xff;
        dst[dstPos++] = byte1;
        dst[dstPos++] = byte2;
      }
      srcPos += numBytes;
    }
    validBitCount++;
    //write eight codes
    if(validBitCount == 8)
    {
      Data[pos] = currCodeByte;
      pos++;
      for (i=0;i</*=*/dstPos;pos++,i++)
        Data[pos] = dst[i];
      dstSize += dstPos+1;

      srcPosBak = srcPos;
      currCodeByte = 0;
      validBitCount = 0;
      dstPos = 0;
    }
  }
  if(validBitCount > 0)
  {
    Data[pos] = currCodeByte;
    pos++;
    for (i=0;i</*=*/dstPos;pos++,i++)
      Data[pos] = dst[i];
    dstSize += dstPos+1;

    currCodeByte = 0;
    validBitCount = 0;
    dstPos = 0;
  }
    
  return dstSize;
}

void YAZ0::yaz0_encode(unsigned char * src, int src_size, unsigned char *dst, int *dst_size )
{
  //check for minimum size
  if(*dst_size < src_size + 0x20)
  {
      *dst_size = -1;
      return;
  }
  
  // write 4 bytes yaz0 header
  memcpy(dst, "Yaz0", 4);
  
  // write 4 bytes uncompressed size
  SU32(dst + 4, src_size);
  
  // write 8 bytes unused dummy 
  memset(dst + 8, 0, 8);
  
  //encode
  *dst_size = yaz0_encode_internal(src, src_size, dst + 16);
}
