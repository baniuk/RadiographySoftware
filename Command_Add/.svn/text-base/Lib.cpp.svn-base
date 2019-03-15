#include "Cpp_Lib.h"
#include <math.h>
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

#define FIND(matrix,wyr,out) {\
	unsigned int tmp_find = 0;\
	unsigned int a_find = 0;\
	for(a_find=0;a_find<(matrix).GetNumofElements();a_find++)\
	if((matrix).data[a_find]##wyr)\
			tmp_find++;\
	(out).AllocateData(1,tmp_find);\
	tmp_find = 0;\
	for(a_find=0;a_find<(matrix).GetNumofElements();a_find++)\
	if((matrix).data[a_find]##wyr)\
			(out).SetPixel(1,tmp_find++,a_find);}

void quickSort( double a[], int l, int r);
int partition( double a[], int l, int r);
void Normalize(C_Matrix_Container* input, unsigned short w1, unsigned short w2);
void CutMatrix(C_Matrix_Container* input, unsigned int start, unsigned int stop);
double round(double liczba);
void diff(C_Matrix_Container* in,C_Matrix_Container* out);
unsigned long iselement(C_Matrix_Container* in, double element);

class Cell
{
public:
	Cell(void);
	~Cell(void);
	C_Matrix_Container** celltab;
	void CreateCell(int num);
	void DeletePart(int start,int stop);
	unsigned int length(void);
private:
		int numofcells;
};

Cell::Cell(void)
{
	numofcells = 0;
}

Cell::~Cell(void)
{
	int a;
	if(numofcells>0)	{
		for(a=0;a<numofcells;a++)
			SAFE_DELETE(celltab[a]);
		delete[] celltab;
	}
}

void Cell::CreateCell(int num)
{
	celltab = new C_Matrix_Container*[num];
	numofcells = num;
	for(int a=0;a<numofcells;a++)
		celltab[a] = new C_Matrix_Container;
}
void Cell::DeletePart(int start,int stop)
{
	for(int a=start;a<stop;a++)
		SAFE_DELETE(celltab[a]);
}
inline unsigned int Cell::length(void)
{
	return numofcells;
}

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"feat");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"MATRIX = feat(IMAGE,IMAGE,IMAGE,NUMBER,NUMBER,MATRIX)");
	definition->numofout = 2;
	definition->numofin = 6;
	definition->typesofin[0] = IMAGE; definition->typesofin[1] = IMAGE;definition->typesofin[2] = IMAGE; 
	definition->typesofin[3] = NUMBER;definition->typesofin[4] = NUMBER; definition->typesofin[5] = MATRIX;
	definition->typesofout[0] = MATRIX;
	definition->typesofout[1] = NUMBER;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"feat");
//	ADDIP_version = 2.0;

}


void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{

	C_Image_Container outln;
	C_Image_Container *image;
	C_Image_Container *indek;
	C_Image_Container *bin;
	C_Image_Container now;
	C_Matrix_Container *minsiz;
	C_Matrix_Container *maxsiz;
	C_Matrix_Container *param;
	

	image=(C_Image_Container*)intab[0]->data;
	indek=(C_Image_Container*)intab[1]->data;
	bin=(C_Image_Container*)intab[2]->data;
	minsiz=(C_Matrix_Container*)intab[3]->data;
	maxsiz=(C_Matrix_Container*)intab[4]->data;
	param=(C_Matrix_Container*)intab[5]->data;

	
}

void CutMatrix(C_Matrix_Container* input, unsigned int start, unsigned int stop)
{
// wycina po kolumnach od start do stop. Modyfikuje strukture weœciow¹
	unsigned int c,r;
	double pixel;
	unsigned long column=0;
	C_Matrix_Container tmp;
	tmp.AllocateData(input->_rows,stop-start);
	for(c=start;c<=stop;c++)	{
		column = 0;
		for(r=0;r<input->_rows;r++)	{
			input->GetPixel(r,c,pixel);
			tmp.SetPixel(r,column++,pixel);
		}
	}
	input->FreeData();
	tmp.CloneObject(input);
}

double std2(C_Matrix_Container* input)
{
// oblicza std po wszystkich elementach input
	unsigned long a,numofelem;
	double srednia=0;
	double suma=0;
	numofelem = input->GetNumofElements();
	for(a=0;a<numofelem;a++)
		srednia+=input->data[a];
	srednia/=numofelem;

	for(a=0;a<numofelem;a++)
		suma+=pow( (input->data[a] - srednia),2 );
	suma = sqrt(suma/(numofelem-1));
	return suma;
}

