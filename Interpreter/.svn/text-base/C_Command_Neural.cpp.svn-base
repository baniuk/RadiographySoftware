#include "StdAfx.h"
#include "C_Command_Neural.h"

C_Command_Neural::C_Command_Neural(void)
{
	UINT parint[2] = {IMAGE,STRING};
	UINT parout[1] = {IMAGE};
	InitCommand("ANNthreshold",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = ANNthreshold(IMAGE,STRING)";
	fun_ver = "1.1";
}

C_Command_Neural::~C_Command_Neural(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Neural::Do(C_Error& err)
{
	C_Image_Container *img;
	C_Image_Container dest;
	char* ANN_filename;

	ANN_filename = (char*)intab[1]->data;
	img = (C_Image_Container*)intab[0]->data;
	dest.AllocateData(img->_rows,img->_cols);

	double In[40000], *pIn;
	pIn = &In[0];
	double Out, *pOut;
	pOut = &Out;
	ANN_params *pANN_set;
	pANN_set = &ANN_set;
	int height,width;
	double *data;
	int i,j;
	double pict_depth=65535;
	// ANN file loading	
	if(ANN_loading(ANN_filename, pANN_set))	{
		err.SetError("ANNtreshold: Can't read *.ann file");
		return;
	}
	// acces to active bits of the mask 
	ANN_mask_acces(pANN_set);

	int mask_xRadius = (int)ceil(1.0*ANN_set.maskSize.cols/2)+1;
	int mask_yRadius = (int)ceil(1.0*ANN_set.maskSize.rows/2)+1;
		
	// get the image data
	height = dest._rows;
	width = dest._cols;
	data = (double *)img->data;
	
	// process the image
	for(i=0;i<height;i++) for(j=0;j<width;j++) 
	{
		if(i<mask_xRadius || i>(height-mask_xRadius-1) || j<mask_yRadius || j>(width-mask_yRadius-1))
		{
			dest.SetPixel(i, j, 0);// set to black
		}
		else
		{
			for (int h=0; h<ANN_set.size_acces; h++)
			{
				In[h] = data[(i+ANN_set.yAcces[h])*width+(j+ANN_set.xAcces[h])]*1.0/(pict_depth);
			}
			ANN_singlePointEstim(pIn,ANN_set,pOut);
			dest.SetPixel(i, j, *pOut*pict_depth);
		}
	}
	dest.isBinary = TRUE;
	outtab[0]->AssignValue((void*)&dest,err);
}

void C_Command_Neural::ANN_mask_acces(ANN_params *ANN_set)
{
	ANN_set->size_acces = 0;
	int xCenterPos = (int)ceil(1.0*ANN_set->maskSize.cols/2)-1;
	int yCenterPos = (int)ceil(1.0*ANN_set->maskSize.rows/2)-1;

	for (int i = 0; i < ANN_set->maskSize.rows; i++)
        for (int j = 0; j < ANN_set->maskSize.cols; j++) 
		  {
			if (*(ANN_set->mask+ i * ANN_set->maskSize.cols +j) == 1)
			{
				ANN_set->xAcces[ANN_set->size_acces] = j - xCenterPos;
				ANN_set->yAcces[ANN_set->size_acces] = i - yCenterPos;
				ANN_set->size_acces++;
			}
        }
}
void C_Command_Neural::ANN_singlePointEstim(double *pIn,ANN_params ANN_set, double *pOut )
{
	double Z1[600], *pZ1 = Z1;
	double L1_p[600], *pL1_p = L1_p;
	double L1_out[600], *pL1_out = L1_out;
	double Z2[600], *pZ2 = Z2;
	double L2_p[600], *pL2_p = L2_p;
	double L2_out[600], *pL2_out = L2_out;
	double Z3[600], *pZ3 = Z3;
	double L3_p[600], *pL3_p = L3_p;
	double L3_out[600], *pL3_out = L3_out;

	//////////////////////////////////////////////////////////////////////////////////////////
	// PROCESSING
	//////////////////////////////////////////////////////////////////////////////////////////

	// Processing by layer 1
	mult_Matrix(ANN_set.W1,  pIn,  ANN_set.sizeW1, ANN_set.sizeX, pZ1);
	sum_Matrix( pZ1,  ANN_set.B1, ANN_set.sizeZ1, pL1_p);
	exec_TrFun(pL1_p, ANN_set.sizeL1_p, ANN_set.type_TRF[0], pL1_out);

	// Processing by layer 2
	mult_Matrix(ANN_set.W2,  pL1_out,  ANN_set.sizeW2, ANN_set.sizeL2_p, pZ2);
	sum_Matrix( pZ2,  ANN_set.B2, ANN_set.sizeZ2, pL2_p);
	exec_TrFun(pL2_p, ANN_set.sizeL2_p, ANN_set.type_TRF[1], pL2_out);

	// Processing by layer 3
	mult_Matrix(ANN_set.W3,  pL2_out,  ANN_set.sizeW3, ANN_set.sizeL3_p, pZ3);
	sum_Matrix( pZ3,  ANN_set.B3, ANN_set.sizeZ3, pL3_p);
	exec_TrFun(pL3_p, ANN_set.sizeL3_p, ANN_set.type_TRF[2], pL3_out);

	*pOut = *pL3_out;
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// END OF PROCESSING
	//////////////////////////////////////////////////////////////////////////////////////////
};

inline double C_Command_Neural::neuron_TRF(double x, int type_TRF)
{
	double out;
	switch(type_TRF)
	{
		case 1:  out= 1.0/(1 + exp(-x)); break;	//logsig_TRF
		case 3:  out= x; break;					//linear_TRF
	};
	return out;
};

void C_Command_Neural::mult_Matrix(double *X, double *Y, W_B_size size_X, W_B_size size_Y, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_Y.cols; j++) 
		{
            *(Z+i*size_Y.cols+j) = 0;
            for (int k = 0; k < size_X.cols; k++)
                *(Z+i*size_Y.cols+j) += *(X+i*size_X.cols+k) * *(Y+k*size_Y.cols+j);
        }
}

void C_Command_Neural::sum_Matrix(double *X, double *Y, W_B_size size_X, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_X.cols; j++) 
		{
            *(Z+i*size_X.cols+j) = *(X+i*size_X.cols+j) + *(Y+i*size_X.cols+j);
		}
}

