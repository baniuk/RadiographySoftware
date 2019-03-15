#if !defined(AFX_LAYERDIALOG_H__287A0CE4_A39C_11D4_BB95_EE49A11A3794__INCLUDED_)
#define AFX_LAYERDIALOG_H__287A0CE4_A39C_11D4_BB95_EE49A11A3794__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// LayerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LayerDialog dialog

class LayerDialog : public CDialog
{
// Construction
public:
	LayerDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LayerDialog)
	enum { IDD = IDD_LAYER };
	CButton	m_b4;
	CButton	m_b3;
	CButton	m_b2;
	CButton	m_b1;
	CString	m_layer;
	int		m_widok;
	int		m_typ;
	//}}AFX_DATA
	BOOL Create();
	CWnd* pMainWindow;
	unsigned int m_skala;
	afx_msg void OnCloseLayer();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LayerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LayerDialog)
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnUpClickUpdown1();
	afx_msg void OnDownClickUpdown1();
	afx_msg void OnSelchangeCombo3();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnRadio6();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERDIALOG_H__287A0CE4_A39C_11D4_BB95_EE49A11A3794__INCLUDED_)
