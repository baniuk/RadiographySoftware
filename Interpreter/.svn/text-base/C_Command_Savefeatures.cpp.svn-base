#include "StdAfx.h"
#include "C_Command_Savefeatures.h"

using namespace std;

C_Command_Savefeatures::C_Command_Savefeatures(void)
{
	UINT parint[3] = {MATRIX,NUMBER,STRING};
	UINT parout[1] = {};
	InitCommand("savefeatures",3,0,parint,parout);
	intab = new C_Variables*[numofinparam];
	outtab = new C_Variables*[numofoutparam];
	syntax = "savefeatures(MATRIX,NUMBER,STRING)";
	fun_ver = "1.0";
}

C_Command_Savefeatures::~C_Command_Savefeatures(void)
{
	SAFE_DELETE(intab);
	SAFE_DELETE(outtab);
}

void C_Command_Savefeatures::Do(C_Error& err)
{
	char* path;
	int number;
	C_Matrix_Container *tab;
	C_Matrix_Container *p_number;

	tab = (C_Matrix_Container*)intab[0]->data;
	p_number = (C_Matrix_Container*)intab[1]->data;
	path = (char*)intab[2]->data;
	
	number = (int)p_number->data[0];
	
	int poz=13;
	ofstream plk_wy1(path);
	plk_wy1<<"cohx"<<" ";
	plk_wy1<<"cohy"<<" ";
	plk_wy1<<"radius1"<<" ";
	plk_wy1<<"radius2"<<" ";
	plk_wy1<<"alfa"<<" ";
	plk_wy1<<"field"<<" ";
	plk_wy1<<"perimeter"<<" ";

	plk_wy1<<"\n";
		for(int i=0;i<number;i++)
		{
			plk_wy1<<(tab->data[i*poz+2])<<" ";
			plk_wy1<<(tab->data[i*poz+3])<<" ";
			plk_wy1<<(tab->data[i*poz+6]/2)<<" ";
			plk_wy1<<(tab->data[i*poz+7]*2)<<" ";
			plk_wy1<<(tab->data[i*poz+8])<<" ";
			plk_wy1<<(tab->data[i*poz+9])<<" ";
			plk_wy1<<(tab->data[i*poz])<<" ";
			plk_wy1<<(tab->data[i*poz+1])<<" ";
			plk_wy1<<"\n";
		}
	plk_wy1.close();
}