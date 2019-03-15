#include "C_Lib.h"

// This is an empty project of DLL interface version 1.0


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"test");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = test(IMAGE,IMAGE)");
	definition->numofout = 1;
	definition->numofin = 2;
	definition->typesofin[0] = IMAGE; definition->typesofin[1] = IMAGE;
	definition->typesofout[0] = IMAGE;
	definition->interface_version = 1;
	strcpy_s(definition->description,MAX_DESC,"TEST");
	definition->ADDIP_version = 2.0;

}

void SetOutput(VARIABLES* outtab, VARIABLES* intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(ERROR_DEF* err, VARIABLES* intab,int nargin, VARIABLES* outtab, int nargout)
{

}