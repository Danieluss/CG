#version 450

in vec2 texCoord;
out vec4 pixelColor;

uniform float alpha;
uniform sampler2D sprite;

void main()
{
    vec4 tmp = texture(sprite, texCoord);
    pixelColor = (tmp.xyz, tmp.w * alpha);
}
