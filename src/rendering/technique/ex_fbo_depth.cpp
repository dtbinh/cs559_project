#include "rendering/technique/ex_fbo_depth.h"

#include "mesh/gen_primitive.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

  // Cube
  static char const* vertex_cube_src = GLSL120(
    attribute vec3 a_position;
    uniform mat4 u_model;
    uniform mat4 u_view;
    uniform mat4 u_proj;
    void main() {
      gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);     
    } 
  );
  
  static char const* fragment_cube_src = GLSL120(
    void main() {
      gl_FragColor = vec4(1,1,1,1);
    }
  );

  // Overlay
  static char const* vertex_shader_src = GLSL120(
    attribute vec2 a_position;
    attribute vec2 a_texcoord;
    varying vec2 v_texcoord;
    void main() {
      v_texcoord = a_texcoord;
      gl_Position = vec4(a_position, -1, 1.0);
    }
  );
  
  static char const* fragment_shader_src = GLSL120(
    uniform sampler2D tex0;
    varying vec2 v_texcoord;
    void main() {
      gl_FragColor = texture2D( tex0, v_texcoord );
    }
  );

namespace cs559 {
  /**
   */
  void ExFboDepth::Init_() {
    // CUBE
    // vao, use Apple version
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // generate sphere
    float r = 2.f;    
    Mesh* mesh = mesh::GenCube332(r);
    num_element_ = mesh->num_indx;

    // setup shader program
    shader_program_ = BuildShaderProgram(vertex_cube_src, fragment_cube_src);    
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);

    // setup vertex
    vbo_ = LoadBuffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->vtx_data, mesh->vtx_size);    
    BindVbo332f(vbo_, "a_position", nullptr, nullptr);
    ebo_ = LoadBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, mesh->idx_data, mesh->idx_size);
    BindEbo(ebo_);
    
    //// Model View Projection
    shader_mvp_.SetShaderProgram(shader_program_);
    shader_mvp_.U_model("u_model");    
    shader_mvp_.U_view("u_view");
    shader_mvp_.U_porj("u_proj");
    view_ = glm::lookAt(glm::vec3(3.0f, 5.0f,  0.1f), /* YOU SHOULD BE VERY VERY CLOSE TO THE OBJECT !!!!!!*/
                        glm::vec3(0.0f, 0.0f,  0.0f),
                        glm::vec3(0.0f, 1.0f,  0.0f) );
    shader_mvp_.SetModelView(model_, view_);
    shader_mvp_.SetProjection(glm::perspective(45.0f, 800.0f / 600.0f, 2.f,  10.f));
    glBindVertexArray(0);

    //============================================================================
    // Overlay 
    overlay_shp_ = BuildShaderProgram(vertex_shader_src, fragment_shader_src);
    glLinkProgram(overlay_shp_);
    glUseProgram(overlay_shp_);
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

     // two triangle to fill full screen
    float val = 0.3f;
    float sft = 0.5f;
    float vertices[] = {
      -val+sft, -val+sft, 0, 1,
      -val+sft,  val+sft, 0, 0,      
       val+sft,  val+sft, 1, 0,
       val+sft, -val+sft, 1, 1,
      -val+sft, -val+sft, 0, 1, 
       val+sft,  val+sft, 1, 0
    };
    int stride = 4*sizeof(GLfloat);
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(overlay_shp_, "a_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, stride, 0);                          

    GLint texAttrib = glGetAttribLocation(overlay_shp_, "a_texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2*sizeof(GLfloat))); 
    
    CHECK_GL_ERROR(" Error \n");
    
    glUniform1i(glGetUniformLocation(overlay_shp_, "tex0"), 0 /* corresponding GL_TEXTURE0 */);

    glBindVertexArray(0);
    CHECK_GL_ERROR(" Fillscreen Error \n");

    //============================================================================
    // FBO
    glGenTextures(1, &depth_tex);
    glBindTexture(GL_TEXTURE_2D, depth_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //NULL means reserve texture memory, but texels are undefined
    //You can also try GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT24 for the internal format.
    //If GL_DEPTH24_STENCIL8_EXT, go ahead and use it (GL_EXT_packed_depth_stencil)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    //-------------------------
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    //Attach
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_tex, 0);
    //-------------------------
    GLenum status;
    status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    assert( status == GL_FRAMEBUFFER_COMPLETE);
    CHECK_GL_ERROR(" - Error \n"); 
  }

  /**
   */
  void ExFboDepth::Draw_() {   
    // draw cube to texture
    glBindFramebuffer(GL_FRAMEBUFFER, fb);    
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);      
    glDrawElements(GL_TRIANGLES, num_element_, GL_UNSIGNED_INT, 0);

    // draw to overlay
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(overlay_shp_);
    glBindVertexArray(vao2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depth_tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    CHECK_GL_ERROR(" - FboDepth::DrawCall \n"); 
  }

}//namespace cs559