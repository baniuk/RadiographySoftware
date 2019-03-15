#pragma once
#define _AFXDLL
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#include <windows.h>

#include "DLL_Cpp_Datatypes.h"
// Put your includes here 
C_Variables *(*globals)[MAX_GLOBALS];
int *lastglobal;

void Introduce(FUNC_DEF* definition);
void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout);
void SetOutput(C_Variables** outtab, C_Variables** intab);

