#include "rendering/opengl/framebuffer.h"
#include <cassert>

#include "rendering/opengl/shader_util.h"

namespace cs559 {

  /**
   */
  void FrameBuffer::Init(int width, int height) {    
    // Create texture buffer: Color
    {
      glGenTextures(1, &texurebuffer_color_hnd_);
      glBindTexture(GL_TEXTURE_2D, texurebuffer_color_hnd_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(GL_TEXTURE_2D, 0 /*level*/, 
        GL_RGB, width, height, 0 /*border*/, 
        GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }

    // Create texture buffer: Depth
    {
      glGenTextures(1, &texurebuffer_depth_hnd_);
      glBindTexture(GL_TEXTURE_2D, texurebuffer_depth_hnd_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(GL_TEXTURE_2D, 0 /*level*/, 
        GL_DEPTH_COMPONENT32F, width, height, 0 /*border*/, 
        GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    // Create texture buffer: Stencil
    //not supported now
    if(0){    
      glGenTextures(1, &texurebuffer_stencil_hnd_);
      glBindTexture(GL_TEXTURE_2D, texurebuffer_stencil_hnd_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      CHECK_GL_ERROR(" --- T4 Error \n");
      glTexImage2D(GL_TEXTURE_2D, 0 /*level*/, 
        GL_STENCIL_INDEX /*not supported ? */, width, height, 0 /*border*/, 
        GL_STENCIL_INDEX /*not supported ? */, GL_UNSIGNED_BYTE, nullptr);
    }    
    
    // Create Renderbuffer Object to hold depth and stencil buffers
    glGenRenderbuffers(1, &renderbuffer_hnd_);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_hnd_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, width, height);
    
    // Create frame buffer    
    glGenFramebuffers(1, &framebuffer_hnd_);
    GLenum CheckResult = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert(GL_FRAMEBUFFER_COMPLETE == CheckResult);
    
    // reset
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    initialized_ = true;
    CHECK_GL_ERROR(" FrameBuffer::Init() Error \n");
  }// Init()

  /**
   */
  void FrameBuffer::BindBegin(FBOTYPE fbo_type) {
    StoreState();
    assert(initialized_);
    fbo_type_ = fbo_type;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_hnd_);
    
    switch (fbo_type_) {
    case FBO_COLOR:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texurebuffer_color_hnd_, 0 /*level*/);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_hnd_);
      // Don't forget clear color and depth for this buffer!
      glEnable(GL_DEPTH_TEST); 
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_CULL_FACE);  glCullFace(GL_BACK);
      glEnable(GL_STENCIL_TEST); glStencilMask(0xFF); glClearStencil(0x00);      
      break;
    case FBO_DEPTH:      
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texurebuffer_depth_hnd_, 0 /*level*/);      
      glEnable(GL_DEPTH_TEST);
      glDepthMask(GL_TRUE);
      glClear(GL_DEPTH_BUFFER_BIT);
      break;
    //case FBO_STENCIL:
      //not supported now      
      //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texurebuffer_depth_hnd_, 0 /*level*/);
      //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texurebuffer_stencil_hnd_, 0 /*level*/);      
      //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texurebuffer_depth_hnd_, 0 /*level*/);
      //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_hnd_);
      /*glEnable(GL_STENCIL_TEST);      
      glClear(GL_STENCIL_BUFFER_BIT);*/
    default:
      break;
    }
    CHECK_GL_ERROR(" FrameBuffer::BindBegin() Error \n");
  }// Bind()

  /**
   */
  void FrameBuffer::BindEnd() {
    ReStoreState();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CHECK_GL_ERROR(" FrameBuffer::BindEnd() Error \n");
  }

  /** Default fbo_read_hnd should be 0
   */
  void FrameBuffer::CopyFboRead(FrameBuffer::FBOTYPE type, GLuint fbo_read_hnd) {
    fbo_type_ = type;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_read_hnd);  
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer_hnd_);  
    switch (fbo_type_) {
    case FBO_COLOR:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texurebuffer_color_hnd_, 0 /*level*/);
      glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer_hnd_);
      glBlitFramebuffer(0, 0, viewport[2], viewport[3], 
        0, 0, viewport[2], viewport[3],  GL_COLOR_BUFFER_BIT, GL_NEAREST); 
      break;
    case FBO_DEPTH:
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texurebuffer_depth_hnd_, 0 /*level*/);
      glBlitFramebuffer(0, 0, viewport[2], viewport[3], 
        0, 0, viewport[2], viewport[3],  GL_DEPTH_BUFFER_BIT, GL_NEAREST); 
      break;
    //case FBO_STENCIL:
    //  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texurebuffer_stencil_hnd_, 0 /*level*/);
    //  glBlitFramebuffer(0, 0, viewport[2], viewport[3],
    //    0, 0, viewport[2], viewport[3],  GL_DEPTH_BUFFER_BIT, GL_NEAREST); 
    //  break;
    default:
      break;
    }
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);       
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    GLenum error = glGetError();
    CHECK_GL_ERROR(" FrameBuffer::CopyFboRead() Error \n");
  }

  /**
   */
  GLuint FrameBuffer::TextureHnd() const { 
    GLuint handle;
    switch (fbo_type_) {
    case cs559::FrameBuffer::FBO_COLOR:
      handle = texurebuffer_color_hnd_;
      break;
    case cs559::FrameBuffer::FBO_DEPTH:
      handle = texurebuffer_depth_hnd_;
      break;
    //case cs559::FrameBuffer::FBO_STENCIL:
    //  handle = texurebuffer_depth_hnd_;
    //  break;
    default:
      break;
    }
    return handle;
  }

  /**
   */
  void FrameBuffer::Release() {
    assert(initialized_);
    if (renderbuffer_hnd_) {
      glDeleteRenderbuffers(1, &renderbuffer_hnd_);
      renderbuffer_hnd_ = 0;
    }
    if (texurebuffer_color_hnd_) {
      glDeleteTextures(1, &texurebuffer_color_hnd_);
      texurebuffer_color_hnd_ = 0;
    }
    if (texurebuffer_depth_hnd_) {
      glDeleteTextures(1, &texurebuffer_depth_hnd_);
      texurebuffer_depth_hnd_ = 0;
    }
    if (framebuffer_hnd_) {
      glDeleteFramebuffers(1, &framebuffer_hnd_);
      framebuffer_hnd_ = 0;
    }
  }

}//namespace cs559 