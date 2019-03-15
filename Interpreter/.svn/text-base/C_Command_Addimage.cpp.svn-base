#include "StdAfx.h"
#include "C_Command_Addimage.h"

C_Command_Addimage::C_Command_Addimage(void)
{
	UINT parint[2] = {IMAGE,IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("addimage",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
}

C_Command_Addimage::~C_Command_Addimage(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Addimage::Do(C_Error& err)
{
	C_Image_Container *image1;
	C_Image_Container *image2;
	BOOL ret;
		
	image1 = (C_Image_Container*)intab[0]->data;
	image2 = (C_Image_Container*)intab[1]->data;
	
	ret = image1->Add(image2);
	if(ret==FALSE)	{
		err.SetError("Addimage: Bad size");
		return;
	}
	image1->Normalize(0,65535);
	
	outtab[0]->AssignValue((void*)image1,err);
}
