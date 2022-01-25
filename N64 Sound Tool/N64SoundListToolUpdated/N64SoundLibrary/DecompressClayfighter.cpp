#include "StdAfx.h"
#include "DecompressClayfighter.h"
#include "SharedFunctions.h"

CDecompressClayfighter::CDecompressClayfighter(void)
{
}

CDecompressClayfighter::~CDecompressClayfighter(void)
{
}

int CDecompressClayfighter::Decompress(byte *output, byte *input)
{
  bool bVar1;
  byte bVar2;
  byte bVar3;
  byte *pbVar4;
  byte *pbVar5;
  
  pbVar5 = output;
  while( true ) {
    while( true ) {
      bVar3 = *input;
      pbVar4 = input + 1;
      if ((bVar3 & 0x80) != 0) break;
      do {
        *pbVar5 = *pbVar4;
        pbVar4 = pbVar4 + 1;
        pbVar5 = pbVar5 + 1;
        bVar1 = bVar3 != 0;
        bVar3 = bVar3 - 1;
        input = pbVar4;
      } while (bVar1);
    }
    bVar2 = bVar3 & 0x7f;
    if (bVar3 == 0xff) break;
    bVar3 = *pbVar4;
    input = input + 2;
    do {
      *pbVar5 = bVar3;
      pbVar5 = pbVar5 + 1;
      bVar1 = bVar2 != 0;
      bVar2 = bVar2 - 1;
    } while (bVar1);
  }
  return (int)pbVar5 - (int)output;
}