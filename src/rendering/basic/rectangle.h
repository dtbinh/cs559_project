#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdio.h>
#include "rendering/basic.h"

namespace cs559 {
  

  class Rectangle : public BasicObj {
  public:
    void UpdateRegion(int x, int y, int width, int height);
    void UpdateRegion(float left_bottom_x, float left_bottom_y, float right_top_x, float right_top_y);
    
  protected:
    void Init_();
    void Draw_();

    void Init_(int x, int y, int width, int height);
  };// end class ObjSimpleRect
  
} // namespace cs559


#endif