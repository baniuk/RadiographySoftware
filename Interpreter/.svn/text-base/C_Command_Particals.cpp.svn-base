#include "StdAfx.h"
#include "C_Command_Particals.h"

using namespace std;

C_Command_Particals::C_Command_Particals(void)
{
	UINT parint[1] = {IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("index",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = index(IMAGE)";
	fun_ver = "2.0";
}

C_Command_Particals::~C_Command_Particals(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Particals::Do(C_Error& err)
{

	C_Image_Container bin;
    C_Image_Container indek;
	C_Image_Container *bino;
    bino = (C_Image_Container*)intab[0]->data;
	bino->CloneObject(&bin);
        bin.CloneObject(&indek);
        indek.Zeros();
    bin.Normalize(0,1);
    double sklejka[100000];
        int x=0;
        int y=0;
    int licz=0;
        int roz1=indek._rows;
        int roz2=indek._cols;
    for(x=1;x<roz1-2;x++)                                                                      
           for(y=1;y<roz2-2;y++)                                                               
           {
                       double war1,war2,war3,war4,war5,war6,war7;
                       bin.GetPixel(x,y,war1);
                                           if(war1==1)
                       {
                                                        indek.GetPixel(x-1,y-1,war2);
                            indek.GetPixel(x-1,y,war3);
                            indek.GetPixel(x-1,y+1,war4);                           
                            indek.GetPixel(x,y-1,war5);
                            if(war2==0&&war3==0&&war4==0&&war5==0)
                            {
                                  licz=licz+1;
                                  indek.SetPixel(x,y,licz);
                                  sklejka[licz]=licz+1;
                            }
                            else
                            {
                                  war6=0;
                                  war7=0; 
                                  if(war2!=0)
                                       war6=war2;
                                  if(war3!=0)
                                       war6=war3;
                                  if(war4!=0)
                                       war6=war4;
                                  if(war5!=0)
                                       war6=war5;
                                  if(war2!=0&&war2!=war6)
                                       war7=war2;
                                  if(war3!=0&&war3!=war6)
                                       war7=war3;
                                  if(war4!=0&&war4!=war6)
                                       war7=war4;
                                  if(war5!=0&&war5!=war6)
                                       war7=war5;
                                  if(war7==0)
                                       indek.SetPixel(x,y,war6);
                                  else
                                  {
                                       indek.SetPixel(x,y,min(war6,war7));
                                       sklejka[(int)max(war6,war7)]=min(war6,war7);
                                  }
                                                        }
                       }
                           }




         for(int i=1;i<licz;i++)
                 {

                        int war;
                        war=1+licz-i;
                        for(x=0;x<indek._rows;x++)                                                                        
                                for(y=0;y<indek._cols;y++)                                                                        
                                {
                                        double war1;
                                        indek.GetPixel(x,y,war1);
                                        if(war1==war)
                                        {
                                            ASSERT(war<1000000);    
											double pom=sklejka[war];
                                                indek.SetPixel(x,y,pom);
                                        }
                                }
                 }
         double obecne=1;
                 double warmax,unused;
                 indek.getMinMax(unused,warmax);         
         for(int pom=1;pom<=warmax;pom++)
                 {
                         int licz1=0;
                         for(x=0;x<indek._rows;x++)
                                 for(y=0;y<indek._cols;y++)
                                 {
                                         double val1;
                                         indek.GetPixel(x,y,val1);
                                         if(val1==pom)
                                         {
                                                 indek.SetPixel(x,y,obecne);
                                                 licz1=licz1+1;

                                         }
                                 }
                        if(licz1>0)
                                obecne=obecne+1;
                 }

         outtab[0]->AssignValue((void*)&indek,err);


}
/*	unsigned int x_res, y_res;
	C_Image_Container *bino;
	C_Image_Container bin;
	C_Image_Container indek;

	bino = (C_Image_Container*)intab[0]->data;
	bino->CloneObject(&bin);

	x_res = bin._rows;
	y_res = bin._cols;
	
	indek.AllocateData(x_res,y_res);
	indek.Zeros();

	bin.Normalize(0,1);
	int x,y,x1,y1,x2,y2;
	double sklejka[1000];
	int licz=0;

	for(x=1;x<indek._rows;x++)
		for(y=1;y<indek._cols-1;y++)
		{
			double war1,war2,war3,war4,war5,war6,war7;
			
			bin.GetPixel(x,y,war1);
			if(war1==1)
			{
				war2=indek.GetPixel(x-1,y-1);
				war3=indek.GetPixel(x-1,y);
				war4=indek.GetPixel(x-1,y+1);
				war5=indek.GetPixel(x,y-1);//??????
				if(war2==0&&war3==0&&war4==0&&war5==0)
				{
					licz=licz+1;
					indek.SetPixel(x,y,licz);
					ASSERT(licz<1000);
					sklejka[licz]=licz+1;
				}
				else
				{
					war6=0;
					war7=0;
					
					if(war2!=0)
						war6=war2;
					if(war3!=0)
						war6=war3;
					if(war4!=0)
						war6=war4;
					if(war5!=0)
						war6=war5;
					if(war2!=0&&war2!=war6)
						war7=war2;
					if(war3!=0&&war3!=war6)
						war7=war3;
					if(war4!=0&&war4!=war6)
						war7=war4;
					if(war5!=0&&war5!=war6)
						war7=war5;
					if(war7==0)
						indek.SetPixel(x,y,war6);
					else
					{
						indek.SetPixel(x,y,min(war6,war7));
						ASSERT((int)max(war6,war7)<1000);
						sklejka[(int)max(war6,war7)]=min(war6,war7);
					}

				}
			}
		}

		for(int i=1;i<licz;i++)
		{
			int war;
			war=1+licz-i;
			for(x1=0;x1<indek._rows;x1++)
				for(y1=0;y1<indek._cols;y1++)
				{
					double war1;
					indek.GetPixel(x1,y1,war1);
					if(war1==war)
					{
						ASSERT(war<1000);
						double pom=sklejka[war];
						indek.SetPixel(x1,y1,pom);
					}
				}
		}

		double obecne=1;
		double warmax,unused;
		indek.getMinMax(unused,warmax);
		
		for(int pom=1;pom<=warmax;pom++)
		{
			int licz1=0;
			for(x2=0;x2<indek._rows;x2++)
				for(y2=0;y2<indek._cols;y2++)
				{
					double val1;
					indek.GetPixel(x2,y2,val1);
					if(val1==pom)
					{
						indek.SetPixel(x2,y2,obecne);
						licz1=licz1+1;
					}
				}
			if(licz1>0)
				obecne=obecne+1;

		}
		outtab[0]->AssignValue((void*)&indek,err);*/
	

/*
void C_Command_Particals::Param(C_Image_Container *image,C_Image_Container *indek,C_Image_Container *bin,double number,double minsiz,double maxsiz,CString path)
{
	C_Image_Container outln;
	Outline(bin,&outln);
	int poz=13;
	double *tab1 = NULL;
	double *tab2 = NULL;

	double *tabpom=new double[number*poz];
	int ind=0;
	for(unsigned int i=0;i<indek._rows;i++)
		for(unsigned int j=0;j<indek._cols;j++)
		{
			double val=0;
			
			double ilosc=0,sumax=0,sumay=0,sumaxx=0,sumayy=0;
			double val1,val2,val3;
			
			double il=0;
			int num=0;
			indek.GetPixel(i,j,val1);
			if(val1>0)
			{
				tab1=new double[100000];
				tab2=new double[100000];
				double obw=0;
				for(unsigned int ii=0;ii<indek._rows;ii++)
					for(unsigned int jj=0;jj<indek._cols;jj++)
					{
						indek.GetPixel(ii,jj,val2);
						
						if(val2==val1)
						{
							
							outln.GetPixel(ii,jj,val);
							if(val==1)
							{
								obw=obw+1;
								tab1[num]=ii;
								tab1[num+50000]=jj;
								num++;
							}
							
							tabpom[ind*poz+1]=obw;
							image->GetPixel(ii,jj,val3);
							
							ilosc=ilosc+val3;
							il++;
							tabpom[ind*poz]=il;
							sumax=sumax+val3*ii;
							sumay=sumay+val3*jj;
							sumaxx=sumaxx+ii;
							sumayy=sumayy+jj;
							indek.SetPixel(ii,jj,0);
						}
					}
					double vec=0;
					
					if(obw>30&&il>minsiz)
					{		
						for(int z=0;z<obw;z++)
							for(int zz=0;zz<obw;zz++)
							{
								double pom=sqrt((tab1[z]-tab1[zz])*(tab1[z]-tab1[zz])+(tab1[z+50000]-tab1[zz+50000])*(tab1[z+50000]-tab1[zz+50000]));
								if(pom>vec)
								{
									vec=pom;
									tabpom[ind*poz+9]=tab1[z];
									tabpom[ind*poz+11]=tab1[zz];
									tabpom[ind*poz+10]=tab1[z+50000];
									tabpom[ind*poz+12]=tab1[zz+50000];
									tabpom[ind*poz+6]=vec;
								}
							}
					}
					if(tabpom[ind*poz]>minsiz&&tabpom[ind*poz]<maxsiz)
					{	
						tabpom[ind*poz+4]=(sumax*1.0)/ilosc;
						tabpom[ind*poz+5]=(sumay*1.0)/ilosc;
						tabpom[ind*poz+2]=(sumaxx*1.0)/il;
						tabpom[ind*poz+3]=(sumayy*1.0)/il;

						ind++;
					}
			}
		}
		elips(image,tabpom,ind,path);
		delete[] tabpom;
		SAFE_DELETE(tab1);
		SAFE_DELETE(tab2);
}
void C_Command_Particals::elips(C_Image_Container *image,double *tab,int ind,CString path)
{
	double srx=0,sry=0,srcx=0,srcy=0,fil=0,alfa=0;
	double pi=3.14;
	int poz=13;
	double vec1=0,vec2=0,x1=0,x2=0,y1=0,y2=0,A=0,L=0,W=0,Ani=0,Rct=0,Elong=0,Comp=0;
	for(int i=0;i<ind;i++)
	{
		double flag1;
		x1=tab[i*poz+9];
		x2=tab[i*poz+11];
		y1=tab[i*poz+10];
		y2=tab[i*poz+12];
		fil=tab[i*poz];
		vec1=tab[i*poz+6];
		if(x1>x2)
		{
			flag1=1;
			srx=x1-(x1-x2)/2;
		}
		else
		{
			srx=x2-(x2-x1)/2;
			flag1=0;
		}
		if(y1>y2)
			sry=y1-(y1-y2)/2;
		else
			sry=y2-(y2-y1)/2;
		
		vec2=fil/(pi*vec1);
		tab[i*poz+7]=vec2;
		Ani=vec1/vec2;
		double val=0;
		if(flag1=1)
			val=abs(y1-sry)/abs(x1-srx);
		else
			val=abs(y2-sry)/abs(x2-srx);

		alfa=atan(val);
		tab[i*poz+8]=alfa;
		double c=0;
		
	}
	double obecne=ind;
	ofstream plk_wy1(path);
	plk_wy1<<"cohx"<<" ";
	plk_wy1<<"cohy"<<" ";
	plk_wy1<<"radius1"<<" ";
	plk_wy1<<"radius2"<<" ";
	plk_wy1<<"alfa"<<" ";
	plk_wy1<<"field"<<" ";
	plk_wy1<<"district"<<" ";
	plk_wy1<<"\n";
		for(int i=0;i<(int)obecne;i++)
		{
			plk_wy1<<(tab[i*poz+2])<<" ";
			plk_wy1<<(tab[i*poz+3])<<" ";
			plk_wy1<<(tab[i*poz+6]/2)<<" ";
			plk_wy1<<(tab[i*poz+7]*2)<<" ";
			plk_wy1<<(tab[i*poz+8])<<" ";
			plk_wy1<<(tab[i*poz])<<" ";
			plk_wy1<<(tab[i*poz+1])<<" ";
			plk_wy1<<"\n";
		}
		
		plk_wy1.close();
}

void C_Command_Particals::Outline(C_Image_Container* image,C_Image_Container* outln)
{
	// POMOCNICZE OBRAZKI
	unsigned int x_res, y_res;
	x_res = image->_cols;
	y_res = image->_rows;

	// DO EROZJI
	C_Image_Container image2;
	image2.AllocateData(y_res,x_res);
	// TYMCZASOWY OBRAZEK BINARNY
	
	// WYNIKOWY
	outln->AllocateData(y_res,x_res);


	// ZMIENNE
	unsigned int size_x = 3;
	unsigned int size_y = 3;
	double zm;
	int k = 0;

	// IMPLEMENTACJA EROZJI
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{	
			k = 0;
			for (unsigned int i = 0; i < size_y; i++)
			{	
				for (unsigned int j = 0; j < size_x; j++)
				{
					if((i+m)-((size_x-1)/2)>0)
						if((j+n-((size_y-1)/2))>0)
							if((i+m)-((size_x-1)/2)<image2._cols)
								if((j+n-((size_y-1)/2))<image2._rows)
								{
									image->GetPixel(j+n-(size_x-1)/2,i+m-(size_y-1)/2,zm);
									if (zm != 1)
									{
										k = 1;
									}
									
								}
				}
			}
			if (k == 1)
			{
				image2.SetPixel(n,m,1);
			}
			else
			{
				image2.SetPixel(n,m,0);
			}
		}
	}

	double zm1 = 0, zm2 = 0;
	// ODEJMOWANIE
	for (unsigned int m = 0; m < (image2._cols); m++)
	{
		for (unsigned int n = 0; n < (image2._rows); n++)
		{
			image->GetPixel(n,m,zm1);
			image2.GetPixel(n,m,zm2);
			if (zm1-zm2 == 0)
			{
				outln->SetPixel(n,m,1);
			}
			else
			{
				outln->SetPixel(n,m,0);
			}
		}
	}
	
}*/