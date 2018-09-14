#include "StdAfx.h"
#include "EDLCompress.h"

/*#define DATATABLE_32BIT       /*some older systems just don't support long long*/
/*#define DEBUGGERY             /*debug: allocated what[]*/
#define INTEL       /*assumes your PC uses INTEL formatting*/

/*I don't feel like passing these flags around
  so, now its a global variable.  Seriously, it's not that big a deal*/
struct bit_field{
       unsigned int swap      :1;
       unsigned int message   :1;
       unsigned int table     :1;
       unsigned int quiet     :1;
       unsigned int list      :1;
       unsigned int trash     :1;
       } flagrant;

typedef struct {
       unsigned long csize;  /*compressed size*/
       unsigned long dsize;  /*decompressed size*/
       unsigned char type;    /*compression type 0-2*/
       unsigned char endian;  /*big(1) or little(0) endian*/
       } EDL;

/*standard 32bit byteswapping*/
unsigned long byteswap(unsigned long w)
{return (w >> 24) | ((w >> 8) & 0x0000ff00) | ((w << 8) & 0x00ff0000) | (w << 24);
}    

/*standard 16bit byteswapping*/
unsigned short int shortswap(unsigned short int w)
{return (w >> 8) | (w << 8);}

/*flip nibble - optional*/
unsigned char nibswap(unsigned char w)
{return (w>>4) | (w<<4);}

unsigned long swapfile(FILE *in,FILE *out)
{unsigned long x,y;
unsigned char z;

fseek(in,0,SEEK_END);
x=ftell(in);

for(y=0;y<x;y++)
   {fseek(in,y,SEEK_SET);
   z=fgetc(in);
   z=nibswap(z);
   fseek(out,y,SEEK_SET);
   fputc(z,out);
   }

return ftell(out);}

/*error messages*/
int erratta(long code)
{switch(code)
       {case -8: fprintf(stderr,"\a\nNot a valid table entry"); break;
        case -9: fprintf(stderr,"\a\nSamples exceed maximum bitcount"); break;
        default: fprintf(stderr,"\a\nUnknown error %i",code); code=0;
        }
return code;}

/*returns FILE offset to next EDL file, or -1 if not found*/
unsigned long EDLscan(FILE *in, unsigned long pos)
{unsigned long end;

fseek(in,0,SEEK_END);
end=ftell(in);
rewind(in);

while(pos<end)
  {fseek(in,pos,SEEK_SET);
  if(fgetc(in)=='E')
    {if(fgetc(in)=='D')
       {if(fgetc(in)=='L') return pos;
       }
    }
  pos++;
  }

return end;}

/*Fills an EDL struct with candy.  White hot candy*/
int FillEDL(EDL *data, FILE *in, unsigned long pos)
{unsigned long x=0;
    
fseek(in,pos+3,SEEK_SET);
x=fgetc(in);
data->type=x&0xF;
data->endian=x>>7;

fread(&x,4,1,in);
if(data->endian) x=byteswap(x);
data->csize=x;
fread(&x,4,1,in);
if(data->endian) x=byteswap(x);
data->dsize=x;

return 0;}

/*These are the three known decompression routines as stripped from TWINE (N64)
  as a note, the endian value is not used,
  since I can't confirm that it is used outside of the header size values
  Each returns the size of the decompressed file, which can be tested against expected*/
/*store*/
unsigned long EDLdec0(unsigned long pos,FILE *in,FILE *out,EDL edl)
{edl.csize+=pos;   /*end of file*/

for(pos+=12;pos<edl.csize;pos++)
   {fseek(in,pos,SEEK_SET);
   fputc(fgetc(in),out);
   if(feof(in)) break;
   if(ftell(out)>edl.dsize) break;
   }
return ftell(out);}

/*this handles the mess of dealing with the 64bit data word,its count, etc
  granted, it doesn't handle it very well, but it does handle it...
  It is nice enough not to devastate anything if you call it when unnecessary*/

/*Older compilers and systems may not support a 64bit datatable
  so, a 32bit version of this table is provided here
  the 64bit one works much faster (fewer table expansions, less file access, etc)
  Note, the definition for the function is different*/
#ifdef DATATABLE_32BIT
unsigned long helper(unsigned long *data,unsigned long bitcount,FILE *in,unsigned long *pos,unsigned long max,int endian)
{unsigned long x,y,z;

if(bitcount>16) return bitcount;   /*essentially, do nothing!*/
z=*data;
y=0;
x=max-*pos;
if(x>2) x=2;   /*#bytes to retrieve from file*/
fseek(in,*pos,SEEK_SET);
fread(&y,x,1,in);
if(endian) y=shortswap(y);
*pos+=x;

*data=y;       /*tack old data on the end of new data for a continuous bitstream*/
*data=*data<<bitcount;
*data|=z;

x*=8;          /*revise bitcount with number of bits retrieved*/
return bitcount+x;}
#else
unsigned long helper(unsigned long long int *data,unsigned long bitcount,FILE *in,unsigned long *pos,unsigned long max,int endian)
{unsigned long x,y,z;

if(bitcount>32) return bitcount;   /*essentially, do nothing!*/
z=*data;
y=0;
x=max-*pos;
if(x>4) x=4;   /*#bytes to retrieve from file*/
fseek(in,*pos,SEEK_SET);
fread(&y,x,1,in);
if(endian) y=byteswap(y);
*pos+=x;

*data=y;       /*tack old data on the end of new data for a continuous bitstream*/
*data=*data<<bitcount;
*data|=z;

x*=8;          /*revise bitcount with number of bits retrieved*/
return bitcount+x;}
#endif

