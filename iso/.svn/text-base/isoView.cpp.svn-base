// isoView.cpp : implementation of the CIsoView class
//

#include "stdafx.h"
#include "iso.h"

#include "isoDoc.h"
#include "isoView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIsoView

IMPLEMENT_DYNCREATE(CIsoView, CScrollView)

BEGIN_MESSAGE_MAP(CIsoView, CScrollView)
	//{{AFX_MSG_MAP(CIsoView)
	ON_COMMAND(ID_WINDOW_INFO, OnWindowInfo)
	ON_COMMAND(ID_LAYER_INFO, OnLayerInfo)
	ON_COMMAND(ID_TOOL_XY, OnToolXy)
	ON_COMMAND(ID_TOOL_XZ, OnToolXz)
	ON_COMMAND(ID_TOOL_YZ, OnToolYz)
	ON_UPDATE_COMMAND_UI(ID_TOOL_XY, OnUpdateToolXy)
	ON_UPDATE_COMMAND_UI(ID_TOOL_XZ, OnUpdateToolXz)
	ON_UPDATE_COMMAND_UI(ID_TOOL_YZ, OnUpdateToolYz)
	ON_COMMAND(ID_TOOL_100, OnTool100)
	ON_COMMAND(ID_TOOL_200, OnTool200)
	ON_COMMAND(ID_TOOL_300, OnTool300)
	ON_COMMAND(ID_TOOL_400, OnTool400)
	ON_UPDATE_COMMAND_UI(ID_TOOL_100, OnUpdateTool100)
	ON_UPDATE_COMMAND_UI(ID_TOOL_200, OnUpdateTool200)
	ON_UPDATE_COMMAND_UI(ID_TOOL_300, OnUpdateTool300)
	ON_UPDATE_COMMAND_UI(ID_TOOL_400, OnUpdateTool400)
	ON_COMMAND(ID_KOLOR_DIALOG, OnKolorDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_TOOLBAR_KOLOR, OnToolbarKolor)
	ON_COMMAND(ID_TOOLBAR_LAYER, OnToolbarLayer)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_KOLOR, OnUpdateToolbarKolor)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_LAYER, OnUpdateToolbarLayer)
	ON_COMMAND(ID_HISTO_DIALOG, OnHistoDialog)
	ON_COMMAND(ID_TOOLBAR_HISTO, OnToolbarHisto)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_HISTO, OnUpdateToolbarHisto)
	ON_COMMAND(ID_SLICE_SAVE, OnSliceSave)
	ON_COMMAND(ID_MODE, OnMode)
	ON_COMMAND(ID_OBRYS, OnObrys)
	ON_COMMAND(ID_GL_DIALOG, OnGlDialog)
	ON_COMMAND(ID_TOOLBAR_GL, OnToolbarGl)
	ON_MESSAGE(WM_DLG_UPDATE,OnInfoDialogUpdate)
	ON_MESSAGE(WM_LAYERUP_UPDATE,OnLayerUpDialogUpdate)
	ON_MESSAGE(WM_KOLOR_UPDATE,OnKolorUpdate)
	ON_MESSAGE(WM_LAYERDOWN_UPDATE,OnLayerDownDialogUpdate)
	ON_MESSAGE(WM_LAYER_SELECT,OnLayerSelect)
	ON_MESSAGE(WM_SKALA_SELECT,OnSkalaSelect)
	ON_MESSAGE(WM_WIDOK_SELECT,OnWidokSelect)
	ON_MESSAGE(WM_DESTROY_LAYER,OnDestroyLayer)
	ON_MESSAGE(WM_DESTROY_DDIALOG,OnDestroyDDialog)
	ON_MESSAGE(WM_DESTROY_KOLOR,OnDestroyKolor)
	ON_MESSAGE(WM_MAKE_ALL,OnMakeAll1)
	ON_MESSAGE(WM_MAKE_CURRENT,OnMakeCurrent1)
	ON_MESSAGE(WM_DESTROY_HISTO,OnDestroyHisto)
	ON_MESSAGE(WM_DESTROY_GLDIALOG,OnDestroyGl)
	ON_MESSAGE(ID_UPDATE_NEW_LAYER_DIALOG,OnLayerUpdateDialog)
	ON_MESSAGE(WM_LISTA,OnLista)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN, OnDeltaposSpin)
	ON_MESSAGE(WM_CHANGE_SLIDER,OnChangeSlider)
	ON_MESSAGE(WM_EX,Extract)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_GL, OnUpdateToolbarGl)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(IDS_SKALA, OnUpdateStatusB)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsoView construction/destruction

CIsoView::CIsoView()
{
	pInfoDialog=new InfoDilalog(this);
	pLayerDialog=new LayerDialog(this);
	pKolorDialog=new KolorDialog(this);
	pHistoDialog=new HistoDialog();
	pOpenGLDialog=new OpenGLDialog();
	pDDialog=new D_DIALOG(this);
	MouseStatus=0;
	dialogL=FALSE;
	dialogK=FALSE;
	dialogH=FALSE;
	dialogO=FALSE;
	dialog3D=FALSE;
	Wlayer=NULL;
	lut.num=256+1;
	lut.factor=NULL;
	lut.interpolateType=IPL_LUT_LOOKUP;
	lut.key=new int[256+1];
	lut.value=new int[256];
	plut=&lut;
	for(int i=0; i<=256; i++)
		lut.key[i] = i;
	pHistoDialog->CopyLUT(lut.value,dialogH);
	czyOrg=FALSE;
/*	plogpal=(LOGPALETTE* ) new char [sizeof(LOGPALETTE) +256*sizeof(PALETTEENTRY)];
	plogpal->palVersion=0x300;
	plogpal->palNumEntries=256;
	
	for(a=0;a<256;a++)
	{
		plogpal->palPalEntry[a].peRed=a;
		plogpal->palPalEntry[a].peGreen=a;
		plogpal->palPalEntry[a].peBlue=a;
		plogpal->palPalEntry[a].peRed=0;
	}*/
//	pl=new CPalette;
//	pl->CreatePalette(plogpal);
	
}

CIsoView::~CIsoView()
{
	delete pInfoDialog;
	delete pLayerDialog;
	delete pKolorDialog;
	delete pHistoDialog;
	delete pOpenGLDialog;
	delete pDDialog;
	delete lut.key;
	delete lut.value;
	
	if(!Wlayer)
		iplDeallocate(Wlayer,IPL_IMAGE_ALL);

	for(int b=0;b<pointGL.GetSize();b++)
		delete (C_Punkty* )pointGL.GetAt(b);
	pointGL.RemoveAll();


}

