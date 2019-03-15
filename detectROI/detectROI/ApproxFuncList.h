void gauss(double *p, double *x, int m, int n, void *data);
void gaussComplex(double *p, double *x, int m, int n, void *data);

 class FunctionModel {
	public:
		void (*funcPrt)(double*, double*, int, int, void*);
		int numOfParam;
		char funcName[20];
		};
 class FunctionModel *models[]; 
 
