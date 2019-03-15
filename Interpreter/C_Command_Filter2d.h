#pragma once
#include "c_commands.h"

class C_Command_Filter2d :
	public C_Commands
{
public:
	C_Command_Filter2d(void);
	virtual void Do(C_Error& err);
	virtual ~C_Command_Filter2d(void);
private:
	void sortquick(double* tabl, int l, int p);
};
