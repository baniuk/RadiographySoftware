#include "StdAfx.h"
#include "C_Command_Immulti.h"

C_Command_Immulti::C_Command_Immulti(void)
{
	UINT parint[2] = {IMAGE,IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("immulti",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = immulti(IMAGE,IMAGE)";
	fun_ver = "1.0";
}

C_Command_Immulti::~C_Command_Immulti(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Immulti::Do(C_Error& err)
{
	C_Image_Container *image1;
	C_Image_Container *image2;
	C_Image_Container o1;
	C_Image_Container o2;
//	C_Image_Container result;
	BOOL ret;
	
	image1 = (C_Image_Container*)intab[0]->data;
	image2 = (C_Image_Container*)intab[1]->data;

	image1->CloneObject(&o1);
	image2->CloneObject(&o2);

	o1.Normalize(0,65535);
	o2.Normalize(0,65535);
	
//	image1->CloneObject(&result);
	ret = o1.DotMulti(&o2);
	if(!ret)	{
		err.SetError("Immulti: Images must be the same");
		return;
	}
	o1.Normalize(0,65535);
	
	outtab[0]->AssignValue((void*)&o1,err);
}
