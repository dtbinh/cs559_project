//
//  shader_util.h
//  cs559fltk
//
//  Created by Leon Chu on 11/11/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#ifndef __cs559fltk__shader_util__
#define __cs559fltk__shader_util__

#include <stdio.h>
#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#include "cs559core.h"

namespace cs559 {

  //
  // Handy class to do shader link / check
  //
  class ShaderMVP {
  public:
    ShaderMVP() :
      shader_program_(0),
      u_model_(-1), 
      u_view_(-1), 
      u_model_it_(-1), 
      u_modelview_it_(-1),
      u_porj_(-1) {
    }

    void SetShaderProgram(GLuint shader_program) {
      shader_program_ = shader_program;
      assert(0 < shader_program_);
    }

    // set
    void U_model(char const* str) {
      u_model_ = glGetUniformLocation(shader_program_, str);
      U_model();// assert check
    }
    void U_view(char const* str) {
      u_view_ = glGetUniformLocation(shader_program_, str);
      U_view();
    }
    void U_model_it(char const* str) {
      u_model_it_ = glGetUniformLocation(shader_program_, str);
      U_model_it();
    }
    void U_modelview_it(char const* str) {
      u_modelview_it_ = glGetUniformLocation(shader_program_, str);
      U_modelview_it();
    }
    void U_porj(char const* str) {
      u_porj_ = glGetUniformLocation(shader_program_, str);
      U_porj();
    }

    // get
    GLint U_model() const {
      assert(-1 < u_model_);
      return u_model_;
    }
    GLint U_view() const  {
      assert(-1 < u_view_);
      return u_view_;
    }
    GLint U_model_it() const  {
      assert(-1 < u_model_it_);
      return u_model_it_;
    }
    GLint U_modelview_it() const { 
      assert(-1 < u_modelview_it_);
      return u_modelview_it_;
    };
    GLint U_porj() const { 
      assert(-1 < u_porj_);
      return u_porj_;
    };

    // Model View
    //
    void SetModelView(glm::mat4 const& model, glm::mat4 const& view) {
      //// model
      glUniformMatrix4fv(u_model_, 1, GL_FALSE, glm::value_ptr(model));
      // model invert transpose
      if (-1 < u_model_it_) {
        glm::mat4 model_it = glm::transpose(glm::inverse(model));    
        glUniformMatrix4fv(u_model_it_, 1, GL_FALSE, glm::value_ptr(model_it));
      }
      //// view
      glUniformMatrix4fv(u_view_, 1, GL_FALSE, glm::value_ptr(view));
      if (-1 < u_modelview_it_) {
        //// modelview invert traspose
        glm::mat4 modelview_it = glm::transpose(glm::inverse(view*model));    
        glUniformMatrix4fv(u_modelview_it_, 1, GL_FALSE, glm::value_ptr(modelview_it));
      }
    }

    // projection
    //
    void SetProjection(glm::mat4 const& proj) {
      glUniformMatrix4fv(u_porj_, 1, GL_FALSE, glm::value_ptr(proj));
    }

  private:
    GLuint shader_program_;
    GLint u_model_;
    GLint u_view_;
    GLint u_model_it_;
    GLint u_modelview_it_;
    GLint u_porj_;
  };// class ShaderData
  

void LoadShader(char const* filename, char*& source, int& length);
GLuint BuildShaderProgram(const GLchar* vertex, const GLchar* fragment);  
GLuint BuildShaderProgramFromFile(char const* file_vertex, char const* file_fragment);

void SetupBufferObjs(GLuint& vbo, GLfloat* vbo_data, int vbo_size, GLuint& ebo, GLuint* ebo_data, int ebo_size);
 




}//end namespace cs669

#endif /* defined(__cs559fltk__shader_util__) */
