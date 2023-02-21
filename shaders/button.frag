#version 460 core
in vec2 TexCoords;
out vec4 fragColor;

uniform sampler2D diffuse;

void main()
{
    //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(diffuse, TexCoords).r);
    //fragColor = vec4(0.8, 0.5, 0.2, 1.0);
    fragColor = vec4(texture(diffuse, TexCoords));
}