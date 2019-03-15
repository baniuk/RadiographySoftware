#include "C_Line.h"
#include "StdAfx.h"

C_Line::C_Line(void)
{
	iloscprzecinkow = 0;
	numofrightparams = 0;
	numofleftparams = 0;
	RHPF = NULL;
	LHPF = NULL;
	RHPFTYPE = NULL;
	STRTYPE = NULL;
	FNTYPE = false;
	numer = -1;
	strtemp1[1] = 0;
	strtemp2[1] = 0;
}
C_Line::C_Line(int num)
{
	iloscprzecinkow = 0;
	numofrightparams = 0;
	numofleftparams = 0;
	RHPF = NULL;
	LHPF = NULL;
	RHPFTYPE = NULL;
	STRTYPE = NULL;
	FNTYPE = false;
	numer = num;
	strtemp1[1] = 0;
	strtemp2[1] = 0;
}
C_Line::~C_Line(void)
{
	int a;
	for(a=0;a<numofrightparams;a++)
		delete RHPF[a];
	SAFE_DELETE(RHPF);
	for(a=0;a<numofleftparams;a++)
		delete LHPF[a];
	SAFE_DELETE(LHPF);
	SAFE_DELETE(RHPFTYPE);
	SAFE_DELETE(STRTYPE);
}


