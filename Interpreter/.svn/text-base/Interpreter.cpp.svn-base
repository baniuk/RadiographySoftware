// Interpreter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	UINT parint[2] = {STRING};
	UINT parout[1] = {IMAGE};
	C_InterpBase test;
	C_Error err;
	int numer;
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}
	
	test.AddLine("a = imread('c:\\test.tif');",err);	numer = 0;
	test.CheckLine(numer,err);
	test.BuildLine(numer,err);
//	test.AddLine("b = th(a,55,0);",err);	numer = 1;
//	test.CheckLine(numer,err);
//	test.BuildLine(numer,err);
//	test.AddLine("c = glshow(b,'tesst',800,600,'fit');",err);	numer = 2;
//	test.CheckLine(numer,err);
//	test.BuildLine(numer,err);
	test.AddLine("d = glshow(a,'tesst',800,600,'fit');",err);	numer = 1;
	test.CheckLine(numer,err);
	test.BuildLine(numer,err);
	test.Start(err);
//	test.PrintVariable("a",err);
//	test.PrintVariable("b",err);
	_getch();
	return nRetCode;
}
