#pragma once
#include "afxcmn.h"


// C_MyEditDialog dialog

class C_MyEditDialog : public CDialog
{
	DECLARE_DYNAMIC(C_MyEditDialog)

public:
	C_MyEditDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~C_MyEditDialog();
	CWnd* pMainWindow;
	BOOL Create(LPCSTR WindowTitle,	DWORD dwStyle, CWnd* pParentWnd, UINT ID = NULL);
// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CRichEditCtrl mydialog;
};
