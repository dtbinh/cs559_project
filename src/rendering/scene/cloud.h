#ifndef CLOUD_H
#define CLOUD_H

#include <vector>

#include "rendering/opengl/opengl_util.h"
#include "rendering/basic.h"
#include "mesh/mesh.h"

namespace cs559 {
  
  //
  // Cubemap
  //
  class Cloud : public SceneObj {
  public:
    Cloud(Camera& camera) : SceneObj(camera) {
    }

  protected:
    void Init_();
    void Draw_();
    
  private:
    /** store, restore states
     */
    OpenGL_UTIL opengl_util_;    
    Mesh* cloud_;
    std::vector<glm::vec4> vcloud_; 

    void UpdateModel(glm::vec3 const& pos);
  };// end class ObjSimpleRect
  
} // namespace cs559

#endif