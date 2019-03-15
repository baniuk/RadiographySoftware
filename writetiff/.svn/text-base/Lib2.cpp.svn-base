#include "Cpp_Lib.h"
//#include "R_Tiff.h"
#include "tiffvar.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>   
// This is an empty project to build DLL library of interface version 2.0

typedef struct 
{
   short  lsb;
   long   bitsPerPixel;
   long   imageLength;
   long   imageWidth;
   long   stripOffset;
   long	  tag256;
   long   tag257;
} tiffHeaderStruct;
int readTiffHeader(char* buf,tiffHeaderStruct* imageHeader);
void myread(char* in, char* out, int &start, int ile);
void mylseek(int &act,int ile,int start);

void extract_long_from_buffer(char buffer[], int lsb, int start, long *number);
void extract_short_from_buffer( char buffer[], int  lsb, int start, short int *number );
void put_long_to_buffer (char buffer[],int  lsb,int start,long *number);
void put_short_to_buffer ( char buffer[], int lsb, int start, short *number );

void Introduce(FUNC_DEF* definition)
{
	strcpy_s(definition->name,MAX_NAME,"writetiff");
	strcpy_s(definition->version,MAX_VER,"1.0");
	strcpy_s(definition->syntax,MAX_SYNT,"writetiff(IMAGE,STRING)");
	definition->numofout = 0;
	definition->numofin = 2;
	definition->typesofin[0] = IMAGE;definition->typesofin[1] = STRING;
//	definition->typesofout[0] = IMAGE;
	definition->interface_version = 2;
	strcpy_s(definition->description,MAX_DESC,"Reads tiff image");
	definition->ADDIP_version = 2.0;

}

void SetOutput(C_Variables** outtab, C_Variables** intab)
{
	// variable output has size of numofout (set in Introduce function)
	
}

void Do(C_Error& err, C_Variables** intab,int nargin, C_Variables** outtab, int nargout)
{
	C_Image_Container nowy;
	C_Image_Container* image;
	FILE *stream;
	tiffHeaderStruct imageHeader;
	unsigned short* tmp;
	short s_image_width,s_image_length;
	

	image = (C_Image_Container*)intab[0]->data;
	image->CloneObject(&nowy);
	tmp = new unsigned short[nowy.GetNumofElements()];
	for(unsigned long a=0;a<nowy.GetNumofElements();a++)
		tmp[a] = (unsigned short)floor(nowy.data[a]);

	readTiffHeader(image->header,&imageHeader);

	s_image_width = nowy._cols;
	put_short_to_buffer(image->header+imageHeader.tag256,imageHeader.lsb,0,&s_image_width);
	s_image_length = nowy._rows;
	put_short_to_buffer(image->header+imageHeader.tag257,imageHeader.lsb,0,&s_image_length);

	_set_fmode(_O_BINARY);
	fopen_s( &stream, (char *)intab[1]->data, "w" );
	fwrite((void*)image->header,sizeof(char),image->header_size,stream);
	
	fwrite((void*)tmp,sizeof(unsigned short),nowy.GetNumofElements(),stream);

	
	fclose(stream);
	delete[] tmp;
//	outtab[0]->AssignValue((void*)&nowy,err);

}
void myread(char* in, char* out, int &start, int ile)
{
	unsigned int licznik=0;
	for(unsigned int a=start;a<start+ile;a++)
		out[licznik++] = in[a];
	start+=ile;
}

void mylseek(int &act,int ile,int start)
{
	act = start+ile;
}
int readTiffHeader(char* buf,tiffHeaderStruct* imageHeader)

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
   int   lsb, not_finished;
   long   bits_per_pixel, image_length, image_width,
	  offset_to_ifd, strip_offset ;
   short  entry_count, field_type, s_bits_per_pixel, s_image_length,
	  length_of_field,   subfile,
	  s_image_width, s_strip_offset, tag_type;
	int actpos = 0;
	unsigned int i,ile=12;
/*---------------------------------------------------------------*
 *    Determine if the file uses MSB first or LSB first          *
 *---------------------------------------------------------------*/
   myread (buf, buffer, actpos,8 );
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
      mylseek(actpos, offset_to_ifd, 0 );
      myread (buf, buffer, actpos,2 );
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
	 myread (buf, buffer,actpos, ile );
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
		   imageHeader->tag256 = actpos+8-ile;

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
			imageHeader->tag257 = actpos+8-ile;
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

      myread ( buf, buffer, actpos,4 );
      extract_long_from_buffer ( buffer, lsb, 0, &offset_to_ifd );

      if ( offset_to_ifd == 0 )
	 not_finished = 0;

   }  /*  ends while not_finished  */

   imageHeader->lsb           = lsb;
   imageHeader->bitsPerPixel  = bits_per_pixel;
   imageHeader->imageLength   = image_length;
   imageHeader->imageWidth    = image_width;
   //_cols = image_width;
   //_rows = image_length;
   imageHeader->stripOffset   = strip_offset;

   return 1;

}  /*  ends read_tiff_header  */

void extract_long_from_buffer(char buffer[], int lsb, int start, long *number)

/*---------------------------------------------------------------*
 *    extract_long_from_buffer ( ...                             *
 *                                                               *
 *    This takes a four byte long out of a buffer of characters  *
 *    It is important to know the byte order LSB or MSB          *
 *---------------------------------------------------------------*/

{
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

void extract_short_from_buffer( char buffer[], int  lsb, int start, short int *number )

/*--------------------------------------------------------------*
 *    extract_short_from_buffer ( ...                           *
 *                                                              *
 *    This takes a two byte long out of a buffer of characters  *
 *    It is important to know the byte order LSB or MSB         *
 *--------------------------------------------------------------*/

{
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

void put_long_to_buffer (
			char buffer[],
			int  lsb,
			int start,
			long *number
			)

{
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

void put_short_to_buffer ( char buffer[], int lsb, int start, short *number )
{
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
