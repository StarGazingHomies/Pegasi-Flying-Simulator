#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 acolour;

out vec3 colour;

// 3D transformation uniforms
uniform mat4 camMatrix;

void main()
{
    colour = acolour;
    gl_Position = camMatrix * vec4(aPos, 1.0f);
}