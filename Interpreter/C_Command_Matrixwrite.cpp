#include "StdAfx.h"
#include "C_Command_Matrixwrite.h"

C_Command_Matrixwrite::C_Command_Matrixwrite(void)
{
	UINT parint[2] = {MATRIX,STRING};
	UINT parout[1] = {};
	InitCommand("matrixwrite",2,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "matrixwrite(MATRIX,STRING)";
	fun_ver = "1.0";
}

C_Command_Matrixwrite::~C_Command_Matrixwrite(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Matrixwrite::Do(C_Error& err)
{
	FILE *stream;
	C_Matrix_Container *in;
	char* filename;
	filename = (char*)intab[1]->data;
	in = (C_Matrix_Container*)intab[0]->data;
	unsigned long actrow, actcol;
	double liczba;

	if( fopen_s( &stream, filename, "w" ) == 0 )
	{
//		fprintf_s(stream,"%ld\n",in->_rows);
//		fprintf_s(stream,"%ld\n",in->_cols);
		fprintf_s(stream,"[");
		if(in->_rows==0 && in->_cols==0)
		{
			fprintf_s(stream,"]");
			fclose( stream );
			return;
		}


		for(actrow=0;actrow<in->_rows;actrow++)
			for(actcol=0;actcol<in->_cols;actcol++)	{
				in->GetPixel(actrow,actcol,liczba);
				fprintf_s(stream,"%f",liczba);
				if(actcol==in->_cols-1)
					fprintf_s(stream,";\n");
				else
					fprintf_s(stream,",");
			}
		fpos_t position;
		fgetpos(stream,&position);
		position-=3;
		fsetpos(stream,&position);
		fprintf_s(stream,"]");
		fclose( stream );
	}
	else	{
		err.SetError("matrixread: Cant open the file");
		return;
	}

}