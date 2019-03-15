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

#define NO_VERBOSE

#define CRACK 0
#define INCLUSION 1


char Crack_Inc(unsigned int c, C_Matrix_Container* feat);
BOOL kryterium3222(C_Image_Container* bw, C_Image_Container* indexed,
				   double scale,unsigned int &poz,double limit,
				   C_Matrix_Container* classification);
void FillImage(C_Image_Container* out, unsigned int start,double scale,unsigned int step);
BOOL CheckType(unsigned int r, unsigned int k, C_Matrix_Container* classification,C_Image_Container* indexed,double type);

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"wskclassifier");	// nazwa w ADDIP
	strcpy_s(definition->version,MAX_VER,"1.0");	// wersja (nie ma wiêkszego znaczenia)
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = wskclassifier(IMAGE,IMAGE,MATRIX,NUMBER,STRING)"); // SYNTAX (nie ma wiêkszego znaczenia)
	definition->numofout = 1;	// ilosc parametrów wyœciowych
	definition->numofin = 5;	// ilosc parametrów wejœciowych
	definition->typesofin[0] = IMAGE;definition->typesofin[1] = IMAGE;definition->typesofin[2] = MATRIX;definition->typesofin[3] = NUMBER;definition->typesofin[4] = STRING;	// typy wejœcia
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
	C_Image_Container *indexed;
	C_Image_Container *bw;
	C_Image_Container out;
	C_Matrix_Container *features;
	C_Matrix_Container *scale;
	C_Matrix_Container classification;	// typy wad
	char* filename;
	unsigned int current_flaw,licznik_crack=0,licznik_inne=0,poz,r,k;
	char type;
	BOOL flag = false;		// true jesi spe³niony warunek z oknem
	FILE *stream;

	bw = (C_Image_Container*)intab[0]->data;		// obraz po zlepianiu z wyjscia wskclassifier
	indexed = (C_Image_Container*)intab[1]->data;
	features = (C_Matrix_Container*)intab[2]->data;
	scale = (C_Matrix_Container*)intab[3]->data;
	filename = (char*)intab[4]->data;

#ifdef VERBOSE			
	bw->DumpBinary("wsk1_bw.out");	
	features->DumpBinary("wsk1_features.out");
	indexed->DumpBinary("wsk1_indexed.out");
#endif

	if( fopen_s( &stream, filename, "w" ) != 0 )	{
		err.SetError("wskclassifier: Cant open report file"); return;}

	fprintf_s(stream,"CLASSIFICATION REPORT\n\n\n");

	classification.AllocateData(features->_rows,2); 	// tu bed¹ zapamietywane indeksy oraz ich rozpoznanie
	for(current_flaw=0;current_flaw<features->_rows;current_flaw++)	{
		fprintf_s(stream,"-----------------------------------------------------------\n");
		fprintf_s(stream,"Flaw index: %d\n",(unsigned int)features->GetPixel(current_flaw,1));
		type = Crack_Inc(current_flaw, features);
		switch(type)	{
			case 0:
				fprintf_s(stream,"Classified as crack due to elong>5 (%f>5)\n",features->GetPixel(current_flaw,2));
				classification.SetPixel(licznik_crack,0,features->GetPixel(current_flaw,1));	// index wady
				classification.SetPixel(licznik_crack++,1,CRACK);	// rozpoznanie jako crack
				break;
			case 1:
				fprintf_s(stream,"Classified as inclusion due to elong<5 (%f<5)\n",features->GetPixel(current_flaw,2));
				classification.SetPixel(licznik_crack,0, features->GetPixel(current_flaw,1));	// index wady
				classification.SetPixel(licznik_crack++,1,INCLUSION);	// rozpoznanie jako inclusion
				break;
		}

	}
/* -------------------------- PRZESUWANIE OKNA -------------------------------------*/
	bw->CloneObject(&out);
	fprintf_s(stream,"\n\n\n###########################################################\n");
	if(kryterium3222(bw, indexed,scale->data[0], poz,0.1,&classification))	{
		fprintf_s(stream,"Weld not classified to class A according to the rule 3.2.2.2\n");
		FillImage(&out,poz,scale->data[0],1);
	} else
		fprintf_s(stream,"Weld classified to class A according to the rule 3.2.2.2\n");

	if(kryterium3222(bw, indexed,scale->data[0], poz,0.15,&classification))	{
		fprintf_s(stream,"Weld not classified to class B according to the rule 3.2.4.2\n");
		FillImage(&out,poz,scale->data[0],3);
	}

	fclose( stream );
/* -------------------------- zaznaczanie inkluzji -------------------------------------*/
	for(r=0;r<out._rows;r++)
		for(k=0;k<out._cols;k++)
		if(CheckType(r, k, &classification, indexed, INCLUSION))
			out.SetPixel(r,k,32000);

	outtab[0]->AssignValue((void*)&out,err);
}

void FillImage(C_Image_Container* out, unsigned int start,double scale,unsigned int step)
{
	unsigned int c,r;
	int delta;
	for(c=start;c<start+(unsigned int)scale;c+=step)
		for(r=0;r<out->_rows;r+=step)	{
			delta = 32000 - (unsigned int)out->GetPixel(r,c);
			out->SetPixel(r,c,out->GetPixel(r,c)+2*delta);
		}
}

BOOL CheckType(unsigned int r, unsigned int k, C_Matrix_Container* classification,C_Image_Container* indexed,double type)
{
	/* sprawdza czy pixel r,k jest typu type. Jeœli tak to zwraca TRUE */
	double ind;
	unsigned int a;
	ind = indexed->GetPixel(r,k);
	for(a=0;a<classification->_rows;a++)
		if(classification->GetPixel(a,0)==ind)	// znaleziona wada
			if(classification->GetPixel(a,1)==type)
				return TRUE;		// punkt o [r,k] jest typu type
			else
				return FALSE;
	return FALSE;

}

BOOL kryterium3222(C_Image_Container* bw, C_Image_Container* indexed,
				   double scale,unsigned int &poz,double limit,
				   C_Matrix_Container* classification)
{
/*	scale - ile punktów na cal
	limit - ile procent powoduje odrzucenie
	classification - klasyfikacja wad - indexy oraz typ wady

	Wady nie bêd¹ce porowatoscia maj¹ nie byæ brane pod uwagê
*/
	unsigned int o,c,r;
	double pole;
	for(o=0;o<bw->_cols-(unsigned int)scale;o++)	{	// okno
		pole = 0.0;
		for(r=0;r<bw->_rows;r++)
			for(c=o;c<o+scale;c++)
				if(bw->GetPixel(r,c)>0)
					if(CheckType(r, c, classification, indexed, INCLUSION))						// tu sprawdzamy czy obiekt nale¿y od porowatosci (1) - czyli jaki jest indeks w indexed(r,k) i czy ten index wystêpuje w classification
						pole ++;
		if(pole>limit*scale*bw->_rows)	{		// warunek 10% z ca³ej spoiny
			poz = o;
			return true;
			break;
		}
	}
	return false;
}


char Crack_Inc(unsigned int c, C_Matrix_Container* feat)
{
	if(feat->GetPixel(c,2)>5)
		return 0;	// crack
	if(feat->GetPixel(c,2)<=5)
		return 1;
	else
		return -1;

}