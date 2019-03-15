#include "StdAfx.h"
#include "C_Command_Eth.h"

C_Command_Eth::C_Command_Eth(void)
{
	UINT parint[6] = {IMAGE,NUMBER,NUMBER,NUMBER,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("eth",6,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = eth(IMAGE,NUMBER,NUMBER,NUMBER,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Eth::~C_Command_Eth(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Eth::Do(C_Error& err)
{
	C_Image_Container *scr;
	C_Image_Container copy;
	C_Image_Container dst;
	C_Matrix_Container *p_GrayScaleLevels;
	C_Matrix_Container *p_ThCoef;
	C_Matrix_Container *p_LargeWindowSize;
	C_Matrix_Container *p_SmallWindowSize;
	C_Matrix_Container *p_meanDiff;
	
	unsigned int i,j,k,l,m,x_res,y_res,index,MSCoef,y_res_temp,x_res_temp;
	double *temp,pixel,P0,P1,sum,u0,u1,*varB,max,thnew,MeanValue;
	unsigned int *hist,th;
	unsigned int LargeWindowSize,SmallWindowSize,GrayScaleLevels;
	double meanDiff, ThCoef;
	
	scr = (C_Image_Container*)intab[0]->data;
	p_LargeWindowSize = (C_Matrix_Container*)intab[1]->data;
	p_SmallWindowSize = (C_Matrix_Container*)intab[2]->data;
	p_meanDiff = (C_Matrix_Container*)intab[3]->data;
	p_ThCoef = (C_Matrix_Container*)intab[4]->data;
	p_GrayScaleLevels = (C_Matrix_Container*)intab[5]->data;

	LargeWindowSize = (unsigned int)p_LargeWindowSize->data[0];
	SmallWindowSize = (unsigned int)p_SmallWindowSize->data[0];
	meanDiff = p_meanDiff->data[0];
	ThCoef = p_ThCoef->data[0];
	GrayScaleLevels = (unsigned int)p_GrayScaleLevels->data[0];



	// Maks must be odd ...then that wroks properly and LargeMaskSize/SmallMaskSize = integer
	MSCoef=(LargeWindowSize - SmallWindowSize)/2;
	// Create new temp tab
	x_res = scr->_rows;
	y_res = scr->_cols;
	x_res_temp = x_res+2*MSCoef;
	y_res_temp = y_res+2*MSCoef;
	
	dst.AllocateData(x_res,y_res);
	scr->CloneObject(&copy);
	copy.Normalize(0,GrayScaleLevels);
	
	temp = new double[x_res_temp*y_res_temp];
	

	// Setting data from src to temp
	// in midle
	for(i=0;i<x_res;i++)
	{
		for(j=0;j<y_res;j++)
		{
			copy.GetPixel(i,j,pixel);
			*(temp + (i+MSCoef)*y_res_temp+(j+MSCoef))= pixel;	
		}

	}
	
	//on left
	for(i=0;i<x_res;i++)
	{
		copy.GetPixel(i,0,pixel);
		for(j=0;j<MSCoef;j++)
		{
			*(temp + (i+MSCoef)*y_res_temp+j)= pixel;	
		}
	}
	
	//on right
	for(i=0;i<x_res;i++)
	{
		copy.GetPixel(i,y_res-1,pixel);	
		for(j=y_res+MSCoef;j<y_res+2*MSCoef;j++)
		{
			*(temp + (i+MSCoef)*y_res_temp+j)= pixel;
		}
	}
	
	//on top
	for(j=0;j<y_res;j++)
	{
		copy.GetPixel(0,j,pixel);
		for(i=0;i<MSCoef;i++)
		{
			*(temp + i*y_res_temp+j+MSCoef)= pixel;
		}
	}
	
	//on bottom
	for(j=0;j<y_res;j++)
	{
		copy.GetPixel(x_res-1,j,pixel);	
		for(i=x_res+MSCoef;i<x_res+2*MSCoef;i++)
		{
			*(temp + i*y_res_temp+j+MSCoef)= pixel;
		}
	}
	
	//on corners
	//left up
	copy.GetPixel(0,0,pixel);	
	for(i=0;i<MSCoef;i++)
	{
		for(j=0;j<MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}
	//right up
	copy.GetPixel(0,y_res-1,pixel);	
	for(i=0;i<MSCoef;i++)
	{
		for(j=y_res+MSCoef;j<y_res+2*MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}
	// left down
	copy.GetPixel(x_res-1,0,pixel);	
	for(i=x_res+MSCoef;i<x_res+2*MSCoef;i++)
	{
		for(j=0;j<MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}
	// right down
	copy.GetPixel(x_res-1,y_res-1,pixel);	
	for(i=x_res+MSCoef;i<x_res+2*MSCoef;i++)
	{
		for(j=y_res+MSCoef;j<y_res+2*MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}


	hist = new unsigned int[GrayScaleLevels];
	varB = new double[GrayScaleLevels];
	


	for(i=0;i<x_res-SmallWindowSize;i=i+SmallWindowSize)
	{
		
		for(j=0;j<y_res-SmallWindowSize;j=j+SmallWindowSize)
		{
//********************************************************************************
			for(k=0;k<GrayScaleLevels;k++)*(hist+k)=0;
			thnew = 0;
			MeanValue = 0;

			// rewriting a fragmen of temp matrix to a vector
			for(k=0;k<LargeWindowSize;k++)
			{
				for(l=0;l<LargeWindowSize;l++)
				{
					//counting histogram of a fragment of matrix
					
					pixel = *(temp+(i+k)*y_res_temp+(j+k));
					MeanValue = MeanValue + *(temp+(i+k)*y_res_temp+(j+k));
					for(m=0;m<GrayScaleLevels;m++)
					{
						if(m==pixel)*(hist+m)=*(hist+m)+1;
				
					}
				}

			}
			//sum of all elements in histogram
			sum=0;
			for(m=0;m<GrayScaleLevels;m++)
			{
				sum = sum + (double)*(hist+m);
		
			}
			// calculating varB vector
			for(k=0;k<GrayScaleLevels;k++)
			{
				P0=0;
				P1=0;
				u0=0;
				u1=0;

				th=k;
				//sum of P0
				for(l=0;l<th;l++)
				{
					P0=P0+(double)*(hist+l);
					u0=u0+l*(*(hist+l)/sum);
				}
				//sum of P1
				for(l=th;l<GrayScaleLevels;l++)
				{
					P1=P1+(double)*(hist+l);
					u1=u1+l*(*(hist+l)/sum);
				}
				P0=P0/sum;
				P1=P1/sum;
				if(P0==0)u0=0;
				else u0=u0/P0;
		
				if(P1==0)u1=0;
				else u1=u1/P1;

				*(varB+k)=P0*P1*((u0-u1)*(u0-u1));
			}

			// getting max of varB vector
			max=*(varB);
			for(k=1;k<GrayScaleLevels;k++)
			{
				if(max<*(varB+k))max=*(varB+k);
				index=k;
			}
			// setting a threshold value
			if(abs(u0-u1)>meanDiff)thnew=(double)(index*256/GrayScaleLevels)*ThCoef; 
			else thnew=MeanValue/(LargeWindowSize*LargeWindowSize)*ThCoef*1.1;
			
			//thnew=(double)(index*256/GrayScaleLevels)*ThCoef;
			pixel = 0;
			for(k=0;k<SmallWindowSize;k++)
			{
				for(l=0;l<SmallWindowSize;l++)
				{
					copy.GetPixel(i+k,j+l,pixel);
					if(pixel<thnew){
 					dst.SetPixel(i+k,j+l,0);
					}
					else{
						dst.SetPixel(i+k,j+l,1);
					}

				}
			}
//********************************************************************************
		}

	}

	delete hist;
	delete varB;
	delete temp;

	dst.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&dst,err);
}

