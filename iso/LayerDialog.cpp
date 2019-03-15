// LayerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "LayerDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LayerDialog dialog


LayerDialog::LayerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(LayerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(LayerDialog)
	m_layer = _T("");
	m_widok = -1;
	m_typ = -1;
	//}}AFX_DATA_INIT
	pMainWindow=pParent;
	AfxEnableControlContainer();
}


void LayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LayerDialog)
	DDX_Control(pDX, IDC_BUTTON4, m_b4);
	DDX_Control(pDX, IDC_BUTTON3, m_b3);
	DDX_Control(pDX, IDC_BUTTON2, m_b2);
	DDX_Control(pDX, IDC_BUTTON1, m_b1);
	DDX_CBString(pDX, IDC_COMBO3, m_layer);
	DDX_Radio(pDX, IDC_RADIO1, m_widok);
	DDX_Radio(pDX, IDC_RADIO4, m_typ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LayerDialog, CDialog)
	//{{AFX_MSG_MAP(LayerDialog)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangeCombo3)
	ON_BN_CLICKED(ID_CLOSE_LAYER, OnCloseLayer)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO5, OnRadio5)
	ON_BN_CLICKED(IDC_RADIO6, OnRadio6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LayerDialog message handlers

BOOL LayerDialog::Create()
{
	BOOL ReturnValue=CDialog::Create(LayerDialog::IDD,pMainWindow);

	return ReturnValue;
}


void LayerDialog::OnClose() 
{
	DestroyWindow();
	pMainWindow->PostMessage(WM_DESTROY_LAYER);
		
	CDialog::OnClose();
}

BEGIN_EVENTSINK_MAP(LayerDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(LayerDialog)
	ON_EVENT(LayerDialog, IDC_UPDOWN1, 3 /* UpClick */, OnUpClickUpdown1, VTS_NONE)
	ON_EVENT(LayerDialog, IDC_UPDOWN1, 2 /* DownClick */, OnDownClickUpdown1, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void LayerDialog::OnUpClickUpdown1() 
{
	
	pMainWindow->PostMessage(WM_LAYERUP_UPDATE);
	
}

void LayerDialog::OnDownClickUpdown1() 
{
	pMainWindow->PostMessage(WM_LAYERDOWN_UPDATE);
	
}


void LayerDialog::OnSelchangeCombo3() 
{
	pMainWindow->PostMessage(WM_LAYER_SELECT);
	
}

void LayerDialog::OnCloseLayer() 
{
	OnClose();
	
}


/*void LayerDialog::OnSelchangeCombo4() 
{
	pMainWindow->PostMessage(WM_SKALA_SELECT);
	
}*/

void LayerDialog::OnButton1() 
{
	m_skala=1;
	m_b1.SetCheck(1);
	m_b2.SetCheck(0);
	m_b3.SetCheck(0);
	m_b4.SetCheck(0);
	pMainWindow->PostMessage(WM_SKALA_SELECT);
	
}

void LayerDialog::OnButton2() 
{
	m_skala=2;
	m_b1.SetCheck(0);
	m_b2.SetCheck(1);
	m_b3.SetCheck(0);
	m_b4.SetCheck(0);
	pMainWindow->PostMessage(WM_SKALA_SELECT);
	
}

void LayerDialog::OnButton3() 
{
	m_skala=3;
	m_b1.SetCheck(0);
	m_b2.SetCheck(0);
	m_b3.SetCheck(1);
	m_b4.SetCheck(0);
	pMainWindow->PostMessage(WM_SKALA_SELECT);
	
}

void LayerDialog::OnButton4() 
{
	m_skala=4;
	m_b1.SetCheck(0);
	m_b2.SetCheck(0);
	m_b3.SetCheck(0);
	m_b4.SetCheck(1);
	pMainWindow->PostMessage(WM_SKALA_SELECT);
	
}

void LayerDialog::OnRadio1() 
{
	pMainWindow->PostMessage(WM_WIDOK_SELECT);
		
}

void LayerDialog::OnRadio2() 
{
	pMainWindow->PostMessage(WM_WIDOK_SELECT);
	
}

void LayerDialog::OnRadio3() 
{
	pMainWindow->PostMessage(WM_WIDOK_SELECT);
	
}

void LayerDialog::OnRadio4() 
{
	pMainWindow->PostMessage(WM_WIDOK_SELECT);
		
}

void LayerDialog::OnRadio5() 
{
	pMainWindow->PostMessage(WM_WIDOK_SELECT);
	
}

void LayerDialog::OnRadio6() 
{
	pMainWindow->PostMessage(WM_WIDOK_SELECT);
	
}

void LayerDialog::OnOK()
{




}