BOOL CIsoView::PreCreateWindow(CREATESTRUCT& cs)
{
	CBrush Tlo(RGB(250,250,250));
	cs.lpszClass=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_OWNDC|CS_BYTEALIGNWINDOW,AfxGetApp()->LoadStandardCursor(IDC_CROSS),(HBRUSH)Tlo.GetSafeHandle());
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CIsoView drawing

void CIsoView::OnDraw(CDC* pDC)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
//	pDoc->m_cube.info.biWidth,pDoc->m_cube.info.biHeight
	if(pDoc->m_cube.mode==SLICE)
	{
		StretchDIBits(pDC->m_hDC,0,0,pDoc->m_cube.info.biWidth,pDoc->m_cube.info.biHeight,0,0,pDoc->m_cube.info.biWidth,pDoc->m_cube.info.biHeight,pDoc->m_cube.layer,&pDoc->m_cube.binfo,DIB_RGB_COLORS,SRCCOPY);
		return;
	}
	if(pDoc->m_cube.acttyp==0)
		StretchDIBits(pDC->m_hDC,0,0,bok*pDoc->m_cube.actskala,bok*pDoc->m_cube.actskala,0,0,bok,bok,pDoc->m_cube.layer,&pDoc->m_cube.binfo,DIB_RGB_COLORS,SRCCOPY);
	else
		StretchDIBits(pDC->m_hDC,0,0,bok*pDoc->m_cube.actskala,bok*pDoc->m_cube.actskala,0,0,bok*pDoc->m_cube.actskala,bok*pDoc->m_cube.actskala,pDoc->m_cube.layer,&pDoc->m_cube.binfo,DIB_RGB_COLORS,SRCCOPY);
	if(pDoc->m_cube.czyROI)
	{
		CPen MyPen(PS_SOLID,1,RGB(128,128,128));
		CBrush MyBrush;
		MyBrush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pOldBrush=pDC->SelectObject(&MyBrush);
		CPen* pOldPen=pDC->SelectObject(&MyPen);
		pDC->SetROP2(R2_XORPEN);
		pDC->Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		pDC->SetROP2(R2_COPYPEN);
		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}


}

void CIsoView::OnInitialUpdate()
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = pDoc->m_cube.obszar.Width();
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CIsoView diagnostics

#ifdef _DEBUG
void CIsoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CIsoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CIsoDoc* CIsoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIsoDoc)));
	return (CIsoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIsoView message handlers

void CIsoView::OnWindowInfo() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!(pInfoDialog->GetSafeHwnd()==0)) return;
	PostMessage(WM_DLG_UPDATE);
	pInfoDialog->Create();
	
}

void CIsoView::OnInfoDialogUpdate(UINT wParam,LONG IParam)
{
	if(pInfoDialog->GetSafeHwnd()==0) return;
	CString x,y,z;
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	x.Format("%.4f",pDoc->m_cube.skalaX);
	y.Format("%.4f",pDoc->m_cube.skalaY);
	z.Format("%.4f",pDoc->m_cube.skalaZ);
	pInfoDialog->m_rozmiar="256x256x256";
	pInfoDialog->m_plik=pDoc->m_cube.wepath;
	pInfoDialog->m_skalax=x;
	pInfoDialog->m_skalay=y;
	pInfoDialog->m_skalaz=z;
	pInfoDialog->UpdateData(FALSE);
}

void CIsoView::OnLayerUpDialogUpdate(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actlayer==255)
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	pDoc->m_cube.actlayer++;
	if((pLayerDialog->GetSafeHwnd()!=0))
	{
		pLayerDialog->m_layer.Format("%.3u",pDoc->m_cube.actlayer);
		pLayerDialog->UpdateData(FALSE);
	}
	Extract(0,0);
//	InvalidateRect(&pDoc->m_cube.obszar,FALSE);
}

void CIsoView::OnLayerDownDialogUpdate(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actlayer==0)
	{
		MessageBeep(0xFFFFFFFF);
		return;
	}
	pDoc->m_cube.actlayer--;
	if((pLayerDialog->GetSafeHwnd()!=0))
	{
		pLayerDialog->m_layer.Format("%.3u",pDoc->m_cube.actlayer);
		pLayerDialog->UpdateData(FALSE);
	}
	Extract(0,0);
//	InvalidateRect(&pDoc->m_cube.obszar,FALSE);
}

void CIsoView::OnUpdateStatusB(CCmdUI *pCmdUI)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pCmdUI->Enable(); 
	CString Info;
	Info.Format("X:%.4f    Y:%.4f    Z:%.4f    L:%d    S:%d%%    W:%d",pDoc->m_cube.skalaX*pDoc->m_cube.actskala,pDoc->m_cube.skalaY*pDoc->m_cube.actskala,pDoc->m_cube.skalaZ*pDoc->m_cube.actskala,pDoc->m_cube.actlayer,pDoc->m_cube.actskala*100,pDoc->m_cube.actwidok);

	pCmdUI->SetText( Info );


}

void CIsoView::OnLayerInfo() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!(pLayerDialog->GetSafeHwnd()==0))
		return;
	pLayerDialog->Create();
	dialogL=TRUE;
	pLayerDialog->m_layer.Format("%.3u",pDoc->m_cube.actlayer);
	switch(pDoc->m_cube.actskala)
	{
	case 1:
		pLayerDialog->m_b1.SetCheck(1);
		pLayerDialog->m_b2.SetCheck(0);
		pLayerDialog->m_b3.SetCheck(0);
		pLayerDialog->m_b4.SetCheck(0);
		break;
	case 2:
		pLayerDialog->m_b1.SetCheck(0);
		pLayerDialog->m_b2.SetCheck(1);
		pLayerDialog->m_b3.SetCheck(0);
		pLayerDialog->m_b4.SetCheck(0);
		break;
	case 3:
		pLayerDialog->m_b1.SetCheck(0);
		pLayerDialog->m_b2.SetCheck(0);
		pLayerDialog->m_b3.SetCheck(1);
		pLayerDialog->m_b4.SetCheck(0);
		break;
	case 4:
		pLayerDialog->m_b1.SetCheck(0);
		pLayerDialog->m_b2.SetCheck(0);
		pLayerDialog->m_b3.SetCheck(0);
		pLayerDialog->m_b4.SetCheck(1);
		break;
	}
	pLayerDialog->m_widok=pDoc->m_cube.actwidok;
	pLayerDialog->m_typ=pDoc->m_cube.acttyp;
	pLayerDialog->UpdateData(FALSE);
}

