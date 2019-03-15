#pragma once

#include "globvar.h"
#include "tiffvar.h"
#include "io.h"

class R_Tiff
{
public:
	R_Tiff(void);														// konstruktor obiektu
	char *picBuffer;
	int readTiffImage(char* filename);									// wczytuje tiffa
	tiffHeaderStruct  imageHeader;
	int openTiffFile( char* fileName, int *fileDesc );					// otwiera plik tiff
	int readTiffHeader( int fileDesc );									// czyta tagi z nag³ówka
	static void extract_long_from_buffer( char buffer[], int lsb, int start, long *number );
	static void extract_short_from_buffer( char buffer[], int  lsb, int start, short int *number );
	int* tif;
	~R_Tiff(void);
};
