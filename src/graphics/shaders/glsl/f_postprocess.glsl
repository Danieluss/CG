#version 120

in vec2 texCoord;

out vec4 pixelColor;

uniform sampler2D screenTexture;
uniform bool sinShake;
uniform bool shake;
uniform float offset;
uniform float amplitude;
uniform float frequency;
uniform bool blink;
uniform vec3 blinkColor;
uniform float blinkMix;

void main() {
    vec2 tmpCoord = texCoord;
    if( sinShake ) {
        tmpCoord.y += sin( ( texCoord.y + offset ) * 3.1415 * 2 * frequency ) * amplitude;
    }
    if( shake ) {
        tmpCoord.y += offset;
    }
    if( blink ) {
        pixelColor = mix( texture( screenTexture, tmpCoord ), blinkColor, blinkMix );
    } else {
        pixelColor = texture( screenTexture, tmpCoord );
    }
}

