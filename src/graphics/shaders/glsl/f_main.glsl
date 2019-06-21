#version 450

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

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    sampler2D depthMapTexture;
    mat4 lightMatrix;
};

uniform Material material;
uniform int numberOfDirectionalLights;
uniform DirectionalLight directionalLights[4];
uniform vec3 cameraLocation;

in vec3 pos;
in vec2 texCoord;
in vec3 normal;
in vec4 posForDirectionalLight[4];

out vec4 pixelColor;

float getShadow(DirectionalLight lightSource, vec4 positionInLightCoordinates, vec3 norm, vec3 dir) {
    vec3 pl = positionInLightCoordinates.xyz*0.5 + 0.5;
    float currentDepth = pl.z;
    float eps = max(0.1*(1-dot(norm, dir)), 0.01);
    float shadow = 0.0;
    vec2 dr = vec2(1.0)/textureSize(lightSource.depthMapTexture, 0);
    for(int i=-1; i <= 1; i++) {
        for(int j=-1; j <= 1; j++) {
            float minDepth = texture(lightSource.depthMapTexture, pl.xy + dr*vec2(i,j)).x;
            if(minDepth+eps < currentDepth && currentDepth <= 1.0) {
                shadow+=1.0;
            }
        }
    }
    shadow/=9.0;
    return shadow;
}

vec3 getDirectionalLight(DirectionalLight lightSource, vec3 ka, vec3 kd, vec3 ks, float k, vec3 norm, vec3 viewerVector, vec4 positionInLightCoordinates) {
    vec3 ambient = ka*lightSource.ambient;
    
    vec3 dir = normalize(lightSource.direction);
    float co = max(dot(norm, dir), 0.0);
    vec3 diffuse = kd*co*lightSource.diffuse;

    vec3 h = normalize(dir+viewerVector);
    float dp = max(dot(norm, h), 0.0);
    vec3 specular = ks*pow(dp, k)/3;
    
    float shadow = getShadow(lightSource, positionInLightCoordinates, norm, dir);
    return ambient+(diffuse+specular)*(1.0-shadow);
}

void main(void) {
    vec3 ka, kd, ks;
    float debug=0;
    if(material.vambient.x < 0) {
        ka = texture(material.ambient, texCoord).xyz;
    } else {
        ka = material.vambient;
    }
    if(material.vdiffuse.x < 0) {
        kd = texture(material.diffuse, texCoord).xyz;
    } else {
        kd = material.vdiffuse;
    }
    if(material.vspecular.x < 0) {
        ks = texture(material.specular, texCoord).xyz;
    } else {
        ks = material.vspecular;
    }
    vec3 norm = normalize(normal);
    vec3 viewerVector = normalize(cameraLocation-pos);
    vec3 col = vec3(0,0,0);
    for(int i=0; i < numberOfDirectionalLights; i++) {
        col+=getDirectionalLight(directionalLights[i], ka, kd, ks, material.shininess, norm, viewerVector, posForDirectionalLight[i]);
    }
//    pixelColor=vec4(col, 1.0);
    pixelColor=vec4(texture(material.specular, texCoord).xyz, 1);
}