#include "Lib.h"
#include "..\interpreter\C_Matrix_Container.cpp"
#include "..\interpreter\C_Image_Container.cpp"

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,20,"Funkcja");
	definition->version = 10;

}
void Do(void)
{
	C_Matrix_Container image;
	image.AllocateData(100,100);
	image.FreeData();
}