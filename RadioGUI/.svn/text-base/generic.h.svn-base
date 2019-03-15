/***********************************************************************/ 
/* Copyright (C) 2003 Definitive Solutions, Inc.  All Rights Reserved. */ 
/*                                                                     */
/* This program is free software; you can redistribute it and/or       */
/* modify it under the terms of the GNU General Public License as      */
/* published by the Free Software Foundation; either version 2 of the  */
/* License, or (at your option) any later version.                     */
/*                                                                     */
/* This program is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU   */
/* General Public License for more details.                            */
/*                                                                     */
/* You should have received a copy of the GNU General Public License   */
/* along with this program; if not, write to the Free Software         */
/* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA            */
/* 02111-1307  USA                                                     */
/***********************************************************************/ 

#if !defined(GENERICH_)
#define GENERICH_
#pragma once

// The Microsoft XML DOM.  Look in "C:\WinNT\System32".
#import "msxml.dll"
using namespace MSXML;

// Microsoft ADO.  Look in "C:\Program Files\Common Files\system\ado".
// "Unary minus operator applied to unsigned type, result still unsigned."
#pragma warning (disable : 4146)
#import "msado15.dll" rename("EOF", "adoEOF")
using namespace ADODB;



/////////////////////////////////////////////////////////////////////////////
// Disabled compiler warnings.

#pragma warning (disable : 4245)	// 'Conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
#pragma warning (disable : 4800)	// Forcing value to bool 'true' or 'false' (performance warning)


/////////////////////////////////////////////////////////////////////////////
// Colors.

#define RGB_BLACK			(RGB(  0,   0,   0))
#define RGB_WHITE			(RGB(255, 255, 255))
#define RGB_RED				(RGB(255,   0,   0))
#define RGB_RED_DARK		(RGB(128,   0,   0))
#define RGB_GREEN			(RGB(  0, 255,   0))
#define RGB_GREEN_DARK		(RGB(  0, 128,   0))
#define RGB_BLUE			(RGB(  0,   0, 255))
#define RGB_BLUE_DARK		(RGB(  0,   0, 128))
#define RGB_YELLOW			(RGB(255, 255, 128))
#define RGB_YELLOW_LIGHT	(RGB(255, 255, 192))
#define RGB_CYAN			(RGB(  0, 255, 255))
#define RGB_CYAN_LIGHT		(RGB(128, 255, 255))
#define RGB_CYAN_DARK		(RGB(  0, 128, 128))
#define RGB_PURPLE			(RGB(255,   0, 255))
#define RGB_PURPLE_DARK		(RGB(128,   0, 128))
#define RGB_GRAY			(RGB(192, 192, 192))
#define RGB_GRAY_DARK		(RGB(128, 128, 128))

#define  HLSMAX   240		// H,L, and S vary over 0-HLSMAX

#define  RGBMAX   255		// R,G, and B vary over 0-RGBMAX
							// HLSMAX BEST IF DIVISIBLE BY 6
							// RGBMAX, HLSMAX must each fit in a byte (255).


/////////////////////////////////////////////////////////////////////////////
// ASSERT Macros.

// This macro can be called at the beginning and ending of every
// method.  It is identical to saying "ASSERT_VALID(); ASSERT_KINDOF();"
// but is written like this so that VALIDATE can be a macro.  It is useful
// as an "early warning" that something has gone wrong with "this" object.
// It also checks the system error as a way to be sure all is well.
#ifndef VALIDATE
	#ifdef _DEBUG
	#define VALIDATE	{															\
								::AfxAssertValidObject(this, __FILE__ , __LINE__ );	\
								_ASSERTE(IsKindOf(GetRuntimeClass()));				\
							}
	#else
		#define VALIDATE
	#endif
#endif

// This fills a hole in MFC - we have ASSERT_VALID, ASSERT_NULL_OR_POINTER,
// but not this.  You should use this instead of ASSERT_NULL_OR_POINTER.
#ifndef ASSERT_NULL_OR_VALID
	#ifdef _DEBUG
	#define ASSERT_NULL_OR_VALID(p, type)											\
					{																\
						if (p) {													\
							::AfxAssertValidObject((p), __FILE__ , __LINE__ );		\
						}															\
					}
	#else
		#define ASSERT_NULL_OR_VALID(p, type)
	#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// EC ("Error Checking" Macros. The syntax is:
