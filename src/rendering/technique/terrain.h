#ifndef TERRAIN_H
#define TERRAIN_H

#include "rendering/basic.h"
#include "rendering/camera.h"

namespace cs559 {

  class Terrain : public SceneObj {
  public:
    Terrain(Camera const& camera) : SceneObj(camera) {      
      clip_pos_ = glm::vec3(0.f, -999.f, 0.f);
      clip_dir_ = glm::vec3(0.f,    1.f, 0.f);
    }

    void SetClipDirection(glm::vec3 dir, glm::vec3 pos);

  protected:
    void Init_();   
    void Draw_();

  private:
    glm::vec3 clip_dir_;
    glm::vec3 clip_pos_;
  };// end class ObjSimpleRect

}//namespace cs559

#endif
