#ifndef FRAMEBUFFER_MNGR_H
#define FRAMEBUFFER_MNGR_H

#define GLEW_STATIC
#include <GL/glew.h>

#include "cs559core.h"
#include "rendering/opengl/opengl_util.h"

namespace cs559 {

class FrameBuffer : public OpenGL_UTIL {
public:
  enum FBOTYPE{FBO_COLOR, FBO_DEPTH/*, FBO_STENCIL*/};

  FrameBuffer() {
    initialized_ = false;
    framebuffer_hnd_ = 0;
    texurebuffer_color_hnd_ = 0;
    texurebuffer_depth_hnd_ = 0;
    texurebuffer_stencil_hnd_ = 0;
    renderbuffer_hnd_ = 0;
    fbo_type_ = FBO_COLOR;
  }

  ~FrameBuffer() {
    Release();
  }

  /** Init framebuffer object: texturebuffer, renderbuffer
   *  fbo_type: FBO_COLOR, FBO_DPETH
   */
  void Init(int width, int height);
  void Release();

  void BindBegin(FBOTYPE fbo_type);
  void BindEnd();

  /** copy the read framebuffer to framebuffer_hnd_
   *  Default fbo_read_hnd should be 0, which is screen's framebuffer
   */
  void CopyFboRead(FrameBuffer::FBOTYPE type, GLuint fbo_read_hnd = 0);

  GLuint TextureHnd() const;

private:
  bool initialized_;
  GLuint framebuffer_hnd_;
  GLuint texurebuffer_color_hnd_;
  GLuint texurebuffer_depth_hnd_;
  GLuint texurebuffer_stencil_hnd_;
  GLuint renderbuffer_hnd_;
  FBOTYPE fbo_type_;
  /** store and restore states
   */
  OpenGL_UTIL opengl_util_;
};//class FrameBufferMngr

}//namespace cs559

#endif