/*generate tables*/
int FillBuffer(unsigned short *large,unsigned char *what,long total,long num,char bufsize)
{unsigned char *buf;
unsigned short *when;
unsigned short *samp;
unsigned short numero=1;
unsigned long *number;
long x,y,z,back;

if(flagrant.table)
  {
	  char filename[20];
  unsigned short jack;
  unsigned long herb;
  FILE *debuggery;

  sprintf(filename,"what%04i.bin",numero);
  while(debuggery = fopen(filename, "rb"))
   {fclose(debuggery);
   numero++;
   sprintf(filename,"what%04i.bin",numero);
   }
  debuggery=fopen(filename,"wb");

  for(jack=0;jack<total;jack++)
   {herb=what[jack];
   herb=byteswap(herb);
   fwrite(&herb,1,4,debuggery);
   }
  fclose(debuggery);
}/*end debug table*/

       /*my implementation is stupid and alays copies the block, so this uses even more memory than it should
       if(!(what=realloc(what,num))
           {printf("\nVirtual memory exhausted.\nCan not continue.\n\tPress ENTER to quit.");
           getchar();
           return 0;
           }*/
       if(!(when=(unsigned short*)calloc(num,2)))
         {printf("\nVirtual memory exhausted.\nCan not continue.\n\tPress ENTER to quit.");
         getchar();
         return 0;
         }/*end calloc*/
       if(!(samp=(unsigned short*)calloc(num,2)))
         {printf("\nVirtual memory exhausted.\nCan not continue.\n\tPress ENTER to quit.");
		 free(when);
         getchar();
         return 0;
         }/*end calloc*/
       if(!(number=(unsigned long*)calloc(16,4)))
         {printf("\nVirtual memory exhausted.\nCan not continue.\n\tPress ENTER to quit.");
	     free(when);
		 free(samp);
         getchar();
         return 0;
         }/*end calloc*/
       memset(large,0,0xC00);         /*both buffers have 0x600 entries each*/

       /*build an occurance table*/
       back=0;  /*back will act as a counter here*/
       for(y=1;y<16;y++)/*sort occurance*/
          {for(x=0;x<total;x++)/*peek at list*/
                {if(what[x]==y)
                   {when[back]=x;
                   back++;
                   number[y]++;
                   }
                }/*end peek*/
          }/*end occurance*/

if(flagrant.table)
  {char filename[20];
  unsigned short jack;
  unsigned long herb;
  FILE *debuggery;

  sprintf(filename,"when%04i.bin",numero);
  debuggery=fopen(filename,"wb");

  for(jack=0;jack<num;jack++)
   {herb=when[jack];
   herb=byteswap(herb);
   fwrite(&herb,1,4,debuggery);
   }
  fclose(debuggery);
}

       x=0;
       for(y=1;y<16;y++)/*sort nibbles*/
          {for(z=number[y];z>0;z--) 
              {what[x]=y; x++;}
          }/*end sort*/
       free(number);

if(flagrant.table)
  {char filename[20];
  unsigned short jack;
  unsigned long herb;
  FILE *debuggery;

  sprintf(filename,"order%04i.bin",numero);
  debuggery=fopen(filename,"wb");

  for(jack=0;jack<num;jack++)
   {herb=what[jack];
   herb=byteswap(herb);
   fwrite(&herb,1,4,debuggery);
   }
  fclose(debuggery);
}
       
       /*generate bitsample table*/
       z=what[0];           /*first sample, so counting goes right*/
       back=0;              /*back will act as the increment counter*/
       for(x=0;x<num;x++)
          {y=what[x];
          if(y!=z) {z=y-z; back*=(1<<z); z=y;}
          y=(1<<y)|back;
          back++;
          do{samp[x]=samp[x]<<1;
             samp[x]+=(y&1);
             y=y>>1;
             }while(y!=1);
          }/*end bitsample table*/

if(flagrant.table)
  {char filename[20];
  unsigned short jack;
  unsigned long herb;
  FILE *debuggery;

  sprintf(filename,"samples%04i.bin",numero);
  debuggery=fopen(filename,"wb");

  for(jack=0;jack<num;jack++)
   {herb=samp[jack];
   herb=byteswap(herb);
   fwrite(&herb,1,4,debuggery);
   }
  fclose(debuggery);
}

       if(!(buf=(unsigned char*)calloc(1<<bufsize,1)))
         {printf("\nVirtual memory exhausted.\nCan not continue.\n\tPress ENTER to quit.");
         getchar();
		 free(when);
		 free(samp);
         return 0;
         }/*end calloc      80013918*/
       
       for(x=0;x<num;x++)  /*fill buffer    8001392C*/
          {back=what[x];      /*#bits in sample*/
          if(back<bufsize)         /*normal entries*/
            {y=1<<back;
             z=samp[x];       /*offset within buffer*/
            do{
              large[z]=(when[x]<<7) + what[x];
              z+=y;
              }while(!(z>>bufsize));
            }/*end normal*/
          else
            {y=(1<<bufsize)-1; /*this corrects bitmask for buffer entries*/
            z=samp[x]&y;
            buf[z]=what[x];
            }/*end copies*/
          }/*end fill*/

if(flagrant.table)
  {
	  char filename[20];
  unsigned short jack;
  unsigned long herb;
  FILE *debuggery;

  sprintf(filename,"table1%04i.bin",numero);
  debuggery=fopen(filename,"wb");

  for(herb=0;herb<(1<<bufsize);herb++)
   {jack=large[herb];
   jack=shortswap(jack);
   fwrite(&jack,1,2,debuggery);
   }
  fclose(debuggery);

  sprintf(filename,"buf%04i.bin",numero);
  debuggery=fopen(filename,"wb");

  for(jack=0;jack<(1<<bufsize);jack++)
   {herb=buf[jack];
   fwrite(&herb,1,1,debuggery);
   }
  fclose(debuggery);
}
       
       /*read coded types > bufsize    80013AA8*/
       z=0;      /*value*/
       for(x=0;!(x>>bufsize);x++)/*read buf*/
          {y=buf[x];
          if(y)
            {y-=bufsize;
		    if(y>8) 
		    {
			  free(buf);
			  return -8;
		    }
            back=(z<<7) + (y<<4);  /*value*0x80 + bits<<4*/
            large[x]=back;
            z+=(1<<y);
            }/*end if(y)*/
          }/*end buf reading*/
       free(buf);
       if(z>0x1FF) return -9;
/*
if(flagrant.table)
  {char filename[20];
  unsigned short jack;
  unsigned long herb;
  FILE *debuggery;

  sprintf(filename,"table2%04i.bin",numero);
  debuggery=fopen(filename,"wb");

  for(herb=0;herb<(1<<bufsize);herb++)
   {jack=large[herb];
   jack=shortswap(jack);
   fwrite(&jack,1,2,debuggery);
   }
  fclose(debuggery);
}
*/
       /*do something tricky with the special entries    80013B3C*/
       back=1<<bufsize;
       for(x=0;x<num;x++)
          {if(what[x]<bufsize) continue;
          z=samp[x] & (back-1);
          z=large[z];     /*in dASM, this is labelled 'short'*/
          y=samp[x]>>bufsize;
          /*80013BEC*/
          do{large[y+(z>>7)+(1<<bufsize)]=(when[x]<<7)+what[x];
             y=y+(1<<(what[x]-bufsize));
             }while((y>>((z>>4)&7))==0);
          }
       free(when);
       free(samp);

if(flagrant.table)
  {char filename[20];
  unsigned short jack;
  unsigned long herb;
  FILE *debuggery;
  
  sprintf(filename,"table3%04i.bin",numero);
  debuggery=fopen(filename,"wb");

  for(herb=0;herb<((1<<bufsize)+0x200);herb++)
   {jack=large[herb];
   jack=shortswap(jack);
   fwrite(&jack,1,2,debuggery);
   }
  fclose(debuggery);
}
       
return 0;}


