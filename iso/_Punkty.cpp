// _Punkty.cpp: implementation of the C_Punkty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "iso.h"
#include "_Punkty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C_Punkty::C_Punkty(int _x,int _y,int _z)
{
	x=_x;
	y=_y;
	z=_z;
}

C_Punkty::~C_Punkty()
{

}
