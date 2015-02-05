//
//  Canvas.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/10/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//
#include "gl_canvas.h"

#include <ctime>

#define GLEW_STATIC
#include <GL/glew.h>

#include <FL/Fl.h>

#include "rendering/opengl/texture_mngr.h"
#include "rendering/basic/planet.h"
#include "rendering/basic/cubemap.h"
#include "rendering/basic/cubemap_ball.h"

#include "rendering/basic/cube.h"
#include "rendering/basic/rectangle.h"
#include "rendering/scene/skybox.h"
#include "rendering/scene/trees.h"
#include "rendering/scene/cloud.h"
#include "rendering/scene/bird.h"
#include "rendering/technique/terrain.h"
#include "rendering/technique/water.h"

#include "rendering/technique/ex_fbo.h"
#include "rendering/technique/ex_stencil.h"

#include "fileio/img_png.h"

//TO-UPDATE
#include "rendering/basic/phong_shading.h"


namespace cs559 {

/**
 */
GLCanvas::GLCanvas(int x, int y, int width, int height, char const* name, int fps)
    : Fl_Gl_Window(x, y, width, height, name) {
  initialized_ = false;
  fps_ = 0;
  next_clock_ = clock() + CLOCKS_PER_SEC;  
  camera_.SetFPS((float)fps);

  TextureMngr::AddFolderPath("./asset/texture/cubemap");
  TextureMngr::AddFolderPath("./asset/texture/terrain");
  TextureMngr::AddFolderPath("./asset/texture");
  
#if 1
  // Skybox, draw first
  scene_objects_.push_back(new Skybox(camera_));   


  // Island
  Terrain* terrain = new Terrain(camera_);
  terrain->SetClipDirection(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
  scene_objects_.push_back(terrain);

  // Ocean
  water = new Water(camera_);
  Terrain* reflected_terrain = new Terrain(camera_);    
  Skybox* reflected_skybox = new Skybox(camera_);
  reflected_terrain->SetClipDirection(glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 0.f));
  reflected_terrain->SetFrontFaceCCW(false);
  reflected_skybox->SetFrontFaceCCW(true);
  water->AddReflectedObj(reflected_skybox);
  water->AddReflectedObj(reflected_terrain); 
  scene_objects_.push_back(water);

  // Trees
  Trees* trees = new Trees(camera_);
  scene_objects_.push_back(trees);


  // Cloud
  Cloud* cloud = new Cloud(camera_);
  scene_objects_.push_back(cloud);
#endif


  // Bird
  Bird* bird = new Bird(camera_);
  scene_objects_.push_back(bird);

  //scene_objects_.push_back(new ObjEarth);    
  //scene_objects_.push_back(new CubemapBall);  
  //scene_objects_.push_back(new Cube);  
  //scene_objects_.push_back(new Rectangle);
  //scene_objects_.push_back(new ExStencil);
  //scene_objects_.push_back(new ExFbo);

} // GLCanvas::GLCanvas()


/**
 *  Called by window
 */
void GLCanvas::OnDraw() {
  
  camera_.UpdateTime();

  // count fps
  if (clock() > (clock_t)next_clock_) {
    printf(" - fps: %d \n", fps_);
    fps_ = 0;  next_clock_ += CLOCKS_PER_SEC; 
  }
  ++fps_;
  // fire
  draw();
}// OnDraw()


//
// Initialize all OpenGL objects
//
void GLCanvas::InitGL() {
  glewExperimental = GL_TRUE;  
  glewInit();
  const GLubyte * glstr = glGetString(GL_VERSION);  
  if (nullptr == glstr) {
    return;
  }
  printf("%s \n", glstr);

  // depth buffer  
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);
  
  // initialize all drawing objects
  for (auto obj : scene_objects_) {
    obj->Init();
  }
  
  initialized_ = true;
  printf(" - All gl objects are initialized. \n");
} //End InitGL()


//  
// Draw Call happens here
//  
void GLCanvas::draw() {
  if (!initialized_) {
    InitGL(); return;
  }
  // Clear the screen    
  glClearColor(0.0f, 0.5, 0.0f, 1.0f);
  glClearDepth(1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  

  // draw all objects
  for (auto obj : scene_objects_) {
    obj->Draw();
  }
  
  CHECK_GL_ERROR(" GLCanvas::draw() Error \n");  
  this->swap_buffers();  
} //End GLCanvas::draw()
  

void GLCanvas::OnEvent(int e) {
  /*printf("%d", e);*/
  
  // remember what button was used, left_button:1, right_button:3
	static int last_push = 0;
  static int last_x = 0;
  static int last_y = 0;

  switch(e) {  
  case FL_PUSH: last_push = Fl::event_button();
    last_x = Fl::event_x ();
    last_y = Fl::event_y ();
    break;
  case FL_RELEASE: last_push = 0;
    break;
  case FL_DRAG:
    camera_.MouseHoldMove(last_push, Fl::event_x() - last_x, Fl::event_y() - last_y);
    last_x = Fl::event_x ();
    last_y = Fl::event_y ();
    break;
  case FL_MOUSEWHEEL:
    camera_.MouseHoldMove(3, Fl::event_dx(), Fl::event_dy());
    break;
  case FL_KEYBOARD:   
    camera_.KeyIn(*Fl::event_text()); //int key = Fl::event_key();    
  default:
    break;
  };  
}// GLCanvas::handle(int e)
  
}