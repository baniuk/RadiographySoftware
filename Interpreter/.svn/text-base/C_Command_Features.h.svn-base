#pragma once
#include "c_commands.h"
#include "..\interpreter\C_Image_Container.h"

class C_Command_Features :
	public C_Commands
{
public:
	C_Command_Features(void);
	virtual void Do(C_Error& err);
	virtual ~C_Command_Features(void);
private:
	void Outline(C_Image_Container* image,C_Image_Container* outln);
};
