#include "Cpp_Lib.h"

extern "C" int  curve_fit(void (*funcPrt)(double*, double*, int, int, void*),
			  char funcName[20],
			  double *YData, 
			  double *XData,
			  double *LowerBounds,
			  double *UpperBounds,
			  int DataSize,
			  int ItNR,
			  int ParamNR, 
			  double *Param,
			  double *opts);
void FastMedian(C_Image_Container *scr,C_Image_Container *dst,int MaskRows,int MaskCols);
void quickSort(double *Array,int *ArrayIndex, int array_size,bool AttachArrayIndex);
void q_sort(double *Array,int *ArrayIndex, int left, int right,bool AttachArrayIndex);
void FuncFitting(C_Image_Container *scr,
			C_Image_Container *dst,
			int FuncModel,
			double xDataScale,
			double yDataScale,
			double paramBoundsPercent,
			bool yRef,
			int dy,
			int MedFilt,
			double *paramTabDouble);
void InitializeParamValue(double *p,int ParamNR,double InitialValue);
void InitializeParamBoundsValue(double *paramResults,double *paramResultsBoundsLow,double *paramResultsBoundsUp,int ParamNR,double paramBoundsPercent);

void Maximum(double *Value,int *postion, double *vect,int vectSize);
void SpecValue(int *position, double *vect,int vectSize,double SpecValue);
void detectROI(C_Image_Container *scr,
			C_Matrix_Container *resultTab,
			int dy,
			int Xhaa,
			int Res,
			int Hup,
			int Hdown,
			char *file
			);
void SimpleDetect(C_Image_Container *dst,
				  int dy,
			      int Xhaa,
			      int Res,
			      int Hup,
				   int Hdown,
				  C_Matrix_Container *resultTab);

void MedianValue(C_Matrix_Container *resultTabMedian,C_Matrix_Container *resultTab,int x_size,int y_size);
void readParamFromFile(double *paramTabDouble,char *file);
void LowingEdge(C_Image_Container *Image,int omitPix,int dy);
