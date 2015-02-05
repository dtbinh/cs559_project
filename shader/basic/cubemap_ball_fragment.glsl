#version 120
varying vec3 v_position_eye;
varying vec3 v_normal_eye;
varying vec3 v_light_eye;

varying vec3 v_reflect_dir;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;

uniform samplerCube tex0;

void main() {
  vec3 v = -normalize(v_position_eye);
  vec3 l = normalize(v_light_eye - v_position_eye);
  vec3 h = normalize(v + l);
  vec3 n = normalize(v_normal_eye);
  
  float spec = clamp(dot(n, h), 0.0, 1.0);
  float diff = clamp(dot(n, l), 0.0, 1.0);
  
  float m = 5.0;
  vec3 light = u_Ka + (u_Kd + u_Ks*pow(spec, m))*diff; //ref: RTR p.144
  
  //gl_FragColor = texture2D(tex0, v_texture)*vec4(light, 0);
  gl_FragColor = textureCube(tex0, v_reflect_dir)*vec4(light, 0);
}
