#include "Cpp_Lib.h"

//!!!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//iii

void cov(C_Matrix_Container* X, C_Matrix_Container* Y);
unsigned int unique(C_Matrix_Container* input,C_Matrix_Container* licznosc);
BOOL isintable(double* tab,double val,unsigned int tab_size);
void RemoveVal(C_Matrix_Container* input,double val);
void GetMinMax(double* data, double& min, double& max,unsigned int ile);
void CutImage(C_Image_Container*im,C_Image_Container *out, double *xc, double *yc, unsigned int ile);

//!!!
//Morfologia. A to obraz, B to maska, bit oznacza czy ma zostaæ wykonane œcinanie/pogrubianie,
//czy "hit-or-miss"
unsigned long Area(C_Image_Container*);
void morf(C_Matrix_Container* A, int M[3][3], bool bit);
void doPliku(char nazwa[], C_Matrix_Container* ress);
double Perimeter(C_Image_Container *in, C_Image_Container *out);
double wspMal(double A, double p);
void CenterOfGravity(C_Image_Container *in, unsigned long A, double *Gx, double *Gy);
double wspBB(C_Image_Container *in, unsigned long A, double Gx, double Gy);
double wspDan(C_Image_Container *in, unsigned long A);
double wspHar(C_Image_Container *in, double gx, double gy, double ilePikseli);
//iii

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"cf");	// nazwa w ADDIP
	strcpy_s(definition->version,MAX_VER,"1.0");	// wersja (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"MATRIX = cf(IMAGE,IMAGE,IMAGE)"); // SYNTAX (nie ma wiêkszego znaczenia)
	definition->numofout = 1;	// ilosc parametrów wyœciowych
	definition->numofin = 3;	// ilosc parametrów wejœciowych
	definition->typesofin[0] = IMAGE;definition->typesofin[1] = IMAGE;definition->typesofin[2] = IMAGE;	// typy wejœcia
	definition->typesofout[0] = MATRIX; // typy wyjœcia
	definition->interface_version = 2;	// wersja interface
	strcpy_s(definition->description,MAX_DESC,"Developed by PB");	// cokolwiek
	definition->ADDIP_version = 2.0; 

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Image_Container *original;
	C_Image_Container *indexed;
	C_Image_Container *bw;
	unsigned int num_of_flaws; // ile jest zaindeksowanych wad
	unsigned int num_of_points=0; // ile jest wszystkich punktów
	unsigned int a,x,y,o,licznik;
	unsigned int pixel,current_flaw,current_flaw_offset,current_flaw_size;

	C_Image_Container res;
	C_Matrix_Container usorted;	// pojedyncze wartosci
	C_Matrix_Container licznosc; // ilosc pixeli kazdego rodzaju
	C_Matrix_Container x_coord;	// wsp x punktów
	C_Matrix_Container y_coord; // wsp y punktów
	C_Matrix_Container offset;	// offset od poczatku tablicy x,y_coord skazuj¹cy na kolejne wady
	C_Matrix_Container lastpoz;	// ostatnia pozycja dl kazdej wady w tablicy indexów

	//!!!
	FILE *plik;
	int M[3][3] = { {7, 1, 7}, {1, 1, 1}, {7, 1, 7} }; //To powinno pozostawiaæ same krawêdzie
	//int M[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} }; //Erozja t¹ mask¹ dzia³a
	int i, j;
	C_Image_Container* B = new C_Image_Container();
	double gy, gx, obw;
	//iii

	original = (C_Image_Container*)intab[0]->data;	// tu dokonujemy konwersji bo w intab wszystko jest trzymane jako void*
	indexed = (C_Image_Container*)intab[1]->data;
	bw = (C_Image_Container*)intab[2]->data;

	num_of_flaws = unique(indexed,&licznosc);
	for(a=1;a<licznosc.GetNumofElements();a++)	// od jedeynki zeby zer nie liczyc
		num_of_points+=(unsigned int)licznosc.data[a];
//licznosc.DumpBinary("c:\\licznosc.dat");
	offset.AllocateData(1,65536);
	offset.Zeros();

	for(o=1;o<licznosc.GetNumofElements();o++)
		if(licznosc.data[o]!=0)
			for(a=1;a<o;a++)
				offset.data[o]+=licznosc.data[a];
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

