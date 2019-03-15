// _ToolBar1.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "_ToolBar1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// C_ToolBar1

C_ToolBar1::C_ToolBar1()
{
	s=10;
}

C_ToolBar1::~C_ToolBar1()
{
}


BEGIN_MESSAGE_MAP(C_ToolBar1, CToolBar)
	//{{AFX_MSG_MAP(C_ToolBar1)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_LISTA, OnSelchangeLista)	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// C_ToolBar1 message handlers


int C_ToolBar1::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	LoadToolBar(IDR_TB1);
	SetButtonStyle(0,TBBS_CHECKBOX);
	SetButtonStyle(1,TBBS_CHECKBOX);
	SetButtonStyle(2,TBBS_CHECKBOX);
	SetButtonStyle(4,TBBS_CHECKBOX);
	SetButtonStyle(5,TBBS_CHECKBOX);
	SetButtonStyle(6,TBBS_CHECKBOX);
	SetButtonStyle(7,TBBS_CHECKBOX);
	SetButtonStyle(12,TBBS_CHECKBOX);
	SetButtonInfo(9,IDC_LISTA,TBBS_SEPARATOR,60);
	SetButtonInfo(10,IDC_SPIN,TBBS_SEPARATOR,35);

	CRect rect;
	CRect spin;
	GetItemRect(9,&rect);
	GetItemRect(10,&spin);
	rect.bottom=rect.top+150;
	spin.bottom=spin.top+22;
	lista.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|WS_VSCROLL,rect,this,IDC_LISTA);
	lista.AddString("0");
	lista.AddString("20");
	lista.AddString("40");
	lista.AddString("60");
	lista.AddString("80");
	lista.AddString("100");
	lista.AddString("120");
	lista.AddString("140");
	lista.AddString("160");
	lista.AddString("180");
	lista.AddString("200");
	lista.AddString("220");
	lista.AddString("240");
	lista.AddString("255");
	lista.SetCurSel(0);
	Spin.Create(WS_CHILD|WS_VISIBLE|UDS_HORZ,spin,this,IDC_SPIN);

	
	return 0;
}


void C_ToolBar1::OnSelchangeLista()
{
	
/*	POSITION posView=GetFirstViewPosition();
	CView* pView=GetNextView(posView);
	pView->PostMessage(WM_LISTA);*/
}
