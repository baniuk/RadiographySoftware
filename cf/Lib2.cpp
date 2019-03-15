#include "Cpp_Lib.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "stdlib.h"
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
#include <process.h>

typedef long int integer;
HANDLE hThread1[8];

#define NUMFEAT 35
#define MAX_NUM_FLAW 1000 // max ilosc obiekt�w

// w��cza  dumping
#define NO_VERBOSE

struct E	{
	double *offset_data;
	double *x_coord_data;
	double *y_coord_data;
	C_Image_Container *original;
	C_Image_Container *indexed;
	C_Image_Container *bw;
	double *features;
	unsigned int num_of_flaws;
	unsigned int start_flaw;
	unsigned int end_flaw;
	unsigned int start_poz;	// pozycja startowa w macierzy features
};

E EX[8];

void  func(void *data);
void cov(C_Matrix_Container* X, C_Matrix_Container* Y);
unsigned int unique(C_Matrix_Container* input,C_Matrix_Container* licznosc);
BOOL isintable(double* tab,double val,unsigned int tab_size);
void RemoveVal(C_Matrix_Container* input,double val);
void GetMinMax(double* data, double& min, double& max,unsigned int ile);
void CutImage(C_Image_Container*im,C_Image_Container *out, double *xc, double *yc, unsigned int ile);
BOOL MatrixMulti(C_Matrix_Container* in1, C_Matrix_Container* in2, C_Matrix_Container* out);

void PB_feat1(unsigned int current_flaw, unsigned int current_flaw_offset,C_Image_Container *res, C_Matrix_Container *x_coord, C_Matrix_Container *y_coord, unsigned int current_flaw_size,double* out1,double* out2,double* out6,double* out7_1,double* out7_2,double* out7_3,double* out7_4,double* out7_5,double* out8,double* out9);
void PB_texture(unsigned int current_flaw, unsigned int current_flaw_offset,C_Image_Container *org, C_Matrix_Container *x_coord, C_Matrix_Container *y_coord, unsigned int current_flaw_size,double* out1,double* out2);
double PB_stdcirc(C_Image_Container *in,double gx,double gy);
double PB_getIndexNumber(C_Image_Container *res);
void featuresMC(C_Image_Container* BwIm,C_Image_Container* BwPer,double Area,double Perimeter,double* Cx,double* Cy,double *Width,double *Length,double *Elongation,double *Rectangulity,double *Compactness,double *Iacf);

unsigned long LN_Area(C_Image_Container*);
double LN_Perimeter(C_Image_Container *in, C_Image_Container *out);
double LN_wspMal(double A, double p);
double LN_wspBB(C_Image_Container *in, double A, double Gx, double Gy);
double LN_wspDan(C_Image_Container *in, double A);
double LN_wspHar(C_Image_Container *in, double gx, double gy, double ilePikseli);
void LN_morf(C_Matrix_Container* F, int M[3][3], bool bit);
void LN_CenterOfGravity(C_Image_Container *in, double A, double *Gx, double *Gy);

