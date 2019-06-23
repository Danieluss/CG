#version 450
in vec2 pos;
in vec2 iTexCoord;

out vec2 texCoord;

uniform mat4 P;
uniform vec2 offset;
uniform float scale;

void main()
{
    texCoord = iTexCoord;
    gl_Position = P * vec4((pos * scale) + offset, 0.0, 1.0);
}