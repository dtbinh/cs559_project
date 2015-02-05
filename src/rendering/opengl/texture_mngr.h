#ifndef TEXTURE_MNGR_H
#define TEXTURE_MNGR_H

#include <vector>
#include <string>
#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>

#include "cs559core.h"
#include "misc/util.h"
#include "rendering/opengl/framebuffer.h"

namespace cs559 {
  
  class TextureMngr {
  public:
    TextureMngr() {
      texture_target_ = GL_TEXTURE_2D;
      texture_hnd_ = 0;
      texture_active_ = GL_TEXTURE0;
    }//TextureMngr()

    /** gen, bind and set parameter
     * param: GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, or GL_REPEAT
     */
    bool Fetch(char const* filename, bool mipmap, GLint param = GL_CLAMP_TO_EDGE);

    /** gen, bind and set parameter
     * param: GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, or GL_REPEAT
     */
    bool FetchCubeMap(char const* fpx, 
      char const* fnx, 
      char const* fpy, 
      char const* fny, 
      char const* fpz, 
      char const* fnz, 
      bool mipmap, 
      GLint param = GL_CLAMP_TO_EDGE);
    
    /** Set and Bind is for the user outside, using two step single texture binding
     * set texture_active_, texture_target_, texture_hnd_ to bind
     */
    void Set(GLenum texture_active, char const* name, bool mipmap, GLint param = GL_CLAMP_TO_EDGE);       
    void Set(GLenum texture_active, GLuint texture_hnd, GLenum target);
    void Bind();

    /** fetch the texture if not found by name
     * texture_active: GL_TEXTURE0 + i
     * target: GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
     * param: GL_CLAMP_TO_EDGE, GL_MIRRORED_REPEAT, or GL_REPEAT
     */
    void Bind(GLenum texture_active, char const* name, bool mipmap, GLint param = GL_CLAMP_TO_EDGE);
    
    /** fetch the texture if not found by name
     * texture_active: GL_TEXTURE0 + i
     * target: GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
     */
    void Bind(GLenum texture_active, GLuint texture_hnd, GLenum target) const;
    
    /** bind cubemap, fetch the texture if not found by name
     * texture_active: GL_TEXTURE0 + i
     * target: GL_TEXTURE_2D or GL_TEXTURE_CUBE_MAP
     */
    void Bind(GLenum texture_active, 
      char const* fpx, 
      char const* fnx, 
      char const* fpy, 
      char const* fny, 
      char const* fpz, 
      char const* fnz, 
      bool mipmap, 
      GLint param = GL_CLAMP_TO_EDGE);

    static void AddFolderPath(char const* folder_path);

  private:
    bool CheckAndSetVecBook(char const* filename /*id*/);
    GLuint GenBindTexture(char const* filename, bool mipmap, GLint param);
    GLuint GenBindTextureCubemap(char const* fpx, char const* fnx, char const* fpy, char const* fny,
      char const* fpz, char const* fnz, bool mipmap, GLint param);

    struct Texbook {
      std::string filename;
      GLuint texture_hnd_;
      GLenum texture_target_;
      Texbook (): 
        filename(), 
        texture_hnd_(0), 
        texture_target_(GL_TEXTURE_2D) {
      }
    };
    typedef std::vector<Texbook> VecTexureBook;
    typedef std::vector<std::string> VecString;

    // singleton
    static VecString folder_path_;
    static VecTexureBook texture_book_;
    GLenum texture_target_; // GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBEMAP
    GLuint texture_hnd_; // glGenTextures and glBindTexture
    GLenum texture_active_; // GL_TEXTURE0, ...
  };//class TextureMngr 

}//namespace cs559

#endif