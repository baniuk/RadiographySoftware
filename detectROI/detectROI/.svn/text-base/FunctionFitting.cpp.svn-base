#include "Includes.h"

void FuncFitting(C_Image_Container *scr,
			C_Image_Container *dst,
			int FuncModel,
			double xDataScale,
			double yDataScale,
			double paramBoundsPercent,
			bool yRef,
			int dy,
			int MedFilt,
			double *paramTabDouble)
{
	C_Image_Container ImageCopy;
	int x_res, y_res,i,j;
	double *xData, *yData,*paramResults,*p,*opts;
	double *paramResultsBoundsLow,*paramResultsBoundsUp;
	double pixel,minVal;
	int maxNrIterFirstCol,maxNrIterNexttCol; 




	// make zero data to dst image
	dst->Zeros();

	x_res = scr->_rows;
	y_res = scr->_cols;

	//Making a copy of intpu image class
	
	scr->CloneObject(&ImageCopy);
	
	
    
	// Alocating memory for a variables

	xData = new double[x_res];
	yData = new double[x_res];
	p = new double[models[FuncModel]->numOfParam];
	paramResults = new double[models[FuncModel]->numOfParam];
	paramResultsBoundsLow = new double[models[FuncModel]->numOfParam];
	paramResultsBoundsUp  = new double[models[FuncModel]->numOfParam];
	opts = new double[5];

	// setting initial Approximation Value of parameters


		

	// Generating start value of function parameters

	InitializeParamValue(paramResults,models[FuncModel]->numOfParam,0.1);
	InitializeParamValue(paramResultsBoundsUp,models[FuncModel]->numOfParam, 0.0);
	InitializeParamValue(paramResultsBoundsLow,models[FuncModel]->numOfParam, 0.0);

	//*(paramResults+2) = (x_res -(x_res%2))/2;//it works for one gauss function otherwise coment this
	
	

	// Generating  x data for all columns and scaling
	for(i=0;i<x_res;i++)
		{
			
			*(xData+i)=((double) i);
			if(xDataScale >0)
			{
				*(xData+i)=((*(xData+i)/x_res)*xDataScale);
				*(paramResults+2)=((*(paramResults+2)/x_res)*xDataScale);//it works for one gauss function otherwise coment this

			}// Scaling the x data to the xDataScale value
			
		}
	
	//scaling the y data to the yDataScale value;
	
	if(yDataScale >0)
	{
		ImageCopy.Normalize(0,(int)yDataScale);

	}
	else
	{
		ImageCopy.Normalize(0,65535);
	}
	
	if(MedFilt>0)
	{
		FastMedian(&ImageCopy,&ImageCopy,MedFilt,MedFilt);
	}




	// move to zero ( looking min value)
	//ImageCopy.getMinMax(minVal,maxVal);
	
	int omitPix = 5;

	
	for(j=0;j<y_res;j++)
	{
		minVal = 255*255;
		for(i=omitPix;i<x_res-omitPix;i++)
		{
			ImageCopy.GetPixel(i,j,pixel);
			if(minVal > pixel) 
			{
				minVal = pixel;
			}
		}
		
		for(i=0;i<x_res;i++)
		{
			ImageCopy.GetPixel(i,j,pixel);
			ImageCopy.SetPixel(i,j,abs(pixel - minVal));
		}

	}
	/*ofstream plik("C:\\Documents and Settings\\Marcin\\Pulpit\\Result\\scrIm1.txt");
	for(i=0;i<x_res;i++)
	{for(j=0;j<y_res;j++){
		ImageCopy.GetPixel(i,j,pixel);
		plik<<pixel<<" ";}
		plik<<endl;}*/

	LowingEdge(&ImageCopy,omitPix,dy);


	
	

	////
	maxNrIterFirstCol = (int)*(paramTabDouble+0);
	maxNrIterNexttCol = (int)*(paramTabDouble+1);

	*(opts+0) = *(paramTabDouble+2);
	*(opts+1) = *(paramTabDouble+3);
	*(opts+2) = *(paramTabDouble+4);
	*(opts+3) = *(paramTabDouble+5);
	*(opts+4) = *(paramTabDouble+6);


	
	
	

	
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
	// Generating  y data
	for(j=0;j<y_res;j+=dy)
	{
		//printf("%d / %d\n",j+1,y_res); // show curently approximated column
		for(i=0;i<x_res;i++)
		{
			ImageCopy.GetPixel(i,j,pixel);
			*(yData+i)= pixel;
		}

		// Approximation of a single column ( getting a parameters value of a approximation function)
		
			if(paramBoundsPercent==NULL )
			{
			curve_fit(models[FuncModel]->funcPrt,
				models[FuncModel]->funcName,
				yData, 
				xData,
				NULL,
				NULL,
				x_res,
				maxNrIterFirstCol,//Parameters[0]->maxNrIterFirstCol,
				models[FuncModel]->numOfParam,
				paramResults,
				opts);//Parameters[0]->opts);
			}
			
			// set prameters bounds 
			
			else if(paramBoundsPercent>NULL)
			{
				if(j==0)
				{
				curve_fit(models[FuncModel]->funcPrt,
				models[FuncModel]->funcName,
				yData, 
				xData,
				NULL,
				NULL,
				x_res,
				maxNrIterFirstCol,//Parameters[0]->maxNrIterFirstCol,
				models[FuncModel]->numOfParam,
				paramResults,
				opts);//Parameters[0]->opts);
					
					
				InitializeParamBoundsValue(paramResults,paramResultsBoundsLow,paramResultsBoundsUp,models[FuncModel]->numOfParam,paramBoundsPercent);
					
				}
				else 
				{			
				if(yRef==0)
				{
					InitializeParamBoundsValue(paramResults,paramResultsBoundsLow,paramResultsBoundsUp,models[FuncModel]->numOfParam,paramBoundsPercent);
				}
				curve_fit(models[FuncModel]->funcPrt,
				models[FuncModel]->funcName,
				yData, 
				xData,
				paramResultsBoundsLow,
				paramResultsBoundsUp,
				x_res,
				maxNrIterNexttCol,//Parameters[0]->maxNrIterNexttCol,
				models[FuncModel]->numOfParam,
				paramResults,
				opts);//Parameters[0]->opts);
				}
			}

		// Reciving a column from a approximated function
		models[FuncModel]->funcPrt(paramResults,yData,models[FuncModel]->numOfParam,x_res,xData);
		for(i=0;i<x_res;i++)
		{
			
			dst->SetPixel(i,j,*(yData+i));
		}
	}

	

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

	delete[] xData;
	delete[] yData;
	delete[] paramResults;
	delete[] paramResultsBoundsLow;
	delete[] paramResultsBoundsUp;
	delete[] p;
	delete[] opts;
	ImageCopy.FreeData();
	
}
void InitializeParamValue(double *p,int ParamNR,double InitialValue)
{
	int i;
	for(i=0;i<ParamNR;i++)*(p+i)=InitialValue;
}
void InitializeParamBoundsValue(double *paramResults,double *paramResultsBoundsLow,double *paramResultsBoundsUp,int ParamNR,double paramBoundsPercent)
{
	int i;

	for(i=0;i<ParamNR;i++)
	{
		*(paramResultsBoundsLow+i) = *(paramResults+i)- (fabs(*(paramResults+i))*paramBoundsPercent);
		*(paramResultsBoundsUp+i) = *(paramResults+i)+ (fabs(*(paramResults+i))*paramBoundsPercent);
	}

}

void LowingEdge(C_Image_Container *Image,int omitPix,int dy)
{
	double *yData,xLeftStart,xRightStart,pixel,tempL,tempR;
	int i,j,x_res,y_res;
	
	x_res = Image->_rows;
	y_res = Image->_cols;

	yData = new double[x_res];

	for(j=0;j<y_res;j+=dy)
	{
		for(i=0;i<x_res;i++)
		{
			Image->GetPixel(i,j,pixel);
			*(yData+i)= pixel;
		}
		xLeftStart = *(yData+omitPix);
		xRightStart = *(yData+x_res-omitPix-1);

		for(i=0;i<x_res;i++)
		{
			tempL = (xLeftStart/(((double)x_res)*((double)x_res-i)));
			tempR = (xRightStart/(((double)x_res))*((double)(i+1)));
			pixel = *(yData+i) - abs(tempL-tempR);
			Image->SetPixel(i,j,pixel);
		}
		/*
		for(i=0;i<x_res;i++)
		{
			tempR = (xRightStart/x_res)*(i+1);
			pixel = *(yData+i) - tempR;
			Image->SetPixel(i,j,pixel);	
		}*/
	}

	delete[] yData;

}


