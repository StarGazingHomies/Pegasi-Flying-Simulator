#version 460 core
in vec2 TexCoords;
in vec3 Color;
out vec4 fragColor;

uniform sampler2D textTex;
// uniform vec3 textColor;

void main()
{
    // Get rid of empty pixels. Only needed if text rendering is not the last thing done.
    // if (texture(textTex, TexCoords).r == 0.0f) discard;

    fragColor = vec4(Color, texture(textTex, TexCoords).r);
    //fragColor = vec4(0.8, 0.5, 0.2, 1.0);
}