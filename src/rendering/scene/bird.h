#ifndef BIRD_H
#define BIRD_H
#include <vector>

#include "rendering/opengl/opengl_util.h"
#include "rendering/basic.h"
#include "mesh/mesh.h"

namespace cs559 {
  
  //
  // Bird
  //
  class Bird : public SceneObj {
  public:
    Bird(Camera& camera) : SceneObj(camera) {
    }

  protected:
    void Init_();
    void Draw_();
    
  private:
    /** store, restore states
     */
    OpenGL_UTIL opengl_util_;    
    Mesh* bird_;
    std::vector<glm::vec4> vbird_; 

    void UpdateModel(glm::vec3 const& pos);
  };// end class ObjSimpleRect
  
} // namespace cs559

#endif