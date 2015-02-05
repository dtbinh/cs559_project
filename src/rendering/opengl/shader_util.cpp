//
//  shader_util.cpp
//  cs559fltk
//
//  Created by Leon Chu on 11/11/14.
//  Copyright (c) 2014 Leon Chu. All rights reserved.
//

#include "rendering/opengl/shader_util.h"
#include "rendering/opengl/opengl_util.h"

namespace cs559 {
  
  //
  //  Build Shader Program
  //  
  GLuint BuildShaderProgram(const GLchar* vertex_src, const GLchar* fragment_src) {
    if (nullptr == vertex_src || nullptr == fragment_src) {
      return 0;
    }
    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_src, NULL);
    glCompileShader(vertexShader);
    
    // check shader result
    GLint vs_status;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vs_status);
    char vs_buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, vs_buffer);
    if (GL_TRUE != vs_status) {
      printf("%s \n", vs_buffer);
      return 0;
    }
    
    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_src, NULL);
    glCompileShader(fragmentShader);
    
    // check shader result
    GLint fs_status;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fs_status);
    char fs_buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, fs_buffer);
    if (GL_TRUE != fs_status) {
      printf("%s \n", fs_buffer);
      return 0;
    }
    
    GLuint shaderProgram = glCreateProgram();
    assert(shaderProgram);

    if (0 != shaderProgram) {
      glAttachShader(shaderProgram, vertexShader);
      glAttachShader(shaderProgram, fragmentShader);
    }
    return shaderProgram;
  }//end BuildShaderProgram
  
  
  //
  // Setup GL Buffer Objects: Vertex Buffer, Element Buffer
  //  
  void SetupBufferObjs(GLuint& vbo, GLfloat* vbo_data, int vbo_size, GLuint& ebo, GLuint* ebo_data, int ebo_size) {
    // vbo
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vbo_size, vbo_data, GL_STATIC_DRAW);
    
    // ebo
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_size, ebo_data, GL_STATIC_DRAW);
  }
  

  
  //
  //  Load Shader
  //  
  void LoadShader(char const* filename, char*& source, int& length) {
    source = nullptr;
    
    FILE* file = fopen(filename, "r");
    if (nullptr == file) {
      return;
    }
    fseek (file, 0, SEEK_END);
    length = (int)ftell(file);
    source = new char[length+1];
    if (nullptr == source) {
      return;
    }
    
    rewind(file);
    fread(source, length, 1, file);
    
    source[length] = '\0';
    
    return;
  } //end LoadShader()
  
  
  //
  // Build Shader Program From File
  //
  GLuint BuildShaderProgramFromFile(char const* vertex_filename, char const* fragment_filename) {
    char* vertex_src = nullptr;
    int   vertex_len = 0;
    LoadShader(vertex_filename, vertex_src, vertex_len);

    char* fragment_src = nullptr;
    int   fragment_len = 0;
    LoadShader(fragment_filename, fragment_src, fragment_len);

    GLuint shader = BuildShaderProgram(vertex_src, fragment_src);
    
    SAFE_ARRAY_DELETE(vertex_src);
    SAFE_ARRAY_DELETE(fragment_src);
    CHECK_GL_ERROR("BuildShaderProgramFromFile");
    return shader;
  }
  
  
  
  
  
  
  
  
  
  
}//end namespace cs559