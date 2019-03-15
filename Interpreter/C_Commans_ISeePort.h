#pragma once
#include "c_commands.h"

class C_Commans_ISeePort :
	public C_Commands
{
public:
	C_Commans_ISeePort(void);
	virtual void Do(C_Error& err);
	virtual ~C_Commans_ISeePort(void);
};
