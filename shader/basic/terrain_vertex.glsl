#version 120
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texture;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_modelview_it;
uniform vec3 u_light;


varying vec3 v_position_eye;
varying vec3 v_normal_eye;
varying vec3 v_light_eye;
varying vec2 v_texture;

varying vec3 v_pos;

void main() {
  
  v_position_eye = vec3(u_view * u_model * vec4(a_position, 1.0));
  v_normal_eye   = vec3(u_modelview_it * vec4(a_normal, 1.0));
  v_light_eye    = vec3(u_view * vec4(u_light, 1.0));

  v_texture = a_texture;

  v_pos = vec3(u_model * vec4(a_position, 1.0));
  
  gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
}

  //v_normal   = vec3(u_view * u_model * vec4(a_normal, 0.0));
