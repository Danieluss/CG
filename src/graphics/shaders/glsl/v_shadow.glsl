#version 450

uniform mat4 lightMatrix;
uniform mat4 M;

in vec3 iPos;

void main(void) {
    gl_Position = lightMatrix*M*vec4(iPos, 1.0);
}