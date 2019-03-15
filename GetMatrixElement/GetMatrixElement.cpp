#include "Cpp_Lib.h"

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"getmatrixelement");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"[MATRIX] = getmatrixelement(MATRIX,MATRIX,MATRIX)");
	definition->numofout = 1;
	definition->numofin = 3;
	definition->typesofin[0] = MATRIX;definition->typesofin[1] = MATRIX;definition->typesofin[2] = MATRIX;
	definition->typesofout[0] = MATRIX;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"developed by PB");
	definition->ADDIP_version = 2.0;
}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Matrix_Container* input;
	C_Matrix_Container* row;
	C_Matrix_Container* col;
	C_Matrix_Container out;
	input = (C_Matrix_Container*)intab[0]->data;
	row = (C_Matrix_Container*)intab[1]->data;
	col = (C_Matrix_Container*)intab[2]->data;
	double min,max;
	unsigned int r,c,lr,lc;

	if(row->_rows>1 || col->_rows>1)	{
		err.SetError("getmatrixelement: ROW and COL must be a vector of 1xR size");
		return;
	}
	row->getMinMax(min,max);
	if(min<0 || max>input->_rows-1)	{
		err.SetError("getmatrixelement: ROW element is out of bounds");
		return;
	}
	col->getMinMax(min,max);
	if(min<0 || max>input->_cols-1)	{
		err.SetError("getmatrixelement: COL element is out of bounds");
		return;
	}
	out.AllocateData(row->_cols,col->_cols);

	lc = lr = 0;
	for(r=0;r<row->_cols;r++)	{
		for(c=0;c<col->_cols;c++)	{
			out.SetPixel(lr,lc,input->GetPixel(row->data[r],col->data[c]));
			lc++;
		}
		lc = 0;
		lr++;
	}


	outtab[0]->AssignValue((void*)&out,err);

}