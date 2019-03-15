#include "Cpp_Lib.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "stdlib.h"
#include <stdio.h>
#include <tchar.h>
#include <crtdbg.h>
#include <process.h>
#include <assert.h>

typedef long int integer;

#define VERBOSE
#define MAX_FLAW_AREA 1

struct PIXEL
{
	double x;
	double y;
};
struct PROSTA
{
	double a;
	double b;
};

struct ELIPSA
{
	PIXEL d1,d2;	// wierzcho³ki dla d³u¿szej osi
	PIXEL k1,k2;	// wierzcho³ki dla krótszej osi
	PIXEL srodek;
	double dl_os;	// dlugosc pó³osi d³u¿szej
	double kr_os;
};

struct point
{
    double x; //X POSITION
    double y; //Y POSITION
    point *next; //POINTER TO NEXT NODE IN THE LIST
    point *prev; //POINTER TO PREVIOUS NODE IN THE LIST
    double angle; //INTERMEDIATE ANGLE VALUE STORAGE
};

const int NumPoints = 4;
point* firstPoint;

void addPoint(point Point); //ADDS POINT TO DOUBLELY LINKED LIST (USED DURING SORTING)
double findAngle(double x1, double y1, double x2, double y2); //FIND ANGLE GIVEN TWO POINTS
void grahamInit(point* tempPoints); //INITIALIZE VARIABLES, RANDOMLY GENERATE POINTS,
                   //LOCATE MIN POINT, AND SORT POINTS BY RELATIVE ANGLES
void grahamMain(point* tempPoints); //SETUP, RUN GRAHAM'S SCAN, AND DISPLAY RESULTS
void grahamScan(point *P); //ACTUAL GRAHAM'S SCAN PROCEDURE
bool isConvexPoint(point *P); //TEST POINT FOR CONVEXITY
void printPoints(PIXEL* pBorder);

bool IsInside(PIXEL& point,PIXEL* points,unsigned int wadanum,double tol);
bool prvTestEdge(PIXEL& point, PIXEL& p0, PIXEL& p1,double tol); 
int prvTestInside(PIXEL& point, PIXEL& p0, PIXEL& p1); 
inline int sgn(double val);
PIXEL prvCheckNumZero(PIXEL point, double tol);
double mround(double in);

void evaluate_ellipse(C_Matrix_Container* x1, C_Matrix_Container* y1,double kat,double kat1, double kr_os, double dl_os, PIXEL* srodek,ELIPSA* elipsa);
void PB_feat1(unsigned int current_flaw, C_Image_Container *res, C_Matrix_Container *x_coord, C_Matrix_Container *y_coord, double* out1,double* out2,double* out6,double* out7_1,double* out7_2,double* out7_3,double* out7_4,double* out7_5);
BOOL MatrixMulti(C_Matrix_Container* in1, C_Matrix_Container* in2, C_Matrix_Container* out);
void cov(C_Matrix_Container* X, C_Matrix_Container* Y);
void PCA(double* a,integer Na,integer LDA,double *D);
void merge(unsigned int current_flaw,
		   C_Matrix_Container* x1,C_Matrix_Container* y1,C_Image_Container* wada1,
		   C_Matrix_Container* x2,C_Matrix_Container* y2,C_Image_Container* wada2,
		   C_Image_Container* result,C_Image_Container* bw,
		   C_Image_Container* out,C_Matrix_Container* outx,C_Matrix_Container* outy);

BOOL kryterium3221(unsigned int ind,C_Matrix_Container* feat);
BOOL kryterium3223(unsigned int ind1,unsigned int ind2,double dist,C_Matrix_Container* feat);
BOOL isconected(unsigned int ind,C_Matrix_Container* con,unsigned int ile,unsigned int& ind2);

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"wskclassifier");	// nazwa w ADDIP
	strcpy_s(definition->version,MAX_VER,"1.0");	// wersja (nie ma wiêkszego znaczenia)
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = wskclassifier(IMAGE,IMAGE,IMAGE,MATRIX,MATRIX,STRING)"); // SYNTAX (nie ma wiêkszego znaczenia)
	definition->numofout = 1;	// ilosc parametrów wyœciowych
	definition->numofin = 6;	// ilosc parametrów wejœciowych
	definition->typesofin[0] = IMAGE;definition->typesofin[1] = IMAGE;definition->typesofin[2] = IMAGE;definition->typesofin[3] = MATRIX;definition->typesofin[4] = MATRIX;definition->typesofin[5] = STRING;	// typy wejœcia
	definition->typesofout[0] = IMAGE; // typy wyjœcia
	definition->interface_version = 2;	// wersja interface
	strcpy_s(definition->description,MAX_DESC,"Developed by PB");	// cokolwiek
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
	C_Image_Container result;
	C_Matrix_Container *dist;
	C_Matrix_Container *features;
	C_Matrix_Container x1;
	C_Matrix_Container x2;
	C_Matrix_Container y1;
	C_Matrix_Container y2;
	C_Image_Container wada1;
	C_Image_Container wada2;
	C_Image_Container out;
	C_Matrix_Container outx;	// po³¹czone indexy
	C_Matrix_Container outy;
	C_Matrix_Container output;
	C_Matrix_Container connections;	// przechowuje informacjê o s¹siadach jeœli s¹
	unsigned int licznik_sasiadow=0; // ile jest wad sasiadujacych
	FILE *stream;
	char* filename;

	unsigned int current_flaw,current_flaw1,ind2;
	double odl;
	char buffor[10];
	
	double imagesize;
	original = (C_Image_Container*)intab[0]->data;	// tu dokonujemy konwersji bo w intab wszystko jest trzymane jako void*
	indexed = (C_Image_Container*)intab[1]->data;
	bw = (C_Image_Container*)intab[2]->data;
	dist = (C_Matrix_Container*)intab[3]->data;
	features = (C_Matrix_Container*)intab[4]->data;
	filename = (char*)intab[5]->data;
	bw->CloneObject(&result);

	if(original->_cols!=indexed->_cols)
		err.SetUserData("wskclassifier: Input images differ in size - results may be improper");
	else
		if(original->_cols!=bw->_cols)
			err.SetUserData("wskclassifier: Input images differ in size - results may be improper");
		else
			if(original->_rows!=indexed->_rows)
				err.SetUserData("wskclassifier: Input images differ in size - results may be improper");
			else
				if(original->_rows!=bw->_rows)
					err.SetUserData("wskclassifier: Input images differ in size - results may be improper");

	
	if( fopen_s( &stream, filename, "w" ) != 0 )	{
		err.SetError("wskclassifier: Cant open report file"); return;}

	fprintf_s(stream,"PRELEMINARY CLASSIFICATION REPORT\n\n\n");
