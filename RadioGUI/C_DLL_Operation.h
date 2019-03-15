#pragma once
#include "C_DLL_Name_Holder.h"
#include "DLL_C_Datatypes.h"

typedef void (__cdecl* DLL_INTR)(FUNC_DEF*);
typedef void (__cdecl* DLL_AG)(C_Variables *(*)[MAX_GLOBALS],int*);
typedef void (__cdecl* DLL_DO_INT1)(ERROR_DEF*, VARIABLES*,int, VARIABLES*, int);
typedef void (__cdecl* DLL_DO_INT2)(C_Error& , C_Variables** ,int, C_Variables**, int);
typedef void (__cdecl* DLL_SET_OUTPUT)(VARIABLES*, VARIABLES*);

//__cdecl
class C_DLL_Operation
{
public:
	C_DLL_Operation(void);
	~C_DLL_Operation(void);
	void AllocateDLLSpace(int numofdll);
	void AddDLL(char* name,char* programpatch);
	int lastdllindex;		// ilosc+1 znalezionych poprawnych bibliotek
	char* CheckName(char* name);	// sprwdza czy nazwa jest komend� i zwraca nazw� pliku
	C_DLL_Name_Holder* dllnameholder;	// tu bed� trzymae nazwy funkcji i odpowiadaj�ce im nazwy
private:
	char functionname[MAX_NAME];		// nazwa funkcji odczytana z dll
	FUNC_DEF data;
	HINSTANCE mylib;
	DLL_INTR Introduce;
	DLL_DO_INT1 Doo1;
	DLL_DO_INT2 Doo2;
	DLL_SET_OUTPUT SetOutput;
	BOOL CheckDLL(char* name, char* programpatch);		// otwiera i sprawdza czy to jest biblioteka ADDIP
};
