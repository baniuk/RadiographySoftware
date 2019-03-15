// Load_Disp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "C_Image.h"


void imageshow(char *WinName,IplImage *img);
void sortquick(double* tabl, int l, int p);
unsigned int Max(C_Image* image);
unsigned int Min(C_Image* image);
void NORM(C_Image* image);
C_Image* FILTER2D(C_Image* image,int type,int sizex,int sizey);

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage *img;
	IplImage *img2;
	CvSize size;

	C_Image image;
	C_Image* image2;
	if(!image.ReadTiff("D:\\test.tif"))
		return -1;

// ----- Wyœwietlanie za pomoc¹ CV ----------------------------------------------
	size.height = image._height;
	size.width = image._width;
	img = cvCreateImage(size, IPL_DEPTH_16U, 1);
	img->imageData = image.ReturnIPLBuffor();
	
	//NORM(&image);
	image2=FILTER2D(&image,0,9,9);

	//size.height = image2->_height;
	//size.width = image2->_width;
	//img = cvCreateImage(size, IPL_DEPTH_16U, 1);
	//img->imageData = image2->ReturnIPLBuffor();
	//imageshow("Image",img);
	
	
	//image2->saveimage2matlab("D:\\test1.tif");
	
	
//------- Sprz¹tanie -----------------------------------------------------------	
	cvDestroyWindow("Image");

	cvReleaseImage(&img);
	
	
	return 0;
}

void imageshow(char *WinName,IplImage *img)
{
	cvNamedWindow(WinName,1);
	cvShowImage(WinName,img);
}

C_Image* FILTER2D(C_Image* image,int type,int sizex,int sizey)
{
	
	
	unsigned int *nowy_obraz;
	unsigned int x_res, y_res;
	x_res = image->_height;
	y_res = image->_width;
	nowy_obraz = new unsigned int[x_res*y_res];
	C_Image nowy(y_res,x_res,nowy_obraz);
	
	double* mask=new double [sizex*sizey];
	int ind=0;
	unsigned int pom=0;

	for(int k=0;k<sizex*sizey;k++)
				mask[k]=0;

	if(type==0)/////////Median rectangle///////////
		for(int i=0;i<image->_height;i++)
			for(int j=0;j<image->_width;j++)
			{
				ind=0;
				for (int m=0;m<sizex;m++)
					for (int n=0;n<sizey;n++)			
						if((i+m)-((sizex-1)/2)>0)
							if((j+n-((sizey-1)/2))>0)
								if((i+m)-((sizex-1)/2)<image->_height)
									if((j+n-((sizey-1)/2))<image->_width)
									{
										
										image->GetPixel(i+m-((sizex-1)/2),j+n-((sizey-1)/2),pom);
										mask[ind]=pom;
										ind=ind+1;
									}

				sortquick(mask,0,ind);
				if(ind%2==0)
				{
					nowy.SetPixel(i,j,mask[ind/2]);	
				}
				if(ind%2!=0)
				{
					nowy.SetPixel(i,j,mask[(ind-1)/2]);
				}

			}
	/////////////////////////
	////////////////////////

	if(type==1)/////////Median cross///////////
		for(int i=0;i<image->_height;i++)
			for(int j=0;j<image->_width;j++)
			{
				ind=0;
				for (int m=0;m<sizex;m++)
					if((i+m)-((sizex-1))/2>0)
						if((i+m)-((sizex-1))/2<image->_height)
						{
							image->GetPixel(i+m-(sizex-1)/2,j,pom);
							mask[ind]=pom;			
							ind=ind+1;
						}
				for (int n=0;n<sizey;n++)
					if((j+n-(sizey-1)/2)>0)							
						if(n!=(sizey+1)/2)
							if((j+n-(sizey-1)/2)<image->_width)
							{
								image->GetPixel(i,(j+n-(sizey-1)/2),pom);
								mask[ind]=pom;			
								ind=ind+1;
							}
				sortquick(mask,0,ind);
			
				
				if(ind%2==0)
				{
					nowy.SetPixel(i,j,mask[ind/2]);	
				}
				if(ind%2!=0)
				{
					nowy.SetPixel(i,j,mask[(ind-1)/2]);
				}
			}
	///////////////
	///////////////
	if(type==2)/////////Median X///////////
		for(int i=0;i<image->_height;i++)
			for(int j=0;j<image->_width;j++)
			{
				int pom1=0;
				ind=0;
				for(int m=0;m<sizex;m++)
					{
						if((i+m)-(sizex-1)/2>0)
							if((i+m)-(sizex-1)/2<image->_height)
								if(j+pom1-(sizex-1)/2>0)
									if(j+pom1-(sizex-1)/2<image->_width)
									{
										image->GetPixel(i+m-(sizex-1)/2,j+pom1-(sizex-1)/2,pom);
										mask[ind]=pom;
										ind++;
									}
						pom1=pom1+1;
						}
				pom1=sizex-1;
				for(int m=0;m<sizex;m++)
				{
					if(pom1!=(sizex-1)/2)
						if((i+m)-(sizex-1)/2>0)
							if(j-pom1+(sizex-1)/2>0)
								if((i+m)-(sizex-1)/2<image->_height)
									if(j-pom1+(sizex-1)/2<image->_width)
									{
										image->GetPixel(i+m-(sizex-1)/2,j+(sizex-1)/2-pom1,pom);
										mask[ind]=pom;
										ind++;
									}
					pom1=pom1+1;
				}
				sortquick(mask,0,ind);
			
			
				if(ind%2==0)
				{
					nowy.SetPixel(i,j,mask[ind/2]);	
				}
				if(ind%2!=0)
				{
					nowy.SetPixel(i,j,mask[(ind-1)/2]);
				}
			}
	if(type==3)/////////Blure///////////
		for(int i=0;i<image->_height;i++)
			for(int j=0;j<image->_width;j++)	
			{
				
				int s=0;
				ind=0;
				for (int m=0;m<sizex;m++)
					for (int n=0;n<sizey;n++)			
						if((i+m)-((sizex-1)/2)>0)
							if((j+n-((sizey-1)/2))>0)
								if((i+m)-((sizex-1)/2)<image->_height)
									if((j+n-((sizey-1)/2))<image->_width)
									{
										
										image->GetPixel(i+m-((sizex-1)/2),j+n-((sizey-1)/2),pom);
										ind=ind+pom;
										s=s+1;
										
									}
				
				nowy.SetPixel(i,j,ind/s);
			
			}
		

	return &nowy;


}



