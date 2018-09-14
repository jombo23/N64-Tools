#include "StdAfx.h"
#include "ODTCompress.h"

CODTCompress::CODTCompress(void)
{
}

CODTCompress::~CODTCompress(void)
{
}

/*****************************************
weird string check function
entirely because nothing else was working
*****************************************/
unsigned long chewyodt(unsigned char *search,unsigned long searchx,unsigned char *table,unsigned long tablex)
{unsigned long x,locate;
unsigned char compare[18];

for(locate=0;locate<tablex;locate++)
    {for(x=0;x<searchx;x++) compare[x]=table[x+locate];
    

    for(x=0;x<searchx;x++)
        {
        if(search[x]!=compare[x]) break;
        }
    if(x==searchx) return locate;
    }    
return 0x8000;}

#define PAGE_OFFSET 0x00000FEE   /*this is the size the compression dohick is padded to*/
/*#define DEBUGGERY           /*saves swap file for debugging*/

/*standard 16bit byteswapping*/
unsigned short int shortswapodt(unsigned short int w)
{return (((w >> 8) & 0xFF) | ((w << 8) & 0xFF00));
}

/****************************************/
/*get the size of a file
it happens often enough it might as well be a function
plus, it won't mess with CUR_POS*/
/****************************************/
unsigned long filesizeodt(FILE *in)
{unsigned long cur,end;
cur=ftell(in);
fseek(in,0,SEEK_END);
end=ftell(in);
fseek(in,cur,SEEK_SET);
return end;}


/**pulls couplets and handles aliasing**/
int Suck(FILE *olde,FILE *swap){
unsigned int addy,y;
unsigned long range;
unsigned char buffer;
 /*character aliasing*/
 /*
 Memory is aliased in Ya-0 derivative compressions
 When the lead bit = 0, it reads in two characters which determine the address and # characters copied
 The alias follows the format aa #A, where Aaa is the address offset and #+3 characters read
 These are copied one byte at a time, so replicating the last written character is possible
 The address is also incremented in 0x1000's using the current file position
 If the combined address value is larger than the current filesizeodt, though, it will subtract the value instead
 
 In this implementation, the file is decompressed into a buffer, so the actual file starts at 0xFEE
 */
 addy=fgetc(olde);
 range=fgetc(olde);
 addy=addy+((range/0x10)*0x100);
 range&=0xF;
 range+=3;
 addy+=(ftell(swap)&0xFFFFF000);
 if(addy>ftell(swap)) addy-=0x1000;
 
 /*read in one at a time (supports duplication this way)*/
 for(y=0;y<range;y++)
  {fseek(swap,addy+y,SEEK_SET);
     buffer = fgetc(swap);
     fseek(swap,0,SEEK_END);
     fputc(buffer,swap);
  }    
    return 0;}





/*************************************
basic idea behind the compression scheme
(note: this is slow and crappy)
There are three buffers:
    search string, null terminated
    alias buffer -which is really just a subset of the output- 
        allocated and null terminated
    packet of data to be output in file, containing:
        bitflag byte stating which are alias bytes
        the bytes themselves, either 1-byte literals or 2-byte aliases (8-16 total)

strstr() is used to search the alias buffer for the search string.
the search string is initially 18 bytes long (3bytes + 15 max),
 and the alias buffer also contains the string-1 char in the case of "replication".
If a hit is not found, both buffers are shortened with NULLs and retested.
 If it is not found with a 3-byte search string,
 the single byte is stored and the corresponding bitflag toggled
  format in output will look like: aa #A, where Aaa is an address, #+3=bytes
  so, let that intel processor byteswap for you by using an int!

In the case of a hit, first the series is retested ot get the last possible hit.
 This provides compression 100% equivalent to the original source.
 The position to the hit is determined and
 added to the alias file offset to determine it's file location.
That value is masked to produce a three-nibble offset.
The bytes are swapped, then the length of the search string -3
 is stored in the last nibble.

As you can tell, the more redundancy within the file, the faster it compresses.
Pretty much any halfwit can come up with a better scheme & implementation.
I'm a hacker, not a coder...

oh yeah...
returns 1 if an error, or 0 if all is well.
*source is your input file (tempy, already extended by 0xFEE)
*out is your outut

source_init is original position in source
out_init is original position in out
index is the current position-0x1000
current is the byte sequence you are testing (in CR starts at 0xFEE)
size is the size of the block during allocation

*alias points to the allocated block of test values
search is your search string, up to 18 char long + NULL
bitflag stores your nifty bitflag data
    bitflag is 0 if a couplet, 1 if a normal byte
    shift left after each iteration (lead = lead << 1;)
*hit and *nexthit point to the results of strstr

values[] stores the eight values
*************************************/

