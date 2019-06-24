#version 450

out vec4 pixelColor;

void main(void) {
    float z = gl_FragCoord.z;
    pixelColor = vec4(z,z,z,1);
}