//
//  window.h
//  cs559fltk
//
//  Created by Leon Chu on 11/10/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__window__
#define __cs559fltk__window__

#include <stdio.h>

// include the useful FlTk stuff too
// FLTk gives some annoying warnings - turn them off
#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)
#include <FL/Fl.h>
#include <FL/Fl_Widget.h>
#include <FL/FL_Window.h>
#include <FL/FL_Gl_Window.h>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Button.H>
#pragma warning(pop)

#include "gl_canvas.h"

namespace cs559 {

typedef unsigned long ulong;

class Window : public Fl_Double_Window {
public:
  Window (int x, int y, int width, int height, int panel_width, char const* name);
  
  // FPS
  int fps_want_;
  ulong next_clock_;
  ulong clock_per_frame_;
  
  GLCanvas* canvas_;
  
  int handle(int e);
  
  bool flag_run;
  bool flag_color;
  bool flag_depth;

private:
  Fl_Button* draw_step_;  
  Fl_Button* button_run_; 
  Fl_Button* color_run_;
  Fl_Button* depth_run_;
  

};

  
}

  
#endif /* defined(__cs559fltk__window__) */
