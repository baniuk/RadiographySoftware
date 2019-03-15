/***********************************************************************/ 
/* Copyright (C) 2002 Definitive Solutions, Inc.  All Rights Reserved. */ 
/* THIS COMPUTER PROGRAM IS PROPRIETARY AND CONFIDENTIAL TO DEFINITIVE */ 
/* SOLUTIONS, INC. AND ITS LICENSORS AND CONTAINS TRADE SECRETS OF     */ 
/* DEFINITIVE SOLUTIONS, INC. THAT ARE PROVIDED PURSUANT TO A WRITTEN  */ 
/* AGREEMENT CONTAINING RESTRICTIONS ON USE AND DISCLOSURE.  ANY USE,  */ 
/* REPRODUCTION, OR TRANSFER EXCEPT AS PROVIDED IN SUCH AGREEMENT      */ 
/* IS STRICTLY PROHIBITED.                                             */ 
/***********************************************************************/ 

// Created by Larry Leonard, Definitive Solutions, Inc.

#include "stdafx.h"

#include "Generic.h"
#include "MyApp.h"
#include "MyRegistry.h"
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>										// GetFileSize
#include <mmsystem.h>								// sndPlaySound
#include <afxdb.h>									// CDBException
#include <afxdisp.h>								// COleDateTime

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// Generic

// This corrects a problem with the ::GetDialogBaseUnits() call, which only
// works for the system font (which no one ever uses in dialogs anymore).
// Call from your view's OnSize() method, passing "this" as the first arg.
// Note that the mult and div are done in separate steps deliberately, in
// order to GUARANTEE the order of evaluation (parentheses DO NOT affect
// the order of evaluation for operators of the same precedence level in C,
// although they do in C++).
/* static */ int Generic::GetHorzPixels(CWnd* pWnd,
										int	  nDlgUnits)
{
	ASSERT_VALID(pWnd);
	_ASSERTE(0 <= nDlgUnits);

	HRESULT hr(S_OK);
	int lnReturnPixels(0);

	if (pWnd  &&  ::IsWindow(pWnd->GetSafeHwnd())) {
		CRect r(0, 0, 4, 8);
		EC_B(::MapDialogRect(pWnd->GetSafeHwnd(), r));
		EC_FAIL(p, pWnd);

		if (SUCCEEDED(hr)) {
			lnReturnPixels = (nDlgUnits * r.right);
			lnReturnPixels /= 4L;
		}
	}

	return lnReturnPixels;
}

// This corrects a problem with the ::GetDialogBaseUnits() call, which only
// works for the system font (which no one ever uses in dialogs anymore).
// Call from your view's OnSize() method, passing "this" as the first arg.
// Note that the mult and div are done in separate steps deliberately, in
// order to GUARANTEE the order of evaluation (parenthese DO NOT affect
// the order of evaluation for operators of the same precedence level in C,
// although they do in C++).
/* static */ int Generic::GetVertPixels(CWnd* pWnd,
										int   nDlgUnits)
{
	ASSERT_VALID(pWnd);
	_ASSERTE(0 <= nDlgUnits);

	HRESULT hr(S_OK);
	int lnReturnPixels(0);

	if (pWnd  &&  ::IsWindow(pWnd->GetSafeHwnd())) {
		CRect r(0, 0, 4, 8);
		EC_B(::MapDialogRect(pWnd->GetSafeHwnd(), r));
		EC_FAIL(p, pWnd);

		if (SUCCEEDED(hr)) {
			lnReturnPixels = (nDlgUnits * r.bottom);
			lnReturnPixels /= 8L;
		}
	}

	return lnReturnPixels;
}

// Spin The Message Loop: C++ version.  See "Advanced Windows Programming", 
// M. Heller, p. 153, and the MS TechNet CD, PSS ID Number: Q99999.
/* static */ UINT Generic::SpinTheMessageLoop(bool bNoDrawing   /* = false */ ,
											  bool bOnlyDrawing /* = false */ ,
											  UINT uiMsgAllowed /* = WM_NULL */ )
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(msg));

	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

		// Extra info for Debug mode only.
		#if 0  // #ifdef  _DEBUG
		static DWORD dwSpin(0);
		CString sMsg;

		switch (msg.message) {
			case 0x000F: sMsg = "WM_PAINT"; break;
			case 0x00A0: sMsg = "WM_NCMOUSEMOVE"; break;
			case 0x0101: sMsg = "WM_KEYUP"; break;
			case 0x0104: sMsg = "WM_SYSKEYDOWN"; break;
			case 0x0111: sMsg = "WM_COMMAND"; break;
			case 0x0113: sMsg = "WM_TIMER"; break;
			case 0x0200: sMsg = "WM_MOUSEMOVE"; break;
			default: sMsg = "{Unknown}"; break;
		}

		TRACE("   Generic.cpp(%d): >>> SpinOnce <%08d> msg <%04x>: <%s>.\n",
		  __LINE__, dwSpin++, msg.message, sMsg);
		#endif

		// Do painting only.
		if (bOnlyDrawing  &&  WM_PAINT == msg.message) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		// Do everything *but* painting.
		else if (bNoDrawing  &&  WM_PAINT == msg.message) {
			break;
		}
		// Special handling for this message.
		else if (WM_QUIT == msg.message) {
			::PostQuitMessage(msg.wParam);
			break;
		}
		// Allow one message (like WM_LBUTTONDOWN).
		else if (uiMsgAllowed == msg.message
		  &&  ! AfxGetApp()->PreTranslateMessage(&msg)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			break;
		}
		// This is the general case.
		else if (! bOnlyDrawing  &&  ! AfxGetApp()->PreTranslateMessage(&msg)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}

	// Update user interface, then free temporary objects.
	AfxGetApp()->OnIdle(0);
	AfxGetApp()->OnIdle(1);

	return msg.message;
}

// Does the path in the Registry contain something like %windir% 
// that requires translation?
/* static */ bool Generic::TranslateEnvPath(CString& sPathRaw)
{
	_ASSERTE(! sPathRaw.IsEmpty());

	HRESULT hr(S_OK);
	CString sPath;
	int nFirstPercent(0);
	bool bReturn(true);

	while (-1 != (nFirstPercent = sPathRaw.Find("%"))) {
		sPath += sPathRaw.Mid(0, nFirstPercent - 1);

		// Extract the %Mumble% sub-string.
		CString sSub(sPathRaw.Mid(nFirstPercent + 1));
		int nSecondPercent(sSub.Find("%"));
		sSub = sSub.Mid(0, nSecondPercent);
		sPathRaw = sPathRaw.Mid(nSecondPercent + 2);

		// Translate the environment variable ("mumble").
		CString sBuffer;
		DWORD dwRet(0UL);
		DWORD dwBufferSize(128);

		EC_B(dwBufferSize > (dwRet = ::GetEnvironmentVariable(
		  sSub, sBuffer.GetBuffer(dwBufferSize), dwBufferSize)));
		sBuffer.ReleaseBuffer();
		EC_FAIL(s, sPathRaw);
		EC_FAIL(d, dwRet);

		if (FAILED(hr)) {
			bReturn = false;
			break;
		}

		// Append the translated substring to the return path.
		sPath += sBuffer;
	}

	// Append whatever is left over and return.
	sPath += sPathRaw;
	sPathRaw = sPath;

	return bReturn;
}

// This handles the problem of, how does a 32-bit program de-serialize a 
// "long double" that was serialized years ago by a 16-bit program.  ("Long
// double" in 16-bit is 80 bits; in 32-bit, its 64 bits!  Go figure.)
// 
// From Q129209: This moves the contents of the buffer into the floating point
// register, which then then takes care of the automatic convertion back to a
// 8-byte long double.
/* static */ double Generic::Load80BitLongDoubleTo64BitDouble(CArchive& ar)
{
	double dReturn(0.0);
	
	BYTE byInteger[10];
	ar.Read(byInteger, 10);

	_asm {
		fld TBYTE PTR byInteger;
		fstp dReturn;
	}

	return dReturn;
}

// Converts a CString to a _variant_t of type VT_DECIMAL.
/* static */ _variant_t Generic::VariantDecimalFromString(const CString& s)
{
	DECIMAL d;
	DECIMAL_SETZERO(d);
	
	d.Lo64 = _atoi64(s);
	_variant_t varDecimal(d);

	return varDecimal;
}

// This simply loads the string from the string table, but has two advantages.
// First, it does error checking, which would normally never get done, but is
// important in localized applications; and, it takes only one line of code in
// the caller instead of two (with minimal overhead).
/* static */ CString Generic::LoadString(UINT uiId)
{
	_ASSERTE(0U < uiId);

	CString s;

	// Note that if this fails, we no longer trust the string table.  The user
	// will just have to buy and English dictionary.  Note too that the caller
	// can determine if this function failed by noticing that the return string
	// begins with the string id, following by three colons, which should be a
	// pretty unusual legitimate string to be in the string table.
	if (! s.LoadString(uiId)) {
		s.Format("%d::: Error loading string from string table!", uiId);
		::MessageBeep(MB_ICONEXCLAMATION);
		DOMYLOGE (s + "\n");
		_ASSERTE(! "Error loading string from string table!");
	}

	return s;
}

