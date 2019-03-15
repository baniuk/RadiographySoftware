// iso.h : main header file for the ISO application
//

#if !defined(AFX_ISO_H__828DA2A4_A121_11D4_BB95_CC0761F9C633__INCLUDED_)
#define AFX_ISO_H__828DA2A4_A121_11D4_BB95_CC0761F9C633__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CIsoApp:
// See iso.cpp for the implementation of this class
//

class CIsoApp : public CWinApp
{
public:
	CIsoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIsoApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISO_H__828DA2A4_A121_11D4_BB95_CC0761F9C633__INCLUDED_)
