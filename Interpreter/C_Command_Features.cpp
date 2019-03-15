#include "StdAfx.h"
#include "C_Command_Features.h"

C_Command_Features::C_Command_Features(void)
{
	UINT parint[5] = {IMAGE,IMAGE,IMAGE,NUMBER,NUMBER};
	UINT parout[2] = {MATRIX,NUMBER};
	InitCommand("calcfeatures",5,2,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "[MATRIX,NUMBER] = calcfeatures(IMAGE,IMAGE,IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Features::~C_Command_Features(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Features::Do(C_Error& err)
{
// int  FUTURES(C_Image *image,C_Image *indek,C_Image *bin,double *tab,double minsiz,double maxsiz,double *param); oryginal, inedek, binarny, zwraca tab, min, max wady, tablica parametrów; zwraca liczbe wad klasyfikowanych
	C_Image_Container *image;
	C_Image_Container *indek;
	C_Image_Container *bin;
	C_Matrix_Container *p_minsiz;
	C_Matrix_Container *p_maxsiz;

	C_Matrix_Container tabpom;
	C_Matrix_Container il_wad;

	tabpom.AllocateData(1,30000);
	il_wad.AllocateData(1,1);
	
	double minsiz, maxsiz;
		
	image = (C_Image_Container*)intab[0]->data;
	indek= (C_Image_Container*)intab[1]->data;
	bin = (C_Image_Container*)intab[2]->data;
	p_minsiz = (C_Matrix_Container*)intab[3]->data;
	p_maxsiz = (C_Matrix_Container*)intab[4]->data;
	minsiz = p_minsiz->data[0];
	maxsiz = p_maxsiz->data[0];


	int poz=13;
	bin->Normalize(0,1);
	C_Image_Container outln;
	Outline(bin,&outln);
	C_Image_Container now;
	now.AllocateData(outln._rows,outln._cols);
	double *tab1=new double[100000];
	double *tab2=new double[100000];
	double val=0;	
	double ilosc=0,sumax=0,sumay=0,sumaxx=0,sumayy=0;
	double val1,val2,val3;
	
	int ind=0;
	double obrx1;
	double obrx2;


	for(unsigned int i=0;i<outln._rows;i++)
		for(unsigned int j=0;j<outln._cols;j++)
		{
			double pom;
			indek->GetPixel(i,j,pom);
			now.SetPixel(i,j,pom);
		}

	for(unsigned int i=0;i<indek->_rows;i++)
		for(unsigned int j=0;j<indek->_cols;j++)
		{
			val=0;
			ilosc=0;sumax=0;sumay=0;sumaxx=0;sumayy=0;
			double il=0;
			int num=0;
			indek->GetPixel(i,j,val1);
			if(val1>0)
			{
				double obw=0;
				for(unsigned int ii=0;ii<indek->_rows;ii++)
					for(unsigned int jj=0;jj<indek->_cols;jj++)
					{
						indek->GetPixel(ii,jj,val2);
						
						if(val2==val1)
						{
							
							outln.GetPixel(ii,jj,val);
							if(val==1)
							{
								obw=obw+1;
								tab1[num]=ii;
								tab1[num+50000]=jj;
								num++;
							}
							
							tabpom.data[ind*poz+1]=obw;
							image->GetPixel(ii,jj,val3);
							ilosc=ilosc+val3;
							il++;
							tabpom.data[ind*poz]=il;
							sumax=sumax+val3*ii;
							sumay=sumay+val3*jj;
							sumaxx=sumaxx+ii;
							sumayy=sumayy+jj;
							indek->SetPixel(ii,jj,0);
						}
					}
					double vec=0;
					
					if(obw>30&&il>minsiz)
					{		
						for(int z=0;z<obw;z++)
		 				for(int zz=0;zz<obw;zz++)
							{
								double pom=sqrt((tab1[z]-tab1[zz])*(tab1[z]-tab1[zz])+(tab1[z+50000]-tab1[zz+50000])*(tab1[z+50000]-tab1[zz+50000]));
								if(pom>vec)
								{
									vec=pom;
									tabpom.data[ind*poz+6]=vec;
									if(tab1[z+50000]>tab1[zz+50000])
									{
										tabpom.data[ind*poz+9]=atan(abs(tab1[z]-tab1[zz])/abs(tab1[z+50000]-tab1[zz+50000]));
									}
									else
									{
										tabpom.data[ind*poz+9]=3.14-atan(abs(tab1[zz]-tab1[z])/abs(tab1[zz+50000]-tab1[z+50000]));
									}
									tabpom.data[ind*poz+7]=tabpom.data[ind*poz]/(3.14*tabpom.data[ind*poz+6]);
									obrx1=tab1[z]*cos(tabpom.data[ind*poz+9])-tab1[z+50000]*sin(tabpom.data[ind*poz+9]);
									obrx2=tab1[zz]*cos(tabpom.data[ind*poz+9])-tab1[zz+50000]*sin(tabpom.data[ind*poz+9]);
									if(abs(obrx1-obrx2)<1)
									{
										tabpom.data[ind*poz+8]=abs((tab1[z]*sin(tabpom.data[ind*poz+9])+tab1[z+50000]*cos(tabpom.data[ind*poz+9]))-(tab1[zz]*sin(tabpom.data[ind*poz+9])+tab1[zz+50000]*cos(tabpom.data[ind*poz+9])));
									}
								}
						}
					}
					if(tabpom.data[ind*poz]>minsiz&&tabpom.data[ind*poz]<maxsiz)
					{	
						tabpom.data[ind*poz+4]=(sumax*1.0)/ilosc;
						tabpom.data[ind*poz+5]=(sumay*1.0)/ilosc;
						tabpom.data[ind*poz+2]=(sumaxx*1.0)/il;
						tabpom.data[ind*poz+3]=(sumayy*1.0)/il;

						ind++;
					}
			}
		}
		
	delete[] tab1;
	delete[] tab2;
	il_wad.data[0] = ind;
	outtab[0]->AssignValue((void*)&tabpom,err);
	outtab[1]->AssignValue((void*)&il_wad,err);
//	return ind;
}

void C_Command_Features::Outline(C_Image_Container* image,C_Image_Container* outln)
{
	// POMOCNICZE OBRAZKI
	unsigned int x_res, y_res;
	x_res = image->_cols;
	y_res = image->_rows;

	// DO EROZJI
	C_Image_Container image2;
	image2.AllocateData(y_res,x_res);
	// TYMCZASOWY OBRAZEK BINARNY
	
	// WYNIKOWY
	outln->AllocateData(y_res,x_res);


	// ZMIENNE
	unsigned int size_x = 3;
	unsigned int size_y = 3;
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
