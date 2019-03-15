inline void CDIB::WritePixel (UINT x,UINT y,COLORREF c)
{
    m_Bits[x + y * m_Size.cx] = c;
}

inline CDIB::CDIB ()
{
    m_Bits=NULL;
    m_Bitmap=NULL;
    m_Size=CSize ( 0, 0 );
}

inline CDIB::~CDIB ()
{
    Destroy ();
}

inline void CDIB::PasteToDC ( CDC *pDC, int x, int y )
{
    // x,y - the DC coordinates to start from
    SetDIBitsToDevice ( pDC->m_hDC, x, y, m_Size.cx, m_Size.cy, 0, 0, 0, m_Size.cy, m_Bits, &m_Info, 0); 
}

inline void CDIB::GetFromBitmap ( CDC *pDC, CBitmap *pBitmap )
{
    if ( m_Bitmap )
        GetDIBits ( pDC->m_hDC, HBITMAP(*pBitmap), 0, m_Size.cy, m_Bits, &(m_Info), DIB_RGB_COLORS ); 
}

inline void CDIB::CopyTo ( CDIB *Dib )
{
    // If DibSize Wrong Re-Create Dib
    if ( (Dib->m_Size.cx!=m_Size.cx) || (Dib->m_Size.cy!=m_Size.cy) )
        Dib->Create ( m_Size.cx, m_Size.cy );
    // do Copy
    memcpy ( Dib->m_Bits, m_Bits, m_Size.cx*m_Size.cy*4 );
}

inline void CDIB::CopyFrom ( const CDIB *Dib )
{
    // If DibSize Wrong Re-Create Dib
    if ( (m_Size.cx!=Dib->m_Size.cx) || (m_Size.cy!=Dib->m_Size.cy) )
        Create ( Dib->m_Size.cx, Dib->m_Size.cx );
    // do Paste
    memcpy ( m_Bits, Dib->m_Bits, m_Size.cx*m_Size.cy*4 );
}

inline COLORREF &
CDIB::ColorAt (int x, int y)
{
    ASSERT ((x >= 0) && (x <= m_Size.cx) &&
            (y >= 0) && (y <= m_Size.cy));
    return m_Bits[x + y * m_Size.cx];
}    

inline BOOL 
CDIB::operator != (const CDIB &dib) const
{
    return !(*this == dib);
}

