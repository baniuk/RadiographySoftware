#include "StdAfx.h"
#include "C_Variables_Matrix.h"

C_Variables_Matrix::C_Variables_Matrix(void)
{
	data = (void*)new C_Matrix_Container;
	type = MATRIX;
	
}

C_Variables_Matrix::C_Variables_Matrix(const char *pname,void* pvalue)
{
	C_Error err;			// to musi byæ globalnie z zewn¹trz
	data = (void*)new C_Matrix_Container;
	type = MATRIX;
	AssignName(pname);
	AssignValue(pvalue,err);

}
C_Variables_Matrix::~C_Variables_Matrix(void)
{
	delete (C_Matrix_Container*)data;
	data = NULL;
}
void C_Variables_Matrix::AssignValue(void* value, C_Error& err)
{
	C_Matrix_Container* tmp;
	C_Matrix_Container* tmp1;
	tmp = (C_Matrix_Container*)data;
	tmp1 = (C_Matrix_Container*)value;
	tmp1->CloneObject(tmp);
}