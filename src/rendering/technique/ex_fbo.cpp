#include "rendering/technique/ex_fbo.h"

namespace cs559 {

  void ExFbo::Init_() {      
    // setup fbo
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
            
    fbo_.Init(viewport[2], viewport[3]);
       
    // setup little rectangle
    rect_.Init();

    // setup framebuffer    
    cube_.Init();

    CHECK_GL_ERROR(" CubeFbo::Init() Error \n");
  }

  /**
   */
  void ExFbo::DrawCall_fbo_color_() {
    fbo_.BindBegin(FrameBuffer::FBO_COLOR);
    {            
      cube_.Draw();
    }
    fbo_.BindEnd();

    // draw rect
    glDisable(GL_DEPTH_TEST); // draw on the top
    rect_.SetTexture(GL_TEXTURE0, fbo_.TextureHnd(), GL_TEXTURE_2D);
    rect_.UpdateRegion(220, 0, 100, 100);
    rect_.Draw();
    glEnable(GL_DEPTH_TEST);
  }

  /**
   */
  void ExFbo::DrawCall_fbo_depth_() {
    fbo_.BindBegin(FrameBuffer::FBO_DEPTH);
    { /* YOU SHOULD BE VERY CLOSE TO OBJECT TO SEE DEPTH !!!!!!*/
      cube_.SetView(glm::lookAt(glm::vec3(3.0f, 5.0f,  0.0f), 
        glm::vec3(0.0f, 0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)));
      cube_.SetProjection(glm::perspective(45.0f, 800.0f / 600.0f, 3.f,  100.f));      
      cube_.Draw();
    }
    fbo_.BindEnd();

    // draw rect
    glDisable(GL_DEPTH_TEST); // draw on the top
    rect_.SetTexture(GL_TEXTURE0, fbo_.TextureHnd(), GL_TEXTURE_2D);
    rect_.UpdateRegion(110, 0, 100, 100);
    rect_.Draw();
    glEnable(GL_DEPTH_TEST);
  }

  /**
   */
  void ExFbo::DrawCall_fbo_copy_() {
    // draw normal cube
    // move camera closely in case you want to use fbo depth
    cube_.SetView(glm::lookAt(glm::vec3(3.0f, 5.0f,  0.0f), glm::vec3(0.0f, 0.0f,  0.0f), glm::vec3(0.0f, 1.0f,  0.0f)));
    cube_.SetProjection(glm::perspective(45.0f, 800.0f / 600.0f, 3.f,  100.f));
    cube_.Draw();

    // copy fbo's texture for rectangle
    fbo_.CopyFboRead(FrameBuffer::FBO_COLOR);

    // draw rect
    glDisable(GL_DEPTH_TEST); // draw on the top
    rect_.SetTexture(GL_TEXTURE0, fbo_.TextureHnd(), GL_TEXTURE_2D);
    rect_.UpdateRegion(0, 0, 100, 100);
    rect_.Draw();
    glEnable(GL_DEPTH_TEST);
  }

  void ExFbo::Draw_() {
    glClearColor( 1.f, 1.f, 0.0f, 1.0f );

    DrawCall_fbo_copy_();
    DrawCall_fbo_color_();
    DrawCall_fbo_depth_();    
  }

}//namespace cs559