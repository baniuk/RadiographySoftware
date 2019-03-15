#pragma once
#include "DataTypes.h"

class C_Error
{
public:
	C_Error(void);
	C_Error(char* perror);
//	char* error;
	char error[maxerror];
	char e_line[maxline];
	char user[maxerror];
	void SetError(char* perror);
	void SetError(char *perror,unsigned int lin);
	void SetError(char *perror,unsigned int lin, char* line);
	void SetUserData(char* pdata);
	BOOL status;		// FALSE - bez errorów
	BOOL data;			// FALSE - bez dodatkowych danych - okreœla czy jest linia w e_line
	BOOL userdata;		// FALSE - bez danych
	unsigned int numline;
public:
	~C_Error(void);
};
