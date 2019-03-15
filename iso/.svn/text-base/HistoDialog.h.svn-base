#if !defined(AFX_HISTODIALOG_H__9DBD5BA4_CDBE_11D4_BB95_DB32A7031D26__INCLUDED_)
#define AFX_HISTODIALOG_H__9DBD5BA4_CDBE_11D4_BB95_DB32A7031D26__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// HistoDialog.h : header file
//
#define dx 295
#define dy 200
//#include "HSlider.h"
/////////////////////////////////////////////////////////////////////////////
// HistoDialog window

class HistoDialog : public CWnd
{
// Construction
public:
	HistoDialog();
	BOOL Create(LPCSTR WindowTitle,
					DWORD dwStyle, CWnd* pParentWnd, UINT ID = NULL);
// Attributes
public:

	CWnd* pMainWindow;
//	CSliderCtrl zakres;
	int tmplut[256];
	double max;
	afx_msg void OnCloseHisto();
	void CopyLUT(int* l,BOOL status);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HistoDialog)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~HistoDialog();

	// Generated message map functions
protected:
	//{{AFX_MSG(HistoDialog)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTODIALOG_H__9DBD5BA4_CDBE_11D4_BB95_DB32A7031D26__INCLUDED_)