void PCA(double* a,integer Na,integer LDA,double *D);

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"cf");	// nazwa w ADDIP
	strcpy_s(definition->version,MAX_VER,"1.3");	// wersja (nie ma wi�kszego znaczenia)
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"MATRIX = cf(IMAGE,IMAGE,IMAGE)"); // SYNTAX (nie ma wi�kszego znaczenia)
	definition->numofout = 1;	// ilosc parametr�w wy�ciowych
	definition->numofin = 3;	// ilosc parametr�w wej�ciowych
	definition->typesofin[0] = IMAGE;definition->typesofin[1] = IMAGE;definition->typesofin[2] = IMAGE;	// typy wej�cia
	definition->typesofout[0] = MATRIX; // typy wyj�cia
	definition->interface_version = 2;	// wersja interface
	strcpy_s(definition->description,MAX_DESC,"Developed by PB, MC, LN");	// cokolwiek
	definition->ADDIP_version = 2.0; 

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void AssignGlobals(C_Variables *(*p_globals)[MAX_GLOBALS],int* p_lastglobal)
{
	globals = p_globals;
	lastglobal = p_lastglobal;	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Image_Container *original;
	C_Image_Container *indexed;
	C_Image_Container *bw;

/*	C_Image_Container original1;
	C_Image_Container indexed1;
	C_Image_Container bw1;
	C_Image_Container original2;
	C_Image_Container indexed2;
	C_Image_Container bw2;
	C_Matrix_Container x_coord2;	// wsp x punkt�w
	C_Matrix_Container y_coord2; // wsp y punkt�w*/

	unsigned int num_of_points=0; // ile jest wszystkich punkt�w
	unsigned int a,x,y,o,licznik;
	unsigned int pixel;
	unsigned int num_of_flaws; // ile jest zaindeksowanych wad
	unsigned int delta,dd;
	char NUMOFPROC = 1;

	C_Matrix_Container usorted;	// pojedyncze wartosci
	C_Matrix_Container licznosc; // ilosc pixeli kazdego rodzaju
	C_Matrix_Container x_coord;	// wsp x punkt�w
	C_Matrix_Container y_coord; // wsp y punkt�w
	C_Matrix_Container offset;	// offset od poczatku tablicy x,y_coord skazuj�cy na kolejne wady
	C_Matrix_Container lastpoz;	// ostatnia pozycja dl kazdej wady w tablicy index�w
	C_Matrix_Container features;
	C_Matrix_Container* global_proc;
	original = (C_Image_Container*)intab[0]->data;	// tu dokonujemy konwersji bo w intab wszystko jest trzymane jako void*
	indexed = (C_Image_Container*)intab[1]->data;
	bw = (C_Image_Container*)intab[2]->data;

	if(original->_cols!=indexed->_cols)
		err.SetUserData("cf: Input images differ in size - results may be improper");
	else
		if(original->_cols!=bw->_cols)
			err.SetUserData("cf: Input images differ in size - results may be improper");
		else
			if(original->_rows!=indexed->_rows)
				err.SetUserData("cf: Input images differ in size - results may be improper");
			else
				if(original->_rows!=bw->_rows)
					err.SetUserData("cf: Input images differ in size - results may be improper");

/*	original->CloneObject(&original1);
	original->CloneObject(&original2);

	indexed->CloneObject(&indexed1);
	indexed->CloneObject(&indexed2);*/

// sprawdzanie czy istnieje zmienna globalna numofproc (ustawiana przez funkcje Threads)
	int th;
	for(th=0;th<*(lastglobal);th++)
		if((*globals)[th]!=NULL)
			if(strcmp("numofproc",(*globals)[th]->name)==0)	{
				global_proc = (C_Matrix_Container*)(*globals)[th]->data;
				NUMOFPROC = (char)global_proc->data[0];
			}
//-------------------------------------------------------------------------------------
	num_of_flaws = unique(indexed,&licznosc)-1; // bo w indexed jeszcze tlo jest
	if(num_of_flaws>MAX_NUM_FLAW)	{
		err.SetUserData("cf: Number of objects exceed allowed limit !!");
		num_of_flaws = MAX_NUM_FLAW;
	}

	for(a=1;a<licznosc.GetNumofElements();a++)	// od jedeynki zeby zer nie liczyc
		num_of_points+=(unsigned int)licznosc.data[a];
//licznosc.DumpBinary("c:\\licznosc.dat");
	offset.AllocateData(1,65536);
	offset.Zeros();

	for(o=1;o<licznosc.GetNumofElements();o++)
		if(licznosc.data[o]!=0)
			for(a=1;a<o;a++)
				offset.data[o]+=licznosc.data[a];
		else
			break;
	offset.data[o]=offset.data[o-1]+licznosc.data[a];	// ostatnia wada 

// wspolrzedne wad
	lastpoz.AllocateData(1,65536);
	usorted.AllocateData(1,65536);
	lastpoz.Zeros();
	usorted.Zeros();
	licznik = 0;
	x_coord.AllocateData(1,num_of_points);
	y_coord.AllocateData(1,num_of_points);
	for(y=0;y<indexed->_rows;y++)
		for(x=0;x<indexed->_cols;x++)	{
			pixel = (unsigned int)indexed->GetPixel(y,x);
			if(pixel!=0)	{
				usorted.data[pixel] = pixel;
				x_coord.data[ (unsigned int)offset.data[pixel]+(unsigned int)lastpoz.data[pixel] ] = x;
				y_coord.data[ (unsigned int)offset.data[pixel]+(unsigned int)lastpoz.data[pixel] ] = y;
				lastpoz.data[pixel]++;
			}
		}
	RemoveVal(&offset,0);
	RemoveVal(&usorted,0);
	features.AllocateData(num_of_flaws,NUMFEAT);
	features.Zeros();
// Main loop
//offset.DumpBinary("c:\\offset.out");
usorted.DumpBinary("usorted.dat");
//_RPT0(_CRT_WARN, "Use the assert macros to evaluate the expression p1 == p2.\n");
	
//	x_coord.CloneObject(&x_coord2);
//	y_coord.CloneObject(&y_coord2);

	if(num_of_flaws<10)
		NUMOFPROC = 1;
	delta = (unsigned int)floor((double)num_of_flaws/NUMOFPROC);
	for(dd=0;dd<NUMOFPROC;dd++)	{
		EX[dd].bw = bw;
		EX[dd].features = features.data;
		EX[dd].indexed = indexed;
		EX[dd].start_flaw = dd*delta;
		EX[dd].end_flaw = EX[dd].start_flaw+delta-1;
		if(dd==NUMOFPROC-1) EX[dd].end_flaw = num_of_flaws-1;
		EX[dd].num_of_flaws = EX[dd].end_flaw-EX[dd].start_flaw+1;
		EX[dd].start_poz = EX[dd].start_flaw*NUMFEAT;	// 21-ilosc cech
		EX[dd].offset_data = offset.data;
		EX[dd].original = original;
		EX[dd].x_coord_data = x_coord.data;
		EX[dd].y_coord_data = y_coord.data;
	}



	for(char thread=0;thread<NUMOFPROC;thread++)
		hThread1[thread] = (HANDLE)_beginthread(func, 0,(void*)&EX[thread]);

	WaitForMultipleObjects(NUMOFPROC,hThread1,TRUE,INFINITE);

//features.DumpBinary("c:\\features.out");
	outtab[0]->AssignValue((void*)&features,err);
}
void func(void *data)
{
	char str[50];
	E *ex1;
	ex1 = (E*)data;
	unsigned int current_flaw,current_flaw_offset,current_flaw_size;
	double LN_area,LN_peri,LN_mal,LN_gx,LN_gy,LN_bb,LN_dan,LN_har;
	C_Image_Container res;		// wycieta wada z indexed
	C_Image_Container org;		// wycieta wada z oryginalu
	C_Image_Container black;		// wycieta wada z BW
	unsigned int l,ll=0,ff=ex1->start_poz;
	C_Matrix_Container fxcoord;
	C_Matrix_Container fycoord;
	double PBf1,PBf2,PBf6,PBf7_1,PBf7_2,PBf7_3,PBf7_4,PBf7_5,PBf8,PBf9;	
	double PBt1,PBt2;
	double MC_Cx,MC_Cy,MC_Width,MC_Length,MC_Elongation,MC_Rectangulity,MC_Compactness,MC_Iacf;
	C_Image_Container B;
	double t_xmin, t_xmax, t_ymin, t_ymax;

#ifdef VERBOSE
	ex1->indexed->DumpBinary("indexed.out");
#endif

	for(current_flaw=ex1->start_flaw;current_flaw<=ex1->end_flaw;current_flaw++)
	{
		// Cutting the flaw
		LN_gx = LN_gy = 0;
		if(current_flaw==0)	{
			current_flaw_offset = 0;
			current_flaw_size = (unsigned int)ex1->offset_data[0];
		} else {
			current_flaw_offset = (unsigned int)ex1->offset_data[current_flaw-1];
			current_flaw_size = (unsigned int)(ex1->offset_data[current_flaw]-ex1->offset_data[current_flaw-1]);
		}

		CutImage(ex1->indexed,	&res,	&ex1->x_coord_data[current_flaw_offset],&ex1->y_coord_data[current_flaw_offset], current_flaw_size);
		CutImage(ex1->original, &org,	&ex1->x_coord_data[current_flaw_offset],&ex1->y_coord_data[current_flaw_offset], current_flaw_size);
		CutImage(ex1->bw,		&black, &ex1->x_coord_data[current_flaw_offset],&ex1->y_coord_data[current_flaw_offset], current_flaw_size);
		GetMinMax(&ex1->x_coord_data[current_flaw_offset], t_xmin, t_xmax, current_flaw_size);
		GetMinMax(&ex1->y_coord_data[current_flaw_offset], t_ymin, t_ymax, current_flaw_size);

	/* USER FUCTIONS HERE
		Usefull parameters:
			unsigned int current_flaw			- current flaw index
			unsigned int current_flaw_offset	- offset in coordinates tables
			C_Image_Container res				- extracted flaw (indexed)
			C_Image_Container org				- extracted flaw (original)
			C_Matrix_Container x_coord			- x coordinates of flaws
			C_Matrix_Container y_coord			- y coordinates of flaws
			unsigned int current_flaw_size		- number of pixels in current flaws
	*/
	/* How to extract coordinates for given flaw: 
			x_coord.data[current_flaw_offset:current_flaw_offset+current_flaw_size]
		*/
// do debugowania i wyswietlania w matlabie
#ifdef VERBOSE
		sprintf_s(str,50,"org%d.out",current_flaw);
		org.DumpBinary(str);
		sprintf_s(str,50,"ind%d.out",current_flaw);
		res.DumpBinary(str);
#endif

//res.DumpBinary("c:\\res.out");
//indexed->DumpBinary("c:\\indexed.dat");
		ll=0;
		fxcoord.AllocateData(current_flaw_size,1);
		fycoord.AllocateData(current_flaw_size,1);
		for(l=current_flaw_offset;l<current_flaw_offset+current_flaw_size;l++)	{
			fxcoord.data[ll] = ex1->x_coord_data[l];
			fycoord.data[ll++] = ex1->y_coord_data[l];
		}
// nagrywanie dla kompatybilosci z wskclassifier
		sprintf_s(str,50,"x%d.out",current_flaw);
		fxcoord.DumpBinary(str);
		sprintf_s(str,50,"y%d.out",current_flaw);
		fycoord.DumpBinary(str);

// ******************************************* PB *************************************
		if(current_flaw_size>3)
			PB_feat1(current_flaw,current_flaw_offset,&res,&fxcoord,&fycoord,current_flaw_size,&PBf1,&PBf2,&PBf6,&PBf7_1,&PBf7_2,&PBf7_3,&PBf7_4,&PBf7_5,&PBf8,&PBf9);
		else { PBf1=PBf2=PBf6=PBf7_1=PBf7_2=PBf7_3=PBf7_4 = PBf7_5 = -1;	}
		PB_texture(current_flaw,current_flaw_offset,&org,&fxcoord,&fycoord,current_flaw_size,&PBt1,&PBt2);
		ex1->features[ff++] = current_flaw;	// number
		ex1->features[ff++] = PB_getIndexNumber(&res);	// indexed number
		ex1->features[ff++] = PBf1;	// data lenghtening
		ex1->features[ff++] = PBf2;	// angle
		ex1->features[ff++] = PBf6;	// Principal lenghtening
		ex1->features[ff++] = PBf7_1;	// Longer axis
		ex1->features[ff++] = PBf7_2;	// shorter axis
		ex1->features[ff++] = PBf7_3;	// pole-punktyin
		ex1->features[ff++] = PBf7_4;	// punkty out
		ex1->features[ff++] = PBf7_5;	// angle ellip nr10
// ******************************************* LN *************************************
		ex1->features[ff++] = LN_area=LN_Area(&res);
		B.AllocateData(res._rows, res._cols); B.Zeros();
		ex1->features[ff++] = LN_peri=LN_Perimeter(&res, &B);
		ex1->features[ff++] = LN_mal=LN_wspMal(LN_area, LN_peri);
		LN_CenterOfGravity(&res, LN_area, &LN_gx, &LN_gy);
		ex1->features[ff++] = LN_gx;
		ex1->features[ff++] = LN_gy;
		ex1->features[ff++] = LN_bb=LN_wspBB(&res, LN_area,LN_gx,LN_gy);
		ex1->features[ff++] = LN_dan=LN_wspDan(&res, LN_area);
		ex1->features[ff++] = LN_har=LN_wspHar(&B, LN_gx,LN_gy,LN_peri);// 18
// ******************************************* PB *************************************
		ex1->features[ff++] = PB_stdcirc(&B,LN_gx,LN_gy);
		ex1->features[ff++] = PBt1;	//20
		ex1->features[ff++] = PBt2;
// ******************************************* MC *************************************
		featuresMC(&black, &B, LN_area, LN_peri, &MC_Cx, &MC_Cy, &MC_Width, &MC_Length, &MC_Elongation, &MC_Rectangulity, &MC_Compactness, &MC_Iacf);
		ex1->features[ff++] = MC_Cx;
		ex1->features[ff++] = MC_Cy;
		ex1->features[ff++] = MC_Width;
		ex1->features[ff++] = MC_Length;
		ex1->features[ff++] = MC_Elongation;
		ex1->features[ff++] = MC_Rectangulity;
		ex1->features[ff++] = MC_Compactness;
		ex1->features[ff++] = MC_Iacf;

		ex1->features[ff++] = PBf8;		// dlugosc wady po osi dluzszej (30)
		ex1->features[ff++] = PBf9;		// dlugosc wady po osi krotszej
		ex1->features[ff++] = t_xmin;	// wsp x minimalna wady - raczej do cel�w testowych do rysowania elips na radiogramie
		ex1->features[ff++] = t_xmax;	// wsp x minimalna wady - raczej do cel�w testowych do rysowania elips na radiogramie
		ex1->features[ff++] = t_ymin;	// wsp x minimalna wady - raczej do cel�w testowych do rysowania elips na radiogramie
		ex1->features[ff++] = t_ymax;	// wsp x minimalna wady - raczej do cel�w testowych do rysowania elips na radiogramie
	}
	
	_endthread();
}
double PB_getIndexNumber(C_Image_Container *res)
{
	// zwraca numer indeksu dla danej wady z funkcji index
	unsigned long a;
	for(a=0;a<res->GetNumofElements();a++)
		if(res->data[a]>0)
			return res->data[a];
	return -1;

}
double PB_stdcirc(C_Image_Container *in,double gx,double gy)
{
	unsigned int r,k,licznik=0;
	C_Matrix_Container data;
	C_Matrix_Container data1;
	C_Matrix_Container out;
//	double srx,sry;
//	sry  = floor((double)(in->_rows+1)/2 + 0.5);    // poniewaz jest odjeta srednia wiec srodkiem jest zero, czyli polowa dlogosc we wspolrzednych obrazka
//	srx = floor((double)(in->_cols+1)/2 + 0.5);
	data.AllocateData(in->GetNumofElements(),1);
	data.Zeros();
//in->DumpBinary("c:\\b.out");
	for(r=0;r<in->_rows;r++)
		for(k=0;k<in->_cols;k++)
			if(in->GetPixel(r,k)!=0)
				data.data[licznik++] = sqrt(((r+1)-gy)*((r+1)-gy) + ((k+1)-gx)*((k+1)-gx) );

	
	data1.AllocateData(--licznik,1);
	for(r=0;r<licznik;r++)
		data1.data[r] = data.data[r];
//data1.DumpBinary("c:\\dd.out");
	data1.stdcol(&out);
	return out.data[0];
}

