#include "rendering/camera.h"
#include <cstdio>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
//#include <glm/gtx/quaternion.hpp>

namespace cs559 {

  /** 
   */
  Camera::Camera() {
    fov_ = 60.f/180.f*3.1415926526f;
    width_ = 800.f;
    height_ = 600.f;
    near_ = 0.1f;
    far_ = 70.f;
    speed_x_ = speed_y_ = speed_z_ = 0.1f;
    eye_pos_   = glm::vec3(0, 1, 10);
    front_vec_ = glm::vec3(0, 0, -1);
    up_vec_    = glm::vec3(0, 1,  0);
    time_ = 0.f;
    fps_ = 30.f;
    UpdateView();
    UpdateProjection();
  }// Camera::Camera()

  /** 
   */
  void Camera::UpdateView() {
    view_ = glm::lookAt(eye_pos_,
                        eye_pos_ + front_vec_,
                        up_vec_ );
  }// Camera::UpdateView()

  /** 
   */
  void Camera::UpdateProjection() {
    projection_ = glm::perspective(fov_, width_ / height_, near_, far_);
  }

  /** 
   */
  void Camera::MouseHoldMove(int hold_button, int dx, int dy) {
    if (0 == hold_button) {
      return;
    }

    if (3 == hold_button) {
      // update eye position
      // dx : rotate around center
      float rot_deg = -0.01f*dx;
      eye_pos_ = glm::rotate(eye_pos_, rot_deg, glm::vec3(0,1,0));
      front_vec_ = glm::rotate(front_vec_, rot_deg, glm::vec3(0,1,0));
      up_vec_ = glm::rotate(up_vec_, rot_deg, glm::vec3(0,1,0));

      // dy : zoom in/out
      glm::vec3 tt = front_vec_;      
      tt.y = 0;
      eye_pos_ -= speed_z_*dy*tt;  
      
    } else {
      // calcuate (dx, dy) vector in up_vec (as (0,1)) space 
      float delta = (float)(atan2(dy, -dx) - 0.5f*3.1415926f);
      glm::vec3 delta_vec = glm::rotate(up_vec_, delta, -front_vec_);
      glm::vec3 normal = glm::cross(front_vec_, delta_vec);

      // update front and top
      float theta = fov_* (float) sqrt( (dx)*(dx) + (dy)*(dy) )/height_;    
      front_vec_ = glm::rotate(front_vec_, theta, normal);
      up_vec_ = glm::rotate(up_vec_, theta, normal);

      // update up_vec to make sky in the top
      front_vec_ = glm::normalize(front_vec_);
      glm::vec3 front_xz = glm::cross(front_vec_, glm::vec3(0,1,0));
      up_vec_ = up_vec_ - glm::dot(up_vec_, front_xz)*front_xz;
      up_vec_ = glm::normalize(up_vec_);
    }
    UpdateView();
  }// Camera::MouseHoldMove()


  /**
   */
  void Camera::KeyIn(char c) {    
    switch(c) {
    case 'j': 
      eye_pos_.y += speed_y_;      
      if (5.f < eye_pos_.y || 0.1f > eye_pos_.y) {
        eye_pos_.y -= speed_y_;
      }
      break;
    case 'k':      
      eye_pos_.y -= speed_y_;
      if (5.f < eye_pos_.y || 0.1f > eye_pos_.y) {
        eye_pos_.y += speed_y_;
      }
      break;
    case 'e':
      eye_pos_ += speed_z_*front_vec_;
      break;
    case 's': eye_pos_.x -= speed_x_;
      break;
    case 'd':
      eye_pos_ -= speed_z_*front_vec_;
      break;
    case 'f': eye_pos_.x += speed_x_;
      break;
    }
    UpdateView();
  }// Camera::KeyIn()

}// namespace cs559 