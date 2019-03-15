//#include "StdAfx.h"
#include "R_Tiff.h"
#include "fcntl.h"
#include "stdlib.h"


R_Tiff::R_Tiff(void)
{
	picBuffer = NULL;
}

int R_Tiff::readTiffImage(char *filename)
/*--------------------------------------------------------------*
 *    Procedure to read data from the tiff's file into          *
 *              the buffer.                                     *
 *                                                              *
 *  Inputs :                                                    *
 *           - *fileName : name of the tiff's file              *
 *         ( - imageHeader : global stucture with information   *
 *                               about tiff's file.)            *
 *  Outputs :                                                   *
 *            - 0 : operaton failed, 1 : operation succeesed,   *
 *          ( - imageHeader : global stucture with information  *
 *                            about tiff's file.)               *
 *--------------------------------------------------------------*/

{
   unsigned int  closed;
   int           i, bytesRead, fileDesc;
   unsigned long area;
   unsigned long position;
/*--------------------------------------------------------------*
 *     Opening the tiff's file.                                 *
 *     Writing parameters :                                     *
 *      length, width, number bits per pixel,                   *
 *      offset to picture's data, to image structure.           *
 *--------------------------------------------------------------*/
   if ( R_Tiff::openTiffFile ( filename, &fileDesc ) )
   {
	   R_Tiff::readTiffHeader ( fileDesc );

      if ( imageHeader.bitsPerPixel == 16 )
      {
	 position  =  lseek ( fileDesc, imageHeader.stripOffset, 0 );
/*--------------------------------------------------------------*
 *     Estimation of the size (in bytes) of shown picture.      *
 *--------------------------------------------------------------*/
	 area = 2*imageHeader.imageWidth * imageHeader.imageLength;

	 unsigned int tmp_area, k;
	 int licz, reszta;
	 licz  = area / 65536;
	 picBuffer = new char[area];
	 reszta = area % 65536;

	 for (k = 0; k < licz+1; k++)
	 {
		bytesRead = read ( fileDesc, picBuffer + (k*65536), 65536 );
	 }
	 if (reszta)
		 bytesRead = read ( fileDesc, picBuffer + (--k*65536), reszta);

	 closed = close ( fileDesc );

	 return 1;
      }

      else
	 return 0;
    }
    else
       return 0;
}  /* ends read_tiff_image */


int R_Tiff::openTiffFile( char *fileName, int *fileDesc )

/*---------------------------------------------------------------*
 *    Function for TIFF file opening                             *
 *                                                               *
 *    Inputs:                                                    *
 *            - *fileName : pointer to name of the tiff's file,  *
 *            - *fileDesc : pointer to the adress of the opened  *
 *                          tiff's file handle.                  *
 *                                                               *
 *    Outputs:                                                   *
 *             - 0 : operation failed, 1 : operation succeesed,  *
 *             - *fileDesc : handle to the opened tiff's file    *
 *---------------------------------------------------------------*/

{

   *fileDesc = open ( fileName, O_RDONLY | O_BINARY );

   if ( *fileDesc == -1 )
      return 0;
   else
      return 1;
}
int R_Tiff::readTiffHeader(int fileDesc)

/*---------------------------------------------------------------*
 *  Function for TIFF header file reading                        *
 *                                                               *
 *  Inputs :                                                     *
 *           - fileDesc : handle to the opened tiff's file       *
 *         ( - imageHeader : global stucture with information    *
 *                               about tiff's file.)             *
 *  Outputs :                                                    *
 *            - 0 : operaton failed, 1 : operation succeesed,    *
 *          ( - imageHeader : global stucture with information   *
 *                            about tiff's file.)                *
 *---------------------------------------------------------------*/

