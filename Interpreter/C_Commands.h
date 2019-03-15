// Klasa zawiera definicje poszczeg�lnych instrukcji. Przechowuje nazwe instrukcji, ilo�� parametr�w wej�ciowych i wyj�ciowych oraz ich format
#pragma once
#include <string.h>
#include "DataTypes.h"
#include "C_Variables.h"
#include "C_Error.h"

class C_Commands
{
public:
	C_Commands(void);
	virtual void InitCommand(char* p_name,int p_numofinparam,int p_numofoutparam,DT* p_inputtype, DT* p_outputtype);
	virtual void PrintYourself(void);
	virtual DT* ReturnDTin(void);	//zwraca format danych wejsciowych
	virtual DT* ReturnDTout(void);	//zwraca format danych wyj�ciowych
	virtual int ReturnNumIn(void);	// zwraca ilo�� parametr�w wej�ciowych
	virtual int ReturnNumOut(void);	// zwraca ilo�� parametr�w wyj�ciowych
	virtual char* ReturnName(void);
	void RepairPath(char* Src, char* Dest);	// dodaje po dwa // do �cie�ki zakonczonej zerem
	C_Variables** intab;				// tablica zawieraj�ca adresy zmiennych wej�ciowych
	C_Variables** outtab;				// tablica zawieraj�ca adresy zmiennych wej�ciowych
	C_Variables *(*globals)[MAX_GLOBALS];				// tablica zawierajaca zmienne globalne  -tablica inicjalizowna w interpreter a tu tylko trzymany wska�nik
	int *lastglobal;		// ilosc zmienych globalnych w tablicy globals - ta ilosc jest trzymana w interpreter
	int current_global;
	virtual void Do(C_Error& err)=0;
	char* fun_ver;					// command version
	char* fun_time;
	char* fun_date;
	char* syntax;
	char* description;	// opis dowolny
	char name[MAXSIZE];		// nazwa
protected:
	size_t strlength;		// dlugosc stringa instrukcji
	int numofoutparam;		// ilosc parametr�wwej�ciowych
	int numofinparam;		// ilosc parametr�wwej�ciowych
	DT* inputtype;	// typ parametr�w wej�ciowych w kolejno�ci
	DT* outputtype;
	void* extarnalApi;	// to do jaki� zewn�trznych klas i obiekt�w
public:
	virtual ~C_Commands(void);
};