void PB_texture(unsigned int current_flaw, unsigned int current_flaw_offset,
				C_Image_Container *org, C_Matrix_Container *x_coord,
				C_Matrix_Container *y_coord, unsigned int current_flaw_size,
				double* out1,double* out2)
{
	double mi,ma,pol,meano1,meano2;
	double liczo1,liczo2,datao1,datao2,mean;
	unsigned long a;
	C_Matrix_Container std;
	liczo1=liczo2=datao1=datao2=mean=0;

	org->std(&std);
	*(out1) = std.data[0];
//...................................
	mi = 65535*2;
	ma = -65535*2;
	
	for(a=0;a<org->GetNumofElements();a++)	{
		mean+=org->data[a];
		if(org->data[a]>0)	{
			if(org->data[a]<mi) mi = org->data[a];
			if(org->data[a]>ma) ma = org->data[a];
		}
	}
	mean/=org->GetNumofElements();
	pol = (ma-mi)/2+mi;
	for(a=0;a<org->GetNumofElements();a++)	{
		if(org->data[a]>pol)	{
			datao1+=org->data[a];
			liczo1++;
		}
		if(org->data[a]<pol && org->data[a]>0)	{
			datao2+=org->data[a];
			liczo2++;
		}
	}
	if(liczo1>0)
		meano1 = datao1/liczo1;
	else
		meano1 = 0;
	if(liczo2>0)
		meano2 = datao2/liczo2;
	else
		meano2 = 0;
	
	if(mean==0)
		*(out2) = -1;
	else
		*(out2) = abs(meano2-meano1)/mean;

}
void featuresMC(C_Image_Container* BwIm,C_Image_Container* BwPer,double Area,double Perimeter,double* Cx,double* Cy,double *Width,double *Length,double *Elongation,double *Rectangulity,double *Compactness,double *Iacf)
{
	// C_Image_Container* BwIm - obraz wej�ciowy czarno-bia�y (0-1) zawieraj�cy wad�
	// C_Image_Container* BwPer - obraz wej�ciowy czarno-bia�y (0-1) zawieraj�cy obw�d wady
	// double Area - parametr wej�ciowy, pole powierzchni wady ( zliczone pixele b�d�ce wad�)
	// double Perimeter - parametr wej�ciowy, pole powierzchni obwodu wady, czyli obw�d (zliczone pixele b�d�ce obwodem)
	// double* Cx - parametr wyj�ciowy, �rodek ci�ko�ci wzgl�dem x
	// double* Cy - parametr wyj�ciowy, �rodek ci�ko�ci wzgl�dem y
	// double *Width - parametr wyj�ciowy, szeroko�� wady ( zawsze kr�tsza ni� d�ugo��)
	// double *Length - parametr wyj�ciowy, d�ugo�� wady ( zawsze d�u�sza ni� szeroko��)
	// double *Elongation - parametr wyj�ciowy, wyd�u�enie (stosunek szeroko�ci do d�ugo�ci)
	// double *Rectangulity - parametr wyj�ciowy, stosunek Area/(Length*Width);
	// double *Compactness - parametr wyj�ciowy, stosunek (Perimeter*Perimeter)/(4*PI*Area)
	// double *Iacf - parametr wyj�ciowy, Lengthering index, stosunek Ia=(Dmax*Dmax*PI)/(4*Area), gdzie Dmax naj�u�sz �rednica wady


	double CGx,CGy,pix,temp,W,L,Elong,Rec,Comp,Ia,D,Dmax,xdim,ydim;
	const double PI = 3.1415926535;
		
	int i,j,xs,xe,ys,ye;
		
	CGx = 0;
	CGy = 0;

	xs=BwIm->_rows-1;
	xe=0;
	ys=BwIm->_cols-1;
	ye=0;

	W =0;
	L = 0;
	Elong = 0;
	Rec = 0;
	Comp = 0;
	Ia=0;
	D=0;
	Dmax=0;

	
	for(i=0;i<BwIm->_rows;i++)
	{
		for(j=0;j<BwIm->_cols;j++)
		{
			BwIm->GetPixel(i,j,pix);
			// Centroid
			if(pix ==1)
			{
				CGx=CGx+i;
				CGy=CGy+j;
			}
			// Width, Length and Elongation coefficient
			if(i<xs)
			{
				xs=i;
			}
			if(i>xe)
			{
				xe=i;
			}
			if(j<ys)
			{
				ys=j;
			}
			if(j>ye)
			{
				ye=j;
			}
		}
	}
	// Width, Length and Elongation coefficient

	W=(double)(xe-xs+1);
	L=(double)(ye-ys+1);
	 
	if(W>L)
	{
		temp=W;
		W=L;
		L=temp;
	}
	 Elong = W/L;
	 if(W<3 && L<3)
	 {
		 Elong = -1;
	 }

	 *Width=W;
	 *Length=L;
	 *Elongation=Elong;

	// Centroid
	if(Area<5)
	{
		CGx=-1;
		CGy=-1;
	}
	else
	{
		CGx = CGx / Area;
		CGy = CGy / Area;
	}

	*Cx = CGx;
	*Cy = CGy;

	//Recatngulity
	Rec = Area/(L*W);
	if(Area<5 || (W<3 && L<3))
	{
		Rec=-1;
	}
	*Rectangulity=Rec;

	//Compactness
	Comp = (Perimeter*Perimeter)/(4*PI*Area);
	if(Perimeter<4)
	{
		Comp = -1;
	}
	*Compactness = Comp;

	//Lengthening index
	
	for(i=0;i<BwPer->_rows;i++)
	{
		for(j=0;j<BwPer->_cols;j++)
		{
			BwPer->GetPixel(i,j,pix);
			if(pix==1)
			{
				xdim = CGx - i;
				ydim = CGy - j;
				D=2*(sqrt((xdim*xdim)+(ydim*ydim)));
				if(D>Dmax)
				{
					Dmax=D;
				}
			}
		}
	}
	Ia=(Dmax*Dmax*PI)/(4*Area);
	if(Dmax<2 || Area<5)
	{
		Ia=-1;
	}
	*Iacf=Ia;	
}


