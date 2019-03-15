#include "Cpp_Lib.h"

// This is an empty project to build DLL library of interface version 2.0
#include <windows.h>
#include <process.h>
#include <crtdbg.h>
#include <math.h>

HANDLE hThread1[8];

struct E	{
	int MaskRows;
	int MaskCols;
	C_Image_Container* dst;
	C_Image_Container* scr;
};

E EX[8];

void  func(void *data);
void quickSort(double *Array,int *ArrayIndex, int array_size);
void q_sort(double *Array,int *ArrayIndex, int left, int right);
bool quickSortfast(double *arr, unsigned int elements);
bool quickSortfastint(int *arr, unsigned int elements);

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"fastmedianmp");	// nazwa w ADDIP
	strcpy_s(definition->version,MAX_VER,"1.3");	// wersja (nie ma wiêkszego znaczenia)
	strcpy_s(definition->fdate,MAX_DATE,__DATE__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->ftime,MAX_TIME,__TIME__);	// data (nie ma wiêkszego znaczenia)
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = fastmedianmp(IMAGE,NUMBER,NUMBER,NUMBER)"); // SYNTAX (nie ma wiêkszego znaczenia)
	definition->numofout = 1;	// ilosc parametrów wyœciowych
	definition->numofin = 4;	// ilosc parametrów wejœciowych
	definition->typesofin[0] = IMAGE;	// typy wejœcia
	definition->typesofout[0] = IMAGE; definition->typesofout[1] = NUMBER; definition->typesofout[2] = NUMBER; definition->typesofout[3] = NUMBER; // typy wyjœcia
	definition->interface_version = 2;	// wersja interface
	strcpy_s(definition->description,MAX_DESC,"Developed by PB");	// cokolwiek
	definition->ADDIP_version = 2.0; 

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void AssignGlobals(C_Variables *(*p_globals)[MAX_GLOBALS],int* p_lastglobal)
{
	globals = p_globals;
	lastglobal = p_lastglobal;	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Image_Container *p_input,*srctmp;
	C_Matrix_Container *p_MaskRows, *p_MaskCols;
	C_Matrix_Container* global_proc;

	C_Image_Container *SRC[maxthreads];
	C_Image_Container *DST[maxthreads];
	unsigned int START[maxthreads];
	unsigned int STOP[maxthreads];
	unsigned int DELTA[maxthreads];
	unsigned int suma;
	BOOL czytranspose = false;

	int MaskRows, MaskCols;
	unsigned int r,k,k1,delta;
	char thread;
	C_Image_Container ImageCopy;

	p_input = (C_Image_Container*)intab[0]->data;
	p_MaskRows = (C_Matrix_Container*)intab[2]->data;
	p_MaskCols = (C_Matrix_Container*)intab[3]->data;

	MaskRows = (int)p_MaskRows->data[0];
	MaskCols = (int)p_MaskCols->data[0];
	char NUMOFPROC = 1;

// sprawdzanie czy istnieje zmienna globalna numofproc (ustawiana przez funkcje Threads)
	int th;
	for(th=0;th<*(lastglobal);th++)
		if((*globals)[th]!=NULL)
			if(strcmp("numofproc",(*globals)[th]->name)==0)	{
				global_proc = (C_Matrix_Container*)(*globals)[th]->data;
				NUMOFPROC = (char)global_proc->data[0];
			}
//-------------------------------------------------------------------------------------

	srctmp = new C_Image_Container;
	p_input->CloneObject(srctmp);

	if(srctmp->_rows>srctmp->_cols && NUMOFPROC!=1)	{
		srctmp->Transpose();
		czytranspose = true;
	}

	for(thread=0;thread<NUMOFPROC;thread++)	{
		DST[thread] = new C_Image_Container;
		SRC[thread] = new C_Image_Container;
	}
//	srctmp->Normalize(0,65535);
	DST[0]->AllocateData(srctmp->_rows,srctmp->_cols);

	suma = 0;
	delta = (unsigned int)floor(srctmp->_cols/(double)NUMOFPROC);
	for(thread=0;thread<NUMOFPROC-1;thread++)	{
		DELTA[thread] = delta;
		suma+=delta;
	}
	DELTA[thread] = srctmp->_cols - suma;

	START[0] = 0;
	STOP[NUMOFPROC-1] = srctmp->_cols-1;
	for(thread=0;thread<NUMOFPROC;thread++)	{
		if(thread>0)
			START[thread] = START[thread-1] + DELTA[thread-1];
		if(thread<NUMOFPROC)
			STOP[thread] = START[thread]+DELTA[thread]-1;
	}

	if(NUMOFPROC>1)	{
		SRC[0]->AllocateData(srctmp->_rows,DELTA[0]+MaskCols);
		SRC[NUMOFPROC-1]->AllocateData(srctmp->_rows,DELTA[NUMOFPROC-1]+MaskCols);
		DST[NUMOFPROC-1]->AllocateData(SRC[NUMOFPROC-1]->_rows, SRC[NUMOFPROC-1]->_cols);
	} else
		SRC[0]->AllocateData(srctmp->_rows,srctmp->_cols);

	for(thread=1;thread<NUMOFPROC-1;thread++)	{
		SRC[thread]->AllocateData(srctmp->_rows,DELTA[thread]+2*MaskCols);
		DST[thread]->AllocateData(SRC[thread]->_rows, SRC[thread]->_cols);
	}

	for(thread=0;thread<NUMOFPROC;thread++)	{
		EX[thread].dst = DST[thread];
		EX[thread].scr = SRC[thread];
		EX[thread].MaskCols = MaskCols;
		EX[thread].MaskRows = MaskRows;

	}



/*	src1 = new C_Image_Container;
	src2 = new C_Image_Container;
	
	x1 = (unsigned int)floor(srctmp->_cols/2.0)+MaskCols;
	x2 = srctmp->_cols-(unsigned int)floor(srctmp->_cols/2.0)+MaskCols;
	src1->AllocateData(srctmp->_rows,x1);
	src2->AllocateData(srctmp->_rows,x2);*/

	if(NUMOFPROC>1)	{
		k1=0;
		for(r=0;r<srctmp->_rows;r++)	{
			for(k=START[0];k<=STOP[0]+MaskCols;k++)	{
				SRC[0]->SetPixel(r,k1++,srctmp->GetPixel(r,k));
			}
			k1 = 0;
		} 
		k1=0;
		for(r=0;r<srctmp->_rows;r++)	{
			for(k=START[NUMOFPROC-1]-MaskCols;k<=STOP[NUMOFPROC-1];k++)	{
				SRC[NUMOFPROC-1]->SetPixel(r,k1++,srctmp->GetPixel(r,k));
			}
			k1 = 0;
		} 
		for(thread=1;thread<NUMOFPROC-1;thread++)	{
			k1=0;
			for(r=0;r<srctmp->_rows;r++)	{
				for(k=START[thread]-MaskCols;k<=STOP[thread]+MaskCols;k++)	{
					SRC[thread]->SetPixel(r,k1++,srctmp->GetPixel(r,k));
				}
				k1 = 0;
			}
		}
	} else
		srctmp->CloneObject(SRC[0]);


	delete srctmp;


	for(thread=0;thread<NUMOFPROC;thread++)
		hThread1[thread] = (HANDLE)_beginthread(func, 0,(void*)&EX[thread]);

	WaitForMultipleObjects(NUMOFPROC,hThread1,TRUE,INFINITE);


	k1 = MaskCols;
	for(thread=1;thread<NUMOFPROC;thread++)
		for(r=0;r<SRC[thread]->_rows;r++)	{
			for(k=START[thread];k<=STOP[thread];k++)
				DST[0]->SetPixel(r,k,DST[thread]->GetPixel(r,k1++));
			k1 = MaskCols;
		}

	if(czytranspose)
		DST[0]->Transpose();

	outtab[0]->AssignValue((void*)DST[0],err);
	for(thread=0;thread<NUMOFPROC;thread++)	{
		delete SRC[thread];
		delete DST[thread];
	}
}

