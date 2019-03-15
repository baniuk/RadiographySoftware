#ifdef MAIN
  #define global /* */
#else
  #define global extern
#endif

#include <stdio.h>

/*--------------------------------------------------------------*
 *           Variables' declarations for program                *
 *--------------------------------------------------------------*/

#include "globtype.h"

global int *markXPositionBigLeft, *markYPositionBigLeft;
global int *markXPositionSmallLeft, *markYPositionSmallLeft;
global int *markXPositionBigRight, *markYPositionBigRight;
global int *markXPositionSmallRight, *markYPositionSmallRight;

global int offsetBigY[ 10 ];
global int offsetBigX[ 10 ];
global int offsetSmallY[ 20 ];
global int offsetSmallX[ 20 ];
global FILE* stream;
//tiffHeaderStruct  imageHeader;
//byte *picBuffer;
global byte SLANT_FLAG;

global position XY;
global int pCounter;
global int intNo;
global char *sharedAddr;
/******************************************************************
 *   Global variables                                             *
 ******************************************************************/
/************************************************

 Field scanning global data definitions

 ************************************************/

global int NO_SCAN_LINES;

global int NoMarksInByte;
global int NoMarksInWord;
global int NoMarksInLong;
global int NoMarksInLine;
global int FreqOfNumber;
global int limit_max_mark_freq;

global int DISTANCE_MAX;
global int DISTANCE_MIN;
global int MAX_MARK_WIDTH;
global int DENSE_SCANNING;
global int START_SCAN;
global int SCAN_LINES;

/*mz New algorithm ******************************/

global struct byte_problem      *CMARKBYTE;
global struct corner_info       *CORNERINFO;
global struct word_problem      *CMARKWORD;
global struct long_word_problem *CMARKLONG;

/*mz End of new algorithm ***********************/

global int *SCAN_LINE_Y_POSITIONS; /* dim: MAX_NO_SCAN_LINES */

/* table of StatMarkLine : No_Scan_Lines lines (only big fields)*/
global struct status_mark_line *MARKLINE; /* dim: MAX_NO_SCAN_LINES */