/* ------------------------------ 1. KASOWANIE NIE WAD --------------------------------------------------*/
	imagesize = bw->GetNumofElements();	// ilosc wszystkich pixeli
	connections.AllocateData(features->_rows*2,2);	// dla bezpieczenstwa razy 2 gdy kilka wad bedzie sasiadowalo
	connections.Zeros();

	for(current_flaw=23;current_flaw<features->_rows-1;current_flaw++)	{	// po wszystkich wadach
		fprintf_s(stream,"-----------------------------------------------------------\n");
		fprintf_s(stream,"Flaw index: %d\n",current_flaw);
		if( (features->GetPixel(current_flaw,10)*100)/imagesize>MAX_FLAW_AREA )	{
			fprintf_s(stream,"Not classified as flaw due to area larger than %d%% of total image\n",MAX_FLAW_AREA);
			continue;		// jesli wada wiêksza niz 1% obrazka to kwalifikujemy jak nie wadê
		}
		if(kryterium3221(current_flaw,features))	{
			fprintf_s(stream,"Classified as crack according to the rule 3.2.2.1\n");
			continue;		// w tym miejscu informacja ze to moze byc pekniecie i przes³anie do analizy kraków
		}
		if(isconected(current_flaw,&connections,licznik_sasiadow,ind2))	{
			fprintf_s(stream,"This flaw has a neighbour %d according to the rule 3.2.2.3\n",ind2);
			continue;
		}


		for(current_flaw1=current_flaw+1;current_flaw1<features->_rows;current_flaw1++)	// kazda z kazd¹
		{
		current_flaw1=56; //debug
			odl = dist->GetPixel(current_flaw1,current_flaw);	// odleglosc z funkcji kamila - odleg³oœæ pomiêdzy dwiema wadami 
			if(odl==0.0)
				continue;
/*debug*/	if(TRUE||kryterium3223(current_flaw,current_flaw1,odl,features))
			{
				connections.SetPixel(licznik_sasiadow,0,current_flaw1);
				connections.SetPixel(licznik_sasiadow++,1,current_flaw);
				fprintf_s(stream,"This flaw has a neighbour %d according to rule 3.2.2.3\n",current_flaw1);
				// sprawdzenie warunku ze srednicami i odleglosciami
				_snprintf_s(buffor,10,"x%d.out",current_flaw);
				if(!x1.ReadBinary(buffor)) { err.SetError("wskclassifier: File not found"); return;}
				_snprintf_s(buffor,10,"y%d.out",current_flaw);
				if(!y1.ReadBinary(buffor)) { err.SetError("wskclassifier: File not found"); return;}
				_snprintf_s(buffor,10,"bw%d.out",current_flaw);
				if(!wada1.ReadBinary(buffor)) { err.SetError("wskclassifier: File not found"); return;}

				_snprintf_s(buffor,10,"x%d.out",current_flaw1);
				if(!x2.ReadBinary(buffor)) { err.SetError("wskclassifier: File not found"); return;}
				_snprintf_s(buffor,10,"y%d.out",current_flaw1);
				if(!y2.ReadBinary(buffor)) { err.SetError("wskclassifier: File not found"); return;}
				_snprintf_s(buffor,10,"bw%d.out",current_flaw1);
				if(!wada2.ReadBinary(buffor)) { err.SetError("wskclassifier: File not found"); return;}
				
				merge(current_flaw,
						   &x1,&y1,&wada1,
						   &x2,&y2,&wada2,
						   &result,bw,
						   &out,&outx,&outy);
				
			//return;	// do debug tylko
			fclose( stream );} //current_flaw1
			break; // nie sprawdzamy czy sasiaduje kilka !!!
		} // kryterium 3223
	}	//current_flaw

	fclose( stream );
	outtab[0]->AssignValue((void*)&result,err);
}

