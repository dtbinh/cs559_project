//
//  Canvas.h
//  cs559fltk
//
//  Created by Leon Chu on 11/10/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__Canvas__
#define __cs559fltk__Canvas__

#include <vector>
#include <fl/Fl_Gl_Window.H>

#include "cs559core.h"
#include "rendering/basic.h"
#include "rendering/camera.h"
#include "rendering/technique/water.h"

namespace cs559 {
  
class GLCanvas : public Fl_Gl_Window {
public:
  GLCanvas(int x, int y, int width, int height, char const* name, int fps);
  
  void OnDraw();
  void OnEvent(int e);

  Water* water;

private:
  // from FlTk
  void draw();
  
  bool initialized_;
  void InitGL();
  void DrawGL();

  Camera camera_;

  ulong fps_;  
  ulong next_clock_;
  std::vector<GLObj*> scene_objects_;
  
}; //End class GLCanvas
  
}



#endif /* defined(__cs559fltk__Canvas__) */
