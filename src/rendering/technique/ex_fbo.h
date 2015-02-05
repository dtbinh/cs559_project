#ifndef CUBE_FBO_H
#define CUBE_FBO_H

#include <stdio.h>

#include "rendering/basic.h"
#include "rendering/basic/cube.h"
#include "rendering/basic/rectangle.h"
#include "rendering/opengl/framebuffer.h"

namespace cs559 {
  
  /** An example to show fbo color and depth usage
   */
  class ExFbo : public BasicObj {
  public:

  protected:
    void Init_();
    void Draw_();

    void DrawCall_fbo_color_();
    void DrawCall_fbo_depth_();
    void DrawCall_fbo_copy_();

  private:
    FrameBuffer fbo_;
    Cube cube_;
    Rectangle rect_;
  };//class CubeFbo 

}//namespace cs559


#endif