/*cool bitwise table type*/
unsigned long EDLdec1(unsigned long pos,FILE *in,FILE *out,EDL edl)
{
unsigned char bits[9];  /*what=p->list of slots*/
long x,y,z,stack=0;
unsigned long count=0;
unsigned long num;
unsigned long back;  /*count=#bits in register, num=#to copy, back=#to backtrack*/
unsigned short smallArray[0x600];
unsigned short large[0x600];   /*when=p->occurance in list*/
unsigned char  table1[]={0,1,2,3,4,5,6,7,8,0xA,0xC,0xE,0x10,0x14,0x18,0x1C,0x20,0x28,0x30,0x38,0x40,0x50,0x60,0x70,0x80,0xA0,0xC0,0xE0,0xFF,0,0,0};
unsigned char  table2[]={0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0,0,0,0};
unsigned short table3[]={0,1,2,3,4,6,8,0xC,0x10,0x18,0x20,0x30,0x40,0x60,0x80,0xC0,0x100,0x180,0x200,0x300,0x400,0x600,0x800,0xC00,0x1000,0x1800,0x2000,0x3000,0x4000,0x6000};
unsigned char  table4[]={0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,0xA,0xA,0xB,0xB,0xC,0xC,0xD,0xD,0,0};
#ifdef DEBUGGERY
unsigned char *what;
#else
unsigned char what[0x400];
#endif
#ifdef DATATABLE_32BIT
unsigned long data=0;
#else
unsigned long long data=0;       /*64bit datatable container*/
#endif

edl.csize+=pos;

for(pos+=12;pos<=edl.csize;back=0)
   {memset(bits,0,8); /*clear bits between... stuff*/
   count=helper(&data,count,in,&pos,edl.csize,edl.endian);
   x=data&1; data=data>>1; count--;
   
   if(x)     /*mode 1 - tables*/
     {count=helper(&data,count,in,&pos,edl.csize,edl.endian);/*build large table*/
     x=data&0x1FF; data=data>>9; count-=9;
     if(flagrant.message) printf("\nmode1\tpos: %X\tout: %X\tdata: %X",pos,ftell(out),data);
     if(x) /*construct tables*/
       {
#ifdef DEBUGGERY
       if(!(what=calloc(x,1)))
         {printf("\nVirtual memory exhausted.\nCan not continue.\n\tPress ENTER to quit.");
         getchar();
         return 0;
         }/*end calloc*/
#else
       memset(what,0,0x400);
#endif
       num=0;    /*true # entries, since 0 entries are not counted!*/
       for(y=0;y<x;y++)/*fill table with nibbles*/
          {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
          back=data&1; data=data>>1; count--;
          if(back)/*grab nibble*/
            {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
            stack=data&0xF; data=data>>4; count-=4;
            }/*end grab*/
          what[y]=stack;
          if(stack) num++;   /*count nonzero entries*/
          }/*end fill*/
       x=FillBuffer(large,what,x,num,10);
#ifdef DEBUGGERY
       free(what);
#endif
       }/*end construction*/
     if(x<0) {x=erratta(x);
             if(x) return x;}

     count=helper(&data,count,in,&pos,edl.csize,edl.endian);/*build smaller table*/
     x=data&0x1FF; data=data>>9; count-=9;
     if(x) /*construct tables*/
       {
#ifdef DEBUGGERY
       if(!(what=calloc(x,1)))
         {printf("\nVirtual memory exhausted.\nCan not continue.\n\tPress ENTER to quit.");
         getchar();
         return 0;
         }/*end calloc*/
#else
       memset(what,0,0x400);
#endif
       num=0;    /*true # entries, since 0 entries are not counted!*/
       for(y=0;y<x;y++)/*fill table with nibbles*/
          {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
          back=data&1; data=data>>1; count--;
          if(back)/*grab nibble*/
            {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
            stack=data&0xF; data=data>>4; count-=4;
            }/*end grab*/
          what[y]=stack;
          if(stack) num++;   /*count nonzero entries*/
          }/*end fill*/
       x=FillBuffer(smallArray,what,x,num,8);
#ifdef DEBUGGERY
       free(what);
#endif
       }/*end construction*/
     if(x<0) {x=erratta(x);
             if(x) return x;}
     
     /*write data*/
    do{
     count=helper(&data,count,in,&pos,edl.csize,edl.endian);/*build smaller table*/
     x=data&0x3FF;
     x=large[x];          /*x=short from thingy*/
     y=x&0xF;             /*y=normal bitcount*/
     z=(x>>4)&7;       /*z=backtrack bitcount*/
     if(flagrant.message) printf("\n\tout: %X\tsample: %04X\tvalue: %X\tdata: %X",ftell(out),x,x>>7,data);
     if(y==0)  /*backtrack entry*/
       {x=x>>7; /*short's data*/
        y=(1<<z)-1;       /*bitmask*/
        count=helper(&data,count,in,&pos,edl.csize,edl.endian);
        y=(data>>10)&y;
        x+=y;
        x=large[x+0x400];
        y=x&0xF;
        }/*end backtrack entry*/
     
     data=data>>y; count-=y;
     y=0;
     x=x>>7;       /*data only*/
     if(x<0x100) 
       {fputc(x,out);
       if(ftell(out)>edl.dsize) return ftell(out);}
     else if(x>0x100)/*copy previous*/
        {z=table2[x-0x101];
        if(z)        /*segment*/
          {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
          y=(1<<z)-1;  /*mask*/
          y=data&y; data=data>>z; count-=z;
          }        /*end segment*/
        z=table1[x-0x101];
        num=z+y+3;
        count=helper(&data,count,in,&pos,edl.csize,edl.endian);
        x=data&0xFF; x=smallArray[x];

        y=x&0xF;             /*y=normal bitcount*/
        z=(x&0x70)>>4;       /*z=backtrack bitcount*/
        if(y==0)  /*backtrack entry*/
          {x=x>>7; /*short's data*/
          y=(1<<z)-1;       /*bitmask*/
          count=helper(&data,count,in,&pos,edl.csize,edl.endian);
          y=(data>>8)&y;
          x+=y;
          x=smallArray[x+0x100];
          y=x&0xF;
          }/*end backtrack entry*/
        data=data>>y; count-=y;
        
        /*pull number of bits*/
        y=0;
        x=x>>7;
        z=table4[x];
        if(z)        /*segment*/
          {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
          y=data&((1<<z)-1); data=data>>z; count-=z;
          }        /*end segment*/
        z=table3[x];
        back=z+y+1;

        /*copy run*/
        for(x=0;num>0;num--)
           {z=ftell(out)-back;
           if(z<0 || z>=ftell(out)) x=0;
           else{fseek(out,0-back,SEEK_END);     /*backward position*/
               x=fgetc(out);
               }
           fseek(out,0,SEEK_END);
           fputc(x,out);
           if(ftell(out)>edl.dsize) return ftell(out);     /*failsafe*/
           }/*end copy run*/
/*        for(x=0;num>0;num-=x)      this is faster but would need a catch
           {x=num;                   to keep it from copying bytes that have
           if(x>8) x=8;              not yet been written
           fseek(out,0-back,SEEK_END);
           fread(bits,1,x,out);
           fseek(out,0,SEEK_END);
           fwrite(bits,1,x,out);
           if(ftell(out)>edl.dsize) return ftell(out);
           }end debug-sometime-later*/
        }/*end copy previous*/
    }while(x!=0x100);     
   }/*mode 1*/
   else      /*mode 0 - */
     {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
     num=data&0x7FFF; data=data>>15; count-=15;
     if(flagrant.message) printf("\nmode0\tpos: %X\tout: %X",pos,ftell(out));
     if(num) 
       {for(fseek(out,0,SEEK_END);num>0;num--)
           {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
           x=data&0xFF; data=data>>8; count-=8;
           fputc(x,out);
           }/*end for()*/
       }/*write bytes*/
     }/*mode 0*/

   /*test EOF*/
   count=helper(&data,count,in,&pos,edl.csize,edl.endian);
   x=data&1; data=data>>1; count--;
   if(x) return ftell(out);        /*1=EOF marker*/
   }

return ftell(out);}

