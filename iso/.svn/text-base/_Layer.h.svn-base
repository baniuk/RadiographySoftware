// _Layer.h: interface for the C_Layer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__LAYER_H__ED565724_A1B8_11D4_BB95_C55C8338C269__INCLUDED_)
#define AFX__LAYER_H__ED565724_A1B8_11D4_BB95_C55C8338C269__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "_Cube.h"
#include <ipl.h>

class C_Layer : public C_Cube  
{
public:
	double skalaX,skalaY,skalaZ;
	CRect obszar;
	BITMAPINFOHEADER info;
	BITMAPINFO binfo;
	RGBQUAD paleta[256];
	C_Layer();

	IplImage* Slayer;
	IplImage* Dlayer;
	IplROI* r;


	char* layer;
	IplImage* ExtractLayer(unsigned int nr_layer,unsigned char widok,unsigned int skala,unsigned char typ);
	void CToDib(IplImage* imag);
	void Threshold(IplImage* In,CRect roii,int val);
	void Histogram(IplImage* In,IplLUT* plut);
	void SetROI();
	unsigned int actlayer;
	unsigned int actskala;
	unsigned int srcskala;
	int actth;
	unsigned char acttyp;
	unsigned int actwidok;
	BOOL czyROI;
	CRect ROI;
	virtual ~C_Layer();
private:
	unsigned int prevskala;
	
};

#endif // !defined(AFX__LAYER_H__ED565724_A1B8_11D4_BB95_C55C8338C269__INCLUDED_)
