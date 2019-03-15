#include "StdAfx.h"
#include "C_InterpBase.h"

C_InterpBase::C_InterpBase(void)
{
	int a;
//	InitInterpreter();
	tmpfun = NULL;
	tab = new C_Commands*[maxbuffor];
	for(a=0;a<maxbuffor;a++)
		tab[a] = NULL;
	var = new C_Variables*[maxvar];
	for(a=0;a<maxbuffor;a++)
		var[a] = NULL;
	line = new C_Line*[maxlines];
	for(a=0;a<MAX_GLOBALS;a++)
		globals[a] = NULL;
	lasttab = 0;
	lastvar = 0;
	lastglobal = 0;
	lastline = -1;
	strtmp[1] = 0;
	parentwindow = NULL;
}

C_InterpBase::C_InterpBase(CWnd* parent)
{
	int a;
//	InitInterpreter();
	tmpfun = NULL;
	tab = new C_Commands*[maxbuffor];
	for(a=0;a<maxbuffor;a++)
		tab[a] = NULL;
	var = new C_Variables*[maxvar];
	for(a=0;a<maxvar;a++)
		var[a] = NULL;
	line = new C_Line*[maxlines];
	for(a=0;a<maxlines;a++)
		line[a] = NULL;
	for(a=0;a<maxlines;a++)
		line[a] = NULL;
	for(a=0;a<MAX_GLOBALS;a++)
		globals[a] = NULL;
	lasttab = 0;
	lastvar = 0;
	lastglobal = 0;
	lastline = -1;
	strtmp[1] = 0;
	parentwindow = parent;

}

C_InterpBase::~C_InterpBase(void)
{
	int a;
	ASSERT(lastvar<maxvar);//50
	ASSERT(lasttab<maxbuffor);//512
	ASSERT(lastglobal<MAX_GLOBALS);
	ASSERT(lastline+1<maxlines);//50
	for(a=0;a<lastvar;a++)
		SAFE_DELETE(var[a]);
	for(a=0;a<lasttab;a++)
		SAFE_DELETE(tab[a]);
	for(a=0;a<lastline+1;a++)
		SAFE_DELETE(line[a]);
	for(a=0;a<lastglobal;a++)
		SAFE_DELETE(globals[a]);
	SAFE_DELETE(var);	
	SAFE_DELETE(tab);
	SAFE_DELETE(line);
	SAFE_DELETE(tmpfun);
}

void C_InterpBase::InitInterpreter(void)
{
	
}
BOOL C_InterpBase::AddLine(const char *lin, C_Error &err)
{
	size_t length;
	length = strlen(lin);
	unsigned int a = 0;	// sprawdzanie czy nie jest pusta linia lub komentarz
	do	{
		;
	} while(lin[a++]==32 && a<length);
	a--;

	if(lin[a]==13 || length<2 || lin[a]=='%')
		return false;
	if(length<2 || lin[a]=='%' || lin[a]==10 || lin[a]==13)
		return false;
	else
		if(lin[a]<65 || (lin[a]<97 && lin[a]>91) || lin[a]>122)
			return false;
		else	{
			lastline++;
			line[lastline] = new C_Line(lastline);	// nadanie numeru linii
			line[lastline]->AnalyseLine(lin,err);
			if(err.status)	{
				err.numline = lastline;
				strcpy_s(err.e_line,maxline,lin); err.data = TRUE;
			}
			return true;
		}
}

