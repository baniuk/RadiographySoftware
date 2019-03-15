#include "StdAfx.h"
#include "C_Command_Dilate.h"

C_Command_Dilate::C_Command_Dilate(void)
{
	UINT parint[3] = {IMAGE,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("dilate",3,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = dilate(IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Dilate::~C_Command_Dilate(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Dilate::Do(C_Error& err)
{
	C_Image_Container *image;
	C_Image_Container image2;
	C_Matrix_Container *p_sizex;
	C_Matrix_Container *p_sizey;
	unsigned int size_x, size_y;
	double max;
	double zm;
	int k = 0;
	
	image = (C_Image_Container*)intab[0]->data;
	p_sizex = (C_Matrix_Container*)intab[1]->data;
	p_sizey = (C_Matrix_Container*)intab[2]->data;
	size_x = (int)p_sizex->data[0];
	size_y = (int)p_sizey->data[0];

//	x_res = image->_rows;
//	y_res = image->_cols;
	image2.AllocateData(image->_rows,image->_cols);

	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{	
			max = 0;
			for (unsigned int i = 0; i < size_y; i++)
			{	
				for (unsigned int j = 0; j < size_x; j++)
				{
					if((i+m)-((size_x-1)/2)>0)
						if((j+n-((size_y-1)/2))>0)
							if((i+m)-((size_x-1)/2)<image2._cols)
								if((j+n-((size_y-1)/2))<image2._rows)
								{
									image->GetPixel(j+n-(size_x-1)/2,i+m-(size_y-1)/2,zm);
									if (zm > max)
									{
										max = zm;
									}
									k++;
								}
				}
			}
			k = 0;
			image2.SetPixel(n,m,max);
		}
	}
	outtab[0]->AssignValue((void*)&image2,err);
}
