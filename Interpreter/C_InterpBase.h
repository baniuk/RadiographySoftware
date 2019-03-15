#pragma once
#include "C_Commands.h"
#include "C_Variables.h"
#include "C_Error.h"
#include "C_Line.h"
#include "DataTypes.h"
#include "C_DLL_Operation.h"
#include "C_CommandStack.h"

class C_InterpBase
{
public:
	C_InterpBase(void);
	C_InterpBase(CWnd* parent);
	C_Line** line;					// obiekt analizy linii
	C_DLL_Operation DLL_Container;	// do obsugi komend w dll
	BOOL AddLine(const char *lin,C_Error& err);	// Zwraca true jesli to jest linia. Jesli spacje albo komentarz to false
	void BuildLine(int num,C_Error& err);		// tworzy obiekty i buduje struktury linii o numerze num
	void CheckLine(int num,C_Error& err);		// sprawdza poprawnoœæ linii o numerze num
	void Start(C_Error& err,C_CommandStack& cs,unsigned int line);							// wykonuje skrypt
	void Step(int linenum, C_Error& err,C_CommandStack& cs);
	void PrintVariable(char* const name,C_Error& err); // pisze zmienna o nazwie name
	void PrintVariable(C_Variables* var,C_Error& err); // pisze zmienna o wskazniku var
	int GetVariable(char* const name,char* tab, C_Error& err); // zwraca zmienn¹ skonwertowan¹ na tekst. zwraca d³ugoœæ ci¹gu
	void SetDLLDir(char* _dlldir);
//	char* GetVariable(C_Variables* var,C_Error& err); // pisze zmienna o wskazniku var
	char dlldir[1024]; // sciezka do aktualnego katalogu z dll
private:
	void InitInterpreter(void);		//tworzy obiekty wszystkich funkcji
//	C_Error* CheckLine(void);		// sprawdza poprawnoœæ linii
	void CreateObject(C_Line* line,int index, C_Commands** ptab, C_Error &err,BOOL st);	// tworzy obieity funkcji w zale¿noœci od ich nazwy
	C_Variables* CreateVariable(C_Line* line,DT type,int index, C_Variables** pvar);	// tworzy obieity zmiennych w zale¿noœci od ich nazwy. Zwraca adres utworzonego obiektu 
	void FindVariable(C_Line* line,C_Error& err);			// znajduje zmienn¹ w tabeli (sprawdza czy jest zdefiniowana w workspace)
	void CheckInputParameters(C_Line* line,C_Error& err);	// sprawdza poprawnoœæ parametrów ejœciowych
	int GetIndexofVariable(char* name);						// zwraca index zmiennej o nazwie name w ablicy zmiennych
	C_Commands** tab;				// tablica wska¿ników zawieraj¹cych wszystkie obiekty funkcji
	C_Commands* tmpfun;				// tymczasowy obiekt potrzebny do analizy linii
	C_Variables** var;				// tablica wska¿ników zawieraj¹cych wszystkie obiekty zmiennych
	C_Variables* globals[MAX_GLOBALS];
	int lastglobal;			// index ostatniej zmiennej globalnej
	C_Error error;
	int lasttab;						// index ostatniej funkcji
	int lastvar;						// index ostatniej zmiennej zdefiniowanej w workspace
	int lastline;						// indeks ostatniej linii
	char strtmp[2];						// do porównywania
	CWnd* parentwindow;
	int DecodeType(UINT type, UINT* tab);	// do wielu typów
public:
	virtual ~C_InterpBase(void);
};