void CIsoView::OnLayerSelect(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pLayerDialog->UpdateData(TRUE);
	pDoc->m_cube.actlayer=atoi(pLayerDialog->m_layer);
	Extract(0,0);
//	InvalidateRect(&pDoc->m_cube.obszar,FALSE);

}

void CIsoView::OnLayerUpdateDialog(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if((pLayerDialog->GetSafeHwnd()==0))
		return;
	pLayerDialog->m_layer.Format("%.3u",pDoc->m_cube.actlayer);
	switch(pDoc->m_cube.actskala)
	{
	case 1:
		pLayerDialog->m_b1.SetCheck(1);
		pLayerDialog->m_b2.SetCheck(0);
		pLayerDialog->m_b3.SetCheck(0);
		pLayerDialog->m_b4.SetCheck(0);
		break;
	case 2:
		pLayerDialog->m_b1.SetCheck(0);
		pLayerDialog->m_b2.SetCheck(1);
		pLayerDialog->m_b3.SetCheck(0);
		pLayerDialog->m_b4.SetCheck(0);
		break;
	case 3:
		pLayerDialog->m_b1.SetCheck(0);
		pLayerDialog->m_b2.SetCheck(0);
		pLayerDialog->m_b3.SetCheck(1);
		pLayerDialog->m_b4.SetCheck(0);
		break;
	case 4:
		pLayerDialog->m_b1.SetCheck(0);
		pLayerDialog->m_b2.SetCheck(0);
		pLayerDialog->m_b3.SetCheck(0);
		pLayerDialog->m_b4.SetCheck(1);
		break;
	}
	pLayerDialog->m_widok=pDoc->m_cube.actwidok;
	pLayerDialog->m_typ=pDoc->m_cube.acttyp;
	pLayerDialog->UpdateData(FALSE);
}

void CIsoView::OnSkalaSelect(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pLayerDialog->UpdateData(TRUE);
	pDoc->m_cube.actskala=pLayerDialog->m_skala;
	pDoc->m_cube.obszar.right=bok*pDoc->m_cube.actskala;
	pDoc->m_cube.obszar.bottom=bok*pDoc->m_cube.actskala;
	Extract(0,0);
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = pDoc->m_cube.obszar.Width();
	SetScrollSizes(MM_TEXT, sizeTotal);
	Invalidate();
}


void CIsoView::OnWidokSelect(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pLayerDialog->UpdateData(TRUE);
	pDoc->m_cube.actwidok=pLayerDialog->m_widok;
	pDoc->m_cube.acttyp=pLayerDialog->m_typ;
	Extract(0,0);
}

void CIsoView::OnToolXy() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actwidok=0;
	OnLayerUpdateDialog(0,0);
	Extract(0,0);
	
}

void CIsoView::OnToolXz() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actwidok=1;
	OnLayerUpdateDialog(0,0);
	Extract(0,0);
}

void CIsoView::OnToolYz() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actwidok=2;
	OnLayerUpdateDialog(0,0);
	Extract(0,0);
}

void CIsoView::OnUpdateToolXy(CCmdUI* pCmdUI) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actwidok==0)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnUpdateToolXz(CCmdUI* pCmdUI) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actwidok==1)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnUpdateToolYz(CCmdUI* pCmdUI) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actwidok==2)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnTool100() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actskala=1;
	pDoc->m_cube.obszar.right=bok*pDoc->m_cube.actskala;
	pDoc->m_cube.obszar.bottom=bok*pDoc->m_cube.actskala;
	OnLayerUpdateDialog(0,0);
	Extract(0,0);
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = pDoc->m_cube.obszar.Width();
	SetScrollSizes(MM_TEXT, sizeTotal);
	Invalidate();
	
}

void CIsoView::OnTool200() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actskala=2;
	pDoc->m_cube.obszar.right=bok*pDoc->m_cube.actskala;
	pDoc->m_cube.obszar.bottom=bok*pDoc->m_cube.actskala;
	OnLayerUpdateDialog(0,0);
	Extract(0,0);
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = pDoc->m_cube.obszar.Width();
	SetScrollSizes(MM_TEXT, sizeTotal);
	Invalidate();
	
}

void CIsoView::OnTool300() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actskala=3;
	pDoc->m_cube.obszar.right=bok*pDoc->m_cube.actskala;
	pDoc->m_cube.obszar.bottom=bok*pDoc->m_cube.actskala;
	OnLayerUpdateDialog(0,0);
	Extract(0,0);
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = pDoc->m_cube.obszar.Width();
	SetScrollSizes(MM_TEXT, sizeTotal);
	Invalidate();
}

void CIsoView::OnTool400() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actskala=4;
	pDoc->m_cube.obszar.right=bok*pDoc->m_cube.actskala;
	pDoc->m_cube.obszar.bottom=bok*pDoc->m_cube.actskala;
	OnLayerUpdateDialog(0,0);
	Extract(0,0);
	CSize sizeTotal;
	sizeTotal.cx = sizeTotal.cy = pDoc->m_cube.obszar.Width();
	SetScrollSizes(MM_TEXT, sizeTotal);
	Invalidate();
	
}

void CIsoView::OnUpdateTool100(CCmdUI* pCmdUI) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actskala==1)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnUpdateTool200(CCmdUI* pCmdUI) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actskala==2)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnUpdateTool300(CCmdUI* pCmdUI) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actskala==3)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnUpdateTool400(CCmdUI* pCmdUI) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.actskala==4)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}


void CIsoView::OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	if(pNMUpDown->iDelta<0)
		OnLayerUpDialogUpdate(0,0);
	else
		OnLayerDownDialogUpdate(0,0);

	
	*pResult = 0;
}

void CIsoView::OnLista(UINT wParam,LONG IParam)
{
	MessageBeep(0xFFFFFFFF);
}

