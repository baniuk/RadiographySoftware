#include "Cpp_Lib.h"
#include <iostream>

// This is an empty project to build DLL library of interface version 2.0


void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"importbinarymatrix");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"MATRIX = importbinarymatrix(STRING)");
	definition->numofout = 1;
	definition->numofin = 1;
	definition->typesofin[0] = STRING;
	definition->typesofout[0] = MATRIX;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"developed by PB");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	FILE *stream;
	C_Matrix_Container out;
	char* filename;
	filename = (char*)intab[0]->data;
	unsigned long rows,cols;

	if( fopen_s( &stream, filename, "rb" ) == 0 )
	{
		fread(&rows, 4, 1,stream);	
		fread(&cols, 4, 1,stream);
		out.AllocateData(rows,cols);
		fread(out.data,8,rows*cols,stream);
		fclose( stream );
		outtab[0]->AssignValue((void*)&out,err);
	}
	else	{
		err.SetError("importbinarymatrix: Cant open the file");
		return;
	}


}

/*
function savebinarymatrix(data,filename)
% saves matrix i binary format accetable by readbinarymatrix command

fid = fopen(filename, 'wt');
ss = size(data);
fprintf(fid,'%ld\n',ss(1));
fprintf(fid,'%ld\n',ss(2));
for r=1:ss(1)
    for k = 1:ss(2)
        fprintf(fid,'%+030.20f\n',data(r,k));
    end
end
        


fclose(fid);
*/