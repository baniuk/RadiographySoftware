#pragma once
// Ver 1.3.1 by PB

class C_Matrix_Container
{
public:
	C_Matrix_Container(void);
	unsigned int _rows, _cols;
	double* data;
	void AllocateData(unsigned int rows, unsigned int cols);
	void Zeros(void);
	void Ones(void);
	void FreeData(void);
	void CloneObject(C_Matrix_Container* dest);
	void GetPixel(unsigned int row, unsigned int col,double&pixel);
	void ImportFromMatlab(double* in,unsigned int row, unsigned int col);
	void ExportToMatlab(double* out);
	double GetPixel(unsigned int row, unsigned int col);
	unsigned int coord2lin(unsigned int row, unsigned int col);
	void SetPixel(unsigned int row, unsigned int col,double pixel);
	BOOL Add(C_Matrix_Container* matrix);
	BOOL Sub(C_Matrix_Container* matrix);
	BOOL DotMulti(C_Matrix_Container* matrix);
	void getMinMax(double& min, double& max);
	BOOL Dump(char* filename);
	void Transpose(void);
	unsigned long GetNumofElements(void);
	BOOL DumpBinary(char *filename);
	BOOL ReadBinary(char *filename);
	double Mean(C_Matrix_Container* out,unsigned int col);
	double Median(void);
	void stdcol(C_Matrix_Container* output);
	void std(C_Matrix_Container* output);
	double std(void);
	void CutMatrixCol(C_Matrix_Container* output, C_Matrix_Container* cols);
	void CutMatrixRow(C_Matrix_Container* output, C_Matrix_Container* rows);
	void RemoveMatrixRow(C_Matrix_Container* index);
	void RemoveMatrixCol(C_Matrix_Container* index);
	unsigned long iselement(double element);
	void quickSort( double a[], int l, int r);
	virtual ~C_Matrix_Container(void);
private:
	int partition( double a[], int l, int r);
};
