#pragma once
#include "c_commands.h"

class C_Command_Neural :
	public C_Commands
{
public:
	C_Command_Neural(void);
	virtual void Do(C_Error& err);
	virtual ~C_Command_Neural(void);
private:
	struct W_B_size {
		int rows;
		int cols;
	}	sizeW1, sizeB1, sizeZ1, sizeL1_p, sizeL1_out,   sizeW2, sizeB2, sizeZ2, sizeL2_p, sizeL2_out,  sizeW3, sizeB3, sizeZ3, sizeL3_p, sizeL3_out,	sizeX;

	struct ANN_params {
		int net_Sch[3];
		int type_TRF[3];
		double W1[4000];
		double B1[4000];
		W_B_size sizeW1;
		W_B_size sizeB1;
		double W2[4000];
		double B2[4000];
		W_B_size sizeW2;
		W_B_size sizeB2;
		double W3[4000];
		double B3[4000];
		W_B_size sizeW3;
		W_B_size sizeB3;
		W_B_size sizeX;
		W_B_size sizeZ1;
		W_B_size sizeL1_p;
		W_B_size sizeZ2;
		W_B_size sizeL2_p;
		W_B_size sizeZ3;
		W_B_size sizeL3_p;
		double mask[4000];
		int maskOnes[4000];
		W_B_size maskSize;
		int size_acces;
		int xAcces[4000];
		int yAcces[4000];
	} ANN_set;

	double neuron_TRF(double x, int type_TRF);
	void mult_Matrix( double *X, double *Y, W_B_size size_X, W_B_size size_Y, double *Z);
	void sum_Matrix(double *X,  double *Y, W_B_size size_X, double *Z);
	void exec_TrFun( double *pL1_p, W_B_size sizeL1_p, int type_TRF, double *pL1_out);
	BOOL ANN_loading(char ANN_filename[], ANN_params *ANN_set);
	void ANN_singlePointEstim(double *pIn,ANN_params ANN_set, double *pOut );
	void ANN_mask_acces(ANN_params *ANN_set);
};
