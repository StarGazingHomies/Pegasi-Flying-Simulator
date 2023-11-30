#version 460 core
out vec4 fragColor;

in vec3 pos;
in vec3 normal;
in vec3 color;
//in vec2 texCoord;
in float height;

uniform sampler2D grass;

// Pseudorandom function from https://stackoverflow.com/a/17479300
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}

uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }

float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}

float random( vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }


void main() {
    if (height == 0.0)
		//fragColor = vec4(texture(grass, pos.xz).r, height, 0.0, 1.0);
        fragColor = vec4(normal/2+0.5, 1.0);
    else if (height < texture(grass, pos.xz).r)
        fragColor = vec4(0.0, height / texture(grass, pos.xz).r, 0.0, 1.0);
    else
        discard;
}