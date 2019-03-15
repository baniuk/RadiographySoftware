#include "StdAfx.h"
#include "C_Command_GLshow.h"
#include <malloc.h>

C_Command_GLshow::C_Command_GLshow(void)
{
	UINT parint[2] = {STRING,IMAGE};
	UINT parout[1] = {STRING};
	InitCommand("glshow",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	handle = NULL;
	parentwindow = NULL;
	obraz = NULL;
	syntax = "STRING = glshow(STRING,IMAGE)";
	fun_ver = "1.0";
}

C_Command_GLshow::C_Command_GLshow(CWnd* window)
{
	UINT parint[2] = {STRING,IMAGE};
	UINT parout[1] = {STRING};
	InitCommand("glshow",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	handle = NULL;
	parentwindow = window;
	obraz = NULL;
	syntax = "STRING = glshow(STRING,IMAGE)";
	fun_ver = "1.0";
}

C_Command_GLshow::~C_Command_GLshow(void)
{
	SAFE_DELETE(obraz); ASSERT(obraz==NULL);
	SAFE_DELETE(handle);	ASSERT(handle==NULL);
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_GLshow::short2byte(unsigned short data, unsigned char &low, unsigned char &up)
{
//	data = data&0x7FFF;
	low = (unsigned char)(data&0xFF);
	up = (unsigned char)( (data&0xFF00)>>8 );
}

void C_Command_GLshow::Do(C_Error& err)
{
	char* tmpnazwa;
//	double min,max;
	C_Image_Container *tmpimg;
	C_Image_Container copy;
	unsigned int tmprows, tmpcols,newwidth,delta,sx,sy;
	unsigned long licznik,licznik1=0,test=0;
	tmpnazwa = (char*)intab[0]->data;
	tmpimg = (C_Image_Container*)intab[1]->data;
	tmprows = tmpimg->_rows;
	tmpcols = tmpimg->_cols;
	static unsigned int maxx=1024, maxy=1024;
	double skalax,skalay,skala;

 /*	tmpimg->getMinMax(min,max);
	tmpimg->Normalize(0,65535);
	tmpimg->getMinMax(min,max);*/

	tmpimg->CloneObject(&copy);
	copy.Normalize(0,65535);
	
/*	newwidth = (unsigned int)(ceil(double(copy._cols/4.0))*4.0);

	obraz = new unsigned char[copy._rows*newwidth];
	delta = newwidth - copy._cols; 
//	obraz = (unsigned char*)_aligned_malloc(copy._rows*copy._cols,1);
	for(licznik=0;licznik<copy._rows*copy._cols;licznik++)	{
		ASSERT(licznik<copy._rows*newwidth);ASSERT(licznik<copy._rows*copy._cols);
		obraz[licznik1] = (unsigned char)floor(copy.data[licznik]);
		test++;
		if(test==copy._cols)	{
			test=0;
			licznik1+=delta+1;
		} else
			licznik1++;
	}*/
	skalax = (double)copy._cols/(double)maxx;//cols==newwidth
	skalay = (double)copy._rows/(double)maxy;

	if(skalax>skalay)	{
		skala = skalax;
		if(skala<1)	{
			sx = copy._cols; sy = copy._rows;	//cols==newwidth
		} else {
			sx = maxx; sy = (unsigned int)(maxy/skalay);
		}
	} else {
		skala = skalay;
		if(skala<1)	{
			sx = copy._cols; sy = copy._rows;//cols==newwidth
		} else {
			sx = (unsigned int)(maxx/skalax); sy = maxy;
		}
	}

//	handle = new C_OpenGL(obraz,newwidth,tmprows,sx,sy);

	obraz = new unsigned char[1024*524*1];
/*	for(licznik=0;licznik<copy._rows*copy._cols;licznik++)	{
		ASSERT(licznik<copy._rows*copy._cols);
		obraz[licznik] = (unsigned char)floor(copy.data[licznik]);
	}*/

	unsigned short c=0;
	unsigned char c1,c2;
//	short2byte(49151,c1,c2);
	/*for(licznik=0;licznik<1024*524-5;licznik+=6)	{
		short2byte(c,c1,c2);
		obraz[licznik] = c1;	//r1
		obraz[licznik+1] = c2; //r2

		obraz[licznik+2] = c1; //g1
		obraz[licznik+3] = c2; //g2

		obraz[licznik+4] = c1; //b1
		obraz[licznik+5] = c2; //b2

		
		ASSERT(licznik+5<1024*524*6);
//		c++;
		if(c>65535) c=0;

	}*/

	for(licznik=0;licznik<1024*524;licznik++)	{
		obraz[licznik] = c;	//r1
		ASSERT(licznik<1024*524*1);
		c++;
		if(c>255) c=0;

	}

	handle = new C_OpenGL(obraz,1024,524,sx,sy);
	handle->Create(tmpnazwa,WS_CAPTION|WS_VISIBLE|WS_SYSMENU|CS_OWNDC|WS_MINIMIZEBOX|WS_POPUP   ,NULL,0);//
	outtab[0]->AssignValue((void*)tmpnazwa,err);
}