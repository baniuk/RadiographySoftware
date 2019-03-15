#include "StdAfx.h"
#include "C_Commans_ISeePort.h"

C_Commans_ISeePort::C_Commans_ISeePort(void)
{
	UINT parint[1] = {NUMBER};
	UINT parout[1] = {};
	InitCommand("iseeport",1,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "iseeport(NUMBER)";
	fun_ver = "1.0";
}

C_Commans_ISeePort::~C_Commans_ISeePort(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Commans_ISeePort::Do(C_Error& err)
{
	C_Matrix_Container *p_proc;
	C_Matrix_Container proc;
	p_proc = (C_Matrix_Container*)intab[0]->data;
	
	p_proc->CloneObject(&proc);

	if(proc.data[0]<0 || proc.data[0]>9999)	{
		err.SetUserData("iseeport: Wrong range");
		proc.data[0] = 22;
	}


//	tmpmatrix.AllocateData(1,1); tmpmatrix.data[0] = 2;

	(*globals)[current_global]->AssignName("iseeport");
	(*globals)[current_global]->AssignValue((void*)&proc,err);

	
}