//
// Chars	Values			Meaning	
// -----	------			-------------------------------------------------
// 0 - 1	"EC_"			Required. "Error Checking". Underscore helps grepping.
//   2		"H", "B", "V"	Required. Function "fnx" return type.
//   3		"EF"			Optional. "fnx" throws exception of "xx" type.
//   4		"_"				Optional. Always execute "fnx", even if current 
//							          "hr" is a failure code.
//
// Since _ASSERTE() and other set the errno back to zero, we cache it.
/////////////////////////////////////////////////////////////////////////////

// This allows you to log the sent value on failure:
// EC_FAIL(s, sKey);
#define EC_FAIL(spec, var)													\
																			\
	if (FAILED(hr)) {														\
		DOMYLOGE ("   * " #var " is <%"#spec">.\n", (var));					\
	}

// This allows you to log the sent format and values on success.
// EC_SUCCEED(("Renamed <%s> to <%s>.\n", sPath1, sPath2));
// Note the double parentheses.
#define EC_SUCCEED(args)													\
																			\
	if (SUCCEEDED(hr)) {													\
		DOMYLOGA args ;														\
	}

// This version tests BOOLs.
#define EC_B(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		if (! (fnx)) {														\
			int nErrCache(::GetLastError());								\
			_ASSERTE(! "EC_B Error");										\
			hr = E_FAIL;													\
			DOMYLOGST(API_FAILURE), #fnx, nErrCache,						\
			  Generic::GetSysErrorString(nErrCache));						\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests BOOLs (ignores current error).
#define EC_B_(fnx)															\
																			\
	if (! (fnx)) {															\
		int nErrCache(::GetLastError());									\
		_ASSERTE(! "EC_B_ Error");											\
		hr = E_FAIL;														\
		DOMYLOGST(API_FAILURE), #fnx, nErrCache,							\
		  Generic::GetSysErrorString(nErrCache));							\
	}																		\

// This version tests HRESULTs.
#define EC_H(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		if (FAILED(hr = (fnx))) {											\
			_ASSERTE(! "EC_H Error");										\
			DOMYLOGST(API_FAILURE), #fnx, hr,								\
			  Generic::GetHrErrorString(hr));								\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests HRESULTs (ignores current error).
#define EC_H_(fnx)															\
																			\
	if (FAILED(hr = (fnx))) {												\
		_ASSERTE(! "EC_H_ Error");											\
		DOMYLOGST(API_FAILURE), #fnx, hr, Generic::GetHrErrorString(hr));	\
	}

// This is for 'fnx' that returns void.
#define EC_V(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
		fnx;																\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests void (ignores current error). (Mostly used to mark the 
// source code as "EC"-converted.
#define EC_V_(fnx)															\
																			\
	fnx;

// This version tests BOOLs, catches CDBException.
#define EC_BED(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
																			\
			if (! (fnx)) {													\
				int nErrCache(::GetLastError());							\
				_ASSERTE(! "EC_BED - CDBException error");					\
				hr = E_FAIL;												\
				DOMYLOGST(API_FAILURE), #fnx, nErrCache,					\
				  Generic::GetSysErrorString(nErrCache));					\
			}																\
		}																	\
		catch (CDBException* e) {											\
			_ASSERTE(! "EC_BED - CDBException");							\
			hr = E_FAIL;													\
			DOMYLOGST(API_FAILURE), #fnx, e->m_nRetCode,					\
			  e->m_strError + " " + e->m_strStateNativeOrigin);				\
			CString sError;													\
			VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));			\
			sError.ReleaseBuffer();											\
			e->ReportError();												\
			e->Delete();													\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests BOOLs, catches CFileException.
#define EC_BEF(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
																			\
			if (! (fnx)) {													\
				int nErrCache(::GetLastError());							\
				_ASSERTE(! "EC_BEF - CFileException error");				\
				hr = E_FAIL;												\
				DOMYLOGST(API_FAILURE), #fnx, nErrCache,					\
				  Generic::GetSysErrorString(nErrCache));					\
			}																\
		}																	\
		catch (CFileException* e) {											\
			_ASSERTE(! "EC_BEF - CFileException");							\
			::SetLastError(e->m_lOsError);									\
			hr = E_FAIL;													\
			DOMYLOGST(API_FAILURE), #fnx, e->m_lOsError,					\
			  Generic::GetSysErrorString(e->m_lOsError));					\
			CString sError;													\
			VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));			\
			sError.ReleaseBuffer();											\
			DOMYLOGE ("EC_BEF CFileException: <%d> - <%s>\n",				\
			  e->m_cause, sError);											\
			e->ReportError();												\
			e->Delete();													\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests HRESULTs, catches _com_error exceptions.
