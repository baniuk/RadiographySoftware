#include "StdAfx.h"
#include "C_Command_Sin.h"

C_Command_Sin::C_Command_Sin(void)
{
	UINT parint[1] = {NUMBER};
	UINT parout[1] = {NUMBER};
	InitCommand("sin",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "NUMBER = sin(NUMBER)";
	fun_ver = "1.0";
}

C_Command_Sin::~C_Command_Sin(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Sin::Do(C_Error& err)
{
	C_Matrix_Container liczba;
	double restmp;
	C_Matrix_Container *tmp;
	tmp = (C_Matrix_Container*)intab[0]->data;
	restmp = sin(tmp->data[0]);
	liczba.AllocateData(1,1);
	liczba.data[0] = restmp;
	outtab[0]->AssignValue((void*)&liczba,err);

}