 #include "Cpp_Lib.h"
 #include <fstream>

using namespace std;

// This is an empty project to build DLL library of interface version 2.0

double Hoare(double *tab,int size,int mid);
double Partition(double* c, int a, int b);

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"iqidet");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"IMAGE = iqidet(IMAGE,NUMBER,NUMBER,NUMBER)");
	definition->numofout = 1;
	definition->numofin = 4;
	definition->typesofin[0] = IMAGE;
	definition->typesofin[1] = NUMBER;
	definition->typesofin[2] = NUMBER;
	definition->typesofin[3] = NUMBER;
	definition->typesofout[0] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"Detects IQI");
	definition->ADDIP_version = 2.0;
}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)

}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
// VARIABLES ---------------------------------------------------------------------------------------------
	C_Image_Container *image;
	C_Matrix_Container *wid;
	C_Matrix_Container *ang;
	C_Matrix_Container *typ;
	C_Image_Container result;
	int width, angle, type, skok;
	int k = 0, u = 0, l =0, x=0;
	int jmp, sk = 0, tmp = 0;
	double zmn = 0,licznik = 0;
	unsigned int i,j;
	double* vect;

	image = (C_Image_Container*)intab[0]->data;
	ang = (C_Matrix_Container*)intab[1]->data;
	wid = (C_Matrix_Container*)intab[2]->data;
	typ = (C_Matrix_Container*)intab[3]->data;
	result.AllocateData(image->_rows,image->_cols);

//	image->Normalize(0,65535);
	result.Zeros();
	width = (int)wid->data[0];
	angle = (int)ang->data[0];
	type = (int)typ->data[0];

// IMPLEMENTATION ----------------------------------------------------------------------------------------
	
	if ((angle <= 3) && (angle >= -3))												// K¥T MNIEJSZY NI¯ 4 STOPNIE
	{
		jmp = 9;	// WARTOŒC SKOKU
	}
	else if (((angle > 3) && (angle <= 6)) || ((angle < -3) && (angle >= -6)))		// K¥T Z PRZEDZIA£U 4-6 STOPNI
	{
		jmp = 7;    // WARTOŒÆ SKOKU
	}
	else if (((angle > 6) && (angle <= 9)) || ((angle < -6) && (angle >= -9)))		// K¥T Z PRZEDZIA£U 7-9 STOPNI
	{
		jmp = 5;	// WARTOŒC SKOKU
	}
	else if (((angle > 9) && (angle <= 15)) || ((angle < -9) && (angle >= -15)))	// K¥T ZPRZEDZIA£U 10-15 STOPNI
	{
		jmp = 3;	// WARTOŒC SKOKU
	}
	else if ((angle > 15) || (angle < -15))											// K¥T WIÊKSZY NI¯ 15 STOPNI
	{
		jmp = 1;	// WARTOŒC SKOKU
	}
	
	int height = (jmp * width);
		
	//angle = 0 - angle;

	if (angle < 0)
		k = 0;
	else
		k = height;

// ERROR MESSAGE ------------------------------------------------------------------------------------------
	if((width % 2 == 0))
	{
		err.SetError("iqidet: 'width' parameter incorrect");

		ofstream plik1("iqi_out.txt");
		plik1<<"Parameters of iqidet function:"<<endl<<endl;
		plik1<<"Angle = "<<angle<<" -> JMP set to: "<<jmp<<endl;
		plik1<<"Width = "<<width<<" - MUST BE ODD VALUE!!!"<<endl;
		plik1<<"Num = "<<type<<endl;
		plik1<<"Height = ???";

		return;
	}

	if (type == 2)
	{
		err.SetError("iqidet: 'num' parameter incorrect");

		ofstream plik1("iqi_out.txt");
		plik1<<"Parameters of iqidet function:"<<endl<<endl;
		plik1<<"Angle = "<<angle<<" -> JMP set to: "<<jmp<<endl;
		plik1<<"Width = "<<width<<endl;
		plik1<<"Num = "<<type<<" - WRONG NUMBER!!!"<<endl;
		plik1<<"Height = "<<height;

		return;
	}
	if (height>image->_rows)
	{
		err.SetError("iqidet: 'width' parameter to high");

		ofstream plik1("iqi_out.txt");
		plik1<<"Parameters of iqidet function:"<<endl<<endl;
		plik1<<"Angle = "<<angle<<" -> JMP set to: "<<jmp<<endl;
		plik1<<"Width = "<<width<<" - TOO HIGH!!!"<<endl;
		plik1<<"Num = "<<type<<endl;
		plik1<<"Height = "<<height<<" - IMAGE HEIGHT EXCEEDED!!!";

		return;
	}

	ofstream plik1("iqi_out.txt");
		plik1<<"Parameters of iqidet function:"<<endl<<endl;
		plik1<<"Angle = "<<angle<<" -> JMP set to: "<<jmp<<endl;
		plik1<<"Width = "<<width<<endl;
		plik1<<"Num = "<<type<<endl;
		plik1<<"Height = "<<height;

	if (type == 1)
	{
	vect=new double [width*jmp];

	for(j = 0; j < image->_rows-height-1; j++)							// WIERSZE OBRAZU
	{			
		for(i = 0; i < image->_cols-width-1; i++)						// KOLUMNY OBRAZU
		{
			for(l = 0; l < width; l++)
			{
				if (angle < 0)
				{
					while (tmp < jmp)
					{
						image->GetPixel(k+j,l+i,zmn);
						vect[u]=zmn;
						u++;
						tmp++;
						k++;
					}
					tmp = 0;
				}
				else
				{
					while (tmp < jmp)
					{
						image->GetPixel(k+j,l+i,zmn);
						vect[u]=zmn;
						u++;
						tmp++;
						k--;
					}
					tmp = 0;
				}
			}
			if (angle < 0)
			{
				k = 0;
			}
			else
			{
				k = height;
			}
			double war;
			war=Hoare(vect,u,u/2);
			result.SetPixel((j+height/2),(i+width/2),war);
			u = 0;
			skok = width - 1;
		}
	}
	}
	else
	{
	vect = new double [width*jmp*type];

	for(j = 0; j < image->_rows-height-1; j++)							// WIERSZE OBRAZU
	{			
		for(i = 0; i < image->_cols-width-1; i++)						// KOLUMNY OBRAZU
		{
			for(int g=0;g<=type;g++){
			for(l = 0; l < width-type; l++)
			{
				if (angle < 0)
				{
					while (tmp < jmp)
					{
						image->GetPixel(k+j,l+i+g,zmn);
						vect[u]=zmn;
						u++;
						tmp++;
						k++;
					}
					tmp = 0;
				}
				else
				{
					while (tmp < jmp)
					{
						image->GetPixel(k+j,l+i+g,zmn);
						vect[u]=zmn;
						u++;
						tmp++;
						k--;
					}
					tmp = 0;
				}
			}
			if (angle < 0)
			{
				k = 0;
			}
			else
			{
				k = height;
			}
			
			}double war;
			war=Hoare(vect,u,u/2);
			result.SetPixel((j+height/2),(i+width/2),war);
			u = 0;
		}
	}
	}

// DEPLOYING RESULTS --------------------------------------------------------------------------------------

	delete[] vect;
	outtab[0]->AssignValue((void*)&result,err);

}

// REQUIRED FUNCTIONS ------------------------------------------------------------------------------------
double Hoare(double *tab,int size,int mid)
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

double Partition(double* c, int a, int b)
{
	double e,tmp;
	e=c[a];        //element dzielacy
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