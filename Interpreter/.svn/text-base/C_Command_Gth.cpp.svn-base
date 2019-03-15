#include "StdAfx.h"
#include "C_Command_Gth.h"

C_Command_Gth::C_Command_Gth(void)
{
	UINT parint[3] = {IMAGE,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("gth",3,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = gth(IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Gth::~C_Command_Gth(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Gth::Do(C_Error& err)
{
	C_Image_Container *scr;
	C_Image_Container dst;
	C_Matrix_Container *p_mask;
	C_Matrix_Container *p_ThCoef;
	
	unsigned int MaskSize;
	double ThCoef;
	unsigned int MSCoef,x_res,y_res,x_res_temp,y_res_temp,i,j;
	double *temp,pixel,Gx,Gy,G,GI,TH;
	
	scr = (C_Image_Container*)intab[0]->data;
	p_mask = (C_Matrix_Container*)intab[1]->data;
	p_ThCoef = (C_Matrix_Container*)intab[2]->data;
	MaskSize = (int)p_mask->data[0];
	ThCoef = p_ThCoef->data[0];

	MSCoef=MaskSize%2;
    MSCoef=(MaskSize-MSCoef)/2;
	// Create new temp tab
	x_res = scr->_rows;
	y_res = scr->_cols;
	x_res_temp = x_res+2*MSCoef;
	y_res_temp = y_res+2*MSCoef;

	dst.AllocateData(x_res,y_res);

	temp = new double[x_res_temp*y_res_temp];
	for(i=0;i<x_res_temp;i++)
	{
		for(j=0;j<y_res_temp;j++)
		{
			*(temp+i*y_res_temp+j)=255*255;
		}
	}
	
	
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

	// couting Gradient value
	G=0;
	GI=0;
	TH=0;
	for(i=0;i<x_res;i++)
	{
		for(j=0;j<y_res;j++)
		{
			Gx=*(temp+(i+2*MSCoef)*y_res_temp+j+MSCoef)-*(temp+(i)*y_res_temp+j+MSCoef);
			Gy=*(temp+(i+MSCoef)*y_res_temp+j+2*MSCoef)-*(temp+(i+MSCoef)*y_res_temp+j);
			if(Gx>Gy){ 
				G=G+Gx;
				scr->GetPixel(i,j,pixel);
				GI=GI+Gx*pixel;

			}
			else if(Gx<=Gy){
				G=G+Gy;
				scr->GetPixel(i,j,pixel);
				GI=GI+Gy*pixel;

			}
		}
	}
	TH=(GI/G);
	TH=ThCoef*TH;
	for(i=0;i<x_res;i++)
	{
		for(j=0;j<y_res;j++)
		{
			
			scr->GetPixel(i,j,pixel);
			if(pixel<=TH){
				dst.SetPixel(i,j,1);
			}
			else{
				dst.SetPixel(i,j,0);
			}
		}
	}

	delete temp;

	dst.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&dst,err);
}
