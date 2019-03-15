#pragma once
#include "DLL_Size_Def.h"

struct ERROR_DEF {
	BOOL status;		// FALSE - no error
	char error[MAX_ERROR];
};

struct VARIABLES {
	DT type;
	void* data;
	unsigned int _rows;
	unsigned int _cols;
};