#include <math.h>
#include "FunctionList.h"
#include "ApproxFuncList.h"


class FunctionModel BasicGauss =
	{
		&gauss,
		3,
		"BasicGauss"
		
	};
class FunctionModel ComplexGauss =
	{
		&gaussComplex,
		5,
		"ComplexGauss"
	};


 class FunctionModel *models[] =
{	
	&BasicGauss,
	&ComplexGauss

};
void gauss(double *p, double *x, int m, int n, void *d)
{
register int i;
double a=*(p+0);
double b=*(p+1);
double x0=*(p+2);
double *data;

data = reinterpret_cast<double*>(d);


	for(i=0;i<n;i++){
			
			
			x[i] =  a*exp(-b*pow(*(data+i)-x0,2));
			//*(x+i) =  a*exp(-b*pow((double)i-x0,2));

		}
}
void gaussComplex(double *p, double *x, int m, int n, void *d)
{
	register int i;
	double	a1=*(p+0);
    //double  b1=*(p+1);
    double  c1=*(p+1);
    double  c2=*(p+2); 
    double  x0=*(p+3);
    double  dx=*(p+4);
	double *data;

data = reinterpret_cast<double*>(d);

	for(i=0;i<n;i++){

		x[i] = a1*exp(-(c1*pow(*(data+i)-x0,4)+c2*pow(*(data+i)-dx*x0,2)));

	}

}