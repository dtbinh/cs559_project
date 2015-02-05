//
//  cubemap_ball.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/21/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "cubemap_ball.h"

#include <vector>
#include <string>
#include <ctime>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "rendering/opengl/shader_util.h"
#include "mesh/gen_primitive.h"

namespace cs559 {
  
  void CubemapBall::Init_() {
    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // generate sphere  
    Mesh* mesh = mesh::GenSphere(2.f, 64, 32);
    num_element_ = mesh->num_indx;

    // setup shader program
    shader_program_ = BuildShaderProgramFromFile("./shader/basic/cubemap_ball_vertex.glsl",
                                                 "./shader/basic/cubemap_ball_fragment.glsl");    
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vtx_data, mesh->vtx_size);
    BindVbo332f(vbo_, "a_position", "a_normal", nullptr);
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->idx_data, mesh->idx_size);
    BindEbo(ebo_);
    
    //// texture, cube map
    texture_.FetchCubeMap("i3dp_px.png", "i3dp_nx.png", "i3dp_py.png", "i3dp_ny.png", "i3dp_pz.png", "i3dp_nz.png", false);
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), 
      0 /* corresponding GL_TEXTURE0 */);
    
    //// Model View
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program 
    shader_mvp_.U_model("u_model");
    shader_mvp_.U_model_it("u_model_it");
    shader_mvp_.U_view("u_view");
    shader_mvp_.U_modelview_it("u_modelview_it");
    shader_mvp_.U_porj("u_proj");
    
    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f,  40.f);

    shader_mvp_.SetModelView(model_, view_);
    shader_mvp_.SetProjection(proj);

    //// light position
    GLint u_light = glGetUniformLocation(shader_program_, "u_light");
    glUniform3f(u_light, 0.f,  5.f,  2.f);
    
    GLint u_Ka = glGetUniformLocation(shader_program_, "u_Ka");
    GLint u_Kd = glGetUniformLocation(shader_program_, "u_Kd");
    GLint u_Ks = glGetUniformLocation(shader_program_, "u_Ks");
    glUniform3f(u_Ka, 0.6f, 0.6f, 0.6f);
    glUniform3f(u_Kd, 0.6f, 0.6f, 0.6f);
    glUniform3f(u_Ks, 0.8f, 0.8f, 0.8f);
    
    //// reset vao
    glBindVertexArray(0);
    CHECK_GL_ERROR(" CubemapBall::Init() Error \n");    
  }

  

  void CubemapBall::Draw_() {

    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    texture_.Bind();
    
    CHECK_GL_ERROR(" -- B1 \n");
    // rotate model
    /*glm::mat4 model_ = glm::rotate(glm::mat4(1),
                        0.5f*(GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC,
                        glm::vec3(0.0f, 1.0f, 0.0f) ); */  
    //shader_mvp_.SetModelView(transform_*model_, view_);
    
    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR(" CubemapBall::DrawCall() Error \n");
  }
  
} // namespace cs559