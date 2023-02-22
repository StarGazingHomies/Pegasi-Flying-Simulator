#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
// Instanced - when making VAO, do glVertexAttribDivisor(2, 1)
layout (location = 2) in vec3 translation;

out DATA
{
    vec2 texCoord;
} data_out;

// 3D transformation uniforms
uniform mat4 view;
uniform mat4 projection;

void main()
{
    data_out.texCoord = aTex;

    gl_Position = vec4(aPos + translation, 1.0f);
}
