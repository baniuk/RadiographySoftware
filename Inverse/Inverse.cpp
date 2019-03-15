#include "Cpp_Lib.h"
#include <math.h>

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"inverse");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = inverse(IMAGE)");
	definition->numofout = 1;
	definition->numofin = 1;
	definition->typesofin[0] = IMAGE;
	definition->typesofout[0] = IMAGE;
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
	C_Image_Container out;
	image = (C_Image_Container*)intab[0]->data;
	double min,max;
	unsigned int l;
	out.AllocateData(image->_rows,image->_cols);
	image->getMinMax(min,max);
//m = max(a);
//out = abs(a-m)+min(a)
	
	for(l=0;l<image->GetNumofElements();l++)
		out.data[l] = fabs(image->data[l]-max) + min;
	outtab[0]->AssignValue((void*)&out,err);

}