/*****************************************************************************************
Program: Ekstrakcja ROI z danego obrazu z wykorzystaniem informacji zapisanych w pliku 
	typu "dataset" programu ISee!
Autor: Lech Napiera³a
Parametry wejœciowe:
	original - obraz z którego nale¿y wyci¹æ ROI
	o - nazwa pliku typu "dataset" do przetworzenia
	roi - numer ROI które nas interesuje (0 - 9)
Parametry wyjœciowe:
	wyjscie - wyciête ROI
Opis dzia³ania:
	Funkcja otwiera zadany plik, dokonuje z niego ekstrakcji informacji n/t ROI,
	nastêpnie z ich wykorzystaniem wycina i zwraca odpowiedni fragment obrazu wejœciowego.
*****************************************************************************************/

#include "Cpp_Lib.h"
#include<stdio.h>
#include <windows.h>
#include<math.h>

//Zmienne:
FILE* plik;
char in[200], out[200];
char koniec[] = "exit\n";
char znak;
char tab[10];
int i, k, j;
int start_x = -1, start_y = -1, end_x = -1, end_y = -1;
C_Matrix_Container *ppp;


void find(char *we, char *wy, int size)
{
 	 char c[200];
	 for(i = 0; i < 200; i++) c[i] = '\0';
 	 int i = 0;	//bêdzie indeksem w ci¹gu
 	 int trafilismy = 1;
 	 while(1)
 	 {
         znak = fgetc(plik);
         if(znak == EOF) break;
         //je¿eli skoñczy³a siê nam linijka
         if(znak == '\n')
         {
		     //je¿eli trafilismy
		     if(trafilismy == 1)
		     {
   			     for(i = 0; i < sizeof(c); i++)
   			     {
	  	             *(wy + i) = c[i];
	  	         }
	  	         break;
 			 }
		     //czyszczenie wszyskiego i przygotowywanie na nowy obrót pêtli
		     for(i = 0; i < 200; i++) c[i] = '\0';
		     i = 0;
		     trafilismy = 1;
		 }
		 else
		 {
		  	 //sprawdzanie czy trafiliœmy
		  	 if(i < size) if(znak != *(we + i)) trafilismy = 0;
		  	 
		  	 c[i] = znak;
		  	 i++;
		 }
     }
}


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"ISeeGetROI");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = ISeeGetROI(IMAGE,STRING,NUMBER)");
	definition->numofout = 1;
	definition->numofin = 3;
	definition->typesofin[0] = IMAGE; 
	definition->typesofin[1] = STRING;
	definition->typesofin[2] = NUMBER;
	definition->typesofout[0] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"TEST");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Image_Container *original;
	C_Image_Container *wyjscie = new C_Image_Container;
	original = (C_Image_Container*)intab[0]->data;
	int roi;
	char *o;
	
	//roi zawiera numer roi który ma zostaæ
	ppp = (C_Matrix_Container*)intab[0]->data;
	o = (char*)intab[1]->data;
	roi = (int)ppp->data[2];	

	plik = fopen(o, "r");
 	in[0] = '[';
	in[1] = 'r';
	in[2] = 'o';
	in[3] = 'i';
	in[4] = '_';
	in[5] = (char)(48 + roi);	//obs³uguje tylko roi 1 do 9!
	find(&in[0], &out[0], 6);
	
	//cout << out << endl;
	
	//czyszczenie outa
	for(i = 0; i < 200; i++) out[i] = '\0';

	in[0] = 's';
	in[1] = 't';
	in[2] = 'a';
	in[3] = 'r';
	in[4] = 't';
	in[5] = '_';
	in[6] = 'x';
	in[7] = '=';
	find(&in[0], &out[0], 8);
	
	//analizowanie out'a
	k = 0;
	for(i = 8; i < 18; i++) tab[i - 8] = out[i];
	for(i = 0; i < 10; i++) if(tab[i] != '\0') k++;
	for(i = 0; i < k; i++) start_x += ((int)tab[i] - 48) * (int)pow(10.0, k - i) / 10;
	
	//cout << out << " " << start_x << endl;
	//czyszczenie outa
	for(i = 0; i < 200; i++) out[i] = '\0';
	for(i = 0; i < 10; i++) tab[i] = '\0';
	
	in[0] = 's';
	in[1] = 't';
	in[2] = 'a';
	in[3] = 'r';
	in[4] = 't';
	in[5] = '_';
	in[6] = 'y';
	in[7] = '=';
	find(&in[0], &out[0], 8);
	
	k = 0;
	for(i = 8; i < 18; i++) tab[i - 8] = out[i];
	for(i = 0; i < 10; i++) if(tab[i] != '\0') k++;
	for(i = 0; i < k; i++) start_y += ((int)tab[i] - 48) * (int)pow(10.0, k - i) / 10;
	
	//cout << out << " " << start_y << endl;
	//czyszczenie outa
	for(i = 0; i < 200; i++) out[i] = '\0';
	for(i = 0; i < 10; i++) tab[i] = '\0';
	
	in[0] = 'e';
	in[1] = 'n';
	in[2] = 'd';
	in[3] = '_';
	in[4] = 'x';
	in[5] = '=';
	//in[6] = '\0';
	//in[7] = '\0';
	find(&in[0], &out[0], 6);
	
	k = 0;
	for(i = 6; i < 18; i++) tab[i - 6] = out[i];
	for(i = 0; i < 10; i++) if(tab[i] != '\0') k++;
	for(i = 0; i < k; i++) end_x += ((int)tab[i] - 48) * (int)pow(10.0, k - i) / 10;
	
	//cout << out << " " << end_x << endl;
	//czyszczenie outa
	for(i = 0; i < 200; i++) out[i] = '\0';
	for(i = 0; i < 10; i++) tab[i] = '\0';
	
	in[0] = 'e';
	in[1] = 'n';
	in[2] = 'd';
	in[3] = '_';
	in[4] = 'y';
	in[5] = '=';
	//in[6] = '\0';
	//in[7] = '\0';
	find(&in[0], &out[0], 6);
	
	k = 0;
	for(i = 6; i < 18; i++) tab[i - 6] = out[i];
	for(i = 0; i < 10; i++) if(tab[i] != '\0') k++;
	for(i = 0; i < k; i++) end_y += ((int)tab[i] - 48) * (int)pow(10.0, k - i) / 10;

	
	if(start_x < 0) start_x = 0;
	if(start_y < 0) start_y = 0;
	if(end_x > original->_cols - 1 || end_x == -1) end_x = original->_cols - 1;
	if(end_y > original->_rows - 1 || end_y == -1) end_y = original->_rows - 1;
	

	fclose(plik);
	wyjscie->AllocateData(end_y - start_y, end_x - start_x);

	//plik = fopen("c:/kalafior.txt", "w");
	for(i = start_y; i < end_y; i++)
	{
		for(j = start_x; j < end_x; j++)
		{
			//fprintf(plik, "%d %d\n", i, j);
			//wyjscie->SetPixel(i - start_y, j - start_x, original->GetPixel(i, j));
			//obrwy->data[i * (obrwe->_cols) + j] =
			wyjscie->data[(i - start_y) * (wyjscie->_cols) + (j - start_x)] = 
				original->data[i * (original->_cols) + j];
		}
	}
	//fclose(plik);
	//remove("c:/set.txt");


	outtab[0]->AssignValue((void*)wyjscie,err);
}