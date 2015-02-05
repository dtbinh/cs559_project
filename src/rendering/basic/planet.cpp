//
//  planet.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/19/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "rendering/basic/planet.h"

#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "rendering/opengl/shader_util.h"
#include "mesh/gen_primitive.h"

namespace cs559 {
  // Init
  //
  void ObjEarth::Init_() {        
    // setup shader program
    shader_program_ = BuildShaderProgramFromFile("./shader/basic/earth_vertex.glsl",
                                                 "./shader/basic/earth_fragment.glsl");    
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // vao
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    // mesh
    Mesh* mesh = mesh::GenSphere(2.f, 64, 32);
    num_element_ = mesh->num_indx;
    // vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vtx_data, mesh->vtx_size);
    BindVbo332f(vbo_, "a_position", "a_normal", "a_texture");
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->idx_data, mesh->idx_size);
    BindEbo(ebo_);
    
    // setup texture
    texture_.Fetch("earth.png", false);
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), 
      0 /* corresponding GL_TEXTURE0 */ );
    
    // model
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program 
    shader_mvp_.U_model("model");
    shader_mvp_.U_view("view");
    shader_mvp_.U_porj("proj");

    glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f,  40.f);

    shader_mvp_.SetModelView(model_, view_);
    shader_mvp_.SetProjection(proj);

    // lighting
    GLint u_light_pos = glGetUniformLocation(shader_program_, "u_light_pos");
    glUniform3f(u_light_pos, 2.f, 3.f, 2.f);
    
    // reset vao
    glBindVertexArray(0);
    CHECK_GL_ERROR(" ObjEarth::Init() Error \n");
  }//end Init()
  
  
  //
  //
  void ObjEarth::Draw_() {
    //---- Link Program will reset the uniform links
    // glLinkProgram(shaderProgram_);
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    texture_.Bind();

    // Calculate transformation
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(-1.5f, 0.9f, 4.4f));    
    model = glm::rotate(
                        model,
                        5.f*(GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC * 180.0f,
                        glm::vec3(0.0f, 1.0f, 0.0f)
                        );    
    SetModel(model);
   
    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);    
    printf(" draw call: Earth \n");
  }//End DrawCall()

} // namespace cs559