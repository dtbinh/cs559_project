#ifndef CUBE_H
#define CUBE_H
#include "rendering/basic.h"
#include "rendering/opengl/shader_util.h"

namespace cs559 {
  
  //
  // Cube
  //
  class Cube : public BasicObj {
  public:

  protected:
    void Init_(float r);
    void Init_();
    void Draw_();

  private:    
    void UpdateModel_();
  };// end class ObjSimpleRect
  
} // namespace cs559



#endif