void C_Line::AnalyseLine(const char *line,C_Error& err)
{
//	numofchars = pnumofchars;
	int pozrowna;				// pozycja znaku równa
	int pozsrednik;				// pozycja œrednika
	int poznawiasl, poznawiasr;	// pozycja œrednika
	int pozprzecinka[256];		// pozycja przecinka
	int a,tmp,tmplen;
	double liczba;
	char *stopstring;
	char leftparam[256];
	int licz=0; //liczy '
	int status = 0;	// kopiuj z wycinaniem spacj, 1 - wszystko
	numofchars = (int)strlen(line);
	if(numofchars>maxline) {
		err.SetError("Line too long");
		return;
	}
	strcpy_s(linetmp,maxline,line);		// kopiowanie linii do bufora
	numofchars = RemoveSpaces(linetmp);
// wyodrebnianie parametrów lewo i prawostronnych
	pozrowna = FindChar(linetmp,'=');	// znajdowanie znaku równa siê
	if(pozrowna==CHARNOEXIST)
		CutLine(linetmp,RHP,0,numofchars); // w RHP znajdzie sie nazwa funkcji z œrednikiem koñcz¹cym
	else	{	// tu obs³uga wielu parametrów wyjœciowych
		CutLine(linetmp,leftparam,0,pozrowna);	// w leftparam wszystko na lewo rowna sie
		numofleftparams = GetNumofLeftParam(leftparam, err);
		if(err.status)
			return;
		LHPF = new char*[numofleftparams];	for(int a=0;a<numofleftparams;a++) LHPF[a] = new char[256];
		ExtractLeft(leftparam, LHPF, err);
//		CutLine(linetmp,LHPF[0],0,pozrowna);		// w LHP znajdzie siê nazwa parametru lewego
		CutLine(linetmp,RHP,pozrowna+1,numofchars); // w RHP znajdzie sie nazwa funkcji z œrednikiem koñcz¹cym
	}
// wyodrebnianie nazwy funkcji i jej parametrów
	pozsrednik = FindChar(RHP,';');	// znajdowanie znaku ;
	poznawiasl = FindChar(RHP,'(');	// znajdowanie znaku (
	poznawiasr = FindChar(RHP,')');	// znajdowanie znaku )
	if( (poznawiasl==CHARNOEXIST && poznawiasr!=CHARNOEXIST) ||
		(poznawiasr==CHARNOEXIST && poznawiasl!=CHARNOEXIST) ||
		(poznawiasr<poznawiasl))
	{
		err.SetError("Check () or ;");
		return;
	}
	if(poznawiasl==CHARNOEXIST && poznawiasr==CHARNOEXIST) {
		CutLine(RHP,FN,0,pozsrednik);			// w FN znajdzie siê nazwa czegoœ gdy nie ma nawiasow
		czysaparametryfunkcji = false;
	} else {
		if( ((poznawiasr - poznawiasl)==1)) {
			CutLine(RHP,FN,0,poznawiasl);			// w FN znajdzie siê nazwa funkcji gdy nie ma parametrów
			czysaparametryfunkcji = false;
		}
		if((poznawiasr - poznawiasl)>1) {
			CutLine(RHP,FN,0,poznawiasl);		// w FN znajdzie siê nazwa funkcji
			CutLine(RHP,PARAM,poznawiasl+1,poznawiasr);		// w FN znajdzie siê parametry same
			czysaparametryfunkcji = true;
		}
	}
// wyodrêbninie parametrów funkcji
	if(czysaparametryfunkcji) {
		for(a=0;a<(int)strlen(PARAM);a++)	{		// tu zmiana z FN
			if(PARAM[a]==39)	{
				status = 1;
				licz++;
			}
			if(PARAM[a]==39 && licz==2)	
				status = 0;	
			if(PARAM[a]==',' && status==0) {	// przecinek ale nie w ci¹gu pomiêdzy ''
				pozprzecinka[iloscprzecinkow] = a;
				iloscprzecinkow++;
			}
		}
		RHPF = new char*[iloscprzecinkow+1]; RHPFTYPE = new BOOL[iloscprzecinkow+1]; STRTYPE = new BOOL[iloscprzecinkow+1];
		for(a=0;a<iloscprzecinkow+1;a++)
			RHPF[a] = new char[256];		// tworzenie tablicy z tablicami z nazwami parametrów funkcji
		numofrightparams = iloscprzecinkow+1;
		tmp = -1;
		for(a=0;a<iloscprzecinkow+1;a++) {
			CutLine(PARAM,RHPF[a],tmp+1,a==iloscprzecinkow ? (int)strlen(PARAM) : pozprzecinka[a]); // w PARMA znajdzie sie same parametrybez nawiasów
			tmp = pozprzecinka[a];
			liczba = strtod(RHPF[a],&stopstring);
			if(*(stopstring)==0)
				RHPFTYPE[a] = false;		// czyli jest liczba
			else	{						// sprawdzam czy nie jest zmienna tekstowa
				tmplen = (int)strlen(RHPF[a]);
				strtemp1[0] = RHPF[a][0];	strtemp2[0] = RHPF[a][tmplen-1];
				if(strcmp(strtemp1,"'")==0 && strcmp(strtemp2,"'")==0)	// czyli string zakoñczony z dwóch stron apostrofem
					{STRTYPE[a] = true;	RHPFTYPE[a] = true;}
				else
					{RHPFTYPE[a] = true; STRTYPE[a] = false;}			// czyli w RHPF jest litera - zmienna
			}
		}
	}
	else	{	// nie ma parametrów funkcji i nie ma nawiasów - nalezy sprawdziæ czy w FN znajduje siê nazwa funkcji czy liczba lub zmienn¹ tekstow¹
		if(poznawiasl==CHARNOEXIST && poznawiasr==CHARNOEXIST)	{
			STRTYPE = new BOOL;
			liczba = strtod(FN,&stopstring);
			if(*(stopstring)==0)		// liczba
			{
				FNTYPE = true;
				STRTYPE[0] = false;
			}
			else	{		// sprawdzam czy nie ma œredników
				tmplen = (int)strlen(FN);
				strtemp1[0] = FN[0];	strtemp2[0] = FN[tmplen-1];
				if(strcmp(strtemp1,"'")==0 && strcmp(strtemp2,"'")==0)	// czyli string zakoñczony z dwóch stron apostrofem
					{STRTYPE[0] = true;	FNTYPE = true;}
				else	{
					err.SetError("String without '");
					return;
					//{STRTYPE[0] = false; FNTYPE = true;}
				}			

			}
			iloscprzecinkow--; // aby nie bylo problemów przy destruktorze
		}
	}

/* Tutaj mamy:	LHP - nazwa parametru lewego
				FN - nazwa funkcji
				RHPF - tablice z nazwami parametrów funkcji jeœli s¹
				czysaparametryfunkcji - true jeœi s¹ jakieœ parametry
				numofrightparams - iloœæ parametrów RHPF
				RHPFTYPE - okreœla czw RHPF s¹ liczby (false) czy litery (true)
				FNTYPE - true jesli w FN nie ma nazwy funkcji tylko liczba albo ci¹g tekstowy

*/
}
int C_Line::GetNumofLeftParam(char* lewe, C_Error& err)
{
	unsigned int poz1, poz2,licznik=0,numofparam=0;
	poz1 = FindChar(lewe,'[');
	poz2 = FindChar(lewe,']');
	size_t len = strlen(lewe);
	if(poz1==CHARNOEXIST && poz2==CHARNOEXIST)
		return 1;
	if(poz1<poz2)	{
		while(licznik<len)	{
			if(*(lewe+licznik)==44)
				numofparam++;
			licznik++;
		}		
	}
	else	{
		err.SetError("Check ()");
		return -1;
	}
	return ++numofparam;
}
void C_Line::ExtractLeft(char* lewe, char** LHPF, C_Error& err)
{
	unsigned int poz1, poz2,licznik=0,start=1,numofprzec=0; //start=1 bo [
	poz1 = FindChar(lewe,'[');
	poz2 = FindChar(lewe,']');
	size_t len = strlen(lewe);
	int numofleftparams=0;
	if(poz1==CHARNOEXIST && poz2==CHARNOEXIST)	{
		CutLine(lewe,LHPF[0],0,(int)len);
		return;
	}
	if(poz1<poz2)	{
		// sa nawiasy
		while(licznik<len)	{
			if(*(lewe+licznik)==44)	{
				CutLine(lewe,LHPF[numofprzec],start,licznik);
				start=licznik+1;			
				numofprzec++;
			}
			licznik++;
		}		
		CutLine(lewe,LHPF[numofprzec],start,licznik-1);
	}
	else	{
		err.SetError("Check []");
		return;
	}

	

}