void CIsoView::Extract(UINT wParam,LONG IParam)
{
	
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.binfo.bmiColors[1].rgbGreen=255)
	{

		pDoc->m_cube.binfo.bmiColors[1].rgbBlue=1;
		pDoc->m_cube.binfo.bmiColors[1].rgbGreen=1;
		pDoc->m_cube.binfo.bmiColors[1].rgbRed=1;
		pDoc->m_cube.binfo.bmiColors[1].rgbReserved=0;
		pDoc->m_cube.binfo.bmiHeader=pDoc->m_cube.info;
	}
	switch(pDoc->m_cube.mode)
	{
	case CUBE:
		if(!pDoc->m_cube.czyZaladowane)
			return;
		Wlayer=pDoc->m_cube.ExtractLayer(pDoc->m_cube.actlayer,pDoc->m_cube.actwidok,pDoc->m_cube.actskala,pDoc->m_cube.acttyp);
		pDoc->m_cube.SetROI();
		if(!(czyOrg))
		{
			if(pKolorDialog->CzyHis)
			{
				pDoc->m_cube.Histogram(Wlayer,plut);
				pHistoDialog->CopyLUT(lut.value,dialogH);
			}
			if(pKolorDialog->CzyTh)
				pDoc->m_cube.Threshold(Wlayer,pDoc->m_cube.ROI,pDoc->m_cube.actth);
		}
		pDoc->m_cube.CToDib(Wlayer);
		InvalidateRect(&pDoc->m_cube.obszar,FALSE);
		break;
	case SLICE:
		CString widok;
		CString nr;
		int start=pDoc->m_cube.wepath.ReverseFind('\\');
		int stop=pDoc->m_cube.wepath.ReverseFind('.');
		CString nazwa=pDoc->m_cube.wepath.Mid(start+1,stop-1-start);
		nr.Format("%d",pDoc->m_cube.actlayer);
		switch(pDoc->m_cube.actwidok)
		{
		case 0:
			widok="_XY";
			break;
		case 1:
			widok="_ZX";
			break;
		case 2:
			widok="_ZY";
			break;
		}
		if(!LoadBitmap(nazwa+"_"+nr+widok+".slc"+".bmp"))
		{
			delete pDoc->m_cube.layer;
			pDoc->m_cube.layer=new char[bok*bok];
			for(int a=0;a<256*256;a++)
				pDoc->m_cube.layer[a]=125;
			::MessageBeep(0xffffffff);
			Invalidate(TRUE);
//			InvalidateRect(&pDoc->m_cube.obszar,FALSE);
			return;
		}
		pDoc->m_cube.obszar.right=pDoc->m_cube.info.biWidth;
		pDoc->m_cube.obszar.bottom=pDoc->m_cube.info.biHeight;
		CSize sizeTotal; 
		sizeTotal.cx = pDoc->m_cube.obszar.Width();
		sizeTotal.cy = pDoc->m_cube.obszar.Height();
		SetScrollSizes(MM_TEXT, sizeTotal);
//		Invalidate(TRUE);
//pDoc->m_cube.CToDib(Wlayer);
		InvalidateRect(&pDoc->m_cube.obszar,FALSE);
		break;
	}

}

void CIsoView::OnKolorDialog() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!(pKolorDialog->GetSafeHwnd()==0))
		return;
	pKolorDialog->Create();
	dialogK=TRUE;
	if(pKolorDialog->CzyTh)
		pKolorDialog->m_thres.SetCheck(1);
	else
		pKolorDialog->m_thres.SetCheck(0);
	pKolorDialog->m_slider.SetValue(pDoc->m_cube.actth);
	pKolorDialog->m_czyorg=czyOrg;
	pKolorDialog->m_czyroi=pDoc->m_cube.czyROI;
	pKolorDialog->UpdateData(FALSE);

		
	
}

