#include "StdAfx.h"
#include "C_Command_Cutroi.h"

C_Command_Cutroi::C_Command_Cutroi(void)
{
	UINT parint[5] = {IMAGE,NUMBER,NUMBER,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("cutroi",5,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = cutroi(IMAGE,NUMBER,NUMBER,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Cutroi::~C_Command_Cutroi(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Cutroi::Do(C_Error& err)
{
	C_Image_Container *image;
	C_Image_Container image2;
	C_Matrix_Container *p_startx;
	C_Matrix_Container *p_starty;
	C_Matrix_Container *p_stopx;
	C_Matrix_Container *p_stopy;
	unsigned int start_x, start_y, stop_y, stop_x;
	unsigned long actrow,actcol,row,col;
	double tmp;

	image = (C_Image_Container*)intab[0]->data;
	p_startx = (C_Matrix_Container*)intab[1]->data;
	p_stopx = (C_Matrix_Container*)intab[2]->data;
	p_starty = (C_Matrix_Container*)intab[3]->data;
	p_stopy = (C_Matrix_Container*)intab[4]->data;
	start_x = (int)p_startx->data[0];
	stop_x = (int)p_stopx->data[0];
	start_y = (int)p_starty->data[0];
	stop_y = (int)p_stopy->data[0];
	
	if(stop_x<=start_x || stop_y<=start_y || start_x<0 || start_y<0 || stop_x>image->_cols || stop_y>image->_rows)	{
		err.SetError("cutroi: Bad range");
		return;
	}
//	image->InheritProperties(&image2);
	image2.AllocateData(stop_y-start_y,stop_x-start_x);
	image2.xorigin = start_x;
	image2.yorigin = start_y;
	
	actrow = actcol = 0;
	for(row=start_y;row<stop_y;row++)	{
		for(col=start_x;col<stop_x;col++)	{
			image->GetPixel(row,col,tmp);
			image2.SetPixel(actrow,actcol++,tmp);
		}
		actcol = 0;
		actrow++;
	}
	outtab[0]->AssignValue((void*)&image2,err);
}
