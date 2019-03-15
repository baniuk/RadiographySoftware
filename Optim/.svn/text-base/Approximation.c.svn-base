/////////////////////////////////////////////////////////////////////////////////
// 
//  Demonstration driver program for the Levenberg - Marquardt minimization
//  algorithm
//  Copyright (C) 2004-05  Manolis Lourakis (lourakis@ics.forth.gr)
//  Institute of Computer Science, Foundation for Research & Technology - Hellas
//  Heraklion, Crete, Greece.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
/////////////////////////////////////////////////////////////////////////////////

/******************************************************************************** 
 * Levenberg-Marquardt minimization demo driver. Only the double precision versions
 * are tested here. See the Meyer case for an example of verifying the jacobian 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <string.h>
#include <conio.h>


#include "lm.h"



/* Sample functions to be minimized with LM and their jacobians.
 * More test functions at http://www.csit.fsu.edu/~burkardt/f_src/test_nls/test_nls.html
 * Check also the CUTE problems collection at ftp://ftp.numerical.rl.ac.uk/pub/cute/;
 * CUTE is searchable through http://numawww.mathematik.tu-darmstadt.de:8081/opti/select.html
 * CUTE problems can also be solved through the AMPL web interface at http://www.ampl.com/TRYAMPL/startup.html
 */

int curve_fit(void (*funcPrt)(double*, double*, int, int, void*),
			  char funcName[20],
			  double *YData, 
			  double *XData,
			  double *LowerBounds,
			  double *UpperBounds,
			  int DataSize,
			  int ItNR,
			  int ParamNR, 
			  double *Param,
			  double *opts)
{
	
//register int i;
int ret;
double  info[LM_INFO_SZ];

//opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
 // opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference jacobian version is used 

 // opts[0]=1e-3; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20; // LM_INIT_MU = 1e-03
  //opts[4]=1e-6; // LM_DIFF_DELTA = 1e-06 // relevant only if the finite difference jacobian version is used  

  /*
double *work, *covar;
work=malloc((LM_DIF_WORKSZ(ParamNR, DataSize)+ParamNR*ParamNR)*sizeof(double));
covar=work+LM_DIF_WORKSZ(ParamNR, DataSize);*/

 ret=dlevmar_bc_dif(funcPrt, Param, YData, ParamNR, DataSize,LowerBounds,UpperBounds, ItNR, opts, info, NULL, NULL, XData);
 //ret=dlevmar_bc_dif(funcPrt, Param, YData, ParamNR, DataSize,0,0, ItNR, opts, info, work, covar, XData);

	/*
  printf("\nResults for %s:\n",funcName);
  printf("Levenberg-Marquardt returned %d in %g iter, reason %g\nSolution: ", ret, info[5], info[6]);
  for(i=0; i<ParamNR; ++i)
    printf("%.7g ", Param[i]);
  printf("\n\nMinimization info:\n");
  for(i=0; i<LM_INFO_SZ; ++i)
    printf("%g ", info[i]);
  printf("\n");*/
  

  return 0;
  
}
