#pragma once

#define N		 4096	/* size of ring buffer */
#define F		   60	/* upper limit for match_length */
#define THRESHOLD	2   /* encode string into position and length
						   if match_length is greater than this */
#define NIL			N	/* index for root of binary search trees */


/********** Arithmetic Compression **********/

/*  If you are not familiar with arithmetic compression, you should read
		I. E. Witten, R. M. Neal, and J. G. Cleary,
			Communications of the ACM, Vol. 30, pp. 520-540 (1987),
	from which much have been borrowed.  */

#define M   15

/*	Q1 (= 2 to the M) must be sufficiently large, but not so
	large as the unsigned long 4 * Q1 * (Q1 - 1) overflows.  */

#define Q1  (1UL << M)
#define Q2  (2 * Q1)
#define Q3  (3 * Q1)
#define Q4  (4 * Q1)
#define MAX_CUM (Q1 - 1)

#define N_CHAR  (256 - THRESHOLD + F)
	/* character code = 0, 1, ..., N_CHAR - 1 */


class CLZARIDecoder
{
public:
	CLZARIDecoder(void);

	FILE* infile;
	FILE* outfile;
	unsigned long int textsize;
	unsigned long codesize;
	unsigned long printcount;

	unsigned char  text_buf[N + F - 1];	/* ring buffer of size N,
			with extra F-1 bytes to facilitate string comparison */
	int		match_position;
	int  match_length;
	/* of longest match.  These are
				set by the InsertNode() procedure. */
	int lson[N + 1];
	int rson[N + 257];
	int dad[N + 1];  /* left & right children &
				parents -- These constitute binary search trees. */

	unsigned long int  low;
	unsigned long int  high;
	unsigned long int  value;

	int  shifts;  /* counts for magnifying low and high around Q2 */
	int  char_to_sym[N_CHAR];
	int sym_to_char[N_CHAR + 1];
	unsigned int sym_freq[N_CHAR + 1];  /* frequency for symbols */
	unsigned int sym_cum[N_CHAR + 1];   /* cumulative freq for symbols */
	unsigned int position_cum[N + 1];   /* cumulative freq for positions */

	void Error(char *message);
	void PutBit(int bit);
	void FlushBitBuffer();
	int GetBit();
	void InitTree();
	void InsertNode(int r);
	void DeleteNode(int p);
	void StartModel(void);
	void UpdateModel(int sym);
	void Output(int bit);
	void EncodeChar(int ch);
	void EncodePosition(int position);
	void EncodeEnd();
	int BinarySearchSym(unsigned int x);
	int BinarySearchPos(unsigned int x);
	void StartDecode();
	int DecodeChar();
	int DecodePosition();
	void Encode();
	void Decode();

	unsigned int  buffer;
	unsigned int  mask;
	bool initBuffer;
};
