#pragma once
//#include "Cpp_Lib.h"
#include "Cell.h"
#define M_PI       3.14159265358979323846
#include <math.h>
#include <crtdbg.h>

// This is an empty project to build DLL library of interface version 1.0

#define FIND(matrix,wyr,out) {\
	unsigned long tmp_find = 0;\
	unsigned long a_find = 0;\
	for(a_find=0;a_find<(matrix).GetNumofElements();a_find++)\
		if((matrix).data[a_find]##wyr)\
			tmp_find++;\
	(out).AllocateData(1,tmp_find);\
	tmp_find = 0;\
	for(a_find=0;a_find<(matrix).GetNumofElements();a_find++)\
		if((matrix).data[a_find]##wyr)\
			(out).SetPixel(0,tmp_find++,a_find);}

void quickSort( double a[], int l, int r);
int partition( double a[], int l, int r);
void Normalize(C_Matrix_Container* input, unsigned short w1, unsigned short w2);
void CutMatrixCol(C_Matrix_Container* input, unsigned int start, unsigned int stop);
void CutMatrixMatCol(C_Matrix_Container* input, C_Matrix_Container* kolumny);
void diff(C_Matrix_Container* in,C_Matrix_Container* out);
unsigned long iselement(C_Matrix_Container* in, double element);
double std2(C_Matrix_Container* input);
void RemoveMatrixCol(C_Matrix_Container* input, unsigned int start, unsigned int stop);
void std(C_Matrix_Container* input,C_Matrix_Container* output);
double mean(C_Matrix_Container* input);
void dividetrend(C_Matrix_Container* input1,Cell* output);
void findonevalue(C_Matrix_Container* input,C_Matrix_Container* output,double wartosc);
double median(double* data,unsigned long size);
void mad(C_Matrix_Container* input,C_Matrix_Container* output);
void GetIndexesAfterSort(C_Matrix_Container* unsorted, C_Matrix_Container* sorted, C_Matrix_Container* output);
void find_peak1(C_Matrix_Container* input,double window, C_Matrix_Container* output);
void choose_peak1(C_Matrix_Container* in,double odl,double limit,C_Matrix_Container* proj,double tol,Cell* out);
double sinc(double x);
void oradon(double *out,double *in,double *setpar);
void prepareradon(C_Matrix_Container* image, double* setpar);
double round(double liczba);
void GetRadonLine(double alpha,double k,C_Matrix_Container* srodek,C_Matrix_Container* out);
unsigned long Optim(C_Matrix_Container* line,double szer);
int prvTestInside(double* p_point, double p0_x, double p0_y, double p1_x, double p1_y);
void BWCUT(C_Matrix_Container* BW,double angle,double rangelow, double rangehi,C_Matrix_Container* out);
void mojradon(C_Matrix_Container* image, int katstep, int katstart,int katstop,unsigned long kstep, C_Matrix_Container* out,BOOL ext);
void AllocAndRotate (  
    C_Matrix_Container       *pSrc, 
    double      dAngle,
    double    clrBack,
	C_Matrix_Container *pDst
);
void Rotate45  (
    C_Matrix_Container *pSrc, 
    double dAngle,
    double clrBack,
    BOOL bMidImage,
	C_Matrix_Container* out);
void Rotate270(C_Matrix_Container *pSrc, C_Matrix_Container* out);
void Rotate180(C_Matrix_Container *pSrc, C_Matrix_Container* out);
void Rotate90(C_Matrix_Container *pSrc, C_Matrix_Container* out);
void VertSkew (
    C_Matrix_Container *pSrc, 
    C_Matrix_Container *pDst, 
    UINT uCol,
    int iOffset,
    double dWeight,
    double clrBack);
void HorizSkew (
    C_Matrix_Container *pSrc, 
    C_Matrix_Container *pDst, 
    UINT uRow, 
    int iOffset, 
    double dWeight,
    double clrBack);



void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"iqi");
	strcpy_s(definition->version,MAX_VER,"0.5");
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wi�kszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"[MATRIX,IMAGE] = iqi(IMAGE,IMAGE,MATRIX,MATRIX,MATRIX,MATRIX)");
	definition->numofout = 2;
	definition->numofin = 6;
	definition->typesofin[0] = IMAGE;definition->typesofin[1] = IMAGE;definition->typesofin[2] = MATRIX;definition->typesofin[3] = MATRIX;definition->typesofin[4] = MATRIX;definition->typesofin[5] = MATRIX;
	definition->typesofout[0] = MATRIX; definition->typesofout[1] = IMAGE;
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
	C_Image_Container *image;
	BOOL nierobic = FALSE;
	C_Matrix_Container *R1;
	C_Image_Container *BW2tmp;
	C_Matrix_Container Xp11;
	C_Matrix_Container *BW2;
	C_Matrix_Container *Xp1;
	C_Matrix_Container BW2CUT;
	C_Matrix_Container Xp;
	C_Matrix_Container p;
//	C_Image_Container radon;
	C_Image_Container out1;
	C_Matrix_Container tmpR1;
	C_Matrix_Container theta; C_Matrix_Container theta1;
	C_Matrix_Container prawd;
	C_Matrix_Container s;
	C_Matrix_Container m;
	C_Matrix_Container ktore;
	C_Matrix_Container pozktore_tmp;
	C_Matrix_Container* oo;
	C_Matrix_Container* mxmy;
	C_Matrix_Container* Mtmp;
	C_Matrix_Container pozktore;
	C_Matrix_Container *k;
	C_Matrix_Container *k1;
	C_Matrix_Container *index_tmp;
	C_Matrix_Container *index_tmp_tmp;
	C_Matrix_Container *tmpktore;
	C_Matrix_Container sorted_proj_tmp;
	C_Matrix_Container sorted_index_tmp;
	C_Matrix_Container poz_wieksze_limit;
	C_Matrix_Container ktmp;
	C_Matrix_Container ktmpdiff;
	C_Matrix_Container BW2m;
	C_Image_Container image_mark;
	Cell unsorted_proj;
	Cell limit;
	Cell sorted_proj;
	Cell sorted_index;
	Cell unsorted_proj1;
	Cell pp;
	C_Matrix_Container proj;
	C_Matrix_Container precik;
	C_Matrix_Container linlin90;
	C_Matrix_Container RR1;
	Cell* out;
	C_Matrix_Container *pzakres;
	C_Matrix_Container *ptol;
	C_Matrix_Container *pszer;
	C_Matrix_Container *psharp;


	double calosc,tmpmax,unused;
	short sign;
	unsigned long u,maxlen,index,outoptim,kesttmp; 
	double angle,rangelow,rangehi,lin1,lin2;
	C_Matrix_Container R;
	res** result;
	double setpar[9];
	double srodek_x,srodek_y;
	double zakres, tol, szer,sharp,peaks = 3,prawd_prazka = 0.05,num_lines = 6;
	unsigned int maxkat = 20;
	theta.AllocateData(1,180);
	for(int a=0;a<180;a++)
		theta.SetPixel(0,a,a);
	theta.CloneObject(&theta1);

	image = (C_Image_Container*)intab[0]->data;
	BW2tmp = (C_Image_Container*)intab[1]->data;
	pzakres = (C_Matrix_Container*)intab[2]->data;
	ptol = (C_Matrix_Container*)intab[3]->data;
	pszer = (C_Matrix_Container*)intab[4]->data;
	psharp = (C_Matrix_Container*)intab[5]->data;

	zakres = pzakres->data[0];
	tol = ptol->data[0];
	szer = pszer->data[0];
	sharp = psharp->data[0];


	BW2m.AllocateData(BW2tmp->_rows,BW2tmp->_cols);
	for(unsigned long hh=0;hh<BW2tmp->GetNumofElements();hh++)
		BW2m.data[hh] = BW2tmp->data[hh];

	BW2 = &BW2m;

	image->CloneObject(&image_mark);

	unsigned int maksp;
	double przekatna;
	if(image->_cols>image->_rows)
		maksp = image->_cols;
	else
		maksp = image->_rows;
	przekatna = (unsigned long)ceil(sqrt(pow(image->_cols,2.0)+pow(image->_rows,2.0)));
	RR1.AllocateData(przekatna,180);
	RR1.Zeros();

	mojradon(BW2,1,0,maxkat,1,&RR1,1);	
	mojradon(BW2,1,180-maxkat,180,1,&RR1,1);

	R1 = &RR1;
	R1->CloneObject(&tmpR1);
	RemoveMatrixCol(&tmpR1,maxkat+1,180-maxkat);//

	
