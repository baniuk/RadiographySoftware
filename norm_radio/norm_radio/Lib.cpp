#include "Cpp_Lib.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <crtdbg.h>

double Hoare(double *tab,int size,int mid);
double Partition(double* c, int a, int b);




void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"norm_radio");
	strcpy_s(definition->version,MAX_VER,"2.0");
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE =norm_radio(IMAGE, NUMBER, NUMBER, NUMBER, NUMBER, MATRIX)");
	definition->numofout = 1;
	definition->numofin = 6;
	definition->typesofin[0] = IMAGE; 
	definition->typesofin[1] = NUMBER;
	definition->typesofin[2] = NUMBER; 
	definition->typesofin[3] = NUMBER;
	definition->typesofin[4] = NUMBER;
	definition->typesofin[5] = MATRIX;
	definition->typesofout[0] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"norm_radio");
	definition->ADDIP_version = 2.0;

}


void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	
	
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Image_Container *img;
	C_Matrix_Container *Inum;
	C_Matrix_Container *Ibasetr;
	C_Matrix_Container *Nminr;
	C_Matrix_Container *Idbase;
	C_Matrix_Container *Matr;

	C_Image_Container result;
	
	// variable output has size of numofout (set in Introduce function)
	img = (C_Image_Container*)intab[0]->data;
	Inum= (C_Matrix_Container*)intab[1]->data;
	Ibasetr= (C_Matrix_Container*)intab[2]->data;
	Nminr= (C_Matrix_Container*)intab[3]->data;
	Idbase= (C_Matrix_Container*)intab[4]->data;
	Matr= (C_Matrix_Container*)intab[5]->data;
	
	double x1,x2,y1,y2;
	Matr->GetPixel(0,0,x1);
	Matr->GetPixel(0,1,y1);
	Matr->GetPixel(0,2,x2);
	Matr->GetPixel(0,3,y2);
	
	double Min=9999999,Max;
	double pix;
	
	img->getMinMax(Min,Max);
	Max++;									// inaczej dla warto�ci maksymalnej b�dzie pisa� poza tablic� w linii poni�ej
	double *hist= new double[(int)Max];


	double Imin, Imax, Nmin, Nmax;
	int i,j;
	//zarowanie hist
	for(i=0;i<Max;i++)
		hist[i]=0;
	
	//tworzenie histogramu
	for(i=0;i<(int)img->_cols;i++)
		{
			for(j=0;j<(int)img->_rows;j++)
			{
				img->GetPixel(j,i,pix);
				_ASSERT(pix<Max && pix>-1);
				hist[(int)pix]=hist[(int)pix]+1;		// w tej linii w�a�nie. rezerwacja new double[max] powoduje �e mo�na adrtesowa� od 0 do max-1, a z getpixel otrzymujesz przeciez max.
				
			}
		}
	

	double najw=0,najm=999,Ibase=0;

	for(i=(int)Min;i<(int)Max;i++)
	{	
		_ASSERT(i<Max && i>-1);
		if(hist[i]>najw)
		{
			najw=hist[i];
			Ibase=i;			
		}
	}

	pix=0;
	
	int licz=0;
	int rozmiar_maska = (int)Idbase->data[0]*2;
	double *maska=new double[(int)Idbase->data[0]*2];
		//liczenie otoczenia maksimum

	int pom1=-(int)Idbase->data[0];

	if(Ibase>(int)Idbase->data[0])
		while(pix<Idbase->data[0])
		{
			_ASSERT((int)Ibase+pom1<Max && (int)Ibase+pom1>-1);
			if(hist[(int)Ibase+pom1]>0)
			{
				_ASSERT(licz<rozmiar_maska && licz>-1);
				maska[licz]=Ibase-pom1;
				pix=pix+1;
				licz++;
				pom1=pom1+1;
			}
			else
				pom1=pom1+1;
			if(Ibase-pom1<0)
			{
				pix=Idbase->data[0];
			}

		}

	pom1=1;
	pix=0;
	
	if(Ibase>(int)Idbase->data[0])
		while(pix<(int)Idbase->data[0])
		{
			_ASSERT((int)Ibase-pom1<Max && (int)Ibase-pom1>-1);
			if(hist[(int)Ibase-pom1]>0)
			{
				_ASSERT(licz<rozmiar_maska && licz>-1);
				maska[licz]=Ibase-pom1;
				pix=pix+1;
				licz++;
				pom1=pom1+1;
			}
			else
				pom1=pom1+1;
			if(Ibase+pom1>Max)
			{
				pix=(int)Idbase->data[0];
			}

		}
		
	_ASSERT(Ibase<Max && Ibase>-1);
	Nmax=hist[(int)Ibase];
	licz=0;

	if(x2<1&&y2<1)
		Ibase=Hoare(maska,licz,licz/2);
	else
	{
		double suma=0;
		for(i=(int)x1;i<=(int)x2;i++)
			for(j=(int)y1;j<=(int)y2;j++)
			{
				img->GetPixel(i,j,pix);
				suma=suma+pix;
				licz++;
			}
		Ibase=suma/licz;
	}

	Nmin=Nminr->data[0]*Nmax/100;


	for(i=0;i<(int)Max;i++)
		if(hist[i]>Nmin)
		{
			_ASSERT(i-1<Max && i-1>-1);
			if(abs(hist[i-1]-Nmin)<abs(hist[i]-Nmin))		// dla i=0 b�dzie liczba ujemna w indeksie hist[i-1]??
				Imin=i-1;
			else
				Imin=i;
			i=(int)Max;
		}
	for(i=(int)Max;i>0;i--)
		if(hist[i]>Nmin)
		{
			_ASSERT(i-1<Max && i-1>-1);
			if(abs(hist[i-1]-Nmin)<abs(hist[i]-Nmin))
				Imax=i-1;
			else
				Imax=i;
			i=0;
		}

	double il1=Ibase-Imin,il2=Imax-Ibase;
	double ilr1=Ibasetr->data[0]-1,ilr2=65534-Ibasetr->data[0];
	
	img->CloneObject(&result);

	for(i=0;i<(int)img->_cols;i++)
			for(j=0;j<(int)img->_rows;j++)
			{
				img->GetPixel(j,i,pix);
				if(Imin>pix)
					pix=Imin;
				if(Imax<pix)
					pix=Imax;
				if((ilr2/il2)<(ilr1/il1))
					if((ilr2/il2)*(pix-Ibase)+Ibasetr->data[0]<1)
						result.SetPixel(j,i,1);
					else
						if((ilr2/il2)*(pix-Ibase)+Ibasetr->data[0]>65534)
							result.SetPixel(j,i,65534);
						else
							result.SetPixel(j,i,ceil((ilr2/il2)*(pix-Ibase)+Ibasetr->data[0]));
				else
					if((ilr1/il1)*(pix-Ibase)+Ibasetr->data[0]<1)
						result.SetPixel(j,i,1);
					else
						if((ilr1/il1)*(pix-Ibase)+Ibasetr->data[0]>65534)
							result.SetPixel(j,i,65534);
						else
							result.SetPixel(j,i,ceil((ilr1/il1)*(pix-Ibase)+Ibasetr->data[0]));
			}
	
	outtab[0]->AssignValue((void*)&result,err);
	delete[] hist;
	delete[] maska;
//	BOOL rr = _CrtDumpMemoryLeaks();
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
