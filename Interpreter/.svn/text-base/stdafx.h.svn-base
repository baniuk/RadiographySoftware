// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

#include "C_InterpBase.h"
#include "C_Command_Load.h"
#include "C_Command_Sin.h"
#include "C_Command_Add.h"
#include "C_Command_Imread.h"
#include "C_Command_Imshow.h"
#include "C_Command_Imclose.h"
#include "C_Command_Erode.h"
#include "C_Command_Dilate.h"
#include "C_Command_SaveImageM.h"
#include "C_Command_Filter2d.h"
#include "C_Command_Imadd.h"
#include "C_Command_Imsub.h"
#include "C_Command_Immulti.h"
#include "C_Command_GLshow.h"
#include "C_Command_Th.h"
#include "C_Command_Outline.h"

#include "Interpreter.h"
#include <string.h>
#include "C_Error.h"
#include "C_Line.h"
#include "C_Variables.h"
#include "C_Variables_Number.h"
#include "C_Variables_String.h"
#include "C_Variables_Matrix.h"
#include "C_Variables_Image.h"
#include "C_Matrix_Container.h"
#include "C_Image_Container.h"
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <gl\gl.h>
#include <gl\glaux.h>
#include "tiffio.h"
// TODO: reference additional headers your program requires here