void merge(unsigned int current_flaw,
		   C_Matrix_Container* x1,C_Matrix_Container* y1,C_Image_Container* wada1,
		   C_Matrix_Container* x2,C_Matrix_Container* y2,C_Image_Container* wada2,
		   C_Image_Container* result,C_Image_Container* bw,
		   C_Image_Container* out,C_Matrix_Container* outx,C_Matrix_Container* outy)
{
	C_Matrix_Container results_kryterium1;
	C_Matrix_Container results_kryterium2_1;
	C_Matrix_Container results_kryterium2_2;
	PIXEL cent1;
	PIXEL cent2;		// srodki elips
	PROSTA e1e2;	// prosta ³¹cz¹ca œrodki ellips
	PROSTA os;	// prosta  - os elipsy
	ELIPSA e1,e2;
	double unused;
	double katy[4];	// k¹ty pomiêdzy prost¹ ³¹czoc¹ srodek a poszczególnumi osiami w 2 elipsach
	BOOL dl_os_e1,kr_os_e1,dl_os_e2,kr_os_e2;		// true - OX - ktora os daje mniejszy k¹t
	PIXEL border[5];	// punkty graniczne obszaru wyznaczonego z warunków na osie (odpowiednie wierzcho³ki elipsy (pierwszy wierzcholek jest powtorzony na koncu - isinside tego wymaga)
	unsigned int g_minx,g_maxx,g_miny,g_maxy,a,xx,yy;	// zakresy do wype³niania obszaru wyznaczonego przez wierzcho³ki
	PIXEL tmppoint;

	results_kryterium1.AllocateData(1,4);
	results_kryterium2_1.AllocateData(1,4);
	results_kryterium2_2.AllocateData(1,4);
	//*********************** DWIE WADY RAZEM ************************************
//				merge(&wada1, &x1, &y1, &wada2, &x2, &y2, &out, &outx, &outy); // out - dane dla dwóch po³¹czonych wad
//				PB_feat1(current_flaw,
//						&out, &outx, &outy,
//						&unused,&results_kryterium1.data[0],//angle,longer,shorter,angle
//						&unused, &results_kryterium1.data[1], &results_kryterium1.data[2], &unused, &unused, &results_kryterium1.data[3]);
	//****************************************************************************
	//********************** ALG 2 *********************************************** (to jest w features - ale w tej funkcjo zmieniono definicje k¹ta!!!
				cent1.x = x1->Mean(NULL,0); cent1.y = y1->Mean(NULL,0);	// wyznaczanie srodków elips
				cent2.x = x2->Mean(NULL,0); cent2.y = y2->Mean(NULL,0);
				PB_feat1(current_flaw,
						wada1, x1, y1,
						&unused,&results_kryterium2_1.data[0],//angle,longer,shorter,angle
						&unused, &results_kryterium2_1.data[1], 
						&results_kryterium2_1.data[2], &unused, &unused, &results_kryterium2_1.data[3]);

				PB_feat1(current_flaw,
						wada2, x2, y2,
						&unused,&results_kryterium2_2.data[0],//angle,longer,shorter,angle
						&unused, &results_kryterium2_2.data[1], 
						&results_kryterium2_2.data[2], &unused, &unused, &results_kryterium2_2.data[3]);


				evaluate_ellipse(x1,y1,results_kryterium2_1.data[0],results_kryterium2_1.data[3],
								 results_kryterium2_1.data[2],
								 results_kryterium2_1.data[1],
								 &cent1, &e1); // wyznacza parametry elipsy 1

				evaluate_ellipse(x2,y2,results_kryterium2_2.data[0],results_kryterium2_1.data[3],
								 results_kryterium2_2.data[2],
								 results_kryterium2_2.data[1],
								 &cent2, &e2); // wyznacza parametry elipsy 2

				// prosta ³¹cz¹ca œrodki elips
				e1e2.a = (cent2.y-cent1.y)/(cent2.x-cent1.x);
				e1e2.b = cent2.y-e1e2.a*cent2.x;
				// prosta os elipsa 1 d³u¿sza
				os.a = (e1.d2.y-e1.d1.y)/(e1.d2.x-e1.d1.x);
				os.b = e1.d2.y-os.a*e1.d2.x;
				// wyznacznie k¹ta pomiêdzy e1e2 i os http://www.math.edu.pl/kat-miedzy-dwiema-prostymi
				katy[0] = abs(atan((os.a-e1e2.a)/(1+os.a*e1e2.a)));
				// prosta os elipsa 1 krotsza
				os.a = (e1.k2.y-e1.k1.y)/(e1.k2.x-e1.k1.x);
				os.b = e1.k2.y-os.a*e1.k2.x;
				// wyznacznie k¹ta pomiêdzy e1e2 i os http://www.math.edu.pl/kat-miedzy-dwiema-prostymi
				katy[1] = abs(atan((os.a-e1e2.a)/(1+os.a*e1e2.a)));
				// prosta os elipsa 2 d³u¿sza
				os.a = (e2.d2.y-e2.d1.y)/(e2.d2.x-e2.d1.x);
				os.b = e2.d2.y-os.a*e2.d2.x;
				// wyznacznie k¹ta pomiêdzy e1e2 i os http://www.math.edu.pl/kat-miedzy-dwiema-prostymi
				katy[2] = abs(atan((os.a-e1e2.a)/(1+os.a*e1e2.a)));
				// prosta os elipsa 2 krotsza
				os.a = (e2.k2.y-e2.k1.y)/(e2.k2.x-e2.k1.x);
				os.b = e2.k2.y-os.a*e2.k2.x;
				// wyznacznie k¹ta pomiêdzy e1e2 i os http://www.math.edu.pl/kat-miedzy-dwiema-prostymi
				katy[3] = abs(atan((os.a-e1e2.a)/(1+os.a*e1e2.a)));

				dl_os_e1=kr_os_e1=dl_os_e2=kr_os_e2=false;
				if(katy[0]>katy[1])
					dl_os_e1 = true;	// dluga os ma wiekszy k¹t
				else
					kr_os_e1 = true;
				if(katy[2]>katy[3])
					dl_os_e2 = true;
				else
					kr_os_e2 = true;

				if(dl_os_e1)	{	// bierzemy wierzcho³ki z osi d³ugiej
					border[0].x = e1.d1.x; border[0].y = e1.d1.y;
					border[1].x = e1.d2.x; border[1].y = e1.d2.y;
				} else {
					border[0].x = e1.k1.x; border[0].y = e1.k1.y;
					border[1].x = e1.k2.x; border[1].y = e1.k2.y;
				}
				if(dl_os_e2)	{	// bierzemy wierzcho³ki z osi d³ugiej
					border[2].x = e2.d1.x; border[2].y = e2.d1.y;
					border[3].x = e2.d2.x; border[3].y = e2.d2.y;
				} else {
					border[2].x = e2.k1.x; border[2].y = e2.k1.y;
					border[3].x = e2.k2.x; border[3].y = e2.k2.y;
				}

				g_minx = INT_MAX ;
				for(a=0;a<4;a++)
					if((unsigned int)border[a].x<g_minx)
						g_minx = (unsigned int)mround(border[a].x);	// skrajny minimalny x

				g_maxx = 0;
				for(a=0;a<4;a++)
					if((unsigned int)border[a].x>g_maxx)
						g_maxx = (unsigned int)mround(border[a].x);	// skrajny maxymalny x

				g_miny = INT_MAX;
				for(a=0;a<4;a++)
					if((unsigned int)border[a].y<g_miny)
						g_miny = (unsigned int)mround(border[a].y);	// skrajny minimalny y

				g_maxy = 0;
				for(a=0;a<4;a++)
					if((unsigned int)border[a].y>g_maxy)
						g_maxy = (unsigned int)mround(border[a].y);	// skrajny maxymalny y

	// sortowanie w kolejnoœci do wype³niania
			   point tab[4];
			   tab[0].x = border[0].x; tab[0].y = border[0].y;
			   tab[1].x = border[1].x; tab[1].y = border[1].y;
			   tab[2].x = border[2].x; tab[2].y = border[2].y;
			   tab[3].x = border[3].x; tab[3].y = border[3].y;
			   grahamInit(tab);
			   grahamScan(firstPoint->next);		// pouk³adanae w kolejnosci
			   printPoints(border);
			   border[4] = border[0];	// do isinside zamkniêcie wielok¹ta

			   for(xx=g_minx;xx<g_maxx;xx++)		// wype³nianie obszaru
					for(yy=g_miny;yy<g_maxy;yy++)	{
						tmppoint.x = xx;
						tmppoint.y = yy;
						if(IsInside(tmppoint,border,5,1))
							result->SetPixel(yy,xx,50000);
					}
			//	merge(&wada1, &x1, &y1, &wada2, &x2, &y2, &out, &outx, &outy); // out - dane dla dwóch po³¹czonych wad

// ³¹czenie w jeden obiekt
	double min1,max1,min2,max2,globalminx,globalmaxx,globalminy,globalmaxy;
	unsigned int i,sizex,sizey,pole,x,y,licznik;
	x1->getMinMax(min1,max1);
	x2->getMinMax(min2,max2);
	if(min1<min2)
		globalminx = min1;
	else
		globalminx = min2;
	if(max1>max2)
		globalmaxx = max1;
	else
		globalmaxx = max2;

	y1->getMinMax(min1,max1);
	y2->getMinMax(min2,max2);
	if(min1<min2)
		globalminy = min1;
	else
		globalminy = min2;
	if(max1>max2)
		globalmaxy = max1;
	else
		globalmaxy = max2;

	sizex = globalmaxx-globalminx+1;
	sizey = globalmaxy-globalminy+1;

	out->AllocateData(sizey,sizex);
	out->Zeros();
	
	pole = 0;
	for(i=0;i<out->GetNumofElements();i++)
		if(out->data[i]>0)
			pole++;		// ilosc bia³ych elementów - potrzebne do wyznaczenia ich wspó³rzêdnych
	
	outx->AllocateData(pole,1);
	outy->AllocateData(pole,1);
	licznik = 0;
	// wyznaczanie wspolrzêdnych wszystkich bia³ych pikseli w wadzie
	for(x=0;x<sizex;x++)
		for(y=0;y<sizey;y++)
			if(out->GetPixel(y,x)>0)	{
				outx->data[licznik] = x+globalminx;
				outy->data[licznik] = y+globalminy;
				out->SetPixel(x,y,result->GetPixel(outx->data[licznik],outy->data[licznik]));
				licznik++;
			}
#ifdef VERBOSE			
	out->DumpBinary("wsk_merge.out");
	outx->DumpBinary("wsk_outx.out");
	outy->DumpBinary("wsk_outy.out");
	x1->DumpBinary("wsk_x1.out");
	x2->DumpBinary("wsk_x2.out");
	y1->DumpBinary("wsk_y1.out");
	y2->DumpBinary("wsk_y2.out");
	results_kryterium1.DumpBinary("wsk_results.out");
	result->DumpBinary("wsk_result.out");
	C_Image_Container BW;
	bw->CloneObject(&BW);
	BW.SetPixel(e1.d1.y,e1.d1.x,40000);
	BW.SetPixel(e1.d2.y,e1.d2.x,40000);
	BW.SetPixel(e1.k1.y,e1.k1.x,20000);
	BW.SetPixel(e1.k2.y,e1.k2.x,20000);

	BW.SetPixel(e2.d1.y,e2.d1.x,40000);
	BW.SetPixel(e2.d2.y,e2.d2.x,40000);
	BW.SetPixel(e2.k1.y,e2.k1.x,20000);
	BW.SetPixel(e2.k2.y,e2.k2.x,20000);
	BW.DumpBinary("wsk_BW.out");
	
#endif
}


