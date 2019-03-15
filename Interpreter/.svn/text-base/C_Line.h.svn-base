#pragma once
#include "C_Error.h"
#include "DataTypes.h"

/*
	Klasa odpowiedzialna za analizê jedej linii kodu i wyodrêbnienie nazw
	funkcji, parametrów wejœciowych i wyjœciowych
*/
class C_Line
{
public:
	C_Line(void);
	C_Line(int num);
	void AnalyseLine(const char* line,C_Error& err);	// rozk³ada linie na czynniki: nazwy parametrów, funkcji itp. 
	char** LHPF;			// tablica z naz parametru lewego a
	char FN[256];			// tablica z naz funkcji sin
	char** RHPF;				// nazwy parametrów funkcji (tablica wska¿ników do wska¿ników nazw)
	BOOL* RHPFTYPE;				// true jeœli dana jest zmienn¹ false jeœli liczb¹ (skoordynowane z RHPF)
	BOOL FNTYPE;				// true jeœli nazwa funkcji jest liczb¹
	BOOL* STRTYPE;				// true jeœli w fn lub rhpf jest string. jesli w fn to strtype[0]
	char linetmp[maxline];	// tablica z lini¹ a=sin(z,10)
	int numofrightparams;		// ilosc parametrów wejœciowych funkcji
	int numofleftparams;		// ilosc parametrów wyjœciowych funkcji
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
	void CutLine(char* line, char* linecutted, int start, int stop); // wycina fragment ci¹gu od start do stop
	int GetLengthOfString(char* str, int maxlen);		// zwraca d³ugoœæ stringa z uwzglednieniem znakow specjalnych
	int GetNumofLeftParam(char* lewe, C_Error& err);
	void ExtractLeft(char* lewe, char** LHPF, C_Error& err);	// wyciaga lewe parametry i zwraca ich ilosc
public:
	virtual ~C_Line(void);
};