//	RR1.DumpBinary("c:\\myrad.m");
	Xp11.AllocateData(1,RR1._rows);
	for(int a=0;a<RR1._rows;a++)
		Xp11.data[a] = -int(RR1._rows/2.0)+a;
//	Xp11.DumpBinary("c:\\myxp1.m");
	Xp1 = &Xp11;//



//	Xp1->Transpose(); for(int a=0;a<Xp1->_cols;a++) Xp1->data[a]-=1;
	Xp1->CloneObject(&Xp);
	srodek_x = floor((BW2->_cols+1.0)/2.0); srodek_y = floor((BW2->_rows+1.0)/2.0);
	
//	RemoveMatrixCol(&tmpR1,maxkat,179-maxkat);
	RemoveMatrixCol(&theta,maxkat,179-maxkat);
//	theta.Dump("c:\\itheta.m");//
//	tmpR1.Dump("c:\\itmpR1.m");//
	calosc = std2(&tmpR1);//
	mad(&tmpR1,&prawd);	//prawd.Dump("c:\\iprawd.m");	//
	tmpR1.Transpose();std(&tmpR1,&s); tmpR1.Transpose(); //s.Dump("c:\\is.m");//
	FIND(s,>calosc,ktore);	//ktore.Dump("c:\\iktore.m");//
	mad(&tmpR1,&m); //m.Dump("c:\\im.m");	//
	FIND(m,<mean(&m),pozktore_tmp); //pozktore_tmp.Dump("c:\\ipozktore_tmp.m");//

	out = new Cell;
	oo = new C_Matrix_Container;
	mxmy = new C_Matrix_Container;
	dividetrend(&pozktore_tmp,out);// out->Dump("c:\\out.m");//
	pozktore.AllocateData(1,out->length());
	for(unsigned int xx=0;xx<out->length();xx++)	{
		tmpR1.CloneObject(oo);
		CutMatrixMatCol(oo,out->celltab[xx]);//oo->Dump("c:\\ioo.m");//
		oo->getMinMax(unused,tmpmax);//
		findonevalue(oo,mxmy,tmpmax); //mxmy->Dump("c:\\imxmy.m");//
		pozktore.data[xx] = mxmy->GetPixel(1,0) + out->celltab[xx]->data[0]; //pozktore.Dump("c:\\ipozktore.m"); //
	}
	calosc = calosc-0.05*calosc;
	delete out;	delete oo; delete mxmy;
//ok
	Mtmp = new C_Matrix_Container;
	Mtmp->AllocateData(1,1);
	unsorted_proj.CreateCell(pozktore.GetNumofElements());
	limit.CreateCell(pozktore.GetNumofElements());
	unsorted_proj1.CreateCell(pozktore.GetNumofElements());
	sorted_index.CreateCell(pozktore.GetNumofElements());
	sorted_proj.CreateCell(pozktore.GetNumofElements());
	for(unsigned int  xx=0;xx<pozktore.GetNumofElements();xx++)	{
		if(pozktore.data[xx]<0) pozktore.data[xx] = 0;
		Mtmp->data[0] = pozktore.data[xx];
		tmpR1.CloneObject(&proj);
		CutMatrixMatCol(&proj,Mtmp); proj.Transpose();//proj.Dump("c:\\iproj.m");//
		proj.CloneObject(unsorted_proj.celltab[xx]); 
		limit.celltab[xx]->AllocateData(1,1); proj.getMinMax(unused,tmpmax); limit.celltab[xx]->data[0] = prawd_prazka*tmpmax;//limit.Dump("c:\\ilimit.m");//
		proj.CloneObject(&sorted_proj_tmp);	quickSort(sorted_proj_tmp.data,0,sorted_proj_tmp._cols-1);//sorted_proj_tmp.Dump("c:\\isorted_proj_tmp.m");
		GetIndexesAfterSort(&proj,&sorted_proj_tmp,&sorted_index_tmp);//sorted_index_tmp.Dump("c:\\isorted_index_tmp.m");//?
		FIND(sorted_proj_tmp,>limit.celltab[xx]->data[0],poz_wieksze_limit);//poz_wieksze_limit.Dump("c:\\ipoz_wieksze_limit.m");//
		sorted_proj_tmp.CloneObject(sorted_proj.celltab[xx]);
		CutMatrixMatCol(sorted_proj.celltab[xx],&poz_wieksze_limit);//sorted_proj.Dump("c:\\isorted_proj.m");//
		sorted_index_tmp.CloneObject(sorted_index.celltab[xx]);
		CutMatrixMatCol(sorted_index.celltab[xx],&poz_wieksze_limit);//sorted_index.Dump("c:\\isorted_index.m");//?
		proj.CloneObject(unsorted_proj1.celltab[xx]);
		CutMatrixMatCol(unsorted_proj1.celltab[xx],&ktore);//unsorted_proj1.Dump("c:\\iunsorted_proj1.m");//
	}
	delete Mtmp;
	
	k1 = new C_Matrix_Container;
	index_tmp = new C_Matrix_Container; index_tmp->AllocateData(1,Xp1->_cols);
	tmpktore = new C_Matrix_Container;
	index_tmp_tmp = new C_Matrix_Container;
	result = new res*[sorted_proj.length()];
	for(unsigned int a=0;a<sorted_proj.length();a++)	{
		find_peak1(unsorted_proj1.celltab[a],sharp,k1);//k1->Dump("c:\\ik.m");//
		for(u=0;u<Xp1->_cols;u++) index_tmp->data[u] = u;
		for(u=0;u<k1->_cols;u++) k1->data[u] = round(k1->data[u]);
		ktore.CloneObject(tmpktore); CutMatrixMatCol(tmpktore,k1);//tmpktore->Dump("c:\\itmpktore.m");//
		index_tmp->CloneObject(index_tmp_tmp); CutMatrixMatCol(index_tmp_tmp,tmpktore);
	//	index_tmp_tmp->Dump("c:\\iindex_tmp_tmp.m");//
		choose_peak1(index_tmp_tmp,zakres,limit.celltab[a]->data[0],unsorted_proj.celltab[a],tol,&pp);//pp.Dump("c:\\ipp.m"); //
		maxlen = 0;
		index = 0;
		for(u=0;u<pp.length();u++)
			if(pp.celltab[u]!=NULL && pp.celltab[u]->_cols>maxlen)	{
				maxlen = pp.celltab[u]->_cols;
				index = u;
			}
		if(pp.length()<1)
			nierobic = TRUE;	
		else
			k = pp.celltab[index];	//k->Dump("c:\\ik.m");//
		if(nierobic==FALSE)	{
			if(k->data!=NULL)	{
				result[a] = new res;
				result[a]->angle = theta.GetPixel(0,(unsigned long)pozktore.data[a]);
				Xp1->CloneObject(&result[a]->k); CutMatrixMatCol(&result[a]->k,k);//result[a]->k.Dump("c:\\iresk.m");
			//	Xp1->Dump("c:\\ixp1.m");
				result[a]->peakval.AllocateData(1,k->_cols);
			//	R1->DumpBinary("c:\\ir1.m");
				for(u=0;u<k->_cols;u++)
					result[a]->peakval.data[u] = R1->GetPixel((unsigned long)k->data[u],(unsigned long)result[a]->angle);//
			//	result[a]->peakval.Dump("c:\\peak.m");
				if(result[a]->angle>90)
					for(u=0;u<result[a]->k._cols;u++)
						result[a]->k.data[u] = -result[a]->k.data[u];
				result[a]->k.CloneObject(&ktmp);
				for(u=0;u<ktmp._cols;u++)
					ktmp.data[u] = abs(ktmp.data[u]);
				diff(&ktmp,&ktmpdiff);
				for(u=0;u<ktmpdiff._cols;u++)
					ktmpdiff.data[u] = abs(ktmpdiff.data[u]);
				result[a]->estimated_len = mean(&ktmpdiff);//
			} else {
				result[a] = new res;
				result[a]->angle = theta.GetPixel(0,(unsigned long)pozktore.data[a]+1);
				result[a]->k.data = NULL;
			}
			delete k;
		}
		else	{
			result[a] = new res;
			result[a]->angle = theta.GetPixel(0,(unsigned long)pozktore.data[a]+1);
			result[a]->k.data = NULL;
		}
	}
	// dlugosc
	for(unsigned int a=0;a<sorted_proj.length();a++)	{
		if(result[a]->k.data!=NULL)	{
			if(result[a]->angle>90) angle = result[a]->angle-90;	else angle = result[a]->angle+90;//
			rangelow = result[a]->k.data[0]-0.1*(result[a]->k.data[result[a]->k._cols-1] - result[a]->k.data[0]);//
			rangehi = result[a]->k.data[result[a]->k._cols-1]+0.1*(result[a]->k.data[result[a]->k._cols-1] - result[a]->k.data[0]);//
			BWCUT(BW2,angle,rangelow,rangehi,&BW2CUT);//BW2CUT.DumpBinary("c:\\ibw2cut.m");//BW2->DumpBinary("c:\\ibw2.m");
			mojradon(&BW2CUT,1,angle,angle+1,1,&R,0);
//prepareradon(&BW2CUT,setpar);
//R.AllocateData((unsigned int)setpar[3],(unsigned int)setpar[5]); //T,R
//oradon(R.data,BW2CUT.data,setpar);

//			R.DumpBinary("c:\\irad.m");
			p.AllocateData(1,R._rows);//
			for(unsigned int xx=0;xx<R._rows;xx++)//
				p.data[xx] = R.GetPixel(xx,angle);//
		//	p.DumpBinary("c:\\ip.m");
			Normalize(&p,0,1);
		//	p.DumpBinary("c:\\ip.m");
			outoptim = Optim(&p,szer);//
			result[a]->k90.AllocateData(1,2);//
			result[a]->k90.data[0] = Xp.data[outoptim]; result[a]->k90.data[1] = Xp.data[outoptim+(unsigned long)szer];//?
		}
	}

	C_Matrix_Container rt;
	for(unsigned int a=0;a<sorted_proj.length();a++)	{
		if(result[a]!=NULL && result[a]->k.data!=NULL)	{
			result[a]->center.AllocateData(1,2);
			result[a]->center.data[0] = floor((BW2->_rows+1.0)/2.0);
			result[a]->center.data[1] = floor((BW2->_cols+1.0)/2.0);
			result[a]->lin.AllocateData(result[a]->k._cols,2);//
			result[a]->lin90.AllocateData(2,2);
			for(unsigned int b=0;b<result[a]->k._cols;b++)	{
				GetRadonLine(result[a]->angle,result[a]->k.data[b],&result[a]->center,&rt);
				result[a]->lin.SetPixel(b,0,rt.data[0]);result[a]->lin.SetPixel(b,1,rt.data[1]);
			}
		//	result[a]->lin.Dump("c:\\ilin.m");
			GetRadonLine(result[a]->angle+90,result[a]->k90.data[0],&result[a]->center,&rt);
			result[a]->lin90.SetPixel(0,0,rt.data[0]); result[a]->lin90.SetPixel(0,1,rt.data[1]);
			GetRadonLine(result[a]->angle+90,result[a]->k90.data[1],&result[a]->center,&rt);
			result[a]->lin90.SetPixel(1,0,rt.data[0]); result[a]->lin90.SetPixel(1,1,rt.data[1]);
		//	result[a]->lin90.Dump("c:\\ilin90.m");
		}
	}
