#version 410 core
layout (location = 0) in vec3 aPos;
// Instanced - when making VAO, do glVertexAttribDivisor(1, 1) and (2, 1)
layout (location = 1) in vec3 translation;
layout (location = 2) in float radius;

void main()
{
    gl_Position = vec4(aPos * radius + translation, 1.0f);
}