void PB_feat1(unsigned int current_flaw, unsigned int current_flaw_offset,
			  C_Image_Container *res, C_Matrix_Container *x_coord,
			  C_Matrix_Container *y_coord, unsigned int current_flaw_size,
			  double* out1,double* out2,
			  double* out6,
			  double* out7_1,double* out7_2,double* out7_3,double* out7_4,
			  double* out7_5,double* out8,double* out9)
{
	double mean_x=0,mean_y=0;
	unsigned int l, punktyin, punktyout,licznik,pozycja;
	double DD[2];
	double tmp[2];
	double minx,maxx,miny,maxy;
	double cosadl,alfadl,alfadl_copy;
	double dluga_os,krotka_os,ratiod,ratiopkt,osa,osb,dl_osa,dl_osb,delta_osa,delta_osb,pole,e,odl1,odl2,tmp1,tmp2,sk1,sk2,sk3;
	C_Matrix_Container F1,F2, Ftmp, Zbiorcza, wyn;
	C_Matrix_Container xy;
	C_Matrix_Container covariance;
	C_Matrix_Container v_org;
	C_Matrix_Container tmpm;
	C_Matrix_Container rotation;
	char secondvector = 1;
	char mainvector;
    C_Matrix_Container punkty_rot;

	xy.AllocateData(2,x_coord->_rows);	// aby �atwiej kopiowa� by�o
	covariance.AllocateData(2,2);
	
	for(l=0;l<x_coord->GetNumofElements();l++)	{
		mean_x+=x_coord->data[l];
		mean_y+=y_coord->data[l];
	}
	mean_x/=x_coord->GetNumofElements();
	mean_y/=y_coord->GetNumofElements();
	
	for(l=0;l<x_coord->_rows;l++)
		xy.data[l] = y_coord->data[l]-mean_y;
	for(l=x_coord->_rows;l<2*x_coord->_rows;l++)
		xy.data[l] = x_coord->data[l-x_coord->_rows]-mean_x;
	xy.Transpose(); //rx2
//xy.DumpBinary("c:\\xy.dat");	
	cov(&xy,&covariance);
//covariance.DumpBinary("c:\\covariance.out");	// ok 
	PCA(covariance.data,2,2,DD);
//covariance.DumpBinary("c:\\pca.out");		//ok
	
	covariance.CloneObject(&v_org);
	if(DD[0]>DD[1])	
	{	mainvector = 0; secondvector = 1;}
	else
	{	mainvector = 1; secondvector = 0;}

	if(mainvector>0)	{
		tmp[0] = covariance.data[0];	tmp[1] = covariance.data[2];	//organizacja - kolumny z matlaba s� liniowo po kolei
		covariance.data[0] = covariance.data[1];
		covariance.data[2] = covariance.data[3];
		covariance.data[1] = tmp[0];	covariance.data[3] = tmp[1];
	}		// ok
//covariance.DumpBinary("c:\\pca2.out");
	covariance.Transpose();
	xy.Transpose();
	punkty_rot.AllocateData(xy._rows,xy._cols);
	MatrixMulti(&covariance,&xy,&punkty_rot);	//ok
//xy.DumpBinary("c:\\xy1.out");
//covariance.DumpBinary("c:\\pca1.out");
//punkty_rot.DumpBinary("c:\\punktyrot.out");
	tmpm.AllocateData(1,punkty_rot._cols);
	for(l=0;l<punkty_rot._cols;l++)
		tmpm.data[l] = punkty_rot.GetPixel(0,l);
	tmpm.getMinMax(minx,maxx);
	for(l=0;l<punkty_rot._cols;l++)
		tmpm.data[l] = punkty_rot.GetPixel(1,l);
	tmpm.getMinMax(miny,maxy);
	if(maxy==miny)	{
		*(out1) = -1;
		*(out8) = -1;
		*(out9) = -1;
	}
	else	{
		*(out1) = (maxx-minx)/(maxy-miny);
		*(out8) = maxx-minx;
		*(out9) = maxy-miny;
	}

// ************************************ FEAT2 ******************************************************
	
	cosadl = (v_org.GetPixel(0,mainvector))/
		(sqrt(v_org.GetPixel(0,mainvector)*v_org.GetPixel(0,mainvector)+v_org.GetPixel(1,mainvector)*v_org.GetPixel(1,mainvector) ) );
	alfadl = alfadl_copy = abs(acos(cosadl)*180/M_PI);
	if(alfadl>90)
		alfadl = abs(alfadl-180)/90;
	else
		alfadl/=90;
	*(out2) = alfadl;

// ************************************ FEAT6 ******************************************************
	
	if(DD[secondvector]==0)
		*(out6) = -1;
	else
		*(out6) = DD[mainvector]/DD[secondvector];

// ************************************ FEAT7 ******************************************************

	alfadl = alfadl_copy;
	alfadl = acos(cosadl);
	if(v_org.GetPixel(1,mainvector)<0) alfadl = -alfadl;
	rotation.AllocateData(2,2);
	rotation.SetPixel(0,0,cos(-alfadl)); rotation.SetPixel(0,1,-sin(-alfadl));
	rotation.SetPixel(1,0,sin(-alfadl)); rotation.SetPixel(1,1,cos(-alfadl));
	xy.Transpose();punkty_rot.AllocateData(xy._rows,xy._cols);
	MatrixMulti(&xy,&rotation,&punkty_rot);	
//punkty_rot.DumpBinary("c:\\punktyrot7.out");
//rotation.Dump("c:\\rotation.m");
//xy.DumpBinary("c:\\xy.dat");
	ratiod = DD[mainvector]/DD[secondvector];
	tmpm.AllocateData(1,punkty_rot._rows);
	for(l=0;l<punkty_rot._rows;l++)
		tmpm.data[l] = punkty_rot.GetPixel(l,0);
	tmpm.getMinMax(minx,maxx);
	for(l=0;l<punkty_rot._rows;l++)
		tmpm.data[l] = punkty_rot.GetPixel(l,1);
	tmpm.getMinMax(miny,maxy);
	ratiopkt = (maxx-minx)/(maxy-miny);
	osa = ratiod*sqrt(v_org.GetPixel(0,mainvector)*v_org.GetPixel(0,mainvector)+
					  v_org.GetPixel(1,mainvector)*v_org.GetPixel(1,mainvector));
	osb = sqrt(v_org.GetPixel(0,secondvector)*v_org.GetPixel(0,secondvector)+
			   v_org.GetPixel(1,secondvector)*v_org.GetPixel(1,secondvector));
	delta_osa = (2*maxx - 0.01*maxx)/50;
	delta_osb = (2*maxy - 0.01*maxy)/50;
	licznik = 0;
	F1.AllocateData(1,2);
	F2.AllocateData(1,2);
	Ftmp.AllocateData(1,2);
	Zbiorcza.AllocateData(51*51,4);	//[dl_osa dl_osb pktyinout pktyinout]
	wyn.AllocateData(51*51,1); wyn.Zeros();
	for(dl_osa=0.01*maxx;dl_osa<2*maxx;dl_osa+=delta_osa)
		for(dl_osb=0.01*maxy;dl_osb<2*maxy;dl_osb+=delta_osb)
		{
			pole = M_PI*dl_osa*dl_osb;
			if(dl_osa>dl_osb)	{
				dluga_os = dl_osa;
				krotka_os = dl_osb;
			} else {
             dluga_os = dl_osb;
             krotka_os = dl_osa;
			}
			e = sqrt(1-(krotka_os*krotka_os)/(dluga_os*dluga_os));
			Ftmp.data[0] = -e*dluga_os; Ftmp.data[1] = 0;
			MatrixMulti(&Ftmp,&rotation,&F1);	
			Ftmp.data[0] = e*dluga_os; Ftmp.data[1] = 0;
			MatrixMulti(&Ftmp,&rotation,&F2);	
			punktyin = 0; punktyout = 0;
			for(l=0;l<xy._rows;l++)
			{
				tmp1 = xy.GetPixel(l,0)-F1.data[0];
				tmp2 = xy.GetPixel(l,1)-F1.data[1];
				odl1 = sqrt(tmp1*tmp1+tmp2*tmp2);
				tmp1 = xy.GetPixel(l,0)-F2.data[0];
				tmp2 = xy.GetPixel(l,1)-F2.data[1];
				odl2 = sqrt(tmp1*tmp1+tmp2*tmp2);
				if(odl1+odl2<=2*dluga_os)
					punktyin++;
				else
					punktyout++;
			}
			if(punktyin!=0 && punktyout!=0)
			{
				Zbiorcza.SetPixel(licznik,0,dluga_os); Zbiorcza.SetPixel(licznik,1,krotka_os);
				sk1 = abs( (pole-punktyin)/punktyout);
				sk2 = abs( dluga_os/krotka_os-ratiopkt );
				sk3 = ((double)punktyin+(double)punktyout)/(double)punktyin;
				wyn.data[licznik] = (0.5*sk1+0.5*sk3+0.01*sk2)/(0.5+0.5+0.01);
				Zbiorcza.SetPixel(licznik,2,pole-punktyin); Zbiorcza.SetPixel(licznik,3,punktyout);
				licznik++;		
			}
		}
//wyn.DumpBinary("c:\\wyn.dat");
		minx = 1e303;

		bool hits=FALSE;
		for(l=0;l<licznik;l++)	{
			if(wyn.data[l]<minx){
				minx = wyn.data[l];
				pozycja = l;
				hits = true;	// oznacza ze choc raz program tu wszed�, czyli jest jakie� minimum
			}
		}

		if(hits)	{
			*(out7_1) = Zbiorcza.GetPixel((unsigned long)pozycja,0);
			*(out7_2) = Zbiorcza.GetPixel((unsigned long)pozycja,1);
			*(out7_3) = Zbiorcza.GetPixel((unsigned long)pozycja,2);
			*(out7_4) = Zbiorcza.GetPixel((unsigned long)pozycja,3);
			*(out7_5) = alfadl;
		} else {
			*(out7_1) = -1;
			*(out7_2) = -1;
			*(out7_3) = -1;
			*(out7_4) = -1;
			*(out7_5) = alfadl;
		}

	
}

