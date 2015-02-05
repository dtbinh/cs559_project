#version 120
varying vec3 v_cube_tex;
uniform samplerCube tex0;

void main() {
    gl_FragColor = textureCube(tex0, v_cube_tex);
}
