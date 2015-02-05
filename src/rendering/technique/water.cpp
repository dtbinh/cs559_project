#include "rendering/technique/water.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "rendering/opengl/opengl_util.h"
#include "mesh/gen_primitive.h"

namespace cs559 {

  /** Vertex Shader
   */
  static char const* water_vertex_shader_src = GLSL120(
    attribute vec3  a_position;
    attribute vec3  a_normal;
    attribute vec2  a_tex_coord;
    uniform   mat4  u_model;
    uniform   mat4  u_view;
    uniform   mat4  u_proj;
    varying   vec2  v_tex_coord;
    varying   vec4  v_pos;
    varying   vec4  v_model_pos;

    void main() {      
      v_tex_coord = a_tex_coord*vec2(35.f, 35.f);    
      v_model_pos = u_model*vec4(a_position, 1.0);
      v_pos = u_proj*u_view*v_model_pos;
      gl_Position = v_pos;
    }
  );
  

  /** Fragment Shader
   */
  static char const* water_fragment_shader_src = GLSL120(
    uniform      vec3  u_light_pos;
    uniform      vec3  u_eye_pos;
    uniform sampler2D  u_tex_reflection;
    uniform sampler2D  u_tex_refraction;
    uniform sampler2D  u_tex_normal;
    uniform sampler2D  u_tex_dudv;
    uniform     float  u_time;
    varying      vec2  v_tex_coord;
    varying      vec4  v_pos;
    varying      vec4  v_model_pos;
    
    void main() {
      float kShine = 128.0;
      float kDistortDuDv = 0.01;
      float kReflection = 0.01;

      vec4 dudv_time = texture2D(u_tex_dudv, v_tex_coord + vec2(u_time))*kDistortDuDv;
       
      //// Fresenl
      vec4 normal4 = texture2D(u_tex_normal, vec2(v_tex_coord + dudv_time.xy));
      normal4 = normal4*2.0 - 1.0;
      vec3 normal = normalize(vec3(normal4));
      
      vec3 light_reflect = u_light_pos - 2.0 * dot(normal, u_light_pos) * normal;
      light_reflect = normalize(light_reflect);
      
      vec4 inv_fresnel = clamp(vec4(dot(vec3(normal), light_reflect)), 0.5, 1.0);
      vec4 fresnel = 1.f - inv_fresnel;
      vec3 p_to_eye = normalize(u_eye_pos - vec3(v_model_pos));

      // projective coordinate
      vec4 dudv_sft = texture2D(u_tex_dudv, vec2(v_tex_coord + dudv_time.xy));
      dudv_sft = normalize(dudv_sft*2.0 - 1.0) * kReflection;      
      vec4 tmp = vec4(1.0/v_pos.w);
      vec4 proj_st = clamp(0.5*v_pos*tmp + 0.5 + dudv_sft, 0.0, 1.0);            
      
      vec4 refract_color = texture2D(u_tex_refraction, proj_st.xy);      
      vec4 reflect_color = texture2D(u_tex_reflection, proj_st.xy);

      // specular      
      float intensity = max(0.0, dot(light_reflect, p_to_eye) );
      vec4 specular = vec4(pow(intensity, kShine));

      gl_FragColor = 0.6*reflect_color*fresnel + 0.9*refract_color*inv_fresnel + specular;
    }
  );



