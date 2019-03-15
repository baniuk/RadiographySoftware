// _OpenGL.cpp : implementation file
//

#include "stdafx.h"
#include "RadioGUI.h"
#include "_OpenGL.h"


// C_OpenGL


C_OpenGL::C_OpenGL(unsigned char* data,unsigned int _xsize, unsigned int _ysize,unsigned int _window_x, unsigned int _window_y)
{
	tmpdata = data;
//	tmpsize_x = xsize;
//	tmpsize_y = ysize;
	
	window_x = _window_x;
	window_y = _window_y;	
	ysize = _ysize;
	xsize = _xsize;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}
C_OpenGL::C_OpenGL()
{
	tmpdata = NULL;
}

C_OpenGL::~C_OpenGL()
{
}

BOOL C_OpenGL::Create(LPCSTR WindowTitle, DWORD dwStyle,CWnd* pParent, UINT nID)
{
	CBrush blackBrush(RGB(0,0,0));
	CString ChildWindowClass = AfxRegisterWndClass(
				CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
				AfxGetApp()->LoadStandardCursor(IDC_CROSS),
				(HBRUSH)blackBrush.GetSafeHandle(),
				AfxGetApp()->LoadIcon(IDI_ICON2));
	pMainWindow=pParent;
	CRect ChildRect(10, 10, window_x, window_y);
	CWnd::CreateEx(0,ChildWindowClass, WindowTitle, dwStyle,
						ChildRect, pParent, nID);
	win_size.X = (REAL)0;
	win_size.Y = (REAL)0;
	win_size.Width = (REAL)window_x;
	win_size.Height = (REAL)window_y;
	return TRUE;
}


BEGIN_MESSAGE_MAP(C_OpenGL, CWnd)
	//{{AFX_MSG_MAP(OpenGLDialog)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
//	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// C_OpenGL message handlers
void C_OpenGL::OnDestroy() 
{
	CWnd::OnDestroy();
//	KillTimer(123);
//	pMainWindow->PostMessage(WM_DESTROY_GLDIALOG);
	::ReleaseDC(m_hWnd,hDC);
}

BOOL C_OpenGL::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;

}

void C_OpenGL::OnSize(UINT nType, int cx, int cy) 
{
/*	CWnd::OnSize(nType, cx, cy);
	VERIFY(wglMakeCurrent(hDC,hRC));
	GLSetupRC();
	VERIFY(wglMakeCurrent(NULL,NULL));	*/
}

int C_OpenGL::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
/*	int nPixelFormat;					// Pixel format index
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
	VERIFY(SetPixelFormat(hDC, nPixelFormat, &pfd));*/

	return 0;
}


void C_OpenGL::OnPaint() 
{
/*	CPaintDC dc(this); // device context for painting
	SolidBrush solidBrush(Color(255, 255, 255));
	Graphics graphics(dc.m_hDC);
	graphics.FillRectangle(&solidBrush,0,0,window_x,window_y);

	Bitmap MyBitmap(xsize,ysize,1024,PixelFormat8bppIndexed ,tmpdata);
	ColorPalette paleta;
	paleta.Flags = PaletteFlagsGrayScale;
	paleta.Count = 256;
	for(int a=0;a<256;a++)
	{
		paleta.Entries[a]=0;
	}

	MyBitmap.SetPalette(&paleta);

	graphics.DrawImage(&MyBitmap,win_size,0,0,xsize,ysize,UnitPixel,NULL,NULL,NULL);*/


/*	
	LinearGradientBrush linGrBrush(Point(0, 10),
								   Point(200, 10),
								   Color(255, 255, 0, 0),   // opaque red
								   Color(255, 0, 0, 255));  // opaque blue

	Pen pen(&linGrBrush);
	graphics.DrawLine(&pen, 0, 10, 200, 10);
	graphics.FillEllipse(&linGrBrush, 0, 30, 200, 100);
	graphics.FillRectangle(&linGrBrush, 0, 155, 500, 30);*/


	info.biSize=sizeof(BITMAPINFOHEADER);
	info.biWidth=xsize;
	info.biHeight=ysize;
	info.biPlanes=1;
	info.biBitCount=8;
	info.biCompression=BI_RGB;
	info.biSizeImage=0;
	info.biXPelsPerMeter=0;
	info.biYPelsPerMeter=0;
	info.biClrUsed=0;
	info.biClrImportant=0;
	binfo.bmiHeader=info;
	for(int a=0;a<256;a++)
	{
		binfo.bmiColors[a].rgbBlue=a;
		binfo.bmiColors[a].rgbRed=a;
		binfo.bmiColors[a].rgbGreen=a;
		binfo.bmiColors[a].rgbReserved=0;
	}

	CPaintDC dc(this); // device context for painting
	hDC = ::GetDC(m_hWnd);	
	dc.SetBkColor(0x00000000);
	StretchDIBits(hDC,0,0,window_x,window_y,0,0,xsize,ysize,(void*)tmpdata,&binfo,DIB_RGB_COLORS,SRCCOPY);
//	dc.LineTo(100,100);
	::ReleaseDC(m_hWnd, hDC); 
		

}


void C_OpenGL::OnCloseGl()
{
	DestroyWindow();	
	GdiplusShutdown(gdiplusToken);
}

/*void C_OpenGL::OnTimer(UINT nIDEvent) 
{
	Invalidate();
	
	CWnd::OnTimer(nIDEvent);
}*/

