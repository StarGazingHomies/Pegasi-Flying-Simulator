#version 410 core

uniform vec3 color = vec3(0.94f, 0.91f, 1.0f);

out vec4 FragColor;

void main()
{
    // If normal needs to be used, it will.
    // Are sin() and cos() slow operations?
    
	FragColor = vec4(color, 1.0f);
}