// ara generalnie nie potrzebne bo jest lizone z parametr�w wad
unsigned long LN_Area(C_Image_Container* ic)
{
	unsigned long i, j;
	unsigned long pole = 0;
	for(i = 0; i < ic->_rows; i++)
	{
		for(j = 0; j < ic->_cols; j++)
		{
			if(ic->GetPixel(i, j) != 0)
			{
				pole = pole + 1;
			}
		}
	}
	return(pole);
}

double LN_Perimeter(C_Image_Container *in, C_Image_Container *out)
{
	int M[3][3] = { {7, 1, 7}, {1, 1, 1}, {7, 1, 7} };
	int i, j;
	double obwod = 0;
	//int maks, miin;
	C_Image_Container* pom = new C_Image_Container();
//	pom->AllocateData(in->_rows, in->_cols);
	//in->getMinMax(maks, miin);
	
	//przepisanie in do pom
	in->CloneObject(pom);
/*	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			pom->SetPixel(i, j, in->GetPixel(i, j));
		}
	}*/
	
	LN_morf(pom, M, false);
	
	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			out->SetPixel(i, j, in->GetPixel(i, j) - pom->GetPixel(i, j));
			if(out->GetPixel(i, j) != 0) obwod++;
		}
	}

	pom->FreeData();
	delete(pom);
	return(obwod);
}

