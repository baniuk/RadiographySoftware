#include "StdAfx.h"
#include "C_Command_Nth.h"

C_Command_Nth::C_Command_Nth(void)
{
	UINT parint[3] = {IMAGE,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("nth",3,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = nth(IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Nth::~C_Command_Nth(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Nth::Do(C_Error& err)
{
	C_Image_Container* scr;
	C_Image_Container dst;
	C_Matrix_Container *p_ThCoef;
	C_Matrix_Container *p_MaskSize;
	
	unsigned int i,j,k,l,x_res,y_res,MSCoef,y_res_temp,x_res_temp;
	double *temp,pixel,ElementNr,MeanValue,stnDvr,th;
	unsigned int MaskSize;
	double ThCoef;

	scr = (C_Image_Container*)intab[0]->data;
	p_MaskSize = (C_Matrix_Container*)intab[1]->data;
	p_ThCoef = (C_Matrix_Container*)intab[2]->data;
	MaskSize = (unsigned int)p_MaskSize->data[0];
	ThCoef = p_ThCoef->data[0];

	// Maks must be odd ...then that wroks properly and LargeMaskSize/SmallMaskSize = integer
	MSCoef=MaskSize%2;
    MSCoef=(MaskSize-MSCoef)/2;
	// Create new temp tab
	x_res = scr->_rows;
	y_res = scr->_cols;
	x_res_temp = x_res+2*MSCoef;
	y_res_temp = y_res+2*MSCoef;
	
	dst.AllocateData(x_res,y_res);
	
	temp = new double[x_res_temp*y_res_temp];
	
	
	// Setting data from src to temp
	// in midle
	for(i=0;i<x_res;i++)
	{
		for(j=0;j<y_res;j++)
		{
			scr->GetPixel(i,j,pixel);
			*(temp + (i+MSCoef)*y_res_temp+(j+MSCoef))= pixel;	
		}

	}
	
	//on left
	for(i=0;i<x_res;i++)
	{
		scr->GetPixel(i,0,pixel);
		for(j=0;j<MSCoef;j++)
		{
			*(temp + (i+MSCoef)*y_res_temp+j)= pixel;	
		}
	}
	
	//on right
	for(i=0;i<x_res;i++)
	{
		scr->GetPixel(i,y_res-1,pixel);	
		for(j=y_res+MSCoef;j<y_res+2*MSCoef;j++)
		{
			*(temp + (i+MSCoef)*y_res_temp+j)= pixel;
		}
	}
	
	//on top
	for(j=0;j<y_res;j++)
	{
		scr->GetPixel(0,j,pixel);
		for(i=0;i<MSCoef;i++)
		{
			*(temp + i*y_res_temp+j+MSCoef)= pixel;
		}
	}
	
	//on bottom
	for(j=0;j<y_res;j++)
	{
		scr->GetPixel(x_res-1,j,pixel);	
		for(i=x_res+MSCoef;i<x_res+2*MSCoef;i++)
		{
			*(temp + i*y_res_temp+j+MSCoef)= pixel;
		}
	}
	
	//on corners
	//left up
	scr->GetPixel(0,0,pixel);	
	for(i=0;i<MSCoef;i++)
	{
		for(j=0;j<MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}
	//right up
	scr->GetPixel(0,y_res-1,pixel);	
	for(i=0;i<MSCoef;i++)
	{
		for(j=y_res+MSCoef;j<y_res+2*MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}
	// left down
	scr->GetPixel(x_res-1,0,pixel);	
	for(i=x_res+MSCoef;i<x_res+2*MSCoef;i++)
	{
		for(j=0;j<MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}
	// right down
	scr->GetPixel(x_res-1,y_res-1,pixel);	
	for(i=x_res+MSCoef;i<x_res+2*MSCoef;i++)
	{
		for(j=y_res+MSCoef;j<y_res+2*MSCoef;j++)
		{
			*(temp + i*y_res_temp+j)= pixel;
		}
	}
	

	
	ElementNr = MaskSize*MaskSize;
	for(i=0;i<x_res;i++)
	{
		for(j=0;j<y_res;j++)
		{
			// getting the lacal mean value
			MeanValue = 0;
			for(k=0;k<MaskSize;k++)
			{
				for(l=0;l<MaskSize;l++)
				{
					
					MeanValue = MeanValue + *(temp+(i+k)*y_res_temp+j+l);
				}

			}
			MeanValue = MeanValue / ElementNr;	
			stnDvr = 0;
			for(k=0;k<MaskSize;k++)
			{
				for(l=0;l<MaskSize;l++)
				{
					stnDvr = stnDvr + pow(*(temp+(i+k)*y_res_temp+j+l)-MeanValue,2);

				}

			}

			stnDvr = sqrt(stnDvr/(ElementNr-1));

			th = MeanValue + ThCoef*stnDvr;
			scr->GetPixel(i,j,pixel);
			if(pixel<th)
			{
 				dst.SetPixel(i,j,1);
			}
			else
			{
				dst.SetPixel(i,j,0);
			}

		}
	}


	delete temp;
	dst.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&dst,err);
}

