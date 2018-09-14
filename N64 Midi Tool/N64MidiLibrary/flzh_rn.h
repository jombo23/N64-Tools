// By Zoinkity
#pragma once


#define N		4096	/* buffer size */
#define F		60	/* lookahead buffer size */
#define THRESHOLD	2
#define NIL		N	/* leaf of tree */

#define N_CHAR  	(256 - THRESHOLD + F)
			/* kinds of characters (character code = 0..N_CHAR-1) */
#define T 		(N_CHAR * 2 - 1)	/* size of table */
#define R 		(T - 1)			/* position of root */
#define MAX_FREQ	0x8000		/* updates tree when the */
					/* root frequency comes to this value. */
typedef unsigned char uchar;

#include "MidiExports.h"

class MIDI_API flzh
{
public:
	flzh(void);

	FILE  *infile;
	FILE *outfile;
	unsigned long int textsize;
	unsigned long int codesize;
	unsigned long int printcount;

	void Encode(void);
	void Decode(void);
	void EncodeEnd();
	int GetBit(void);
	int GetByte(void);
	int DecodeChar();
	int DecodePosition();
	void Putcode(int l, unsigned c);
	void EncodeChar(unsigned c);
	void EncodePosition(unsigned c);
	void InitTree(void);
	void InsertNode(int r);
	void DeleteNode(int p);
	void StartHuff(void);
	void reconst(void);
	void update(int c);

	unsigned code, len;

	unsigned putbuf;
	uchar putlen;

	unsigned char
		text_buf[N + F - 1];
	int		match_position, match_length,
		lson[N + 1], rson[N + 257], dad[N + 1];
	unsigned freq[T + 1];	/* frequency table */

	int prnt[T + N_CHAR];	/* pointers to parent nodes, except for the */
				/* elements [T..T + N_CHAR - 1] which are used to get */
				/* the positions of leaves corresponding to the codes. */

	int son[T];		/* pointers to child nodes (son[], son[] + 1) */

	unsigned getbuf;
	uchar getlen;
};