void LN_morf(C_Matrix_Container* F, int M[3][3], bool bit)
{
	int p1 = 0, p2 = 0, p = 0, c, pasuje = 1, m, n;
	unsigned int i, j;
	double pom0, pom1; //Te dwie zmienne s�u�� jako zero i jeden, bo macie� jest dziwnie wyskalowana
	//int piksel1, piksel2;
	C_Matrix_Container B;
	C_Matrix_Container A;
	B.AllocateData(F->_rows + 2, F->_cols + 2);
	A.AllocateData(F->_rows + 2, F->_cols + 2);
	B.Zeros();
	A.Zeros();
	for(i = 0; i < F->_rows; i++)
	{
		for(j = 0; j < F->_cols; j++)
		{
			A.SetPixel(i + 1, j + 1, F->GetPixel(i, j));
		}
	}
	A.getMinMax(pom0, pom1);
//A.DumpBinary("C:\\a.out");
//F->DumpBinary("C:\\f.out");
	//double B[A->_rows][A->_cols];
	//c = int((M->_cols) / 2);	
	c = 1;
	if(bit)	//wybrano pogrubianie/�cienianie
	{
		for(i = c; i < A._rows - c; i++)
		{
			for(j = c; j < A._cols - c; j++)
			{
				pasuje = 1;
				for(m = -c; m <= c; m++)
				{
					for(n = -c; n<= c; n++)
					{	
						if((A.GetPixel(i + m, j + n) != pom1 && M[m + c][n + c] == 1) || 
							(A.GetPixel(i + m, j + n) != pom0 && M[m + c][n + c] == 0))
							pasuje = 0;
					}
				}
				if(pasuje == 1)
				{
					if(M[c][c] == 1)
						B.SetPixel(i, j, 0);
					else if(M[c][c] == 0)
						B.SetPixel(i, j, 1);
					else
					{
						if(B.GetPixel(i, j) == 0) B.SetPixel(i, j, pom1);
						else B.SetPixel(i, j, pom0);
					}
				}
			}
		}
	}
	else //wybrano "hit-or-miss"
	{
		for(i = c; i < A._rows - c; i++)
		{
			for(j = c; j < A._cols - c; j++)
			{
				pasuje = 1;
				for(m = -c; m <= c; m++)
				{
					for(n = -c; n<= c; n++)
					{	
						if((A.GetPixel(i + m, j + n) != pom1 && M[m + c][n + c] == 1) ||
							(A.GetPixel(i + m, j + n) != pom0 && M[m + c][n + c] == 0))
							pasuje = 0;
					}
				}
				if(pasuje == 1) B.SetPixel(i, j, pom1);
				else B.SetPixel(i, j, pom0);
			}
		}
	}
	
	//doPliku("dumpB.txt", B);
//B.DumpBinary("C:\\b.out");
	for(i = 0; i < F->_rows; i++)
	{
		for(j = 0; j < F->_cols; j++)
		{
			F->SetPixel(i, j, B.GetPixel(i + 1, j + 1));
		}
	}
}

