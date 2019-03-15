// isoView.h : interface of the CIsoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISOVIEW_H__828DA2AC_A121_11D4_BB95_CC0761F9C633__INCLUDED_)
#define AFX_ISOVIEW_H__828DA2AC_A121_11D4_BB95_CC0761F9C633__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "InfoDilalog.h"
#include "LayerDialog.h"
#include "KolorDialog.h"
#include "HistoDialog.h"
#include "OpenGLDialog.h"
#include "D_DIALOG.h"
//#include "_Toolbar1.h"

class CIsoView : public CScrollView
{
protected: // create from serialization only
	CIsoView();
	DECLARE_DYNCREATE(CIsoView)

// Attributes
public:
	CIsoDoc* GetDocument();
//	HBITMAP bb;
//	CDC MemDC;
	unsigned char* pData;
	int MouseStatus;
	int actpozx,actpozy,oldpozx,oldpozy;
	CRect old;
	IplImage* Wlayer;
	InfoDilalog* pInfoDialog;
	LayerDialog* pLayerDialog;
	KolorDialog* pKolorDialog;
	HistoDialog* pHistoDialog;
	OpenGLDialog* pOpenGLDialog;
	D_DIALOG* pDDialog;
	IplLUT lut;
	IplLUT* plut;
	BOOL czyOrg;
	CPtrArray pointGL;
	CFile wej;
	void Extract(UINT wParam,LONG IParam);
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL
	afx_msg void OnUpdateStatusB(CCmdUI *pCmdUI);
	// Implementation
public:
	virtual ~CIsoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	BOOL dialogL;
	BOOL dialogK;
	BOOL dialogH;
	BOOL dialogO;
	BOOL dialog3D;
	BOOL LoadBitmap(CString name);
protected:
	//{{AFX_MSG(CIsoView)
	afx_msg void OnWindowInfo();
	afx_msg void OnLayerInfo();
	afx_msg void OnToolXy();
	afx_msg void OnToolXz();
	afx_msg void OnToolYz();
	afx_msg void OnUpdateToolXy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolXz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolYz(CCmdUI* pCmdUI);
	afx_msg void OnTool100();
	afx_msg void OnTool200();
	afx_msg void OnTool300();
	afx_msg void OnTool400();
	afx_msg void OnUpdateTool100(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTool200(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTool300(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTool400(CCmdUI* pCmdUI);
	afx_msg void OnKolorDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnToolbarKolor();
	afx_msg void OnToolbarLayer();
	afx_msg void OnUpdateToolbarKolor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolbarLayer(CCmdUI* pCmdUI);
	afx_msg void OnHistoDialog();
	afx_msg void OnToolbarHisto();
	afx_msg void OnUpdateToolbarHisto(CCmdUI* pCmdUI);
	afx_msg void OnSliceSave();
	afx_msg void OnMode();
	afx_msg void OnObrys();
	afx_msg void OnGlDialog();
	afx_msg void OnToolbarGl();
	afx_msg void OnUpdateToolbarGl(CCmdUI* pCmdUI);
	//}}AFX_MSG
	void OnInfoDialogUpdate(UINT wParam,LONG IParam);
	void OnLayerUpDialogUpdate(UINT wParam,LONG IParam);
	void OnLayerDownDialogUpdate(UINT wParam,LONG IParam);
	void OnLayerSelect(UINT wParam,LONG IParam);
	void OnKolorUpdate(UINT wParam,LONG IParam);
	void OnSkalaSelect(UINT wParam,LONG IParam);
	void OnWidokSelect(UINT wParam,LONG IParam);
	void OnLayerUpdateDialog(UINT wParam,LONG IParam);
	void OnLista(UINT wParam,LONG IParam);
	void OnDeltaPos(UINT wParam,LONG IParam);
	void OnChangeSlider(UINT wParam,LONG IParam);
	void OnDestroyKolor(UINT wParam,LONG IParam);
	void OnDestroyLayer(UINT wParam,LONG IParam);
	void OnDestroyDDialog(UINT wParam,LONG IParam);
	void OnDestroyHisto(UINT wParam, LONG IParam);
	void OnDestroyGl(UINT wParam, LONG IParam);
	void OnMakeAll1(UINT wParam, LONG IParam);
	void OnMakeCurrent1(UINT wParam, LONG IParam);
	void Trace();
	afx_msg void OnDeltaposSpin(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in isoView.cpp
inline CIsoDoc* CIsoView::GetDocument()
   { return (CIsoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOVIEW_H__828DA2AC_A121_11D4_BB95_CC0761F9C633__INCLUDED_)
