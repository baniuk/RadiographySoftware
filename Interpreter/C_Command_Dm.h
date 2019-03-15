#pragma once
#include "c_commands.h"

class C_Command_Dm :
	public C_Commands
{
public:
	C_Command_Dm(void);
	virtual void Do(C_Error& err);
private:
	BOOL CheckSize(char* stream, unsigned int &row, unsigned int &col);
public:
	virtual ~C_Command_Dm(void);
};
