//
//  window.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/10/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "window.h"
#include <ctime>
#include "rendering/technique/water.h"

namespace cs559 {

// Loop drawing, FPS contorl
void Idler(void* w) {
  Window* window = (Window*)w;  
  ulong& next_clock_ = window->next_clock_;
  ulong  curr_clock = clock();

  if (curr_clock > next_clock_) {    
    next_clock_ = curr_clock + window->clock_per_frame_;        
    // update
    if (window->flag_run) {
      window->canvas_->OnDraw();      
    }
  } 
  window->damage(1);
}// Idler()
  
void DrawCB(Fl_Widget*, GLCanvas* canvas) {
  canvas->OnDraw();
}

void RunCB(Fl_Widget*, Window* w) {
  Window* window = (Window*)w;
  window->flag_run ^= true;
}

void DepthDebugCB(Fl_Widget*, GLCanvas* canvas) {
  canvas->water->fbo1_show^= true;
}

void ColorDebugCB(Fl_Widget*, GLCanvas* canvas) {
  canvas->water->fbo2_show^= true;
}


Window::Window(int x, int y, int width, int height, int panel_width, char const* name)
  : Fl_Double_Window(x, y, width + panel_width, height, name) {    
  
  //// loop contorl
  fps_want_ = 30;
  clock_per_frame_ = (ulong)((float) CLOCKS_PER_SEC / (float)fps_want_);
  next_clock_ = clock();
  Fl::add_idle(Idler,this);


  //// UI
  { 
    begin(); 
    int acc_y_pos = 10;
    // setup Fl gl contex
    canvas_ = new GLCanvas(0, 0, width, height, name, fps_want_);  
    
    // run button
    button_run_ = new Fl_Button(width, acc_y_pos, panel_width, 20, "Run");
    button_run_->callback((Fl_Callback*)RunCB, this);
    acc_y_pos += 25;
    
    // draw button
    draw_step_ = new Fl_Button(width, acc_y_pos, panel_width, 20, "Draw");
    draw_step_->callback((Fl_Callback*)DrawCB, canvas_);
    acc_y_pos += 25;  

    // draw button
    color_run_ = new Fl_Button(width, acc_y_pos, panel_width, 20, "Color");
    color_run_->callback((Fl_Callback*)ColorDebugCB, canvas_);
    acc_y_pos += 25;

    // draw button
    depth_run_ = new Fl_Button(width, acc_y_pos, panel_width, 20, "Depth");
    depth_run_->callback((Fl_Callback*)DepthDebugCB, canvas_);
    acc_y_pos += 25;

    flag_run = false;
    end();
  }

}// Window::Window()

// bypass event to canvas
int Window::handle(int e) {   
  canvas_->OnEvent(e);  
  return Fl_Double_Window::handle(e);
}

}// namespace cs559