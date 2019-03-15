#pragma once
#include "C_Commands.h"
#include "DLL_C_Datatypes.h"

//typedef void (__cdecl* DLL_DO_INT1)(ERROR_DEF*, VARIABLES*,int, VARIABLES*, int);
//typedef void (__cdecl* DLL_INTR)(FUNC_DEF*);

class C_Command_DLL :
	public C_Commands
{
public:
	C_Command_DLL(void);
	C_Command_DLL(char* name, char* programpatch, C_Error &err);
	virtual void Do(C_Error& err);
	virtual ~C_Command_DLL(void);
private:
	HINSTANCE mylib; // uchwyt do DLL dla funkcji ładowanych z DLL
	DLL_INTR Introduce;
	DLL_AG AssignGlobals;
	DLL_DO_INT1 Doo1;
	DLL_DO_INT2 Doo2;
	DLL_SET_OUTPUT SetOutput;
	unsigned int interface_version;
	FUNC_DEF data;
	VARIABLES *inputstruct;
	VARIABLES *outputstruct;
	BOOL czyglobals;
	 
};
