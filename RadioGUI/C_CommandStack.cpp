#include "StdAfx.h"
#include "C_CommandStack.h"

C_CommandStack::C_CommandStack(void)
{
	numofstrings = 0;
}

C_CommandStack::~C_CommandStack(void)
{
	ClearTable();
}

void C_CommandStack::AddString(char* string)
{
	tab[numofstrings] = new char[maxerror];
	strcpy_s(tab[numofstrings],maxerror,string);
	numofstrings++;
}

int C_CommandStack::GetNumStrings(void)
{
	return numofstrings;
}

void C_CommandStack::ClearTable(void)
{
	int a;
	for(a=0;a<numofstrings;a++)
		delete[] tab[a];
	numofstrings = 0;
}

char* C_CommandStack::GetString(int num)
{
	if(num>numofstrings)
		return NULL;
	else
		return tab[num];
	
}