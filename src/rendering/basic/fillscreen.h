//
//  fillscreen.h
//  cs559fltk
//
//  Created by Leon Chu on 11/24/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__fillscreen__
#define __cs559fltk__fillscreen__

#include <stdio.h>
#include "rendering/basic.h"

namespace cs559 {
  
  /**
   * To prevent face culling,
   * draw wwo quad, front and back.
   */
  class Fillscreen : public BasicObj {
  public:
    void SetColor(float r, float g, float b);

  protected:
    void Init_();
    void Draw_();
  };// end class ObjSimpleRect
  
} // namespace cs559

#endif /* defined(__cs559fltk__fillscreen__) */