// Given a CString of numeric substrings (separated by any non-digit character)
// return the sent CStringList with the numeric substrings appended as separate
// strings.
/* static */ void Generic::ParseNumericList(CStringList&   sl,
											const CString& s)
{
	_ASSERTE(! s.IsEmpty());

	CString sList(s);

	while (0 < sList.GetLength()) {
		CString sDross(sList.SpanExcluding("0123456789"));
		int nLen(sDross.GetLength());
		sList = sList.Mid(nLen);
		CString sSub(sList.SpanIncluding("0123456789"));
		nLen = sSub.GetLength();
		sList = sList.Mid(nLen);

		if (0 < sSub.GetLength()) {
			sl.AddTail(sSub);
		}
	}
}

// Given a CString of whitespace-separated words, parse them into a CStringList.
/* static */ void Generic::ParseToWords(const CString& s,
										CStringList&   sl)
{
	CString sWord;

	for (int nChar = 0; nChar < s.GetLength(); ++nChar) {
		char c(s.GetAt(nChar));

		if (! ::isspace(c)) {
			sWord += c;
		}
		else {

			if (! sWord.IsEmpty()) {
				sl.AddTail(sWord);
				sWord.Empty();
			}
		}
	}

	if (! sWord.IsEmpty()) {
		sl.AddTail(sWord);
	}
}

// Sort the sent string list in the order specified.
/* static */ bool Generic::SortCStringList(CStringList& sl,
										   bool         bAscending)
{
	bool bReturn(true);

	// Create the sorting listbox control. Hopefully, the DlgId is not already
	// in use!
	CListBox lbSorting;
	CRect rcListbox(10,10,100,100);

	if (lbSorting.Create(WS_CHILD | LBS_SORT, rcListbox, AfxGetMainWnd(), 19998)) {

		// Insert the string list into the listbox.
		POSITION pos(sl.GetHeadPosition());

		while (pos  &&  bReturn) {
			CString s(sl.GetNext(pos));
			int nAddString(lbSorting.AddString(s));

			switch (nAddString) {

				case LB_ERR:
				_ASSERTE(! "Error");
				DOMYLOGE ("AddString failed for string <%s> with LB_ERR.\n", s);
				bReturn = false;
				break;

				case LB_ERRSPACE:
				_ASSERTE(! "Error");
				DOMYLOGE ("AddString failed for string <%s> with LB_ERRSPACE.\n", s);
				bReturn = false;
				break;

				default:
				break;
			}
		}

		// Now, clear the string list and dump the listbox to it.  This is where
		// we specify the sort order.
		sl.RemoveAll();

		for (int nItem = 0; nItem < lbSorting.GetCount(); ++nItem) {
			CString s;
			lbSorting.GetText(nItem, s);
			bAscending ? sl.AddTail(s) : sl.AddHead(s);
		}
	}
	else {
		_ASSERTE(! "Error - sorting listbox failed to create");
		DOMYLOGST(API_FAILURE), "lbSorting.Create", ::GetLastError(), 
		  Generic::GetSysErrorString());
		bReturn = false;
	}

	return bReturn;
}

// Sort the sent CStringArray.
/* static */ void Generic::SortCStringArray(CStringArray& sa,
											bool          bAscending)
{
	bool bNotDone(true);

	while (bNotDone) {
		bNotDone = false;

		for (int nPos = 0; nPos < sa.GetUpperBound(); ++nPos) {
			bNotDone |= Generic::SortCStringArrayHelper(sa, nPos, bAscending);
		}
	}
}

// Add the string to the list only if it is not already in the list.
/* static */ bool Generic::AddUniqueToCStringList(CStringList&   sl,
												  const CString& sText)
{
	bool bReturn(false);
	POSITION pos(sl.Find(sText));

	if (! pos) {
		sl.AddTail(sText);
		bReturn = true;
	}

	return bReturn;
}

// For reasons that elude me the CStringArray class does not have a operator=.
/* static */ void Generic::CopyCStringArray(const CStringArray& saSource,
											      CStringArray& saTarget)
{
	for (int n = 0; n < saSource.GetSize(); ++n) {
		saTarget.SetAtGrow(n, saSource[n]);
	}
}

// For reasons that elude me the CStringList class does not have a operator=.
/* static */ void Generic::CopyCStringList(const CStringList& slSource,
										   CStringList&       slTarget)
{
	POSITION pos(slSource.GetHeadPosition());

	while (pos) {
		CString s(slSource.GetNext(pos));
		slTarget.AddHead(s);
	}
}

// For reasons that elude me the CDWordArray class does not have a operator=.
/* static */ void Generic::CopyCDWordArray(const CDWordArray& dwaSource,
											     CDWordArray& dwaTarget)
{
	for (int n = 0; n < dwaSource.GetSize(); ++n) {
		dwaTarget.SetAtGrow(n, dwaSource[n]);
	}
}

// Compares and swaps.  Returns TRUE if a swap was needed.
/* static */ bool Generic::SortCStringArrayHelper(CStringArray& sa,
												  int           nPos,
												  bool          bAscending)
{
	_ASSERTE(0 <= nPos);

	bool bReturn(false);
	CString sTemp;
	int nPosFirst(nPos);
	int nPosNext(nPos + 1);

	if ((bAscending  &&  sa.GetAt(nPosFirst).CompareNoCase(sa.GetAt(nPosNext)) > 0)  ||
	  (! bAscending  &&  sa.GetAt(nPosFirst).CompareNoCase(sa.GetAt(nPosNext)) < 0)) {
		sTemp = sa.GetAt(nPosFirst);
		sa.SetAt(nPosFirst, sa.GetAt(nPosNext));
		sa.SetAt(nPosNext, sTemp);
		bReturn = true;
	}

	return bReturn;
}

// Compare IP address, or versions, of format "12.3.04.999".  Returns a 
// positive number if "s1" is larger than "s2", negative if "s2" is larger
// than "s1", and zero if they are equal. Works for any number of dots (and 
// any non-numeric character, not just dots).
/* static */ int Generic::CompareDottedString(const CString s1,
											  const CString s2)
{
	int nReturn(0);

	CString sA(s1);
	CString sB(s2);

	while (true) {

		// Create the substrings.
		CString sPartA(sA.SpanIncluding("0123456789"));
		CString sPartB(sB.SpanIncluding("0123456789"));

		// Compare the substrings.
		nReturn = atoi(sPartA) > atoi(sPartB);

		if (0 != nReturn) {
			break;
		}

		// If both would be empty next time, we're done.
		if (1 >= sA.GetLength()  &&  1 >= sB.GetLength()) {
			break;
		}

		// Prepare the strings for next time.
		if (1 < sA.GetLength()) {
			sA = sA.Mid(sPartA.GetLength() + 1);
		}

		if (1 < sB.GetLength()) {
			sB = sB.Mid(sPartB.GetLength() + 1);
		}
	}

	return nReturn;
}

// Converts the sent string to be lead-filled with zeroes to create it the sent
// length, using the sent character.
/* static */ void Generic::LeadFillWithChar(CString&       s,
											int            nTotalLen,
											const CString& sChar)
{
	_ASSERTE(0 < nTotalLen);
	_ASSERTE(1 == sChar.GetLength());

	int nLen(s.GetLength());
	int nToAdd(nTotalLen - nLen);

	if (0 < nToAdd) {
		CString sToAdd(sChar[0], nToAdd);
		s = sToAdd + s;
		_ASSERTE(nTotalLen == s.GetLength());
	}
}

// Is the sent string only numerals?
/* static */ bool Generic::IsAllDigits(const CString& s)
{
	CString sDigits(s.SpanIncluding("0123456789"));

	return s.GetLength() == sDigits.GetLength();
}

// Access to data in ini file using a CString.  Features include: check the 
// error conditions correctly, allow you to pass NULL's in, use CStrings
// internally, doesn't use hardcoded numbers, handle default values, and allows
// for an optional size to be passed.  Returns FALSE to mean failure.
/* static */ bool Generic::GetFromIniFile(const CString& sIniFile,
										  const CString& sSection,
										  const CString& sEntry,
										  const CString& sDefault,
										  CString&       sData,
										  int            nSize /* = MAX_PATH */ )
{
	_ASSERTE(0 < nSize  &&  "Buffer size must be positive");

	bool bReturn(false);

	if (0 < nSize) {
		sData.Empty();

		DWORD dwReturn(::GetPrivateProfileString(sSection, sEntry, sDefault,
		  sData.GetBuffer(nSize), nSize, sIniFile));

		sData.ReleaseBuffer();

		// Buffer was too small.
		if (nSize - 1L == static_cast<LONG> (dwReturn)  || 
			nSize - 2L == static_cast<LONG> (dwReturn)) {

			DOMYLOGST(API_FAILURE), "GetPrivateProfileString() buffer too small",
			  dwReturn, sEntry);
		}
		else if (sData.GetLength() != static_cast<LONG> (dwReturn)) {
			// Some other error.
			DOMYLOGST(API_FAILURE), "GetPrivateProfileString() other error",
			  dwReturn, sEntry);
		}
		else if (0 == sData.GetLength()) {
			// No data read - INI file, section, or key missing?
			DOMYLOGE ("No data read from <%s>, section <%s>, key <%s>\n",
			  sIniFile, sSection, sEntry);
		}
		else {
			// All is well.
			bReturn = true;
		}
	}
	else {
		// Bad data passed in - length must be positive.
		DOMYLOGST(API_FAILURE), "Bad size", nSize, sEntry);
	}

	return bReturn;
}

