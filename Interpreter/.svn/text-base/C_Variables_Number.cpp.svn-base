#include "StdAfx.h"
#include "C_Variables_Number.h"

C_Variables_Number::C_Variables_Number(const char *pname,void* pvalue)
{
	C_Error err;
	data = (void*)new double;
	type = NUMBER;
	AssignName(pname);
	AssignValue(pvalue,err);
}
C_Variables_Number::C_Variables_Number(void)
{
	type = NUMBER;
	data = (void*)new double;
}
C_Variables_Number::~C_Variables_Number(void)
{
	delete (double*)data;
	data = NULL;
}
void C_Variables_Number::AssignValue(void *value, C_Error& err)
{
	double* tmp;
	tmp = (double*)data;
	tmp[0] = *((double*)value);
	data = tmp;		//sprawdzi czy dziala
}