void func(void *data)
{
	E *ex;
	ex = (E*)data;
	int MaskRows = ex->MaskCols;
	int MaskCols = ex->MaskCols;
	C_Image_Container* dst = ex->dst;
	C_Image_Container* scr = ex->scr;
	int *temp_index,*temp2_index,*temp_next_index,i,j,a,b,x,y,Imrows,Imcols,ArrayLength,DevModMid,m_mid_pos_row,m_mid_pos_col,z;
	int Imrows_temp,Imcols_temp;
	double *temp,*temp2,*temp_next_MaskCols,pixel,*Im1,median;
	// Maks must be odd ...then that wroks properly and LargeMaskSize/SmallMaskSize = integer
	// Create new temp tab
	Imrows = scr->_rows;
	Imcols = scr->_cols;

	ArrayLength = MaskRows*MaskCols;
	
	// getting the midle position in vector ArrayLength
	DevModMid=ArrayLength%2;
	DevModMid=(ArrayLength-DevModMid)/2;
	// getting the midle position in mask rows and columns
	m_mid_pos_row=MaskRows%2;
	m_mid_pos_row=(MaskRows-m_mid_pos_row)/2;

	m_mid_pos_col=MaskCols%2;
	m_mid_pos_col=(MaskCols-m_mid_pos_col)/2;

	if(MaskRows>=MaskCols)
    z=MaskRows;
	else if(MaskRows<MaskCols)
    z=MaskCols;
//	BOOL test;

	temp = new double[ArrayLength];
	temp_index = new int[ArrayLength];

	temp2 = new double[ArrayLength-MaskRows];
	temp2_index = new int[ArrayLength-MaskRows];

	temp_next_MaskCols = new double[z];
	temp_next_index = new int[z];

	Imrows_temp = Imrows + 2*m_mid_pos_row;
	Imcols_temp = Imcols + 2*m_mid_pos_col;
	Im1 = new double[Imrows_temp*Imcols_temp];

	for(i=0;i<Imrows_temp;i++)
	{
		for(j=0;j<Imcols_temp;j++)
		{
			if((i>=m_mid_pos_row && i<Imrows_temp-m_mid_pos_row) && (j>=m_mid_pos_col && j<Imcols_temp-m_mid_pos_col))
			{
				scr->GetPixel(i-m_mid_pos_row,j-m_mid_pos_col,pixel);
				*(Im1+(i)*Imcols_temp+j)=pixel;
			}
			
			else if(i<m_mid_pos_row)
			{
				if(j>=m_mid_pos_col && j<Imcols_temp-m_mid_pos_col)
				{
					scr->GetPixel(0,j-m_mid_pos_col,pixel);
					*(Im1+(i)*Imcols_temp+j)=pixel;
				}
				else if(j<m_mid_pos_col)
				{
					scr->GetPixel(0,0,pixel);
					*(Im1+(i)*Imcols_temp+j)=pixel;
				}
				else if(j>=Imcols_temp-m_mid_pos_col)
				{
					scr->GetPixel(0,Imcols-1,pixel);// chyba b³¹d
					*(Im1+(i)*Imcols_temp+j)=pixel;
				}
			}
			else if(i>=Imrows_temp-m_mid_pos_row)
			{
				if(j>=m_mid_pos_col && j<Imcols_temp-m_mid_pos_col)
				{
					scr->GetPixel(Imrows-1,j-m_mid_pos_col,pixel);
					*(Im1+(i)*Imcols_temp+j)=pixel;
				}
				else if(j<m_mid_pos_col)
				{
					scr->GetPixel(Imrows-1,0,pixel);
					*(Im1+(i)*Imcols_temp+j)=pixel;
				}
				else if(j>=Imcols_temp-m_mid_pos_col)
				{
					scr->GetPixel(Imrows-1,Imcols-1,pixel);
					*(Im1+(i)*Imcols_temp+j)=pixel;
				}
			}
			else if(j<m_mid_pos_row &&(i>=m_mid_pos_row && i<Imrows_temp-m_mid_pos_row))
			{
				scr->GetPixel(i-m_mid_pos_row,0,pixel);
				*(Im1+(i)*Imcols_temp+j)=pixel;
			}
			else if(j>Imcols_temp-m_mid_pos_col &&(i>=m_mid_pos_row && i<Imrows_temp-m_mid_pos_row))
			{
				scr->GetPixel(i-m_mid_pos_row,Imcols-1,pixel);
				*(Im1+(i)*Imcols_temp+j)=pixel;
			}
			
		}
	}
	


//********************************************************************************************************************
//********************************************************************************************************************
//********************************************************************************************************************

// moving the mask accros the image and looking for a median value
for (i=0;i<Imrows_temp-MaskRows+1;i++)
{  //LLL1       
     for (j=0;j<Imcols_temp-MaskCols+1;j++)
             { //LL1
                 
                 
// *********************************************************
// ************************ Start For Moving mask **********
// *********************************************************       
               
                    if(j==0)
                            {//for j=0
                            for(a=0;a<MaskRows;a++)
                                for(b=0;b<MaskCols;b++)
                                 {
                                    *(temp+a*MaskCols+b)=*(Im1+(i+a)*Imcols_temp+j+b);
									//*(temp_index+a*MaskCols+b)=a*MaskCols+b+1;
                                    *(temp_index+a*MaskCols+b)=a+b*MaskRows+1;
                                 }
                                        quickSort(temp,temp_index,ArrayLength);
										/*test=quickSortfast(temp,ArrayLength);
										_ASSERT(test);
										quickSortfastint(temp_index,ArrayLength);
										_ASSERT(test);*/
                                        median=*(temp+DevModMid);
										dst->SetPixel(i,j,median);

                                //*(Im2+(i+m_mid_pos_row)*Imcols+j+m_mid_pos_col)=median;
                         }// for j==0
                    if(j>0)
                        { // for j = MaskCols
                            for(a=0;a<MaskRows;a++)
                            {
                                *(temp_next_MaskCols+a)=*(Im1+(i+a)*Imcols_temp+j+MaskCols-1);
                                *(temp_next_index+a)=a+1+ArrayLength;
                            }

                                quickSort(temp_next_MaskCols,temp_next_index,MaskRows);
								/*test = quickSortfastint(temp_next_index,MaskRows);
								_ASSERT(test);
								test = quickSortfastint(temp_next_index,MaskRows);
								_ASSERT(test);*/
                 // removing previous elements from temp2 and writing it to temp
                                y=0;
                                for(x=0;x<ArrayLength;x++)
                                    {// for L1
                                        if(*(temp_index+x)>MaskRows)
                                            { // L1.1
                                                *(temp2+x-y)=*(temp+x);
                                                *(temp2_index+x-y)=*(temp_index+x);
                                            } // R1.1
                                        else if(*(temp_index+x)<=MaskRows)
                                            {//L1.2
                                                y=y+1;
                                            }//R1.2
                                    }// for R1
                                a=0;
                                b=0;
                // Conecting old vector and new mask column to one sorted vector                
                                while(a+b<ArrayLength)
                                    {//while L1
                                        if(a<(ArrayLength-MaskRows) && b<MaskRows)
                                            {//L1
                                                if(*(temp2+a)>*(temp_next_MaskCols+b))
                                                    {//L1.1
                                                       *(temp+a+b)= *(temp_next_MaskCols+b);
                                                       *(temp_index+a+b)=*(temp_next_index+b)-MaskRows;
                                                        b=b+1;
                                                    }//R1.1
                                                else if(*(temp2+a)<*(temp_next_MaskCols+b)) 
                                                    {//L1.2
                                                       *(temp+a+b)= *(temp2+a);
                                                       *(temp_index+a+b)=*(temp2_index+a)-MaskRows;
                                                        a=a+1;
                                                    }//R1.2
                                                else if(*(temp2+a)==*(temp_next_MaskCols+b))
                                                    {//L1.3
                                                       *(temp+a+b)= *(temp_next_MaskCols+b);
                                                       *(temp_index+a+b)=*(temp_next_index+b)-MaskRows;
                                                        b=b+1;
                                                       *(temp+a+b)= *(temp2+a);
                                                       *(temp_index+a+b)=*(temp2_index+a)-MaskRows;
                                                        a=a+1;  
                                                    }//R1.3    
                                            }//R1
                                        else if(a==(ArrayLength-MaskRows) && b<MaskRows)
                                            {//L2
                                                *(temp+a+b)= *(temp_next_MaskCols+b);
                                                *(temp_index+a+b)=*(temp_next_index+b)-MaskRows;
                                                 b=b+1;
                                            }//R2
                                        else if(a<(ArrayLength-MaskRows) && b==MaskRows)
                                            {//L3
                                                *(temp+a+b)= *(temp2+a);
                                                *(temp_index+a+b)=*(temp2_index+a)-MaskRows;
                                                 a=a+1;
                                            }//R3

                                    }//while R1
                // looking for a median value                

                                median=*(temp+DevModMid);
								dst->SetPixel(i,j,median);

                                //*(Im2+(i+m_mid_pos_row)*Imcols+j-MaskCols+1+m_mid_pos_col)=median;

                        }// for j=MaskCols
             }//RR1
    }//RRR1

//********************************************************************************************************************
//********************************************************************************************************************
//********************************************************************************************************************
	delete temp;
	delete temp2;
	delete temp_index;
	delete temp2_index;
	delete temp_next_MaskCols;
	delete temp_next_index;
	delete Im1;
	_endthread();
}