// Parse full path down to just title (the "8" part of "8.3").
/* static */ CString Generic::TitleFromPath(const CString& sPath)
{
	CString sFile(Generic::FileFromPath(sPath));
	int nIndex(sFile.ReverseFind('.'));

	if (-1 != nIndex) {
		sFile = sFile.Left(nIndex);
	}

	return sFile;
}

// Parse full path down to just filename.
/* static */ CString Generic::FileFromPath(const CString& sPath)
{
	CString sFile(sPath);
	int nIndex(sFile.ReverseFind('\\'));

	if (-1 != nIndex) {
		sFile = sFile.Mid(nIndex + 1);
	}

	return sFile;
}

// Parse full path down to just the drive letter: "C".
/* static */ CString Generic::DriveFromPath(const CString& sPath)
{
	CString sDrive;

	if (1 < sPath.GetLength()  &&  ':' == sPath.GetAt(1)) {
		sDrive = sPath.GetAt(0);
	}

	return sDrive;
}

// Parse full path down to just dir: sent "c:\a\b\file.txt" returns "c:\a\b".
/* static */ CString Generic::DirFromPath(const CString& sPath)
{
	CString sDir(sPath);
	int nIndex(sDir.ReverseFind('\\'));

	if (-1 != nIndex) {
		sDir = sDir.Left(nIndex);
	}

	return sDir;
}

// Parse full path down to just filename and one dir.
/* static */ CString Generic::DirAndFileFromPath(const CString& sPath)
{
	CString sFile(sPath);
	int nIndex(sFile.ReverseFind('\\'));

	if (-1 != nIndex) {
		CString sFile2(sFile.Left(nIndex));
		nIndex = sFile2.ReverseFind('\\');
		sFile = sFile.Mid(nIndex + 1);
	}

	return sFile;
}

// Parse full path down to just extension.
/* static */ CString Generic::ExtFromPath(const CString& sPath)
{
	CString sExt(Generic::FileFromPath(sPath));
	int nIndex(sExt.ReverseFind('.'));

	if (-1 != nIndex) {
		sExt = sExt.Mid(nIndex + 1);
	}
	else {
		sExt.Empty();
	}

	return sExt;
}

// Given a base directory "c:\a\b\c\d", and a path "c:\a\b\x\y.cpp", returns 
// the string "..\..\x\y.cpp". If the two paths are completely unrelated,
// it returns 'sSentPath'.
/* static */ CString Generic::RelativeFromBase(const CString& sSentBaseDir,
											   const CString& sSentPath)
{
	_ASSERTE(! sSentBaseDir.IsEmpty());
	_ASSERTE(! sSentPath.IsEmpty());

	CString sReturn;
	CString sBaseDir(sSentBaseDir);
	CString sPath(sSentPath);

	// Be careful to ensure the last char on the base dir is a backslash for
	// comparisons to work correctly.
	if (sBaseDir.Right(1) != "\\") {
		sBaseDir += "\\";
	}

	// Convert to same case for comparisons.
	sBaseDir.MakeLower();
	sPath.MakeLower();

	// First case is that the path is a child of the base dir.  In that case,
	// we just convert "c:\a\b\c\d\z.cpp" to "z.cpp".
	int nDiff(sPath.Find(sBaseDir));

	if (0 == nDiff) {
		sReturn = sPath.Mid(sBaseDir.GetLength());
	}
	else {
		// Second case is that the path is NOT a child of the base dir.  In this
		// case, we have to convert "c:\a\b\x\y\w.cpp" to "..\..\w.cpp".
		CString sPrefix;
		
		// First find the prefix, the beginning part that is the same in both
		// the base dir and the path.
		for (int nIndex = 0; nIndex < sBaseDir.GetLength(); ++nIndex) {
			
			if (sPath[nIndex] == sBaseDir[nIndex]) {
				sPrefix += sPath[nIndex];
			}
			else {
				break;
			}
		}

		// Back up to the first backslash to avoid getting coincidences 
		// between filenames and dir names in the prefix.
		while (sPrefix.Right(1) != '\\'  &&  ! sPrefix.IsEmpty()) {
			sPrefix = sPrefix.Left(sPrefix.GetLength() - 1);
		}

		// If there is a prefix, that means that 'sSentPath' is indeed a
		// subpath of 'sSentBaseDir'.
		if (! sPrefix.IsEmpty()) {
			_ASSERTE('\\' != sPrefix[0]);
			_ASSERTE('\\' == sPrefix[sPrefix.GetLength() - 1]);

			// Create the suffix, the tail part of the path that is different
			// from the base dir.
			CString sSuffix(sPath.Mid(sPrefix.GetLength()));
			_ASSERTE('\\' != sSuffix[0]);
			_ASSERTE('\\' != sSuffix[sSuffix.GetLength() - 1]);
			
			// Create the affix, the tail part of the base dir that is 
			// different from the path.
			CString sAffix(sBaseDir.Mid(sPrefix.GetLength()));
			_ASSERTE('\\' != sAffix[0]);
			_ASSERTE('\\' == sAffix[sAffix.GetLength() - 1]);

			// Now get how many backslashes there are in the affix.
			int nSlashCount(0);
			
			while (! sAffix.IsEmpty()) {
				int nChar(sAffix.Find("\\"));

				if (-1 != nChar) {
					++nSlashCount;
					sAffix = sAffix.Mid(nChar + 1);
				}
				else {
					break;
				}
			}

			// Now create a new prefix, and append the suffix.
			for (int nSlash = 0; nSlash < nSlashCount; ++nSlash) {
				sAffix += "..\\";
			}

			sReturn = sAffix + sSuffix;
		}
		else {
			// If there is no prefix, that means that 'sSentPath' and 
			// 'sSentBaseDir' are completely unrelated. So, we return 
			// 'sSentPath' as the shortest possible version.
			sReturn = sSentPath;
		}
	}

	return sReturn;
}

// Returns TRUE if the sent path refers to a file that exists.
/* static */ bool Generic::IsFile(const CString& sPath)
{
	bool bReturn(false);

	_finddata_t fi = { 0 };
	long lnHandle(_findfirst(sPath, &fi));
	
	if (-1 == lnHandle) {
		_ASSERTE(ERROR_FILE_NOT_FOUND == ::GetLastError()  ||  ERROR_PATH_NOT_FOUND == ::GetLastError());
		::SetLastError(0);
		fi.size = -1;
	}
	else {
		// File exists.
		VERIFY(0 == _findclose(lnHandle));
		bReturn = true;
	}

	return bReturn;
}

// Returns size in bytes, or zero to mean empty in 'dwSize'.  Returns TRUE if
// file exists, FALSE if it does not.  (Or, you can use 'IsFile()' to see if a
// file exists.)
/* static */ bool Generic::GetFileSize(const CString& sPath, 
									   DWORD&         dwSize)
{
	_ASSERTE(! sPath.IsEmpty());

	bool bReturn(false);
	_finddata_t fi = { 0 };
	long lnHandle(_findfirst(sPath, &fi));
	dwSize = 0;
	
	if (-1 == lnHandle) {
		TRACE("   _findfirst() GetLastError(): <%d>.\n", ::GetLastError());
		::SetLastError(0);
		_ASSERTE(! "Use Generic::IsFile() first to avoid logging this error!");
		DOMYLOGW ("Generic::GetFileSize() cannot find file <%s> on disk!\n", sPath);
	}
	else {
		// File exists.
		VERIFY(0 == _findclose(lnHandle));
		bReturn = true;
		dwSize = fi.size;
	}

	return bReturn;
}

// What day number (1 thru 366) is the sent date?
/* static */ int Generic::GetJulianDayNumber(const CTime& tim)
{
	return atoi(tim.Format("%j"));
}

// Convert a total number of seconds to a CTimeSpan.
/* static */ CTimeSpan Generic::TimeSpanFromSeconds(int nSeconds)
{
	_ASSERTE(0 <= nSeconds  &&  "Negative time makes no sense");
	
	int nSecs(nSeconds);

	int nDays (nSecs  / (60 * 60 * 24));
	nSecs -=  (nDays  * (60 * 60 * 24));

	int nHours(nSecs  / (60 * 60));
	nSecs -=  (nHours * (60 * 60));

	int nMins (nSecs  / 60);
	nSecs -=  (nMins  * 60);

	return CTimeSpan(nDays, nHours, nMins, nSecs);
}

