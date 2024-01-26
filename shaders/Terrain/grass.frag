#version 460 core
out vec4 fragColor;

in vec3 pos;
in vec3 normal;
//in vec2 texCoord;
in float height;
flat in int instance;

uniform sampler2D grass;
uniform sampler2D grassOffsets;
uniform float chunkSize = 8.0;
uniform float grassThickness = 0.7;

void main() {
    vec3 chunkPos = fract(pos / chunkSize);

    vec4 curGrassBlade = texture(grass, chunkPos.xz);
    float grassHeight = curGrassBlade.a;
    vec3 curGrassColor = curGrassBlade.rgb;
    ivec2 texSize = textureSize(grass, 0);

    // Get position relative to pixel
    vec2 pixelPos = fract(chunkPos.xz * texSize);
    float relHeight = height / grassHeight; // Height, 0.0 to 1.0

    // Calculate offsets
    vec2 baseOffset = texture(grassOffsets, chunkPos.xz).rg;
    vec2 topOffset = texture(grassOffsets, chunkPos.xz).ba;
    vec2 offset = mix(baseOffset, topOffset, relHeight);
    pixelPos += offset;

    //fragColor = vec4(height, 0.0, 0.0, 1.0);
    if (height == 0.0)
		//fragColor = vec4(texture(grass, pos.xz).r, height / grassHeight, 0.0, 1.0);
        //fragColor = vec4(normal/2+0.5, 1.0);
        //fragColor = vec4(offset, 0.0f, 1.0f);
        //fragColor = vec4(1.0f) * instance / 16;
        fragColor = texture(grass, pos.xz);
        //discard;
    else if (relHeight <= 1 && length(2*pixelPos-1) < grassThickness * sqrt(1 - relHeight))
        //fragColor = vec4(curGrassColor, 1.0) * clamp(height/grassHeight, 0, 1);
        fragColor = vec4(curGrassColor * relHeight, 1.0);
        //discard;
    else
        discard;
}