BOOL isconected(unsigned int ind,C_Matrix_Container* con,unsigned int ile,unsigned int& ind2)
{
	unsigned int a;
	for(a=0;a<ile;a++)
		if(con->GetPixel(a,0)==ind)	{
			ind2 = (unsigned int)con->GetPixel(a,1);
			return TRUE;	// dany element jest juz po³¹czony
		}
	return FALSE;
}

BOOL kryterium3221(unsigned int ind,C_Matrix_Container* feat)
{
	if(feat->GetPixel(ind,2)>2)
		return TRUE;
	else
		return FALSE;

}

BOOL kryterium3223(unsigned int ind1,unsigned int ind2,double dist,C_Matrix_Container* feat)
{
	double kr;

	if(feat->GetPixel(ind1,10) > feat->GetPixel(ind2,10))
		kr = feat->GetPixel(ind2,6);			// krótszy wymiar mniejszego wskazania
	else
		kr = feat->GetPixel(ind1,6);

	if(dist<kr)
		return TRUE;
	else
		return FALSE;

}

void evaluate_ellipse(C_Matrix_Container* x1, C_Matrix_Container* y1,double kat,double kat1, double kr_os, double dl_os, PIXEL* srodek,ELIPSA* elipsa)
{
	// wspó³rzêdne wierzcho³ków s¹ zwracane w odniesieniu do rzeczywistej pozycji elipsy
	double meanx,meany;
	C_Matrix_Container rotation;
	C_Matrix_Container xy;
	C_Matrix_Container punkty_rot;
//	if(kat1>0)
//		kat+=M_PI/2;
	meanx = x1->Mean(NULL,0);
	meany = y1->Mean(NULL,0);

	elipsa->kr_os = kr_os;
	elipsa->dl_os = dl_os;
	elipsa->srodek.x = srodek->x;elipsa->srodek.y = srodek->y;

	rotation.AllocateData(2,2);
	xy.AllocateData(2,4);
	rotation.SetPixel(0,0,cos(-kat)); rotation.SetPixel(0,1,-sin(-kat));
	rotation.SetPixel(1,0,sin(-kat)); rotation.SetPixel(1,1,cos(-kat));
	xy.SetPixel(0,0,dl_os); //xd1
	xy.SetPixel(1,0,0); //yd1
	xy.SetPixel(0,1,-dl_os); //xd2
	xy.SetPixel(1,1,0); //yd2
	xy.SetPixel(0,2,0); //xk1
	xy.SetPixel(1,2,kr_os); //yk1
	xy.SetPixel(0,3,0); //xk2
	xy.SetPixel(1,3,-kr_os); //yk2

	xy.Transpose();
	punkty_rot.AllocateData(xy._rows,xy._cols);
	MatrixMulti(&xy,&rotation,&punkty_rot);
	punkty_rot.Transpose();

	elipsa->d1.x = punkty_rot.GetPixel(0,0)+meanx; elipsa->d1.y = punkty_rot.GetPixel(1,0)+meany;
	elipsa->d2.x = punkty_rot.GetPixel(0,1)+meanx; elipsa->d2.y = punkty_rot.GetPixel(1,1)+meany;
	elipsa->k1.x = punkty_rot.GetPixel(0,2)+meanx; elipsa->k1.y = punkty_rot.GetPixel(1,2)+meany;
	elipsa->k2.x = punkty_rot.GetPixel(0,3)+meanx; elipsa->k2.y = punkty_rot.GetPixel(1,3)+meany;

}
/*
PBf1;	// data lenghtening
PBf2;	// angle
PBf6;	// Principal lenghtening
PBf7_1;	// Longer axis
PBf7_2;	// shorter axis
PBf7_3;	// pole-punktyin
PBf7_4;	// punkty out
PBf7_5;	// angle ellip nr10
*/
/*void merge(C_Image_Container* wada1, C_Matrix_Container* x1, C_Matrix_Container* y1,
		   C_Image_Container* wada2, C_Matrix_Container* x2, C_Matrix_Container* y2,
		   C_Image_Container* out,
		   C_Matrix_Container* outx,C_Matrix_Container* outy)
{
	double min1,max1,min2,max2,globalminx,globalmaxx,globalminy,globalmaxy;
	unsigned int i,sizex,sizey,pole,x,y,licznik;
	x1->getMinMax(min1,max1);
	x2->getMinMax(min2,max2);
	if(min1<min2)
		globalminx = min1;
	else
		globalminx = min2;
	if(max1>max2)
		globalmaxx = max1;
	else
		globalmaxx = max2;

	y1->getMinMax(min1,max1);
	y2->getMinMax(min2,max2);
	if(min1<min2)
		globalminy = min1;
	else
		globalminy = min2;
	if(max1>max2)
		globalmaxy = max1;
	else
		globalmaxy = max2;

	sizex = globalmaxx-globalminx+1;
	sizey = globalmaxy-globalminy+1;

	out->AllocateData(sizey,sizex);
	for(i=0;i<x1->GetNumofElements();i++)
		out->SetPixel(y1->data[i]-globalminy,x1->data[i]-globalminx,wada1->GetPixel(y1->data[i]-globalminy,x1->data[i]-globalminx));
	for(i=0;i<x2->GetNumofElements();i++)
		out->SetPixel(y2->data[i]-globalminy,x2->data[i]-globalminx,wada2->GetPixel(y2->data[i]-globalminy,x2->data[i]-globalminx));
	
	pole = 0;
	for(i=0;i<out->GetNumofElements();i++)
		if(out->data[i]>0)
			pole++;		// ilosc bia³ych elementów - potrzebne do wyznaczenia ich wspó³rzêdnych
	
	outx->AllocateData(pole,1);
	outy->AllocateData(pole,1);
	licznik = 0;
	// wyznaczanie wspolrzêdnych wszystkich bia³ych pikseli w wadzie
	for(x=0;x<sizex;x++)
		for(y=0;y<sizey;y++)
			if(out->GetPixel(y,x)>0)	{
				outx->data[licznik] = x+globalminx;
				outy->data[licznik] = y+globalminy;
				licznik++;
			}

}
*/

