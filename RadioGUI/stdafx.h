// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0502		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0502	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

/*
#if (_MSC_VER >= 1300) 
//VC7 or later, building with pre-VC7 runtime libraries
extern "C" long _ftol( double ); //defined by VC6 C libs
extern "C" long _ftol2( double dblSource ) { return _ftol( dblSource ); }
#endif
*/
//extern "C" long _ftol( double ); //defined by VC6 C libs
//extern "C" long _ftol2( double dblSource ) { return _ftol( dblSource ); }

 ///FORCE:MULTIPLE 
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define _USE_MATH_DEFINES
//#define _M_AMD64
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define VERSION "2.6.3"
//#define PRZEMEK._VERSION

#include "..\interpreter\C_InterpBase.h"
#include "..\interpreter\C_Command_Load.h"
#include "..\interpreter\C_Command_Sin.h"
#include "..\interpreter\C_Command_Add.h"
#include "..\interpreter\C_Command_Imread.h"
#include "..\interpreter\C_Command_Imshow.h"
#include "..\interpreter\C_Command_Imclose.h"
#include "..\interpreter\C_Command_Erode.h"
#include "..\interpreter\C_Command_Dilate.h"
#include "..\interpreter\C_Command_SaveImageM.h"
#include "..\interpreter\C_Command_Filter2d.h"
#include "..\interpreter\C_Command_Imadd.h"
#include "..\interpreter\C_Command_Imsub.h"
#include "..\interpreter\C_Command_Immulti.h"
#include "..\interpreter\C_Command_GLshow.h"
#include "..\interpreter\C_Command_Th.h"
#include "..\interpreter\C_Command_Hist.h"
#include "..\interpreter\C_Command_Outline.h"
#include "..\interpreter\C_Command_Bdilate.h"
#include "..\interpreter\C_Command_Berode.h"
#include "..\interpreter\C_Command_Gth.h"
#include "..\interpreter\C_Command_Particals.h"
#include "..\interpreter\C_Command_Ith.h"
#include "..\interpreter\C_Command_Median.h"
#include "..\interpreter\C_Command_Oth.h"
#include "..\interpreter\C_Command_Fftfilt.h"
#include "..\interpreter\C_Command_Eth.h"
#include "..\interpreter\C_Command_Fillholes.h"
#include "..\interpreter\C_Command_Chdir.h"
#include "..\interpreter\C_Command_Nth.h"
#include "..\interpreter\C_Command_Sth.h"
#include "..\interpreter\C_Command_Imwrite.h"
#include "..\interpreter\C_Command_Neural.h"
#include "..\interpreter\C_Command_Matrixread.h"
#include "..\interpreter\C_Command_Matrixwrite.h"
#include "..\interpreter\C_Command_Cutroi.h"
#include "..\interpreter\C_Command_FastMedianqs.h"
#include "..\interpreter\C_Command_Features.h"
#include "..\interpreter\C_Command_Savefeatures.h"
#include "..\interpreter\C_Command_Showfeatures.h"
#include "..\interpreter\C_Command_Prewitt.h"
#include "..\interpreter\C_Command_Sobel.h"
#include "..\interpreter\C_Command_Roberts.h"
#include "..\interpreter\C_Command_Annclasifier.h"
#include "..\interpreter\C_Command_Normalize.h"
#include "..\interpreter\C_Command_Exit.h"
#include "..\interpreter\C_Command_Dm.h"
#include "..\interpreter\C_Command_DLL.h"
#include "..\interpreter\C_Command_Thread.h"
#include "..\interpreter\C_Commans_ISeePort.h"

//#include "Interpreter.h"
#include <string.h>
#include "..\interpreter\C_Error.h"
#include "..\interpreter\C_Line.h"
#include "..\interpreter\C_Variables.h"
#include "..\interpreter\C_Variables_Number.h"
#include "..\interpreter\C_Variables_String.h"
#include "..\interpreter\C_Variables_Image.h"
#include "..\interpreter\C_Variables_Matrix.h"
#include "..\interpreter\C_Matrix_Container.h"
#include "..\interpreter\C_Image_Container.h"
#include "C_DLL_Operation.h"
#include <math.h>
#include <cv.h>
#include <highgui.h>
//#include <cxcore.h>
#include "tiffio.h"
#include <gl.h>
#include <glaux.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "_OpenGL.h"
#include "_OpenGL_Hist.h"
#include <direct.h>
#include "C_CommandStack.h"
