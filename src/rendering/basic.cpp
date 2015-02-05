//
//  basic.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/19/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "rendering/basic.h"

namespace cs559 {
  //============================================================
  // class GLObj
  //

  /** Do init before drawing!
   */
  void GLObj::Init() {
    if (initialized_) {
      return;
    }
    Init_();
    initialized_ = true;
    glEnableVertexAttribArray(0); // reset vbo
    glBindVertexArray(0); // reset vao

    if (front_face_changed_) {
      glGetIntegerv(GL_FRONT_FACE, (GLint*)prev_frontface_setting_);
    }
  }// void GLObj::Init()

  /** Do Draw after init!
    */ 
  void GLObj::Draw()  {
    assert(initialized_);
    
    //TODO: should we check if GL_CULL_FACE is enabled 
    //      when front_face_changed_ is set.
    if (front_face_changed_) { 
      glFrontFace(curr_frontface_setting_[0]);
    }

    Draw_();

    if (front_face_changed_) {         
      glFrontFace(prev_frontface_setting_[0]);      
    }
  }

  /** Set Front Face direction CCW or CW
   */
  void GLObj::SetFrontFaceCCW(bool ccw) {
    front_face_changed_ = true;
    curr_frontface_setting_[0] = ( ccw ? GL_CCW : GL_CW );
  }

  //============================================================
  // class BasicObj
  //

  /**
   */
  void BasicObj::MulTransform(glm::mat4 const& transform) {
    SetTransform(transform*transform_);    
  }

  /**
   */
  void BasicObj::SetTransform(glm::mat4 const& transform) {
    glUseProgram(shader_program_);    
    transform_ = transform;  
    shader_mvp_.SetModelView(transform_*model_, view_);
  }

  /**
   */
  void BasicObj::SetModel(glm::mat4 const& model) {
    glUseProgram(shader_program_);
    model_ = model;
    shader_mvp_.SetModelView(transform_*model_, view_);
  }

  /**
   */
  void BasicObj::SetView(glm::mat4 const& view) {
    glUseProgram(shader_program_);    
    view_ = view;
    shader_mvp_.SetModelView(transform_*model_, view_);
  }

  /**
   */
  void BasicObj::SetProjection(glm::mat4 const& proj) {
    glUseProgram(shader_program_);
    shader_mvp_.SetProjection(proj);
  }

