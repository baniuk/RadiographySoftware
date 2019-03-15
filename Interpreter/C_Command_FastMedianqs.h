#pragma once
#include "c_commands.h"


class C_Command_FastMedianqs :
	public C_Commands
{
public:
	C_Command_FastMedianqs(void);
	virtual void Do(C_Error& err);
	virtual ~C_Command_FastMedianqs(void);
private:
	void quickSort(double *Array,int *ArrayIndex, int array_size);
	void q_sort(double *Array,int *ArrayIndex, int left, int right);
};
