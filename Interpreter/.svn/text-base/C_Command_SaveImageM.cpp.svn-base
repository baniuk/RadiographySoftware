#include "StdAfx.h"
#include "C_Command_SaveImageM.h"

C_Command_SaveImageM::C_Command_SaveImageM(void)
{
	UINT parint[2] = {STRING,IMAGE};
	UINT parout[1] = {};
	InitCommand("saveimagem",2,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "saveimagem(STRING,IMAGE)";
	fun_ver = "1.0";
}

C_Command_SaveImageM::~C_Command_SaveImageM(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_SaveImageM::Do(C_Error& err)
{
	FILE *stream;
	C_Image_Container *tmpimg;
	char* tmp;
	tmp = (char*)intab[0]->data;
	tmpimg = (C_Image_Container*)intab[1]->data;
	unsigned int pcols[1];
	unsigned int prows[1];
	pcols[0] = tmpimg->_cols;
	prows[0] = tmpimg->_rows;
	if( fopen_s( &stream, tmp, "w+b" ) == 0 )
	{
		fwrite(pcols, sizeof( unsigned int ),1,stream);
		fwrite(prows, sizeof( unsigned int ),1,stream);
		fwrite(tmpimg->data, sizeof( double ), tmpimg->GetNumofElements(), stream);
		fclose( stream );
	}
	else	{
		err.SetError("saveimagem: Cant save the file");
		return;
	}
}