void std(C_Matrix_Container* input,C_Matrix_Container* output)
{
// oblicza std po wszystkich kolumnach input
	unsigned long a,numofelem,col;
	double srednia;
	double suma;
	output->AllocateData(1,input->_cols);
	for(col=0;col<input->_cols;col++)	{
		srednia = 0;
		suma = 0;
		numofelem = input->_rows;		// ilosc eklementow w kolumnia
		for(a=0;a<numofelem;a++)
			srednia+=input->data[a];
		srednia/=numofelem;
	
		for(a=0;a<numofelem;a++)
			suma+=pow( (input->data[a] - srednia),2 );
		suma = sqrt(suma/(numofelem-1));
		output->SetPixel(1,col,suma);
	}
}

double mean(C_Matrix_Container* input)
{
// oblicza sredni¹ po wektorze
	unsigned long a,col;
	double suma=0,srednia;
	for(col=0;col<input->_cols;col++)
		srednia+=input->data[a];
	srednia/=input->GetNumofElements();
	return srednia;
}

void dividetrend(C_Matrix_Container* input,Cell* output)
{
	// zmienia input
	unsigned long a,length,licznik=0,start,b;
	double stop;
	C_Matrix_Container g;
	C_Matrix_Container pozw;
	length = input->_cols;	// dlugosc wektora wejsciowego
	quickSort(input->data,0,length-1);
	// diff
	g.AllocateData(a,length-1);
	for(a=0;a<length-1;a++)
		g.data[a] = input->data[a+1] - input->data[a];
	// end of diff

	FIND(g,>1,pozw);
/*	// find
	tmp = 0;
	for(a=0;a<length-1;a++)
		if(g.data[a]>1)
			tmp++;
	pozw.AllocateData(1,tmp);
	tmp = 0;
	for(a=0;a<length-1;a++)
		if(g.data[a]>1)
			pozw.SetPixel(1,tmp++,a);
	// end of find*/
	start = 1;
	output->CreateCell(pozw.GetNumofElements());
	for(a=0;a<pozw.GetNumofElements();a++)	{
		stop = pozw.data[a];
		output->celltab[a]->AllocateData(1,(unsigned int)(stop-start+1));
		for(b=start;b<=stop;b++)
			output->celltab[a]->data[licznik++] = input->data[b];
		start = (unsigned int)stop + 1;
		licznik = 0;
	}
	if(pozw.data[a]<length-1)	{
		output->celltab[a+1]->AllocateData(1,length - (unsigned int)pozw.data[a] + 1);
		for(b=(unsigned int)pozw.data[a]+1;b<=length;b++)
			output->celltab[a]->data[licznik++] = input->data[b];
		licznik = 0;
	}
	else	{
		if(pozw.data[a]==length-1)	{
			output->celltab[a+1]->AllocateData(1,1);
			output->celltab[a+1]->data[0] = input->data[(int)pozw.data[a]];
		}
	}

	output->DeletePart(a+2,pozw.GetNumofElements());

	/*
	in = sort(in);
g = diff(in);

pozw = find(g>1);
start = 1;
if ~isempty(pozw)
    for a=1:length(pozw)
        stop = pozw(a);
        out{a} = in(start:stop);
        start = stop+1;
    end
    if pozw(a)<length(in)-1
        out{a+1} = [in(pozw(a)+1:end)];
    elseif pozw(a)==length(in)-1
        out{a+1} = in(pozw(a));
    end
else
    out=[];
end*/

}

void unique(C_Matrix_Container* input,C_Matrix_Container* output)
{
	C_Matrix_Container tmp;
	unsigned long a,ilosc=1,licznik=0;
	input->CloneObject(&tmp);
	quickSort(tmp.data,0,tmp._cols);
	for(a=0;a<tmp._cols-1;a++)
		if(tmp.data[a+1]!=tmp.data[a])
			ilosc++;
	output->AllocateData(1,ilosc);
	output->data[licznik++] = tmp.data[1];
	for(a=0;a<tmp._cols-1;a++)
		if(tmp.data[a+1]!=tmp.data[a])
			output->data[licznik++] = tmp.data[a+1];
}
void quickSort( double a[], int l, int r)
{
   int j;

   if( l < r ) 
   {
   	// divide and conquer
        j = partition( a, l, r);
       quickSort( a, l, j-1);
       quickSort( a, j+1, r);
   }
	
}

