#include "StdAfx.h"
#include "C_Commands.h"

C_Commands::C_Commands()
{
	description = NULL;
	fun_time = NULL;
	fun_date = NULL;	// te pola nie musza wytepowac we wszystkich dll dlatego ustawiane na null
	outputtype = NULL;
	inputtype = NULL;
//	globals = NULL;	
}

C_Commands::~C_Commands(void)
{
	SAFE_DELETE(inputtype);
	SAFE_DELETE(outputtype);
	
}
void C_Commands::PrintYourself(void)
{
	printf("Function: %s\n",name);
	printf("	Syntax: %s\n",syntax);
	printf("    Version: %s\n",fun_ver);
	printf("\n");
	
}

DT* C_Commands::ReturnDTin(void)
{
	return inputtype;
}

DT* C_Commands::ReturnDTout(void)
{
	return outputtype;
}
int C_Commands::ReturnNumIn(void)
{
	return numofinparam;
}
int C_Commands::ReturnNumOut(void)
{
	return numofoutparam;
}
char* C_Commands::ReturnName(void)
{
	return name;
}
void C_Commands::InitCommand(char* p_name,int p_numofinparam,int p_numofoutparam,DT* p_inputtype, DT* p_outputtype)
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

}

void C_Commands::RepairPath(char* Src, char* Dest)
{	// not tested yet
	size_t zm1;
	char nazwa[512];
	unsigned int j;
	int k = 0;
	zm1 = strlen(Src);
	for (j=0;j<zm1;j++)
		nazwa[j]=*(Src++);
		
	for (j=0;j<zm1;j++)	{
		Dest[k++]=nazwa[j];
		if (nazwa[j]=='\\')	{
			Dest[k++]='\\';
		}
	}
	Dest[k] = 0;

}