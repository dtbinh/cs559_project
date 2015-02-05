//
//  fillscreen.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/24/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//
#include "fillscreen.h"
#include "rendering/opengl/shader_util.h"

namespace cs559 {
  
  static char const* vertex_shader_src = GLSL120(
    attribute vec2 a_position;
    void main() {
      gl_Position = vec4(a_position, 0.0, 1.0);
    }
  );
  
  static char const* fragment_shader_src = GLSL120(
    uniform vec3 u_color;
    void main() {
      gl_FragColor = vec4(u_color,1);
    }
  );

  void Fillscreen::Init_() {
    shader_program_ = BuildShaderProgram(vertex_shader_src, fragment_shader_src);
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // two triangle to fill full screen
    float vertices[] = {
      -1.0f, -1.0f,      -1.0f,  1.0f,       1.0f,  1.0f,
       1.0f, -1.0f,      -1.0f, -1.0f,       1.0f,  1.0f,
       1.0f,  1.0f,      -1.0f,  1.0f,      -1.0f, -1.0f,
       1.0f,  1.0f,      -1.0f, -1.0f,       1.0f, -1.0f,       
    };
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    GLint posAttrib = glGetAttribLocation(shader_program_, "a_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);
    CHECK_GL_ERROR(" Fillscreen::Init() Error \n");
  }
  
  void Fillscreen::SetColor(float r, float g, float b) {
    glUseProgram(shader_program_);
    glUniform3f(glGetUniformLocation(shader_program_, "u_color"), r, g, b);
    CHECK_GL_ERROR(" Fillscreen::SetColor Error \n");
  }

  void Fillscreen::Draw_() {
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    CHECK_GL_ERROR(" Fillscreen::Draw_ Error \n");
  }
} // namespace cs559