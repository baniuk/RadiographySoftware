// _Punkty.h: interface for the C_Punkty class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__PUNKTY_H__FB719B1F_5B7A_4C07_8244_48E7DCB5D4DD__INCLUDED_)
#define AFX__PUNKTY_H__FB719B1F_5B7A_4C07_8244_48E7DCB5D4DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C_Punkty  
{
public:
	C_Punkty(int _x,int _y,int _z);
	virtual ~C_Punkty();
	int x,y,z;

};

#endif // !defined(AFX__PUNKTY_H__FB719B1F_5B7A_4C07_8244_48E7DCB5D4DD__INCLUDED_)
