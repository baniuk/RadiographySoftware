#include "StdAfx.h"
#include "C_Command_Matrixread.h"

C_Command_Matrixread::C_Command_Matrixread(void)
{
	UINT parint[1] = {STRING};
	UINT parout[1] = {MATRIX};
	InitCommand("matrixread",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "MATRIX = matrixread(STRING)";
	fun_ver = "2.0";
}

C_Command_Matrixread::~C_Command_Matrixread(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Matrixread::Do(C_Error& err)
{
#define BUFFOR_SIZE 50	
	FILE *stream;
	C_Matrix_Container out;
	char* filename;
	char bufor[BUFFOR_SIZE];
	BOOL res;
	_CRT_DOUBLE liczba;
	int status, ch, licznik=0, actrow, actcol;
	unsigned int rows, cols;
	filename = (char*)intab[0]->data;
	actrow = actcol = 0;
//	fpos_t pos;
	if( fopen_s( &stream, filename, "r" ) == 0 )
	{
		res = CheckSize(stream, rows, cols);
//		fscanf_s(stream,"%ld",&rows);
//		fscanf_s(stream,"%ld",&cols);
//		fgetpos(stream,&pos); pos+=2;
//		fsetpos(stream,&pos);
		ch=fgetc(stream);
//		if(ch!=91)	{
//			err.SetError("matrixread: No [");
//			return;
//		}
		if(res==FALSE)	{
			err.SetError("matrixread: Concentration error - check [],;");
			return;
		}
		out.AllocateData(rows,cols);
		while(feof(stream)==0)	{
			ch = fgetc(stream);
			while(ch==32)
				ch = fgetc(stream);
			if(ch==44 || ch==59 || ch==93)	{
				// srednik lub przecinek - dekodowac liczbe
				bufor[licznik] = '\0';
				status = _atodbl(&liczba,bufor);
				if(status==1)	{
					err.SetError("matrixread: Unknown char");
					return;
				}
				out.SetPixel(actrow,actcol,liczba.x);
				if(ch==44)
					actcol++;
				else
					if(ch==59)	{
						actcol = 0;
						actrow++;
					}
				licznik = 0;				
			}
			else	{
				// jest liczba - do bufora
				bufor[licznik++] = (char)ch;		
				ASSERT(licznik<BUFFOR_SIZE);

			}
		}		
		fclose( stream );
/*		if(cols!=actcol+1 || rows!=actrow+1)	{
			err.SetError("matrixread: Concetration error");
			return;
		}*/
	}
	else	{
		err.SetError("matrixread: Cant open the file");
		return;
	}
	outtab[0]->AssignValue((void*)&out,err);

}

BOOL C_Command_Matrixread::CheckSize(FILE* stream, unsigned int &row, unsigned int &col)
{
	int ch;
	int nawiasl=0;
	int nawiasp=0;
	int sredniki=0;
	int przecinki=0;
	int licznik=0;
	int last; // ostatni wczytany znak
	while(feof(stream)==0)	{
		ch=fgetc(stream); licznik++;
		if(licznik==1 && ch!=91) return FALSE;
		if(ch==91){ nawiasl++; last=91;}
		if(ch==93){ nawiasp++; last=93;}
		if(ch==59){ sredniki++;last=59;}
		if(ch==44){ przecinki++;last=44;}
	}
	if(last==59)
		sredniki--;

	if( (nawiasl*nawiasp)!=1 )
		return FALSE;
	if(sredniki>0)
		if( (przecinki % (sredniki+1)) != 0) 
			return FALSE;
	rewind(stream);
	col = (przecinki/(sredniki+1))+1;
	row = sredniki+1;
	return TRUE;
}