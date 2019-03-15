// _OpenGL_Hist.cpp : implementation file
//

#include "stdafx.h"
#include "RadioGUI.h"
#include "_OpenGL_Hist.h"


// C_OpenGL_Hist

IMPLEMENT_DYNAMIC(C_OpenGL_Hist, CWnd)

C_OpenGL_Hist::C_OpenGL_Hist()
{

}
C_OpenGL_Hist::C_OpenGL_Hist(double* data,unsigned int p_points, double pmax, double pwzm, unsigned int xsize, unsigned int ysize)
{
	tmpcols = xsize;
	tmprows = ysize;
	histogram = data;
	points = p_points;
	dol = 10;
	gora = 10;
	lewo = 10;
	prawo = 10;
	max = pmax;
	delta = floor(points/(tmpcols-lewo-prawo));
	if(delta==0)
		delta = 1;
	wzmocnienie = pwzm;

}
C_OpenGL_Hist::~C_OpenGL_Hist()
{
}
BOOL C_OpenGL_Hist::Create(LPCSTR WindowTitle, DWORD dwStyle,CWnd* pParent, UINT nID)
{
	CBrush blackBrush(RGB(0,0,0));
	CString ChildWindowClass = AfxRegisterWndClass(
				CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
				AfxGetApp()->LoadStandardCursor(IDC_CROSS),
				(HBRUSH)blackBrush.GetSafeHandle(),
				AfxGetApp()->LoadIcon(IDI_ICON2));
	pMainWindow=pParent;
	CRect ChildRect(0, 0, tmpcols, tmprows);
	CWnd::CreateEx(WS_EX_CONTROLPARENT,ChildWindowClass, WindowTitle, dwStyle,
						ChildRect, pParent, nID);
	return TRUE;
}
BEGIN_MESSAGE_MAP(C_OpenGL_Hist, CWnd)
	//{{AFX_MSG_MAP(OpenGLDialog1)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void C_OpenGL_Hist::OnDestroy() 
{
	CWnd::OnDestroy();
//	KillTimer(123);
//	pMainWindow->PostMessage(WM_DESTROY_GLDIALOG);
	wglMakeCurrent(hDC,NULL);
	wglDeleteContext(hRC);
	::ReleaseDC(m_hWnd,hDC);
}

BOOL C_OpenGL_Hist::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;

}

void C_OpenGL_Hist::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
/*	VERIFY(wglMakeCurrent(hDC,hRC));
	unsigned int xsize,ysize;
	float nRange = 250.0f;
	if(cy==0)
		cy=1;
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	xsize = cx;
	ysize = tmprows*xsize/tmpcols;
	if(ysize>tmpsize_y)	{
		ysize = cy;
		xsize = tmpcols*ysize/tmprows;
	}
	xscale = (float)xsize/(float)tmpcols;
	yscale = (float)ysize/(float)tmprows;
	xoffset = (GLint)((cx-xsize)*0.5);
	yoffset = (GLint)((cy+ysize)*0.5);
	glPixelStorei(GL_UNPACK_ALIGNMENT,4);
	glPixelZoom(xscale,-yscale);
//	glRasterPos2f(-xoffset,yoffset);		

	if (cx <= cy) 
		glOrtho (-nRange, nRange, -nRange*cy/cx, nRange*cy/cx, -nRange, nRange);
    else 
		glOrtho (-nRange*cx/cy, nRange*cx/cy, -nRange, nRange, -nRange, nRange);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	VERIFY(wglMakeCurrent(NULL,NULL));	*/
}

int C_OpenGL_Hist::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int nPixelFormat;					// Pixel format index
	hDC = ::GetDC(m_hWnd);			// Get the Device context

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this structure
		1,								// Version of this structure	
		PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
		PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
		PFD_DOUBLEBUFFER,				// Double buffered mode
		PFD_TYPE_RGBA,					// RGBA Color mode
		24,								// Want 24bit color 
		0,0,0,0,0,0,					// Not used to select mode
		0,0,							// Not used to select mode
		0,0,0,0,0,						// Not used to select mode
		32,								// Size of depth buffer
		0,								// Not used to select mode
		0,								// Not used to select mode
		PFD_MAIN_PLANE,					// Draw in main plane
		0,								// Not used to select mode
		0,0,0 };						// Not used to select mode

	// Choose a pixel format that best matches that described in pfd
	nPixelFormat = ChoosePixelFormat(hDC, &pfd);

	// Set the pixel format for the device context
	VERIFY(SetPixelFormat(hDC, nPixelFormat, &pfd));

	// Create the rendering context
	hRC = wglCreateContext(hDC);

	// Make the rendering context current, perform initialization, then
	// deselect it
//	SetTimer(123,17,NULL);
	VERIFY(wglMakeCurrent(hDC,hRC));
	GLSetupRC();
	wglMakeCurrent(NULL,NULL);
	return 0;
}

void C_OpenGL_Hist::GLSetupRC()
{
	glClearColor(0.98f,0.98f,0.98f,1.0f);
	glViewport(0,0,tmpcols,tmprows);
	glOrtho(0.0,tmpcols-0.0,0.0,tmprows-0.0,-1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
}

void C_OpenGL_Hist::OnPaint() 
{
	double val;
	unsigned int licznik=0;
	unsigned int a;
	CPaintDC dc(this); // device context for painting
	
	wglMakeCurrent(hDC,hRC);

	glLoadIdentity();

	glColor3f(0.5f,0.5f,0.5f);
	glMatrixMode(GL_MODELVIEW);
	glBegin(GL_LINES);
	licznik = 0;
	for(a=0;a<points;a+=(unsigned int)delta)	{
		val = histogram[licznik]/max*(wzmocnienie*tmprows-gora-40);
		if(val<1.0) val = 1.0;
		glVertex3f(licznik+lewo,dol,0);
		glVertex3f(licznik+lewo,(float)val+dol,0);
		licznik++;
	}
	
	glEnd();

	glFlush();
	SwapBuffers(hDC);

	wglMakeCurrent(hDC,NULL);

}


void C_OpenGL_Hist::OnCloseGl()
{
	DestroyWindow();	
}
/*
void C_OpenGL_Hist::OnTimer(UINT nIDEvent) 
{
	Invalidate();
	
	CWnd::OnTimer(nIDEvent);
}*/


