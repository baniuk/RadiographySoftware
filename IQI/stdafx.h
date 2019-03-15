#pragma once

#include "Cpp_Lib.h"
#include "Cell.h"

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }