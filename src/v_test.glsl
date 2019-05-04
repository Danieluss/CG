#version 450

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform mat3 normalMatrix;

in vec4 iPos;
in vec4 iNormal;
in vec2 iTexCoord;

out vec3 pos;
out vec2 texCoord;
out vec3 normal;

void main(void) {
    texCoord = iTexCoord;
    normal = normalMatrix*iNormal.xyz;
    vec4 p = M*iPos;
    pos = p.xyz;
    gl_Position = P*V*p;
}