//
//  cubemap.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/20/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "rendering/basic/cubemap.h"

#include <ctime>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "rendering/opengl/shader_util.h"
#include "mesh/gen_primitive.h"

namespace cs559 {
  
  void Cubemap::Init_() {    
    // setup shader program
    shader_program_ = BuildShaderProgramFromFile("./shader/basic/cubemap_vertex.glsl",
                                                 "./shader/basic/cubemap_fragment.glsl");
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    float r = 15.f;    
    Mesh* mesh = mesh::GenCube332(r);
    num_element_ = mesh->num_indx;

    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vtx_data, mesh->vtx_size);
    BindVbo332f(vbo_, "a_position", nullptr, nullptr);
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->idx_data, mesh->idx_size);
    BindEbo(ebo_);        
    
    // texture, cube map    
    texture_.FetchCubeMap("sky_px.png", "sky_nx.png", "sky_py.png", "sky_ny.png", "sky_pz.png", "sky_nz.png", false);
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), 0 /* corresponding GL_TEXTURE0 */);
  
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
    CHECK_GL_ERROR(" Cubemap::Init() Error \n");
  }
  
  void Cubemap::UpdateView_() {    
    glm::vec3 eye = glm::rotate(glm::vec3(0.0f, 0.0f, 9.0f),
                      -0.1f*(GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC * 3.1415926f,
                      glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(eye,
                                 glm::vec3(0.0f, 0.0f, 0.0f),
                                 glm::vec3(0.0f, 1.0f, 0.0f) );
    SetView(view);
  }// UpdateView()
  
  void Cubemap::Draw_() {
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);    
    texture_.Bind();       
    UpdateView_();    
    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);    
    CHECK_GL_ERROR(" Cubemap::DrawCall() Error \n");
  }// DrawCall()
  
} // namespace cs559
