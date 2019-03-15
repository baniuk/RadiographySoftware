#include "Includes.h"

// This is an empty project of DLL interface version 2.0

void AddROIlines(C_Image_Container  *ImageROI, C_Matrix_Container *resultTabMedian);
void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"detectROI");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"[MATRIX,MATRIX,IMAGE] = detectROI(IMAGE,NUMBER,NUMBER,NUMBER,NUMBER,NUMBER,NUMBER,STRING)");
	definition->numofout = 3;
	definition->numofin = 8;
	definition->typesofin[0] = IMAGE; 
	definition->typesofin[1] = NUMBER;
	definition->typesofin[2] = NUMBER;
	definition->typesofin[3] = NUMBER;
	definition->typesofin[4] = NUMBER;
	definition->typesofin[5] = NUMBER;
	definition->typesofin[6] = NUMBER;
	definition->typesofin[6] = STRING;
	definition->typesofout[0] = MATRIX;
	definition->typesofout[1] = MATRIX;
	definition->typesofout[2] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"Detect ROI by a gauss function.");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Image_Container *Image1;
	C_Image_Container tempImage1;
	C_Image_Container ImageROI;

	C_Matrix_Container resultTab;
	C_Matrix_Container resultTabMedian;

	C_Matrix_Container *dy_temp;
	C_Matrix_Container *Xhaa_temp;
	C_Matrix_Container *Resolution_temp;
	C_Matrix_Container *Hup_temp;
	C_Matrix_Container *Hdown_temp;
	C_Matrix_Container *ErrorLim_temp;
	char *file;

	int Xhaa,Resolution,Hup,Hdown;
	int NrXLine;
	int x_res,y_res,temp_yres,dy;
	double ErrorLim;


	NrXLine = 7;

	Image1 = (C_Image_Container*)intab[0]->data;
	Image1->CloneObject(&tempImage1);
	Image1->CloneObject(&ImageROI);

	x_res = tempImage1._rows;
	y_res = tempImage1._cols;

	dy_temp=(C_Matrix_Container*)intab[1]->data;
	Xhaa_temp=(C_Matrix_Container*)intab[2]->data;
	Resolution_temp=(C_Matrix_Container*)intab[3]->data;
	Hup_temp=(C_Matrix_Container*)intab[4]->data;
	Hdown_temp=(C_Matrix_Container*)intab[5]->data;
	ErrorLim_temp=(C_Matrix_Container*)intab[6]->data;
	file = (char*)intab[7]->data;

	dy = (unsigned int)dy_temp->data[0];
	Xhaa = (int)Xhaa_temp->data[0];
	Resolution = (int)Resolution_temp->data[0];
	Hup = (int)Hup_temp->data[0];
	Hdown = (int)Hdown_temp->data[0];
	ErrorLim = (double)ErrorLim_temp->data[0];

	temp_yres=y_res%dy;
	if(temp_yres>0){temp_yres = ((y_res -temp_yres)/dy)+1;}
	else {temp_yres = (y_res -temp_yres)/dy;}

	resultTab.AllocateData(NrXLine,temp_yres);
	resultTabMedian.AllocateData(NrXLine,1);
	resultTab.Zeros();
	resultTabMedian.Zeros();


	detectROI(&tempImage1,
			&resultTab,
			dy,
			Xhaa,
			Resolution,
			Hup,
			Hdown,
			file
			);

	MedianValue(&resultTabMedian,&resultTab,NrXLine,temp_yres);
	AddROIlines(&ImageROI,&resultTabMedian);
	outtab[0]->AssignValue((void*)&resultTab,err);
	outtab[1]->AssignValue((void*)&resultTabMedian,err);
	outtab[2]->AssignValue((void*)&ImageROI,err);

	resultTab.FreeData();
	resultTabMedian.FreeData();
	
	dy_temp->FreeData();
	Xhaa_temp->FreeData();
	Resolution_temp->FreeData();
	Hup_temp->FreeData();
	Hdown_temp->FreeData();
	ErrorLim_temp->FreeData();
}
void AddROIlines(C_Image_Container  *ImageROI, C_Matrix_Container *resultTabMedian)
{
	int x_res,y_res,ystart,i,j,z,jump,x_size;
	double pixel,line;
	
	ImageROI->Normalize(0,65535);
	x_res = ImageROI->_rows;
	y_res = ImageROI->_cols;

	x_size = resultTabMedian->_rows;
	jump =1;
	ystart = 0;
	for(i=0;i<x_res;i+=jump)
	{
		for(j=ystart;j<y_res;j+=jump)
		{
			ImageROI->GetPixel(i,j,pixel);
			//pixel = 65535;
			pixel = 0.5* pixel;
			if(pixel>65535){pixel =65535;}
			ImageROI->SetPixel(i,j,pixel);
		}
		ystart=abs(ystart-1);
	}

	for(z=0;z<x_size;z++)
	{
		resultTabMedian->GetPixel(z,0,line);
		line = abs(line);
		double temp;
		
		temp = (double)x_res;

		if(line>=temp)
		{
			line = temp-1;
		}
		else if(line==0)
		{
			line = 1;
		}		
		for(j=0;j<y_res;j++)
		{	
			
			ImageROI->SetPixel((unsigned int)line,j,65535);
			ImageROI->SetPixel((unsigned int)(line-1),j,65535);
			
				
		}

	}

}