// Main loop
	// Cutting the flaw
	current_flaw = 1;
	if(current_flaw==0)	{
		current_flaw_offset = 0;
		current_flaw_size = (unsigned int)offset.data[0];
	} else {
		current_flaw_offset = (unsigned int)offset.data[current_flaw-1];
		current_flaw_size = (unsigned int)(offset.data[current_flaw]-offset.data[current_flaw-1]);
	}

	CutImage(indexed, &res, &x_coord.data[current_flaw_offset],&y_coord.data[current_flaw_offset], current_flaw_size);

/* USER FUCTIONS HERE
	Usefull parameters:
		unsigned int current_flaw			- current flaw index
		unsigned int current_flaw_offset	- offset in coordinates tables
		C_Image_Container res				- extracted flaw
		C_Matrix_Container x_coord			- x coordinates of flaws
		C_Matrix_Container y_coord			- y coordinates of flaws
		unsigned int current_flaw_size		- number of pixels in current flaws
*/
/* How to extract coordinates for given flaw: 
		x_coord.data[current_flaw_offset:current_flaw_offset+current_flaw_size]
	*/
	
//	usorted.DumpBinary("c:\\usorted.dat");
//	x_coord.DumpBinary("c:\\x.dat");
//	y_coord.DumpBinary("c:\\y.dat");
//	offset.DumpBinary("c:\\offset.dat");
//	res.DumpBinary("c:\\res.out");

	//!!!
	//M = new C_Matrix_Container();
	//M->SetPixel(0, 0, 7);
	/*
	M.SetPixel(0, 1, 1);
	M.SetPixel(0, 2, 7);
	M.SetPixel(1, 0, 1);
	M.SetPixel(1, 1, 1);
	M.SetPixel(1, 2, 1);
	M.SetPixel(2, 0, 7);
	M.SetPixel(2, 1, 1);
	M.SetPixel(2, 2, 7);
	*/
	//morf(bw, M, true);

	//bw->DumpBinary("test.dat");
	plik = fopen("tezt.txt", "w");
	//fprintf(plik, "%ld", Area(&res));
	
	//res.DumpBinary("plikk2.txt");
	doPliku("dump.txt", bw);
	B->AllocateData(bw->_rows, bw->_cols);
	B->Zeros();
	obw = Perimeter(bw, B);
	fprintf(plik, "Area: %ld\n", Area(bw));
	fprintf(plik, "Perimeter: %e\n", obw);
	fprintf(plik, "Wspó³czynnik Malinowskiej: %e\n", wspMal(Area(bw), Perimeter(bw, B)));
	//Gx = &gx;
	//Gy = &gy;
	gx = 0;
	gy = 0;
	CenterOfGravity(bw, Area(bw), &gx, &gy);
	fprintf(plik, "Gx: %e\n", gx);
	fprintf(plik, "Gy: %e\n", gy);
	fprintf(plik, "Wspó³czynnik Blaira-Blissa: %e\n", wspBB(bw, Area(bw), gx, gy));
	fprintf(plik, "Wspó³czynnik Danielssona: %e\n", wspDan(bw, Area(bw)));
	fprintf(plik, "Wspó³czynnik Harlicka: %e\n", wspHar(B, gx, gy, obw));
	doPliku("dump2.txt", B);
	fclose(plik);
	//iii


	outtab[0]->AssignValue((void*)indexed,err);
}

//!!!
unsigned long Area(C_Image_Container* ic)
{
	unsigned long i, j;
	unsigned long pole = 0;
	//FILE *plik;
	//plik = fopen("damp2.txt", "w");
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
	/*
	for(i = 0; i < ic->_rows; i++)
	{
		for(j = 0; j < ic->_cols; j++)
		{
			fprintf(plik, "%ld, ", ic->GetPixel(i, j));
		}
		fprintf(plik, "\n");
	}
	*/
	//fclose(plik);
	return(pole);
}

//Funkcja zapisuje do pliku obraz bw
void doPliku(char nazwa[], C_Matrix_Container* ress)
{
	FILE *plik;
	int i, j;
	plik = fopen(nazwa, "w");
	for(i = 0; i < ress->_rows; i++)
	{
		for(j = 0; j < ress->_cols; j++)
		{
			/*
			if(ress->GetPixel(i, j) != 0)
			{
				fprintf(plik, "%d", 1);
			}
			else
			{
				fprintf(plik, "%d", 0);
			}
			*/
			fprintf(plik, "%d", (int)ress->GetPixel(i, j));
			if(j != ress->_cols - 1) fprintf(plik, ",");
		}
		fprintf(plik, "\n");
	}
	fclose(plik);
}


