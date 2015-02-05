#ifndef SKYBOX_H
#define SKYBOX_H

#include "rendering/opengl/opengl_util.h"
#include "rendering/basic.h"

namespace cs559 {
  
  //
  // Cubemap
  //
  class Skybox : public SceneObj {
  public:
    Skybox(Camera& camera) : SceneObj(camera) {
    }

  protected:
    void Init_();
    void Draw_();
    
    void UpdateView_();

  private:
    /** store, restore states
     */
    OpenGL_UTIL glutil_;
  };// end class ObjSimpleRect
  
} // namespace cs559

#endif