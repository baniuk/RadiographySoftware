#include "StdAfx.h"
#include "C_Command_Roberts.h"

C_Command_Roberts::C_Command_Roberts(void)
{
	UINT parint[2] = {IMAGE,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("roberts",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = roberts(IMAGE,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Roberts::~C_Command_Roberts(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Roberts::Do(C_Error& err)
{
	C_Image_Container* image;
	C_Image_Container image2;
	C_Matrix_Container *p_type;
	
	int type;

	image = (C_Image_Container*)intab[0]->data;
	p_type = (C_Matrix_Container*)intab[1]->data;
	type = (int)p_type->data[0];
	
	unsigned int x_res, y_res;
	x_res = image->_cols;
	y_res = image->_rows;
	
	image2.AllocateData(y_res,x_res);
	// ZMIENNE
	// ZMIENNE
	double zmn, store;
	int k = 0;
	int horz[3][3] = {
						{ 0, 0, 0},
						{-1, 0, 0},
						{ 0, 1, 0}
					};	// Maska horyzontalna
	int vert[3][3] = {
						{ 0,  0,  0},
						{ 0,  0, -1},
						{ 0,  1,  0} 
					};	// Maska wertykalna

	// IMPLEMENTACJA

	if (type == 0)		// HORZ
	{
		for(unsigned int j = 1; j < image->_rows-1; j++)  
		{			
			for(unsigned int i = 1; i < image->_cols-1; i++)
			{
				double grey=0;
				for(unsigned int k = 0; k < 3; k++)
				{
					for(unsigned int l = 0; l < 3; l++)
					{
						image->GetPixel(k+j-1,l+i-1,zmn);
						store = horz[k][l]*zmn;
						grey=grey+store;
					}				
				}
				image2.SetPixel(j,i,grey);
			}
		}
	};

	if (type == 1)		// VERT
	{
		for(unsigned int j = 1; j < image->_rows-1; j++)  
		{			
			for(unsigned int i = 1; i < image->_cols-1; i++)
			{
				double grey=0;
				for(unsigned int k = 0; k < 3; k++)
				{
					for(unsigned int l = 0; l < 3; l++)
					{
						image->GetPixel(k+j-1,l+i-1,zmn);
						store = vert[k][l]*zmn;
						grey=grey+store;
					}				
				}
				image2.SetPixel(j,i,grey);
			}
		}
	}
	image2.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&image2,err);
}
