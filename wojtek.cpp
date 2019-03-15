// Load_Disp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "C_Image.h"
#include <iostream>
#include <fstream>


using namespace std;

void imageshow(char *WinName,IplImage *img);

void MEDIAN(C_Image* input,C_Image* output,int type,int sizex,int sizey);
double Partition(double* c, int a, int b);
double Hoare(double *tab,int size,int mid);

void PARTICALS(C_Image *image,C_Image *bin);
double Maks(C_Image *image);
void Param(C_Image *image,C_Image *indek,C_Image *bin,double *tab,double number);
void Outline(C_Image* image,C_Image *outln);

int _tmain(int argc, _TCHAR* argv[])
{
	IplImage *img;
	
	
	C_Image image;
	C_Image nowy;
	
	if(!image.ReadTiff("D:\\test.tif"))
		return -1;
// -------------------------------------------------------------------------------------------------		



// nagranie do pliku - format bin - do obs³ugi w matlabie
	//image.saveimage2matlab("c:\\test.bin");		
	
// tworzenie nowego obrazka (nie tiffa)
//	C_Image nowy_obraz();
//	unsigned int x_res, y_res;
//	x_res = 600;
//	y_res = 800;
//	nowy_obraz.AllocateData(y_res,x_res);

// kopiowanie obrazków
//	C_Image destination;		// tu bêdzie kopia
//	image.CloneImage(&destination);		// skopiowanie obrazka image do destination
	



	//Median(&image,&nowy,0,9,9);
//	image.saveimage2matlab("c:\\test1.bin");	

// ----- Wyœwietlanie za pomoc¹ CV ----------------------------------------------
	//size.height = image._rows;
	//size.width = image._cols;
	//img = cvCreateImage(size, IPL_DEPTH_16U, 1);
	//img->imageData = image.ReturnIPLBuffor();
	//imageshow("Image",img);
	//cvWaitKey();
	
//------- Sprz¹tanie -----------------------------------------------------------	
	//cvDestroyWindow("Image");
	//cvReleaseImage(&img);
	//image2=Median(&image,0,3,3);
	return 0;
}

void imageshow(char *WinName,IplImage *img)
{
	cvNamedWindow(WinName,1);
	cvShowImage(WinName,img);
}



