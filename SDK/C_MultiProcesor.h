#pragma once


class C_MultiProcesor
{
public:
	BOOL SetNumProc(int num);
	BOOL AutoSetNumProc(void);
	int GetNumProc(void);
	C_MultiProcesor(void);
	~C_MultiProcesor(void);
private:
	int numofprcesors;
};
