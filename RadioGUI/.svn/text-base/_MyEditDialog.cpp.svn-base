// _MyEditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "RadioGUI.h"
#include "_MyEditDialog.h"


// C_MyEditDialog dialog

IMPLEMENT_DYNAMIC(C_MyEditDialog, CDialog)

C_MyEditDialog::C_MyEditDialog(CWnd* pParent /*=NULL*/)
	: CDialog(C_MyEditDialog::IDD, pParent)
{

}

C_MyEditDialog::~C_MyEditDialog()
{
}

void C_MyEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT21, mydialog);
}


BEGIN_MESSAGE_MAP(C_MyEditDialog, CDialog)
END_MESSAGE_MAP()


// C_MyEditDialog message handlers

BOOL C_MyEditDialog::Create(LPCSTR WindowTitle, DWORD dwStyle,CWnd* pParent, UINT nID)
{
	CBrush blackBrush(RGB(0,0,0));
	CString ChildWindowClass = AfxRegisterWndClass(
				CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
				AfxGetApp()->LoadStandardCursor(IDC_CROSS),
				(HBRUSH)blackBrush.GetSafeHandle(),
				AfxGetApp()->LoadIcon(IDI_ICON2));
	pMainWindow=pParent;
	CRect ChildRect(0, 0, 500, 500);
	CreateEx(WS_EX_CONTROLPARENT,ChildWindowClass, WindowTitle, dwStyle,
						ChildRect, pParent, nID);
	return TRUE;
}