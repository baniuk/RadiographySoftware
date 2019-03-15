#pragma once
#include "Cpp_Lib.h"
#include <stdio.h>
class C_Cell
{
public:
	C_Cell(void);
	~C_Cell(void);
	C_Matrix_Container** celltab;
	void CreateCell(int num);
	void DeletePart(unsigned long start,unsigned long stop);
	unsigned int length(void);
	void Dump(char* filename);
private:
	unsigned int numofcells;
};
