#include "StdAfx.h"
#include "C_Command_Fftfilt.h"

C_Command_Fftfilt::C_Command_Fftfilt(void)
{
	UINT parint[3] = {IMAGE,NUMBER,NUMBER};
	UINT parout[2] = {IMAGE,IMAGE};
	InitCommand("fftfilt",3,2,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "[IMAGE IMAGE] = fftfilt(IMAGE,NUMBER,NUMBER)";
	fun_ver = "1.7";
}

C_Command_Fftfilt::~C_Command_Fftfilt(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Fftfilt::Do(C_Error& err)
{
	C_Image_Container *imagein;
	C_Image_Container *image;
	C_Matrix_Container *p_start;
	C_Matrix_Container *p_stop;
	C_Image_Container im;
	C_Image_Container copy;
	C_Matrix_Container outre,outim,tmpoutre,tmpoutim,tmpoutre1,tmpoutim1;
	unsigned long a,aa,i=2;
	fftw_complex *out, *out_in;
	fftw_plan g, g1,g2;
	unsigned int imsize,imsize1;
	unsigned long x,y;
	double wspy,wspx;
	
	double centerx,centery,val,start, stop,gaus,gaus1,val1;
	fftw_complex* line;

	C_Image_Container output;

	imagein = (C_Image_Container*)intab[0]->data;
	p_start = (C_Matrix_Container*)intab[1]->data;
	p_stop = (C_Matrix_Container*)intab[2]->data;
	start = (double)p_start->data[0];
	stop = (double)p_stop->data[0];

	imagein->CloneObject(&copy);
	image = &copy;

// po rzedach
	im.AllocateData(image->_rows,image->_cols);
	im.Zeros();
	imsize = image->_cols;
	while(i<1.5 * imsize) i *= 2;
	imsize = i;
	imsize1 = imsize;
	line = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * imsize);
	out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * imsize);
		
	tmpoutre.AllocateData(image->_rows,imsize);
	tmpoutim.AllocateData(image->_rows,imsize);
	for(a=0;a<image->_rows;a++)	{
		FlipLine(imsize,image->_cols,image->data+a*image->_cols,im.data+a*im._cols,line);	
		g = fftw_plan_dft_1d(imsize, line, out, FFTW_FORWARD,FFTW_ESTIMATE);
		fftw_execute(g);
		fftw_destroy_plan(g);fftw_cleanup();
		for(aa=0;aa<imsize;aa++)	{
			tmpoutre.SetPixel(a,aa,out[aa][0]);
			tmpoutim.SetPixel(a,aa,out[aa][1]);
		}
	}
	for(a=0;a<tmpoutre.GetNumofElements();a++)	{
		tmpoutre.data[a]/= imsize;
		tmpoutim.data[a]/= imsize;
	}
	fftw_free(line);
	fftw_free(out);

	tmpoutre.Transpose();
	tmpoutim.Transpose();

// po kolumnach
	imsize = image->_rows;
	while(i<1.5 * imsize) i *= 2;
	imsize = i;
	line = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * imsize);
	out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * imsize);

	outre.AllocateData(tmpoutre._rows,imsize);
	outim.AllocateData(tmpoutre._rows,imsize);
	for(a=0;a<tmpoutre._rows;a++)	{
		FlipLine(imsize,tmpoutre._cols,tmpoutre.data+a*tmpoutre._cols,tmpoutim.data+a*tmpoutim._cols,line);	
		g2 = fftw_plan_dft_1d(imsize, line, out, FFTW_FORWARD,FFTW_ESTIMATE);
		fftw_execute(g2);
		fftw_destroy_plan(g2);fftw_cleanup();
		for(aa=0;aa<imsize;aa++)	{
			outre.SetPixel(a,aa,out[aa][0]);
			outim.SetPixel(a,aa,out[aa][1]);
		}
	}
	fftw_free(line);
	fftw_free(out);

	outre.Transpose();
	outim.Transpose();

	for(a=0;a<outre.GetNumofElements();a++)	{
		outre.data[a]/= imsize;
		outim.data[a]/= imsize;
	}
	Swap(&outre,&outim,outre._cols,outre._rows);

	centerx = outre._cols/2+0.5;
	centery = outre._rows/2+0.5;

