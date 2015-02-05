//
//  misc_util.h
//  cs559fltk
//
//  Created by Leon Chu on 11/12/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__misc_util__
#define __cs559fltk__misc_util__

#include <stdio.h>
#include "cs559core.h"

namespace cs559 {

  template <typename T>
  T Clip(T const& n, T const& lower, T const& upper) {
    return n <= lower ? lower : n >= upper ? upper : n;
  }

  /* Random number 0 ~ 1
   */
  float random();

  /* Random number -1 ~ 1
   */
  float random2();
 
  void NormalMap2DuDv(char const* normalmap_filenmae, char const* dudvmap_filenmae);

}//end namespace cs669

#endif /* defined(__cs559fltk__misc_util__) */
