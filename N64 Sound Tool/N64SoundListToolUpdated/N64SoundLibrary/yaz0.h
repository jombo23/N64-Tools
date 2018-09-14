// Spinout
#pragma once

class YAZ0
{
public:
	unsigned long yaz0_decode (unsigned char *src, unsigned char *dst, int& fileSizeCompressed);
	int yaz0_get_size( unsigned char * src);
	void yaz0_encode(unsigned char * src, int src_size, unsigned char *dst, int *dst_size );
};