// --------------------------------- MOJ ----------------------------------------
	val = 1/(2*M_PI*start*start);
	val1 = 1/(2*M_PI*stop*stop);

	if(outre._rows>outre._cols)	{
		wspy = 1.0;
		wspx = outre._rows/outre._cols;
	} else	{
		wspx = 1.0;
		wspy = outre._cols/outre._rows;
	}
	for(y=0;y<outre._rows;y++)
		for(x=0;x<outre._cols;x++)	{
			if(start>0)	{
				gaus = (1/(2*M_PI*start*start))*exp(-(pow( (y-centery)*wspy,2)+pow( (x-centerx)*wspx,2))/(2*start*start));
				gaus/=val;
				gaus = 1-gaus;
				outre.SetPixel(y,x,gaus*outre.GetPixel(y,x));
				outim.SetPixel(y,x,gaus*outim.GetPixel(y,x));
			}

			if(stop>0)	{
				gaus1 = (1/(2*M_PI*stop*stop))*exp(-(pow( (y-centery)*wspy,2)+pow( (x-centerx)*wspx,2))/(2*stop*stop));
				gaus1/=val1;

				outre.SetPixel(y,x,gaus1*outre.GetPixel(y,x));
				outim.SetPixel(y,x,gaus1*outim.GetPixel(y,x));
			}
		}
// --------------------------------- MOJ ----------------------------------------

	Swap(&outre,&outim,outre._cols,outre._rows);	

	out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * outre._cols*outre._rows);
	out_in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * outre._cols*outre._rows);
	for(a=0;a<outre.GetNumofElements();a++)	{
		out[a][0] = outre.data[a];
		out[a][1] = outim.data[a];
	}

	g1 = fftw_plan_dft_2d(outre._rows, outre._cols, out, out_in, FFTW_BACKWARD,FFTW_ESTIMATE);
	fftw_execute(g1);
	fftw_destroy_plan(g1);

	tmpoutre.AllocateData(outre._rows,outre._cols);
	tmpoutim.AllocateData(outre._rows,outre._cols);
	for(a=0;a<tmpoutre.GetNumofElements();a++)	{
		tmpoutre.data[a] = out_in[a][0];
		tmpoutim.data[a] = out_in[a][1];
	}


	output.AllocateData(image->_rows, image->_cols);
	output.Zeros();

	double lux;
	double luy;

	lux=0;luy=0;

	for(y=0;y<image->_rows;y++)
		for(x=0;x<image->_cols;x++)
			output.SetPixel(y,x,sqrt(pow(tmpoutre.GetPixel(y+(unsigned long)luy,x+(unsigned long)lux),2)+pow(tmpoutim.GetPixel(y+(unsigned long)luy,x+(unsigned long)lux),2)));

//output.DumpBinary("c:\\output_raw.txt");

	outtab[0]->AssignValue((void*)&output,err);
	outtab[1]->AssignValue((void*)&output,err);
	fftw_free(out_in);
	fftw_free(out);
	fftw_cleanup();
}

void C_Command_Fftfilt::FlipLine(unsigned int imsize,unsigned int orgsize,double* inre,double* inim,fftw_complex* out)
{
	unsigned int ilerazy;
	double reszta;
	short kier;
	unsigned int start,stop,poz,licz;
	long a;

	ilerazy = (unsigned int)floor((double)imsize/(double)orgsize);
	reszta = imsize - ilerazy*orgsize;

	for(a=0;a<orgsize;a++)	{
			out[a][0] = inre[a];
			out[a][1] = inim[a];
		}
	a = 0;
	kier = -1;
	poz = orgsize;
	start = orgsize-1;
	stop = 0;
	while(ilerazy-1>0)	{
		licz = 0;
		while(licz<orgsize-1)	{
			ASSERT(poz<imsize);
			ASSERT(start<orgsize);
			out[poz][0] = inre[start];
			out[poz][1] = inim[start];
			poz++;
			start+=kier;
			licz++;
		}
		kier = -(kier);
		if(kier>0)	{
			start =0;
			stop = orgsize-1;
		} else {
			start = orgsize-1;
			stop = 0;
		}
		ilerazy--;
	}
	while(reszta>0)	{
		ASSERT(poz<imsize);
		ASSERT(start<orgsize);
		out[poz][0] = inre[start];
		out[poz][1] = inim[start];
		poz++;
		start+=kier;
		reszta--;
	}
}


