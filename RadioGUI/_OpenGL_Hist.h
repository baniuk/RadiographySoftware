#pragma once


// C_OpenGL_Hist

class C_OpenGL_Hist : public CWnd
{
	DECLARE_DYNAMIC(C_OpenGL_Hist)

public:
	C_OpenGL_Hist();
	C_OpenGL_Hist(double* data,unsigned int p_points, double pmax, double pwzm,unsigned int xsize, unsigned int ysize);
	CWnd* pMainWindow;
	HGLRC hRC;
	HDC hDC;
	BOOL Create(LPCSTR WindowTitle,	DWORD dwStyle, CWnd* pParentWnd, UINT ID = NULL);
	unsigned short* tmpdata;
	unsigned int tmpsize_x, tmpsize_y, tmprows, tmpcols;
	void OnCloseGl();
	void GLSetupRC();
	virtual ~C_OpenGL_Hist();

protected:
	//{{AFX_MSG(OpenGLDialog1)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
//	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	double *histogram;
	double max;
	unsigned int points;
	float gora,dol,lewo,prawo;
	double delta;
	double wzmocnienie;
};


