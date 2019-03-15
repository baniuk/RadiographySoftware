#include "StdAfx.h"
#include "C_Command_Ith.h"

C_Command_Ith::C_Command_Ith(void)
{
	UINT parint[2] = {IMAGE,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("ith",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = ith(IMAGE,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Ith::~C_Command_Ith(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Ith::Do(C_Error& err)
{
	C_Image_Container scr;
	C_Image_Container *input;
	C_Image_Container dst;
	C_Matrix_Container *p_ThCoef;
	
	unsigned int i,j,k,x_res,y_res;
	double pixel;
	unsigned int *hist,th,thnew,m1,m2,cm1,cm2;
	double ThCoef;

	input = (C_Image_Container*)intab[0]->data;
	p_ThCoef = (C_Matrix_Container*)intab[1]->data;
	ThCoef = p_ThCoef->data[0];

	input->CloneObject(&scr);
	scr.Normalize(0,255);

	x_res = scr._rows;
	y_res = scr._cols;

	dst.AllocateData(x_res,y_res);
	
	hist = new unsigned int[256];
	
	for(k=0;k<256;k++)*(hist+k)=0;

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

	thnew=56;
	th=0;
	while(th!=thnew)
	{
		m1=0;
		m2=0;
		cm1=0;
		cm2=0;
		th=thnew;
		for(i=0;i<th;i++)
		{
			m1=m1+*(hist+i)*i;
			cm1=cm1+*(hist+i);
		}
		if(cm1==0)cm1=1;
		m1=m1/cm1;

		for(i=th;i<256;i++)
		{
			m2=m2+*(hist+i)*i;
			cm2=cm2+*(hist+i);
		}
		if(cm2==0)cm2=1;
		m2=m2/cm2;

		thnew=(m1+m2)/2;
		
	}

	thnew = (unsigned int)(thnew*ThCoef);
	for(i=0;i<x_res;i++)
		{
			for(j=0;j<y_res;j++)
			{
				
				scr.GetPixel(i,j,pixel);
				if(pixel<=thnew){
					dst.SetPixel(i,j,1);
				}
				else{
					dst.SetPixel(i,j,0);
				}
			}
		}	
	delete hist;
	dst.isBinary = TRUE;

	outtab[0]->AssignValue((void*)&dst,err);
}

