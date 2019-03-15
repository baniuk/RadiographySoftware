#include "StdAfx.h"
#include "C_Command_Bdilate.h"

C_Command_Bdilate::C_Command_Bdilate(void)
{
	UINT parint[3] = {IMAGE,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("bdilate",3,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = bdilate(IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.1";
}

C_Command_Bdilate::~C_Command_Bdilate(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Bdilate::Do(C_Error& err)
{
	C_Image_Container *imagetmp,*image;
	C_Image_Container image2;
	C_Image_Container image3;
	C_Matrix_Container *p_sizex;
	C_Matrix_Container *p_sizey;
	unsigned int x_res, y_res;
	int size_x, size_y;

	imagetmp = (C_Image_Container*)intab[0]->data;
	p_sizex = (C_Matrix_Container*)intab[1]->data;
	p_sizey = (C_Matrix_Container*)intab[2]->data;
	size_x = (int)p_sizex->data[0];
	size_y = (int)p_sizey->data[0];

	// TWORZENIE NOWEGO OBRAZKA
	x_res = imagetmp->_cols;
	y_res = imagetmp->_rows;
	image2.AllocateData(y_res,x_res);
	imagetmp->CloneObject(&image3);
	image3.Normalize(0,1);
	image = &image3;

	// ZMIENNE
	double zm;
	int k = 0;

	// IMPLEMENTACJA
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{	
			k = 0;
			for (int i = 0; i < size_y; i++)
			{	
				for (int j = 0; j < size_x; j++)
				{
					if((i+m)-((size_x-1)/2)>0)
						if((j+n-((size_y-1)/2))>0)
							if((i+m)-((size_x-1)/2)<image2._cols)
								if((j+n-((size_y-1)/2))<image2._rows)
								{
									image->GetPixel(j+n-(size_x-1)/2,i+m-(size_y-1)/2,zm);
									if (zm == 0)
									{
										k = 1;
//										i = size_y;
//										j = size_x;
									}
									
								}
				}
			}
			if (k == 1)
			{
				image2.SetPixel(n,m,0);
			}
			else
			{
				image2.SetPixel(n,m,1);
			}
		}
	}
	image2.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&image2,err);
}