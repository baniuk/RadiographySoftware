#pragma once
#define _AFXDLL
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#include <windows.h>

#include "DLL_C_Datatypes.h"
// Put your includes here 

void Introduce(FUNC_DEF* definition);
void Do(ERROR_DEF* err, VARIABLES* intab,int nargin, VARIABLES* outtab, int nargout);
void SetOutput(VARIABLES* outtab, VARIABLES* intab);