// niewykryte linie
	for(unsigned int a=0;a<sorted_proj.length();a++)	{
		if(result[a]!=NULL && result[a]->k.data!=NULL)	{
			result[a]->k_estim.AllocateData(1,num_lines-result[a]->k._cols);
			result[a]->lin_estim.AllocateData(num_lines-result[a]->k._cols,2);
			rt.AllocateData(1,2);
			for(unsigned int b=0;b<num_lines-result[a]->k._cols;b++)	{
				sign = (result[a]->k.data[1] - result[a]->k.data[0])/abs(result[a]->k.data[1] - result[a]->k.data[0]);
				if((result[a]->peakval.data[0] - result[a]->peakval.data[result[a]->peakval._cols-1])<0)	{
					kesttmp = result[a]->k.data[0] - sign*(b+1)*result[a]->estimated_len;//
					GetRadonLine(result[a]->angle,kesttmp,&result[a]->center,&rt);
				} else	{
					kesttmp = result[a]->k.data[result[a]->k._cols-1] - sign*(b+1)*result[a]->estimated_len;
					GetRadonLine(result[a]->angle,kesttmp,&result[a]->center,&rt);
				}
				result[a]->k_estim.data[b] = kesttmp;
				result[a]->lin_estim.SetPixel(b,0,rt.data[0]); result[a]->lin_estim.SetPixel(b,1,rt.data[1]); 
			}//b
		}
	}
