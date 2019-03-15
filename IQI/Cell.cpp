#pragma once
#include "Cell.h"

Cell::Cell(void)
{
	numofcells = 0;
}

Cell::~Cell(void)
{
	unsigned int a;
	if(numofcells>0)	{
		for(a=0;a<numofcells;a++)
			SAFE_DELETE(celltab[a]);
		delete[] celltab;
	}
}
void Cell::Dump(char* filename)
{
	size_t len;
	len = strlen(filename);
	char *filename1;
	filename1 = new char[len+5];
	for(unsigned int a=0;a<numofcells;a++)	{
		sprintf_s(filename1,len+5,"%s_%d",filename,a);
		if(celltab[a]!=NULL)
			if(celltab[a]->data!=NULL)
				celltab[a]->Dump(filename1);
	}
}

void Cell::CreateCell(int num)
{
	celltab = new C_Matrix_Container*[num];
	numofcells = num;
	for(unsigned int a=0;a<numofcells;a++)
		celltab[a] = new C_Matrix_Container;
}
void Cell::DeletePart(unsigned long start,unsigned long stop)
{
	for(int a=start;a<stop;a++)
		SAFE_DELETE(celltab[a]);
//	numofcells-=(stop-start); 
}
unsigned int Cell::length(void)
{
	return numofcells;
}