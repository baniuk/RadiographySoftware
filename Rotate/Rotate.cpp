// Rotate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Rotate.h"
#include "RotateByShear.h"
#include "dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//
// Sample implementation :
// -----------------------
// This implementation uses COLORREF for a pixel.
// This implementation fits the DIB data structure.
//
class CRotateByShearRGB : public CRotateByShear<COLORREF>
{
public: 
    CRotateByShearRGB (ProgressAnbAbortCallBack callback = NULL) : 
        CRotateByShear<COLORREF> (callback) {}

    virtual ~CRotateByShearRGB() {}

    COLORREF GetRGB (COLORREF *pImage,      // Pointer to image
                     SIZE  sImage,      // Size of image
                     UINT  x,           // X coordinate
                     UINT  y            // Y coordinate
                    )
    {
        return pImage [x + y * sImage.cx];
    }

        // Set RGB value at given pixel coordinates
    void SetRGB (COLORREF  *pImage,   // Pointer to image
                 SIZE       sImage,   // Size of image
                 UINT       x,        // X coordinate
                 UINT       y,        // Y coordinate
                 COLORREF   clr       // New color to set
                )
    {
        pImage [x + y * sImage.cx] = clr;
    }

        // Create a new bitmap, given a bitmap dimensions
    COLORREF *CreateNewBitmap (SIZE  sImage) // Size of image
    {
        return new COLORREF [sImage.cx * sImage.cy];
    }

        // Create a new bitmap which is an identical copy of the source bitmap
    COLORREF *CopyBitmap (COLORREF *pImage,     // Pointer to source image
                          SIZE      sImage      // Size of source (and destination) image
                         )
    {
        COLORREF *pDst =  CreateNewBitmap (sImage);
        if (NULL != pDst)
        {
            memcpy (pDst, pImage, sizeof (COLORREF) * sImage.cx * sImage.cy);
        }
        return pDst;
    }          

        // Destroy a bitmap previously created in call to CreateNewBitmap(..)
        // or to CopyBitmap (...)
    void DestroyBitmap (COLORREF *pImage,   // Pointer to image
                        SIZE                // Size of image
                       )
    {
        delete [] pImage;
    }

};
    



/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

BOOL Progress (BYTE bPercentComplete)
{
    printf ("%d percent completed...\r", bPercentComplete);
    return TRUE;
    // If we return FALSE here, the rotation will be immediately aborted
}


void RotateFile (int argc, char *argv[])
{
    if (argc != 4)
    {
        printf ("Usage: Rotate <source file> <destination file> <roation angle>\n"
                "e.g.: Rotate src.ppm dst.ppm 47.3\n");
        exit (1);
    }
    double dAngle = atof (argv[3]);
    if (0.0 == dAngle)
    {
        printf ("Cannot parse rotation angle or roation angle is zero.\n");
        exit (1);
    }

    CDIB Src;

    if (DIB_LOAD_OK != Src.Load (argv[1]))
    {
        printf ("Cannot read from source file %s.\n", argv[1]);
        exit (1);
    }

    SIZE sDst;

    printf ("Attempting to rotate %s by %f degrees into %s.\n", argv[1], dAngle, argv[2]);

    CRotateByShearRGB Rot(Progress);
    COLORREF *pDst = Rot.AllocAndRotate (
                                        Src.m_Bits,
                                        Src.m_Size,
                                        dAngle,
                                        &sDst,
                                        RGB(0,0,0)
                                    );
    if (NULL == pDst)
    {
        printf ("Cannot allocate memory for operation.\n");
        exit (1);
    }
    {
        CDIB Dst;
        Dst.Create (sDst.cx, sDst.cy);

        Dst.m_Bits = pDst;

        if (DIB_SAVE_OK != Dst.SavePPM (argv[2]))
        {
            printf ("Cannot save rotated image to %s.\n", argv[2]);
            exit (1);
        }
    }
    Rot.DestroyBitmap (pDst, sDst);
    printf ("Success.\n");
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
        RotateFile (argc, argv);
	}

	return nRetCode;
}


