#include "StdAfx.h"
#include "C_Command_Th.h"

C_Command_Th::C_Command_Th(void)
{
	UINT parint[3] = {IMAGE,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("ath",3,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = ath(IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Th::~C_Command_Th(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Th::Do(C_Error& err)
{
	C_Image_Container *src;
	C_Image_Container dest;
	C_Matrix_Container *p_mask;
	C_Matrix_Container *p_ThCoef;
	
	unsigned int mask;
	double ThCoef;
	unsigned int MsCoef,ElementNr;
	double MeanValue;
	int x1,x2,y1,y2,x,y,i,j,wx1,wx2,wy1,wy2,x_res,y_res;
	
	src = (C_Image_Container*)intab[0]->data;
	p_mask = (C_Matrix_Container*)intab[1]->data;
	p_ThCoef = (C_Matrix_Container*)intab[2]->data;
	mask = (int)p_mask->data[0];
	ThCoef = p_ThCoef->data[0];
	x_res = (int)src->_rows;
	y_res = (int)src->_cols;

	dest.AllocateData(src->_rows,src->_cols);

	i=0;
	j=0;
	x=0;
	y=0;
	MsCoef=mask%2;
    MsCoef=(mask-MsCoef)/2;
 	for(x=0;x<x_res;x++)
    {//L1
        for(y=0;y<y_res;y++)
        {//L2
			MeanValue=0;
            ElementNr=0;
			x1=x-MsCoef;
            x2=x+MsCoef;
            y1=y-MsCoef;
            y2=y+MsCoef;
			
			// Counting numbers of elements in a moving mask
            if(x1<0){wx1=x1;}
            else    {wx1=0;}
            
            if(y1<0){wy1=y1;}
            else    {wy1=0;}
            
            if((x_res-x2-1)<0){wx2=x_res-x2-1;}
            else    {wx2=0;}
            
            if((y_res-y2-1)<0){wy2=y_res-y2-1;}
            else    {wy2=0;}
			
			

			ElementNr = mask*mask+(wx1+wx2+wy1+wy2)*mask+(wx1*wy1  +  wx1*wy2  +  wx2*wy1  +  wx2*wy2);
			
			
			//Cheanging size of mask in a edge of Image
            

			
                    if(x1<0){x1=0;}
                    if(y1<0){y1=0;}
                    if(x2>x_res-1){x2=x_res-1;}
                    if(y2>y_res-1){y2=y_res-1;}  
			
			for(i=x1;i<x2+1;i++)
			{for(j=y1;j<y2+1;j++)
                { 
					MeanValue = MeanValue + *(src->data + i*y_res+j);
				}
              }
			MeanValue = MeanValue / ElementNr;
			
			if(*(src->data + x*y_res + y)<= MeanValue - ThCoef)
            {
                *(dest.data + x*y_res + y)=1;
            }
            else
            {
                *(dest.data + x*y_res + y)=0;
            } 
   	    }//L2
    }//L1
	dest.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&dest,err);
}