#include "Cpp_Lib.h"

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"getimsize");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"[NUMBER,NUMBER] = getimsize(IMAGE)");
	definition->numofout = 2;
	definition->numofin = 1;
	definition->typesofin[0] = IMAGE;
	definition->typesofout[0] = NUMBER; definition->typesofout[1] = NUMBER;
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
	C_Image_Container* image;
	C_Matrix_Container row;
	C_Matrix_Container col;
	image = (C_Image_Container*)intab[0]->data;

	row.AllocateData(1,1);
	col.AllocateData(1,1);
	row.data[0] = image->_rows;
	col.data[0] = image->_cols;
	outtab[0]->AssignValue((void*)&row,err);
	outtab[1]->AssignValue((void*)&col,err);

}