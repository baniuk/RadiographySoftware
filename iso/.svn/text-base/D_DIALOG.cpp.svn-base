// D_DIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "D_DIALOG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// D_DIALOG dialog


D_DIALOG::D_DIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(D_DIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(D_DIALOG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	pMainWindow=pParent;
}


void D_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(D_DIALOG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(D_DIALOG, CDialog)
	//{{AFX_MSG_MAP(D_DIALOG)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MAKE_ALL, OnMakeAll)
	ON_BN_CLICKED(ID_CLOSE_DD, OnCloseDDialog)
	ON_BN_CLICKED(IDC_MAKE_CURRENT, OnMakeCurrent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// D_DIALOG message handlers

BOOL D_DIALOG::Create() 
{
	BOOL ReturnValue=CDialog::Create(D_DIALOG::IDD,pMainWindow);

	return ReturnValue;
}

void D_DIALOG::OnClose() 
{
	DestroyWindow();
	pMainWindow->PostMessage(WM_DESTROY_DDIALOG);
	
	CDialog::OnClose();
}

void D_DIALOG::OnCloseDDialog()
{
	OnClose();
}


void D_DIALOG::OnMakeAll() 
{
	pMainWindow->PostMessage(WM_MAKE_ALL);
	
}

void D_DIALOG::OnMakeCurrent() 
{
	pMainWindow->PostMessage(WM_MAKE_CURRENT);
	
}
