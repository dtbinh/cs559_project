#version 120
varying vec3 v_position_eye;
varying vec3 v_normal_eye;
varying vec3 v_light_eye;
varying vec2 v_texture;

varying vec3 v_pos;
uniform vec3 u_clip_dir;
uniform vec3 u_clip_pos;

uniform vec3 u_Ka;
uniform vec3 u_Kd;
uniform vec3 u_Ks;

uniform sampler2D tex0;

void main() {
  float clipval = dot(v_pos - u_clip_pos, u_clip_dir);
  if (clipval<0) {
    discard;
  }

  vec3 v = -normalize(v_position_eye);
  vec3 l = normalize(v_light_eye - v_position_eye);
  vec3 h = normalize(v + l);
  vec3 n = normalize(v_normal_eye);
  
  float spec = clamp(dot(n, h), 0.0, 1.0);
  float diff = clamp(dot(n, l), 0.0, 1.0);
  
  float m = 5.0;
  vec3 light = u_Ka + (u_Kd + u_Ks*pow(spec, m))*diff; //ref: RTR p.144
  
  //vec3 light = u_Ka + u_Kd * diff + u_Ks * pow(spec, m);
  
  gl_FragColor = texture2D(tex0, v_texture)*vec4(light, 0);
  //gl_FragColor = vec4(light, 0);
}