//	result[0]->k_estim.Dump("c:\\ikestim.m");//
//	result[0]->lin_estim.Dump("c:\\ilinestim.m");//
	unsigned int aa;
	for(unsigned int a=0;a<sorted_proj.length();a++)	{
		if(result[a]!=NULL && result[a]->k.data!=NULL)	{
			linlin90.AllocateData(result[a]->lin._rows+result[a]->lin_estim._rows,2);
			for(aa=0;aa<result[a]->lin._rows;aa++)	{
				linlin90.SetPixel(aa,0,result[a]->lin.GetPixel(aa,0));
				linlin90.SetPixel(aa,1,result[a]->lin.GetPixel(aa,1));
			}
			for(aa=0;aa<result[a]->lin_estim._rows;aa++)	{
				linlin90.SetPixel(aa+result[a]->lin._rows,0,result[a]->lin_estim.GetPixel(aa,0));
				linlin90.SetPixel(aa+result[a]->lin._rows,1,result[a]->lin_estim.GetPixel(aa,1));
			}
			precik.AllocateData(result[a]->lin._rows+result[a]->lin_estim._rows,4);
			for(unsigned int xx=0;xx<result[a]->lin._rows+result[a]->lin_estim._rows;xx++)	{
				precik.SetPixel(xx,0,(result[a]->lin90.GetPixel(0,1)-linlin90.GetPixel(xx,1))/(linlin90.GetPixel(xx,0)-result[a]->lin90.GetPixel(0,0)));
				precik.SetPixel(xx,1,linlin90.GetPixel(xx,0)*precik.GetPixel(xx,0)+linlin90.GetPixel(xx,1));
				precik.SetPixel(xx,2,(result[a]->lin90.GetPixel(1,1)-linlin90.GetPixel(xx,1))/(linlin90.GetPixel(xx,0)-result[a]->lin90.GetPixel(1,0)));
				precik.SetPixel(xx,3,linlin90.GetPixel(xx,0)*precik.GetPixel(xx,2)+linlin90.GetPixel(xx,1));

				precik.SetPixel(xx,0,precik.GetPixel(xx,0)+srodek_x);//kol
				precik.SetPixel(xx,1,precik.GetPixel(xx,1)+srodek_y);
				precik.SetPixel(xx,2,precik.GetPixel(xx,2)+srodek_x);
				precik.SetPixel(xx,3,precik.GetPixel(xx,3)+srodek_y);
			}
			
			precik.CloneObject(&result[a]->preciki);
	//		precik.Dump("C:\\iprecik.m");
		}
	}

	//--------------------------------------------------------------
	C_Matrix_Container &mypr = result[0]->preciki;
	C_Image_Container ttt;
	int ps = 10;
	image_mark.Normalize(0,65535);
	image_mark.CloneObject(&ttt);
	unsigned int row,kol;
	for(row=0;row<floor(image_mark._rows/2.0);row++)
		for(kol=0;kol<image_mark._cols;kol++)	{
			image_mark.SetPixel(row,kol,ttt.GetPixel((ttt._rows-1)-row,kol));
			image_mark.SetPixel((ttt._rows-1)-row,kol,ttt.GetPixel(row,kol));

		}
	for(unsigned int a=0;a<precik._rows;a++)	{
		for(row=mypr.GetPixel(a,1)-ps;row<mypr.GetPixel(a,1)+ps;row++)
			for(kol=mypr.GetPixel(a,0)-ps;kol<mypr.GetPixel(a,0)+ps;kol++)
				image_mark.SetPixel(row,kol,65535);
		for(row=mypr.GetPixel(a,3)-ps;row<mypr.GetPixel(a,3)+ps;row++)
			for(kol=mypr.GetPixel(a,2)-ps;kol<mypr.GetPixel(a,2)+ps;kol++)
				image_mark.SetPixel(row,kol,65535);

	}

	image_mark.CloneObject(&ttt);
	for(row=0;row<floor(image_mark._rows/2.0);row++)
		for(kol=0;kol<image_mark._cols;kol++)	{
			image_mark.SetPixel(row,kol,ttt.GetPixel((ttt._rows-1)-row,kol));
			image_mark.SetPixel((ttt._rows-1)-row,kol,ttt.GetPixel(row,kol));

		}

	outtab[0]->AssignValue((void*)&result[0]->preciki,err);
	outtab[1]->AssignValue((void*)&image_mark,err);
	delete index_tmp_tmp; 
	delete tmpktore;
//	SAFE_DELETE(k); 
	delete index_tmp; 
	for(u=0;u<sorted_proj.length();u++)
		SAFE_DELETE(result[u]);
	delete result;

// --------------------RADON---------------------------------------
//	prepareradon(image,setpar);
//	radon.AllocateData((unsigned int)setpar[3],(unsigned int)setpar[5]); //T,R
//	oradon(radon.data,image->data,setpar);
//
//	mojradon(image,0,0,&out);
// -----------------------------------------------------------------

//	outtab[0]->AssignValue((void*)&result[0]->lin,err);

}

void BWCUT(C_Matrix_Container* BW,double angle,double rangelow, double rangehi,C_Matrix_Container* out)
{
	double srodekx,srodeky,y0,y1;
	double srodek[2];
	double range[2];
	double dif;
	double points[8];
	double edges[8];
	double xp[8];
	double yp[8];
	unsigned long licznik,a;
	unsigned int mrows, ncols, count,x,y;
	double maxx, maxy,ret;
	double p_point[2];
	double *p_x, *p_y;
	C_Matrix_Container outmp;

	srodek[1] = floor((BW->_cols+1)/2.0);
	srodek[0] = floor((BW->_rows+1)/2.0);
	range[0] = rangelow;
	range[1] = rangehi;
	dif = range[1]-range[0];
	y0 = 0;
	y1 = BW->_rows;


	double x0_1 = (y0-srodek[0])/tan((angle)*M_PI/180)+range[0]+srodek[1]-0.03*dif;
	double x1_1 = (y1-srodek[0])/tan((angle)*M_PI/180)+range[0]+srodek[1]-0.03*dif;
	double x0_2 = (y0-srodek[0])/tan((angle)*M_PI/180)+range[1]+srodek[1]+0.03*dif;
	double x1_2 = (y1-srodek[0])/tan((angle)*M_PI/180)+range[1]+srodek[1]+0.03*dif;

	points[0] = x0_1;
	points[1] = y1;
	points[2] = x1_1;
	points[3] = y0;
	points[4] = x1_2;
	points[5] = y0;
	points[6] = x0_2;
	points[7] = y1;

	edges[0] = 1;
	edges[1] = 2;
	edges[2] = 2;
	edges[3] = 3;
	edges[4] = 3;
	edges[5] = 4;
	edges[6] = 4;
	edges[7] = 1;


	xp[0] = points[0];
	xp[1] = points[2];
	xp[2] = points[2];
	xp[3] = points[4];
	xp[4] = points[4];
	xp[5] = points[6];
	xp[6] = points[6];
	xp[7] = points[0];

	yp[0] = points[1];
	yp[1] = points[3];
	yp[2] = points[3];
	yp[3] = points[5];
	yp[4] = points[5];
	yp[5] = points[7];
	yp[6] = points[7];
	yp[7] = points[1];

	maxx = BW->_cols;
	maxy = BW->_rows;

	p_x = xp;
	p_y = yp;
	outmp.AllocateData(BW->_rows,BW->_cols);
	licznik = 0;
	ncols = 8;
	for(x=0;x<maxx;x++)
		for(y=0;y<maxy;y++)	{
			count = 0;
			p_point[0] = x;
			p_point[1] = y;
			for(a=0;a<ncols;a+=2)
				count+=prvTestInside(p_point,p_x[a],p_y[a],p_x[a+1],p_y[a+1]);
			ret = (count % 2)==1;
			outmp.SetPixel(y,x,ret);
		}
		BW->CloneObject(out);
		for(x=0;x<out->GetNumofElements();x++)
			out->data[x]*=outmp.data[x];
//	tmp = isinsideC(xp,yp,BW->_cols,BW->_rows);


}

int prvTestInside(double* p_point, double p0_x, double p0_y, double p1_x, double p1_y)
{
	double tmp_x, tmp_y, t, xp;
	if(p1_y==p0_y)
		return 0;
	if(p0_y>p1_y)	{
		tmp_x = p0_x;
		tmp_y = p0_y;
		p0_x = p1_x;	p0_y = p1_y;
		p1_x = tmp_x;	p1_y = tmp_y;
	}
	if(p0_y<p_point[1] && p1_y<=p_point[1])
		return 0;
	if(p0_y>p_point[1] && p1_y>p_point[1])
		return 0;
	if(p0_x<p_point[0] && p1_x<p_point[0])
		return 0;
	if(p0_x>=p_point[0] && p1_x>=p_point[0])
		return 1;
	t = (p_point[1] - p0_y)/(p1_y-p0_y);
	xp = p0_x + t*(p1_x-p0_x);
	if(xp>=p_point[0])
		return 1;
	else
		return 0;

}

unsigned long Optim(C_Matrix_Container* line,double szer)
{
	unsigned long i,v,poz;
	double suma;
	double maks = 1e66;
	for(i=0;i<line->_cols-szer;i++)	{
		suma = 0.0;
		for(v=i;v<i+szer;v++)
			suma+=pow(line->data[v] - 1.0,2);
		if(suma<maks)	{
			maks = suma;
			poz = i;
		}
	}
	return poz;
}

