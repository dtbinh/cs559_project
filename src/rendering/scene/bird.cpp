#include "rendering/scene/bird.h"

#include <ctime>
#include <algorithm>    // std::sort

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "mesh/load_terrain.h"
#include "mesh/gen_primitive.h"

namespace cs559 {
  /** Vertex Shader
  */
  static char const* bird_vertex_shader_src = GLSL120(
  attribute  vec3  a_position;   
  attribute  vec3  a_normal;
  attribute  vec2  a_tex_coord;
  attribute float  a_control;
  uniform    mat4  u_model;
  uniform    mat4  u_view;
  uniform    mat4  u_proj;
  varying    vec2  v_tex_coord;
  varying   float  v_bird_id;

  void main() {
    v_tex_coord = a_tex_coord;
    vec4 pos = u_model*vec4(a_position, 1.0);
    v_bird_id = a_control;
    gl_Position = u_proj*u_view*pos;
  }
  );

  /** Fragment Shader
  */
  static char const* bird_fragment_shader_src = GLSL120(    
    uniform sampler2D  u_tex_bird; 
    uniform     float  u_time;
    varying      vec2  v_tex_coord;
    varying     float  v_bird_id;

  void main() {    
      gl_FragColor = texture2D(u_tex_bird, vec2(v_tex_coord.x, v_tex_coord.y));           
  }
  );


  /**
  */
  static
    int UpdateMesh3321(Mesh*& trees, Mesh*& terrain, float thld_y, int max_count, std::vector<glm::vec4> & vtrees) {
      int vtx_wid = 9;
      int num_vtx_per_tree = 36;

      float*& terraindata = terrain->vtx_data;
      int step = vtx_wid*sizeof(float);
      int terrain_num_vtx = terrain->vtx_size/step;
      int count = 0;

      vtrees.resize(max_count);

      float*& treedata = trees->vtx_data;

      for (int i = 0; i < terrain_num_vtx; ++i) {
        if (terraindata[i*8 + 1/*y index*/] > thld_y) {  
          vtrees[count].x = terraindata[i*8 + 0];
          vtrees[count].y = terraindata[i*8 + 1];
          vtrees[count].z = terraindata[i*8 + 2];        
          for (int j = 0; j < 4; ++j) {
            treedata[count*num_vtx_per_tree+ j*vtx_wid + 3] = vtrees[count].x;
            treedata[count*num_vtx_per_tree+ j*vtx_wid + 4] = vtrees[count].y;
            treedata[count*num_vtx_per_tree+ j*vtx_wid + 5] = vtrees[count].z;
          }        
          ++count;        
        }
      }    
      return count;
  }


  void Bird::Init_() {
    float width  = 0.1f;
    float height = 2.f;
    int num_birds = 1;
    bird_ = mesh::Bird3321(num_birds, width, height);    
    num_element_ = bird_->num_indx;

    vbird_.resize(num_birds);

    // assign cloud postion
    float bird_height = 30;
    float sft_scale = 49;
    for (int i = 0; i < num_birds; ++i) {
      float v = random();
      float r2 = random2();
      float r3 = random2();
      // position
      vbird_[i].x = r2*sft_scale;
      vbird_[i].y = bird_height;
      vbird_[i].z = r3*sft_scale;
      vbird_[i].w = v;
      for (int j = 0; j < 4; ++j) {
        // normal
        bird_->vtx_data[i*36 + j*9 + 3] = vbird_[i].x;
        bird_->vtx_data[i*36 + j*9 + 4] = vbird_[i].y;
        bird_->vtx_data[i*36 + j*9 + 5] = vbird_[i].z;
        // control
        bird_->vtx_data[i*36 + j*9 + 8] = vbird_[i].w;        
      }

    }

    // setup shader program
    shader_program_ = BuildShaderProgram(bird_vertex_shader_src, bird_fragment_shader_src);
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);

    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // vbo id
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, bird_->vtx_data, bird_->vtx_size);
    BindVbo3321f(vbo_, "a_position", "a_normal", "a_tex_coord", "a_control");
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, bird_->idx_data, bird_->idx_size);
    BindEbo(ebo_);

    // setup texture
    texture_.Fetch("bird.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_bird"), 
      0 /* corresponding GL_TEXTURE0 */);

    //// Model View
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program 
    shader_mvp_.U_model("u_model");
    shader_mvp_.U_view("u_view");
    shader_mvp_.U_porj("u_proj");
    //SetModel(model);
    SetView(camera_.View());
    SetProjection(camera_.Projection());

    glUniform1f(glGetUniformLocation(shader_program_, "u_time"), 0.1f);

    //// reset vao
    glBindVertexArray(0);
    CHECK_GL_ERROR(" Terrain::Init() Error \n");
  }

  /**
   */
  void Bird::Draw_() {
    opengl_util_.StoreState();
    SetupProgramVVP(shader_program_);

    texture_.Bind(GL_TEXTURE0, "bird.png", true);

    UpdateModel(glm::vec3(0.f, 0.2f, -0.99f));

    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);

    opengl_util_.ReStoreState();
    CHECK_GL_ERROR(" Trees::Draw_() Error \n");
  }

   /**
   */
  static
  void UpdateMesh2(Mesh*& cloud, std::vector<glm::vec4> & vcloud, float t) {  
    int vtx_wid = 9;
    int num_vtx_per_tree = 54;    
    int num_tree = vcloud.size();
    float*& treedata = cloud->vtx_data;

    for (int i = 0; i < num_tree; ++i) {              
      float control =  vcloud[i].w;
      float y = 0.1*sin(1.1f*t);
      if (y < 0.3f && y > -0.9) {
        treedata[i*num_vtx_per_tree +  1] = y;
        treedata[i*num_vtx_per_tree + 10] = y;
        treedata[i*num_vtx_per_tree + 37] = y;
        treedata[i*num_vtx_per_tree + 46] = y;
      }
    }
  }

  /**
   */
  void Bird::UpdateModel(glm::vec3 const& pos) {
    float r = 3;
    float t = camera_.Time();
    float px = r*cos(0.5*t);
    float pz = r*sin(0.5*t);
    float py = 3;//height
    glm::mat4 model = glm::translate(glm::vec3(px, py, pz));
    model = glm::rotate(model, 0.5f*t, glm::vec3(0.f, 1.f, 0.f));    
    SetModel(model);

    UpdateMesh2(bird_, vbird_, camera_.Time());

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData (GL_ARRAY_BUFFER, 0, bird_->vtx_size, bird_->vtx_data);
    CHECK_GL_ERROR(" Trees::UpdateModel() Error \n");    
  }

} // namespace cs559
