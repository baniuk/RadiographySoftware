#pragma once
#define _AFXDLL
#include <windows.h>
#include "DLL_Cpp_Datatypes.h"
#include <stdio.h>
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
class Cell
{
public:
	Cell(void);
	~Cell(void);
	C_Matrix_Container** celltab;
	void CreateCell(int num);
	void DeletePart(unsigned long start,unsigned long stop);
	unsigned int length(void);
	void Dump(char* filename);
private:
	unsigned int numofcells;
};

struct res	{
	double angle;
	C_Matrix_Container k;
	C_Matrix_Container k90;
	C_Matrix_Container peakval;
	double estimated_len;
	C_Matrix_Container center;
	C_Matrix_Container lin;
	C_Matrix_Container lin90;
	C_Matrix_Container k_estim;
	C_Matrix_Container lin_estim;
	C_Matrix_Container preciki;
};