#ifndef WATER_H
#define WATER_H

#include <vector>

#include "rendering/basic.h"
#include "rendering/camera.h"
#include "rendering/opengl/texture_mngr.h"
#include "rendering/opengl/framebuffer.h"
#include "rendering/opengl/stencil.h"
#include "rendering/basic/rectangle.h"

namespace cs559 {

 class Water : public SceneObj {
  public:
    Water(Camera const& camera) : SceneObj(camera) {
      fbo1_show = false;
      fbo2_show = false;
    }
   
    /** Draw reflection objects
     */
    void AddReflectedObj(BasicObj* refected_objs) {
      reflected_objs_.push_back(refected_objs);
    };

    bool fbo1_show;
    bool fbo2_show;

 protected:
    void Init_();    
    void Draw_();

 private:
   TextureMngr texture_mngr_;      
   FrameBuffer fbo_;
   FrameBuffer fbo_depth_;
   std::vector<BasicObj*> reflected_objs_;

   // debug tool
   Rectangle rect_;
   Rectangle rect2_;

   // draw reflection
   Stencil stencil_;

   // Bind textures
   void UpdateVPT_(float time, glm::vec3 const& view, glm::vec3 const& light);

   void DebugStencil_();
  };// end class ObjSimpleRect
 
}//namespace cs559

#endif