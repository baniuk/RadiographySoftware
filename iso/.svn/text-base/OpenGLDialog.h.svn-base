#if !defined(AFX_OPENGLDIALOG_H__F92031E1_EFDB_11D4_BB95_837330191578__INCLUDED_)
#define AFX_OPENGLDIALOG_H__F92031E1_EFDB_11D4_BB95_837330191578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGLDialog.h : header file
//
#include "_Punkty.h"
/////////////////////////////////////////////////////////////////////////////
// OpenGLDialog window

class OpenGLDialog : public CWnd
{
// Construction
public:
	OpenGLDialog();
	BOOL Create(LPCSTR WindowTitle,
					DWORD dwStyle, CWnd* pParentWnd, UINT ID = NULL);
	void GetPoints(CPtrArray* _ppoints);
	void OnCloseGl();

// Attributes
public:
	CWnd* pMainWindow;
	HGLRC hRC;
	HDC hDC;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OpenGLDialog)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~OpenGLDialog();

	// Generated message map functions
private:
	CPtrArray* ppointGL;
	C_Punkty* tmp;
protected:
	//{{AFX_MSG(OpenGLDialog)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLDIALOG_H__F92031E1_EFDB_11D4_BB95_837330191578__INCLUDED_)
