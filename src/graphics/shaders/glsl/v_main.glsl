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
in vec3 iTangent;
in vec2 iTexCoord;

out vec3 pos;
out vec2 texCoord;
out vec3 normal;
out vec3 tangent;
out vec3 bitangent;
out vec4 posForDirectionalLight[4];

void main(void) {
    texCoord = iTexCoord;
    vec3 iBitangent = cross(iNormal, iTangent);
    normal = normalize(normalMatrix*iNormal);
    tangent = normalize(vec3(M*vec4(iTangent, 0.0)));
    bitangent = normalize(vec3(M*vec4(iBitangent, 0.0)));
    vec4 p = M*vec4(iPos.xyz, 1.0);
    pos = p.xyz;
    gl_Position = P*V*p;
    for(int i=0; i < numberOfDirectionalLights; i++) {
        posForDirectionalLight[i] = directionalLights[i].lightMatrix*p;
    }
}