void quickSort(double *Array,int *ArrayIndex, int array_size)
{
  q_sort(Array,ArrayIndex, 0, array_size - 1);
}


void q_sort(double *Array,int *ArrayIndex, int left, int right)
{
  int  l_hold, r_hold,pivot_index;
  double pivot;

  l_hold = left;
  r_hold = right;
  pivot = *(Array+left);
  pivot_index = *(ArrayIndex+left);
  while (left < right)
  {
    while ((*(Array+right) >= pivot) && (left < right))
      right--;
    if (left != right)
    {
      *(Array+left) = *(Array+right);
	  *(ArrayIndex+left) = *(ArrayIndex+right);		
      left++;
    }
    while ((*(Array+left) <= pivot) && (left < right))
      left++;
    if (left != right)
    {
      *(Array+right) = *(Array+left);
	  *(ArrayIndex+right) = *(ArrayIndex+left);
      right--;
    }
  }
  *(Array+left) = pivot;
  *(ArrayIndex+left) = 	pivot_index;	
  pivot_index = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot_index)
    q_sort(Array,ArrayIndex, left, pivot_index-1);
  if (right > pivot_index)
    q_sort(Array,ArrayIndex ,pivot_index+1, right);
}

//  quickSort
//
//  This public-domain C implementation by Darel Rex Finley.
//
//  * Returns YES if sort was successful, or NO if the nested
//    pivots went too deep, in which case your array will have
//    been re-ordered, but probably not sorted correctly.
//
//  * This function assumes it is called with valid parameters.
//
//  * Example calls:
//    quickSort(&myArray[0],5); // sorts elements 0, 1, 2, 3, and 4
//    quickSort(&myArray[3],5); // sorts elements 3, 4, 5, 6, and 7

