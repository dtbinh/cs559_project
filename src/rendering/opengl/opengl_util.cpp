#include "rendering/opengl/opengl_util.h"

#include <stdio.h>
#include <cassert>

#include "rendering/basic/fillscreen.h"
#include "fileio/img_png.h"

namespace cs559 {

  /** Save color data to png image, 3 channel (RGB) or 4 channel (RGBA)
   */
  void OpenGL_UTIL::ReadColorTo(char const* filename, int width, int height, int num_channel, bool flip_y) {
    ImgPng img(width, height, num_channel);
    switch (num_channel) {
    case 3:
      glReadPixels(0, 0, img.width, img.height, GL_RGB, GL_UNSIGNED_BYTE, img.data);
      img.Encode(filename, flip_y);
      break;
    case 4:
      glReadPixels(0, 0, img.width, img.height, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
      img.Encode(filename, flip_y);
      break;
    default:
      break;
    }
  };

  /** Save stencil data to png image, 1 channel
   */
  void OpenGL_UTIL::ReadStencilTo(char const* filename, int width, int height, bool flip_y, int threshold) {
    ImgPng img(width, height, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadPixels(0, 0, img.width, img.height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, img.data);
    if (threshold >= 0) {
      img.Threshold(threshold, 255);
    }
    img.Encode(filename, flip_y);
  };

  /** Can be used to test Stencil Test
   */
  void OpenGL_UTIL::DrawFullScreen(float r, float g, float b) {
    static bool init = false;
    static Fillscreen fillscreen;
    if (!init) {
      fillscreen.Init();
      init = true;
    }
    fillscreen.SetColor(r, g, b);
    fillscreen.Draw();
  }
    
  /** Get ViewPort size
   */
  void OpenGL_UTIL::GetViewportSize(int& width, int& height) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    width = viewport[2];
    height = viewport[3];
  }

  //
  //  Check OpenGL Error
  //  
  bool CheckGLError(char const* str, char const* file, int line) {
    GLenum errCode;    
    if ((errCode = glGetError()) != GL_NO_ERROR) {
      const GLubyte errString[] = "";//* errString = gluErrorString(errCode);
      printf(" - file: %s \n - line: %d \n", file, line); //__FILE__, __LINE__
      if (NULL!=str) {        
        printf ("%s, OpenGL Error %d: %s\n", str, errCode, errString);
      } else {
        printf ("OpenGL Error %d: %s\n", errCode, errString);
      }
      return true;
    }
    return false;
  }

  bool CheckGLError(char const* str) {
    GLenum errCode;    
    if ((errCode = glGetError()) != GL_NO_ERROR) {
      const GLubyte errString[] = "";//* errString = gluErrorString(errCode);      
      if (NULL!=str) {        
        printf ("%s, OpenGL Error %d: %s\n", str, errCode, errString);
      } else {
        printf ("OpenGL Error %d: %s\n", errCode, errString);
      }
      return true;
    }
    return false;
  }
  
}//namespace cs559 