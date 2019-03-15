//{{AFX_INCLUDES()
#include "slider.h"
//}}AFX_INCLUDES
#if !defined(AFX_KOLORDIALOG_H__46F03E24_C16E_11D4_BB95_FD61CDE437A0__INCLUDED_)
#define AFX_KOLORDIALOG_H__46F03E24_C16E_11D4_BB95_FD61CDE437A0__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// KolorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KolorDialog dialog

class KolorDialog : public CDialog
{
// Construction
public:
	KolorDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(KolorDialog)
	enum { IDD = IDD_KOLOR_DIALOG };
	CButton	m_thres;
	CSlider	m_slider;
	BOOL	m_czyroi;
	BOOL	m_czyorg;
	//}}AFX_DATA
	afx_msg void OnCloseKolor();
	BOOL Create();
	CWnd* pMainWindow;
	BOOL CzyTh;
	BOOL CzyHis;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KolorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KolorDialog)
	virtual void OnOK();
	afx_msg void OnButtonThreshold();
	afx_msg void OnClose();
	afx_msg void OnChangeSlider2();
	afx_msg void OnCzyroi();
	afx_msg void OnCzyorg();
	afx_msg void OnKeyDownSlider2(short FAR* KeyCode, short Shift);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KOLORDIALOG_H__46F03E24_C16E_11D4_BB95_FD61CDE437A0__INCLUDED_)
