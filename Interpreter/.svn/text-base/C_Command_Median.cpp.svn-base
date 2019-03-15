#include "StdAfx.h"
#include "C_Command_Median.h"

C_Command_Median::C_Command_Median(void)
{
	UINT parint[4] = {IMAGE,NUMBER,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("median",4,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = median(IMAGE,NUMBER,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Median::~C_Command_Median(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Median::Do(C_Error& err)
{
	C_Image_Container *p_input,*input;
	C_Image_Container output;
	C_Image_Container input1;
	unsigned int type, sizex, sizey;
	C_Matrix_Container *p_type, *p_sizex, *p_sizey;

	p_input = (C_Image_Container*)intab[0]->data;
	p_type = (C_Matrix_Container*)intab[1]->data;
	p_sizex = (C_Matrix_Container*)intab[2]->data;
	p_sizey = (C_Matrix_Container*)intab[3]->data;

	type = (int)p_type->data[0];
	sizex = (int)p_sizex->data[0];
	sizey = (int)p_sizey->data[0];

	p_input->CloneObject(&input1);
	input1.Normalize(0,65535);
	input = &input1;

//	input->Normalize(0,65535);
	unsigned int x_res, y_res;
	x_res = input->_rows;
	y_res = input->_cols;
	output.AllocateData(x_res,y_res);
	
	double* mask=new double [sizex*sizey];
	int ind=0;
	double pom=0;
	if(type==0)
	{
		for(unsigned int i=0;i<input->_rows;i++)
			for(unsigned int j=0;j<input->_cols;j++)
			{
				ind=0;
				for (unsigned int m=0;m<sizex;m++)
					for (unsigned int n=0;n<sizey;n++)			
						if((i+m)-((sizex-1)/2)>0)
							if((j+n-((sizey-1)/2))>0)
								if((i+m)-((sizex-1)/2)<input->_rows)
									if((j+n-((sizey-1)/2))<input->_cols)
									{
										input->GetPixel(i+m-((sizex-1)/2),j+n-((sizey-1)/2),pom);
										mask[ind]=pom;
										ind=ind+1;
									}
				if(ind%2==0)
				{
					double war;
					war=Hoare(mask,ind,ind/2);
					output.SetPixel(i,j,war);	
				}
				if(ind%2!=0)
				{
					double war;
					war=Hoare(mask,ind,(ind-1)/2);
					output.SetPixel(i,j,war);	
				}
			}
	}
	if(type==1)/////////Median cross///////////
	{
		for(unsigned int i=0;i<input->_rows;i++)
			for(unsigned int j=0;j<input->_cols;j++)
			{
				ind=0;
				for (unsigned int m=0;m<sizex;m++)
					if((i+m)-((sizex-1))/2>0)
						if((i+m)-((sizex-1))/2<input->_rows)
						{
							input->GetPixel(i+m-(sizex-1)/2,j,pom);
							mask[ind]=pom;			
							ind=ind+1;
						}
				for (unsigned int n=0;n<sizey;n++)
					if((j+n-(sizey-1)/2)>0)							
						if(n!=(sizey+1)/2)
							if((j+n-(sizey-1)/2)<input->_cols)
							{
								input->GetPixel(i,(j+n-(sizey-1)/2),pom);
								mask[ind]=pom;			
								ind=ind+1;
							}
				if(ind%2==0)
				{
					double war;
					war=Hoare(mask,ind,ind/2);
					output.SetPixel(i,j,war);	
				}
				if(ind%2!=0)
				{
					double war;
					war=Hoare(mask,ind,(ind+1)/2);
					output.SetPixel(i,j,war);	
				}
			}
	}

	if(type==2)///////////Median X///////////
	{
		for(unsigned int i=0;i<input->_rows;i++)
			for(unsigned int j=0;j<input->_cols;j++)
			{
				int pom1=0;
				ind=0;
				for(unsigned int m=0;m<sizex;m++)
					{
						if((i+m)-(sizex-1)/2>0)
							if((i+m)-(sizex-1)/2<input->_rows)
								if(j+pom1-(sizex-1)/2>0)
									if(j+pom1-(sizex-1)/2<input->_cols)
									{
										input->GetPixel(i+m-(sizex-1)/2,j+pom1-(sizex-1)/2,pom);
										mask[ind]=pom;
										ind++;
									}
						pom1=pom1+1;
						}
				pom1=sizex-1;
				for(unsigned int m=0;m<sizex;m++)
				{
					if(pom1!=(sizex-1)/2)
						if((i+m)-(sizex-1)/2>0)
							if(j-pom1+(sizex-1)/2>0)
								if((i+m)-(sizex-1)/2<input->_rows)
									if(j-pom1+(sizex-1)/2<input->_cols)
									{
										input->GetPixel(i+m-(sizex-1)/2,j+(sizex-1)/2-pom1,pom);
										mask[ind]=pom;
										ind++;
									}
					pom1=pom1+1;
				}
				if(ind%2==0)
				{
					double war;
					war=Hoare(mask,ind,ind/2);
					output.SetPixel(i,j,war);	
				}
				if(ind%2!=0)
				{
					double war;
					war=Hoare(mask,ind,(ind-1)/2);
					output.SetPixel(i,j,war);	
				}

			}
	}

//	image3.isBinary = TRUE;
	delete[] mask;
//	double min,max;
//	output.getMinMax(min,max);
	outtab[0]->AssignValue((void*)&output,err);
}

double C_Command_Median::Hoare(double *tab,int size,int mid)
{
	int n,i,j;
	n=size;
	double k,w;
	w=mid;
	i=0;
	j=n-1;
	n=(int)w;
	while (i!=j)
		{
			k=Partition(tab,i,j);
			k=k-i+1;
			if (k>=w) j=i+(int)k-1;
				if (k<w)
					{
						w-=(int)k;
						i+=(int)k;
					}
		}
	return(tab[i]);
}

double C_Command_Median::Partition(double* c, int a, int b)
{
	double e,tmp;
	e=c[a];        //elemennt dzielacy
	while (a<b)
    {
        while ((a<b) && (c[b]>=e)) b--;
        while ((a<b) && (c[a]<e)) a++;
        if (a<b)
                {
                tmp=c[a];
                c[a]=c[b];
                c[b]=tmp;
                }
        }
	return a;
}