/*boring RLE magic*/
unsigned long EDLdec2(unsigned long pos,FILE *in,FILE *out,EDL edl)
{unsigned char bits[9];
long x;
unsigned long count=0,num,back;  /*count=#bits in register, num=#to copy, back=#to backtrack*/
#ifdef DATATABLE_32BIT
unsigned long data=0;
#else
unsigned long long int data=0;       /*64bit datatable container*/
#endif

/*set up data and count*/
edl.csize+=pos;

for(pos+=12;pos<=edl.csize;back=0)
   {memset(bits,0,9); /*clear bits between... stuff*/
   count=helper(&data,count,in,&pos,edl.csize,edl.endian);
   x=data&1; data=data>>1; count--;
   if(x)     /*mode 1 - copy*/
     {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
     bits[0]=data&1; bits[1]=(data&2)>>1; bits[2]=(data&4)>>2; bits[3]=(data&8)>>3;
     if(flagrant.message) {printf("\nmode1\tpos: %X\tout: %X\tdata: %X",pos,ftell(out),data);
                           printf("\n\t%X%X%X%X",bits[0],bits[1],bits[2],bits[3]);}
     if(bits[0]) {/*bit1:1*/
           num=2;
           data=data>>2; count-=2;
           if(bits[1]) 
             {data=data>>1; count--;
             num++;
             bits[8]=3;
             if(bits[2])
               {bits[8]=11;
               count=helper(&data,count,in,&pos,edl.csize,edl.endian);
               num=data&0xFF; data=data>>8; count-=8;
               if(num==0) return ftell(out);   /*this implies #bytes=0, signifying EOF*/
               num+=8;
               }/*bits[2]*/
             }/*bits[1]*/
           }/*bits[0]=1*/
     else{
         bits[8]=3;
         num=4;
         data=data>>3; count-=3;     /*minimum shift*/
         if(bits[1]) num++;
         if(bits[2])
           {bits[8]=4;
           data=data>>1; count--;
           num=2*(num-1)+bits[3];
           if(num==9)          /*special case write mode*/
             {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
             num=data&0xF; data=data>>4; count-=4;
             num*=4;
             for(num+=12;num>0;num--)
                {count=helper(&data,count,in,&pos,edl.csize,edl.endian);
                 x=data&0xFF; data=data>>8; count-=8;
                 fputc(x,out);
                 if(ftell(out)>edl.dsize) return ftell(out);     /*failsafe*/
                }
             continue;
             }/*num==9*/
           }/*bits[2]*/
          }/*bits[0]=0*/

     if(bits[8])        /*handle those backward offset types*/
       {count=helper(&data,count,in,&pos,edl.csize,edl.endian);  /*copy next 6, then work out size*/
       bits[0]=data&1; bits[1]=(data&2)>>1; 
       bits[2]=(data&4)>>2; bits[3]=(data&8)>>3;
       bits[4]=(data&0x10)>>4; bits[5]=(data&0x20)>>5;
       data=data>>1; count--;
       if(flagrant.message) printf("\t%X\t%X%X%X%X%X%X",bits[8],bits[0],bits[1],bits[2],bits[3],bits[4],bits[5]);
       
       if(bits[0])
         {if(bits[2])
            {if(bits[4])       /*10101 10111 11101 11111*/
               {data=data>>4; count-=4;
               back=0x400;
               if(bits[1]) back+=0x200;
               if(bits[3]) back+=0x100;
               }/*end bits[4]*/
             else              /*101000 101001 101100 101101 111000 111001 111100 111101*/
               {data=data>>5; count-=5;
               back=0x800;
               if(bits[1]) back+=0x400;
               if(bits[3]) back+=0x200;
               if(bits[5]) back+=0x100;
               }/*end bits[4]==0*/
            }/*end bits[2]*/
         else        /*bits[2]==0*/
            {if(bits[1])           /*110*/
               {back=0x100;
               data=data>>2; count-=2;
               }
            else                   /*1000 1001*/
               {data=data>>3; count-=3;
               back=0x200;
               if(bits[3]) back+=0x100;
               }
            }/*end bits[2]==0*/
         }/*end bits[0]*/  
       }/*bits[8]*/
     
     /*get the backward offset*/
     count=helper(&data,count,in,&pos,edl.csize,edl.endian);
     back=(data&0xFF)+back+1;        /*assured to copy at least 1 byte*/
     data=data>>8; count-=8;
     if(flagrant.message) printf("\n\tnum: %X\tback: %X",num,back);
     /*copy data from source*/
        for(x=0;num>0;num--)
           {x=ftell(out)-back;
           if(x<0 || x>=ftell(out)) x=0;
           else{fseek(out,0-back,SEEK_END);     /*backward position*/
               x=fgetc(out);
               }
           fseek(out,0,SEEK_END);
           fputc(x,out);
           if(ftell(out)>edl.dsize) return ftell(out);     /*failsafe*/
           }/*end copy run*/
/*        for(x=0;num>0;num-=x)      this is faster but would need a catch
           {x=num;                   to keep it from copying bytes that have
           if(x>8) x=8;              not yet been written
           fseek(out,0-back,SEEK_END);
           fread(bits,1,x,out);
           fseek(out,0,SEEK_END);
           fwrite(bits,1,x,out);
           if(ftell(out)>edl.dsize) return ftell(out);
           }end debug-sometime-later*/    
     }/*if()*/
   else{     /*mode 0 - push one byte to output*/
        count=helper(&data,count,in,&pos,edl.csize,edl.endian);
        if(flagrant.message) printf("\nmode0\tpos: %X\tout: %X\tdata: %X",pos,ftell(out),data);
        x=data&0xFF; data=data>>8; count-=8;
        fputc(x,out);
        if(ftell(out)>edl.dsize) return ftell(out);     /*failsafe*/
        }
   }

return ftell(out);}