void morf(C_Matrix_Container* F, int M[3][3], bool bit)
{
	int p1 = 0, p2 = 0, p = 0, c, pasuje = 1, m, n;
	unsigned int i, j;
	double pom0, pom1; //Te dwie zmienne s³u¿¹ jako zero i jeden, bo macie¿ jest dziwnie wyskalowana
	F->getMinMax(pom0, pom1);
	//int piksel1, piksel2;
	C_Matrix_Container* B = new C_Image_Container();
	C_Matrix_Container* A = new C_Image_Container();
	B->AllocateData(F->_rows + 2, F->_cols + 2);
	A->AllocateData(F->_rows + 2, F->_cols + 2);
	B->Zeros();
	A->Zeros();
	for(i = 0; i < F->_rows; i++)
	{
		for(j = 0; j < F->_cols; j++)
		{
			A->SetPixel(i + 1, j + 1, F->GetPixel(i, j));
		}
	}
	//double B[A->_rows][A->_cols];
	//c = int((M->_cols) / 2);	
	c = 1;
	if(bit)	//wybrano pogrubianie/œcienianie
	{
		for(i = c; i < A->_rows - c; i++)
		{
			for(j = c; j < A->_cols - c; j++)
			{
				pasuje = 1;
				for(m = -c; m <= c; m++)
				{
					for(n = -c; n<= c; n++)
					{	
						if((A->GetPixel(i + m, j + n) != pom1 && M[m + c][n + c] == 1) || (A->GetPixel(i + m, j + n) != pom0 && M[m + c][n + c] == 0))
							pasuje = 0;
					}
				}
				if(pasuje == 1)
				{
					if(M[c][c] == 1)
						B->SetPixel(i, j, 0);
					else if(M[c][c] == 0)
						B->SetPixel(i, j, 1);
					else
					{
						if(B->GetPixel(i, j) == 0) B->SetPixel(i, j, pom1);
						else B->SetPixel(i, j, pom0);
					}
				}
			}
		}
	}
	else //wybrano "hit-or-miss"
	{
		for(i = c; i < A->_rows - c; i++)
		{
			for(j = c; j < A->_cols - c; j++)
			{
				pasuje = 1;
				for(m = -c; m <= c; m++)
				{
					for(n = -c; n<= c; n++)
					{	
						if((A->GetPixel(i + m, j + n) != pom1 && M[m + c][n + c] == 1) || (A->GetPixel(i + m, j + n) != pom0 && M[m + c][n + c] == 0))
							pasuje = 0;
					}
				}
				if(pasuje == 1) B->SetPixel(i, j, pom1);
				else B->SetPixel(i, j, pom0);
			}
		}
	}
	
	//doPliku("dumpB.txt", B);
	for(i = 0; i < F->_rows; i++)
	{
		for(j = 0; j < F->_cols; j++)
		{
			F->SetPixel(i, j, B->GetPixel(i + 1, j + 1));
		}
	}
	
	B->FreeData();
	delete(B);
}

//przez wartoœæ zwracany jest obwód przedmiotu, a w out'cie znajduje siê sam wyciety obwód
double Perimeter(C_Image_Container *in, C_Image_Container *out)
{
	int M[3][3] = { {7, 1, 7}, {1, 1, 1}, {7, 1, 7} };
	int i, j;
	double obwod = 0;
	//int maks, miin;
	C_Matrix_Container* pom = new C_Image_Container();
	pom->AllocateData(in->_rows, in->_cols);
	//in->getMinMax(maks, miin);
	
	//przepisanie in do pom
	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			pom->SetPixel(i, j, in->GetPixel(i, j));
		}
	}
	
	morf(pom, M, false);
	
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

//Funkcja licz¹ca œrodek ciê¿koœci obiektu.
//A - powierzchnia
//Gx i Gy to wynik - wyliczone œrodki ciê¿kosci.
void CenterOfGravity(C_Image_Container *in, unsigned long A, double *Gx, double *Gy)
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
	//+1 ¿eby zachowaæ zgodnoœæ z matlabem
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

