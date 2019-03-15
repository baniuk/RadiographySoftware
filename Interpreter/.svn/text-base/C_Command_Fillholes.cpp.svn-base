#include "StdAfx.h"
#include "C_Command_Fillholes.h"

C_Command_Fillholes::C_Command_Fillholes(void)
{
	UINT parint[3] = {IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("fillholes",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = fillholes(IMAGE)";
	fun_ver = "1.0";
}

C_Command_Fillholes::~C_Command_Fillholes(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Fillholes::Do(C_Error& err)
{
	C_Image_Container *image;
	C_Image_Container image2;
	double zm , max, sklejka[1000], pom, obecne, pomoc, dod, dod2;
	double war1, war2, war3, war4, war5, war6, war7, val1, pole, maksi;
	unsigned int i, m, n, x_res, y_res, licz, licz1, id;
	int war;
		
	image = (C_Image_Container*)intab[0]->data;
	image2.AllocateData(image->_rows,image->_cols);

	obecne = 1;
	licz = licz1 = id = 0;
	zm = max = pomoc = dod = dod2 = pole = maksi = 0;
	x_res = image->_cols;
	y_res = image->_rows;

	C_Image_Container imtemp;
	image->CloneObject(&imtemp);

// ======== NEGATYW ==================================================== //
	for (m = 1; m < image->_rows; m++)
	{
		for (n = 1; n < image->_cols; n++)
		{
			imtemp.GetPixel(m,n,zm);
			if(zm == 0)
				imtemp.SetPixel(m,n,1);
			else
				imtemp.SetPixel(m,n,0);
		}
	}
	// ===================================================================== //

	// ========= INDEKSACJA ================================================ //
	C_Image_Container indeks;
	indeks.AllocateData(y_res,x_res);

	for (m = 0; m < image->_rows; m++)
		for (n = 0; n < image->_cols; n++)
		{
			imtemp.GetPixel(m,n,pom);
			indeks.SetPixel(m,n,pom);
		}

	for(m = 1; m < image->_rows; m++)
		for(n = 1; n < indeks._cols; n++)
		{
			imtemp.GetPixel(m,n,war1);
			if(war1 == 1)
			{
				indeks.GetPixel(m-1, n-1, war2);
				indeks.GetPixel(m-1, n  , war3);
				indeks.GetPixel(m-1, n+1, war4);
				indeks.GetPixel(m  , n-1, war5);
				if( (war2 == 0)&&(war3 == 0)&&(war4 == 0)&&(war5 == 0) )
				{
					licz = licz+1;
					indeks.SetPixel(m,n,licz);
					sklejka[licz] = licz+1;
				}
				else
				{
					war6 = 0;
					war7 = 0;
					
					if(war2 != 0)
						war6 = war2;
					if(war3 != 0)
						war6 = war3;
					if(war4 != 0)
						war6 = war4;
					if(war5 != 0)
						war6 = war5;
					if( (war2 != 0)&&(war2 != war6) )
						war7 = war2;
					if( (war3 != 0)&&(war3 != war6) )
						war7 = war3;
					if( (war4 != 0)&&(war4 != war6) )
						war7 = war4;
					if( (war5 != 0)&&(war5 != war6) )
						war7 = war5;
					if(war7 == 0)
						indeks.SetPixel(m,n,war6);
					else
					{
						indeks.SetPixel(m,n,min(war6, war7));
						sklejka[(int)max(war6, war7)] = min(war6, war7);
					}

				}
			}
		}
		for(i=1; i < licz; i++)
		{
			war = 1+licz-i;
			for(m = 0; m < image->_rows; m++)
				for(n = 0; n < image->_cols; n++)
				{
					double war1;
					indeks.GetPixel(m,n,war1);
					if(war1 == war)
					{
						pom = sklejka[war];
						indeks.SetPixel(m,n,pom);
					}
				}
		}
	
	double warmax,warmin;
	indeks.getMinMax(warmin,warmax);

	for(int pom = 1; pom <= warmax; pom++)
	{
		for(m = 1; m < image->_rows; m++)
			for(n = 1; n < image->_cols; n++)
			{
				indeks.GetPixel(m,n,val1);
				if(val1 == pom)
				{
					indeks.SetPixel(m,n,obecne);
					licz1 = licz1+1;
				}
			}
		if(licz1 > 0)
			obecne = obecne+1;
	}
	// ===================================================================== //

	// ========= OBLICZANIE PÓL OBIEKTÓW =================================== //
	double *tabl = new double[obecne+1];

	for (i = 0; i <= obecne; i++)
	{
		for (m = 0; m < image->_cols; m++)
		{
			for (n = 0; n < image->_rows; n++)
			{
				indeks.GetPixel(n,m,dod);
				if (dod == i+1)
				{
					tabl[i+1]=pole;
					pole++;
				}
			}
		}
		pole = 0;
	}
	// ===================================================================== //

	// ========= SZUKANIE NAJWIÊKSZEGO OBIEKTU (t³a) ======================= //
	for (i = 0; i <= obecne; i++)
	{
		if (tabl[i] > maksi)
		{
			maksi = tabl[i];
			id = i;
		}
	}
	// ===================================================================== //

	// ========= ZEROWANIE NAJWIÊKSZEGO OBIEKTU (t³a) ====================== //
	for (m = 0; m < image->_cols; m++)
	{
		for (n = 0; n < image->_rows; n++)
		{
			indeks.GetPixel(n,m,dod);
			if (dod == id)
				indeks.SetPixel(n,m,0);
			else
				indeks.SetPixel(n,m,1);
		}
	}
	// ===================================================================== //
	
	// ========= WYPE£NIANIE OTWORÓW ======================================= //
	for (m = 0; m < image->_cols; m++)
	{
		for (n = 0; n < image->_rows; n++)
		{
			imtemp.GetPixel(n,m,dod);
			indeks.GetPixel(n,m,dod2);
			if ( (dod + dod2) == 0.0)
				indeks.SetPixel(n,m,1);
		}
	}
	indeks.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&indeks,err);
	delete[] tabl;
}