  /**
   */
  GLuint BasicObj::LoadBuffer(GLenum target, GLenum usage, GLvoid const* data, GLsizeiptr size) {
    GLuint buffer_obj = 0;
    glGenBuffers(1, &buffer_obj);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_obj);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    return buffer_obj;
  }
  
  
  /** vbo: position
   */
  bool BasicObj::BindVbo3f(GLuint vbo, char const* a_position) {
    int num_pos  = 3;
    long ofs_pos  = 0;
    int stride    = sizeof(GLfloat) * (num_pos);
    
    GLint position_Attrib = glGetAttribLocation(shader_program_, a_position);

    glEnableVertexAttribArray(position_Attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(position_Attrib, num_pos, GL_FLOAT, GL_FALSE, stride, (void*)ofs_pos);
    
    CHECK_GL_ERROR("BindVbo3");
    return true;
  } //end GLObj::bind_vbo332()
  
  
  /** vbo: position + normal
   */
  bool BasicObj::BindVbo33f(GLuint vbo, char const* a_position, char const* a_normal) {
    int num_pos  = 3;
    int num_nrm  = 3;
    long ofs_pos  = 0;
    long ofs_nrm  = sizeof(GLfloat) * (num_pos);
    int stride    = sizeof(GLfloat) * (num_pos + num_nrm);
    
    GLint position_Attrib = glGetAttribLocation(shader_program_, a_position);
    glEnableVertexAttribArray(position_Attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(position_Attrib, num_pos, GL_FLOAT, GL_FALSE, stride, (void*)ofs_pos);

    if (nullptr != a_normal) {
      GLint normal_Attrib   = glGetAttribLocation(shader_program_, a_normal);
      glEnableVertexAttribArray(normal_Attrib);
      glVertexAttribPointer(normal_Attrib,   num_nrm, GL_FLOAT, GL_FALSE, stride, (void*)ofs_nrm);
    }
    
    CHECK_GL_ERROR("BindVbo33f");
    return true;
  } //end GLObj::bind_vbo332()
  
  
  /**  vbo: position + normal + texture
   */
  bool BasicObj::BindVbo332f(GLuint vbo, char const* a_position, char const* a_normal, char const* a_texture) {
    int num_pos  = 3;
    int num_nrm  = 3;
    int num_tex  = 2;
    long ofs_pos  = 0;
    long ofs_nrm  = sizeof(GLfloat) * (num_pos);
    long ofs_tex  = sizeof(GLfloat) * (num_pos + num_nrm);
    int stride    = sizeof(GLfloat) * (num_pos + num_nrm + num_tex);
     
    assert(nullptr != a_position);
    GLint position_Attrib = glGetAttribLocation(shader_program_, a_position);
    glEnableVertexAttribArray(position_Attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(position_Attrib, num_pos, GL_FLOAT, GL_FALSE, stride, (void*)ofs_pos);
    
    if (nullptr != a_normal) {
      GLint normal_Attrib   = glGetAttribLocation(shader_program_, a_normal);
      glEnableVertexAttribArray(normal_Attrib);
      glVertexAttribPointer(normal_Attrib,   num_nrm, GL_FLOAT, GL_FALSE, stride, (void*)ofs_nrm);
    }
    
    if (nullptr != a_texture) {
      GLint texture_Attrib  = glGetAttribLocation(shader_program_, a_texture);
      glEnableVertexAttribArray(texture_Attrib);
      glVertexAttribPointer(texture_Attrib,  num_tex, GL_FLOAT, GL_FALSE, stride, (void*)ofs_tex);
    }
    
    CHECK_GL_ERROR("BindVbo332f");
    return true;
  } //end GLObj::bind_vbo332()
  
      

  bool BasicObj::BindVbo3321f(GLuint vbo, char const* a_position, char const* a_normal, char const* a_texture, char const* a_control) {
    int num_pos   = 3;
    int num_nrm   = 3;
    int num_tex   = 2;
    int num_ctl   = 1;
    long ofs_pos  = 0;
    long ofs_nrm  = sizeof(GLfloat) * (num_pos);
    long ofs_tex  = sizeof(GLfloat) * (num_pos + num_nrm);
    long ofs_ctl  = sizeof(GLfloat) * (num_pos + num_nrm + num_tex);
    int stride    = sizeof(GLfloat) * (num_pos + num_nrm + num_tex + num_ctl);
     
    assert(nullptr != a_position);
    GLint position_Attrib = glGetAttribLocation(shader_program_, a_position);
    glEnableVertexAttribArray(position_Attrib);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(position_Attrib, num_pos, GL_FLOAT, GL_FALSE, stride, (void*)ofs_pos);
    
    if (nullptr != a_normal) {
      GLint normal_Attrib   = glGetAttribLocation(shader_program_, a_normal);
      glEnableVertexAttribArray(normal_Attrib);
      glVertexAttribPointer(normal_Attrib,   num_nrm, GL_FLOAT, GL_FALSE, stride, (void*)ofs_nrm);
    }
    
    if (nullptr != a_texture) {
      GLint texture_Attrib  = glGetAttribLocation(shader_program_, a_texture);
      glEnableVertexAttribArray(texture_Attrib);
      glVertexAttribPointer(texture_Attrib,  num_tex, GL_FLOAT, GL_FALSE, stride, (void*)ofs_tex);
    }

    if (nullptr != a_control) {
      GLint control_Attrib  = glGetAttribLocation(shader_program_, a_control);
      glEnableVertexAttribArray(control_Attrib);
      glVertexAttribPointer(control_Attrib,  num_ctl, GL_FLOAT, GL_FALSE, stride, (void*)ofs_ctl);
    }
    
    CHECK_GL_ERROR("BindVbo332f");
    return true;
  } //end GLObj::bind_vbo332()


  /**
   */
  bool BasicObj::BindEbo(GLuint ebo) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);    
    CHECK_GL_ERROR("BindEbo");
    return true;
  }//end BasicObj::bind_ebo()
  


  //============================================================
  // class SceneObj
  //
  void SceneObj::SetupProgramVVP(GLuint program) {
    glUseProgram(program);
    glBindVertexArray(vao_);    
    SetView(camera_.View());
    SetProjection(camera_.Projection());
  }
}//namespace cs559