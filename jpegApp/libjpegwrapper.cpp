#include <cstdio>

extern "C"
{
#include "jpeglib.h"
}

JSAMPROW  * image_buffer; /* Points to large array of R,G,B-order data */
int image_height; /* Number of rows in image */
int image_width;  /* Number of columns in image */
int row_stride;   /* physical row width in output buffer */
int output_components;

int jpegDecompress(char * filename)
{
 /* initialize jpeg_compress_struct and jpeg_error_mgr structures */

 struct jpeg_decompress_struct cinfo;
 struct jpeg_error_mgr jerr;


 cinfo.err = jpeg_std_error(&jerr);
 jpeg_create_decompress(&cinfo);

 FILE * infile = NULL;  /* source file */
 JSAMPARRAY buffer;  /* Output row buffer */

 /* Allocate and initialize a JPEG decompression object */

 cinfo.err = jpeg_std_error(&jerr);
 jpeg_create_decompress(&cinfo);    /* initialize the JPEG decompression object. */

 /* Specify file source */

  if ( (infile = fopen(filename, "rb")) == NULL ) {
  fprintf(stderr, "can't open %s\n", filename);
  return 0;
 }

 jpeg_stdio_src(&cinfo, infile);

 /* Call jpeg_read_header() to obtain image info */

 jpeg_read_header(&cinfo, TRUE);

 /* Set parameters for decompression */

 /* We don't need to change any of the defaults set by jpeg_read_header() */

 /* Start decompressor */

 jpeg_start_decompress(&cinfo);

 /* make an output work buffer of the right size. */
 /* JSAMPLEs per row in output buffer */
 image_width       = cinfo.image_width;
 image_height      = cinfo.image_height;
 output_components = cinfo.output_components;
 row_stride        = cinfo.output_width * cinfo.output_components;

 /* Make a one-row-high sample array that will go away when done with image */
 image_buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, cinfo.output_height);

 /* While (scan lines remain to be read)  jpeg_read_scanlines(...); */
 while (cinfo.output_scanline < cinfo.output_height) {
     JSAMPROW row_pointer[1];
          row_pointer[0] = (JSAMPLE *) (image_buffer+cinfo.output_scanline * row_stride);
     (void)jpeg_read_scanlines(&cinfo,row_pointer , 1);
 }

 /* Finish decompression */

 jpeg_finish_decompress(&cinfo);

 /* Release JPEG decompression object */
 jpeg_destroy_decompress(&cinfo);

 fclose(infile);

 fprintf(stdout, "Fuck Yeah\n");

 return 0;
}


void jpegCompress (char * filename, int quality)
{
  /* initialize jpeg_compress_struct and jpeg_error_mgr structures */

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  FILE * outfile;		/* target file */
  JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */

  /* Allocate and initialize JPEG compression object */
  cinfo.err = jpeg_std_error(&jerr);
  /* Initialize the JPEG compression object. */
  jpeg_create_compress(&cinfo);

  /* Specify data destination (eg, a file) */
  if ((outfile = fopen(filename, "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filename);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  /* set parameters for compression */

  cinfo.image_width = image_width; 	               /* image width and height, in pixels */
  cinfo.image_height = image_height;
  cinfo.input_components = output_components;	   /* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; 	               /* colorspace of input image */
  jpeg_set_defaults(&cinfo);

  jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

  /* Start compressor */

  jpeg_start_compress(&cinfo, TRUE);

  /* While (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  while (cinfo.next_scanline < cinfo.image_height) {
    row_pointer[0] = (JSAMPLE *) (image_buffer+cinfo.next_scanline * row_stride);
    (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Finish compression */

  jpeg_finish_compress(&cinfo);

  fclose(outfile);

  jpeg_destroy_compress(&cinfo);


}

