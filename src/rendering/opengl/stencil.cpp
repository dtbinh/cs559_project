#include "rendering/opengl/stencil.h"


namespace cs559 {
  
  
  /**
   */
  void Stencil::DrawBeg(
    GLenum func, GLint ref, GLuint mask, 
    GLenum fail, GLenum zfail, GLenum zpass) {
    StoreState();    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    /*glStencilFunc(func, ref, mask);
    glStencilOp(fail, zfail, zpass);*/
    
    glStencilFunc(GL_NEVER, 0x01, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
    
   
    glClear(GL_STENCIL_BUFFER_BIT);  // needs mask=0xFF
    glClear(GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR(" StencilUtil::DrawBeg Error \n");
  }

  /**
   */
  void Stencil::DrawEnd() {
    ReStoreState();
    CHECK_GL_ERROR(" StencilUtil::DrawEnd Error \n");
  }

  /**
   */
  void Stencil::UseBeg(GLenum func, GLint ref, GLuint mask) {
    StoreState();    

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);    
    //glDisable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    glStencilFunc(func, ref, mask);
    /*glStencilFunc(GL_EQUAL, 0x01, 0xFF);*/
  }

  /**
   */
  void Stencil::UseEnd() {
    ReStoreState();      
  }

}// namespace cs559