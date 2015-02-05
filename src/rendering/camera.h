#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "cs559core.h"

namespace cs559 {
  
  //
  // projection
  //
  class Camera {
  public:
    Camera();

    void KeyIn(char c);
    void MouseHoldMove(int hold_button, int dx, int dy);

    void SetEye(glm::vec3 eye) { eye_pos_ = eye;
    }
    void SetView(glm::mat4 view) { view_ = view;
    }
    void SetProjection(glm::mat4 projection) { projection_ = projection;
    }

    glm::vec3 Eye() const {
      return eye_pos_;
    }

    glm::vec3 EyeDir() const {
      return front_vec_;
    }

    glm::mat4 View() const {      
      return view_;
    }

    glm::mat4 Projection() const {      
      return projection_;
    }

    float Time() const {
      return time_;
    }

    void UpdateTime() {
      time_ += 1.f/fps_;
    }

    void SetFPS(float fps) {
      fps_ = fps;
    }

  private:
    // view
    glm::vec3 eye_pos_ /*position of the eye*/;
    glm::vec3 front_vec_ /*vector of the front direction, center = eye + front*/;
    glm::vec3 up_vec_ /*vector of the up direction*/;
    glm::mat4 view_;

    // project
    float fov_;
    float far_, near_;
    float width_, height_;    
    glm::mat4 projection_;

    float time_;
    float fps_;

    // keyboard positoin control
    float speed_x_, speed_y_, speed_z_;

    void UpdateView();
    void UpdateProjection();
  };//class Camera 

}//namespace cs559

#endif