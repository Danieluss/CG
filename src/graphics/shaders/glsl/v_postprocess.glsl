#version 120
in vec2 pos;
in vec2 iTexCoord;

out vec2 texCoord;

void main() {
    gl_Position = vec4( pos, 0.0, 1.0 );
    texCoord = iTexCoord;

}