// Return the number of days in a given month.
/* static */ int Generic::GetDaysInMonth(int nMM,
										 int nYYYY)
{
	_ASSERTE(0 < nMM  &&  12 >= nMM);
	_ASSERTE(1583 < nYYYY);

	int nDaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int nReturn(nDaysInMonth[nMM - 1]);

	if (2 == nMM  &&  Generic::IsLeapYear(nYYYY)) {
		++nReturn;
	}

	_ASSERTE(0 < nReturn  &&  32 > nReturn);

	return nReturn;
}

// Is the sent year a leap year?
/* static */ bool Generic::IsLeapYear(int nYYYY)
{
	_ASSERTE(1583 < nYYYY);

	return (nYYYY % 4 == 0  &&  nYYYY % 100 != 0)  ||  nYYYY % 400 == 0;
}

// Figure out the date "n" months from now.
/* static */ COleDateTime Generic::AddMonthsToDate(
												const COleDateTime& odt,
												int                 nMonths)
{

	// Figure out the ostensible date (this may give us something goofy like
	// 02/31/2001).
	COleDateTime odtReturn(odt);

	int nYear(odt.GetYear());
	int nMonth(odt.GetMonth());
	int nDay(odt.GetDay());
	nMonth += nMonths;

	if (12 < nMonth) {
		int nExcessMonths(nMonth - 12);
		nMonth = (nMonth % 12);
		nYear += ((nExcessMonths / 12) + 1);

		if (0 == nMonth) {
			nMonth = 12;
			nYear--;
		}
	}

	// Handle the fact that not all months have days 29, 30, and 31.
	int nDayNew(0);

	switch (nMonth) {

		// 30 days has September, April, June, and November.
		case 9:
		case 4:
		case 6:
		case 11:
			nDayNew = min(nDay, 30);
			break;

		// February.
		case 2: 
			nDayNew = min(nDay, 28);

			if (0 == nYear % 4) {
				nDayNew = min(nDay, 29);

				if (0 == nYear % 100) {
					nDayNew = min(nDay, 28);

					if (0 == nYear % 400) {
						nDayNew = min(nDay, 29);
					}
				}
			}
			
			break;

		// All the rest have 31.
		default:
			nDayNew = min(nDay, 31);
			break;
	}

	odtReturn.SetDate(nYear, nMonth, nDayNew);
	
	return odtReturn;
}

// Convert sent milliseconds to a CString like "DD:HH:MM:SS.mmm".
/* static */ CString Generic::MillisecsToString(DWORD dwMS)
{
	CString s;

	// Parse out the values.
	int nMilliSecs(dwMS);

	int nDays (nMilliSecs / (1000 * 60 * 60 * 24));
	nMilliSecs -= (nDays  * (1000 * 60 * 60 * 24));

	int nHours(nMilliSecs / (1000 * 60 * 60));
	nMilliSecs -= (nHours * (1000 * 60 * 60));

	int nMins (nMilliSecs / (1000 * 60));
	nMilliSecs -= (nMins  * (1000 * 60));

	int nSecs (nMilliSecs / (1000));
	nMilliSecs -= (nSecs  * (1000));

	// Format each piece.  Only include the pieces that are necessary, and 
	// only have leading zeros if the previous segment existed.
	CString sPiece;
	CString sFormat;

	if (nDays) {
		sFormat = "%d:";
		sPiece.Format(sFormat, nDays);
		s += sPiece;
	}
		
	if (nDays  ||  nHours) {
		sFormat = (10 > nHours  &&  nDays) ? "%02d:" : "%2d:";
		sPiece.Format(sFormat, nHours);
		s += sPiece;
	}

	if (nDays  ||  nHours  ||  nMins) {
		sFormat = (10 > nMins  &&  (nDays  ||  nHours)) ? "%02d:" : "%2d:";
		sPiece.Format(sFormat, nMins);
		s += sPiece;
	}
	
	sFormat = (10 > nSecs  &&  (nDays  ||  nHours  ||  nMins)) ? "%02d.%03d" : "%2d.%03d";
	sPiece.Format(sFormat, nSecs, nMilliSecs);
	s += sPiece;
	
	return s;
}

// Leave before any further damage can be done to the registry.
// Memory leaks are the least of our worries.
/* static */ void Generic::QuickExit(const CString& sFile,
									 int            nLine)
{
	DOMYLOGA ("QUICK_EXIT\n");
	_ASSERTE(! "About to ::PostQuitMessage(-1)");
	::PostQuitMessage(-1);
	Generic::SpinTheMessageLoop();
}

// Format the system error number returned by ::GetLastError() to a string.
/* static */ CString Generic::GetSysErrorString(int nErrSent /* = 0 */ )
{
	static string str;

	// Use sent if sent.
	if (0 == nErrSent) {
		nErrSent = GetLastError();
	}

	// There is an error number still.
	if (nErrSent) {
		LPVOID lpMsgBuf = NULL;
		
		if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER  |
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			  FORMAT_MESSAGE_FROM_SYSTEM,				// Flags.
			NULL,										// Message source.
			nErrSent,									// Message id.
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// Default language.
			(LPTSTR) &lpMsgBuf,							// Buffer to receive.
			0,											// Max size of buffer.
			NULL										// Argument list.
			)) {

			// Remove goofy final characters.
			str = reinterpret_cast<const char*> (lpMsgBuf);
			str = str.substr(0, str.length() - 2);
		}
		else {
			DOMYLOGE ("\n::FormatMessage() failed with error <%d>.", nErrSent);
		}

		VERIFY(NULL == ::LocalFree(lpMsgBuf));
	}
	else {
		// No error number.  We must not return the default string, "The
		// operation completed successfully.", because that confuses the user.
		// Calling CString::LoadString() from here always fails with errno 
		// 1814: "The specified resource name cannot be found in the image 
		// file."
	}

	return str.data();
}

// Obtain the error message for a given HRESULT.
/* static */ CString Generic::GetHrErrorString(HRESULT hr)
{
	LPVOID lpMsgBuf = NULL;
	static string str;

	if (::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER  |
	  FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0,
		NULL))
	{
		// Remove goofy final characters.
		str = static_cast<char*> (lpMsgBuf);
		str = str.substr(0, str.length() - 2);
	}
	else {
		//DOMYLOGE ("[ *** ::FormatMessage() failed with error <%d> *** ] ",
		//  ::GetLastError());
	}

	// Free the buffer.
	VERIFY(NULL == ::LocalFree(lpMsgBuf));

	return str.data();
}

// Get the parse error number and string.
/* static */ CString Generic::GetXMLDOMErrorString(
									const IXMLDOMDocumentPtr& spXMLDOMDocument)
{
	IXMLDOMParseErrorPtr pParseError = spXMLDOMDocument->GetparseError();
	
	CString sErr;
	sErr.Format("%x", pParseError->GeterrorCode());

	static CString sReason(sErr + ": " + 
		static_cast<const char*> (pParseError->Getreason()));

	// Remove goofy final characters.
	sReason = sReason.Left(sReason.GetLength() - 2);

	return sReason;
}

// This function registers a COM server on a remote system to be run locally.
// This is made difficult by the fact the ::CreateProcess( ) cannot take UNC
// style paths, only mapped drive letters.  So, we first find a free drive /
// letter, temporarily map it, call WinExec( ), and unmap the drive.
//
// 'sRemoteName' is like "\\\\damar\\dordps"
// 'sServerPath' is like "MyDirOne\\MyDirTwo\\DorDPSX_South.exe"
//
// 01/29/02: Added code to use this function to register as well as unregister.
/* static */ bool Generic::RegisterRemoteServerLocally(
									const CString& sRemoteName,
									const CString& sServerPath,
									const bool&    bRegister /* = true */ )
{
	_ASSERTE(! sRemoteName.IsEmpty());
	_ASSERTE(! sServerPath.IsEmpty());

	bool bReturn(false);

	DOMYLOGA ("Registering the COM server for <%s> on <%s>...\n", sServerPath,
	  sRemoteName);

	// Concat the full file name e.g \\DAMAR\dordps\dir1\dir2\myexec.exe
	CString sFullFilePathName(sRemoteName + "\\" + sServerPath);

	// Get the 8.3 filename ver. This is due to a problem in CreateProcess 
	// (called from WinExec) that crashes the system for long file name.
	char szShortFilePathName[MAX_PATH + 1] = { 0 };

	DWORD dwRetBuffer(::GetShortPathName(sFullFilePathName,
	  szShortFilePathName, MAX_PATH));

	if (dwRetBuffer == MAX_PATH) {
		// Our buffer was smaller than what we should have allotted.
		_ASSERTE(! "Error");
		DOMYLOGE ("GetShortPathName failed to convert long pathname to short "
		  "pathname - increase the buffer size.\n");
	}
	else if (0 == dwRetBuffer) {
		_ASSERTE(! "Error");
		DOMYLOGST(API_FAILURE), "::GetShortPathName: " + sFullFilePathName,
		  ::GetLastError(), Generic::GetSysErrorString());
	}
	else {
		// Copy the character pointer to a CString, easier to manipulate
		CString strShortFilePathName = (dwRetBuffer == 0)
		  ? sFullFilePathName 
		  : szShortFilePathName;

		CString strCommandLine = bRegister
		  ? strShortFilePathName + " /RegServer" 
		  : strShortFilePathName + " /UnRegServer";

		UINT uiExec (::WinExec(strCommandLine, SW_HIDE));
		
		if (31 < uiExec) {
			DOMYLOGA ("Successfully registered COM server!\n");
			bReturn = true;
		}
		else {
			_ASSERTE(! "Error");
			DOMYLOGST(API_FAILURE), "::WinExec", uiExec, strCommandLine);
		}
	}

	return bReturn;
}

