#pragma once

class C_Matrix_Container
{
public:
	C_Matrix_Container(void);
	unsigned int _rows, _cols;
	double* data;
	void AllocateData(unsigned int rows, unsigned int cols);
	void Zeros(void);
	void FreeData(void);
	void CloneObject(C_Matrix_Container* dest);
	void GetPixel(unsigned long row, unsigned long col,double&pixel);
	void SetPixel(unsigned long row, unsigned long col,double pixel);
	BOOL Add(C_Matrix_Container* matrix);
	BOOL Sub(C_Matrix_Container* matrix);
	BOOL DotMulti(C_Matrix_Container* matrix);
	void getMinMax(double& min, double& max);
	unsigned long GetNumofElements(void);
	virtual ~C_Matrix_Container(void);
};
