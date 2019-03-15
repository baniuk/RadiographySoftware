// HistoDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "HistoDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HistoDialog

HistoDialog::HistoDialog()
{
	
}

HistoDialog::~HistoDialog()
{
}


BEGIN_MESSAGE_MAP(HistoDialog, CWnd)
	//{{AFX_MSG_MAP(HistoDialog)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// HistoDialog message handlers
BOOL HistoDialog::Create(LPCSTR WindowTitle, DWORD dwStyle,	CWnd* pParent, UINT nID)
{
	CBrush yellowBrush(RGB(220, 220, 220));
	CString ChildWindowClass = AfxRegisterWndClass(
				CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
				AfxGetApp()->LoadStandardCursor(IDC_CROSS),
				(HBRUSH)yellowBrush.GetSafeHandle(),
				AfxGetApp()->LoadIcon(IDI_H_ICON));
	pMainWindow=pParent;
	CRect ChildRect(400, 400, 400+dx, 400+dy);
	CWnd::Create(ChildWindowClass, WindowTitle, dwStyle,
						ChildRect, pParent, nID);
	return TRUE;
}

void HistoDialog::OnDestroy() 
{
	CWnd::OnDestroy();
	
	pMainWindow->PostMessage(WM_DESTROY_HISTO);

}

void HistoDialog::OnCloseHisto()
{
	DestroyWindow();

}

void HistoDialog::CopyLUT(int* l,BOOL status)
{
	for(int i=0;i<256;i++)
		tmplut[i]=l[i];
	double max=0;
	double s;
	for(i=0;i<256;i++)
		if(tmplut[i]>max)
			max=tmplut[i];
	for(i=0;i<256;i++)
	{
		s=((double)tmplut[i]*dy-40)/max;
		tmplut[i]=s;
	}
	if(status)
		Invalidate();
}

void HistoDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int x=27;
	for(int l=0;l<256;l++)
	{
		dc.MoveTo(x,dy-30);
		dc.LineTo(x,dy-30-tmplut[l]);
		x+=2;
		
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}

/*int HistoDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rect(0,0,25,170);
	zakres.Create(TBS_VERT|WS_VISIBLE|WS_CHILD,rect,this,IDC_ZAKRES);
	zakres.CreateEx(WS_EX_CLIENTEDGE,"SLIDER",NULL,TBS_VERT|WS_VISIBLE,rect,this,IDC_ZAKRES);
	zakres.SetRange(1,6,TRUE );

	
	return 0;
}*/





//BOOL HistoDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
//{
//	LPNMHDR pnmh=(LPNMHDR) lParam; 
//	if(pnmh->code==TB_THUMBTRACK)
//		MessageBeep(0xffffffff);
//	
//	return CWnd::OnNotify(wParam, lParam, pResult);
//}




