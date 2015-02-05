#include "rendering/scene/skybox.h"

#include <vector>
#include <string>
#include <ctime>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "rendering/opengl/shader_util.h"
#include "misc/util.h"
#include "mesh/gen_primitive.h"

namespace cs559 {
  
  void Skybox::Init_() {    
    // setup shader program
    shader_program_ = BuildShaderProgramFromFile("./shader/basic/cubemap_vertex.glsl",
                                                 "./shader/basic/cubemap_fragment.glsl");
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    float r = 50.f;    
    Mesh* mesh = mesh::GenSphere(r, 10, 10);
    num_element_ = mesh->num_indx;

    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vtx_data, mesh->vtx_size);
    BindVbo332f(vbo_, "a_position", nullptr, nullptr);
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->idx_data, mesh->idx_size);
    BindEbo(ebo_);        
    
    // texture, cube map    
    texture_.FetchCubeMap("sky_nx.png", "sky_px.png", "sky_py.png", "sky_ny.png", "sky_nz.png", "sky_pz.png", false);
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), 
      0 /* corresponding GL_TEXTURE0 */);
  
    //// Model View
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program 
    shader_mvp_.U_model("u_model");    
    shader_mvp_.U_view("u_view");    
    shader_mvp_.U_porj("u_proj");
    
    shader_mvp_.SetModelView(model_, camera_.View());
    shader_mvp_.SetProjection(camera_.Projection());
    
    // reset vao
    glBindVertexArray(0);
    CHECK_GL_ERROR("Cubemap::Init() Error");
  }
  
  void Skybox::UpdateView_() {   
    glm::mat4 view = camera_.View();
    view[3][0] = 0;
    view[3][1] = 0;
    view[3][2] = 0;
    SetView(view);

    //SetView(camera_.View()); 
  }// UpdateView()
  
  void Skybox::Draw_() {
    glutil_.StoreState();
    glDepthMask(false);

    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    texture_.Bind();
    
    UpdateView_(); 

    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);    
    glutil_.ReStoreState();
  }// DrawCall()
  
} // namespace cs559
