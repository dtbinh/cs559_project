#version 120
attribute vec3 a_position;
attribute vec3 a_normal;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_model_it;
uniform mat4 u_modelview_it;
uniform vec3 u_light;

varying vec3 v_position_eye;
varying vec3 v_normal_eye;
varying vec3 v_light_eye;

varying vec3 v_reflect_dir;

vec3 GetEye(in mat4 view) {
  mat4 rot_it = transpose(view);
  vec3 irt = vec3(view*vec4(0,0,0,1));
  return -vec3(rot_it*vec4(irt,0));
}

void main() {
  
  v_position_eye = vec3(u_view * u_model * vec4(a_position, 1.0));
  v_normal_eye   = vec3(u_modelview_it * vec4(a_normal, 1.0));
  v_light_eye    = vec3(u_view * vec4(u_light, 1.0));
  
  // cubemap
  vec3 eye_pos = GetEye(u_view);
  vec3 eye_dir = normalize(a_position - eye_pos);
  vec3 model_n = vec3(u_model_it * vec4(a_normal,1.0));
  v_reflect_dir = reflect(eye_dir, model_n);
  
  gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
}

  //v_normal   = vec3(u_view * u_model * vec4(a_normal, 0.0));
