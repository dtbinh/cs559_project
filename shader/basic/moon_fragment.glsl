#version 120
varying vec3 v_position;
varying vec3 v_normal;
varying vec2 v_texture;

uniform vec3 u_light_pos;

uniform sampler2D tex0;

void main() {
  float distance = length(u_light_pos - v_position);
  vec3  light_vector = normalize(u_light_pos - v_position);
  float diffuse = max(dot(v_normal, light_vector), 0.1);
  diffuse = diffuse * (1.0 / (0.1 + (0.1 * distance * distance)));
  
  gl_FragColor = texture2D(tex0, v_texture)*diffuse;
}
