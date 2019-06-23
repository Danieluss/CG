#version 450
in vec2 pos;
in vec2 iTexCoord;

out vec2 texCoord;

uniform mat4 P;
uniform mat4 VM;

void main()
{
    texCoord = iTexCoord;
    gl_Position = P * VM * vec4(pos, 0.0, 1.0);
}