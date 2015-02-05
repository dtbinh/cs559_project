//
//  cubemap_ball.h
//  cs559fltk
//
//  Created by Leon Chu on 11/21/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__cubemap_ball__
#define __cs559fltk__cubemap_ball__

#include <stdio.h>

#include "rendering/basic.h"
#include "rendering/opengl/shader_util.h"

namespace cs559 {
  //
  // Cubemap Ball
  //
  class CubemapBall : public BasicObj {
  public:

  protected:    
    void Init_();
    void Draw_();
  };// end class ObjSimpleRect
  
} // namespace cs559


#endif /* defined(__cs559fltk__cubemap_ball__) */