unsigned long compress(FILE *source, FILE *out, unsigned long current,unsigned long source_size)
{unsigned long searchx,size,hit,y;
unsigned char *alias, search[18], bitflag,value[16];
unsigned short x,valuex;

if(!(alias = (unsigned char *) malloc(0x1012)))
    {fprintf(stderr,"\nhorrible memory allocation problem\naborting....");
     system("PAUSE");
     exit(1);
     }    


/*lupus*/
while(current<filesizeodt(source))
{
    
valuex=0;
bitflag=0;  /*reset flag.  add 1 if set, then advance with leftshift*/
for(x=0;x<8;x++)    /*do this 8 times - one for each value*/
    {bitflag = bitflag >> 1; /*advance to next flag*/
    /*grab search string.
    it should return # actually read, so use that to determine allocation size*/
    fseek(source,current,SEEK_SET);
    if((searchx=fread(search,1,18,source))>0)/*if done reading, loop until finished*/
    {
     /*set size for allocated block 0xFFF + #read.  if current<0x1000, use current.
     realloc with a NULL pointer should act like malloc*/
     if(current<0x1000) size=current;
     else size=0xFFF;
     fseek(source,current-size,SEEK_SET);
     if(!((unsigned char *) realloc(alias,size+searchx-1)))
          {fprintf(stderr,"\nhorrible memory allocation problem\naborting....");
          system("PAUSE");
          exit(1);
          }    
     /*copy data into block*/
     fread(alias,1,size+searchx-1,source);
     
     /*actual search: repeat until a hit found or string is 2char*/
     while(searchx>2)
          {if((hit=chewyodt(search,searchx,alias,size))<0x8000) break;
          /*otherwise, try a smaller string*/
          searchx--;
          }
     /*new search method, should be less stupid*/
          
     if(searchx>2)   /*if string found though, get latest possible string*/
          {/*while (strstr(hit,search)!=NULL) hit=strstr(hit,search);*/
          /*don't need *size* anymore this run - set as file offset*/
          size=current-size+hit;
          value[valuex]= (size & 0xFF);
          size=size >> 4;
          value[valuex+1]= (char)((size & 0xF0) + (searchx-3));
          current+=searchx;
          valuex+=2;
          }
     else  {bitflag+= 0x80;          /*if hit NULL (never found)*/
          fseek(source,current,SEEK_SET);
          value[valuex]=fgetc(source);
          current++;
          valuex++;
          }     
     }   /*end of values left>0 check*/
   }/*end for(valuex=0;valuex<8;valuex++)*/

/*now that the whole value is assembled, write to out*/
fputc(bitflag,out);
for(x=0;x<valuex;x++) fputc(value[x],out);

}   /*lupus*/

/*Roswell that ends well*/
free(alias);
return current;}   

/**basic ripping**/
int Rend(FILE *olde,FILE *newe,int offset,unsigned long pos,unsigned long csize,unsigned long dsize)
{
unsigned long x,addies[3];
unsigned char lead, trick;
FILE *swap;

  swap=fopen("tempADSASD.bin", "wb+");
   if (swap == NULL)
	   return 0;

/*add 0xFEE bytes to -swap-*/
for(x=0;x<PAGE_OFFSET;x++) fputc(0,swap);

fseek(olde,(offset + pos),SEEK_SET);

int inputReadSpot = ftell(olde);
while(ftell(olde)<(offset+csize))
    {/*test a premature EOF*/
     if(x>dsize)
        {
			fclose(swap);
			::DeleteFile("tempADSASD.bin");
			return 0;
         }
    /*decompression*/
    lead=fgetc(olde);
    for(x=0;x<8;x++)
            {if(lead & 1)
               {trick = fgetc(olde);
               fputc(trick,swap);}
            else Suck(olde,swap);
            lead = lead >> 1;
            }
      fseek(swap,0,SEEK_END);
      x=ftell(swap)-PAGE_OFFSET;
	  inputReadSpot = ftell(olde);
    }    

/*copy and truncate file from -swap- to -newe-*/
fseek(swap,PAGE_OFFSET,SEEK_SET);
for(x=0;x<dsize;x++) fputc(fgetc(swap),newe);

fclose(swap);
::DeleteFile("tempADSASD.bin");
return dsize;
}

int CODTCompress::decode(FILE * olde, int offset, CString filename, int& fileSizeCompressed)
{
unsigned short x,y;
FILE *newe;

 
	newe = fopen(filename,"wb+");
  
  fread(&y,2,1,olde);
  fread(&x,2,1,olde);
  x=shortswapodt(x);
  y=shortswapodt(y);
  fileSizeCompressed = (x * 8) + 8;
  x=Rend(olde,newe,offset,8,(unsigned long)x*8,(unsigned long)y*8);
  fclose(newe);
  return x;
}

bool CODTCompress::encode(CString inFileName, CString outFileName)
{
char filename[130],y;
FILE *in, *out, *tempy;
unsigned long x;
unsigned short z;

	in = fopen(inFileName, "rb");
	if (in == NULL)
		return false;

/*well, had to add 0xFEE 00's anyway
make a temp file, and copy the original to it
this will be the main sample to work with*/
	tempy=fopen(inFileName + "tempaSDASD.bin", "wb+");
    if (tempy == NULL)
	{
		fclose(in);
		return false;
	}
/*in theory, filesizeodt() doesn't tamper with file position*/
for(x=0;x<0xFEE;x++) fputc(0,tempy);
for(x=0;x<filesizeodt(in);x++) fputc(fgetc(in),tempy);
fclose(in);

	out = fopen(outFileName, "wb+");
	if (out == NULL)
	{
		fclose(tempy);
		return false;
		}
	/*the first two pointers get overwritten later, so just send something that size*/
	z=x/8;
	z=shortswapodt(z);
	fwrite(&z,2,1,out);
	x=0;
	fwrite(&x,2,1,out);
	fwrite(&x,4,1,out); /*out @ 0x8*/

	printf("\nCompressing.\nThis will take a while due to some crappy code...\n");
	/*now to actually do some compression*/
	compress(tempy,out,0xfee,x);

  

/*mop up
here the offsets are added to the output file
at +4, set the size of data, which is the filesizeodt -0xC (data length)
then, extend to word and set the total file size to 0x0
this is included here as the compression is also used with different formats*/
z=filesizeodt(out)/8;
z=shortswapodt(z);
fseek(out,2,SEEK_SET);
fwrite(&z,2,1,out);

/*word extend*/
fseek(out,-1,SEEK_END);
y=fgetc(out);
fseek(out,0,SEEK_END);
while(filesizeodt(out)%4) fputc(y,out);


  fclose(out);
  fclose(tempy);
  return true;
}