  /** init
   * 0. Material:
   *    a. normal map
   *    b. du/dv map (could be generated from normal)
   * 1. Render 3 textures
   *    a. reflection
   *    b. refracton (1.a + du/dv)
   *    c. depth (option)
   * 2. final
   *    a. refracton + bumpmap (normal map + light position)
   */
  void Water::Init_() {
    // init reflected objects
    for (auto obj : reflected_objs_) {
      obj->Init();
    }
    rect_.Init();
    rect2_.Init();
    rect_.UpdateRegion(0, 0, 0.3, 0.225);
    rect2_.UpdateRegion(0.31, 0, 0.61, 0.225);

    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // generate xz plane
    Mesh* plane = mesh::PlaneXZ332(99);
    num_element_ = plane->num_indx;
    
    // setup shader program
    shader_program_ = BuildShaderProgram(water_vertex_shader_src, water_fragment_shader_src);
    
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, plane->vtx_data, plane->vtx_size);
    BindVbo332f(vbo_, "a_position", nullptr, "a_tex_coord");
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, plane->idx_data, plane->idx_size);
    BindEbo(ebo_);
    // don't forget to unbind!
    glBindVertexArray(0);

    //// Model View
    shader_mvp_.SetShaderProgram(shader_program_);
    // link shader program
    GLint u_model = glGetUniformLocation(shader_program_, "u_model");

    shader_mvp_.U_model("u_model");    
    shader_mvp_.U_view("u_view");   
    shader_mvp_.U_porj("u_proj");

    SetModel(glm::mat4(1));
    SetProjection(camera_.Projection());
    SetView(camera_.View());

    //// Textures
    // normal map
    bool tex1rlt = texture_mngr_.Fetch("water_normal.png", true, GL_REPEAT);
    assert(tex1rlt);
    // du dv map
    bool tex2rlt = texture_mngr_.Fetch("water_dudv2.png", true, GL_REPEAT);
    assert(tex2rlt);
    // refraction
    bool tex3rlt = texture_mngr_.Fetch("refraction_map3.png", true, GL_REPEAT);
    assert(tex3rlt);

    // reflection will use FBO's color texture
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_reflection"), 0 /* corresponding GL_TEXTURE0 */);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_normal"),     1 /* corresponding GL_TEXTURE1 */);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_dudv"),       2 /* corresponding GL_TEXTURE2 */);
    glUniform1i(glGetUniformLocation(shader_program_, "u_tex_refraction"), 3 /* corresponding GL_TEXTURE3 */); 
    
    // view, light and time u_time
    //TOTO: update
    glUniform3f(glGetUniformLocation(shader_program_, "u_eye_pos"),  0.0f,  1.0f, 2.0f);
    glUniform3f(glGetUniformLocation(shader_program_, "u_light_pos"), 0.1f, 1.0f, 0.1f);
    glUniform1f(glGetUniformLocation(shader_program_, "u_time"), 0.1f);

    // fbo
    int wid = 0, hei = 0;
    OpenGL_UTIL::GetViewportSize(wid, hei);
    fbo_.Init(wid, hei);
    fbo_depth_.Init(wid, hei);
    CHECK_GL_ERROR(" Water::Init_() Error \n");
  } // Water::Init_()


  /** Draw stencil to little screen
   */
  void Water::DebugStencil_() {
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
   
    fbo_.BindBegin(FrameBuffer::FBO_COLOR); { 
      OpenGL_UTIL::SetBGColor(1.f, 0.f, 0.f);
      stencil_.DrawBeg(GL_NEVER, 0x01, 0xFF, GL_REPLACE, GL_REPLACE, GL_REPLACE); {      
        glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);
      }
      stencil_.DrawEnd();
      stencil_.UseBeg(GL_EQUAL, 0x01, 0xFF); {      
        glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);      
      }
      stencil_.UseEnd();
    }
    fbo_.BindEnd();
    
    rect_.SetTexture(GL_TEXTURE0, fbo_.TextureHnd(), GL_TEXTURE_2D);
    rect_.UpdateRegion(0.0f, 0.0f, 0.2f, 0.15f);
    rect_.Draw();
  } // Water::DebugStencil_()

  

  /**
   * 0. Material:
   *    a. normal map
   *    b. du/dv map (could be generated from normal)
   * 1. Render 3 textures
   *    a. reflection
   *    b. refracton (1.a + du/dv)
   *    c. depth (option)
   * 2. final
   *    a. refracton + bumpmap (normal map + light position)
   */
  void Water::Draw_() {
    //// draw reflection
    // draw to FBO for reflection texture
    fbo_.BindBegin(FrameBuffer::FBO_COLOR); 
    {             
      for (auto obj : reflected_objs_) {
        obj->MulTransform(OpenGL_UTIL::GetMirrorY());
        obj->Draw();
        obj->MulTransform(OpenGL_UTIL::GetMirrorY());
      }
    }
    fbo_.BindEnd();

    if (fbo1_show) {
      fbo_depth_.BindBegin(FrameBuffer::FBO_DEPTH);
      {
        for (auto obj : reflected_objs_) {
          obj->MulTransform(OpenGL_UTIL::GetMirrorY());
          obj->Draw();
          obj->MulTransform(OpenGL_UTIL::GetMirrorY());
        }
      }
      fbo_depth_.BindEnd();
      rect2_.SetTexture(GL_TEXTURE0, fbo_depth_.TextureHnd(), GL_TEXTURE_2D);      
      rect2_.UpdateRegion(0.41f, 0.0f, 0.81f, 0.3f);
      rect2_.Draw();
    }
    // debug reflection
    if (fbo2_show) {
      rect_.SetTexture(GL_TEXTURE0, fbo_.TextureHnd(), GL_TEXTURE_2D);
      rect_.UpdateRegion(0.0f, 0.0f, 0.4f, 0.3f);
      rect_.Draw();
    }

    

    // now we have four textures: reflection, refraction, normal, dudv
    // ready to draw ocean
    SetupProgramVVP(shader_program_);
    
    UpdateVPT_(0.01f*camera_.Time(), camera_.Eye(), glm::vec3(0.5, 1.0, -1.0));

    texture_.Bind(GL_TEXTURE0, fbo_.TextureHnd(), GL_TEXTURE_2D);
    texture_.Bind(GL_TEXTURE1, "water_normal.png", true);
    texture_.Bind(GL_TEXTURE2, "water_dudv2.png", true);
    texture_.Bind(GL_TEXTURE3, "refraction_map3.png", true);

    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);

    CHECK_GL_ERROR(" Water::Draw_() Error \n");
  }

  /** Bind textures
    */
  void Water::UpdateVPT_(float time, glm::vec3 const& eye, glm::vec3 const& light) {
    // update time
    glUniform1f(glGetUniformLocation(shader_program_, "u_time"), time);
    glUniform3f(glGetUniformLocation(shader_program_, "u_eye_pos"),  eye.x,  eye.y, eye.z);
    glUniform3f(glGetUniformLocation(shader_program_, "u_light_pos"), light.x, light.y, light.z);
  }

}// namespace cs559
