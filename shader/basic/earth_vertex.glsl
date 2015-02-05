#version 120
attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texture;

varying vec3  v_normal;
varying vec2  v_texture;
varying float v_diffuse;

uniform vec3 u_light_pos;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
  
  vec3 model_view_vertex = vec3(view * model * vec4(a_position, 0.0));
  vec3 model_view_normal = vec3(view * model * vec4(a_normal, 0.0));
  float distance = length(u_light_pos - model_view_vertex);
  
  vec3 light_vector = normalize(u_light_pos - model_view_vertex);
  float diffuse = max(dot(model_view_normal, light_vector), 0.6);
  
  v_diffuse = diffuse * (1.0 / (1.0 + (0.2 * distance * distance)));
  
  v_texture = a_texture;
  v_normal = a_normal;
  
  gl_Position = proj * view * model * vec4(a_position, 1.0);
}
