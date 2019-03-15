#include "StdAfx.h"
#include "C_Command_Hist.h"

C_Command_Hist::C_Command_Hist(void)
{
	UINT parint[2] = {IMAGE,NUMBER};
	UINT parout[1] = {};
	InitCommand("hist",2,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	handle = NULL;
	parentwindow = NULL;
	syntax = "hist(IMAGE,NUMBER)";
	fun_ver = "1.0";
}
C_Command_Hist::C_Command_Hist(CWnd* window)
{
	UINT parint[2] = {IMAGE,NUMBER};
	UINT parout[1] = {};
	InitCommand("hist",2,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	handle = NULL;
	parentwindow = window;
	syntax = "hist(IMAGE,NUMBER)";
	fun_ver = "1.0";
}
C_Command_Hist::~C_Command_Hist(void)
{
	SAFE_DELETE(handle);
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_Hist::Do(C_Error& err)
{
	C_Image_Container *image1;
	C_Matrix_Container *p_wzmocnienie;
	double wzmocnienie;

	double max,min;
		
	image1 = (C_Image_Container*)intab[0]->data;
	p_wzmocnienie = (C_Matrix_Container*)intab[1]->data;
	wzmocnienie = p_wzmocnienie->data[0];

	image1->Hist(256,hist);

/*	hist.AllocateData(1,200);
	for(unsigned int a=0;a<hist.GetNumofElements();a++)
		hist.data[a] = 0;
	hist.data[0] = 100; hist.data[50] = 100; hist.data[100] = 100; hist.data[150] = 100;*/

	hist.getMinMax(min,max);

	handle = new C_OpenGL_Hist(hist.data,hist._cols,max,wzmocnienie,300,200);

	handle->Create("Histogram",WS_CAPTION|WS_VISIBLE|WS_SYSMENU|CS_OWNDC|WS_MINIMIZEBOX,parentwindow,0);
	
	
}
