#include "cube.h"
#include <ctime>

#include "mesh/gen_primitive.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "rendering/opengl/texture_mngr.h"

namespace cs559 {

//static
//char const* vertex_shader_src = GLSL120(
//  attribute vec3 a_position;
//  uniform mat4 u_model;
//  uniform mat4 u_view;
//  uniform mat4 u_proj;
// 
//  void main() {
//    gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
//  } 
//);
//
//static
//char const* fragment_shader_src = GLSL120(
//  void main() {
//    gl_FragColor = vec4(1,1,1,1);
//  } 
//);
  void Cube::Init_() {
    Init_(1.f);
  }

  void Cube::Init_(float r) {
    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // generate sphere    
    Mesh* mesh = mesh::GenCube332(r);
    num_element_ = mesh->num_indx;

    // setup shader program
    shader_program_ = BuildShaderProgramFromFile(
      "./shader/basic/phong_vertex.glsl",
      "./shader/basic/phong_fragment.glsl");
    //shader_program_ = BuildShaderProgram(vertex_shader_src, fragment_shader_src);
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vtx_data, mesh->vtx_size);
    BindVbo332f(vbo_, "a_position", "a_normal", "a_texture");
    //BindVbo332f(vbo_, "a_position", nullptr, nullptr);
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->idx_data, mesh->idx_size);
    BindEbo(ebo_);
    
    // setup texture    
    texture_.Fetch("hello.png", false);
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), 0 /* corresponding GL_TEXTURE0 */);

    //// Model View
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program 
    shader_mvp_.U_model("u_model");    
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
    glUniform3f(u_Kd, 0.8f, 0.8f, 0.8f);
    glUniform3f(u_Ks, 0.0f, 0.0f, 0.0f);
    
    //// reset vao
    glBindVertexArray(0);
    CHECK_GL_ERROR(" Cube::Init() \n");
  }

  void Cube::UpdateModel_() {
    // rotate model
    glm::mat4 model = glm::rotate(glm::mat4(1),
                        5.f*(GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC,
                        glm::vec3(1.0f, 0.0f, 0.0f) );
    SetModel(model);
  }

  void Cube::Draw_() {    
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);    
    texture_.Bind();    
    UpdateModel_();  
    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);
  }
} // namespace cs559