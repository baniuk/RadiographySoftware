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
	strcpy_s(definition->name,MAX_NAME,"statistic");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"[NUMBER, NUMBER, NUMBER, NUMBER] =statistic(IMAGE, MATRIX)");
	definition->numofout = 4;
	definition->numofin = 2;
	definition->typesofin[0] = IMAGE; 
	definition->typesofout[0] = NUMBER;
	definition->typesofout[1] = NUMBER; 
	definition->typesofout[2] = NUMBER;
	definition->typesofout[3] = NUMBER;
	definition->typesofin[1] = MATRIX;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"statistic");
	//ADDIP_version = 2.0;

}


void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	
	
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab,int nargout)
{


	C_Image_Container *image;
	C_Matrix_Container *Matr;
	
	
	// variable output has size of numofout (set in Introduce function)
	image = (C_Image_Container*)intab[0]->data;
	Matr= (C_Matrix_Container*)intab[1]->data;
	
	double x1,x2,y1,y2;
	Matr->GetPixel(0,0,x1);
	Matr->GetPixel(0,1,y1);
	Matr->GetPixel(0,2,x2);
	Matr->GetPixel(0,3,y2);
	
	double mini,maks,sred,medi;

	if(x2==0&&y2==0)
	{
		x2=image->_rows;
		y2=image->_cols;
		image->getMinMax(mini,maks);
	}
	else
	{
		double p1=9999999;
		double p2=-9999999;
		double p3;
		for(int i=x1;i<x2;i++)
			for(int j=y1;j<y2;j++)
			{
				image->GetPixel(i,j,p3);
				if(p3<p1)
					p1=p3;
				if(p3>p2)
					p2=p3;
			}
		mini=p1;
		maks=p2;
	}
	double *mask1=new double[(y2-y1)];
	double *mask2=new double[(x2-x1)];
	double pom1=0;
	double suma=0;
	for(int i=x1;i<x2;i++)
	{
		for(int j=y1;j<y2;j++)
		{
			image->GetPixel(i,j,pom1);
			_ASSERT(j-(int)y1<y2-y1 && j-(int)y1>-1);
			mask1[j-(int)y1]=pom1;
			suma=suma+pom1;
		}
		_ASSERT(i-(int)x1<x2-x1 && i-(int)x1>-1);
		mask2[i-(int)x1]=Hoare(mask1,(int)(y2-y1),(int)(y2-y1)/2);
	}
	

	
	//*mini.data=mi;
	//*maks.data=ma;
	sred=suma/((x2-x1)*(y2-y1));
	medi=Hoare(mask2,(int)(x2-x1),(int)((x2-x1)/2));
	
	C_Matrix_Container maks1;
	C_Matrix_Container mini1;
	C_Matrix_Container sred1;
	C_Matrix_Container medi1;

	maks1.AllocateData(1,1);
	mini1.AllocateData(1,1);
	sred1.AllocateData(1,1);
	medi1.AllocateData(1,1);

	maks1.data[0]=maks;
	mini1.data[0]=mini;
	sred1.data[0]=sred;
	medi1.data[0]=medi;

	delete[] mask1;
	delete[] mask2;
	outtab[0]->AssignValue((void*)&maks1,err);
	outtab[1]->AssignValue((void*)&mini1,err);
	outtab[2]->AssignValue((void*)&sred1,err);
	outtab[3]->AssignValue((void*)&medi1,err);
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