int partition( double a[], int l, int r)
{
   int i, j;
   double pivot,t;
   pivot = a[l];
   i = l; j = r+1;
		
   while( 1)
   {
   	do ++i; while( a[i] <= pivot && i <= r );
   	do --j; while( a[j] > pivot );
   	if( i >= j ) break;
   	t = a[i]; a[i] = a[j]; a[j] = t;
   }
   t = a[l]; a[l] = a[j]; a[j] = t;
   return j;
}
void find_peak1(C_Matrix_Container* input,double window, C_Matrix_Container* output)
{
	C_Matrix_Container data;
	C_Matrix_Container z;
	C_Matrix_Container ss;
	C_Matrix_Container diff;
	C_Matrix_Container out;
	C_Matrix_Container datans;
	C_Matrix_Container cut_data;
	C_Matrix_Container *cut_data_f = &cut_data;
	C_Matrix_Container indicator;
	Cell zz;
	double min,max;
	unsigned long l_data,licznik,peak_number,start_ind,start,tmp,a;

	input->CloneObject(&data);
	input->CloneObject(&datans);

	Normalize(&data,0,1);
	l_data = data.GetNumofElements();
	licznik = 0;
	peak_number = 1;
	start_ind = 0;

	out.AllocateData(1,l_data-(int)window);
	for(start=0;start<l_data-(int)window;start++)	{
		data.CloneObject(&cut_data);
		CutMatrix(&cut_data,start,start+(unsigned int)window-1);
		std(cut_data_f,&indicator);
// ------ wewn¹trz okna ----------------
		peak_number = 1;
		cut_data.getMinMax(min,max);
		// find
		tmp = 0;
		for(a=0;a<cut_data.GetNumofElements();a++)
			if(cut_data.data[a]==max)
				tmp++;
		ss.AllocateData(1,tmp);
		tmp = 0;
		for(a=0;a<cut_data.GetNumofElements();a++)
			if(cut_data.data[a]==max)
				ss.SetPixel(1,tmp++,a);
		// end of find
		out.data[licznik++] = ss.data[0]+start-1;
	}
	CutMatrix(&out,0,--licznik);
	licznik = 0;
	diff.AllocateData(1,out.GetNumofElements()-1);
	for(a=0;a<out.GetNumofElements()-1;a++)
		diff.data[a] = out.data[a+1]-out.data[a];
	// find
	tmp = 0;
	for(a=0;a<diff.GetNumofElements();a++)
		if(diff.data[a]==0)
			tmp++;
	z.AllocateData(1,tmp);
	tmp = 0;
	for(a=0;a<diff.GetNumofElements();a++)
		if(diff.data[a]==0)
			z.SetPixel(1,tmp++,a);
	// end of find
	licznik = 0;
	dividetrend(&z,&zz);
	out.AllocateData(1,zz.length());
	for(a=0;a<zz.length();a++)
		if(zz.celltab[a]->_cols==window-1)
			output->data[licznik++] = out.data[(int)zz.celltab[a]->data[0]];
	CutMatrix(output,0,--licznik);
/*
for start=1:l_data-window
    cut_data = data(start:start+window-1);
    if window<3*forder
        cut_data_f = cut_data;
    else
        cut_data_f = filtfilt(b,a,cut_data);
    end 
    
    indicator = std(cut_data);
% ------ wewn¹trz okna ----------------

%     peak_number = 1;
    ss=find(cut_data==unique(max(cut_data)));
    out(licznik) = ss(1)+start-1;
    licznik= licznik + 1;
% po tej operacji jeœli rozmiar okna bêdzie zblizony do rozmiaru okna to w
% out pojawi¹ siê stale fragmenty przebiegu (bo podczas przesuwania okna
% bêdzie dominowal ten sam pik, rozmiar okna powinien byc taki aby nie by³o
% mozliwosci zlapania dwochprazkow naraz
end
licznik = 1;
z = find(diff(out)==0);
zz = dividetrend(z);
for a=1:length(zz)
    if length(zz{a})==window-1
        pozktore(licznik) = out(zz{a}(1));
        licznik = licznik + 1;
    end
end
*/

}
void RemoveIndexes(C_Matrix_Container* in, C_Matrix_Container* index)
{
	// usuwa elementy z in o indexach z index
	C_Matrix_Container tmp;
	unsigned long licznik = 0,a,ilerazy;
	
	ilerazy = index->GetNumofElements();
	if(in->_cols>1 && in->_rows>1)
		return;
	if((in->GetNumofElements()-ilerazy)<1)	{
		in->FreeData();
		return;
	}
	in->CloneObject(&tmp);
	in->FreeData();
	if(tmp._cols>1)
		in->AllocateData(1,tmp._cols-ilerazy);
	else
		in->AllocateData(tmp._rows-ilerazy,1);
	for(a=0;a<tmp.GetNumofElements();a++)
		if(iselement(index,a)>0)
			in->data[licznik++] = tmp.data[a];
}