//Funkcja licz�ca �rodek ci�ko�ci obiektu.
//A - powierzchnia
//Gx i Gy to wynik - wyliczone �rodki ci�kosci.
void LN_CenterOfGravity(C_Image_Container *in, double A, double *Gx, double *Gy)
{
	int i, j;
	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			if(in->GetPixel(i, j) != 0)
			{
				(*Gx) += j + 1;
				(*Gy) += i + 1;
			}
		}
	}
	//+1 �eby zachowa� zgodno�� z matlabem
	if(A > 0)
	{
		(*Gx) = *(Gx) / A;
		(*Gy) = *(Gy) / A;
	}
	else
	{
		(*Gx) = 0;
		(*Gy) = 0;
	}
}
double LN_wspMal(double A, double p)
{
	if(A > 0) return((p / (2 * sqrt(3.14125 * A))) - 1);
	else return(-1);
}

//Wsp�czynnik Blaira-Blissa
//A - powierzchnia
//Gx - �rodek ci�ko�ci w kierunku x
//Gy - �rodek ci�ko�ci w kierunku y
double LN_wspBB(C_Image_Container *in, double A, double Gx, double Gy)
{
	int i, j;
	double pom = 0;
	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			if(in->GetPixel(i, j) != 0)
			{
				pom = pom + pow(abs(i - Gy), 2) + pow(abs(j - Gx), 2);
			}
		}
	}
	if(pom > 0) return(A / sqrt(2 * 3.14125 * pom));
	else return(-1);
}