////////////////////MEDIANA//////////////////////////////////////////
void MEDIAN(C_Image* input,C_Image* output, int type,int sizex,int sizey)
{
	
	unsigned int x_res, y_res;
	x_res = input->_rows;
	y_res = input->_cols;
	output->AllocateData(x_res,y_res);
	
	double* mask=new double [sizex*sizey];
	int ind=0;
	double pom=0;
	if(type==0)
	{
		for(int i=0;i<input->_rows;i++)
			for(int j=0;j<input->_cols;j++)
			{
				ind=0;
				for (int m=0;m<sizex;m++)
					for (int n=0;n<sizey;n++)			
						if((i+m)-((sizex-1)/2)>0)
							if((j+n-((sizey-1)/2))>0)
								if((i+m)-((sizex-1)/2)<input->_rows)
									if((j+n-((sizey-1)/2))<input->_cols)
									{
										input->GetPixel(i+m-((sizex-1)/2),j+n-((sizey-1)/2),pom);
										mask[ind]=pom;
										ind=ind+1;
									}
				if(ind%2==0)
				{
					double war;
					war=Hoare(mask,ind,ind/2);
					output->SetPixel(i,j,war);	
				}
				if(ind%2!=0)
				{
					double war;
					war=Hoare(mask,ind,(ind-1)/2);
					output->SetPixel(i,j,war);	
				}
			}
	}
	if(type==1)/////////Median cross///////////
	{
		for(int i=0;i<input->_rows;i++)
			for(int j=0;j<input->_cols;j++)
			{
				ind=0;
				for (int m=0;m<sizex;m++)
					if((i+m)-((sizex-1))/2>0)
						if((i+m)-((sizex-1))/2<input->_rows)
						{
							input->GetPixel(i+m-(sizex-1)/2,j,pom);
							mask[ind]=pom;			
							ind=ind+1;
						}
				for (int n=0;n<sizey;n++)
					if((j+n-(sizey-1)/2)>0)							
						if(n!=(sizey+1)/2)
							if((j+n-(sizey-1)/2)<input->_cols)
							{
								input->GetPixel(i,(j+n-(sizey-1)/2),pom);
								mask[ind]=pom;			
								ind=ind+1;
							}
				if(ind%2==0)
				{
					double war;
					war=Hoare(mask,ind,ind/2);
					output->SetPixel(i,j,war);	
				}
				if(ind%2!=0)
				{
					double war;
					output->SetPixel(i,j,war);	
				}
			}
	}

	if(type==2)///////////Median X///////////
	{
		for(int i=0;i<input->_rows;i++)
			for(int j=0;j<input->_cols;j++)
			{
				int pom1=0;
				ind=0;
				for(int m=0;m<sizex;m++)
					{
						if((i+m)-(sizex-1)/2>0)
							if((i+m)-(sizex-1)/2<input->_rows)
								if(j+pom1-(sizex-1)/2>0)
									if(j+pom1-(sizex-1)/2<input->_cols)
									{
										input->GetPixel(i+m-(sizex-1)/2,j+pom1-(sizex-1)/2,pom);
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
								if((i+m)-(sizex-1)/2<input->_rows)
									if(j-pom1+(sizex-1)/2<input->_cols)
									{
										input->GetPixel(i+m-(sizex-1)/2,j+(sizex-1)/2-pom1,pom);
										mask[ind]=pom;
										ind++;
									}
					pom1=pom1+1;
				}
				if(ind%2==0)
				{
					double war;
					war=Hoare(mask,ind,ind/2);
					output->SetPixel(i,j,war);	
				}
				if(ind%2!=0)
				{
					double war;
					war=Hoare(mask,ind,(ind-1)/2);
					output->SetPixel(i,j,war);	
				}

			}
	}
}



double Hoare(double *tab,int size,int mid)
{
	int n,i,j;
	n=size;
	double k,w;
	w=mid;
	i=0;
	j=n-1;
	n=(int)w;
	while (i!=j)
		{
			k=Partition(tab,i,j);
			k=k-i+1;
			if (k>=w) j=i+(int)k-1;
				if (k<w)
					{
						w-=(int)k;
						i+=(int)k;
					}
		}
	return(tab[i]);

}



double Partition(double* c, int a, int b)
{
double e,tmp;
e=c[a];        //elemennt dzielacy
while (a<b)
        {
        while ((a<b) && (c[b]>=e)) b--;
        while ((a<b) && (c[a]<e)) a++;
        if (a<b)
                {
                tmp=c[a];
                c[a]=c[b];
                c[b]=tmp;
                }
        }
return a;
}

/////////////////////////koniec MEDIANY//////////////////////////////////


////////////////////////////PARTICALS///////////////////////////////////////


void PARTICALS(C_Image *image,C_Image *bin)
{
	
	
	unsigned int x_res, y_res;
	x_res = image->_rows;
	y_res = image->_cols;
	C_Image indek;
	indek.AllocateData(x_res,y_res);

	double sklejka[1000];
	int licz=0;

	for(int x=0;x<indek._rows;x++)
		for(int y=0;y<indek._cols;y++)
		{
			double pom;
			bin->GetPixel(x,y,pom);
			indek.SetPixel(x,y,pom);
		}

	

	for(int x=1;x<indek._rows;x++)
		for(int y=1;y<indek._cols;y++)
		{
			double war1,war2,war3,war4,war5,war6,war7;
			
			bin->GetPixel(x,y,war1);
			if(war1==1)
			{
				indek.GetPixel(x-1,y-1,war2);
				indek.GetPixel(x-1,y,war3);
				indek.GetPixel(x-1,y+1,war4);
				indek.GetPixel(x,y-1,war5);
				if(war2==0&&war3==0&&war4==0&&war5==0)
				{
					licz=licz+1;
					indek.SetPixel(x,y,licz);
					sklejka[licz]=licz+1;
				}
				else
				{
					war6=0;
					war7=0;
					
					if(war2!=0)
						war6=war2;
					if(war3!=0)
						war6=war3;
					if(war4!=0)
						war6=war4;
					if(war5!=0)
						war6=war5;
					if(war2!=0&&war2!=war6)
						war7=war2;
					if(war3!=0&&war3!=war6)
						war7=war3;
					if(war4!=0&&war4!=war6)
						war7=war4;
					if(war5!=0&&war5!=war6)
						war7=war5;
					if(war7==0)
						indek.SetPixel(x,y,war6);
					else
					{
						indek.SetPixel(x,y,min(war6,war7));
						sklejka[(int)max(war6,war7)]=min(war6,war7);
					}

				}
			}
		}
		for(int i=1;i<licz;i++)
		{
			int war;
			war=1+licz-i;
			for(int x=0;x<indek._rows;x++)
				for(int y=0;y<indek._cols;y++)
				{
					double war1;
					indek.GetPixel(x,y,war1);
					if(war1==war)
					{
						double pom=sklejka[war];
						indek.SetPixel(x,y,pom);
					}
				}
		}

		double obecne=1;
		double warmax=Maks(&indek);
		
		for(int pom=1;pom<=warmax;pom++)
		{
			int licz1=0;
			for(int x=0;x<indek._rows;x++)
				for(int y=0;y<indek._cols;y++)
				{
					double val1;
					indek.GetPixel(x,y,val1);
					if(val1==pom)
					{
						indek.SetPixel(x,y,obecne);
						licz1=licz1+1;
					}
				}
			if(licz1>0)
				obecne=obecne+1;

		}
		double *tab=new double [(int)obecne*7];

		Param(image,&indek,bin,tab,obecne);

		
		
		ofstream plk_wy("d:\\parameters.txt");
		plk_wy<<"Lp. |filed|obw | x | y | x | y |\n";
		for(int i=0;i<obecne-1;i++)
		{
			plk_wy<<i+1<<"  |";
			plk_wy<<(tab[i])<<"    |";
			plk_wy<<(tab[i+(int)obecne])<<"|";
			plk_wy<<(tab[i+2*(int)obecne])<<"|";
			plk_wy<<(tab[i+3*(int)obecne])<<"|";
			plk_wy<<(tab[i+4*(int)obecne])<<"|";
			plk_wy<<(tab[i+5*(int)obecne])<<"|";
			plk_wy<<(tab[i+6*(int)obecne])<<"|";
			plk_wy<<"\n";
		}
		plk_wy.close();
		
}


double Maks(C_Image *image)
{
	double war=-100000;
	double war1;
	for(int i=0;i<image->_rows;i++)
		for(int j=0;j<image->_cols;j++)
		{
			image->GetPixel(i,j,war1);
			if(war1>war)
			{
				war=war1;
			}
		}
	return war;
}


void Param(C_Image *image,C_Image *indek,C_Image *bin,double *tab,double number)
{
	C_Image outln;
	Outline(bin,&outln);
	double val;
	
	int ind=0;
	for(int i=0;i<indek->_rows;i++)
		for(int j=0;j<indek->_cols;j++)
		{
			double val=0;
			
			double ilosc=0,sumax=0,sumay=0,sumaxx=0,sumayy=0;
			double val1,val2,val3;
			
			double il=0;;
			indek->GetPixel(i,j,val1);
			if(val1>0)
			{
				double *tab1=new double[100000];
				double *tab2=new double[100000];
				double obw=0;
				for(int ii=0;ii<indek->_rows;ii++)
					for(int jj=0;jj<indek->_cols;jj++)
					{
						indek->GetPixel(ii,jj,val2);
						
						if(val2==val1)
						{
							
							outln.GetPixel(ii,jj,val);
							if(val==1)
							{
								obw=obw+1;
							}
							tab[ind+(int)number]=obw;
							image->GetPixel(ii,jj,val3);
							tab1[ind]=ii;
							tab1[ind+50000]=jj;
							ilosc=ilosc+val3;
							il++;
							tab[ind]=il;

							sumax=sumax+val3*ii;
							sumay=sumay+val3*jj;
							sumaxx=sumaxx+ii;
							sumayy=sumayy+jj;
							indek->SetPixel(ii,jj,0);
						}
					}
					double vec=0;
					if(obw>30)
					{
						
						for(int z=0;z<obw;z++)
							for(int zz=0;zz<obw;z++)
							{
								double pom=sqrt((tab1[z]-tab1[zz])*(tab1[z]-tab1[zz])+(tab1[z+50000]-tab1[zz+50000])*(tab1[z+50000]-tab1[zz+50000]));
								if(pom>vec)
									vec=pom;
							}

						
					}
					tab[ind+2*(int)number]=(sumax*1.0)/ilosc;
					tab[ind+3*(int)number]=(sumay*1.0)/ilosc;
					tab[ind+4*(int)number]=(sumaxx*1.0)/il;
					tab[ind+5*(int)number]=(sumayy*1.0)/il;		
					tab[ind+6*(int)number]=vec;	
					ind++;
			}
		}
}

////////////////////////KONIEC PARTICALS//////////////////////////////////

void Outline(C_Image* image,C_Image* outln)
{
	// POMOCNICZE OBRAZKI
	unsigned int x_res, y_res;
	x_res = image->_cols;
	y_res = image->_rows;

	// DO EROZJI
	C_Image image2;
	image2.AllocateData(y_res,x_res);
	// TYMCZASOWY OBRAZEK BINARNY
	
	// WYNIKOWY
	outln->AllocateData(y_res,x_res);


	// ZMIENNE
	int size_x = 3;
	int size_y = 3;
	double min;
	double zm;
	int k = 0;

	// IMPLEMENTACJA EROZJI
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{	
			k = 0;
			for (unsigned int i = 0; i < size_y; i++)
			{	
				for (unsigned int j = 0; j < size_x; j++)
				{
					if((i+m)-((size_x-1)/2)>0)
						if((j+n-((size_y-1)/2))>0)
							if((i+m)-((size_x-1)/2)<image2._cols)
								if((j+n-((size_y-1)/2))<image2._rows)
								{
									image->GetPixel(j+n-(size_x-1)/2,i+m-(size_y-1)/2,zm);
									if (zm != 1)
									{
										k = 1;
									}
									
								}
				}
			}
			if (k == 1)
			{
				image2.SetPixel(n,m,1);
			}
			else
			{
				image2.SetPixel(n,m,0);
			}
		}
	}

	double zm1 = 0, zm2 = 0;
	// ODEJMOWANIE
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{
			image->GetPixel(n,m,zm1);
			image2.GetPixel(n,m,zm2);
			if (zm1-zm2 == 0)
			{
				outln->SetPixel(n,m,1);
			}
			else
			{
				outln->SetPixel(n,m,0);
			}
		}
	}
	
}