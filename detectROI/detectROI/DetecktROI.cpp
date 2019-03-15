
#include "Includes.h"

void detectROI(C_Image_Container *scr,
			C_Matrix_Container *resultTab,
			int dy,
			int Xhaa,
			int Res,
			int Hup,
			int Hdown,
			char *file
			)
{
	
	C_Image_Container dst;
	int x_res,y_res;
	double xSc,ySc,boundLim;
	bool refY;
	int MedMask;
	int i;
	double *paramTabDouble,pixel;
	



	x_res = scr->_rows;
	y_res = scr->_cols;
	scr->CloneObject(&dst);
	dst.Zeros();

	paramTabDouble = new double[12];
	


	//readParamFromFile(paramTabDouble,"C:\\Program Files\\PSz\\ADDIP\\ADDIP\\DLL\\ApproximationParam.txt");
	readParamFromFile(paramTabDouble,file);

	xSc = *(paramTabDouble+7);
	ySc = *(paramTabDouble+8);
	boundLim = *(paramTabDouble+9);
	if(*(paramTabDouble+10)==0) refY=0;else refY=1;
	MedMask = (int)*(paramTabDouble+11);

	
	//FuncFitting(scr,&dst,0,1,0,0.2,0,dy,3);
	FuncFitting(scr,&dst,0,xSc,ySc,boundLim,refY,dy,MedMask,paramTabDouble);
//void FuncFitting(C_Image_Container *scr,
			//C_Image_Container *dst,
			//int FuncModel,
			//double xDataScale,
			//double yDataScale,
			//double paramBoundsPercent,
			//bool yRef,
			//int dy,
			//int MedFilt);
	
	
	
	SimpleDetect(&dst,dy,Xhaa,Res,Hup,Hdown,resultTab);
	
	//SimpleDetect(C_Image *dst,
				  //int xyArea[4],
				 // int dy,
			     // int Xhaa,
			     // int Res,
			     // int Hup,
				 // int Hdown}


	dst.FreeData();
	delete[] paramTabDouble;
	
}

void SimpleDetect(C_Image_Container *dst,
				  int dy,
			      int Xhaa,
			      int Res,
			      int Hup,
				  int Hdown,
				  C_Matrix_Container *resultTab)
{
	int x_res,y_res;
	int i,j,z;
	
	double *yData;
	double pixel,Value,TempValueUp,TempValueDown;
	int xstart,xend;
	int XhaaDistance,xposition,xUpPositionL,xUpPositionR,xDownPositionL,xDownPositionR,XhaaDistanceL,XhaaDistanceR;

	XhaaDistance = (Xhaa * 1000)/Res;
	
	x_res = dst->_rows;
	y_res = dst->_cols;

	
	yData = new double[x_res];
	


		xstart = 1;
		xend = x_res;

	
	
	z=0;
	for(j=0;j<y_res;j+=dy)
	{
		
		for(i=0;i<x_res;i++)
		{
			dst->GetPixel(i,j,pixel);
			*(yData+i)= pixel;
		}

		Maximum(&Value,&xposition,yData,x_res);


		 
		

		resultTab->SetPixel(3,z, (double)xposition);
		
		

		TempValueUp = ((100-(double)Hup)*Value)/100;

	

		SpecValue(&xUpPositionL, yData,x_res,TempValueUp);
		xUpPositionR = xposition + (xposition - xUpPositionL);

		TempValueDown = ((double)Hdown * Value)/100;

		
	

		SpecValue(&xDownPositionL, yData,x_res,TempValueDown);
		xDownPositionR = xposition + (xposition - xDownPositionL); 
		
		XhaaDistanceL = xDownPositionL - XhaaDistance;
		if(XhaaDistanceL<xstart)XhaaDistanceL = -xstart;
		resultTab->SetPixel(0,z,(double)XhaaDistanceL);

		resultTab->SetPixel(1,z,(double)xDownPositionL);
		resultTab->SetPixel(2,z,(double)xUpPositionL);
		resultTab->SetPixel(4,z,(double)xUpPositionR);
		resultTab->SetPixel(5,z,(double)xDownPositionR);

		XhaaDistanceR = xDownPositionR + XhaaDistance;
		if(XhaaDistanceR>xend)XhaaDistanceR = -xend;
		resultTab->SetPixel(6,z,(double)XhaaDistanceR);
		
		z=z+1;
	}

	delete[] yData;


	

}
void Maximum(double *Value,int *position, double *vect,int vectSize)
{
	int i,x;
	double Xmax;
	
	x=0;
	Xmax = *(vect+x);
	
	for(i=0;i<vectSize;i++)
	{
		if(*(vect+i)>Xmax)
		{
			Xmax = *(vect+i);
			x = i;
		}

	}
	*(Value+0) = Xmax;
	*(position+0) = x;
	

}
void SpecValue(int *position, double *vect,int vectSize,double SpecValue)
{
	int i,x;
	x=0;
	
	
	for(i=1;i<vectSize-1;i++)
	{
		if(*(vect+i-1)<=SpecValue && *(vect+i+1)>=SpecValue)
		{			
			x = i;
		} 
	}
	*position = x;
}
void MedianValue(C_Matrix_Container *resultTabMedian,C_Matrix_Container *resultTab,int x_size,int y_size)
{	
	int i,j,med_ind;
	double pixel,med_val;
	double *temp_Vect;


	temp_Vect = new double[y_size];

	
	for(i=0;i<x_size;i++)
	{
		for(j=0;j<y_size;j++)
		{
			resultTab->GetPixel(i,j,pixel);
			*(temp_Vect+j) = pixel;
		}
		quickSort(temp_Vect,0,y_size,0);
		med_ind = y_size%2;
		if(med_ind>0){med_ind = ((y_size - med_ind)/2)+1;}
		else {med_ind = ((y_size - med_ind)/2);}

		med_val = *(temp_Vect+med_ind);
		
		resultTabMedian->SetPixel(i,0,med_val);

	}


	delete[] temp_Vect;
	

}

void readParamFromFile(double *paramTabDouble,char *file)
{
	 ifstream fin;
	 fin.open(file);
	 char value,data[10];
	 int cont,paramNr,i;
	 double temp;
	
	 paramNr=0;
	 if (!fin.is_open())
	 {
		 /*
		*(paramTabDouble+0) = Parameters[0]->maxNrIterFirstCol;
		*(paramTabDouble+1) = Parameters[0]->maxNrIterNexttCol;
		*(paramTabDouble+2) = Parameters[0]->opts[0];
		*(paramTabDouble+3) = Parameters[0]->opts[1];
		*(paramTabDouble+4) = Parameters[0]->opts[2];
		*(paramTabDouble+5) = Parameters[0]->opts[3];
		*(paramTabDouble+6) = Parameters[0]->opts[4];
		*(paramTabDouble+7) = 1;
		*(paramTabDouble+8) = 0;
		*(paramTabDouble+9) = 0.2;
		*(paramTabDouble+10) = 0;
		*(paramTabDouble+11) = 3;*/
	 }
	 else
	 {
		 while (fin.good()) 
		 {
			cont=0;
			value = fin.get();
			if(value =='=')
			{
				value = fin.get();

				while(value!=';')
				{	
					
					data[cont] = value;
					cont++;		
					value = fin.get();
				}
				
				for(i=cont;i<10;i++)
				{
					data[i]=' ';
				}
				temp = atof(data);
				*(paramTabDouble+paramNr)= temp;
				paramNr++;
			}
		 }
		 fin.close();
	 }
}