#define EC_HEC(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			if (FAILED(hr = (fnx))) {										\
				_com_issue_error(hr);										\
			}																\
		}																	\
		catch(_com_error &e) {												\
			_ASSERTE(! "EC_HEC - _com_error exception");					\
																			\
			if (SUCCEEDED(hr)) hr = e.WCodeToHRESULT(e.WCode());			\
			if (SUCCEEDED(hr)) hr = e.Error();								\
			if (SUCCEEDED(hr)) hr = E_FAIL;									\
																			\
			DOMYLOGST(API_FAILURE), #fnx, hr,								\
			  Generic::GetHrErrorString(hr));								\
																			\
			Generic::PrintComError(e);										\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests HRESULTs, catches _com_error exceptions. Send it a
// _ConnectionPtr as the active connection.
#define EC_HEC_C(fnx, pconn)												\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			if (FAILED(hr = (fnx))) {										\
				_com_issue_error(hr);										\
			}																\
		}																	\
		catch(_com_error &e) {												\
			_ASSERTE(! "EC_HEC_C - _com_error exception");					\
																			\
			if (SUCCEEDED(hr)) hr = e.WCodeToHRESULT(e.WCode());			\
			if (SUCCEEDED(hr)) hr = e.Error();								\
			if (SUCCEEDED(hr)) hr = E_FAIL;									\
																			\
			DOMYLOGST(API_FAILURE), #fnx, hr,								\
			  Generic::GetHrErrorString(hr));								\
																			\
			Generic::PrintComError(e);										\
																			\
			if (pconn) {													\
				_variant_t vtConnect = pconn;								\
																			\
				if (VT_DISPATCH == vtConnect.vt) {							\
					Generic::PrintProviderError(vtConnect);					\
				}															\
			}																\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests HRESULTs, catches _com_error exceptions. Send it a
// _RecordsetPtr as prs to get the active connection from.
#define EC_HEC_RS(fnx, prs)													\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			if (FAILED(hr = (fnx))) {										\
				_com_issue_error(hr);										\
			}																\
		}																	\
		catch(_com_error &e) {												\
			_ASSERTE(! "EC_HEC_RS - _com_error exception");					\
																			\
			if (SUCCEEDED(hr)) hr = e.WCodeToHRESULT(e.WCode());			\
			if (SUCCEEDED(hr)) hr = e.Error();								\
			if (SUCCEEDED(hr)) hr = E_FAIL;									\
																			\
			DOMYLOGST(API_FAILURE), #fnx, hr,								\
			  Generic::GetHrErrorString(hr));								\
																			\
			Generic::PrintComError(e);										\
																			\
			if (NULL != prs  &&  adStateOpen == prs->GetState()) {			\
				_variant_t vtConnect = prs->GetActiveConnection();			\
																			\
				if (VT_DISPATCH == vtConnect.vt) {							\
					Generic::PrintProviderError(vtConnect);					\
				}															\
			}																\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests HRESULTs, catches CFile Exception.
#define EC_HEF(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
																			\
			if (FAILED(hr = (fnx))) {										\
				_ASSERTE(! "EC_HEF CFileException error");					\
				DOMYLOGST(API_FAILURE), #fnx, hr,							\
				  Generic::GetHrErrorString(hr));							\
			}																\
		}																	\
		catch (CFileException* e) {											\
			_ASSERTE(! "EC_HEF - CFileException");							\
			::SetLastError(e->m_lOsError);									\
			if (SUCCEEDED(hr)) hr = E_FAIL;									\
			DOMYLOGST(API_FAILURE), #fnx, e->m_lOsError,					\
			  Generic::GetSysErrorString(e->m_lOsError));					\
			CString sError;													\
			VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));			\
			sError.ReleaseBuffer();											\
			DOMYLOGE ("EC_HEF CFileException: <%d> - <%s>\n",				\
			  e->m_cause, sError);											\
			e->ReportError();												\
			e->Delete();													\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests voids, catches _com_error exceptions, and ignores