// Print COM errors.
/* static */ void Generic::PrintComError(_com_error &e)
{
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

	DOMYLOGE ("COM Error: Code <%x>\n", e.Error());
	DOMYLOGE ("   Code meaning <%s>\n", e.ErrorMessage());
	DOMYLOGE ("   Source <%s>\n", (LPCSTR) bstrSource);

	// For some reason they sometimes append a CR for us.
	CString sDescription((LPCSTR) bstrDescription);
	sDescription.TrimRight();
	DOMYLOGE ("   Description <%s>\n", sDescription);
}

// Print OLE Provider error.
/* static */ void Generic::PrintProviderError(_ConnectionPtr pConnection)
{
	// pErr is a record object in the Connection's Error collection.
	ErrorPtr pErr = NULL;

	for (long i = 0; i < pConnection->Errors->Count; ++i) {
		pErr = pConnection->Errors->GetItem(i);

		DOMYLOGE ("OLE Provider error <%x> <%s>\n", pErr->Number, 
		  (LPCSTR) pErr->Description);
	}
}

// Given a number return a comma-ized string, like "1,234,567".
/* static */ CString Generic::IntToCommas(int nInt)
{
	CString s;
	int nValue(abs(nInt));

	while (nValue) {
		int nSegment(nValue % 1000);
		nValue /= 1000;

		CString sSegment;
		sSegment.Format(nValue ? "%03d," : "%d,", nSegment);
		
		s = sSegment + s;
	}

	// Remove trailing comma.
	s = s.Left(s.GetLength() - 1);

	// Add leading negative.
	if (0 > nInt) {
		s = "-" + s;
	}

	return s;
}

// Given a number return a comma-ized string, like "1,234,567.89".
/* static */ CString Generic::DoubleToCommas(
										double dDouble,
										int    nDigitsAfterDecimal /* = 2 */ )
{
	// Handle rounding issue.
	bool bNegative(false);
	double dValue(Generic::RoundDouble(dDouble, nDigitsAfterDecimal));

	if (0.0 > dValue) {
		dValue = fabs(dValue);
		bNegative = true;
	}

	// Now do the stringizing.
	double dOrd(0.0);
	double dMan(modf(dValue, &dOrd));

	CString sOrd(Generic::IntToCommas(static_cast<int> (dOrd)));
	CString sMan;
	sMan.Format("%.*f", nDigitsAfterDecimal, fabs(dMan));

	// Build the return value; strip off leading zero from mantissa,
	// account for negative values.
	CString sValue(sOrd + sMan.Mid(1));
	
	if (bNegative) {
		sValue = "-" + sValue;
	}

	return sValue;
}

// Given a string with commas, return an int.
/* static */ int Generic::CommasToInt(const CString& sCommas)
{
	CString s(Generic::StripNonNumeric(sCommas));
	return atoi(s);
}

// Returns the sent target string with everything except the sent valid
// characters removed.
/* static */ CString Generic::RemoveAllExcept(const CString& sTarget,
											  const CString& sValidChars)
{
	CString sReturn;
	CString sTargetLocal(sTarget);

	while (true) {
		CString sSub(sTargetLocal.SpanIncluding(sValidChars));

		if (sTargetLocal.IsEmpty()) {
			break;
		}
		else {
			sReturn += sSub;

			if (sSub.GetLength() < sTargetLocal.GetLength()) {
				sTargetLocal = sTargetLocal.Mid(sSub.GetLength() + 1);
			}
			else {
				break;
			}
		}
	}

	return sReturn;
}

// Given a string with commas, return a double.
/* static */ double Generic::CommasToDouble(const CString& sCommas)
{
	CString s(Generic::StripNonNumeric(sCommas));
	return atof(s);
}

// Given a string like "$ 123,567.89" remove everything except numerals and 
// periods and minus signs
/* static */ CString Generic::StripNonNumeric(const CString& s)
{
	CString sInput(s);
	CString sReturn;
	CString sPiece;

	while (! sInput.IsEmpty()) {
		sPiece = sInput.SpanIncluding("-.0123456789");
		sReturn += sPiece;

		if (sPiece.GetLength() + 1 < sInput.GetLength()) {
			sInput = sInput.Mid(sPiece.GetLength() + 1);
		}
		else {
			break;
		}
	}

	return sReturn;
}

// Given a string like "(Howdy, there!)" remove everything except numerals
// and alphabetical.
/* static */ CString Generic::StripNonAlphaNumeric(const CString& s)
{
	CString sInput(s);
	CString sReturn;
	CString sPiece;

	while (! sInput.IsEmpty()) {
		
		sPiece = sInput.SpanIncluding(
		  "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

		sReturn += sPiece;

		if (sPiece.GetLength() + 1 < sInput.GetLength()) {
			sInput = sInput.Mid(sPiece.GetLength() + 1);
		}
		else {
			break;
		}
	}

	return sReturn;
}

// Add half and truncate.
/* static */ double Generic::RoundDouble(double dDouble,
										 int    nDigitsAfterDecimal /* = 2 */ )
{
	double dRoundingAddition(5.0 / pow(10.0, nDigitsAfterDecimal + 1));
	double dValue(dDouble + dRoundingAddition);
	dValue = (dValue * pow(10.0, nDigitsAfterDecimal));
	dValue = floor(dValue) / pow(10.0, nDigitsAfterDecimal);
	
	return dValue;
}


/////////////////////////////////////////////////////////////////////////////
// Conversion routines: RGB to HLS (Red-Green-Blue to Hue-Luminosity-Saturation).
// See Microsoft KnowledgeBase article Q29240.

#define  UNDEFINED  (HLSMAX * 2 / 3)	// Hue is undefined if Saturation is 0 
										// (grey-scale).  This value determines 
										// where the Hue scrollbar is initially 
										// set for achromatic colors.

// Convert RGB to HLS.
/* static */ void Generic::RGBtoHLS(COLORREF crRGB,
									WORD&    wH,
									WORD&    wL,
									WORD&    wS)
{
	// Get R, G, and B out of COLORREF.
	WORD wR(GetRValue(crRGB));
	WORD wG(GetGValue(crRGB));
	WORD wB(GetBValue(crRGB));
	
	// Calculate "lightness".
	BYTE cMax(static_cast<BYTE> (max(max(wR, wG), wB)));
	BYTE cMin(static_cast<BYTE> (min(min(wR, wG), wB)));

	wL = (((cMax + cMin) * HLSMAX) + RGBMAX) / (2 * RGBMAX);
	
	// r=g=b --> achromatic case.
	if (cMax == cMin) {
		wS = 0;
		wH = UNDEFINED;
	}
	else {
		// Chromatic case.

		// Saturation.
		if (wL <= (HLSMAX / 2)) {
			wS = (((cMax - cMin) * HLSMAX) + ((cMax+cMin) / 2)) / (cMax + cMin);
		}
		else {
			wS = (((cMax - cMin) * HLSMAX) + ((2 * RGBMAX - cMax - cMin) / 2)) / (2 * RGBMAX - cMax - cMin);
		}

		// Hue.
		WORD wRdelta((((cMax - wR) * (HLSMAX / 6)) + ((cMax - cMin) / 2)) / (cMax - cMin));
		WORD wGdelta((((cMax - wG) * (HLSMAX / 6)) + ((cMax - cMin) / 2)) / (cMax - cMin));
		WORD wBdelta((((cMax - wB) * (HLSMAX / 6)) + ((cMax - cMin) / 2)) / (cMax - cMin));
		
		if (wR == cMax) {
			wH = wBdelta - wGdelta;
		}
		else if (wG == cMax) {
			wH = (HLSMAX / 3) + wRdelta - wBdelta;
		}
		else {
			// B == cMax
			wH = ((2 * HLSMAX) / 3) + wGdelta - wRdelta;
		}

		if (wH < 0) {
			wH += HLSMAX;
		}

		if (wH > HLSMAX) {
			wH -= HLSMAX;
		}
	}
}

// Convert HLS to RGB.
/* static */ COLORREF Generic::HLStoRGB(WORD wH,
										WORD wL,
										WORD wS)
{
	WORD wR(0);
	WORD wG(0);
	WORD wB(0);
	
	// Achromatic case.
	if (0 == wS) {
		wR = wG = wB = (wL * RGBMAX) / HLSMAX;

		if (UNDEFINED != wH) {
			_ASSERTE(! "ERROR");
		}
	}
	else {
		// Chromatic case.
		WORD Magic1(0);
		WORD Magic2(0);

		// Set up magic numbers.
		if (wL <= HLSMAX / 2) {
			Magic2 = (wL * (HLSMAX + wS) + (HLSMAX / 2)) / HLSMAX;
		}
		else {
			Magic2 = wL + wS - ((wL * wS) + (HLSMAX / 2)) / HLSMAX;
		}

		Magic1 = 2 * wL - Magic2;
		
		// Get RGB, change units from HLSMAX to RGBMAX.
		wR = (HueToRGB(Magic1, Magic2, wH + (HLSMAX / 3)) * RGBMAX + (HLSMAX / 2)) / HLSMAX;
		wG = (HueToRGB(Magic1, Magic2, wH)                * RGBMAX + (HLSMAX / 2)) / HLSMAX;
		wB = (HueToRGB(Magic1, Magic2, wH - (HLSMAX / 3)) * RGBMAX + (HLSMAX / 2)) / HLSMAX;
	}
	
	return RGB(wR,wG,wB);
}

// Utility routine for HLStoRGB.
/* static */ WORD Generic::HueToRGB(WORD w1, WORD w2, WORD wH)
{
	// Range check: note values passed add/subtract thirds of range.
	if (wH < 0) {
		wH += HLSMAX;
	}

	if (wH > HLSMAX) {
		wH -= HLSMAX;
	}

	// Return r, g, or b value from this tridrant.
	if (wH < HLSMAX / 6) {
		return w1 + (((w2 - w1) * wH + (HLSMAX / 12)) / (HLSMAX / 6));
	}

	if (wH < HLSMAX / 2) {
		return w2;
	}

	if (wH < (HLSMAX * 2) / 3) {
		return w1 + (((w2 - w1) * (((HLSMAX * 2) / 3) - wH) + (HLSMAX / 12)) / (HLSMAX / 6));
	}
	else {
		return w1;
	}
}

// Can't just use ~ because the first two bytes are zeros.
/* static */ COLORREF Generic::FlipRGB(COLORREF cr)
{
	WORD wR(~ GetRValue(cr));
	WORD wG(~ GetGValue(cr));
	WORD wB(~ GetBValue(cr));

	return RGB(wR, wG, wB);
}

// Appends Path2 to Path1.  Neither path has to exist.
/* static */ bool Generic::AppendFiles(const CString& sPath1,
									   const CString& sPath2)
{
	_ASSERTE(! sPath1.IsEmpty());
	_ASSERTE(! sPath2.IsEmpty());

	bool bReturn(false);
	CFile file1;
	CFile file2;

	// Open the file objects.
	if (file1.Open(sPath1, CFile::modeCreate | CFile::modeNoTruncate |
	  CFile::modeReadWrite | CFile::shareExclusive)) {

		if (file2.Open(sPath2, CFile::modeRead | CFile::shareDenyWrite)) {

			// Create a big buffer to transfer the data in.
			PBYTE pBuf = NULL;
			const int nBufSize(64 * 1024);

			try {
				pBuf = new BYTE[nBufSize];
			}
			catch(CMemoryException* e) {
				DOMYLOGST(NEW_FAILURE), "BYTE");
				pBuf = NULL;
				e->Delete();
			}

			if (pBuf) {

				try {
					DWORD dwBytesRead(0);
					file1.SeekToEnd();

					// Read in from file2, and append it to file1, until we hit eof 
					// on file2.
					do {
						dwBytesRead = file2.Read(pBuf, nBufSize);
						file1.Write(pBuf, dwBytesRead);
					} while (nBufSize == dwBytesRead);

					// Done appending!  Close the files.
					file1.Close();
					file2.Close();
					bReturn = true;
					DOMYLOGA ("APPEND_FILE_SUCCESS %s and %s\n", sPath2, sPath1);
				}
				catch(CFileException* e) {
					DOMYLOGE ("Generic::AppendFiles() failed with error <%s>.\n",
					  Generic::GetSysErrorString());
					e->Delete();
				}

				// Release memory.
				delete pBuf;
				pBuf = NULL;
			}
		}
		else {
			DOMYLOGE ("CFile::Open() failed with error <%s>.\n",
			  Generic::GetSysErrorString());
		}
	}
	else {
		DOMYLOGE ("CFile::Open() failed with error <%s>.\n",
		  Generic::GetSysErrorString());
	}

	return bReturn;
}

