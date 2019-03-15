#include "StdAfx.h"
#include "C_Command_Annclasifier.h"

C_Command_Annclasifier::C_Command_Annclasifier(void)
{
	UINT parint[2] = {MATRIX,STRING};
	UINT parout[1] = {MATRIX};
	InitCommand("ANNclassifier",2,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "MATRIX = ANNclassifier(MATRIX,STRING)";
	fun_ver = "1.2";
}

C_Command_Annclasifier::~C_Command_Annclasifier(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Annclasifier::Do(C_Error& err)
{
	C_Matrix_Container *p_inpVector;
	C_Matrix_Container p_outVector;
	char* ANN_filename;
	double *inpVector, *outVector;
	unsigned int a,b;
	double data;
	
	ANN_filename = (char*)intab[1]->data;
	p_inpVector = (C_Matrix_Container*)intab[0]->data;
	inpVector = new double[p_inpVector->_rows];
	

	ANN_params *pANN_set;
	pANN_set = &ANN_set;

	ANN_classifier_loading(ANN_filename, pANN_set,err);
	outVector = new double[pANN_set->num_rows];
	p_outVector.AllocateData(pANN_set->num_rows,p_inpVector->_cols);

	for(a=0;a<p_inpVector->_cols;a++)	{
		
		for(b=0;b<p_inpVector->_rows;b++)	{
			p_inpVector->GetPixel(b,a,data);
			inpVector[b] = data;
		}
		
		ANN_singleSimulation(inpVector,ANN_set,outVector);
		for(b=0;b<pANN_set->num_rows;b++)	{
			data = outVector[b];
			p_outVector.SetPixel(b,a,data);				
		}

	}
	outtab[0]->AssignValue((void*)&p_outVector,err);
}

void C_Command_Annclasifier::ANN_singleSimulation(double *pIn,ANN_params ANN_set, double *pOut )
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
	if (int(ANN_set.net_Sch[1]) == 0)
	{
		for (int i = 0; i < ANN_set.net_Sch[0]; i++) { *(pOut+i) = *(pL1_out+i); }
		return;
		
	}

	// Processing by layer 2
	mult_Matrix(ANN_set.W2,  pL1_out,  ANN_set.sizeW2, ANN_set.sizeL2_p, pZ2);
	sum_Matrix( pZ2,  ANN_set.B2, ANN_set.sizeZ2, pL2_p);
	exec_TrFun(pL2_p, ANN_set.sizeL2_p, ANN_set.type_TRF[1], pL2_out);
	if (int(ANN_set.net_Sch[2]) == 0)
	{
		for (int i = 0; i < ANN_set.net_Sch[1]; i++) { *(pOut+i) = *(pL2_out+i); }
		return;
	}

	// Processing by layer 3
	mult_Matrix(ANN_set.W3,  pL2_out,  ANN_set.sizeW3, ANN_set.sizeL3_p, pZ3);
	sum_Matrix( pZ3,  ANN_set.B3, ANN_set.sizeZ3, pL3_p);
	exec_TrFun(pL3_p, ANN_set.sizeL3_p, ANN_set.type_TRF[2], pL3_out);
	for (int i = 0; i < ANN_set.net_Sch[2]; i++) { *(pOut+i) = *(pL3_out+i); }
 

	//////////////////////////////////////////////////////////////////////////////////////////
	// END OF PROCESSING
	//////////////////////////////////////////////////////////////////////////////////////////
};

double C_Command_Annclasifier::neuron_TRF(double x, int type_TRF)
{
	double out;
	switch(type_TRF)
	{
		case 1:  out= 1.0/(1 + exp(-x)); break;				//logsig_TRF
		case 2:  out= 2.0/(1 + exp(-2*x))-1; break;			//tansig_TRF  
		case 3:  out= x; break;								//purelin_TRF
		case 4:  if (x>=0) out = 1; else out = 0; break;	//hardlim_TRF
		case 5:  if (x>=0) out = 1; else out =-1; break;	//hardlims_TRF
	};
	return out;
};

void C_Command_Annclasifier::mult_Matrix(double *X, double *Y, W_B_size size_X, W_B_size size_Y, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_Y.cols; j++) 
		{
            *(Z+i*size_Y.cols+j) = 0;
            for (int k = 0; k < size_X.cols; k++)
                *(Z+i*size_Y.cols+j) += *(X+i*size_X.cols+k) * *(Y+k*size_Y.cols+j);
        }
}

void C_Command_Annclasifier::sum_Matrix(double *X, double *Y, W_B_size size_X, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_X.cols; j++) 
		{
            *(Z+i*size_X.cols+j) = *(X+i*size_X.cols+j) + *(Y+i*size_X.cols+j);
		}
}


void C_Command_Annclasifier::exec_TrFun(double *pL1_p, W_B_size sizeL1_p, int type_TRF, double *pL1_out)
{
	
	for (int i = 0; i < sizeL1_p.rows; i++)
        for (int j = 0; j < sizeL1_p.cols; j++) 
		{
            *(pL1_out+i*sizeL1_p.cols+j) = neuron_TRF( *(pL1_p+i*sizeL1_p.cols+j), type_TRF);
		}
	
}


void C_Command_Annclasifier::ANN_classifier_loading(char ANN_filename[], ANN_params *ANN_set, C_Error& err)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// open file and get the weights and biases
	//////////////////////////////////////////////////////////////////////////////////////////
	FILE *infile;
	char buffer[30];
	if ((fopen_s(&infile,ANN_filename,"r")) != NULL)	
	{
		err.SetError("Annclasifier: Can't read *.ann file");
		return;
	}
		//net
		fgets(buffer,30,infile); 
		int index_loop = 0; 
		for(int a=0;a<3;a++) ANN_set->net_Sch[a] = 0;
		while (1)
		{
			fgets(buffer,30,infile);	
			if (buffer[0] == 'E') break;
				else {ANN_set->net_Sch[index_loop++] = atoi(buffer);}
		}
		ANN_set->num_rows = ANN_set->net_Sch[index_loop-1];
		// end of net


		//transfer function
		// 1-logsig, 2-tansig, 3-purelin, 4-hardlim, 5-hardlims
		index_loop = 0; 
		while (1)
		{
			fgets(buffer,30,infile); 	
			if (buffer[0] == 'E') break;
			else 
			{
				if (strstr(buffer,"logsig"))	ANN_set->type_TRF[index_loop++] = 1;
				if (strstr(buffer,"tansig"))	ANN_set->type_TRF[index_loop++] = 2;
				if (strstr(buffer,"purelin"))	ANN_set->type_TRF[index_loop++] = 3;
				if (strstr(buffer,"hardlim"))	ANN_set->type_TRF[index_loop++] = 4;
				if (strstr(buffer,"hardlims"))	ANN_set->type_TRF[index_loop++] = 5;
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
		if (int (ANN_set->net_Sch[1]) != 0)
		{
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
		}

		///////////// LAYER 3 //////////////
		if (int (ANN_set->net_Sch[2]) != 0)
		{
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

		}
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
}