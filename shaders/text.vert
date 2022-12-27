#version 460 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aColor;
out vec2 TexCoords;
out vec3 Color;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 0.0f, 1.0);
    TexCoords = aTex;
    Color = aColor;
}