#include "StdAfx.h"
#include "C_Command_Thread.h"

C_Command_Thread::C_Command_Thread(void)
{
	UINT parint[1] = {NUMBER};
	UINT parout[1] = {};
	InitCommand("threads",1,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "threads(NUMBER)";
	fun_ver = "1.0";


	/*strcpy_s(definition->name,MAX_NAME,"cf");	// nazwa w ADDIP
	strcpy_s(definition->version,MAX_VER,"1.0");	// wersja (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"MATRIX = cf(IMAGE,IMAGE,IMAGE)"); // SYNTAX (nie ma wiêkszego znaczenia)
	definition->numofout = 1;	// ilosc parametrów wyœciowych
	definition->numofin = 3;	// ilosc parametrów wejœciowych
	definition->typesofin[0] = IMAGE;definition->typesofin[1] = IMAGE;definition->typesofin[2] = IMAGE;	// typy wejœcia
	definition->typesofout[0] = MATRIX; // typy wyjœcia
	definition->interface_version = 2;	// wersja interface
	strcpy_s(definition->description,MAX_DESC,"Developed by PB");	// cokolwiek
	definition->ADDIP_version = 2.0; */
}

C_Command_Thread::~C_Command_Thread(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Thread::Do(C_Error& err)
{
	C_Matrix_Container *p_proc;
	C_Matrix_Container proc;
	p_proc = (C_Matrix_Container*)intab[0]->data;
	
	p_proc->CloneObject(&proc);

	if(proc.data[0]>8)	{
		err.SetUserData("thread: Maximum 8 processes allowed");
		proc.data[0] = 8;
	}


//	tmpmatrix.AllocateData(1,1); tmpmatrix.data[0] = 2;

	(*globals)[current_global]->AssignName("numofproc");
	(*globals)[current_global]->AssignValue((void*)&proc,err);

	
}