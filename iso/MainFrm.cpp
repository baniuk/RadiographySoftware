// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "iso.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	IDS_SKALA,	// status line indicator
	
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	
	
}

CMainFrame::~CMainFrame()
{
		
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	CRect zz(134,0,182,15);
	CRect ss(0,0,130,15);
//	CRect cc(0,0,130,15);
//	ClientToScreen(cc);
	ClientToScreen(ss);
	ClientToScreen(zz);
	
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.Create(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	
	if (!m_wndToolBar2.Create(this) ||
		!m_wndToolBar2.LoadToolBar(IDR_OKNA))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar2.SetButtonStyle(0,TBBS_CHECKBOX);
	m_wndToolBar2.SetButtonStyle(1,TBBS_CHECKBOX);
	m_wndToolBar2.SetButtonStyle(2,TBBS_CHECKBOX);
	m_wndToolBar2.SetButtonStyle(3,TBBS_CHECKBOX);

	m_wndToolBar1.Create(this);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo(1,IDS_SKALA,SBPS_NORMAL,280 );

	// TODO: Remove this if you don't want tool tips or a resizeable toolbar
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar1.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	m_wndToolBar2.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	m_wndToolBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar,CBRS_ALIGN_ANY,ss);
	DockControlBar(&m_wndToolBar1,CBRS_ALIGN_ANY,zz);
	DockControlBar(&m_wndToolBar2,AFX_IDW_DOCKBAR_RIGHT);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
//	cs.cx=640;
//	cs.cy=480;
	cs.style=WS_OVERLAPPEDWINDOW|WS_MAXIMIZE;
	
	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers



