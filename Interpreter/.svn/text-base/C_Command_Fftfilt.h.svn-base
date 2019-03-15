#pragma once
#include "c_commands.h"
#include "C_Image_Container.h"
#include <fftw3.h>

class C_Command_Fftfilt :
	public C_Commands
{
public:
	C_Command_Fftfilt(void);
	virtual void Do(C_Error& err);
	virtual ~C_Command_Fftfilt(void);
private:
	unsigned long cor2ind(unsigned int rows, unsigned int cols, unsigned int actrow, unsigned int actcol);
	void Swap(C_Matrix_Container* outre, C_Matrix_Container* outim,unsigned int imsizec,unsigned int imsizer);
	void filterLargeSmall(double* outre, int maxn, double filterLarge, double filterSmall, int stripesHorVert, double scaleStripes);
	void FlipLine(unsigned int imsize,unsigned int orgsize,double* inre,double* inim,fftw_complex* out);

};
