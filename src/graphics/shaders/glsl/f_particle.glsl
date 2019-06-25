#version 450

in vec2 oPos;
in vec2 texCoord;

out vec4 pixelColor;

uniform float alpha;
uniform float camNear;
uniform float camFar;
uniform sampler2D sprite;
uniform float scale;
uniform sampler2D depthMapTexture;
uniform float width;
uniform float height;

float normalizeDistance( in float z ) {
    return ( 2* camNear ) / ( camFar + camNear - z * ( camFar - camNear ) );
}

float depthDistance( in vec2 uv ) {
    float z = texture( depthMapTexture, uv ).x;
    return normalizeDistance( z );
}

void main() {
    vec2 pos = vec2( gl_FragCoord.x / width, gl_FragCoord.y / height );
    float currentDistance = depthDistance( pos );
    float spriteDistance = normalizeDistance( gl_FragCoord.z );
    float normScale = scale / ( camFar - camNear );
    float collisionAlpha = clamp( ( currentDistance - spriteDistance + ( normScale ) ) / normScale, 0, 1 );
    vec4 tmp = texture( sprite, texCoord );
    pixelColor = vec4( tmp.xyz, tmp.w * collisionAlpha * alpha );
}
