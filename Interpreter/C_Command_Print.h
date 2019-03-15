#pragma once
#include "c_commands.h"
/* Drukuje na ekranie lub gdzies indziej zawartosc zmiennej podanej jako parametr*/

class C_Command_Print :
	public C_Commands
{
public:
	C_Command_Print(void);
//	virtual void PrintYourself(void);
//	virtual DT* ReturnDTin(void);	//zwraca format danych wejsciowych
//	virtual DT* ReturnDTout(void);	//zwraca format danych wyjœciowych
//	virtual int ReturnNumIn(void);	// zwraca iloœæ parametrów wejœciowych
//	virtual int ReturnNumOut(void);	// zwraca iloœæ parametrów wyjœciowych
//	virtual char* ReturnName(void);
//	virtual void InitCommand(char* p_name,int p_numofinparam,int p_numofoutparam,DT* p_inputtype, DT* p_outputtype);
	virtual void Do(void);
public:
	virtual ~C_Command_Print(void);
};
