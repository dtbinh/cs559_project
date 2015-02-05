//
//  cubemap.h
//  cs559fltk
//
//  Created by Leon Chu on 11/20/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__cubemap__
#define __cs559fltk__cubemap__

#include <stdio.h>

#include "rendering/basic.h"

namespace cs559 {
  
  //
  // Cubemap
  //
  class Cubemap : public BasicObj {
  public:
    
  protected:
    void Init_();
    void Draw_();

  private:
    void UpdateView_();
  };// end class ObjSimpleRect
  
} // namespace cs559



#endif /* defined(__cs559fltk__cubemap__) */
