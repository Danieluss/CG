#version 450

out vec4 pixelColor;

struct Material {
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    vec3 vambient;
    vec3 vdiffuse;
    vec3 vspecular;
    vec3 vnormal;
    float shininess;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
};

uniform Material material;

in vec2 texCoord;

void main(void) {
    pixelColor = texture(material.diffuse, texCoord);
}