/*returns file size*/
unsigned long filesize(FILE *in)
{fseek(in,0,SEEK_END);
 return ftell(in);}

/*file copying routine: copies 1000 bytes of blank data to temp, then the original file contents
  mode 0 just copies; mode 1+ adds the blank data*/
unsigned long filecopy(FILE *in, FILE *out,int mode)
{unsigned long x,y;

 /*Copy in 0x1000  of zeroes.  LZ assumes anything prior to the file is zero, 
 and the N64 does this to the file prior to decompression*/
 if(mode) {for(x=0;x<0x1000;x++) fputc(0,out);}
 
 /*copy the original file's contents to end of buffer*/
 x=filesize(in);
 fseek(in,0,SEEK_SET);
 for(y=0;y<x;y++) fputc(fgetc(in),out);
 
 return ftell(out);}

/*this takes a [value] of [size] bits and inserts it into [data], incrementing [bitcount]
  returns the bitcount*/
unsigned long helper(unsigned long *data,unsigned long *bitcount,unsigned long value,int size,FILE *out,unsigned long *pos)
{unsigned long v,w,x,y,z;
 /*shift current data and tack on new value*/
 x=*data;
 y=*bitcount;
 z=*pos;
 
#ifdef DEBUGGERY
 printf("\nBuffer: %08X-%X\tvalue: %X\tsize: %X",x,y,value,size);
#endif

 /*copy the value one bit at a time to the data buffer to prevent overruns*/
for(w=0;w<size;w++)
      {v=value>>w;       /*bit at value*/
       v&=1;
       x|=v<<y;          /*bit is pushed bitcount bits left and tacked onto value*/
       /*update bitcount and test for overrun*/
       y++;
       if(y>32) erratta(3);
       /*copy the buffer to file if it happens to equal 32bits*/
       if(y<32) continue;
       fseek(out,z,SEEK_SET);
#ifdef INTEL
       x=byteswap(x);
#endif
       fwrite(&x,4,1,out);
       x=0;
       y=0;
       z+=4;
       }

#ifdef DEBUGGERY
 printf("\tnFinal: %08X-%X",x,y);
#endif 
 *data=x;
 *bitcount=y;
 *pos=z;
 return y;}


