#include "StdAfx.h"
#include "C_Command_Print.h"

C_Command_Print::C_Command_Print(void)
{
	UINT parint[1] = {NUMBER};
	UINT parout[1] = {};
	InitCommand("print",1,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "print(NUMBER)";
	fun_ver = "1.0";
}

C_Command_Print::~C_Command_Print(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

/*void C_Command_Print::PrintYourself(void)
{
	printf("Funkcja: %s\n",name);
	printf("	Parametry wejœciowe: %u\n",numofinparam);
	printf("	Parametry wyjœciowe: %u\n",numofoutparam);
	printf("	Typ wejœciowy: %u",inputtype[0]);
	for(int a=1;a<numofinparam;a++)
		printf(", %u",inputtype[a]);
	printf("\n");	
	
	printf("	Typ wyjœciowy: %u",outputtype[0]);
	for(int a=1;a<numofoutparam;a++)
		printf(", %u",outputtype[a]);
	printf("\n");
	
}

DT* C_Command_Print::ReturnDTin(void)
{
	return inputtype;
}

DT* C_Command_Print::ReturnDTout(void)
{
	return outputtype;
}
int C_Command_Print::ReturnNumIn(void)
{
	return numofinparam;
}
int C_Command_Print::ReturnNumOut(void)
{
	return numofoutparam;
}
char* C_Command_Print::ReturnName(void)
{
	return name;
}
void C_Command_Print::InitCommand(char* p_name,int p_numofinparam,int p_numofoutparam,DT* p_inputtype, DT* p_outputtype)
{
	int a;
	numofinparam = p_numofinparam;
	numofoutparam = p_numofoutparam;
	inputtype = new DT[numofinparam];
	outputtype = new DT[numofoutparam];
	strlength = strnlen(p_name, MAXSIZE);
	strcpy_s(name,MAXSIZE,p_name);
	for(a=0;a<numofinparam;a++)
		inputtype[a] = p_inputtype[a];
	for(a=0;a<numofoutparam;a++)
		outputtype[a] = p_outputtype[a];

}*/

void C_Command_Print::Do()
{

	
}
