//#ifndef _EXTERNALDLL
#include "StdAfx.h"
#include "C_Error.h"
//#endif
C_Error::C_Error(void)
{
	status = FALSE;
	data = FALSE;
	userdata = FALSE;
	numline = 9999;
}
C_Error::C_Error(char* perror)
{
//	error = perror;
	strcpy_s(error,maxerror,perror);
	status = TRUE;
	data = FALSE;
	userdata = FALSE;
	numline = 9999;
}

C_Error::~C_Error(void)
{
}
void C_Error::SetError(char *perror)
{
//	error = perror;
	strcpy_s(error,maxerror,perror);
	status = TRUE;
	data = FALSE;
}

void C_Error::SetUserData(char *perror)
{
	strcpy_s(user,maxerror,perror);
	userdata = TRUE;
}

void C_Error::SetError(char *perror,unsigned int lin)
{
//	error = perror;
	strcpy_s(error,maxerror,perror);
	status = TRUE;
	data = FALSE;
	numline = lin;

}

void C_Error::SetError(char *perror,unsigned int lin, char* line)
{
//	error = perror;
	strcpy_s(error,maxerror,perror);
	strcpy_s(e_line,maxline,line);
	status = TRUE;
	data = TRUE;
	numline = lin;

}