#include "texture_mngr.h"
#include <cassert>
#include "rendering/opengl/shader_util.h"
#include "fileio/img_png.h"

namespace cs559 {

  /**
   */
  // singleton object born here
  TextureMngr::VecString TextureMngr::folder_path_;
  TextureMngr::VecTexureBook TextureMngr::texture_book_;

  //
  GLuint TextureMngr::GenBindTexture(char const* filename, bool mipmap, GLint param) {
    int img_wid = 0;
    int img_hei = 0;
    unsigned char * img_data = nullptr;

    // search file through possible folders
    char fname[FILENAME_MAX];
    for (VecString::const_iterator it = folder_path_.begin(), end = folder_path_.end(); it!=end; ++it) {      
      sprintf(fname, "%s/%s", it->c_str(), filename);
      img_data = ImgPng::Decode(fname, img_wid, img_hei, 4 /* RGBA */);
      if (nullptr != img_data) {
        break;
      }
    }

    GLuint textures = 0;
    
    if (NULL != img_data) {
      glGenTextures(1, &textures);
      glBindTexture(GL_TEXTURE_2D, textures);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmap? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_wid, img_hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_data);      
      if (mipmap) {
        /*glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0); 
          glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 7); */
        glGenerateMipmap(GL_TEXTURE_2D);
      }
      delete [] img_data;
    }
    CHECK_GL_ERROR("TextureMngr::GenBindTexture Error");
    return textures;
  }

  /**
   */
  GLuint TextureMngr::GenBindTextureCubemap(char const* fpx, char const* fnx, char const* fpy, char const* fny,
      char const* fpz, char const* fnz, bool mipmap, GLint param){   
    // update file names
    std::string foldername;
    // find folder path
    for (VecString::const_iterator it = folder_path_.begin(), end = folder_path_.end(); it!=end; ++it) {      
      char fname[FILENAME_MAX]; sprintf(fname, "%s/%s", it->c_str(), fpx);
      FILE* file = fopen(fname,"rb");
      if (nullptr != file) {
        foldername = *it; fclose(file);
        break;
      }
    }
    VecString filenames;
    filenames.push_back(foldername+"/"+fpx);
    filenames.push_back(foldername+"/"+fnx);
    filenames.push_back(foldername+"/"+fpy);
    filenames.push_back(foldername+"/"+fny);
    filenames.push_back(foldername+"/"+fpz);
    filenames.push_back(foldername+"/"+fnz);

    // load data
    unsigned char** faceData = new unsigned char*[6];
    int size = 0;    
    for (int i = 0; i < 6; ++i) {      
      faceData[i] = ImgPng::Decode(filenames[i].c_str(), size, size, 4 /* RGBA */);
      assert(nullptr != faceData[i]);
    }
     
    GLenum format = GL_RGBA;
    GLenum type = GL_UNSIGNED_BYTE;
    
    GLuint textureObject;
    glGenTextures(1, &textureObject);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureObject);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, param);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, param);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, param);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mipmap? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);

    for (int f = 0; f < 6; ++f) {
      GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + f;
      glTexImage2D(face, 0, format, size, size, 0, format, type, faceData[f]);
    }
    if (mipmap) {
      glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
    
    // release resource
    for (int i = 0; i < 6; ++i) {
      delete [] faceData[i];
    }
    delete [] faceData;
    
    return textureObject;
  }// TextureMngr::GenBindTextureCubemap

  bool TextureMngr::Fetch(char const* filename, bool mipmap, GLint param) {
    // check if we fetched the file before     
    if (CheckAndSetVecBook(filename)) {
      return true;
    }
    // read file now      
    texture_hnd_ = GenBindTexture(filename, mipmap, param);
    assert(0 != texture_hnd_);

    // update member
    if (0 != texture_hnd_) {
      Texbook texbook;
      texbook.filename = filename;
      texbook.texture_hnd_ = texture_hnd_;
      texbook.texture_target_ = texture_target_ = GL_TEXTURE_2D;
      texture_book_.push_back(texbook);
    }
    return 0 != texture_hnd_;
  };

  bool TextureMngr::FetchCubeMap(char const* fpx, char const* fnx, char const* fpy, char const* fny,
    char const* fpz, char const* fnz, bool mipmap, GLint param) {  
    std::string id = std::string(fpx) + std::string(fnx) + std::string(fpy) + std::string(fny) 
      + std::string(fpz) + std::string(fnz);
    // check if we fetched the file before     
    if (CheckAndSetVecBook(id.c_str())) {
      return true;
    }

    // read file now      
    texture_hnd_ = GenBindTextureCubemap(fpx, fnx, fpy, fny, fpz, fnz, mipmap, param);
    assert(0 != texture_hnd_);

    // update member
    if (0 != texture_hnd_) {
      Texbook texbook;
      texbook.filename = id;
      texbook.texture_hnd_ = texture_hnd_;
      texbook.texture_target_ = texture_target_ = GL_TEXTURE_CUBE_MAP;      
      texture_book_.push_back(texbook);       
    }
    return 0 != texture_hnd_;      
  }// TextureMngr::FetchCubeMap
  
  void TextureMngr::Set(GLenum texture_active, char const* name, bool mipmap, GLint param) {
    texture_active_ = texture_active;
    if (!CheckAndSetVecBook(name)) {
      bool FetchResult = Fetch(name, mipmap, param);
      assert(FetchResult);
    }    
  }// TextureMngr::Set(GLenum texture_active, char const* name)

  void TextureMngr::Set(GLenum texture_active, GLuint texture_hnd, GLenum target) {
    assert(0 != texture_hnd);
    texture_active_ = texture_active;
    texture_target_ = target;
    texture_hnd_ = texture_hnd; //
  }// TextureMngr::Set(GLenum texture_active, GLuint texture_hnd, GLenum target)

  void TextureMngr::Bind() {    
    glActiveTexture(texture_active_);    
    glBindTexture(texture_target_, texture_hnd_);
  }// TextureMngr::Bind()

  void TextureMngr::Bind(GLenum texture_active, char const* name, bool mipmap, GLint param) {
    if (Fetch(name, mipmap, param)) {
      glActiveTexture(texture_active);
      glBindTexture(texture_target_, texture_hnd_);
    } else {
      bool Bind_Texture_Fail = false;
      assert(Bind_Texture_Fail);
    }
  }// Bind(GLenum texture_active, char const* name)

  void TextureMngr::Bind(GLenum texture_active, char const* fpx, char const* fnx, char const* fpy, char const* fny, char const* fpz, char const* fnz, bool mipmap, GLint param) {
    if (FetchCubeMap(fpx, fnx, fpy, fny, fpz, fnz, mipmap, param)) {
      glActiveTexture(texture_active);
      glBindTexture(texture_target_, texture_hnd_);
    } else {
      bool Bind_Texture_Fail = false;
      assert(Bind_Texture_Fail);
    }
  }// TextureMngr::Bind(GLenum texture_active, char const* fpx, char const* fnx, char const* fpy, char const* fny, char const* fpz, char const* fnz)

  void TextureMngr::Bind(GLenum texture_active, GLuint texture_hnd, GLenum target) const {    
    assert(0 != texture_hnd);
    glActiveTexture(texture_active);
    glBindTexture(target, texture_hnd);
  }// Bind(char const* filename)

  bool TextureMngr::CheckAndSetVecBook(char const* id) {
    for (VecTexureBook::const_iterator it = texture_book_.begin(), end = texture_book_.end(); it!= end; ++it) {        
      if (!it->filename.compare(id)) {
        texture_hnd_ = it->texture_hnd_;        
        texture_target_ = it->texture_target_;
        return true;
      }
    }
    return false;
  }// TextureMngr::CheckAndSetVecBook()


  void TextureMngr::AddFolderPath(char const* folder_path) {
    for (VecString::const_iterator it = folder_path_.begin(), end = folder_path_.end(); it!=end; ++it) {
      if (!it->compare(folder_path) /*the ! means the two string is the same*/) {
        return;
      }
    }
    folder_path_.push_back(folder_path);
  }// AddFolderPath()

 }//namespace cs559