void PB_feat1(unsigned int current_flaw,
			  C_Image_Container *res, C_Matrix_Container *x_coord,
			  C_Matrix_Container *y_coord,
			  double* out1,double* out2,
			  double* out6,
			  double* out7_1,double* out7_2,double* out7_3,double* out7_4,
			  double* out7_5)
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

	xy.AllocateData(2,x_coord->_rows);	// aby ³atwiej kopiowaæ by³o
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
		tmp[0] = covariance.data[0];	tmp[1] = covariance.data[2];	//organizacja - kolumny z matlaba s¹ liniowo po kolei
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
	if(maxy==miny)
		*(out1) = -1;
	else
		*(out1) = (maxx-minx)/(maxy-miny);

// ************************************ FEAT2 ******************************************************
	
	double wa,wb;
	cosadl = (v_org.GetPixel(0,mainvector))/
		(sqrt(v_org.GetPixel(0,mainvector)*v_org.GetPixel(0,mainvector)+v_org.GetPixel(1,mainvector)*v_org.GetPixel(1,mainvector) ) );
	alfadl = alfadl_copy = abs(acos(cosadl)*180/M_PI);
	if(alfadl>90)
		alfadl = abs(alfadl-180)/90;
	else
		alfadl/=90;
//	xxx = v_org.GetPixel(1,mainvector);
//	yyy = v_org.GetPixel(0,mainvector);
	wa = (v_org.GetPixel(0,mainvector)-0.0)/(v_org.GetPixel(1,mainvector)-0.0);
	wb = v_org.GetPixel(0,mainvector)-wa*v_org.GetPixel(1,mainvector);
	*(out2) = atan((wa-0.0)/(1+0.0*wa));

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
				hits = true;	// oznacza ze choc raz program tu wszed³, czyli jest jakieœ minimum
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

