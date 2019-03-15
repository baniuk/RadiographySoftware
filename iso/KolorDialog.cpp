// KolorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "KolorDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KolorDialog dialog


KolorDialog::KolorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(KolorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(KolorDialog)
	m_czyroi = FALSE;
	m_czyorg = FALSE;
	//}}AFX_DATA_INIT
	pMainWindow=pParent;
	CzyTh=FALSE;
	CzyHis=FALSE;
	AfxEnableControlContainer();
}


void KolorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(KolorDialog)
	DDX_Control(pDX, IDC_BUTTON_THRESHOLD, m_thres);
	DDX_Control(pDX, IDC_SLIDER2, m_slider);
	DDX_Check(pDX, IDC_CZYROI, m_czyroi);
	DDX_Check(pDX, IDC_CZYORG, m_czyorg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(KolorDialog, CDialog)
	//{{AFX_MSG_MAP(KolorDialog)
	ON_BN_CLICKED(IDC_BUTTON_THRESHOLD, OnButtonThreshold)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CZYROI, OnCzyroi)
	ON_BN_CLICKED(IDC_CZYORG, OnCzyorg)
	ON_BN_CLICKED(ID_CLOSE_KOLOR, OnCloseKolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// KolorDialog message handlers

BOOL KolorDialog::Create()
{
	BOOL ReturnValue=CDialog::Create(KolorDialog::IDD,pMainWindow);

	return ReturnValue;
}

void KolorDialog::OnButtonThreshold() 
{
	if(CzyTh)
		CzyTh=FALSE;
	else
		CzyTh=TRUE;
	pMainWindow->PostMessage(WM_EX);
	
}



void KolorDialog::OnClose() 
{
	DestroyWindow();
	pMainWindow->PostMessage(WM_DESTROY_KOLOR);
	
	CDialog::OnClose();
}

void KolorDialog::OnCloseKolor() 
{
	OnClose();
	
}

BEGIN_EVENTSINK_MAP(KolorDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(KolorDialog)
	ON_EVENT(KolorDialog, IDC_SLIDER2, 2 /* Change */, OnChangeSlider2, VTS_NONE)
	ON_EVENT(KolorDialog, IDC_SLIDER2, -602 /* KeyDown */, OnKeyDownSlider2, VTS_PI2 VTS_I2)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


void KolorDialog::OnChangeSlider2() 
{
	pMainWindow->PostMessage(WM_CHANGE_SLIDER);
	
	
}

void KolorDialog::OnOK()
{


}


void KolorDialog::OnCzyroi() 
{
	pMainWindow->PostMessage(WM_KOLOR_UPDATE);
	
}

void KolorDialog::OnCzyorg() 
{
	pMainWindow->PostMessage(WM_KOLOR_UPDATE);
	
}



void KolorDialog::OnKeyDownSlider2(short FAR* KeyCode, short Shift) 
{
	// TODO: Add your control notification handler code here
	
}
