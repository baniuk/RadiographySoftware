#include "StdAfx.h"
#include "C_Command_Imread.h"

C_Command_Imread::C_Command_Imread(void)
{
	UINT parint[1] = {STRING};
	UINT parout[1] = {IMAGE};
	InitCommand("imread",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = imread(STRING)";
	fun_ver = "1.0";
}

C_Command_Imread::~C_Command_Imread(void)
{	
//	for(int a=0;a<numofinparam;a++)
//		if(intab!=NULL) SAFE_DELETE(intab[a]);
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Imread::Do(C_Error& err)
{
	char* tmpnazwa;
	char repair[512];
	tsize_t stripSize;
	int stripMax, stripCount;
	unsigned long imageOffset, bufferSize, result;
	unsigned long rows;
//	unsigned int stopx,stopy;
	int licznik;
//	double pix;
	unsigned char tmp,tmp1;
	unsigned char *buffer;
//	unsigned char *buffer1;
//	double testx,testy;
	uint16 photo;
	C_Image_Container tmpimg;
	C_Image_Container parity;

	err.SetUserData("Imread: This is obsolote function. Use readtiff instead");

	tmpnazwa = (char*)intab[0]->data;
	RepairPath(tmpnazwa,repair);

	TIFF* tif = TIFFOpen(repair, "r");

	if(!tif)	{
		err.SetError("Imread: Can't open the image");
		return;
	}
	else	{
		stripSize = TIFFStripSize(tif);
		stripMax = TIFFNumberOfStrips(tif);
		imageOffset = 0;
		bufferSize = TIFFNumberOfStrips(tif) * stripSize;
		buffer = new unsigned char[bufferSize];
		for (stripCount = 0; stripCount < stripMax; stripCount++)	{
			if((result = TIFFReadEncodedStrip (tif, stripCount, buffer + imageOffset, stripSize)) == -1)	{
				err.SetError("Imread: Read error on input strip number");
				return;
			}
			imageOffset += result;
		}
		if(TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photo) == 0)	{
			err.SetError("Imread: Image has an undefined photometric interpretation");
			return;
		}
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &tmpimg._rows);
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &tmpimg._cols);

		licznik = 0;
		if(bufferSize!=tmpimg._rows*tmpimg._cols*2)	{
			tmpimg._rows+=2;
		}
		tmpimg.AllocateData(tmpimg._rows, tmpimg._cols);
		tmpimg.Zeros();
		for(rows=0;rows<(unsigned long)(bufferSize);rows+=2)	{
			tmp = buffer[rows];
			tmp1 = buffer[rows+1];
			tmpimg.data[licznik] =(double)(tmp+256*tmp1);
			licznik++;
		}
/*		testx = floor((double)tmpimg._cols/2) - tmpimg._cols/2;
		testy = floor((double)tmpimg._rows/2) - tmpimg._rows/2;
		if(testx!=0)
			stopx = tmpimg._cols - 1;
		if(testy!=0)
			stopy = tmpimg._rows - 1;
		if(testx!=0 || testy!=0)	{
			tmpimg.CloneObject(&parity);
			tmpimg.FreeData();
			tmpimg.AllocateData(stopy,stopx);
			for(unsigned int y=0;y<stopy;y++)
				for(unsigned int x=0;y<stopx;x++)	{
					parity.GetPixel(y,x,pix);
					tmpimg.SetPixel(y,x,pix);
				}
		}
		*/
//double minn,maxx;
//tmpimg.getMinMax(minn,maxx);
		outtab[0]->AssignValue((void*)&tmpimg,err);
		delete[] buffer;
		TIFFClose(tif);
	}
}