void CIsoView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPoint tmp=GetScrollPosition();
	if(MouseStatus==0)
	{
		actpozx=point.x;
		actpozy=point.y;	
		oldpozx=point.x;
		oldpozy=point.y;
		if(pDoc->m_cube.czyROI)
		{
			if(((unsigned int)point.x+tmp.x==pDoc->m_cube.ROI.left*pDoc->m_cube.actskala)||((unsigned int)point.x+tmp.x==pDoc->m_cube.ROI.right*pDoc->m_cube.actskala)||((unsigned int)point.x+1+tmp.x==pDoc->m_cube.ROI.left*pDoc->m_cube.actskala)||((unsigned int)point.x-1+tmp.x==pDoc->m_cube.ROI.right*pDoc->m_cube.actskala))
			{
				if(((unsigned int)point.y+tmp.y<(unsigned int)pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala)&&((unsigned int)point.y+tmp.y>(unsigned int)pDoc->m_cube.ROI.top*pDoc->m_cube.actskala))
					::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
				return;
			}
			if(((unsigned int)point.y+tmp.y==pDoc->m_cube.ROI.top*pDoc->m_cube.actskala)||((unsigned int)point.y+tmp.y==pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala)||((unsigned int)point.y+1+tmp.y==pDoc->m_cube.ROI.top*pDoc->m_cube.actskala)||((unsigned int)point.y-1+tmp.y==pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala))
			{
				if(((unsigned int)point.x+tmp.x<(unsigned int)pDoc->m_cube.ROI.right*pDoc->m_cube.actskala)&&((unsigned int)point.x+tmp.x>(unsigned int)pDoc->m_cube.ROI.left*pDoc->m_cube.actskala))
					::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
				return;
			}
			CRect tmp1;
			tmp1.top=pDoc->m_cube.ROI.top*pDoc->m_cube.actskala-tmp.y;
			tmp1.left=pDoc->m_cube.ROI.left*pDoc->m_cube.actskala-tmp.x;
			tmp1.right=pDoc->m_cube.ROI.right*pDoc->m_cube.actskala-tmp.x;
			tmp1.bottom=pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala-tmp.y;
			if(tmp1.PtInRect(point))
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEALL));
			else
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
		}
	}
	if(MouseStatus==1)
	{
		CClientDC MyDC(this);
		CPen MyPen(PS_SOLID,1,RGB(128,128,128));
		CBrush MyBrush;
		MyBrush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pOldBrush=MyDC.SelectObject(&MyBrush);
		CPen* pOldPen=MyDC.SelectObject(&MyPen);
		MyDC.SetROP2(R2_XORPEN);
	
		if(point.x<0)
			point.x=0;
		if(point.y<0)
			point.y=0;
	
		MyDC.Rectangle(actpozx,actpozy,point.x,point.y);
		MyDC.Rectangle(actpozx,actpozy,oldpozx,oldpozy);
		oldpozx=point.x;
		oldpozy=point.y;
		MyDC.SetROP2(R2_COPYPEN);
		MyDC.SelectObject(pOldPen);
		MyDC.SelectObject(pOldBrush);
	}
	if(MouseStatus==2)
	{
		
		CClientDC MyDC(this);
		CPen MyPen(PS_SOLID,1,RGB(128,128,128));
		CBrush MyBrush;
		MyBrush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pOldBrush=MyDC.SelectObject(&MyBrush);
		CPen* pOldPen=MyDC.SelectObject(&MyPen);
		MyDC.SetROP2(R2_XORPEN);
		if(point.x<0)
			point.x=0;
		if(point.y<0)
			point.y=0;
		pDoc->m_cube.ROI.left=(point.x+tmp.x)/pDoc->m_cube.actskala;
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		MyDC.Rectangle(oldpozx*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		oldpozx=pDoc->m_cube.ROI.left;
		MyDC.SetROP2(R2_COPYPEN);
		MyDC.SelectObject(pOldPen);
		MyDC.SelectObject(pOldBrush);
	}
	if(MouseStatus==3)
	{
		
		CClientDC MyDC(this);
		CPen MyPen(PS_SOLID,1,RGB(128,128,128));
		CBrush MyBrush;
		MyBrush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pOldBrush=MyDC.SelectObject(&MyBrush);
		CPen* pOldPen=MyDC.SelectObject(&MyPen);
		MyDC.SetROP2(R2_XORPEN);
		if(point.x<0)
			point.x=0;
		if(point.y<0)
			point.y=0;
		pDoc->m_cube.ROI.right=(point.x+tmp.x)/pDoc->m_cube.actskala;
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,oldpozx*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		oldpozx=pDoc->m_cube.ROI.right;
		MyDC.SetROP2(R2_COPYPEN);
		MyDC.SelectObject(pOldPen);
		MyDC.SelectObject(pOldBrush);
	}
	if(MouseStatus==4)
	{
		
		CClientDC MyDC(this);
		CPen MyPen(PS_SOLID,1,RGB(128,128,128));
		CBrush MyBrush;
		MyBrush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pOldBrush=MyDC.SelectObject(&MyBrush);
		CPen* pOldPen=MyDC.SelectObject(&MyPen);
		MyDC.SetROP2(R2_XORPEN);
		if(point.x<0)
			point.x=0;
		if(point.y<0)
			point.y=0;
		pDoc->m_cube.ROI.bottom=(point.y+tmp.y)/pDoc->m_cube.actskala;
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,oldpozy*pDoc->m_cube.actskala);
		oldpozy=pDoc->m_cube.ROI.bottom;
		MyDC.SetROP2(R2_COPYPEN);
		MyDC.SelectObject(pOldPen);
		MyDC.SelectObject(pOldBrush);
	}
	if(MouseStatus==5)
	{
		
		CClientDC MyDC(this);
		CPen MyPen(PS_SOLID,1,RGB(128,128,128));
		CBrush MyBrush;
		MyBrush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pOldBrush=MyDC.SelectObject(&MyBrush);
		CPen* pOldPen=MyDC.SelectObject(&MyPen);
		MyDC.SetROP2(R2_XORPEN);
		if(point.x<0)
			point.x=0;
		if(point.y<0)
			point.y=0;
		pDoc->m_cube.ROI.top=(point.y+tmp.y)/pDoc->m_cube.actskala;
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,oldpozy*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		oldpozy=pDoc->m_cube.ROI.top;
		MyDC.SetROP2(R2_COPYPEN);
		MyDC.SelectObject(pOldPen);
		MyDC.SelectObject(pOldBrush);
	}
	if(MouseStatus==6)
	{
		CClientDC MyDC(this);
		CPen MyPen(PS_SOLID,1,RGB(128,128,128));
		CBrush MyBrush;
		MyBrush.CreateStockObject(HOLLOW_BRUSH);
		CBrush* pOldBrush=MyDC.SelectObject(&MyBrush);
		CPen* pOldPen=MyDC.SelectObject(&MyPen);
		MyDC.SetROP2(R2_XORPEN);
		if(point.x<0)
			point.x=0;
		if(point.y<0)
			point.y=0;
		pDoc->m_cube.ROI.top+=(point.y/pDoc->m_cube.actskala-actpozy/pDoc->m_cube.actskala);
		pDoc->m_cube.ROI.bottom+=(point.y/pDoc->m_cube.actskala-actpozy/pDoc->m_cube.actskala);
		pDoc->m_cube.ROI.left+=(point.x/pDoc->m_cube.actskala-actpozx/pDoc->m_cube.actskala);
		pDoc->m_cube.ROI.right+=(point.x/pDoc->m_cube.actskala-actpozx/pDoc->m_cube.actskala);
		MyDC.Rectangle(pDoc->m_cube.ROI.left*pDoc->m_cube.actskala,pDoc->m_cube.ROI.top*pDoc->m_cube.actskala,pDoc->m_cube.ROI.right*pDoc->m_cube.actskala,pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala);
		MyDC.Rectangle(old.left*pDoc->m_cube.actskala,old.top*pDoc->m_cube.actskala,old.right*pDoc->m_cube.actskala,old.bottom*pDoc->m_cube.actskala);
		old.left=pDoc->m_cube.ROI.left;
		old.right=pDoc->m_cube.ROI.right;
		old.top=pDoc->m_cube.ROI.top;
		old.bottom=pDoc->m_cube.ROI.bottom;
		actpozx=point.x;
		actpozy=point.y;
		MyDC.SetROP2(R2_COPYPEN);
		MyDC.SelectObject(pOldPen);
		MyDC.SelectObject(pOldBrush);

	}
	CScrollView::OnMouseMove(nFlags, point);
}

void CIsoView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	MouseStatus=1;
	SetCapture();
	CScrollView::OnRButtonDown(nFlags, point);
}