//Wsp�czynnik Danielssona
double LN_wspDan(C_Image_Container *in, double A)
{
//	FILE *plik;
	int i, j;
	int M[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
	unsigned long li = 0; //suma po wszystkich odleg�o�ciach
	bool isZeros = false;
	C_Matrix_Container* pom2 = new C_Image_Container();
	C_Matrix_Container* fO = new C_Image_Container(); // Funkcja odleg�o�ci
	pom2->AllocateData(in->_rows, in->_cols);
	pom2->Zeros();
	fO->AllocateData(in->_rows, in->_cols);
	fO->Zeros();
	for(i=0;i<pom2->GetNumofElements();i++)
		if(in->data[i] != 0)
			pom2->data[i] = 1;

/*	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			if(in->GetPixel(i, j) != 0)
			{
				pom2->SetPixel(i, j, 1);
			}
		}
	}*/
	//Tutaj pom zawiera jedynki tam, gdzie in mia� niewiadomo co.
	//Teraz trzeba zrobi� kolejne erozje i dodawa� wyniki.
	while(!isZeros)//dop�ki kolejne erozje zmieniaj� co� w obrazie
	{
		//Erozja pom2
		LN_morf(pom2, M, false);
//		pom2->DumpBinary("c:\\pom.bin");
		//doPliku("pom2.txt", pom2);
		//Sprawdzenie, czy pom2 jest z�o�one z samych zer. Je�eli tak, to break.
		isZeros = true;
		for(i = 1; i < pom2->_rows - 1; i++)
		{
			for(j = 1; j < pom2->_cols - 1; j++)
			{
				if(pom2->GetPixel(i, j) != 0)
				{
					isZeros = false;
					break;
				}
			}
			if(!isZeros) break;
		}

		//Dodanie pom2 do fO
		for(i = 1; i < pom2->_rows - 1; i++)
		{
			for(j = 1; j < pom2->_cols - 1; j++)
			{
				fO->SetPixel(i, j, fO->GetPixel(i, j) + pom2->GetPixel(i, j));
			}
		}
	}
	for(i = 1; i < pom2->_rows - 1; i++)
	{
		for(j = 1; j < pom2->_cols - 1; j++)
		{
			if(fO->GetPixel(i, j) != 0) 
			{
				li += fO->GetPixel(i, j);
			}
		}
	}
	delete pom2;
	delete fO;
	if(li > 0) return(pow((double)A, 3) / pow((double)li, 2));
	else return(-1);
}

double LN_wspHar(C_Image_Container *in, double gx, double gy, double ilePikseli)
{
	int i, j;
	double suma1 = 0, suma2 = 0;
	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			if(in->GetPixel(i, j) != 0) 
			{
				suma1 += sqrt(pow((gx - j + 1), 2) + pow((gy - i + 1), 2));
				suma2 += pow((gx - j + 1), 2) + pow((gy - i + 1), 2);
			}
		}
	}
	if(ilePikseli * suma2 != 1) return(sqrt(pow(suma1, 2) / (ilePikseli * suma2 - 1)));
	else return(-1);
}

void GetMinMax(double* data, double& min, double& max,unsigned int ile)
{
	unsigned long r;
	min = 1.7e308;
	max = -1.7e308;;
	for(r=0;r<ile;r++)	{
		if(data[r]<min)	min = data[r];
		if(data[r]>max) max = data[r];
	}

}
void CutImage(C_Image_Container *im, C_Image_Container *out, double *xc, double *yc, unsigned int ile)
{
	unsigned int i;
	double minx,maxx,miny,maxy;
	GetMinMax(xc, minx, maxx,ile);
	GetMinMax(yc, miny, maxy,ile);
	out->AllocateData((unsigned int)(maxy-miny+1),(unsigned int)(maxx-minx+1));
	out->Zeros();
	for(i=0;i<ile;i++)
		out->SetPixel((unsigned int)(yc[i]-miny),(unsigned int)(xc[i]-minx),im->GetPixel((unsigned int)yc[i],(unsigned int)xc[i]));
}

void cov(C_Matrix_Container* X, C_Matrix_Container* Y)
// liczy covariancje macierzy X (po kolumnach)
{
	double sredniax, sredniay;
	unsigned long row,colx,coly;
	double covariance;

	for(colx=0;colx<X->_cols;colx++)
		for(coly=0;coly<X->_cols;coly++)	{
			covariance = 0;
			sredniax = X->Mean(NULL,colx);
			sredniay = X->Mean(NULL,coly);
			for(row=0;row<X->_rows;row++)
				covariance+=(X->GetPixel(row,colx) - sredniax)*(X->GetPixel(row,coly) - sredniay);
			covariance/=X->_rows-1;
			Y->SetPixel(colx,coly,covariance);
		}
}

unsigned int unique(C_Matrix_Container* input,C_Matrix_Container* licznosc)
// w licznosc znajduje sie informacja ile bylo pixeli kazdego rodzaju. 
{
	unsigned int a,ile=0,licznik=0;
	licznosc->AllocateData(1,65536);
	licznosc->Zeros();
	double tmp;
	for(a=0;a<input->GetNumofElements();a++)	{
		tmp = input->data[a];
		licznosc->data[(unsigned int)tmp]++;
	}

	for(a=0;a<licznosc->GetNumofElements();a++)
		if(licznosc->data[a]!=0)
			ile++;	// ile roznych pixeli

	return ile;
}

BOOL isintable(double* tab,double val,unsigned int tab_size)
{
	unsigned int a;
	for(a=0;a<tab_size;a++)
		if(tab[a]==val)	{
			return TRUE;
			break;
		}
	return FALSE;
}


void RemoveVal(C_Matrix_Container* input,double val)
{
// kasuje warto�c val z vektora 1xn
	unsigned int a,ile=0,licznik=0;
	C_Matrix_Container tmp;

	for(a=0;a<input->GetNumofElements();a++)
		if(input->data[a]!=val)
			ile++;

	tmp.AllocateData(1,ile);
	for(a=0;a<input->GetNumofElements();a++)
		if(input->data[a]!=val)
			tmp.data[licznik++] = input->data[a];

	tmp.CloneObject(input);
}
BOOL MatrixMulti(C_Matrix_Container* in1, C_Matrix_Container* in2, C_Matrix_Container* out)
{
	unsigned int w,k,i;
	double wartosc,st;
	if(in1->_cols!=in2->_rows)
		return FALSE;
	if( (out->_cols!=in2->_cols)||(out->_rows!=in1->_rows) )
		return FALSE;

	out->Zeros();
	for(w=0; w<out->_rows; w++) 
		for(k=0; k<out->_cols; k++)
			for(i=0; i<in1->_cols; i++)	{
				wartosc = in1->GetPixel(w,i)*in2->GetPixel(i,k);
				st = out->GetPixel(w,k);
				out->SetPixel(w,k,st+wartosc); 
			}
	return TRUE;
}