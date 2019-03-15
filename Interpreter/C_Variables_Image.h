#pragma once
#include "c_variables.h"

class C_Variables_Image:
	public C_Variables
{
public:
	C_Variables_Image(const char *pname,void* pvalue);
	C_Variables_Image(void);
	virtual void AssignValue(void* value, C_Error& err);
public:
	virtual ~C_Variables_Image(void);
};