unsigned long iselement(C_Matrix_Container* in, double element)
{
	// ile razy wystepuje element w tablicy in
	unsigned long a,ile=0;
	for(a=0;a<in->GetNumofElements();a++)
		if(in->data[a]==element)
			ile++;
	return ile;
}
void RemoveElement(C_Matrix_Container* in,double element)
{
	// usuwa element z tablicy zmieniaj¹c jej rozmiar. Dziala tylko na wektorach
	unsigned long a,ilerazy = 0, licznik = 0,liczniktmp = 0;
	C_Matrix_Container tmp;
	if(in->_cols>1 && in->_rows>1)
		return;
	for(a=0;a<in->GetNumofElements();a++)
		if(in->data[a]==element)
			ilerazy++;
	in->CloneObject(&tmp);
	in->FreeData();
	if(tmp._cols>1)
		in->AllocateData(1,tmp._cols-ilerazy);
	else
		in->AllocateData(tmp._rows-ilerazy,1);
	for(a=0;a<tmp.GetNumofElements();a++)
		if(tmp.data[a]!=element)
			in->data[licznik++] = tmp.data[a];
}

void choose_peak1(C_Matrix_Container* in,double odl,double limit,C_Matrix_Container* proj,double tol,Cell* out)
{
	double zakres[2];
	unsigned long a,c=0,licznik=0,aa;
	C_Matrix_Container din;
	C_Matrix_Container zz;
	C_Matrix_Container zzd;
	C_Matrix_Container utmp;
	Cell *kand = out;
	double suma=0;
	

	zakres[0] = round(odl + (-tol*odl));
	zakres[1] = round(odl + (tol*odl));
	diff(in,&din);
	kand->CreateCell(din._cols);
	for(a=0;a<din._cols;a++)	{
		if( (din.data[a] >=zakres[0])&&(din.data[a]<=zakres[1]))	{
			kand->celltab[c]->data[licznik++] = in->data[a];
			kand->celltab[c]->data[licznik++] = in->data[a+1];
		} else {
			c++;
			licznik = 0;
		}
	}
	kand->DeletePart(c,din._cols);	// c albo --c ???
	for(a=0;a<kand->length();a++)	{
		zz.AllocateData(1,kand->celltab[a]->_cols);
		for(aa=0;aa<kand->celltab[a]->_cols;aa++)
			if(proj->data[(unsigned long)kand->celltab[a]->data[aa]]>limit)
				zz.data[aa] = 1.0;
			else
				zz.data[aa] = 0.0;
		zzd.AllocateData(1,kand->celltab[a]->_cols);
		FIND(zz,==0,zzd);
		RemoveIndexes(kand->celltab[a],&zzd);
		if(kand->celltab[a]->data!=NULL)	{
			unique(kand->celltab[a],&utmp);
			utmp.CloneObject(kand->celltab[a]);
		}

		
		//skonczyæ
	}
	

/*
zakres = round([odl odl] + odl*[-tol tol]);
c = 1;
cont = 1;
licznik = 1;
din = diff(in);
kand{1} = [];
for a=1:length(din)
    if din(a) >= zakres(1) && din(a)<=zakres(2)
         kand{c}(licznik) = in(a);
         licznik = licznik + 1;
         kand{c}(licznik) = in(a+1);
         licznik = licznik + 1;
    else
        c = c + 1;
        licznik = 1;
    end
end

for a=1:length(kand)
    if ~isempty(kand{a})
        zz = proj(kand{a})>limit;
        zzd = find(zz==0);
        kand{a}(zzd) = [];
    end
    if isempty(kand{a})
        kand{a} = [];
    else
        kand{a} = unique(kand{a});
    end        
end       
out = kand;
*/

}
void Normalize(C_Matrix_Container* input, unsigned short w1, unsigned short w2)
{
	double min, max, delta;
	unsigned long r;
	input->getMinMax(min,max);
	if(min==w1 && max==w2)
		return;				// bez normalizacji
	delta = abs(w2-w1);
	for(r=0;r<input->GetNumofElements();r++)
		input->data[r] = delta*input->data[r]/(max-min)-min*delta/(max-min);
}

double round(double liczba)
{
	if( abs(liczba-floor(liczba))<0.5)
		return floor(liczba);
	else
		return ceil(liczba);
}

void diff(C_Matrix_Container* in,C_Matrix_Container* out)
{
	// diff po kolumnach dla jednego rzedu
	unsigned long a;
	out->AllocateData(1,in->_cols-1);
	for(a=0;a<in->_cols-1;a++)
		out->data[a] = in->data[a+1] - in->data[a];
}