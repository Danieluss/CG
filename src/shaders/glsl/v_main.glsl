#version 450

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

uniform mat3 normalMatrix;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D depthMapTexture;
    mat4 lightMatrix;
};
uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[4];

in vec4 iPos;
in vec4 iNormal;
in vec2 iTexCoord;

out vec3 pos;
out vec2 texCoord;
out vec3 normal;
out vec4 posForDirectionalLight[4];

void main(void) {
    texCoord = iTexCoord;
    normal = normalMatrix*iNormal.xyz;
    vec4 p = M*iPos;
    pos = p.xyz;
    gl_Position = P*V*p;
    for(int i=0; i < numberOfDirectionalLights; i++) {
        posForDirectionalLight[i] = directionalLights[i].lightMatrix*p;
    }
}