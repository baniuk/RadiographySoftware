#pragma once
#include "c_commands.h"

class C_Command_Add :
	public C_Commands
{
public:
	C_Command_Add(void);
	virtual void Do(C_Error& err);
public:
	virtual ~C_Command_Add(void);
};