unsigned int Max(C_Image* image)
{
	unsigned int maks=0;
	unsigned int pom;
	for(int i=0;i<image->_height;i++)
			for(int j=0;j<image->_width;j++)
			{
				image->GetPixel(i,j,pom);
				if(maks<pom)
					maks=pom;
			}

	return maks;
}


unsigned int Min(C_Image* image)
{
	unsigned int mini=99999;
	unsigned int pom;
	for(int i=0;i<image->_height;i++)
			for(int j=0;j<image->_width;j++)
			{
				image->GetPixel(i,j,pom);
				if(mini>pom)
					mini=pom;
			}
	return mini;
}

void NORM(C_Image* image)
{
	unsigned int pom1;
	float pom2;
	unsigned int mini=Min(image);
	unsigned int maks=0;
	maks=Max(image);

	for(int i=0;i<image->_height;i++)
			for(int j=0;j<image->_width;j++)
			{
				image->GetPixel(i,j,pom1);
				pom2=floor((double)(((pom1-mini*1.0)/(maks-mini))*65535.0));
				image->SetPixel(i,j,pom2);
			}
}
void sortquick(double* tabl, int l, int p) 
{ 
	double v=tabl[(l+p)/2]; 
	int i,j;
	double temp; 
	i=l; 
	j=p; 
	do 
	{ 
		while (tabl[i]<v) i++; 
		while (v<tabl[j]) j--; 
		if (i<=j)
		{ 
			temp=tabl[i];
			tabl[i]=tabl[j];
			tabl[j]=temp;
			i++;
			j--;
		} 
	} 
	while (i<=j);
	if (l<j) sortquick(tabl,l,j); 
	if (i<p) sortquick(tabl,i,p); 
	
} 