void C_InterpBase::CreateObject(C_Line* line,int index, C_Commands** ptab, C_Error &err,BOOL st)
{
	// je�li st==0 to znaczy ze obiekt tworzony na chwile, je�li 1 to finalne tworzenie
	char* DLLname;
	if(strcmp(line->FN,"sin")==0)	{
		ptab[index] = new C_Command_Sin;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"imread")==0)	{
		ptab[index] = new C_Command_Imread;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"imshow")==0)	{
		ptab[index] = new C_Command_Imshow;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"imclose")==0)	{
		ptab[index] = new C_Command_Imclose;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"erode")==0)	{
		ptab[index] = new C_Command_Dilate;		// !!! tak ma byc
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"dilate")==0)	{
		ptab[index] = new C_Command_Erode;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"saveimagem")==0)	{
		ptab[index] = new C_Command_SaveImageM;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"filter2d")==0)	{
		ptab[index] = new C_Command_Filter2d;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"imadd")==0)	{
		ptab[index] = new C_Command_Imadd;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"add")==0)	{
		ptab[index] = new C_Command_Add;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"imsub")==0)	{
		ptab[index] = new C_Command_Imsub;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"immulti")==0)	{
		ptab[index] = new C_Command_Immulti;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
#ifdef WINDOWS_COMPILATION	
	if(strcmp(line->FN,"glshow")==0)	{
		ptab[index] = new C_Command_GLshow(parentwindow);
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"hist")==0)	{
		ptab[index] = new C_Command_Hist(parentwindow);
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
#else
	if(strcmp(line->FN,"glshow")==0)	{
		ptab[index] = new C_Command_GLshow;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"hist")==0)	{
		ptab[index] = new C_Command_Hist;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
#endif

	if(strcmp(line->FN,"ath")==0)	{
		ptab[index] = new C_Command_Th;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"outline")==0)	{
		ptab[index] = new C_Command_Outline;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"bdilate")==0)	{
		ptab[index] = new C_Command_Berode;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"berode")==0)	{
		ptab[index] = new C_Command_Bdilate;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"gth")==0)	{
		ptab[index] = new C_Command_Gth;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"ith")==0)	{
		ptab[index] = new C_Command_Ith;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"index")==0)	{
		ptab[index] = new C_Command_Particals;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"median")==0)	{
		ptab[index] = new C_Command_Median;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"oth")==0)	{
		ptab[index] = new C_Command_Oth;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"fftfilt")==0)	{
		ptab[index] = new C_Command_Fftfilt;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"eth")==0)	{
		ptab[index] = new C_Command_Eth;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"fillholes")==0)	{
		ptab[index] = new C_Command_Fillholes;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"chdir")==0)	{
		ptab[index] = new C_Command_Chdir;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"nth")==0)	{
		ptab[index] = new C_Command_Nth;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"sth")==0)	{
		ptab[index] = new C_Command_Sth;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"imwrite")==0)	{
		ptab[index] = new C_Command_Imwrite;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"ANNthreshold")==0)	{
		ptab[index] = new C_Command_Neural;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"matrixread")==0)	{
		ptab[index] = new C_Command_Matrixread;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"matrixwrite")==0)	{
		ptab[index] = new C_Command_Matrixwrite;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"cutroi")==0)	{
		ptab[index] = new C_Command_Cutroi;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"fastmedian")==0)	{
		ptab[index] = new C_Command_FastMedianqs;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"calcfeatures")==0)	{
		ptab[index] = new C_Command_Features;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"genindexedimage")==0)	{
		ptab[index] = new C_Command_Showfeatures;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"savefeatures")==0)	{
		ptab[index] = new C_Command_Savefeatures;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"sobel")==0)	{
		ptab[index] = new C_Command_Sobel;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"roberts")==0)	{
		ptab[index] = new C_Command_Roberts;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"prewitt")==0)	{
		ptab[index] = new C_Command_Prewitt;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"ANNclassifier")==0)	{
		ptab[index] = new C_Command_Annclasifier;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"normalize")==0)	{
		ptab[index] = new C_Command_Normalize;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"exit")==0)	{
		ptab[index] = new C_Command_Exit;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
/*	if(strcmp(line->FN,"iqi")==0)	{
		ptab[index] = new C_Command_IQI;
		return;
	}*/
/*	if(strcmp(line->FN,"crackdetection")==0)	{
		ptab[index] = new C_Command_Crackdetection;
		return;
	}*/
	if(strcmp(line->FN,"dm")==0)	{
		ptab[index] = new C_Command_Dm;
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}
	if(strcmp(line->FN,"threads")==0)	{
		ptab[index] = new C_Command_Thread;
		if(st)	{
			globals[lastglobal] = new C_Variables_Matrix(); 
			ptab[index]->current_global = lastglobal;
			ptab[index]->globals = &globals;
			ptab[index]->lastglobal = &lastglobal;
			lastglobal++;
		}
		return;
	}
	if(strcmp(line->FN,"iseeport")==0)	{
		ptab[index] = new C_Commans_ISeePort;
		if(st)	{
			globals[lastglobal] = new C_Variables_Matrix(); 
			ptab[index]->current_global = lastglobal;
			ptab[index]->globals = &globals;
			ptab[index]->lastglobal = &lastglobal;
			lastglobal++;
		}
		return;
	}
	DLLname = DLL_Container.CheckName(line->FN);
	if(DLLname!=NULL)	{
		ptab[index] = new C_Command_DLL(DLLname,dlldir,err);
		ptab[index]->globals = &globals;
		ptab[index]->lastglobal = &lastglobal;
		return;
	}

	ptab[index] = NULL;
}

