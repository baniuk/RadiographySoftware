#include "Includes.h"

// This is an empty project of DLL interface version 2.0


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"NNDataPrepare");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);  
    strcpy_s(definition->ftime,MAX_TIME,__TIME__);  
	strcpy_s(definition->syntax,MAX_SYNT,"MATRIX = NNDataPrepare(MATRIX,MATRIX,NUMBER)");
	definition->numofout = 1;
	definition->numofin = 3;
	definition->typesofin[0] = MATRIX; 
	definition->typesofin[1] = MATRIX;
	definition->typesofin[2] = NUMBER;
	definition->typesofout[0] = MATRIX;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"Prepering data to NN");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}
void AssignGlobals(C_Variables *(*p_globals)[MAX_GLOBALS],int* p_lastglobal)
{
        globals = p_globals;
        lastglobal = p_lastglobal;      
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Matrix_Container* pInTab;
	C_Matrix_Container InTab_temp;
	C_Matrix_Container* pRemoveParam;
	C_Matrix_Container* pDirection;

	pInTab = (C_Matrix_Container*)intab[0]->data;
	pRemoveParam = (C_Matrix_Container*)intab[1]->data;
	pDirection = (C_Matrix_Container*)intab[2]->data;
	pInTab->CloneObject(&InTab_temp);

	unsigned int Direction;
	unsigned int temp;
	unsigned int row, col;

	Direction = (unsigned int)pDirection->data[0];

	/*temp = pInTab->_cols - pRemoveParam->_cols;
	if (Direction == 1)// Features in columns, each row different defect
	{
		row = pInTab->_rows;
		col = temp;
	}
	else // Features in rows, each column different defect
	{
		row = temp;
		col = pInTab->_cols;
	}

	InTab_temp.AllocateData(row,col);*/

	NNDataMod(&InTab_temp,pInTab,pRemoveParam,Direction);

	outtab[0]->AssignValue((void*)&InTab_temp,err);
	
	InTab_temp.FreeData();
	
}

void NNDataMod(C_Matrix_Container* outFeats,
			   C_Matrix_Container* inFeats,
			   C_Matrix_Container* remFeats,
			   unsigned int dr)
{
	unsigned int i,j;
	double data1;

	outFeats->RemoveMatrixCol(remFeats);
	if (dr!=1) outFeats->Transpose();
		//Features in rows, each column different defect
}
