#include "C_Lib.h"

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"testadd");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = test(IMAGE,IMAGE)");
	definition->numofout = 1;
	definition->numofin = 2;
	definition->typesofin[0] = IMAGE; definition->typesofin[1] = IMAGE;
	definition->typesofout[0] = IMAGE;
	definition->interface_version = 1;
	strcpy_s(definition->description,MAX_DESC,"TEST");
	definition->ADDIP_version = 2.0;
}

void SetOutput(VARIABLES* outtab, VARIABLES* intab)
{
	// variable output has size of numofout (set in Introduce function)
	outtab[0].type = IMAGE;
	outtab[0]._cols = intab[0]._cols;
	outtab[0]._rows = intab[0]._rows;
}

void Do(ERROR_DEF* err, VARIABLES* intab,int nargin, VARIABLES* outtab, int nargout)
{
	double *in1;
	double *in2;
	double *out;

	int a;

	in1 = (double*)intab[0].data;
	in2 = (double*)intab[1].data;
	if( (intab[0]._cols != intab[1]._cols) || (intab[0]._rows != intab[1]._rows))	{
		err->status = 1;
		strcpy_s(err->error,MAX_ERROR,"testadd: ERROR");
		return;
	}
	
	outtab[0].type = IMAGE;
	outtab[0]._cols = intab[0]._cols;
	outtab[0]._rows = intab[0]._rows;
	out = new double[intab[0]._cols*intab[0]._rows];
	for(a=0;a<intab[0]._cols*intab[0]._rows;a++)
		out[a] = in1[a]+in2[a];
	memcpy(outtab[0].data,(void*)(out),sizeof(double)*intab[0]._cols*intab[0]._rows);
	delete[] out;
}

/*
#pragma once
#define IMAGE  0x00000010
#define STRING 0x00000100
#define MATRIX 0x00001000
#define NUMBER MATRIX
#define DT UINT
#define MAX_VER 6
#define MAX_NAME 20
#define MAX_DESC 256
#define MAX_SYNT 256
#define MAX_NUMBER_FUNCTION_PARAM 50

struct ERROR_DEF {
	BOOL status;		// FALSE - no error
	char error[512];
};

struct FUNC_DEF {
	char version[MAX_VER];		// function version
	char name[MAX_NAME];		// function name
	char syntax[MAX_SYNT];		// example of use
	char description[MAX_DESC];	// whatever you want
	int numofout;				// number of input parameters
	int numofin;				// number of output parameters
	UINT typesofin[MAX_NUMBER_FUNCTION_PARAM];			// table of parameters type 
	UINT typesofout[MAX_NUMBER_FUNCTION_PARAM];
	unsigned int interface_version;		// version of interface
};

struct VARIABLES {
	DT type;
	void* data;
	unsigned int _rows;
	unsigned int _cols;
};
*/