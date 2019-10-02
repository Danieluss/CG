#version 450
in vec2 pos;
in vec2 iTexCoord;

out vec2 texCoord;
out vec2 oPos;

uniform mat4 P;
uniform mat4 VM;

void main() {
    texCoord = iTexCoord;
    vec4 tmp = P * VM * vec4(pos, 0.0, 1.0);
    gl_Position = tmp;
    oPos = tmp.xy;
}