void GetRadonLine(double alpha,double k,C_Matrix_Container* srodek,C_Matrix_Container* out)
{

	double ap,x1,a,b;
	if(alpha>90)
		alpha-=180;
	ap = alpha+90;
	if(alpha<90)	{
		x1 = k/cos((90-alpha)*M_PI/180);
		a = tan((alpha+90)*M_PI/180);
		b = x1;
	} else
		if(alpha>90 && alpha<180)	{
			x1 = k*sin(alpha*M_PI/180);
			a = tan((alpha)*M_PI/180);
			b = x1;
		} else
			if(alpha==90 || alpha==0 || alpha==180)	{
				a = tan(ap*M_PI/180);
				b = k;
			}
	out->AllocateData(1,2);
	out->data[0] = a;
	out->data[1] = b;
}

void mad(C_Matrix_Container* input,C_Matrix_Container* output)
{
	// liczy mad dla dla wszystki kolumn. Je�li tylko jeden rz�d to liczy dla tego rz�du
	double med;
	unsigned long a,r,c;
	double* ttmp;
	C_Matrix_Container tmp;
	input->CloneObject(&tmp);
	if(input->_rows==1)	{
		med = median(tmp.data,tmp._cols);
		for(a=0;a<tmp._cols;a++)	{
			tmp.data[a]-=med;
			tmp.data[a] = abs(tmp.data[a]);
		}
		output->AllocateData(1,1);
		output->data[0] = median(tmp.data,tmp._cols);
	}
	if(input->_rows>1)	{
		ttmp = new double[input->_rows];
		output->AllocateData(1,input->_cols);
		for(c=0;c<input->_cols;c++)	{
			for(r=0;r<input->_rows;r++)
				ttmp[r] = input->GetPixel(r,c);
			med = median(ttmp,input->_rows);
			for(a=0;a<input->_rows;a++)	{
				ttmp[a]-=med;
				ttmp[a] = abs(ttmp[a]);
			}
			output->SetPixel(0,c,median(ttmp,input->_rows));
		}
		delete[] ttmp;
	}
}

double median(double* data,unsigned long size)
{
	quickSort(data,0,size-1);
	if(size%2!=0)
		return data[(unsigned long)ceil(size/2.0)];
	else
		return (data[size/2] + data[size/2+1])/2;
}

void findonevalue(C_Matrix_Container* input,C_Matrix_Container* output,double wartosc)
{
	// znajduje jedn� warto�� w macierzy. Zwraca macierz 2xn z indeksami znalezionej warto�ci 0xn rz�dy
	unsigned long r,k,licznik=0;
	output->AllocateData(2,iselement(input,wartosc));
	for(r=0;r<input->_rows;r++)
		for(k=0;k<input->_cols;k++)
			if(input->GetPixel(r,k)==wartosc)	{
				output->SetPixel(0,licznik,(double)r);
				output->SetPixel(1,licznik,(double)k);
				licznik++;
			}
}
void mojradon(C_Matrix_Container* image, int katstep, int katstart,int katstop,unsigned long kstep, C_Matrix_Container* out,BOOL ext)
{
	unsigned long przekatna,licznik = 0,r,k,liczkat=0;
	int kat;
	C_Matrix_Container tmp,kopia;
	image->CloneObject(&kopia);
	double c,s,new_x,new_y,x,y,suma=0,srodek_x,srodek_y,maksp;
	if(image->_cols>image->_rows)
		maksp = image->_cols;
	else
		maksp = image->_rows;
//	przekatna = (unsigned long)ceil(sqrt(pow(maksp,2)+pow(maksp,2)));
	przekatna = (unsigned long)ceil(sqrt(pow(image->_cols,2.0)+pow(image->_rows,2.0)));
	if(ext==0)	{
		out->AllocateData(przekatna,180);
		out->Zeros();
	}
//	obrot.AllocateData(przekatna,przekatna);
	
	srodek_x = floor(przekatna/2.0);
	srodek_y = floor(przekatna/2.0);
	Normalize(&kopia,0,65535);
	for(kat=180-katstop;kat<180-katstart;kat++)	{
		AllocAndRotate(&kopia,kat,0,&tmp);
//		obrot.Zeros();
//		for(r=0;r<tmp._rows;r++)
//			for(k=0;k<tmp._cols;k++)
//				obrot.SetPixel(r+srodek_y-round(tmp._rows/2.0),k+srodek_x-round(tmp._cols/2.0),tmp.GetPixel(r,k));


//		tmp.DumpBinary("c:\\obr.m");
		licznik = 0;
		for(x=0;x<tmp._cols;x++)	{
			suma = 0.0;
			for(y=0;y<tmp._rows;y++)	
				suma+=tmp.GetPixel(y,x);
			out->SetPixel(out->_rows-((unsigned long)x+srodek_x-round(tmp._cols/2.0))-1,180-kat-1,suma/65535);
		}		
	}		
}

double round(double liczba)
{
	if( abs(liczba)-abs(floor(liczba))<0.5)
		return floor(liczba);
	else
		return ceil(liczba);
}
void prepareradon(C_Matrix_Container* image, double* setpar)
{
	int M, T, R;
	double Delta_x, x_min, Delta_theta, rho_min, Delta_rho, theta_min;

	M = image->_cols;
	Delta_x = 1.0;
	x_min = -(M-1)*0.5*Delta_x;
	T = (int)ceil(M_PI*(M-1));
	Delta_theta=M_PI/T;
	Delta_rho = Delta_x/sqrt(2.0);
	R = 2*M-1;
	rho_min = -Delta_rho*(R-1)*0.5;
	theta_min = 0;
	setpar[0]  =      M;     
	setpar[1]  =      x_min; 
	setpar[2]  =      Delta_x;	
	setpar[3]  =      T;      //rz�dy out
	setpar[4]  =      Delta_theta;  
	setpar[5]  =      R;       // kolumny out
	setpar[6]  =      rho_min;
	setpar[7]  =      Delta_rho;
	setpar[8]  =      theta_min;

}

double sinc(double x)
{

  if (x<0) x=-x;
  if (x<1e-10) 
    return 1.0;
  else
    return sin(x)/x;
}

void oradon(double *out,double *in,double *setpar)
{
  int m,n, M;
  int r, R;
  int t, T;
  double sum,x_min,Delta_x,Delta_rho,rho_min,Delta_theta;
  double costheta,sintheta,theta;
  double rho,rhooffset;
  double theta_min;
  double idx,mintrig,psi;
  double *xar,scal;
  double pierwiastek = sqrt(0.5);

  M            = (int)setpar[0];
  x_min        = setpar[1];
  Delta_x      = setpar[2];
  T            = (int)setpar[3];
  Delta_theta  = setpar[4];
  R            = (int)setpar[5];
  rho_min      = setpar[6];
  Delta_rho    = setpar[7];
  theta_min    = setpar[8];

  scal=M_PI/Delta_x;
  xar=(double *)malloc(M*sizeof(double));

  for ( m = 0 ; m < M ; m++ ) 
    xar[m]=m*Delta_x+x_min;

  idx=1.0/Delta_x;
  for(t=0; t<T;t++)
  {
//    printf("t=%i (%i)\n",t,T);
    theta=t*Delta_theta+theta_min;
    sintheta=sin(theta);
    costheta=cos(theta);
    rhooffset=x_min*(costheta+sintheta);
    if (sintheta>pierwiastek)
      mintrig=1.0/sintheta;
    else
      mintrig=1.0/fabs(costheta);

    for(r=0; r<R; r++ )
    {
      rho=r*Delta_rho+rho_min;
      sum=0.0;
      for (m=0;m<M;m++)
      {
        rhooffset=rho-xar[m]*costheta;
        for (n=0;n<M;n++)
        {
          psi=scal*(rhooffset-xar[n]*sintheta);
          sum+=sinc(mintrig*psi)*in[m+n*M];
        }
      }
      out[t+T*r]+=sum*mintrig*Delta_x;
    }
  }
  free(xar);
}
void RemoveMatrixCol(C_Matrix_Container* input, unsigned int start, unsigned int stop)
{
	// usuwa kolumny pomiedzy start i stop tak jak []
	unsigned int sstart,sstop;
	unsigned long actr,actc,column=0;
	double pixel;
	C_Matrix_Container l,p;
	BOOL lewa,prawa;
	if(start == 0)	{
		sstart = 0;
		lewa = FALSE;
	}
	else	{
		sstart = start-2; if(sstart<0) sstart=0;
		lewa = TRUE;
		input->CloneObject(&l);
	}
	if(stop == input->_cols)	{
		sstop = input->_cols;
		prawa = FALSE;
	}
	else	{
		sstop = stop;
		prawa = TRUE;
		input->CloneObject(&p);
	}
	if(lewa)
		CutMatrixCol(&l,0,sstart);
	if(prawa)
		CutMatrixCol(&p,sstop,p._cols-1);
	if(lewa & prawa)	{
		input->AllocateData(l._rows,l._cols+p._cols);
		for(actc=0;actc<l._cols;actc++)	{
			for(actr=0;actr<l._rows;actr++)
				input->SetPixel(actr,column,l.GetPixel(actr,actc));
			column++;
		}
		for(actc=0;actc<p._cols;actc++)	{
			for(actr=0;actr<p._rows;actr++)
				input->SetPixel(actr,column,p.GetPixel(actr,actc));
			column++;
		}
	}
	else
		if(prawa)
			p.CloneObject(input);
		else
			l.CloneObject(input);		
}
void CutMatrixCol(C_Matrix_Container* input, unsigned int start, unsigned int stop)
{
// wycina po kolumnach od start do stop. Modyfikuje strukture we�ciow�. Zostaj� te wyci�te np (1:4)
	unsigned int c,r;
	double pixel;
	unsigned long column=0;
	C_Matrix_Container tmp;
	tmp.AllocateData(input->_rows,stop-start+1);
	for(c=start;c<=stop;c++)	{
		for(r=0;r<input->_rows;r++)	{
			input->GetPixel(r,c,pixel);
			tmp.SetPixel(r,column,pixel);
		}
		column++;
	}
	input->FreeData();
	tmp.CloneObject(input);
}

void CutMatrixMatCol(C_Matrix_Container* input, C_Matrix_Container* kolumny)
{
// wycina z input kolumny o indeksach kolumny
	C_Matrix_Container tmp;
	unsigned long r,c,kol;
	if(kolumny->_cols!=1 && kolumny->_rows!=1)
		return;
	if(kolumny->_cols==1)
		kol = kolumny->_rows;
	if(kolumny->_rows==1)
		kol = kolumny->_cols;
	tmp.AllocateData(input->_rows,kol);
	for(c=0;c<kol;c++)
		for(r=0;r<input->_rows;r++)
			tmp.SetPixel(r,c,input->GetPixel(r,(unsigned long)kolumny->data[c]));
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
			srednia+=input->GetPixel(a,col);
		srednia/=numofelem;
	
		for(a=0;a<numofelem;a++)
			suma+=pow( (input->GetPixel(a,col) - srednia),2 );
		suma = sqrt(suma/(numofelem-1));
		output->SetPixel(0,col,suma);
	}
}

double mean(C_Matrix_Container* input)
{
// oblicza sredni� po wektorze
	unsigned long col;
	double suma=0,srednia=0;
	for(col=0;col<input->_cols;col++)
		srednia+=input->data[col];
	srednia/=input->GetNumofElements();
	return srednia;
}

void dividetrend(C_Matrix_Container* input1,Cell* output)
{
	C_Matrix_Container input;
	unsigned long a,length,licznik=0,start,b;
	double stop;
	C_Matrix_Container g;
	C_Matrix_Container pozw;
	input1->CloneObject(&input);
	length = input._cols;	// dlugosc wektora wejsciowego
	quickSort(input.data,0,length-1);
	diff(&input,&g);
	FIND(g,>1,pozw);
	start = 0;
	output->CreateCell(pozw.GetNumofElements()+1);
	for(a=0;a<pozw.GetNumofElements();a++)	{
		stop = pozw.data[a];
		output->celltab[a]->AllocateData(1,(unsigned int)(stop-start+1));
		for(b=start;b<=stop;b++)
			output->celltab[a]->data[licznik++] = input.data[b];
		start = (unsigned int)stop + 1;
		licznik = 0;
	}
	if(pozw.data[--a]<length-1)	{
		output->celltab[a+1]->AllocateData(1,length - (unsigned int)(pozw.data[a]+1));
		for(b=(unsigned int)pozw.data[a]+1;b<length;b++)	{
			_ASSERT(b<input._cols);
			output->celltab[a+1]->data[licznik++] = input.data[b];
		}
		licznik = 0;
	}
	else	{
		if(pozw.data[a]==length-1)	{
			output->celltab[a+1]->AllocateData(1,1);
			_ASSERT((int)pozw.data[a]<input._cols);
			output->celltab[a+1]->data[0] = input.data[(int)pozw.data[a]];
		}
	}

//	output->DeletePart(a+2,pozw.GetNumofElements());

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
	quickSort(tmp.data,0,tmp._cols-1);
	for(a=0;a<tmp._cols-1;a++)
		if(tmp.data[a+1]!=tmp.data[a])
			ilosc++;
	output->AllocateData(1,ilosc);
	output->data[licznik++] = tmp.data[0];
	for(a=0;a<tmp._cols-1;a++)
		if(tmp.data[a+1]!=tmp.data[a])
			output->data[licznik++] = tmp.data[a+1];
}
void GetIndexesAfterSort(C_Matrix_Container* unsorted, C_Matrix_Container* sorted, C_Matrix_Container* output)
{
	// zwraca indeksy - odpowiada: [sorted,output] = sort(unsorted)
	output->AllocateData(1,sorted->_cols);
	unsigned long a,b,start=0,licznik=0;
	C_Matrix_Container uns;
	unsorted->CloneObject(&uns);
	for(b=0;b<sorted->_cols;b++)
		for(a=0;a<sorted->_cols;a++)
			if(uns.data[a]==sorted->data[b])	{
				output->data[licznik++] = a;
				uns.data[a] = 1e306;
				break;
			}
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
	C_Matrix_Container diffret;
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
		CutMatrixCol(&cut_data,start,start+(unsigned int)window-1);
		std(cut_data_f,&indicator);
// ------ wewn�trz okna ----------------
		peak_number = 1;
		cut_data.getMinMax(min,max);
		// find
		FIND(cut_data,==max,ss);
		// end of find
		out.data[licznik++] = ss.data[0]+start;
	}
	CutMatrixCol(&out,0,--licznik);
	licznik = 0;
	diff(&out,&diffret);
	
	FIND(diffret,==0,z);	// sprawdzone

	licznik = 0;
	dividetrend(&z,&zz);
	output->AllocateData(1,zz.length());
	for(a=0;a<zz.length();a++)
		if(zz.celltab[a]->_cols==window-1)
			output->data[licznik++] = out.data[(int)zz.celltab[a]->data[0]];
	CutMatrixCol(output,0,--licznik);
