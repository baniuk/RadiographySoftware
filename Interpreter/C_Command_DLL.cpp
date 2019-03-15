#include "StdAfx.h"
#include "C_Command_DLL.h"

C_Command_DLL::C_Command_DLL(void)
{


}

C_Command_DLL::C_Command_DLL(char* name, char* programpatch,C_Error &err)
{
	UINT *parint;
	UINT *parout;
	int ret;
	char tmpdir[1024];

	data.fdate[0] = '-';data.fdate[1] = '-';data.fdate[2] = '-';data.fdate[3] = '-';data.fdate[4] = '\0';
	data.ftime[0] = '-';data.ftime[1] = '-';data.ftime[2] = '-';data.ftime[3] = '-';data.ftime[4] = '\0';

	_getcwd(tmpdir,1024);
	ret = _chdir(programpatch);
	ret = _chdir("dll");
	if(ret!=0)	{
		err.SetError("Cant open library");
		_chdir(tmpdir);
		return;
	}
	mylib = LoadLibrary(name);
	_chdir(tmpdir);
	if(mylib==NULL)	{
		err.SetError("Cant open library");
		return;
	}
	Introduce = (DLL_INTR)GetProcAddress(mylib,"Introduce");
	if(!Introduce)	{
		err.SetError("Error");
		return;
	}
	Doo1 = (DLL_DO_INT1)GetProcAddress(mylib,"Do");
	Doo2 = (DLL_DO_INT2)GetProcAddress(mylib,"Do");
	if(!Doo1 && !Doo2)	{
		err.SetError("Error");
		return;
	}
	SetOutput = (DLL_SET_OUTPUT)GetProcAddress(mylib,"SetOutput");
	if(!SetOutput)	{
		err.SetError("Error");
		return;
	}
	AssignGlobals = (DLL_AG)GetProcAddress(mylib,"AssignGlobals");
	if(!AssignGlobals)
		czyglobals = false;
	else
		czyglobals = true;


	Introduce(&data);	

	parint = data.typesofin;
	parout = data.typesofout;
	InitCommand(data.name,data.numofin,data.numofout,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = data.syntax;
	fun_ver = data.version;
	fun_time = data.ftime;
	fun_date = data.fdate;
	description = data.description;
	interface_version = data.interface_version;
	
			
	inputstruct = NULL;
	outputstruct = NULL;

/*	UINT parint[3] = {IMAGE,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("bdilate",3,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = bdilate(IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.1";
	*/
}

C_Command_DLL::~C_Command_DLL(void)
{
	if(mylib!=NULL)
		FreeLibrary(mylib);
	SAFE_DELETE(outputstruct);
	SAFE_DELETE(inputstruct);
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_DLL::Do(C_Error &err)
{
	int a;
	C_Image_Container *image[MAX_NUMBER_FUNCTION_PARAM];		// maksymalna ilosc parametr�w wejsciowych
	C_Matrix_Container *matrix[MAX_NUMBER_FUNCTION_PARAM];
	char *strings[MAX_NUMBER_FUNCTION_PARAM];

	C_Image_Container *image_out[MAX_NUMBER_FUNCTION_PARAM];		// maksymalna ilosc parametr�w wyjsciowychllllllllllllllllllllllllllllllllllllllllllllllllllllllll
	C_Matrix_Container *matrix_out[MAX_NUMBER_FUNCTION_PARAM];
	char *strings_out[MAX_NUMBER_FUNCTION_PARAM];

	ERROR_DEF error;
	error.status = 0;
	if(interface_version==1)
	{
		inputstruct = new VARIABLES[numofinparam];
		outputstruct = new VARIABLES[numofoutparam];
		for(a=0;a<numofinparam;a++)	{
			inputstruct[a].type = intab[a]->type;
			if(inputstruct[a].type==IMAGE)	{
				image[a] = (C_Image_Container*)intab[a]->data;
				inputstruct[a].data = image[a]->data;
				inputstruct[a]._cols = image[a]->_cols;
				inputstruct[a]._rows = image[a]->_rows;
			}
			if(inputstruct[a].type==MATRIX)	{
				matrix[a] = (C_Matrix_Container*)intab[a]->data;
				inputstruct[a].data = matrix[a]->data;
				inputstruct[a]._cols = matrix[a]->_cols;
				inputstruct[a]._rows = matrix[a]->_rows;
			}
			if(inputstruct[a].type==STRING)	{
				strings[a] = (char*)intab[a]->data;
				inputstruct[a].data = (void*)strings[a];
			}
		}

		SetOutput(outputstruct, inputstruct);
		for(a=0;a<numofoutparam;a++)	{
			if(outputstruct[a].type==IMAGE)	{
				image_out[a] = new C_Image_Container;
				image_out[a]->AllocateData(outputstruct[a]._rows, outputstruct[a]._cols);
				outputstruct[a].data = (void*)image_out[a]->data;				
			}
			if(outputstruct[a].type==MATRIX)	{
				matrix_out[a] = new C_Matrix_Container;
				matrix_out[a]->AllocateData(outputstruct[a]._rows, outputstruct[a]._cols);
				outputstruct[a].data = (void*)matrix_out[a]->data;				
			}
			if(outputstruct[a].type==STRING)	{
				strings_out[a] = new char[MAX_STRING];
				outputstruct[a].data = (void*)strings_out[a];				
			}
		}

		Doo1(&error,inputstruct,numofinparam,outputstruct,numofoutparam);

		for(a=0;a<numofoutparam;a++)	{
			if(outputstruct[a].type==IMAGE)	{
				outtab[a]->AssignValue((void*)image_out[a],err);
				SAFE_DELETE(image_out[a]);
			}
		}



		if(error.status)	{
			err.SetError(error.error);
			SAFE_DELETE(inputstruct);
			SAFE_DELETE(outputstruct);
			return;
		}

	}

	if(interface_version==2)
	{
		if(czyglobals)
			AssignGlobals(globals,lastglobal);
		Doo2(err,intab,numofinparam,outtab,numofoutparam);

	}
	
}

/*
C_Image_Container *image1;
	C_Image_Container *image2;
	C_Image_Container o1;
	C_Image_Container o2;
//	C_Image_Container result;
	BOOL ret;
	
	image1 = (C_Image_Container*)intab[0]->data;
	image2 = (C_Image_Container*)intab[1]->data;
	
	image1->CloneObject(&o1);
	image2->CloneObject(&o2);
	
	o1.Normalize(0,65535);
	o2.Normalize(0,65535);

//	image1->CloneObject(&result);
	ret = o1.Add(&o2);
	if(!ret)	{
		err.SetError("Imadd: Images must be the same");
		return;
	}
	o1.Normalize(0,65535);
	
	outtab[0]->AssignValue((void*)&o1,err);
	*/