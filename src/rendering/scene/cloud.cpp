#include "rendering/scene/cloud.h"

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
  static char const* cloud_vertex_shader_src = GLSL120(
    attribute  vec3  a_position;   
    attribute  vec3  a_normal;
    attribute  vec2  a_tex_coord;
    attribute float  a_control;
    uniform    mat4  u_model;
    uniform    mat4  u_view;
    uniform    mat4  u_proj;
    varying    vec2  v_tex_coord;
    varying   float  v_cloud_id;

    void main() {
      v_tex_coord = a_tex_coord;
      vec4 pos = u_model*vec4(a_position, 1.0) + vec4(a_normal, 0);
      v_cloud_id = a_control;
      gl_Position = u_proj*u_view*pos;
    }
  );
  
  /** Fragment Shader
   */
  static char const* cloud_fragment_shader_src = GLSL120(    
    uniform sampler2D  u_tex_cloud0;
    uniform sampler2D  u_tex_cloud1;
    uniform sampler2D  u_tex_cloud2;
    uniform     float  u_time;
    varying      vec2  v_tex_coord;
    varying     float  v_cloud_id;

    void main() {
      if (v_cloud_id < 0.16) {
        gl_FragColor = texture2D(u_tex_cloud0, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_cloud_id < 0.33) {
        gl_FragColor = texture2D(u_tex_cloud0, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_cloud_id < 0.50) {
        gl_FragColor = texture2D(u_tex_cloud1, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_cloud_id < 0.66) {
        gl_FragColor = texture2D(u_tex_cloud1, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_cloud_id < 0.83) {
        gl_FragColor = texture2D(u_tex_cloud2, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else {
        gl_FragColor = texture2D(u_tex_cloud2, vec2(v_tex_coord.x, v_tex_coord.y)); 
      }      
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

  /**
   */
  void Cloud::Init_() {
    float width  = 18.f;
    float height = 9.f;
    int num_clouds = 30;
    cloud_ = mesh::BillBoards3321(num_clouds, width, height);    
    num_element_ = cloud_->num_indx;
    
    vcloud_.resize(num_clouds);

    // assign cloud postion
    float cloud_height = 30;
    float sft_scale = 49;
    for (int i = 0; i < num_clouds; ++i) {
      float v = random();
      float r2 = random2();
      float r3 = random2();
      // position
      vcloud_[i].x = r2*sft_scale;
      vcloud_[i].y = cloud_height;
      vcloud_[i].z = r3*sft_scale;
      vcloud_[i].w = v;
      for (int j = 0; j < 4; ++j) {
        // normal
        cloud_->vtx_data[i*36 + j*9 + 3] = vcloud_[i].x;
        cloud_->vtx_data[i*36 + j*9 + 4] = vcloud_[i].y;
        cloud_->vtx_data[i*36 + j*9 + 5] = vcloud_[i].z;
        // control
        cloud_->vtx_data[i*36 + j*9 + 8] = vcloud_[i].w;        
      }
      
    }

    // setup shader program
    shader_program_ = BuildShaderProgram(cloud_vertex_shader_src, cloud_fragment_shader_src);
    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);

    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // vbo id
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, cloud_->vtx_data, cloud_->vtx_size);
    BindVbo3321f(vbo_, "a_position", "a_normal", "a_tex_coord", "a_control");
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, cloud_->idx_data, cloud_->idx_size);
    BindEbo(ebo_);
 
    // setup texture
    texture_.Fetch("cloud0.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_cloud0"), 
      0 /* corresponding GL_TEXTURE0 */);
    texture_.Fetch("cloud1.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_cloud1"), 
      1 /* corresponding GL_TEXTURE1 */);
    texture_.Fetch("cloud2.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_cloud2"), 
      2 /* corresponding GL_TEXTURE2 */);

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
  void Cloud::Draw_() {
    opengl_util_.StoreState();
    SetupProgramVVP(shader_program_);
    
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    texture_.Bind(GL_TEXTURE0, "cloud0.png", true);
    texture_.Bind(GL_TEXTURE1, "cloud1.png", true);
    texture_.Bind(GL_TEXTURE2, "cloud2.png", true);

    UpdateModel(glm::vec3(0.f, 0.2f, -0.99f));

    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);

    opengl_util_.ReStoreState();
    CHECK_GL_ERROR(" Trees::Draw_() Error \n");
  }

  struct CompareByZ{
    glm::mat4 view;    
    bool operator()(const glm::vec4 &a, const glm::vec4 &b) {          
      // gl_Position = u_proj * u_view*u_model*vec4(a_position, 1.0) + vec4(a_normal, 0);
      /*
      glm::vec4 aa = view*(model*glm::vec4(0,0,0,1) + view*glm::vec4(a,0));
      glm::vec4 bb = view*(model*glm::vec4(0,0,0,1) + view*glm::vec4(b,0));*/
      glm::vec3 aa(a.x, a.y, a.z);
      glm::vec3 bb(b.x, b.y, b.z);
      return (view*glm::vec4(aa,0)).z < (view*glm::vec4(bb,0)).z;      
    }
  }; 


  /**
   */
  static
  void UpdateMesh2(Mesh*& cloud, std::vector<glm::vec4> & vcloud, std::vector<glm::vec4> & cloud_bk) {  
    int vtx_wid = 9;
    int num_vtx_per_tree = 36;    
    int num_tree = vcloud.size();
    float*& treedata = cloud->vtx_data;

    for (int i = 0; i < num_tree; ++i) {      
      glm::vec4 d = vcloud[i] - cloud_bk[i];  
      float control =  vcloud[i].w;
      for (int j = 0; j < 4; ++j) {
        treedata[i*num_vtx_per_tree+ j*vtx_wid + 3] += d.x;
        treedata[i*num_vtx_per_tree+ j*vtx_wid + 4] += d.y;
        treedata[i*num_vtx_per_tree+ j*vtx_wid + 5] += d.z;
        treedata[i*num_vtx_per_tree+ j*vtx_wid + 8]  = control;
      }
    }
  }


  /**
   */
  void Cloud::UpdateModel(glm::vec3 const& pos) {
    glm::vec3 eye = camera_.EyeDir();
    float deg = atan2(-eye.z, eye.x) + 0.5f*3.1415926526f/*90deg*/;
    glm::mat4 model = glm::translate(pos);
    model = glm::rotate(model, deg, glm::vec3(0.f, 1.f, 0.f));    
    SetModel(model);

    CompareByZ cbz;    
    cbz.view = camera_.View();    
    std::vector<glm::vec4> cloud_bk = vcloud_; 
    std::sort(vcloud_.begin(), vcloud_.end(), cbz);
    // make cloud moving
    for (auto& c : vcloud_) {
      c.x += 0.2f;
      if (c.x > 49) {
        c.x = -49;
      }
    }
    UpdateMesh2(cloud_, vcloud_, cloud_bk);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData (GL_ARRAY_BUFFER, 0, cloud_->vtx_size, cloud_->vtx_data);
    CHECK_GL_ERROR(" Trees::UpdateModel() Error \n");    
  }
  
} // namespace cs559