#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out DATA
{
    vec2 texCoord;
} data_out;

void main()
{
    data_out.texCoord = aTex;

    gl_Position = vec4(aPos, 1.0f);
}
