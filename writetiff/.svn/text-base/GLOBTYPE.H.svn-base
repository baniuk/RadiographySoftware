/*--------------------------------------------------------------*
 *         Types' definitions for program                       *
 *--------------------------------------------------------------*/

typedef unsigned char byte;

typedef struct 
{
   short  lsb;
   long   bitsPerPixel;
   long   imageLength;
   long   imageWidth;
   long   stripOffset;
} tiffHeaderStruct;

typedef struct
{
   int X;
   int Y;
} position;

typedef struct
	{
	int	x;
	int	y;
	}  xy_pos_tab;

/*mz New algorithm */

typedef struct byte_problem
    {
    unsigned LT : 1;
    unsigned LL : 4;
    unsigned RT : 1;
    unsigned RL : 4;
    unsigned MN : 2;
    unsigned ML : 4;
    } ByteProblem;

typedef struct corner_info
    {
    unsigned BN : 4;
    unsigned BL : 4;
    } CornerInfo;

typedef struct word_problem
    {
    unsigned WN : 4;
    unsigned WL : 4;
    } WordProblem;

typedef struct long_word_problem
    {
    unsigned ok     : 1;
    unsigned number : 4;
    unsigned LL     : 3;
    } LongWordProblem;

/*mz End of new algorithm */

typedef struct status_mark_byte
	{
	unsigned ok     : 1;
	unsigned number : 2;
	unsigned left   : 2;
	unsigned right  : 2;
	unsigned rest   : 1;
	}StatMarkByte;


typedef struct status_mark_word
	{
	unsigned ok     : 1;
	unsigned number : 3;
	unsigned left   : 2;
	unsigned right  : 2;
	}StatMarkWord;


typedef struct status_mark_long_word
	{
	unsigned ok     : 1;
	unsigned number : 3;
	unsigned left   : 2;
	unsigned right  : 2;
	}StatMarkLongWord;


typedef struct status_mark_line
	{
	unsigned ok     : 1;
	unsigned number : 3;
	unsigned rest   : 4;
	}StatMarkLine;

typedef struct status_mark_field
	{
	unsigned ok     : 1;
	unsigned number : 3;
	unsigned rest   : 4;
	}StatMarkField;