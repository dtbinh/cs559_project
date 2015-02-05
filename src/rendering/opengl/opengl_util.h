#ifndef OPENGL_UTIL_H
#define OPENGL_UTIL_H

#define GLEW_STATIC
#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "cs559core.h"

namespace cs559 {

  bool CheckGLError(char const* str);
  bool CheckGLError(char const* str, char const* file, int line);

  #define CHECK_GL_ERROR(str) CheckGLError(str, __FILE__, __LINE__);

  /**
   *
   */
  class OpenGL_UTIL {
  public:

    /** Save color data to png image, 3 channel (RGB) or 4 channel (RGBA)
     */
    static
    void ReadColorTo(char const* filename, int width, int height, int num_channel, bool flip_y);

    /** Save stencil data to png image, 1 channel
     *  set threshold > 0 to shreshold the image, otherwise it's to dark to see anyting...
     */
    static
    void ReadStencilTo(char const* filename, int width, int height, bool flip_y, int threshold);

    /** Can be used to test Stencil Test
     */
    static
    void DrawFullScreen(float r, float g, float b);

    static
    void GetViewportSize(int& width, int& height);
    
    /**
     */
    static
    glm::mat4 GetMirrorY() {
      glm::mat4 mirror(1,  0,  0,  0, 
                       0, -1,  0,  0,
                       0,  0,  1,  0,
                       0,  0,  0,  1);
      return mirror;
    }

    static
    void SetBGColor(float r, float g, float b) {
      glClearColor(r, g, b, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    /**
     */
    void StoreState() {
      isEnableBlend_ = glIsEnabled(GL_BLEND);
      isEnableStencil_ = glIsEnabled(GL_STENCIL_TEST);
      isEnableCullFace_ = glIsEnabled(GL_CULL_FACE);
      isEnableDepthTest_ = glIsEnabled(GL_DEPTH_TEST);
      glGetBooleanv(GL_COLOR_WRITEMASK, color_mask_);
      glGetBooleanv(GL_DEPTH_WRITEMASK, depth_mask_);
      glGetIntegerv(GL_STENCIL_WRITEMASK, (GLint*)stencil_mask_);
      glGetIntegerv(GL_STENCIL_FUNC, (GLint*)stencil_fun_);
      glGetIntegerv(GL_STENCIL_REF, (GLint*)stencil_ref_);
      glGetIntegerv(GL_STENCIL_VALUE_MASK, (GLint*)stencil_val_mask_);
      glGetIntegerv(GL_CULL_FACE_MODE, (GLint*)cullface_mode_);
      glGetIntegerv(GL_BLEND_SRC, (GLint*)&blend_fun_[0]);
      glGetIntegerv(GL_BLEND_DST, (GLint*)&blend_fun_[1]);
    }

    /**
     */
    void ReStoreState() {
      glColorMask(color_mask_[0], color_mask_[1], color_mask_[2], color_mask_[3]);
      glDepthMask(depth_mask_[0]);      
      glStencilMask(stencil_mask_[0]);
      glStencilFunc(stencil_fun_[0], stencil_ref_[0], stencil_val_mask_[0]);
      glCullFace(cullface_mode_[0]);
      glBlendFunc(blend_fun_[0], blend_fun_[1]);

      if (isEnableBlend_) {
        glEnable(GL_BLEND);
      } else {
        glDisable(GL_BLEND);
      }
      if (isEnableStencil_) {
        glEnable(GL_STENCIL_TEST);
      } else {
        glDisable(GL_STENCIL_TEST);
      }
      if (isEnableCullFace_) {
        glEnable(GL_CULL_FACE);
      } else {
        glDisable(GL_CULL_FACE);
      }
      if (isEnableDepthTest_) {
        glEnable(GL_DEPTH_TEST);        
      } else {
        glDisable(GL_DEPTH_TEST);
      }
    }// ReStoreState_()
  protected:

  private:
    GLboolean isEnableBlend_;
    GLboolean isEnableStencil_;
    GLboolean isEnableCullFace_;
    GLboolean isEnableDepthTest_;
    GLboolean color_mask_[4];
    GLboolean depth_mask_[1];
    GLuint    stencil_mask_[1];
    GLenum    stencil_fun_[1];
    GLint     stencil_ref_[1];
    GLuint    stencil_val_mask_[1];
    GLenum    cullface_mode_[1];
    GLenum    blend_fun_[2];

  };// class OpenGL_UTIL

}//namespace cs559 

#endif
