#include "StdAfx.h"
#include "C_Command_Chdir.h"

C_Command_Chdir::C_Command_Chdir(void)
{
	UINT parint[1] = {STRING};
	UINT parout[1] = {};
	InitCommand("chdir",1,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "chdir(STRING)";
	fun_ver = "1.0";
}

C_Command_Chdir::~C_Command_Chdir(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Chdir::Do(C_Error& err)
{
	char* tmpnazwa;
	char repair[512];
	int ret;

	tmpnazwa = (char*)intab[0]->data;
	RepairPath(tmpnazwa,repair);

	ret = _chdir(tmpnazwa);
	if(ret!=0)	{
		err.SetError("chdir: Can't change the directory");
		return;
	}

	
}
