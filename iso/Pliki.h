#if !defined(AFX_PLIKI_H__ED565725_A1B8_11D4_BB95_C55C8338C269__INCLUDED_)
#define AFX_PLIKI_H__ED565725_A1B8_11D4_BB95_C55C8338C269__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Pliki.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Pliki dialog

class Pliki : public CFileDialog
{
	DECLARE_DYNAMIC(Pliki)

public:
	Pliki(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

protected:
	//{{AFX_MSG(Pliki)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLIKI_H__ED565725_A1B8_11D4_BB95_C55C8338C269__INCLUDED_)
