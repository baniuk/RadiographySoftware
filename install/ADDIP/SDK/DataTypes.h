#pragma once
#define IMAGE  0x00000010
#define STRING 0x00000100
#define MATRIX 0x00001000
#define NUMBER MATRIX
#define DT UINT

#define maxline 256			// maksymalna ilosc znakow w linii
#define CHARNOEXIST 32000
#define maxnameofleftparametr 256
#define maxwindows 256;
#define maxbuffor 50		// ilosc komend
#define maxvar 1024			// ilosc zmiennych w programie (te parmaetry okre�lane w fazie init b�d�)
#define maxlines 512		// maksymlana ilo�� linii
#define maxerror 512
#define MAXSIZE 25 // maks nazwa funkcji
#define MAX_GLOBALS 25 // max ilosc zmiennych globalnych
#define maxthreads 8 // maxymalna ilosc watkow
/*
class C_MATRIX
{
public:
	C_MATRIX(int rows, int cols);
	void SetValue(int row, int col,int value);
	int GetValue(int row, int col);
	unsigned int rows, cols;
	int* data;
public:
	~C_MATRIX(void);
};
*/