void CIsoView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CClientDC MyDC(this);
	CPen MyPen(PS_SOLID,1,RGB(128,128,128));
	CBrush MyBrush;
	MyBrush.CreateStockObject(HOLLOW_BRUSH);
	CBrush* pOldBrush=MyDC.SelectObject(&MyBrush);
	CPen* pOldPen=MyDC.SelectObject(&MyPen);
	MyDC.SetROP2(R2_XORPEN);
	MyDC.Rectangle(actpozx,actpozy,oldpozx,oldpozy);
	MyDC.SetROP2(R2_COPYPEN);
	MyDC.SelectObject(pOldPen);
	MyDC.SelectObject(pOldBrush);
	::ReleaseCapture();
	pDoc->m_cube.ROI.SetRect(actpozx,actpozy,oldpozx,oldpozy);
	pDoc->m_cube.ROI.top/=pDoc->m_cube.actskala;
	pDoc->m_cube.ROI.left/=pDoc->m_cube.actskala;
	pDoc->m_cube.ROI.right/=pDoc->m_cube.actskala;
	pDoc->m_cube.ROI.bottom/=pDoc->m_cube.actskala;
	if(pDoc->m_cube.ROI.top==pDoc->m_cube.ROI.bottom)
		pDoc->m_cube.ROI.bottom++;
	if(pDoc->m_cube.ROI.left==pDoc->m_cube.ROI.right)
		pDoc->m_cube.ROI.right++;
	pDoc->m_cube.ROI.NormalizeRect();
	pDoc->m_cube.czyROI=TRUE;
	pKolorDialog->m_czyroi=pDoc->m_cube.czyROI;
	if(dialogK)
		pKolorDialog->UpdateData(FALSE);
	Extract(0,0);
	MouseStatus=0;

	CScrollView::OnRButtonUp(nFlags, point);
}


void CIsoView::OnChangeSlider(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pDoc->m_cube.actth=pKolorDialog->m_slider.GetValue();
	Extract(0,0);

}

