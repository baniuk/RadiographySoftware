#ifndef _DIB_H
#define _DIB_H

typedef enum
{
    DIB_LOAD_OK,                // file read OK
    DIB_LOAD_NOFILE_OR_CORRUPT, // file not found or corrupted
    DIB_LOAD_NO_MEM             // Out of mem
} DIBLoadResult;

typedef enum
{
    DIB_SAVE_OK,                // file saved ok
    DIB_SAVE_CANT_WRITE,        // Cannot write to file
} DIBSaveResult;

class CDIB
{
public:
    COLORREF *m_Bits;
    BITMAPINFO m_Info;
    HBITMAP m_Bitmap;
    CSize m_Size;

    CDIB ();
    virtual ~CDIB ();

    void WritePixel (UINT x,UINT y,COLORREF c);

    DIBLoadResult Load (CString);
    DIBLoadResult LoadPPM (CString);


    DIBSaveResult SavePPM (CString, UINT uDescID = 0);
    DIBSaveResult SaveBMP (CString);


    BOOL Create (int Width, int Height );
    BOOL Create (CDC *pDC, UINT uBitmapID);   // From resource bitmap
    void Destroy ();

    void PasteToDC ( CDC *pDC, int x, int y );
    void GetFromDC ( CDC *pDC, int x, int y, int w, int h );
    void GetFromBitmap ( CDC *pDC, CBitmap *pBitmap );
    
    void Fill ( int R, int G, int B );
    void FillGlass ( int R, int G, int B, int A );
    void FillStippledGlass ( int R, int G, int B );

    void CopyTo   ( CDIB *Dib );
    void CopyFrom ( const CDIB *Dib );

    void CopyTransparentlyFrom ( CDIB *Dib, int R, int G, int B );

    void Add ( CDIB *Dib );
    void Blend ( CDIB *Dib, int A );
    void Darken ( CDIB *Dib );
    void Difference ( CDIB *Dib );
    void Lighten ( CDIB *Dib );
    void Multiply ( CDIB *Dib );
    void Screen ( CDIB *Dib );

    void CopyRect ( CDIB *Dib, int x, int y );
    void PasteRect ( CDIB *Dib, int x, int y );

    void PasteCKRect ( CDIB *Dib, int x, int y, int R, int G, int B );

    void FillRect ( int x, int y, int w, int h, int R, int G, int B );
    void FillGlassRect ( int x, int y, int w, int h, int R, int G, int B, int A );
    void FillStippledGlassRect ( int x, int y, int w, int h, int R, int G, int B );
    
    void AddRect ( CDIB *Dib, int x, int y );
    void BlendRect ( CDIB *Dib, int x, int y, int A );
    void DarkenRect ( CDIB *Dib, int x, int y );
    void DifferenceRect ( CDIB *Dib, int x, int y );
    void LightenRect ( CDIB *Dib, int x, int y );
    void MultiplyRect ( CDIB *Dib, int x, int y );
    void ScreenRect ( CDIB *Dib, int x, int y );

    void Line ( int x1, int y1, int x2, int y2, int R, int G, int B );
    void LineGlass ( int x1, int y1, int x2, int y2, int R, int G, int B, int A );

    void ClippedLine ( int x1, int y1, int x2, int y2, int R, int G, int B );

    COLORREF &ColorAt (int x, int y);

    BOOL operator == (const CDIB &) const;
    BOOL operator != (const CDIB &) const;
    CDIB &operator = (const CDIB &);


private:

};


// Inline sections:
#include "DIB.inl"

#endif
