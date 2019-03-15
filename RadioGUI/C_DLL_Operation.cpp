#include "StdAfx.h"
#include "C_DLL_Operation.h"
#include <strsafe.h>
C_DLL_Operation::C_DLL_Operation(void)
{
	dllnameholder = NULL;
	lastdllindex = 0;
}

C_DLL_Operation::~C_DLL_Operation(void)
{
//	SAFE_DELETE(dllnameholder);
	if(dllnameholder!=NULL)
		delete[] dllnameholder;
}

void C_DLL_Operation::AllocateDLLSpace(int numofdll)
{
	dllnameholder = new C_DLL_Name_Holder[numofdll];

}
void C_DLL_Operation::AddDLL(char* name,char* programpatch)
{
	if(CheckDLL(name,programpatch))	{
		strcpy_s(dllnameholder[lastdllindex].DLLName,256+3,name);
		strcpy_s(dllnameholder[lastdllindex].FunctionName,MAX_NAME,data.name);
		lastdllindex++;
	}
}

BOOL C_DLL_Operation::CheckDLL(char* name,char* programpatch)
{
	LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
	BOOL ret;
	Doo1 = NULL;
	Doo2 = NULL;
	char tmpdir[1024];
	_getcwd(tmpdir,1024);
	ret = _chdir(programpatch);
	ret = _chdir("dll");
	if(ret!=0)	{
		_chdir(tmpdir);
		return FALSE;
	}
	mylib = LoadLibrary(name);
	DWORD dw = GetLastError(); 
#ifdef DEBUG	
	if(dw)
	{
		char lpszFunction[]="LoadLibrary";
		FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
		StringCchPrintf((LPTSTR)lpDisplayBuf,LocalSize(lpDisplayBuf) / sizeof(TCHAR),TEXT("%s failed with error %d: %s"),lpszFunction, dw, lpMsgBuf); 
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}
#endif
	_chdir(tmpdir);
	if(mylib==NULL)	return FALSE;
	Introduce = (DLL_INTR)GetProcAddress(mylib,"Introduce");
	if(!Introduce)	{FreeLibrary(mylib); return FALSE;}
	Introduce(&data);			// do odczytania nazwy funkcji
	if(data.interface_version==1)
		Doo1 = (DLL_DO_INT1)GetProcAddress(mylib,"Do");
	if(data.interface_version==2)
		Doo2 = (DLL_DO_INT2)GetProcAddress(mylib,"Do");
	if(!Doo1 && !Doo2)	{FreeLibrary(mylib); return FALSE;}
	SetOutput = (DLL_SET_OUTPUT)GetProcAddress(mylib,"SetOutput");
	if(!SetOutput)	{FreeLibrary(mylib); return FALSE;}

	ret = FreeLibrary(mylib);
	return TRUE;

}

char* C_DLL_Operation::CheckName(char* name)
{
	int a;
	for(a=0;a<lastdllindex;a++)	{
		if(strcmp(name,dllnameholder[a].FunctionName)==0)
			return dllnameholder[a].DLLName;
	}
	return NULL;
}