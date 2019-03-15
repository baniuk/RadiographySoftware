// Load_Disp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
//#include <fstream.h>
#include "math.h"
#include "conio.h"
#include "ANN_classifier.h"

void ANN_classifier(double* inpVector,  double* outVector, char ANN_filename[]);

int _tmain(int argc, _TCHAR* argv[])
{
	
	
// -------------------------------------------------------------------------------------------------		
// W TYM MIEJSCU WSTAWIAMY WŁASNY KOD

// deklaracje przykładowego wektora wejściowego
	double inpVector[] = {-0.5, -4};
	double *pinpVector;
	double outVector[] = {100,200,300,400};
	double *poutVector;
	pinpVector = &inpVector[0];
	poutVector = &outVector[0];
	printf("\nInput Vector:\nx1:%f\nx2:%f\n\n",inpVector[0],inpVector[1]);
	
// wczytanie danych sieci
	char filename[] = "ffnet_ex4_NN_classifier_10_4.ann";

// przetwarzanie
	ANN_classifier(pinpVector, poutVector ,filename);

// wyświetlanie wyników
	printf("\nOutput Vector:\nClass1: %f\nClass2: %f\nClass3: %f\nClass4: %f\n",outVector[0], outVector[1] , outVector[2], outVector[3]);
	_getch();
	return 0;
}


//Przemek ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


void ANN_classifier(double* inpVector,  double* outVector, char ANN_filename[])
{
		ANN_params *pANN_set;
		pANN_set = &ANN_set;
		ANN_classifier_loading(ANN_filename, pANN_set);
		printf("\nANN classifier loaded\n\nProcessing... \n\n");
		ANN_singleSimulation(inpVector,ANN_set,outVector);		
}

void ANN_singleSimulation(double *pIn,ANN_params ANN_set, double *pOut )
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

double neuron_TRF(double x, int type_TRF)
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

void mult_Matrix(double *X, double *Y, W_B_size size_X, W_B_size size_Y, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_Y.cols; j++) 
		{
            *(Z+i*size_Y.cols+j) = 0;
            for (int k = 0; k < size_X.cols; k++)
                *(Z+i*size_Y.cols+j) += *(X+i*size_X.cols+k) * *(Y+k*size_Y.cols+j);
        }
}

void sum_Matrix(double *X, double *Y, W_B_size size_X, double *Z)
{
	for (int i = 0; i < size_X.rows; i++)
        for (int j = 0; j < size_X.cols; j++) 
		{
            *(Z+i*size_X.cols+j) = *(X+i*size_X.cols+j) + *(Y+i*size_X.cols+j);
		}
}


void exec_TrFun(double *pL1_p, W_B_size sizeL1_p, int type_TRF, double *pL1_out)
{
	
	for (int i = 0; i < sizeL1_p.rows; i++)
        for (int j = 0; j < sizeL1_p.cols; j++) 
		{
            *(pL1_out+i*sizeL1_p.cols+j) = neuron_TRF( *(pL1_p+i*sizeL1_p.cols+j), type_TRF);
		}
	
}


void ANN_classifier_loading(char ANN_filename[], ANN_params *ANN_set)
{
	//////////////////////////////////////////////////////////////////////////////////////////
	// open file and get the weights and biases
	//////////////////////////////////////////////////////////////////////////////////////////
	FILE *infile;
	char buffer[30];
	if ((fopen_s(&infile,ANN_filename,"r")) != NULL)	
	{
		printf("\n No file or file corrupted\n");
		_getch();
		exit(0);
	}
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