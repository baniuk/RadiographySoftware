#pragma once
#include "tiffio.h"
#include <highgui.h>
#include <cxcore.h>

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }

class C_Image
{
public:
	C_Image(void);		// konstruktor uzywany do tworzenia obiektu 
	double* _data;		// obrazek
	unsigned long _cols;		// szeroko��
	unsigned long _rows;		// wysoko��
	BOOL ReadTiff(char* filename);	// wczytuje tiffa i zwraca true je�li wczyta lub false jesli nie
	void GetPixel(unsigned long row, unsigned long col,double &pixel);	// zwraca warto�� pixela {x,y}
	void SetPixel(unsigned long row, unsigned long col,double pixel);		// ustawia warto�� pixela
	unsigned long GetNumofElements(void);											// zwraca ilo�� pixeli w obrazku	
	void saveimage2matlab(char* filename);										// nagrywa obrazek w formacie matlaba
	char* ReturnIPLBuffor();													// zwraca wska�nik do bufora w formacie biblioteki IPL
	void AllocateData(unsigned int rows, unsigned int cols);					// Allokuje pami�� dla danego obrazka
	void CloneObject(C_Image* dest);											// kopiuje aktualny obiekt do dest (dest musi by � wcze�niej stworzone!!) 
	void Normalize(unsigned short w1, unsigned short w2);						// normalizuje do zakresu w1 i w2
	void getMinMax(double& min, double& max);									// znajduje minimum i maksimum
	void Zeros(void);															// zeruje tablic�
//	void Imshow(char *tmp, int binary);
	~C_Image(void);
private:
	TIFF* tif;
	char* _iplbuffor;
	void FreeData(void);														// Zwalnia pami��
};

/*
Uwagi:
1. Numeracja pixeli od zera
2. Obrazek jest umieszczony w tablicy _data rz�dami
3. Brak zabezpiecze� przed przekroczeniem zakresu row, col - trzeba uwa�a� - jak b�dzie potrzeba to dorobi�
4. W nazwach plik�w podajemy dwa znaki \ np. c:\\file,tif

Jak odczyta� zapisany obrazek w matlabie??
Ano tak:

fid = fopen('c:\test.bin','r');
x = 800;
y = 600;
test = fread(fid,x*y,'float');
test1=reshape(test,x,y)';

figure; imshow(test1,[]);
test1s = flipud(Scale01(test1));
figure; h=pcolor(test1s); colormap(gray); set(h,'EdgeColor','none');%set(gca,'DataAspectRatio',[1 1 1]);

fclose(fid);

*/