// current error.
#define EC_VEC_(fnx)														\
																			\
	try {																	\
		fnx;																\
	}																		\
	catch(_com_error &e) {													\
		_ASSERTE(! "EC_VEC_ - _com_error exception");						\
																			\
		if (SUCCEEDED(hr)) hr = e.WCodeToHRESULT(e.WCode());				\
		if (SUCCEEDED(hr)) hr = e.Error();									\
		if (SUCCEEDED(hr)) hr = E_FAIL;										\
																			\
		DOMYLOGST(API_FAILURE), #fnx, hr,									\
		  Generic::GetHrErrorString(hr));									\
																			\
		Generic::PrintComError(e);											\
	}


// This version tests voids, catches _com_error exceptions. Send it a
// _ConnectionPtr as the active connection.
#define EC_VEC_C(fnx, pconn)												\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			fnx;															\
		}																	\
		catch(_com_error &e) {												\
			_ASSERTE(! "EC_VEC_C - _com_error exception");					\
																			\
			if (SUCCEEDED(hr)) hr = e.WCodeToHRESULT(e.WCode());			\
			if (SUCCEEDED(hr)) hr = e.Error();								\
			if (SUCCEEDED(hr)) hr = E_FAIL;									\
																			\
			DOMYLOGST(API_FAILURE), #fnx, hr,								\
			  Generic::GetHrErrorString(hr));								\
																			\
			Generic::PrintComError(e);										\
																			\
			if (pconn) {													\
				_variant_t vtConnect = pconn;								\
																			\
				if (VT_DISPATCH == vtConnect.vt) {							\
					Generic::PrintProviderError(vtConnect);					\
				}															\
			}																\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests voids, catches _com_error exceptions. Send it a 
// _RecordsetPtr as prs to get the active connection from.
#define EC_VEC_RS(fnx, prs)													\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			fnx;															\
		}																	\
		catch(_com_error &e) {												\
			_ASSERTE(! "EC_VEC_RS - _com_error exception");					\
																			\
			if (SUCCEEDED(hr)) hr = e.WCodeToHRESULT(e.WCode());			\
			if (SUCCEEDED(hr)) hr = e.Error();								\
			if (SUCCEEDED(hr)) hr = E_FAIL;									\
																			\
			DOMYLOGST(API_FAILURE), #fnx, hr,								\
			  Generic::GetHrErrorString(hr));								\
																			\
			Generic::PrintComError(e);										\
																			\
			if (NULL != prs  &&  adStateOpen == prs->GetState()) {			\
				_variant_t vtConnect = prs->GetActiveConnection();			\
																			\
				if (VT_DISPATCH == vtConnect.vt) {							\
					Generic::PrintProviderError(vtConnect);					\
				}															\
			}																\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests void, catches CDBExceptions, and ignores the current
// error.
#define EC_VED_(fnx)														\
																			\
	try {																	\
		fnx;																\
	}																		\
	catch (CDBException* e) {												\
		_ASSERTE(! "EC_VED - CDBException");								\
		hr = E_FAIL;														\
		DOMYLOGST(API_FAILURE), #fnx, e->m_nRetCode,						\
		  e->m_strError + " " + e->m_strStateNativeOrigin);					\
		CString sError;														\
		VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));				\
		sError.ReleaseBuffer();												\
		e->ReportError();													\
		e->Delete();														\
	}

// This version tests void, catches CDBExceptions.
#define EC_VED(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			fnx;															\
		}																	\
		catch (CDBException* e) {											\
			_ASSERTE(! "EC_VED - CDBException");							\
			hr = E_FAIL;													\
			DOMYLOGST(API_FAILURE), #fnx, e->m_nRetCode,					\
			  e->m_strError + " " + e->m_strStateNativeOrigin);				\
			CString sError;													\
			VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));			\
			sError.ReleaseBuffer();											\
			e->ReportError();												\
			e->Delete();													\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests void, catches CFileExceptions.
#define EC_VEF(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			fnx;															\
		}																	\
		catch (CFileException* e) {											\
			_ASSERTE(! "EC_VEF - CFileException");							\
			::SetLastError(e->m_lOsError);									\
			hr = E_FAIL;													\
			DOMYLOGST(API_FAILURE), #fnx, e->m_lOsError,					\
			  Generic::GetSysErrorString(e->m_lOsError));					\
			CString sError;													\
			VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));			\
			sError.ReleaseBuffer();											\
			DOMYLOGE ("EC_VEF CFileException: <%d> - <%s>\n",				\
			  e->m_cause, sError);											\
			e->ReportError();												\
			e->Delete();													\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests void, catches CMemoryExceptions.
