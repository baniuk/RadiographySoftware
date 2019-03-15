#include "Cpp_Lib.h"

// This is an empty project to build DLL library of interface version 2.0


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"copyimage");	// nazwa w ADDIP
	strcpy_s(definition->version,MAX_VER,"1.0");	// wersja (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"[IMAGE, IMAGE] = copyimage(IMAGE,IMAGE)"); // SYNTAX (nie ma wiêkszego znaczenia)
	definition->numofout = 1;	// ilosc parametrów wyœciowych
	definition->numofin = 0;	// ilosc parametrów wejœciowych
//	definition->typesofin[0] = IMAGE; definition->typesofin[1] = IMAGE;	// typy wejœcia
	definition->typesofout[0] = STRING;
	definition->interface_version = 2;	// wersja interface
	strcpy_s(definition->description,MAX_DESC,"TEST");	// cokolwiek
	definition->ADDIP_version = 2.0; 

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
/*	Rozmiar intab i outtab s¹ takie jak podano w Introduce, tak samo typy obiektów przekazywanych w tych zmiennych.*/
	
	char tmp[] = "c:\\";

	outtab[0]->AssignValue((void*)tmp,err);		// na koñcu przypisujemy wyniki do poszczególnych wyjœc


}