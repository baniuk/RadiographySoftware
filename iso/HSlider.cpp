// HSlider.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "HSlider.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HSlider

HSlider::HSlider()
{
}

HSlider::~HSlider()
{
}


BEGIN_MESSAGE_MAP(HSlider, CSliderCtrl)
	//{{AFX_MSG_MAP(HSlider)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HSlider message handlers

