#version 460 core

in vec3 crntPos;
in vec2 texCoord;
in vec3 debug;

out vec4 FragColor;

void main()
{
    FragColor = vec4(debug, 1.0f);
}