//
//  stencil.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/24/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "rendering/technique/ex_stencil.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "mesh/gen_primitive.h"
#include "rendering/opengl/shader_util.h"
#include "rendering/opengl/stencil.h"

namespace cs559 {

static
char const* vertex_shader_src = GLSL120(
  attribute vec3 a_position;
  uniform mat4 u_model;
  uniform mat4 u_view;
  uniform mat4 u_proj;
 
  void main() {
    gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
  } 
);

static
char const* fragment_shader_src = GLSL120(
  void main() {
    gl_FragColor = vec4(1,1,1,1);
  } 
);
  
  void ExStencil::Init_() {
    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // generate xz plane
    Mesh* plane = mesh::PlaneXZ332(4);
    num_element_ = plane->num_indx;
    
    // setup shader program
    shader_program_ = BuildShaderProgram(vertex_shader_src, fragment_shader_src);
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, plane->vtx_data, plane->vtx_size);
    BindVbo332f(vbo_, "a_position", nullptr, nullptr);
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, plane->idx_data, plane->idx_size);
    BindEbo(ebo_);
    
    //// Model View
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program
    shader_mvp_.U_model("u_model");    
    shader_mvp_.U_view("u_view");   
    shader_mvp_.U_porj("u_proj");
    
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f,  40.f);

    shader_mvp_.SetModelView(model_, view_);
    shader_mvp_.SetProjection(proj);

    // reset vao
    glBindVertexArray(0);
    
    //
    cubemap_.Init();
    cube_.Init();
    cube_.SetModel(glm::translate(glm::vec3(0,0,-2)));
    //
    CHECK_GL_ERROR(" Stencil::Init() Error \n");
  }
  
  /**
   */
  void ExStencil::Draw_() { 
    //because we're inside the box, frontface is CW
    cubemap_.SetFrontFaceCCW(false);
    cubemap_.Draw();

    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);

    // draw stencil       
    stencil_.DrawBeg(GL_NEVER, 0x01, 0xFF, GL_REPLACE, GL_KEEP, GL_KEEP);
    {
      glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);     
    }
    stencil_.DrawEnd();
    
    //OpenGL_UTIL::ReadStencilTo("stencil.png", 800, 600, true, 0 /*threshold, or you see nothing*/);

    // draw relfection
    stencil_.UseBeg(GL_EQUAL, 0x01, 0xFF);
    {
      cubemap_.MulTransform(OpenGL_UTIL::GetMirrorY());
      cubemap_.SetFrontFaceCCW(true);
      cubemap_.Draw();
      cubemap_.MulTransform(OpenGL_UTIL::GetMirrorY());
      
      glm::vec3 ball_pos(0.f, 2.f, 0.f);
      cube_.SetTransform(glm::translate(ball_pos));
      cube_.MulTransform(OpenGL_UTIL::GetMirrorY());
      cube_.SetFrontFaceCCW(false);
      cube_.Draw();
      cube_.MulTransform(OpenGL_UTIL::GetMirrorY());
    }
    stencil_.UseEnd();
    
    // normal draw
    cube_.SetFrontFaceCCW(true);
    cube_.Draw();

    CHECK_GL_ERROR(" Stencil::DrawCall() Error \n");
  }//DrawCall()
  
} // namespace cs559