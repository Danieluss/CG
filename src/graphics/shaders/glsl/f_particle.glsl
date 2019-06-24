#version 450

in vec2 texCoord;

out vec4 pixelColor;

uniform float alpha;
uniform float camNear;
uniform float camFar;
uniform sampler2D sprite;
uniform float scale;
uniform sampler2D depthMapTexture;

float depthDistance(in vec2 uv) {
    float zB = texture2D( depthMapTexture, uv ).x;
    float zN = 2.0 * zB - 1.0;
    return 2.0 * camNear * camFar / ( camFar + camNear - zN * ( camFar - camNear ) );
}

void main() {
    float collisionAlpha = clamp( ( depthDistance( texCoord ) - ( distance( gl_FragCoord, vec4( 0, 0, 0, 0 ) ) - scale / 2 ) ) / scale, 0, 1 );
    vec4 tmp = texture( sprite, texCoord );
    pixelColor = vec4( tmp.xyz, tmp.w * alpha * collisionAlpha );
}