/*EDL compression types.
  All accept the copied input file, an output file, and return the compressed file size*/
/*EDL compression type 0 - store
  no routine required.  It is store, after all...*/

/*EDL compression type 1 - LZ*/
unsigned long EDLcmp1(FILE *in,FILE *out)
{unsigned long x,ipos=0x1000,opos=12;
 /*this is a silly way to get the compressed file size, but it does work*/
 return ftell(out);}

/*EDL compression type 2 - RLE*/
/*uses a series of bits to determine what each run consists of
0	write next byte
110	num=2
1110	num=3,  offset
1111xxxxxxxx	num=x, offset
1000	num=4, offset
1010	num=5, offset
10010	num=6, offset
10011	num=7, offset
10110	num=8, offset
10111xxxx	write next x+3 words	(x+3)*4 bytes
111100000000	terminate

offset types:
	0	0x0
	110	0x100
	1000	0x200
	1001	0x300
	10101	0x400
	10111	0x500
	11101	0x600
	11111	0x700
	101000	0x800
	101001	0x900
	101100	0xA00
	101101	0xB00
	111000	0xC00
	111001	0xD00
	111100	0xE00
	111101	0xF00

backtrack:
xxxxxxxx	backtrack value (added to offset above when applicable)
reads num bytes from backtrack+offset

data retains the bits that will be copied every 16bits.  bitcount goes up as you write bits to it.

buffer stores a record of each hit for a match, then go back through to determine which is best
	index keeps the number of entries in buffer[]
string is your search string of len size, compared against file.  len=n in strncmp
Idea is you fill window with first 0x2000 bytes.  
	Load two bytes into search and find all matches between here and its position
	then, get next byte and search all these locations again
	repeat until the largest possible string is found, and use the one occurring at the closest address
search string, at most, can be 256 bytes, since largest type is num=FF. 
	longest possible straight data write is 0x48 bytes - 0x12 words

only stupid thing has to do with a run of straight written data
	if  a match can't be found, increment run
		when a match finally is found, first you copy in the entire run, set run to zero, then do the len
[number] dictates the maximum length of the search[] string*/
unsigned long EDLcmp2(FILE *in,FILE *out)
{unsigned long x,y,size,data=0,bitcount=0,ipos=0x1000,opos=12,index=0,len=0,run=0,number=0x100;
 unsigned short buffer[0x1000];
 unsigned char search[256],window[0x1100];

#ifdef DEBUGGERY
 printf("\nStarting EDL2 compression...");
#endif

 size=filesize(in);
 if((size-ipos)<number) number=size-ipos;
 
 /*initially fill window - later, you need only move it*/
 fseek(in,0,SEEK_SET);
 for(x=0;x<0x1100;x++) window[x]=fgetc(in);
 /*now fill the search window with max characters - again, will move it later*/
 fseek(in,ipos,SEEK_SET);
 for(x=0;x<number;x++) search[x]=fgetc(in);
 
 len=0;
 while(ipos+len<size)
	{/*fix ipos, refill the buffers, then loop*/
	 ipos+=len;
     for(x=0;x<0x1000;x++) window[x]=window[x+len];
     if((size-ipos)<number)
       {number=size-ipos;
#ifdef DEBUGGERY
       printf("\nSet number to %X-%X=%X",size,ipos,number);
#endif
        }
	 for(x=0;x<number;x++)
	    {if((x+len)<number) search[x]=search[x+len];
         else
           {fseek(in,ipos+x,SEEK_SET);
	        search[x]=fgetc(in);
            }
	     window[x+0x1000]=search[x];
	     }
#ifdef DEBUGGERY
       if(number<0x8)
         {printf("\n\t");
         for(x=0;x<number;x++) printf("%02X ",search[x]);
         }
#endif
     
     index=0;
	 len=2;
	 /*search for a hit of len+2*/
     for(x=0;x<0x1000;x++)
        {/*as you find them, record the positions in buffer and increment index*/
         if(!memcmp(window+x,search,len))
           {buffer[index]=(short)x;    /*evil pointer math*/
            index++;
            }
         }/*end for()*/
	 /*if you found matches (index>0), inc len, then go back through them testing for len+2 entries*/
     if(index && number>1)
       {do {y=0;
            for(x=0;x<index;x++)
               {if(!memcmp(window+buffer[x],search,len+1))
                  {buffer[y]=buffer[x];
                   y++;
                   }/*end if(onhit)*/
               }/*end for()*/
            if(y) {index=y;
                   len++;
                   }
           if(len==0xFF || len>number) break;        /*kill if you hit the max string length*/
            } while(y);
#ifdef DEBUGGERY
	       printf("\n\tMatches! len: %X\thits: %X",len,index);
#endif
           /*now that you have a match, find the favorite match and set index to this value*/
	       y=0;
           for(x=0;x<index;x++)
              {if(buffer[y]<buffer[x]) y=x;
               }
           index=y;
#ifdef DEBUGGERY
           printf("\tusing: #%X-%04X %04X\n\t",index,buffer[index],0xFFF-buffer[index]);
	       for(x=0;x<len;x++) printf("%02X ",search[x]);
#endif
           /*convert happy buffer value to backward offset*/
           if(buffer[index]>=0x1000) len=1;
           else buffer[index]=0xFFF-buffer[index];
	       /*bugfix: kill len2 entries that fall outside the first 0x100
             also kill stupid buffer values, like <=0*/
           if(len==2 && buffer[index]>0xFF) len=1;
        }/*end if(index)*/
	 else len=1;  /*straight binary run*/

     /*when len=1 this indicates a run
       normally, just increment run
       however, near EOF you'll need to write them*/
     if(len<2)
       {run++;
        if((ipos+len)<size) continue;
        ipos++;
        }
	 
	 /*first, test if run>0.  If so, you first have to copy in the run of straight characters*/
	 if(run)
		{/*copy any sets of words first - start at ipos-run*/
#ifdef DEBUGGERY
        printf("\n  Copying run of %X chars at %X",run,ipos-run-0x1000);
#endif
		fseek(in,ipos-run,SEEK_SET);
		x=run/4;
		while(x>3)
			{helper(&data,&bitcount,0x1D,5,out,&opos);	/*command type*/
			 x-=3;
			 y=x&0xF;
			 helper(&data,&bitcount,y,4,out,&opos);		/*copy size to command*/
			 x-=y;
			 y=(y+3)*4;
			 for(run-=y;y>0;y--)			/*copy actual data*/
				{helper(&data,&bitcount,fgetc(in),8,out,&opos);
				 }
			 }
		/*write individual bytes, then set run to zero*/
		while(run>0)
			{helper(&data,&bitcount,0,1,out,&opos);
			 helper(&data,&bitcount,fgetc(in),8,out,&opos);
			 run--;
			 }
		}
	 /*crap, will need to figure out a way to loop for copies, do a run, and not break on EOF*/
	 if(len<2) continue;

	 /*do the copy byte commands*/
	 switch(len)
		{case 2:
			helper(&data,&bitcount,0x3,3,out,&opos);
			break;
		 case 3:
			helper(&data,&bitcount,0x7,4,out,&opos);
			break;
		 case 4:
			helper(&data,&bitcount,0x1,4,out,&opos);
			break;
		 case 5:
			helper(&data,&bitcount,0x5,4,out,&opos);
			break;
		 case 6:
			helper(&data,&bitcount,0x9,5,out,&opos);
			break;
		 case 7:
			helper(&data,&bitcount,0x19,5,out,&opos);
			break;
		 case 8:
			helper(&data,&bitcount,0xD,5,out,&opos);
			break;
		 default:
			helper(&data,&bitcount,0xF,4,out,&opos);
			helper(&data,&bitcount,len-8,8,out,&opos);
			break;
		}
	 
     /*in every case but 2, also send a sinister offset chunk*/
	 if(len>2)
		{     if(buffer[index]>=0xF00) helper(&data,&bitcount,0x2F,6,out,&opos);
		 else if(buffer[index]>=0xE00) helper(&data,&bitcount,0x0F,6,out,&opos);
		 else if(buffer[index]>=0xD00) helper(&data,&bitcount,0x27,6,out,&opos);
		 else if(buffer[index]>=0xC00) helper(&data,&bitcount,0x07,6,out,&opos);
		 else if(buffer[index]>=0xB00) helper(&data,&bitcount,0x2D,6,out,&opos);
		 else if(buffer[index]>=0xA00) helper(&data,&bitcount,0x0D,6,out,&opos);
		 else if(buffer[index]>=0x900) helper(&data,&bitcount,0x25,6,out,&opos);
		 else if(buffer[index]>=0x800) helper(&data,&bitcount,0x05,6,out,&opos);
		 else if(buffer[index]>=0x700) helper(&data,&bitcount,0x1F,5,out,&opos);
		 else if(buffer[index]>=0x600) helper(&data,&bitcount,0x17,5,out,&opos);
		 else if(buffer[index]>=0x500) helper(&data,&bitcount,0x1D,5,out,&opos);
		 else if(buffer[index]>=0x400) helper(&data,&bitcount,0x15,5,out,&opos);
		 else if(buffer[index]>=0x300) helper(&data,&bitcount,0x09,4,out,&opos);
		 else if(buffer[index]>=0x200) helper(&data,&bitcount,0x01,4,out,&opos);
		 else if(buffer[index]>=0x100) helper(&data,&bitcount,0x03,3,out,&opos);
		 else 				 helper(&data,&bitcount,0,1,out,&opos);
		}
	 /*set backtrack value*/
	 helper(&data,&bitcount,buffer[index]&0xFF,8,out,&opos);
#ifdef DEBUGGERY
	 printf("\n  Wrote backtrack @ %X: %X, %X long -%X bytes back",opos,ipos-0x1000,len,buffer[index]);
#endif
     }
 
 /*set the terminator at end of file*/
 helper(&data,&bitcount,0x0F,12,out,&opos);	/*command type*/
 /*bugfix: flush the buffer with a few calls of 0*/
 if(bitcount) helper(&data,&bitcount,0,32-(bitcount%32),out,&opos);
 
 /*this is a silly way to get the compressed file size, but it does work*/
 return opos;}









