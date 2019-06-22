#version 450

uniform mat4 PV;
uniform mat4 M;

in vec3 iPos;

void main(void) {
    gl_Position = PV*M*vec4(iPos, 1.0);
}