#include "StdAfx.h"
#include "C_Command_Exit.h"

C_Command_Exit::C_Command_Exit(void)
{
	UINT parint[1] = {NUMBER};
	UINT parout[1] = {};
	InitCommand("exit",1,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "exit";
	fun_ver = "1.1";
}

C_Command_Exit::~C_Command_Exit(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Exit::Do(C_Error& err)
{
	C_Matrix_Container *p_number;
	p_number = (C_Matrix_Container*)intab[0]->data;

	exit((int)p_number->data[0]);	
}
