#pragma once
#include "C_Error.h"
#include "DataTypes.h"
/*
	Klasa odpowiedzialna za po³¹czenie danych reprezentowanych przez inne klasy z nazw¹ zmiennej w jêzyku skryptowym
*/
class C_Variables
{
public:
	C_Variables(void);
	char name[maxnameofleftparametr];
	DT type;
	void* data;
	virtual void AssignValue(void* value, C_Error& err)=0;
	virtual void AssignName(const char* pname);
public:
	virtual ~C_Variables(void);
};
