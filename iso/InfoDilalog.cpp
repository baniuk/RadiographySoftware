// InfoDilalog.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "InfoDilalog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InfoDilalog dialog


InfoDilalog::InfoDilalog(CWnd* pParent /*=NULL*/)
	: CDialog(InfoDilalog::IDD, pParent)
{
	//{{AFX_DATA_INIT(InfoDilalog)
	m_skalax = _T("");
	m_skalay = _T("");
	m_skalaz = _T("");
	m_rozmiar = _T("");
	m_plik = _T("");
	//}}AFX_DATA_INIT
	pMainWindow=pParent;
}


void InfoDilalog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InfoDilalog)
	DDX_Text(pDX, IDC_STATIC_SKALAX, m_skalax);
	DDX_Text(pDX, IDC_STATIC_SKALAY, m_skalay);
	DDX_Text(pDX, IDC_STATIC_SKALAZ, m_skalaz);
	DDX_Text(pDX, IDC_STATIC_ROZMIAR, m_rozmiar);
	DDX_Text(pDX, IDC_STATIC_PLIK, m_plik);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(InfoDilalog, CDialog)
	//{{AFX_MSG_MAP(InfoDilalog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_CLOSE_INFO, OnCloseInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InfoDilalog message handlers

BOOL InfoDilalog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL InfoDilalog::Create()
{
	BOOL ReturnValue=CDialog::Create(InfoDilalog::IDD,pMainWindow);
//	UpdateData(FALSE);
	return ReturnValue;
}

void InfoDilalog::OnClose() 
{
	DestroyWindow();
	
	CDialog::OnClose();
}

void InfoDilalog::OnCloseInfo() 
{
	OnClose();
}




