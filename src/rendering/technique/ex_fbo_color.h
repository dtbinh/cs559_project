#ifndef RECTANGLE2_H
#define RECTANGLE2_H

#include <stdio.h>
#include "rendering/basic.h"
#include "rendering/opengl/framebuffer.h"

namespace cs559 {
  
  //
  // Cubemap
  //
  class ExFboColor : public BasicObj {
  public:

  protected:
    void Init_();    
    void Draw_();

    void Init_(int x, int y, int width, int height);
   
  private:
    FrameBuffer fbo_;
  };// end class ObjSimpleRect
  
} // namespace cs559


#endif