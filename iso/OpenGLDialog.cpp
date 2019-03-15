// OpenGLDialog.cpp : implementation file
//

#include "stdafx.h"
#include "iso.h"
#include "OpenGLDialog.h"
//#include "gl/Gl.h"
//#include "gl/glaux.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void GLSetupRC();

/////////////////////////////////////////////////////////////////////////////
// OpenGLDialog

OpenGLDialog::OpenGLDialog()
{

}

OpenGLDialog::~OpenGLDialog()
{
}


BEGIN_MESSAGE_MAP(OpenGLDialog, CWnd)
	//{{AFX_MSG_MAP(OpenGLDialog)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// OpenGLDialog message handlers
BOOL OpenGLDialog::Create(LPCSTR WindowTitle, DWORD dwStyle,CWnd* pParent, UINT nID)
{
	CBrush blackBrush(RGB(0,0,0));
	CString ChildWindowClass = AfxRegisterWndClass(
				CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
				AfxGetApp()->LoadStandardCursor(IDC_CROSS),
				(HBRUSH)blackBrush.GetSafeHandle(),
				AfxGetApp()->LoadIcon(IDI_ICON1));
	pMainWindow=pParent;
	CRect ChildRect(0, 0, 500, 500);
	CWnd::Create(ChildWindowClass, WindowTitle, dwStyle,
						ChildRect, pParent, nID);
	return TRUE;
}

void OpenGLDialog::OnDestroy() 
{
	CWnd::OnDestroy();
	KillTimer(123);
	pMainWindow->PostMessage(WM_DESTROY_GLDIALOG);
	wglMakeCurrent(hDC,NULL);
	wglDeleteContext(hRC);
	::ReleaseDC(m_hWnd,hDC);
}

BOOL OpenGLDialog::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;

}

void OpenGLDialog::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	VERIFY(wglMakeCurrent(hDC,hRC));

	float nRange = 250.0f;
	if(cy==0)
		cy=1;
	glViewport(0,0,cx,cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (cx <= cy) 
		glOrtho (-nRange, nRange, -nRange*cy/cx, nRange*cy/cx, -nRange, nRange);
    else 
		glOrtho (-nRange*cx/cy, nRange*cx/cy, -nRange, nRange, -nRange, nRange);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	VERIFY(wglMakeCurrent(NULL,NULL));


	
}

int OpenGLDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
	SetTimer(123,17,NULL);
	VERIFY(wglMakeCurrent(hDC,hRC));
	GLSetupRC();
	wglMakeCurrent(NULL,NULL);
	return 0;
}

void GLSetupRC()
{
	glClearColor(0.0f,0.0f,0.0f,1.0f);	
	glColor3f(1.0f,1.0f,1.0f);
	glViewport(0,0,500,500);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_FLAT);
}

void OpenGLDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	wglMakeCurrent(hDC,hRC);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f,1.0f,1.0f);
	glRotatef(2,1,1,1);
//	auxWireTeapot(80.0f);
	glBegin(GL_POINTS);
	for(int t=0;t<ppointGL->GetSize();t++)
	{
		tmp=(C_Punkty* )ppointGL->GetAt(t);
		glVertex3i(tmp->x,tmp->y,tmp->z);
	}
	glEnd();
	glFlush();
	
	SwapBuffers(hDC);
	wglMakeCurrent(hDC,NULL);

}


void OpenGLDialog::OnCloseGl()
{
	DestroyWindow();	
}

void OpenGLDialog::OnTimer(UINT nIDEvent) 
{
	Invalidate();
	
	CWnd::OnTimer(nIDEvent);
}

void OpenGLDialog::GetPoints(CPtrArray* _ppoints)
{
	ppointGL=_ppoints;
}
