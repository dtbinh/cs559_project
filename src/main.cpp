//
//  main.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/10/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include <stdio.h>

#include <FL/Fl.H>
#include "window.h"

#include "fileio/img_png.h"
#include "misc/util.h"

int main(int argc, char** argv) {
  int window_width = 800;
  int window_height = 600;
  int panel_width = 100;
  char const* window_name = "Project 2";
  
  cs559::Window hw8(100, 100, window_width, window_height, panel_width, window_name);
  hw8.show();
  
  return Fl::run();
}