#version 120
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texture;

varying vec3  v_position;
varying vec3  v_normal;
varying vec2  v_texture;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
  
  v_position = vec3(view * model * vec4(a_position, 0.0));
  v_normal   = vec3(view * model * vec4(a_normal, 0.0));
  v_texture = a_texture;
  v_normal  = a_normal;
  
  gl_Position = proj * view * model * vec4(a_position, 1.0);
}
