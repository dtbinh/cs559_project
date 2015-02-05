#ifndef STENCIL_UTIL_H
#define STENCIL_UTIL_H

#include <glm/glm.hpp>
#include "rendering/opengl/opengl_util.h"

namespace cs559 {

  /** 
   */
  class Stencil : public OpenGL_UTIL {
  public:
    Stencil() {
    }

    /** Ex: DrawBeg(GL_NEVER, 0x01, 0xFF, GL_REPLACE, GL_KEEP, GL_KEEP)
     */
    void DrawBeg(GLenum func, GLint ref, GLuint mask, GLenum fail, GLenum zfail, GLenum zpass);

    /**
     */
    void DrawEnd();

    /** Ex: UseBeg(GL_EQUAL, 0x01, 0xFF);
     */
    void UseBeg(GLenum func, GLint ref, GLuint mask);

    /**
     */
    void UseEnd();

  };// class StencilUtil{}

}// namespace cs559

#endif