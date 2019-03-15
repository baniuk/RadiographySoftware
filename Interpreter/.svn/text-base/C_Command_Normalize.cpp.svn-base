#include "StdAfx.h"
#include "C_Command_Normalize.h"

C_Command_Normalize::C_Command_Normalize(void)
{
	UINT parint[1] = {IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("normalize",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = normalize(IMAGE)";
	fun_ver = "1.0";
}

C_Command_Normalize::~C_Command_Normalize(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Normalize::Do(C_Error& err)
{
	C_Image_Container* scr;
	C_Image_Container dst;
	
	scr = (C_Image_Container*)intab[0]->data;
		
	scr->CloneObject(&dst);
	dst.Normalize(0,65535);
	outtab[0]->AssignValue((void*)&dst,err);
}
