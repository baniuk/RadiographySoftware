
#include "stdafx.h"
#include <stdio.h>

char buffer[255];
int i;

int _tmain(int argc, _TCHAR* argv[])
{
	FILE *pFile;
	
	if ((fopen_s(&pFile,"D:\\plik.txt","r") != NULL))
	{
		printf("\n No file or file corrupted\n");
	}
	
	while ( i != EOF)
	{
		fgets(buffer,255,pFile);	
		printf(buffer);
		getchar();
		i=fgetc(pFile);
	}
	
	getchar();
	return 0;
}