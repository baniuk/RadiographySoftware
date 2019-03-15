#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define IMAGE  0x00000010
#define STRING 0x00000100
#define MATRIX 0x00001000
#define NUMBER MATRIX
#define DT UINT
#include <windows.h>
#include "..\interpreter\C_Matrix_Container.h"
#include "..\interpreter\C_Image_Container.h"
#include "..\interpreter\C_Error.h"
#include "..\interpreter\C_Variables.h"
#define MAX_VER 6
#define MAX_NAME 20
#define MAX_SYNT 256
struct FUNC_DEF {
	char version[MAX_VER];
	char name[MAX_NAME];
	char syntax[MAX_SYNT];
	int numofout;
	int numofin;
	UINT* typesofin;
	UINT* typesofout;
};

// Put your includes here 
#include <math.h>
#include <string.h>
#include <tiffio.h>

void Introduce(FUNC_DEF* definition);
void Do(C_Error& err,C_Variables** intab,C_Variables** outtab);
void RepairPath(char* Src, char* Dest);