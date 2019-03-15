#include "StdAfx.h"
#include "C_Command_Dm.h"


C_Command_Dm::C_Command_Dm(void)
{
	UINT parint[1] = {STRING};
	UINT parout[1] = {MATRIX};
	InitCommand("dm",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "MATRIX = dm(STRING)";
	fun_ver = "1.0";
}

C_Command_Dm::~C_Command_Dm(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Dm::Do(C_Error& err)
{
	C_Matrix_Container out;
	char* matrix;
	unsigned int roz;
	char bufor[31];
	BOOL res;
	unsigned int a=0;
	_CRT_DOUBLE liczba;
	int status, ch, licznik=0, actrow, actcol;
	actrow = actcol = 0;
	unsigned int rows, cols;
	matrix = (char*)intab[0]->data;
	roz = (unsigned int)strlen(matrix);
	res = CheckSize(matrix, rows, cols);
	if(res==FALSE)	{
		err.SetError("matrixread: Concentration error - check [],;");
		return;
	}
	out.AllocateData(rows,cols);
	while(a<roz)	{
		ch = matrix[a];
		while(ch==32)	{
			ch = matrix[a];
			a++;
		}
		if(ch==44 || ch==59 || ch==93)	{
			// srednik lub przecinek - dekodowac liczbe
			bufor[licznik] = '\0';
			status = _atodbl(&liczba,bufor);
			if(status==1)	{
				err.SetError("dm: Unknown char");
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
			if(ch>47 && ch<58)	// jest liczba - do bufora
				bufor[licznik++] = (char)ch;				
		}
		a++;
	}		


/*	actrow = actcol = 0;
	fpos_t pos;
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
			}
		}		
		fclose( stream );
/*		if(cols!=actcol+1 || rows!=actrow+1)	{
			err.SetError("matrixread: Concetration error");
			return;
		}*/
/*	}
	else	{
		err.SetError("matrixread: Cant open the file");
		return;
	}*/
	outtab[0]->AssignValue((void*)&out,err);

}

BOOL C_Command_Dm::CheckSize(char* stream, unsigned int &row, unsigned int &col)
{
	int ch;
	int nawiasl=0;
	int nawiasp=0;
	int sredniki=0;
	int przecinki=0;
	int licznik=0;
	unsigned int a;
	unsigned int roz;
	roz = (unsigned int)strlen(stream);
	for(a=0;a<roz;a++)	{
		ch=stream[a]; licznik++;
		if(licznik==1 && ch!=91) return FALSE;
		if(ch==91) nawiasl++;
		if(ch==93) nawiasp++;
		if(ch==59) sredniki++;
		if(ch==44) przecinki++;
	}

	if( (nawiasl*nawiasp)!=1 )
		return FALSE;
	if(sredniki>0)
		if( (przecinki % (sredniki+1)) != 0) 
			return FALSE;
	col = (przecinki/(sredniki+1))+1;
	row = sredniki+1;
	return TRUE;
}
