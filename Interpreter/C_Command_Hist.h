#pragma once
#include "c_commands.h"
#include <gl.h>
#include <glaux.h>
#include "_OpenGL_Hist.h"
#include "C_Matrix_Container.h"

class C_Command_Hist :
	public C_Commands
{
public:
	C_Command_Hist(void);
	C_Command_Hist(CWnd* window);
	virtual void Do(C_Error& err);
	virtual ~C_Command_Hist(void);
private:
	C_OpenGL_Hist* handle;	
	CWnd* parentwindow;
	C_Matrix_Container hist;		// wyniki
};
