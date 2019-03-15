#include "Cpp_Lib.h"
#include "R_Tiff.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>   
// This is an empty project to build DLL library of interface version 2.0


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"readtiff");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = readtiff(STRING)");
	definition->numofout = 1;
	definition->numofin = 1;
	definition->typesofin[0] = STRING;
	definition->typesofout[0] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"Reads tiff image");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	R_Tiff image;
	C_Image_Container nowy;
	FILE *stream;

	if (image.readTiffImage((char *)intab[0]->data)==0)
	{
		err.SetError("readtiff: No image to load or wrong image type");
		return;
	}

	nowy.AllocateData(image.imageHeader.imageLength,image.imageHeader.imageWidth);

	unsigned char tmp,tmp1;
	unsigned long licznik = 0,i;

	for (i = 0; i < (2*image.imageHeader.imageWidth*image.imageHeader.imageLength); i+=2)
	{
		if (image.imageHeader.lsb)
		{
			tmp = image.picBuffer[i];	
			tmp1 = image.picBuffer[i+1];
		}
		else
		{
			tmp = image.picBuffer[i+1];
			tmp1 = image.picBuffer[i];
		}
		nowy.data[licznik++] = (double)(tmp + 256 * tmp1);
	}
	

	_set_fmode(_O_BINARY);
	fopen_s( &stream, (char *)intab[0]->data, "r" );
	nowy.CreateHeaderBuffor(image.imageHeader.stripOffset);
	fread((void*)nowy.header,sizeof(char),image.imageHeader.stripOffset,stream);
	fclose(stream);
	outtab[0]->AssignValue((void*)&nowy,err);

}