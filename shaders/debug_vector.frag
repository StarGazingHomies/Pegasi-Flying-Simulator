#version 460 core
out vec4 FragColor;

in vec3 colour;

void main()
{
	// For now, this is good
	FragColor = vec4(colour, 1.0f);
}