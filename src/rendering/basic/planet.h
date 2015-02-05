//
//  planet.h
//  cs559fltk
//
//  Created by Leon Chu on 11/19/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__planet__
#define __cs559fltk__planet__

#include <stdio.h>

#include "rendering/basic.h"

namespace cs559 {
  //
  // Earth
  //
  class ObjEarth : public BasicObj {
  public:

  protected:
    void Init_();
    void Draw_();
    
  private:
    GLint uni_model_;
  };// end class ObjSimpleRect
  
} // namespace cs559

#endif /* defined(__cs559fltk__planet__) */
