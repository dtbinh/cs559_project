#include "rendering/technique/terrain.h"

#include <ctime>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "mesh/load_terrain.h"
#include "mesh/gen_primitive.h"
namespace cs559 {

  void Terrain::Init_() {

    float r = 10.f;
    float h = 2.f;
    Mesh* mesh = mesh::LoadTerrainImg332("./asset/texture/terrain/terrain2.png", r, h);

    //mesh->RemoveLEQ_Y(-0.01f);
    //mesh->ScaleY(1.1f);

    num_element_ = mesh->num_indx;

    // setup shader program
    shader_program_ = BuildShaderProgramFromFile(
      "./shader/basic/terrain_vertex.glsl",
      "./shader/basic/terrain_fragment.glsl");

    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);

    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vtx_data, mesh->vtx_size);
    BindVbo332f(vbo_, "a_position", "a_normal", "a_texture");
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->idx_data, mesh->idx_size);
    BindEbo(ebo_);
 
    // setup texture    
    texture_.Fetch("terrain.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), 
      0 /* corresponding GL_TEXTURE0 */);

    //// Model View
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program 
    shader_mvp_.U_model("u_model");    
    shader_mvp_.U_view("u_view");
    shader_mvp_.U_modelview_it("u_modelview_it");
    shader_mvp_.U_porj("u_proj");

    shader_mvp_.SetModelView(model_, camera_.View());
    shader_mvp_.SetProjection(camera_.Projection());

    //// light position
    GLint u_light = glGetUniformLocation(shader_program_, "u_light");
    glUniform3f(u_light, 0.f,  5.f,  2.f);
    
    GLint u_Ka = glGetUniformLocation(shader_program_, "u_Ka");
    GLint u_Kd = glGetUniformLocation(shader_program_, "u_Kd");
    GLint u_Ks = glGetUniformLocation(shader_program_, "u_Ks");
    glUniform3f(u_Ka, 0.6f, 0.6f, 0.6f);
    glUniform3f(u_Kd, 0.8f, 0.8f, 0.8f);
    glUniform3f(u_Ks, 0.0f, 0.0f, 0.0f);

    // clip 
    GLint u_clip_dir = glGetUniformLocation(shader_program_, "u_clip_dir");
    glUniform3f(u_clip_dir, clip_dir_.x, clip_dir_.y, clip_dir_.z);
    GLint u_clip_pos = glGetUniformLocation(shader_program_, "u_clip_pos");
    glUniform3f(u_clip_pos, clip_pos_.x, clip_pos_.y, clip_pos_.z);

    //// reset vao
    glBindVertexArray(0);
    CHECK_GL_ERROR(" Terrain::Init() Error \n");
  }// Terrain::Init()


  void Terrain::Draw_() {
    SetupProgramVVP(shader_program_);    
    texture_.Bind();

    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);

    CHECK_GL_ERROR(" Terrain::Draw_() Error \n");
  }// Terrain::DrawCall()

  /**
   *
   */
  void Terrain::SetClipDirection(glm::vec3 dir, glm::vec3 pos) {
    clip_dir_ = dir;
    clip_pos_ = pos;   
  }

}// namespace cs559