void C_Command_Fftfilt::filterLargeSmall(double* outre, int maxn,double filterLarge, double filterSmall, int stripesHorVert, double scaleStripes)
{
                
	int maxN = maxn;
                        
	double* fht = outre;
    double* filter = new double[maxN*maxN];
    for(unsigned int i=0; i<maxN*maxN; i++)
		filter[i] = 1.0;           

    int row;
    int backrow;
    double rowFactLarge;
    double rowFactSmall;
                
    int col;
    int backcol;
    double factor;
    double colFactLarge;
    double colFactSmall;
                
    double factStripes;
                
                // calculate factor in exponent of Gaussian from filterLarge / filterSmall

    double scaleLarge = filterLarge*filterLarge;
    double scaleSmall = filterSmall*filterSmall;
    scaleStripes = scaleStripes*scaleStripes;


                // loop over rows
                for (int j=1; j<maxN/2; j++) {
                        row = j * maxN;
                        backrow = (maxN-j)*maxN;
                        rowFactLarge = exp(-(j*j) * scaleLarge);
                        rowFactSmall = exp(-(j*j) * scaleSmall);
                        

                        // loop over columns
                        for (col=1; col<maxN/2; col++){
                                backcol = maxN-col;
                                colFactLarge = exp(- (col*col) * scaleLarge);
                                colFactSmall = exp(- (col*col) * scaleSmall);
                                factor = (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall;
                                switch (stripesHorVert) {
                                        case 1: factor *= (1 - exp(- (col*col) * scaleStripes)); break;// hor stripes
                                        case 2: factor *= (1 - exp(- (j*j) * scaleStripes)); // vert stripes
                                }
                                
                                fht[col+row] *= factor;
                                fht[col+backrow] *= factor;
                                fht[backcol+row] *= factor;
                                fht[backcol+backrow] *= factor;
                                filter[col+row] *= factor;
                                filter[col+backrow] *= factor;
                                filter[backcol+row] *= factor;
                                filter[backcol+backrow] *= factor;
                        }
                }

                //process meeting points (maxN/2,0) , (0,maxN/2), and (maxN/2,maxN/2)
                int rowmid = maxN * (maxN/2);
                rowFactLarge = exp(- (maxN/2)*(maxN/2) * scaleLarge);
                rowFactSmall = exp(- (maxN/2)*(maxN/2) * scaleSmall);      
                factStripes = exp(- (maxN/2)*(maxN/2) * scaleStripes);
                
                fht[maxN/2] *= (1 - rowFactLarge) * rowFactSmall; // (maxN/2,0)
                fht[rowmid] *= (1 - rowFactLarge) * rowFactSmall; // (0,maxN/2)
                fht[maxN/2 + rowmid] *= (1 - rowFactLarge*rowFactLarge) * rowFactSmall*rowFactSmall; // (maxN/2,maxN/2)
                filter[maxN/2] *= (1 - rowFactLarge) * rowFactSmall; // (maxN/2,0)
                filter[rowmid] *= (1 - rowFactLarge) * rowFactSmall; // (0,maxN/2)
                filter[maxN/2 + rowmid] *= (1 - rowFactLarge*rowFactLarge) * rowFactSmall*rowFactSmall; // (maxN/2,maxN/2)

                switch (stripesHorVert) {
                        case 1: fht[maxN/2] *= (1 - factStripes);
                                        fht[rowmid] = 0;
                                        fht[maxN/2 + rowmid] *= (1 - factStripes);
                                        filter[maxN/2] *= (1 - factStripes);
                                        filter[rowmid] = 0;
                                        filter[maxN/2 + rowmid] *= (1 - factStripes);
                                        break; // hor stripes
                        case 2: fht[maxN/2] = 0;
                                        fht[rowmid] *=  (1 - factStripes);
                                        fht[maxN/2 + rowmid] *= (1 - factStripes);
                                        filter[maxN/2] = 0;
                                        filter[rowmid] *=  (1 - factStripes);
                                        filter[maxN/2 + rowmid] *= (1 - factStripes);
                                        break; // vert stripes
                }               
                
                //loop along row 0 and maxN/2   
                rowFactLarge = exp(- (maxN/2)*(maxN/2) * scaleLarge);
                rowFactSmall = exp(- (maxN/2)*(maxN/2) * scaleSmall);                      
                for (col=1; col<maxN/2; col++){
                        backcol = maxN-col;
                        colFactLarge = exp(- (col*col) * scaleLarge);
                        colFactSmall = exp(- (col*col) * scaleSmall);
                        
                        switch (stripesHorVert) {
                                case 0:
                                        fht[col] *= (1 - colFactLarge) * colFactSmall;
                                        fht[backcol] *= (1 - colFactLarge) * colFactSmall;
                                        fht[col+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall;
                                        fht[backcol+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall;
                                        filter[col] *= (1 - colFactLarge) * colFactSmall;
                                        filter[backcol] *= (1 - colFactLarge) * colFactSmall;
                                        filter[col+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall;
                                        filter[backcol+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall;  
                                        break;                  
                                case 1:
                                        factStripes = exp(- (col*col) * scaleStripes);
                                        fht[col] *= (1 - colFactLarge) * colFactSmall * (1 - factStripes);
                                        fht[backcol] *= (1 - colFactLarge) * colFactSmall * (1 - factStripes);
                                        fht[col+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                                        fht[backcol+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                                        filter[col] *= (1 - colFactLarge) * colFactSmall * (1 - factStripes);
                                        filter[backcol] *= (1 - colFactLarge) * colFactSmall * (1 - factStripes);
                                        filter[col+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                                        filter[backcol+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                                        break;
                                case 2:
                                        factStripes = exp(- (maxN/2)*(maxN/2) * scaleStripes); 
                                        fht[col] = 0;
                                        fht[backcol] = 0;
                                        fht[col+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                                        fht[backcol+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                                        filter[col] = 0;
                                        filter[backcol] = 0;
                                        filter[col+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                                        filter[backcol+rowmid] *= (1 - colFactLarge*rowFactLarge) * colFactSmall*rowFactSmall * (1 - factStripes);
                        }
                }
                
                // loop along column 0 and maxN/2
                colFactLarge = exp(- (maxN/2)*(maxN/2) * scaleLarge);
                colFactSmall = exp(- (maxN/2)*(maxN/2) * scaleSmall);
                for (int j=1; j<maxN/2; j++) {
                        row = j * maxN;
                        backrow = (maxN-j)*maxN;
                        rowFactLarge = exp(- (j*j) * scaleLarge);
                        rowFactSmall = exp(- (j*j) * scaleSmall);

                        switch (stripesHorVert) {
                                case 0:
                                        fht[row] *= (1 - rowFactLarge) * rowFactSmall;
                                        fht[backrow] *= (1 - rowFactLarge) * rowFactSmall;
                                        fht[row+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall;
                                        fht[backrow+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall;
                                        filter[row] *= (1 - rowFactLarge) * rowFactSmall;
                                        filter[backrow] *= (1 - rowFactLarge) * rowFactSmall;
                                        filter[row+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall;
                                        filter[backrow+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall;
                                        break;
                                case 1:
                                        factStripes = exp(- (maxN/2)*(maxN/2) * scaleStripes);
                                        fht[row] = 0;
                                        fht[backrow] = 0;
                                        fht[row+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);
                                        fht[backrow+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);
                                        filter[row] = 0;
                                        filter[backrow] = 0;
                                        filter[row+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);
                                        filter[backrow+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);
                                        break;
                                case 2:
                                        factStripes = exp(- (j*j) * scaleStripes);
                                        fht[row] *= (1 - rowFactLarge) * rowFactSmall * (1 - factStripes);
                                        fht[backrow] *= (1 - rowFactLarge) * rowFactSmall * (1 - factStripes);
                                        fht[row+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);
                                        fht[backrow+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);
                                        filter[row] *= (1 - rowFactLarge) * rowFactSmall * (1 - factStripes);
                                        filter[backrow] *= (1 - rowFactLarge) * rowFactSmall * (1 - factStripes);
                                        filter[row+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);
                                        filter[backrow+maxN/2] *= (1 - rowFactLarge*colFactLarge) * rowFactSmall*colFactSmall * (1 - factStripes);      
                        }
                }
				delete[] filter;
		}     


void C_Command_Fftfilt::Swap(C_Matrix_Container* outre, C_Matrix_Container* outim,unsigned int imsizec,unsigned int imsizer)
{
	C_Matrix_Container mtmpre,mtmpim;
	mtmpre.AllocateData(imsizer/2,imsizec/2);
	mtmpim.AllocateData(imsizer/2,imsizec/2);
	unsigned long licznik,y,x;

	licznik = 0;
	for(y=0;y<imsizer/2;y++)
		for(x=0;x<imsizec/2;x++)	{
			mtmpre.data[licznik] = outre->GetPixel(y,x);
			outre->SetPixel(y,x,outre->GetPixel(y+imsizer/2,x+imsizec/2));
			mtmpim.data[licznik] = outim->GetPixel(y,x);
			outim->SetPixel(y,x,outim->GetPixel(y+imsizer/2,x+imsizec/2));
			licznik++;
		}
	for(y=0;y<imsizer/2;y++)
		for(x=0;x<imsizec/2;x++)	{
			outre->SetPixel(y+imsizer/2,x+imsizec/2,mtmpre.GetPixel(y,x));
			outim->SetPixel(y+imsizer/2,x+imsizec/2,mtmpim.GetPixel(y,x));
		}

// 2 z 3 
	licznik = 0;
	for(y=0;y<imsizer/2;y++)
		for(x=imsizec/2;x<imsizec;x++)	{
			mtmpre.data[licznik] = outre->GetPixel(y,x);
			outre->SetPixel(y,x,outre->GetPixel(y+imsizer/2,x-imsizec/2));
			mtmpim.data[licznik] = outim->GetPixel(y,x);
			outim->SetPixel(y,x,outim->GetPixel(y+imsizer/2,x-imsizec/2));
			licznik++;
		}
	for(y=imsizer/2;y<imsizer;y++)
		for(x=0;x<imsizec/2;x++)	{
			outre->SetPixel(y,x,mtmpre.GetPixel(y-imsizer/2,x));
			outim->SetPixel(y,x,mtmpim.GetPixel(y-imsizer/2,x));
		}
}

inline unsigned long C_Command_Fftfilt::cor2ind(unsigned int rows, unsigned int cols, unsigned int actrow, unsigned int actcol)
{
	return actrow * cols + actcol;
}


/*
void C_Command_Fftfilt::Do(C_Error& err)
{
	C_Image_Container *image;
	fftw_complex *out, *out_in;
	fftw_complex* in;
	fftw_plan g, g1;
	unsigned long ret;
	unsigned long x,y;
	double tmp;

	C_Image_Container output;

	C_Matrix_Container *p_start;
	C_Matrix_Container *p_stop;
	double start, stop;
	
	image = (C_Image_Container*)intab[0]->data;
	p_start = (C_Matrix_Container*)intab[1]->data;
	p_stop = (C_Matrix_Container*)intab[2]->data;
	start = p_start->data[0];
	stop = p_stop->data[0];

	if(start>=stop)	{
		err.SetError("fftfilt: cutoff1 should be smaller than cutoff2");
		return;
	}
	in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * image->_cols*image->_rows);
    out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * image->_cols*image->_rows);
	out_in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * image->_cols*image->_rows);
	for(unsigned long a=0;a<image->GetNumofElements();a++)	{
		in[a][0] = image->data[a];
		in[a][1] = 0.0;
	}
	g = fftw_plan_dft_2d(image->_rows, image->_cols, in, out, FFTW_FORWARD,FFTW_ESTIMATE);
	fftw_execute(g);

	for(x=(unsigned long)floor(image->_rows*start);x<(unsigned long)floor(image->_rows*stop);x++)		//504
		for(y=(unsigned long)floor(image->_cols*start);y<(unsigned long)floor(image->_cols*stop);y++)	{	//768
			ret = cor2ind(image->_rows, image->_cols, x, y);
			out[ret][0] = 0.0;
			out[ret][1] = 0.0;
		}


	g1 = fftw_plan_dft_2d(image->_rows, image->_cols, out, out_in, FFTW_BACKWARD,FFTW_ESTIMATE);
	fftw_execute(g1);

	output.AllocateData(image->_rows, image->_cols);

	for(unsigned long a=0;a<image->GetNumofElements();a++)	{
		tmp = sqrt(pow(out_in[a][0],2) + pow(out_in[a][1],2));
		output.data[a] = tmp;
	}
	outtab[0]->AssignValue((void*)&output,err);
	fftw_destroy_plan(g);
	fftw_destroy_plan(g1);
    fftw_free(in); 
    fftw_free(out);
	fftw_free(out_in);
}
*/