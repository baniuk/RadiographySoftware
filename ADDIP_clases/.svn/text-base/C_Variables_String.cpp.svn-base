#include "StdAfx.h"
#include "C_Variables_String.h"

C_Variables_String::C_Variables_String(const char *pname,void* pvalue)
{
	C_Error err;
	type = STRING;
	data = (void*)new char[256];
	AssignValue(pvalue,err);
	AssignName(pname);
}
C_Variables_String::C_Variables_String(void)
{
	type = STRING;
	data = (void*)new char[256];
}
C_Variables_String::~C_Variables_String(void)
{
	delete (char*)data;
	data = NULL;
}
void C_Variables_String::AssignValue(void *value, C_Error& err)
{
	char* tmp;
	char* tmp1;
	unsigned int a;
	char strtemp1, strtemp2;
	size_t d;
	tmp = (char*)value;
	d = strlen(tmp);
	tmp1 = new char[d-1];

	strtemp1 = tmp[0];	strtemp2 = tmp[d-1];
	if(strtemp1==39 && strtemp2==39)	{		// czyli string zakoñczony z dwóch stron apostrofem
		for(a=0;a<d-2;a++)
			tmp1[a] = tmp[a+1];
		ASSERT(a<d-1);
		tmp1[a] = 0;
		d = strlen(tmp1);
		strcpy_s((char*)data,256,tmp1);		// kopiowanie linii do bufora
	}
	else
		strcpy_s((char*)data,256,tmp);		// kopiowanie linii do bufora
	delete tmp1;
}
