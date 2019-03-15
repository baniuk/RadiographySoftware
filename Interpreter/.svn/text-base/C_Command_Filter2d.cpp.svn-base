#include "StdAfx.h"
#include "C_Command_Filter2d.h"

C_Command_Filter2d::C_Command_Filter2d(void)
{
	UINT parint[4] = {IMAGE,NUMBER,NUMBER,NUMBER};
	UINT parout[1] = {IMAGE};
	InitCommand("filter2d",4,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = filter2d(IMAGE,NUMBER,NUMBER,NUMBER)";
	fun_ver = "1.0";
}

C_Command_Filter2d::~C_Command_Filter2d(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Filter2d::Do(C_Error& err)
{
	C_Image_Container *image;
	C_Image_Container nowy;

	C_Matrix_Container *p_type;
	C_Matrix_Container *p_sizex;
	C_Matrix_Container *p_sizey;
	unsigned int type, sizex, sizey;
	double *mask;
	unsigned int x_res, y_res;
	
	image = (C_Image_Container*)intab[0]->data;
	p_type = (C_Matrix_Container*)intab[1]->data;
	p_sizex = (C_Matrix_Container*)intab[2]->data;
	p_sizey = (C_Matrix_Container*)intab[3]->data;
	type = (int)p_type->data[0];
	sizex = (int)p_sizex->data[0];
	sizey = (int)p_sizey->data[0];

	x_res = image->_rows;
	y_res = image->_cols;
	nowy.AllocateData(x_res,y_res);
	
	mask = new double[sizex*sizey];
	int ind = 0;
	double pom = 0;

	for(unsigned int k=0;k<sizex*sizey;k++)
		mask[k]=0;

	if(type==0)/////////Median rectangle///////////
		for(unsigned int i=0;i<image->_rows;i++)
			for(unsigned int j=0;j<image->_cols;j++)
			{
				ind=0;
				for (unsigned int m=0;m<sizex;m++)
					for (unsigned int n=0;n<sizey;n++)			
						if((i+m)-((sizex-1)/2)>0)
							if((j+n-((sizey-1)/2))>0)
								if((i+m)-((sizex-1)/2)<image->_rows)
									if((j+n-((sizey-1)/2))<image->_cols)
									{										
										image->GetPixel(i+m-((sizex-1)/2),j+n-((sizey-1)/2),pom);
										mask[ind]=pom;
										ind=ind+1;
									}

				sortquick(mask,0,ind);
				if(ind%2==0)
				{
					nowy.SetPixel(i,j,mask[ind/2]);	
				}
				if(ind%2!=0)
				{
					nowy.SetPixel(i,j,mask[(ind-1)/2]);
				}

			}
	if(type==1)/////////Median cross///////////
		for(unsigned int i=0;i<image->_rows;i++)
			for(unsigned int j=0;j<image->_cols;j++)
			{
				ind=0;
				for (unsigned int m=0;m<sizex;m++)
					if((i+m)-((sizex-1))/2>0)
						if((i+m)-((sizex-1))/2<image->_rows)
						{
							image->GetPixel(i+m-(sizex-1)/2,j,pom);
							mask[ind]=pom;			
							ind=ind+1;
						}
				for (unsigned int n=0;n<sizey;n++)
					if((j+n-(sizey-1)/2)>0)							
						if(n!=(sizey+1)/2)
							if((j+n-(sizey-1)/2)<image->_cols)
							{
								image->GetPixel(i,(j+n-(sizey-1)/2),pom);
								mask[ind]=pom;			
								ind=ind+1;
							}
				sortquick(mask,0,ind);
			
				
				if(ind%2==0)
				{
					nowy.SetPixel(i,j,mask[ind/2]);	
				}
				if(ind%2!=0)
				{
					nowy.SetPixel(i,j,mask[(ind-1)/2]);
				}
			}
	///////////////
	///////////////
	if(type==2)/////////Median X///////////
		for(unsigned int i=0;i<image->_rows;i++)
			for(unsigned int j=0;j<image->_cols;j++)
			{
				int pom1=0;
				ind=0;
				for(unsigned int m=0;m<sizex;m++)
					{
						if((i+m)-(sizex-1)/2>0)
							if((i+m)-(sizex-1)/2<image->_rows)
								if(j+pom1-(sizex-1)/2>0)
									if(j+pom1-(sizex-1)/2<image->_cols)
									{
										image->GetPixel(i+m-(sizex-1)/2,j+pom1-(sizex-1)/2,pom);
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
								if((i+m)-(sizex-1)/2<image->_rows)
									if(j-pom1+(sizex-1)/2<image->_cols)
									{
										image->GetPixel(i+m-(sizex-1)/2,j+(sizex-1)/2-pom1,pom);
										mask[ind]=pom;
										ind++;
									}
					pom1=pom1+1;
				}
				sortquick(mask,0,ind);
			
			
				if(ind%2==0)
				{
					nowy.SetPixel(i,j,mask[ind/2]);	
				}
				if(ind%2!=0)
				{
					nowy.SetPixel(i,j,mask[(ind-1)/2]);
				}
			}
	if(type==3)/////////Blure///////////
		for(unsigned int i=0;i<image->_rows;i++)
			for(unsigned int j=0;j<image->_cols;j++)	
			{
				
				int s=0;
				ind=0;
				for (unsigned int m=0;m<sizex;m++)
					for (unsigned int n=0;n<sizey;n++)			
						if((i+m)-((sizex-1)/2)>0)
							if((j+n-((sizey-1)/2))>0)
								if((i+m)-((sizex-1)/2)<image->_rows)
									if((j+n-((sizey-1)/2))<image->_cols)
									{
										
										image->GetPixel(i+m-((sizex-1)/2),j+n-((sizey-1)/2),pom);
										ind=ind+(int)pom;
										s=s+1;
										
									}
				
				nowy.SetPixel(i,j,ind/s);
			
			}
	
	outtab[0]->AssignValue((void*)&nowy,err);
//	delete mask;	// dziwne b³êdy
}

void C_Command_Filter2d::sortquick(double* tabl, int l, int p) 
{ 
	double v=tabl[(l+p)/2]; 
	int i,j;
	double temp; 
	i=l; 
	j=p; 
	do 
	{ 
		while (tabl[i]<v) i++; 
		while (v<tabl[j]) j--; 
		if (i<=j)
		{ 
			temp=tabl[i];
			tabl[i]=tabl[j];
			tabl[j]=temp;
			i++;
			j--;
		} 
	} 
	while (i<=j);
	if (l<j) sortquick(tabl,l,j); 
	if (i<p) sortquick(tabl,i,p); 
	
} 