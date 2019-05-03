#version 450

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

in vec4 vertex;

void main(void) {
    gl_Position = P*V*M*vertex;
}