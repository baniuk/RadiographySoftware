#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#include <windows.h>
#include "..\interpreter\C_Matrix_Container.h"
#include "..\interpreter\C_Image_Container.h"
struct FUNC_DEF {
	int version;
	char name[20];
} func;

// Put your includes here 
#include <math.h>
#include <string.h>

void Introduce(FUNC_DEF* definition);
void Do(void);