/*
for start=1:l_data-window
    cut_data = data(start:start+window-1);
    if window<3*forder
        cut_data_f = cut_data;
    else
        cut_data_f = filtfilt(b,a,cut_data);
    end 
    
    indicator = std(cut_data);
% ------ wewn�trz okna ----------------

%     peak_number = 1;
    ss=find(cut_data==unique(max(cut_data)));
    out(licznik) = ss(1)+start-1;
    licznik= licznik + 1;
% po tej operacji je�li rozmiar okna b�dzie zblizony do rozmiaru okna to w
% out pojawi� si� stale fragmenty przebiegu (bo podczas przesuwania okna
% b�dzie dominowal ten sam pik, rozmiar okna powinien byc taki aby nie by�o
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
	// usuwa element z tablicy zmieniaj�c jej rozmiar. Dziala tylko na wektorach
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
	C_Matrix_Container projkand;
	Cell *kand = out;
	double suma=0;
	

	zakres[0] = round(odl + (-tol*odl));
	zakres[1] = round(odl + (tol*odl));
	diff(in,&din);
	kand->CreateCell(din._cols);
	for(unsigned long cc=0;cc<din._cols;cc++)
		kand->celltab[cc]->AllocateData(1,(in->_cols-2)*2+2);
	for(a=0;a<din._cols;a++)	{
		if( (din.data[a] >=zakres[0])&&(din.data[a]<=zakres[1]))	{
			kand->celltab[c]->data[licznik++] = in->data[a];
			kand->celltab[c]->data[licznik++] = in->data[a+1];
		} else {
			if(licznik==0){
				SAFE_DELETE(kand->celltab[c]);}
			else{
				RemoveMatrixCol(kand->celltab[c],licznik,din._cols);}
			c++;
			licznik = 0;
		}
	}
	kand->DeletePart(c+=1,din._cols);	// c albo --c ???
	kand->Dump("c:\\ikand.m");
	in->Dump("c:\\iin.m");//
	for(a=0;a<kand->length();a++)	{
		if(kand->celltab[a]!=NULL)	{
			proj->CloneObject(&projkand);
			CutMatrixMatCol(&projkand,kand->celltab[a]);
			FIND(projkand,<limit,zz);
			if(zz._cols>0)
				RemoveIndexes(kand->celltab[a],&zzd);
			if(kand->celltab[a]->data!=NULL)	{
				kand->Dump("c:\\ikand.m");
				unique(kand->celltab[a],&utmp);
				utmp.CloneObject(kand->celltab[a]);
			}
		}

		
		//skonczy�
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
	if(min==max)
		return;
	if(min==w1 && max==w2)
		return;				// bez normalizacji
	delta = abs(w2-w1);
	for(r=0;r<input->GetNumofElements();r++)
		input->data[r] = delta*input->data[r]/(max-min)-min*delta/(max-min);
}

void diff(C_Matrix_Container* in,C_Matrix_Container* out)
{
	// diff po kolumnach dla jednego rzedu
	unsigned long a;
	out->AllocateData(1,in->_cols-1);
	for(a=0;a<in->_cols-1;a++)
		out->data[a] = in->data[a+1] - in->data[a];
}

void HorizSkew (
    C_Matrix_Container *pSrc, 
    C_Matrix_Container *pDst, 
    UINT uRow, 
    int iOffset, 
    double dWeight,
    double clrBack)
{
	int i;
	for (i = 0; i < iOffset; i++)
    {
        // Fill gap left of skew with background
		pDst->SetPixel(uRow,i,clrBack);
    }
	double pxlOldLeft = clrBack;
	for (i = 0; i < pSrc->_cols; i++) 
    {
        // Loop through row pixels
		double pxlSrc = pSrc->GetPixel(uRow,i);
        // Calculate weights
        double pxlLeft = pxlSrc*dWeight;
        // Update left over on source
        pxlSrc = pxlSrc - (pxlLeft - pxlOldLeft);
        // Check boundries 
		if ((i + iOffset >= 0) && (i + iOffset < (int)pDst->_cols))
        {
			pDst->SetPixel(uRow,i+iOffset, pxlSrc);
        }
        // Save leftover for next pixel in scan
        pxlOldLeft = pxlLeft;
    }
	 // Go to rightmost point of skew
    i += iOffset;  
	if (i < pDst->_cols)
    {
        // If still in image bounds, put leftovers there
		pDst->SetPixel(uRow,i, pxlOldLeft);
    }
	while (++i < pDst->_cols)
    {   // Clear to the right of the skewed line with background
        pDst->SetPixel(uRow,i, clrBack);
    }
}

void VertSkew (
    C_Matrix_Container *pSrc, 
    C_Matrix_Container *pDst, 
    UINT uCol,
    int iOffset,
    double dWeight,
    double clrBack)
/*------------------------------------------------------------------------------

  Function: VertSkew

  Purpose:  Skews a column vertically (with filtered weights)

  Input:    Image to skew (+dimensions)
            Destination of skewed image (+dimensions)
            Column index
            Skew offset
            Relative weight of upper pixel
            Background color

  Output:   None.

  Remarks:  Limited to 45 degree skewing only. Filters two adjacent pixels.

------------------------------------------------------------------------------*/
{
    int i;
//	if(iOffset>4000)
//		return;
	for (i = 0; i < iOffset; i++)
    {
        // Fill gap above skew with background
		pDst->SetPixel(i,uCol,clrBack);
    }

    double pxlOldLeft = clrBack;

    int iYPos;
	for (i = 0; i < pSrc->_rows; i++) 
    {
        // Loop through column pixels
		double pxlSrc = pSrc->GetPixel(i,uCol);
        iYPos = i + iOffset;
        // Calculate weights
        double pxlLeft = pxlSrc * dWeight;
        // Update left over on source
        pxlSrc = pxlSrc - (pxlLeft - pxlOldLeft);
        // Check boundries
		if ((iYPos >= 0) && (iYPos < (int)pDst->_rows))
        {
			pDst->SetPixel(iYPos,uCol,pxlSrc);
        }
        // Save leftover for next pixel in scan
        pxlOldLeft = pxlLeft;
    }
    // Go to bottom point of skew
    i = iYPos;  
	if (i < pDst->_rows)
    {
        // If still in image bounds, put leftovers there
        pDst->SetPixel(i,uCol,pxlOldLeft); 
    }
	while (++i < pDst->_rows)
    {
        // Clear below skewed line with background
        pDst->SetPixel(i,uCol,clrBack);
    }
}   // CRotateByShear::VertSkew

void Rotate90(C_Matrix_Container *pSrc, C_Matrix_Container* out)//SIZE sSrc, SIZE *psDst)
/*------------------------------------------------------------------------------

  Function: Rotate90

  Purpose:  Rotates an image by 90 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{

	out->AllocateData(pSrc->_cols,pSrc->_rows);
	out->Zeros();
    
	for (UINT uY = 0; uY < UINT(pSrc->_rows); uY++)
    {
		for (UINT uX = 0; uX < UINT(pSrc->_cols); uX++)
        {
			out->SetPixel(out->_rows - uX - 1, uY, pSrc->GetPixel(uY,uX));// SetRGB (pDst, *psDst, uY, (*psDst).cy - uX - 1, GetRGB (pSrc, sSrc, uX, uY));
        }
    }

}   // CRotateByShear::Rotate90

void Rotate180(C_Matrix_Container *pSrc, C_Matrix_Container* out)
/*------------------------------------------------------------------------------

  Function: Rotate180

  Purpose:  Rotates an image by 180 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{

	out->AllocateData(pSrc->_rows,pSrc->_cols);

	for (UINT uY = 0; uY < UINT(pSrc->_rows); uY++)
    {
		for (UINT uX = 0; uX < UINT(pSrc->_cols); uX++)
        {
			out->SetPixel(out->_rows - uY - 1, out->_cols - uX - 1, pSrc->GetPixel(uY,uX));// SetRGB (pDst, *psDst, (*psDst).cx - uX - 1, (*psDst).cy - uY - 1, GetRGB (pSrc, sSrc, uX, uY));
        }
    }
}   // CRotateByShear::Rotate180

void Rotate270(C_Matrix_Container *pSrc, C_Matrix_Container* out)
/*------------------------------------------------------------------------------

  Function: Rotate270

  Purpose:  Rotates an image by 270 degrees (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size

  Output:   Pointer to newly allocated rotated image

  Remarks:  Precise rotation, no filters required.

------------------------------------------------------------------------------*/
{
    out->AllocateData(pSrc->_cols,pSrc->_rows);

    for (UINT uY = 0; uY < UINT(pSrc->_rows); uY++)
    {
        for (UINT uX = 0; uX < UINT(pSrc->_cols); uX++)
        {
			out->SetPixel(uX, out->_cols - uY - 1, pSrc->GetPixel(uY,uX)); //SetRGB (pDst, *psDst, (*psDst).cx - uY - 1, uX, GetRGB (pSrc, sSrc, uX, uY));
        }
    }
}   // CRotateByShear::Rotate270