// Rename a file.
/* static */ bool Generic::RenameFile(const CString& sPath1,
									  const CString& sPath2)
{
	_ASSERTE(! sPath1.IsEmpty());
	_ASSERTE(! sPath2.IsEmpty());

	HRESULT hr(S_OK);
	EC_VEF(CFile::Rename(sPath1, sPath2));
	EC_FAIL(s, sPath1);
	EC_FAIL(s, sPath2);
	EC_SUCCEED(("Renamed <%s> to <%s>.\n", sPath1, sPath2));

	return SUCCEEDED(hr);
}

// Delete a file.
/* static */ bool Generic::RemoveFile(const CString& sPath,
									  int&           nErr)
{
	_ASSERTE(! sPath.IsEmpty());

	bool bReturn(false);

	bReturn = ::DeleteFile(sPath);
	nErr = ::GetLastError();

	if (bReturn) {
		DOMYLOGA ("Removed file <%s>.\n", sPath);
	}
	else {
		// Since this can happen before logging is enabled, we also trace out.
		CString sFmt("::DeleteFile() failed for file <%s> with error <%d>: <%s>.\n");
		CString sMsg;
		sMsg.Format(sFmt, sPath, nErr, Generic::GetSysErrorString());
		TRACE(sMsg);
		DOMYLOGE (sMsg);
	}

	return bReturn;
}

// Delete the oldest messages in the file until we have a file smaller than
// the sent size.
/* static */ HRESULT Generic::TrimFileToSize(const CString& sFileName,
											 DWORD          dwMaxLogSize)
{
	HRESULT hr(S_OK);

	// How big is the file now?
	DWORD dwLogSize(0L);
	EC_B(Generic::GetFileSize(sFileName, dwLogSize));

	// If the file file is too big.
	DWORD dwBytesToTrim(dwLogSize - dwMaxLogSize);
	
	if (0 < dwBytesToTrim) {
		CWaitCursor wc;

		// Open the output file under a slightly different name. (Since
		// 'sFileName' can be either a filename or a pathname, we have to check
		// to see if we can prepend a tilde, to.)
		CString sPathOut(sFileName + ".tmp");

		if (-1 == sPathOut.Find('\\')) {
			sPathOut = "~" + sPathOut;
		}

		CStdioFile filOut;

		EC_B(filOut.Open(sPathOut, CFile::modeCreate | CFile::modeReadWrite |
		  CFile::shareExclusive));

		// Open the input file and move to the first character we want to keep.
		CString sPathIn(sFileName);
		CStdioFile filIn;
		EC_B(filIn.Open(sPathIn, CFile::modeRead | CFile::shareDenyWrite));
		EC_VED(filIn.Seek(dwBytesToTrim, CFile::begin));

		// Read each line in and write it out. We throw away the first line,
		// as it's probably just a fragment of a line, given the seek().
		CString sLine;
		EC_B(filIn.ReadString(sLine));

		while (filIn.ReadString(sLine)) {
			filOut.WriteString(sLine + "\n");
		}

		// Close the files.
		EC_VED_(filIn.Close());
		EC_VED_(filOut.Close());

		// If all went well, delete the input file and rename the output file.
		if (SUCCEEDED(hr)) {
			int nErr(0);
			EC_B(Generic::RemoveFile(sPathIn, nErr));
			EC_B(Generic::RenameFile(sPathOut, sPathIn));
		}
	}

	return hr;
}

// Is the sent path a directory?
/* static */ bool Generic::IsDirectory(const CString& sPath)
{
	_ASSERTE(! sPath.IsEmpty());

	bool bReturn(false);
	struct _stat s;
	::ZeroMemory(&s, sizeof s);

	// If sPath is "C:", _stat() will return false, so we fix that here.
	if (2 == sPath.GetLength()  &&  ':' == sPath.GetAt(1)) {
		UINT uiType(::GetDriveType(sPath));
		bReturn = (DRIVE_UNKNOWN != uiType)  &&  (DRIVE_NO_ROOT_DIR != uiType);
	}
	// Not just a drive. Does the path even exist?
	else if (0 == _stat(sPath, &s)) {

		// Is it a dir?
		if (s.st_mode & _S_IFDIR) {
			bReturn = true;
		}
	}

	return bReturn;
}

// Create an entire path. The Win32 ::CreateDirectory() will not create
// intermediate folders, and SHCreateDirectoryEx() is only available under
// Windows 2000. Returns a standard error code or ERROR_SUCCESS (0).
/* static */ int Generic::CreateDirectory(const CString& sPath)
{
	_ASSERTE(! sPath.IsEmpty());

	int nReturn(ERROR_SUCCESS);

	// If it doesn't already exist.
	if (! Generic::IsDirectory(sPath)) {

		// If it has a backslash, we recursively call this function again on just
		// the parent tree, and then create this child.
		if (-1 != sPath.Find('\\')) {
			CString sDir(Generic::DirFromPath(sPath));
			nReturn = Generic::CreateDirectory(sDir);

			if (ERROR_SUCCESS == nReturn) {

				nReturn = ::CreateDirectory(sPath, /* lpSecurity */ NULL) ?
				  ERROR_SUCCESS : ERROR_INVALID_FUNCTION;
			}
		}
		else {
			// Otherwise, we create the directory.
			nReturn = ::CreateDirectory(sPath, /* lpSecurity */ NULL) ?
			  ERROR_SUCCESS : ERROR_INVALID_FUNCTION;
		}
	}

	return nReturn;
}

