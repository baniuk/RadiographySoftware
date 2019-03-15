#if !defined(AFX_INFODILALOG_H__377BDEE0_A1E3_11D4_BB95_ABDA0ADE2C58__INCLUDED_)
#define AFX_INFODILALOG_H__377BDEE0_A1E3_11D4_BB95_ABDA0ADE2C58__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// InfoDilalog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// InfoDilalog dialog

class InfoDilalog : public CDialog
{
// Construction
public:
	InfoDilalog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(InfoDilalog)
	enum { IDD = IDD_INFO_DIALOG };
	CString	m_skalax;
	CString	m_skalay;
	CString	m_skalaz;
	CString	m_rozmiar;
	CString	m_plik;
	//}}AFX_DATA
	BOOL Create();
	CWnd* pMainWindow;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InfoDilalog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(InfoDilalog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCloseInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFODILALOG_H__377BDEE0_A1E3_11D4_BB95_ABDA0ADE2C58__INCLUDED_)
