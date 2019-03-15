/*-----------------------------------------------------------------*
 *           Module for reading tiff's file                        *
 *-----------------------------------------------------------------*/

#include "globvar.h"
#include "tiffvar.h"
#include "funprot.h"

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdlib.h>
#include <conio.h>

/*****************************************************************/

int openTiffFile ( char *fileName, int *fileDesc )

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

/*****************************************************************/

int readTiffHeader ( int fileDesc )

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
   extract_long_from_buffer ( buffer, lsb, 4, &offset_to_ifd );
   not_finished = 1;
   while ( not_finished )
   {

/*---------------------------------------------------------------*
 *    Seek to the ID and read the entry_count,                   *
 *    i.e. the number of entries in the IFD.                     *
 *---------------------------------------------------------------*/
      position = lseek ( fileDesc, offset_to_ifd, 0 );
      bytes_read = read ( fileDesc, buffer, 2 );
      extract_short_from_buffer ( buffer, lsb, 0, &entry_count );

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
	 extract_short_from_buffer ( buffer, lsb, 0, &tag_type );
	 switch( tag_type )
	 {
	    case 255:  /* Subfile Type */

	       extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );
	       extract_short_from_buffer ( buffer, lsb, 8, &subfile );

	       break;

	    case 256:  /* ImageWidth */

	       extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

	       if ( field_type == 3 )
	       {
		  extract_short_from_buffer ( buffer, lsb, 8, &s_image_width );
		  image_width = s_image_width;
	       }

	       else
		  extract_long_from_buffer ( buffer, lsb, 8, &image_width );
	       if ( ( image_width % 8 ) != 0 )
		  image_width += 8 - ( image_width % 8 );
	       break;

	    case 257:  /* ImageLength */

	       extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

	       if ( field_type == 3 )
	       {
		  extract_short_from_buffer ( buffer, lsb, 8, &s_image_length );
		  image_length = s_image_length;
	       }
	       else
		  extract_long_from_buffer ( buffer, lsb, 8, &image_length );

	       break;

	    case 258:  /* BitsPerPixel */

	       extract_short_from_buffer ( buffer, lsb, 2, &field_type );
	       extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

	       if ( field_type == 3 )
	       {
		  extract_short_from_buffer ( buffer, lsb, 8, &s_bits_per_pixel );
		  bits_per_pixel = s_bits_per_pixel;
	       }

	       else
		  extract_long_from_buffer ( buffer, lsb, 8, &bits_per_pixel );

	       break;

	    case 273:  /* StripOffset */

		extract_short_from_buffer ( buffer, lsb, 2, &field_type );
		extract_short_from_buffer ( buffer, lsb, 4, &length_of_field );

		if ( field_type == 3 )
		{
		   extract_short_from_buffer ( buffer, lsb, 8, &s_strip_offset );
		   strip_offset = s_strip_offset;
		}

		else
		   extract_long_from_buffer ( buffer, lsb, 8, &strip_offset );

		break;

	    default:

	       break;


	 } /* ends switch tag_type */

      } /* ends loop over i directory entries */

      bytes_read = read ( fileDesc, buffer, 4 );
      extract_long_from_buffer ( buffer, lsb, 0, &offset_to_ifd );

      if ( offset_to_ifd == 0 )
	 not_finished = 0;

   }  /*  ends while not_finished  */

   imageHeader.lsb           = lsb;
   imageHeader.bitsPerPixel  = bits_per_pixel;
   imageHeader.imageLength   = image_length;
   imageHeader.imageWidth    = image_width;
   imageHeader.stripOffset   = strip_offset;

   if ( imageHeader.bitsPerPixel != 1 )
      return 0;

   return 1;

}  /*  ends read_tiff_header  */



/*****************************************************************/

static void extract_long_from_buffer ( char buffer[], int lsb, int start, long *number )

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



/****************************************************************/

static void extract_short_from_buffer ( char buffer[], int  lsb, int start, short int *number )

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

/****************************************************************/

int readTiffImage ( char *fileName )

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
   int           i, bytesRead, area, fileDesc;
   unsigned long position;
/*--------------------------------------------------------------*
 *     Opening the tiff's file.                                 *
 *     Writing parameters :                                     *
 *      length, width, number bits per pixel,                   *
 *      offset to picture's data, to image structure.           *
 *--------------------------------------------------------------*/
   if ( openTiffFile ( fileName, &fileDesc ) )
   {
      readTiffHeader ( fileDesc );

      if ( imageHeader.bitsPerPixel == 1 )
      {
	 position  =  lseek ( fileDesc, imageHeader.stripOffset, 0 );
/*--------------------------------------------------------------*
 *     Estimation of the size (in bytes) of shown picture.      *
 *--------------------------------------------------------------*/
	 area = imageHeader.imageWidth * imageHeader.imageLength;

	/* if ( ( area % 8 ) != 0 )
	    area += 8; */

	 area >>= 3;

	 bytesRead = read ( fileDesc, picBuffer, area );
	 closed = close ( fileDesc );

	 return 1;
      }

      else
	 return 0;
    }
    else
       return 0;
}  /* ends read_tiff_image */

