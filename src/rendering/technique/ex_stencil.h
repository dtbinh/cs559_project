//
//  stencil.h
//  cs559fltk
//
//  Created by Leon Chu on 11/24/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__stencil__
#define __cs559fltk__stencil__

#include <stdio.h>

#include "rendering/basic.h"


#include "rendering/basic/cube.h"
#include "rendering/basic/cubemap.h"

#include "rendering/opengl/stencil.h"
#include "rendering/opengl/framebuffer.h"

#include "rendering/basic/fillscreen.h"

namespace cs559 {
  
  //
  // Stencil
  //
  class ExStencil : public BasicObj {
  public:

  protected:
    void Init_();
    void Draw_();
    
  private:
    GLint u_model_;
    GLint u_modelview_it_;
        
    Cube cube_;
    Cubemap cubemap_;

    Stencil stencil_;

    Fillscreen fillscreen_;
  };// end class ObjSimpleRect
  
} // namespace cs559


#endif /* defined(__cs559fltk__stencil__) */
