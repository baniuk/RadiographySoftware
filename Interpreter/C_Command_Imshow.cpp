#include "StdAfx.h"
#include "C_Command_Imshow.h"

C_Command_Imshow::C_Command_Imshow(void)
{
	UINT parint[3] = {STRING,IMAGE,NUMBER};
	UINT parout[1] = {STRING};
	InitCommand("imshow",3,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	InternalCleaning = NULL;
	syntax = "STRING = imshow(STRING,IMAGE,NUMBER)";
	fun_ver = "1.0";
//	numofopenwindows = 0;
}

C_Command_Imshow::~C_Command_Imshow(void)
{
//	char* tmp = (char*)intab[0]->data; // nie dzia³a bo zmienne znikaj¹ szybciej ni¿ instrukcje
	if(InternalCleaning) cvDestroyWindow(InternalCleaning);
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
	SAFE_DELETE(InternalCleaning);
}
void C_Command_Imshow::Do(C_Error& err)
{
	char* tmp;
	int xres = 1024;
	int yres = 768;
	double coef;
	C_Matrix_Container *p_binary;
	int binary;
	unsigned int r,k;
	IplImage *img;
	IplImage *imgnew;
	CvSize size;
	CvSize sizenew;
	C_Image_Container *input;
	C_Image_Container *tmpimg;
	C_Image_Container dtmpimg;
	tmp = (char*)intab[0]->data;
	input = (C_Image_Container*)intab[1]->data;
	p_binary = (C_Matrix_Container*)intab[2]->data;
	binary = (int)p_binary->data[0];
	outtab[0]->AssignValue((void*)tmp,err);

	if(input->_cols%2 !=0)	{
		dtmpimg.AllocateData(input->_rows,input->_cols-1);
		for(r=0;r<input->_rows;r++)
			for(k=0;k<input->_cols-1;k++)
				dtmpimg.SetPixel(r,k,input->GetPixel(r,k));
		tmpimg = &dtmpimg;
	} else
		tmpimg = input;


	cvNamedWindow(tmp,1);
	size.height = tmpimg->_rows;
	size.width = tmpimg->_cols;
//	tmpimg->Normalize(0,65535);
//	for(int a=0;a<tmpimg->_cols;a++)
//		tmpimg->SetPixel(tmpimg->_rows-1,a,0);

	img = cvCreateImage(size, IPL_DEPTH_16U, 1);
	if(binary==0)
		img->imageData = tmpimg->ReturnIPLBuffor();
	else
		img->imageData = tmpimg->ReturnIPLBuffor(0,65535);

	
	if(size.width>xres || size.height>yres)	{
		if(size.width/xres>size.height/yres)
			coef = (double)size.width/(double)xres;
		else
			coef = (double)size.height/(double)yres;
		sizenew.height = (int)floor(size.height/coef);
		sizenew.width = (int)floor(size.width/coef);
		imgnew = cvCreateImage(sizenew, IPL_DEPTH_16U, 1);
		cvResize(img,imgnew,CV_INTER_LINEAR);
		cvShowImage(tmp,imgnew);
		cvReleaseImage(&img);
		cvReleaseImage(&imgnew);
	}
	else	{
		cvShowImage(tmp,img);
		cvReleaseImage(&img);
	}

	InternalCleaning = new char[strlen(tmp)+1];
	strcpy_s(InternalCleaning,strlen(tmp)+1,tmp);
}
