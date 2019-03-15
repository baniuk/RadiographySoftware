// Load_Disp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
//#include <fstream.h>
#include "math.h"
#include "conio.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "C_Image.h"
#include "ANN_filter.h"


void ANN_transform(C_Image* img,  C_Image* dest, char ANN_filename[]);
void imageshow(char *WinName,IplImage *img);


int _tmain(int argc, _TCHAR* argv[])
{
	IplImage *img;
	CvSize size;

		
	
// -------------------------------------------------------------------------------------------------		
// W TYM MIEJSCU WSTAWIAMY WŁASNY KOD
// PAMIĘTAMY ŻE PISZEMY FUNKCJĘ KTÓRA POBIERA I ZWRACA WSKAŹNIKI DO KLASY C_Image TAK JAK BYŁO POPRZEDNIO USTALANE
// Mamy do dyspozycji obiekt klasy C_Image zawierający podstawowe funkcje od obsługi obrazka oraz sam obrazek - szczegóły w pliku C_Image.h

// Przykłady:	
	//Wczytanie tiffa
	C_Image image, *pimage;
	pimage = &image;
	if(!image.ReadTiff("c:\\TestSample_2.tif")) //"c:\\obraztestowy2_SMALL.tif"
		return -1;
	//image.Normalize(0,65535);

// kopiowanie obrazków
	C_Image destination, *pdestination;	// tu będzie kopia
	pdestination = &destination;
	image.CloneObject(&destination);	// skopiowanie obrazka image do destination

// ----- Wyświetlanie za pomocą CV ----------------------------------------------
	size.height = image._rows;
	size.width = image._cols;
	img = cvCreateImage(size, IPL_DEPTH_16U, 1);
	img->imageData = image.ReturnIPLBuffor();
	imageshow("Image",img);
	cvWaitKey();

//// pobranie pixela
//	double pixel;image.GetPixel(100, 100, pixel);
//	fprintf(stderr, "Image pixel value %f", pixel);
	;
	
// wczytanie danych sieci
	char filename[] = "crossMask_33x33_ANN_ff_14_7_1.ann";

// przetwarzanie
	ANN_transform(pimage, pdestination ,filename);
	
	// Przyk│ady:	
/*	Wczytanie tiffa
	C_Image image;
	image.ReadTiff("c:\\test.tif");

// nagranie do pliku - format bin - do obs│ugi w matlabie
	image.saveimage2matlab("c:\\test.bin");		
	
// pobranie pixela
	unsigned int pixel;
	image.GetPixel(100, 800, pixel);

// ustawienie pixela
	image.SetPixel(100, 800, 10);

// tworzenie nowego obrazka (nie tiffa)
	C_Image nowy_obraz();
	unsigned int x_res, y_res;
	x_res = 600;
	y_res = 800;
	nowy_obraz.AllocateData(y_res,x_res);

// kopiowanie obrazkˇw
	C_Image destination;		// tu bŕdzie kopia
	image.CloneImage(&destination);		// skopiowanie obrazka image do destination
	
// szukanie minimum i maksimum na obrazku
	double min, max;
	image.getMinMax(min,max);	% w zmiennych min i maks pojawi╣ siŕ wartoťci minimalne i maksymalne

// Skalowanie obrazka
	image.Normalize(0,65535);	% normalizacja w zakresie mp. 0 - 65535

	void MojaFunkcja(C_Image* src, C_Image* dst)
	{
		src->Normalize(0,65535); % !!! Tu modyfikujemy obrazek ┐rˇd│owy co jest niebezpieczne gdy┐ nie wiemy czy czasem nie 	wp│ynie 			to na dzia│anie innych funkcji
		% powinno byŠ tak:
		C_Image kopia;	%
		src->CloneObject(&kopia);	% tworzymy kopiŕ lokaln╣ obrazka
		kopia.Normalize(0,65535);	% spokojnie sobie j╣ modyfikujemy

		kopia.CloneObject(dst);	% kopiujemy wynik do wyjťcia
	}

*/

// ----- Wyświetlanie za pomocą CV ----------------------------------------------

	size.height = image._rows;
	size.width = image._cols;
	img = cvCreateImage(size, IPL_DEPTH_16U, 1);
	img->imageData = destination.ReturnIPLBuffor();
	imageshow("Image",img);
	cvWaitKey();
	
//------- Sprzątanie -----------------------------------------------------------	
	cvDestroyWindow("Image");
	cvReleaseImage(&img);
	return 0;
}

void imageshow(char *WinName,IplImage *img)
{
	cvNamedWindow(WinName,1);
	cvShowImage(WinName,img);
}




//Przemek ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