{
   char   buffer[12];
   int    bytes_read, closed, i, j, lsb, not_finished;
   long   bits_per_pixel, image_length, image_width,
	  offset_to_ifd, position, strip_offset,  value ;
   short  entry_count, field_type, s_bits_per_pixel, s_image_length,
	  length_of_field,   subfile,
	  s_image_width, s_strip_offset, tag_type;

/*---------------------------------------------------------------*
 *    Determine if the file uses MSB first or LSB first          *
 *---------------------------------------------------------------*/
   bytes_read = read ( fileDesc, buffer, 8 );
   if ( buffer[0] == 0x49 )
      lsb = 1;
   else
      lsb = 0;

/*---------------------------------------------------------------*
 *    Read the offset to the IFD                                 *
 *---------------------------------------------------------------*/
   R_Tiff::extract_long_from_buffer ( buffer, lsb, 4, &offset_to_ifd );
   not_finished = 1;
   while ( not_finished )
   {

/*---------------------------------------------------------------*
 *    Seek to the ID and read the entry_count,                   *
 *    i.e. the number of entries in the IFD.                     *
 *---------------------------------------------------------------*/
      position = lseek ( fileDesc, offset_to_ifd, 0 );
      bytes_read = read ( fileDesc, buffer, 2 );
      R_Tiff::extract_short_from_buffer ( buffer, lsb, 0, &entry_count );

/*---------------------------------------------------------------*
 *    Now loop over the directory entries.                       *
 *    Look only for the tags we need. These are:                 *
 *       ImageLength                                             *
 *       ImageWidth                                              *
 *       BitsPerPixel (BitsPerSample)                            *
 *       StripOffset                                             *
 *---------------------------------------------------------------*/
      for ( i = 0; i < entry_count; i ++ )
      {
	 bytes_read = read ( fileDesc, buffer, 12 );
	 R_Tiff::extract_short_from_buffer ( buffer, lsb, 0, &tag_type );
	 switch( tag_type )
	 {
	    case 255:  /* Subfile Type */

	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );
	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 8, &subfile );

	       break;

	    case 256:  /* ImageWidth */

	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

	       if ( field_type == 3 )
	       {
		  R_Tiff::extract_short_from_buffer ( buffer, lsb, 8, &s_image_width );
		  image_width = s_image_width;
	       }

	       else
		  R_Tiff::extract_long_from_buffer ( buffer, lsb, 8, &image_width );
	       if ( ( image_width % 8 ) != 0 )
		  image_width += 8 - ( image_width % 8 );
	       break;

	    case 257:  /* ImageLength */

	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

	       if ( field_type == 3 )
	       {
		  R_Tiff::extract_short_from_buffer ( buffer, lsb, 8, &s_image_length );
		  image_length = s_image_length;
	       }
	       else
		  R_Tiff::extract_long_from_buffer ( buffer, lsb, 8, &image_length );

	       break;

	    case 258:  /* BitsPerPixel */

	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       R_Tiff::extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

	       if ( field_type == 3 )
	       {
		  R_Tiff::extract_short_from_buffer ( buffer, lsb, 8, &s_bits_per_pixel );
		  bits_per_pixel = s_bits_per_pixel;
	       }

	       else
		  R_Tiff::extract_long_from_buffer ( buffer, lsb, 8, &bits_per_pixel );

	       break;

	    case 273:  /* StripOffset */

		R_Tiff::extract_short_from_buffer ( buffer, lsb, 2, &field_type );
		R_Tiff::extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

		if ( field_type == 3 )
		{
		   R_Tiff::extract_short_from_buffer ( buffer, lsb, 8, &s_strip_offset );
		   strip_offset = s_strip_offset;
		}

		else
		   R_Tiff::extract_long_from_buffer ( buffer, lsb, 8, &strip_offset );

		break;

	    default:

	       break;


	 } /* ends switch tag_type */

      } /* ends loop over i directory entries */

      bytes_read = read ( fileDesc, buffer, 4 );
      R_Tiff::extract_long_from_buffer ( buffer, lsb, 0, &offset_to_ifd );

      if ( offset_to_ifd == 0 )
	 not_finished = 0;

   }  /*  ends while not_finished  */

   imageHeader.lsb           = lsb;
   imageHeader.bitsPerPixel  = bits_per_pixel;
   imageHeader.imageLength   = image_length;
   imageHeader.imageWidth    = image_width;
   //_cols = image_width;
   //_rows = image_length;
   imageHeader.stripOffset   = strip_offset;

   return 1;

}  /*  ends read_tiff_header  */

void R_Tiff::extract_short_from_buffer( char buffer[], int  lsb, int start, short int *number )

/*--------------------------------------------------------------*
 *    extract_short_from_buffer ( ...                           *
 *                                                              *
 *    This takes a two byte long out of a buffer of characters  *
 *    It is important to know the byte order LSB or MSB         *
 *--------------------------------------------------------------*/

{
   int i;
   union short_char_union   lcu;

   if ( lsb == 1 )
   {
      lcu.s_alpha[0] = buffer[start + 0];
      lcu.s_alpha[1] = buffer[start + 1];
   }  /*  ends if lsb = 1  */

   if ( lsb == 0 )
   {
      lcu.s_alpha[0] = buffer[start + 1];
      lcu.s_alpha[1] = buffer[start + 0];
   }  /*  ends if lsb = 0  */

   *number = lcu.s_num;

}  /*  ends extract_short_from_buffer  */

void R_Tiff::extract_long_from_buffer(char buffer[], int lsb, int start, long *number)

/*---------------------------------------------------------------*
 *    extract_long_from_buffer ( ...                             *
 *                                                               *
 *    This takes a four byte long out of a buffer of characters  *
 *    It is important to know the byte order LSB or MSB          *
 *---------------------------------------------------------------*/

{
   int i;
   union long_char_union   lcu;

   if ( lsb == 1 )
   {
      lcu.l_alpha[0] = buffer[start+0];
      lcu.l_alpha[1] = buffer[start+1];
      lcu.l_alpha[2] = buffer[start+2];
      lcu.l_alpha[3] = buffer[start+3];
   }  /*  ends if lsb = 1  */

   if ( lsb == 0 )
   {
      lcu.l_alpha[0] = buffer[start+3];
      lcu.l_alpha[1] = buffer[start+2];
      lcu.l_alpha[2] = buffer[start+1];
      lcu.l_alpha[3] = buffer[start+0];
   }  /*  ends if lsb = 0  */

   *number = lcu.l_num;

}  /*  ends extract_long_from_buffer  */



R_Tiff::~R_Tiff(void)
{
	delete picBuffer;
}
