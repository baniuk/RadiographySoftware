// _Layer.cpp: implementation of the C_Layer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iso.h"
#include "_Layer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C_Layer::C_Layer()
{
	skalaX=1.9571;
	skalaY=1.9608;
	skalaZ=1.9608;
	actlayer=0;
	actskala=1;
	actwidok=_XY;
	actth=50;
	srcskala=bok;
	prevskala=1;
	obszar.left=0;
	obszar.top=0;
	obszar.right=bok;
	obszar.bottom=bok;
	info.biSize=sizeof(BITMAPINFOHEADER);
	info.biWidth=bok;
	info.biHeight=bok;
	info.biPlanes=1;
	info.biBitCount=8;
	info.biCompression=0;
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
	layer=new char[bok*bok];
	Dlayer=iplCreateImageHeader(
			1,
			0,
			IPL_DEPTH_8U,
			"GRAY",
			"GRAY",
			IPL_DATA_ORDER_PIXEL,
			IPL_ORIGIN_TL,
			IPL_ALIGN_DWORD,
			bok,
			bok,
			NULL,
			NULL,
			NULL,
			NULL);
	iplAllocateImage(Dlayer,0,0);

	Slayer=iplCreateImageHeader(
		1,
		0,
		IPL_DEPTH_8U,
		"GRAY",
		"GRAY",
		IPL_DATA_ORDER_PIXEL,
		IPL_ORIGIN_TL,
		IPL_ALIGN_DWORD,
		bok,
		bok,
		NULL,
		NULL,
		NULL,
		NULL);
	iplAllocateImage(Slayer,0,0);
	ROI.SetRect(0,0,1,1);
	r=iplCreateROI(0,0,0,1,1);
	czyROI=FALSE;

}

C_Layer::~C_Layer()
{
	delete layer;
	iplDeallocate(Slayer,IPL_IMAGE_ALL);
	iplDeallocate(Dlayer,IPL_IMAGE_ALL);
	iplDeleteROI(r);

}

void C_Layer::CToDib(IplImage* imag)
{
	iplConvertToDIBSep(imag,&info,layer,IPL_DITHER_NONE,IPL_PALCONV_NONE);
}

void C_Layer::Threshold(IplImage* In,CRect roii,int val)
{
	if(czyROI)
		In->roi=r;
	else
		In->roi=NULL;
	iplThreshold(In,In,val);
	In->roi=NULL;

}

void C_Layer::Histogram(IplImage* In,IplLUT* plut)
{
	if(czyROI)
		In->roi=r;
	else
		In->roi=NULL;
	iplComputeHisto(In, &plut );
	In->roi=NULL;

}

void C_Layer::SetROI()
{
	ROI.NormalizeRect();
	CPoint tmp=ROI.TopLeft();
	if(acttyp==0)
		iplSetROI(r,0,tmp.x,tmp.y,ROI.Width(),ROI.Height());
	else
		iplSetROI(r,0,tmp.x*actskala,tmp.y*actskala,ROI.Width()*actskala,ROI.Height()*actskala);
}

IplImage* C_Layer::ExtractLayer(unsigned int nr_layer,unsigned char widok,unsigned int skala,unsigned char typ)
{
	unsigned long int a,b,c;
	b=0;

	if(skala!=prevskala)
	{
		prevskala=skala;
		info.biWidth=bok*skala;
		info.biHeight=bok*skala;
		binfo.bmiHeader=info;
		iplDeallocate(Dlayer,IPL_IMAGE_ALL);
		Dlayer=iplCreateImageHeader(
			1,
			0,
			IPL_DEPTH_8U,
			"GRAY",
			"GRAY",
			IPL_DATA_ORDER_PIXEL,
			IPL_ORIGIN_TL,
			IPL_ALIGN_DWORD,
			bok*skala,
			bok*skala,
			NULL,
			NULL,
			NULL,
			NULL);
		iplAllocateImage(Dlayer,0,0);
	}

	
	switch(widok)
	{
	case _XY:
		for(a=bok*bok*nr_layer;a<bok*bok*nr_layer+bok*bok;a++)
			{
				Slayer->imageData[b]=cube[a];
				b++;
			}
		break;
	case _ZX:
		for(a=nr_layer;a<bok*(bok-1)+1+nr_layer;a+=bok)
			for(c=a;c<bok*bok*(bok-1)+1+a;c+=bok*bok)
			{
				Slayer->imageData[b]=cube[c];
				b++;
			}
		break;
	case _ZY:
		for(a=nr_layer*bok;a<bok+bok*nr_layer;a++)
			for(c=a;c<bok*bok*(bok-1)+1+a;c+=bok*bok)
			{
				Slayer->imageData[b]=cube[c];
				b++;
			}
		break;
	}
	if(typ!=0)
	{
		delete layer;
		layer=new char[bok*bok*skala*skala];
		info.biWidth=bok*skala;
		info.biHeight=bok*skala;
		binfo.bmiHeader=info;
	}
	else
	{	
		delete layer;
		layer=new char[bok*bok];
		info.biWidth=bok;
		info.biHeight=bok;
		binfo.bmiHeader=info;
		return Slayer;
	}
	
	switch(skala)
	{
	case 1:
		return Slayer;
		break;
	case 2:
		iplZoom(Slayer,Dlayer,2,1,2,1,typ);
		return Dlayer;
		break;
	case 3:
		iplZoom(Slayer,Dlayer,3,1,3,1,typ);
		return Dlayer;
		break;
	case 4:
		iplZoom(Slayer,Dlayer,4,1,4,1,typ);
		return Dlayer;
		break;
	}
return Slayer;
}


