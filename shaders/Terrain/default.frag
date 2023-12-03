#version 460 core
out vec4 fragColor;

in vec3 pos;
in vec3 normal;
in vec3 color;
//in vec2 texCoord;
in float height;

uniform sampler2D grass;
uniform float chunkSize = 8.0;
uniform float thickness = 1;

void main() {
    vec4 curGrassBlade = texture(grass, pos.xz / chunkSize);
    float grassHeight = curGrassBlade.a;
    vec3 curGrassColor = curGrassBlade.rgb;
    ivec2 texSize = textureSize(grass, 0);

    // Get position relative to pixel
    vec2 pixelPos = fract(pos.xz / chunkSize * texSize);

    if (height == 0.0)
		//fragColor = vec4(texture(grass, pos.xz).r, height, 0.0, 1.0);
        fragColor = vec4(normal/2+0.5, 1.0);
    else if (height < grassHeight && length(2*pixelPos-1) < 1 * sqrt(1 - height / grassHeight))
        fragColor = vec4(curGrassColor, 1.0) * clamp(height/grassHeight, 0, 1);
    else
        discard;
}