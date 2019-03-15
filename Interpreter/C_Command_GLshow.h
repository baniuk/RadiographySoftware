#pragma once
#include "c_commands.h"
#include <gl.h>
#include <glaux.h>
#include "_OpenGL.h"

class C_Command_GLshow :
	public C_Commands
{
public:
	C_Command_GLshow(void);
	C_Command_GLshow(CWnd* window);
	void short2byte(unsigned short data, unsigned char &low, unsigned char &up);
	virtual void Do(C_Error& err);
public:
	virtual ~C_Command_GLshow(void);
private:
	C_OpenGL* handle;	
	CWnd* parentwindow;
	unsigned char *obraz;
};