#define EC_VEM(fnx)															\
																			\
	if (SUCCEEDED(hr)) {													\
																			\
		try {																\
			fnx;															\
		}																	\
		catch (CMemoryException* e) {										\
			int nErrCache(::GetLastError());								\
			_ASSERTE(! "EC_VEM - CMemoryException");						\
			hr = E_FAIL;													\
			CString sError;													\
			VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));			\
			sError.ReleaseBuffer();											\
			DOMYLOGE ("EC_VEM CMemoryException: <%d> - <%s>\n",				\
			  nErrCache, sError);											\
			e->ReportError();												\
			e->Delete();													\
		}																	\
	}																		\
	else {																	\
		DOMYLOGE ("EC_ skipped executing <%s>.\n", #fnx);					\
	}

// This version tests void (ignores current error), catches CFileException.
#define EC_VEF_(fnx)														\
																			\
	try {																	\
		fnx;																\
	}																		\
	catch (CFileException* e) {												\
		_ASSERTE(! "EC_VEF_ - CFileException error");						\
		::SetLastError(e->m_lOsError);										\
		if (SUCCEEDED(hr)) hr = E_FAIL;										\
		DOMYLOGST(API_FAILURE), #fnx, e->m_lOsError,						\
		  Generic::GetSysErrorString(e->m_lOsError));						\
		CString sError;														\
		VERIFY(e->GetErrorMessage(sError.GetBuffer(512), 512));				\
		sError.ReleaseBuffer();												\
		DOMYLOGE ("EC_VEF_ CFileException: <%d> - <%s>\n",					\
		  e->m_cause, sError);												\
		e->ReportError();													\
		e->Delete();														\
	}																		\

// This version tests void (ignores current error), catches
// COleDispatchException.
#define EC_VEOD_(fnx)														\
																			\
	try {																	\
		fnx;																\
	}																		\
	catch (COleDispatchException* e) {										\
		_ASSERTE(! "EC_VEOD_ - COleDispatchException error");				\
		if (SUCCEEDED(hr)) hr = e->m_wCode;									\
		if (SUCCEEDED(hr)) hr = E_FAIL;										\
		DOMYLOGST(API_FAILURE), #fnx, hr, Generic::GetHrErrorString(hr));	\
		CString sError(e->m_strDescription);								\
		DOMYLOGE ("EC_VEOD_ COleDispatchException: <%d> - <%s>\n", hr,		\
		  sError);															\
		e->ReportError();													\
		e->Delete();														\
	}																		\



/////////////////////////////////////////////////////////////////////////////
// Generic class - a hodge-podge of utility functions

// Declare depending on whether this file is in an EXE or a DLL.
class
	#ifdef _WINDLL
	AFX_EXT_CLASS
	#endif
