#pragma once
#include "c_commands.h"
#include <cv.h>
#include <highgui.h>
#include <cxcore.h>
#include <tiffio.h>

class C_Command_Imread :
	public C_Commands
{
public:
	C_Command_Imread(void);
	virtual void Do(C_Error& err);
public:
	virtual ~C_Command_Imread(void);
};
