#include "StdAfx.h"
#include "C_Command_Showfeatures.h"

C_Command_Showfeatures::C_Command_Showfeatures(void)
{
	UINT parint[5] = {IMAGE,IMAGE,MATRIX,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("genindexedimage",5,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = genindexedimage(IMAGE,IMAGE,MATRIX,NUMBER,NUMBER)";
	fun_ver = "2.0";
}

C_Command_Showfeatures::~C_Command_Showfeatures(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Showfeatures::Do(C_Error& err)
{
	int number,color;
	double* tab;
	C_Image_Container* input_image;
	C_Image_Container* bin;
	C_Image_Container image;
	C_Matrix_Container *tab_m;
	C_Matrix_Container *p_number;
	C_Matrix_Container *p_color;
// dodane przezemnie
	double pozycjax, pozycjay;

	input_image = (C_Image_Container*)intab[0]->data;
	bin = (C_Image_Container*)intab[1]->data;
	tab_m = (C_Matrix_Container*)intab[2]->data;
	p_number = (C_Matrix_Container*)intab[3]->data;
	p_color = (C_Matrix_Container*)intab[4]->data;
	number = (int)p_number->data[0];
	color = (int)p_color->data[0];
	tab = tab_m->data;
	input_image->CloneObject(&image);

	double zero[9][9]={
		{0,0,0,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,1,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,1,0},
		{0,1,0,0,0,0,0,1,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,1,1,1,0,0,0}};
	double one[9][9]={
		{0,0,0,0,0,1,0,0,0},
		{0,0,0,0,1,1,0,0,0},
		{0,0,0,1,0,1,0,0,0},
		{0,0,1,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0,0},
		{0,0,0,0,0,1,0,0,0}};
	double two[9][9]={
		{0,0,0,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0,0},
		{0,0,0,0,1,0,0,0,0},
		{0,0,0,1,0,0,0,0,0},
		{0,0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0,0},
		{0,0,1,1,1,1,1,0,0}};
	double three[9][9]={
		{0,0,0,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,1,1,0,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,1,1,1,0,0,0}};
	double four[9][9]={
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,0,0,1,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0}};
	double five[9][9]={
		{0,0,1,1,1,1,1,0,0},
		{0,0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0,0},
		{0,0,1,1,1,1,0,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,1,1,1,1,0,0,0}};
	double six[9][9]={
		{0,0,0,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,0,0,0},
		{0,0,1,0,0,0,0,0,0},
		{0,0,1,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,1,1,1,0,0,0}};
	double seven[9][9]={
		{0,0,1,1,1,1,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,1,0,0,0},
		{0,0,0,0,1,0,0,0,0},
		{0,0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0},
		{0,0,0,1,0,0,0,0,0}};
	double eight[9][9]={
		{0,0,0,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,1,1,1,0,0,0}};
	double nine[9][9]={
		{0,0,0,1,1,1,0,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,1,1,1,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,0,0,0,0,1,0,0},
		{0,0,1,0,0,0,1,0,0},
		{0,0,0,1,1,1,0,0,0}};

	int poz=13;
	
	C_Image_Container outln;
	Outline(bin,&outln);
	double col;
	if(color==1)
		col=65000;
	else
		col=0;
	for(unsigned int i=0;i<bin->_rows;i++)
		for(unsigned int j=0;j<bin->_cols;j++)
		{
			double pix;
			outln.GetPixel(i,j,pix);
			
			if(pix>0)
			{
					image.SetPixel(i,j,col);	
			}
		}


	for(int i=0;i<number;i++)
	{
		double pozx=tab[i*poz+2];
		double pozy=tab[i*poz+3];
		std::stringstream ss;
		std::string str;
		ss << i;
		ss >> str;
		ss.clear();
		int roz = (int)str.length();
		for(int k=0;k<roz;k++)
				{
					if(str[k]=='0')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(zero[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='1')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(one[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='2')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(two[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='3')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(three[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='4')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(four[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='5')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(five[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='6')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(six[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='7')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(seven[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='8')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(eight[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
					if(str[k]=='9')
						for(int x=0;x<9;x++)
							for(int y=0;y<9;y++)	{
								pozycjax = 10+x+pozx; pozycjay = 10+k*10+y+pozy;
								if(pozycjax>=image._rows) pozycjax = image._rows-1; if(pozycjay>=image._cols) pozycjay = image._cols-1;
								if(nine[x][y]>0)
									image.SetPixel((unsigned long)(pozycjax),(unsigned long)(pozycjay),col);
							}
				}
		str.clear();	
	}
	outtab[0]->AssignValue((void*)&image,err);
	
}
void C_Command_Showfeatures::Outline(C_Image_Container* image,C_Image_Container* outln)
{
	// POMOCNICZE OBRAZKI
	unsigned int x_res, y_res;
	x_res = image->_cols;
	y_res = image->_rows;

	// DO EROZJI
	C_Image_Container image2;
	image2.AllocateData(y_res,x_res);
	// TYMCZASOWY OBRAZEK BINARNY
	
	// WYNIKOWY
	outln->AllocateData(y_res,x_res);


	// ZMIENNE
	unsigned int size_x = 3;
	unsigned int size_y = 3;
	double zm;
	int k = 0;

	// IMPLEMENTACJA EROZJI
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{	
			k = 0;
			for (unsigned int i = 0; i < size_y; i++)
			{	
				for (unsigned int j = 0; j < size_x; j++)
				{
					if((i+m)-((size_x-1)/2)>0)
						if((j+n-((size_y-1)/2))>0)
							if((i+m)-((size_x-1)/2)<image2._cols)
								if((j+n-((size_y-1)/2))<image2._rows)
								{
									image->GetPixel(j+n-(size_x-1)/2,i+m-(size_y-1)/2,zm);
									if (zm != 1)
									{
										k = 1;
									}
									
								}
				}
			}
			if (k == 1)
			{
				image2.SetPixel(n,m,1);
			}
			else
			{
				image2.SetPixel(n,m,0);
			}
		}
	}

	double zm1 = 0, zm2 = 0;
	// ODEJMOWANIE
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{
			image->GetPixel(n,m,zm1);
			image2.GetPixel(n,m,zm2);
			if (zm1-zm2 == 0)
			{
				outln->SetPixel(n,m,1);
			}
			else
			{
				outln->SetPixel(n,m,0);
			}
		}
	}
	
}