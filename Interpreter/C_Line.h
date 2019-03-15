#pragma once
#include "C_Error.h"
#include "DataTypes.h"

/*
	Klasa odpowiedzialna za analiz� jedej linii kodu i wyodr�bnienie nazw
	funkcji, parametr�w wej�ciowych i wyj�ciowych
*/
class C_Line
{
public:
	C_Line(void);
	C_Line(int num);
	void AnalyseLine(const char* line,C_Error& err);	// rozk�ada linie na czynniki: nazwy parametr�w, funkcji itp. 
	char** LHPF;			// tablica z naz parametru lewego a
	char FN[256];			// tablica z naz funkcji sin
	char** RHPF;				// nazwy parametr�w funkcji (tablica wska�nik�w do wska�nik�w nazw)
	BOOL* RHPFTYPE;				// true je�li dana jest zmienn� false je�li liczb� (skoordynowane z RHPF)
	BOOL FNTYPE;				// true je�li nazwa funkcji jest liczb�
	BOOL* STRTYPE;				// true je�li w fn lub rhpf jest string. jesli w fn to strtype[0]
	char linetmp[maxline];	// tablica z lini� a=sin(z,10)
	int numofrightparams;		// ilosc parametr�w wej�ciowych funkcji
	int numofleftparams;		// ilosc parametr�w wyj�ciowych funkcji
	int numer;					// numer linii od zera liczony
	int numofchars;			//ilosc znakow w linii
private:
	C_Error error;
	char RHP[256];			// tablica z naz parametru prawego (funkcja+parametry) sin(z,10)
	char PARAM[256];		// parametry funkcji z,10

	int iloscprzecinkow;
	char strtemp1[2];
	char strtemp2[2];
	bool czysaparametryfunkcji;		// true jesli sa parametry funkcji
	int RemoveSpaces(char* line); // usuwa spacje i zwraca nowa dlugosc linii
	int FindChar(char* line, const char znak);	// zwraca indeks danego znaku
	void CutLine(char* line, char* linecutted, int start, int stop); // wycina fragment ci�gu od start do stop
	int GetLengthOfString(char* str, int maxlen);		// zwraca d�ugo�� stringa z uwzglednieniem znakow specjalnych
	int GetNumofLeftParam(char* lewe, C_Error& err);
	void ExtractLeft(char* lewe, char** LHPF, C_Error& err);	// wyciaga lewe parametry i zwraca ich ilosc
public:
	virtual ~C_Line(void);
};
