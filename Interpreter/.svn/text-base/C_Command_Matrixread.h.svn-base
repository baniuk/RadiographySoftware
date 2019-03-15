#pragma once
#include "c_commands.h"

class C_Command_Matrixread :
	public C_Commands
{
public:
	C_Command_Matrixread(void);
	virtual void Do(C_Error& err);
private:
	BOOL CheckSize(FILE* stream, unsigned int &row, unsigned int &col);
public:
	virtual ~C_Command_Matrixread(void);
};