void CIsoView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CPoint tmp=GetScrollPosition();
	old.left=pDoc->m_cube.ROI.left;
	old.right=pDoc->m_cube.ROI.right;
	old.top=pDoc->m_cube.ROI.top;
	old.bottom=pDoc->m_cube.ROI.bottom;
	if(((unsigned int)point.x+tmp.x==pDoc->m_cube.ROI.left*pDoc->m_cube.actskala)||((unsigned int)point.x+1+tmp.x==pDoc->m_cube.ROI.left*pDoc->m_cube.actskala))
	{
		MouseStatus=2;
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	if(((unsigned int)point.x+tmp.x==pDoc->m_cube.ROI.right*pDoc->m_cube.actskala)||((unsigned int)point.x+1+tmp.x==pDoc->m_cube.ROI.right*pDoc->m_cube.actskala))
	{
		MouseStatus=3;
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	if(((unsigned int)point.y+tmp.y==pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala)||((unsigned int)point.y-1+tmp.y==pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala))
	{
		MouseStatus=4;
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	if(((unsigned int)point.y+tmp.y==pDoc->m_cube.ROI.top*pDoc->m_cube.actskala)||((unsigned int)point.y+1+tmp.y==pDoc->m_cube.ROI.top*pDoc->m_cube.actskala))
	{
		MouseStatus=5;
		CScrollView::OnLButtonDown(nFlags, point);
		return;
	}
	CRect tmp1;
	tmp1.top=pDoc->m_cube.ROI.top*pDoc->m_cube.actskala-tmp.y;
	tmp1.left=pDoc->m_cube.ROI.left*pDoc->m_cube.actskala-tmp.x;
	tmp1.right=pDoc->m_cube.ROI.right*pDoc->m_cube.actskala-tmp.x;
	tmp1.bottom=pDoc->m_cube.ROI.bottom*pDoc->m_cube.actskala-tmp.y;
	if(tmp1.PtInRect(point))
	{
//		actpozx=point.x;
//		actpozy=point.y;
		MouseStatus=6;
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

void CIsoView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	MouseStatus=0;
	pDoc->m_cube.ROI.NormalizeRect();
	if(pDoc->m_cube.ROI.top<0)
		pDoc->m_cube.ROI.top=0;
	if(pDoc->m_cube.ROI.left<0)
		pDoc->m_cube.ROI.left=0;
	Extract(0,0);
	
	CScrollView::OnLButtonUp(nFlags, point);
}

void CIsoView::OnToolbarKolor() 
{
	if(!(pKolorDialog->GetSafeHwnd()==0))
	{
		pKolorDialog->OnCloseKolor();
		return;
	}
	OnKolorDialog();
	
}

void CIsoView::OnToolbarLayer() 
{
	if(!(pLayerDialog->GetSafeHwnd()==0))
	{
		pLayerDialog->OnCloseLayer();
		return;
	}
	OnLayerInfo();
}

void CIsoView::OnToolbarGl() 
{
	if(!(pOpenGLDialog->GetSafeHwnd()==0))
	{
		pOpenGLDialog->OnCloseGl();
		return;
	}
	OnGlDialog();
	
}

void CIsoView::OnUpdateToolbarKolor(CCmdUI* pCmdUI) 
{
	if(dialogK)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnUpdateToolbarLayer(CCmdUI* pCmdUI) 
{
	if(dialogL)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);

}

void CIsoView::OnUpdateToolbarGl(CCmdUI* pCmdUI) 
{
	if(dialogO)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnDestroyKolor(UINT wParam,LONG IParam)
{
	dialogK=FALSE;

}

void CIsoView::OnDestroyLayer(UINT wParam,LONG IParam)
{
	dialogL=FALSE;

}

void CIsoView::OnDestroyGl(UINT wParam,LONG IParam)
{
	dialogO=FALSE;
//	::MessageBeep(0xffffffff);

}

void CIsoView::OnDestroyHisto(UINT wParam, LONG IParam)
{
	dialogH=FALSE;
//	::MessageBeep(0xffffffff);

}

void CIsoView::OnHistoDialog() 
{
	if(dialogH)
		return;
//	pHistoDialog->Create("Histogram", WS_POPUP | WS_CAPTION | WS_VISIBLE | WS_SYSMENU, this, ID_H);
	pHistoDialog->Create("Histogram", WS_CAPTION | WS_VISIBLE | WS_SYSMENU, this, ID_H);
	dialogH = TRUE;
	pKolorDialog->CzyHis=TRUE;
	Extract(0,0);
}

void CIsoView::OnGlDialog() 
{
	if(dialogO)
		return;
	pOpenGLDialog->GetPoints(&pointGL);
	pOpenGLDialog->Create("OPENGL",WS_CAPTION|WS_VISIBLE|WS_SYSMENU|CS_OWNDC|WS_THICKFRAME|WS_MINIMIZEBOX,this,ID_O);
//	pOpenGLDialog->Create("OPENGL",WS_CAPTION|WS_POPUP|WS_VISIBLE|WS_SYSMENU|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|CS_OWNDC|WS_THICKFRAME|WS_MINIMIZEBOX,this,ID_O);
	dialogO=TRUE;
}

void CIsoView::OnToolbarHisto() 
{
	if(dialogH)
	{
		pHistoDialog->OnCloseHisto();
		return;
	}
	OnHistoDialog();
}

void CIsoView::OnUpdateToolbarHisto(CCmdUI* pCmdUI) 
{
	if(dialogH)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
	
}

void CIsoView::OnKolorUpdate(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	pKolorDialog->UpdateData(TRUE);
	pDoc->m_cube.czyROI=pKolorDialog->m_czyroi;
	czyOrg=pKolorDialog->m_czyorg;
	Extract(0,0);

}

void CIsoView::OnSliceSave() 
{
	char* tmp;
	unsigned int l,a,c;
	l=0;
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->m_cube.czyZaladowane)
		return;
	BITMAPINFOHEADER i;
	BITMAPFILEHEADER s;
	RGBQUAD paleta[256];
	CString widok;
	CFile ss;
	int start=pDoc->m_cube.wepath.ReverseFind('\\');
	int stop=pDoc->m_cube.wepath.ReverseFind('.');
	CString nazwa=pDoc->m_cube.wepath.Mid(start+1,stop-1-start);
	CString nr;
	nr.Format("%d",pDoc->m_cube.actlayer);
	switch(pDoc->m_cube.actwidok)
	{
	case 0:
		widok="_XY";
		break;
	case 1:
		widok="_ZX";
		break;
	case 2:
		widok="_ZY";
		break;
	}
	i.biSize=sizeof(i);
	i.biPlanes=1;
	i.biBitCount=8;
	i.biCompression=0;
	i.biSizeImage=0;
	i.biXPelsPerMeter=0;
	i.biYPelsPerMeter=0;
	i.biClrUsed=0;
	i.biClrImportant=0;
	for(unsigned int b=0;b<256;b++)
	{
		paleta[b].rgbBlue=b;
		paleta[b].rgbRed=b;
		paleta[b].rgbGreen=b;
		paleta[b].rgbReserved=0;
	}
	s.bfType='MB';
	s.bfSize=sizeof(BITMAPINFOHEADER)+sizeof(BITMAPFILEHEADER)+sizeof(paleta)+bok*bok;
	s.bfReserved1=0;
	s.bfReserved2=0;
	s.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(paleta)+sizeof(BITMAPINFOHEADER);
	if(pKolorDialog->CzyTh)
		ss.Open(nazwa+"_"+nr+widok+".slc"+".bmp",CFile::modeCreate|CFile::modeWrite);
	else
		ss.Open(nazwa+"_"+nr+widok+".bmp",CFile::modeCreate|CFile::modeWrite);
	if(pDoc->m_cube.czyROI)
	{
		unsigned int szer,wys;
		start=pDoc->m_cube.r->xOffset;
		szer=pDoc->m_cube.r->width;
		wys=pDoc->m_cube.r->height;
		while((szer & 3)!=0)
			szer++;
		tmp=new char[szer*wys];
		if(pDoc->m_cube.acttyp==0)
			c=256*(pDoc->m_cube.r->yOffset+wys);
		else
			c=256*pDoc->m_cube.actskala*(pDoc->m_cube.r->yOffset+wys);
		for(b=0;b<wys;b++)
		{
			for(a=start+c;a<start+szer+c;a++)
			{
				tmp[l]=Wlayer->imageData[a];
				l++;
				
			}
			if(pDoc->m_cube.acttyp==0)
				c-=256;
			else
				c-=256*pDoc->m_cube.actskala;
		}
		i.biWidth=szer;
		i.biHeight=wys;
		ss.Write(&s,sizeof(BITMAPFILEHEADER));
		ss.Write(&i,sizeof(BITMAPINFOHEADER));
		ss.Write(paleta,sizeof(paleta));
		ss.Write(tmp,szer*wys);
		delete tmp;
	}
	else
	{
		if(pDoc->m_cube.acttyp==0)
		{
			i.biWidth=256;
			i.biHeight=256;
			c=256*i.biHeight-256;/////////////////////
		}	
		else
		{
			i.biWidth=256*pDoc->m_cube.actskala;
			i.biHeight=256*pDoc->m_cube.actskala;
			c=256*pDoc->m_cube.actskala*i.biHeight-256*pDoc->m_cube.actskala;/////////////////
		}
		tmp=new char[i.biWidth*i.biHeight];
		for(b=0;b<i.biHeight;b++)
		{
			for(a=0+c;a<i.biWidth+c;a++)
			{
				tmp[l]=Wlayer->imageData[a];
				l++;
			}
			if(pDoc->m_cube.acttyp==0)
				c-=256;
			else
				c-=256*pDoc->m_cube.actskala;
		}
		ss.Write(&s,sizeof(BITMAPFILEHEADER));
		ss.Write(&i,sizeof(BITMAPINFOHEADER));
		ss.Write(paleta,sizeof(paleta));
		ss.Write(tmp,i.biWidth*i.biHeight);
		delete tmp;
	}
	ss.Close();

}

BOOL CIsoView::LoadBitmap(CString name)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	BITMAPINFOHEADER i;
	BITMAPFILEHEADER s;
	RGBQUAD paleta[256];
	if(!wej.Open(name,CFile::modeRead))
		return FALSE;
	delete pDoc->m_cube.layer;
	wej.Read(&s,sizeof(BITMAPFILEHEADER));
	wej.Read(&i,sizeof(BITMAPINFOHEADER));
	wej.Read(paleta,sizeof(paleta));
	pDoc->m_cube.layer=new char[i.biWidth*i.biHeight];
	wej.Read(pDoc->m_cube.layer,i.biWidth*i.biHeight);
	pDoc->m_cube.info.biWidth=i.biWidth;
	pDoc->m_cube.info.biHeight=i.biHeight;
	pDoc->m_cube.binfo.bmiHeader=pDoc->m_cube.info;
	wej.Close();
	return TRUE;

}
void CIsoView::OnMode() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_cube.mode==CUBE)
		pDoc->m_cube.mode=SLICE;
	else
	{
		pDoc->m_cube.mode=CUBE;
		pDoc->m_cube.obszar.right=bok*pDoc->m_cube.actskala;
		pDoc->m_cube.obszar.bottom=bok*pDoc->m_cube.actskala;
		CSize sizeTotal;
		sizeTotal.cx = sizeTotal.cy = pDoc->m_cube.obszar.Width();
		SetScrollSizes(MM_TEXT, sizeTotal);
	}	
	Invalidate(TRUE);
	Extract(0,0);

}

