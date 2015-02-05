#include "rendering/technique/ex_fbo_color.h"
#include "rendering/opengl/shader_util.h"

namespace cs559 {
  
  static char const* vertex_shader_src = GLSL150(
    attribute vec2 a_position;
    attribute vec2 a_tex_coord;
    varying vec2 v_tex_coord;
    void main() {
      v_tex_coord = a_tex_coord;
      gl_Position = vec4(a_position, 0.0, 1.0);
    }
  );
  
  static char const* fragment_shader_src = GLSL150(
    varying vec2 v_tex_coord;
    uniform sampler2D tex0;  
    out vec4 outColor;
    void main() {
      outColor = texture2D(tex0, v_tex_coord);
    }
  );

  void ExFboColor::Init_() {
    Init_(100, 100, 500, 300);
  }


  void ExFboColor::Init_(int x /*left:0*/, int y /*top:0*/, int width, int height) {
    shader_program_ = BuildShaderProgram(vertex_shader_src, fragment_shader_src);
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);
    
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    
    // mesh setup
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    // left bottom is (-1, -1)
    float xleft = 2.f*float(x)/float(viewport[2]) - 1.f;
    float ybottom = 2.f*float(y)/float(viewport[2]) - 1.f;
    float wid = 2.f*float(width)/float(viewport[2]);
    float hei = 2.f*float(height)/float(viewport[3]);
    float xright = xleft + wid;
    float ytop = ybottom + hei;

    // two triangle to fill full screen
    float vertices[] = {
      xleft, ytop,       xleft,  ybottom,    xright, ybottom,
      xright, ybottom,   xright, ytop,       xleft, ytop,
      0.f, 0.f,          0.f, 1.f,           1.f, 1.f,
      1.f, 1.f,          1.f, 0.f,           0.f, 0.f
    };

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // postion
    GLint posAttrib = glGetAttribLocation(shader_program_, "a_position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // texture coordinate
    GLint texture_Attrib  = glGetAttribLocation(shader_program_, "a_tex_coord");
    glEnableVertexAttribArray(texture_Attrib);
    glVertexAttribPointer(texture_Attrib,  2, GL_FLOAT, GL_FALSE, 0, (void*)(12*sizeof(float)));

    // texture
    texture_.Fetch("hello.png", false);
    glUniform1i(glGetUniformLocation(shader_program_, "tex0"), 
      0/* corresponding to GL_TEXTURE0 */);

    // framebuffer
    fbo_.Init(800, 600);

    CHECK_GL_ERROR(" Rectangle::Init() Error \n");
  }//Rectangle::Init()

  
  void ExFboColor::Draw_() {
    // render to FBO's texture
    fbo_.BindBegin(FrameBuffer::FBO_COLOR);
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    texture_.Bind();    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    fbo_.BindEnd();

    // use FBO's texutre to do render    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fbo_.TextureHnd());
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    printf(" draw call: Rectangle \n");
  }//Rectangle::DrawCall()

} // namespace cs559