/* 
 r3400i.h
*/

#define getRS(word)\
	(unsigned int)(unsigned int)((word >> 21) & 31)

#define getRT(word)\
	(unsigned int)((word >> 16) & 31)

#define getRD(word)\
	(unsigned int)((word >> 11) & 31)

#define getSA(word)\
	(unsigned int)((word >> 6) & 31)
/*
#define getFT(word)\
	(unsigned int)((word >> 16) & 31)

#define getFS(word)\
	(unsigned int)((word >> 11) & 31)

#define getFD(word)\
	(unsigned int)((word >> 6) & 31)
*/
#define getFT(word)\
	(unsigned int)(unsigned int)((word >> 21) & 31)

#define getFS(word)\
	(unsigned int)((word >> 16) & 31)

#define getFD(word)\
	(unsigned int)((word >> 11) & 31)

#define getBASE(word)\
	(unsigned int)((word >> 21) & 31)

#define getIMM(word)\
	(unsigned short int)(word & 0xFFFF)

#define getSIMM(word)\
	(signed short int)(word & 0xFFFF)

#define getOFFSET(word)\
	((signed short)(word & 0xFFFF)) * 4

#define getTARGET(word)\
	(unsigned int)((word & 0x3FFFFFF) << 2)

#define flip32(word)\
	(unsigned int)(((word&0xFF000000)>>24)|((word&0x00FF0000)>>8)|((word&0x0000FF00)<<8)|((word&0x000000FF)<<24))

#define flip16(value)\
	(unsigned short) ((value&0xFF00)>>8) | ((value&0xFF)<<8)
