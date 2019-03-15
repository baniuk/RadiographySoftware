// MP_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

unsigned __stdcall func(void *data);
EX ex1;	// jesli jest w main to w funkcji nie ma prawid³owo zainicjalizowanych zmiennych??
EX ex2;	
HANDLE hThread1[2];
//double in[10] = {1,2,3,4,5,6,7,8,9,10};
//double out[10];

int _tmain(int argc, _TCHAR* argv[])
{
//	EX ex1;
	double in[10] = {1,2,3,4,5,6,7,8,9,10};
double out[10];
	ex1.in = in;
	ex1.out = out;

 	ex2.in = in+3;
	ex2.out = out+5;


	hThread1[0] = (HANDLE)_beginthreadex(NULL,0, func, (void*)&ex1,0,NULL );
	hThread1[1] = (HANDLE)_beginthreadex(NULL,0, func, (void*)&ex2,0,NULL );
//	WaitForSingleObject( hThread1[0], INFINITE );

//	hThread1[0] = (HANDLE)_beginthreadex(NULL,0,Median_1,(void*)&ex1,0,&threadID1);
//	hThread1[1] = (HANDLE)_beginthreadex(NULL,0,Median_2,(void*)&ex2,0,&threadID2);
	WaitForMultipleObjects(2,hThread1,TRUE,INFINITE);

	return 0;
}

unsigned __stdcall func(void *data)
{
	EX *ex1;
	ex1 = (EX*)data;
	double d;
	double *f = new double[5];
	for(int a=0;a<5;a++)
	{
		d = *(ex1->in+a);
		f[a] = d;
		*(ex1->out+a) = f[a];
	}
	delete[] f;
	_endthreadex(0);
	return 0;
//	ReleaseMutex( hRunMutex );

}