bool IsInside(PIXEL& point,PIXEL* points,unsigned int wadanum,double tol)
{
	int count = 0;
	unsigned int a;
	PIXEL p0;
	PIXEL p1;
	point = prvCheckNumZero(point,tol);
	for(a=0;a<wadanum-1;a++) {
		p0 = prvCheckNumZero(points[a],tol);
		p1 = prvCheckNumZero(points[a+1],tol);
		if(prvTestEdge(point,p0,p1,tol))
			return true;
		count += prvTestInside(point,p0,p1);
	}
    return (count%2)==1;
}

int prvTestInside(PIXEL& point, PIXEL& p0, PIXEL& p1)
{
	PIXEL tmp;
	double t, xp;
	if(p1.y==p0.y)
		return 0;
	if(p0.y > p1.y) {
		tmp = p0;
		p0 = p1;
		p1 = tmp;
	}
	if( (p0.y<point.y) && (p1.y<=point.y) )
		return 0;
	if( (p0.y>point.y) && (p1.y>point.y) )
		return 0;
	if( (p0.x<point.x) && (p1.x<point.x) )
		return 0;
	if( (p0.x>=point.x) && (p1.x>=point.x) )
		return 1;

	t = (point.y-p0.y)/(p1.y-p0.y);
	xp = p0.x + t*(p1.x-p0.x);
	if(xp>=point.x)
		return 1;
	else
		return 0;
}