C_Variables* C_InterpBase::CreateVariable(C_Line* line, DT type, int index, C_Variables **pvar)
{// TU DOCAC KASOWANIE PRZED TWORZENIEM
	SAFE_DELETE(pvar[index]);
	switch(type)	{
//		case NUMBER:
//			pvar[index] = new C_Variables_Matrix();
//			break;
		case STRING:
			pvar[index] = new C_Variables_String();
			break;
		case IMAGE:
			pvar[index] = new C_Variables_Image();
			break;
		case MATRIX:
			pvar[index] = new C_Variables_Matrix();
			break;
	}
	return pvar[index];
}

void C_InterpBase::CheckLine(int num,C_Error& err)
{
/* Tutaj mamy:	LHP - nazwa parametru lewego
				FN - nazwa funkcji
				RHPF - tablice z nazwami parametr�w funkcji je�li s�
				czysaparametryfunkcji - true je�li s� jakie� parametry
				RHPFTYPE - okre�la czw RHPF s� liczby (false) czy litery (true)

*/
	if(!line[num]->FNTYPE)	{// sprawdzanie jesli jest cos co moze byc nazwa funkcji a nie np liczba (a=5)
		CreateObject(line[num],0,&tmpfun,err,false);
		if(err.status) return;
		if(tmpfun==NULL)	{
			err.SetError("Unknown Command",num,line[num]->linetmp);
			return;
		}
	}
	if(!line[num]->FNTYPE)	{
		CheckInputParameters(line[num],err);	
		if(err.status) {
			err.numline = num;	
			strcpy_s(err.e_line,maxline,line[num]->linetmp); err.data = TRUE;
			return;
		}
	}

}
void C_InterpBase::CheckInputParameters(C_Line* line,C_Error& err)
{
/* por�wnuje parametry wej�ciowe przekazane w wywo�aniu funkcji do tych kt�re s� zdefiniowane w workspace */	
	FindVariable(line,err);		// sprawdza czy takie zmienne istniej�
	SAFE_DELETE(tmpfun);	// ostatni moment zycia zmiennej tymczasowej do analizy linii
	if(err.status) return;

}
void C_InterpBase::FindVariable(C_Line* line,C_Error& err)
{
	DT* parintmp = tmpfun->ReturnDTin();
	int a,b,znacznik=0;
	if(tmpfun->ReturnNumIn()!=line->numofrightparams)	{
		err.SetError("Bad number of input parameters");
		return;
	}
	if(tmpfun->ReturnNumOut()!=line->numofleftparams)	{
		err.SetError("Bad number of output parameters");
		return;
	}
	for(a=0;a<line->numofrightparams;a++)	{					// po parametrach funkcji
		if(line->RHPFTYPE[a]&& !line->STRTYPE[a])			// tylko gdy parametr nie jest liczb� i ne jest ci�giem tekstowym
			if(lastvar>0)	{
				for(b=0;b<lastvar;b++)		{					// po tym co w workspace
					znacznik = 0;
					if(strcmp(var[b]->name,line->RHPF[a])==0)	{	// je�li jest
						znacznik = 1;
						break;
					}
				}
				if(znacznik==1)	{
					if(var[b]->type!=parintmp[a])	{
							err.SetError("Bad type");
							return;
						}	
				}
				else	{
					err.SetError("Unknown variable");
				}
			}
			else	{
				err.SetError("Unknown variable");
				return;
			}
	}
}
int C_InterpBase::GetIndexofVariable(char* name)
{
	int a;
	for(a=0;a<lastvar;a++)
		if(strcmp(name,var[a]->name)==0)
			return a;
	return -1;
}
void C_InterpBase::BuildLine(int num,C_Error& err)
{
	int a,index;
	DT* type;
	double liczba;
	C_Matrix_Container tmpmatrix;	
	if(line[num]->FNTYPE)	{	//gdy fn nie jest nazwa funkcji
		index = GetIndexofVariable(line[num]->LHPF[0]);
		if(line[num]->STRTYPE[0])	{	// gdy jest stringiem
			if(index<0)	{	// zmienna nie istnieje - tworzymy
				CreateVariable(line[num],STRING,lastvar,var);	// adres stworzonej zmiennej jest zapami�tywany w tablicy intab w obiekcie funkcji
				var[lastvar]->AssignName(line[num]->LHPF[0]);
				var[lastvar]->AssignValue((void*)line[num]->FN,err);
				lastvar++;
			}
			else	{
				CreateVariable(line[num],STRING,index,var);	// adres stworzonej zmiennej jest zapami�tywany w tablicy intab w obiekcie funkcji
				var[index]->AssignName(line[num]->LHPF[0]);
				var[index]->AssignValue((void*)line[num]->FN,err);
			}
		}
		else	{		// gdy jest liczb�
			liczba = atof(line[num]->FN);
			if(index<0)	{	// zmienna nie istnieje - tworzymy
				CreateVariable(line[num],NUMBER,lastvar,var);	// adres stworzonej zmiennej jest zapami�tywany w tablicy intab w obiekcie funkcji
				var[lastvar]->AssignName(line[num]->LHPF[0]);
				tmpmatrix.AllocateData(1,1); tmpmatrix.data[0] = liczba;
				var[lastvar]->AssignValue((void*)&tmpmatrix,err);
				lastvar++;
			}
			else	{
				CreateVariable(line[num],NUMBER,index,var);	// adres stworzonej zmiennej jest zapami�tywany w tablicy intab w obiekcie funkcji
				var[index]->AssignName(line[num]->LHPF[0]);
				var[index]->AssignValue((void*)&liczba,err);
			}
		}
	}
	else	{	// gdy w FN nie jest liczba ani ci�g tekstowy
		CreateObject(line[num],lasttab,tab,err,true);		// tworzenie obiektu funkcji
		if(err.status) return;
		if(line[num]->numofleftparams>0)	{			// je�li s� jakie� parametry wyj�ciowe
			type = tab[lasttab]->ReturnDTout();	// to pobieramy jakie s� ich typy
			for(a=0;a<line[num]->numofleftparams;a++)	{	// tworzymy potrzebne zmienne wyj�ciowe
				index = GetIndexofVariable(line[num]->LHPF[a]);
				if(index<0)	{	// zmienna nie istnieje - tworzymy
					tab[lasttab]->outtab[a] = CreateVariable(line[num],type[a],lastvar,var);	// adres stworzonej zmiennej jest zapami�tywany w tablicy intab w obiekcie funkcji
					tab[lasttab]->outtab[a]->AssignName(line[num]->LHPF[a]);
					lastvar++;
				}
				else	{
					tab[lasttab]->outtab[a] = CreateVariable(line[num],type[a],index,var);		// tworzymy w miejsce istniej�cej o tej nazwie
					tab[lasttab]->outtab[a]->AssignName(line[num]->LHPF[a]);
				}
			}	// for(a)
		}	// if(line.numofleftparams)
	// Zmienne wej�ciowe
		for(a=0;a<line[num]->numofrightparams;a++)
			if(!line[num]->RHPFTYPE[a])	{		// czyli jest cyfra
				liczba = atof(line[num]->RHPF[a]);
				tab[lasttab]->intab[a] = CreateVariable(line[num],NUMBER,lastvar,var);
				lastvar++;
				tab[lasttab]->intab[a]->AssignName("");
				tmpmatrix.AllocateData(1,1); tmpmatrix.data[0] = liczba;
				tab[lasttab]->intab[a]->AssignValue((void*)&tmpmatrix,err);
			}
			else	{
				if(line[num]->RHPFTYPE[a] && !line[num]->STRTYPE[a])	{		// jesli zmienna
					index = GetIndexofVariable(line[num]->RHPF[a]);
					tab[lasttab]->intab[a] = var[index];
				}
				if(line[num]->RHPFTYPE[a] && line[num]->STRTYPE[a])	{		// ci�g tekstowy
					tab[lasttab]->intab[a] = CreateVariable(line[num],STRING,lastvar,var);
					lastvar++;
					tab[lasttab]->intab[a]->AssignName("");
					tab[lasttab]->intab[a]->AssignValue((void*)line[num]->RHPF[a],err);
				}
			}
			lasttab++;
	}	// of czyjestliczba
}