void C_Command_Neural::exec_TrFun(double *pL1_p, W_B_size sizeL1_p, int type_TRF, double *pL1_out)
{
	
	for (int i = 0; i < sizeL1_p.rows; i++)
        for (int j = 0; j < sizeL1_p.cols; j++) 
		{
            *(pL1_out+i*sizeL1_p.cols+j) = neuron_TRF( *(pL1_p+i*sizeL1_p.cols+j), type_TRF);
		}
	
}


BOOL C_Command_Neural::ANN_loading(char ANN_filename[], ANN_params *ANN_set)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// open file and get the weights and biases
	//////////////////////////////////////////////////////////////////////////////////////////
	FILE *infile;
	char buffer[30];
	if ((fopen_s(&infile,ANN_filename,"r")) != NULL)	
		return TRUE;
			//net
	fgets(buffer,30,infile); 
	int index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->net_Sch[index_loop++] = atoi(buffer);}
	}
		// end of net


		//transfer function
		// 1-logsig,   3-linear
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile); 	
		if (buffer[0] == 'E') break;
		else 
		{
			if (strstr(buffer,"logsig")) ANN_set->type_TRF[index_loop++] = 1;
			if (strstr(buffer,"linear")) ANN_set->type_TRF[index_loop++] = 3;
		}
	}
		// end of transfer function


		///////////// LAYER 1 //////////////

		//W1
	fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->W1[index_loop++] = atof(buffer);}
	}
	// end of W1
		//B1
	fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->B1[index_loop++] = atof(buffer);}
	}
	// end of B1
		//sizeW1
	fgets(buffer,30,infile);
	index_loop = 0; 
	int temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->sizeW1.rows = temp[0]; ANN_set->sizeW1.cols = temp[1];
	}
		// end of sizeW1

		//sizeB1
	fgets(buffer,30,infile);
	index_loop = 0; 
	temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->sizeB1.rows = temp[0]; ANN_set->sizeB1.cols = temp[1];
	}
	// end of sizeB1

	///////////// LAYER 2 //////////////
		//W2
	fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->W2[index_loop++] = atof(buffer);}
	}	
	// end of W2
		//B2
		fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->B2[index_loop++] = atof(buffer);}
	}
	// end of B2
		//sizeW2
	fgets(buffer,30,infile);
	index_loop = 0; 
	temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->sizeW2.rows = temp[0]; ANN_set->sizeW2.cols = temp[1];
	}
	// end of sizeW2
		//sizeB2
	fgets(buffer,30,infile);
	index_loop = 0; 
	temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->sizeB2.rows = temp[0]; ANN_set->sizeB2.cols = temp[1];
	}
	// end of sizeB2

	///////////// LAYER 3 //////////////
		//W3
	fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->W3[index_loop++] = atof(buffer);}
	}
	// end of W3
		//B3
	fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->B3[index_loop++] = atof(buffer);}
	}
	// end of B3
		//sizeW3
	fgets(buffer,30,infile);
	index_loop = 0; 
	temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->sizeW3.rows = temp[0]; ANN_set->sizeW3.cols = temp[1];
	}
	// end of sizeW3
		//sizeB3
	fgets(buffer,30,infile);
	index_loop = 0; 
	temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->sizeB3.rows = temp[0]; ANN_set->sizeB3.cols = temp[1];
	}
	// end of sizeB3


	///////////// INPUT //////////////

	//sizeX
	fgets(buffer,30,infile);
	index_loop = 0; 
	temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->sizeX.rows = temp[0]; ANN_set->sizeX.cols = temp[1];
	}
	// end of sizeX

	//mask
	fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->mask[index_loop++] = atof(buffer);}
	}
	// end of mask

	//maskOnes
	fgets(buffer,30,infile);
	index_loop = 0; 
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {ANN_set->maskOnes[index_loop++] = atoi(buffer);}
	}
	// end of maskOnes

	//maskSize
	fgets(buffer,30,infile);
	index_loop = 0; 
	temp[2];
	while (1)
	{
		fgets(buffer,30,infile);	
		if (buffer[0] == 'E') break;
			else {temp[index_loop++] = atoi(buffer);}
		ANN_set->maskSize.rows = temp[0]; ANN_set->maskSize.cols = temp[1];
	}
	// end of maskSize

	fclose(infile);

	//////////////////////////////////////////////////////////////////////////////////////////
	// end of data loading
	//////////////////////////////////////////////////////////////////////////////////////////


	ANN_set->sizeZ1.rows = ANN_set->sizeW1.rows;				ANN_set->sizeZ1.cols = ANN_set->sizeX.cols; 
	ANN_set->sizeL1_p    = ANN_set->sizeB1;
	ANN_set->sizeZ2.rows = ANN_set->sizeW2.rows;				ANN_set->sizeZ2.cols = ANN_set->sizeL1_p.cols;
	ANN_set->sizeL2_p    = ANN_set->sizeB2;
	ANN_set->sizeZ3.rows = ANN_set->sizeW3.rows;				ANN_set->sizeZ3.cols = ANN_set->sizeL2_p.cols;
	ANN_set->sizeL3_p    = ANN_set->sizeB3;
	return FALSE;
}