#version 460 core
in vec2 TexCoords;
in vec3 Color;
out vec4 fragColor;

uniform sampler2D textTex;
// uniform vec3 textColor;

void main()
{
    fragColor = vec4(Color, texture(textTex, TexCoords).r);
    //fragColor = vec4(0.8, 0.5, 0.2, 1.0);
}