CEDLCompress::CEDLCompress(void)
{
	flagrant.quiet=1;
	flagrant.message=0;
	flagrant.swap=0;
	flagrant.table=0;
	flagrant.list=0;
	flagrant.trash=0;
}

CEDLCompress::~CEDLCompress(void)
{
}

int CEDLCompress::decode(FILE* in, int offset, CString filename, int& fileSizeCompressed, CString& type)
{
	fseek(in, offset, SEEK_SET);
	EDL edl;
	int cur = offset;
	FillEDL(&edl,in,cur);
	if(edl.csize==0 || edl.dsize==0)
    {
		return 0;
	}
	if(edl.type>2)
		return 0;

	type.Format("EDL%X", edl.type);

	FILE* out = fopen(filename, "wb");
	if (out == NULL)
		return 0;

	int returnSize = 0;
	if(edl.type==0)      returnSize=EDLdec0(cur,in,out,edl);
    else if(edl.type==1) returnSize=EDLdec1(cur,in,out,edl);
    else if(edl.type==2) returnSize=EDLdec2(cur,in,out,edl);

	fclose(out);
	if (returnSize != edl.dsize)
		return 0;

	fileSizeCompressed = edl.csize;
	return returnSize;

}

bool CEDLCompress::encode(CString inFileName, CString outFileName)
{

 FILE *in, *out, *temp;
 int type=2;
 int endian=0x80;
 unsigned long x,y;

#ifdef FILE_DEBUG
 if(!(temp=fopen("debug.bin","wb+"))) erratta(2);
#else
 if(!(temp=fopen(inFileName +"tmpasdasd.bin", "wb+"))) return false;
#endif

 if (!(in = fopen(inFileName, "rb"))) 
 {
	fclose(temp);
	return false;
 }

 if (!(out = fopen(outFileName, "wb+")))
 {
	fclose(in);
	fclose(temp);
	return false;
 }

 char filename[9];
 /*tack on an EDL header - INTEL order
   x will be decompressed size and the first four bytes of filename=type
   compressed size is a placeholder, technically*/
 fseek(out,0,SEEK_SET);
 sprintf(filename,"EDL\0\0\0\0\0");
 filename[3]=endian|type;
 x=filesize(in);
 fwrite(filename,1,8,out);
#ifdef INTEL
 if(endian)  x=byteswap(x);
#else
 if(!endian) x=byteswap(x);
#endif
 fwrite(&x,4,1,out); 

 /*create a copy of the original binary with a prelude of 0x1000 zeroes; returns filesize*/
 /*EDLcmp0 - filecopy*/
 if(!type) y=filecopy(in,out,type);
 else filecopy(in,temp,type);
 fclose(in);

 /*start compression*/
 if(type==2) 	  y=EDLcmp2(temp,out);
 else if(type==1) y=EDLcmp1(temp,out);
 /*update compressed size and close files*/
 fseek(out,4,SEEK_SET);
 x=y;
#ifdef INTEL
 if(endian)  y=byteswap(y);
#else
 if(!endian) y=byteswap(y);
#endif
 fwrite(&y,4,1,out);
 fclose(temp);
 ::DeleteFile(inFileName +"tmpasdasd.bin");
 
 /*display compression ratio and stuff*/
 fseek(out,8,SEEK_SET);
 fread(&y,4,1,out);
#ifdef INTEL
 if(endian)  y=byteswap(y);
#else
 if(!endian) y=byteswap(y);
#endif 
 x-=12;
  fclose(out);

}