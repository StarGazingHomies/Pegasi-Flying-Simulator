#version 410 core

in vec3 dir;
in vec2 texCoord;

uniform vec3 color = vec3(0.1f, 0.0f, 0.1f);

out vec4 FragColor;

// Testing stuff, for now
uniform vec3 sunPos = vec3(0.707f, 0.707f, 0.0f);
uniform float sunRad = 0.98f;
uniform vec3 sunColor = vec3(0.92f, 0.87f, 0.7f);

uniform vec3 moonPos = vec3(-0.707f, -0.707f, 0.0f);
uniform float moonRad = 0.99f;
uniform vec3 moonColor = vec3(0.92f, 0.87f, 0.98f);

void main()
{
    // If normal needs to be used, it will.
    // Are sin() and cos() slow operations?
    
	FragColor = vec4(color, 1.0f);

    if (dot(dir, sunPos) > sunRad) {
        FragColor = vec4(sunColor, 1.0f);
    }

    if (dot(dir, moonPos) > moonRad) {
        FragColor = vec4(moonColor, 1.0f);
    }
}
