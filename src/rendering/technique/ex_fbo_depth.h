#ifndef DEPTH_FBO_H
#define DEPTH_FBO_H

#include "rendering/basic.h"

namespace cs559 {

  class ExFboDepth : public BasicObj {
  public:
    ExFboDepth(){   
      vao2 = 0; 
      vbo2 = 0;
      depth_tex = 0;
      fb = 0;
      overlay_shp_ = 0;
    }

  protected:
    void Init_();    
    void Draw_();   

  private:
    GLuint vao2, vbo2;
    GLuint depth_tex;
    GLuint fb;
    GLuint overlay_shp_;
  };// end class ObjSimpleRect

}//namespace cs559

#endif
