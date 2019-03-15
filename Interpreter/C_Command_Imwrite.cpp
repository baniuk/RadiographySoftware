#include "StdAfx.h"
#include "C_Command_Imwrite.h"

C_Command_Imwrite::C_Command_Imwrite(void)
{
	UINT parint[3] = {STRING,IMAGE,NUMBER};
	UINT parout[1] = {};
	InitCommand("imwrite",3,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "imwrite(STRING,IMAGE,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Imwrite::~C_Command_Imwrite(void)
{	
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Imwrite::Do(C_Error& err)
{
	char* tmpnazwa;
	char repair[512];
	C_Image_Container *image;
	C_Image_Container image1;
	C_Matrix_Container *p_status;
	unsigned long licznik,rows;
	double status;
	uint16 *buffer1;
	uint32 image_width, image_height;
    float xres, yres;
    uint16 spp, bpp, photo, res_unit;
    TIFF *out;
    uint32 j;

	tmpnazwa = (char*)intab[0]->data;
	image = (C_Image_Container*)intab[1]->data;
	p_status = (C_Matrix_Container*)intab[2]->data;
	status = p_status->data[0];

	RepairPath(tmpnazwa,repair);

	if((out = TIFFOpen(tmpnazwa, "w")) == NULL){
		err.SetError("Imwrite: Could not open outgoing image");
		return;
	}
	// We need to know the width and the height before we can malloc
	image->CloneObject(&image1);
	if(status>0)
		image1.Normalize(0,65535);
	image_width = image->_cols;;
    image_height = image->_rows;
	buffer1 = new uint16[image_width*image_height];
	licznik = 0;
	for(rows=0;rows<image->GetNumofElements();rows++)
		buffer1[rows] = (uint16)floor(image1.data[rows]);
	spp = 1; /* Samples per pixel */
    bpp = 16; /* Bits per sample */
    photo = PHOTOMETRIC_MINISBLACK;

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, image_width / spp);
    TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, bpp);
    TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, spp);
    TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(out, TIFFTAG_PHOTOMETRIC, photo);
    TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_BOTLEFT);
	TIFFSetField(out, TIFFTAG_XPOSITION,image->xorigin);
	TIFFSetField(out, TIFFTAG_YPOSITION,image->yorigin);
    /* It is good to set resolutions too (but it is not nesessary) */
    xres = yres = 100;
    res_unit = RESUNIT_INCH;
    TIFFSetField(out, TIFFTAG_XRESOLUTION, xres);
    TIFFSetField(out, TIFFTAG_YRESOLUTION, yres);
    TIFFSetField(out, TIFFTAG_RESOLUTIONUNIT, res_unit);
	
	for (j = 0; j < image_height; j++)
        TIFFWriteScanline(out, &buffer1[j * image_width], j, 0);
	TIFFClose(out);
	delete[] buffer1;

}