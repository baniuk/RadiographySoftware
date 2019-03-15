#include "StdAfx.h"
#include "C_MultiProcesor.h"

C_MultiProcesor::C_MultiProcesor(void)
{
	numofprcesors = 1;
}

C_MultiProcesor::~C_MultiProcesor(void)
{
}

BOOL C_MultiProcesor::SetNumProc(int num)
{
	if(num%2==0) {
		numofprcesors = num;
		return true;
	} else
		return false;
}

BOOL C_MultiProcesor::AutoSetNumProc(void)
{
	return true;
}

int C_MultiProcesor::GetNumProc(void)
{
	return numofprcesors;
}