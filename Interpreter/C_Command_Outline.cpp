#include "StdAfx.h"
#include "C_Command_Outline.h"

C_Command_Outline::C_Command_Outline(void)
{
	UINT parint[1] = {IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("outline",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = outline(IMAGE)";
	fun_ver = "1.1";
}

C_Command_Outline::~C_Command_Outline(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Outline::Do(C_Error& err)
{
	C_Image_Container *imagetmp,*image;
	C_Image_Container image3;
	C_Image_Container image2;
	C_Image_Container image4;
	unsigned int x_res, y_res;
	double zm = 0;
	double zm1 = 0, zm2 = 0;
	int k = 0;
		
	imagetmp = (C_Image_Container*)intab[0]->data;
	
// POMOCNICZE OBRAZKI

	x_res = imagetmp->_cols;
	y_res = imagetmp->_rows;

	// DO EROZJI
	image2.AllocateData(y_res,x_res);
	
	// WYNIKOWY
	image3.AllocateData(y_res,x_res);
	imagetmp->CloneObject(&image4);
	image4.Normalize(0,1);
	image = &image4;

	// IMPLEMENTACJA EROZJI
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{	
			k = 0;
			for (unsigned int i = 0; i < 3; i++)
			{	
				for (unsigned int j = 0; j < 3; j++)
				{
					if((i+m-1)>0)
						if((j+n-1)>0)
							if(((i+m)-1)<image2._cols)
								if(((j+n)-1)<image2._rows)
								{
									image->GetPixel(j+n-1,i+m-1,zm);
									if (zm != 0)
									{
										k = 1;
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
	
	// OUTLINE
	
	for (unsigned int m = 0; m < (image3._cols); m++)
	{
		for (unsigned int n = 0; n < (image3._rows); n++)
		{
			image->GetPixel(n,m,zm1);
			image2.GetPixel(n,m,zm2);
			if (zm1-zm2 == 0)
			{
				image3.SetPixel(n,m,0);
			}
			else
			{
				image3.SetPixel(n,m,1);
			}
		}
	}
//	image3.Normalize(0,65535);
//	double min,max;
//	image3.getMinMax(min,max);
	image3.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&image3,err);
}

