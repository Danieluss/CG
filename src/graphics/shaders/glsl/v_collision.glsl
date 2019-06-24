#version 450

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

in vec3 iPos;

void main(void) {
    gl_Position = P*V*M*vec4(iPos.xyz, 1.0);
}