// Get the number of subfolders *including* the sent path.
/* static */ int Generic::GetSubDirCount(const CString& sPath)
{
	_ASSERTE(! sPath.IsEmpty());
	int nReturn(0);

	// This is a recursive function. First check if the path is a dir.
	if (Generic::IsDirectory(sPath)) {
		++nReturn;

		// Process each subdir.
		CFileFind ff;
		bool bFoundFile(ff.FindFile(sPath + "\\*.*"));

		// Process all the children, recursively.
		while (bFoundFile) {
			bFoundFile = ff.FindNextFile();
			
			// Ignore special cases.
			if (! ff.IsDots()    &&
				! ff.IsHidden()  &&
				! ff.IsSystem()  &&
				! ff.IsTemporary()) {

				// Process the subdir.
				if (ff.IsDirectory()) {
					nReturn += Generic::GetSubDirCount(ff.GetFilePath());
				}
			}
		}
	}

	return nReturn;
}

// Converts from pointsize to logical units.
/* static */ int Generic::PointsToLogicalHeight(const CDC* pDC,
												int        nPoints)
{
	ASSERT_VALID(pDC);
	_ASSERTE(0 < nPoints);

	if (pDC) {
		return -MulDiv(nPoints, pDC->GetDeviceCaps(LOGPIXELSY),
		  /* nPointsPerInch */ 72);
	}
	else {
		_ASSERTE(! "pDC is NULL");
		return -1;
	}
}

// Searches for the last match of a substring.
/* static */ int Generic::ReverseFindOneOf(const CString& s,
										   const CString& sSub,
										   int            nStart /* = -1 */ )
{
	int nReturn(-1);
	int nLen(s.GetLength());
	int nLenSub(sSub.GetLength());

	if (0 < nLenSub  &&  0 < nLen) {

		if (-1 == nStart  ||  nStart >= nLen) {
			nStart = nLen - 1;
		}

		for (int nX = nStart; nX >= 0; --nX) {

			if (0 == s.Mid(nX,1).FindOneOf(sSub)) {
				nReturn = nX;
				break;
			}
		}
	}
	
	return nReturn;
}

// Find regardless of case.  Convert from  'sBig.Find(sSmall)'.
/* static */ int Generic::FindNoCase(const CString& sBig,
									 const CString& sSmall)
{
	CString sB(sBig);
	CString sS(sSmall);

	sB.MakeUpper();
	sS.MakeUpper();

	return sB.Find(sS);
}

//
/* static */ bool Generic::RemoveTrailingSlash(CString& sPath)
{
	bool bReturn(false);

	if (! sPath.IsEmpty()) {
		CString sLast(sPath.Right(1));

		if (0 == sLast.Compare("\\")) {
			int nLen(sPath.GetLength());
			sPath = sPath.Left(nLen - 1);
			bReturn = true;
		}
	}

	return bReturn;
}

// Pass in the following:
//
//		sPlacementKey -
//			"Software\Your Company Name\Your Product\Placements"
//
//		sPlacementSubKey - 
//			"YourDlg"
//
// We will use the Values of 'WindowPlacement' and 'LastScreenWidthPixels'.
/* static */ HRESULT Generic::SaveWindowPlacement(const CWnd*    pWnd,
												  const CString& sPlacementKey,
												  const CString& sPlacementSubKey)
{
	ASSERT_VALID(pWnd);
	_ASSERTE(! sPlacementKey.IsEmpty());
	_ASSERTE(! sPlacementSubKey.IsEmpty());

	HRESULT hr(S_OK);
	int nLastScreenWidthPixels(0);
	EC_B(pWnd);

	if (SUCCEEDED(hr)) {

		// Save the position for the next time.
		MyRegistry regWrite(HKEY_CURRENT_USER, sPlacementKey + "\\" +
		  sPlacementSubKey, /* bCreate */ true);

		EC_B(regWrite.IsOpen());
		WINDOWPLACEMENT wp = { sizeof WINDOWPLACEMENT };
		EC_B(pWnd->GetWindowPlacement(&wp));

		EC_B(regWrite.SaveValueBinary("WindowPlacement",
		  reinterpret_cast<BYTE*> (&wp), sizeof(wp)));

		// Save the screen resolution, so that when we re-start we can detect
		// if it's changed. If so, we won't size and position, because it
		// causes problems.
		nLastScreenWidthPixels = ::GetSystemMetrics(SM_CXSCREEN);

		EC_B(regWrite.SaveValueInt("LastScreenWidthPixels",
		  nLastScreenWidthPixels));
	}

	return hr;
}

// Pass in the following:
//
//		sPlacementKey -
//			"Software\Your Company Name\Your Product\Placements"
//
//		sPlacementSubKey - 
//			"YourDlg"
//
// We will use the Values of 'WindowPlacement' and 'LastScreenWidthPixels'.
//
// Return is the nShowCmd value the caller should use; if -1 is returned, don't
// use it!
//
// The place to call this for CMainFrame is in its ActivateFrame() override
// (use the ClassWizard to add it).
/* static */ int Generic::RestoreWindowPlacement(
									CWnd*          pWnd,
									const CString& sPlacementKey,
									const CString& sPlacementSubKey,
									int            nShowWindowCmd /* = -1 */ )
{
	ASSERT_VALID(pWnd);
	_ASSERTE(! sPlacementKey.IsEmpty());
	_ASSERTE(! sPlacementSubKey.IsEmpty());

	int nReturn(-1);

	// Restore the previous run's position (but only if it exists in the
	// Registry, and the screen reolution hasn't changed).
	MyRegistry reg;
	
	if (reg.Open(HKEY_CURRENT_USER, sPlacementKey + "\\" + sPlacementSubKey,
	  /* bCreate */ false)) {

		// Compare the screen resolution when the placement values were saved to
		// the screen resolution in use now.
		int nCurrentScreenWidthPixels(::GetSystemMetrics(SM_CXSCREEN));
		int nLastScreenWidthPixels(0);

		bool bFoundLast(reg.ReadValueInt("LastScreenWidthPixels",
			nLastScreenWidthPixels));

		if (! bFoundLast  ||
		  nCurrentScreenWidthPixels == nLastScreenWidthPixels) {

			WINDOWPLACEMENT wp;
			::ZeroMemory(&wp, sizeof(wp));
			wp.length = sizeof(wp);
			
			if (reg.ReadValueBinary("WindowPlacement",
			  reinterpret_cast<BYTE*> (&wp),
			  reinterpret_cast<unsigned long&> (wp.length))) {

				// If the user wants to override what was found, do so.
				if (-1 != nShowWindowCmd) {
					wp.showCmd = nShowWindowCmd;
				}

				// Place the window.
				VERIFY(pWnd->SetWindowPlacement(&wp));
				nReturn = wp.showCmd;
			}
		}
		else {
			// The screen resolution changed since the last time we ran, or they
			// were deleted by the OnDestroy() handler.  Don't load those obsolete
			// or non-existent placement values.
		}
	}

	// There is a MS bug that keeps maximized and minimized windows from being
	// restored correctly.  From UseNet article: "This error is caused by 
	// SetWindowPlacement() function, which works incorrectly when "showCmd" is 
	// set to SW_SHOWMINIMIZED or SW_SHOWMAXIMIZED.  SetWindowPlacement() 
	// correctly computes size for maximized or minimized window, but passes 
	// them to variables for NORMAL state!"  So, the calling function will 
	// have to take the value returned here, and use it to call ShowWindow() 
	// on itself.
	//
	// And no, we cannot do that here for the caller - it would just be
	// un-done by ActivateFrame (which is good place to call this function from;
	// just replace the value returned from this function instead of the one
	// passed in to CMainFrame::ActivateFrame() when the base class function
	// CFrameWnd::ActivateFrame() is called.

	return nReturn;
}

// Calculate a CRC for the sent buffer.
/* static */ unsigned long Generic::GetCRC(PBYTE pbyBuf, long lnLen)
{
	_ASSERTE(pbyBuf);
	_ASSERTE(0L < lnLen);

	// CRC lookup table.
	static unsigned long crc32_table[256] = { 
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
		0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
		0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
		0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
		0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
		0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
		0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
		0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
		0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
		0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
		0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
		0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
		0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
		0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
		0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
		0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
		0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
		0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
		0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
		0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
		0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
		0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
		0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
		0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
		0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
		0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
		0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
		0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
		0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
		0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
		0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
		0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
		0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
		0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
		0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
		0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
		0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
		0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
		0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
		0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D};

	// CRC value - preconditioning sets non zero value.
	unsigned long ulCRC(0xFFFFFFFF);

	// Loop through the array and calculate CRC.
	for (long lnBufferPos = 0; lnBufferPos < lnLen; ++lnBufferPos) {
		int k((ulCRC ^ pbyBuf[lnBufferPos]) & 0x000000FFL);
		ulCRC = ((ulCRC >> 8) & 0x00FFFFFFL) ^ crc32_table[k];
	}

	// Postconditioning.
	ulCRC = ~ulCRC;

	return ulCRC;
}

