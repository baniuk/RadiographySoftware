/*****************************************************************************************
Program: Implementacja progowania Sauvola z wykorzystaniem wielo¹tkowoœci, funkcja ADDIP'a
Autor: Lech Napiera³a
Parametry wejœciowe:
	owe - obraz w odcieniach szaroœci do sprogowania
	w - rozmiar okna progowania (koniecznie nieparzysty!)
	k - parametr 'k' progowania (standardowa wartoœæ to 0.2)
Parametry wyjœciowe:
	owy - obraz dwukolorowy po progowaniu
*****************************************************************************************/

#include "Cpp_Lib.h"
#include <windows.h>
#include <math.h>
#include <process.h>
#include <stdio.h>

#define N 400	//maksymalna liczba procesorów

FILE* plik;

//Zmienne
C_Image_Container *owe; //obraz wejœciowy
C_Image_Container *owy = new C_Image_Container; //obraz wyjœciowy
C_Image_Container *owe2 = new C_Image_Container;//obraz wejœciowy do kwadratu
C_Image_Container *II = new C_Image_Container;  //Integral image
C_Image_Container *II2 = new C_Image_Container; //Integral image kwadratów
C_Matrix_Container *ppp;

//w - rozmiar okna, parametr k i n - liczba procesorów
float k;
int w, n = 1;

int i, j, r, d, g, pom;		//wiêkszoœæ do wyznacznia przedzia³ów
double R;
bool xlonger = false;
double max, min;

//Zmienne do w¹tków
//DWORD  threadId[N];
HANDLE hThread[N];

//Struktury do przekazywania danych w¹tkom:
struct CIC
{
	C_Image_Container *o1;
	C_Image_Container *o2;
} cic[2];

struct SAV
{
	int imin, imax;
	int jmin, jmax;
} sav[N];

//bool cicread = true;

// This is an empty project of DLL interface version 2.0

//Compute Integral Image - liczy Integral Image z tego co dostanie na wejœciu
//z obrwe robimy integrala i zapisujemy w obrwy
unsigned int WINAPI CII(void* param)
	{
		//plik = fopen("c:\\kalafior.txt", "w");
		//double min, max;

		struct CIC *c;

		c = (struct CIC *)param;

		C_Image_Container *obrwe = c->o1;
		C_Image_Container *obrwy = c->o2;
		/*
		C_Image_Container *obrwe = ((struct CIC *)param)->o1;
		C_Image_Container *obrwy = ((struct CIC *)param)->o2;
		*/
		//cicread = false;
		int i, j;
		/*
		II = zeros(a(1), a(2));
		II(1, 1) = A(1, 1); //1
		for i = 2:a(1), //2
			II(i, 1) = II(i - 1, 1) + A(i, 1);
		end;
		for j = 2:a(2),//3
			II(1, j) = II(1, j - 1) + A(1, j);
		end;
		for i = 2:a(1), //4
			for j = 2:a(2),
				II(i, j) = II(i - 1, j) + II(i, j - 1) - II(i - 1, j - 1) + A(i, j);
			end;
		end;

		Czyli:*/

		
		//obrwy->data[0] = obrwe->data[0]; //1
		for(i = 1; i < obrwe->_cols; i++) 
		{
			obrwy->data[i] = obrwe->data[i] + obrwy->data[i - 1]; //2
			
		}
		for(i = 1; i < obrwe->_rows; i++) //3
		{
			obrwy->data[i * obrwe->_cols] = obrwe->data[i * obrwe->_cols] + obrwy->data[(i - 1) * obrwe->_cols];
			//fprintf(plik, "%f\n", obrwy->data[i * obrwe->_cols]);
		}
		
	
		
		for(i = 1; i < obrwe->_rows; i++) //4
		{
			//fprintf(plik, "%f\n", obrwe->data[i]);
			for(j = 1; j < obrwe->_cols; j++)
			{
				
				//obrwy->SetPixel(i, j, 
				//	obrwy->GetPixel(i - 1, j) + obrwy->GetPixel(i, j - 1) - 
				//	obrwy->GetPixel(i - 1, j - 1) + obrwe->GetPixel(i, j));
				
				//obrwy->data[7];
				obrwy->data[i * (obrwe->_cols) + j] =
					obrwy->data[(i - 1) * obrwe->_cols + j] + obrwy->data[i * obrwe->_cols + j - 1] - 
					obrwy->data[(i - 1) * obrwe->_cols + j - 1] + obrwe->data[i * obrwe->_cols + j];

				//fprintf(plik, "%f\n", obrwy->data[i * (obrwe->_rows) + j]);

			}
			//if(i > (obrwe->_cols / 2)) break;
		}
		
		
		/*
		obrwe->getMinMax(min, max);
		fprintf(plik, "%f\n%f", min, max);
		obrwy->getMinMax(min, max);
		fprintf(plik, "\n%f\n%f", min, max);
		*/
		
		//for(i = 15 * obrwe->_cols + 1; i < 16 * obrwe->_cols; i++) obrwy->data[i] = 100;
		/*
		obrwy->data[0] = 100;
		for(i = 1; i < obrwe->_cols; i++) obrwy->data[i] = 100;
		
		for(i = 1; i < obrwe->_rows; i++) //3
			obrwy->data[i * obrwe->_cols] = 1;
		
		
		for(i = 1; i < obrwe->_rows; i++) //4
		{
			for(j = 1; j < obrwe->_cols; j++)
			{
				
				//obrwy->SetPixel(i, j, 
				//	obrwy->GetPixel(i - 1, j) + obrwy->GetPixel(i, j - 1) - 
				//	obrwy->GetPixel(i - 1, j - 1) + obrwe->GetPixel(i, j));
				
				//obrwy->data[7];
				//fprintf(plik, "%d  %d\n", i, j);
				if(j == 30) obrwy->data[i * obrwe->_cols + j] = 100;
				//if(obrwy->data[i * obrwe->_rows + j] == 0) fprintf(plik, "(%d, %d)  %d \n", i, j, i * obrwe->_rows + j);
					//obrwy->data[(i - 1) * obrwe->_rows + j] + obrwy->data[i * obrwe->_rows + j - 1] - 
					//obrwy->data[(i - 1) * obrwe->_rows + j - 1] + obrwe->data[i * obrwe->_rows + j];
				//if(i < 5 && j < 100)fprintf(plik, "%d ,", i * obrwe->_rows + j);
			}
			//fprintf(plik, "\n");
		}
		*/
		
		//fclose(plik);
		
		return 0;
	}

