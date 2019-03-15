#include "StdAfx.h"
#include "C_Image.h"

C_Image::C_Image(void)
{
	_data = NULL;
	_iplbuffor = NULL;
	tif = NULL;
}

C_Image::~C_Image(void)
{
	SAFE_DELETE(_data);
	SAFE_DELETE(_iplbuffor);
	if(tif) TIFFClose(tif);
	cvReleaseImage(&img);

}
void C_Image::AllocateData(unsigned int rows, unsigned int cols)
{
	_data = new double[rows*cols];
	_rows = rows;
	_cols = cols;
	_iplbuffor = NULL;
	tif = NULL;
}
void C_Image::GetPixel(unsigned long row, unsigned long col,double &pixel)
{
	pixel = _data[row*_cols+col];
}
void C_Image::SetPixel(unsigned long row, unsigned long col,double pixel)
{
	_data[row*_cols+col] = pixel;

}
void C_Image::saveimage2matlab(char* filename)
{
	FILE *stream;
	if( fopen_s( &stream, filename, "w+b" ) == 0 )
	{
		fwrite(_data, sizeof( double ), C_Image::GetNumofElements(), stream);
		fclose( stream );
	}
   else
      printf( "Problem opening the file\n" );
}

BOOL C_Image::ReadTiff(char *filename)
{
	tsize_t stripSize;
	int stripMax, stripCount;
	unsigned long imageOffset, bufferSize, result;
	unsigned int pom_h,pom_w;
	unsigned long rows, count;
	int licznik;
	char tmp,tmp1;
	char *buffer,tempbyte;
	uint16 photo, fillorder;

	TIFF* tif = TIFFOpen(filename, "r");
	if(!tif) return false;
	stripSize = TIFFStripSize(tif);
	stripMax = TIFFNumberOfStrips(tif);
	imageOffset = 0;
	bufferSize = TIFFNumberOfStrips(tif) * stripSize;
	buffer = (char *)malloc(bufferSize);
	for (stripCount = 0; stripCount < stripMax; stripCount++)	{
		if((result = TIFFReadEncodedStrip (tif, stripCount, buffer + imageOffset, stripSize)) == -1)	{
			fprintf(stderr, "Read error on input strip number %d\n", stripCount);
			return false;
		}
		imageOffset += result;
	}
	if(TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photo) == 0)	{
		fprintf(stderr, "Image has an undefined photometric interpretation\n");
		return false;
	}

/*	// Deal with fillorder
	if(TIFFGetField(tif, TIFFTAG_FILLORDER, &fillorder) == 0){
		fprintf(stderr, "Image has an undefined fillorder\n");
		return false;
	}
	if(fillorder != FILLORDER_MSB2LSB){
// We need to swap bits -- ABCDEFGH becomes HGFEDCBA
		printf("Fixing the fillorder\n");
		for(count = 0; count < bufferSize; count++){
			tempbyte = 0;
			if(buffer[count] & 128) tempbyte += 1;
			if(buffer[count] & 64) tempbyte += 2;
			if(buffer[count] & 32) tempbyte += 4;
			if(buffer[count] & 16) tempbyte += 8;
			if(buffer[count] & 8) tempbyte += 16;
			if(buffer[count] & 4) tempbyte += 32;
			if(buffer[count] & 2) tempbyte += 64;
			if(buffer[count] & 1) tempbyte += 128;
			buffer[count] = tempbyte;
		}
	}*/


	TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &pom_h);
	_rows = pom_h;
	TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &pom_w);
	_cols = pom_w;
	licznik = 0;
	_data = new double[_cols*_rows];// chyba rzedami 2 bajty na piksel
	for(rows=0;rows<(unsigned int)(_cols*2*_rows);rows+=2)	{
		tmp = buffer[rows];
		tmp1 = buffer[rows+1];
		_data[licznik] =(double)(tmp+256*tmp1);
		licznik++;
	}
	return true;
}

char* C_Image::ReturnIPLBuffor()
{
	unsigned long licznik, licznikdata;
	unsigned int reszta;

	licznik = 0;
	licznikdata = 0;
	_iplbuffor = new char[_cols*2*_rows];
	do	{
		reszta = (unsigned short)_data[licznikdata] % 256;
		_iplbuffor[licznik] = (char)reszta;
		_iplbuffor[licznik+1] = (char)(((unsigned short)_data[licznikdata++] - reszta)/256);
		licznik += 2;
	} while(licznik<_cols*2*_rows);
	return _iplbuffor;
}

void C_Image::FreeData(void)
{
	SAFE_DELETE(_data);
}
void C_Image::CloneObject(C_Image* dest)
{
	dest->_cols = _cols;
	dest->_rows = _rows;
	dest->FreeData();
	dest->AllocateData(dest->_rows,dest->_cols);
	memcpy(dest->_data,_data,sizeof(double)*dest->_rows*dest->_cols);
}

void C_Image::getMinMax(double& min, double& max)
{
	unsigned long r;
	min = 1.7e308;
	max = -1.7e308;;
	for(r=0;r<GetNumofElements();r++)	{
		if(_data[r]<min)	min = _data[r];
		if(_data[r]>max) max = _data[r];
	}
}
void C_Image::Zeros(void)
{
	unsigned long a;
	for(a=0;a<GetNumofElements();a++)
		_data[a] = 0.0;
}

void C_Image::Normalize(unsigned short w1, unsigned short w2)
{
	double min, max, delta;
	unsigned long r;
	getMinMax(min,max);
	if(min==w1 && max==w2)
		return;				// bez normalizacji
	delta = abs(w2-w1);
	for(r=0;r<GetNumofElements();r++)
		_data[r] = delta*_data[r]/(max-min)-min*delta/(max-min);
}

unsigned long C_Image::GetNumofElements(void)
{
	return _rows*_cols;
}