/*****************************************************************/

/*---------------------------------------------------------------*
 *    Function for TIFF header file writing                      *
 ----------------------------------------------------------------*/


/*--------------------------------------------------------------*
*    put_long_to_buffer ( ...                                   *
*                                                               *
*    This takes a four byte long out of a buffer of characters  *
*    It is important to know the byte order LSB or MSB          *
----------------------------------------------------------------*/

void put_long_to_buffer (
			byte buffer[],
			int  lsb,
			int start,
			long *number
			)

{
	int i;
	union long_char_union   lcu;

	lcu.l_num = *number;

	if ( lsb == 1 )
	 {
	 buffer[start+0] = lcu.l_alpha[0];
	 buffer[start+1] = lcu.l_alpha[1];
	 buffer[start+2] = lcu.l_alpha[2];
	 buffer[start+3] = lcu.l_alpha[3];
	 }  /*  ends if lsb = 1  */
	if ( lsb == 0 )
	 {
	 buffer[start+3] = lcu.l_alpha[0];
	 buffer[start+2] = lcu.l_alpha[1];
	 buffer[start+1] = lcu.l_alpha[2];
	 buffer[start+0] = lcu.l_alpha[3];
	 }  /*  ends if lsb = 0  */
}  /*  ends put_long_to_buffer  */



/*--------------------------------------------------------------*
*    put_short_to_buffer ( ...                                  * 
*                                                               *
*    This takes a two byte long out of a buffer of characters   *
*    It is important to know the byte order LSB or MSB          *
----------------------------------------------------------------*/

void put_short_to_buffer ( byte buffer[], int lsb, int start, short int *number )
{
	int i;
	union short_char_union   lcu;
	lcu.s_num = *number;
	if ( lsb == 1 )
	 {
	 buffer[start+0] = lcu.s_alpha[0];
	 buffer[start+1] = lcu.s_alpha[1];
	 }  /*  ends if lsb = 1  */
	if ( lsb == 0 )
	 {
	 buffer[start+1] = lcu.s_alpha[2];
	 buffer[start+0] = lcu.s_alpha[3];
	 }  /*  ends if lsb = 0  */
}  /*  ends put_short_from_buffer  */