unsigned int WINAPI SAUVOLA(void* param)
{
	int i, j, imin, imax, jmin, jmax, fW;
	double wspW, wspW2, srednia, suma1, odSt, t;
	unsigned long w2;
	imin = ((struct SAV*)param)->imin;
	imax = ((struct SAV*)param)->imax;
	jmin = ((struct SAV*)param)->jmin;
	jmax = ((struct SAV*)param)->jmax;
	//MATLAB
	//wspW2 = 1 / (w^2 - 1);
	wspW = 1 / pow(w, 2.0);
	wspW2 = 1 / (pow(w, 2.0) - 1);
	w2 = pow(w, 2.0);
	fW = int(w / 2.0);			//floor(w)
	for(i = imin; i <= imax; i++)
	{
		for(j = jmin; j <= jmax; j++)
		{
			/* MATLAB
			srednia = 
				(II(i + floor(w/2), j + floor(w/2)) + 
				II(i - floor(w/2) - 1, j - floor(w/2) - 1) - 
				II(i - floor(w/2) - 1, j + floor(w/2)) - 
				II(i + floor(w/2), j - floor(w/2) - 1)) * 
				wspW;
			%         odSt = sqrt(wspW * sum(sum(A(i - floor(w/2):i + floor(w/2), j - floor(w/2):j + floor(w/2)))) - srednia^2);
			suma1 = 
				(II2(i + floor(w/2), j + floor(w/2)) + 
				II2(i - floor(w/2) - 1, j - floor(w/2) - 1) - 
				II2(i - floor(w/2) - 1, j + floor(w/2)) - 
				II2(i + floor(w/2), j - floor(w/2) - 1));
			odSt = sqrt(wspW2 * (suma1 - w^2 * srednia^2));
			t = srednia * (1 + k * ((odSt / double(R)) - 1));
			if(C(i, j) > t)
				B(i, j) = 1;
			end;
			*/
			
			
			srednia = 
				(II->data[(i + fW) * owe->_cols + j + fW] +
				II->data[(i - fW - 1) * owe->_cols + j - fW - 1] -
				II->data[(i - fW - 1) * owe->_cols + j + fW] -
				II->data[(i + fW) * owe->_cols + j - fW - 1]) *
				wspW;
			suma1 =
				II2->data[(i + fW) * owe->_cols + j + fW] +
				II2->data[(i - fW - 1) * owe->_cols + j - fW - 1] -
				II2->data[(i - fW - 1) * owe->_cols + j + fW] -
				II2->data[(i + fW) * owe->_cols + j - fW - 1];
			odSt = sqrt(wspW2 * (suma1 - w2 * pow(srednia, 2)));
			t = srednia * (1 + k * ((odSt / R) - 1));
			
			//t = R;
			
			if(owe->data[i * (owe->_cols) + j] > t)
				owy->data[i * (owe->_cols) + j] = 1;
			//owy->data[i * (owe->_cols) + j] = GetCurrentThreadId();
		}
	}
	return(0);
}


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"Sauvola");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = Sauvola(IMAGE,NUMBER,NUMBER)");
	definition->numofout = 1;
	definition->numofin = 3;
	definition->typesofin[0] = IMAGE; //Obraz do przetworzenia
	definition->typesofin[1] = NUMBER;//rozmiar okna (nieparzysty)
	definition->typesofin[2] = NUMBER;//parametr k (standardowo 0.2 - 0.5)
	//definition->typesofin[3] = NUMBER;//iloœæ procesorów
	definition->typesofout[0] = IMAGE;//Obraz przetworzony
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"Sauvola by LN");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void AssignGlobals(C_Variables *(*p_globals)[MAX_GLOBALS],int* p_lastglobal)
{
	//FILE * plik;
	//plik = fopen("c:/bizon.txt", "w");
    globals = p_globals;
    lastglobal = p_lastglobal;
    for(i = 0; i < *(lastglobal); i++)
	{
        if((*globals)[i]!=NULL)
	    {
            if(strcmp("numofproc", (*globals)[i]->name) == 0) 
			{
                ppp = (C_Matrix_Container*)(*globals)[i]->data;
                n = (int)ppp->data[0];
            }
	    }
	}
	//fprintf(plik, "%d", n);
	//fclose(plik);
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	//Ekstrakcja danych z wejœcia
	owe = (C_Image_Container*)intab[0]->data;
	owy->AllocateData(owe->_rows, owe->_cols);
	owy->Zeros();
	owe2->AllocateData(owe->_rows, owe->_cols);
	II->AllocateData(owe->_rows, owe->_cols);
	II->Zeros();
	II2->AllocateData(owe->_rows, owe->_cols);
	II2->Zeros();

	ppp = (C_Matrix_Container*)intab[1]->data;
	w = (int)ppp->data[0];
	ppp = (C_Matrix_Container*)intab[2]->data;
	k = (float)ppp->data[0];
	//ppp = (C_Matrix_Container*)intab[3]->data;
	//n = (int)ppp->data[0];
	
	//n = (int)NUMOFPROC;
	

	owe->getMinMax(min, max);
	R = ((max - min) / 2.0);

	//Wyliczanie owe2 (kwadraty wejœcia)
	for(i = 0; i < (owe->_cols * owe->_rows); i++)
	{
		owe2->data[i] = pow(owe->data[i], 2);
	}

	//Liczenie Integral Images na w¹tkach:
	cic[0].o1 = owe2;
	cic[0].o2 = II2;
	cic[1].o1 = owe;
	cic[1].o2 = II;
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, CII, (void*)&cic[0], 0, NULL);
	////hThread1[0] = (HANDLE)_beginthreadex(NULL,0, func, (void*)&ex1,0,NULL );
	//while(cicread);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, CII, (void*)&cic[1], 0, NULL);
	//CII(NULL);
	//WaitForSingleObject(hThread[0], INFINITE);
	//WaitForSingleObject(hThread[1], INFINITE);
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);
	//DZIA£A!!!!! : )))


	//LICZENIE SAMEGO PROGOWANIA SAUVOLA NA W¥TKACH:

	//Podzia³ obrazu
	if(w < owe->_cols && w < owe->_rows) //Sprawdzamy czy obraz jest wiêkszy od maski, inaczej dalsze liczenie nie ma sensu
	{
		if(owe->_cols - w < n || owe->_rows - w < n) n = 1;
		if(owe->_cols > owe->_rows) 
		{
			pom = owe->_cols - w;
			xlonger = true;
		}
		else
		{
			pom = owe->_rows - w;
			xlonger = false;
		}
		if(n <= 0) n = 1;
		d = (int)(pom / n);
		r = pom % n;
		for(i = 0; i < n; i++)
 		{
	 		g = j + d - 1;
			  
			if(r > 0)
			{
				r--;
				g++;
			}
			/*
			cout << "Przedzial " << i + 1 << ": ";
			cout << j << " - " << g << endl;
			*/
			if(xlonger)
			{
				sav[i].imin = (int)(w / 2.0) + 1;
				sav[i].imax = owe->_rows - (int)(w / 2.0) - 1;
				sav[i].jmin = j + (int)(w / 2.0);
				sav[i].jmax = g + (int)(w / 2.0);
			}
			else
			{
				sav[i].imin = j + (int)(w / 2.0) + 1;
				sav[i].imax = g + (int)(w / 2.0) + 1;
				sav[i].jmin = (int)(w / 2.0) + 2;
				sav[i].jmax = owe->_cols - (int)(w / 2.0);
			}
	        
			j = g + 1;
		}

		for(i = 0; i < n; i++)
		{
			hThread[i] = (HANDLE)_beginthreadex(NULL, 0, SAUVOLA, (void*)&sav[i], 0, NULL);
		}
		WaitForMultipleObjects(n, hThread, TRUE, INFINITE);
		for(i = 0; i < n; i++)
		{
			CloseHandle(hThread[i]);
		}
	}
	
	//Wyrzucenie przetworzonego obrazu na wyjœcie
	outtab[0]->AssignValue((void*)owy,err);
}