bool quickSortfast(double *arr, unsigned int elements) {

  #define  MAX_LEVELS  10000

  int  beg[MAX_LEVELS], end[MAX_LEVELS],i=0, L, R ;
  double piv;

  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L]; if (i==MAX_LEVELS-1) return false;
      while (L<R) {
        while (arr[R]>=piv && L<R) R--; if (L<R) arr[L++]=arr[R];
        while (arr[L]<=piv && L<R) L++; if (L<R) arr[R--]=arr[L]; }
      arr[L]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L; }
    else {
      i--; }}
  return true; }

bool quickSortfastint(int *arr, unsigned int elements) {

  #define  MAX_LEVELS  10000

  int  beg[MAX_LEVELS], end[MAX_LEVELS],i=0, L, R ;
  int piv;

  beg[0]=0; end[0]=elements;
  while (i>=0) {
    L=beg[i]; R=end[i]-1;
    if (L<R) {
      piv=arr[L]; if (i==MAX_LEVELS-1) return false;
      while (L<R) {
        while (arr[R]>=piv && L<R) R--; if (L<R) arr[L++]=arr[R];
        while (arr[L]<=piv && L<R) L++; if (L<R) arr[R--]=arr[L]; }
      arr[L]=piv; beg[i+1]=L+1; end[i+1]=end[i]; end[i++]=L; }
    else {
      i--; }}
  return true; }