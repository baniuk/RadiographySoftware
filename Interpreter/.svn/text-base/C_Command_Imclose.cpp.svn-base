#include "StdAfx.h"
#include "C_Command_Imclose.h"

C_Command_Imclose::C_Command_Imclose(void)
{
	UINT parint[1] = {STRING};
	UINT parout[1] = {IMAGE};
	InitCommand("imclose",1,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "imclose(STRING)";
	fun_ver = "1.0";
}

C_Command_Imclose::~C_Command_Imclose(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Imclose::Do(C_Error& err)
{
	char* tmp;
	tmp = (char*)intab[0]->data;
	cvDestroyWindow(tmp);
}