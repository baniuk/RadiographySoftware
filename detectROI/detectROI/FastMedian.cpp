#include "Includes.h"

void  FastMedian(C_Image_Container *scr,C_Image_Container *dst,int MaskRows,int MaskCols)
{
	int *temp_index,*temp2_index,*temp_next_index,i,j,a,b,x,y,Imrows,Imcols,ArrayLength,DevModMid,m_mid_pos_row,m_mid_pos_col,z;
	int Imrows_temp,Imcols_temp;
	double *temp,*temp2,*temp_next_MaskCols,pixel,*Im1,median;
	
		
	// Type paramet no available at this moment

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


	temp = new double[ArrayLength];
	temp_index = new int[ArrayLength];

	temp2 = new double[ArrayLength-MaskRows];
	temp2_index = new int[ArrayLength-MaskRows];

	temp_next_MaskCols = new double[z];
	temp_next_index = new int[z];

	Imrows_temp = Imrows + 2*m_mid_pos_row;
	Imcols_temp = Imcols + 2*m_mid_pos_col;
	Im1 = new double[Imrows_temp*Imcols_temp];
	
	
/*
	for(i=0;i<Imrows;i++)
	{
		for(j=0;j<Imcols;j++)
		{
			scr->GetPixel(i,j,pixel);
			*(Im1+(i+m_mid_pos_row)*Imcols_temp+j+m_mid_pos_col)=pixel;
		}
	}
*/

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
					scr->GetPixel(0,Imcols-1,pixel);
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
                                        quickSort(temp,temp_index,ArrayLength,1);
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

                                quickSort(temp_next_MaskCols,temp_next_index,MaskRows,1);
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

	delete[] temp;
	delete[] temp2;
	delete[] temp_index;
	delete[] temp2_index;
	delete[] temp_next_MaskCols;
	delete[] temp_next_index;
	delete[] Im1;
	


}
void quickSort(double *Array,int *ArrayIndex, int array_size,bool AttachArrayIndex)
{
  q_sort(Array,ArrayIndex, 0, array_size - 1,AttachArrayIndex);
}


void q_sort(double *Array,int *ArrayIndex, int left, int right,bool AttachArrayIndex)
{
  int  l_hold, r_hold,pivot_index;
  double pivot;

  l_hold = left;
  r_hold = right;
  pivot = *(Array+left);
  if(AttachArrayIndex==1){pivot_index = *(ArrayIndex+left);}
  while (left < right)
  {
    while ((*(Array+right) >= pivot) && (left < right))
      right--;
    if (left != right)
    {
      *(Array+left) = *(Array+right);
	  if(AttachArrayIndex==1){*(ArrayIndex+left) = *(ArrayIndex+right);		}
      left++;
    }
    while ((*(Array+left) <= pivot) && (left < right))
      left++;
    if (left != right)
    {
      *(Array+right) = *(Array+left);
	  if(AttachArrayIndex==1){*(ArrayIndex+right) = *(ArrayIndex+left);}
      right--;
    }
  }
  *(Array+left) = pivot;
  if(AttachArrayIndex==1){*(ArrayIndex+left) = 	pivot_index;	}
  pivot_index = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot_index)
    q_sort(Array,ArrayIndex, left, pivot_index-1,AttachArrayIndex);
  if (right > pivot_index)
    q_sort(Array,ArrayIndex ,pivot_index+1, right,AttachArrayIndex);
}