#pragma once
#include "c_commands.h"

class C_Command_Savefeatures :
	public C_Commands
{
public:
	C_Command_Savefeatures(void);
	virtual void Do(C_Error& err);
	virtual ~C_Command_Savefeatures(void);
};
