//
//  phong_shading.h
//  cs559fltk
//
//  Created by Leon Chu on 11/19/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__phong_shading__
#define __cs559fltk__phong_shading__

#include <stdio.h>

#include "rendering/basic.h"


namespace cs559 {
  
  //
  // Moon
  //
  class Ball : public BasicObj {
  public:
    void Init();
    void DrawCall();
    
  private:
    GLint u_model_;
    GLint u_modelview_it_;
  };// end class ObjSimpleRect
  
} // namespace cs559



#endif /* defined(__cs559fltk__phong_shading__) */
