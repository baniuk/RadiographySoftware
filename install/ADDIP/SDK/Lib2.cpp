#include "Cpp_Lib.h"

// This is an empty project of DLL interface version 2.0


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"test");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = test(IMAGE,IMAGE)");
	definition->numofout = 1;
	definition->numofin = 2;
	definition->typesofin[0] = IMAGE; definition->typesofin[1] = IMAGE;
	definition->typesofout[0] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"TEST");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{

}