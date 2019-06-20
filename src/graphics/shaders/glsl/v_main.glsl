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

in vec3 iPos;
in vec3 iNormal;
in vec2 iTexCoord;

out vec3 pos;
out vec2 texCoord;
out vec3 normal;
out vec4 posForDirectionalLight[4];

void main(void) {
    texCoord = iTexCoord;
    normal = normalMatrix*iNormal;
    vec4 p = M*vec4(iPos.xyz, 1.0);
    pos = p.xyz;
    gl_Position = P*V*p;
    for(int i=0; i < numberOfDirectionalLights; i++) {
        posForDirectionalLight[i] = directionalLights[i].lightMatrix*p;
    }
}