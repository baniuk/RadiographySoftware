#pragma once
#include "c_commands.h"

class C_Command_Median :
	public C_Commands
{
public:
	C_Command_Median(void);
	virtual void Do(C_Error& err);
	virtual ~C_Command_Median(void);
private:
	double Hoare(double *tab,int size,int mid);
	double Partition(double* c, int a, int b);
};