Generic
{
// Construction.
public:
	Generic();
	virtual ~Generic();

// Declared but not defined.
private:
	Generic(const Generic& rhs);
	Generic& operator=(const Generic& rhs);

// Interface.
public:
	static int			GetHorzPixels(CWnd* pWnd, int nDlgUnits);
	static int			GetVertPixels(CWnd* pWnd, int nDlgUnits);
	
	static UINT			SpinTheMessageLoop(bool bNoDrawing = false,
							bool bOnlyDrawing = false,
							UINT uiMsgAllowed = WM_NULL);

	static bool			TranslateEnvPath(CString& sPathRaw);
	static CString		IntToCommas(int nInt);
	static CString		DoubleToCommas(double dDouble, int nDigitsAfterDecimal = 2);
	static int			CommasToInt(const CString& sCommas);
	static double		CommasToDouble(const CString& sCommas);
	static double		RoundDouble(double dDouble, int nDigitsAfterDecimal = 2);
	static CString		StripNonNumeric(const CString& s);
	static CString		RemoveAllExcept(const CString& sTarget, const CString& sValidChars);
	static CString		StripNonAlphaNumeric(const CString& s);
	static double		Load80BitLongDoubleTo64BitDouble(CArchive& ar);
	static _variant_t	VariantDecimalFromString(const CString& s);
	static CString		LoadString(UINT uiId);
	static void			ParseNumericList(CStringList& sl, const CString& s);
	static void			ParseToWords(const CString& s, CStringList& sl);
	static bool			SortCStringList(CStringList& sl, bool bAscending);
	static void			SortCStringArray(CStringArray& sa, bool bAscending);

	static bool			AddUniqueToCStringList(CStringList& sl,
							const CString& sText);

	static void			CopyCStringArray(const CStringArray& saSource,
							CStringArray& saTarget);

	static void			CopyCStringList(const CStringList& slSource,
							CStringList& slTarget);

	static void			CopyCDWordArray(const CDWordArray& dwaSource,
							CDWordArray& dwaTarget);

	static int			CompareDottedString(const CString s1, const CString s2);

	static void			LeadFillWithChar(CString& s, int nTotalLen,
							const CString& sChar);
	
	static bool			GetFromIniFile(const CString& sIniFile,
							const CString& sSection, const CString& sEntry,
							const CString& sDefault,CString& sData,
							int nSize = MAX_PATH);

	static bool			IsAllDigits(const CString& s);

	static CString		TitleFromPath(const CString& sPath);
	static CString		DirFromPath(const CString& sPath);
	static CString		DirAndFileFromPath(const CString& sPath);
	static CString		FileFromPath(const CString& sPath);
	static CString		DriveFromPath(const CString& sPath);
	static CString		ExtFromPath(const CString& sPath);
	
	static CString		RelativeFromBase(const CString& sBaseDir,
							const CString& sPath);

	static bool			IsFile(const CString& sPath);
	static bool			GetFileSize(const CString& sPath, DWORD& siz);
	static bool			IsDirectory(const CString& sPath);
	static int			CreateDirectory(const CString& sPath);
	static int			GetSubDirCount(const CString& sPath);

	static int			PointsToLogicalHeight(const CDC* pDC, int nPoints);

	static int			GetJulianDayNumber(const CTime& tim);
	static CTimeSpan	TimeSpanFromSeconds(int nSeconds);
	static int			GetDaysInMonth(int nMM, int nYYYY);
	static bool			IsLeapYear(int nYYYY);
	static COleDateTime	AddMonthsToDate(const COleDateTime& odt, int nMonths);

	static CString		MillisecsToString(DWORD dwMS);
	static void			QuickExit(const CString& sFile, int nLine);
	static CString		GetSysErrorString(int nErrSent = 0);
	static CString		GetHrErrorString(HRESULT hr);

	// If this method fails to compile, you may not have a recent version
	// of MSXML.DLL on your machine.  Verion 4.72 is known to be too early;
	// version 8.00 is known to work.
	static CString		GetXMLDOMErrorString(
							const MSXML::IXMLDOMDocumentPtr& spXMLDOMDocument);

	static bool			RegisterRemoteServerLocally(const CString& sRemoteName,
							const CString& sServerPath, const bool& bRegister = true);

	static void			PrintComError(_com_error &e);
	static void			PrintProviderError(_ConnectionPtr pConnection);
	
	static int			ReverseFindOneOf(const CString& s, const CString& sSub,
							int nStart = -1);

	static int			FindNoCase(const CString& sBig, const CString& sSmall);

	static void			RGBtoHLS(COLORREF crRGB, WORD& wH, WORD& wL, WORD& wS);
	static COLORREF		HLStoRGB(WORD wH, WORD wL, WORD wS);
	static WORD			HueToRGB(WORD w1, WORD w2, WORD wH);
	static COLORREF		FlipRGB(COLORREF cf);

	static bool			AppendFiles(const CString& sPath1, const CString& sPath2);
	static bool			RenameFile(const CString& sPath1, const CString& sPath2);
	static bool			RemoveFile(const CString& sPath, int& nErr);
	static HRESULT		TrimFileToSize(const CString& sFileName, DWORD dwMaxLogSize);

	static bool			RemoveTrailingSlash(CString& sPath);

	static HRESULT		SaveWindowPlacement(const CWnd* pWnd,
							const CString& sPlacementKey,
							const CString& sPlacementSubKey);

	static int			RestoreWindowPlacement(CWnd* pWnd,
							const CString& sPlacementKey,
							const CString& sPlacementSubKey,
							int nShowWindowCmd = -1);

	static unsigned long	GetCRC(PBYTE pbyBuf, long lnLen);

	static void			PlayWavResource(const CString& sWavName);
	
	static void			WhichFontsInstalled(bool& bSmallFonts,
							bool& bLargeFonts,
							bool& bNeitherButCloserToSmallFonts);

	static void			LogRegistryTree(HKEY hRootKey,
							const CString& sKey, int& nDepth);

// Implementation.
private:

	static bool			SortCStringArrayHelper(CStringArray& sa, int nPos,
							bool bAscending);
};

#endif	// GENERICH_