void CIsoView::OnObrys()
{
	if(!(pDDialog->GetSafeHwnd()==0))
	{
		pDDialog->OnCloseDDialog();
		return;
	}
	pDDialog->Create();
	dialog3D=TRUE;
}

void CIsoView::OnDestroyDDialog(UINT wParam,LONG IParam)
{
	dialog3D=FALSE;

}

void CIsoView::OnMakeCurrent1(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc->m_cube.czyZaladowane)
		return;
	pKolorDialog->CzyTh=TRUE;
	if(!(pKolorDialog->GetSafeHwnd()==0))
		pKolorDialog->m_thres.SetCheck(1);
	Extract(0,0);
	BeginWaitCursor();
	Trace();
	EndWaitCursor();

}
void CIsoView::OnMakeAll1(UINT wParam,LONG IParam)
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	int start;
	if(!pDoc->m_cube.czyZaladowane)
		return;
	for(int b=0;b<pointGL.GetSize();b++)
		delete (C_Punkty* )pointGL.GetAt(b);
	pointGL.RemoveAll();
	pKolorDialog->CzyTh=TRUE;
	if(!(pKolorDialog->GetSafeHwnd()==0))
		pKolorDialog->m_thres.SetCheck(1);
	start=pDoc->m_cube.actlayer;
	BeginWaitCursor();
	for(int l=start;l<start+200;l++)
	{
		pDoc->m_cube.actlayer=l;
		Extract(0,0);
		Trace();
	}
	EndWaitCursor();
	
}

void CIsoView::Trace() 
{
	CIsoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if((pDoc->m_cube.mode==CUBE)&&(!(pKolorDialog->CzyTh)))
		return;
	if(pDoc->m_cube.acttyp!=_FAST)
	{
		pDoc->m_cube.acttyp=_FAST;
		OnLayerUpdateDialog(0,0);
		Extract(0,0);
	}
	pDoc->m_cube.binfo.bmiColors[1].rgbBlue=0;
	pDoc->m_cube.binfo.bmiColors[1].rgbGreen=250;
	pDoc->m_cube.binfo.bmiColors[1].rgbRed=0;
	pDoc->m_cube.binfo.bmiColors[1].rgbReserved=0;
	pDoc->m_cube.binfo.bmiHeader=pDoc->m_cube.info;
	int minx,maxx,miny,maxy,xs,ys;
	int x,y,r,x1,y1,xk,yk;
	double alfa;
	BOOL czybreak;
	char pixel[1];
	char pixelout[1];
	pixel[0]=1;
	czybreak=FALSE;
	for(x=0;x<pDoc->m_cube.info.biWidth;x++)
		if(czybreak)
			break;
		else
		{
			for(y=0;y<pDoc->m_cube.info.biHeight;y++)
			{
				iplGetPixel(Wlayer,x,y,pixelout);
				if((unsigned char)pixelout[0]>50)
				{
//					iplPutPixel(Wlayer,x,y,pixel);
					czybreak=TRUE;
					minx=x;
					break; 
				}
			}
		}

	czybreak=FALSE;
	for(x=pDoc->m_cube.info.biWidth-1;x>=0;x--)
		if(czybreak)
			break;
		else
		{
			for(y=0;y<pDoc->m_cube.info.biHeight;y++)
			{
				iplGetPixel(Wlayer,x,y,pixelout);
				if((unsigned char)pixelout[0]>50)
				{
//					iplPutPixel(Wlayer,x,y,pixel);
					czybreak=TRUE;
					maxx=x;
					break; 
				}
			}
		}

	czybreak=FALSE;
	for(y=0;y<pDoc->m_cube.info.biWidth;y++)
		if(czybreak)
			break;
		else
		{
			for(x=0;x<pDoc->m_cube.info.biHeight;x++)
			{
				iplGetPixel(Wlayer,x,y,pixelout);
				if((unsigned char)pixelout[0]>50)
				{
//					iplPutPixel(Wlayer,x,y,pixel);
					czybreak=TRUE;
					miny=y;
					break; 
				}
			}
		}

	czybreak=FALSE;
	for(y=pDoc->m_cube.info.biWidth-1;y>=0;y--)
		if(czybreak)
			break;
		else
		{
			for(x=0;x<pDoc->m_cube.info.biHeight;x++)
			{
				iplGetPixel(Wlayer,x,y,pixelout);
				if((unsigned char)pixelout[0]>50)
				{
//					iplPutPixel(Wlayer,x,y,pixel);
					czybreak=TRUE;
					maxy=y;
					break; 
				}
			}
		}
	
	xs=minx+(maxx-minx)/2;
	ys=miny+(maxy-miny)/2;
	r=MAX(sqrt((xs-minx)*(xs-minx)+(ys-miny)*(ys-miny)),sqrt((maxx-xs)*(maxx-xs)+(maxy-ys)*(maxy-ys)))+3;
	iplPutPixel(Wlayer,xs,ys,pixel);
	for(alfa=0.0;alfa<2*PI;alfa+=0.1)
	{
		x1=(double)r*cos(alfa); xk=x1+xs;
		y1=(double)r*sin(alfa); yk=y1+ys;
		if((xk)>255) xk=255;
		if((xk)<0) xk=0;
		if((yk)>255) yk=255;
		if((yk)<0) yk=0;
//		iplPutPixel(Wlayer,xk,yk,pixel);
		for(double t=1;t>=0;t-=0.001)
		{
			iplGetPixel(Wlayer,(int)(xs+(xk-xs)*t),(int)(ys+(yk-ys)*t),pixelout);
//			iplPutPixel(Wlayer,(int)(xs+(xk-xs)*t),(int)(ys+(yk-ys)*t),pixel);
			if((unsigned char)pixelout[0]>254)
			{
//				iplPutPixel(Wlayer,(int)(xs+(xk-xs)*t),(int)(ys+(yk-ys)*t),pixel);
				pointGL.Add(new C_Punkty((int)(xs+(xk-xs)*t-128),(int)(ys+(yk-ys)*t-128),pDoc->m_cube.actlayer));
				break;
			}
		}

	}

	pDoc->m_cube.CToDib(Wlayer);
	Invalidate();
/*	if(pDoc->m_cube.mode==SLICE)
	{
		iplDeallocateImage(Wlayer);
		Wlayer->height=ox;
		Wlayer->width=oy;
		iplAllocateImage(Wlayer,0,0);
//		Extract(0,0);
	}*/

}






