#version 450

struct Material {
    sampler2D ambient;
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[4];
uniform vec3 cameraLocation;

in vec3 pos;
in vec2 texCoord;
in vec3 normal;

out vec4 pixelColor;

vec3 getDirectionalLight(DirectionalLight lightSource, vec3 ka, vec3 kd, vec3 ks, float k, vec3 norm, vec3 viewerVector) {
    vec3 ambient = ka*lightSource.ambient;
    
    vec3 dir = normalize(lightSource.direction);
    float co = max(dot(norm, dir), 0.0);
    vec3 diffuse = kd*co*lightSource.diffuse;

    vec3 h = normalize(dir+viewerVector);
    float dp = max(dot(norm, h), 0.0);
    vec3 specular = ks*pow(dp, k)/3;
    
    return ambient+diffuse+specular;
}

void main(void) {
    vec3 ka=texture(material.ambient, texCoord).xyz;
    vec3 kd=texture(material.diffuse, texCoord).xyz;
    vec3 norm = normalize(normal);
    vec3 viewerVector = normalize(cameraLocation-pos);
    vec3 col = vec3(0,0,0);
    for(int i=0; i < numberOfDirectionalLights; i++) {
        col+=getDirectionalLight(directionalLights[i], ka, kd, material.specular, material.shininess, norm, viewerVector);
    }
    pixelColor=vec4(col, 1.0);
}