// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _AFXDLL
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _USE_MATH_DEFINES
//#define _M_AMD64
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

#include <windows.h>
#include <math.h>
//#include <iostream>


//#include <string.h>
#include "C_Error.h"
#include "C_Variables.h"
#include "C_Variables_Number.h"
#include "C_Variables_String.h"
#include "C_Variables_Image.h"
#include "C_Variables_Matrix.h"
#include "C_Matrix_Container.h"
#include "C_Image_Container.h"
#include "C_MultiProcesor.h"
// TODO: reference additional headers your program requires here