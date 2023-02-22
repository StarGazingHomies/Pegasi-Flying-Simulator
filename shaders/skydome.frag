#version 410 core

in vec2 texCoord;

uniform vec3 color = vec3(0.1f, 0.0f, 0.1f);

out vec4 FragColor;

void main()
{
    // If normal needs to be used, it will.
    // Are sin() and cos() slow operations?
    
	FragColor = vec4(color, 1.0f);
}
