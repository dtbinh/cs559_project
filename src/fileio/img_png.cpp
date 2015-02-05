#include "fileio/img_png.h"

#include <stdarg.h>
#include <png.h>

// ref: http://zarb.org/~gc/html/libpng.html

void abort_(const char * s, ...)
{
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

/**
 * main function
 */
static
unsigned char* DecodePNG(
  char const* filename, 
  unsigned char& num_channel, 
  int& width, 
  int& height, 
  int& stride, 
  int force_num_channel /*ovride num_channel*/)
{
  typedef unsigned char uchar;

  int wid, hei;
  png_byte color_type;
  png_byte bit_depth;

  png_structp png_ptr;
  png_infop info_ptr;
  int number_of_passes;
  png_bytep * row_pointers;
  //====

  char header[8];    // 8 is the maximum size that can be checked

  /* open file and test for it being a png */
  FILE *fp = fopen(filename, "rb");
  if (!fp)
    return NULL;
  fread(header, 1, 8, fp);
  if (png_sig_cmp((png_const_bytep)header, 0, 8))
    abort_("[read_png_file] File %s is not recognized as a PNG file", filename);

  /* initialize stuff */
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[read_png_file] png_create_read_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[read_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during init_io");

  png_init_io(png_ptr, fp);
  png_set_sig_bytes(png_ptr, 8);

  png_read_info(png_ptr, info_ptr);

  wid = png_get_image_width(png_ptr, info_ptr);
  hei = png_get_image_height(png_ptr, info_ptr);
  color_type = png_get_color_type(png_ptr, info_ptr);
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  /* read file */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[read_png_file] Error during read_image");

  int src_num_channel = 4;
  switch(color_type) {
  case PNG_COLOR_TYPE_RGB: src_num_channel = 3;
    break;
  case PNG_COLOR_TYPE_RGBA: src_num_channel = 4;
    break;
  case PNG_COLOR_TYPE_GRAY: src_num_channel = 1;
    break;
  case PNG_COLOR_TYPE_GRAY_ALPHA: src_num_channel = 2;
    break;
  }
  /** create a chunk of memory here
   */
  width = wid;
  height = hei;
  num_channel = (force_num_channel? force_num_channel : src_num_channel);
  stride = width*num_channel;

  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  uchar* buf = (uchar*) malloc(sizeof(uchar) * height * stride);  
 
  if (NULL != row_pointers && NULL != buf) {    
    // read all at once or row by row if the number of channel is not compatible
    if (num_channel == src_num_channel) {
      // set row pointer
      for (int y=0; y<height; y++) {
        row_pointers[y] = & buf[y*stride];//(png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
      }
      png_read_image(png_ptr, row_pointers); 
    } else if (num_channel > src_num_channel) {
      for (int y = 0; y < height; ++y) {
        uchar* row_ptr = &buf[y*stride];
        png_read_row(png_ptr, row_ptr, NULL);
        // re-position
        for (int i = width-1; i >=0 ; --i) {
          uchar* tgt = &row_ptr[i*num_channel];
          uchar* src = &row_ptr[i*src_num_channel];
          for (int j = num_channel-1; j >= src_num_channel; --j)
            tgt[j] = 255;
          for (int j = src_num_channel-1; j >= 0; --j)
            tgt[j] = src[j];         
        }  
      }
    } else if (num_channel < src_num_channel) {
      uchar* src_row_buf = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
      if (NULL != src_row_buf) {
        for (int y = 0; y < height; ++y) {
          png_read_row(png_ptr, src_row_buf, NULL);
          uchar* tgt_row_ptr = &buf[y*stride];
          // convert to gray, neglect alpha
          if (1 == num_channel && 3 <= src_num_channel) {
            for (int x = 0; x < width; ++x)
              tgt_row_ptr[x] = (uchar)(
                0.3f*src_row_buf[src_num_channel*x+0] +
                0.6f*src_row_buf[src_num_channel*x+1] +
                0.1f*src_row_buf[src_num_channel*x+2] );
          } else /*neglect remained channels*/{
            for (int x = 0; x < width; ++x)
              for (int k = 0; k < num_channel; ++k)
                tgt_row_ptr[x*num_channel+k] = src_row_buf[x*src_num_channel+k];              
          }//if
        }// for
      }//if
    }//if
  }  
  free(row_pointers);   
  fclose(fp);
  return buf;
}

/**
 */
unsigned char* ImgPng::Decode(
  char const* filename, 
  unsigned char& num_channel, 
  int& width, 
  int& height, 
  int& stride) {
  return DecodePNG(filename, num_channel, width, height, stride, 0);
}

/**
 */
unsigned char* ImgPng::Decode(
  char const* filename,
  int& width, 
  int& height, 
  int& stride, 
  int force_num_channel) {
  unsigned char num_channel;
  return DecodePNG(filename, num_channel, width, height, stride, force_num_channel);
}

/**
 */
unsigned char* ImgPng::Decode(
  char const* filename,
  int& width, 
  int& height, 
  int force_num_channel) {
  unsigned char num_channel;
  int stride;
  return DecodePNG(filename, num_channel, width, height, stride, force_num_channel);
}

/**
 */
static
void EncodePNG(
  char const* filename, 
  void* data,
  unsigned char num_channel, 
  int width, 
  int height, 
  int stride, 
  bool flip_y )
{
  png_byte color_type;

  switch(num_channel) {
  case 3: color_type = PNG_COLOR_TYPE_RGB;
    break;
  case 4: color_type = PNG_COLOR_TYPE_RGBA;
    break;
  case 1: color_type = PNG_COLOR_TYPE_GRAY;
    break;
  case 2: color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
    break;
  }
  png_byte bit_depth = 8;

  png_structp png_ptr;
  png_infop info_ptr;
  //int number_of_passes;
  png_bytep * row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);

  if (NULL == row_pointers)
    abort_("[write_png_file] row_pointers malloc failed");

  // init row_pointers
  for (int i = 0; i < height; ++i) {
    row_pointers[i] = & ((unsigned char*)data)[ (flip_y?height-i-1:i)*stride];
  }

  /* create file */
  FILE *fp = fopen(filename, "wb");
  if (!fp)
    abort_("[write_png_file] File %s could not be opened for writing", filename);

  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[write_png_file] png_create_write_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);

  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr, info_ptr, width, height,
    bit_depth, color_type, PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);

  /* write bytes */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing bytes");

  png_write_image(png_ptr, row_pointers);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  free(row_pointers);
  fclose(fp);
}


/**
 */
void ImgPng::Encode(
  char const* filename, 
  void* data,
  unsigned char num_channel, 
  int width, 
  int height, 
  int stride ) {
  EncodePNG(filename, data, num_channel, width, height, stride, false);
}


/**
 */
void ImgPng::Encode(char const* filename, bool flip_y) {
  EncodePNG(filename, data, num_channel, width, height, stride, flip_y);
}



/** Simple threshold
 * set pixel = val if pixel > thld
 */
void ImgPng::Threshold(int thld, int val) {
  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j)
      for (int k = 0; k < num_channel; ++k)
        if (data[i*stride + j*num_channel + k] > thld)
          data[i*stride + j*num_channel + k] = val;
}
