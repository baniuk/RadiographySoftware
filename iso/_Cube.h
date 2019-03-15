// _Cube.h: interface for the C_Cube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__CUBE_H__ED565721_A1B8_11D4_BB95_C55C8338C269__INCLUDED_)
#define AFX__CUBE_H__ED565721_A1B8_11D4_BB95_C55C8338C269__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class C_Cube  
{
public:
	C_Cube();
	unsigned char cube[bok*bok*bok];
	CString wepath;
//	CString wepathpath;
	char mode;
	BOOL czyZaladowane;
	virtual ~C_Cube();


};

#endif // !defined(AFX__CUBE_H__ED565721_A1B8_11D4_BB95_C55C8338C269__INCLUDED_)
