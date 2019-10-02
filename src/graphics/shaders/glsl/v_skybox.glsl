#version 450

in vec3 iPos;

out vec3 texCoord;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

void main() {
    texCoord = iPos;
    vec4 pos = P * V * M * vec4(iPos, 1.0);
    gl_Position = pos.xyww;
}