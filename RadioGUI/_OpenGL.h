#pragma once

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

// C_OpenGL

class C_OpenGL : public CWnd
{
public:
	C_OpenGL(unsigned char* data,unsigned int _xsize, unsigned int _ysize,unsigned int _window_x, unsigned int _window_y);
	C_OpenGL();
	RectF win_size;
	CWnd* pMainWindow;
	HDC hDC;
	HGLRC hRC;
	BOOL Create(LPCSTR WindowTitle,	DWORD dwStyle, CWnd* pParentWnd, UINT ID = NULL);
	unsigned char* tmpdata;
	unsigned int window_x, window_y, ysize, xsize;
	void OnCloseGl();
	virtual ~C_OpenGL();
	BITMAPINFOHEADER info;
	BITMAPINFO binfo;
	RGBQUAD paleta[256];
protected:
	//{{AFX_MSG(OpenGLDialog)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
//	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	float xscale,yscale;
	GLint xoffset,yoffset;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
};


