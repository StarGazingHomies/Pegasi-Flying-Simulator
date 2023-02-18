// Heightmap vertex shader
#version 460 core

// vertex position
layout (location = 0) in vec3 aPos;
// texture coordinate
layout (location = 1) in vec2 aTex;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    // pass texture coordinate though
    TexCoord = aTex;
}