#version 120
varying vec3 v_normal;
varying vec2 v_texture;
varying float v_diffuse;
uniform sampler2D tex0;

void main() {
  gl_FragColor = texture2D(tex0, v_texture)*v_diffuse;
}
