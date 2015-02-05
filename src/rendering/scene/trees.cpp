#include "rendering/scene/trees.h"

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
  static char const* tree_vertex_shader_src = GLSL120(
    attribute  vec3  a_position;   
    attribute  vec3  a_normal;
    attribute  vec2  a_tex_coord;
    attribute float  a_control;
    uniform    mat4  u_model;
    uniform    mat4  u_view;
    uniform    mat4  u_proj;
    varying    vec2  v_tex_coord;
    varying   float  v_tree_id;

    void main() {
      v_tex_coord = a_tex_coord;
      vec4 pos = u_model*vec4(a_position, 1.0) + vec4(a_normal, 0);
      v_tree_id = a_control;
      gl_Position = u_proj*u_view*pos;
    }
  );
  
  /** Fragment Shader
   */
  static char const* tree_fragment_shader_src = GLSL120(    
    uniform sampler2D  u_tex_tree0;
    uniform sampler2D  u_tex_tree1;
    uniform sampler2D  u_tex_tree2;
    uniform sampler2D  u_tex_tree3;
    uniform sampler2D  u_tex_tree4;
    uniform sampler2D  u_tex_tree5;
    uniform     float  u_time;
    varying      vec2  v_tex_coord;
    varying     float  v_tree_id;

    void main() {
      if (v_tree_id < 0.16) {
        gl_FragColor = texture2D(u_tex_tree0, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_tree_id < 0.33) {
        gl_FragColor = texture2D(u_tex_tree1, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_tree_id < 0.50) {
        gl_FragColor = texture2D(u_tex_tree2, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_tree_id < 0.66) {
        gl_FragColor = texture2D(u_tex_tree3, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else if (v_tree_id < 0.83) {
        gl_FragColor = texture2D(u_tex_tree4, vec2(v_tex_coord.x, v_tex_coord.y));      
      } else {
        gl_FragColor = texture2D(u_tex_tree5, vec2(v_tex_coord.x, v_tex_coord.y)); 
      }      
    }
  );

  /**
   */
  int NumTrees332(Mesh*& terrain, float thld_y) {
    float*& terraindata = terrain->vtx_data;
    int step = 8*sizeof(float);
    int terrain_num_vtx = terrain->vtx_size/step;
    int count = 0;
    for (int i = 0; i < terrain_num_vtx; ++i) {
      if (terraindata[i*8 + 1/*y index*/] > thld_y) {
        ++count;
      }
    }
    return count;
  }

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
  void Trees::Init_() {    
    float width  = 0.4f;
    float height = 0.4f;
    float r = 10.f;
    float h = 2.f;
    thld_y_ = 0.2f;

    terrainmesh_ = mesh::LoadTerrainImg332("./asset/texture/terrain/terrain2.png", r, h);            
    num_trees_ = NumTrees332(terrainmesh_, thld_y_);

    treemesh_ = mesh::BillBoards3321(num_trees_, width, height);
    UpdateMesh3321(treemesh_, terrainmesh_, thld_y_, num_trees_, vtrees_);
    num_element_ = treemesh_->num_indx;

    for (int i = 0; i < num_trees_; ++i) {
      float v = random();
      treemesh_->vtx_data[i*36+ 8] = v;
      treemesh_->vtx_data[i*36+17] = v;
      treemesh_->vtx_data[i*36+26] = v;
      treemesh_->vtx_data[i*36+35] = v;
      vtrees_[i].w = v;
    }

    // setup shader program
    shader_program_ = BuildShaderProgram(tree_vertex_shader_src, tree_fragment_shader_src);

    glBindFragDataLocation(shader_program_, 0, "outColor");
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);

    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    // vbo id
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, treemesh_->vtx_data, treemesh_->vtx_size);
    BindVbo3321f(vbo_, "a_position", "a_normal", "a_tex_coord", "a_control");
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, treemesh_->idx_data, treemesh_->idx_size);
    BindEbo(ebo_);
 
    // setup texture
    texture_.Fetch("tree1a.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_tree0"), 
      0 /* corresponding GL_TEXTURE0 */);
    texture_.Fetch("tree2a.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_tree1"), 
      1 /* corresponding GL_TEXTURE1 */);
    texture_.Fetch("tree3a.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_tree2"), 
      2 /* corresponding GL_TEXTURE2 */);

    texture_.Fetch("tree4a.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_tree3"), 
      3 /* corresponding GL_TEXTURE3 */);
    texture_.Fetch("tree5a.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_tree4"), 
      4 /* corresponding GL_TEXTURE4 */);
    texture_.Fetch("tree6a.png", true);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_tree5"), 
      5 /* corresponding GL_TEXTURE5 */);

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
  void UpdateMesh2(Mesh*& trees, std::vector<glm::vec4> & vtrees, std::vector<glm::vec4> & trees_bk) {  
    int vtx_wid = 9;
    int num_vtx_per_tree = 36;    
    int num_tree = vtrees.size();
    float*& treedata = trees->vtx_data;
    for (int i = 0; i < num_tree; ++i) {
      glm::vec4 d =  vtrees[i] - trees_bk[i];  
      float control =  vtrees[i].w;
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
  void Trees::UpdateModel(glm::vec3 const& pos) {
    glm::vec3 eye = camera_.EyeDir();
    float deg = atan2(-eye.z, eye.x) + 0.5f*3.1415926526f/*90deg*/;
    glm::mat4 model = glm::translate(pos);
    model = glm::rotate(model, deg, glm::vec3(0.f, 1.f, 0.f));    
    SetModel(model);

    CompareByZ cbz;    
    cbz.view = camera_.View();    
    std::vector<glm::vec4> trees_bk = vtrees_; 
    std::sort(vtrees_.begin(), vtrees_.end(), cbz);
    UpdateMesh2(treemesh_, vtrees_, trees_bk);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferSubData (GL_ARRAY_BUFFER, 0, treemesh_->vtx_size, treemesh_->vtx_data);
    CHECK_GL_ERROR(" Trees::UpdateModel() Error \n");    
  }
 

  /**
   */
  void Trees::Draw_() {
    opengl_util_.StoreState();
    SetupProgramVVP(shader_program_); 
    
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    texture_.Bind(GL_TEXTURE0, "tree1a.png", true);
    texture_.Bind(GL_TEXTURE1, "tree2a.png", true);
    texture_.Bind(GL_TEXTURE2, "tree3a.png", true); 
    texture_.Bind(GL_TEXTURE3, "tree4a.png", true);
    texture_.Bind(GL_TEXTURE4, "tree5a.png", true);
    texture_.Bind(GL_TEXTURE5, "tree6a.png", true);

    UpdateModel(glm::vec3(0.f, 0.18f, 0.f));

    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);

    opengl_util_.ReStoreState();
    CHECK_GL_ERROR(" Trees::Draw_() Error \n");
  }

}//namespace cs559
