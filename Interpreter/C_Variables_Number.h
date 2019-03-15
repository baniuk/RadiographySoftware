#pragma once
#include "c_variables.h"

class C_Variables_Number :
	public C_Variables
{
public:
	C_Variables_Number(const char *pname,void* pvalue);
	C_Variables_Number(void);
	virtual void AssignValue(void* value, C_Error& err);
//	virtual void AssignName(const char* pname);
public:
	virtual ~C_Variables_Number(void);
};
