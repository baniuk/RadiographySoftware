#include "StdAfx.h"
#include "C_Command_Oth.h"

C_Command_Oth::C_Command_Oth(void)
{
	UINT parint[2] = {IMAGE,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("oth",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = oth(IMAGE,NUMBER)";
	fun_ver = "1.1";
}

C_Command_Oth::~C_Command_Oth(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Oth::Do(C_Error& err)
{
	C_Image_Container scr;
	C_Image_Container *input;
	C_Image_Container dst;
	C_Matrix_Container *p_ThCoef;
	C_Matrix_Container *p_GrayLvL;
	
	unsigned int i,j,k,x_res,y_res,index,GrayLvL;
	double pixel,P0,P1,sum,u0,u1,*varB,max,thnew,ThCoef;
	unsigned int *hist,th;

	input = (C_Image_Container*)intab[0]->data;
	p_ThCoef = (C_Matrix_Container*)intab[1]->data;
	p_GrayLvL = (C_Matrix_Container*)intab[1]->data;
	ThCoef = p_ThCoef->data[0];
//	GrayLvL = (int)p_GrayLvL->data[0];
	GrayLvL = 0;

	input->CloneObject(&scr);
	scr.Normalize(0,255);

	x_res = scr._rows;
	y_res = scr._cols;
	
	dst.AllocateData(x_res,y_res);

	hist = new unsigned int[256];
	varB = new double[256];
	
	for(k=0;k<256;k++)
		*(hist+k)=0;

	for(i=0;i<x_res;i++)
	{
		for(j=0;j<y_res;j++)
		{
			scr.GetPixel(i,j,pixel);
 		
			for(k=0;k<256;k++)
			{
				if(k==pixel)*(hist+k)=*(hist+k)+1;
				
			}
		}

	}
	
	//sum of all elements in histogram
	sum=0;
	for(i=0;i<256;i++)
	{
		sum = sum + (double)*(hist+i);
		
	}
	
	// calculating varB vector
	for(i=0;i<256;i++)
	{
		P0=0;
		P1=0;
		u0=0;
		u1=0;

		th=i;
		//sum of P0
		for(j=0;j<th;j++)
		{
			P0=P0+(double)*(hist+j);
			u0=u0+j*(*(hist+j)/sum);
		}
		
		//sum of P1
		for(j=th;j<256;j++)
		{
			P1=P1+(double)*(hist+j);
			u1=u1+j*(*(hist+j)/sum);
		}
		P0=P0/sum;
		P1=P1/sum;

		if(P0==0)u0=0;
		else u0=u0/P0;
		
		if(P1==0)u1=0;
		else u1=u1/P1;

		*(varB+i)=P0*P1*((u0-u1)*(u0-u1));
		
	}

	// getting max of varB vector
	max=*(varB);
	for(k=1;k<256;k++)
	{
		
		if(max<*(varB+k))max=*(varB+k);
		index=k;
	}
	
	thnew=(double)index*ThCoef; 
	
	pixel=0;
	double minV,maxV;
	scr.getMinMax(minV,maxV);

	for(i=0;i<x_res;i++)
		{
			for(j=0;j<y_res;j++)
			{
				
				scr.GetPixel(i,j,pixel);
				if(pixel<thnew){
 					if(GrayLvL>2)	{
						pixel=GrayLvL-ceil(((thnew-pixel)/(thnew-minV))*(GrayLvL-1));
						dst.SetPixel(i,j,pixel*(1/(GrayLvL-1)));
					}
					else dst.SetPixel(i,j,1);
				}
				else
					dst.SetPixel(i,j,0);
			}
		}

	
	delete hist;
	delete varB;
	dst.isBinary = TRUE;

	outtab[0]->AssignValue((void*)&dst,err);
}