int writeHeader ( int fileDesc )
{
	unsigned char   buffer[12];

	int     bytes_read, closed, i, j, lsb, not_finished;
	long    bits_per_pixel, image_length, image_width,
			offset_to_ifd, position, strip_offset,  value ;
	short   entry_count, field_type, s_bits_per_pixel, s_image_length,
			length_of_field, subfile, s_image_width, s_strip_offset, tag_type;
	short tag_tab[20]={254,256,257,258,259,262,271,273,277,278,279,282,283,284,286,287,296};



	/*-----------------------------------------------------------*
	*    Determine if the file uses MSB first or LSB first       *
	-------------------------------------------------------------*/
	lsb = imageHeader.lsb;
	if ( lsb == 1 )
		buffer[0] = 0x49;
	else
		buffer[0] = 0x48;

	/*-----------------------------------------------------------*
	*    Read the offset to the IFD                              *
	-------------------------------------------------------------*/
	offset_to_ifd = 8;
	buffer[1]=73; buffer[2]=42;buffer[3]=0;
	put_long_to_buffer (buffer, lsb, 4, &offset_to_ifd );
	write( fileDesc, buffer, 8 );

	/*---------------------------------------------------------*
	*    Seek to the ID and read the entry_count,              *
	*    i.e. the number of entries in the IFD.                *
	-----------------------------------------------------------*/
	entry_count = 17;
	put_short_to_buffer ( buffer, lsb, 0, &entry_count );
	write ( fileDesc, buffer, 2 );

	/*---------------------------------------------------------*
	*    Now loop over the directory entries.                  *
	*    Look only for the tags we need. These are:            *
	*       ImageLength                                        *
	*       ImageWidth                                         *
	*       BitsPerPixel (BitsPerSample)                       *
	*       StripOffset                                        *
	-----------------------------------------------------------*/
	for ( i=0; i<entry_count; i++ )
		{
		tag_type = tag_tab[i];
		put_short_to_buffer ( buffer, lsb, 0, &tag_type);
		switch ( tag_type )
			{
			case 254: /* ? */
				buffer[2]=4; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=0; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 255:  /* Subfile Type */
				field_type = 3;
				length_of_field = 2;
				put_short_to_buffer ( buffer, lsb, 2, &field_type );
				put_short_to_buffer ( buffer, lsb, 4, &length_of_field);
				put_short_to_buffer ( buffer, lsb, 8, &subfile );
				break;

			case 256:  /* ImageWidth */
				field_type = 3;
				length_of_field = 1;
				put_short_to_buffer ( buffer, lsb, 2, &field_type );
				put_short_to_buffer ( buffer, lsb, 4, &length_of_field);
				s_image_width = imageHeader.imageWidth;
				put_short_to_buffer ( buffer, lsb, 8, &s_image_width );
				break;

			case 257:  /* ImageLength */
				field_type = 3;
				length_of_field = 1;
				put_short_to_buffer ( buffer, lsb, 2, &field_type );
				put_short_to_buffer ( buffer, lsb, 4, &length_of_field);
				s_image_length = imageHeader.imageLength;
				put_short_to_buffer ( buffer, lsb, 8, &s_image_length);
				break;

			case 258:  /* BitsPerPixel */
				field_type = 3;
				length_of_field = 1;
				put_short_to_buffer ( buffer, lsb, 2, &field_type );
				put_short_to_buffer ( buffer, lsb, 4, &length_of_field);
				s_bits_per_pixel = imageHeader.bitsPerPixel;
				put_short_to_buffer ( buffer, lsb, 8, &s_bits_per_pixel);
				break;

			case 259: /* ? */
				buffer[2]=3; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=1; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 262: /* ? */
				buffer[2]=3; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=1; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 271: /* ? */
				buffer[2]=2; buffer[3]=0; buffer[4]=9; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=12; buffer[9]=1; buffer[10]=0; buffer[11]=0;
				break;

			case 273:  /* StripOffset */
				field_type = 4;
				length_of_field = 1;
				put_short_to_buffer ( buffer, lsb, 2, &field_type );
				put_short_to_buffer ( buffer, lsb, 4, &length_of_field);
				strip_offset = imageHeader.stripOffset;
				put_long_to_buffer ( buffer, lsb, 8, &strip_offset );
				break;

			case 277: /* ? */
				buffer[2]=3; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=1; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 278: /* ? */
				buffer[2]=4; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=228; buffer[9]=8; buffer[10]=0; buffer[11]=0;
				break;

			case 279: /* ? */
				buffer[2]=4; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=236; buffer[9]=26; buffer[10]=5; buffer[11]=0;
				break;

			case 282: /* ? */
				buffer[2]=5; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=218; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 283: /* ? */
				buffer[2]=5; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=226; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;
			case 284: /* ? */
				buffer[2]=3; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=1; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 286: /* ? */
				buffer[2]=5; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=234; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 287: /* ? */
				buffer[2]=5; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=242; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			case 296: /* ? */
				buffer[2]=3; buffer[3]=0; buffer[4]=1; buffer[5]=0; buffer[6]=0;
				buffer[7]=0; buffer[8]=2; buffer[9]=0; buffer[10]=0; buffer[11]=0;
				break;

			default:
				break;


			} /* ends switch tag_type */
		write ( fileDesc, buffer, 12);
	} /* ends loop over i directory entries */

	offset_to_ifd = 0;
	put_long_to_buffer ( buffer, lsb, 0, &offset_to_ifd );
	write ( fileDesc, buffer, 4 );

	/* add 62 bytes for compatibility */
	buffer[0]=200;buffer[1]=0;buffer[2]=0; buffer[3]=0; buffer[4]=1; buffer[5]=0;
	buffer[6]=0;buffer[7]=0; buffer[8]=200; buffer[9]=0; buffer[10]=0; buffer[11]=0;
	write ( fileDesc, buffer, 12 );
	buffer[0]=1;buffer[1]=0;buffer[2]=0; buffer[3]=0; buffer[4]=0; buffer[5]=0;
	buffer[6]=0;buffer[7]=0; buffer[8]=1; buffer[9]=0; buffer[10]=0; buffer[11]=0;
	write ( fileDesc, buffer, 12 );
	buffer[0]=0;buffer[1]=0;buffer[2]=0; buffer[3]=0; buffer[4]=1; buffer[5]=0;
	buffer[6]=0;buffer[7]=0; buffer[8]=0; buffer[9]=0; buffer[10]=0; buffer[11]=0;
	write ( fileDesc, buffer, 12 );
	buffer[0]=0;buffer[1]=0;buffer[2]=0; buffer[3]=0; buffer[4]=0; buffer[5]=0;
	buffer[6]=0;buffer[7]=0; buffer[8]=0; buffer[9]=0; buffer[10]=0; buffer[11]=0;
	write ( fileDesc, buffer, 12 );
	buffer[0]=0;buffer[1]=0;buffer[2]=77; buffer[3]=73; buffer[4]=67; buffer[5]=82;
	buffer[6]=79;buffer[7]=84; buffer[8]=69; buffer[9]=75; buffer[10]=32; buffer[11]=0;
	write ( fileDesc, buffer, 12 );
	buffer[0]=0;buffer[1]=0;
	write ( fileDesc, buffer, 2 );

	return 1;

}  /*  ends write_tiff_header */

/*******************************************************************/

int writeToDisk ( char *name )
{
   int fileDesc;

   fileDesc = open ( name , O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE);
   if ( fileDesc < 0 )
      return 0;
   writeHeader ( fileDesc );
   write ( fileDesc, picBuffer, imageHeader.imageWidth * (imageHeader.imageLength+7)/8 );
   close ( fileDesc );
   return 1;
}
