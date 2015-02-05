#ifndef IMG_PNG_H
#define IMG_PNG_H

#include <stdlib.h>  /* malloc, free */

/** PNG Image decode/encode
 *  Decode Example:
 *    char filename[] = "./asset/sea.png";
 *  
 *    // method 1
 *    ImgPng img(filename);
 *
 *    // method 2, force decode into 3 channel
 *    ImgPng img_3ch(filename, 3);
 *
 *    // method 3
 *    unsigned char num_channel;
 *    int width, height, stride;
 *    unsinged char* raw_data = ImgPng::Decde(filename, num_channel, width, height, stride);
 * 
 *  Encode Example:
 *    ImgPng::Encode("test.png", img.data, img.num_channel, img.width, img.height, img.stride);   
 */
struct ImgPng {
  unsigned char* data;
  int width, height, stride;
  unsigned char num_channel;

  /** Load image at initialization
   *  RAII (resource acuisition in initialization)
   */
  ImgPng(char const* filename, int force_num_channel = 0){
    if (0 == force_num_channel) {
      data = Decode(filename, num_channel, width, height, stride);
    } else {
      num_channel = force_num_channel;
      data = Decode(filename, width, height, stride, force_num_channel);
    }
  }

  /** RAII, Check Valid() before using it
   */
  ImgPng(int wid, int hei, unsigned char n_ch) {
    width = wid; height = hei; num_channel = n_ch; stride = width*num_channel;
    data = (unsigned char*)malloc(sizeof(unsigned char)* height*stride);
  }

  ~ImgPng() { if(NULL != data) {free(data);} data = NULL;
  }

  bool Valid() const {return NULL != data;}

  unsigned char R(int x, int y) const { return data[y*stride + x*num_channel + 0];}
  unsigned char G(int x, int y) const { return data[y*stride + x*num_channel + 1];}
  unsigned char B(int x, int y) const { return data[y*stride + x*num_channel + 2];}
  unsigned char A(int x, int y) const { return data[y*stride + x*num_channel + 3];}
  
  void Set(int x, int y, int k, unsigned char val) { data[y*stride + x*num_channel + k] = val;}
  void SetR(int x, int y, unsigned char val) { data[y*stride + x*num_channel + 0] = val;}
  void SetG(int x, int y, unsigned char val) { data[y*stride + x*num_channel + 1] = val;}
  void SetB(int x, int y, unsigned char val) { data[y*stride + x*num_channel + 2] = val;}
  void SetA(int x, int y, unsigned char val) { data[y*stride + x*num_channel + 3] = val;}

  /** Simple threshold
   * set pixel = val if pixel > thld
   */
  void Threshold(int thld, int val);

  static
  unsigned char* Decode(char const* filename, unsigned char& num_channel, int& width, int& height, int& stride);

  static
  unsigned char* Decode(char const* filename, int& width, int& height, int& stride, int force_num_channel);

  /** stride = width*force_num_channel
   */
  static
  unsigned char* Decode(char const* filename, int& width, int& height, int force_num_channel);

  static
  void Encode(char const* filename, void* data, unsigned char num_channel, int width, int height, int stride);

  void Encode(char const* filename, bool flip_y);

private:
  // not allowed
  ImgPng();
  ImgPng(ImgPng const& rhs);
  ImgPng& operator = (ImgPng const& rhs);
};// struct PngImg {}

#endif