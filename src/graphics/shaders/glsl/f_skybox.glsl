#version 450

out vec4 pixelColor;

in vec3 texCoord;

uniform samplerCube skybox;

void main() {
    pixelColor = texture(skybox, texCoord);
}