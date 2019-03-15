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

#include "lm.h"


/* Sample functions to be minimized with LM and their jacobians.
 * More test functions at http://www.csit.fsu.edu/~burkardt/f_src/test_nls/test_nls.html
 * Check also the CUTE problems collection at ftp://ftp.numerical.rl.ac.uk/pub/cute/;
 * CUTE is searchable through http://numawww.mathematik.tu-darmstadt.de:8081/opti/select.html
 * CUTE problems can also be solved through the AMPL web interface at http://www.ampl.com/TRYAMPL/startup.html
 */

void kwadrat(double *p, double *x, int m, int n, void *data)
{
register int i;
	for(i=0;i<n;i++){
	x[i]=10*((p[0]+5)*(p[0]+5) + (p[1]+2)*(p[1]+2));
	}
}

void blad(double *p, double *x, int m, int n, double *data)
{
register int i,j;


	for(i=0;i<n;i++){
		
			x[i] =  p[0]*exp(-p[1]*pow(i-p[2],2));

		}
	

}



int makeLM()
{
register int i, j;
int problem, ret;
double p[5], // 6 is max(2, 3, 5)
	   x[2000]; // 16 is max(2, 3, 5, 6, 16)
double *data;
double *low,*up;
int m, n;
double opts[LM_OPTS_SZ], info[LM_INFO_SZ];
char *probname[]={
	"Kwadrat",
	"blad",
	"blad2"
	
    
};

  opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
  opts[4]=LM_DIFF_DELTA; // relevant only if the finite difference jacobian version is used 

  /* uncomment the appropriate line below to select a minimization problem */
  problem= 2;
		  

  switch(problem){
  default: fprintf(stderr, "unknown problem specified (#%d)! Note that some minimization problems require LAPACK.\n", problem);
           exit(1);
    break;
	
  case 0:
	  m=2; n=2;
	p[0]=-1.0; p[1]=1.0;
    for(i=0; i<n; i++) x[i]=0.0;
	ret=dlevmar_dif(kwadrat, p, x, m, n, 10000, opts, info, NULL, NULL, NULL);
	break;

  case 1:
	  data =(double *)malloc(10*sizeof(double));
		m=3; n=100;

	p[0]=0.0; p[1]=0; p[2]=0.0;

	for(i=0;i<n;i++)
	{
		x[i] = 2*exp(-5*pow(i-50,2));

	}
  
	ret=dlevmar_dif(blad, p, x, m, n, 10000, opts, info, NULL, NULL, NULL);
	break;
	

	case 2:
	  data =(double *)malloc(10*sizeof(double));
		m=3; n=100;

	p[0]=0.0; p[1]=0; p[2]=0.0;

	for(i=0;i<n;i++)
	{
		x[i] = 10*exp(-0.5*pow(i-50,2));

	}
	
	 low =(double *)malloc(3*sizeof(double));
	 up =(double *)malloc(3*sizeof(double));

  	*(low+0) = 0; *(low+1)=0; *(low+2)=0;
	*(up+0) = 100; *(up+1)=100; *(up+2) = 100;
	ret=dlevmar_bc_dif(blad, p, x, m, n,low,up, 100000, opts, info, NULL, NULL, NULL);
	break;
	
 
  } /* switch */
  
  printf("Results for %s:\n", probname[problem]);
  printf("Levenberg-Marquardt returned %d in %g iter, reason %g\nSolution: ", ret, info[5], info[6]);
  for(i=0; i<m; ++i)
    printf("%.7g ", p[i]);
  printf("\n\nMinimization info:\n");
  for(i=0; i<LM_INFO_SZ; ++i)
    printf("%g ", info[i]);
  printf("\n");

  free (data);
  free (up);
  free(low);
  return 0;
}
