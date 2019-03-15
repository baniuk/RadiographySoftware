// isoDoc.cpp : implementation of the CIsoDoc class
//

#include "stdafx.h"
#include "iso.h"

#include "isoDoc.h"
//#include "isoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsoDoc

IMPLEMENT_DYNCREATE(CIsoDoc, CDocument)

BEGIN_MESSAGE_MAP(CIsoDoc, CDocument)
	//{{AFX_MSG_MAP(CIsoDoc)
	ON_COMMAND(ID_OPEN_DATA, OnOpenData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsoDoc construction/destruction

CIsoDoc::CIsoDoc()
{
	// TODO: add one-time construction code here

}

CIsoDoc::~CIsoDoc()
{
}

BOOL CIsoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	delete m_cube.layer;
	m_cube.layer=new char[bok*bok];
	for(unsigned int a=0;a<bok*bok;a++)
		m_cube.layer[a]=0;
	m_cube.wepath.Empty();
	POSITION posView=GetFirstViewPosition();
	CView* pView=GetNextView(posView);
	m_cube.actlayer=0;
	m_cube.actskala=1;
	m_cube.actwidok=_XY;
	m_cube.acttyp=_FAST;
	m_cube.skalaX=1.9571;
	m_cube.skalaY=1.9608;
	m_cube.skalaZ=1.9608;
	m_cube.obszar.left=0;
	m_cube.obszar.top=0;
	m_cube.obszar.right=bok;
	m_cube.obszar.bottom=bok;
	m_cube.info.biWidth=bok;
	m_cube.info.biHeight=bok;
	pView->PostMessage(WM_DLG_UPDATE);
	pView->PostMessage(ID_UPDATE_NEW_LAYER_DIALOG);
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CIsoDoc serialization

void CIsoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CIsoDoc diagnostics

#ifdef _DEBUG
void CIsoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIsoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIsoDoc commands

void CIsoDoc::OnOpenData() 
{
	Pliki input(TRUE,"iso",NULL,NULL,"Pliki danych (*.iso)|*.iso|Wszystkie pliki (*.*)|*.*");
	input.m_ofn.lpstrTitle="Otwórz dane"; 
	int returnvalue=input.DoModal();
	if(returnvalue==IDCANCEL) return;
	m_cube.wepath=input.GetPathName();
//	int start=m_cube.wepath.ReverseFind('\\');
//	m_cube.wepathpath=m_cube.wepath.Left(start+1);

	input.m_ofn.lpstrInitialDir=m_cube.wepath;
	CFile we;
	if(!(we.Open(m_cube.wepath,CFile::modeRead)))
	{
		MessageBox(NULL,"B³¹d otwarcia pliku","UWAGA",MB_ICONEXCLAMATION|MB_OK);
		return;
	}
	we.Read(m_cube.cube,bok*bok*bok);
	we.Close();
	m_cube.czyZaladowane=TRUE;
	POSITION posView=GetFirstViewPosition();
	CView* pView=GetNextView(posView);
	pView->PostMessage(WM_DLG_UPDATE);
	pView->PostMessage(WM_EX);
	pView->Invalidate(TRUE);

}
