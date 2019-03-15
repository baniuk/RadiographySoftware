#include "StdAfx.h"
#include "C_Command_Add.h"

C_Command_Add::C_Command_Add(void)
{
	UINT parint[2] = {MATRIX,MATRIX};
	UINT parout[1] = {MATRIX};
	InitCommand("add",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "MATRIX = add(MATRIX,MATRIX)";
	fun_ver = "1.1";
}

C_Command_Add::~C_Command_Add(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Add::Do(C_Error& err)
{
	C_Matrix_Container liczba;
	C_Matrix_Container *tmpp,*tmp1p;
	double tmp,tmp1;
	tmpp = (C_Matrix_Container*)intab[0]->data;
	tmp1p = (C_Matrix_Container*)intab[1]->data;

	tmp = (double)tmpp->data[0];
	tmp1 = (double)tmp1p->data[0];

	liczba.AllocateData(1,1);
	liczba.SetPixel(0,0,tmp+tmp1);
	outtab[0]->AssignValue((void*)&liczba,err);

}