int C_Line::RemoveSpaces(char *line)
{
	int a,licznik=0;
	char pozspacje[256];
	int status = 0;	// kopiuj z wycinaniem spacj, 1 - wszystko
	int licz=0; //liczy '
	for(a=0;a<numofchars+1;a++) {
		if(linetmp[a]==39)	{
			status = 1;
			licz++;
		}
		if(linetmp[a]==39 && licz==2)
			status = 0;		


		if(linetmp[a]!=(char)' ') {
			pozspacje[licznik] = linetmp[a];
			licznik++;
		}
		else
			if(linetmp[a]==(char)' ' && status==1)	{
				pozspacje[licznik] = linetmp[a];
				licznik++;
			}
	}
	strcpy_s(linetmp,licznik,pozspacje);
	return licznik;
}


int C_Line::FindChar(char *line, const char znak)
{
	int a;
	int status = 0;	// kopiuj z wycinaniem spacj, 1 - wszystko
	int licz=0; //liczy '
	for(a=0;a<numofchars;a++)	{
		if(line[a]==39)	{
			status = 1;
			licz++;
		}
		if(line[a]==39 && licz==2)	{
			status = 0;
			licz = 0;
		}
		if(status==0)
			if(line[a]==znak) {
				return a;
				break;
			}
	}
	return CHARNOEXIST;
}
void C_Line::CutLine(char *line, char *linecutted, int start, int stop)
{
	int a;
	int licznik = 0;
	for(a=start;a<stop;a++)
		*(linecutted++) = line[a];
	*(linecutted) = 0;	// koñczenie zerem aby dobrze strlen dzia³a³

}

int C_Line::GetLengthOfString(char* str, int maxlen)
{
	int a;
	for(a=0;a<maxlen;a++)
		if(str[a]==0)
			break;
	return a;
}