bool prvTestEdge(PIXEL& point, PIXEL& p0, PIXEL& p1,double tol)
{
	PIXEL tmp;
	double wyznacznik;
	if( (p0.x>p1.x) || (p0.y>p1.y) ) {
		tmp = p1;
		p1 = p0;
		p0 = tmp;
	}
	wyznacznik = p0.x*p1.y + p1.x*point.y + point.x*p0.y - p1.y*point.x - point.y*p0.x - p0.y*p1.x;  // sprawdziæ w matlabie
	return ( (p0.x<=point.x && point.x<=p1.x) && (p0.y<=point.y && point.y<=p1.y) ) && (!sgn(mround(wyznacznik/tol)*tol));
}


PIXEL prvCheckNumZero(PIXEL point, double tol)
{
	PIXEL out;
	out.x = mround(point.x/tol)*tol;
	out.y = mround(point.y/tol)*tol;
	return out;
}

inline int sgn(double val)
{
	if(val>0)
		return 1;
	if(val<0)
		return -1;
	else
		return 0;
}

double mround(double in)
{
	double tmp;
	tmp = floor(in);
	if(fabs(tmp)>0.5)
		return ceil(in);
	else
		return tmp;
}

void printPoints(PIXEL* pBorder)
{
    point *curPoint=firstPoint;
    unsigned int a=0;
    do
    {
		pBorder[a].x = curPoint->x; pBorder[a].y = curPoint->y;
		assert(a<4);
        curPoint=curPoint->next;
		a++;
    } while (curPoint!=firstPoint); //CONTINUE UNTIL HAVING LOOPED BACK AROUND TO FIRSTPOINT
}

void grahamMain(point* tempPoints)
{
    grahamInit(tempPoints); //INITIALIZE DATA FOR GRAHAM'S SCAN
    //printPoints(); //PRINT OUT SORTED POINTS
//    drawPoints(); //RENDER POINTS ON SCREEN
//  drawPermeter(1); //DRAW PERIMETER BEFORE GRAHAM'S SCAN
    grahamScan(firstPoint->next); //RUN GRAHAM'S SCAN STARTING AT SECOND NODE CLOCKWISE
//    drawPermeter(2); //DRAW PERIMETER AFTER GRAHAM'S SCAN
    //printPoints(); //PRINT OUT CONVEX HULL
}

void grahamInit(point* tempPoints)
{
    int minPoint=0;
    double tempAngle=0;
//    point tempPoints[1000]; //CREATE STATIC ARRAY FOR RANDOM POINT GENERATION
    point *tempPtr;
    
    firstPoint=NULL; //INIT FIRSTPOINT POINTER
    
 /*   for (int i=0;i<NumPoints;i++) //GENERATE RANDOM POINTS
    {
        tempPoints[i].x=rand()%400+50;
        tempPoints[i].y=rand()%400+50;
    }*/
    
    for (int k=1;k<NumPoints;k++)  //FIND MIN POINT
        if (tempPoints[k].y<tempPoints[minPoint].y)
            minPoint=k;
    
    //cout<<"MinPoint: "<<tempPoints[minPoint].x<<" "<<tempPoints[minPoint].y<<endl;  //PRINT OUT MINIMAL POINT
    
    for (int i=0;i<NumPoints;i++) //SORT RANDOM POINTS
    {
        tempPoints[i].angle=findAngle(tempPoints[minPoint].x,tempPoints[minPoint].y,tempPoints[i].x,tempPoints[i].y);
        addPoint(tempPoints[i]);
        //cout<<"angle: "<<tempPoints[i].angle<<" x: "<<tempPoints[i].x<<" y: "<<tempPoints[i].y<<endl;
    }
    
    tempPtr=firstPoint;
    do  //FIND LAST NODE IN LINKED LIST
    {
        tempPtr=tempPtr->next;
    } while (tempPtr->next!=NULL);
        
    tempPtr->next=firstPoint; //COMPLETE CIRCULAR LINKED LIST
    firstPoint->prev=tempPtr; //COMPLETE CIRCULAR LINKED LIST
    
    tempPtr=firstPoint->next;
/*    for (int i=1;i<NumPoints;i++) //DRAW LINES FROM MIN POINT TO ALL OTHERS
    {
        drawLine(firstPoint, tempPtr, 3);
        tempPtr=tempPtr->next;
    } */
}

