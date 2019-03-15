#include "StdAfx.h"
#include "C_Command_IQI.h"

C_Command_IQI::C_Command_IQI(void)
{
	UINT parint[1] = {IMAGE};
	UINT parout[1] = {IMAGE};
	InitCommand("iqi",1,1,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "IMAGE = iqi(IMAGE)";
	fun_ver = "0.2";
}

C_Command_IQI::~C_Command_IQI(void)
{	
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}
void C_Command_IQI::Do(C_Error& err)
{
	C_Image_Container *image;
	C_Image_Container output;
	int M, T, R;
	double Delta_x, x_min, Delta_theta, rho_min, Delta_rho, theta_min;
	double setpar[9];

	image = (C_Image_Container*)intab[0]->data;
	M = image->_cols;
	Delta_x = 1.0;
	x_min = -(M-1)*0.5*Delta_x;
	T = (int)ceil(M_PI*(M-1));
	Delta_rho = Delta_x/sqrt(2.0);
	R = 2*M-1;
	rho_min = -Delta_rho*(R-1)*0.5;
	theta_min = 0;
	setpar[0]  =      M;     
	setpar[1]  =      x_min; 
	setpar[2]  =      Delta_x;
	setpar[3]  =      T;      
	setpar[4]  =      Delta_theta;   //inicjalizacja BRAK
	setpar[5]  =      R;       
	setpar[6]  =      rho_min;
	setpar[7]  =      Delta_rho;
	setpar[8]  =      theta_min; 
	
	oradon(output.data,image->data,setpar);

	outtab[0]->AssignValue((void*)&output,err);

}

double C_Command_IQI::sinc(double x)
{

  if (x<0) x=-x;
  if (x<1e-10) 
    return 1.0;
  else
    return sin(x)/x;
}

void C_Command_IQI::oradon(double *out,double *in,double *setpar)
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
}