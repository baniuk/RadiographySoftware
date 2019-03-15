#pragma once
#include "c_variables.h"

class C_Variables_Matrix :
	public C_Variables
{
public:
	C_Variables_Matrix(const char *pname,void* pvalue);
	C_Variables_Matrix(void);
	virtual void AssignValue(void* value, C_Error& err);
//	virtual void AssignName(const char* pname);
public:
	~C_Variables_Matrix(void);
};
