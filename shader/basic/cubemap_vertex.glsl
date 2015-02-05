#version 120
attribute vec3 a_position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

varying vec3 v_cube_tex;

void main() {
  // cubemap
  v_cube_tex = normalize(a_position);
  gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
}

  //v_normal   = vec3(u_view * u_model * vec4(a_normal, 0.0));
