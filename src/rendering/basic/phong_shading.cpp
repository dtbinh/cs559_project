//
//  phong_shading.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/19/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "phong_shading.h"

#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering/opengl/shader_util.h"
#include "primitive/gen_model.h"

namespace cs559 {
  
  void Ball::Init() {
    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // generate sphere
    float r = 2.f;
    int nx = 64, ny = 32;
    int    vtx_size = 0;
    float* vtx_data = nullptr;
    int           idx_size = 0;
    unsigned int* idx_data = nullptr;
    num_element_ = GenSphere(r, nx, ny, vtx_data, vtx_size, idx_data, idx_size);
    
    // setup shader program
    shader_program_ = BuildShaderProgramFromFile(
                                                "/Volumes/data/sync_space/Dropbox/workspace/UW_CS/14Fall/559_computer_graphic/hw/project2/cs559fltk/cs559fltk/shader/basic/phong_vertex.glsl",
                                                "/Volumes/data/sync_space/Dropbox/workspace/UW_CS/14Fall/559_computer_graphic/hw/project2/cs559fltk/cs559fltk/shader/basic/phong_fragment.glsl");
    
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vtx_data, sizeof(float)*vtx_size);
    BindVbo332f(vbo_, "a_position", "a_normal", "a_texture");
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, idx_data, sizeof(unsigned int)*idx_size);
    BindEbo(ebo_);
    
    // setup texture
    GLuint act_tex = GL_TEXTURE2;
    glActiveTexture(act_tex);
    char const* tex_file = "/Volumes/data/sync_space/Dropbox/workspace/UW_CS/14Fall/559_computer_graphic/hw/project2/texture/moon.png";
    GLuint texture_bind_ = GenBindTexture(tex_file);
    GLint const texture_idx = act_tex - GL_TEXTURE0;
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), texture_idx);
    
    // model
    u_model_ = glGetUniformLocation(shader_program_, "u_model");
    glm::mat4 model;
    model = glm::translate(model, glm::vec3( -2.0f, 1.5f, -5.0f));

    glUniformMatrix4fv(u_model_, 1, GL_FALSE, glm::value_ptr(model));

    // view
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 9.0f),
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f) );
    GLint u_view = glGetUniformLocation(shader_program_, "u_view");
    glUniformMatrix4fv(u_view, 1, GL_FALSE, glm::value_ptr(view));
    
    // modelview invert traspose
    glm::mat4 modelview_it = glm::transpose(glm::inverse(view*model));
    u_modelview_it_ = glGetUniformLocation(shader_program_, "u_modelview_it");
    glUniformMatrix4fv(u_modelview_it_, 1, GL_FALSE, glm::value_ptr(modelview_it));

    // projection
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f,  20.f);
    GLint u_proj = glGetUniformLocation(shader_program_, "u_proj");
    glUniformMatrix4fv(u_proj, 1, GL_FALSE, glm::value_ptr(proj));
    
    // light position
    GLint u_light = glGetUniformLocation(shader_program_, "u_light");
    glUniform3f(u_light, 0.f,  5.f,  2.f);
    
    GLint u_Ka = glGetUniformLocation(shader_program_, "u_Ka");
    GLint u_Kd = glGetUniformLocation(shader_program_, "u_Kd");
    GLint u_Ks = glGetUniformLocation(shader_program_, "u_Ks");
    glUniform3f(u_Ka, 0.1f, 0.1f, 0.1f);
    glUniform3f(u_Kd, 0.5f, 0.5f, 0.5f);
    glUniform3f(u_Ks, 0.8f, 0.8f, 0.8f);
    
    // reset vao
    glBindVertexArray(0);
    CheckGLError(" Ball::Init() Error \n");
  }
  
  
  void Ball::DrawCall() {
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);    
    printf(" draw call: Phong Shading \n");
  }
  
  
} //namespace cs559