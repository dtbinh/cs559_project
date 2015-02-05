//
//  misc_util.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/12/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "misc/util.h"

#include <cstdlib>

#include <FL/Fl_PNG_Image.H>

#include "fileio/img_png.h"

namespace cs559 {

/* Random number 0 ~ 1
 */
float random() {
  float v = (float)rand()/RAND_MAX;
  return v;
}

/* Random number -1 ~ 1
 */
float random2() {
  float v = (float)rand()/RAND_MAX;
  v = rand()%2 ? -v : v;
  return v;
}

/*  
 * read png image, 4 channel result
 */  
unsigned char* ReadPNG(char const* filename, int& width, int& height) {
  if (!filename) {
    printf("No filename given.\n");
    return 0;
  }// if
  
  Fl_PNG_Image *png_image = new Fl_PNG_Image(filename);
  width  = png_image->w();
  height = png_image->h();
  int depth  = png_image->d();
  
  if(width <= 0 || height <= 0){
    // printf("Fail to load in image\n");
    return NULL;
  }
  if( !(depth == 3 || 4== depth) ) {
    printf("The image is not in RGB or RGBA\n");
    return NULL;
  }
  
  unsigned char *temp_data = NULL;
  
  if (4 == depth) {
    int total = width * height * depth;
    temp_data = new unsigned char[total];
    for(int i = 0; i < width; i++){
      int raw_index = i * height;
      for(int j = 0; j < height; j++){
        int org_raw_height = (raw_index + j) * 4;
        int dst_raw_height = (raw_index + j) * 4;
        for(int k = 0; k < depth; k++){
          int org_index = org_raw_height + k;
          int dst_index = dst_raw_height + k;
          temp_data[dst_index] =  png_image->data()[0][org_index];
        }       
        //temp_data[i * height * 4 + j * 4 + 3] = 255;
      }
    }
  } else {
    int total = width * height * (depth + 1);
    temp_data = new unsigned char[total];
    
    for(int i = 0; i < width; i++){
      int raw_index = i * height;
      for(int j = 0; j < height; j++){
        int org_raw_height = (raw_index + j) * 3;
        int dst_raw_height = (raw_index + j) * 4;
        for(int k = 0; k < depth; k++){
          int org_index = org_raw_height + k;
          int dst_index = dst_raw_height + k;
          temp_data[dst_index] =  png_image->data()[0][org_index];
        }
        temp_data[i * height * 4 + j * 4 + 3] = 255;
      }
    }
  }
//    unsigned char *result = Reverse_Rows(temp_data, *width, *height);
//    delete [] temp_data;
//    return result;
  return temp_data;
}
 

/** TODO: need filter
 */
void NormalMap2DuDv(char const* normalmap_filenmae, char const* dudvmap_filenmae) {
  ImgPng normal(normalmap_filenmae, 3);
  if (NULL == normal.data) {
    return;
  }
  int const Height = normal.height;
  int const Width  = normal.width;
 
  ImgPng dudv(Width, Height, 3);
  if (!dudv.Valid()) {
    return;
  }
  
  float scale = 150.f;
  for (int y = 0; y < Height; ++y) {
    for (int x = 0; x < Width-1; ++x) {
      /*int r = scale*((int)normal.R(x+1,y) - (int)normal.R(x,y)) + 128;
      dudv.SetR(x, y, (uchar)Clip(r, 0, 255));
      int g = scale*((int)normal.G(x+1,y) - (int)normal.G(x,y)) + 128;
      dudv.SetG(x, y, (uchar)Clip(g, 0, 255));
      int b = scale*((int)normal.B(x+1,y) - (int)normal.B(x,y)) + 128;
      dudv.SetB(x, y, (uchar)Clip(b, 0, 255));*/
    }
  }
  dudv.Encode(dudvmap_filenmae, false);
  return;
}

}//end namespace cs559