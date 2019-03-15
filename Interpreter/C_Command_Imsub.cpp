#include "StdAfx.h"
#include "C_Command_Imsub.h"

C_Command_Imsub::C_Command_Imsub(void)
{
	UINT parint[2] = {IMAGE,IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("imsub",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = imsub(IMAGE,IMAGE)";
	fun_ver = "1.0";
}

C_Command_Imsub::~C_Command_Imsub(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Imsub::Do(C_Error& err)
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
	ret = o1.Sub(&o2);
	if(!ret)	{
		err.SetError("Imsub: Images must be the same");
		return;
	}
	o1.Normalize(0,65535);
//	o1.isBinary = TRUE;
	
	outtab[0]->AssignValue((void*)&o1,err);

}