void ANN_transform(C_Image* img, C_Image* dest, char ANN_filename[])
{
		double In[40000], *pIn;
		pIn = &In[0];
		double Out, *pOut;
		pOut = &Out;
		ANN_params *pANN_set;
		pANN_set = &ANN_set;
		int height,width;
		double *data;
		int i,j,k;
		double pict_depth=65535;

		// ANN file loading	
		ANN_loading(ANN_filename, pANN_set);
		printf("\n Processing... \n");
	
		// acces to active bits of the mask 
		ANN_mask_acces(pANN_set);

		int mask_xRadius = ceil(1.0*ANN_set.maskSize.cols/2)+1;
		int mask_yRadius = ceil(1.0*ANN_set.maskSize.rows/2)+1;

		
		// get the image data
		height = dest->_rows;
		width = dest->_cols;
		data = (double *)img->_data;

	
		// process the image
		for(i=0;i<height;i++) for(j=0;j<width;j++) 
		{
			if(i<mask_xRadius || i>(height-mask_xRadius-1) || j<mask_yRadius || j>(width-mask_yRadius-1))
			{
				dest->SetPixel(i, j, 0);// set to black
			}
			else
			{
				for (int h=0; h<ANN_set.size_acces; h++)
				{
					In[h] = data[(i+ANN_set.yAcces[h])*width+(j+ANN_set.xAcces[h])]*1.0/(pict_depth);
				}
				ANN_singlePointEstim(pIn,ANN_set,pOut);
				dest->SetPixel(i, j, *pOut*pict_depth);
			}
		}
}

void ANN_mask_acces(ANN_params *ANN_set)
{
	ANN_set->size_acces = 0;
	int xCenterPos = ceil(1.0*ANN_set->maskSize.cols/2)-1;
	int yCenterPos = ceil(1.0*ANN_set->maskSize.rows/2)-1;

	for (int i = 0; i < ANN_set->maskSize.rows; i++)
        for (int j = 0; j < ANN_set->maskSize.cols; j++) 
		  {
			if (*(ANN_set->mask+ i * ANN_set->maskSize.cols +j) == 1)
			{
				ANN_set->xAcces[ANN_set->size_acces] = j - xCenterPos;
				ANN_set->yAcces[ANN_set->size_acces] = i - yCenterPos;
				ANN_set->size_acces++;
			}
        }
}
void ANN_singlePointEstim(double *pIn,ANN_params ANN_set, double *pOut )
{
	double Z1[600], *pZ1 = Z1;
	double L1_p[600], *pL1_p = L1_p;
	double L1_out[600], *pL1_out = L1_out;
	double Z2[600], *pZ2 = Z2;
	double L2_p[600], *pL2_p = L2_p;
	double L2_out[600], *pL2_out = L2_out;
	double Z3[600], *pZ3 = Z3;
	double L3_p[600], *pL3_p = L3_p;
	double L3_out[600], *pL3_out = L3_out;

	//////////////////////////////////////////////////////////////////////////////////////////
	// PROCESSING
	//////////////////////////////////////////////////////////////////////////////////////////

	// Processing by layer 1
	mult_Matrix(ANN_set.W1,  pIn,  ANN_set.sizeW1, ANN_set.sizeX, pZ1);
	sum_Matrix( pZ1,  ANN_set.B1, ANN_set.sizeZ1, pL1_p);
	exec_TrFun(pL1_p, ANN_set.sizeL1_p, ANN_set.type_TRF[0], pL1_out);

	// Processing by layer 2
	mult_Matrix(ANN_set.W2,  pL1_out,  ANN_set.sizeW2, ANN_set.sizeL2_p, pZ2);
	sum_Matrix( pZ2,  ANN_set.B2, ANN_set.sizeZ2, pL2_p);
	exec_TrFun(pL2_p, ANN_set.sizeL2_p, ANN_set.type_TRF[1], pL2_out);

	// Processing by layer 3
	mult_Matrix(ANN_set.W3,  pL2_out,  ANN_set.sizeW3, ANN_set.sizeL3_p, pZ3);
	sum_Matrix( pZ3,  ANN_set.B3, ANN_set.sizeZ3, pL3_p);
	exec_TrFun(pL3_p, ANN_set.sizeL3_p, ANN_set.type_TRF[2], pL3_out);

	*pOut = *pL3_out;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// END OF PROCESSING
	//////////////////////////////////////////////////////////////////////////////////////////
};

double neuron_TRF(double x, int type_TRF)
{
	double out;
	switch(type_TRF)
	{
		case 1:  out= 1.0/(1 + exp(-x)); break;	//logsig_TRF
		case 3:  out= x; break;					//linear_TRF
	};
	return out;
};

void mult_Matrix(double *X, double *Y, W_B_size size_X, W_B_size size_Y, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_Y.cols; j++) 
		{
            *(Z+i*size_Y.cols+j) = 0;
            for (int k = 0; k < size_X.cols; k++)
                *(Z+i*size_Y.cols+j) += *(X+i*size_X.cols+k) * *(Y+k*size_Y.cols+j);
        }
}

void sum_Matrix(double *X, double *Y, W_B_size size_X, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_X.cols; j++) 
		{
            *(Z+i*size_X.cols+j) = *(X+i*size_X.cols+j) + *(Y+i*size_X.cols+j);
		}
}


