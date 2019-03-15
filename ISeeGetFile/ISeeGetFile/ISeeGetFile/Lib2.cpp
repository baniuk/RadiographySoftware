/*****************************************************************************************
Program: Ekstrakcja nazwy pliku graficznego z pliku typu "dataset" programu ISee!
Autor: Lech Napiera�a
Parametry wej�ciowe:
	o - �cie�ka do pliku typu "dataset" kt�ry powinien zosta� przetworzony
Parametry wyj�ciowe:
	tmp - �cie�ka do pliku graficznego, zapisana w formacie ADDIP-a
Opis dzia�ania:
	Funkcja otwiera plik wej�ciowy, dokonuje z niego ekstrakcji informacji n/t zwi�zanego
	z nim pliku graficznego, po czym przetwarza �cie�k� do niego do formatu ADDIP-a i j�
	zwraca.
*****************************************************************************************/



#include "Cpp_Lib.h"
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//#include <winsock2.h>

#include<stdio.h>
#include <windows.h>
//#include <string.h>

int k;
//char tmp[] = "bizon";
FILE * plik;
FILE * plik2;
char znak;


//Funkcja przeszukuje plik od tego punktu w poszukiwaniu zadanego ci�gu.
//Zwraca pe�n� linijk� zawieraj�c� ten ci�g
//size to rozmiar ci�gu wej�ciowego
void find(char *we, char *wy, int size)
{
 	 char c[200];
 	 int i = 0;	//b�dzie indeksem w ci�gu
 	 int trafilismy = 1;
 	 while(1)
 	 {
         znak = fgetc(plik);
         if(znak == EOF) break;
         //je�eli sko�czy�a si� nam linijka
         if(znak == '\n')
         {
		     //je�eli trafilismy
		     if(trafilismy == 1)
		     {
   			     for(i = 0; i < sizeof(c); i++)
   			     {
	  	             *(wy + i) = c[i];
	  	         }
	  	         break;
 			 }
		     //czyszczenie wszyskiego i przygotowywanie na nowy obr�t p�tli
		     for(i = 0; i < 200; i++) c[i] = '\0';
		     i = 0;
		     trafilismy = 1;
		 }
		 else
		 {
		  	 //sprawdzanie czy trafili�my
		  	 if(i < size) if(znak != *(we + i)) trafilismy = 0;
		  	 
		  	 c[i] = znak;
		  	 i++;
		 }
     }
}



//iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii

// This is an empty project of DLL interface version 2.0


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"ISeeGetFile");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"STRING = ISeeGetFile(STRING)");
	definition->numofout = 1;
	//definition->numofout = 0;
	//definition->numofin = 1;
	definition->numofin = 1;
	definition->typesofin[0] = STRING; 
	//definition->typesofin[1] = IMAGE;
	definition->typesofout[0] = STRING;
	//definition->typesofin[0] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"ISeeGetFile");
	definition->ADDIP_version = 2.0;
}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	int i;
	char znak = 'a';
	char *o, in[200], out[200], tmp[200];
	o = (char*)intab[0]->data;
	plik = fopen(o, "r");
	
	in[0] = 'i';
	in[1] = 'm';
	in[2] = 'a';
	in[3] = 'g';
	in[4] = 'e';

	find(&in[0], &out[0], 5);

	//Przepisywanie nazwy pliku na wyj�cie, tak �eby addip poprawnie go wczyta�. Musi on by� w formacie: "c:\\a\\b.tif"

	//Je�eli ten if jest spe�niony, to znaczy �e nazwa pliku ma ju� na pocz�tku liter� dysku, nie trzeba jej dodawa�
	if(out[7] == ':')
	{
		k = 6;
		for(i = 6; i < 200; i++)
		{
 			tmp[i - k] = out[i];

 			if(out[i] == '\\')
 			{
 				//cout << "Spe�niony warunek" << endl;
 				k = k - 1;
 				tmp[i - k] = '\\';
 				//cout << i << " " << k << endl;
 				//cout << bufor << endl;
 			}
			if(out[i] == '\0') break;
		}
	}
	else //Ta cz�� kodu jest wykonywana je�eli plik znajduje si� na dysku C
	{
		tmp[0] = 'C';
		tmp[1] = ':';
		tmp[2] = '\\';
		tmp[3] = '\\';
		k = 2;
		for(i = 6; i < 200; i++)
		{
 			tmp[i - k] = out[i];

 			if(out[i] == '\\')
 			{
 				//cout << "Spe�niony warunek" << endl;
 				k = k - 1;
 				tmp[i - k] = '\\';
 				//cout << i << " " << k << endl;
 				//cout << bufor << endl;
 			}
			if(out[i] == '\0') break;
		}
	}
	
	outtab[0]->AssignValue((void*)tmp,err);
}