void Rotate45  (
    C_Matrix_Container *pSrc, 
    double dAngle,
    double clrBack,
    BOOL bMidImage,
	C_Matrix_Container* out)
/*------------------------------------------------------------------------------

  Function: Rotate45

  Purpose:  Rotates an image by a given degree in range [-45.0 .. +45.0]
            (counter clockwise)

  Input:    Image to rotate (+dimensions)
            Pointer to destination size
            Degree of rotation
            Background color
            Was middle image used (for correct progress report)

  Output:   Pointer to newly allocated rotated image

  Remarks:  Using the 3-shear technique.

------------------------------------------------------------------------------*/
{
	unsigned int u,ttt;   
	double dShear;
	double dRadAngle;
	double dSinE;
	double dTan;
	if (0.0 == dAngle)
    {
        // No rotation at all
		pSrc->CloneObject(out);
		return;
    }
#define ROTATE_PI  double (3.1415926535897932384626433832795)
    dRadAngle = dAngle * ROTATE_PI / double(180); // Angle in radians
    dSinE = sin (dRadAngle);
    dTan = tan (dRadAngle / 2.0);

    // Calc first shear (horizontal) destination image dimensions 
    unsigned long sDst1_cx,sDst1_cy;
	sDst1_cx = pSrc->_cols + int(double(pSrc->_rows) * fabs(dTan));
	sDst1_cy = pSrc->_rows;


	C_Matrix_Container pDst1;
    /******* Perform 1st shear (horizontal) ******/

    // Allocate image for 1st shear
	pDst1.AllocateData(sDst1_cy,sDst1_cx);
	pDst1.Zeros();
    
	for (u = 0; u < UINT(pDst1._rows); u++) 
    {  
        if (dTan >= 0.0)
        {
            // Positive angle
            dShear = ((double)u + 0.5) * dTan;
        }
        else
        {
            // Negative angle
			dShear = ((double)((int)u - (int)pDst1._rows) + 0.5) * dTan;
        }
        int iShear = int (floor (dShear));
        HorizSkew ( pSrc, 
                    &pDst1, 
                    u, 
                    iShear, 
                    dShear - double(iShear),
                    clrBack);
    }

    /******* Perform 2nd shear  (vertical) ******/

    // Calc 2nd shear (vertical) destination image dimensions
    unsigned long sDst2_cx,sDst2_cy;
	sDst2_cx = sDst1_cx;
	sDst2_cy = (double (pSrc->_cols) * fabs (dSinE) + double (pSrc->_rows) * cos (dRadAngle)) + 1;
	C_Matrix_Container pDst2;
    // Allocate image for 2nd shear
	pDst2.AllocateData(sDst2_cy,sDst2_cx);
	pDst2.Zeros();
    
    double dOffset;     // Variable skew offset
    if (dSinE > 0.0)
    {   
        // Positive angle
		dOffset = double ((int)pSrc->_cols - 1) * dSinE;
    }
    else
    {
        // Negative angle
		dOffset = -dSinE * double ((int)pSrc->_cols - (int)pDst2._cols);
    }

	for (u = 0; u < UINT(pDst2._cols); u++, dOffset -= dSinE) 
    {
        int iShear = int (floor (dOffset));
        VertSkew (  &pDst1, 
                    &pDst2,
                    u,
                    iShear,
                    dOffset - double(iShear),
                    clrBack);
    }

    /******* Perform 3rd shear (horizontal) ******/

    // Free result of 1st shear

    // Calc 3rd shear (horizontal) destination image dimensions
	unsigned long sDst3_cx,sDst3_cy;
	sDst3_cx = UINT (double(pSrc->_rows) * fabs (dSinE) + double(pSrc->_cols) * cos (dRadAngle)) + 1;
	sDst3_cy = sDst2_cy;
	out->AllocateData(sDst3_cy,sDst3_cx);
	out->Zeros();
    
    if (dSinE >= 0.0)
    {
        // Positive angle
		dOffset = double((int)pSrc->_cols - 1) * dSinE * -dTan;
    }
    else 
    {
        // Negative angle
		dOffset = dTan * (double((int)pSrc->_cols - 1) * -dSinE + double(1 - (int)out->_rows));
    }
    for (u = 0; u < UINT(out->_rows); u++, dOffset += dTan)
    {
        int iShear = int (floor(dOffset));
        HorizSkew ( &pDst2,
                    out,
                    u,
                    iShear,
                    dOffset - double (iShear),
                    clrBack
                  );
            }
    
}   // CRotateByShear::Rotate45

void AllocAndRotate (  
    C_Matrix_Container       *pSrc, 
    double      dAngle,
    double    clrBack,
	C_Matrix_Container *pDst
)
/*------------------------------------------------------------------------------

  Function: AllocAndRotate

  Purpose:  Rotates an image by a given degree

  Input:    Image to rotate (+dimensions)
            Angle of rotation
            Pointers to dimensions of rotated image
            Background color

  Output:   Pointer to newly allocated rotated image

  Remarks:  Angle is unlimited. 3-shears technique is used.

------------------------------------------------------------------------------*/
{
	BOOL test=FALSE;
    C_Matrix_Container pMidImg;
	pSrc->CloneObject(&pMidImg);

    while (dAngle >= 360.0)
    {
        // Bring angle to range of (-INF .. 360.0)
        dAngle -= 360.0;
    }
    while (dAngle < 0.0)
    {
        // Bring angle to range of [0.0 .. 360.0) 
        dAngle += 360.0;
    }
    if ((dAngle > 45.0) && (dAngle <= 135.0)) 
    {
        // Angle in (45.0 .. 135.0] 
        // Rotate image by 90 degrees into temporary image,
        // so it requires only an extra rotation angle 
        // of -45.0 .. +45.0 to complete rotation.
        Rotate90 (pSrc, &pMidImg);
        dAngle -= 90.0;
		test=TRUE;
    }
    else if ((dAngle > 135.0) && (dAngle <= 225.0)) 
    { 
        // Angle in (135.0 .. 225.0] 
        // Rotate image by 180 degrees into temporary image,
        // so it requires only an extra rotation angle 
        // of -45.0 .. +45.0 to complete rotation.
        Rotate180 (pSrc, &pMidImg);
        dAngle -= 180.0;
		test=TRUE;
    }
    else if ((dAngle > 225.0) && (dAngle <= 315.0)) 
    { 
        // Angle in (225.0 .. 315.0] 
        // Rotate image by 270 degrees into temporary image,
        // so it requires only an extra rotation angle 
        // of -45.0 .. +45.0 to complete rotation.
        Rotate270 (pSrc, &pMidImg);
        dAngle -= 270.0;
		test=TRUE;
    }

    // If we got here, angle is in (-45.0 .. +45.0]


    Rotate45 ( &pMidImg,
                            dAngle,
                            clrBack,
                            test,
							pDst);

}   // AllocAndRotate