void exec_TrFun(double *pL1_p, W_B_size sizeL1_p, int type_TRF, double *pL1_out)
{
	
	for (int i = 0; i < sizeL1_p.rows; i++)
        for (int j = 0; j < sizeL1_p.cols; j++) 
		{
            *(pL1_out+i*sizeL1_p.cols+j) = neuron_TRF( *(pL1_p+i*sizeL1_p.cols+j), type_TRF);
		}
	
}


void ANN_loading(char ANN_filename[], ANN_params *ANN_set)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// open file and get the weights and biases
	//////////////////////////////////////////////////////////////////////////////////////////
	FILE *infile;
	char buffer[30];
	if ((fopen_s(&infile,ANN_filename,"r")) != NULL)	
	{
		printf("\n No file or file corrupted\n");
		_getch();
		exit(0);
	}
		//net
		fgets(buffer,30,infile); 
		int index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->net_Sch[index_loop++] = atoi(buffer);}
		}
		// end of net


		//transfer function
		// 1-logsig,   3-linear
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile); 	
			if (buffer[0] == 'E') break;
			else 
			{
				if (strstr(buffer,"logsig")) ANN_set->type_TRF[index_loop++] = 1;
				if (strstr(buffer,"linear")) ANN_set->type_TRF[index_loop++] = 3;
			}
		}
		// end of transfer function


		///////////// LAYER 1 //////////////

		//W1
		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->W1[index_loop++] = atof(buffer);}
		}
		// end of W1

		//B1
		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->B1[index_loop++] = atof(buffer);}
		}
		// end of B1

		//sizeW1
		fgets(buffer,30,infile);
		index_loop = 0; 
		int temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->sizeW1.rows = temp[0]; ANN_set->sizeW1.cols = temp[1];
		}
		// end of sizeW1

		//sizeB1
		fgets(buffer,30,infile);
		index_loop = 0; 
		temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->sizeB1.rows = temp[0]; ANN_set->sizeB1.cols = temp[1];
		}
		// end of sizeB1


		///////////// LAYER 2 //////////////

		//W2
		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->W2[index_loop++] = atof(buffer);}
		}	
		// end of W2

		//B2

		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->B2[index_loop++] = atof(buffer);}
		}
		// end of B2

		//sizeW2
		fgets(buffer,30,infile);
		index_loop = 0; 
		temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->sizeW2.rows = temp[0]; ANN_set->sizeW2.cols = temp[1];
		}
		// end of sizeW2

		//sizeB2
		fgets(buffer,30,infile);
		index_loop = 0; 
		temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->sizeB2.rows = temp[0]; ANN_set->sizeB2.cols = temp[1];
		}
		// end of sizeB2


		///////////// LAYER 3 //////////////

		//W3
		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->W3[index_loop++] = atof(buffer);}
		}
		// end of W3

		//B3
		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->B3[index_loop++] = atof(buffer);}
		}
		// end of B3

		//sizeW3
		fgets(buffer,30,infile);
		index_loop = 0; 
		temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->sizeW3.rows = temp[0]; ANN_set->sizeW3.cols = temp[1];
		}
		// end of sizeW3

		//sizeB3
		fgets(buffer,30,infile);
		index_loop = 0; 
		temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->sizeB3.rows = temp[0]; ANN_set->sizeB3.cols = temp[1];
		}
		// end of sizeB3


		///////////// INPUT //////////////

		//sizeX
		fgets(buffer,30,infile);
		index_loop = 0; 
		temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->sizeX.rows = temp[0]; ANN_set->sizeX.cols = temp[1];
		}
		// end of sizeX

		//mask
		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->mask[index_loop++] = atof(buffer);}
		}
		// end of mask

		//maskOnes
		fgets(buffer,30,infile);
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->maskOnes[index_loop++] = atoi(buffer);}
		}
		// end of maskOnes

		//maskSize
		fgets(buffer,30,infile);
		index_loop = 0; 
		temp[2];
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {temp[index_loop++] = atoi(buffer);}
			ANN_set->maskSize.rows = temp[0]; ANN_set->maskSize.cols = temp[1];
		}
		// end of maskSize

	fclose(infile);

	//////////////////////////////////////////////////////////////////////////////////////////
	// end of data loading
	//////////////////////////////////////////////////////////////////////////////////////////


	ANN_set->sizeZ1.rows = ANN_set->sizeW1.rows;				ANN_set->sizeZ1.cols = ANN_set->sizeX.cols; 
	ANN_set->sizeL1_p    = ANN_set->sizeB1;
	ANN_set->sizeZ2.rows = ANN_set->sizeW2.rows;				ANN_set->sizeZ2.cols = ANN_set->sizeL1_p.cols;
	ANN_set->sizeL2_p    = ANN_set->sizeB2;
	ANN_set->sizeZ3.rows = ANN_set->sizeW3.rows;				ANN_set->sizeZ3.cols = ANN_set->sizeL2_p.cols;
	ANN_set->sizeL3_p    = ANN_set->sizeB3;

}