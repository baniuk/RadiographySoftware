#include "StdAfx.h"
#include "C_Command_Crackdetection.h"

C_Command_Crackdetection::C_Command_Crackdetection(void)
{
	UINT parint[1] = {STRING};
	UINT parout[1] = {STRING};
	InitCommand("crackdetection",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "STRING = crackdetection(STRING)";
	fun_ver = "0.1";
}

C_Command_Crackdetection::~C_Command_Crackdetection(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Crackdetection::Do(C_Error& err)
{
	
}

