// Load_Disp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include "C_Image.h"
#include <fftw3.h>


void imageshow(char *WinName,IplImage *img);
unsigned long cor2ind(unsigned int rows, unsigned int cols, unsigned int actrow, unsigned int actcol);


int _tmain(int argc, _TCHAR* argv[])
{
	IplImage *img;
	CvSize size;
	unsigned long ret,ret1;


	fftw_complex *out, *out_in;
	fftw_complex* in;
	fftw_plan g, g1;

	C_Image image;
	C_Image output;
	C_Image fft_real;
	C_Image fft_img;
	if(!image.ReadTiff("c:\\TestSample_2.3.tif"))
		return -1;
	double tmp;
// -------------------------------------------------------------------------------------------------		
// W TYM MIEJSCU WSTAWIAMY W£ASNY KOD
// PAMIÊTAMY ¯E PISZEMY FUNKCJÊ KTÓRA POBIERA I ZWRACA WSKANIKI DO KLASY C_Image TAK JAK BY£O POPRZEDNIO USTALANE
// Mamy do dyspozycji obiekt klasy C_Image zawieraj¹cy podstawowe funkcje od obs³ugi obrazka oraz sam obrazek - szczegó³y w pliku C_Image.h

// Przyk³ady:	
/*	Wczytanie tiffa
	C_Image image;
	image.ReadTiff("c:\\test.tif");

// nagranie do pliku - format bin - do obs³ugi w matlabie
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

// kopiowanie obrazków
	C_Image destination;		// tu bêdzie kopia
	image.CloneImage(&destination);		// skopiowanie obrazka image do destination
	
// szukanie minimum i maksimum na obrazku
	double min, max;
	image.getMinMax(min,max);	% w zmiennych min i maks pojawi¹ siê wartoœci minimalne i maksymalne

// Skalowanie obrazka
	image.Normalize(0,65535);	% normalizacja w zakresie mp. 0 - 65535

	void MojaFunkcja(C_Image* src, C_Image* dst)
	{
		src->Normalize(0,65535); % !!! Tu modyfikujemy obrazek ¿ród³owy co jest niebezpieczne gdy¿ nie wiemy czy czasem nie 	wp³ynie 			to na dzia³anie innych funkcji
		% powinno byæ tak:
		C_Image kopia;	%
		src->CloneObject(&kopia);	% tworzymy kopiê lokaln¹ obrazka
		kopia.Normalize(0,65535);	% spokojnie sobie j¹ modyfikujemy

		kopia.CloneObject(dst);	% kopiujemy wynik do wyjœcia
	}

*/


//	image.saveimage2matlab("c:\\test1.bin");	

	in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * image._cols*image._rows);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * image._cols*image._rows);
	out_in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * image._cols*image._rows);
	
	image.Normalize(0,65535);
/*	for(unsigned long a=0;a<image.GetNumofElements();a++)	{
		in[a][0] = image._data[a];
		in[a][1] = 0.0;
	}

	g = fftw_plan_dft_2d(image._rows, image._cols, in, out, FFTW_FORWARD,FFTW_ESTIMATE);

	fftw_execute(g);*/

/*	int rad = 1500;
	int ny = (int)floor( (double)image._cols/2) + 1;
	int nx = (int)floor( (double)image._rows/2) + 1;
	for(int j = 0; j < ny; j++)
	{
		for(int i = 0; i < nx; i++)
		{
			if ( rad < i*i + j*j )
			{
				// Set to 0 real part and imaginary part
				//in this test, i and j represent the horizontal and vertical frequency
				(*(out + (i*ny+j)))[0] = (double)0;
				(*(out + (i*ny+j)))[1] = (double)0;
			}
		}
	}*/
/*	for(unsigned int x=200;x<250;x++)		//504
		for(unsigned int y=200;y<384;y++)	{	//768
			ret = cor2ind(image._rows, image._cols, y, x);
			out[ret][0] = 0.0;
			out[ret][1] = 0.0;
		}*/
/*	for(unsigned int x=0;x<image._cols/2;x++)		//504
		for(unsigned int y=0;y<image._rows/2;y++)	{	//768
			ret = cor2ind(image._rows, image._cols, y, x);
			ret1 = cor2ind(image._rows, image._cols, y, image._cols-x-1);
			out[ret1][0] = out[ret][0];
			out[ret1][1] = out[ret][1];
		}
	for(unsigned int x=0;x<image._cols;x++)		//504
		for(unsigned int y=0;y<image._rows/2;y++)	{	//768
			ret = cor2ind(image._rows, image._cols, y, x);
			ret1 = cor2ind(image._rows, image._cols, image._rows-y-1, x);
			out[ret1][0] = out[ret][0];
			out[ret1][1] = out[ret][1];
		}*/


/*	for(unsigned long a=image.GetNumofElements()/4;a<image.GetNumofElements();a++)	{
		out[a][0] = 0.0;
		out[a][1] = 0.0;
	}*/

/*	fft_real.AllocateData(image._rows, image._cols);
	fft_img.AllocateData(image._rows, image._cols);
	for(unsigned long a=0;a<fft_real.GetNumofElements();a++)	{
		fft_real._data[a] = out[a][0];
		fft_img._data[a] = out[a][1];
	}
	fft_img.saveimage2matlab("c:\\img.dat");
	fft_real.saveimage2matlab("c:\\real.dat");


	g1 = fftw_plan_dft_2d(image._rows, image._cols, out, out_in, FFTW_BACKWARD,FFTW_ESTIMATE);

	fftw_execute(g1);*/

/*	for(unsigned long a=0;a<image.GetNumofElements();a++)	{
		out_in[a][0] = out_in[a][0]/(image._rows*image._cols);
		out_in[a][1] = out_in[a][1]/(image._rows*image._cols);
	}*/

/*	output.AllocateData(image._rows, image._cols);

	for(unsigned long a=0;a<image.GetNumofElements();a++)	{
		tmp = sqrt(pow(out_in[a][0],2) + pow(out_in[a][1],2));
//		tmp = abs(out[a][0]);
		output._data[a] = tmp;
	}

		output.saveimage2matlab("c:\\test1.bin");
	output.Normalize(0,65535);*/


// ----- Wyœwietlanie za pomoc¹ CV ----------------------------------------------
	size.height = image._rows;
	size.width = image._cols;
	img = cvCreateImage(size, IPL_DEPTH_16U, 1);
	img->imageData = image.ReturnIPLBuffor();
	imageshow("Image",img);
	cvWaitKey();
	
//------- Sprz¹tanie -----------------------------------------------------------	
	cvDestroyWindow("Image");
	cvReleaseImage(&img);

	fftw_destroy_plan(g);
    fftw_free(in); 
    fftw_free(out);

	return 0;
}
inline unsigned long cor2ind(unsigned int rows, unsigned int cols, unsigned int actrow, unsigned int actcol)
{
	return actrow * cols + actcol;
}
void imageshow(char *WinName,IplImage *img)
{
	cvNamedWindow(WinName,1);
	cvShowImage(WinName,img);
}