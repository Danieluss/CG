#version 450

uniform mat4 lightMatrix;
uniform mat4 M;

in vec4 iPos;

void main(void) {
    gl_Position = lightMatrix*M*iPos;
}