//Wspó³czynnik Malinowskiej
//A - powierzcnia
//p - obwód
double wspMal(double A, double p)
{
	if(A > 0) return((p / (2 * sqrt(3.14125 * A))) - 1);
	else return(-1);
}

//Wspó³czynnik Blaira-Blissa
//A - powierzchnia
//Gx - œrodek ciê¿koœci w kierunku x
//Gy - œrodek ciê¿koœci w kierunku y
double wspBB(C_Image_Container *in, unsigned long A, double Gx, double Gy)
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

//Wspó³czynnik Danielssona
double wspDan(C_Image_Container *in, unsigned long A)
{
	FILE *plik;
	int i, j;
	int M[3][3] = { {1, 1, 1}, {1, 1, 1}, {1, 1, 1} };
	unsigned long li = 0; //suma po wszystkich odleg³oœciach
	bool isZeros = false;
	C_Matrix_Container* pom2 = new C_Image_Container();
	C_Matrix_Container* fO = new C_Image_Container(); // Funkcja odleg³oœci
	pom2->AllocateData(in->_rows, in->_cols);
	pom2->Zeros();
	fO->AllocateData(in->_rows, in->_cols);
	fO->Zeros();
	for(i = 0; i < in->_rows; i++)
	{
		for(j = 0; j < in->_cols; j++)
		{
			if(in->GetPixel(i, j) != 0)
			{
				pom2->SetPixel(i, j, 1);
			}
		}
	}
	//Tutaj pom zawiera jedynki tam, gdzie in mia³ niewiadomo co.
	//Teraz trzeba zrobiæ kolejne erozje i dodawaæ wyniki.
	while(!isZeros)//dopóki kolejne erozje zmieniaj¹ coœ w obrazie
	{
		//Erozja pom2
		morf(pom2, M, false);
		//doPliku("pom2.txt", pom2);
		//Sprawdzenie, czy pom2 jest z³o¿one z samych zer. Je¿eli tak, to break.
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
	/*
	doPliku("fO.txt", fO);
	plik = fopen("tezzt.txt", "w");
	fprintf(plik, "%ld\n", A);
	fprintf(plik, "%ld\n", A^3);
	fprintf(plik, "%ld\n", li);
	fprintf(plik, "%ld\n", li^2);
	fprintf(plik, "%e\n", (A^3) / pow((double)li, 2));
	//fprintf(plik, "%e\n", pow(suma1, 2)) / (ilePikseli * suma2 - 1);
	//fprintf(plik, "%e\n", sqrt(pow(suma1, 2) / (ilePikseli * suma2 - 1)));
	fclose(plik);
	*/
	
	//return(pow(A, 3) / pow(li, 2));
	pom2->FreeData();
	fO->FreeData();
	if(li > 0) return(pow((double)A, 3) / pow((double)li, 2));
	else return(-1);
}

double wspHar(C_Image_Container *in, double gx, double gy, double ilePikseli)
{
	int i, j;
	double suma1 = 0, suma2 = 0;
	FILE *plik;
	//C_Image_Container* pom = new C_Image_Container();
	//pom->Zeros();
	//ilePikseli = Perimeter(in, pom);
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
	/*
	plik = fopen("tezzt.txt", "w");
	fprintf(plik, "%e\n", suma1);
	fprintf(plik, "%e\n", suma2);
	fprintf(plik, "%e\n", ilePikseli);
	fprintf(plik, "%e\n", pow(suma1, 2));
	fprintf(plik, "%e\n", ilePikseli * suma2 - 1);
	fprintf(plik, "%e\n", pow(suma1, 2)) / (ilePikseli * suma2 - 1);
	fprintf(plik, "%e\n", sqrt(pow(suma1, 2) / (ilePikseli * suma2 - 1)));
	fclose(plik);
	*/
	if(ilePikseli * suma2 != 1) return(sqrt(pow(suma1, 2) / (ilePikseli * suma2 - 1)));
	else return(-1);
}
//iii

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

/*!!!
void cov(C_Matrix_Container* X, C_Matrix_Container* Y)
// liczy covariancje macierzy X
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
*///iii

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
// kasuje wartoœc val z vektora 1xn
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