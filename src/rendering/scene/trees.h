#ifndef TREES_H
#define TREES_H

#include <stdio.h>
#include <vector>

#include "rendering/basic.h"
#include "rendering/basic/cube.h"
#include "rendering/basic/rectangle.h"
#include "rendering/opengl/framebuffer.h"
#include "mesh/mesh.h"

namespace cs559 {
  
  /** An example to show fbo color and depth usage
   */
  class Trees : public SceneObj {
  public:
    Trees(Camera const& camera) : SceneObj(camera) {
    }

  protected:
    void Init_();
    void Draw_();

  private:
    OpenGL_UTIL opengl_util_;

    void UpdateModel(glm::vec3 const& pos);
    
    Mesh * treemesh_;
    Mesh * terrainmesh_;
    float thld_y_;
    int num_trees_;    
    std::vector<glm::vec4> vtrees_; 
  };//class CubeFbo 

}//namespace cs559

#endif