double findAngle(double x1, double y1, double x2, double y2)
{
    double deltaX=x2-x1;
    double deltaY=y2-y1;
    
    if (deltaX==0 && deltaY==0)
        return 0;
    
    double angle=atan(deltaY/deltaX)*(180.0/3.141592);
    
    //TAKE INTO ACCOUNT QUADRANTS, VALUE: 0° - 360°
    if (deltaX>=0 && deltaY>=0)
        angle=90+angle;
    else if (deltaX>=0 && deltaY<0)
        angle=90+angle;
    else if (deltaX<0 && deltaY>0)
        angle=270+angle;
    else if (deltaX<0 && deltaY<=0)
        angle=270+angle;
    
    return angle;
}

void addPoint(point Point)
{
    point *tempPoint,*tempPointA,*tempPointB, *curPoint;
    
    //ALLOCATE A NEW POINT STRUCTURE AND INITIALIZE INTERNAL VARIABLES
    tempPoint = new point;
    tempPoint->x=Point.x;
    tempPoint->y=Point.y;  
    tempPoint->angle=Point.angle;  
    tempPoint->next=NULL;
    tempPoint->prev=NULL;
    
    
    if (firstPoint==NULL) //TEST IF LIST IS EMPTY
    {
        firstPoint=tempPoint;
        return;
    }

    if (firstPoint->next==NULL && tempPoint->angle >= firstPoint->angle) 
    //TEST IF ONLY ONE NODE IN LIST AND CURRENT NODE HAS GREATER ANGLE
    {
        firstPoint->next=tempPoint;
        tempPoint->prev=firstPoint;
        return;
    }
    
    curPoint=firstPoint;
    
    while (tempPoint->angle >= curPoint->angle && curPoint->next!=NULL)
    //CONTINUE THROUGH LIST UNTIL A NODE IS FOUND WITH A GREATER ANGLE THAN CURRENT NODE
        curPoint=curPoint->next;
  
    if (curPoint==firstPoint) //TEST IF NODE IS FIRSTPOINT.  IF SO, ADD AT FRONT OF LIST.
    {
        firstPoint->prev=tempPoint;
        tempPoint->next=firstPoint;
        firstPoint=tempPoint;
        return;
    }
    else if (curPoint->next==NULL && tempPoint->angle >= curPoint->angle) 
    //TEST IF WHILE LOOP REACHED FINAL NODE IN LIST.  IF SO, ADD AT END OF THE LIST.
    {
        curPoint->next=tempPoint;
        tempPoint->prev=curPoint;
        return;
    }
    else //OTHERWISE, INTERMEDIATE NODE HAS BEEN FOUND.  INSERT INTO LIST.  
    {
        tempPointA=curPoint->prev;
        tempPointB=curPoint->prev->next;
        tempPoint->next=tempPointB;
        tempPoint->prev=tempPointA;
        tempPoint->prev->next=tempPoint;
        tempPoint->next->prev=tempPoint;
    }
    
    return;   
}

void grahamScan(point *P)
{
    point *tempPrev, *tempNext;
    
    if (P==firstPoint) //IF RETURNED TO FIRST POINT, DONE
        return;
    
    if (!isConvexPoint(P)) //IF POINT IS CONCAVE, ELIMINATE FROM PERIMETER
    {
        tempPrev=P->prev; 
        tempNext=P->next;
        tempPrev->next=tempNext;
        tempNext->prev=tempPrev;
 //       drawLine(tempPrev, tempNext,3); //DRAW LINE SHOWING NEW EDGE
        delete P; //FREE MEMORY
        grahamScan(tempPrev); //RUN GRAHAM'S SCAN ON PREVIOUS POINT TO CHECK IF CONVEXITY HAS CHANGED IT

    }
    else //POINT IS CONVEX
        grahamScan(P->next); //PROCEED TO NEXT POINT
}

bool isConvexPoint(point* P)
{
    double CWAngle=findAngle(P->x,P->y,P->prev->x,P->prev->y); //COMPUTE CLOCKWISE ANGLE
    double CCWAngle=findAngle(P->x,P->y,P->next->x,P->next->y); //COMPUTER COUNTERCLOCKWISE ANGLE
    double difAngle;
    
//    drawLine(P->prev, P->next, 3);
    
    if (CWAngle>CCWAngle)
    {
        difAngle=CWAngle-CCWAngle;  //COMPUTE DIFFERENCE BETWEEN THE TWO ANGLES
        
        if (difAngle>180)
            return false; //POINT IS CONCAVE
        else
            return true; //POINT IS CONVEX
    }
    else if (CWAngle<CCWAngle)
    {
        difAngle=CCWAngle-CWAngle;  //COMPUTE DIFFERENCE BETWEEN THE TWO ANGLES
        
        if (difAngle>180)
            return true; //POINT IS CONVEX
        else
            return false; //POINT IS CONCAVE
    }
}