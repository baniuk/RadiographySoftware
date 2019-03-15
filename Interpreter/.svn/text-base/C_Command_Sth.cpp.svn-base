#include "StdAfx.h"
#include "C_Command_Sth.h"

C_Command_Sth::C_Command_Sth(void)
{
	UINT parint[4] = {IMAGE,NUMBER,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("sth",4,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = sth(IMAGE,NUMBER,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Sth::~C_Command_Sth(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Sth::Do(C_Error& err)
{
	C_Image_Container *scr;
	C_Image_Container dst;
	C_Matrix_Container *p_MaskSize;
	C_Matrix_Container *p_ThCoef;
	C_Matrix_Container *p_VarDynamicRange;
	
	unsigned int MaskSize;
	double ThCoef;
	double VarDynamicRange;
	unsigned int i,j,k,l,x_res,y_res,MSCoef,y_res_temp,x_res_temp;
	double *temp,pixel,ElementNr,MeanValue,stnDvr,alfa,th;
	
	scr = (C_Image_Container*)intab[0]->data;
	p_MaskSize = (C_Matrix_Container*)intab[1]->data;
	p_ThCoef = (C_Matrix_Container*)intab[2]->data;
	p_VarDynamicRange = (C_Matrix_Container*)intab[3]->data;
	MaskSize = (unsigned int)p_MaskSize->data[0];
	ThCoef = p_ThCoef->data[0];
	VarDynamicRange = p_VarDynamicRange->data[0];

		// Maks must be odd ...then that wroks properly and LargeMaskSize/SmallMaskSize = integer
	MSCoef=MaskSize%2;
    MSCoef=(MaskSize-MSCoef)/2;
	// Create new temp tab
	x_res = scr->_rows;
	y_res = scr->_cols;
	x_res_temp = x_res+2*MSCoef;
	y_res_temp = y_res+2*MSCoef;
	
	dst.AllocateData(scr->_rows,scr->_cols);
	
	temp = new double[x_res_temp*y_res_temp];
	
	
	// Setting data from scr to temp
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

			alfa = 1 - stnDvr/(VarDynamicRange);
			
			th = MeanValue*(1-ThCoef*alfa);
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


	delete[] temp;
	dst.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&dst,err);
}
