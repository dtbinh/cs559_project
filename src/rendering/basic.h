//
//  basic.h
//  cs559fltk
//
//  Created by Leon Chu on 11/19/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__basic__
#define __cs559fltk__basic__

#include <stdio.h>
#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#include "cs559core.h"
#include "rendering/camera.h"
#include "rendering/opengl/shader_util.h" // ShaderMVP
#include "rendering/opengl/texture_mngr.h" // TextureMngr

#define GLSL120(src) "#version 120\n" #src
#define GLSL150(src) "#version 150 core\n" #src

namespace cs559 {
  
  //
  //  GL Drawing Object Interface
  //
  class GLObj {
  public:
    GLObj () {
      vao_ = vbo_ = ebo_ = shader_program_ = 0;
      initialized_ = false;
      front_face_changed_ = false;
      prev_frontface_setting_[1] = GL_CCW;
    }
    
    /** Init before draw!
     */
    void Init();

    /** Draw after init!
     */ 
    void Draw();

    /** Draw CCW or CW
     */    
    void SetFrontFaceCCW(bool ccw);
    
  protected:    
    virtual void Init_() = 0;
    virtual void Draw_() = 0;
    virtual void Release_() = 0;

    // Create Vertex Array Object
    GLuint vao_;
    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo_;
    // Create an element array
    GLuint ebo_;
    // Create shader program
    GLuint shader_program_;

  private:
    bool initialized_;
    bool front_face_changed_;
    GLenum prev_frontface_setting_[1];   
    GLenum curr_frontface_setting_[1];

  }; //end GLObj
  
  
  //
  // each basic obj contains at most ONE textre
  //
  class BasicObj : public GLObj {
  public:
    BasicObj () : GLObj() {
      num_element_ = 0;      
      transform_ = glm::mat4(1);
      model_ = glm::mat4(1);
      view_ = glm::lookAt(glm::vec3(3.0f, 5.0f, 9.0f),
                          glm::vec3(0.0f, 0.0f, 0.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f) );
    }//BasicObj ()

    void MulTransform(glm::mat4 const& transform); // accumulate transform
    void SetTransform(glm::mat4 const& transform); //apply in model
    void SetModel(glm::mat4 const& model);
    void SetView(glm::mat4 const& view);
    void SetProjection(glm::mat4 const& proj);
    
    void SetTexture(GLuint texture_act, GLuint texture_hnd, GLenum target) {      
      texture_.Set(texture_act, texture_hnd, target);
    }
    void SetTexture(GLuint texture_act, char const* filename, bool mipmap, GLint param) {      
      texture_.Set(texture_act, filename, mipmap, param);
    }

  protected:
    // interface
    virtual void Init_() = 0;
    virtual void Draw_() = 0;
    //TODO (lc@madison) release resource
    virtual void Release_() {};


    //// member function    
    // load buffer (e.g. vertex buffer), return buffer id
    GLuint LoadBuffer(GLenum target, GLenum usage, GLvoid const* data, GLsizeiptr size);
    // data type GL_FLOAT
    bool BindVbo3f(GLuint vbo, char const* a_position);
    bool BindVbo33f(GLuint vbo, char const* a_position, char const* a_normal);
    bool BindVbo332f(GLuint vbo, char const* a_position, char const* a_normal, char const* a_texture);    
    bool BindVbo3321f(GLuint vbo, char const* a_position, char const* a_normal, char const* a_texture, char const* a_control);
    bool BindEbo(GLuint vbo);
    
    //// member data    
    GLsizei num_element_; // glDrawElements(..., num_element_, ...)
    TextureMngr texture_;
    ShaderMVP shader_mvp_;   

    glm::mat4 transform_;
    glm::mat4 model_;
    glm::mat4 view_;
  };//end class BasicObj
  

  //
  // will check camera position whenever doing drawcall
  //
  class SceneObj : public BasicObj {
  public:
    SceneObj(Camera const& camera) : camera_(camera) {
    }
    
  protected:
    //// interface
    virtual void Init_() = 0;
    virtual void Draw_() = 0;
    //TODO (lc@madison) release resource
    virtual void Release_() {};

    Camera const& camera_;

    void SetupProgramVVP(GLuint program);

  }; //class SceneObj 

}// namespace cs559



#endif /* defined(__cs559fltk__basic__) */
