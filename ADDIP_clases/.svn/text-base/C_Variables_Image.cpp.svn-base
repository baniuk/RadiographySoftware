#include "StdAfx.h"
#include "C_Variables_Image.h"

C_Variables_Image::C_Variables_Image(void)
{
	data = (void*)new C_Image_Container;
	type = IMAGE;
}

C_Variables_Image::C_Variables_Image(const char *pname,void* pvalue)
{
	C_Error err;			// to musi byæ globalnie z zewn¹trz
	data = (void*)new C_Image_Container;
	type = IMAGE;
	AssignName(pname);
	AssignValue(pvalue,err);
}

C_Variables_Image::~C_Variables_Image(void)
{
	C_Image_Container* tmp = (C_Image_Container*)data;
	delete tmp;
	data = NULL;
}

void C_Variables_Image::AssignValue(void *value, C_Error& err)
{
	C_Image_Container* tmp;
	C_Image_Container* tmp1;
	tmp = (C_Image_Container*)data;
	tmp1 = (C_Image_Container*)value;
	tmp1->CloneObject(tmp);
}