void C_InterpBase::Start(C_Error& err,C_CommandStack& cs, unsigned int line)
{
	char blad[512];
	int numoflines = lasttab;
	int a=line;

	do	{
		tab[a]->Do(err);
		if(err.userdata)	{
			cs.AddString(err.user);	
			err.userdata = FALSE;
		}
		a++;
	}
	while(a<numoflines && !err.status);
	if(err.status)	{
		sprintf_s(blad,_T("Error in line %d - %s"),a,err.error);
		err.SetError(blad);
	}
}
void C_InterpBase::Step(int linenum, C_Error& err,C_CommandStack& cs)
{
	char blad[512];

	tab[linenum]->Do(err);
	if(err.userdata)
		cs.AddString(err.user);	
	
	if(err.status)	{
		sprintf_s(blad,"Error in line %d - %s",linenum,err.error);
		err.SetError(blad);
	}
}

void C_InterpBase::PrintVariable(char* const name,C_Error& err)
{
	int number;
	number = GetIndexofVariable(name);
	if(number<0)	{
		err.SetError("Unknown variable");
		return;
	}
	switch(var[number]->type)	{
		case NUMBER:
			printf("%f\n",*((double*)var[number]->data));
			break;
		case STRING:
			printf("%s\n",(char*)var[number]->data);
			break;
	}
}
void C_InterpBase::PrintVariable(C_Variables* var,C_Error& err)
{
	switch(var->type)	{
		case NUMBER:
			printf("%f\n",*((double*)var->data));
			break;
		case STRING:
			printf("%s\n",(char*)var->data);
			break;
	}
}
int C_InterpBase::GetVariable(char* const name,char* tab, C_Error& err)
{
	int number;
	int znaki;
	number = GetIndexofVariable(name);
	if(number<0)	{
		err.SetError("Unknown variable");
		return 0;
	}
	switch(var[number]->type)	{
//		case NUMBER:
//			znaki = sprintf_s(tab,256,"%8.10f\n",*((double*)var[number]->data));
//			break;
		case MATRIX:
			C_Matrix_Container *tmp1;
			tmp1 = (C_Matrix_Container*)var[number]->data;
//			znaki = sprintf_s(tab,256,"[%3.2f ...\n",*((double*)var[number]->data));
			znaki = sprintf_s(tab,256,"%s x = %d, y = %d\n","MATRIX",tmp1->_cols, tmp1->_rows );
			break;
		case STRING:
			znaki = sprintf_s(tab,256,"%s\n",(char*)var[number]->data);
			break;
		case IMAGE:
			C_Image_Container *tmp;
			tmp = (C_Image_Container*)var[number]->data;
			znaki = sprintf_s(tab,256,"%s x = %d, y = %d\n","IMAGE",tmp->_cols, tmp->_rows );
			break;
		default:
			znaki = sprintf_s(tab,256,"%s\n","not printable");
			break;
	}
	return znaki;
}

void C_InterpBase::SetDLLDir(char* _dlldir)
{
	strcpy_s(dlldir,1024,_dlldir);
}

int C_InterpBase::DecodeType(UINT type, UINT* tab)
{
	int a;
	int t[8] = {1,2,4,8,16,32,64,128};
	UINT tabtmp[8];
	int licznik = 0;
	UINT ret;
	for(a=0;a<8;a++)	{
		ret = type & t[a];
		if(ret>0)
			tabtmp[licznik++] = ret;
	}

	for(a=0;a<licznik;a++)
		tab[a] = tabtmp[a];	// sprawdzi� jak s� tablice bez rozmiaru
	return licznik-1;



}