// Plays a WAVE resource.
void Generic::PlayWavResource(const CString& sWavName)
{
	_ASSERTE(! sWavName.IsEmpty());

	HRESULT hr(S_OK);

	HRSRC hResInfo(NULL);
	EC_B(hResInfo = ::FindResource(AfxGetResourceHandle(), sWavName, "WAVE"));

	HANDLE hRes(NULL);
	EC_B(hRes = ::LoadResource(AfxGetResourceHandle(), hResInfo));
	
	LPVOID lpRes = NULL;
	EC_B(lpRes = ::LockResource(hRes));

	EC_B(::sndPlaySound(reinterpret_cast<LPCSTR> (lpRes), SND_MEMORY |
	  SND_ASYNC | SND_NODEFAULT));
}

// Are "Small Fonts" or "Large Fonts" installed? Can return TRUE in
// 'bSmallFonts', or in 'bLargeFonts', or in neither. If 'bSmallFonts' and
// 'bLargeFonts' are *both* FALSE, then 'bNeitherButCloserToSmallFonts' will
// be set accordingly.
// 
// The following is from "INFO: Large Fonts Versus Small Fonts Use in Windows
// (Q177795)."
//
// Video drivers for the Windows operating systems have historically offered
// the capability to operate in large fonts or small fonts. These "modes"
// differ in that the fonts used in the system and in applications appear
// larger on the display.
//
// Video drivers accomplish this by reporting different values for the
// LOGPIXELSY and LOGPIXELSX flags of the GetDeviceCaps() function. They also
// install a new set of system raster fonts that directly support the new
// display units. The operating system is not aware of this difference and is
// not specifically aware of whether the user has selected large fonts or small
// fonts. Thus, it cannot report which mode the system is currently in.
//
// Traditionally, video drivers report a value of 96 pixels per inch for small
// fonts LOGPIXELSX and LOGPIXELSY values and 120 pixels per inch for large
// fonts LOGPIXELSX and LOGPIXELSY values. However, this is not reliable
// because there is no standard dictating these values. Furthermore, Windows
// 95 allows the user to customize these return values to any arbitrary value.
// Other methods, such as the name of the video driver, have historically been
// used to infer large fonts versus small fonts use as well, but none of these
// methods are reliable.
/* static */ void Generic::WhichFontsInstalled(bool& bSmallFonts,
											   bool& bLargeFonts,
											   bool& bNeitherButCloserToSmallFonts)
{
	// Initialize.
	bSmallFonts = false;
	bLargeFonts = false;
	bNeitherButCloserToSmallFonts = false;

	HWND hwndDesktop(::GetDesktopWindow());
	_ASSERTE(hwndDesktop);

	HDC hdcDesktop(::GetDC(hwndDesktop));
	_ASSERTE(hdcDesktop);

	int nLogPixelsX(::GetDeviceCaps(hdcDesktop, LOGPIXELSX));
	
	if (120 == nLogPixelsX) {
		bLargeFonts = true;
	}
	else if (96 == nLogPixelsX) {
		bSmallFonts = true;
	}
	else {
		int nSmallDiff(abs( 96 - nLogPixelsX));
		int nLargeDiff(abs(120 - nLogPixelsX));
		bNeitherButCloserToSmallFonts = nSmallDiff < nLargeDiff;
	}

	VERIFY(::ReleaseDC(hwndDesktop, hdcDesktop));
}

// Logs everything in an entire Registry tree.
/* static */ void Generic::LogRegistryTree(HKEY           hRootKey,
										   const CString& sKey,
										   int&           nDepth)
{
	_ASSERTE(hRootKey);
	_ASSERTE(! sKey.IsEmpty());
	_ASSERTE(0 <= nDepth  &&  "Should be sent by the caller as 0");

	const int nSpacing(5);

	// Open the Registry for the sent key.
	if (hRootKey  &&  ! sKey.IsEmpty()) {
		MyRegistry reg(hRootKey, sKey);

		if (reg.IsOpen()) {

			// Log this key.
			CString sPad1(' ', nDepth * nSpacing);
			DOMYLOGA ("%s%s\n", sPad1, sKey);
			++nDepth;

			// Log the values found at this key.
			CStringList slValues;
			CDWordArray dwaTypes;

			if (reg.EnumValues(slValues, dwaTypes)) {
				CString sPad2(' ', nDepth * nSpacing);
				POSITION pos(slValues.GetHeadPosition());

				for (int nIndex = 0; pos; ++nIndex) {
					CString sValue(slValues.GetNext(pos));
					_ASSERTE(! sValue.IsEmpty());

					DWORD dwType(dwaTypes.GetAt(nIndex));
					CString sType;

					switch (dwType) {
						case REG_NONE:                       sType = "REG_NONE"; break;
						case REG_SZ:                         sType = "REG_SZ"; break;
						case REG_EXPAND_SZ:                  sType = "REG_EXPAND_SZ"; break;
						case REG_BINARY:                     sType = "REG_BINARY"; break;
						case REG_DWORD:                      sType = "REG_DWORD"; break;
						case REG_DWORD_BIG_ENDIAN:           sType = "REG_DWORD_BIG_ENDIAN"; break;
						case REG_LINK:                       sType = "REG_LINK"; break;
						case REG_MULTI_SZ:                   sType = "REG_MULTI_SZ"; break;
						case REG_RESOURCE_LIST:              sType = "REG_RESOURCE_LIST"; break;
						case REG_FULL_RESOURCE_DESCRIPTOR:   sType = "REG_FULL_RESOURCE_DESCRIPTOR"; break;
						case REG_RESOURCE_REQUIREMENTS_LIST: sType = "REG_RESOURCE_REQUIREMENTS_LIST"; break;
						default: _ASSERTE(! "Illegal default case"); break;
					}

					// Now log the data for each value.
					CString sData;

					switch (dwType) {

						// Undisplayable.
						case REG_NONE:
						case REG_LINK:
						case REG_RESOURCE_LIST:
						case REG_FULL_RESOURCE_DESCRIPTOR:
						case REG_RESOURCE_REQUIREMENTS_LIST:
							sData = "{ Undisplayable }";
							break;

						// Strings.
						case REG_SZ:
						case REG_EXPAND_SZ:

							if (reg.ReadValueString(sValue, sData)) {
								;
							}
							else {
								sData = "{ Error }";
							}

							break;

						// An array of null-terminated strings, terminated by two null characters.
						case REG_MULTI_SZ:
							{
							char* pszData = NULL;

							if (reg.ReadValueMultiString(sValue, pszData)) {
								sData = pszData;
								delete [] pszData;
								pszData = NULL;
							}
							else {
								sData = "{ Error }";
							}
							}
							break;

						// DWORDs.
						case REG_DWORD:
						case REG_DWORD_BIG_ENDIAN:
							{
							DWORD dwData(0);

							if (reg.ReadValueDWord(sValue, dwData)) {
								sData.Format("%d", dwData);
							}
							else {
								sData = "{ Error }";
							}
							}
							break;

						// Binary data - display in hex format. We can log as
						// long a string as we want, but a CString can only be
						// 2 Meg in length (but we don't want to see that much).
						case REG_BINARY:
							{
							DWORD dwDataLen(0);
							PBYTE pbyData = NULL;

							if (reg.ReadValueBinary(sValue, pbyData, dwDataLen)) {

								if (0 < dwDataLen) {
									pbyData  = new BYTE[dwDataLen];

									if (reg.ReadValueBinary(sValue, pbyData, dwDataLen)) {
										CString sByte;

										for (DWORD dwByte = 0;
										  dwByte < dwDataLen  &&  dwByte < 512;
										  ++dwByte) {

											sByte.Format("%02x ", pbyData[dwByte]);
											sData += sByte;
										}

										if (512 <= dwDataLen) {
											sData += "... { truncated } ";
										}
									}
									else {
										sData = "{ Error }";
									}

									delete [] pbyData;
									pbyData  = NULL;
								}
								else {
									sData = "{ Zero-length }";
							}
							}
							}
							break;

						// Anything else.
						default:
							_ASSERTE(! "Illegal default case");
							break;
					}

					DOMYLOGA ("%sValue: %s (%s): %s\n", sPad2, sValue, sType, sData);
				}
			}

			// Get the list of all the subkeys below the sent key.
			CStringList slSubKeys;
			
			if (reg.EnumSubKeys(slSubKeys)) {
				POSITION pos(slSubKeys.GetHeadPosition());

				while (pos) {
					CString sSubKey(slSubKeys.GetNext(pos));
					_ASSERTE(! sSubKey.IsEmpty());

					// Log this subkey recursively.
					sSubKey = sKey + "\\" + sSubKey;
					LogRegistryTree(hRootKey, sSubKey, nDepth);
				}
			}

			--nDepth;
		}
	}
}